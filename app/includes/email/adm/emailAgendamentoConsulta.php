<div style="text-align: center;">
    <h2 class="font-blue" style="font-weight: bold; color: #3598dc;">Seu atendimento foi agendado</h2>
</div>
<div style="font-size: 14px; color: #666;">
    <p style="margin: 25px 0 25px 0">
        Olá <b><?= $paciente; ?></b>, tudo bem?
    </p>
    <p style="margin: 25px 0 25px 0">
        Seu atendimento no estabelecimento <b><?= $nome_fantasia ?></b> foi agendado para o dia
        <b><?= $data_inicio ?></b>
        no horário de <b><?= $hora_inicio ?></b> às <b><?= $hora_fim ?></b>.
    </p>
    <p style="margin: 25px 0 25px 0">
        Na véspera da data marcada enviaremos um email para a confirmação do seu atendimento.
    </p>
</div>