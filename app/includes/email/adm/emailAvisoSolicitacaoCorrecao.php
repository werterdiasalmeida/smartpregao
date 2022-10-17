<div style="text-align: center;">
    <h2 class="font-blue" style="font-weight: bold; color: #3598dc;">Solicita��o de altera��o de agenda</h2>
</div>
<div style="font-size: 14px; color: #666;">
    <p style="margin: 25px 0 25px 0">
        Ol� <b><?= $nome_destinatario; ?></b>, tudo bem?
    </p>
    <p style="margin: 25px 0 25px 0; text-align: justify;">
        O usu�rio do sistema <b><?= $nome; ?></b> solicitou a altera��o das informa��es de uma consulta agendada para o dia
        <b><?= $data_inicio ?></b> no hor�rio de <b><?= $hora_inicio ?></b> �s <b><?= $hora_fim ?></b>,
        no estabelecimento <b><?= $nome_fantasia ?></b>.
    </p>
    <p style="margin: 25px 0 25px 0">
        <b>Descri��o da solicita��o:</b>
        <p style="text-align: justify"><?= $descricao_solicitacao ?></p>
    </p>
    <p style="margin: 25px 0 25px 0">
        Por favor, visite o sistema e efetue as altera��es necess�rias.
    </p>
</div>