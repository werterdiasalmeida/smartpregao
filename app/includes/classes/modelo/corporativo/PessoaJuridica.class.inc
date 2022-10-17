<?php

class PessoaJuridica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_pessoajuridica";

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
        'razao_social' => null,
        'nome_fantasia' => null,
        'cnpj' => null,
        'logo_arquivo_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'classificacao' => null,
        'flg_cabecalho_rodape' => 'S'
    );

    const TIPO_PUBLICA = 'PU';
    const TIPO_PRIVADA = 'PR';

    /**
     * @param $params
     * @param array $arquivos
     * @return bool|int
     * @throws CnpjIndisponivel
     */
    public function salvarPessoaJuridica($params, $arquivos = array())
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        if($params['cnpj'] && !$this->isCnpjDisponivel($params['cnpj'], $params['id'])) {
            throw new CnpjIndisponivel();
        }
        if ($params['remove_logo'] != 'S') {
            $arquivoId  = $params['arquivo_id'];
            if (isset($arquivos['logo'])
                && isset($arquivos['logo']['name']) && !empty($arquivos['logo']['name'])
                && isset($arquivos['logo']['type']) && !empty($arquivos['logo']['type'])
                && isset($arquivos['logo']['size']) && !empty($arquivos['logo']['size'])) {
                $file = new FilesSimec();
                $arquivoId = $file->setUploadArquivoEspecifico("Logo da PJ {$params['nome_fantasia']}", 'logo');
            }
        } else {
            $this->removeLogo($params['id']);
        }
        $dados = array(
            'id' => $params['id'],
            'razao_social' => $params['razao_social'],
            'nome_fantasia' => $params['nome_fantasia'],
            'logo_arquivo_id' => $arquivoId,
            'cnpj' => $params['cnpj'],
            'classificacao' => $params['classificacao'],
            'flg_cabecalho_rodape' => $params['flg_cabecalho_rodape']
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        $id = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);

        $mEnderecoPj = new EnderecoPessoaJuridica();
        $mEnderecoPj->excluirPorPessoaJuridica($id);
        if($params['enderecos']){
            $params['enderecos'] = str_replace(['\"', "\'"], ['"',"'"], $params['enderecos']);
            $params['enderecos'] = utf8_encode($params['enderecos']);
            $params['enderecos'] = utf8ArrayDecode(json_decode($params['enderecos'], true));
            foreach ($params['enderecos'] as $endereco){
                $endereco['pessoajuridica_id'] = $id;
                $mEnderecoPj->salvarEndereco($endereco);
                $mEnderecoPj->setDadosNull();
            }
        }

        $mContatoPj = new ContatoPessoaJuridica();
        $mContatoPj->excluirPorPessoaJuridica($id);

        if($params['contatos']){
            if(!is_array($params['contatos'])){
                $params['contatos'] = str_replace('\"', '"', $params['contatos']);
                $params['contatos'] = utf8_encode($params['contatos']);
                $params['contatos'] = utf8ArrayDecode(json_decode($params['contatos'], true));
            }

            foreach ($params['contatos'] as $contato){
                if(empty($contato['contato'])){
                    continue;
                }

                $contato['pessoajuridica_id'] = $id;
                $mContatoPj->salvarContatos($contato);
                $mContatoPj->setDadosNull();
            }
        }

        $mRepresentantePj = new RepresentantePessoaJuridica();
        $mRepresentantePj->excluirPorPessoaJuridica($id);
        if($params['representantes']){
            $params['representantes'] = str_replace('\"', '"', $params['representantes']);
            $params['representantes'] = utf8_encode($params['representantes']);
            $params['representantes'] = utf8ArrayDecode(json_decode($params['representantes'], true));

            foreach ($params['representantes'] as $representante){
                $representante['pessoajuridica_id'] = $id;
                $mRepresentantePj->salvarRepresentante($representante);
                $mRepresentantePj->setDadosNull();
            }
        }

        return $id;
    }

    private function removeLogo($id) {
        if ($id != null) {
            $sql = "select logo_arquivo_id from corporativo.tb_pessoajuridica where id = " . $id;
            $arqId = $this->pegaUm($sql);
            $sql = "update corporativo.tb_pessoajuridica set logo_arquivo_id = null  where id = " .$id;
            $this->pegaUm($sql);
            $file = new FilesSimec();
            $file->setRemoveUpload( $arqId);
        }
    }

    public function isCnpjDisponivel($cnpj, $id = null)
    {
        $where = array(
            "cnpj = '{$cnpj}'",
            "excluido IS FALSE"
        );
        if($id){
            $where[] = "id != {$id}";
        }

        return intval($this->recuperarUm('count(*)', $where)) === 0;
    }

    /**
     * @param $retorno
     * @return array
     */
    public function getDadosEnderecoEContatoPrincipal($retorno)
    {
        $mContatoPessoaJuridica = new ContatoPessoaJuridica();
        $mEnderecoPessoaJuridica = new EnderecoPessoaJuridica();
        $contatoPessoaJuridica = $mContatoPessoaJuridica->listarEspecificosPorPessoaJuridica($retorno['pessoajuridica_id']);
        $enderecoPessoaJuridica = $mEnderecoPessoaJuridica->getPrincipalPorPessoaJuridica($retorno['pessoajuridica_id']);

        if (!empty($enderecoPessoaJuridica)) {
            $enderecoPessoaJuridica['endereco_pessoajuridica_id'] = $enderecoPessoaJuridica['id'];
            $enderecoPessoaJuridica['cep'] = formata_cep($enderecoPessoaJuridica['cep']);
            unset($enderecoPessoaJuridica['id']);
            $retorno = array_merge($retorno, $enderecoPessoaJuridica);
        }

        if (!empty($contatoPessoaJuridica)) {
            $retorno['contato_email'] = $contatoPessoaJuridica['contato_email'];
        }
        return $retorno;
    }
}