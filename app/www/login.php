<?php
$uid = (int) $_REQUEST['uid'];

// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'www/adm/_constantes.php';
// abre conexão com o servidor de banco de dados
$db = new cls_banco();
if (isset($_SESSION['MSG_AVISO']) && !is_array($_SESSION['MSG_AVISO'])) {
    $_SESSION['MSG_AVISO'] = array(
        $_SESSION['MSG_AVISO']
    );
}

//redireciona para a tela inicial do sistema caso esteja logado (pelo sisid)
if (!$_POST && (verificarUsuarioLogado())) {
    header("Location: muda_sistema.php?sisid={$_SESSION['sisid']}");
    die();
}

if (isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'login':
            // executa a rotina de autenticação quando o formulário for submetido
            include APPRAIZ . "includes/autenticar.inc";
            break;
        case 'verificar':
            die(json_encode(array('cpf' => isCpfDisponivel($_POST['usucpf']), 'email' => isEmailDisponivel($_POST['usuemail']))));
            break;
        case 'recuperar':
            // verifica se a conta está ativa
            $sql = sprintf(
                "SELECT u.* FROM seguranca.usuario u WHERE (u.usucpf = '%s' OR u.usuemail = '%s')",
                corrige_cpf($_REQUEST['usucpf']),
                $_REQUEST['usucpf']
            );
            $usuario = (object)$db->pegaLinha($sql);

            if ($usuario->suscod != 'A') {
                $_SESSION['MSG_AVISO'][] = "A conta não está ativa.";
                header("Location: /");
                die;
            }

            $_SESSION['mnuid'] = 10;
            $_SESSION['sisid'] = 4;
            $_SESSION['exercicio_atual'] = $db->pega_ano_atual();
            $_SESSION['usucpf'] = $usuario->usucpf;
            $_SESSION['usucpforigem'] = $usuario->usucpf;

            // cria uma nova senha
            $senha = strtoupper(senha());
            $sql = sprintf(
                "UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f', usumigrado = 'f' WHERE usucpf = '%s'",
                md5_encrypt_senha($senha, ''),
                $usuario->usucpf
            );
            $db->executar($sql);

            // envia email de confirmação
            $destinatario = $usuario->usuemail;
            $assunto = "Recuperação de Senha";
            $conteudo = sprintf(
                "%s %s<br/>Sua senha foi alterada para %s<br>Ao se conectar, altere esta senha para uma de sua preferência.",
                $usuario->ususexo == 'F' ? 'Prezada Sra.' : 'Prezado Sr.',
                $usuario->usunome,
                $senha
            );

            $remetente = array();
            $remetente['email'] = $email_from;
            $remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];
            enviar_email($remetente, $destinatario, $assunto, $conteudo);

            $db->commit();
            $_SESSION = array();
            $_SESSION['MSG_AVISO'][] = "Recuperação de senha concluída. Em breve você receberá uma nova senha por email.";
            header("Location: /");
            exit();
            break;
        case 'cadastrar':
            $arMsgCadastro = array();
            if (empty($_POST['usunome'])) {
                $arMsgCadastro[] = "Nome completo não informado.";
            }

            if (!validaCPF($_POST['usucpf'])) {
                $arMsgCadastro[] = "O CPF informado é inválido.";
            } else if (!isCpfDisponivel($_POST['usucpf'])) {
                $arMsgCadastro[] = "O CPF informado já está sendo utilizado.";
            }

            if (empty($_POST['usudatanascimento']) || !verifica_data($_POST['usudatanascimento'])) {
                $arMsgCadastro[] = "A data de nascimento informada é inválida.";
            }

            if (empty($_POST['ususexo'])) {
                $arMsgCadastro[] = "Sexo não informado.";
            }

            if (empty($_POST['usufonenum'])) {
                $arMsgCadastro[] = "Telefone não informado.";
            }

            if (empty($_POST['usuemail'])) {
                $arMsgCadastro[] = "O email informado é inválido.";
            } else if (!isEmailDisponivel($_POST['usuemail'])) {
                $arMsgCadastro[] = "O email informado já está sendo utilizado.";
            }

            if (strlen($_POST['ususenha']) < 8 || $_POST['ususenha'] !== $_POST['rususenha']) {
                $arMsgCadastro[] = "A senha informada é inválida.";
            }

            if (count($arMsgCadastro) == 0) {
                $sisidAdm = ADM_SISID;
                $pflcodOperador = ADM_PERFIL_GERAL;
                $usucpf = str_replace(array('.', '-'), '', $_POST['usucpf']);
                $ususenha = md5_encrypt_senha($_POST['ususenha'], '');
                $usudatanascimento = formata_data_sql($_POST['usudatanascimento']);

                $_SESSION['sisid'] = $sisidAdm;
                $_SESSION['usucpf'] = $usucpf;
                $_SESSION['usucpforigem'] = $_SESSION['usucpf'];
                $uid = intval($uid);
                $uid = $uid ? $uid : 'null';

                $sqlUsuario = "INSERT INTO seguranca.usuario(
                                  usucpf,
                                  usunome,
                                  usuemail,
                                  usustatus,
                                  usufonenum,
                                  ususenha,
                                  ususexo,
                                  usudatainc,
                                  suscod,
                                  usudatanascimento,
                                  usucodigoindicacao
                                  )
                               VALUES (
                                  '{$usucpf}',
                                  '{$_POST['usunome']}',
                                  '{$_POST['usuemail']}',
                                  'A',
                                  '{$_POST['usufonenum']}',
                                  '{$ususenha}',
                                  '{$_POST['ususexo']}',
                                  now(),
                                  'A',
                                  '{$usudatanascimento}',
                                  {$uid}
                              );";

                $db->executar($sqlUsuario);

                /*$sqlUsuarioSistema = "INSERT INTO seguranca.usuario_sistema(
                                          usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
                                      VALUES ('{$usucpf}', {$sisidAdm}, 'P', {$pflcodOperador}, null, 'A');";
                $db->executar($sqlUsuarioSistema);

                $sqlPerfil = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$usucpf}', {$pflcodOperador});";
                $db->executar($sqlPerfil);*/

                $cpf = formatar_cpf($usucpf);
                $sql = "SELECT 
                      COUNT(*)
                    FROM 
                      profissionalsaude.tb_profissional prof
                    INNER JOIN
                      corporativo.tb_pessoafisica pes ON prof.pessoafisica_id = pes.id
                    WHERE
                      prof.excluido IS FALSE
                      AND
                      pes.excluido IS FALSE
                      AND
                      pes.cpf = '{$cpf}'";

                if ($db->pegaUm($sql) > 0) {
                    $pflcodMedico = ADM_PERFIL_PROFISSIONAL_SAUDE;
                    $sqlPerfil = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$usucpf}', {$pflcodMedico});";
                    $db->executar($sqlPerfil);

                    $sqlUsuarioSistema = "INSERT INTO seguranca.usuario_sistema(
                                          usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
                                      VALUES ('{$usucpf}', {$sisidAdm}, 'A', {$pflcodMedico}, null, 'A');";
                    $db->executar($sqlUsuarioSistema);
                }

                $db->commit();

                $destinatario = $_POST['usuemail'];
                $assunto = "Cadastro no ClubVida";

                $msgCadastro = 'Seu cadastro foi efetuado com sucesso, aguarde a liberação do seu acesso.';
                $conteudo = "Prezado(a) Sr(a). {$_POST['usunome']}<br /><br />
                		     {$msgCadastro}";

                $remetente = array();
                $remetente['email'] = $email_from;
                $remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];
                enviar_email($remetente, $destinatario, $assunto, $conteudo);

                /*$_POST['usucpf'] = $usucpf;
                $_SESSION['logincadastro'] = true;
                include APPRAIZ . "includes/autenticar.inc";*/
                $_SESSION['MSG_AVISO'][] = $msgCadastro;
            }

            break;
    }
}

