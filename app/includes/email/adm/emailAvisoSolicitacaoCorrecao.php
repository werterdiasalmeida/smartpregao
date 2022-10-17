<div style="text-align: center;">
    <h2 class="font-blue" style="font-weight: bold; color: #3598dc;">Solicitação de alteração de agenda</h2>
</div>
<div style="font-size: 14px; color: #666;">
    <p style="margin: 25px 0 25px 0">
        Olá <b><?= $nome_destinatario; ?></b>, tudo bem?
    </p>
    <p style="margin: 25px 0 25px 0; text-align: justify;">
        O usuário do sistema <b><?= $nome; ?></b> solicitou a alteração das informações de uma consulta agendada para o dia
        <b><?= $data_inicio ?></b> no horário de <b><?= $hora_inicio ?></b> às <b><?= $hora_fim ?></b>,
        no estabelecimento <b><?= $nome_fantasia ?></b>.
    </p>
    <p style="margin: 25px 0 25px 0">
        <b>Descrição da solicitação:</b>
        <p style="text-align: justify"><?= $descricao_solicitacao ?></p>
    </p>
    <p style="margin: 25px 0 25px 0">
        Por favor, visite o sistema e efetue as alterações necessárias.
    </p>
</div>