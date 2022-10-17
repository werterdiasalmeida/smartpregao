<?php
$p = getParam();

$mObservacao = new ObservacaoAgenda();
$observacoes = $mObservacao->listarPorAgenda($p['agenda_id'], true);
?>

<table id="tabela-observacoes-agenda" data-toggle="table" class="table table-striped" data-unique-id="id" data-height="365">
    <thead>
    <tr>
        <th data-field="data_inclusao">Data</th>
        <th data-field="observacao">Observação</th>
        <th data-field="usuario_inclusao">Incluído por</th>
    </tr>
    </thead>
    <tbody>
    </tbody>
</table>

<script type="text/javascript">
    $(function(){
        $('#tabela-observacoes-agenda').bootstrapTable({
            data : <?= $observacoes; ?>
        });
    });
</script>