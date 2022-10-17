<?php

$mDashboard = new Dashboard();
$mEstabelecimento = new Estabelecimento();

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

$dadosLista = $mDashboard->getInformacoesGerais($arWhere,false);

$totalTicketMedio = 0;
$totalFaturamento = 0;
$totalConsultorios = 0;
$totalValorSalas = 0;
$totalPacientesAtendidos = 0;
$totalPacientesAgendados = 0;
$totalPacientesAtendidosRemunerados = 0;
$totalAgendaTotal = 0;
$totalAgendaDisponivel = 0;

$arJsonAgenda = [];
foreach ($dadosLista AS $indice => $dados){
    $jsonAgenda = json_decode(utf8_decode($dados['json_agenda']),true);
    foreach ($jsonAgenda AS $tipoAgenda){
        $dadosLista[$indice]['agendas'][$tipoAgenda['estado_id']] = $tipoAgenda;
        $dadosLista[$indice]['agenda_total'] += $tipoAgenda['qtd'];

        if($tipoAgenda['estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL){
            $totalAgendaDisponivel += $tipoAgenda['qtd'];
        }
    }

    $totalFaturamento += $dados['valor_remuneradas'];
    $totalConsultorios += $dados['qtd_salas'];
    $totalPacientesAtendidos += $dados['qtd_total'];
    $totalPacientesAgendados += $dados['qtd_total_agendadas'];
    $totalPacientesAtendidosRemunerados += $dados['qtd_remuneradas'];
    $totalAgendaTotal += $dadosLista[$indice]['agenda_total'];

    $arJsonAgenda[$dados['estabelecimento_id']] = $dados['json_agenda'];
}
$arJsonAgenda = utf8ArrayEncode($arJsonAgenda);

$totalValorSalas  = $totalFaturamento / ( $totalConsultorios ?: 1);
$totalTicketMedio = $totalFaturamento / ( $totalPacientesAtendidosRemunerados ?: 1);

?>
<style type="text/css">
    .data-dropdown{
        width: 100px ;
        display: inline;
    }

    .titulo-rowspan{
        background-color: #5E9BCF;
        color: #FFFFFF;
    }

    .tooltip-event {
        width: 350px;
        height: auto;
        position: absolute;
        z-index: 10001;
        padding: 10px 10px 10px 10px;
        line-height: 150%;
        -webkit-border-radius: 10px !important;
        -moz-border-radius: 10px !important;
        border-radius: 10px !important;
        background-color: #f1f4f9;
        border: 1px solid #a5a5a5 !important;
    }

    .tooltip-event label{
        /*width: 100px;*/
        /*margin: 0;*/
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
            <i class="fa fa-list"></i>
            <span class="caption-subject bold uppercase">
                Visão geral
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-12 col-md-12 col-sm-12 col-xs-12">
                <table style="width: 100%" id="tabela-visao-geral" class="table table-striped table-hover">
                    <thead>
                    <tr>
                        <th data-field='estabelecimento_id' data-align='center' rowspan="2">Ação</th>
                        <th data-field='nome' data-sortable='true' rowspan="2">Nome</th>
                        <th data-field='valor' data-align='right' data-formatter='formataValor' data-sortable='true' rowspan="2">Faturamento (R$)</th>
                        <th data-field='qtdConsultorios' data-align='center' data-sortable='true' rowspan="2">Total de</br>consultórios</th>
                        <th data-field='valorMedioSala' data-align='right' data-formatter='formataValor' data-sortable='true' rowspan="2">Valor</br>médio / sala (R$)</th>
                        <th data-field='qtdAgendados' data-align='center' data-sortable='true' rowspan="2">Pacientes</br>agendados</th>
                        <th data-field='qtd' data-align='center' data-sortable='true' rowspan="2">Pacientes</br>atendidos</th>
                        <th data-field='ticketMedio' data-align='right' data-formatter='formataValor' data-sortable='true' rowspan="2">Ticket</br>médio (R$)</th>
                        <th data-field='agenda' data-align='center' colspan="2">Agenda</th>
                    </tr>
                    <tr>
                        <th data-field='agendaTotal' data-align='center' data-sortable='true' class="titulo-rowspan">Total</th>
                        <th data-field='agendaDisponivel' data-align='center' data-sortable='true' class="titulo-rowspan">Disponível</th>
                    </tr>
                    <tr>
                        <th data-field='estabelecimento_id' data-align='right' data-sort="fixed" colspan="2">TOTAIS</th>
                        <th data-field='valor' data-align='right' data-formatter='formataValor' data-sortable='true'><span class="order-fixed"></span><?= formata_valor($totalFaturamento) ?></th>
                        <th data-field='qtdConsultorios' data-align='center' data-sortable='true'><span class="order-fixed"></span><?= $totalConsultorios; ?></th>
                        <th data-field='valorMedioSala' data-align='right' data-formatter='formataValor' data-sortable='true'><span class="order-fixed"></span><?= formata_valor($totalValorSalas) ?></th>
                        <th data-field='qtdAgendados' data-align='center' data-sortable='true'><span class="order-fixed"></span><?= $totalPacientesAgendados ?></th>
                        <th data-field='qtd' data-align='center' data-sortable='true'><span class="order-fixed"></span><?= $totalPacientesAtendidos ?></th>
                        <th data-field='ticketMedio' data-align='right' data-formatter='formataValor' data-sortable='true'><span class="order-fixed"></span><?= formata_valor($totalTicketMedio) ?></th>
                        <th data-field='agendaTotal' data-align='center' data-sortable='true'><span class="order-fixed"></span><?= $totalAgendaTotal ?></th>
                        <th data-field='agendaDisponivel' data-align='center' data-sortable='true'><span class="order-fixed"></span><?= $totalAgendaDisponivel ?></th>
                    </tr>
                    </thead>
                    <tbody>
                    <? foreach ($dadosLista AS $dado): ?>
                        <tr>
                            <td>
                                <div style="white-space: nowrap;">
                                    <a class="btn btn-circle btn-icon-only btn-default green-jungle" href="?modulo=principal/dashboard&acao=C&p=<?= setParam(['estabelecimento_id' => $dado['estabelecimento_id'], 'tipo' => $tipo,'inicio' => $_REQUEST['inicio'], 'fim' => $_REQUEST['fim']],false) ?>" data-toggle="tooltip" title="Visualizar no dashboard">
                                        <i class="icon-eye"></i>
                                    </a>
                                    <? if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($dado['estabelecimento_id'], false, 'adm.php?modulo=principal/estabelecimento/agenda&acao=C')): ?>
                                        <a class="btn btn-circle btn-icon-only btn-default purple" href="?modulo=principal/estabelecimento/agenda&acao=C&p=<?= setParam(['id' => $dado['estabelecimento_id']],false) ?>" data-toggle="tooltip" title="Agenda">
                                            <i class="icon-calendar"></i>
                                        </a>
                                    <? endif; ?>
                                </div>
                            </td>
                            <td><?= $dado['nome'] ?></td>
                            <td><?= $dado['valor_remuneradas'] ?></td>
                            <td><?= ($dado['qtd_salas'] ?: 0) ?></td>
                            <td><?= $dado['valor_medio_sala'] ?></td>
                            <td><?= ($dado['qtd_total_agendadas'] ?: 0) ?></td>
                            <td><?= ($dado['qtd_total'] ?: 0) ?></td>
                            <td><?= $dado['ticket_medio'] ?></td>

                            <td>
                                <span class="span-detalhe-agenda">
                                    <input type="hidden" id="estabelecimento_id" value='<?= $dado['estabelecimento_id'] ?>'>
                                    <i class="fa fa-question-circle" aria-hidden="true"></i>
                                    <?= ($dado['agenda_total'] ?: 0); ?>
                                </span>
                            </td>

                            <td><?= ($dado['agendas'][EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL]['qtd'] ?: 0) ?></td>
                        </tr>
                    <? endforeach; ?>
                    </tbody>
                </table>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    var jsonAgenda = <?= json_encode($arJsonAgenda); ?>;

    $(function () {
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

        $('#tabela-visao-geral').bootstrapTable({
            striped: true,
            pagination: true,
            pageSize: 20,
            search: true,
            paginationHAlign: 'left',
            onPostBody : function () {
                $('#tabela-visao-geral').find('tbody tr .span-detalhe-agenda').each(function(index, tr){
                    var id = $(tr).data('id');
                    if(id !== null){
                        $(this).mouseover(function(){
                            eventMouseOver(this);
                        }).mouseout(function(){
                            eventMouseout(this);
                        });
                    }
                });
            }
        });

        $('#tabela-visao-geral .order-fixed').parent().removeClass('sortable').removeClass('both').removeClass('asc').removeClass('desc');
    });


    function eventMouseOver(elemento){
        var elemento   = $(elemento);
        var json_itens = JSON.parse( jsonAgenda[$(elemento).find('#estabelecimento_id').val()]);

        var tooltip  = '<div class="tooltip-event visible-lg">';
        tooltip     += '<table style="width: 100%" id="tabela-div-tooltip" class="table table-striped table-hover">' +
                        '<thead>' +
                        '<tr>' +
                        '<th data-field="estado" >Situação</th>' +
                        '<th data-field="qtd" data-align="center">Quantidade</th>'+
                        '</tr>' +
                        '</thead>';
        tooltip     += '</table></div>';
        $("body").append(tooltip);

        $('#tabela-div-tooltip').bootstrapTable();
        $('#tabela-div-tooltip').bootstrapTable('load', json_itens);

        $(this).mouseover(function (e) {
            $(this).css('z-index', 10000);
            $('.tooltip-event').fadeIn('500').fadeTo('10', 1.9);
        }).mousemove(function (e) {
            var divTooltip = $('.tooltip-event');
            divTooltip.css('top', e.pageY - (divTooltip.height()/2));
            divTooltip.css('left', event.clientX - 370);
        });

    }

    function eventMouseout(elemento){
        $(this).css('z-index', 8);
        $('.tooltip-event').remove();
    }
</script>
