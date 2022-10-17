<?php
$mDashboard = new Dashboard();
$mEstabelecimento = new Estabelecimento();

$param = getParam();

//Pegando as variáveis do Param, se existirem
if($param['estabelecimento_id']){
    $_REQUEST['estabelecimento_id'] = array($param['estabelecimento_id']);
}

if($param['tipo']){
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
$dataFim    = (new DateTime());
if($tipo == 'H'){
    $textoTipoPadrao = "Hoje";
    $dataInicio = (new DateTime());
    $dataFim    = (new DateTime());
} elseif($tipo == 'E'){
    $textoTipoPadrao = "Esta semana";
    $dataInicio = (new DateTime())->format('N') == 0 ? (new DateTime()) : (new DateTime())->modify('last sunday');
    $dataFim    = clone $dataInicio;
    $dataFim    = $dataFim->add(new DateInterval('P6D'));

} elseif($tipo == 'D'){
    $dataInicio = new DateTime($data_inicio);
    $dataFim    = new DateTime($data_termino);
    $textoTipoPadrao = $dataInicio->format('d/m/Y') . ' até ' . $dataFim->format('d/m/Y');
} elseif($tipo == 'A'){
    $textoTipoPadrao = "Este ano";
    $dataInicio = (new DateTime())->setDate((new DateTime())->format('Y'), 1, 1);;
    $dataFim    = (new DateTime())->setDate((new DateTime())->format('Y'), 12, 31);;
}

$inicioFormatada = $dataInicio->format('d/m/Y');
$fimFormatada = $dataFim->format('d/m/Y');

//Definições da combo de estabelecimento
$estabelecimentos = $mEstabelecimento->getComboPorResponsabilidade();
$comboEstabelecimentos = $mEstabelecimento->monta_combom('estabelecimento_id', $estabelecimentos, 'S', 'Todos', "",  '', '', '', '', '', true);

//Carregando as informações dos gráficos
$arWhere = array(
    'inicio' => $dataInicio->format('Y-m-d'),
    'fim' => $dataFim->format('Y-m-d'),
    'estabelecimento_id' => $ids
);


$dadosAgendaPorTipo = $mDashboard->getInformacoesAgendaPorTipo($arWhere);

$totalConsultorios = 0;
$totalFaturamento = 0;
$qtdProcedimentosRemunerados = 0;
$pacientesAtendidos = array();
$faturamentoPorTipo = array();
$faturamentoTodosTipo = array();
$faturamentoPorProfissional = array();
$faturamentoPorSala = array();
$arAtendimentosPeriodo = array();

foreach($dadosAgendaPorTipo AS $dado){
    $pacientesAtendidos[$dado['tipo_consulta']][$dado['especialidade']]['name'] = $dado['especialidade'];
    $pacientesAtendidos[$dado['tipo_consulta']][$dado['especialidade']]['y']++;
    $pacientesAtendidos[$dado['tipo_consulta']][$dado['especialidade']]['id'] = $dado['tipo_consulta_id'];
    $pacientesAtendidos[$dado['tipo_consulta']][$dado['especialidade']]['especialidade_id'] = $dado['especialidade_id'];

    $faturamentoPorProfissional[$dado['tipo_consulta']][$dado['profissional_pessoafisica_id']]['name'] = $dado['profissional'];
    $faturamentoPorProfissional[$dado['tipo_consulta']][$dado['profissional_pessoafisica_id']]['y']++;
    $faturamentoPorProfissional[$dado['tipo_consulta']][$dado['profissional_pessoafisica_id']]['id'] = $dado['tipo_consulta_id'];
    $faturamentoPorProfissional[$dado['tipo_consulta']][$dado['profissional_pessoafisica_id']]['profissional_pessoafisica_id'] = $dado['profissional_pessoafisica_id'];

    $faturamentoPorTipo[$dado['tipo_consulta']]['valor'] += $dado['valor'];
    $faturamentoPorTipo[$dado['tipo_consulta']]['nome'] = $dado['tipo_consulta'];
    $faturamentoPorTipo[$dado['tipo_consulta']]['id'] = $dado['tipo_consulta_id'];
    $faturamentoPorTipo[$dado['tipo_consulta']]['qtd']++;

    $totalFaturamento += $dado['valor'];
    $totalConsultorios += $dado['valor'];

    $faturamentoPorSala[(!empty($dado['sala']) ? $dado['sala'] : 'Outros')]['valor'] += $dado['valor'];
    $faturamentoPorSala[(!empty($dado['sala']) ? $dado['sala'] : 'Outros')]['qtd']++;

    $arAtendimentosPeriodo[$dado['dia_consulta']][$dado['tipo_consulta']][] = $dado['valor'];

    if($dado['remunerada'] == 't'){
        $qtdProcedimentosRemunerados++;
    }
}

function ordenarArrayFaturamento($a,$b)
{
    return ($a["y"] >= $b["y"]) ? -1 : 1;
}

foreach ($pacientesAtendidos AS $chave => $dado){
    $dado = array_values($dado ? $dado : array());
    usort($dado, "ordenarArrayFaturamento");
    $pacientesAtendidos[$chave] = json_encode(utf8ArrayEncode(array_values($dado ? $dado : array())));
}

foreach ($faturamentoPorProfissional AS $chave => $dado){
    $dado = array_values($dado ? $dado : array());
    usort($dado, "ordenarArrayFaturamento");
    $faturamentoPorProfissional[$chave] = json_encode(utf8ArrayEncode($dado));
}


//Gerando o array da opção "Todos" de "Pacientes Atendidos"
foreach ($faturamentoPorTipo AS $chave => $dado){
    $faturamentoTodosTipo[] = array('name' => $dado['nome'], 'y' => $dado['qtd'], 'id' => $dado['id']);
}
usort($faturamentoTodosTipo, "ordenarArrayFaturamento");

$totalConsultorios = count($faturamentoPorSala);

uasort($faturamentoPorTipo, function($a, $b) {
    return ($a['qtd'] > $b['qtd']) ? -1 : 1;
});

//Organizando e calculando o ticket médio por tipo
$pagina = 1;
$qtdPorPagina = 2;
foreach ($faturamentoPorTipo AS $chave => $dado){
    $faturamentoPorTipo[$chave]['ticket'] = $dado['valor'] / ($dado['qtd'] != 0 ? $dado['qtd'] : 1);

    if($qtdPorPagina > 3){
        $pagina++;
        $qtdPorPagina = 1;
    }
    $faturamentoPorTipo[$chave]['ticket'] = $dado['valor'] / ($dado['qtd'] != 0 ? $dado['qtd'] : 1);
    $faturamentoPorTipo[$chave]['pagina'] = $pagina;

    $qtdPorPagina++;
}

//Organizando as informações dos registros por sala
$cor = 1;
$totalRegistrosPorSala = 0;
$arSalasData = array();
foreach ($faturamentoPorSala AS $indice => $sala){
    $totalRegistrosPorSala += $sala['qtd'];
    $arSalasData[$indice]['y'] = $sala['qtd'];
    $arSalasData[$indice]['valor'] = $sala['valor'];
    $arSalasData[$indice]['color'] = $cor % 2 == 0 ? '#1B9CFC' : '#337ab7';

    $cor++;
}

//Organizando as informações do gráfico de atendimentos no período
$arDadosGraficoPeriodo = array();
foreach ($arAtendimentosPeriodo AS $data => $atendimentos){
    foreach ($faturamentoPorTipo AS $tipo => $dados){
        $arDadosGraficoPeriodo[$tipo][$data]['y'] = (count($atendimentos[$tipo]));
        $arDadosGraficoPeriodo[$tipo][$data]['valor'] = array_sum(is_array($atendimentos[$tipo]) ? $atendimentos[$tipo] : array());
    }
}

foreach ($arDadosGraficoPeriodo AS $tipo => $dado){
    $arDadosGraficoPeriodo[$tipo] = array(
            'name' => $tipo,
            'color' => '',
            'data' => array_values($dado)
    );
}

//Passando tudo pra json para os gráficos
$jsonDadosGraficoPeriodo = json_encode(utf8ArrayEncode(array_values($arDadosGraficoPeriodo ? $arDadosGraficoPeriodo : array())));
$jsonDatas = json_encode(utf8ArrayEncode(array_keys($arAtendimentosPeriodo ? $arAtendimentosPeriodo : array())));
$jsonSalas = json_encode(utf8ArrayEncode(array_keys($faturamentoPorSala ? $faturamentoPorSala : array())));
$jsonSalasData = json_encode(utf8ArrayEncode(array_values($arSalasData ? $arSalasData : array())));
$jsonFaturamentoPorTipo = json_encode(utf8ArrayEncode(array_values($faturamentoPorTipo ? $faturamentoPorTipo : array())));
$jsonFaturamentoTodosTipo = json_encode(utf8ArrayEncode($faturamentoTodosTipo ? $faturamentoTodosTipo : array()));
$jsonFaturamentoPorProfissional = json_encode(utf8ArrayEncode(array_values($faturamentoPorProfissional ? $faturamentoPorProfissional : array())));

$pAjax = setParam($arWhere, false);
?>
<style type="text/css">
    .arrow-down {
        width: 0;
        height: 0;
        border-left: 15px solid transparent;
        border-right: 15px solid transparent;
        border-top: 15px solid;
        margin: auto;
    }

    .data-dropdown{
        width: 100px ;
        display: inline;
    }

    .price-table-head {
        color: #fff;
        padding: 20px 0;
    }

    .bg-blue {
        background: #3598dc !important;
    }

    .div-pacientes-atendidos {
        border: 1px solid #ffffff;
    }

    .div-pacientes-atendidos:hover {
        border: 1px solid #337ab7;
    }

    .div-pacientes-atendidos-selecionado {
        font-weight: bolder !important;
        color: #ffffff !important;
        background-color: #337ab7 !important;
        border: 1px solid #337ab7 !important;
    }

    .dashboard-stat.rosa {
        background-color: #FC427B;
        color: #fff;
    }

    .dashboard-stat.verde {
        background-color: #74C8B4;
        color: #fff;
    }

    .dashboard-stat.azul-claro {
        background-color: #25CCF7;
        color: #fff;
    }

    .dashboard-stat.azul-escuro {
        background-color: #1B9CFC;
        color: #fff;
    }

    .dashboard-stat.rosa .visual > i, .dashboard-stat.azul-claro .visual > i, .dashboard-stat.verde .visual > i, .dashboard-stat.azul-escuro .visual > i{
        color: #fff;
        opacity: .1;
        filter: alpha(opacity=10);
    }

    #tabela-botoes-pacientes-atendidos{
        border: 0px;
    }
    #tabela-botoes-pacientes-atendidos tr {
        border: 0px;
    }

    #tabela-botoes-pacientes-atendidos tr td {
        border: 0px;
    }

    .div-tabela-botoes .fixed-table-container {
        border: 0px !important;
    }

    .modal-grande {
        width: 90%;
    }

    .titulo-donut-pacientes-atendidos, titulo-donut-pacientes-atendidos-profissional{
        text-align: center;
        font-weight: 700!important;
        font-size: 16px;
        color: #666;
    }
