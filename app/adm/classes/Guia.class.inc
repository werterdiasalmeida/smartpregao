<?php

class Guia extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_guia";

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
        'guia' => null,
        'tipo_guia_id' => null,
        'agenda_id' => null,
        'convenio_estabelecimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getDadosAgendaGuia($agendaId)
    {
        $sqlConsultas = "SELECT
                    est.registro_ans AS registro_ans,
                    cep.numero AS numero_carteira,
                    TO_CHAR(cep.validade, 'DD/MM/YYYY') AS validade_carteira,
                    'N' AS atendimento_rn, 
                    paciente.nome_completo AS nome_beneficiario,
                    paciente.cartao_nacional_saude AS cartao_nacional_saude,
                    coe.codigo_operadora AS codigo_operadora,
                    pje.cnpj AS cnpj_contratado,
                    pje.nome_fantasia AS nome_contratado,
                    COALESCE(est.cnes, '9999999') AS cnes,
                    p.cpf AS codigo_operadora_profissional,
                    p.nome_completo AS nome_profissional,
                    tpr.codigo_conselho AS conselho_profissional,
                    pro.registro_profissional AS numero_conselho_profissional,
                    uf.uf_codigo_ibge AS uf_conselho_profissional,
                    esp.cbos AS cbos,
                    9 AS indicacao_acidente,
                    '04' AS tipo_atendimento,
                    TO_CHAR(epa.inicio::date, 'DD/MM/YYYY') AS data_atendimento,
                    CASE WHEN epa.tipo_consulta_id = 5 THEN 1 ELSE 2 END AS tipo_consulta,
                    '00' AS codigo_tabela,
                    proc.codigo_ans AS codigo_procedimento,
                    epa.valor_procedimento AS valor_procedimento,
                    est.id AS estabelecimento_id,
                    coe.id AS convenio_estabelecimento_id,
                    epa.id AS agenda_id,
                    proc.tipo_guia_id,
                    TO_CHAR(epa.inicio, 'HH24:MI') AS hora_inicial,
                    TO_CHAR(epa.fim, 'HH24:MI') AS hora_final,
                    TO_CHAR(epa.inicio::date, 'DD/MM/YYYY') AS data_solicitacao,
                    coe.codigo_operadora AS codigo_operadora_executante,
                    pje.nome_fantasia AS nome_contratado_executante,
                    epa.valor_procedimento AS total_procedimentos,
                    epa.valor_procedimento AS total_geral,
                    1 AS carater_atendimento,
                    proc.id AS procedimento_id,
                    vlr.id AS valor_procedimento_id
                FROM
                    estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                LEFT JOIN
                    estabelecimentosaude.tb_valor_procedimento vlr ON epa.valor_procedimento_id = vlr.id
                LEFT JOIN
                    estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON vlr.tabela_procedimento_convenio_id = tpc.id
                LEFT JOIN
                    estabelecimentosaude.tb_procedimento proc ON vlr.procedimento_id = proc.id
                LEFT JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                LEFT JOIN
                    estabelecimentosaude.tb_tipo_consulta tco ON epa.tipo_consulta_id = tco.id
                LEFT JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                LEFT JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                LEFT JOIN
                    profissionalsaude.tb_especialidade esp ON ep.especialidade_id = esp.id
                LEFT JOIN
                    profissionalsaude.tb_profissional pro ON ep.profissional_id = pro.id
                LEFT JOIN
                    corporativo.tb_pessoafisica p ON pro.pessoafisica_id = p.id
                LEFT JOIN
                    corporativo.tb_pessoafisica paciente ON epa.pessoa_paciente_id = paciente.id
                LEFT JOIN
                    estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                LEFT JOIN
                    corporativo.tb_pessoajuridica pje ON est.pessoajuridica_id = pje.id
                LEFT JOIN
                    profissionalsaude.tb_tipo_profissional tpr ON tpr.id = pro.tipo_profissional_id
                LEFT JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento_paciente cep ON coe.id = cep.convenio_estabelecimento_id 
                                                                                 AND cep.pessoa_paciente_id = epa.pessoa_paciente_id
                                                                                 AND cep.excluido IS FALSE
                LEFT JOIN 
                    corporativo.tb_unidade_federacao uf ON pro.uf_registro = uf.uf_sigla
                WHERE
                    epa.id = {$agendaId}";

        $agendaGuia = $this->pegaLinha($sqlConsultas);

        $mTipoGuia = new TipoGuia();
        $tipoGuiaId = $agendaGuia['tipo_guia_id'] ?: $mTipoGuia->getIdByTipo(TipoGuia::TIPO_GUIA_CONSULTA);
        $tipo = $mTipoGuia->getTipoById($tipoGuiaId);

        switch ($tipo) {
            case TipoGuia::TIPO_GUIA_EXAME :
                $mValorCodigo = new ValorProcedimentoCodigo();
                $agendaGuia['valor_procedimento'] = $mValorCodigo->getAssocByProcedimentoValor($agendaGuia['procedimento_id'], $agendaGuia['valor_procedimento_id']);
                break;
        }

        return $agendaGuia;
    }

    public function getListAjax($where = [], $offset = null, $limit = '100')
    {
        $where[] = "gui.excluido IS FALSE";

        $sql = "SELECT
                    gui.guia->>'numero_guia_prestador' AS guia_numero,
                    tgu.descricao AS guia_tipo,
                    gui.guia->>'nome_beneficiario' AS paciente_nome,
                    gui.guia->>'numero_carteira' AS paciente_num_carteira,
                    age.inicio::date AS atendimento_data,
                    age.valor_procedimento AS atendimento_valor,
                    gui.id,
                    gui.agenda_id,
                    tgu.tipo AS tipo_guia,
                    gui.guia
                FROM
                    estabelecimentosaude.tb_guia gui
                LEFT JOIN
                    estabelecimentosaude.tb_tipo_guia tgu ON gui.tipo_guia_id = tgu.id
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_profissional_agenda age ON gui.agenda_id = age.id
                WHERE
                    " . implode(' AND ', $where) . "
                ORDER BY
                    5 ASC, 1 ASC";

        $_POST['offset'] = $offset;
        $paginacao =  Paginacao::getPaginacao($this, $sql, $limit);

        return [
            'total' => $paginacao['total'],
            'totalNotFiltered' => $paginacao['total'],
            'rows' => $paginacao['dados']
        ];
    }

    public function getListAjaxSemLote(array $where = [], $offset = null, $limit = '100', $loteId = null)
    {
        $where[] = "gui.id NOT IN (SELECT 
                                        log.guia_id 
                                   FROM 
                                        estabelecimentosaude.tb_lote_guia log
                                   INNER JOIN
                                        estabelecimentosaude.tb_lote lot ON log.lote_id = lot.id AND lot.excluido IS FALSE
                                   WHERE 
                                        log.excluido IS FALSE" . (!empty($loteId) ? " AND lote_id != {$loteId}" : "") . ")";

        $lista = $this->getListAjax($where, $offset, $limit);
        $lista['rows'] = array_map(function($guia){
            $dadosGuia = json_decode(utf8_decode($guia['guia']), true);
            $guia['erros'] = $this->validar($dadosGuia, $guia['tipo_guia']);
            $guia['is_valida'] = count($guia['erros']) === 0;

            unset($guia['guia']);
            return $guia;
        }, $lista['rows']);

        return $lista;
    }

    public function getGuiaByAgenda($agendaId)
    {
        $dadosGuia = $this->recuperarLinha('*', ["agenda_id = {$agendaId}", "excluido IS FALSE"], false);

        if (empty($dadosGuia)) {
            return [];
        }

        $dadosGuia['guia_json'] = $dadosGuia['guia'];
        $dadosGuia['guia'] = utf8ArrayDecode(json_decode(utf8_encode($dadosGuia['guia_json']), true));

        $mTipoGuia = new TipoGuia();
        switch ($mTipoGuia->getTipoById($dadosGuia['tipo_guia_id'])) {
            case TipoGuia::TIPO_GUIA_EXAME :
                $dadosGuia['guia']['executado'] = is_array($dadosGuia['guia']['executado'])
                    ? $dadosGuia['guia']['executado'] : [];
                foreach ($dadosGuia['guia']['executado'] as &$executado) {
                    $executado['data_execucao'] = formata_data($executado['data_execucao']);
                }

                $dadosGuia['guia']['profissional_executante'] = is_array($dadosGuia['guia']['profissional_executante'])
                    ? $dadosGuia['guia']['profissional_executante'] : [];
                foreach ($dadosGuia['guia']['profissional_executante'] as &$profissionalExecutante) {
                    $profissionalExecutante['data_realizacao'] = formata_data($profissionalExecutante['data_realizacao']);
                }

                $dadosGuia['guia']['data_realizacao_procedimentos'] = isset($dadosGuia['guia']['data_realizacao_procedimentos']) && is_array($dadosGuia['guia']['data_realizacao_procedimentos'])
                    ? $dadosGuia['guia']['data_realizacao_procedimentos'] : [];
                foreach ($dadosGuia['guia']['data_realizacao_procedimentos'] as &$dataRealizacaoProcedimentos) {
                    if(isset($dataRealizacaoProcedimentos['data_realizacao_procedimentos'])) {
                        $dataRealizacaoProcedimentos['data_realizacao_procedimentos'] = formata_data($dataRealizacaoProcedimentos['data_realizacao_procedimentos']);
                    }
                }
                break;
        }

        return $dadosGuia;
    }

    public function gerarGuiaByAgenda($agendaId)
    {
        $agenda = $this->getDadosAgendaGuia($agendaId);

        $mTipoGuia = new TipoGuia();
        $tipoGuiaId = $agenda['tipo_guia_id'] ?: $mTipoGuia->getIdByTipo(TipoGuia::TIPO_GUIA_CONSULTA);
        $tipo = $mTipoGuia->getTipoById($tipoGuiaId);

        switch ($tipo) {
            case TipoGuia::TIPO_GUIA_EXAME :
                $guia = $agenda;
                $procedimentos = explode(',', $agenda['codigo_procedimento']);

                $guia['solicitado'] = array_map(function ($codProcedimento) use ($guia) {
                    return [
                        'codigo_tabela' => $guia['codigo_tabela'],
                        'codigo_procedimento' => $codProcedimento,
                        'descricao_procedimento' => $guia['valor_procedimento'][$codProcedimento]['procedimento'],
                        'qtd_solicitada' => 1,
                        'qtd_autorizada' => 1,
                    ];
                }, $procedimentos);

                $guia['executado'] = array_map(function ($codProcedimento) use ($guia) {
                    return [
                        'data_execucao' => $guia['data_atendimento'],
                        'hora_inicial_execucao' => $guia['hora_inicial'],
                        'hora_final_execucao' => $guia['hora_final'],
                        'codigo_tabela_execucao' => $guia['codigo_tabela'],
                        'codigo_procedimento_execucao' => $codProcedimento,
                        'descricao_execucao' => $guia['valor_procedimento'][$codProcedimento]['procedimento'],
                        'qtd_execucao' => 1,
                        'via' => null,
                        'tec' => null,
                        'fator_red_acresc' => null,
                        'valor_unitario' => $guia['valor_procedimento'][$codProcedimento]['valor'],
                        'valor_total' => $guia['valor_procedimento'][$codProcedimento]['valor'],
                    ];
                }, $procedimentos);

                $guia['profissional_executante'] = [
                    [
                        'seq_ref' => 1,
                        'grau_part' => 12,
                        'codigo_operadora' => $guia['codigo_operadora_profissional'],
                        'nome_profissional' => $guia['nome_profissional'],
                        'conselho_profissional' => $guia['conselho_profissional'],
                        'numero_conselho_profissional' => $guia['numero_conselho_profissional'],
                        'uf_conselho_profissional' => $guia['uf_conselho_profissional'],
                        'cbos' => $guia['cbos'],
                        'data_realizacao' => $guia['data_atendimento'],
                    ]
                ];

                $guia['data_realizacao_procedimentos'] = [
                    ['data_realizacao_procedimentos' => $guia['data_atendimento']]
                ];

                break;
            default :
                $guia = $agenda;
                break;
        }

        return $this->salvarGuia($guia);
    }

    public function salvarGuia($params)
    {
        $dadosGuiaBd = $this->getGuiaByAgenda($params['agenda_id']);
        $dadosGuia = array_merge(($dadosGuiaBd ? $dadosGuiaBd['guia'] : []), $params);
        $dadosGuia['convenio_estabelecimento_id'] = $params['convenio_estabelecimento_id'];

        $mTipoGuia = new TipoGuia();
        $tipoGuiaId = $params['tipo_guia_id'] ?: $mTipoGuia->getIdByTipo(TipoGuia::TIPO_GUIA_CONSULTA);
        $tipoGuia = $mTipoGuia->getTipoById($tipoGuiaId);
        $guia = $this->gerarDadosGuia($params, $tipoGuia);

        $dadosGuia['id'] = $dadosGuiaBd['id'];
        $dadosGuia['agenda_id'] = $params['agenda_id'];
        $dadosGuia['tipo_guia_id'] = $tipoGuiaId;
        $dadosGuia['convenio_estabelecimento_id'] = $params['convenio_estabelecimento_id'];

        $dadosGuia['guia'] = json_encode(utf8ArrayEncode($guia));
        $id = $this->manter($dadosGuia);

        $valorProfissional = isset($params['valor_profissional']) ?
            $params['valor_profissional'] : null;

        switch ($tipoGuia) {
            case TipoGuia::TIPO_GUIA_EXAME :
                $valorProcedimento = $params['total_geral'];
                break;
            default :
                $valorProcedimento = $params['valor_procedimento'];
                break;
        }

        $mAgenda = new EstabelecimentoProfissionalAgenda();
        $mAgenda->alterarValorAgenda($dadosGuia['agenda_id'], $valorProcedimento, $valorProfissional);

        return $id;
    }

    public function gerarDadosGuia($params, $tipoGuia)
    {
        switch ($tipoGuia) {
            case TipoGuia::TIPO_GUIA_EXAME :
                $guia = [
                    'registro_ans' => $params['registro_ans'],
                    'numero_guia_prestador' => $params['numero_guia_prestador'],
                    'numero_guia_principal' => $params['numero_guia_principal'],
                    'data_autorizacao' => formata_data_sql($params['data_autorizacao']),
                    'senha' => $params['senha'],
                    'data_validade_senha' => formata_data_sql($params['data_validade_senha']),
                    'numero_guia_operadora' => $params['numero_guia_operadora'],
                    'numero_carteira' => $params['numero_carteira'],
                    'validade_carteira' => formata_data_sql($params['validade_carteira']),
                    'nome_beneficiario' => $params['nome_beneficiario'],
                    'cartao_nacional_saude' => $params['cartao_nacional_saude'],
                    'atendimento_rn' => $params['atendimento_rn'],
                    'codigo_operadora' => $params['codigo_operadora'],
                    'nome_contratado' => $params['nome_contratado'],
                    'nome_profissional' => $params['nome_profissional'],
                    'conselho_profissional' => $params['conselho_profissional'],
                    'numero_conselho_profissional' => $params['numero_conselho_profissional'],
                    'uf_conselho_profissional' => $params['uf_conselho_profissional'],
                    'cbos' => $params['cbos'],
                    'carater_atendimento' => $params['carater_atendimento'],
                    'data_solicitacao' => formata_data_sql($params['data_solicitacao']),
                    'indicacao_clinica' => $params['indicacao_clinica'],
                    'solicitado' => [],
                    'codigo_operadora_executante' => $params['codigo_operadora_executante'],
                    'nome_contratado_executante' => $params['nome_contratado_executante'],
                    'cnes' => $params['cnes'],
                    'tipo_atendimento' => $params['tipo_atendimento'],
                    'indicacao_acidente' => $params['indicacao_acidente'],
                    'tipo_consulta' => $params['tipo_consulta'],
                    'motivo_encerramento' => $params['motivo_encerramento'],
                    'executado' => [],
                    'profissional_executante' => [],
                    'observacao' => $params['observacao'],
                    'total_procedimentos' => (strpos($params['total_procedimentos'], ',') === false ? $params['total_procedimentos'] : desformata_valor($params['total_procedimentos'])),
                    'total_taxas_alugueis' => (strpos($params['total_taxas_alugueis'], ',') === false ? $params['total_taxas_alugueis'] : desformata_valor($params['total_taxas_alugueis'])),
                    'total_materiais' => (strpos($params['total_materiais'], ',') === false ? $params['total_materiais'] : desformata_valor($params['total_materiais'])),
                    'total_opme' => (strpos($params['total_opme'], ',') === false ? $params['total_opme'] : desformata_valor($params['total_opme'])),
                    'total_medicamentos' => (strpos($params['total_medicamentos'], ',') === false ? $params['total_medicamentos'] : desformata_valor($params['total_medicamentos'])),
                    'total_gases_medicinais' => (strpos($params['total_gases_medicinais'], ',') === false ? $params['total_gases_medicinais'] : desformata_valor($params['total_gases_medicinais'])),
                    'total_geral' => (strpos($params['total_geral'], ',') === false ? $params['total_geral'] : desformata_valor($params['total_geral'])),
                ];

                $guia['solicitado'] = is_array($guia['solicitado']) ? $guia['solicitado'] : [];
                $guia['executado'] = is_array($guia['executado']) ? $guia['executado'] : [];
                $guia['profissional_executante'] = is_array($guia['profissional_executante']) ? $guia['profissional_executante'] : [];

                if(!is_array($params['solicitado'])) {
                    $solicitado = str_replace('\"', '"', $params['solicitado']);
                    $params['solicitado'] = utf8ArrayDecode(json_decode(utf8_encode($solicitado), true));
                }

                $guia['solicitado'] = array_map(function ($solicitado) {
                    return [
                        'codigo_tabela' => $solicitado['codigo_tabela'],
                        'codigo_procedimento' => $solicitado['codigo_procedimento'],
                        'descricao_procedimento' => $solicitado['descricao_procedimento'],
                        'qtd_solicitada' => $solicitado['qtd_solicitada'],
                        'qtd_autorizada' => $solicitado['qtd_autorizada'],
                    ];
                }, $params['solicitado']);

                if(!is_array($params['executado'])) {
                    $executado = str_replace('\"', '"', $params['executado']);
                    $params['executado'] = utf8ArrayDecode(json_decode(utf8_encode($executado), true));
                }

                $guia['executado'] = array_map(function ($executado) {
                    return [
                        'data_execucao' => formata_data_sql($executado['data_execucao']),
                        'hora_inicial_execucao' => $executado['hora_inicial_execucao'],
                        'hora_final_execucao' => $executado['hora_final_execucao'],
                        'codigo_tabela_execucao' => $executado['codigo_tabela_execucao'],
                        'codigo_procedimento_execucao' => $executado['codigo_procedimento_execucao'],
                        'descricao_execucao' => $executado['descricao_execucao'],
                        'qtd_execucao' => $executado['qtd_execucao'],
                        'via' => $executado['via'],
                        'tec' => $executado['tec'],
                        'fator_red_acresc' => (strpos($executado['fator_red_acresc'], ',') === false ? $executado['fator_red_acresc'] : desformata_valor($executado['fator_red_acresc'])),
                        'valor_unitario' => (strpos($executado['valor_unitario'], ',') === false ? $executado['valor_unitario'] : desformata_valor($executado['valor_unitario'])),
                        'valor_total' => (strpos($executado['valor_total'], ',') === false ? $executado['valor_total'] : desformata_valor($executado['valor_total'])),
                    ];
                }, $params['executado']);

                if(!is_array($params['profissional_executante'])) {
                    $profissionalExecutante = str_replace('\"', '"', $params['profissional_executante']);
                    $params['profissional_executante'] = utf8ArrayDecode(json_decode(utf8_encode($profissionalExecutante), true));
                }

                $guia['profissional_executante'] = array_map(function ($profissionalExecutante) {
                    return [
                        'seq_ref' => $profissionalExecutante['seq_ref'],
                        'grau_part' => $profissionalExecutante['grau_part'],
                        'codigo_operadora' => $profissionalExecutante['codigo_operadora'],
                        'nome_profissional' => $profissionalExecutante['nome_profissional'],
                        'conselho_profissional' => $profissionalExecutante['conselho_profissional'],
                        'numero_conselho_profissional' => $profissionalExecutante['numero_conselho_profissional'],
                        'uf_conselho_profissional' => $profissionalExecutante['uf_conselho_profissional'],
                        'cbos' => $profissionalExecutante['cbos'],
                        'data_realizacao' => formata_data_sql($profissionalExecutante['data_realizacao']),
                    ];
                }, $params['profissional_executante']);

                if(!is_array($params['data_realizacao_procedimentos'])) {
                    $dataRealizacaoProcedimentos = str_replace('\"', '"', $params['data_realizacao_procedimentos']);
                    $params['data_realizacao_procedimentos'] = utf8ArrayDecode(json_decode(utf8_encode($dataRealizacaoProcedimentos), true));
                }

                $guia['data_realizacao_procedimentos'] = array_map(function ($dataRealizacao) {
                    return [
                        'data_realizacao_procedimentos' => formata_data_sql($dataRealizacao['data_realizacao_procedimentos'])
                    ];
                }, $params['data_realizacao_procedimentos']);

                break;
            default :
                if(is_array($params['valor_procedimento'])) {
                    $vlrProcedimento = array_shift($params['valor_procedimento']);
                    $params['valor_procedimento'] = $vlrProcedimento['valor'];
                }

                $guia = [
                    'registro_ans' => $params['registro_ans'],
                    'numero_guia_prestador' => $params['numero_guia_prestador'],
                    'numero_guia_operadora' => $params['numero_guia_operadora'],
                    'numero_carteira' => $params['numero_carteira'],
                    'validade_carteira' => formata_data_sql($params['validade_carteira']),
                    'atendimento_rn' => $params['atendimento_rn'],
                    'nome_beneficiario' => $params['nome_beneficiario'],
                    'cartao_nacional_saude' => $params['cartao_nacional_saude'],
                    'codigo_operadora' => $params['codigo_operadora'],
                    'nome_contratado' => $params['nome_contratado'],
                    'cnes' => $params['cnes'],
                    'nome_profissional' => $params['nome_profissional'],
                    'conselho_profissional' => $params['conselho_profissional'],
                    'numero_conselho_profissional' => $params['numero_conselho_profissional'],
                    'uf_conselho_profissional' => $params['uf_conselho_profissional'],
                    'cbos' => $params['cbos'],
                    'indicacao_acidente' => $params['indicacao_acidente'],
                    'data_atendimento' => formata_data_sql($params['data_atendimento']),
                    'tipo_consulta' => $params['tipo_consulta'],
                    'codigo_tabela' => $params['codigo_tabela'],
                    'codigo_procedimento' => $params['codigo_procedimento'],
                    'valor_procedimento' => (strpos($params['valor_procedimento'], ',') === false ? $params['valor_procedimento'] : desformata_valor($params['valor_procedimento'])),
                    'observacao' => $params['observacao'],
                ];
                break;

        }

        if (empty($guia['numero_guia_prestador'])) {
            if (empty($params['estabelecimento_id'])) {
                $agenda = $this->getDadosAgendaGuia($params['agenda_id']);
                $params['estabelecimento_id'] = $agenda['estabelecimento_id'];
            }

            $guia['numero_guia_prestador'] = $this->gerarNumeroGuia($params['estabelecimento_id']);
        }

        return $guia;
    }

    public function isAgendaPossuiGuia($agendaId)
    {
        if (empty($agendaId)) {
            return false;
        }

        $qtdGuia = $this->recuperarUm('COUNT(1)', ["agenda_id = {$agendaId}", "excluido IS FALSE"], false);
        return $qtdGuia > 0;
    }

    public function gerarNumeroGuia($estabelecimentoId)
    {
        $anoAtual = date('Y');

        $numeroGuia = 1 + $this->pegaUm(
                "SELECT 
                          COUNT(1) 
                      FROM 
                          estabelecimentosaude.tb_guia gui
                      INNER JOIN
                          estabelecimentosaude.tb_estabelecimento_profissional_agenda age ON gui.agenda_id = age.id
                      INNER JOIN
                          estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON age.estabelecimento_especialidade_profissional_id = eep.id
                      WHERE
                          eep.estabelecimento_id = {$estabelecimentoId}
                          AND
                          TO_CHAR(gui.criado_em, 'YYYY') = '{$anoAtual}'");

        // Ex: 00001/2019
        return str_pad($numeroGuia, 5, '0', STR_PAD_LEFT) . "/{$anoAtual}";
    }

    public function arquivarGuiaByAgenda($agendaId)
    {
        $guia = $dadosGuia = $this->recuperarLinha('*', ["agenda_id = {$agendaId}", "excluido IS FALSE"], false);
        if(empty($guia)) {
            return false;
        }

        $guia['excluido'] = 't';
        $dados = $this->manter($guia);
        $this->setDadosNull();

        return $dados;
    }

    public function getDominioRecemNascido()
    {
        return [
            ['codigo' => 'S', 'descricao' => 'Sim'],
            ['codigo' => 'N', 'descricao' => 'Não'],
        ];
    }

    public function getDominioRecemNascidoRadio()
    {
        $domRadio = [];
        foreach ($this->getDominioRecemNascido() as $dom) {
            $domRadio[$dom['descricao']] = [
                'valor' => $dom['codigo']
            ];
        }

        return $domRadio;
    }

    public function getDominioIndicacaoAcidente($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '0', 'descricao' => '0 - Trabalho'],
            ['codigo' => '1', 'descricao' => '1 - Trânsito'],
            ['codigo' => '2', 'descricao' => '2 - Outros acidentes'],
            ['codigo' => '9', 'descricao' => '9 - Não acidentes'],
        ]);
    }

    public function getDominioTipoConsulta($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '1', 'descricao' => '1 - Primeira'],
            ['codigo' => '2', 'descricao' => '2 - Seguimento'],
            ['codigo' => '3', 'descricao' => '3 - Pré-Natal'],
            ['codigo' => '4', 'descricao' => '4 - Por encaminhamento'],
        ]);
    }

    public function getDominioTipoTabela($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '18', 'descricao' => '18 - Taxas hospitalares, diárias e gases medicinais'],
            ['codigo' => '19', 'descricao' => '19 - Materiais'],
            ['codigo' => '20', 'descricao' => '20 - Medicamentos'],
            ['codigo' => '22', 'descricao' => '22 - Procedimentos e eventos em saúde (medicina, odonto e demais áreas de saúde)'],
            ['codigo' => '00', 'descricao' => '00 - Tabela própria das operadoras'],
        ]);
    }

    public function getDominioConselho($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];
        return array_merge($optSelecione, [
            ['codigo' => '01', 'descricao' => 'CRAS'],
            ['codigo' => '02', 'descricao' => 'COREN'],
            ['codigo' => '03', 'descricao' => 'CRF'],
            ['codigo' => '04', 'descricao' => 'CRFA'],
            ['codigo' => '05', 'descricao' => 'CREFITO'],
            ['codigo' => '06', 'descricao' => 'CRM'],
            ['codigo' => '07', 'descricao' => 'CRN'],
            ['codigo' => '08', 'descricao' => 'CRO'],
            ['codigo' => '09', 'descricao' => 'CRP'],
            ['codigo' => '10', 'descricao' => 'OUT'],
        ]);
    }

    public function getDominioUf($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        $mUf = new UnidadeFederacao();
        return array_merge(
            $optSelecione,
            $mUf->recuperarTodos("uf_codigo_ibge AS codigo, uf_sigla AS descricao", [], 1)
        );
    }

    public function getDominioCaraterAtendimento($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '1', 'descricao' => '1 - Eletiva'],
            ['codigo' => '2', 'descricao' => '2 - Urgência/Emergência'],
        ]);
    }

    public function getDominioTipoAtendimento($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '01', 'descricao' => '01 - Remoção'],
            ['codigo' => '02', 'descricao' => '02 - Pequena Cirurgia'],
            ['codigo' => '03', 'descricao' => '03 - Terapias'],
            ['codigo' => '04', 'descricao' => '04 - Consulta'],
            ['codigo' => '05', 'descricao' => '05 - Exames (englobando exame radiológico)'],
            ['codigo' => '06', 'descricao' => '06 - Atendimento Domiciliar'],
            ['codigo' => '07', 'descricao' => '07 - Internação'],
            ['codigo' => '08', 'descricao' => '08 - Quimioterapia'],
            ['codigo' => '09', 'descricao' => '09 - Radioterapia'],
            ['codigo' => '10', 'descricao' => '10 - Terapia Renal Substitutiva (TRS)'],
            ['codigo' => '11', 'descricao' => '11 - Pronto Socorro'],
            ['codigo' => '13', 'descricao' => '13 - Pequenos atendimentos'],
            ['codigo' => '14', 'descricao' => '14 - Admissional'],
            ['codigo' => '15', 'descricao' => '15 - Demissional'],
            ['codigo' => '16', 'descricao' => '16 - Periódico'],
            ['codigo' => '17', 'descricao' => '17 - Retorno ao trabalho'],
            ['codigo' => '18', 'descricao' => '18 - Mudança de função'],
            ['codigo' => '14', 'descricao' => '14 - Saúde Ocupacional - Admissional'],
            ['codigo' => '15', 'descricao' => '15 - Saúde Ocupacional - Demissional'],
            ['codigo' => '16', 'descricao' => '16 - Saúde Ocupacional - Periódico'],
            ['codigo' => '17', 'descricao' => '17 - Saúde Ocupacional - Retorno ao trabalho'],
            ['codigo' => '18', 'descricao' => '18 - Saúde Ocupacional - Mudança de função'],
            ['codigo' => '19', 'descricao' => '19 - Saúde Ocupacional - Promoção a saúde'],
            ['codigo' => '20', 'descricao' => '20 - Saúde Ocupacional - Beneficiário novo'],
            ['codigo' => '21', 'descricao' => '21 - Saúde Ocupacional - Assistência a demitidos'],
        ]);
    }

    public function getDominioGrauParticipacao($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '00', 'descricao' => '00 - Cirurgião'],
            ['codigo' => '01', 'descricao' => '01 - Primeiro Auxiliar'],
            ['codigo' => '02', 'descricao' => '02 - Segundo Auxiliar'],
            ['codigo' => '03', 'descricao' => '03 - Terceiro Auxiliar'],
            ['codigo' => '04', 'descricao' => '04 - Quarto Auxiliar'],
            ['codigo' => '05', 'descricao' => '05 - Instrumentador'],
            ['codigo' => '06', 'descricao' => '06 - Anestesista'],
            ['codigo' => '07', 'descricao' => '07 - Auxiliar de Anestesista'],
            ['codigo' => '08', 'descricao' => '08 - Consultor'],
            ['codigo' => '09', 'descricao' => '09 - Perfusionista'],
            ['codigo' => '10', 'descricao' => '10 - Pediatra na sala de parto'],
            ['codigo' => '11', 'descricao' => '11 - Auxiliar SADT'],
            ['codigo' => '12', 'descricao' => '12 - Clínico'],
            ['codigo' => '13', 'descricao' => '13 - Intensivista'],
        ]);
    }

    public function getDominioViaAcesso($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '1', 'descricao' => '1 - Única'],
            ['codigo' => '2', 'descricao' => '2 - Mesma via'],
            ['codigo' => '3', 'descricao' => '3 - Diferentes vias'],
        ]);
    }

    public function getDominioTecnicaUtilizada($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '1', 'descricao' => '1 - Convencional'],
            ['codigo' => '2', 'descricao' => '2 - Videolaparoscopia'],
            ['codigo' => '3', 'descricao' => '3 - Robótica'],
        ]);
    }

    public function getDominioMotivoEncerramento($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => 41, 'descricao' => '41 - Óbito com declaração de óbito fornecida pelo médico assistente'],
            ['codigo' => 42, 'descricao' => '42 - Óbito com declaração de Óbito fornecida pelo Instituto Médico Legal'],
            ['codigo' => 43, 'descricao' => '43 - Óbito com declaração de Óbito fornecida pelo Serviço de Verificação de Óbito - SVO']
        ]);
    }

    public function getDominioCBO($selecione = true)
    {
        $optSelecione = $selecione ? [['codigo' => '', 'descricao' => ' ']] : [];

        return array_merge($optSelecione, [
            ['codigo' => '201115', 'descricao' => '201115 - Geneticista'],
            ['codigo' => '203015', 'descricao' => '203015 - Pesquisador em biologia de microorganismos e parasitas'],
            ['codigo' => '213150', 'descricao' => '213150 - Físico médico'],
            ['codigo' => '221105', 'descricao' => '221105 - Biólogo'],
            ['codigo' => '225105', 'descricao' => '225105 - Médico acupunturista'],
            ['codigo' => '225110', 'descricao' => '225110 - Médico alergista e imunologista'],
            ['codigo' => '225148', 'descricao' => '225148 - Médico anatomopatologista'],
            ['codigo' => '225115', 'descricao' => '225115 - Médico angiologista'],
            ['codigo' => '225120', 'descricao' => '225120 - Médico cardiologista'],
            ['codigo' => '225210', 'descricao' => '225210 - Médico cirurgião cardiovascular'],
            ['codigo' => '225215', 'descricao' => '225215 - Médico cirurgião de cabeça e pescoço'],
            ['codigo' => '225220', 'descricao' => '225220 - Médico cirurgião do aparelho digestivo'],
            ['codigo' => '225225', 'descricao' => '225225 - Médico cirurgião geral'],
            ['codigo' => '225230', 'descricao' => '225230 - Médico cirurgião pediátrico'],
            ['codigo' => '225235', 'descricao' => '225235 - Médico cirurgião plástico'],
            ['codigo' => '225240', 'descricao' => '225240 - Médico cirurgião torácico'],
            ['codigo' => '225305', 'descricao' => '225305 - Médico citopatologista'],
            ['codigo' => '225125', 'descricao' => '225125 - Médico clínico'],
            ['codigo' => '225130', 'descricao' => '225130 - Médico de família e comunidade'],
            ['codigo' => '225135', 'descricao' => '225135 - Médico dermatologista'],
            ['codigo' => '225140', 'descricao' => '225140 - Médico do trabalho'],
            ['codigo' => '225310', 'descricao' => '225310 - Médico em endoscopia'],
            ['codigo' => '225145', 'descricao' => '225145 - Médico em medicina de tráfego'],
            ['codigo' => '225150', 'descricao' => '225150 - Médico em medicina intensiva'],
            ['codigo' => '225315', 'descricao' => '225315 - Médico em medicina nuclear'],
            ['codigo' => '225320', 'descricao' => '225320 - Médico em radiologia e diagnóstico por imagem'],
            ['codigo' => '225155', 'descricao' => '225155 - Médico endocrinologista e metabologista'],
            ['codigo' => '225160', 'descricao' => '225160 - Médico fisiatra'],
            ['codigo' => '225245', 'descricao' => '225245 - Médico foniatra'],
            ['codigo' => '225165', 'descricao' => '225165 - Médico gastroenterologista'],
            ['codigo' => '225170', 'descricao' => '225170 - Médico generalista'],
            ['codigo' => '225175', 'descricao' => '225175 - Médico geneticista'],
            ['codigo' => '225180', 'descricao' => '225180 - Médico geriatra'],
            ['codigo' => '225250', 'descricao' => '225250 - Médico ginecologista e obstetra'],
            ['codigo' => '225185', 'descricao' => '225185 - Médico Hematologista'],
            ['codigo' => '225190', 'descricao' => '225190 - Médico Hemoterapeuta'],
            ['codigo' => '225195', 'descricao' => '225195 - Médico Homeopata'],
            ['codigo' => '225103', 'descricao' => '225103 - Médico infectologista'],
            ['codigo' => '225106', 'descricao' => '225106 - Médico legista'],
            ['codigo' => '225255', 'descricao' => '225255 - Médico Mastologista'],
            ['codigo' => '225109', 'descricao' => '225109 - Médico Nefrologista'],
            ['codigo' => '225260', 'descricao' => '225260 - Médico  neurocirurgião'],
            ['codigo' => '225350', 'descricao' => '225350 - Médico neurofisiologista'],
            ['codigo' => '225112', 'descricao' => '225112 - Médico neurologista'],
            ['codigo' => '225118', 'descricao' => '225118 - Médico nutrologista'],
            ['codigo' => '225265', 'descricao' => '225265 - Médico oftalmologista'],
            ['codigo' => '225121', 'descricao' => '225121 - Médico oncologista'],
            ['codigo' => '225270', 'descricao' => '225270 - Médico ortopedista e traumatologista'],
            ['codigo' => '225275', 'descricao' => '225275 - Médico otorrinolaringologista'],
            ['codigo' => '225325', 'descricao' => '225325 - Médico patologista clínico'],
            ['codigo' => '225124', 'descricao' => '225124 - Médico pediatra'],
            ['codigo' => '225127', 'descricao' => '225127 - Médico pneumologista'],
            ['codigo' => '225280', 'descricao' => '225280 - Médico proctologista'],
            ['codigo' => '225133', 'descricao' => '225133 - Médico psiquiatra'],
            ['codigo' => '225330', 'descricao' => '225330 - Médico radioterapeuta'],
            ['codigo' => '225136', 'descricao' => '225136 - Médico reumatologista'],
            ['codigo' => '225139', 'descricao' => '225139 - Médico sanitarista'],
            ['codigo' => '225285', 'descricao' => '225285 - Médico urologista'],
            ['codigo' => '223204', 'descricao' => '223204 - Cirurgião dentista - auditor'],
            ['codigo' => '223208', 'descricao' => '223208 - Cirurgião dentista - clínico geral'],
            ['codigo' => '223212', 'descricao' => '223212 - Cirurgião dentista - endodontista'],
            ['codigo' => '223216', 'descricao' => '223216 - Cirurgião dentista - epidemiologista'],
            ['codigo' => '223220', 'descricao' => '223220 - Cirurgião dentista - estomatologista'],
            ['codigo' => '223224', 'descricao' => '223224 - Cirurgião dentista - implantodontista'],
            ['codigo' => '223228', 'descricao' => '223228 - Cirurgião dentista - odontogeriatra'],
            ['codigo' => '223232', 'descricao' => '223232 - Cirurgião dentista - odontologista legal'],
            ['codigo' => '223236', 'descricao' => '223236 - Cirurgião dentista - odontopediatra'],
            ['codigo' => '223240', 'descricao' => '223240 - Cirurgião dentista - ortopedista e ortodontista'],
            ['codigo' => '223244', 'descricao' => '223244 - Cirurgião dentista - patologista bucal'],
            ['codigo' => '223248', 'descricao' => '223248 - Cirurgião dentista - periodontista'],
            ['codigo' => '223252', 'descricao' => '223252 - Cirurgião dentista - protesiólogo bucomaxilofacial'],
            ['codigo' => '223256', 'descricao' => '223256 - Cirurgião dentista - protesista'],
            ['codigo' => '223260', 'descricao' => '223260 - Cirurgião dentista - radiologista'],
            ['codigo' => '223264', 'descricao' => '223264 - Cirurgião dentista - reabilitador oral'],
            ['codigo' => '223268', 'descricao' => '223268 - Cirurgião dentista - traumatologista bucomaxilofacial'],
            ['codigo' => '223272', 'descricao' => '223272 - Cirurgião dentista de saúde coletiva'],
            ['codigo' => '223505', 'descricao' => '223505 - Enfermeiro'],
            ['codigo' => '223605', 'descricao' => '223605 - Fisioterapeuta geral'],
            ['codigo' => '223910', 'descricao' => '223910 - Ortoptista'],
            ['codigo' => '223620', 'descricao' => '223620 - Peripatologista'],
            ['codigo' => '223905', 'descricao' => '223905 - Terapeuta ocupacional'],
            ['codigo' => '223710', 'descricao' => '223710 - Nutricionista'],
            ['codigo' => '223810', 'descricao' => '223810 - Fonoaudiólogo'],
            ['codigo' => '239425', 'descricao' => '239425 - Psicopedagogo'],
            ['codigo' => '251510', 'descricao' => '251510 - Psicólogo clínico'],
            ['codigo' => '251545', 'descricao' => '251545 - Neuropsicólogo'],
            ['codigo' => '251550', 'descricao' => '251550 - Psicanalista'],
            ['codigo' => '251605', 'descricao' => '251605 - Assistente social'],
            ['codigo' => '322205', 'descricao' => '322205 - Técnico de enfermagem'],
            ['codigo' => '322220', 'descricao' => '322220 - Técnico de enfermagem psiquiátrica'],
            ['codigo' => '322225', 'descricao' => '322225 - Instrumentador cirúrgico'],
            ['codigo' => '322230', 'descricao' => '322230 - Auxiliar de enfermagem'],
            ['codigo' => '516210', 'descricao' => '516210 - Cuidador de idosos'],
            ['codigo' => '225121', 'descricao' => '225121 - Médico oncologista clínico'],
            ['codigo' => '225325', 'descricao' => '225325 - Médico patologista'],
            ['codigo' => '223276', 'descricao' => '223276 - Cirurgião dentista - odontologia do trabalho'],
            ['codigo' => '223280', 'descricao' => '223280 - Cirurgião dentista - dentística'],
            ['codigo' => '223284', 'descricao' => '223284 - Cirurgião dentista - disfunção temporomandibular e dor orofacial'],
            ['codigo' => '223288', 'descricao' => '223288 - Cirurgião dentista - odontologia para pacientes com necessidades especiais'],
            ['codigo' => '223293', 'descricao' => '223293 - Cirurgião-dentista da estratégia de saúde da família'],
            ['codigo' => '225122', 'descricao' => '225122 - Médico cancerologista pediátrico'],
            ['codigo' => '225142', 'descricao' => '225142 - Médico da estratégia de saúde da família'],
            ['codigo' => '225151', 'descricao' => '225151 - Médico anestesiologista'],
            ['codigo' => '225203', 'descricao' => '225203 - Médico em cirurgia vascular'],
            ['codigo' => '225290', 'descricao' => '225290 - Médico cancerologista cirúrgico'],
            ['codigo' => '225295', 'descricao' => '225295 - Médico cirurgião da mão'],
            ['codigo' => '225335', 'descricao' => '225335 - Médico patologista clínico / medicina laboratorial'],
            ['codigo' => '225340', 'descricao' => '225340 - Médico hemoterapeuta'],
            ['codigo' => '225345', 'descricao' => '225345 - Médico hiperbarista'],
            ['codigo' => '223625', 'descricao' => '223625 - Fisioterapeuta respiratória'],
            ['codigo' => '223630', 'descricao' => '223630 - Fisioterapeuta neurofuncional'],
            ['codigo' => '223635', 'descricao' => '223635 - Fisioterapeuta traumato-ortopédica funcional'],
            ['codigo' => '223640', 'descricao' => '223640 - Fisioterapeuta osteopata'],
            ['codigo' => '223645', 'descricao' => '223645 - Fisioterapeuta quiropraxista'],
            ['codigo' => '223650', 'descricao' => '223650 - Fisioterapeuta acupunturista'],
            ['codigo' => '223655', 'descricao' => '223655 - Fisioterapeuta esportivo'],
            ['codigo' => '223660', 'descricao' => '223660 - Fisioterapeuta  do trabalho'],
            ['codigo' => '223435', 'descricao' => '223435 - Farmacêutico industrial'],
            ['codigo' => '223430', 'descricao' => '223430 - Farmacêutico em saúde pública'],
            ['codigo' => '223415', 'descricao' => '223415 - Farmacêutico analista clínico'],
            ['codigo' => '223405', 'descricao' => '223405 - Farmacêutico'],
            ['codigo' => '223420', 'descricao' => '223420 - Farmacêutico de alimentos'],
            ['codigo' => '223440', 'descricao' => '223440 - Farmacêutico toxicologista'],
            ['codigo' => '223445', 'descricao' => '223445 - Farmacêutico hospitalar e clínico'],
            ['codigo' => '223425', 'descricao' => '223425 - Farmacêutico práticas integrativas e complementares'],
            ['codigo' => '223565', 'descricao' => '223565 - Enfermeiro da estratégia de saúde da família'],
            ['codigo' => '223560', 'descricao' => '223560 - Enfermeiro sanitarista'],
            ['codigo' => '223555', 'descricao' => '223555 - Enfermeiro puericultor e pediátrico'],
            ['codigo' => '223550', 'descricao' => '223550 - Enfermeiro psiquiátrico'],
            ['codigo' => '223545', 'descricao' => '223545 - Enfermeiro obstétrico'],
            ['codigo' => '223540', 'descricao' => '223540 - Enfermeiro neonatologista'],
            ['codigo' => '223535', 'descricao' => '223535 - Enfermeiro nefrologista'],
            ['codigo' => '223530', 'descricao' => '223530 - Enfermeiro do trabalho'],
            ['codigo' => '223525', 'descricao' => '223525 - Enfermeiro de terapia intensiva'],
            ['codigo' => '223520', 'descricao' => '223520 - Enfermeiro de centro cirúrgico'],
            ['codigo' => '223515', 'descricao' => '223515 - Enfermeiro de bordo'],
            ['codigo' => '223510', 'descricao' => '223510 - Enfermeiro auditor'],
            ['codigo' => '223570', 'descricao' => '223570 - Perfusionista'],
            ['codigo' => '223705', 'descricao' => '223705 - Dietista'],
            ['codigo' => '223845', 'descricao' => '223845 - Fonoaudiólogo em voz'],
            ['codigo' => '223830', 'descricao' => '223830 - Fonoaudiólogo em linguagem'],
            ['codigo' => '223825', 'descricao' => '223825 - Fonoaudiólogo em disfagia'],
            ['codigo' => '223835', 'descricao' => '223835 - Fonoaudiólogo em motricidade orofacial'],
            ['codigo' => '223840', 'descricao' => '223840 - Fonoaudiólogo em saúde coletiva'],
            ['codigo' => '223820', 'descricao' => '223820 - Fonoaudiólogo em audiologia'],
            ['codigo' => '223815', 'descricao' => '223815 - Fonoaudiólogo educacional'],
            ['codigo' => '225154', 'descricao' => '225154 - Médico antroposófico'],
            ['codigo' => '221105', 'descricao' => '221105 - Biólogo'],
            ['codigo' => '999999', 'descricao' => '999999 - Desconhecido'],
        ]);
    }

    public static function isVisualizaGuia($estabelecimentoId)
    {
        return UsuarioResponsabilidadeEstabelecimento::isPossuiPerfilEstabelecimento($estabelecimentoId, array(
            ADM_PERFIL_LOCAL,
            ADM_PERFIL_GERAL,
            ADM_PERFIL_RECEPCIONISTA
        ));
    }

    public function isGuiaHabilitada($guiaId)
    {
        $sql = "SELECT 
                    COUNT(1) 
                FROM 
                    estabelecimentosaude.tb_lote_guia lgu
                INNER JOIN
                    estabelecimentosaude.tb_lote lot ON lgu.lote_id = lot.id AND lot.excluido IS FALSE
                INNER JOIN
                    workflow.documento doc ON lot.documento_id = doc.docid
                WHERE
                    lgu.excluido IS FALSE
                    AND
                    lgu.guia_id = {$guiaId}
                    AND
                    doc.esdid != " . Lote::WF_ESTADO_CORRECAO;

        $qtd = $this->pegaUm($sql);
        return intval($qtd) === 0;
    }

    public function validarById($guiaId)
    {
        $dadosGuia = $this->getTodosDados($guiaId);

        $mTipoGuia = new TipoGuia();
        $tipo = $mTipoGuia->getTipoById($dadosGuia['tipo_guia_id']);

        return $this->validar($dadosGuia['guia'], $tipo);
    }

    public function validar(array $guia, $tipo)
    {
        switch ($tipo) {
            case TipoGuia::TIPO_GUIA_EXAME :
                return $this->validarExame($guia);
                break;
            default :
                return $this->validarConsulta($guia);
                break;
        }
    }

    public function validarConsulta(array $guia)
    {
        $mValidacao = new GuiaValidacao();

        $mValidacao->validarObrigatorio($guia['registro_ans'], 'Registro ANS');
        $mValidacao->validarTamanhoMinimo($guia['registro_ans'], 'Registro ANS', 6);
        $mValidacao->validarTamanhoMaximo($guia['registro_ans'], 'Registro ANS', 6);
        $mValidacao->validarNumero($guia['registro_ans'], 'Registro ANS');

        $mValidacao->validarObrigatorio($guia['numero_guia_prestador'], 'Nº Guia Principal');
        $mValidacao->validarTamanhoMaximo($guia['numero_guia_prestador'], 'Nº Guia Principal', 20);

        $mValidacao->validarTamanhoMaximo($guia['numero_guia_operadora'], 'Nº Guia atribuído pela operadora', 20);

        $mValidacao->validarObrigatorio($guia['numero_carteira'], 'Número da Carteira');
        $mValidacao->validarTamanhoMaximo($guia['numero_carteira'], 'Número da Carteira', 20);

        $mValidacao->validarObrigatorio($guia['atendimento_rn'], 'Atendimento a RN');
        $mValidacao->validarDominio($guia['atendimento_rn'], 'Atendimento a RN', $this->getDominioRecemNascido());

        $mValidacao->validarObrigatorio($guia['nome_beneficiario'], 'Nome do Beneficiário');
        $mValidacao->validarTamanhoMaximo($guia['nome_beneficiario'], 'Nome do Beneficiário', 70);

        $mValidacao->validarTamanhoMaximo($guia['cartao_nacional_saude'], 'Cartão Nacional de Saúde', 15);

        $mValidacao->validarObrigatorio($guia['codigo_operadora'], 'Código na Operadora');
        $mValidacao->validarTamanhoMaximo($guia['codigo_operadora'], 'Código na Operadora', 14);

        $mValidacao->validarObrigatorio($guia['nome_contratado'], 'Nome do Contratado');
        $mValidacao->validarTamanhoMaximo($guia['nome_contratado'], 'Nome do Contratado', 70);

        $mValidacao->validarObrigatorio($guia['cnes'], 'Código CNES');
        $mValidacao->validarTamanhoMaximo($guia['cnes'], 'Código CNES', 7);

        $mValidacao->validarObrigatorio($guia['nome_profissional'], 'Nome do Profissional Executante');
        $mValidacao->validarTamanhoMaximo($guia['nome_profissional'], 'Nome do Profissional Executante', 70);

        $mValidacao->validarObrigatorio($guia['conselho_profissional'], 'Conselho Profissional');
        $mValidacao->validarDominio($guia['conselho_profissional'], 'Conselho Profissional', $this->getDominioConselho());

        $mValidacao->validarObrigatorio($guia['numero_conselho_profissional'], 'Nº Conselho Profissional');
        $mValidacao->validarTamanhoMaximo($guia['numero_conselho_profissional'], 'Nº Conselho Profissional', 15);

        $mValidacao->validarObrigatorio($guia['uf_conselho_profissional'], 'UF do Conselho Profissional');
        $mValidacao->validarDominio($guia['uf_conselho_profissional'], 'UF do Conselho Profissional', $this->getDominioUf());

        $mValidacao->validarObrigatorio($guia['cbos'], 'CBOS');
        $mValidacao->validarDominio($guia['cbos'], 'CBOS', $this->getDominioCBO());

        $mValidacao->validarObrigatorio($guia['indicacao_acidente'], 'Indicação de Aciente');
        $mValidacao->validarDominio($guia['indicacao_acidente'], 'Indicação de Aciente', $this->getDominioIndicacaoAcidente());

        $mValidacao->validarObrigatorio($guia['data_atendimento'], 'Data do Atendimento');
        $mValidacao->validarData($guia['data_atendimento'], 'Data do Atendimento');

        $mValidacao->validarObrigatorio($guia['tipo_consulta'], 'Tipo de Consulta');
        $mValidacao->validarDominio($guia['tipo_consulta'], 'Tipo de Consulta', $this->getDominioTipoConsulta());

        $mValidacao->validarObrigatorio($guia['codigo_tabela'], 'Tabela de Procedimentos');
        $mValidacao->validarDominio($guia['codigo_tabela'], 'Tabela de Procedimentos', $this->getDominioTipoTabela());

        $mValidacao->validarObrigatorio($guia['codigo_procedimento'], 'Código do Procedimento');
        $mValidacao->validarTamanhoMaximo($guia['codigo_procedimento'], 'Código do Procedimento', 10);

        $mValidacao->validarObrigatorio($guia['valor_procedimento'], 'Valor do Procedimento');
        $mValidacao->validarDinheiro($guia['valor_procedimento'], 'Valor do Procedimento');

        $mValidacao->validarTamanhoMaximo($guia['observacao'], 'Observação', 500);

        return $mValidacao->getErros();
    }

    public function validarExame(array $guia)
    {
        $mValidacao = new GuiaValidacao();

        $mValidacao->validarObrigatorio($guia['registro_ans'], 'Registro ANS');
        $mValidacao->validarTamanhoMinimo($guia['registro_ans'], 'Registro ANS', 6);
        $mValidacao->validarTamanhoMaximo($guia['registro_ans'], 'Registro ANS', 6);
        $mValidacao->validarNumero($guia['registro_ans'], 'Registro ANS');

        $mValidacao->validarTamanhoMaximo($guia['numero_guia_principal'], 'Nº Guia Principal', 20);

        $mValidacao->validarObrigatorio($guia['numero_guia_prestador'], 'Nº Guia Principal');
        $mValidacao->validarTamanhoMaximo($guia['numero_guia_prestador'], 'Nº Guia Principal', 20);

        $mValidacao->validarTamanhoMaximo($guia['numero_guia_operadora'], 'Nº Guia atribuído pela operadora', 20);

        $mValidacao->validarData($guia['data_autorizacao'], 'Data da Autorização');

        $mValidacao->validarTamanhoMaximo($guia['senha'], 'Senha de Autorização', 20);

        $mValidacao->validarData($guia['data_validade_senha'], 'Data de Validade da Senha');

        $mValidacao->validarObrigatorio($guia['numero_carteira'], 'Número da Carteira');
        $mValidacao->validarTamanhoMaximo($guia['numero_carteira'], 'Número da Carteira', 20);

        $mValidacao->validarObrigatorio($guia['atendimento_rn'], 'Atendimento a RN');
        $mValidacao->validarDominio($guia['atendimento_rn'], 'Atendimento a RN', $this->getDominioRecemNascido());

        $mValidacao->validarObrigatorio($guia['nome_beneficiario'], 'Nome do Beneficiário');
        $mValidacao->validarTamanhoMaximo($guia['nome_beneficiario'], 'Nome do Beneficiário', 70);

        $mValidacao->validarTamanhoMaximo($guia['cartao_nacional_saude'], 'Cartão Nacional de Saúde', 15);

        $mValidacao->validarObrigatorio($guia['codigo_operadora'], 'Código na Operadora');
        $mValidacao->validarTamanhoMaximo($guia['codigo_operadora'], 'Código na Operadora', 14);

        $mValidacao->validarObrigatorio($guia['nome_contratado'], 'Nome do Contratado');
        $mValidacao->validarTamanhoMaximo($guia['nome_contratado'], 'Nome do Contratado', 70);

        $mValidacao->validarObrigatorio($guia['nome_profissional'], 'Nome do Profissional Executante');
        $mValidacao->validarTamanhoMaximo($guia['nome_profissional'], 'Nome do Profissional Executante', 70);

        $mValidacao->validarObrigatorio($guia['conselho_profissional'], 'Conselho Profissional');
        $mValidacao->validarDominio($guia['conselho_profissional'], 'Conselho Profissional', $this->getDominioConselho());

        $mValidacao->validarObrigatorio($guia['numero_conselho_profissional'], 'Nº Conselho Profissional');
        $mValidacao->validarTamanhoMaximo($guia['numero_conselho_profissional'], 'Nº Conselho Profissional', 15);

        $mValidacao->validarObrigatorio($guia['uf_conselho_profissional'], 'UF do Conselho Profissional');
        $mValidacao->validarDominio($guia['uf_conselho_profissional'], 'UF do Conselho Profissional', $this->getDominioUf());

        $mValidacao->validarObrigatorio($guia['cbos'], 'CBOS');
        $mValidacao->validarDominio($guia['cbos'], 'CBOS', $this->getDominioCBO());

        $mValidacao->validarData($guia['data_solicitacao'], 'Data da Solicitação');

        $mValidacao->validarObrigatorio($guia['carater_atendimento'], 'Caráter do Atendimento');
        $mValidacao->validarDominio($guia['carater_atendimento'], 'Caráter do Atendimento', $this->getDominioCaraterAtendimento());

        $mValidacao->validarTamanhoMaximo($guia['indicacao_clinica'], 'Indicação Clínica', 500);

        $mValidacao->validarObrigatorio($guia['codigo_operadora_executante'], 'Código na Operadora do Executante');
        $mValidacao->validarTamanhoMaximo($guia['codigo_operadora_executante'], 'Código na Operadora do Executante', 14);

        $mValidacao->validarObrigatorio($guia['nome_contratado_executante'], 'Nome do Contratado');
        $mValidacao->validarTamanhoMaximo($guia['nome_contratado_executante'], 'Nome do Contratado', 70);

        $mValidacao->validarObrigatorio($guia['cnes'], 'Nome do Contratado');
        $mValidacao->validarTamanhoMaximo($guia['cnes'], 'Nome do Contratado', 7);

        $mValidacao->validarObrigatorio($guia['tipo_atendimento'], 'Tipo de Atendimento');
        $mValidacao->validarDominio($guia['tipo_atendimento'], 'Tipo de Atendimento', $this->getDominioTipoAtendimento());

        $mValidacao->validarObrigatorio($guia['indicacao_acidente'], 'Indicação de Acidente');
        $mValidacao->validarDominio($guia['indicacao_acidente'], 'Indicação de Acidente', $this->getDominioIndicacaoAcidente());

        $mValidacao->validarDominio($guia['tipo_consulta'], 'Tipo de Consulta', $this->getDominioTipoConsulta());

        $mValidacao->validarDominio($guia['motivo_encerramento'], 'Motivo de Encerramento', $this->getDominioMotivoEncerramento());

        $mValidacao->validarObrigatorio($guia['executado'], 'Dados da Execução / Procedimentos e Exames Realizados');

        if(is_array($guia['executado'])) {
            foreach ($guia['executado'] as $index => $executado) {
                $sequencial = $index + 1;

                $mValidacao->validarObrigatorio($executado['data_execucao'], "Executado: {$sequencial} - Data");
                $mValidacao->validarData($executado['data_execucao'], "Executado: {$sequencial} - Data");

                $mValidacao->validarHora($executado['hora_inicial_execucao'], "Executado: {$sequencial} - Hora Inicial");
                $mValidacao->validarHora($executado['hora_final_execucao'], "Executado: {$sequencial} - Hora Final");

                $mValidacao->validarObrigatorio($executado['codigo_tabela_execucao'], "Executado: {$sequencial} - Tabela de Procedimentos");
                $mValidacao->validarDominio($executado['codigo_tabela_execucao'], "Executado: {$sequencial} - Tabela de Procedimentos", $this->getDominioTipoTabela());

                $mValidacao->validarObrigatorio($executado['codigo_procedimento_execucao'], "Executado: {$sequencial} - Código do Procedimento");
                $mValidacao->validarTamanhoMaximo($executado['codigo_procedimento_execucao'], "Executado: {$sequencial} - Código do Procedimento", 10);

                $mValidacao->validarObrigatorio($executado['descricao_execucao'], "Executado: {$sequencial} - Descrição do Procedimento");
                $mValidacao->validarTamanhoMaximo($executado['descricao_execucao'], "Executado: {$sequencial} - Descrição do Procedimento", 150);

                $mValidacao->validarObrigatorio($executado['qtd_execucao'], "Executado: {$sequencial} - Quantidade");
                $mValidacao->validarNumero($executado['qtd_execucao'], "Executado: {$sequencial} - Quantidade");
                $mValidacao->validarTamanhoMaximo($executado['qtd_execucao'], "Executado: {$sequencial} - Quantidade", 3);

                $mValidacao->validarDominio($executado['via'], "Executado: {$sequencial} - Via", $this->getDominioViaAcesso());

                $mValidacao->validarDominio($executado['tec'], "Executado: {$sequencial} - Técnica Utilizada", $this->getDominioTecnicaUtilizada());

                $mValidacao->validarObrigatorio($executado['fator_red_acresc'], "Executado: {$sequencial} - Redução/Acréscimo");
                $mValidacao->validarNumero($executado['fator_red_acresc'], "Executado: {$sequencial} - Redução/Acréscimo");

                $mValidacao->validarObrigatorio($executado['valor_unitario'], "Executado: {$sequencial} - Valor Unitário");
                $mValidacao->validarNumero($executado['valor_unitario'], "Executado: {$sequencial} - Valor Unitário");

                $mValidacao->validarObrigatorio($executado['valor_total'], "Executado: {$sequencial} - Valor Total");
                $mValidacao->validarNumero($executado['valor_total'], "Executado: {$sequencial} - Valor Total");
            }
        }

        $mValidacao->validarTamanhoMaximo($guia['observacao'], 'Observação / Justificativa', 500);

        $mValidacao->validarNumero($guia['total_procedimentos'], 'Total de Procedimentos');

        $mValidacao->validarNumero($guia['total_taxas_alugueis'], 'Total de Taxas e Aluguéis');

        $mValidacao->validarNumero($guia['total_materiais'], 'Total de Materiais');

        $mValidacao->validarNumero($guia['total_opme'], 'Total de OPME');

        $mValidacao->validarNumero($guia['total_gases_medicinais'], 'Total de Gases Medicinais');

        $mValidacao->validarNumero($guia['total_medicamentos'], 'Total de Medicamentos');

        $mValidacao->validarObrigatorio($guia['total_geral'], 'Total Geral');
        $mValidacao->validarNumero($guia['total_geral'], 'Total Geral');

        return $mValidacao->getErros();
    }


}