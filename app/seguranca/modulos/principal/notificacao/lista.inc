<?php
$mNotificacao = new NotificacaoApp();
$arWhere = array();

if ($_POST['act']) {
    switch ($_POST['act']) {
        case 'pesquisar' :
            if($_POST['titulomensagem']){
                $titulomensagem = $_POST['titulomensagem'];
                $arWhere[] = "(
                    noa.noatitulo ILIKE '%{$titulomensagem}%'
                    OR
                    noa.noamensagem ILIKE '%{$titulomensagem}%'
                )";
            }
            break;
        case 'excluir' :
            $arDados = array(
                'noaid' => $_POST['noaid'],
                'noastatus' => 'I'
            );

            $mNotificacao->popularDadosObjeto($arDados)->salvar();
            $mNotificacao->commit();

            $db->sucesso($_REQUEST['modulo']);
            break;
        case 'enviar_notificacao' :
            $arDispositivos = $mNotificacao->enviar($_POST['noaid']);
            $mNotificacao->commit();

            if(count($arDispositivos) > 0){
                $db->sucesso($_REQUEST['modulo']);
            }else{
                $db->insucesso('Não foi possível enviar a notificação para nenhum dispositivo.', '', $_REQUEST['modulo'], $_REQUEST['acao']);
            }
            break;
        case 'visualizar_envios' :
            include_once 'listaEnvios.inc';
            die();
            break;
    }
}

include APPRAIZ . "includes/cabecalho.inc";
monta_titulo($titulo_modulo, '');

?>

<form id="formulario" name="formulario" method="post">
    <input type=hidden name="act" value="pesquisar"/>
    <input type=hidden name="noaid" value=""/>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <td class="SubTituloDireita">
                Título/Mensagem:
            </td>
            <td>
                <?= campo_texto('titulomensagem', 'N', 'S', '', 50, 100, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda">&nbsp;</td>
            <td class="subtituloEsquerda" colspan="2">
                <input type="submit" name="Pesquisar" value="Pesquisar"/>
                <input type="button" name="Limpar" value="Limpar" onclick="location.href='?modulo=principal/notificacao/lista&acao=A'"/>
            </td>
        </tr>
        <tr>
            <td colspan="2">
                <a style="cursor: pointer;" onclick="javascript: window.location='?modulo=principal/notificacao/formulario&acao=A';" title="Clique para incluir uma nova Notificação">
                    <img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir nova Notificação</b>
                </a>
            </td>
        </tr>
    </table>
</form>


<!-- Modal -->
<div class="modal fade" id="modal-envios" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Envios</h4>
            </div>
            <div class="modal-body">
                ...
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<?php
$arConfig = array(
    "style" => "width:100%;",
    "totalLinha" => false,
    "totalRegistro" => true
);

$arCabecalho = array(
    "Ação",
    "Título",
    "Mensagem",
    "Incluído em",
    "Incluído por"
);

$arParamCol = array();
$arParamCol[1]["html"]  = "<php>return '<div style=\"white-space: pre;\">{noamensagem}</div>';</php>";
$arParamCol[2]["html"]  = "<php>return formata_timestamp('{noadtcadastro}');</php>";
$arParamCol[0]["style"] = "min-width: 200px;";
$arParamCol[1]["style"] = "min-width: 200px;";
$arParamCol[2]["style"] = "min-width: 120px; text-align: center;";
$arParamCol[3]["style"] = "min-width: 200px;";

$acao = <<<ACAO
    <div style="text-align: center; min-width: 80px;">
        <img src="/imagens/enviar.png" style="cursor:pointer" title="Enviar Notificação" onclick="enviarNotificacao( '{noaid}' )">
        <img src="/imagens/consultar.gif" style="cursor:pointer" title="Visualizar Envios" onclick="visualizarEnvios( '{noaid}' )">
        <img src="/imagens/excluir.gif" style="cursor:pointer" title="Excluir" onclick="excluir( '{noaid}' )">
    </div>
ACAO;

$rs = $mNotificacao->getLista($arWhere, true);

$oLista = new Lista($arConfig);
$oLista->setCabecalho($arCabecalho);
$oLista->setCorpo($rs, $arParamCol);
$oLista->setAcao($acao);
$oLista->show();
?>

<link rel="stylesheet" href="../includes/bootstrap/3.0.0/css/bootstrap.min-simec.css">

<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script type="text/javascript" src="../includes/bootstrap/3.0.0/js/bootstrap.min.js"></script>

<script type="text/javascript">
    function excluir(cod){
        if ( confirm('Tem certeza que deseja apagar este registro?') ){
            document.formulario.act.value = 'excluir';
            document.formulario.noaid.value = cod;

            document.formulario.submit();
        }
    }

    function enviarNotificacao(cod){
        if ( confirm('Tem certeza que deseja enviar esta notificação agora?') ){
            document.formulario.act.value = 'enviar_notificacao';
            document.formulario.noaid.value = cod;

            document.formulario.submit();
        }
    }

    function visualizarEnvios(cod){
        divCarregando();
        $('#modal-envios .modal-body').load( window.location.href, {noaid : cod, act : 'visualizar_envios'}, function(){
            divCarregado();
            $('#modal-envios').modal('show');
        });
    }
</script>