</style>

<div class="page-bar portlet light bordered" style="margin-top: 0; margin-bottom: 20px; padding: 5px 20px 0 20px; border-right: 0 !important; border-left: 0 !important;">
    <div class="portlet-body">
        <div class="row">
            <div class="col-sm-6 col-md-6">
                <div class="col-sm-3" style="padding-top: 5px; padding-right: 5px;">
                    Período:
                </div>
                <div class="col-sm-9">
                    <div class="dropdown" style="width: 100%">
                        <button class="btn btn-default dropdown-toggle" type="button" id="dropdownMenu1" data-toggle="dropdown" aria-haspopup="true" aria-expanded="true" style="width: 100%">
                            <span class="label-periodo" style="float: left;"><?= $textoTipoPadrao ?></span>
                            <span class="bs-caret" style="float: right;">
                                                <i class="fa fa-angle-down" aria-hidden="true"></i>
                                            </span>
                        </button>
                        <ul class="dropdown-menu" aria-labelledby="dropdownMenu1" style="width: 300px;">
                            <li><a href="javascript: filtrarDadosTela('H')">Hoje</a></li>
                            <li><a href="javascript: filtrarDadosTela('E')">Esta semana</a></li>
                            <li><a href="javascript: filtrarDadosTela('U')">Últimos 30 dias</a></li>
                            <li><a href="javascript: filtrarDadosTela('A')">Este ano</a></li>
                            <li role="separator" class="divider"></li>
                            <li>
                                <div style="padding: 5px">
                                    <div class="">
                                        Período de
                                    </div>
                                    <div class="input-group">
                                        <div>
                                            <input type="text" id="inicio" name="inicio" value="<?= $inicioFormatada ?>" class="normal data-dropdown form-control" onchange="" data-date-format="dd/mm/yyyy">
                                        </div>
                                        <span class="input-group-addon">Até</span>
                                        <div>
                                            <input type="text" id="fim" name="fim" value="<?= $fimFormatada ?>" class="normal data-dropdown form-control" onchange="" data-date-format="dd/mm/yyyy">
                                        </div>
                                        <span class="input-group-btn">
                                           <button type="button" class="btn btn-filtrar" onclick="filtrarDadosTela('D')"><i class="fa fa-search" aria-hidden="true"></i></button>
                                       </span>

                                    </div>
                                </div>

                            </li>
                        </ul>
                        <input type="hidden" name="tipoPeriodo" value="">
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
                            <?= $comboEstabelecimentos ?>
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
                Painel Administrativo
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 verde" href="javascript: exibeDetalheTicketMedico()">
                    <div class="visual">
                        <i class="fa fa-user"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            R$ <span data-counter="counterup" data-value="formata_valor(($qtdProcedimentosRemunerados != 0 ? ($totalFaturamento/$qtdProcedimentosRemunerados) : $totalFaturamento))"><?= formata_valor(($qtdProcedimentosRemunerados != 0 ? ($totalFaturamento/$qtdProcedimentosRemunerados) : $totalFaturamento)) ?></span>
                        </div>
                        <div class="desc"> Ticket médio</div>
                    </div>
                </a>
            </div>
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 azul-claro" href="#">
                    <div class="visual">
                        <i class="fa fa-usd"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            R$ <span data-counter="counterup" data-value="<?= formata_valor($totalFaturamento); ?> "><?= formata_valor($totalFaturamento); ?> </span>
                        </div>
                        <div class="desc"> Faturamento</div>
                    </div>
                </a>
            </div>
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 purple" href="#">
                    <div class="visual">
                        <i class="fa fa-home"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup" data-value="<?= $totalConsultorios ?>"><?= $totalConsultorios ?> </span>
                        </div>
                        <div class="desc"> Total de consultórios</div>
                    </div>
                </a>
            </div>
        </div>
    </div>
