<?php

class EditalClienteMonitoramento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.editalcliente_monitoramento";

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
        'data' => null,
        'hora' => null,
        'comentario' => null,                
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = " TRUE ";

        $sql = "SELECT
                    TO_CHAR(data, 'DD/MM/YYYY') || ' ' || hora AS data_hora,
                    comentario
                FROM
                    edital.editalcliente_monitoramento 
                 
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY data";
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
    public function salvarMonitoramento($params)
    {
        $dados = $params;
        // Tratamento dos dados
        $dados['data'] = formata_data_sql($params['data']);       
        $id = $this->popularDadosObjeto($dados)->salvar();
        return $id;
    }

}
