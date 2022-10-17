<?php
$p = getParam();

$mLembreteTarefa = new LembreteTarefa();
$lembretesTarefas = $mLembreteTarefa->listarPorLembreteTarefa($p['lembrete_tarefa_id']);
?>
<style type="text/css">
    .chats li.out .message {
        margin-right: 10px;
    }

    .chats li.in .message {
        margin-left: 10px;
    }
</style>
<div class="row">
    <div class="col-md-12">
        <div class="form-group">
            <button type="button" class="btn" onclick="atualizarListaLembretesTarefas();">
                <i class="fa fa-arrow-left"></i>
                Voltar
            </button>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div id="div-msg" style="height: 335px; overflow-y: auto;">
            <ul class="chats">
                <?php foreach($lembretesTarefas as $lembreteTarefa) : ?>
                    <li class="<?= ($lembreteTarefa['usucpf_inclusao'] == $_SESSION['usucpf']) ? 'out' : 'in' ?>">
                        <!--<img class="avatar" alt="" src="../assets/layouts/layout/img/avatar2.jpg" />-->
                        <div class="message">
                            <span class="arrow"> </span>
                            <span class="name"> <?= $lembreteTarefa['usuario_inclusao'] ?> </span>
                            <span class="datetime"> em <strong><?= $lembreteTarefa['data_inclusao_formatada'] ?></strong> </span>
                            <span class="body"><?= $lembreteTarefa['lembrete_tarefa'] ?></span>
                        </div>
                    </li>
                <?php endforeach; ?>
            </ul>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div class="form-group">
            <div class="input-group">
                <?= campo_textom('lembrete_tarefa', 'S', 'S', '', 90, 1000, '', '', '', '', 0, '', '', null, '', null, null, 'Escreve sua mensagem'); ?>
                <span class="input-group-btn">
                    <button class="btn blue" type="submit" data-toggle="tooltip" title="Adicionar">
                        <i class="fa fa-plus"></i>
                    </button>
                </span>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        $('[name=lembrete_tarefa]').focus();

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
                                'profissional_id' => $p['profissional_id'],
                                'lembrete_tarefa_id' => $p['lembrete_tarefa_id'],
                        ), false) ?>',
                        act : 'adicionar-lembrete-tarefa',
                        lembrete_tarefa : $('#form-lembretes-tarefas').find('[name=lembrete_tarefa]').val()
                    },
                    success: function (retorno) {
                        exibirMensagensLembreteTarefa('<?= setParam(getParam(), false); ?>');
                    },
                    error: function (result) {
                        App.unblockUI('#form-lembretes-tarefas');
                        exibirAlert('Não foi possível enviar a mensagem');
                    }
                });
            }
        });

        var divMsg = $('#div-msg');
        divMsg[0].scrollTop = divMsg[0].scrollHeight;
    });
</script>