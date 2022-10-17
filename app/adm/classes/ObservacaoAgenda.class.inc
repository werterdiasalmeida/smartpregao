<?php

class ObservacaoAgenda extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_observacao_agenda";

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
        'estabelecimento_profissional_agenda_id' => null,
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
            'estabelecimento_profissional_agenda_id' => $params['estabelecimento_profissional_agenda_id'],
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function listarPorAgenda($agendaId, $jsonEncode = false)
    {
        $sql = "SELECT
                    oba.descricao AS observacao,
                    usu.usunome AS usuario_inclusao,
                    TO_CHAR(oba.criado_em, 'DD/MM/YYYY HH:mi') AS data_inclusao,
                    oba.id
                FROM
                    estabelecimentosaude.tb_observacao_agenda oba
                INNER JOIN
                    seguranca.usuario usu ON oba.criado_por = usu.id
                WHERE
                    oba.estabelecimento_profissional_agenda_id = {$agendaId}
                    AND
                    oba.excluido IS FALSE
                ORDER BY
                    oba.criado_em DESC";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }
}