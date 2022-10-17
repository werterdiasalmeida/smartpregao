<?php
$p = getParam();
$defaultDate = "{$_REQUEST['ano']}-{$_REQUEST['mes']}-01";
$defaultEndDate = "{$_REQUEST['ano']}-{$_REQUEST['mes']}-" . date('t', strtotime($defaultDate));

$mProfissional = new Profissional();
$profissional = $mProfissional->getTodosDados($p['profissional_id']);
$mEspecialidadePro = new EspecialidadeProfissional();
$especialidade = array_shift(array_filter($mEspecialidadePro->listarPorProfissional($profissional['id'], false, $p['id']), function($especialidade){
    return $especialidade['id'] == $_REQUEST['especialidade_profissional_id'];
}));

$mAgenda = new EstabelecimentoProfissionalAgenda();
$jsonEventos = $mAgenda->gerarEventosPorMesCalendario($_REQUEST['especialidade_profissional_id'], $p['id'], $_REQUEST['mes'], $_REQUEST['ano'], $_REQUEST['dias'], true);
?>

<div class="row">
    <div class="col-sm-3 text-center">
        <div id="mini-calendar"></div>
    </div>

    <div class="col-sm-3 text-center">
        <?php montaImgFoto($profissional['foto_arquivo_id'], $profissional['sexo'], '150px', '', '200', '200', 'img-circle bg-white') ?>
    </div>

    <div class="col-sm-6 text-left">
        <div class="row">
            <div class="col-md-12">
                <div class="row">
                    <div class="col-md-8">
                        <div class="form-group">
                            <label>Nome</label>
                            <br>
                            <strong><?= $profissional['nome_completo'] ?></strong>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Consultas no mês</label>
                            <br>
                            <strong class="qtd-consultas-mes"></strong>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-md-8">
                        <div class="form-group">
                            <label class="obrigatorio">Especialidade</label>
                            <br>
                            <strong><?= $especialidade['especialidade'] ?></strong>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label class="obrigatorio">Duração da consulta</label>
                            <br>
                            <strong><?= $especialidade['duracao_consulta'] ?> <small>minutos</small></strong>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-12">
        <div id="visualizacao-agenda" class="has-toolbar"></div>
        <h4 class="text-center">Horários <span style="color: #E43A45;">em conflito</span> não serão incluídos na agenda.</h4>
    </div>
</div>

<style type="text/css">
    .tooltip-event {
        width: auto;
        min-width: 350px;
        height: auto;
        position: absolute;
        z-index: 10001;
        padding: 10px 10px 10px 10px;
        line-height: 150%;
        -webkit-border-radius: 10px !important;
        -moz-border-radius: 10px !important;
        border-radius: 10px !important;
        color: #FFF;
    }

    .datepicker.datepicker-inline {
        width: 100% !important;
    }

    .datepicker.datepicker-inline .table-condensed {
        margin: auto !important;
    }

    .day-content {
        position: relative;
        width: 20px;
        height: 20px;
        padding-top: 5px;
    }

    .buble-events {
        width: 15px;
        height: 15px;
        border-radius: 10px !important;
        background-color: #609bcd;
        color: #FFF;
        font-size: 9px;
        position: absolute;
        top: -5px;
        right: -5px;
        padding: 2px !important;
        text-align: center;
    }
</style>

