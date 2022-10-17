<?php

class ObservacaoPessoaFisica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_observacao_pessoafisica";

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
        'descricao' => null,
        'pessoafisica_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function salvarObservacao($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $dados = array(
            'id' => $params['id'],
            'descricao' => $params['descricao'],
            'pessoafisica_id' => $params['pessoafisica_id'],
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function listarPorPessoa($pessoaFisicaId, $jsonEncode = false)
    {
        $sql = "SELECT
                    TO_CHAR(oba.criado_em, 'DD/MM/YYYY HH:mi') AS data_inclusao,
                    oba.descricao AS observacao,
                    usu.usunome AS usuario_inclusao,
                    oba.id
                FROM
                    corporativo.tb_observacao_pessoafisica oba
                INNER JOIN
                    seguranca.usuario usu ON oba.criado_por = usu.id
                WHERE
                    oba.pessoafisica_id = {$pessoaFisicaId}
                    AND
                    oba.excluido IS FALSE
                ORDER BY
                    oba.criado_em DESC";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }
}