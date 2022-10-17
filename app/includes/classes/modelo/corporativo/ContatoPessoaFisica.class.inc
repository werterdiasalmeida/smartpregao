<?php

class ContatoPessoaFisica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_contato_pessoafisica";

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
        'pessoafisica_id' => null,
        'tipo_contato_id' => null,
        'contato' => null,
        'flg_principal' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function excluirPorPessoaFisica($pessoaFisicaId)
    {
        $sql = "UPDATE {$this->stNomeTabela} SET excluido = TRUE WHERE pessoafisica_id = {$pessoaFisicaId}";
        $this->executar($sql);
    }

    public function salvarContatos($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        if(!isset($params['tipo_contato_id']) || empty($params['tipo_contato_id'])){
            $mTipo = new TipoContato();
            $params['tipo_contato_id'] = $mTipo->getIdPorSigla($params['tipo_contato_sigla']);

            $params['id'] = $this->recuperarUm('id', array(
                "pessoafisica_id = {$params['pessoafisica_id']}",
                "tipo_contato_id = {$params['tipo_contato_id']}"
            ));
        }

        $dados = array(
            'id' => $params['id'],
            'pessoafisica_id' => $params['pessoafisica_id'],
            'tipo_contato_id' => $params['tipo_contato_id'],
            'contato' => $params['contato'] ? $params['contato'] : '',
            'flg_principal' => $params['flg_principal'] ? $params['flg_principal'] : 'f',
            'excluido' => 'f',
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        $retorno = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
        return $retorno;
    }

    public function listarPorPessoaFisica($pessoaFisicaId, $jsonEncode = false)
    {
        $sql = "SELECT
                    cpj.*,
                    tc.descricao AS tipo_contato,
                    tc.sigla AS sigla_tipo_contato
                FROM
                    corporativo.tb_contato_pessoafisica cpj
                JOIN
                    corporativo.tb_tipo_contato tc ON tc.id = cpj.tipo_contato_id
                WHERE
                    cpj.pessoafisica_id = {$pessoaFisicaId}
                    AND
                    cpj.excluido IS FALSE";

        $contatos = $this->carregar($sql);
        $contatos = $contatos ? $contatos : array();
        return $jsonEncode ? json_encode(utf8ArrayEncode($contatos)) : $contatos;
    }

    public function listarEspecificosPorPessoaFisica($pessoaFisicaId)
    {
        $contatos = $this->listarPorPessoaFisica($pessoaFisicaId);
        $contatosRetorno = array();

        foreach ($contatos as $contato){
            switch ($contato['sigla_tipo_contato']){
                case TipoContato::TIPO_EMAIL:
                    $contatosRetorno['contato_email'] = $contato['contato'];
                    break;
                case TipoContato::TIPO_CELULAR:
                    $contatosRetorno['contato_celular'] = $contato['contato'];
                    break;
                case TipoContato::TIPO_TELEFONE_FIXO:
                    $contatosRetorno['contato_telefone'] = $contato['contato'];
                    break;
            }
        }

        return $contatosRetorno;
    }
}