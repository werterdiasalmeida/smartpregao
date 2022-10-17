<?php
$mContrato = new Contrato();
$arWhere = array();

if ($_POST['act']) {
    switch ($_POST['act']) {
        case 'pesquisar' :

            if ($_POST['cnpj']) {
                $cnpj = trim($_POST['cnpj']);
                $arWhere[] = "pjr.cnpj = '{$cnpj}'";
            }

            if ($_POST['numero']) {
                $numero = trim($_POST['numero']);
                $arWhere[] = "con.numero ILIKE '%{$numero}%'";
            }
            break;
        case 'reativarContrato' :
            try {
                $mContrato->reativarContrato(['id' => $_REQUEST['conid']]);
                $db->commit();
                $db->sucesso('financeiro/contrato/lista&acao=C');
            } catch (Exception $e) {
                $db->insucesso("Não foi possível concluir a solicitação", '', 'financeiro/contrato/lista&acao=C');
            }
            break;
        case 'cancelarContrato' :
        case 'formularioCancelarContrato' :
            include_once 'formularioCancelarContrato.inc';
            die();
            break;
    }
}

include APPRAIZ . "includes/cabecalho.inc";
monta_titulo($titulo_modulo, '');

?>

    <form id="formulario" name="formulario" method="post">
        <input type=hidden name="act" value="pesquisar"/>
        <input type=hidden name="conid" value=""/>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
            <tr>
                <td class="SubTituloDireita">
                    CNPJ:
                </td>
                <td>
                    <?= campo_texto('cnpj', 'N', 'S', '', 50, 100, '', ''); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    Número do Contrato:
                </td>
                <td>
                    <?= campo_texto('numero', 'N', 'S', '', 50, 20, '', ''); ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloEsquerda">&nbsp;</td>
                <td class="subtituloEsquerda" colspan="2">
                    <input type="submit" name="Pesquisar" value="Pesquisar"/>
                    <input type="button" name="Limpar" value="Limpar"
                           onclick="location.href='?modulo=financeiro/contrato/lista&acao=C'"/>
                </td>
            </tr>
            <tr>
                <td colspan="2">
                    <a style="cursor: pointer;"
                       onclick="javascript: window.location='?modulo=financeiro/contrato/formulario&acao=C';"
                       title="Clique para incluir um novo Contrato">
                        <img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir
                            novo Contrato</b>
                    </a>
                </td>
            </tr>
        </table>
    </form>

    <!-- Modal -->
    <div class="modal fade" id="modal-cancelamento-contrato" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
        <div class="modal-dialog modal-lg" role="document">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                        <span aria-hidden="true">&times;</span>
                    </button>
                    <h4 class="modal-title" id="myModalLabel">Cancelar Contrato</h4>
                </div>
                <div class="modal-body">
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
    "Número do Contrato",
    "CNPJ do Estabelecimento",
    "Razão Social",
    "Situação",
    "Incluído em",
    "Incluído por"
);

$situacaoAtivo = Contrato::SITUACAO_CONTRATO_ATIVO;
$situacaoPendentePagamento = Contrato::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO;
$situacaoInativo = Contrato::SITUACAO_CONTRATO_INATIVO;

$arParamCol = array();
$arParamCol[1]["style"] = "text-align: right;";
$arParamCol[3]["style"] = "text-align: center;";
$arParamCol[3]['html'] = "<php>return htmlLabelSituacao('{situacao}');</php>";
$arParamCol[4]["html"] = "<php>return formata_timestamp('{criado_em}');</php>";

$acaoConsultar = <<<ACAO
            <div style="min-width: 20px;">
                <a href="?modulo=financeiro/contrato/formulario&acao=C<php> return setParam(array('contratoId' => '{id}')); </php>" title="Visualizar Contrato">
                     <img src="/imagens/consultar.gif">
                </a>
            </div>
ACAO;

$acao = "<php> return getAcaoLista('{situacao}', '{id}'); </php>";

$rs = $mContrato->getLista($arWhere, true);

$oLista = new Lista($arConfig);
$oLista->setCabecalho($arCabecalho);
$oLista->setCorpo($rs, $arParamCol);
$oLista->setAcao($acao);
$oLista->show();
?>

    <link rel="stylesheet" href="../includes/bootstrap/3.0.0/css/bootstrap.min-simec.css">

    <script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
    <script type="text/javascript" src="../includes/bootstrap/3.0.0/js/bootstrap.min.js"></script>
    <script type="text/javascript" src="../includes/jquery-mask/jquery-mask-1.13.3.js"></script>

    <script type="text/javascript">
        $(function () {
            $("#cnpj").mask('00.000.000/0000-00', {reverse: true});
        });

        function abrirModalCancelarContrato(pParam) {
            divCarregando();
            $('#modal-cancelamento-contrato .modal-body').load(window.location.href, {
                act: 'formularioCancelarContrato',
                p: pParam
            }, function () {
                divCarregado();
                $('#modal-cancelamento-contrato').modal('show');
            });
        }

        function reativarContrato(cod) {
            if (confirm('Deseja reativar este contrato?')) {
                document.formulario.act.value = 'reativarContrato';
                document.formulario.conid.value = cod;

                document.formulario.submit();
            }
        }
    </script>
<?php
function htmlLabelSituacao($situacao)
{
    $arrSitaucaoContrato = Contrato::getArraySituacaoContrato();

    return "<span style=\"color: {$arrSitaucaoContrato[$situacao]['cor']};\">{$arrSitaucaoContrato[$situacao]['descricao']}</span>";
}

function getAcaoLista($situacao, $id)
{
    $pParamGet = setParam(['contrato_id' => $id]);
    $pParam = setParam(['contrato_id' => $id], false);
    $arrBtns = [
        <<<ACAO
            <a href="?modulo=financeiro/contrato/formulario&acao=C$pParamGet" title="Visualizar Contrato">
                 <img src="/imagens/consultar.gif">
            </a>
ACAO
    ];

    if ($situacao == Contrato::SITUACAO_CONTRATO_ATIVO || $situacao == Contrato::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO) {
        $arrBtns[] = <<<ACAO
            <a href="javascript:abrirModalCancelarContrato('$pParam')" title="Cancelar Contrato">
                 <img src="/imagens/excluir.gif">
            </a>
ACAO;
    }

    if ($situacao == Contrato::SITUACAO_CONTRATO_CANCELADO) {
        $arrBtns[] = <<<ACAO
            <a href="javascript:reativarContrato($id)" title="Reativar Contrato">
                 <img src="/imagens/check.png">
            </a>
ACAO;
    }

    $strBtns = implode(" ", $arrBtns);
    $acao = <<<ACAO
            <div style="min-width: 20px;">
                {$strBtns}
            </div>
ACAO;

    return $acao;
}