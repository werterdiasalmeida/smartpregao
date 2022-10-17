<?php
$p = getParam();
$mUsuarioResponsabilidade = new UsuarioResponsabilidadeEstabelecimento();

if(isset($_REQUEST['act']) && !empty($_REQUEST['act'])){
    switch ($_REQUEST['act']){
        case 'salvarResponsabilidade':
            $params = array_merge($_POST, getParam());

            $mUsuarioResponsabilidade->salvarResponsabilidadeEstabelecimento($params);
            $mUsuarioResponsabilidade->commit();

            $labels = array();
            foreach($mUsuarioResponsabilidade->listarPerfisResponsabilidade($p['usucpf'], $_SESSION['sisid']) as $responsabilidade){
                $labels["estabelecimentos_{$responsabilidade['pflcod']}"] = getLabels($responsabilidade['estabelecimentos']);
            }

            $db->sucessomAjax('', $labels);
            die;
            break;
    }
}

$estabelecimento = array();
$estabelecimentos = $mUsuarioResponsabilidade->listarEstabelecimentos($p['usucpf'], $_REQUEST['pflcod'], true);
?>
<table id="tabela-estabelecimentos" data-toggle="table" class="table table-striped" data-unique-id="id"
       data-pagination="true" data-height="300" data-search="true" data-maintain-selected="true">
    <thead>
    <tr>
        <th data-field="selecionado" data-checkbox="true"></th>
        <th data-field="nome_fantasia" data-width="70%">Estabelecimento</th>
        <th data-field="cnpj" data-align="center">CNPJ</th>
    </tr>
    </thead>
    <tbody>
    </tbody>
</table>

<script type="text/javascript">
    $(function () {
        var data = <?= $estabelecimentos ?>.
        map(function (row) {
            if (row.selecionado) {
                row.selecionado = row.selecionado === 't';
            }

            return row;
        });

        setTimeout(function () {
            $('#tabela-estabelecimentos').bootstrapTable({
                data: data,
            }).bootstrapTable('refresh');

            $('.btn-incluir').removeClass('hidden').click(function(){
                var estabelecimentos = $('#tabela-estabelecimentos').bootstrapTable('getData').map(function (row) {
                    if (row.selecionado) {
                        return row.id;
                    }
                }).filter(function(value){
                    return value;
                });

                App.blockUI({
                    message: 'Carregando...',
                    target: '#modal-responsabilidade .modal-content'
                });

                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: {
                        act: 'salvarResponsabilidade',
                        estabelecimentos: estabelecimentos,
                        pflcod : <?= $_REQUEST['pflcod'] ?>
                    },
                    dataType: 'json',
                    success: function (retorno) {
                        $.each(retorno.params, function(responsabilidadeClass, labels){
                            $('.' + responsabilidadeClass).html(labels);
                        });

                        $('#modal-responsabilidade').modal('hide');
                        App.unblockUI('#modal-responsabilidade .modal-content');
                    },
                    error: function (result) {
                        exibirAlert('Não foi possível salvar a responsabilidade.');
                        App.unblockUI('#modal-responsabilidade .modal-content');
                    }
                });
            });
        }, 150);
    });
</script>