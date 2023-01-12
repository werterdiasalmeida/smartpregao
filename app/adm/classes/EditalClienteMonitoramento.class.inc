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

    public function getList($arWhere, $paginacao = true, $order = null)
    {
        
        $sql = "SET TIMEZONE TO 'America/Sao_Paulo';";
        $this->executar($sql);
        $sql = "
                SELECT
                    to_char(TO_TIMESTAMP(data || ' ' || hora || ':00', 'YYYY-MM-DD HH24:MI:SS')  - CURRENT_TIMESTAMP, 'DD HH24:MI') as falta,
                    TO_CHAR(data, 'DD/MM/YYYY') || ' ' || hora AS data_hora,
                    comentario, id
                FROM
                    edital.editalcliente_monitoramento 
                 
                WHERE
                {$arWhere}            
                " ;
        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY id DESC";
            }
        }
        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 5);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }
    public function getListCliente($arWhere, $paginacao = true, $order = null)
    {

        $sql = "
                SELECT
                    edi.orgao || ' [' || edital || '] <br/>' ||edi.valor_estimado as edital,
                    TO_CHAR(data, 'DD/MM/YYYY') || ' ' || hora AS data_hora,
                    comentario
                FROM
                    edital.editalcliente_monitoramento mon 
					JOIN edital.editalcliente edc on mon.editalcliente_id = edc.id 
					JOIN   edital.edital edi on edc.edital_id = edi.id
                WHERE
                {$arWhere}       
                " ;

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY data DESC";
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
