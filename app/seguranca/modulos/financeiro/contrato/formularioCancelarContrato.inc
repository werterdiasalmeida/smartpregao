<?php
$params = getParam();
$mContrato = new Contrato();
$contratoId = $params['contrato_id'];

if ($_POST['act']) {
    switch ($_POST['act']) {
        case 'cancelarContrato' :
            try {
                $mContrato->cancelarContrato(['id' => $contratoId]);
                $db->commit();
                $db->sucesso('financeiro/contrato/lista&acao=C');
            } catch (Exception $e) {
                $db->insucesso("Não foi possível concluir a solicitação", '', 'financeiro/contrato/lista&acao=C');
            }
    }
}

$dadosContrato = $mContrato->getTodosDadosPorId($contratoId);
$valorCancelamento = $mContrato->getValorCancelamentoContrato(['id' => $contratoId]);
?>
<form name="formulario" method="post">
    <input type="hidden" name="act" value="cancelarContrato">
    <input type="hidden" name="p" value="<?= setParam(['contrato_id' => $contratoId], false) ?>">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <td class="SubTituloDireita">
                CNPJ:
            </td>
            <td>
                <?= $dadosContrato['cnpj']; ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Razão Social:
            </td>
            <td>
                <?= $dadosContrato['razao_social']; ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Nome Fantasia:
            </td>
            <td>
                <?= $dadosContrato['nome_fantasia']; ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Valor da Multa (R$):
            </td>
            <td>
                <?= formata_valor($valorCancelamento); ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda">&nbsp;</td>
            <td class="subtituloEsquerda" colspan="2">
                <input type="button" id="btn-cancelar" style="background-color: #D25154; border-color: #D25154;" name="Cancelar Contrato" value="Cancelar Contrato"/>
            </td>
        </tr>
    </table>
</form>


<script type="text/javascript">

    $(function () {
        $("#modal-cancelamento-contrato #btn-cancelar").click(function () {
            $("#modal-cancelamento-contrato form").submit();
        });
    });
</script>