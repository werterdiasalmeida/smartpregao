<?php

class EstabelecimentoAgenda extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_estabelecimento_agenda";

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
        'estabelecimento_id' => null,
        'inicio' => null,
        'fim' => null,
    );

    public function salvarAgenda($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);
        $data = formata_data_sql($params['data_agenda']);
        $horaInicio = str_pad($params['hora_inicio'], 5, '0', STR_PAD_LEFT) . ':00';
        $horaFim = str_pad($params['hora_fim'], 5, '0', STR_PAD_LEFT) . ':00';

        $dados = array(
            'id' => $params['id'],
            'estabelecimento_id' => $params['estabelecimento_id'],
            'editalcliente_id' => $params['editalcliente_id'],
            'inicio' => "{$data} {$horaInicio}",
            'fim' => "{$data} {$horaFim}",
        );
        
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }


    public function salvarHorariosAgenda($params)
    {
        if(is_array($params['hora_inicio'])){
            foreach ($params['hora_inicio'] as $i => $horaInicio){
                if(empty($horaInicio) || !isset($params['hora_fim'][$i])
                    || empty($params['hora_fim'][$i])){
                    continue;
                }

                $agenda = $params;
                $agenda['hora_inicio'] = $horaInicio;
                $agenda['hora_fim'] = $params['hora_fim'][$i];

                $this->salvarAgenda($agenda);
                $this->setDadosNull();
            }
        }
    }

    public function getList($arWhere = array(), $paginacao = true, $order = null, $limit = null, $offset = null)
    {
        $arWhere[] = "epa.excluido IS FALSE";
        $sqlLimit = $limit ? "LIMIT {$limit}" : '';
        $sqlOffset = $offset ? "OFFSET {$offset}" : '';

        $sql = "SELECT
                  epa.inicio,
                  epa.fim,
                  p.nome_completo,
                  CASE 
                  WHEN p.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                  THEN tp.pron_trat_masc || ' ' || p.nome_completo
                  WHEN p.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                  THEN tp.pron_trat_fem || ' ' || p.nome_completo
                  ELSE p.nome_completo
                  END AS nome_completo_medico,
                  tp.descricao AS tipo,
                  e.descricao AS especialidade,
                  est.esddsc AS estado,
                  p.nome_completo AS profissional,
                  epa.inicio AS inicio_orig,
                  TO_CHAR(epa.inicio, 'HH24:mi') AS hora_inicio,
                  TO_CHAR(epa.fim, 'HH24:mi') AS hora_fim,
                  TO_CHAR(epa.inicio, 'DD/MM/YYYY') AS data_agenda,
                  TO_CHAR(epa.inicio, 'YYYYMMDD') AS data_agrupador,
                  p.sexo,
                  tc.descricao AS tipo_consulta,
                  est.esddsc AS estado,
                  sae.sala,
                  est.esdid AS estado_id,
                  pro.id AS profissional_id,
                  p.id AS pessoa_id,
                  pac.id AS paciente_id,
                  pac.nome_completo AS paciente_nome_completo,
                  pac.foto_arquivo_id AS paciente_foto,
                  pac.sexo AS paciente_sexo,
                  TO_CHAR(pac.data_nascimento, 'DD/MM/YYYY') AS paciente_data_nascimento,
                  extract(year from age(pac.data_nascimento)) AS paciente_idade,
                  ep.especialidade_id,
                  eep.estabelecimento_id,
                  epa.sala_estabelecimento_id,
                  con.nome AS convenio,
                  epa.encaixe,
                  epa.valor_procedimento,
                  proc.descricao AS procedimento,
                  tco.descricao AS tipo_consulta,
                  est.esdid AS estado_codigo,
                  epa.documento_id,
                  tcp.telefone_celular_paciente,
                  tfp.telefone_paciente,
                  epa.id
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                                                                    AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_profissional pro ON ep.profissional_id = pro.id
                INNER JOIN
                  corporativo.tb_pessoafisica p ON pro.pessoafisica_id = p.id
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                INNER JOIN
                  profissionalsaude.tb_especialidade e ON ep.especialidade_id = e.id
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                INNER JOIN
                  workflow.estadodocumento est ON doc.esdid = est.esdid
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento es ON eep.estabelecimento_id = es.id
                LEFT JOIN 
                  corporativo.tb_pessoafisica pac ON epa.pessoa_paciente_id = pac.id
                LEFT JOIN
                  estabelecimentosaude.tb_sala_estabelecimento sae ON epa.sala_estabelecimento_id = sae.id
                LEFT JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento con ON epa.convenio_id = con.id
                LEFT JOIN
                  estabelecimentosaude.tb_tipo_consulta tc ON epa.tipo_consulta_id = tc.id
                LEFT JOIN 
                  estabelecimentosaude.tb_valor_procedimento vlr ON epa.valor_procedimento_id = vlr.id
                LEFT JOIN
                  estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON vlr.tabela_procedimento_convenio_id = tpc.id  
                LEFT JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                LEFT JOIN
                  estabelecimentosaude.tb_procedimento proc ON vlr.procedimento_id = proc.id
                LEFT JOIN
                  estabelecimentosaude.tb_tipo_consulta tco ON epa.tipo_consulta_id = tco.id
                LEFT JOIN
                  (SELECT 
                      cpj.contato AS telefone_celular_paciente,
                      cpj.pessoafisica_id AS paciente_id
                    FROM
                        corporativo.tb_contato_pessoafisica cpj
                    JOIN
                        corporativo.tb_tipo_contato tc ON tc.id = cpj.tipo_contato_id
                    WHERE
                        cpj.excluido IS FALSE
                        AND
                        tc.sigla = '".TipoContato::TIPO_CELULAR."') tcp ON tcp.paciente_id = pac.id
                LEFT JOIN
                  (SELECT 
                      cpj.contato AS telefone_paciente,
                      cpj.pessoafisica_id AS paciente_id
                    FROM
                        corporativo.tb_contato_pessoafisica cpj
                    JOIN
                        corporativo.tb_tipo_contato tc ON tc.id = cpj.tipo_contato_id
                    WHERE
                        cpj.excluido IS FALSE
                        AND
                        tc.sigla = '".TipoContato::TIPO_TELEFONE_FIXO."') tfp ON tfp.paciente_id = pac.id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY epa.inicio";
            }
        }

        $sql .= " {$sqlLimit} {$sqlOffset} ";

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }
    public function getListPassiveisEstorno($arWhere = array(), $paginacao = true, $order = null, $limit = null, $offset = null)
    {
        $arWhere[] = "epa.excluido IS FALSE";
        $arWhere[] = "doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA;

        $dataMaxima = new DateTime(date('Y-m-d'));
        $dataMaxima->sub(new DateInterval('P7D'));
        $arWhere[] = " epa.fim::date >= '{$dataMaxima->format('Y-m-d')}'::date ";

        $sqlLimit = $limit ? "LIMIT {$limit}" : '';
        $sqlOffset = $offset ? "OFFSET {$offset}" : '';

        $sql = "SELECT
                  pj.razao_social,
                  e.descricao as especialidade,
                  pac.nome_completo AS paciente_nome_completo,
                  CASE 
                    WHEN p.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                      THEN tp.pron_trat_masc || ' ' || p.nome_completo
                    WHEN p.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                      THEN tp.pron_trat_fem || ' ' || p.nome_completo
                    ELSE p.nome_completo
                  END AS nome_completo_medico,
                  TO_CHAR(epa.inicio, 'DD/MM/YYYY') AS data_agenda,
                  TO_CHAR(epa.inicio, 'HH24:mi') AS hora_inicio,
                  tc.descricao AS tipo_consulta,
                  proc.descricao AS procedimento,
                  sae.sala,
                  epa.id,
                  es.id AS estabelecimento_id
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                                                                    AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_profissional pro ON ep.profissional_id = pro.id
                INNER JOIN
                  corporativo.tb_pessoafisica p ON pro.pessoafisica_id = p.id
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                INNER JOIN
                  profissionalsaude.tb_especialidade e ON ep.especialidade_id = e.id
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                INNER JOIN
                  workflow.estadodocumento est ON doc.esdid = est.esdid
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento es ON eep.estabelecimento_id = es.id
                LEFT JOIN 
                  corporativo.tb_pessoajuridica pj ON es.pessoajuridica_id = pj.id
                LEFT JOIN 
                  corporativo.tb_pessoafisica pac ON epa.pessoa_paciente_id = pac.id
                LEFT JOIN
                  estabelecimentosaude.tb_sala_estabelecimento sae ON epa.sala_estabelecimento_id = sae.id
                LEFT JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento con ON epa.convenio_id = con.id
                LEFT JOIN
                  estabelecimentosaude.tb_tipo_consulta tc ON epa.tipo_consulta_id = tc.id
                LEFT JOIN 
                  estabelecimentosaude.tb_valor_procedimento vlr ON epa.valor_procedimento_id = vlr.id
                LEFT JOIN
                  estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON vlr.tabela_procedimento_convenio_id = tpc.id  
                LEFT JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                LEFT JOIN
                  estabelecimentosaude.tb_procedimento proc ON vlr.procedimento_id = proc.id
                LEFT JOIN
                  estabelecimentosaude.tb_tipo_consulta tco ON epa.tipo_consulta_id = tco.id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY epa.inicio";
            }
        }

        $sql .= " {$sqlLimit} {$sqlOffset} ";

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function getListCalendario($agendas, $jsonEncode = false)
    {
        $corEventos = $this->getCoresEventoPorEstado();
        $iconeEventos = $this->getIconesEventoPorEstado();

        $lista = array_map(function($linha) use($corEventos, $iconeEventos, $jsonEncode){
            $tituloEvento = "{$linha['nome_completo_medico']} - {$linha['especialidade']}";

            $encaixe = $linha['encaixe'] && $linha['encaixe'] === 't' ? ' - Encaixe' : '';

            $descricao = "<div class='row' style='margin-bottom: 15px;'>
                              <div class='col-sm-6'>
                                  <img src='{$iconeEventos[$linha['estado_id']]}' style='width: 25px; height: 25px;' />
                                  {$linha['estado']}
                                  {$encaixe}
                              </div>
                              <div class='col-sm-6 text-right'>
                                  <strong>{$linha['data_agenda']} {$linha['hora_inicio']}</strong>
                              </div>
                          </div>";

            if($linha['paciente_nome_completo']){
                $tituloEvento = "{$linha['paciente_nome_completo']} - {$linha['especialidade']}";
                $descricao .= "<div class='row'>
                                    <div class='col-sm-4 text-right'>
                                        " . montaImgFoto($linha['paciente_foto'], $linha['paciente_sexo'], '80px', '', '', '', 'img-circle bg-white', true) . "
                                    </div>
                                    
                                    <div class='col-sm-8 text-left'>
                                        <div class='row'>
                                            <div class='col-sm-12'>
                                                <strong>{$linha['paciente_nome_completo']}</strong>
                                            </div>
                                            <div class='col-sm-12'>
                                                <strong>{$linha['paciente_data_nascimento']}</strong>
                                            </div>
                                            <div class='col-sm-12'>
                                                <strong>{$linha['paciente_idade']} <small>anos</small></strong>
                                            </div>
                                        </div>
                                    </div>
                                </div>";
            }

            $descricao .= "<div class='row'>
                               <div class='col-sm-4 text-right'>
                                   <label>Profissional:</label>
                               </div>
                               <div class='col-sm-8'>
                                   <strong>{$linha['nome_completo_medico']}</strong>
                               </div>
                           </div>";

            if($linha['sala']){
                $descricao .= "<div class='row'>
                                   <div class='col-sm-4 text-right'>
                                       <label>Sala:</label>
                                   </div>
                                   <div class='col-sm-8'>
                                       <strong>{$linha['sala']}</strong>
                                   </div>
                               </div>";
            }

            if($linha['tipo_consulta']){
                $descricao .= "<div class='row'>
                                   <div class='col-sm-4 text-right'>
                                       <label>Tipo de serviço:</label>
                                   </div>
                                   <div class='col-sm-8'>
                                       <strong>{$linha['tipo_consulta']}</strong>
                                   </div>
                               </div>";
            }

            if($linha['procedimento']){
                $descricao .= "<div class='row'>
                                   <div class='col-sm-4 text-right'>
                                       <label>Procedimento:</label>
                                   </div>
                                   <div class='col-sm-8'>
                                       <strong>{$linha['procedimento']}</strong>
                                   </div>
                               </div>";
            }


            if($linha['convenio'] && ConvenioEstabelecimento::isVisualizaConvenio($linha['estabelecimento_id'])){
                $descricao .= "<div class='row'>
                                   <div class='col-sm-4 text-right'>
                                       <label>Convênio:</label>
                                   </div>
                                   <div class='col-sm-8'>
                                       <strong>{$linha['convenio']}</strong>
                                   </div>
                               </div>";
            }

            if($linha['valor_procedimento'] && ValorProcedimento::isVisualizaValores($linha['estabelecimento_id'])){
                $descricao .= "<div class='row'>
                                   <div class='col-sm-4 text-right'>
                                       <label>Valor:</label>
                                   </div>
                                   <div class='col-sm-8'>
                                       <strong>R$ ". formata_valor($linha['valor_procedimento']) ."</strong>
                                   </div>
                               </div>";
            }

            if(in_array($linha['estado_id'], array(self::WF_ESTADO_DISPONIVEL, self::WF_ESTADO_BLOQUEADA))){
                $tituloEvento = $linha['estado'];
            }

            $proximoEstadoWf = wf_pegarProximosEstados($linha['documento_id'], array('id' => $linha['id']));

            $dadosRetorno = array(
                'title' => $tituloEvento . $encaixe,
                'dsc' => $descricao,
                'start' => $linha['inicio'],
                'end' => $linha['fim'],
                'hora' => $linha['hora_inicio'] . ' - ' . $linha['hora_fim'],
                'hora_inicio' => $linha['hora_inicio'],
                'tipo_consulta' => $linha['tipo_consulta'] ? $linha['tipo_consulta'] : '-',
                'tipo_sigla' => $linha['tipo_sigla'],
                'sala' => $linha['sala'] ? $linha['sala'] : '-',
                'estado' => $linha['estado'],
                'color' => $corEventos[$linha['estado_id']],
                'icone' => $iconeEventos[$linha['estado_id']],
                'p_id' => $linha['profissional_id'],
                'pes_id' => $linha['pessoa_id'],
                'pac_id' => $linha['paciente_id'],
                'esd_id' => $linha['estado_id'],
                'next_esd_id' => $proximoEstadoWf,
                'documento_id' => $linha['documento_id'],
                'esp_id' => $linha['especialidade_id'],
                'data_agrupador' => $linha['data_agrupador'],
                'paciente' => $linha['paciente_nome_completo'] . $encaixe,
                'telefone_paciente' => implode(array_filter([$linha['telefone_paciente'], $linha['telefone_celular_paciente']]), '<br>') ?: '',
                'nome_completo_medico' => $linha['nome_completo_medico'],
                'convenio' => $linha['convenio'] ?: '',
                'marcar_consulta' => $linha['estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL && strtotime($linha['inicio_orig']) >= strtotime(date('Y-m-d H:i:s')),
                'a_id' => $linha['id'],
                'p' => setParam(array(
                    'agenda_id' => $linha['id'],
                    'id' => $linha['estabelecimento_id'],
                ), false)
            );

            return $jsonEncode ? utf8ArrayEncode($dadosRetorno): $dadosRetorno;
        }, $agendas);

        return $jsonEncode ? json_encode($lista): $lista;
    }

    public function getTodosDados($id)
    {
        $sql = "SELECT DISTINCT
                  epa.inicio,
                  epa.inicio::date AS data_inicio,
                  epa.inicio::time AS hora_inicio,
                  epa.fim,
                  epa.fim::date AS data_fim,
                  epa.fim::time AS hora_fim,
                  epa.tipo_consulta_id,
                  epa.encaixe,
                  epa.duracao_consulta,
                  e.descricao AS especialidade,
                  CASE 
                  WHEN p.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                  THEN tp.pron_trat_masc || ' ' || p.nome_completo
                  WHEN p.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                  THEN tp.pron_trat_fem || ' ' || p.nome_completo
                  ELSE p.nome_completo
                  END AS profissional,
                  epa.convenio_id,
                  epa.documento_id,
                  ep.profissional_id,
                  p.id AS pessoa_id,
                  p.cpf AS cpf_profissional,
                  p.sexo AS sexo_profissional,
                  p.foto_arquivo_id AS foto_arquivo_id_profissional,
                  ep.especialidade_id,
                  epa.estabelecimento_especialidade_profissional_id,
                  esd.esdid AS wf_estado_id,
                  esd.esddsc AS wf_estado,
                  epa.pessoa_paciente_id,
                  epa.sala_estabelecimento_id,
                  coe.nome AS convenio,
                  epa.valor_procedimento_id,
                  epa.valor_procedimento,
                  proc.descricao AS procedimento,
                  tco.descricao AS tipo_consulta,
                  eep.estabelecimento_id,
                  pep.id AS profissional_estabelecimento_procedimento_id,
                  paciente.nome_completo AS paciente,
                  epa.id AS estabelecimento_profissional_agenda_id,
                  aa.nota AS nota_avaliacao,
                  aa.id AS avaliacao_id,
                  aa.melhorias AS melhorias_avaliacao,
                  aa.comentario AS comentario_avaliacao,
                  cap.numero AS numero_convenio_plano,
                  epa.valor_profissional,
                  epa.percentual_profissional,
                  sae.sala,
                  paciente.cpf as cpf_paciente
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                        /*AND eep.excluido IS FALSE*/
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                                                                    /*AND ep.excluido IS FALSE*/
                INNER JOIN
                  profissionalsaude.tb_profissional pro ON ep.profissional_id = pro.id
                INNER JOIN
                  corporativo.tb_pessoafisica p ON pro.pessoafisica_id = p.id
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                INNER JOIN
                  profissionalsaude.tb_especialidade e ON ep.especialidade_id = e.id
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                INNER JOIN
                  workflow.estadodocumento esd ON doc.esdid = esd.esdid
                LEFT JOIN 
                  estabelecimentosaude.tb_valor_procedimento vlr ON epa.valor_procedimento_id = vlr.id
                LEFT JOIN
                  estabelecimentosaude.tb_tabela_procedimento_convenio tpc ON vlr.tabela_procedimento_convenio_id = tpc.id  
                LEFT JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento coe ON tpc.convenio_id = coe.id
                LEFT JOIN
                  estabelecimentosaude.tb_procedimento proc ON vlr.procedimento_id = proc.id
                LEFT JOIN
                  estabelecimentosaude.tb_tipo_consulta tco ON epa.tipo_consulta_id = tco.id
                LEFT JOIN
                  estabelecimentosaude.tb_sala_estabelecimento sae ON epa.sala_estabelecimento_id = sae.id
                LEFT JOIN
                  estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep ON pep.procedimento_id = proc.id 
                                                                                          AND pep.profissional_id = pro.id
                                                                                          AND pep.estabelecimento_id = eep.estabelecimento_id
                                                                                          /*AND pep.excluido IS FALSE*/
                LEFT JOIN
                  corporativo.tb_pessoafisica paciente ON epa.pessoa_paciente_id = paciente.id
                LEFT JOIN
                  estabelecimentosaude.tb_avaliacao_atendimento aa ON aa.agenda_id = epa.id 
                                                                      /*AND aa.excluido IS FALSE*/
                LEFT JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento_paciente cap ON coe.id = cap.convenio_estabelecimento_id 
                                                                               AND cap.pessoa_paciente_id = epa.pessoa_paciente_id
                                                                               /*AND cap.excluido IS FALSE*/
                WHERE
                  epa.id = {$id}";

        return $this->pegaLinha($sql);
    }

    /**
     * @param $id
     * @return array|false
     */
    public function getDadosPainelSenha($id)
    {
        $sql = "SELECT DISTINCT
                  paciente.nome_completo,
                  sae.sala,
                  epa.sala_estabelecimento_id,
                  epa.id AS estabelecimento_profissional_agenda_id
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa               
                LEFT JOIN
                  estabelecimentosaude.tb_sala_estabelecimento sae ON epa.sala_estabelecimento_id = sae.id
                LEFT JOIN
                  corporativo.tb_pessoafisica paciente ON epa.pessoa_paciente_id = paciente.id
                WHERE
                  epa.id = {$id}";

        return $this->pegaLinha($sql);
    }

    public function isHorarioDisponivel($estabelecimentoId, $profissionalId, $dataAgenda, $horaInicio, $horaFim, $agendaId = null)
    {
        $dataAgenda = formata_data_sql($dataAgenda);
        $horaInicio = str_pad($horaInicio, 5, '0', STR_PAD_LEFT) . ':00';
        $horaFim = str_pad($horaFim, 5, '0', STR_PAD_LEFT) . ':00';

        $andId = $agendaId ? " AND epa.id != {$agendaId}" : '';

        $sql = "SELECT DISTINCT
                  count(*)
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                                                                    AND ep.excluido IS FALSE
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                WHERE
                  eep.estabelecimento_id = {$estabelecimentoId}
                  AND
                  ep.profissional_id = {$profissionalId}
                  AND
                  doc.esdid NOT IN (" . implode(', ', [self::WF_ESTADO_CANCELADA, self::WF_ESTADO_REAGENDADA]) . ")
                  AND
                  epa.encaixe IS FALSE
                  AND
                  epa.excluido IS FALSE
                  AND  (
                    (tsrange(epa.inicio::timestamp, epa.fim::timestamp) * tsrange('{$dataAgenda} {$horaInicio}'::timestamp, '{$dataAgenda} {$horaFim}'::timestamp)) != 'empty'
                    /*OR
                    ('{$dataAgenda} {$horaInicio}' BETWEEN epa.inicio AND epa.fim)
                    OR
                    ('{$dataAgenda} {$horaFim}' BETWEEN epa.inicio AND epa.fim)*/
                  )
                  {$andId}";

        return intval($this->pegaUm($sql)) === 0;
    }

    public function isHorarioSalaDisponivel($estabelecimentoId, $salaId, $dataAgenda, $horaInicio, $horaFim, $agendaId = null)
    {
        $dataAgenda = formata_data_sql($dataAgenda);
        $horaInicio = str_pad($horaInicio, 5, '0', STR_PAD_LEFT) . ':00';
        $horaFim = str_pad($horaFim, 5, '0', STR_PAD_LEFT) . ':00';

        $andId = $agendaId ? " AND epa.id != {$agendaId}" : '';

        $sql = "SELECT DISTINCT
                  count(*)
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                WHERE
                  eep.estabelecimento_id = {$estabelecimentoId}
                  AND
                  epa.sala_estabelecimento_id = {$salaId}
                  AND
                  doc.esdid NOT IN (" . implode(', ', [self::WF_ESTADO_CANCELADA, self::WF_ESTADO_REAGENDADA]) . ")
                  AND
                  epa.encaixe IS FALSE
                  AND
                  epa.excluido IS FALSE
                  AND  (
                    (tsrange(epa.inicio::timestamp, epa.fim::timestamp) * tsrange('{$dataAgenda} {$horaInicio}'::timestamp, '{$dataAgenda} {$horaFim}'::timestamp)) != 'empty'
                    /*OR
                    ('{$dataAgenda} {$horaInicio}' BETWEEN epa.inicio AND epa.fim)
                    OR
                    ('{$dataAgenda} {$horaFim}' BETWEEN epa.inicio AND epa.fim)*/
                  )
                  {$andId}";

        return intval($this->pegaUm($sql)) === 0;
    }

    public function getCoresEventoPorEstado()
    {
        return array(
            self::WF_ESTADO_DISPONIVEL => '#3AC386',
            self::WF_ESTADO_AGENDADA => '#E37B2F',
            self::WF_ESTADO_REAGENDADA => '#ACA020',
            self::WF_ESTADO_CONFIRMADA => '#0D746B',
            self::WF_ESTADO_REALIZADA => '#1CAEEC',
            self::WF_ESTADO_CANCELADA => '#790026',
            self::WF_ESTADO_NAO_REALIZADA => '#FB0019',
            self::WF_ESTADO_BLOQUEADA => '#94A0AF',
            self::WF_ESTADO_EM_ATENDIMENTO => '#008fa9',
            self::WF_ESTADO_AGUARDANDO_ATENDIMENTO => '#662e91',
            self::WF_ESTADO_ESTORNADA => '#462727',
        );
    }

    public function getIconesEventoPorEstado()
    {
        return array(
            self::WF_ESTADO_DISPONIVEL => '../imagens/workflow/clubvida-consulta-disponivel.png',
            self::WF_ESTADO_AGENDADA => '../imagens/workflow/clubvida-consulta-agendada.png',
            self::WF_ESTADO_REAGENDADA => '../imagens/workflow/clubvida-consulta-reagendada.png',
            self::WF_ESTADO_CONFIRMADA => '../imagens/workflow/clubvida-consulta-confirmada.png',
            self::WF_ESTADO_REALIZADA => '../imagens/workflow/clubvida-consulta-realizada.png',
            self::WF_ESTADO_CANCELADA => '../imagens/workflow/clubvida-consulta-cancelada.png',
            self::WF_ESTADO_NAO_REALIZADA => '../imagens/workflow/clubvida-consulta-naorealizada.png',
            self::WF_ESTADO_BLOQUEADA => '../imagens/workflow/clubvida-consulta-bloqueada.png',
            self::WF_ESTADO_EM_ATENDIMENTO => '../imagens/workflow/clubvida-consulta-em-atendimento.png',
            self::WF_ESTADO_AGUARDANDO_ATENDIMENTO => '../imagens/workflow/clubvida-consulta-aguardando-atendimento.png',
            self::WF_ESTADO_ESTORNADA => '../imagens/workflow/clubvida-consulta-cancelada.png'
        );
    }

    public function getEstadosWf($jsonEncode = false, $arWhere = array())
    {
        $arWhere[] = "esd.esdstatus = 'A'";
        $arWhere[] = "esd.tpdid = " . self::WF_TIPO_DOCUMENTO;

        $sql = "SELECT 
                    esd.esdid AS id,
                    esd.esddsc AS descricao
                FROM
                    workflow.estadodocumento esd
                WHERE
                    " . implode(' AND ', $arWhere) . "
                ORDER BY
                    esd.esdordem";
        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        $cores = $this->getCoresEventoPorEstado();
        $icones = $this->getIconesEventoPorEstado();
        $rsRetorno = array();

        // Coloca os dados na mesma ordem do array de cores
        foreach($cores as $id => $cor){
            $dadosSituacao = array();
            foreach ($rs as $linha){
                if($linha['id'] == $id){
                    $dadosSituacao = $linha;
                    break;
                }
            }

            if(!is_null($dadosSituacao['id'])) {
                $rsRetorno[] = array(
                    'id' => $dadosSituacao['id'],
                    'descricao' => $dadosSituacao['descricao'],
                    'cor' => $cor,
                    'icone' => $icones[$dadosSituacao['id']]
                );
            }
        }

        return $jsonEncode ? json_encode(utf8ArrayEncode($rsRetorno)) : $rsRetorno;
    }

    public function getEstadosWfParaCheckbox($arrWhere = array())
    {
        $arrEstadosWf = $this->getEstadosWf(false, $arrWhere);

        $arrEstadosWfCheckbox = array(
            array(
                'codigo' => '',
                'descricao' => "<div class='descricao label label-sm' style='color: #000;'>Todos</div>",
                'cor' => null
            )
        );

        foreach ($arrEstadosWf as $estadoWf) {
            $arrEstadosWfCheckbox[] = array(
                'codigo' => $estadoWf['id'],
                'descricao' => "<div class='descricao label label-sm' style='background-color: {$estadoWf['cor']}'>{$estadoWf['descricao']}</div>"
            );
        }

        return $arrEstadosWfCheckbox;
    }

    public function getListQtdAgendas($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "epa.excluido IS FALSE";

        $sql = "SELECT
                  TO_CHAR(epa.inicio, 'YYYYMMDD') AS data,
                  count(*) AS qtd
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                                                                    AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_profissional pro ON ep.profissional_id = pro.id
                INNER JOIN
                  corporativo.tb_pessoafisica p ON pro.pessoafisica_id = p.id
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                INNER JOIN
                  profissionalsaude.tb_especialidade e ON ep.especialidade_id = e.id
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                INNER JOIN
                  workflow.estadodocumento est ON doc.esdid = est.esdid
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento es ON eep.estabelecimento_id = es.id
                WHERE
                  " . implode(' AND ', $arWhere) . "
                GROUP BY
                    1";

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY data";
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

    public function getQtdAgendas($arWhere = array(), $jsonEncode = false)
    {
        $rs = $this->getListQtdAgendas($arWhere, false);

        $retorno = array();
        foreach($rs as $linha){
            $retorno[$linha['data']] = $linha['qtd'];
        }

        return $jsonEncode ? json_encode($retorno) : $retorno;
    }

    public function getQtdAgendasPorEstabelecimento($idEstabelecimento, $jsonEncode = false)
    {
        $arWhere = array(
            // "doc.esdid != " . self::WF_ESTADO_CANCELADA,
            "eep.estabelecimento_id = {$idEstabelecimento}"
        );

        return $this->getQtdAgendas($arWhere, $jsonEncode);
    }

    public function montaWhereAgendaDisponivel($param)
    {
        $arWhere = array(
            "doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL,
            "es.id = {$param['id']}"
        );

        if($param['pessoa_id'] && $param['pessoa_id'] != '-1'){
            $arWhere[] = "p.id = {$param['pessoa_id']}";
        }

        if($param['especialidade_id'] && $param['especialidade_id'] != '-1'){
            $arWhere[] = "e.id = {$param['especialidade_id']}";
        }

        if($param['data_selecionada'] && $param['data_selecionada'] != '-1'){
            $arWhere[] = "epa.inicio::date = '{$param['data_selecionada']}'";
        }

        return $arWhere;
    }

    public function getHorarios($arWhere = array(), $jsonEncode = false)
    {
        $rs = $this->getList($arWhere, false, 'epa.inicio');
        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }



    /**
     * @param $params
     * @return bool|int
     * @throws AgendaIndisponivel
     * @throws ValorProcedimentoInvalido
     */
    public function agendarConsulta($params)
    {
        if(!$this->isAgendaDisponivel($params['agenda_id'])) {
            throw new AgendaIndisponivel();
        }

        $agenda = $this->salvarDadosConsulta($params);

        wf_alterarEstado(
            $agenda['documento_id'],
            self::WF_ACAO_RESERVAR,
            '',
            array('id' => $agenda['id'])
        );

        return $agenda['id'];
    }

    public function isAgendaDisponivel($id)
    {
        $where = array("id = '{$id}'", "pessoa_paciente_id IS NOT NULL");
        return intval($this->recuperarUm('count(*)', $where)) === 0;
    }

    public function getListcalendarioAgrupadoPorData($arWhere = array(), $jsonEncode = false)
    {
        $listaEventos = $this->getList($arWhere, false);
        $agendas = $this->getListCalendario($listaEventos, false);
        return $this->agruparCalendarioPorData($agendas, $jsonEncode);
    }

    public function agruparCalendarioPorData($agendas, $jsonEncode = false)
    {
        $agendasAgrupadas = array();

        foreach($agendas as $agenda){
            if(!isset($agendasAgrupadas[$agenda['data_agrupador']])){
                $agendasAgrupadas[$agenda['data_agrupador']] = array();
            }

            $agendasAgrupadas[$agenda['data_agrupador']][] = $agenda;
        }

        return $jsonEncode ? json_encode(utf8ArrayEncode($agendasAgrupadas )): $agendasAgrupadas;
    }

    public function isGrupoHorarioDisponivel($dataAgenda, $horarios, $disponibilidade)
    {
        $horarios = $horarios ? $horarios : array();
        $data = formata_data_sql($dataAgenda);

        foreach($horarios as $indice => $horario){
            if(!$disponibilidade[$indice]){
                continue;
            }

            $inicio = strtotime("{$data} {$horario['inicio']}");
            $fim = strtotime("{$data} {$horario['fim']}");

            foreach($horarios as $indiceComparativo => $horarioComparativo){
                if($indice <= $indiceComparativo){
                    continue;
                }

                $inicioComparativo = strtotime("{$data} {$horarioComparativo['inicio']}");
                $fimComparativo = strtotime("{$data} {$horarioComparativo['fim']}");

                $disponibilidade[$indice]['horarioDisponivel'] = $disponibilidade[$indice]['horarioDisponivel'] && !(
                        ($inicioComparativo > $inicio && $inicioComparativo < $fim)
                        ||
                        ($fimComparativo > $inicio && $fimComparativo < $fim)
                        ||
                        ($inicio > $inicioComparativo && $inicio < $fimComparativo)
                        ||
                        ($fim > $inicioComparativo && $fim < $fimComparativo)
                        ||
                        ($inicio == $inicioComparativo && $fim == $fimComparativo)
                    );
            }
        }

        return $disponibilidade;
    }

    /**
     * @param $params
     * @return mixed
     * @throws AgendaIndisponivel
     * @throws CpfIndisponivel
     */
    public function reagendarConsulta($params)
    {
        $this->agendarConsulta($params);
        $this->setDadosNull();

        $this->carregarPorId($params['reagenda_id']);
        $agenda = $this->getDados();

        wf_alterarEstado(
            $agenda['documento_id'],
            self::WF_ACAO_REAGENDAR,
            '',
            array('id' => $agenda['id'])
        );
    }

    public function gerarEventosPorMesCalendario($especialidadeProfissionalId, $estabelecimentoId, $mes, $ano, $diasDaSemana, $jsonEncode = false)
    {
        $eventos = array_map(function($evento){
            return array(
                'title' => $evento['titulo'],
                'dsc' => $evento['descricao'],
                'start' => $evento['inicio'],
                'end' => $evento['fim'],
                'inicio' => $evento['inicio'],
                'fim' => $evento['fim'],
                'color' => $evento['cor'],
                'disponivel' => $evento['disponivel'],
                'estabelecimento_especialidade_profissional_id' => $evento['estabelecimento_especialidade_profissional_id'],
                'sala_estabelecimento_id' => $evento['sala_estabelecimento_id'],
            );

        }, $this->gerarEventosPorMes($especialidadeProfissionalId, $estabelecimentoId, $mes, $ano, $diasDaSemana));

        return $jsonEncode ? json_encode(utf8ArrayEncode($eventos)) : $eventos;
    }

    public function gerarEventosPorMes($especialidadeProfissionalId, $estabelecimentoId, $mes, $ano, $diasDaSemana, $jsonEncode = false)
    {
        $inicio = date('Y-m-d', strtotime("{$ano}-{$mes}-01"));
        $fim = date('Y-m-t', strtotime($inicio));

        return $this->gerarEventosPorPeriodo($especialidadeProfissionalId, $estabelecimentoId, $inicio, $fim, $diasDaSemana, $jsonEncode);
    }

    public function gerarEventosPorPeriodo($especialidadeProfissionalId, $estabelecimentoId, $dataInicio, $dataFim, $diasDaSemana, $jsonEncode = false)
    {
        $eventos = array();

        $mEspecialidadePro = new EspecialidadeProfissional($especialidadeProfissionalId);
        $especialidade = array_shift(array_filter($mEspecialidadePro->listarPorProfissional($mEspecialidadePro->profissional_id, false, $estabelecimentoId), function($especialidade) use ($especialidadeProfissionalId){
            return $especialidade['id'] == $especialidadeProfissionalId;
        }));

        $mEstabelecimentoEspecialidade = new EstabelecimentoEspecialidadeProfissional();
        $estabelecimentoEspecialidade = $mEstabelecimentoEspecialidade->getPorEspecialidadeProfissionalEstabelecimento($especialidadeProfissionalId, $estabelecimentoId);

        $diasDaSemana = array_filter($diasDaSemana, function($diaSemana){
            return $diaSemana['selecionado'] === 'true';
        });

        $data = $dataInicio;
        $duracaoConsulta = $especialidade['duracao_consulta'];
        $adicionarUmDia = function ($data){
            return date('Y-m-d', strtotime('+1 day', strtotime($data)));
        };

        $gerarEvento = function ($dataHoraInicio, $dataHoraFim, $duracaoConsulta, $estabelecimentoEspecialidadeProfissionalId, $sala){
            $eventos = array();
            $dataHora = $dataHoraInicio;
            $corEventos = $this->getCoresEventoPorEstado();

            while (date('YmdHi', strtotime($dataHora)) < date('YmdHi', strtotime($dataHoraFim))){
                $dataHoraTerminoConsulta = date('Y-m-d H:i:s', strtotime("+{$duracaoConsulta} minutes", strtotime($dataHora)));

                $horaInicio = date('H:i', strtotime($dataHora));
                $horaFim = date('H:i', strtotime($dataHoraTerminoConsulta));
                $salaDsc = $sala ? "\n {$sala['sala']}" : '';

                $eventos[] = array(
                    'estabelecimento_especialidade_profissional_id' => $estabelecimentoEspecialidadeProfissionalId,
                    'titulo' => "{$horaInicio} - {$horaFim} {$salaDsc}" ,
                    'inicio' => $dataHora,
                    'cor' => $corEventos[self::WF_ESTADO_DISPONIVEL],
                    'fim' => $dataHoraTerminoConsulta,
                    'sala_estabelecimento_id' => $sala ? $sala['id'] : null,
                );

                $dataHora = $dataHoraTerminoConsulta;
            }

            return $eventos;
        };

        $verificarDisponibilidade = function($eventos, $estabelecimentoId, $profissionalId, $data, $salaId = null){
            $validacaoEventos = array_map(function($evento){
                return array(
                    'inicio' => date('H:i', strtotime($evento['inicio'])),
                    'fim' => date('H:i', strtotime($evento['fim']))
                );
            }, $eventos);

            $validadeEventos = $this->verificarDisponbilidadeHorarios($validacaoEventos, $estabelecimentoId, $profissionalId,
                date('d/m/Y', strtotime($data)), $salaId, null, false,
                'O profissional já possui agenda marcada nesse horário.', 'Sala não disponível.');

            foreach($validadeEventos as $validadeTipoEvento){
                foreach($validadeTipoEvento as $indice => $validadeEvento){
                    if(!isset($eventos[$indice]['disponivel'])){
                        $eventos[$indice]['disponivel'] = true;
                    }

                    $eventos[$indice]['disponivel'] = $eventos[$indice]['disponivel'] && $validadeEvento['disponivel'];
                    if(!$validadeEvento['disponivel']){
                        $eventos[$indice]['titulo'] .= "\nConflito de agenda";
                        $eventos[$indice]['descricao'] .= "\n {$validadeEvento['msg']}";
                        $eventos[$indice]['cor'] = '#E43A45';
                    }
                }
            }

            return array_values($eventos);
        };

        $mSala = new SalaEstabelecimento();
        $salas = array();
        foreach($mSala->listarPorEstabelecimento($estabelecimentoId) as $sala){
            $salas[$sala['id']] = $sala;
        }


        while (date('Ymd', strtotime($data)) <= date('Ymd', strtotime($dataFim))){
            $diaSemanaData = date('w', strtotime($data));

            if(!isset($diasDaSemana[$diaSemanaData])){
                $data = $adicionarUmDia($data);
                continue;
            }

            $diaDaSemana = $diasDaSemana[$diaSemanaData];
            $eventosManha = array();
            $eventosTarde = array();

            if($diaDaSemana['hora_inicio_manha'] && $diaDaSemana['hora_fim_manha']){
                $dataHoraInicioManha = "{$data} {$diaDaSemana['hora_inicio_manha']}:00";
                $dataHoraFimManha = "{$data} {$diaDaSemana['hora_fim_manha']}:00";
                $salaManha = $diaDaSemana['sala_manha'] ? $salas[$diaDaSemana['sala_manha']] : null;

                $eventosManha = $gerarEvento($dataHoraInicioManha, $dataHoraFimManha, $duracaoConsulta, $estabelecimentoEspecialidade['id'], $salaManha);
                $eventosManha = $verificarDisponibilidade($eventosManha, $estabelecimentoId, $mEspecialidadePro->profissional_id, $data, $salaManha['id']);
            }

            if($diaDaSemana['hora_inicio_tarde'] && $diaDaSemana['hora_fim_tarde']){
                $dataHoraInicioTarde = "{$data} {$diaDaSemana['hora_inicio_tarde']}:00";
                $dataHoraFimTarde = "{$data} {$diaDaSemana['hora_fim_tarde']}:00";
                $salaTarde = $diaDaSemana['sala_tarde'] ? $salas[$diaDaSemana['sala_tarde']] : null;

                $eventosTarde = $gerarEvento($dataHoraInicioTarde, $dataHoraFimTarde, $duracaoConsulta, $estabelecimentoEspecialidade['id'], $salaTarde);
                $eventosTarde = $verificarDisponibilidade($eventosTarde, $estabelecimentoId, $mEspecialidadePro->profissional_id, $data, $salaTarde['id']);
            }

            $eventos = array_merge($eventos, $eventosManha, $eventosTarde);
            $data = $adicionarUmDia($data);
        }

        return $jsonEncode ? json_encode(utf8ArrayEncode($eventos)) : $eventos;
    }

    public function salvarEventosAgenda($eventos)
    {
        $eventos = $eventos ? $eventos : array();
        $eventos = array_filter($eventos, function($evento){
            return $evento['disponivel'] === '1';
        });

        foreach($eventos as $evento){
            $data = date('d/m/Y', strtotime($evento['inicio']));
            $horaInicio = date('H:i', strtotime($evento['inicio']));
            $horaFim = date('H:i', strtotime($evento['fim']));

            $dados = array(
                'estabelecimento_especialidade_profissional_id' => $evento['estabelecimento_especialidade_profissional_id'],
                'sala_estabelecimento_id' => $evento['sala_estabelecimento_id'],
                'data_agenda' => $data,
                'hora_inicio' => $horaInicio,
                'hora_fim' => $horaFim
            );

            $this->salvarAgenda($dados);
            $this->setDadosNull();
        }
    }

    public function verificarDisponbilidadeHorarios($horarios, $estabelecimentoId, $profissionalId, $dataAgenda, $salaId = null, $agendaId = null, $jsonEncode = false, $msgProfissional = '', $msgSala = '')
    {
        $disponibilidade = array();

        if(is_array($horarios)){
            $disponibilidade = array_map(function($horario) use ($estabelecimentoId, $profissionalId, $dataAgenda, $agendaId){
                $horarioDisponivel = array(
                    'periodoHoraValida' => false,
                    'horarioDisponivel' => false
                );

                if(isIntervaloDeHoraValido($horario['inicio'], $horario['fim'])) {
                    $horarioDisponivel['periodoHoraValida'] = true;
                    $horarioDisponivel['horarioDisponivel'] = $this->isHorarioDisponivel($estabelecimentoId, $profissionalId, $dataAgenda, $horario['inicio'], $horario['fim'], $agendaId);
                }

                return $horarioDisponivel;
            }, $horarios);

            $disponibilidade = $this->isGrupoHorarioDisponivel($dataAgenda, $horarios, $disponibilidade);
        }

        $disponibilidade = array_map(function($disponibilidade) use($msgProfissional){
            $msg = $msgProfissional;

            if (!$disponibilidade['periodoHoraValida']) {
                $msg = 'A hora de início não pode ser maior que a hora de término';
            } else if (!$disponibilidade['horarioDisponivel']) {
                $msg = 'O profissional selecionado não possui esse horário disponível.';
            }

            return array(
                'disponivel' => $disponibilidade['horarioDisponivel'],
                'msg' => $msg
            );
        }, $disponibilidade);

        $disponibilidadeSala = array();
        if($salaId){
            $disponibilidadeSala = array_map(function($horario) use ($estabelecimentoId, $salaId, $dataAgenda, $agendaId){
                $horarioDisponivel = array(
                    'periodoHoraValida' => false,
                    'horarioDisponivel' => false
                );

                if(isIntervaloDeHoraValido($horario['inicio'], $horario['fim'])) {
                    $horarioDisponivel['periodoHoraValida'] = true;
                    $horarioDisponivel['horarioDisponivel'] = $this->isHorarioSalaDisponivel($estabelecimentoId, $salaId, $dataAgenda, $horario['inicio'], $horario['fim'], $agendaId);
                }

                return $horarioDisponivel;
            }, $horarios);

            $disponibilidadeSala = array_map(function($disponibilidade) use ($msgSala){
                $msg = $msgSala;

                if (!$disponibilidade['periodoHoraValida']) {
                    $msg = 'A hora de início não pode ser maior que a hora de término';
                } else if (!$disponibilidade['horarioDisponivel']) {
                    $msg = 'A sala selecionada não possui esse horário disponível.';
                }

                return array(
                    'disponivel' => $disponibilidade['horarioDisponivel'],
                    'msg' => $msg
                );
            }, $disponibilidadeSala);
        }

        $disponibilidadeRetorno = array(
            'disponibilidade' => $disponibilidade,
            'disponibilidade_sala' => $disponibilidadeSala
        );

        return $jsonEncode ? json_encode(utf8ArrayEncode($disponibilidadeRetorno)) : $disponibilidadeRetorno;
    }

    /**
     * @param $params
     * @return bool|int
     * @throws AgendaIndisponivel
     * @throws ProfissionalIndisponivel
     * @throws SalaIndisponivel
     */
    public function salvarEncaixe($params)
    {
        $params['encaixe'] = 'true';
        $params['agenda_id'] = $this->salvarAgenda($params);
        $this->agendarConsulta($params);

        return $params['agenda_id'];
    }

    public function atender($id)
    {
        $this->carregarPorId($id);

        return wf_alterarEstado(
            $this->documento_id,
            self::WF_ACAO_ATENDER,
            '',
            array('id' => $id)
        );
    }

    public function finalizarAtendimento($id)
    {
        $this->carregarPorId($id);
        $duracao = $this->calcularDuracaoRealizacaoConsulta($id);
        $duracao = array_map(function($d){
            return str_pad($d, 2, '0', STR_PAD_LEFT);
        }, $duracao);
        $this->duracao_consulta = "{$duracao['hora']}:{$duracao['minuto']}:{$duracao['segundo']}";
        $this->salvar();

        return wf_alterarEstado(
            $this->documento_id,
            self::WF_ACAO_FINALIZAR_ATENDIMENTO,
            '',
            array('id' => $id)
        );
    }

    public function getDataHoraAcao($id, $acaoId)
    {
        $sql = "SELECT
                    TO_CHAR(his.htddata, 'DD/MM/YYYY HH24:MI:ss') AS hora_tramitacao
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                INNER JOIN
                  workflow.historicodocumento his ON doc.docid = his.docid
                                                 AND his.aedid = {$acaoId}
                WHERE
                  epa.id = {$id}
                ORDER BY
                  his.htddata DESC";

        return $this->pegaUm($sql);
    }

    public function calcularDuracaoRealizacaoConsulta($id)
    {
        $sql = "SELECT DISTINCT
                    his.htddata AS hora_tramitacao,
                    now() AS hora_atual_bd
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                INNER JOIN
                  workflow.historicodocumento his ON doc.docid = his.docid
                                                 AND his.aedid = " . self::WF_ACAO_ATENDER . "
                WHERE
                  epa.id = {$id}
                ORDER BY
                  his.htddata DESC";

        $horario = $this->pegaLinha($sql);
        $dataInicioAtendimento = new DateTime(date('Y-m-d H:i:s', strtotime($horario['hora_tramitacao'])));
        $dataAtual = new DateTime(date('Y-m-d H:i:s', strtotime($horario['hora_atual_bd'])));

        $intervalo = $dataInicioAtendimento->diff( $dataAtual );

        return array(
            'hora' => $intervalo->h,
            'minuto' => $intervalo->i,
            'segundo' => $intervalo->s,
            'duracao_segundos' => $dataAtual->getTimestamp() - $dataInicioAtendimento->getTimestamp()
        );
    }

    public function isAlteracaoAgendaDisponivel($id)
    {
        $sql = "SELECT DISTINCT
                    his.htddata AS hora_tramitacao,
                    now() AS hora_atual_bd
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid
                INNER JOIN
                  workflow.historicodocumento his ON doc.docid = his.docid
                                                 AND his.aedid = " . self::WF_ACAO_FINALIZAR_ATENDIMENTO . "
                WHERE
                  epa.id = {$id}
                ORDER BY
                  his.htddata DESC";

        $horario = $this->pegaLinha($sql);
        $dataInicioAtendimento = new DateTime(date('Y-m-d H:i:s', strtotime($horario['hora_tramitacao'])));
        $dataAtual = new DateTime(date('Y-m-d H:i:s', strtotime($horario['hora_atual_bd'])));

        $intervalo = $dataInicioAtendimento->diff( $dataAtual );
        $diferencaHoras = $intervalo->h + ($intervalo->days * 24);
        return $diferencaHoras <= QTD_HORAS_ALTERACAO_PRONTUARIO;
    }

    public function isUsuarioPodeAtender($agendaId, $cpf = null)
    {
        $dadosAgenda = $this->getTodosDados($agendaId);

        $cpf = $cpf ? $cpf : $_SESSION['usucpf'];
        $desformataCpf = function ($cpf) {
            return trim(str_replace(array('.', '-'), '', $cpf));
        };

        return $desformataCpf($cpf) === $desformataCpf($dadosAgenda['cpf_profissional']);
    }

    public function isAtendendoOutro($idAgenda, $idEstabelecimento, $idProfissional, $esdid = self::WF_ESTADO_EM_ATENDIMENTO){
        $sql = "SELECT 
                  count(epa.id)
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN 
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id AND eep.excluido IS FALSE
                INNER JOIN 
                  profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                INNER JOIN 
                  profissionalsaude.tb_profissional p ON ep.profissional_id = p.id AND p.excluido is FALSE    
                INNER JOIN 
                  workflow.documento doc ON doc.docid = epa.documento_id
                WHERE
                  epa.excluido is FALSE
                  AND
                  eep.estabelecimento_id = {$idEstabelecimento}
                  AND 
                  p.pessoafisica_id = (SELECT pessoafisica_id from profissionalsaude.tb_profissional where id = {$idProfissional})
                  AND
                  doc.esdid = {$esdid}
                  AND 
                  epa.id != {$idAgenda}";

        return $this->pegaUm($sql) > 0;
    }

    public static function getAtendimentoByUsuarioLogado($idEstabelecimento = null){
        if(empty($idEstabelecimento)){
            $idEstabelecimento = $_REQUEST['estabelecimento_id'] ? $_REQUEST['estabelecimento_id'] :
                $_SESSION['estabelecimento_selecionado'];
        }

        $cpf = formatar_cpf($_SESSION['usucpf']);

        $p = getParam();
        $whereAgenda = $p['agenda_id'] ? "AND epa.id != {$p['agenda_id']}" : '';

        $result = false;
        if($idEstabelecimento && $cpf){
            $sql = "SELECT 
                  epa.id, 
                  eep.estabelecimento_id,
                  pespaciente.nome_completo AS paciente_nome_completo,
                  TO_CHAR(epa.inicio, 'HH24:mi') AS hora_inicio,
                  TO_CHAR(epa.inicio, 'DD/MM/YYYY') AS data_agenda,
                  epa.criado_em
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN 
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id AND eep.excluido IS FALSE
                INNER JOIN 
                  profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                INNER JOIN 
                  profissionalsaude.tb_profissional p ON ep.profissional_id = p.id AND p.excluido is FALSE    
                INNER JOIN 
                  workflow.documento doc ON doc.docid = epa.documento_id
                INNER JOIN
                  corporativo.tb_pessoafisica pespaciente ON epa.pessoa_paciente_id = pespaciente.id
                WHERE
                  epa.excluido is FALSE
                  {$whereAgenda}
                  AND
                  eep.estabelecimento_id = {$idEstabelecimento}
                  AND 
                  p.pessoafisica_id = (SELECT id from corporativo.tb_pessoafisica where cpf = '{$cpf}')
                  AND
                  doc.esdid = ".EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO."
                  ORDER BY epa.criado_em";

            $modelo = new EstabelecimentoProfissionalAgenda();
            $result = $modelo->pegaLinha($sql);
        }

        return $result;
    }

    public static function montaAvisoAtendimento(){
        $arAtendimento = self::getAtendimentoByUsuarioLogado();

        if($arAtendimento){
            $p = setParam(array(
                'agenda_id' => $arAtendimento['id'],
                'id' => $arAtendimento['estabelecimento_id'],
            ), false);
            $msg = <<<HTML
        <span>Existe uma consulta em atendimento, clique 
        <a href="?modulo=principal/medico/atendimento&acao=C&p={$p}" >aqui</a> 
        para ir até ela.
        </span>

HTML;
            addMsgWarning($msg);
        }else{
            unset($_SESSION['MSG_SISTEMA_WARNING']);
        }
    }

    public function getDadosCalendario($estabelecimentoId, $mes, $ano, $jsonEncode = false)
    {
        $arWherePadraoAgenda = $this->getWhereMesAnoAgendaByEstabelecimento($estabelecimentoId, $mes, $ano);

        $mesAno = array("{$ano}-{$mes}");
        $mesAno = $jsonEncode ? json_encode($mesAno) : $mesAno;
        $listaEventos = $this->getList($arWherePadraoAgenda, false);
        $eventos = $this->getListCalendario($listaEventos, false);
        $eventosAgrupados = $this->agruparCalendarioPorData($eventos, $jsonEncode);

        return array(
            'mes_ano' => $mesAno,
            'eventos' => $jsonEncode ? json_encode(utf8ArrayEncode($eventos)): $eventos,
            'eventos_agrupados' => $eventosAgrupados,
        );
    }

    public function excluirHorario($id)
    {
        $arrDaods = $this->getTodosDados($id);
        $registroExcluido = false;

        //Um horárop só pode ser excluído se estiver no estado disponível ou bloqueado
        if(in_array($arrDaods['wf_estado_id'], array(EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL, EstabelecimentoProfissionalAgenda::WF_ESTADO_BLOQUEADA))) {
            $this->excluirLogicamente($id);
            $registroExcluido = true;
        }

        return $registroExcluido;
    }

    public function getWhereMesAnoAgendaByEstabelecimento($estabelecimentoId, $mes, $ano)
    {
        $dataInicio = "{$ano}-{$mes}-01";
        $dataFim = date('Y-m-t', strtotime($dataInicio));

        $now = date('Y-m-d H:i:s');
        $arWherePadraoAgenda = [
            "eep.estabelecimento_id = {$estabelecimentoId}",
            "(
                doc.esdid != " . EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL . "
                OR
                (doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL . " AND epa.inicio >= '{$now}')
             )",
            "epa.inicio::date >= '{$dataInicio}'",
            "epa.inicio::date <= '{$dataFim}'"
        ];
        return $arWherePadraoAgenda;
    }

    public function getAtendimentosAgendadosComInicioNoProximoDia()
    {
        $esdid = self::WF_ESTADO_AGENDADA;

        $dataAtendimento = new DateTime(date('Y-m-d'));
        $dataAtendimento->add(new DateInterval('P1D'));
        $dataAtendimento = $dataAtendimento->format('Y-m-d');

        $sql = "SELECT 
                  epa.id AS agenda_id
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN 
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id AND eep.excluido IS FALSE
                INNER JOIN 
                  profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                INNER JOIN 
                  profissionalsaude.tb_profissional p ON ep.profissional_id = p.id AND p.excluido is FALSE    
                INNER JOIN 
                  workflow.documento doc ON doc.docid = epa.documento_id
                WHERE
                  epa.excluido is FALSE
                  AND epa.inicio::date = '{$dataAtendimento}'::date
                  AND doc.esdid = {$esdid}";

        return $this->carregarArray($sql);
    }

    /**
     * @param $params
     * @return string
     * @throws CpfIndisponivel
     * @throws ValorProcedimentoInvalido
     */
    public function salvarDadosConsulta($params)
    {
        if (!isset($params['valor_procedimento_id']) || empty($params['valor_procedimento_id'])) {
            throw new ValorProcedimentoInvalido();
        }

        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $mPessoa = new PessoaFisica();
        $paramsPf = $params;
        $paramsPf['id'] = $params['paciente_id'];
        $pessoaFisicaId = $mPessoa->salvarPessoaFisica($paramsPf, array(), false, true, false);

        $mValorProcedimento = new ValorProcedimento();
        $valor = $mValorProcedimento->calcularValorCobranca($params['valor_procedimento_id']);

        $this->carregarPorId($params['agenda_id']);
        $agenda = $this->getDados();
        $agenda['pessoa_paciente_id'] = $pessoaFisicaId;
        $agenda['tipo_consulta_id'] = $params['tipo_consulta_id'];
        $agenda['valor_procedimento_id'] = $params['valor_procedimento_id'];
        $agenda['valor_procedimento'] = $valor['valor'];
        $agenda['valor_profissional'] = $valor['valor_profissional'];
        $agenda['percentual_profissional'] = $valor['percentual_profissional'];
        $agenda['convenio_id'] = $valor['convenio_id'];

        // Passa $agenda no primeiro parâmetro para identificar a alteração
        $dados = $this->getDadosCarimbo($agenda, $agenda);
        $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);

        if($params['numero_convenio_plano']) {
            $mConvenioPaciente = new ConvenioEstabelecimentoPaciente();
            $mConvenioPaciente->salvarConvenioPaciente($pessoaFisicaId, $agenda['convenio_id'], $params['numero_convenio_plano'], $params['validade_convenio_plano']);
        }

        if ($params['observacao']) {
            $mObservacao = new ObservacaoAgenda();
            $mObservacao->salvarObservacao(array(
                'estabelecimento_profissional_agenda_id' => $agenda['id'],
                'descricao' => $params['observacao'],
            ));
        }
        return $agenda;
    }

    public function alterarValorAgenda($agendaId, $valorProcedimento, $valorProfissional = null)
    {
        $agenda = $this->recuperarLinha('*', ["id = {$agendaId}"], false);
        $estadoAtual = wf_pegarEstadoAtual($agenda['documento_id']);

        if(is_array($valorProcedimento)) {
            $vlrProcedimento = array_shift($valorProcedimento);
            $valorProcedimento = $vlrProcedimento['valor'];
        }

        $valorProcedimento = (strpos($valorProcedimento, ',') === false ? $valorProcedimento : desformata_valor($valorProcedimento));

        $alterarValor = false;
        if($valorProfissional !== null) {
            $valorProfissional = desformata_valor($valorProfissional);
            $alterarValor = $agenda['valor_profissional'] != floatval($valorProfissional)
                && $estadoAtual['esdid'] == self::WF_ESTADO_REALIZADA;
        } else {
            $valorProfissional = $agenda['valor_profissional'];
        }

        $mValorPagar = new ValorAPagar();
        if($alterarValor) {
            $mValorPagar->gerarValorNegativoAgenda($agendaId);
            $mValorPagar->setDadosNull();
        }

        $this->manter([
            'id' => $agendaId,
            'valor_procedimento' => $valorProcedimento,
            'valor_profissional' => $valorProfissional
        ]);

        if($alterarValor) {
            $mValorPagar->gerarValorAgenda($agendaId);
        }

        return $agendaId;
    }

    public function gerarUrlAtendimentoRemoto($agendaId)
    {
        $hashids = new SimecHashids();
        $roomId = $hashids->encode($agendaId, 6);

        return $this->getUrlAtendimentoRemotoByRoomId($roomId);

    }

    public function getUrlAtendimentoRemotoByRoomId($roomId)
    {
        $serverUrl = $_SERVER['HTTP_ORIGIN'] ?? (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? "https" : "http") . "://$_SERVER[HTTP_HOST]";

        return "{$serverUrl}/consultorio-virtual/{$roomId}";
    }

    public function getDadosPainelFilaAtendimento($estabelecimentoId, $data = null)
    {
        $sqlData = $data ? " AND epa.inicio::date = '{$data}' " : '';
        $sql = "SELECT
                    CASE WHEN COUNT(DISTINCT CASE WHEN doc.esdid = 720104 THEN epa.id ELSE null END) > 0 THEN SUM(DISTINCT tempo_espera) / COUNT(DISTINCT CASE WHEN doc.esdid = 720104 THEN epa.id ELSE null END) ELSE 0 END AS tempo_medio_espera,
                    COUNT(DISTINCT CASE WHEN doc.esdid = " . self::WF_ESTADO_AGUARDANDO_ATENDIMENTO . " THEN epa.id ELSE null END) AS qtd_aguardando,
                    COUNT(DISTINCT CASE WHEN doc.esdid = " . self::WF_ESTADO_EM_ATENDIMENTO . " THEN epa.id ELSE null END) AS qtd_em_atendimento,
                    COUNT(DISTINCT CASE WHEN doc.esdid = " . self::WF_ESTADO_REALIZADA . " THEN epa.id ELSE null END) AS qtd_atendido,
                    COUNT(DISTINCT epa.id) AS qtd
                FROM
                       estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                LEFT JOIN
                       estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                             AND eep.excluido IS FALSE
                INNER JOIN
                       workflow.documento doc ON epa.documento_id = doc.docid
                INNER JOIN
                       workflow.estadodocumento est ON doc.esdid = est.esdid
                INNER JOIN
                       estabelecimentosaude.tb_estabelecimento es ON eep.estabelecimento_id = es.id
                LEFT JOIN (
                       SELECT
                              dochistorico.docid,
                              EXTRACT(EPOCH FROM (hist_fim.htddata - hist_inicio.htddata)::INTERVAL)/60 AS tempo_espera
                       FROM
                              workflow.documento dochistorico
                       INNER JOIN
                              workflow.historicodocumento hist_inicio ON hist_inicio.docid = dochistorico.docid
                                                                     AND hist_inicio.aedid = " . self::WF_ACAO_REGISTRAR_PRESENCA . "
                       INNER JOIN
                              workflow.historicodocumento hist_fim ON hist_fim.docid = dochistorico.docid
                                                                     AND hist_fim.aedid = " . self::WF_ACAO_FINALIZAR_ATENDIMENTO . "
                           ) historico ON historico.docid = doc.docid
                WHERE
                      doc.esdid IN (" . self::WF_ESTADO_AGUARDANDO_ATENDIMENTO . ", " . self::WF_ESTADO_EM_ATENDIMENTO . ", " . self::WF_ESTADO_REALIZADA . ")
                      AND
                      es.id = {$estabelecimentoId}
                      {$sqlData}
                ORDER BY
                       1";

        return $this->carregarArray($sql);
    }

    public function getFilaAtendimento($estabelecimentoId, $data = null)
    {
        $sqlData = $data ? " AND epa.inicio::date = '{$data}' " : '';
        $sql = "SELECT
                            epa.id,
                            pespac.nome_completo AS paciente,
                            TO_CHAR(his.htddata, 'HH24:mm:ss') AS hora,
                            se.sala,
                            array_to_string(array_agg(DISTINCT CASE
                                                                   WHEN pespro.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                                                                       THEN tp.pron_trat_masc || ' ' || pespro.nome_completo
                                                                   WHEN pespro.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                                                                       THEN tp.pron_trat_fem || ' ' || pespro.nome_completo
                                                                   ELSE pespro.nome_completo
                                END), ', ') AS profissional,
                            array_to_string(array_agg(DISTINCT e.descricao), ', ')  AS especialidade
                        FROM
                            estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                                LEFT JOIN
                            estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                AND eep.excluido IS FALSE
                                INNER JOIN
                            profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                                AND ep.excluido IS FALSE
                                INNER JOIN
                            profissionalsaude.tb_profissional pro ON ep.profissional_id = pro.id
                                LEFT JOIN
                            profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                                INNER JOIN
                            corporativo.tb_pessoafisica pespro ON pespro.id = pro.pessoafisica_id
                                INNER JOIN
                            corporativo.tb_pessoafisica pespac ON pespac.id = epa.pessoa_paciente_id
                                INNER JOIN
                            profissionalsaude.tb_especialidade e ON ep.especialidade_id = e.id
                                INNER JOIN
                            workflow.documento doc ON epa.documento_id = doc.docid
                                INNER JOIN
                            workflow.estadodocumento est ON doc.esdid = est.esdid
                                INNER JOIN
                            estabelecimentosaude.tb_estabelecimento es ON eep.estabelecimento_id = es.id
                                LEFT JOIN
                            estabelecimentosaude.tb_sala_estabelecimento se ON epa.sala_estabelecimento_id = se.id
                                INNER JOIN
                           workflow.historicodocumento his ON doc.docid = his.docid
                                                          AND his.aedid = " . self::WF_ACAO_ATENDER . "
                WHERE
                      es.id = {$estabelecimentoId}
                      {$sqlData}
                GROUP BY
                    epa.id,
                    pespac.nome_completo,
                    se.sala,
                    his.htddata
                ORDER BY
                       his.htddata DESC
                LIMIT 5
                ";

        return $this->carregarArray($sql);
    }

}