<script type="text/javascript">
    var defaultDate = new Date('<?= $defaultDate ?> 00:00:00');
    $(function(){
        var container = $('.abrir-agenda-form');

        container.find('.btn-voltar')
            .removeClass('hidden')
            .unbind('click')
            .click(function(){
                App.blockUI({
                    message: 'Carregando...',
                    target: '.abrir-agenda-form'
                });

                $('#abrir-agenda').load(window.location.href, {
                    p: '<?= $_REQUEST['p'] ?>',
                    especialidade_profissional_id: <?= $_REQUEST['especialidade_profissional_id'] ?>,
                    mes: <?= $_REQUEST['mes'] ?>,
                    ano: <?= $_REQUEST['ano'] ?>,
                    dias: <?= json_encode($_REQUEST['dias']) ?>,
                    act: 'abrir-agenda'
                }, function () {
                    App.unblockUI('.abrir-agenda-form');
                });
            });

        container.find('.btn-salvar').html('<i class="fa fa-save"></i> Salvar').unbind('click').click(function(){
            // var eventos = container.find('#visualizacao-agenda').fullCalendar('getEventSources')[0].rawEventDefs;
            var eventos = container.find('#visualizacao-agenda').fullCalendar('getEventSources')[0].eventDefs.map(function(evento){
                return evento.miscProps;
            });

            App.blockUI({
                message: 'Carregando...',
                target: '.abrir-agenda-form'
            });

            $.ajax({
                type: "POST",
                url: window.location.href,
                dataType : 'json',
                data: {
                    p : '<?= setParam(array('id' => $p['id']), false) ?>',
                    act : 'salvar-agenda',
                    eventos : JSON.stringify(eventos.filter(function(evento){
                        return evento.disponivel === true;
                    }))
                },
                success: function (retorno) {
                    App.unblockUI('.abrir-agenda-form');
                    exibirMsg(retorno.msg);
                    $('#modal-abrir-agenda').modal('hide');
                },
                error: function (result) {
                    App.unblockUI('.abrir-agenda-form');
                    exibirAlert('Não foi possível incluir na agenda');
                }
            });

        });

        container.find('#visualizacao-agenda').fullCalendar({
            header: {
                left: '',
                center: '',
                right: 'title'
            },
            scrollTime: '07:00:00',
            defaultView: 'month',
            slotDuration: '00:05:00',
            displayEventTime: false,
            editable: false,
            droppable: false,
            eventLimit: true,
            events: <?= $jsonEventos ?>,
            eventRender: function(event, element) {
                element.prepend( "<span class='closeon' style='float: right;'><i class='fa fa-close' title='Remover'></i></span>" );
                element.find(".closeon").click(function() {
                    container.find('#visualizacao-agenda').fullCalendar('removeEvents', event._id);
                    calcularQtdConsultasMes();
                });
            },
            eventMouseover: function (row, event, view) {
                if(row.dsc){
                    tooltip = '<div class="tooltip-event" style="background-color: ' + row.color + ';">' + row.dsc + '</div>';

                    $("body").append(tooltip);
                    $(this).mouseover(function (e) {
                        $(this).css('z-index', 10000);
                        $('.tooltip-event').fadeIn('500').fadeTo('10', 1.9);
                    }).mousemove(function (e) {
                        $('.tooltip-event').css('top', e.pageY + 10).css('left', e.pageX - 200);
                    });
                }
            },
            eventMouseout: function (data, event, view) {
                $(this).css('z-index', 8);

                $('.tooltip-event').remove();
            },
            eventResizeStart: function () {
                tooltip.hide()
            },
            eventDragStart: function () {
                tooltip.hide()
            },
            viewDisplay: function () {
                tooltip.hide()
            },
        }).fullCalendar('gotoDate', defaultDate);

        calcularQtdConsultasMes();
        desenharMiniCalendario();
    });

    function calcularQtdConsultasMes(){
        var container = $('.abrir-agenda-form');
        var qtdConsultas = 0;
        var qtdConsultasConflito = 0;

        container.find('#visualizacao-agenda').fullCalendar('getEventSources')[0].eventDefs.map(function(evento){
            console.log(evento.miscProps.disponivel);
            if(evento.miscProps.disponivel === true){
                qtdConsultas++;
            }else{
                qtdConsultasConflito++;
            }
        });

        container.find('.qtd-consultas-mes').html(
            qtdConsultas + (qtdConsultas === 1 ? ' nova consulta' : ' novas consultas') +
            '<br />' +
            qtdConsultasConflito + (qtdConsultasConflito === 1 ? ' consulta ' : ' consultas ') +
            '<span style="color: #E43A45;">em conflito</span>'
        );
        desenharMiniCalendario();
    }

    function desenharMiniCalendario(){
        var container = $('.abrir-agenda-form');
        var eventos = [];

        $.each(container.find('#visualizacao-agenda').fullCalendar('getEventSources')[0].eventDefs, function(){
            if(this.miscProps.disponivel === true){
                var inicioAgrupador = moment(this.miscProps.inicio).format('YYYYMMDD');
                if(!eventos[inicioAgrupador]){
                    eventos[inicioAgrupador] = [];
                }

                eventos[inicioAgrupador].push(this.miscProps);
            }
        });

        container.find('#mini-calendar').datepicker('destroy').datepicker({
            language: 'pt-BR',
            todayHighlight: true,
            startDate : defaultDate,
            endDate: '+0d',
            maxViewMode: 0,
            beforeShowDay: function (date) {
                var data = moment(date).format('YYYYMMDD');
                var qtdEventos = eventos[data] ? eventos[data].length : 0;
                if(qtdEventos === 0){
                    return {
                        content: '<div class="day-content">' + moment(date).format('D') + '</div>'
                    }
                }

                var bubleEvents = '<div class="buble-events">' + qtdEventos + '</div>';
                return {
                    content: '<div class="day-content">' + moment(date).format('D') + bubleEvents + '</div>'
                }
            }
        });
    }
</script>
