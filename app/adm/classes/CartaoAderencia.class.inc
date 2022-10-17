<?php

class CartaoAderencia extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "clubvidacard.tb_cartao_aderente";

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
        'matricula' => null,
        'dt_filiacao' => null,
        'vendedor' => null,
        'nome' => null,
        'dt_nascimento' => null,
        'cpf' => null,
        'rg' => null,
        'orgao_emissor' => null,
        'cep' => null,
        'uf_id' => null,
        'municipio_id' => null,
        'bairro' => null,
        'logradouro' => null,
        'residencia_propria' => null,
        'telefone' => null,
        'celular' => null,
        'email' => null,
        'dependentes' => null,
        'opcao_pagamento' => null,
        'dados_pagamento' => null,
        'docid' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'estabelecimento_id' => null,
    );


    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "ca.excluido IS FALSE";

        $sql = "SELECT
                    UPPER(pes.nome_fantasia) as nome_fantasia,
                    e.esddsc,
                    ca.dt_filiacao,
                    ca.vendedor,
                    '(' || ca.cpf || ') ' || ca.nome as nome,
                    uf.uf_sigla,
                    m.municipio,
                    ca.opcao_pagamento,
                    ca.id,
                    CASE WHEN (SELECT COUNT(*) FROM clubvidacard.tb_cartao_aderente_historico h WHERE h.id_cartao_aderente = ca.id) > 0 THEN true ELSE false END AS possui_historico
                FROM
                    clubvidacard.tb_cartao_aderente ca
                JOIN corporativo.tb_municipio m ON ca.municipio_id = m.id
                JOIN corporativo.tb_unidade_federacao uf ON m.uf_id = uf.id
                LEFT JOIN workflow.documento d ON d.docid = ca.docid
                LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid
                LEFT JOIN estabelecimentosaude.tb_estabelecimento est ON ca.estabelecimento_id = est.id
                LEFT JOIN corporativo.tb_pessoajuridica pes ON est.pessoajuridica_id = pes.id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY ca.criado_em DESC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function salvarCartaoAderencia($dados, $camposNulos = array())
    {

        if (!$dados['dt_filiacao'])
            $dados['dt_filiacao'] = date("d-m-Y");
        $dados['dt_filiacao'] = formata_data_sql($dados['dt_filiacao']);
        $dados['dt_nascimento'] = formata_data_sql($dados['dt_nascimento']);

        $dependentes = [];

        foreach ($dados['dependentes']['nome'] as $key => $nome) {

            $dependentes[] = [
                'nome' => $nome,
                'parentesco' => $dados['dependentes']['parentesco'][$key],
                'dt_nascimento' => formata_data_sql($dados['dependentes']['dt_nascimento'][$key]),
                'cpf' => $dados['dependentes']['cpf'][$key],
            ];
        }

        $dados['dependentes'] = json_encode(utf8ArrayEncode($dependentes));

        switch ($dados['opcao_pagamento']) {
            case 'EE':
                $dados['dados_pagamento'] = json_encode(utf8ArrayEncode($dados['conta_energia']));
                break;
            case 'DE':
                $dados['dados_pagamento'] = json_encode(utf8ArrayEncode($dados['conta_bancaria']));
                break;
            case 'CC':
                $dados['dados_pagamento'] = json_encode(utf8ArrayEncode($dados['cartao_credito']));
                break;
            case 'CA':
                $dados['dados_pagamento'] = json_encode(utf8ArrayEncode($dados['carne']));
                break;
        }

        if (empty($dados['docid'])) {
            $dados['docid'] = wf_cadastrarDocumento(TIPO_DOCUMENTO_FLUXO_ADERENCIA_CARTAO_BENEFICIOS, 'Fluxo aderencia cartão de benefícios');
        }

        if ($dados['id']) {
            $mCartaoAderenciaHistorico = new CartaoAderenciaHistorico();
            $mCartaoAderenciaHistorico->salvarCartaoAderenciaHistorico(['id_cartao_aderente' => $dados['id'], 'motivo' => $dados['motivo']]);
        }

        $dados = $this->getDadosCarimbo($dados, $dados);

        //ver($dados,d);

        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function excluir($id = null, $retorno = null)
    {
        $this->excluirLogicamente($id);
    }

    public function getDadosbyId($id)
    {
        $arWhere[] = "ca.excluido IS FALSE";
        $arWhere[] = "ca.id = {$id}";

        $sql = "SELECT
                  ca.*
                FROM
                  clubvidacard.tb_cartao_aderente ca
                WHERE
                  " . implode(' AND ', $arWhere) . "
                ORDER BY
                    2;";

        $retorno = $this->pegaLinha($sql);
        return $retorno ? $retorno : array();
    }

    public function getByCPF($cpf)
    {
        $cpf = formatar_cpf($cpf);
        $sql = "
                select
                       c.cpf,
                       c.nome,
                       TO_CHAR(c.dt_nascimento, 'DD/MM/YYYY') as dt_nascimento,
                       TO_CHAR(c.dt_filiacao, 'DD/MM/YYYY') as dt_filiacao,
                       e.esddsc as situacao,
                       dependentes
                from clubvidacard.tb_cartao_aderente c
                join workflow.documento d ON c.docid = d.docid
                join workflow.estadodocumento e ON d.esdid = e.esdid
                where c.cpf = '{$cpf}'
                  and c.excluido is false
                
                -- UNION ALL
                -- 
                -- select dependente.cpf,
                --        dependente.nome,
                --        TO_CHAR(dependente.dt_nascimento, 'DD/MM/YYYY') as dt_nascimento,
                --        TO_CHAR(c.dt_filiacao, 'DD/MM/YYYY') as dt_filiacao,
                --        e.esddsc as situacao
                -- from clubvidacard.tb_cartao_aderente as c
                --          join lateral jsonb_to_recordset(dependentes) as dependente(cpf text, nome text, dt_nascimento date) on true
                --          join workflow.documento d ON c.docid = d.docid
                --          join workflow.estadodocumento e ON d.esdid = e.esdid
                -- where c.excluido is false
                --   and dependente.cpf = '{$cpf}'
                ";

        return $this->pegaLinha($sql);
    }
}
