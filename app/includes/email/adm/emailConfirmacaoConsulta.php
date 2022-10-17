<?php
$urlConfirmacaoAtendimento = "{$baseUrl}/confirmacao_agendamento_atendimento.php?p={$hash_agenda_id}";
?>

<div style="text-align: center;">
    <h2 class="font-blue" style="font-weight: bold; color: #3598dc;">Confirme seu atendimento</h2>
</div>
<div style="font-size: 14px; color: #666;">
    <p style="margin: 25px 0 25px 0">
        Olá <b><?= $paciente; ?></b>, tudo bem?
    </p>
    <p style="margin: 25px 0 25px 0">
        Precisamos confirmar seu atendimento que foi agendado para o dia
        <b><?= $data_inicio ?></b>
        no horário de <b><?= $hora_inicio ?></b> às <b><?= $hora_fim ?></b>,
        no estabelecimento <b><?= $nome_fantasia ?></b>.
    </p>
    <p style="margin: 25px 0 25px 0">
        Para confirmar basta clicar no botão abaixo.
    </p>
</div>
<div style="text-align: center;">
    <a style="display: inline-block; margin-bottom: 0; font-weight: 400;
                            text-align: center; vertical-align: middle; touch-action: manipulation;
                            cursor: pointer; white-space: nowrap; text-decoration: unset;
                            padding: 6px 12px; font-size: 14px; line-height: 1.42857; border-radius: 4px;
                            user-select: none; color: #FFF; background-color: #3598dc; border: 1px solid #3598dc;"
       href="<?= $urlConfirmacaoAtendimento ?>">
        Confirmar atendimento
    </a>
</div>