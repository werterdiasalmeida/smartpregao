<?php
if($_POST['act']) {
    switch ($_POST['act']) {
        case 'listaAcessoProfissionais' :
            include_once 'listaAcessoProfissionais.inc';
            die;
            break;
    }
}

include APPRAIZ . "includes/cabecalho.inc";
monta_titulo($titulo_modulo, '');

$mDispositivo = new Dispositivo();
$mProfissional = new Profissional();
$dispositivosRegistrados = $mDispositivo->gerarRelatorioDispositivosRegistrados(true);
$dispositivosPlataforma = $mDispositivo->gerarRelatorioDispositivosPlataforma(true);
$profissionaisApp = $mProfissional->gerarRelatorioProfissionaisApp(true);
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <td style="width: 50%;">
            <h3 style="text-align: center;">
                Dispositivos por plataforma
            </h3>
            <div id="dispositivos-plataforma" style="height: 350px;"></div>
        </td>
        <td style="width: 50%;">
            <h3 style="text-align: center;">
                Acesso de profissionais de saúde ao app
            </h3>
            <div id="profissionais-acesso-app" style="height: 350px;"></div>
        </td>
    </tr>
    <tr>
        <td colspan="2">
            <h3 style="text-align: center;">
                Dispositivos registrados por plataforma
            </h3>
            <div id="dispositivos-registrados" style="height: 350px;"></div>
        </td>
    </tr>
</table>

<div class="modal fade in" id="modal-acesso-profissional" tabindex="-1" role="modal-acesso-profissional" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Acesso de profissionais de saúde ao APP</h4>
            </div>
            <div class="modal-body">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn" data-dismiss="modal" style="color: #FFF;">Fechar</button>
            </div>
        </div>
    </div>
</div>

<link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
<link href="/includes/metronic/global/plugins/bootstrap-table/bootstrap-table.min.css" rel="stylesheet" type="text/css" />

<script src="/includes/metronic/global/plugins/jquery.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/highcharts/js/highcharts.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap-table/bootstrap-table.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap-table/locale/bootstrap-table-pt-BR.min.js" type="text/javascript"></script>

<script type="text/javascript">
    let AppDashboard = {
        init : function () {
            this.desenharGraficoDispositivosRegistrados();
            this.desenharGraficoDispositivosPlataforma();
            this.desenharGraficoProfissionaisAcessoApp();
        },
        desenharGraficoDispositivosRegistrados : function () {
            Highcharts.chart('dispositivos-registrados', {
                chart: {
                    type: 'areaspline',
                    plotBackgroundColor: null,
                    backgroundColor: null
                },
                credits: {
                    enabled: false
                },
                title: null,
                legend: {
                    itemMarginBottom: 10
                },
                tooltip: {

                },
                yAxis: {
                    title: null
                },
                xAxis: {
                    categories: <?= $dispositivosRegistrados['categorias'] ?>
                },
                plotOptions: {
                    area: {
                        marker: {
                            enabled: false,
                            symbol: 'circle',
                            radius: 2,
                            states: {
                                hover: {
                                    enabled: true
                                }
                            }
                        }
                    }
                },
                series: <?= $dispositivosRegistrados['dados'] ?>
            });
        },
        desenharGraficoDispositivosPlataforma : function () {
            Highcharts.chart('dispositivos-plataforma', {
                chart: {
                    type: 'column',
                    plotBackgroundColor: null,
                    backgroundColor: null
                },
                credits: {
                    enabled: false
                },
                title: {
                    text: null
                },
                subtitle: {
                    text: null
                },
                xAxis: {
                    categories: <?= $dispositivosPlataforma['categorias'] ?>,
                    crosshair: true
                },
                yAxis: {
                    min: 0,
                    title: {
                        text: ''
                    }
                },
                plotOptions: {
                    column: {
                        pointPadding: 0.2,
                        borderWidth: 0
                    }
                },
                series: <?= $dispositivosPlataforma['dados'] ?>
            });
        },
        desenharGraficoProfissionaisAcessoApp : function () {
            Highcharts.chart('profissionais-acesso-app', {
                chart: {
                    type: 'pie',
                    plotBackgroundColor: null,
                    backgroundColor: null
                },
                credits: {
                    enabled: false
                },
                title: {
                    text: ''
                },
                tooltip: {
                    pointFormat: '{series.name}: <b>{point.y}</b> <small>({point.percentage:.1f}%)</small>'
                },
                plotOptions: {
                    pie: {
                        events: {
                            click: function (event) {
                                AppDashboard.exibirProfissionaisAcessoApp(event.point.options.tipo)
                            }
                        },
                        allowPointSelect: true,
                        cursor: 'pointer',
                        dataLabels: {
                            enabled: false
                        },
                        showInLegend: true
                    }
                },
                series: [{
                    name: 'Profissionais de saúde',
                    colorByPoint: true,
                    data: <?= $profissionaisApp ?>
                }]
            });
        },
        exibirProfissionaisAcessoApp : function (tipo) {
            let modalAcessoProfissional = $('#modal-acesso-profissional');
            switch (tipo) {
                case 'acessou' :
                    modalAcessoProfissional.find('.modal-title').html('Profissionais de saúde que acessaram o APP');
                    break;
                case 'nao_acessou' :
                    modalAcessoProfissional.find('.modal-title').html('Profissionais de saúde que não acessaram o APP');
                    break;
                default :
                    modalAcessoProfissional.find('.modal-title').html('');
                    break;
            }

            modalAcessoProfissional.find('.modal-body').html('<img src="../imagens/carregando.gif" /> Carregando...');
            modalAcessoProfissional.find('.modal-body').load(
                window.location.href, {act : 'listaAcessoProfissionais', tipo : tipo}, function(resp){}
            );
            modalAcessoProfissional.modal('show');
        }
    };

    $(function(){
        AppDashboard.init();
    });
</script>

