<?php

class LembreteTarefa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "profissionalsaude.tb_lembrete_tarefa";

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
        'profissional_id' => null,
        'estabelecimento_id' => null,
        'lembrete_tarefa_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null
    );

    public function salvarLembreteTarefa($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $dados = array(
            'id' => $params['id'],
            'descricao' => $params['descricao'],
            'profissional_id' => $params['profissional_id'],
            'estabelecimento_id' => $params['estabelecimento_id'],
            'lembrete_tarefa_id' => $params['lembrete_tarefa_id'],
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function listarPorProfissionalEstabelecimento($profissionalId, $estabelecimentoId, $jsonEncode = false)
    {
        $sql = "SELECT
                    lt.descricao AS lembrete_tarefa,
                    lt.criado_em AS data_inclusao,
                    TO_CHAR(lt.criado_em, 'DD/MM/YYYY HH24:MI') as data_inclusao_formatada,
                    usu.usunome AS usuario_inclusao,
                    (SELECT COUNT(*) FROM profissionalsaude.tb_lembrete_tarefa lts WHERE lts.lembrete_tarefa_id = lt.id AND lts.excluido IS FALSE) AS qtd_msg,
                    lt.estabelecimento_id,
                    lt.profissional_id,
                    lt.id
                FROM
                    profissionalsaude.tb_lembrete_tarefa lt
                INNER JOIN
                    seguranca.usuario usu ON lt.criado_por = usu.id
                WHERE
                    lt.profissional_id = {$profissionalId}
                    AND
                    lt.estabelecimento_id = {$estabelecimentoId}
                    AND
                    lt.excluido IS FALSE
                    AND
                    lt.lembrete_tarefa_id IS NULL
                ORDER BY
                    lt.criado_em DESC";

        $rs = $this->carregar($sql);
        $rs = $rs ? array_map(function($row){
            $row['p'] = setParam(array(
                'id' => $row['estabelecimento_id'],
                'profissional_id' => $row['profissional_id'],
                'lembrete_tarefa_id' => $row['id'],
            ), false);

            return $row;
        }, $rs) : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    public function listarPorLembreteTarefa($lembreteTarefaId)
    {
        $sql = "SELECT
                    lt.descricao AS lembrete_tarefa,
                    lt.criado_em AS data_inclusao,
                    TO_CHAR(lt.criado_em, 'DD/MM/YYYY HH24:MI') as data_inclusao_formatada,
                    usu.usunome AS usuario_inclusao,
                    usu.usucpf AS usucpf_inclusao,
                    lt.estabelecimento_id,
                    lt.profissional_id,
                    lt.id
                FROM
                    profissionalsaude.tb_lembrete_tarefa lt
                INNER JOIN
                    seguranca.usuario usu ON lt.criado_por = usu.id
                WHERE
                    (lt.id = {$lembreteTarefaId}
                    OR
                    lt.lembrete_tarefa_id = {$lembreteTarefaId})
                    AND
                    lt.excluido IS FALSE
                ORDER BY
                    lt.criado_em ASC";

        $rs = $this->carregar($sql);
        return $rs ? $rs : array();
    }
}