<?php
$p = getParam();
$mAgenda = new EstabelecimentoProfissionalAgenda($p['agenda_id']);

$mObservacaoFisica = new ObservacaoPessoaFisica();
$observacoes = $mAgenda->pessoa_paciente_id ? $mObservacaoFisica->listarPorPessoa($mAgenda->pessoa_paciente_id, true) : array();
?>

<table id="tabela-observacoes-pessoa" data-toggle="table" class="table table-striped" data-unique-id="id" data-height="365">
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
        $('#tabela-observacoes-pessoa').bootstrapTable({
            data : <?= $observacoes; ?>
        });
    });
</script>