<?php

class EditalClienteContrato extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.editalcliente_contrato";

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
        'numero_contrato' => null,
        'vencimento_contrato' => null,
        'processo_adminitrativo' => null,
        'responsavel_interno' => null,
        'valor_total' => null,
        'inicio_contrato' => null
        
    );
    
    public function salvarContrato($params, $arquivos = null)
    {
        //ver($params,d);
        // Tratamento dos dados
        $dados = array(
            'id' => $params['editalcliente_contrato_id'],
            'numero_contrato' => $params['numero_contrato'],
            'tipo_acompanhamento' => $params['tipo_acompanhamento'],
            'vencimento_contrato' => formata_data_sql($params['vencimento_contrato']),
            'inicio_contrato' => formata_data_sql($params['inicio_contrato']),
            'processo_adminitrativo' => $params['processo_adminitrativo'],
            'responsavel_interno' => $params['responsavel_interno'],
            'editalcliente_id' => $params['editalcliente_id'],
            'valor_total' => formata_valor_sql($params['valor_total']),
        );
        $id = $this->popularDadosObjeto($dados)->salvar();
        return $id;
    }
}