</div>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-users"></i>
            <span class="caption-subject bold uppercase">
                Pacientes atendidos
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12 div-tabela-botoes">
                <table style="width: 100%" id="tabela-botoes-pacientes-atendidos" >
                    <thead>
                        <tr style="display: none;">
                            <th data-field='id'></th>
                            <th data-field='pagina'></th>
                            <th data-field='html'></th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>0</td>
                            <td>1</td>
                            <td>
                                <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12" style="margin-top: 10px;">
                                    <a class="dashboard-stat dashboard-stat-v2 widget-bg-color-white div-pacientes-atendidos paciente_atendido_0"
                                       href="javascript: alteraDivsPacientesAtendidos('0')">
                                        <div class="visual"></div>
                                        <div class="details">
                                            <div class="number">
                                                <span data-counter="counterup" data-value="<?= count($dadosAgendaPorTipo) ?>"><?= count($dadosAgendaPorTipo) ?> </span>
                                            </div>
                                            <div class="desc"> Todos</div>
                                        </div>
                                    </a>
                                </div>
                            </td>
                        </tr>
                        <?php foreach($faturamentoPorTipo AS $nome => $valores): ?>
                            <tr>
                                <td style="display: none"><?= $valores['id'] ?></td>
                                <td style="display: none"><?= $valores['pagina'] ?></td>
                                <td>
                                    <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12" style="margin-top: 10px;">
                                        <a class="dashboard-stat dashboard-stat-v2 widget-bg-color-white div-pacientes-atendidos paciente_atendido_<?= $valores['id'] ?>"
                                           href="javascript: alteraDivsPacientesAtendidos('<?= $valores['id'] ?>')">
                                            <div class="visual"></div>
                                            <div class="details">
                                                <div class="number">
                                                    <span data-counter="counterup" data-value="<?= $valores['qtd'] ?>"><?= $valores['qtd'] ?> </span>
                                                </div>
                                                <div class="desc"> <?= $nome ?></div>
                                            </div>
                                        </a>
                                    </div>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                    </tbody>
                </table>
            </div>
            <div class="col-lg-8 col-md-8 col-sm-8 col-xs-12">
                <div class="text-center">
                    <?php
                        $db->monta_radiom('tipo-paciente-atendido', array(
                            array('codigo' => 'M', 'descricao' => 'Médico'),
                            array('codigo' => 'P', 'descricao' => 'Procedimento'),
                        ), 'S', 'N', 'P')
                    ?>
                </div>
                <div id="donut-pacientes-atendidos-0"><div class="titulo-donut-pacientes-atendidos">Todos</div><div id="donut-pacientes-atendidos-0-content"></div></div>
                <?php foreach($faturamentoPorTipo AS $nome => $valores): ?>
                    <div>
                        <div id="donut-pacientes-atendidos-<?= $valores['id'] ?>"><div class="titulo-donut-pacientes-atendidos"><?= $valores['nome'] ?></div><div id="donut-pacientes-atendidos-<?= $valores['id'] ?>-content"></div></div>
                        <div id="donut-pacientes-atendidos-profissional-<?= $valores['id'] ?>"><div class="titulo-donut-pacientes-atendidos"><?= $valores['nome'] ?></div><div id="donut-pacientes-atendidos-profissional-<?= $valores['id'] ?>-content"></div></div>
                    </div>
                <?php endforeach; ?>
            </div>
        </div>

    </div>
