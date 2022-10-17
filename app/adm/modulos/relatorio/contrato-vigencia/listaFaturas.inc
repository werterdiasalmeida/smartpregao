<?php
$p = getParam();
$mContratoFatura = new ContratoFatura();

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'listarFaturas' :
            $arrWhere = [
                "cnf.contrato_id = {$p['contrato_id']}"
            ];

            $whereSituacao = ContratoFatura::getWhereSituacaoFaturaParaRelatorio($p['situacao']);
            $arrWhere[] = $whereSituacao;

            $dataPaginacao = $mContratoFatura->getLista($arrWhere, true);

            die(getDataPaginacaoParaJson($dataPaginacao));
            break;
        case 'enviarLembreteEmail' :
            $sucessoEnvio = $mContratoFatura->enviarFaturaIuguParaEmailPorId($p['id']);
            if ($sucessoEnvio) {
                $db->sucessomAjax('relatorio/contrato-vigencia/lista', '', "Email enviado com sucesso");
            } else {
                $db->insucessomAjax('relatorio/contrato-vigencia/lista', '', 'Não foi possível enviar o Email');
            }
            break;
        case 'enviarEmailAvisoCancelamento' :
            $sucessoEnvio = $mContratoFatura->enviarEmailAvisoCancelamento($p['id']);
            if ($sucessoEnvio) {
                $db->sucessomAjax('relatorio/contrato-vigencia/lista', '', "Email enviado com sucesso");
            } else {
                $db->insucessomAjax('relatorio/contrato-vigencia/lista', '', 'Não foi possível enviar o Email');
            }
            break;
    }
}
?>

<table
        class='tabela-fatura table table-striped table-hover'
        data-toggle='table'
        data-pagination='true'
        data-side-pagination="server"
        data-page-size='12'
        data-search='false'>
    <thead>
    <th data-field="descricao" data-align="left">Descrição</th>
    <th data-field="valor" data-align="right" data-formatter="formataValor">Valor</th>
    <th data-field="data_vencimento" data-align="right" data-formatter="formatarData">Data de Vencimento</th>
    <?php if ($p['situacao'] != Fatura::SITUACAO_FATURA_PAGA): ?>
        <th data-align="center" data-formatter="acoesFormatter">Ações</th>
    <?php endif; ?>
    </thead>
</table>

<script type="application/javascript">
    var params = '<?= setParam($p) ?>';
    var queryParams = function (params) {
        params['act'] = 'listarFaturas';
        return params;
    };

    $(function () {
        $('[data-toggle="tooltip"]').tooltip();
        var tabelaAgenda = $('.tabela-fatura');
        tabelaAgenda.bootstrapTable({
            url: window.location.href + params,
            contentType: 'application/x-www-form-urlencoded; charset=UTF-8',
            method: 'post',
            queryParams: queryParams,
            onAll: function () {
                $('[data-toggle="tooltip"]').tooltip();
            }
        });
    });

    function acoesFormatter(value, row, index) {
        var hrefEnviarLembreteEmail = "javascript: enviarLembreteEmail('" + row.p + "')";
        var hrefEmailAvisoCancelamento = "javascript: enviarEmailAvisoCancelamento('" + row.p + "')";

        var arrButtons = [];

        <?php if($p['situacao'] == Fatura::SITUACAO_FATURA_GERADA): ?>
        arrButtons.push(
            '<a class="btn btn-circle btn-icon-only btn-default blue-steel" href="' + hrefEnviarLembreteEmail + '" data-toggle="tooltip" title="Enviar lembrete por Email">',
            '<i class="fa fa-envelope"></i> ',
            '</a> '
        );
        <?php endif; ?>

        <?php if($p['situacao'] == Fatura::SITUACAO_FATURA_VENCIDA): ?>
        if (row.numero_parcela != "") {
            arrButtons.push(
                '<a class="btn btn-circle btn-icon-only btn-default red" href="' + hrefEmailAvisoCancelamento + '" data-toggle="tooltip" title="Enviar aviso de cancelamento">',
                '<i class="fa fa-envelope"></i> ',
                '</a> '
            );
        }
        <?php endif; ?>

        return arrButtons.join('');
    }

    function enviarLembreteEmail(pParams) {
        $.ajax({
            type: "GET",
            url: window.location.href,
            dataType: 'json',
            data: {
                act: 'enviarLembreteEmail',
                p: pParams
            },
            beforeSend: function (xhr) {
                App.blockUI({message: 'Enviando email...', target: '#faturas-modal .modal-body'});
            },
            complete: function (data) {
                App.unblockUI('#faturas-modal .modal-body');
            },
            success: function (result) {
                if (result['codigo'] === 1) {
                    exibirSucesso("Email enviado com sucesso")
                } else {
                    exibirAlert('Não foi possível enviar o email');
                }
            },
            error: function (result) {
                exibirAlert('Não foi possível enviar o email');
            }
        });
    }

    function enviarEmailAvisoCancelamento(pParams) {
        $.ajax({
            type: "GET",
            url: window.location.href,
            dataType: 'json',
            data: {
                act: 'enviarEmailAvisoCancelamento',
                p: pParams
            },
            beforeSend: function (xhr) {
                App.blockUI({message: 'Enviando email...', target: '#faturas-modal .modal-body'});
            },
            complete: function (data) {
                App.unblockUI('#faturas-modal .modal-body');
            },
            success: function (result) {
                if (result['codigo'] === 1) {
                    exibirSucesso("Email enviado com sucesso")
                } else {
                    exibirAlert('Não foi possível enviar o email');
                }
            },
            error: function (result) {
                exibirAlert('Não foi possível enviar o email');
            }
        });
    }
</script>