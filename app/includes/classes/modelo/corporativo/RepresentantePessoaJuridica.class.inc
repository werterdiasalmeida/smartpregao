<?php

class RepresentantePessoaJuridica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_representante_pessoajuridica";

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
        'pessoajuridica_id' => null,
        'nome' => null,
        'cargo' => null,
        'contato' => null,
        'flg_principal' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function excluirPorPessoaJuridica($pessoaJuridicaId)
    {
        $sql = "UPDATE {$this->stNomeTabela} SET excluido = TRUE WHERE pessoajuridica_id = {$pessoaJuridicaId}";
        $this->executar($sql);
    }

    public function salvarRepresentante($params)
    {
        $camposNulos = array();
        foreach ($params as $campo => $valor){
            if($valor === ''){
                $params[$campo] = null;
                $camposNulos[] = $campo;
            }
        }

        $dados = array(
            'id' => $params['id'],
            'pessoajuridica_id' => $params['pessoajuridica_id'],
            'nome' => $params['nome'],
            'cargo' => $params['cargo'],
            'contato' => $params['contato'],
            'flg_principal' => $params['flg_principal'],
            'excluido' => 'f',
        );

        if(empty($params['id'])){
            $dadosCarimbo = array(
                'criado_em' => 'now()',
                'criado_por' => $_SESSION['usuario_id'],
            );
        }else{
            $dadosCarimbo = array(
                'atualizado_em' => 'now()',
                'atualizado_por' => $_SESSION['usuario_id'],
            );
        }

        $dados = array_merge($dados, $dadosCarimbo);
        $retorno = $this->popularDadosObjeto($dados)->salvar();
        return $retorno;
    }

    public function listarPorPessoaJuridica($pessoaJuridicaId, $jsonEncode = false)
    {
        $sql = "SELECT
                    rep.*
                FROM
                    corporativo.tb_representante_pessoajuridica rep
                WHERE
                    rep.pessoajuridica_id = {$pessoaJuridicaId}
                    AND
                    rep.excluido IS FALSE";

        $contatos = $this->carregar($sql);
        return $jsonEncode ? json_encode(utf8ArrayEncode($contatos)) : $contatos;
    }
}