<?php

class EditalBatimentoHabilitacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.editalcliente_batimento_hablitacao";

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
        'editalcliente_id' => null,
        'anexoestabelecimento_id' => null,
        'tipo_batimento' => null,
        'data' => null,
        'item_edital' => null,
    );

    public function getListDocumentos($arWhere, $paginacao = true, $order = null)
    {
        $arWhere[] = "ane.excluido IS FALSE";
        $arWhere[] = "est.excluido IS FALSE";
        $sql = "SELECT
                  ane.nome,
                  ane.detalhe AS detalhe,
                  ane.id AS anexo_id
                FROM
                  estabelecimentosaude.tb_anexo_estabelecimento ane
                INNER JOIN
                  public.arquivo arq ON ane.arquivo_id = arq.arqid
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON ane.estabelecimento_id = est.id
                WHERE
                  " . implode(' AND ', $arWhere);
        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY ane.criado_em DESC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 20);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }
    public function getListCliente($arWhere, $paginacao = true, $order = null)
    {
        $where =  implode(' AND ', $arWhere);

        $sql = "
        SELECT
            bat.item_edital,
            array_to_string(array_agg(ane.nome), ', ') as atestados
        FROM
            estabelecimentosaude.tb_anexo_estabelecimento ane
        INNER JOIN edital.editalcliente_batimento_hablitacao bat
            on bat.anexoestabelecimento_id = ane.id
        INNER JOIN
            public.arquivo arq ON ane.arquivo_id = arq.arqid
        INNER JOIN
            estabelecimentosaude.tb_estabelecimento est ON ane.estabelecimento_id = est.id
            WHERE
        {$where}       
        group by 1 ";

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY bat.item_edital";
            }
        }
        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }
    public function salvarBatimentoHabilitacao($params)
    {
        if (is_array($params['atestados_habilitacao'])) {
            foreach($params['atestados_habilitacao'] as $dado){
                $this->setDadosNull();
                $salvar['anexoestabelecimento_id'] = $dado;
                $salvar['editalcliente_id'] = $params['editalcliente_id'];
                $salvar['item_edital'] = $params['item_edital_habilitacao_tecnica'];
                $this->popularDadosObjeto($salvar)->salvar();
                $this->commit();
            }
        }
        return true;
    }
}
