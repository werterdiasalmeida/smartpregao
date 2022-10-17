<?php

class Lote extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_lote";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("id");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id' => null,
        'numero' => null,
        'descricao' => null,
        'valor' => null,
        'estabelecimento_id' => null,
        'convenio_estabelecimento_id' => null,
        'documento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'tipo_guia_id' => null,
        'sequencial' => null,
    );

    const WF_TIPO_DOCUMENTO = 7202;

    const WF_ESTADO_GERADO = 720201;
    const WF_ESTADO_CORRECAO = 720202;

    public function getList ($where = [], $paginacao = false, $order = null)
    {
        $where[] = "lot.excluido IS FALSE";
        $sql = "
            SELECT
                lot.numero,
                lot.descricao AS lote_descricao,
                con.nome AS convenio, 
                tgu.descricao AS tipo_guia_descricao,
                lot.valor,
                esd.esddsc AS estado,
                con.cnpj,
                lot.id AS lote_id,
                lot.estabelecimento_id,
                con.cnpj,
                esd.esdid
            FROM
                estabelecimentosaude.tb_lote lot
            INNER JOIN
                estabelecimentosaude.tb_convenio_estabelecimento con ON lot.convenio_estabelecimento_id = con.id
            LEFT JOIN
                workflow.documento doc ON lot.documento_id = doc.docid
            LEFT JOIN 
                workflow.estadodocumento esd ON doc.esdid = esd.esdid
            LEFT JOIN
                estabelecimentosaude.tb_tipo_guia tgu ON lot.tipo_guia_id = tgu.id
            WHERE
                " . implode(' AND ', $where);

        if (!is_null($order)) {
            $sql .= " ORDER BY {$order}";
        } else {
            $sql .= " ORDER BY numero DESC";
        }

        if ($paginacao) {
            return Paginacao::getPaginacao($this, $sql, 15);
        }

        return $this->carregarArray($sql);
    }

    public function getTodosDados($id, $carregarGuias = true)
    {
        $sql = "SELECT
                    lot.*,
                    pj.cnpj AS estabelecimento_cnpj,
                    con.cnpj AS convenio_cnpj
                FROM
                    estabelecimentosaude.tb_lote lot
                LEFT JOIN
                    estabelecimentosaude.tb_estabelecimento est ON lot.estabelecimento_id = est.id
                LEFT JOIN
                    corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                LEFT JOIN 
                    estabelecimentosaude.tb_convenio_estabelecimento con ON lot.convenio_estabelecimento_id = con.id
                WHERE
                    lot.id = {$id}";

        $dados = $this->pegaLinha($sql);

        if($carregarGuias) {
            $mLoteGuia = new LoteGuia();
            $dados['guias_selecionadas'] = $mLoteGuia->getIdGuiasByLote($id);
        }

        return $dados;
    }

    public function salvarLote(array $params)
    {
        $guiasSelecionadas = is_array($params['guias_selecionadas']) ? $params['guias_selecionadas']
            : json_decode(str_replace('\"', '"', $params['guias_selecionadas']));

        $dados = [
            'id' => $params['id'],
            'numero' => $this->gerarNumeroLote($params['estabelecimento_id'], $params['id']),
            'descricao' => $params['descricao'],
            'valor' => $this->calcularTotalGuiasPorGuias($guiasSelecionadas),
            'estabelecimento_id' => $params['estabelecimento_id'],
            'convenio_estabelecimento_id' => $params['convenio_estabelecimento_id'],
            'tipo_guia_id' => $params['tipo_guia_id'],
            'documento_id' => $this->gerarDocumento($params['id']),
        ];

        $estadoAtual = wf_pegarEstadoAtual($dados['documento_id']);
        if(!empty($params['id']) && $estadoAtual['esdid'] != self::WF_ESTADO_CORRECAO) {
            throw new Exception('O lote só pode ser alterado no estado "Em correção".');
        }

        $loteId = $this->manter($dados);
        $mLoteGuia = new LoteGuia();
        $mLoteGuia->excluirPorLote($loteId);

        foreach ($guiasSelecionadas as $index => $guiaId) {
            $mLoteGuia->salvarLoteGuia($loteId, $guiaId);
            $mLoteGuia->setDadosNull();
        }

        return $loteId;
    }

    public function calcularTotalGuiasPorGuias(array $guiasId)
    {

        $sql = "SELECT
                    SUM(COALESCE(guia->>'total_geral', guia->>'valor_procedimento', '0')::NUMERIC)
                FROM
                    estabelecimentosaude.tb_guia
                WHERE
                    id IN (". implode(', ', $guiasId) . ");";
        return $this->pegaUm($sql);
    }

    public function gerarNumeroLote($estabelecimentoId, $loteId = null)
    {
        if(empty($loteId)) {
            $numeroGuia = 1 + $this->pegaUm(
                    "SELECT 
                              COALESCE(MAX(lot.numero::integer), (eco.inicial_lote - 1))
                          FROM 
                              estabelecimentosaude.tb_estabelecimento_configuracao eco
                          LEFT JOIN
                              estabelecimentosaude.tb_lote lot ON eco.estabelecimento_id = lot.estabelecimento_id
                          WHERE
                              eco.estabelecimento_id = {$estabelecimentoId}
                          GROUP BY
                              eco.inicial_lote");

            return str_pad($numeroGuia, 7, '0', STR_PAD_LEFT);
        }

        return $this->recuperarUm('numero', ["id = {$loteId}"]);
    }

    public function gerarDocumento($id = null)
    {
        if(empty($id)) {
            return wf_cadastrarDocumento(self::WF_TIPO_DOCUMENTO, 'Lote de Guias');
        }

        return $this->recuperarUm('documento_id', ["id = {$id}"]);
    }

    public function isPodeAlterar($loteId)
    {
        $sql = "SELECT 
                    doc.esdid 
                FROM 
                    estabelecimentosaude.tb_lote lot 
                INNER JOIN 
                    workflow.documento doc ON lot.documento_id = doc.docid
                WHERE
                    lot.id = {$loteId}";
        $estadoId = $this->pegaUm($sql);

        return in_array($estadoId, [self::WF_ESTADO_CORRECAO]);
    }

    public function incrementarSequencial($loteId)
    {
        $sequencial = intval($this->recuperarUm('sequencial', ["id = {$loteId}"]));

        return $this->popularDadosObjeto([
            'id' => $loteId,
            'sequencial' => ($sequencial + 1)
        ])->salvar();
    }
}