if ($_REQUEST['expirou']) {
    $_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}
?>
<!DOCTYPE html>
<!--[if IE 8]>
<html lang="en" class="ie8 no-js"> <![endif]-->
<!--[if IE 9]>
<html lang="en" class="ie9 no-js"> <![endif]-->
<!--[if !IE]><!-->
<html lang="en">
<!--<![endif]-->
<!-- BEGIN HEAD -->

<head>
    <meta charset="ISO-8859-1" />
    <title>Smart Pregão</title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <meta content="width=device-width, initial-scale=1" name="viewport" />
    <meta content="" name="description" />
    <meta content="" name="author" />
    <!-- BEGIN GLOBAL MANDATORY STYLES -->
    <link href="//fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css" rel="stylesheet" type="text/css" />
    <!-- END GLOBAL MANDATORY STYLES -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <link href="/includes/metronic/global/plugins/select2/css/select2.min.css" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/select2/css/select2-bootstrap.min.css" rel="stylesheet" type="text/css" />
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL STYLES -->
    <link href="/includes/metronic/global/css/components.min.css" rel="stylesheet" id="style_components" type="text/css" />
    <link href="/includes/metronic/global/css/plugins.min.css" rel="stylesheet" type="text/css" />
    <!-- END THEME GLOBAL STYLES -->
    <!-- BEGIN PAGE LEVEL STYLES -->
    <link href="/includes/metronic/pages/css/login.min.css" rel="stylesheet" type="text/css" />
    <!-- END PAGE LEVEL STYLES -->
    <!-- BEGIN THEME LAYOUT STYLES -->
    <!-- END THEME LAYOUT STYLES -->
    <link rel="shortcut icon" href="/favicon.ico?v=<?= VERSAO ?>" />
