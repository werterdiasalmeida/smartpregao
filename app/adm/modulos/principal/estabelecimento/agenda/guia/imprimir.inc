<?php
$p = getParam();
$mGuia = new Guia();
$guia = $mGuia->getGuiaByAgenda($p['agenda_id']);

$mAgenda = new EstabelecimentoProfissionalAgenda();
$agenda = $mAgenda->getTodosDados($p['agenda_id']);

$mEstabelecimento = new Estabelecimento();
$estabelecimento = $mEstabelecimento->getTodosDados($agenda['estabelecimento_id']);

$mTipoGuia = new TipoGuia();
$tipoGuia = $mTipoGuia->getTipoById($guia['tipo_guia_id']);

switch ($tipoGuia) {
    case TipoGuia::TIPO_GUIA_EXAME :
        $guia['guia']['data_autorizacao'] = formata_data($guia['guia']['data_autorizacao']);
        $guia['guia']['data_validade_senha'] = formata_data($guia['guia']['data_validade_senha']);
        $guia['guia']['validade_carteira'] = formata_data($guia['guia']['validade_carteira']);
        $guia['guia']['data_solicitacao'] = formata_data($guia['guia']['data_solicitacao']);

        $guia['guia']['total_procedimentos'] = formata_valor($guia['guia']['total_procedimentos']);
        $guia['guia']['total_taxas_alugueis'] = formata_valor($guia['guia']['total_taxas_alugueis']);
        $guia['guia']['total_materiais'] = formata_valor($guia['guia']['total_materiais']);
        $guia['guia']['total_opme'] = formata_valor($guia['guia']['total_opme']);
        $guia['guia']['total_medicamentos'] = formata_valor($guia['guia']['total_medicamentos']);
        $guia['guia']['total_gases_medicinais'] = formata_valor($guia['guia']['total_gases_medicinais']);
        $guia['guia']['total_geral'] = formata_valor($guia['guia']['total_geral']);

        $guia['guia']['solicitado'] = array_map(function($solicitado){
            $solicitado['qtd_solicitada'] = formata_valor($solicitado['qtd_solicitada'], 0);
            $solicitado['qtd_autorizada'] = formata_valor($solicitado['qtd_autorizada'], 0);
            return $solicitado;
        }, $guia['guia']['solicitado']);

        $mUf = new UnidadeFederacao();
        $guia['guia']['profissional_executante'] = array_map(function($executado) use ($mUf) {
            $executado['uf_conselho_profissional'] = $mUf->getSiglaByCodIbge($executado['uf_conselho_profissional']);
            return $executado;
        }, $guia['guia']['profissional_executante']);

        $guia['guia']['executado'] = array_map(function($executado){
            $executado['qtd_execucao'] = formata_valor($executado['qtd_execucao'], 0);
            $executado['fator_red_acresc'] = formata_valor($executado['fator_red_acresc']);
            $executado['valor_total'] = formata_valor($executado['valor_total']);
            $executado['valor_unitario'] = formata_valor($executado['valor_unitario']);
            return $executado;
        }, $guia['guia']['executado']);

        $guia['guia']['uf_conselho_profissional'] = $mUf->getSiglaByCodIbge($guia['guia']['uf_conselho_profissional']);
        break;
    default :
        $guia['guia']['validade_carteira'] = formata_data($guia['guia']['validade_carteira']);
        $guia['guia']['data_atendimento'] = formata_data($guia['guia']['data_atendimento']);
        $guia['guia']['valor_procedimento'] = formata_valor($guia['guia']['valor_procedimento']);

        $mUf = new UnidadeFederacao();
        $guia['guia']['uf_conselho_profissional'] = $mUf->getSiglaByCodIbge($guia['guia']['uf_conselho_profissional']);
        break;
}

$mGuiaImpressao = new GuiaImpressao($guia['guia'], $estabelecimento, $tipoGuia);
$mGuiaImpressao->generate();