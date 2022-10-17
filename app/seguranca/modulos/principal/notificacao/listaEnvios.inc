<?php
$noaid = $_POST['noaid'];
$mNotificacao = new NotificacaoApp($noaid);
$mNotificacaoEnviada = new NotificacaoAppEnviada();
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <td class="SubTituloDireita" style="font-weight: bold;">
            Notificação:
        </td>
        <td>
            <?= $mNotificacao->noatitulo; ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="font-weight: bold;">
            Aplicativo:
        </td>
        <td>
            <?php
            $mApp = new Aplicativo($mNotificacao->appid);
            echo $mApp->appnome;
            ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" style="font-weight: bold;">
            Destinatários:
        </td>
        <td>
            <?php
            $mNotificacaoUsuario = new NotificacaoUsuario();
            $arDestinatarios = array_map(function($destinatario){
                return $destinatario['usunome'] . ' (' . formatar_cpf($destinatario['usucpf'])  . ') - ' . $destinatario['displataforma'] . ' ' . $destinatario['disversaoplataforma'];
            }, $mNotificacaoUsuario->getDispositivosByNotificacao($mNotificacao->noaid));

            echo implode('<br />', $arDestinatarios);
            ?>
        </td>
    </tr>
</table>
<?php
$rsListaEnvio = $mNotificacaoEnviada->getLista(array("nae.noaid = {$noaid}"), false, 10);

$arParamCol = array();
$arParamCol[1]['style'] = 'text-align: center;';

$arParamCol[2]['html'] = '<php>return formata_valor("{qtd_enviado}", 0);</php>';
$arParamCol[3]['html'] = '<php>return formata_valor("{qtd_recebido}", 0);</php>';
$arParamCol[4]['html'] = '<php>return formata_valor("{qtd_lida}", 0);</php>';

$arParamCol[2]['type'] = 'numeric';
$arParamCol[3]['type'] = 'numeric';
$arParamCol[4]['type'] = 'numeric';

$arCabecalho = array(
    'Enviado por',
    'Enviado em',
    'Enviados',
    'Recebidos',
    'Lidos'
);

$oLista = new Lista($arConfig);
$oLista->setCabecalho($arCabecalho);
$oLista->setCorpo($rsListaEnvio, $arParamCol);
$oLista->show();