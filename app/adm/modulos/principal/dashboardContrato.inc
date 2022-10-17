<?php
$mDashboardContrato = new DashboardContrato();
$mContrato = new Contrato();
$params = getParam();

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'carregarDashboard' :
            require 'dashboardContratoContent.inc';
            die();
            break;
        case 'listarEstabelecimentos' :
            $arrWhere = [];

            if (!empty($params['estabelecimento_id'])) {
                $arrWhere[] = "esc.estabelecimento_id IN (" . implode(", ", $params['estabelecimento_id']) . ")";
            }

            if (!empty($params['inicio']) && !empty($params['fim'])) {
                $arrWherePeriodo[] = "con.inicio_vigencia BETWEEN '{$params['inicio']}'::date AND '{$params['fim']}'::date";
                $arrWherePeriodo[] = "con.fim_vigencia BETWEEN '{$params['inicio']}'::date AND '{$params['fim']}'::date";
                $arrWhere[] = "(" . implode(" OR ", $arrWherePeriodo) . ")";
            }

            if (!empty($_REQUEST['situacao'])) {
                $arrWhere[] = "con.situacao = '{$_REQUEST['situacao']}'";
            }

            $lista = $mContrato->getListaRelatorio($arrWhere);
            die(getDataPaginacaoParaJson($lista));
            break;
        case 'listarEstabelecimentosQunatitivoFatura' :
            $arrWhere = [];

            if (!empty($params['estabelecimento_id'])) {
                $arrWhere[] = "esc.estabelecimento_id IN (" . implode(", ", $params['estabelecimento_id']) . ")";
            }

            if (!empty($params['inicio']) && !empty($params['fim'])) {
                $arrWhere[] = "fat.data_vencimento BETWEEN '{$params['inicio']}'::date AND '{$params['fim']}'::date";
            }

            if (!empty($_REQUEST['situacao_fatura'])) {
                $whereSituacao = ContratoFatura::getWhereSituacaoFaturaParaRelatorio($_REQUEST['situacao_fatura']);
                $arrWhere[] = $whereSituacao;
            }

            $lista = $mContrato->getListaRelatorio($arrWhere);
            die(getDataPaginacaoParaJson($lista));
            break;
        case 'listarFaturas' :
            $mContratoFatura = new ContratoFatura();

            $params = getParam();
            $arrWhere = [
                "cnf.contrato_id = {$_REQUEST['contrato_id']}"
            ];

            if (!empty($params['inicio']) && !empty($params['fim'])) {
                $arrWhere[] = "fat.data_vencimento BETWEEN '{$params['inicio']}'::date AND '{$params['fim']}'::date";
            }

            $whereSituacao = ContratoFatura::getWhereSituacaoFaturaParaRelatorio($_REQUEST['situacao_fatura']);
            $arrWhere[] = $whereSituacao;

            $dataPaginacao = $mContratoFatura->getLista($arrWhere, true);

            die(getDataPaginacaoParaJson($dataPaginacao));
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
?>
<style type="text/css">
    a.dashboard-stat {
        margin-top: 5px;
    }
</style>

<input type="hidden" id="tipoPeriodo" value="">
<div class="dashboard-content">
</div>

<div class="modal fade" id="detalhe-modal" tabindex="-1" role="basic" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h2 class="modal-title">Estabelecimentos</h2>
            </div>
            <div class="modal-body">
                <table id="table-detalhe"></table>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn dark btn-outline" data-dismiss="modal">Fechar</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<script src="/includes/metronic/global/plugins/highcharts/js/highcharts.js?v=<?= VERSAO ?>"
        type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/highcharts/js/modules/no-data-to-display.js?v=<?= VERSAO ?>"
        type="text/javascript"></script>
