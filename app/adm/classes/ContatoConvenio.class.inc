<?php

class ContatoConvenio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_contato_convenio";

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
        'convenio_id' => null,
        'tipo_contato_id' => null,
        'contato' => null,
        'flg_principal' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function excluirPorConvenio($convenioId)
    {
        $sql = "UPDATE {$this->stNomeTabela} SET excluido = TRUE WHERE convenio_id = {$convenioId}";
        $this->executar($sql);
    }

    public function salvarContatos($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        if(!isset($params['tipo_contato_id']) || empty($params['tipo_contato_id'])){
            $mTipo = new TipoContato();
            $params['tipo_contato_id'] = $mTipo->getIdPorSigla($params['tipo_contato_sigla']);
            $params['flg_principal'] ? $params['flg_principal'] : 'f';

            $params['id'] = $this->recuperarUm('id', array(
                "convenio_id = {$params['convenio_id']}",
                "tipo_contato_id = {$params['tipo_contato_id']}",
                "flg_principal = '{$params['flg_principal']}'",
            ));
        }

        $dados = array(
            'id' => $params['id'],
            'convenio_id' => $params['convenio_id'],
            'tipo_contato_id' => $params['tipo_contato_id'],
            'contato' => $params['contato'] ? $params['contato'] : '',
            'flg_principal' => $params['flg_principal'],
            'excluido' => 'f',
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        $retorno = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
        return $retorno;
    }

    public function listarPorConvenio($convenioId, $jsonEncode = false)
    {
        $sql = "SELECT
                    contato.*,
                    tc.descricao AS tipo_contato,
                    tc.sigla AS sigla_tipo_contato
                FROM
                    estabelecimentosaude.tb_contato_convenio contato
                JOIN
                    corporativo.tb_tipo_contato tc ON tc.id = contato.tipo_contato_id
                WHERE
                    contato.convenio_id = {$convenioId}
                    AND
                    contato.excluido IS FALSE";

        $contatos = $this->carregar($sql);
        $contatos = $contatos ? $contatos : array();
        return $jsonEncode ? json_encode(utf8ArrayEncode($contatos)) : $contatos;
    }

    public function listarEspecificosPorConvenio($convenioId)
    {
        $contatos = $this->listarPorConvenio($convenioId);
        $contatosRetorno = array();

        foreach ($contatos as $contato){
            switch ($contato['sigla_tipo_contato']){
                case TipoContato::TIPO_EMAIL:
                    $contatosRetorno['contato_email'] = $contato['contato'];
                    break;
                case TipoContato::TIPO_TELEFONE_FIXO:
                    if($contato['flg_principal'] === 't'){
                        $contatosRetorno['contato_principal'] = $contato['contato'];
                    }else{
                        $contatosRetorno['contato_telefone'] = $contato['contato'];
                    }
                    break;
            }
        }

        return $contatosRetorno;
    }

    public function copiarDados($origem,$destino)
    {
        $sql = "SELECT
                 cc.*
               FROM
                 estabelecimentosaude.tb_contato_convenio cc
               WHERE
                 cc.excluido IS FALSE AND
                 cc.convenio_id = '{$origem}';";

        $dados = $this->carregar($sql);
        $arReferenciaChaves = array();
        if (is_array($dados) && count($dados) > 0) {
            foreach ($dados as $index => $item) {
                $arReferenciaChaves[$item['id']] = $this->manter(array(
                    'convenio_id' => $destino,
                    'tipo_contato_id' => $item['tipo_contato_id'],
                    'contato' => $item['contato'],
                    'flg_principal' => $item['flg_principal'],
                    'criado_em' => 'now()',
                    'criado_por' => $_SESSION['usuario_id'],
                    'excluido' => 'false'
                ));
                $this->setDadosNull();
            }
        }

        return $arReferenciaChaves;
    }
}