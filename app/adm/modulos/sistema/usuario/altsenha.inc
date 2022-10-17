<?php
/*
  Sistema Simec
  Setor responsável: SPO-MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Gilberto Arruda Cerqueira Xavier
  Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
  Módulo:altsenha.inc
  Finalidade: permitir a alteração de senha
  */

$modulo = $_REQUEST['modulo'];//


if ($_REQUEST['act'] == 'alterar') {
    // checagem de consistencias
    $sql = "select ususenha from seguranca.usuario where usucpf='" . $_SESSION['usucpf'] . "'";

    $senha_banco = $db->recuperar($sql);
    $senha = md5_decrypt_senha($senha_banco['ususenha'], '');
    if ($senha <> $_REQUEST['senhaatual']) {
        // A senha atual não confere
        $db->insucessom($_REQUEST['modulo'], '', 'A senha atual não confere.');
        exit();
    }

}
if (($_REQUEST['act'] == 'alterar') and (!is_array($msgerro))) {
    // fazer alteração da senha.

    $sql = "update usuario set usuchaveativacao = 't', ususenha='" . md5_encrypt_senha($_REQUEST['novasenha'], '') .
        "' where usucpf='" . $_SESSION['usucpf'] . "'";
    $saida = $db->executar($sql);
    $db->commit();
    $db->sucessom($modulo);

}
include APPRAIZ . "includes/cabecalhom.inc";

monta_titulom('Alteração de senha');
?>


<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-red-sunglo">
            <i class="icon-key font-red-sunglo"></i>
            <span class="caption-subject bold uppercase">
                <?= ($deveAlterarSenha ? "Por favor, efetue a alteração da sua senha" : 'Realizar alteração de senha' )   ?>
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="altsenha-form" method="POST">
            <div class="form-body">
                <div class="form-group">
                    <label>Senha atual</label>
                    <input class="form-control spinner" type="password" placeholder="Informe a sua senha atual" name="senhaatual" />
                </div>
                <div class="form-group">
                    <label>Nova senha</label>
                    <input class="form-control spinner" type="password" placeholder="Informe a nova senha" name="novasenha" id="novasenha" />
                </div>
                <div class="form-group">
                    <label>Confirmação de senha</label>
                    <input class="form-control spinner" type="password" placeholder="Confirme sua nova senha" name="confsenha" />
                </div>

                <div class="form-actions">
                    <button type="submit" class="btn blue">Atualizar</button>
                </div>

                <input type=hidden name="modulo" value="<?= $modulo ?>" />
                <input type=hidden name="act" value="alterar" />
            </div>
        </form>
</div>

<script type="text/javascript">
    $(function(){
        $(".altsenha-form").validate({
            errorElement: "span",
            errorClass: "help-block",
            focusInvalid: true,
            ignore: "",
            rules: {
                senhaatual: {required: true},
                novasenha: {required: true, minlength: 8},
                confsenha: {equalTo: "#novasenha"}
            },
            messages: {},
            invalidHandler: function (e, r) {
            },
            highlight: function (e) {
                $(e).closest(".form-group").addClass("has-error")
            },
            success: function (e) {
                e.closest(".form-group").removeClass("has-error");
                e.remove();
            },
            errorPlacement: function (e, r) {
                1 === r.closest(".input-icon").size() ? e.insertAfter(r.closest(".input-icon")) : e.insertAfter(r);
            },
            submitHandler: function (e) {
                e.submit();
            }
        });

        $(".altsenha-form input").keypress(function (e) {
            return 13 == e.which ? ($(".altsenha-form").validate().form() && $(".altsenha-form").submit(), false) : void 0;
        });
    });
</script>
