<?php
$permissaoTela = 'S';
$mContrato = new Contrato();
$mContratoFatura = new ContratoFatura();
$mFatura = new Fatura();
$params = getParam();
$id = isset($params['id']) ? $params['id'] : null;
$contratoId = isset($params['contratoId']) ? $params['contratoId'] : null;
$pContrato = setParam(array('contratoId' => $contratoId));

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'salvar' :
            $arquivoId = null;
            if (isset($_FILES['arquivo'])
                && isset($_FILES['arquivo']['name']) && !empty($_FILES['arquivo']['name'])
                && isset($_FILES['arquivo']['type']) && !empty($_FILES['arquivo']['type'])
                && isset($_FILES['arquivo']['size']) && !empty($_FILES['arquivo']['size'])) {
                $file = new FilesSimec();
                $arquivoId = $file->setUploadArquivoEspecifico("Comprovante de pagamento", 'arquivo');
            }

            $arrDados = array(
                'id' => $_POST['fatura_id'],
                'contrato_id' => $_POST['contrato_id'],
                'data_pagamento' => formata_data_sql($_POST['data_pagamento']),
                'comprovante_pagamento' => $arquivoId,
            );

            $contratoId = $mContratoFatura->confirmarPagamentoPorComprovante($arrDados);
            $db->commit();
            $db->sucesso('financeiro/fatura/lista&acao=C', $pContrato);
            break;
        case 'download' :
            $file = new FilesSimec();
            $params = getParam();
            $file->getDownloadArquivo($params['comprovante_pagamento']);
            die;
            break;
    }
}

if (!empty($id)) {
    $dados = $mContratoFatura->getTodosDadosPorId($id);
    extract($dados);
    if ($situacao == Fatura::SITUACAO_FATURA_PAGA) {
        $permissaoTela = 'N';
    }
}

include APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba($abacod_tela, $url, $pContrato);
monta_titulo($titulo_modulo, '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica os campos obrigatórios.');
$mContrato->exibirCabecalhoContrato($contratoId);
?>

<form id="formulario" name="formulario" enctype="multipart/form-data" method="post">
    <input type="hidden" id="act" name="act" value="salvar"/>
    <input type="hidden" id="id" name="id" value="<?= $id ?>"/>
    <input type="hidden" id="fatura_id" name="fatura_id" value="<?= $fatura_id ?>"/>
    <input type="hidden" id="contrato_id" name="contrato_id" value="<?= $contrato_id ?>"/>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <td colspan="2" class="SubTituloDireita" style="text-align: center; font-weight: bold;">
                Dados da Fatura
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Número da Parcela:
            </td>
            <td>
                <label><?= $numero_parcela ?></label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data de Vencimento:
            </td>
            <td>
                <label><?= formata_data($data_vencimento) ?></label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Valor da Fatura (R$):
            </td>
            <td>
                <label><?= formata_valor($valor) ?></label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Situação da Fatura:
            </td>
            <td>
                <label><?= $str_situacao; ?></label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data do Pagamento:
            </td>
            <td>
                <?= campo_data2('data_pagamento', 'S', $permissaoTela, '', 'S'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Comprovante de Pagamento:
            </td>
            <td>
                <div class="btn-arquivo">
                    <a href="javascript:downloadComprovante('<?= setParam(array('id' => $id, 'comprovante_pagamento' => $comprovante_pagamento), false); ?>');">
                        <img src="/imagens/download.png">
                        Download comprovante de pagamento
                    </a>
                    <br>
                    <?php if ($permissaoTela == 'S'): ?>
                        <a href="javascript:exibeBotoesArquivo(false);">
                            <img src="/imagens/excluir.gif">
                            Enviar outro arquivo
                        </a>
                    <?php endif; ?>
                    <input type="hidden" name="comprovante_pagamento" id="comprovante_pagamento"
                           value="<?= $dadosPagamento['comprovante_pagamento']; ?>">
                </div>
                <div class="input-arquivo">
                    <input type="file" name="arquivo"
                           class="form-control" <?= $permissaoTela == 'N' ? 'disabled' : ''; ?>/> <?= obrigatorio(); ?>
                </div>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda">&nbsp;</td>
            <td class="subtituloEsquerda" colspan="2">
                <?php if ($permissaoTela == 'S'): ?>
                    <input type="button" value="Confirmar Pagamento" id="btn-salvar" onclick="enviarFormulario();"/>
                <?php endif; ?>
                <input type="button" value="Voltar" id="btn-voltar"
                       onclick="window.location = '?modulo=financeiro/fatura/lista&acao=C<?= $pContrato ?>';"/>
            </td>
        </tr>
    </table>
</form>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css?v=<?= VERSAO_SISTEMA; ?>" type="text/css"
      rel="stylesheet"></link>
<script type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js?v=<?= VERSAO_SISTEMA; ?>"></script>
<script type="text/javascript" src="../includes/moment/moment.min.js?v=<?= VERSAO_SISTEMA; ?>"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script type="text/javascript" src="../includes/jquery-mask/jquery-mask-1.13.3.js"></script>
<script type="text/javascript">
    $(function () {
        <? if($comprovante_pagamento): ?>
        exibeBotoesArquivo(true);
        <? else: ?>
        exibeBotoesArquivo(false);
        <? endif; ?>

        $("#cnpj").mask('00.000.000/0000-00', {reverse: true});
        $('#valor_contratado').mask("#.##0,00", {reverse: true});
        $('#quantidade_parcelas').mask("###", {reverse: true});
    });

    function enviarFormulario(tipo) {
        if (!validaBranco(document.formulario.data_pagamento, 'Data do Pagamento')) return;
        if (!validaBranco(document.formulario.arquivo, 'Tipo de Contrato')) return;

        document.formulario.submit();
    }

    function downloadComprovante(id) {
        window.location = "?modulo=financeiro/fatura/formulario&acao=C&act=download&p=" + id;
    }

    function exibeBotoesArquivo(exibir) {
        if (exibir == true) {
            $('.btn-arquivo').show();
            $('.input-arquivo').hide();
        } else {
            $('.btn-arquivo').hide();
            $('.input-arquivo').show();
            $('#comprovante_pagamento').val('');
        }
    }
</script>


