<?php
$urlConfirmacaoAtendimento = "{$baseUrl}/avaliacao_atendimento.php?p={$hash_agenda_id}";
?>

<div style="text-align: center;">
    <h2 class="font-blue" style="font-weight: bold; color: #3598dc;">Avalie seu atendimento</h2>
</div>
<div style="font-size: 14px; color: #666;">
    <p style="margin: 25px 0 25px 0">
        Olá <b><?= $paciente; ?></b>,
    </p>
    <p style="margin: 25px 0 25px 0">
        Como foi seu atendimento no estabelecimento <b><?= $nome_fantasia ?></b>
        com <?= $sexo_profissional == 'M' ? 'o' : 'a' ?> <b><?= $profissional ?></b>
        no dia <b><?= $data_inicio ?></b> às <b><?= $hora_inicio ?></b>?
    </p>
    <p style="margin: 25px 0 25px 0">
        Precisamos da sua avaliação para melhorar cada vez mais o nosso atendimento.<br>
        Para avaliar basta clicar no botão abaixo.
    </p>
</div>
<div style="text-align: center;">
    <a style="display: inline-block; margin-bottom: 0; font-weight: 400;
                            text-align: center; vertical-align: middle; touch-action: manipulation;
                            cursor: pointer; white-space: nowrap; text-decoration: unset;
                            padding: 6px 12px; font-size: 14px; line-height: 1.42857; border-radius: 4px;
                            user-select: none; color: #FFF; background-color: #3598dc; border: 1px solid #3598dc;"
       href="<?= $urlConfirmacaoAtendimento ?>">
        Avaliar atendimento
    </a>
</div>