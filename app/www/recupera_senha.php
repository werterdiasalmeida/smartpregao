<?php
include "includes_publico.inc";

$remetente = array();
$remetente['email'] = $email_from;
$remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];

function erro(){
    global $db;
    $db->commit();
    $_SESSION = array();
    $_SESSION['MSG_AVISO'] = func_get_args();
    header( "Location: login.php" );
    exit();
}

// executa a rotina de recuperação de senha quando o formulário for submetido
if ($_POST['formulario']) {

    // verifica se a conta está ativa
    $sql = sprintf(
        "SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = '%s'",
        corrige_cpf($_REQUEST['usucpf'])
    );
    $usuario = (object)$db->pegaLinha($sql);
    if ($usuario->suscod != 'A') {
        erro("A conta não está ativa.");
    }

    $_SESSION['mnuid'] = 10;
    $_SESSION['sisid'] = 4;
    $_SESSION['exercicio_atual'] = $db->pega_ano_atual();
    $_SESSION['usucpf'] = $usuario->usucpf;
    $_SESSION['usucpforigem'] = $usuario->usucpf;

    // cria uma nova senha
    //$senha = $db->gerar_senha();
    $senha = strtoupper(senha());
    $sql = sprintf(
        "UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f' WHERE usucpf = '%s'",
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

    enviar_email($remetente, $destinatario, $assunto, $conteudo);

    $db->commit();
    $_SESSION = array();
    $_SESSION['MSG_AVISO'][] = "Recuperação de senha concluída. Em breve você receberá uma nova senha por email.";
    header("Location: /");
    exit();
}

if ($_REQUEST['expirou']) {
    $_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}

include "cabecalho_publico.inc";
?>

    <div id="caixa-login">
        <div class="conteudo">
            <form method="post" name="formulario">
                <input type=hidden name="formulario" value="1"/>
                <input type=hidden name="modulo" value="./inclusao_usuario.php"/>

                <div class="col-md-12 col-sm-12">
                    <div class="col-md-3 col-sm-2">

                    </div>
                    <div id="bem_vindo_2" class="col-md-6 col-sm-8">
                        &nbsp;
                    </div>
                    <div class="col-md-3 col-sm-2">

                    </div>
                </div>
                <div class="col-md-3 col-sm-2">

                </div>
                <div class="col-md-6 col-sm-8">
                    <div id="container-publico">
                        <div id="caixa-formulario" style="max-width: 500px;">
                            <table border="0" align="center" cellpadding="0" cellspacing="0" class="tabela_modulos">
                                <tr>
                                    <td class="td_bg">
                                        Recupere a Senha
                                    </td>
                                </tr>
                                <tr>
                                    <td align="left">
                                        <!--Caixa de Login-->
                                        <table border="0" cellspacing="0" cellpadding="3" style="width: 100%;">
                                            <tr>
                                                <td class="subtitulodireita">CPF:</td>
                                                <td>
                                                    <input type="text" name="usucpf" value="" class="login_input"
                                                           onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);"
                                                           style="width: 95%;"/>
                                                    <?= obrigatorio(); ?>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td></td>
                                                <td align="left">
                                                    <a class="button" href="javascript:enviar_formulario()">Lembrar
                                                        Senha</a>
                                                    <a class="button" href="./login.php">Voltar</a>
                                                </td>
                                            </tr>
                                        </table>
                                        <!--fim Caixa de Login -->
                                    </td>
                                </tr>
                            </table>
                        </div>
                    </div>
                </div>
                <br style="clear: both;" />
                <div class="col-md-3 col-sm-2">

                </div>

                <div id="bem_vindo" class="col-md-6 col-sm-8">
                    &nbsp;
                </div>
                <div class="col-md-3 col-sm-2">

                </div>
            </form>
        </div>
    </div>

    <script language="javascript">

        document.formulario.usucpf.focus();

        function enviar_formulario() {
            if (validar_formulario()) {
                document.formulario.submit();
            }
        }

        function validar_formulario() {
            var validacao = true;
            var mensagem = '';
            if (!validar_cpf(document.formulario.usucpf.value)) {
                mensagem += '\nO cpf informado não é válido.';
                validacao = false;
            }
            if (!validacao) {
                alert(mensagem);
            }
            return validacao;
        }
    </script>

<?php include "rodape_publico.inc"; ?>