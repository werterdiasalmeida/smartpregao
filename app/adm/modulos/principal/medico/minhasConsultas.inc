<?php
$situacoesWf = array(
    EstabelecimentoProfissionalAgenda::WF_ESTADO_AGENDADA,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_CONFIRMADA,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA
);
if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'formulario' :
        case 'adicionarObservacao' :
        case 'adicionarObservacaoPessoa' :
        case 'formularioGuia':
        case 'salvarGuia':
        case 'imprimirGuia':
        case 'buscarTuss':
            $iniciarAtendimento = true;
            include_once APPRAIZ . 'adm/modulos/principal/estabelecimento/agenda/agendaFormulario.inc';
            die;
            break;
        case 'carregarAgenda' :
            $dataFiltro = explode("-", $_REQUEST['data']);
            $situacoes = is_array($_REQUEST['situacoes']) && count($_REQUEST['situacoes']) > 0 ? $_REQUEST['situacoes'] : $situacoesWf;
            $estabelecimentoId = $_REQUEST['estabelecimento_id'];
            $paciente = utf8_decode($_REQUEST['paciente']);
            $pesquisaSimples = $_REQUEST['pesquisa_simples'];

            $mAgenda = new EstabelecimentoProfissionalAgenda();

            $ano = $dataFiltro[0];
            $mes = $dataFiltro[1];

            $arWherePadraoAgenda = $mAgenda->getWhereMesAnoAgendaByEstabelecimento($estabelecimentoId, $mes, $ano);

            $usucpfFormatado = formatar_cpf($_SESSION['usucpf']);
            $arWherePadraoAgenda[] = (is_array($situacoes) && count($situacoes) > 0 ? "doc.esdid IN (" . implode(', ', $situacoes) . ")" : 'TRUE');
            $arWherePadraoAgenda[] = "p.cpf = '{$usucpfFormatado}'";

            if($pesquisaSimples){
                $arWherePadraoAgenda[] = "(
                pac.nome_completo ILIKE '%{$pesquisaSimples}%' 
                OR 
                tp.descricao ILIKE '%{$pesquisaSimples}%' 
                OR 
                e.descricao ILIKE '%{$pesquisaSimples}%' 
                OR 
                est.esddsc ILIKE '%{$pesquisaSimples}%' 
                )";
            }else{
                $arWherePadraoAgenda[] = ($paciente ? "pac.nome_completo ILIKE '%{$paciente}%'" : "TRUE");
            }
            $eventosAgrupados = $mAgenda->getListcalendarioAgrupadoPorData($arWherePadraoAgenda, true);
            die($eventosAgrupados);
            break;
        case 'chamarPacientePainel':
            $parametros  = getParam();
            $mAgenda     = new EstabelecimentoProfissionalAgenda();
            $dadosAgenda = $mAgenda->getDadosPainelSenha($parametros['agenda_id']);

            $mPainelSenhaRegistro = new PainelSenhaRegistro();
            $mPainelSenhaRegistro->chamarSenha(['painel_senha_id' => $_REQUEST['painelId'], 'numero' => $dadosAgenda['nome_completo'] , 'guiche' => $dadosAgenda['sala']]);
            $mPainelSenhaRegistro->commit();
            $db->sucessomAjax($_REQUEST['modulo'], []);
            break;
    }
}

$usucpfFormatado = formatar_cpf($_SESSION['usucpf']);
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mProfissional = new Profissional();
$estabelecimentos = $mProfissional->listaEstabelecimentosPorCpfProfissional($usucpfFormatado);

$arrWhereEstadosWf = array("esd.esdid IN (" . implode(', ', $situacoesWf) . ")");
$arrSituacaoWf = $mAgenda->getEstadosWfParaCheckbox($arrWhereEstadosWf);

Estabelecimento::setEstabelecimentoDoMedicoNaSession();

// Recuperando o painel de senhas atual de atendimento (se houver)
$mPainelSenha      = new PainelSenha();
$painelAtendimento = $mPainelSenha->getByEstabelecimento($_SESSION['estabelecimento_selecionado'],'A');

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalhoDoMedico();
EstabelecimentoProfissionalAgenda::montaAvisoAtendimento();

