<?php
$incluirAnexoDocumentos = isset($incluirAnexoDocumentos) ? $incluirAnexoDocumentos : false;

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'verificarCpf':
            $p = getParam();
            $mPessoaFisica = new PessoaFisica();
            $id = $_POST['pessoa_id'] ? $_POST['pessoa_id'] : $p['id'];
            die(json_encode(array(
                'disponivel' => $mPessoaFisica->isCpfDisponivel($_POST['cpf'], $id)
            )));
            break;
    }
}
?>

<div class="row">
    <div class="col-md-9">
        <div class="row">
            <div class="col-md-12">
                <div class="form-group">
                    <label>Nome completo</label>
                    <?= campo_textom('nome_completo', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                </div>
            </div>
        </div>

        <div class="row">
            <!--<div class="col-md-8">
                <div class="form-group">
                    <label>Nome social</label>
                    <?/*= campo_textom('nome_social', 'N', 'S', '', 70, 70, '', '', '', '', 0); */?>
                </div>
            </div>-->
            <div class="col-md-6">
                <div class="form-group">
                    <label>CPF</label>
                    <?php
                    $cpfHabilitado = $cpf && $id ? 'N' : 'S';
                    if($incluirAnexoDocumentos) : ?>
                        <div class="input-group">
                            <?= campo_textom('cpf', ($cpfObrigatorio ? 'S' : 'N'), $cpfHabilitado, '', 19, 19, '999.999.999-99', '', '', '', 0); ?>
                            <input type="hidden" name="arquivo_cpf" value="<?= $arquivo_cpf ?>" />
                            <span class="input-group-btn">
                                <button class="btn btn-icon-only btn-default blue btn-anexo-documento btn-anexo-cpf" data-type="cpf" type="button" title="Enviar arquivo">
                                    <i class="icon-paper-clip"></i>
                                </button>
                            </span>
                        </div>
                    <?php else : ?>
                        <?= campo_textom('cpf', ($cpfObrigatorio ? 'S' : 'N'), $cpfHabilitado, '', 19, 19, '999.999.999-99', '', '', '', 0); ?>
                    <?php endif; ?>
                </div>
            </div>
            <div class="col-md-6">
                <div class="form-group">
                    <label>Data de nascimento</label>
                    <?= campo_datam('data_nascimento', 'S', 'S', '', 'S'); ?>
                </div>
            </div>
        </div>

        <div class="row">
            <div class="col-md-4">
                <div class="form-group">
                    <label>Sexo</label>
                    <?= campo_radiom('sexo', array("Masculino" => array("valor" => "M"), "Feminino" => array("valor" => "F")), 'S'); ?>
                </div>
            </div>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>Foto</label>
            <br />
            <div class="fileinput fileinput-new" data-provides="fileinput">
                <div class="fileinput-new thumbnail" style="width: 150px; height: 100px;">
                    <?php montaImgFoto($foto_arquivo_id, $sexo); ?>
                </div>
                <div class="fileinput-preview fileinput-exists thumbnail"
                     style="max-width: 150px; max-height: 100px;"></div>
                <div>
                    <span class="btn default btn-file">
                        <span class="fileinput-new"> Selecionar imagem </span>
                        <span class="fileinput-exists"> Alterar </span>
                        <input type="file" name="foto"/>
                    </span>
                    <a href="javascript:;" class="btn red fileinput-exists"
                       data-dismiss="fileinput">
                        Remover </a>
                </div>
            </div>
            <br />
            <small>(Imagem com tamanho mínimo de 250x250)</small>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function() {
        /*$('#cpf').change(function(){
            verificarCpf($(this).val());
        });*/
    });

    function verificarCpf(cpf, success, error){
        success = success ? success : function (result) { };
        error = error ? error : function (result) { };

        if(cpf === ''){
            success();
            return;
        }

        App.blockUI({
            message: 'Carregando...',
            target: '.form-body'
        });

        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType : 'json',
            data: {act : 'verificarCpf', cpf: cpf, pessoa_id: $('[name=pessoafisica_id]').val()},
            success: function (retorno) {
                App.unblockUI('.form-body');
                if(!retorno.disponivel){
                    addMsgErroCampo($('#cpf'), 'O CPF informado não está disponível.');
                }else {
                    success(retorno);
                }
            },
            error: function (result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a operação');
            }
        });
    }
</script>