</head>
<!-- END HEAD -->
<style>
    img {
        border-radius: 10px !important;
    }
</style>

<body class=" login">
    <!-- BEGIN LOGO -->
    <div class="logo">
        <a href="/">
            <img src="/imagens/logo2.png?v=<?= VERSAO ?>" style="height: 65px;" alt="" /> </a>
    </div>
    <!-- END LOGO -->
    <!-- BEGIN LOGIN -->
    <div class="content">
        <!-- BEGIN LOGIN FORM -->
        <form class="login-form" action="/" method="post">
            <h3 class="form-title font-blue">Acessar</h3>

            <?php if (isset($_SESSION['MSG_AVISO']) && is_array($_SESSION['MSG_AVISO']) && count($_SESSION['MSG_AVISO']) > 0) : ?>
                <div class="alert alert-danger">
                    <button class="close" data-close="alert"></button>
                    <span><?= implode('</span><br /><span>', $_SESSION['MSG_AVISO']); ?></span>
                </div>
            <?php
                unset($_SESSION['MSG_AVISO']);
            endif;
            ?>

            <div class="alert alert-danger display-hide alert-validacao">
                <button class="close" data-close="alert"></button>
                <span> Informe seus dados de acesso. </span>
            </div>
            <div class="form-group">
                <!--ie8, ie9 does not support html5 placeholder, so we just show field title for that-->
                <label class="control-label visible-ie8 visible-ie9">Email</label>
                <input class="form-control form-control-solid placeholder-no-fix" type="text" autocomplete="off" placeholder="Email ou CPF" name="usucpf" />
            </div>
            <div class="form-group">
                <label class="control-label visible-ie8 visible-ie9">Senha</label>
                <input class="form-control form-control-solid placeholder-no-fix" type="password" autocomplete="off" placeholder="Senha" name="ususenha" />
            </div>
            <div class="form-actions">
                <button type="submit" class="btn blue uppercase">Entrar</button>
                <a href="javascript:;" id="forget-password" class="forget-password">Esqueceu sua senha?</a>
                <br/><br/>
            </div>
            <div class="create-account">
                <p>
                    <a href="javascript:;" id="register-btn" class="uppercase">Criar uma conta Grátis</a>
                </p>
            </div>
            <input type="hidden" name="act" value="login" />
        </form>
        <!-- END LOGIN FORM -->
        <!-- BEGIN FORGOT PASSWORD FORM -->
        <form class="forget-form" action="/" method="post">
            <h3 class="font-blue">Esqueceu sua senha?</h3>

            <p> Informe seu email ou CPF abaixo. </p>

            <div class="alert alert-danger display-hide alert-validacao">
                <button class="close" data-close="alert"></button>
                <span> Informe seu email. </span>
            </div>
            <div class="form-group">
                <input class="form-control placeholder-no-fix" type="text" autocomplete="off" placeholder="Email ou CPF" name="usucpf" />
            </div>
            <div class="form-actions">
                <button type="button" id="back-btn" class="btn blue btn-outline">Voltar</button>
                <button type="submit" class="btn btn-success uppercase pull-right">Recuperar Senha</button>
            </div>
            <input type="hidden" name="act" value="recuperar" />
        </form>
        <!-- END FORGOT PASSWORD FORM -->
        <!-- BEGIN REGISTRATION FORM -->
        <form class="register-form" action="/" method="post">
            <h3 class="font-blue">Criar conta</h3>

            <?php if (isset($arMsgCadastro) && is_array($arMsgCadastro) && count($arMsgCadastro) > 0) : ?>
                <div class="alert alert-danger">
                    <button class="close" data-close="alert"></button>
                    <span>
                        <?= implode('</span><br /><span>', $arMsgCadastro); ?>
                    </span>
                </div>
            <?php endif; ?>

            <div class="form-group">
                <label class="control-label visible-ie8 visible-ie9">Nome Completo</label>
                <input class="form-control placeholder-no-fix" type="text" placeholder="Nome Completo" name="usunome" maxlength="50" value="<?= isset($_POST['usunome']) ? $_POST['usunome'] : ''; ?>" />
            </div>
            <div class="form-group">
                <label class="control-label visible-ie8 visible-ie9">CPF</label>
                <input class="form-control placeholder-no-fix" type="text" placeholder="CPF" name="usucpf" value="<?= isset($_POST['usucpf']) ? $_POST['usucpf'] : ''; ?>" />
            </div>
            <div class="form-group">
                <label class="control-label visible-ie8 visible-ie9">Data de nascimento</label>
                <input class="form-control placeholder-no-fix" type="text" placeholder="Data de nascimento" name="usudatanascimento" value="<?= isset($_POST['usudatanascimento']) ? $_POST['usudatanascimento'] : ''; ?>" />
            </div>
            <div class="form-group">
                <label class="control-label visible-ie8 visible-ie9">Sexo</label>
                <div class="mt-radio-inline">
                    <label class="mt-radio">
                        <input type="radio" name="ususexo" value="M" <?= (isset($_POST['ususexo']) && $_POST['ususexo'] == 'M' ? 'checked' : ''); ?>> Masculino
                        <span></span>
                    </label>
                    <label class="mt-radio">
                        <input type="radio" name="ususexo" value="F" <?= (isset($_POST['ususexo']) && $_POST['ususexo'] == 'F' ? 'checked' : ''); ?>> Feminino
                        <span></span>
                    </label>
                </div>
            </div>
            <div class="form-group">
                <label class="control-label visible-ie8 visible-ie9">Telefone</label>
                <input class="form-control placeholder-no-fix" type="text" placeholder="Telefone" name="usufonenum" value="<?= isset($_POST['usufonenum']) ? $_POST['usufonenum'] : ''; ?>" />
            </div>
            <div class="form-group">
                <!--ie8, ie9 does not support html5 placeholder, so we just show field title for that-->
                <label class="control-label visible-ie8 visible-ie9">Email</label>
                <input class="form-control placeholder-no-fix" type="text" placeholder="Email" name="usuemail" maxlength="100" value="<?= isset($_POST['usuemail']) ? $_POST['usuemail'] : ''; ?>" />
            </div>

            <div class="form-group">
                <label class="control-label visible-ie8 visible-ie9">Senha</label>
                <input class="form-control placeholder-no-fix" type="password" autocomplete="off" id="ususenha" placeholder="Senha" name="ususenha" maxlength="50" />
            </div>
            <div class="form-group">
                <label class="control-label visible-ie8 visible-ie9">Confirmar senha</label>
                <input class="form-control placeholder-no-fix" type="password" autocomplete="off" placeholder="Confirmar senha" name="rususenha" />
            </div>
            <div class="form-actions">
                <button type="button" id="register-back-btn" class="btn blue btn-outline">Voltar</button>
                <button type="submit" id="register-submit-btn" class="btn btn-success uppercase pull-right">Criar Conta</button>
            </div>
            <input type="hidden" name="act" value="cadastrar" />
            <input type="hidden" name="uid" value="<?= $uid; ?>" />
        </form>
        <!-- END REGISTRATION FORM -->
    </div>
    <div class="copyright">
        2022 © Smart Pregão
    </div>
    <!--[if lt IE 9]>
