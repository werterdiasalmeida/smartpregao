<div style="text-align: center;">
    <h2 class="font-blue" style="font-weight: bold; color: #3598dc;">Seu atendimento foi agendado</h2>
</div>
<div style="font-size: 14px; color: #666;">
    <p style="margin: 25px 0 25px 0">
        Ol� <b><?= $paciente; ?></b>, tudo bem?
    </p>
    <p style="margin: 25px 0 25px 0">
        Seu atendimento no estabelecimento <b><?= $nome_fantasia ?></b> foi agendado para o dia
        <b><?= $data_inicio ?></b>
        no hor�rio de <b><?= $hora_inicio ?></b> �s <b><?= $hora_fim ?></b>.
    </p>
    <p style="margin: 25px 0 25px 0">
        Na v�spera da data marcada enviaremos um email para a confirma��o do seu atendimento.
    </p>
</div>