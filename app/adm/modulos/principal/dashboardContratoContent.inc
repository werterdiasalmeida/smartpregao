<?php
$mDashboardContrato = new DashboardContrato();
$mEstabelecimento = new Estabelecimento();

$param = getParam();
//Pegando as variáveis do Param, se existirem
if ($param['estabelecimento_id']) {
    $_REQUEST['estabelecimento_id'] = array($param['estabelecimento_id']);
}

if ($param['tipo']) {
    $_REQUEST['tipo'] = $param['tipo'];
    $_REQUEST['inicio'] = $param['inicio'] ? $param['inicio'] : $_REQUEST['inicio'];
    $_REQUEST['fim'] = $param['fim'] ? $param['fim'] : $_REQUEST['fim'];
}

//Recuperando os valores de filtros enviados na tela anterior
$ids = $_REQUEST['estabelecimento_id'] ? $_REQUEST['estabelecimento_id'] : array();
$tipo = $_REQUEST['tipo'] ? $_REQUEST['tipo'] : 'U';
$data_inicio = $_REQUEST['inicio'] ? formata_data_sql($_REQUEST['inicio']) : null;
$data_termino = $_REQUEST['fim'] ? formata_data_sql($_REQUEST['fim']) : null;

//Definições da combo de período
$textoTipoPadrao = "Últimos 30 dias";
$dataInicio = (new DateTime())->sub(new DateInterval('P30D'));
$dataFim = (new DateTime());
if ($tipo == 'H') {
    $textoTipoPadrao = "Hoje";
    $dataInicio = (new DateTime());
    $dataFim = (new DateTime());
} elseif ($tipo == 'E') {
    $textoTipoPadrao = "Esta semana";
    $dataInicio = (new DateTime())->format('N') == 0 ? (new DateTime()) : (new DateTime())->modify('last sunday');
    $dataFim = clone $dataInicio;
    $dataFim = $dataFim->add(new DateInterval('P6D'));

} elseif ($tipo == 'D') {
    $dataInicio = new DateTime($data_inicio);
    $dataFim = new DateTime($data_termino);
    $textoTipoPadrao = $dataInicio->format('d/m/Y') . ' até ' . $dataFim->format('d/m/Y');
} elseif ($tipo == 'A') {
    $textoTipoPadrao = "Este ano";
    $dataInicio = (new DateTime())->setDate((new DateTime())->format('Y'), 1, 1);;
    $dataFim = (new DateTime())->setDate((new DateTime())->format('Y'), 12, 31);;
}

$inicioFormatada = $dataInicio->format('d/m/Y');
$fimFormatada = $dataFim->format('d/m/Y');

//Carregando as informações dos gráficos
$arWhere = array(
    'inicio' => $dataInicio->format('Y-m-d'),
    'fim' => $dataFim->format('Y-m-d'),
    'estabelecimento_id' => $ids
);

$pWhere = setParam($arWhere, false);

