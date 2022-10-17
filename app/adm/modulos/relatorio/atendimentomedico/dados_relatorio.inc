<?php

$arrLabels = array(
    "codigo_agenda" => "Agenda",
    "data_inicio" => "Data da consulta",
    "nome_profissional" => "Nome do profissional",
    "dsc_procedimento" => "Procedimento",
    "nome_paciente" => "Nome do paciente",
    "nome_convenio" => "Nome do convênio",
    "nome_clinica" => "Nome da clínica",
    "dsc_tipo_consulta"  => "Tipo de consulta",
    "data_hora_inicio" => "Data e Hora",
    "mesano" => "Mês / Ano",
    "valor_procedimento" => 'Valor do Procedimento',
    "valor_repassado" => 'Valor Repassado',
    "valor_repassar" => 'Valor a Repassar',
    "valor_servico" => 'Valor do Serviço',
);

$arrColunas = array(
    "nome_profissional",
    "dsc_procedimento",
    "nome_paciente",
    "nome_convenio",
    "nome_clinica",
    "dsc_tipo_consulta",
    "data_inicio",
    "data_hora_inicio",
    "valor_procedimento",
    "valor_repassado",
    "valor_repassar",
    "valor_servico",
);

$arrAgrupadores = array(
    "data_inicio",
    "nome_profissional",
    "dsc_procedimento",
    "nome_paciente",
    "nome_convenio",
    "nome_clinica",
    "mesano",
);

$arrAgrupadoColunas = array(
    "data_inicio",
    "codigo_agenda",
    "nome_profissional",
    "dsc_procedimento",
    "nome_paciente",
    "nome_convenio",
    "nome_clinica",
    "dsc_tipo_consulta",
    "data_hora_inicio",
    "data_inicio",
    "valor_procedimento",
    "valor_repassado",
    "valor_repassar",
    "valor_servico",
);

$arrPesquisa = array(
    'nome_clinica' => array('tipo' => 'texto'),
    'data_inicio' => array('tipo' => 'data'),
    'nome_convenio' => array('tipo' => 'texto')
);

$arrPesquisa = array(
    'nome_clinica',
    'data_inicio',
    'nome_convenio'
);

$mRelatorio = new RelatorioNovo($arrLabels, $arrColunas, $arrAgrupadores, $arrAgrupadoColunas);

function monta_sql()
{

    extract($_REQUEST);
    $where = array();
    $where[] = "est.excluido IS FALSE";
    $where[] = "doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA;
    $where[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();

    if($estabelecimento_id){
        array_push($where, " est.id IN (". implode(', ', $estabelecimento_id).")");
    }

    if($profissional_id){
        array_push($where, " pr.id IN (". implode(', ', $profissional_id).")");
    }

    // período de consulta
    if( $data_de){
        $data_de = formata_data_sql($data_de);
        array_push($where, " epa.inicio >= '{$data_de} 00:00:00' ");
    }

    if( $data_ate ){
        $data_ate = formata_data_sql($data_ate);
        array_push($where, " epa.inicio <= '{$data_ate} 23:59:59' ");
    }

    //ORDENAR CAMPOS
    $ORDENA = "";
    if($_REQUEST['ordenarcampo']){
        $ORDENA = "ORDER BY ";
        $ORDENA .= implode(', ', $_REQUEST['ordenarcampo']);
    }

    if($ORDENA != ""){
        $ORDENA .= " ".$_REQUEST['tipoordenar'];
    }


    $sql = "
   SELECT DISTINCT
        epa.id as codigo_agenda,
       to_char(epa.inicio::date,'DD/MM/YYYY') as data_inicio,
       (CASE EXTRACT(month from epa.inicio)
          WHEN '1' THEN 'Janeiro/'
          WHEN '2' THEN 'Fevereiro/'
          WHEN '3' THEN 'Março/'
          WHEN '4' THEN 'Abril/'
          WHEN '5' THEN 'Maio/'
          WHEN '6' THEN 'Junho/'
          WHEN '7' THEN 'Julho/'
          WHEN '8' THEN 'Agosto/'
          WHEN '9' THEN 'Setembro/'
          WHEN '10' THEN 'Outubro/'
          WHEN '11' THEN 'Novembro/'
          WHEN '12' THEN 'Dezembro/'
           END)  || to_char(epa.inicio::date,'YYYY') as mesano,
       pf.nome_completo AS nome_profissional,
       to_char(epa.inicio,'DD/MM/YYYY - HH24:MI') as data_hora_inicio,
       p.descricao as dsc_procedimento,
       paciente.nome_completo as nome_paciente,
       ce.nome as nome_convenio,
       vp.valor as valor_procedimento, 
       vap.valor_a_pagar as valor_servico,
       CASE WHEN vap.valor_pago_id is null 
       then vap.valor_a_pagar
       else null
       end as valor_repassar,
       CASE WHEN vap.valor_pago_id is not null 
       then vap.valor_a_pagar
       else null 
       end as valor_repassado,
       pj.nome_fantasia as nome_clinica,
       tc.descricao as dsc_tipo_consulta
FROM
     estabelecimentosaude.tb_valor_a_pagar pag
       INNER JOIN estabelecimentosaude.tb_estabelecimento_profissional_agenda epa ON epa.id = pag.agenda_id AND epa.excluido IS FALSE
       INNER JOIN estabelecimentosaude.tb_tipo_consulta tc ON tc.id = epa.tipo_consulta_id -- AND tc.excluido IS FALSE
       INNER JOIN estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id -- AND eep.excluido IS FALSE
       INNER JOIN profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id -- AND ep.excluido IS FALSE
       INNER JOIN profissionalsaude.tb_profissional pr ON pr.id = ep.profissional_id -- AND pr.excluido IS FALSE
       INNER JOIN corporativo.tb_pessoafisica pf ON pf.id = pr.pessoafisica_id -- AND pf.excluido IS FALSE
       INNER JOIN estabelecimentosaude.tb_estabelecimento est ON est.id = eep.estabelecimento_id AND est.excluido IS FALSE
       INNER JOIN corporativo.tb_pessoajuridica AS pj ON pj.id = est.pessoajuridica_id
       INNER JOIN estabelecimentosaude.tb_convenio_estabelecimento ce ON ce.id = epa.convenio_id -- AND ce.excluido IS FALSE
       INNER JOIN corporativo.tb_pessoafisica paciente ON paciente.id = epa.pessoa_paciente_id -- AND paciente.excluido IS FALSE
       INNER JOIN estabelecimentosaude.tb_valor_procedimento vp ON vp.id = epa.valor_procedimento_id -- AND vp.excluido IS FALSE
       INNER JOIN estabelecimentosaude.tb_procedimento p ON p.id = vp.procedimento_id -- AND p.excluido IS FALSE
       INNER JOIN workflow.documento doc ON doc.docid = epa.documento_id
       left JOIN (SELECT vap2.agenda_id, vap2.valor_pago_id, sum(vap2.valor_a_pagar) as valor_a_pagar FROM estabelecimentosaude.tb_valor_a_pagar vap2 where vap2.excluido != true
       group by vap2.agenda_id, vap2.valor_pago_id) vap ON vap.agenda_id = pag.agenda_id
       WHERE " . implode(' AND ', $where) . "
		 {$ORDENA}";


    return $sql;
}
