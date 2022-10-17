<?php
$p = getParam();
$mProfissional = new Profissional($p['profissional_id']);
$mTipoProfissional = new TipoProfissional($mProfissional->tipo_profissional_id);
$mPessoaFisica = new PessoaFisica($mProfissional->pessoafisica_id);
?>

<div class="row">
    <div class="col-md-5">
        <div class="portlet light bordered" style="min-height: 365px;">
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
                        <div class="col-sm-12 col-md-6 col-lg-5 text-center">
                            <?php montaImgFoto($mPessoaFisica->foto_arquivo_id, $mPessoaFisica->sexo, '150px', '', '200', '200', 'img-circle bg-white') ?>
                        </div>
                        <div class="col-sm-12 col-md-6 col-lg-7">
                            <div class="row">
                                <div class="col-md-12">
                                    <div class="form-group">
                                        <label>Nome</label>
                                        <h4 class="bold no-margin"><?= $mProfissional->getPrefixoProfissional($p['profissional_id']); ?></h4>
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
    </div>

    <div class="col-md-7">
        <div class="portlet light bordered" style="height: 365px;">
            <div class="portlet-title">
                <div class="caption font-blue-sharp">
                    <i class="fa fa-user-md font-blue-sharp"></i>
                    <span class="caption-subject bold uppercase">
                        Selecione as especialidades
                    </span>
                </div>
            </div>
            <div class="portlet-body form">
                <?php include_once 'listaEspecialidades.inc'; ?>
            </div>
        </div>
    </div>
</div>