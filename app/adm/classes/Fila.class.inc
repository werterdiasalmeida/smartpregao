<?php

class Fila extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_fila";

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
        'descricao' => null,
        'municipio_id' => null,
        'especialidade_id' => null,
        'valor' => null,
        'data_limite' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "fil.excluido IS FALSE";

        $sql = "SELECT
                  fil.descricao,
                  mun.municipio || '/' || uf.uf_sigla AS municipio,
                  esp.descricao AS especialidade,
                  TO_CHAR(fil.data_limite, 'DD/MM/YYYY') AS data_limite_formatada,
                  fil.valor,
                  fil.id
                FROM
                  estabelecimentosaude.tb_fila fil
                INNER JOIN
                  corporativo.tb_municipio mun ON fil.municipio_id = mun.id
                INNER JOIN
                  profissionalsaude.tb_especialidade esp ON fil.especialidade_id = esp.id
                INNER JOIN
                  corporativo.tb_unidade_federacao uf ON mun.uf_id = uf.id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY fil.data_limite DESC";
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

    /**
     * @param $params
     * @throws Exception
     */
    public function salvarFila($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $dados = array(
            'id' => $params['id'],
            'descricao' => $params['descricao'],
            'municipio_id' => $params['municipio_id'],
            'especialidade_id' => $params['especialidade_id'],
            'valor' => desformata_valor(str_replace('R$', '', $params['valor'])),
            'data_limite' => formata_data_sql($params['data_limite']),
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        $id = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);

        $mParticipante = new FilaParticipante();
        $mParticipante->excluirParticipantePorFila($id);

        if (is_array($params['documento']) && is_array($params['tipo_documento_id'])) {
            foreach ($params['documento'] as $indice => $documento) {

                if (empty($documento) || !isset($params['tipo_documento_id'][$indice])
                    || empty($params['tipo_documento_id'][$indice])) {
                    continue;
                }

                $dados = array(
                    'id' => isset($params['participante_id'][$indice]) ? $params['participante_id'][$indice] : null,
                    'fila_id' => $id,
                    'tipo_documento_id' => $params['tipo_documento_id'][$indice],
                    'documento' => $documento,
                );

                $mParticipante->salvarParticipante($dados);
                $mParticipante->setDadosNull();
            }
        }
    }

    public function excluirFila($id)
    {
        $this->excluirLogicamente($id);
    }
}