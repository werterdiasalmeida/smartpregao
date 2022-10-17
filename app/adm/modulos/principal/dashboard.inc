<?php

$mDashboard = new Dashboard();

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'carregarDashboard' :
            require  'dashboardContent.inc';
            die();
            break;
        case 'carregaVisaoGeral' :
            require  'visaoGeralContent.inc';
            die();
            break;
        case 'carregarDadosByTipoConsulta' :
            $param = getParam();
            $param['tipo_consulta_id'] = $_REQUEST['tipo_consulta_id'];
            $param['especialidade_id'] = $_REQUEST['especialidade_id'];
            $param['pessoafisica_id']  = $_REQUEST['pessoafisica_id'];

            $dados = $mDashboard->getInformacoesComplementaresAgendaPorTipo($param);
            $json = json_encode(utf8ArrayEncode($dados ? $dados : array()));
            die($json);
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
?>
<style type="text/css">

</style>

<input type="hidden" id="tipoPeriodo" value="">
<div class="dashboard-content">
</div>

<script src="/includes/metronic/global/plugins/highcharts/js/highcharts.js?v=<?= VERSAO ?>" type="text/javascript"></script>
<script type="text/javascript">
    $(function () {
        <? if($_REQUEST['acao'] == 'G'): ?>
            carregaVisaoGeral('<?= $_REQUEST['p'] ?>');
        <? else: ?>
            carregaDashboard('<?= $_REQUEST['p'] ?>');
        <? endif; ?>
    });

    function carregaDashboard(p){
        //carregando
        App.blockUI({message: 'Carregando...',target: '.page-content'});

        var data = {
            act: 'carregarDashboard',
            estabelecimento_id: $('[name=estabelecimento_id]').val(),
            inicio: $('#inicio').val(),
            fim: $('#fim').val(),
            tipo: $('#tipoPeriodo').val(),
            p : p
        }

        $('.dashboard-content').load('?modulo=principal/dashboard&acao=C', data, function(){
            App.unblockUI('.page-content');
        });
    }

    function carregaVisaoGeral(p){
        //carregando
        App.blockUI({message: 'Carregando...',target: '.page-content'});

        var data = {
            act: 'carregaVisaoGeral',
            estabelecimento_id: $('[name=estabelecimento_id]').val(),
            inicio: $('#inicio').val(),
            fim: $('#fim').val(),
            tipo: $('#tipoPeriodo').val(),
            p : p
        }

        $('.dashboard-content').load('?modulo=principal/dashboard&acao=G', data, function(){
            App.unblockUI('.page-content');
        });
    }

    function filtrarDadosTela(tipo) {
        if(tipo){
            $('#tipoPeriodo').val(tipo);
        }

        <? if($_REQUEST['acao'] == 'G'): ?>
        carregaVisaoGeral();
        <? else: ?>
        carregaDashboard();
        <? endif; ?>
    }

    function alteraDivsPacientesAtendidos(id) {
        if(id){
            $('.div-pacientes-atendidos').removeClass('div-pacientes-atendidos-selecionado');
            $('[id^=donut-pacientes-atendidos-]:not([id$="content"])').hide();

            $('.paciente_atendido_' + id).addClass('div-pacientes-atendidos-selecionado');

            var table = $('#tabela-botoes-pacientes-atendidos');
            var row = table.bootstrapTable('getRowByUniqueId',id);
            if(row && row.pagina){
                if(table.bootstrapTable('getOptions').pageNumber != row.pagina){
                    table.bootstrapTable('selectPage', row.pagina);
                }
            }
        }else{
            var arId = $('[id^=donut-pacientes-atendidos-]:visible:not([id$="content"])').attr('id').split('donut-pacientes-atendidos-');
            id = arId[1];

            $('.paciente_atendido_' + id).addClass('div-pacientes-atendidos-selecionado');
        }

        var divElemento = $('#donut-pacientes-atendidos-' + id);
        if(id != 0 && $('[name=tipo-paciente-atendido]:checked').val() == 'M'){
            divElemento = $('#donut-pacientes-atendidos-profissional-' + id);
        }
        divElemento.show();
    }
</script>
