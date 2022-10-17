<?php

class ValorAPagar extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_valor_a_pagar";

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
        'agenda_id' => null,
        'valor_pago_id' => null,
        'valor_a_pagar' => null,
        'data_operacao' => null,
        'responsavel_operacao' => null,
        'solicitacao_correcao_agenda_id' => null,
        'excluido' => null
    );

    public function getExtratoCompletoEstabelecimentoByWhere($estabelecimentoId,$filtros)
    {
        $arWhere = array();
        if(!empty($filtros['profissional_id'])){
            $arWhere[] = " pr.id = " . (int) $filtros['profissional_id'];
        }

        if(!empty($filtros['inicio']) && !empty($filtros['fim'])){
            $arWhere[] = " pag.data_operacao::date BETWEEN '{$filtros['inicio']}' AND '{$filtros['fim']}' ";
        }

        $arWhere[] = "pag.excluido IS FALSE";
        $arWhere[] = "est.id = '{$estabelecimentoId}'";

        $sql = "  SELECT
                    pag.id,
                    'pagar' AS tipo,
                    epa.valor_procedimento AS valor_total,
                    epa.percentual_profissional AS percentual_repasse,
                    pag.valor_a_pagar AS valor,
                    COALESCE(p.descricao,'') || ' - Paciente: ' || COALESCE(paciente.nome_completo,'') || ' - ' || COALESCE(ce.nome,'') AS descricao, 
                    pag.data_operacao,
                    pag.valor_pago_id AS referencia,
                    null AS arquivo_id,
                    pag.agenda_id,
                    pf.nome_completo AS profissional,
                    tc.remunerada,
                    pag.solicitacao_correcao_agenda_id,
                    sca.descricao_solicitacao AS descricao_solicitacao_correcao
                  FROM
                    {$this->stNomeTabela} pag
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_profissional_agenda epa ON epa.id = pag.agenda_id AND epa.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id -- AND tc.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id -- AND eep.excluido IS FALSE
                  INNER JOIN
                      profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id -- AND ep.excluido IS FALSE
                  INNER JOIN
                      profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id -- AND pr.excluido IS FALSE
                  INNER JOIN
                      corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id -- AND pf.excluido IS FALSE                      
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_convenio_estabelecimento ce ON ce.id = epa.convenio_id -- AND ce.excluido IS FALSE
                  INNER JOIN
                      corporativo.tb_pessoafisica paciente ON paciente.id = epa.pessoa_paciente_id -- AND paciente.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id -- AND vp.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id -- AND p.excluido IS FALSE
                  LEFT JOIN
                      estabelecimentosaude.tb_solicitacao_correcao_agenda sca ON pag.solicitacao_correcao_agenda_id = sca.id
                  WHERE
                    " . implode(' AND ', $arWhere) . "
                UNION ALL
                  SELECT
                    pag.id,
                    'pago' AS tipo,
                    null AS valor_total,
                    null AS percentual_repasse,
                    pag.valor_pagamento AS valor,
                    'Pagamento efetuador por ' || pj.nome_fantasia AS descricao,
                    pag.data_operacao,
                    null AS referencia,
                    pag.arquivo_comprovante_id AS arquivo_id,
                    null AS agenda_id,
                    pf.nome_completo AS profissional,
                    null AS remunerada,
                    null AS solicitacao_correcao_agenda_id,
                    null AS descricao_solicitacao_correcao
                  FROM
                    estabelecimentosaude.tb_valor_pago pag
                  INNER JOIN
                    estabelecimentosaude.tb_estabelecimento est ON est.id = pag.estabelecimento_id AND est.excluido IS FALSE
                  INNER JOIN
                    corporativo.tb_pessoajuridica pj ON pj.id = est.pessoajuridica_id AND pj.excluido IS FALSE
                  INNER JOIN
                    profissionalsaude.tb_profissional pr ON pr.id = pag.profissional_id --AND pr.excluido IS FALSE
                  INNER JOIN
                    corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id --AND pf.excluido IS FALSE
                  WHERE
                    " . implode(' AND ', $arWhere) . "
                ORDER BY
                  data_operacao";

        $retorno = $this->carregar($sql);

        return $retorno ? $retorno : array();
    }

    public function getRelatorioAtendimentoPorMedico($filtros = array())
    {
        $arWhere = array();
        if(!empty($filtros['profissional_id'])){
            $arWhere[] = " pr.id = " . (int) $filtros['profissional_id'];
        }

        if(!empty($filtros['inicio']) && !empty($filtros['fim'])){
            $arWhere[] = " pag.data_operacao::date BETWEEN '{$filtros['inicio']}' AND '{$filtros['fim']}' ";
        }

        $arWhere[] = "pag.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();

        $sql = "  SELECT
                    pf.nome_completo AS profissional,
                    p.descricao,
                    paciente.nome_completo as paciente,
                    ce.nome AS planosaude, 
                    epa.valor_procedimento AS valor_total,
                    pag.valor_a_pagar AS valor,
                    pag.data_operacao,
                    pag.id
                  FROM
                    {$this->stNomeTabela} pag
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_profissional_agenda epa ON epa.id = pag.agenda_id AND epa.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id -- AND tc.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id -- AND eep.excluido IS FALSE
                  INNER JOIN
                      profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id -- AND ep.excluido IS FALSE
                  INNER JOIN
                      profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id -- AND pr.excluido IS FALSE
                  INNER JOIN
                      corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id -- AND pf.excluido IS FALSE                      
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_convenio_estabelecimento ce ON ce.id = epa.convenio_id -- AND ce.excluido IS FALSE
                  INNER JOIN
                      corporativo.tb_pessoafisica paciente ON paciente.id = epa.pessoa_paciente_id -- AND paciente.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id -- AND vp.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id -- AND p.excluido IS FALSE
                  WHERE
                    " . implode(' AND ', $arWhere) . "
                ORDER BY
                  data_operacao";

        $retorno = $this->carregar($sql);

        return $retorno ? $retorno : array();
    }


    public function getSaldoAnteriorEstabelecimentoByWhere($estabelecimentoId,$profissionalId,$dataLimite)
    {
        $arWhere = array();
        if(!empty($profissionalId)){
            $arWhere[] = " pr.id = " . (int) $profissionalId;
        }

        if(!empty($dataLimite)){
            $arWhere[] = " pag.data_operacao::date < '{$dataLimite}' ";
        }

        $arWhere[] = "pag.excluido IS FALSE";

        $sql = " SELECT
                   COALESCE(creditos.valor,0) - COALESCE(repasses.valor,0)  AS creditos
                FROM
                  estabelecimentosaude.tb_estabelecimento est
                LEFT JOIN (
                    SELECT
                      SUM(pag.valor_a_pagar) AS valor,
                      est.id AS estabelecimento_id
                    FROM
                      estabelecimentosaude.tb_valor_a_pagar pag
                    INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_profissional_agenda epa ON epa.id = pag.agenda_id AND epa.excluido IS FALSE
                    INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id --AND eep.excluido IS FALSE
                    INNER JOIN
                      profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id --AND ep.excluido IS FALSE
                    INNER JOIN
                      profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id --AND pr.excluido IS FALSE
                    INNER JOIN
                      corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id-- AND pf.excluido IS FALSE
                    INNER JOIN
                      estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                    WHERE
                      " . implode(' AND ', $arWhere) . "
                    GROUP BY
                      2
                  ) creditos ON creditos.estabelecimento_id = est.id
                  LEFT JOIN (
                    SELECT
                      SUM(pag.valor_pagamento) AS valor,
                      est.id AS estabelecimento_id
                    FROM
                      estabelecimentosaude.tb_valor_pago pag
                    INNER JOIN
                      estabelecimentosaude.tb_estabelecimento est ON est.id = pag.estabelecimento_id AND est.excluido IS FALSE
                    INNER JOIN
                      corporativo.tb_pessoajuridica pj ON pj.id = est.pessoajuridica_id AND pj.excluido IS FALSE
                    INNER JOIN
                      profissionalsaude.tb_profissional pr ON pr.id = pag.profissional_id --AND pr.excluido IS FALSE
                    INNER JOIN
                      corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id --AND pf.excluido IS FALSE
                    WHERE
                      " . implode(' AND ', $arWhere) . "
                    GROUP BY
                      2
                  ) repasses ON repasses.estabelecimento_id = est.id
                WHERE
                  est.id = '{$estabelecimentoId}';";

        return $this->pegaUm($sql);
    }


    public function getExtratoCompletoProfissionalByWhere($cpfProfissional,$filtros)
    {
        $arWhere = array();
        if(!empty($filtros['estabelecimento_id'])){
            $arWhere[] = " est.id = " . (int) $filtros['estabelecimento_id'];
        }

        if(!empty($filtros['inicio']) && !empty($filtros['fim'])){
            $arWhere[] = " pag.data_operacao::date BETWEEN '{$filtros['inicio']}' AND '{$filtros['fim']}' ";
        }

        $arWhere[] = "pag.excluido IS FALSE";
        $arWhere[] = "pf.cpf = '{$cpfProfissional}'";

        $sql = "  SELECT
                    pag.id,
                    'pagar' AS tipo,
                    epa.valor_procedimento AS valor_total,
                    epa.percentual_profissional AS percentual_repasse,
                    pag.valor_a_pagar AS valor,
                    COALESCE(p.descricao,'') || ' - Paciente: ' || COALESCE(paciente.nome_completo,'') || ' - ' || COALESCE(ce.nome,'') AS descricao,
                    pag.data_operacao,
                    pag.valor_pago_id AS referencia,
                    null AS arquivo_id,
                    pag.agenda_id,
                    tc.remunerada,
                    pag.solicitacao_correcao_agenda_id,
                    sca.descricao_solicitacao AS descricao_solicitacao_correcao
                  FROM
                    {$this->stNomeTabela} pag
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_profissional_agenda epa ON epa.id = pag.agenda_id AND epa.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id --AND tc.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id --AND eep.excluido IS FALSE
                  INNER JOIN
                      profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id --AND ep.excluido IS FALSE
                  INNER JOIN
                      profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                  INNER JOIN
                      corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id --AND pf.excluido IS FALSE                      
                  INNER JOIN
                      estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_convenio_estabelecimento ce ON ce.id = epa.convenio_id --AND ce.excluido IS FALSE
                  INNER JOIN
                      corporativo.tb_pessoafisica paciente ON paciente.id = epa.pessoa_paciente_id --AND paciente.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id --AND vp.excluido IS FALSE
                  INNER JOIN
                      estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id --AND p.excluido IS FALSE
                  LEFT JOIN
                      estabelecimentosaude.tb_solicitacao_correcao_agenda sca ON pag.solicitacao_correcao_agenda_id = sca.id
                  WHERE
                    " . implode(' AND ', $arWhere) . "
                UNION ALL
                  SELECT
                    pag.id,
                    'pago' AS tipo,
                    null AS valor_total,
                    null AS percentual_repasse,
                    pag.valor_pagamento AS valor,
                    'Pagamento efetuador por ' || pj.nome_fantasia AS descricao,
                    pag.data_operacao,
                    null AS referencia,
                    pag.arquivo_comprovante_id AS arquivo_id,
                    null AS agenda_id,
                    null AS remunerada,
                    null AS solicitacao_correcao_agenda_id,
                    null AS descricao_solicitacao_correcao
                  FROM
                    estabelecimentosaude.tb_valor_pago pag
                  INNER JOIN
                    estabelecimentosaude.tb_estabelecimento est ON est.id = pag.estabelecimento_id AND est.excluido IS FALSE
                  INNER JOIN
                    corporativo.tb_pessoajuridica pj ON pj.id = est.pessoajuridica_id AND pj.excluido IS FALSE
                  INNER JOIN
                    profissionalsaude.tb_profissional pr ON pr.id = pag.profissional_id --AND pr.excluido IS FALSE
                  INNER JOIN
                    corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id --AND pf.excluido IS FALSE
                  WHERE
                    " . implode(' AND ', $arWhere) . "
                ORDER BY
                  data_operacao";

        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public function getSaldoAnteriorProfissionalByWhere($cpfProfissional,$estabelecimentoId,$dataLimite)
    {
        $arWhere = array();
        if(!empty($estabelecimentoId)){
            $arWhere[] = " est.id = " . (int) $estabelecimentoId;
        }

        if(!empty($dataLimite)){
            $arWhere[] = " pag.data_operacao::date < '{$dataLimite}' ";
        }

        $arWhere[] = "pag.excluido IS FALSE";

        $sql = " SELECT
                  COALESCE(creditos.valor,0) - COALESCE(repasses.valor,0) AS creditos
                FROM
                  corporativo.tb_pessoafisica pf
                LEFT JOIN (
                    SELECT
                      SUM(pag.valor_a_pagar) AS valor,
                      pf.id AS pessoafisica_id
                    FROM
                      estabelecimentosaude.tb_valor_a_pagar pag
                    INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_profissional_agenda epa ON epa.id = pag.agenda_id AND epa.excluido IS FALSE
                    INNER JOIN
                      estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id --AND eep.excluido IS FALSE
                    INNER JOIN
                      profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id --AND ep.excluido IS FALSE
                    INNER JOIN
                      profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id --AND pr.excluido IS FALSE
                    INNER JOIN
                      corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id --AND pf.excluido IS FALSE
                    INNER JOIN
                      estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                    WHERE
                      " . implode(' AND ', $arWhere) . "
                    GROUP BY
                      2
                  ) creditos ON creditos.pessoafisica_id = pf.id
                  LEFT JOIN (
                    SELECT
                      SUM(pag.valor_pagamento) AS valor,
                      pf.id AS pessoafisica_id
                    FROM
                      estabelecimentosaude.tb_valor_pago pag
                    INNER JOIN
                      estabelecimentosaude.tb_estabelecimento est ON est.id = pag.estabelecimento_id AND est.excluido IS FALSE
                    INNER JOIN
                      corporativo.tb_pessoajuridica pj ON pj.id = est.pessoajuridica_id AND pj.excluido IS FALSE
                    INNER JOIN
                      profissionalsaude.tb_profissional pr ON pr.id = pag.profissional_id --AND pr.excluido IS FALSE
                    INNER JOIN
                      corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id --AND pf.excluido IS FALSE
                    WHERE
                      " . implode(' AND ', $arWhere) . "
                    GROUP BY
                      2
                  ) repasses ON repasses.pessoafisica_id = pf.id
                WHERE
                  pf.cpf = '{$cpfProfissional}';";

        return $this->pegaUm($sql);
    }

    public function efetuaPagamentoEmLote($itens,$valorPagoId)
    {
        $strIds = implode("','", $itens);
        $sql = "UPDATE {$this->stNomeTabela} SET valor_pago_id = '{$valorPagoId}' WHERE excluido IS FALSE AND id IN('{$strIds}');";
        return $this->pegaUm($sql);
    }

    public function getByValorPago($valorPagoId)
    {
        $sql = "SELECT
                    data_operacao AS data,
                    COALESCE(p.descricao,'') || ' - Paciente: ' || COALESCE(paciente.nome_completo,'') || ' - ' || COALESCE(ce.nome,'') AS descricao,
                    epa.valor_procedimento AS valorprocedimento,
                    epa.percentual_profissional AS percentualrepasse,
                    pag.valor_a_pagar AS valorpagamento
                FROM
                    {$this->stNomeTabela} pag
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa ON epa.id = pag.agenda_id AND epa.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                INNER JOIN
                  corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id AND pf.excluido IS FALSE                      
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento ce ON ce.id = epa.convenio_id AND ce.excluido IS FALSE
                INNER JOIN
                  corporativo.tb_pessoafisica paciente ON paciente.id = epa.pessoa_paciente_id AND paciente.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id AND vp.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id AND p.excluido IS FALSE
                WHERE
                  pag.valor_pago_id = {$valorPagoId} AND 
                  pag.excluido IS FALSE;";

        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public function desabilitarValoresPorAgenda($agenda_id)
    {
        $mValorPago = new ValorPago();
        $valores = $this->recuperarTodos('id, valor_pago_id',array("agenda_id = {$agenda_id}", "excluido IS FALSE"));
        if(is_array($valores) && count($valores) > 0){
            foreach ($valores AS $valor){
                if($valor['id']){
                    $this->excluirLogicamente($valor['id']);
                }

                if($valor['valor_pago_id']){
                    $mValorPago->excluirLogicamente($valor['valor_pago_id']);
                }
            }
        }
    }

    public function gerarValorNegativoAgenda($agenda_id, $solicitacao_correcao_agenda_id = null)
    {
        $arValorApagar = $this->recuperarTodos('*',array("agenda_id = {$agenda_id}", "excluido IS FALSE", "valor_a_pagar >= 0"), "data_operacao DESC");
        $valorApagar = isset($arValorApagar[0]) ? $arValorApagar[0] : array();

        if(is_array($valorApagar) && count($valorApagar) > 0){
            $valorApagar['id'] = null;
            $valorApagar['valor_pago_id'] = null;
            $valorApagar['data_operacao'] = 'now()';
            $valorApagar['valor_a_pagar'] = -1 * abs($valorApagar['valor_a_pagar']);
            $valorApagar['responsavel_operacao'] = $_SESSION['usuario_id'];
            $valorApagar['solicitacao_correcao_agenda_id'] = $solicitacao_correcao_agenda_id;

            $this->manter($valorApagar);
        }
    }

    public function gerarValorAgenda($agenda_id)
    {
        $mAgenda = new EstabelecimentoProfissionalAgenda($agenda_id);
        $dadosAgenda = $mAgenda->getDados();

        $mTipoConsulta = new TipoConsulta($dadosAgenda['tipo_consulta_id']);
        $dadosTipoConsulta = $mTipoConsulta->getDados();

        $valor = 0;
        if ($dadosTipoConsulta['remunerada'] == 't') {
            if (empty($dadosAgenda['valor_profissional']) && !empty($dadosAgenda['valor_procedimento'])) {
                $valor = $dadosAgenda['valor_procedimento'] * PERCENTUAL_GLOBAL_REPASSE_MEDICO / 100;
            } else {
                $valor = $dadosAgenda['valor_profissional'];
            }
        }

        //Gravando o registro de valor a pagar para o médico
        $dados = array(
            'agenda_id' => $agenda_id,
            'valor_a_pagar' => $valor,
            'data_operacao' => 'now()',
            'responsavel_operacao' => $_SESSION['usuario_id'],
            'excluido' => 'false'
        );
        $this->popularDadosObjeto($dados)->salvar();
    }
}