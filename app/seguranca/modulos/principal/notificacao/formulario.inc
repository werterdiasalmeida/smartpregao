<?php
$permissaoTela = 'S';
$mNotificacao = new NotificacaoApp();

if($_POST['act']) {
    switch ($_POST['act']) {
        case 'salvar' :
        case 'salvar_enviar' :
            $arDados = array(
                'noatitulo'		=> $_POST['noatitulo'],
                'noamensagem'	=> $_POST['noamensagem'],
                'noastatus'		=> 'A',
                'noadtcadastro'	=> 'now()',
                'usucpf'		=> $_SESSION['usucpf'],
                'appid'		    => $_POST['appid']
            );

            $noaid = $mNotificacao->popularDadosObjeto($arDados)->salvar();

            $mNotificacaoUsuario = new NotificacaoUsuario();
            if(is_array($_POST["diuid_{$_POST['appid']}"])){
                foreach($_POST["diuid_{$_POST['appid']}"] as $diuid){
                    $arDadosUsuario = array(
                        'noaid' => $noaid,
                        'diuid' => $diuid
                    );

                    $mNotificacaoUsuario->popularDadosObjeto($arDadosUsuario)->salvar();
                    $mNotificacaoUsuario->setDadosNull();
                }
            }

            if($_POST['act'] == 'salvar_enviar'){
                $arDispositivos = $mNotificacao->enviar();
                if(count($arDispositivos) == 0){
                    $mNotificacao->commit();
                    $db->insucesso('Não foi possível enviar a notificação para nenhum dispositivo.', '', 'principal/notificacao/lista', 'A');
                }
            }

            $mNotificacao->commit();
            $db->sucesso('principal/notificacao/lista');
            break;
    }
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
monta_titulo($titulo_modulo, '');
?>

<form id="formulario" name="formulario" method="post">
    <input type="hidden" id="act" name="act" />
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <td class="SubTituloDireita">
                Título:
            </td>
            <td>
                <?= campo_texto('noatitulo', 'S', $permissaoTela, '', 50, 100, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Descrição:
            </td>
            <td>
                <?= campo_textarea('noamensagem', 'S', $permissaoTela, '', 52, 10, 200); ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloDireita" style="text-align: center; font-weight: bold;">
                Destinatário
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Aplicativo:
            </td>
            <td>
                <?php
                $mAplicativo = new Aplicativo();
                $arApps = $mAplicativo->recuperarTodos('appid AS codigo, appnome AS descricao', array("appstatus = 'A'"), 2);
                $db->monta_combo('appid', $arApps, 'S', 'Selecione...', 'exibirUsuarios();', '', '', '350', 'S');
                ?>
            </td>
        </tr>
        <tr class="tr-usuarios" style="display: none;">
            <td class="SubTituloDireita">
                Usuários:
            </td>
            <td>
                <?php
                $arApps = $arApps ? $arApps : array();
                $mDispositivoUsuario = new DispositivoUsuario();
                foreach($arApps as $app){
                    $sqlDispositivoUsuario = $mDispositivoUsuario->getDispositivosByAplicativo($app['codigo'], true);
                    echo "<div id='usuarios-{$app['codigo']}' class='div-usuarios' style='display: none;'>";
                    combo_popup('diuid_' . $app['codigo'], $sqlDispositivoUsuario, 'Selecione o usuários');
                    echo obrigatorio();
                    echo "</div>";
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda">&nbsp;</td>
            <td class="subtituloEsquerda" colspan="2">
                <input type="button" value="Salvar" id="btn-salvar" onclick="enviarFormulario('salvar');"/>
                <input type="button" value="Salvar e Enviar Notificação" id="btn-salvar-enviar" onclick="enviarFormulario('salvar_enviar');"/>
                <input type="button" value="Voltar" id="btn-voltar" onclick="window.location = '?modulo=principal/notificacao/lista&acao=A';" />
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script type="text/javascript">
    function enviarFormulario(tipo){
        if (!validaBranco(document.formulario.noatitulo, 'Título')) return;
        if (!validaBranco(document.formulario.noamensagem, 'Descrição')) return;
        if (!validaBranco(document.formulario.appid, 'Aplicativo')) return;

        var elemUsuarios = document.getElementById("diuid_"+document.formulario.appid.value);
        selectAllOptions(elemUsuarios);
        if (!validaBranco(elemUsuarios, 'Usuários')) return;

        if(tipo == 'salvar' || (tipo == 'salvar_enviar' && confirm('Tem certeza que deseja enviar esta notificação agora?'))){
            document.formulario.act.value = tipo;
            document.formulario.submit();
        }
    }

    function exibirUsuarios(){
        $('.div-usuarios').hide();

        var appid = $('[name=appid]').val();
        var divUsuarios = $('#usuarios-'+appid);
        if(divUsuarios.length > 0){
            $(divUsuarios).show();
            $('.tr-usuarios').show();
        }else{
            $('.tr-usuarios').hide();
        }
    }
</script>


