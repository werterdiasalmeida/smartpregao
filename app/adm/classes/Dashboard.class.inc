<?php

class Dashboard extends Modelo
{
    public function getInformacoesAgendaPorTipo($filtros = array())
    {
        $arWhere = array();
        if(!empty($filtros['estabelecimento_id'])){
            $arWhere[] = " est.id IN ('" . implode("','", $filtros['estabelecimento_id']) . "')";
        }

        if(!empty($filtros['tipo_consulta_id'])){
            $arWhere[] = " tc.id = " . $filtros['tipo_consulta_id'];
        }

        if(!empty($filtros['inicio']) && !empty($filtros['fim'])){
            $arWhere[] = " epa.inicio::date BETWEEN '{$filtros['inicio']}' AND '{$filtros['fim']}' ";
        }

        $arWhere[] = "epa.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento('est', 'id', [ADM_PERFIL_LOCAL,ADM_PERFIL_GERAL]);

        $sql = "SELECT
                  tc.id AS tipo_consulta_id,
                  tc.descricao AS tipo_consulta,
                  TO_CHAR(epa.inicio,'DD/MM') AS dia_consulta,
                  esp.id AS especialidade_id,
                  esp.descricao AS especialidade,
                  se.id AS sala_id,
                  se.sala,
                  CASE
                    WHEN tc.remunerada is true THEN epa.valor_procedimento
                    ELSE 0
                  END AS valor,
                  tc.remunerada,
                  pfpr.nome_completo AS profissional,
                  pfpr.id AS profissional_pessoafisica_id
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                LEFT JOIN
                  estabelecimentosaude.tb_sala_estabelecimento se ON se.id = epa.sala_estabelecimento_id AND se.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id AND tc.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id AND esp.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                INNER JOIN
                  corporativo.tb_pessoafisica pfpr ON pfpr.id = pr.pessoafisica_id AND pfpr.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON tp.id = pr.tipo_profissional_id AND tp.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id AND vp.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id AND p.excluido IS FALSE
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid AND doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA . "
                WHERE
                  " . implode(' AND ', $arWhere) . "
                ORDER BY
                  epa.inicio ASC";

        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public function getInformacoesComplementaresAgendaPorTipo($filtros = array())
    {
        $arWhere = array();
        if(!empty($filtros['estabelecimento_id'])){
            $arWhere[] = " est.id IN ('" . implode("','", $filtros['estabelecimento_id']) . "')";
        }

        if(!empty($filtros['tipo_consulta_id'])){
            $arWhere[] = " tc.id = " . $filtros['tipo_consulta_id'];
        }

        if(!empty($filtros['especialidade_id'])){
            $arWhere[] = " esp.id = " . $filtros['especialidade_id'];
        }

        if(!empty($filtros['pessoafisica_id'])){
            $arWhere[] = " pfprofissional.id = " . $filtros['pessoafisica_id'];
        }

        if(!empty($filtros['inicio']) && !empty($filtros['fim'])){
            $arWhere[] = " epa.inicio::date BETWEEN '{$filtros['inicio']}' AND '{$filtros['fim']}' ";
        }

        $arWhere[] = "epa.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento('est', 'id', [ADM_PERFIL_LOCAL,ADM_PERFIL_GERAL]);

        $sql = "SELECT
                  TO_CHAR(epa.inicio,'DD/MM/YYYY') AS data_consulta,
                  pj.nome_fantasia as estabelecimento,
                  pfprofissional.nome_completo AS profissional,
                  pfpaciente.nome_completo AS paciente,
                  tc.descricao AS tipo_consulta,
                  esp.descricao AS especialidade,
                  se.sala,
                  con.nome AS convenio,
                  CASE
                    WHEN tc.remunerada is true THEN epa.valor_procedimento
                    ELSE 0
                  END AS valor,
                  tc.remunerada
                FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  corporativo.tb_pessoafisica pfpaciente ON pfpaciente.id = epa.pessoa_paciente_id
                INNER JOIN
                  estabelecimentosaude.tb_convenio_estabelecimento con ON con.id = epa.convenio_id
                LEFT JOIN
                  estabelecimentosaude.tb_sala_estabelecimento se ON se.id = epa.sala_estabelecimento_id AND se.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id AND tc.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id AND esp.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                INNER JOIN
                  corporativo.tb_pessoafisica pfprofissional ON pfprofissional.id = pr.pessoafisica_id
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON tp.id = pr.tipo_profissional_id AND tp.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                INNER JOIN
                  corporativo.tb_pessoajuridica pj ON pj.id = est.pessoajuridica_id
                INNER JOIN
                  estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id AND vp.excluido IS FALSE
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid AND doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA . "
                WHERE
                  " . implode(' AND ', $arWhere) . "
                ORDER BY
                  epa.inicio ASC";

        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public function getInformacoesGerais($filtros = array(), $paginacao = true)
    {
        $arWhere = array();
        $arWhere[] = "est.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento('est', 'id', [ADM_PERFIL_LOCAL,ADM_PERFIL_GERAL]);
        if(!empty($filtros['estabelecimento_id'])){
            $arWhere[] = " est.id IN ('" . implode("','", $filtros['estabelecimento_id']) . "')";
        }

        //Where do join
        $arWhereInterno = array();
        if(!empty($filtros['tipo_consulta_id'])){
            $arWhereInterno[] = " tc.id = " . $filtros['tipo_consulta_id'];
        }

        if(!empty($filtros['inicio']) && !empty($filtros['fim'])){
            $arWhereInterno[] = " epa.inicio::date BETWEEN '{$filtros['inicio']}' AND '{$filtros['fim']}' ";
        }
        $arWhereInterno[] = "epa.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento('est', 'id', [ADM_PERFIL_LOCAL,ADM_PERFIL_GERAL]);

        $sql = "SELECT
                  pj.nome_fantasia as nome,
                  CASE
                    WHEN salas.qtd = 0 THEN 1
                    ELSE salas.qtd
                  END AS qtd_salas,
                  consultas_agendadas.qtd AS qtd_total_agendadas,
                  consultas.qtd AS qtd_total,
                  consultas.valor AS valor_total,
                  consultas_remuneradas.qtd AS qtd_remuneradas,
                  consultas_remuneradas.valor AS valor_remuneradas,
                  (COALESCE(consultas_remuneradas.valor,0) / COALESCE(consultas_remuneradas.qtd,1)) AS ticket_medio,
                  (COALESCE(consultas_remuneradas.valor,0) / COALESCE(salas.qtd,1)) AS valor_medio_sala,
                  json_agenda.json AS json_agenda,
                  est.id AS estabelecimento_id
                FROM
                  estabelecimentosaude.tb_estabelecimento est
                INNER JOIN
                  corporativo.tb_pessoajuridica pj ON pj.id = est.pessoajuridica_id
                INNER JOIN
                ( 
                    SELECT
                        j.estabelecimento_id,
                        json_agg(j ORDER BY qtd DESC) AS json
                    FROM (
                        SELECT
                              est.id AS estabelecimento_id,
                              esd.esddsc estado,
                              esd.esdid estado_id,
                              COUNT(epa.id) AS qtd
                        FROM
                            estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                        LEFT JOIN
                          estabelecimentosaude.tb_sala_estabelecimento se ON se.id = epa.sala_estabelecimento_id AND se.excluido IS FALSE
                        LEFT JOIN
                          estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id AND tc.excluido IS FALSE
                        INNER JOIN
                          estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                        INNER JOIN
                          estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                        INNER JOIN
                          workflow.documento doc ON epa.documento_id = doc.docid
                        INNER JOIN
                          workflow.estadodocumento esd ON esd.esdid = doc.esdid
                        WHERE
                           " . implode(' AND ', $arWhereInterno) . "
                           AND CASE WHEN esd.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL . " THEN (epa.inicio::date >= now()::date) ELSE true END
                        GROUP BY 
                            1,2,3 
                        ORDER BY
                            qtd DESC) j
                    GROUP BY 1
                  ) json_agenda ON json_agenda.estabelecimento_id = est.id
                LEFT JOIN
                  (
                    SELECT
                      est.id AS estabelecimento_id,
                      SUM(epa.valor_procedimento) AS valor,
                      COUNT(epa.id) AS qtd
                    FROM
                        estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                    LEFT JOIN
                        estabelecimentosaude.tb_sala_estabelecimento se ON se.id = epa.sala_estabelecimento_id AND se.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id AND tc.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id AND esp.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_tipo_profissional tp ON tp.id = pr.tipo_profissional_id AND tp.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id AND vp.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id AND p.excluido IS FALSE
                    INNER JOIN
                      workflow.documento doc ON epa.documento_id = doc.docid AND doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA . "
                    WHERE
                      " . implode(' AND ', $arWhereInterno) . "
                    GROUP BY 1
                  ) consultas ON consultas.estabelecimento_id = est.id
                LEFT JOIN
                  (
                    SELECT
                      est.id AS estabelecimento_id,
                      COUNT(epa.id) AS qtd
                    FROM
                        estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                    LEFT JOIN
                        estabelecimentosaude.tb_sala_estabelecimento se ON se.id = epa.sala_estabelecimento_id AND se.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id AND tc.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id AND esp.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_tipo_profissional tp ON tp.id = pr.tipo_profissional_id AND tp.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                    INNER JOIN
                      workflow.documento doc ON epa.documento_id = doc.docid AND doc.esdid NOT IN (" . EstabelecimentoProfissionalAgenda::WF_ESTADO_REAGENDADA . ", " . EstabelecimentoProfissionalAgenda::WF_ESTADO_CANCELADA . ", " . EstabelecimentoProfissionalAgenda::WF_ESTADO_NAO_REALIZADA . ")
                    WHERE
                      " . implode(' AND ', $arWhereInterno) . "
                    GROUP BY 1
                  ) consultas_agendadas ON consultas_agendadas.estabelecimento_id = est.id  
                LEFT JOIN
                  (
                    SELECT
                      est.id AS estabelecimento_id,
                      SUM(epa.valor_procedimento) AS valor,
                      COUNT(epa.id) AS qtd
                    FROM
                        estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                    LEFT JOIN
                        estabelecimentosaude.tb_sala_estabelecimento se ON se.id = epa.sala_estabelecimento_id AND se.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id AND tc.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id AND esp.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_tipo_profissional tp ON tp.id = pr.tipo_profissional_id AND tp.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id AND vp.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id AND p.excluido IS FALSE
                    INNER JOIN
                      workflow.documento doc ON epa.documento_id = doc.docid AND doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA . "
                    WHERE
                      " . implode(' AND ', $arWhereInterno) . " AND tc.remunerada IS TRUE
                    GROUP BY 1
                  ) consultas_remuneradas ON consultas_remuneradas.estabelecimento_id = est.id
                LEFT JOIN
                  (
                    SELECT
                      est.id AS estabelecimento_id,
                      COUNT( DISTINCT COALESCE(se.id,999999)) AS qtd
                    FROM
                        estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                    LEFT JOIN
                        estabelecimentosaude.tb_sala_estabelecimento se ON se.id = epa.sala_estabelecimento_id AND se.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id AND tc.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id AND esp.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id AND pr.excluido IS FALSE
                    INNER JOIN
                        profissionalsaude.tb_tipo_profissional tp ON tp.id = pr.tipo_profissional_id AND tp.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id AND vp.excluido IS FALSE
                    INNER JOIN
                        estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id AND p.excluido IS FALSE
                    INNER JOIN
                      workflow.documento doc ON epa.documento_id = doc.docid AND doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA . "
                    WHERE
                      " . implode(' AND ', $arWhereInterno) . "
                    GROUP BY 1
                  ) salas ON salas.estabelecimento_id = est.id
                WHERE
                  " . implode(' AND ', $arWhere) . "
                  " . (empty($_POST['ordem']) ? ' ORDER BY pj.nome_fantasia ASC ' : '');

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 2);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }
}