$dadosDashboard = $mDashboardContrato->getDadosDashboard($arWhere);
?>
<div class="page-bar portlet light bordered"
     style="margin-top: 0; margin-bottom: 20px; padding: 5px 20px 0 20px; border-right: 0 !important; border-left: 0 !important;">
    <div class="portlet-body">
        <div class="row">
            <div class="col-sm-6 col-md-6">
                <div class="col-sm-3" style="padding-top: 5px; padding-right: 5px;">
                    Período:
                </div>
                <div class="col-sm-9">
                    <div class="dropdown" style="width: 100%">
                        <button class="btn btn-default dropdown-toggle" type="button" id="dropdownMenu1"
                                data-toggle="dropdown" aria-haspopup="true" aria-expanded="true"
                                style="width: 100%">
                            <span class="label-periodo" style="float: left;"><?= $textoTipoPadrao ?></span>
                            <span class="bs-caret" style="float: right;">
                                                <i class="fa fa-angle-down" aria-hidden="true"></i>
                                            </span>
                        </button>
                        <ul class="dropdown-menu" aria-labelledby="dropdownMenu1" style="width: 300px;">
                            <li><a href="javascript: alterarPeriodo('H')">Hoje</a></li>
                            <li><a href="javascript: alterarPeriodo('E')">Esta semana</a></li>
                            <li><a href="javascript: alterarPeriodo('U')">Últimos 30 dias</a></li>
                            <li><a href="javascript: alterarPeriodo('A')">Este ano</a></li>
                            <li role="separator" class="divider"></li>
                            <li>
                                <div style="padding: 5px">
                                    <div class="">
                                        Período de
                                    </div>
                                    <div class="input-group">
                                        <div>
                                            <input type="text" id="inicio" name="inicio"
                                                   value="<?= $inicioFormatada ?>"
                                                   class="normal data-dropdown form-control" onchange=""
                                                   data-date-format="dd/mm/yyyy">
                                        </div>
                                        <span class="input-group-addon">Até</span>
                                        <div>
                                            <input type="text" id="fim" name="fim" value="<?= $fimFormatada ?>"
                                                   class="normal data-dropdown form-control" onchange=""
                                                   data-date-format="dd/mm/yyyy">
                                        </div>
                                        <span class="input-group-btn">
                                           <button type="button" class="btn btn-filtrar"
                                                   onclick="alterarPeriodo('D')"><i class="fa fa-search"
                                                                                    aria-hidden="true"></i></button>
                                       </span>

                                    </div>
                                </div>

                            </li>
                        </ul>
                        <input type="hidden" name="tipoPeriodo" value="">
                        <input type="hidden" name="p_params" value="<?= $pWhere ?>">
                    </div>
                </div>
            </div>
            <div class="col-sm-6 col-md-6">
                <div class="form-group">
                    <div class="row">
                        <div class="col-sm-5 col-md-3 col-lg-3">
                            Estabelecimento:
                        </div>
                        <div class="col-sm-7 col-md-9  col-lg-9">
                            <?php
                            $estabelecimentos = $mEstabelecimento->getComboPorResponsabilidade();
                            $comboEstabelecimentos = $mEstabelecimento->monta_combo('estabelecimento_id', $estabelecimentos, 'S', 'Todos', "", '', '', '', '', '', true);
                            echo $comboEstabelecimentos;
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-bar-chart"></i>
            <span class="caption-subject bold uppercase">
                Contratos
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 green-meadow"
                   href="javascript: abrirTabelaDetalheContrato('<?= Contrato::SITUACAO_CONTRATO_ATIVO ?>')">
                    <div class="visual">
                        <i class="fa fa-check-square"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup"
                                  data-value="<?= $dadosDashboard['dados_contrato']['qtd_ativo'] ?>">0</span>
                        </div>
                        <div class="desc"> Contratos ativos</div>
                    </div>
                </a>
            </div>
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 yellow-saffron"
                   href="javascript: abrirTabelaDetalheContrato('<?= Contrato::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO ?>')">
                    <div class="visual">
                        <i class="fa fa-info-circle"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup"
                                  data-value="<?= $dadosDashboard['dados_contrato']['qtd_ativo_pendente_pagamento'] ?>">0</span>
                        </div>
                        <div class="desc"> Contratos com pendência de pagamento</div>
                    </div>
                </a>
            </div>
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 red-sunglo"
                   href="javascript: abrirTabelaDetalheContrato('<?= Contrato::SITUACAO_CONTRATO_CANCELADO ?>')">
                    <div class="visual">
                        <i class="fa fa-times-circle"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup"
                                  data-value="<?= $dadosDashboard['dados_contrato']['qtd_cancelado'] ?>">0</span>
                        </div>
                        <div class="desc"> Contratos cancelados</div>
                    </div>
                </a>
            </div>
        </div>
    </div>
</div>