monta_titulom('Minhas consultas');
$db->cria_abam(ABA_MEDICO, 'adm.php?modulo=principal/medico/minhasConsultas&acao=C', array(), array(), false);
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-calendar font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                    Consultas
                </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row align-items-center">
            <div class="col-xs-12 col-lg-3">
            </div>
            <div class="col-xs-12 col-sm-12 col-md-12 col-lg-9">
                <form class="form-pesquisar" method="post">
                    <div class="form-body">
                        <div class="row pesquisa-simples">
                            <div class="col-md-12">
                                <div class="input-group">
                                    <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquisar nas consultas'); ?>
                                    <span class="input-group-btn">
                                            <button class="btn blue" type="submit" data-toggle="tooltip"
                                                    title="Pesquisar">
                                                <i class="fa fa-search"></i>
                                            </button>
                                        </span>
                                    <span class="input-group-btn" data-toggle="tooltip" title="Pesquisa avançada">
                                            <button class="btn green-jungle btn-pesquisa-avancada" type="button">
                                                <i class="fa fa-search-plus"></i>
                                            </button>
                                        </span>
                                </div>
                            </div>
                        </div>

                        <div class="row pesquisa-avancada hidden">
                            <div class="col-sm-12">
                                <div class="form-group">
                                    <label class="control-label">Paciente</label>
                                    <?= campo_textom('paciente', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', '', '', null, null, ''); ?>
                                </div>
                            </div>
                        </div>

                        <div class="row pesquisa-avancada hidden">
                            <div class="col-sm-12">
                                <div class="form-group">
                                    <label class="control-label">Situações</label>
                                    <div class="input-group">
                                        <div class="col-sm-12 container-combo-situacao-prossional-agenda">
                                            <?= $db->monta_checkboxm('situacoes', $arrSituacaoWf, 'S', '', $situacoesWf); ?>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="row pesquisa-avancada hidden">
                            <div class="col-sm-12">
                                <div class="pesquisa-avancada">
                                    <button type="submit" class="btn blue">
                                        <i class="fa fa-search"></i> Pesquisar
                                    </button>
                                    <button type="button" class="btn green-jungle btn-pesquisa-simples">
                                        <i class="fa fa-search-minus"></i> Pesquisa simples
                                    </button>
                                </div>
                            </div>
                        </div>

                    </div>
                </form>
            </div>
        </div>

        <div class="row">
            <div class="col-sm-12">
                <div class="lista-agenda">
                    <div class="row">
                        <div class="col-lg-3" style="margin-bottom: 8px;">
                            <div class="row text-center">
                                <div class="col-xs-12 col-sm-12 col-lg-12" style="margin-bottom: 8px;">
                                    <div class="text-center">
                                        <div id="mini-calendar"></div>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="col-lg-9">
                            <div class="portlet light calendar bordered" style="background-color: #FFF;">
                                <div class="row">
                                    <div class="col-md-12">
                                        <div id="data-tabela" style="margin: 5px 0; font-size: 17px;"></div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <table id="tabela-consultas" class="table table-striped" data-unique-id="id"
                                               data-pagination="true">
                                            <thead>
                                            <tr>
                                                <th data-field="hora" data-align="center">Hora</th>
                                                <th data-field="title" data-formatter="formatAcaoDescricao">Paciente</th>
                                                <th data-field="tipo_consulta">Tipo de serviço</th>
                                                <th data-field="estado">Situação</th>
                                                <th data-formatter="formatAcaoConsulta" data-align="center">Ação</th>
                                            </tr>
                                            </thead>
                                        </table>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-abrir-agenda" tabindex="-1" role="modal-abrir-agenda" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <form id="form-abrir-agenda" method="post">
                <div id="form-abrir-agenda-content"></div>
            </form>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-guia" tabindex="-1" role="modal-guia" aria-hidden="true">
    <div class="modal-dialog" style="width: 90%;">
        <div class="modal-content">
            <form name="formulario-guia" class="guia-form" method="POST">
                <p style="height: 500px;">Carregando...</p>
            </form>
        </div>
    </div>
</div>

<style type="text/css">
    .datepicker.datepicker-inline {
        width: 100% !important;
    }

    .datepicker.datepicker-inline .table-condensed {
        margin: auto !important;
    }

    .day-content {
        position: relative;
        width: 30px;
        height: 30px;
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

<link href="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.css" rel="stylesheet" type="text/css"/>
<script src="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/fullcalendar/lang/pt-br.js" type="text/javascript"></script>
<script type="text/javascript">
    var req = null;
    var Eventos = {
        eventosAgrupados: [],
        getEventosAgrupados: function () {
            return this.eventosAgrupados;
        },
        setEventosAgrupados: function (eventosAgrupados) {
            return this.eventosAgrupados = eventosAgrupados;
        }
    };

    $(function () {
        $('#modal-abrir-agenda').on('hidden.bs.modal', function () {
            $('.timepicker-24').timepicker('remove');
            $(this).find('.modal-dialog').css({width: ''});
        });


        $('.modal').on('hidden.bs.modal', function () {
            $(this).find('.modal-body').html('');
        });

        $('.btn-pesquisar').click(function () {
            filtrarEventos();
        });

        $('[name=estabelecimento_id]').change(function () {
            $('[name=form-estabelecimento]').submit();
        });

        $('[name=paciente]').keydown(function (e) {
            if (e.which == 13) {
                // filtrarEventos();
            }
        }).change(function () {
            // filtrarEventos();
        });

        isMarcarValorTodos();
        $("[name=situacoes]:not(#situacoes_)").change(function () {
            isMarcarValorTodos();
        });

        $("#situacoes_").change(function () {
            marcarTodasSituacoes();
        });

        $('.form-pesquisar').validateForm({
            submitHandler: function (e) {
                filtrarEventos();
            }
        });

        $('#tabela-consultas').bootstrapTable();
        desenharMiniCalendario(new Date());
        filtrarEventos();

        var miniCalendar = $('#mini-calendar');

        miniCalendar.datepicker().on('changeDate', function (e) {
            var data = moment(e.date).format('YYYYMMDD');
            atualizarDadosTabela(data);
        }).on('changeMonth', function (e) {
            var dataAtual = new Date();
            var date = new Date(e.date);
            date.setDate(1);

            if(dataAtual.getMonth() === date.getMonth()) {
                date = dataAtual;
            }
            miniCalendar.datepicker('setDate', date);

            filtrarEventos();
        });
    });

    function isMarcarValorTodos() {
        var isTodasOpcoesMarcadas =
            $("[name=situacoes]:not(#situacoes_)").length === $("[name=situacoes]:checked:not(#situacoes_)").length;

        if(isTodasOpcoesMarcadas){
            $("#situacoes_").prop('checked', true);
        } else {
            $("#situacoes_").prop('checked', false);
        }
    }

    function marcarTodasSituacoes() {
        var flgTodasSituacoes = $("#situacoes_").prop('checked');

        if(flgTodasSituacoes) {
            $("[name=situacoes]").prop('checked', true);
        } else {
            $("[name=situacoes]").prop('checked', false);
        }
    }

    function getSituacoesSelecionadas() {
        return $("[name=situacoes]:checked:not(#situacoes_)").map(function () {
            return $(this).val();
        }).get();
    }

    function filtrarEventos() {
        if (req === null) {
            App.blockUI({
                message: 'Carregando...',
                target: '.lista-agenda'
            });

            var params = {
                act: 'carregarAgenda',
                estabelecimento_id : $('[name=estabelecimento_id]').val(),
                data : moment($('#mini-calendar').datepicker('getDate')).format('YYYY-MM-DD')
            };

            if($('.form-pesquisar .pesquisa-simples:visible').length > 0){
                params.pesquisa_simples = $('[name=pesquisa_simples]').val();
            }else{
                params.situacoes = getSituacoesSelecionadas();
                params.paciente = $('[name=paciente]').val();
            }

            $.ajax({
                type: "POST",
                dataType: 'JSON',
                url: window.location.href,
                data: params,
                success: function (retorno) {
                    Eventos.setEventosAgrupados(retorno);
                    desenharMiniCalendario();
                    atualizarDadosTabela();
                },
                complete: function () {
                    App.unblockUI('.lista-agenda');
                }
            });
        }
    }

    function desenharMiniCalendario(currentdate) {
        var miniCalendar = $('#mini-calendar');
        currentdate = currentdate ? currentdate : miniCalendar.datepicker('getDate');

        miniCalendar.datepicker('destroy');
        miniCalendar.datepicker({
            language: 'pt-BR',
            todayHighlight: true,
            beforeShowDay: function (date) {
                var data = moment(date).format('YYYYMMDD');
                var eventosAgrupados = Eventos.getEventosAgrupados();

                if (!eventosAgrupados[data]) {
                    return {
                        content: '<div class="day-content">' + moment(date).format('D') + '</div>'
                    }
                }

                var qtdEventos = eventosAgrupados[data].length;

                var bubleEvents = '';
                if (qtdEventos) {
                    bubleEvents = eventosAgrupados[data] && eventosAgrupados[data].length ? '<div class="buble-events">' + qtdEventos + '</div>' : '';
                }

                return {
                    content: '<div class="day-content">' + moment(date).format('D') + bubleEvents + '</div>'
                }
            }
        });

        miniCalendar.datepicker('setDate', new Date(currentdate));
    }

    function abrirAgenda(p, data, hora) {
        var content = $('#form-abrir-agenda-content');
        content.html('<div style="height: 150px; width: 100%;"></div>');
        p = p ? p : '<?= setParam(getParam(), false) ?>';
        data = data ? data : null;
        hora = hora ? hora : hora;

        App.blockUI({
            message: 'Carregando...',
            target: '#form-abrir-agenda-content'
        });

        content.load(window.location.href, {
            act: 'formulario',
            p: p,
            data_agenda: data,
            hora_inicio: hora,
            profissional_id: '<?= ($p['profissional_id'] ? $p['profissional_id'] : '') ?>'
        }, function () {
            App.unblockUI('#form-abrir-agenda-content');
        });

        $('#modal-abrir-agenda').modal({
            backdrop: 'static',
        }).modal('show');
    }

    function chamarPainel(painel, p) {
      Swal.fire({
        title: 'Deseja chamar o paciente no painel senhas?',
        icon: 'question',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Sim',
        cancelButtonText: 'Não'
      }).then((result) => {
        if(result.isConfirmed) {
          $.ajax({
            type: "POST",
            dataType: 'JSON',
            url: window.location.href,
            data: {act: 'chamarPacientePainel', 'painelId' : painel, 'p': p},
            success: function (retorno) {
              if(retorno.codigo === 1){
                exibirSucesso(retorno.msg ? retorno.msg : 'Operação realizada com sucesso!');
              }else{
                this.error(retorno);
              }
            }
          });
        }
      });
    }

    function formatAcaoDescricao(value, row, index) {
        return '<a href="javascript:;" onclick="abrirAgenda(\'' + row.p + '\')">' +
            '<span style="display: inline-block; width: 15px; height: 15px; border-radius: 5px; margin-right: 7px; vertical-align: bottom; background-color: ' +
            row.color +
            '"></span>' + value +
            '</a>';
    }

    function formatAcaoConsulta(value, row, index) {
        var btnSenha = "";
        if (row.esd_id == '<?= EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO ?>' && '<?= $painelAtendimento['id'] ?>' != '') {
          btnSenha = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:;" onclick="chamarPainel(\'<?= $painelAtendimento['id'] ?>\', \'' + row.p + '\')" title="Chamar no painel" style="margin: 2px;">'
              + '<i class="fa fa-ticket"></i>'
              + '</a>';
        }

        var btnVisualizar = '<a class="btn btn-circle btn-icon-only btn-default green" href="javascript:;" onclick="abrirAgenda(\'' + row.p + '\')" title="Visualizar" style="margin: 2px;">'
            + '<i class="icon-magnifier"></i>'
            + '</a>';

        var btnProntuario = '<a class="btn btn-circle btn-icon-only btn-default blue" target="_blank" href="?modulo=principal/medico/atendimento&acao=C&p=' + row.p + '" title="Prontuário" style="margin: 2px;">'
            + '<i class="icon-notebook"></i>'
            + '</a>';

        return btnSenha + btnProntuario + btnVisualizar
    }

    function atualizarDadosTabela() {
        var miniCalendar = $('#mini-calendar');
        var dataAgrupada =  new Date(miniCalendar.datepicker('getDate'));
        var dataAgrupadaFmt =  moment(dataAgrupada).format("YYYYMMDD");
        var tabelaConsultas = $('#tabela-consultas');
        var eventosAgrupados = Eventos.getEventosAgrupados();
        var eventos = eventosAgrupados[dataAgrupadaFmt] ? eventosAgrupados[dataAgrupadaFmt] : [];
        tabelaConsultas.bootstrapTable('load', eventos);
        tabelaConsultas.bootstrapTable('selectPage', 1);

        var divDataTabela = $('#data-tabela');
        var ano = dataAgrupada.getFullYear();
        var mes = dataAgrupada.getMonth() + 1;
        var dia = dataAgrupada.getDate();

        var mesesExtenso = {
            '1': 'janeiro',
            '2': 'fevereiro',
            '3': 'março',
            '4': 'abril',
            '5': 'maio',
            '6': 'junho',
            '7': 'julho',
            '8': 'agosto',
            '9': 'setembro',
            '10': 'outubro',
            '11': 'novembro',
            '12': 'dezembro'
        };

        divDataTabela.html(dia + ' de ' + mesesExtenso[mes] + ' de ' + ano);
    }
</script>