<script type="text/javascript">
    $(function () {
        carregarDashboard();
    });

    function carregarDashboard() {
        //carregando
        App.blockUI({message: 'Carregando...', target: '.page-content'});

        var data = {
            act: 'carregarDashboard',
            estabelecimento_id: $('[name=estabelecimento_id]').val(),
            inicio: $('#inicio').val(),
            fim: $('#fim').val(),
            tipo: $('#tipoPeriodo').val()
        };

        $('.dashboard-content').load(window.location.href, data, function () {
            $("[data-counter='counterup']").counterUp({
                time: 400
            });

            App.unblockUI('.page-content');
        });

        $('#detalhe-modal').on('hide.bs.modal', function (e) {
            $('#detalhe-modal #table-detalhe').bootstrapTable('destroy');
        })
    }

    function abrirTabelaDetalheContrato(situacaoContrato) {
        var modal = $('#detalhe-modal');
        var pParams = $("[name=p_params]").val();

        modal.modal({
            backdrop: 'static'
        }).modal('show');

        var queryParams = function (params) {
            params['act'] = 'listarEstabelecimentos';
            params['situacao'] = situacaoContrato;
            return params;
        };

        $('#detalhe-modal #table-detalhe').bootstrapTable({
            url: window.location.href + '&p=' + pParams,
            contentType: 'application/x-www-form-urlencoded; charset=UTF-8',
            method: 'post',
            pagination: true,
            sidePagination: "server",
            pageSize: '12',
            queryParams: queryParams,
            onAll: function () {
                $('[data-toggle="tooltip"]').tooltip();
            },
            columns: [
                {
                    field: 'numero',
                    title: 'Número do Contrato'
                },
                {
                    field: 'cnpj',
                    title: 'CNPJ'
                },
                {
                    field: 'nome_fantasia',
                    title: 'Nome Fantasia'
                },
                {
                    field: 'vigencia_contrato',
                    title: 'Vigência'
                }
            ]
        });
    }

    function abrirTabelaDetalheFatura(situacaoFatura) {
        var modal = $('#detalhe-modal');

        modal.modal({
            backdrop: 'static'
        }).modal('show');

        var pParams = $("[name=p_params]").val();
        var queryParams = function (params) {
            params['act'] = 'listarEstabelecimentosQunatitivoFatura';
            params['situacao_fatura'] = situacaoFatura;
            params['p'] = pParams;
            return params;
        };

        $('#detalhe-modal #table-detalhe').bootstrapTable({
            url: window.location.href,
            contentType: 'application/x-www-form-urlencoded; charset=UTF-8',
            method: 'post',
            pagination: true,
            sidePagination: "server",
            pageSize: '12',
            detailView: true,
            queryParams: queryParams,
            onAll: function () {
                $('[data-toggle="tooltip"]').tooltip();
            },
            columns: [
                {
                    field: 'cnpj',
                    title: 'CNPJ'
                },
                {
                    field: 'nome_fantasia',
                    title: 'Nome Fantasia'
                },
                {
                    field: 'numero',
                    title: 'Número do Contrato'
                },
                {
                    field: 'qtd_total_parcelas',
                    title: 'Qtd. de Faturas'
                }
            ],
            onExpandRow: function (index, row, $detail) {
                criarTabelaFaturas($detail, row, situacaoFatura);
            }
        });
    }

    function criarTabelaFaturas(htmlPai, dados, situacaoFatura) {
        var pParams = $("[name=p_params]").val();

        var queryParams = function (params) {
            params['act'] = 'listarFaturas';
            params['situacao_fatura'] = situacaoFatura;
            params['contrato_id'] = dados.id;
            params['p'] = pParams;
            return params;
        };

        htmlPai.html('<table></table>').find('table').bootstrapTable({
            url: window.location.href,
            contentType: 'application/x-www-form-urlencoded; charset=UTF-8',
            method: 'post',
            pagination: true,
            sidePagination: "server",
            pageSize: '12',
            queryParams: queryParams,
            onAll: function () {
                $('[data-toggle="tooltip"]').tooltip();
            },
            columns: [
                {
                    field: 'descricao',
                    title: 'Descrição'
                },
                {
                    field: 'valor',
                    title: 'Valor',
                    formatter: 'formataValor'
                },
                {
                    field: 'data_vencimento',
                    title: 'Data de Vencimento',
                    formatter: 'formatarData'
                }
            ]
        });
    }
</script>