<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-money"></i>
            <span class="caption-subject bold uppercase">
                Faturamento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="padding: 0">
                <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">
                    <a class="dashboard-stat dashboard-stat-v2 blue-sharp"
                       href="javascript: abrirTabelaDetalheFatura('<?= Fatura::SITUACAO_FATURA_PAGA ?>')">
                        <div class="visual">
                            <i class="fa fa-arrow-down"></i>
                        </div>
                        <div class="details">
                            <div class="number">
                                R$ <span data-counter="counterup"
                                         data-value="<?= formata_valor($dadosDashboard['dados_faturamento']['vlr_parcelas_pagas']); ?>">0</span>
                            </div>
                            <div class="desc"> Valor pago</div>
                        </div>
                    </a>
                </div>
                <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">
                    <a class="dashboard-stat dashboard-stat-v2 grey-silver"
                       href="javascript: abrirTabelaDetalheFatura('<?= Fatura::SITUACAO_FATURA_GERADA ?>')">
                        <div class="visual">
                            <i class="fa fa-hourglass"></i>
                        </div>
                        <div class="details">
                            <div class="number">
                                R$ <span data-counter="counterup"
                                         data-value="<?= formata_valor($dadosDashboard['dados_faturamento']['vlr_parcelas_a_vencer']); ?>">0</span>
                            </div>
                            <div class="desc"> Valor a receber</div>
                        </div>
                    </a>
                </div>
                <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">
                    <a class="dashboard-stat dashboard-stat-v2 red-sunglo"
                       href="javascript: abrirTabelaDetalheFatura('<?= Fatura::SITUACAO_FATURA_VENCIDA ?>')">
                        <div class="visual">
                            <i class="fa fa-times-circle"></i>
                        </div>
                        <div class="details">
                            <div class="number">
                                R$ <span data-counter="counterup"
                                         data-value="<?= formata_valor($dadosDashboard['dados_faturamento']['vlr_parcelas_vencidas']); ?>">0</span>
                            </div>
                            <div class="desc"> Valor vencido</div>
                        </div>
                    </a>
                </div>
            </div>
            <div class="col-lg-8 col-md-8 col-sm-8 col-xs-12">
                <div id="grafico-faturas" style="height: 300px; margin: 35px;"></div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function () {
        $('#inicio').datepicker({
            orientation: "left",
            autoclose: true,
            language: 'pt-BR'
        }).on('changeDate', function () {
            $('#fim').datepicker('setStartDate', new Date($(this).data('datepicker').getFormattedDate('yyyy-mm-dd 00:00:00')));
        });

        $('#fim').datepicker({
            orientation: "left",
            autoclose: true,
            language: 'pt-BR'
        }).on('changeDate', function () {
            $('#inicio').datepicker('setEndDate', new Date($(this).data('datepicker').getFormattedDate('yyyy-mm-dd 00:00:00')));
        });

        $('[name=estabelecimento_id]').selectize({
            persist: false,
            maxItems: null,
            placeholder: 'Todos',
            plugins: ['remove_button'],
            valueField: 'codigo',
            labelField: 'descricao',
            searchField: ['descricao'],
            sortField: [
                {field: 'descricao', direction: 'asc'}
            ],
            items: <?= json_encode($ids) ?>,
            onChange: function () {
                carregarDashboard();
            }
        });

        Highcharts.chart('grafico-faturas', {
            chart: {
                plotBackgroundColor: null,
                backgroundColor: null,
                plotBorderWidth: 0,
                plotShadow: false,
                type: 'pie'
            },
            title: {
                text: null
            },
            tooltip: {
                pointFormat: 'Porcentagem: <b>{point.percentage:.1f}%</b>' +
                    '<br>Valor: <b>R$ {point.valor}</b>'
            },
            lang: {
                noData: "<p style='font-size: 20px;'>Nenhum lançamento no período</p>"
            },
            noData: {
                style: {
                    useHTML: true
                }
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    cursor: 'pointer',
                    dataLabels: {
                        enabled: false
                    },
                    showInLegend: true
                }
            },
            credits: {
                enabled: false
            },
            series: <?= $dadosDashboard['dados_grafico_faturamento'] ?>
        });
    });

    function alterarPeriodo(tipo) {
        $('#tipoPeriodo').val(tipo);
        carregarDashboard();
    }
</script>