</div>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-home"></i>
            <span class="caption-subject bold uppercase">
                Salas de atendimento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12">
                <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12" style="margin-top: 10px;">
                    <a class="dashboard-stat dashboard-stat-v2 azul-escuro" href="#">
                        <div class="visual">
                            <i class="fa fa-home"></i>
                        </div>
                        <div class="details">
                            <div class="number">
                                R$ <span data-counter="counterup" data-value="<?= formata_valor($totalFaturamento / ($totalConsultorios == 0 ? 1 : $totalConsultorios)); ?>"><?= formata_valor($totalFaturamento / ($totalConsultorios == 0 ? 1 : $totalConsultorios)); ?></span>
                            </div>
                            <div class="desc"> Valor médio/sala</div>
                        </div>
                    </a>
                </div>
                <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12" style="margin-top: 10px;">
                    <a class="dashboard-stat dashboard-stat-v2 rosa" href="#">
                        <div class="visual">
                            <i class="fa fa-home"></i>
                        </div>
                        <div class="details">
                            <div class="number">
                               <span data-counter="counterup" data-value="<?= $totalConsultorios; ?>"><?= $totalConsultorios; ?></span></div>
                            <div class="desc"> Salas cadastradas</div>
                        </div>
                    </a>
                </div>
            </div>
            <div class="col-lg-8 col-md-8 col-sm-8 col-xs-12">
                <div id="grafico-sala-atendimento" style="height: 300px"></div>
            </div>
        </div>
    </div>