<script src="/includes/metronic/global/plugins/respond.min.js"></script>
<script src="/includes/metronic/global/plugins/excanvas.min.js"></script>
<![endif]-->
    <!-- BEGIN CORE PLUGINS -->
    <script src="/includes/metronic/global/plugins/jquery.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/js.cookie.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-hover-dropdown/bootstrap-hover-dropdown.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-slimscroll/jquery.slimscroll.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery.blockui.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-switch/js/bootstrap-switch.min.js" type="text/javascript"></script>
    <!-- END CORE PLUGINS -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <script src="/includes/metronic/global/plugins/jquery-validation/js/jquery.validate.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-validation/js/additional-methods.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/select2/js/select2.full.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-inputmask/jquery.inputmask.bundle.min.js" type="text/javascript"></script>
    <script src="/includes/funcoes.js" type="text/javascript"></script>
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN PAGE LEVEL SCRIPTS -->
    <script type="text/javascript">
        $(function() {
            /* LOGIN */
            $(".login-form").validate({
                errorElement: "span",
                errorClass: "help-block",
                focusInvalid: false,
                rules: {
                    usucpf: {
                        required: true
                    },
                    ususenha: {
                        required: true
                    }
                },
                messages: {
                    usucpf: {
                        required: "Email é obrigatório."
                    },
                    ususenha: {
                        required: "Senha é obrigatória."
                    }
                },
                invalidHandler: function(e, r) {
                    $(".alert-validacao", $(".login-form")).show()
                },
                highlight: function(e) {
                    $(e).closest(".form-group").addClass("has-error")
                },
                success: function(e) {
                    e.closest(".form-group").removeClass("has-error");
                    e.remove();
                },
                errorPlacement: function(e, r) {
                    e.insertAfter(r.closest(".input-icon"));
                },
                submitHandler: function(e) {
                    e.submit()
                }
            });

            $(".login-form input").keypress(function(e) {
                return 13 == e.which ? ($(".login-form").validate().form() && $(".login-form").submit(), false) : void 0;
            });

            jQuery("#forget-password").click(function() {
                jQuery(".login-form").hide()[0].reset();
                jQuery(".forget-form").show()[0].reset();
            });

            jQuery("#back-btn").click(function() {
                jQuery(".login-form").show()[0].reset();
                jQuery(".forget-form").hide()[0].reset();
            });
            /* LOGIN */

            /* ESQUECEU SENHA */
            $(".forget-form").validate({
                errorElement: "span",
                errorClass: "help-block",
                focusInvalid: false,
                ignore: "",
                rules: {
                    usucpf: {
                        required: true
                    }
                },
                messages: {
                    usucpf: {
                        required: "Email é obrigatório."
                    }
                },
                invalidHandler: function(e, r) {
                    $(".alert-validacao", $(".forget-form")).show()
                },
                highlight: function(e) {
                    $(e).closest(".form-group").addClass("has-error");
                },
                success: function(e) {
                    e.closest(".form-group").removeClass("has-error");
                    e.remove();
                },
                errorPlacement: function(e, r) {
                    e.insertAfter(r.closest(".input-icon"))
                },
                submitHandler: function(e) {
                    e.submit()
                }
            });

            $(".forget-form input").keypress(function(e) {
                return 13 == e.which ? ($(".forget-form").validate().form() && $(".forget-form").submit(), false) : void 0
            });
            /* ESQUECEU SENHA */

            /* CADASTRAR USUÁRIO */
            $(".register-form").validate({
                errorElement: "span",
                errorClass: "help-block",
                focusInvalid: true,
                ignore: "",
                rules: {
                    usunome: {
                        required: true
                    },
                    usucpf: {
                        required: true,
                        cpf: true
                    },
                    usudatanascimento: {
                        required: true,
                        dateITA: true
                    },
                    ususexo: {
                        required: true
                    },
                    usufonenum: {
                        required: true
                    },
                    usuemail: {
                        required: true,
                        email: true
                    },
                    ususenha: {
                        required: true,
                        minlength: 8
                    },
                    rususenha: {
                        equalTo: "#ususenha"
                    }
                },
                messages: {},
                invalidHandler: function(e, r) {},
                highlight: function(e) {
                    $(e).closest(".form-group").addClass("has-error")
                },
                success: function(e) {
                    e.closest(".form-group").removeClass("has-error");
                    e.remove();
                },
                errorPlacement: function(e, r) {
                    switch (r.attr('name')) {
                        case 'ususexo':
                            1 === r.closest(".mt-radio-inline").size() ? e.insertAfter(r.closest(".mt-radio-inline")) : e.insertAfter(r);
                            break;
                        default:
                            1 === r.closest(".input-icon").size() ? e.insertAfter(r.closest(".input-icon")) : e.insertAfter(r);
                    }
                },
                submitHandler: function(e) {
                    var form = $('.register-form');
                    var btnSubmit = form.find('button[type=submit]');
                    var btnValue = btnSubmit.html();
                    btnSubmit.prop('disabled', true).html('Criando sua conta...');

                    $.ajax({
                        method: 'POST',
                        url: 'login.php',
                        dataType: 'json',
                        data: {
                            act: 'verificar',
                            usucpf: form.find('[name=usucpf]').val(),
                            usuemail: form.find('[name=usuemail]').val()
                        },
                        success: function(data) {
                            if (data && data.cpf && data.email) {
                                e.submit();
                            } else if (!data) {
                                alertCadastro('Não foi possível criar sua conta. Por favor tente novamente mais tarde.');
                                btnSubmit.prop('disabled', false).html(btnValue);
                            } else {
                                if (!data.cpf) {
                                    var campoCpf = form.find('[name=usucpf]');
                                    campoCpf.after('<span id="usucpf-error" class="help-block">O CPF informado já está sendo utilizado.</span>');
                                    campoCpf.closest('.form-group').addClass('has-error');
                                }

                                if (!data.email) {
                                    var campoEmail = form.find('[name=usuemail]');
                                    campoEmail.after('<span id="usuemail-error" class="help-block">O Email informado já está sendo utilizado.</span>');
                                    campoEmail.closest('.form-group').addClass('has-error');
                                }

                                btnSubmit.prop('disabled', false).html(btnValue);
                            }
                        },
                        error: function(data) {
                            btnSubmit.prop('disabled', false).html(btnValue);
                            alertCadastro('Não foi possível criar sua conta. Por favor tente novamente mais tarde.');
                            console.error(data);
                        },
                    });
                }
            });

            $(".register-form input").keypress(function(e) {
                return 13 == e.which ? ($(".register-form").validate().form() && $(".register-form").submit(), false) : void 0;
            });

            jQuery("#register-btn").click(function() {
                jQuery(".login-form").hide();
                jQuery(".register-form").show();
            });

            jQuery("#register-back-btn").click(function() {
                jQuery(".login-form").show();
                jQuery(".register-form").hide();
            });
            /* CADASTRAR USUÁRIO */

            $("form.register-form [name=usucpf]").inputmask("999.999.999-99");

            $("form.register-form [name=usudatanascimento]").inputmask({
                mask: ["d/m/y"],
                placeholder: 'DD/MM/YYYY'
            });

            $("form.register-form [name=usufonenum]").inputmask({
                mask: ["(99) 9999-9999", "(99) 99999-9999"],
            });

            <?php if (isset($arMsgCadastro) && count($arMsgCadastro) > 0) : ?>
                jQuery(".login-form").hide();
                jQuery(".register-form").show();
            <?php endif; ?>

            <?php if (isset($_GET['criar-conta'])) : ?>
                jQuery("#register-btn").click();
            <?php endif; ?>

        });

        function alertCadastro(msg) {
            var alert = $('.register-form .alert-cadastro');
            alert.find('span').html(msg);
            alert.show();
        }
    </script>
    <!-- END PAGE LEVEL SCRIPTS -->
    <!-- BEGIN THEME LAYOUT SCRIPTS -->
    <!-- END THEME LAYOUT SCRIPTS -->
</body>

</html>
<?php


function isCpfDisponivel($cpf)
{
    global $db;
    $cpf = str_replace(array('.', '-'), '', $cpf);

    $sqlCpf = "SELECT count(*) FROM seguranca.usuario WHERE usucpf = '{$cpf}';";
    $qtdCpf = $db->pegaUm($sqlCpf);
    return $qtdCpf === '0';
}

function isEmailDisponivel($email)
{
    global $db;

    $sqlEmail = "SELECT count(*) FROM seguranca.usuario WHERE usuemail = '{$email}'";
    $qtdEmail = $db->pegaUm($sqlEmail);
    return $qtdEmail === '0';
}
