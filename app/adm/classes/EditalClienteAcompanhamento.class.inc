<?php

class EditalClienteAcompanhamento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "edital.editalcliente_acompanhamento";

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
        'tipo_acompanhamento' => null,
        'arqid' => null,
        'data' => null,
        'comentario' => null,
    );

    public function getList($arWhere, $paginacao = true, $order = null)
    {
        $sql = "
                SELECT
                    TO_CHAR(data, 'DD/MM/YYYY') AS data,
                    arq.arqnome || '.' || arq.arqextensao || ' (' ||(arq.arqtamanho / 1024) || ' KB' || ')' AS arquivo,
                    comentario, arq.arqid, id
                FROM
                    edital.editalcliente_acompanhamento aco
                LEFT JOIN
                    public.arquivo arq ON aco.arqid = arq.arqid
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
    public function salvarAcompanhamento($params, $arquivos = null)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);
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
            'id' => $params['id'],
            'arqid' => $arquivoId,
            'tipo_acompanhamento' => $params['tipo_acompanhamento'],
            'data' => formata_data_sql($params['data']),
            'comentario' => $params['comentario'],
            'editalcliente_id' => $params['editalcliente_id'],
        );
        $id = $this->popularDadosObjeto($dados)->salvar();
        return $id;
    }
}
