<?php
function posReagendar($agendaId)
{
    $mAgenda = new EstabelecimentoProfissionalAgenda($agendaId);
    $agenda = $mAgenda->getDados();
    $mAgenda->setDadosNull();

    $dadosNovaAgenda = array(
        'estabelecimento_especialidade_profissional_id' => $agenda['estabelecimento_especialidade_profissional_id'],
        'inicio' => $agenda['inicio'],
        'fim' => $agenda['fim'],
        'documento_id' => wf_cadastrarDocumento(EstabelecimentoProfissionalAgenda::WF_TIPO_DOCUMENTO, 'Agenda do profissional'),
        'criado_em' => 'now()',
        'criado_por' => $_SESSION['usuario_id']
    );

    $mAgenda->popularDadosObjeto($dadosNovaAgenda)->salvar();
    return true;
}

function posCancelar($agendaId)
{
    $mAgenda = new EstabelecimentoProfissionalAgenda($agendaId);

    if ($mAgenda->pessoa_paciente_id) {
        posReagendar($agendaId);
    }

    return true;
}

function verificarRealizar($agendaId)
{
    $mValorAPagar = new ValorAPagar();
    $registroExistente = $mValorAPagar->recuperarLinha('*', array('excluido IS NOT TRUE', 'agenda_id = ' . (int)$agendaId), false);
    if (!empty($registroExistente)) {
        $mValorAPagar->desabilitarValoresPorAgenda($agendaId);
        // return 'A consulta já foi finalizada!';
    }

    return true;
}

function posRealizar($agendaId)
{
    $mValorAPagar = new ValorAPagar();
    $registroExistente = $mValorAPagar->recuperarLinha('*', array('excluido IS NOT TRUE', 'agenda_id = ' . (int)$agendaId), false);

    if (empty($registroExistente)) {
        $mValorAPagar->gerarValorAgenda($agendaId);
    }

    $mEmail = new NotificacaoAtendimento();
    $mEmail->enviarEmailDeAvaliacaoDoAtendimento($agendaId);

    return true;
}

function posAgendar($agendaId)
{
    $mEmail = new NotificacaoAtendimento();
    $mEmail->enviarEmailDeAtendimentoAgendado($agendaId);

    return true;
}

function posConfirmar($agendaId)
{
    $mEmail = new NotificacaoAtendimento();
    $mEmail->enviarEmailDeAtendimentoConfirmado($agendaId);

    return true;
}

function posRegistrarPresenca($agendaId)
{
    $mGuia = new Guia();
    $mGuia->gerarGuiaByAgenda($agendaId);

    return true;
}

function posGerarLote($loteId)
{
    $mLote = new Lote();
    $mLote->incrementarSequencial($loteId);

    return true;
}