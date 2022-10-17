<?php
include "includes_publico.inc";

// Particularidade feita para o PDE Escola
$selecionar_modulo_habilitado = 'S';

$sisid = $_REQUEST['sisid'];
$usucpf = $_REQUEST['usucpf'];

// leva o usuário para o passo seguinte do cadastro
if ($_REQUEST['usucpf'] && $_REQUEST['modulo'] && $_REQUEST['varaux'] == '1') {
    $_SESSION = array();
    if ($theme) $_SESSION['theme_temp'] = $theme;
    header("Location: cadastrar_usuario_2.php?sisid=$sisid&usucpf=$usucpf");
    exit();
}

include "cabecalho_publico.inc";

$mensagens = implode('<br/>', (array)$_SESSION['MSG_AVISO']);
$_SESSION['MSG_AVISO'] = null;
$titulo_modulo = 'Solicitação de Cadastro';
$subtitulo_modulo = 'Preencha os Dados Abaixo e clique no botão: "Continuar".';
?>
    <div id="caixa-login">
        <div class="conteudo">
            <form method="post" name="formulario" id="formulario" onsubmit="return false;">
                <input type=hidden name="modulo" value="./inclusao_usuario.php"/>
                <input type=hidden name="varaux" value="">

                <div class="col-md-12 col-sm-12">
                    <div class="col-md-3 col-sm-2">

                    </div>
                    <div id="bem_vindo_2" class="col-md-6 col-sm-8">
                        &nbsp;
                    </div>
                    <div class="col-md-3 col-sm-2">

                    </div>
                </div>
                <div class="col-md-2 col-sm-2 col-xm-0">

                </div>

                <div class="col-md-8 col-sm-8 col-xm-12">
                    <div id="container-publico">
                        <div id="caixa-formulario" style="">
                            <!-- Lista de Módulos-->
                            <table border="0" cellpadding="0" cellspacing="0" class="tabela_modulos">
                                <tr>
                                    <td class="td_bg"><?= $titulo_modulo ?></small></td>
                                </tr>
                                <tr>
                                    <td align="center">
                                        <? if (strlen($mensagens) > 5) { ?>
                                            <div class="error_msg"><? echo(($mensagens) ? $mensagens : ""); ?></div>
                                        <? } ?>
                                    </td>
                                </tr>
                                <tr>
                                    <td valign="middle" class="td_table_inicio">
                                        <style>
                                            .CampoEstilo{
                                                width: 90% !important;
                                            }
                                        </style>
                                        <table style="width: 100%;">
                                            <tr>
                                                <td style="font-weight: bold;" align='right'>Módulos:</td>
                                                <td>
                                                    <?php
                                                    $sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t' order by descricao ";
                                                    $arSistemas = $db->carregar($sql);
                                                    if(!isset($_POST['sisid']) && count($arSistemas) == 1){
                                                        $sisid = $arSistemas[0]['codigo'];
                                                    }
                                                    $db->monta_combo("sisid", $arSistemas, $selecionar_modulo_habilitado, "", 'selecionar_modulo', '', '', '');
                                                    ?>
                                                    <?= obrigatorio(); ?>
                                                </td>
                                            </tr>
                                            <?php if ($sisid): ?>
                                                <tr>
                                                    <td align='right' class="subtitulodireita">&nbsp;</td>
                                                    <td>
                                                        <?php
                                                        $sql = sprintf("select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado from sistema where sisid = %d", $sisid);
                                                        $sistema = (object)$db->pegaLinha($sql);
                                                        if ($sistema->sisid) :
                                                            ?>
                                                            <font color="#555555" face="Verdana">
                                                                <b><?= $sistema->sisdsc ?></b><br/>

                                                                <p><?= $sistema->sisfinalidade ?></p>
                                                                <ul>
                                                                    <li><span
                                                                            style="color: #000000">Público-Alvo:</span> <?= $sistema->sispublico ?>
                                                                        <br></li>
                                                                    <li><span
                                                                            style="color: #000000">Sistemas Relacionados:</span> <?= $sistema->sisrelacionado ?>
                                                                    </li>
                                                                </ul>
                                                            </font>
                                                        <?php endif; ?>
                                                    </td>
                                                </tr>
                                            <?php endif; ?>

                                            <tr>
                                                <td style="font-weight: bold;" align='right'>CPF:</td>
                                                <td>
                                                    <input id="usucpf" type="text" name="usucpf" style="width: 84%;"
                                                           value=<? print '"' . $usucpf . '"'; ?> class="login_input"
                                                           onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);"/>
                                                    <img border='0' src='../imagens/obrig.gif'
                                                         title='Indica campo obrigatório.'>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>&nbsp;</td>
                                                <td>
                                                    <a class="button" href="javascript:validar_formulario()">Continuar</a>
                                                    <a class="button" href="./login.php">Voltar</a>
                                                </td>
                                            </tr>
                                        </table>
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
                <input type="hidden" name="sisfinalidade_selc" value="<?= $sisfinalidade_selc ?>"/>
            </form>
        </div>
    </div>

    <script language="javascript">

        function selecionar_modulo() {
            document.formulario.submit();
        }

        function validar_formulario() {
            var validacao = true;
            var mensagem = '';

            if (document.formulario.sisid.value == "") {
                mensagem += '\nSelecione o módulo no qual você pretende ter acesso.';
                validacao = false;
            }

            if (document.formulario.usucpf.value == '' || !validar_cpf(document.formulario.usucpf.value)) {
                mensagem += '\nO cpf informado não é válido.';
                validacao = false;
            }

            document.formulario.varaux.value = '1';

            if (!validacao) {
                alert(mensagem);
            } else {
                document.formulario.submit();
            }
        }
    </script>

<?php include "rodape_publico.inc"; ?>