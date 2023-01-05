<?php

class AnexoEstabelecimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_anexo_estabelecimento";

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
        'arquivo_id' => null,
        'nome' => null,        
        'detalhe' => null,
        'validade' => null,        
        'tipo' => null,
        'estabelecimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "ane.excluido IS FALSE";
        $arWhere[] = "est.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();

        $sql = "SELECT
                  ane.nome,
                  arq.arqnome || '.' || arq.arqextensao || ' (' ||(arq.arqtamanho / 1024) || ' KB' || ')' AS descricao,
                  tipo,
                  TO_CHAR(ane.validade, 'DD/MM/YYYY') AS validade,
                  ane.detalhe AS detalhe,
                  arq.arqid,
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
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
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

    public function salvarAnexo($params, $arquivos)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        if (isset($arquivos['arquivo'])
            && isset($arquivos['arquivo']['name']) && !empty($arquivos['arquivo']['name'])
            && isset($arquivos['arquivo']['type']) && !empty($arquivos['arquivo']['type'])
            && isset($arquivos['arquivo']['size']) && !empty($arquivos['arquivo']['size'])) {
            $file = new FilesSimec();
            $arquivoId = $file->setUploadArquivoEspecifico("Anexo do estabelecimento", 'arquivo');
        }
        if($params['validade'])
            $params['validade'] = ajusta_data($params['validade']);
        $dados = array(
            'id' => $params['id'],
            'arquivo_id' => $arquivoId,
            'nome' => $params['nome'],
            'tipo' => $params['tipo'],
            'detalhe' => $params['detalhe'],
            'validade' => $params['validade'],
            'excluido' => 'false',
            'estabelecimento_id' => $params['estabelecimento_id'],
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function excluirAnexo($id)
    {
        return $this->excluirLogicamente($id);
    }
}