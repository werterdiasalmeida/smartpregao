<?php
if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'verificarCnpj':
            $p = getParam();
            $mPessoaJuridica = new PessoaJuridica();

            die(json_encode(array(
                'disponivel' => $mPessoaJuridica->isCnpjDisponivel($_POST['cnpj'], $p['id'])
            )));
            break;
    }
}
?>

<div class="row">
    <div class="col-md-9">
        <div class="row">
            <div class="col-md-6">
                <div class="form-group">
                    <label>Razão social</label>
                    <?= campo_textom('razao_social', 'S', 'S', '', 190, 190, '', '', '', '', 0); ?>
                </div>
            </div>
            <div class="col-md-6">
                <div class="form-group">
                    <label>Nome fantasia</label>
                    <?= campo_textom('nome_fantasia', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                </div>
            </div>
        </div>

        <div class="row">
            <div class="col-md-6">
                <div class="form-group">
                    <label>CNPJ</label>
                    <?= campo_textom('cnpj', 'S', 'S', '', 19, 19, '99.999.999/9999-99', '', '', '', 0); ?>
                </div>
            </div>
            
        </div>
    </div>

    <div class="col-md-3">
        <div class="form-group">
            <label>Logo</label>
            <br/>

            <div class="fileinput fileinput-new" data-provides="fileinput">
                <div class="fileinput-new thumbnail" style="width: 150px; height: 100px;">
                    <img src="<?= (!empty($dados['logo_arquivo_id']) ? "../slideshow/slideshow/verimagem.php?arqid={$dados['logo_arquivo_id']}" : '') ?>"
                         id="preview"
                         alt=""/>
                </div>
                <div class="fileinput-preview fileinput-exists thumbnail"
                     style="max-width: 150px; max-height: 100px;"></div>
                <div>
                    <span class="btn default btn-file">
                            <span class="fileinput-new" id="bp_inserir"> Selecionar imagem </span>
                            <span class="fileinput-exists" id="bp_atualizar"> Alterar </span>
                            <input type="file" id="file-logo" name="logo"/>
                    </span>
                    <a href="javascript:;"
                       class="btn red"
                       id="bp_remover"
                       style="display: none;"
                       data-dismiss="fileinput"
                       onclick="remover()">
                        Remover
                    </a>
                </div>
            </div>
            <br/>
            <small>(Imagem com tamanho mínimo de 250x250)</small>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function () {
        localStorage.setItem('remove_logo', '');
        const tem_imagem = '<?= empty($dados['logo_arquivo_id']) ? 'N' : 'S' ?>';
        if (tem_imagem === 'S') {
            $('#bp_inserir').html(' Alterar ');
            $('#bp_remover').show();
        }
        $('#cnpj').change(function () {
            verificarCnpj($(this).val());
        });

        $("#file-logo").change(function (e) {
            localStorage.setItem('remove_logo', '');
            if (e.target.files && e.target.files.length > 0) {
                $('#bp_remover').show();
            }
        });
    });

    function verificarCnpj(cnpj, success, error) {
        success = success ? success : function (result) {
        };
        error = error ? error : function (result) {
        };

        if (cnpj === '') {
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
            dataType: 'json',
            data: {act: 'verificarCnpj', cnpj: cnpj},
            success: function (retorno) {
                App.unblockUI('.form-body');
                if (!retorno.disponivel) {
                    addMsgErroCampo($('#cnpj'), 'O CNPJ informado não está disponível.');
                } else {
                    success(retorno);
                }
            },
            error: function (result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a operação');
            }
        });
    }

    function remover() {
        $('#bp_remover').hide();
        localStorage.setItem('remove_logo', 'S');
        $('#preview').attr("src", '');
        $('#bp_inserir').html(' Selecionar imagem ');
    }
</script>