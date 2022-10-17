<?php
$p = getParam();

$mProfissional = new Profissional($p['profissional_id']);
$mTipoProfissional = new TipoProfissional($mProfissional->tipo_profissional_id);
$mPessoaFisica = new PessoaFisica($mProfissional->pessoafisica_id);
?>
<div class="row">
    <div class="col-md-5">
        <div class="portlet light bordered" style="height: 525px;">
            <div class="portlet-title">
                <div class="caption font-blue-sharp">
                    <i class="icon-users font-blue-sharp"></i>
                    <span class="caption-subject bold uppercase">
                        Profissional
                    </span>
                </div>
            </div>
            <div class="portlet-body form">
                <div class="form-body">
                    <div class="row">
                        <div class="col-md-3 text-center">
                            <?php montaImgFoto($mPessoaFisica->foto_arquivo_id, $mPessoaFisica->sexo, '150px', '', '200', '200', 'img-circle bg-white') ?>
                        </div>
                        <div class="col-md-9">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label>Nome</label>
                                    <h4 class="bold no-margin"><?= $mPessoaFisica->nome_completo ?></h4>
                                </div>
                            </div>
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label>Registro</label>
                                    <h4 class="bold no-margin"><?= "{$mProfissional->registro_profissional}/{$mProfissional->uf_registro}" ?></h4>
                                </div>
                            </div>
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label>Tipo</label>
                                    <h4 class="bold no-margin"><?= $mTipoProfissional->descricao ?></h4>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="col-md-7">
        <div class="portlet light bordered" style="height: 525px;">
            <div class="portlet-title">
                <div class="caption font-blue-sharp">
                    <i class="fa fa-comments font-blue-sharp"></i>
                    <span class="caption-subject bold uppercase">
                        Lembretes e tarefas
                    </span>
                </div>
            </div>
            <div id="tabela-lembrete-tarefa" class="portlet-body form">
                <?php include_once 'lembretesTarefasLista.inc'; ?>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    function atualizarListaLembretesTarefas(){
        App.blockUI({
            message: 'Carregando...',
            target: '#form-lembretes-tarefas'
        });

        $('#tabela-lembrete-tarefa').load(window.location.href, {
            act: 'listar-lembretes-tarefas',
            p: '<?= setParam($p, false) ?>',
        }, function () {
            App.unblockUI('#form-lembretes-tarefas');
        });
    }
</script>