<?php
$p = getParam();

$mLembreteTarefa = new LembreteTarefa();
$lembretesTarefas = $mLembreteTarefa->listarPorProfissionalEstabelecimento($p['profissional_id'], $p['id'], true);
?>
<style type="text/css">
    .bubble-msg {
        width: 15px;
        height: 15px;
        border-radius: 10px !important;
        background-color: #cd3939;
        color: #FFF;
        font-size: 9px;
        position: absolute;
        top: -5px;
        right: -5px;
        padding: 2px !important;
        text-align: center;
    }
</style>

<div class="row">
    <div class="col-md-12">
        <div class="form-group">
            <div class="input-group">
                <?= campo_textom('lembrete_tarefa', 'S', 'S', '', 90, 1000, '', '', '', '', 0, '', '', null, '', null, null, 'Escreve o lembrete/tarefa aqui'); ?>
                <span class="input-group-btn">
                    <button class="btn blue" type="submit" data-toggle="tooltip" title="Adicionar">
                        <i class="fa fa-plus"></i>
                    </button>
                </span>
            </div>
        </div>
    </div>
</div>

<div class="row">
    <div id="" class="col-md-12">
        <table id="tabela-lembretes-tarefas" data-toggle="table" class="table table-striped" data-unique-id="id" data-height="370">
            <thead>
            <tr>
                <th data-field="data_inclusao_formatada" data-width="10%" data-align="center">Data</th>
                <th data-field="lembrete_tarefa">Lembrete/Tarefa</th>
                <th data-field="usuario_inclusao" data-width="20%">Incluído por</th>
                <th data-formatter="formatAcaoLembreteTarefa" data-width="5%" data-align="center">Ação</th>
            </tr>
            </thead>
            <tbody>
            </tbody>
        </table>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        setTimeout(function(){
            $('[name=lembrete_tarefa]').focus();
        }, 500);

        $('#tabela-lembretes-tarefas').bootstrapTable({
            data : <?= $lembretesTarefas; ?>
        });

        $(function(){
            var form = $('#form-lembretes-tarefas');
            adicionarLabelObrigatorio(form.find(':required'));

            form.validate().destroy();
            form.validateForm({
                rules : {},
                submitHandler: function (e) {
                    App.blockUI({
                        message: 'Carregando...',
                        target: '#form-lembretes-tarefas'
                    });

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType : 'json',
                        data: {
                            p : '<?= setParam(array(
                                'id' => $p['id'],
                                'profissional_id' => $p['profissional_id']
                            ), false) ?>',
                            act : 'adicionar-lembrete-tarefa',
                            lembrete_tarefa : $('#form-lembretes-tarefas').find('[name=lembrete_tarefa]').val()
                        },
                        success: function (retorno) {
                            atualizarListaLembretesTarefas();
                        },
                        error: function (result) {
                            App.unblockUI('#form-lembretes-tarefas');
                            exibirAlert('Não foi possível incluir o lembrete/tarefa');
                        }
                    });
                }
            });
        });
    });

    function formatAcaoLembreteTarefa(value, row, index) {
        var bubbleMsg = '';
        if(row.qtd_msg > 0){
            bubbleMsg = '<div class="bubble-msg">' + (parseInt(row.qtd_msg) + 1) + '</div>';
        }

        return '<span style="position: relative;">'
                + '<a class="btn btn-circle btn-icon-only btn-default yellow-casablanca" href="javascript:exibirMensagensLembreteTarefa(\'' + row.p + '\')" title="Exibir mensagens" style="margin: 2px;">'
                    + '<i class="fa fa-comments"></i>'
                    + bubbleMsg
                + '</a>'
             + '</span>';
    }

    function exibirMensagensLembreteTarefa (p) {
        App.blockUI({
            message: 'Carregando...',
            target: '#form-lembretes-tarefas'
        });

        $('#tabela-lembrete-tarefa').load(window.location.href, {
            act: 'mensagens-lembrete-tarefa',
            p: p
        }, function () {
            App.unblockUI('#form-lembretes-tarefas');
        });
    }
</script>