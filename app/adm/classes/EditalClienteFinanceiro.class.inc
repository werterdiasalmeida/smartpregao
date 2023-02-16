<?php

class EditalClienteFinanceiro extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.editalcliente_financeiro";

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
        'arqid' => null,
        'titulo' => null,
        'data_vencimento' => null,
        'data_competencia' => null,
        'tipo' => null,
        'descricao' => null,
        'origem_destino' => null,
        'nome_origem_destino' => null,
        'valor' => null,
        'tipo_documento' => null,
        'situacao' => null

    );

    public function salvarFinanceiro($params, $arquivos = null)
    {
        if (
            isset($arquivos['arquivo'])
            && isset($arquivos['arquivo']['name']) && !empty($arquivos['arquivo']['name'])
            && isset($arquivos['arquivo']['type']) && !empty($arquivos['arquivo']['type'])
            && isset($arquivos['arquivo']['size']) && !empty($arquivos['arquivo']['size'])
        ) {
            $file = new FilesSimec();
            $arquivoId = $file->setUploadArquivoEspecifico("Anexo", 'arquivo');
        }
        // Tratamento dos dados
        $dados = array(
            'id' => $params['editalcliente_financeiro_id'],
            'editalcliente_id' => $params['editalcliente_id'],
            'arqid' => $arquivoId,
            'titulo' => $params['titulo'],
            'data_vencimento' => formata_data_sql($params['data_vencimento']),
            'data_competencia' => formata_data_sql($params['data_competencia']),
            'tipo' => $params['tipo'],
            'descricao' => $params['descricao'],
            'origem_destino' => $params['origem_destino'],
            'nome_origem_destino' => $params['nome_origem_destino'],
            'valor' => formata_valor_sql($params['valor']),
            'tipo_documento' => $params['tipo_documento'],
            'situacao' => $params['situacao'],
        );
        $id = $this->popularDadosObjeto($dados)->salvar();
        return $id;
    }
    public function getList($arWhere, $paginacao = true, $order = null)
    {
        $sql = "
                SELECT
                    ecf.tipo,
                    TO_CHAR(ecf.data_vencimento, 'DD/MM/YYYY') AS data,
                    ecf.titulo,
                    ecf.valor, 
                    ecf.situacao,
                    arq.arqnome || '.' || arq.arqextensao || ' (' ||(arq.arqtamanho / 1024) || ' KB' || ')' AS arquivo,
                    arq.arqid,
                    ecf.id
                FROM
                edital.editalcliente_financeiro ecf
                LEFT JOIN
                    public.arquivo arq ON ecf.arqid = arq.arqid
                WHERE
                {$arWhere}";
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
    public function getDadosFinanceiroGlobal($editalcliente_id)
    {

        $saida = array();
        $sql = "SELECT SUM(valor) 
                FROM edital.editalcliente_financeiro 
                WHERE editalcliente_id = {$editalcliente_id} 
                AND situacao = 'Pago/Recebido'
                AND tipo IN ('Entrada', 'A Receber') ";
        $saida['total_recebimentos'] = $this->pegaUm($sql);
        $sql = "SELECT SUM(valor) 
                FROM edital.editalcliente_financeiro 
                WHERE editalcliente_id = {$editalcliente_id} 
                AND situacao = 'Pago/Recebido'
                AND tipo IN ('Saída', 'A Pagar') ";
        $saida['total_pagamentos'] = $this->pegaUm($sql);
        $sql = "SELECT '0.00'";
        $saida['saldo_total'] = $saida['total_recebimentos'] -  $saida['total_pagamentos'];
       
   

        return $saida;

    }
    
}