</div>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-clock-o"></i>
            <span class="caption-subject bold uppercase">
                Atendimentos no período
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div id="area-atendimento-periodo" style="height: 250px;"></div>
        </div>
    </div>
</div>
<div class="modal fade in" id="modal-informacoes" tabindex="-1" role="modal-informacoes" aria-hidden="true">
    <div class="modal-dialog modal-grande">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">titulo</h4>
            </div>
            <div class="modal-body">
                corpo
            </div>
            <div class="modal-footer">
                <button type="button" class="btn blue" data-dismiss="modal">Fechar</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
</div>
<script type="text/javascript">
    $(function () {
        $('#tabela-botoes-pacientes-atendidos').bootstrapTable({
            cache: false,
            striped: false,
            pagination: true,
            pageSize: 3,
            classes: '',
            uniqueId: 'id',
            height: 460,
            paginationHAlign: 'left',
            formatShowingRows: function() {
                return '';
            },
            onPageChange: function() {
                alteraDivsPacientesAtendidos();
            },
            showHeader: false
        }).bootstrapTable('hideColumn','id').bootstrapTable('hideColumn','pagina');;


        $('#inicio').datepicker({
            orientation: "left",
            autoclose: true,
            language: 'pt-BR'
        }).on('changeDate', function(){
            $('#fim').datepicker('setStartDate', new Date($(this).data('datepicker').getFormattedDate('yyyy-mm-dd 00:00:00')));
        });

        $('#fim').datepicker({
            orientation: "left",
            autoclose: true,
            language: 'pt-BR'
        }).on('changeDate', function(){
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
                filtrarDadosTela();
            }
        });

        Highcharts.setOptions({
            colors: ['#74C8B4', '#25CCF7', '#1B9CFC', '#FC427B', '#EAB543', '#22759B', '#E71E59']
        });

        Highcharts.chart('grafico-sala-atendimento', {
            chart: {
                type: 'column',
                plotBackgroundColor: null,
                backgroundColor: null,
                plotBorderWidth: 0,
                plotShadow: false
            },
            title: {
                text: null
            },
            tooltip: {
                formatter: function () {
                    return "<b>" + this.key + ":</b> " + this.y + " ("+ formataValor((this.y / <?= ($totalRegistrosPorSala != 0 ? $totalRegistrosPorSala : 1) ?>) * 100) + "%)<br><b>Faturamento:</b> R$ " + formataValor(this.point.valor);
                }
            },
            yAxis: {
                title: null
            },
            xAxis: {
                categories: <?= $jsonSalas ?>
            },
            legend: {
                enabled: false
            },
            credits: {
                enabled: false
            },
            series: [{
                name: null,
                data: <?= $jsonSalasData ?>
            }]
        });

        var char0 = Highcharts.chart('donut-pacientes-atendidos-0-content', {
            chart: {
                plotBackgroundColor: null,
                backgroundColor: null,
                plotBorderWidth: 0,
                plotShadow: false
            },
            legend: {
                labelFormatter: function () {
                    return "<b>" + this.name + "</b> ("+ formataValor(this.percentage) + " %)";
                }
            },
            title: {
                text: '<?= count($dadosAgendaPorTipo) ?><br>Pacientes<br>atendidos',
                align: 'center',
                verticalAlign: 'middle',
                y: -40
            },
            tooltip: {
                formatter: function () {
                    return "<b>" + this.key + ":</b> " + this.y + " ("+ formataValor(this.percentage) + " %)";
                }
            },
            plotOptions: {
                pie: {
                    events: {
                        click: function (event) {
                            alteraDivsPacientesAtendidos(event.point.id);
                        }
                    },
                    point: {
                        events: {
                            legendItemClick: function (event) {
                                setTimeout(() => {
                                    if(this.total > 1){
                                        char0.setTitle({ text: this.total + '<br>Pacientes<br>atendidos' });
                                    }else if(this.total == 1){
                                        char0.setTitle({ text: this.total + '<br>Paciente<br>atendido' });
                                    }else{
                                        char0.setTitle({ text: 'Nenhum<br>paciente<br>atendido' });
                                    }

                                }, 100);
                            }
                        }
                    },
                    showInLegend: true,
                    dataLabels: {
                        enabled: true,
                        distance: -50,
                        style: {
                            fontWeight: 'bold',
                            color: 'white'
                        }
                    },
                }
            },
            credits: {
                enabled: false
            },
            series: [{
                dataLabels: {
                    enabled: false,
                    formatter: function () {
                        return formataValor(this.percentage) + " %";
                    },
                    style: {
                        fontSize: 15
                    }
                },
                type: 'pie',
                name: 'Especialidade',
                innerSize: '50%',
                data: <?= $jsonFaturamentoTodosTipo ?>
            }]
        });

        <?php foreach($faturamentoPorTipo AS $indice => $valores): ?>
            char<?= $valores['id'] ?> = Highcharts.chart('donut-pacientes-atendidos-<?= $valores['id'] ?>-content', {
                chart: {
                    plotBackgroundColor: null,
                    backgroundColor: null,
                    plotBorderWidth: 0,
                    plotShadow: false
                },
                legend: {
                    labelFormatter: function () {
                        return "<b>" + this.name + "</b> ("+ formataValor(this.percentage) + " %)";
                    }
                },
                title: {
                    text: '<?= $valores['qtd'] ?><br>Pacientes<br>atendidos',
                    align: 'center',
                    verticalAlign: 'middle',
                    y: -40
                },
                tooltip: {
                    formatter: function () {
                        return "<b>" + this.key + ":</b> " + this.y + " ("+ formataValor(this.percentage) + " %)";
                    }
                },
                plotOptions: {
                    pie: {
                        events: {
                            click: function (event) {
                                exibeDetalhePacientesAtendidos(event.point.id, event.point.especialidade_id, event.point.name);
                            }
                        },
                        point: {
                            events: {
                                legendItemClick: function (event) {
                                    setTimeout(() => {
                                        if(this.total > 1){
                                            char<?= $valores['id'] ?>.setTitle({ text: this.total + '<br>Pacientes<br>atendidos' });
                                        }else if(this.total == 1){
                                            char<?= $valores['id'] ?>.setTitle({ text: this.total + '<br>Paciente<br>atendido' });
                                        }else{
                                            char<?= $valores['id'] ?>.setTitle({ text: 'Nenhum<br>paciente<br>atendido' });
                                        }

                                    }, 100);
                                }
                            }
                        },
                        showInLegend: true,
                        dataLabels: {
                            enabled: true,
                            distance: -50,
                            style: {
                                fontWeight: 'bold',
                                color: 'white'
                            }
                        },
                    }
                },
                credits: {
                    enabled: false
                },
                series: [{
                    dataLabels: {
                        enabled: false,
                        formatter: function () {
                            return formataValor(this.percentage) + " %";
                        },
                        style: {
                            fontSize: 15
                        }
                    },
                    type: 'pie',
                    name: 'Especialidade',
                    innerSize: '50%',
                    data: <?= $pacientesAtendidos[$indice] ?>
                }]

            });

            charprofissional<?= $valores['id'] ?> = Highcharts.chart('donut-pacientes-atendidos-profissional-<?= $valores['id'] ?>-content', {
                chart: {
                    plotBackgroundColor: null,
                    backgroundColor: null,
                    plotBorderWidth: 0,
                    plotShadow: false
                },
                legend: {
                    labelFormatter: function () {
                        return "<b>" + this.name + "</b> ("+ formataValor(this.percentage) + " %)";
                    }
                },
                title: {
                    text: '<?= $valores['qtd'] ?><br>Pacientes<br>atendidos',
                    align: 'center',
                    verticalAlign: 'middle',
                    y: -40
                },
                tooltip: {
                    formatter: function () {
                        return "<b>" + this.key + ":</b> " + this.y + " ("+ formataValor(this.percentage) + " %)";
                    }
                },
                plotOptions: {
                    pie: {
                        events: {
                            click: function (event) {
                                exibeDetalhePacientesAtendidos(event.point.id, event.point.especialidade_id, event.point.name, event.point.profissional_pessoafisica_id);
                            }
                        },
                        point: {
                            events: {
                                legendItemClick: function (event) {
                                    setTimeout(() => {
                                        if(this.total > 1){
                                            char<?= $valores['id'] ?>.setTitle({ text: this.total + '<br>Pacientes<br>atendidos' });
                                        }else if(this.total == 1){
                                            char<?= $valores['id'] ?>.setTitle({ text: this.total + '<br>Paciente<br>atendido' });
                                        }else{
                                            char<?= $valores['id'] ?>.setTitle({ text: 'Nenhum<br>paciente<br>atendido' });
                                        }

                                    }, 100);
                                }
                            }
                        },
                        showInLegend: true,
                        dataLabels: {
                            enabled: true,
                            distance: -50,
                            style: {
                                fontWeight: 'bold',
                                color: 'white'
                            }
                        },
                    }
                },
                credits: {
                    enabled: false
                },
                series: [{
                    dataLabels: {
                        enabled: false,
                        formatter: function () {
                            return formataValor(this.percentage) + " %";
                        },
                        style: {
                            fontSize: 15
                        }
                    },
                    type: 'pie',
                    name: 'Especialidade',
                    innerSize: '50%',
                    data: <?= $faturamentoPorProfissional[$indice] ?>
                }]
            });

        <?php endforeach; ?>

        Highcharts.chart('area-atendimento-periodo', {
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
                formatter: function () {
                    return "<b>" + this.key + "</b><br><b>" + this.series.name + ":</b> " + this.y + "<br><b>Faturamento: R$ </b>" + formataValor(this.point.valor);
                }
            },
            yAxis: {
                title: null
            },
            xAxis: {
                categories: <?= $jsonDatas ?>
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
            series: <?= $jsonDadosGraficoPeriodo ?>
        });

        if($('.div-pacientes-atendidos:first').length > 0){
            $('.div-pacientes-atendidos:first')[0].click();
        }

        $('[name=tipo-paciente-atendido]').change(function(){
            if($('[id^=donut-pacientes-atendidos-0]:visible').length == 0){
                $('[id^=donut-pacientes-atendidos]:visible:not([id$="content"])').hide().siblings().show();
            }
        });
    });

    function exibeDetalheTicketMedico(){
        var modal = $('#modal-informacoes');

        modal.find('.modal-title').html('Ticket médio por tipo de consulta');

        if(modal.find('.modal-dialog').hasClass('modal-grande')){
            modal.find('.modal-dialog').removeClass('modal-grande');
        }

        var html = "<table id='table-ticket-medio' class='table table-striped' data-toggle='table' data-pagination='true' data-page-list='[5, 10, 20, 50, 100, 200]' data-search='true' data-unique-id='id' data-id-field='id'>";
        html += "<thead>";
        html += "<tr>";
        html += "<th data-field='nome' data-sortable='true'>Tipo</th>";
        html += "<th data-field='valor' data-formatter='formataValor' data-sortable='true'>Valor Total</th>";
        html += "<th data-field='qtd' data-sortable='true'>Quantidade</th>";
        html += "<th data-field='ticket' data-formatter='formataValor' data-sortable='true'>Ticket médio</th>";
        html += "</tr>";
        html += "</thead>";
        html += "</table>";

        modal.find('.modal-body').html(html);
        modal.find('#table-ticket-medio').bootstrapTable();
        modal.find('#table-ticket-medio').bootstrapTable('load', <?= $jsonFaturamentoPorTipo ?>);

        modal.modal('show');
    }


    function exibeDetalhePacientesAtendidos(tipo_consulta_id, especialidade_id, especialidade, pessoafisica_id){
        App.blockUI({
            message: 'Carregando...',
            target: '.form-body'
        });

        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType : 'json',
            data: {
                act : 'carregarDadosByTipoConsulta',
                tipo_consulta_id: tipo_consulta_id,
                especialidade_id: especialidade_id,
                pessoafisica_id: pessoafisica_id,
                p: '<?= $pAjax ?>'
            },
            success: function (retorno) {
                var modal = $('#modal-informacoes');

                var tipo = "especialidade";
                if(pessoafisica_id){
                    tipo = "profissional";

                }
                modal.find('.modal-title').html('Lista de pacientes atendidos por ' + tipo + ' - ' + especialidade);

                if(!modal.find('.modal-dialog').hasClass('modal-grande')){
                    modal.find('.modal-dialog').addClass('modal-grande');
                }

                var total = 0;
                $.each(retorno, function(){
                    total = total + parseFloat(this.valor);
                });

                var html = "<table id='table-detalhe-atendimentos' class='table table-striped' data-toggle='table' data-pagination='true' data-page-list='[5, 10, 20, 50, 100, 200]' data-search='true' data-unique-id='id' data-id-field='id'>";
                html += "<thead>";
                html += "<tr>";
                html += "<th data-field='data_consulta' data-sortable='true'>Data</th>";

                var colspan = 5;
                if(!$('[name=estabelecimento_id]').val() || $('[name=estabelecimento_id]').val().length > 1){
                    html += "<th data-field='estabelecimento' data-sortable='true'>Estabelecimento</th>";
                    colspan = 6;
                }

                html += "<th data-field='paciente' data-sortable='true'>Paciente</th>";
                html += "<th data-field='profissional' data-sortable='true'>Profissional</th>";
                html += "<th data-field='convenio' data-sortable='true'>Convênio</th>";
                html += "<th data-field='sala' data-sortable='true'>Sala</th>";
                html += "<th data-field='valor' data-align='right' data-formatter='formataValor' data-sortable='true'>Valor (R$)</th>";
                html += "</tr>";
                html += "<tr>";
                html += "<th data-field='sala' data-align='right' colspan='"+ colspan +"' data-sortable='true'><span class='order-fixed'></span>Total (R$):</th>";
                html += "<th data-field='valor' data-align='right' data-formatter='formataValor' data-sortable='true'><span class='order-fixed'></span>" + formataValor(total) + "</th>";
                html += "</tr>";
                html += "</thead>";
                html += "</table>";

                modal.find('.modal-body').html(html);
                modal.find('#table-detalhe-atendimentos').bootstrapTable();
                modal.find('#table-detalhe-atendimentos').bootstrapTable('load', retorno);
                modal.find('#table-detalhe-atendimentos .order-fixed').parent().removeClass('sortable').removeClass('both').removeClass('asc').removeClass('desc');
                modal.modal('show');

                App.unblockUI('.form-body');


            },
            error: function (result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a operação');
            }
        });
    }
</script>
