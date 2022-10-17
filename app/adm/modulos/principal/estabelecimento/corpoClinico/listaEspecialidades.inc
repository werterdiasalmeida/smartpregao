<?php
$p = getParam();
$profissionalId = ($_REQUEST['profissional_id'] ? $_REQUEST['profissional_id'] : $p['profissional_id']);
$pParam = setParam(array_merge($p, array('profissional_id' => $profissionalId)), false);
$mEspecialidadeProfissional = new EspecialidadeProfissional();
$especialidades = $mEspecialidadeProfissional->recuperarPorProfissional($profissionalId, true, $p['id']);
?>

<!--data-click-to-select="true"-->
<table id="tabela-especialidades" data-toggle="table" class="table table-striped" data-unique-id="id"
       data-pagination="true" data-height="300">
    <thead>
    <tr>
        <th data-field="selecionado" data-checkbox="true"></th>
        <th data-field="especialidade" data-width="70%">Especialidade</th>
        <th data-field="duracao_consulta" data-formatter="campoDuracaoConsulta" data-width="20%">Duração das consultas <small>(em minutos)</small></th>
    </tr>
    </thead>
    <tbody>
    </tbody>
</table>

<script type="text/javascript">
    $(function () {
        var data = <?= $especialidades ?>.
        map(function (row) {
            if (row.selecionado) {
                row.selecionado = row.selecionado === '1';
            }

            return row;
        });

        setTimeout(function () {
            $('#tabela-especialidades').bootstrapTable({
                data: data,
                onCheck : function (){
                    refreshTable();
                },
                onUncheck : function (){
                    refreshTable();
                },
                onCheckAll : function (){
                    refreshTable();
                },
                onUncheckAll : function (){
                    refreshTable();
                },
                onPostBody : function (){
                    $('.duracao-consulta').TouchSpin({
                        min: 5,
                        max: 120,
                        step: 5
                    });
                }
            }).bootstrapTable('refresh');

            $("#especialidades:visible").closest('.portlet.light.bordered').animate({backgroundColor: "#FFFFCC"}, 1000).animate({backgroundColor: "#f1f4f9"}, 1000);
            $(".modal:visible .btn-incluir").removeClass('hidden').unbind('click')
                .click(function () {
                    var especialidades = $('#tabela-especialidades').bootstrapTable('getData').map(function (row) {
                        if (row.selecionado) {
                            return {
                                id : row.id,
                                duracao_consulta : row.duracao_consulta
                            };
                        }
                    });

                    App.blockUI({
                        message: 'Carregando...',
                        target: '#modal-corpo-clinico .modal-content'
                    });

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        data: {act: 'salvarEspecialidades', especialidades: especialidades, p: '<?= $pParam ?>'},
                        dataType: 'json',
                        success: function (retorno) {
                            App.unblockUI('#modal-corpo-clinico .modal-content');
                            exibirSucesso('Especialidades incluídas com sucesso.');
                            $('#modal-corpo-clinico, #modal-alterar-corpo-clinico').modal('hide');
                            listar();
                        },
                        error: function (result) {
                            exibirAlert('Não foi possível salvar as especialidades.');
                            App.unblockUI('#modal-corpo-clinico .modal-content');
                        },
                    });
                });
        }, 150);
    });

    function campoDuracaoConsulta(value, row, index){
        var disabled = !row.selecionado ? 'disabled' : '';
        row.duracao_consulta = row.duracao_consulta ? row.duracao_consulta : 5;

        return '<input type="text" id="duracao_consulta_' + row.id + '" name="duracao_consulta[' + row.id + ']" '
             + disabled + ' value="' + row.duracao_consulta + '" class="normal  form-control duracao-consulta" '
             + 'required aria-required="true" onchange="atualizarDuracaoConsulta(' + index + ', this);" />';
    }

    function refreshTable(){
        atualizarTabela($('#tabela-especialidades'));
    }

    function atualizarDuracaoConsulta(index, elem){
        var table = $('#tabela-especialidades');
        var dados = table.bootstrapTable('getData');
        var linha = dados[index];
        linha.duracao_consulta = $(elem).val();

        adicionarItemTabela(linha, table, index)
    }
</script>
