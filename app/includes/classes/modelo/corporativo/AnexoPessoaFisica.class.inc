<?php

class AnexoPessoaFisica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_anexo_pessoafisica";

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
        'tipo_anexo_id' => null,
        'observacao' => null,
        'pessoafisica_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );


    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "ane.excluido IS FALSE";

        $sql = "SELECT
                  arq.arqnome || '.' || arq.arqextensao AS descricao,
                  (arq.arqtamanho / 1024) || ' KB' AS tamanho,
                  tan.descricao AS tipo,
                  TO_CHAR(ane.criado_em, 'DD/MM/YYYY HH:mi:ss') AS criado_em_formatado,
                  ane.observacao,
                  arq.arqid,
                  ane.id AS anexo_id
                FROM
                  corporativo.tb_anexo_pessoafisica ane
                INNER JOIN
                  public.arquivo arq ON ane.arquivo_id = arq.arqid
                INNER JOIN
                  corporativo.tb_tipo_anexo tan ON ane.tipo_anexo_id = tan.id
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
            $dado = Paginacao::getPaginacao($this, $sql, 500);
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

        $dados = array(
            'id' => $params['id'],
            'arquivo_id' => $arquivoId,
            'tipo_anexo_id' => $params['tipo_anexo_id'],
            'observacao' => $params['observacao'],
            'excluido' => 'false',
            'pessoafisica_id' => $params['pessoafisica_id'],
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function excluirAnexo($id)
    {
        return $this->excluirLogicamente($id);
    }

    public function salvarDocumentosPessoa($params, $pessoaFisicaId)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);
        $mTipoAnexo = new TipoAnexo();

        $tipos = array(
            'arquivo_cns' => TipoAnexo::TIPO_CNS,
            'arquivo_cpf' => TipoAnexo::TIPO_CPF,
        );

        foreach($tipos as $name => $sigla){
            if($params[$name]){
                $tipoId = $mTipoAnexo->getIdBySigla($sigla);
                $dados = array(
                    'id' => $this->recuperarUm('id', array("pessoafisica_id = {$pessoaFisicaId}", "tipo_anexo_id = '{$tipoId}'", "arquivo_id = {$params[$name]}")),
                    'arquivo_id' => $params[$name],
                    'tipo_anexo_id' => $tipoId,
                    'observacao' => '',
                    'excluido' => 'false',
                    'pessoafisica_id' => $pessoaFisicaId,
                );

                $dados = $this->getDadosCarimbo($params, $dados);
                $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
                $this->setDadosNull();
            }
        }
    }

    public function getUltimoBySiglaPessoa($sigla, $pessoaId)
    {
        $sql = "SELECT
                    ap.id,
                    ap.arquivo_id,
                    ap.observacao,
                    ta.descricao
                FROM
                    corporativo.tb_anexo_pessoafisica ap
                INNER JOIN
                    corporativo.tb_tipo_anexo ta ON ap.tipo_anexo_id = ta.id
                WHERE
                    ap.excluido IS FALSE
                    AND
                    ap.pessoafisica_id = {$pessoaId}
                    AND
                    ta.sigla = '{$sigla}'
                ORDER BY
                    ap.criado_em DESC";

        $rs = $this->pegaLinha($sql);
        return $rs ? $rs : array();
    }
}