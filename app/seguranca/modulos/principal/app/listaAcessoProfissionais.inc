<?php
$mProfissional = new Profissional();
$listaProfissionaisApp = $mProfissional->listarProfissionaisAcessoApp($_POST['tipo']);
?>

<table id="tabela-profissionais-acesso-app" data-toggle="table" data-pagination="true" data-page-size="10" data-height="550" data-search="true" class="table table-striped">
    <thead>
    <tr>
        <th data-sortable="true" data-width="15%" data-align="center">CPF</th>
        <th data-sortable="true" data-width="75%">Nome</th>
        <th data-sortable="true" data-width="10%">Email</th>
    </tr>
    </thead>
    <tbody>
        <?php foreach ($listaProfissionaisApp as $linha) : ?>
            <tr>
                <td><?= formatar_cpf($linha['cpf']) ?></td>
                <td><?= $linha['nome'] ?></td>
                <td><?= $linha['email'] ?></td>
            </tr>
        <?php endforeach; ?>
    </tbody>
</table>

<script type="text/javascript">
    $(function() {
        setTimeout(function() {
            $('#tabela-profissionais-acesso-app').bootstrapTable();
        }, 100);
    });
</script>
