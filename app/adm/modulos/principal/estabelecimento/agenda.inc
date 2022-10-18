<?php
ini_set("memory_limit", "1024M");

$mAgenda = new EstabelecimentoProfissionalAgenda();
$mEstabelecimentoProfissional = new EstabelecimentoEspecialidadeProfissional();
$mPessoaFisica = new PessoaFisica();
$p = getParam();

if (!isset($p['id'])) {
    addMsgInfo('Por favor, selecione um estabelecimento.');
    header('Location: ?modulo=principal/estabelecimento/lista&acao=C');
    die;
}

UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);
if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'incluirAgenda' :
            $params = array_merge($_POST, getParam());
            $params = utf8ArrayDecode($params);

            $estabelecimentoId = $params['id'];
            $params['id'] = isset($params['agenda_id']) ? $params['agenda_id'] : null;

            $mAgenda->salvarHorariosAgenda($params);
            $mAgenda->commit();

            $db->sucessomAjax($_REQUEST['modulo'], array(
                'id' => $estabelecimentoId
            ));
            break;
        case 'buscarDadosCalendario' :
            $dadosAgenda = $mAgenda->getDadosCalendario($p['id'], $_REQUEST['mes'], $_REQUEST['ano'], false);
            $db->sucessomAjax($_REQUEST['modulo'], $dadosAgenda);
            break;
        case 'filtrarPacientesCombo' :
            $busca = $_REQUEST['busca'];
            $mPessoa = new PessoaFisica();

            die($mPessoa->getListByCampoDeBusca($busca));
            break;
        case 'formulario' :
        case 'filtrarEspecialidades' :
        case 'verificarHorario' :
        case 'adicionarObservacao' :
        case 'adicionarObservacaoPessoa' :
        case 'alterarFoto':
        case 'excluirHorarioAgenda':
        case 'formularioGuia':
        case 'salvarGuia':
        case 'imprimirGuia':
        case 'buscarTuss':
            include_once 'agenda/agendaFormulario.inc';
            die;
        case 'solicitarCorrecao':
        case 'salvarSolicitacaoCorrecao':
        case 'cancelarSolicitacaoCorrecao':
            include_once 'agenda/solicitacaoCorrecao.inc';
            die;
            break;
        case 'formularioMarcarConsulta' :
        case 'agendaMarcarConsulta' :
        case 'agendaListaHorarios' :
        case 'selecionarPacienteConsulta' :
            include_once 'agenda/marcarConsultaFormulario.inc';
            die;
            break;
        case 'chamarSenha' :
        case 'excluirPainelSenha' :
        case 'criarPainelSenha' :
        case 'criarNumeroSenha' :
            include_once 'agenda/chamarSenha.inc';
            die;
            break;
        case 'formularioEncaixe' :
        case 'incluirEncaixe' :
            include_once 'agenda/encaixeFormulario.inc';
            die;
            break;
        case 'agendarConsulta' :
        case 'alterarDadosConsulta' :
        case 'buscarValoresProcedimento' :
        case 'carregarNumeroCartaoConvenio' :
            include_once 'agenda/pacienteConsultaFormulario.inc';
            die;
            break;
        case 'imprimirProtocolo' :
            include_once 'agenda/imprimirProtocolo.inc';
            die;
            break;
        case 'verificarCpf':
            $mPessoaFisica = new PessoaFisica();
            die(json_encode(array(
                'disponivel' => $mPessoaFisica->isCpfDisponivel($_POST['cpf'], $_POST['paciente_id'])
            )));
            break;
        case 'filtrarProcedimentos':
            if(empty($_REQUEST['profissional_id'])){
                die('-');
            }
            $mProcedimento = new ProfissionalEstabelecimentoProcedimento();
            $procedimentos = $mProcedimento->getComboProcedimentosPorProfissionalEstabelecimento($p['id'], $_REQUEST['profissional_id']);
            $db->monta_combom("profissional_estabelecimento_procedimento_id", $procedimentos, 'S', 'Selecione o procedimento', '', '', '', '', 'S');
            die;
            break;
    }
}

$filtroEspecialidades = null;
$filtroProfissionais = null;
if ($p['profissional_id']) {
    $mProfissional = new Profissional($p['profissional_id']);
    $mEspecialidadePro = new EspecialidadeProfissional();
    $especialidadesProfissional = $mEspecialidadePro->recuperarPorProfissional($p['profissional_id'], false, $p['id']);

    $filtroProfissionais = array($mProfissional->pessoafisica_id);
    $filtroEspecialidades = array_map(function ($especialidade) {
        return $especialidade['especialidade_id'];
    }, $especialidadesProfissional);
}

$arrSituacaoWf = $mAgenda->getEstadosWfParaCheckbox();

$arrSituacaoWfMarcadosPorPadrao = array(
    EstabelecimentoProfissionalAgenda::WF_ESTADO_DISPONIVEL,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_AGENDADA,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_CONFIRMADA,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_BLOQUEADA,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_EM_ATENDIMENTO,
    EstabelecimentoProfissionalAgenda::WF_ESTADO_AGUARDANDO_ATENDIMENTO,
);

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true);

$dadosAgenda = $mAgenda->getDadosCalendario($p['id'], date('m'), date('Y'), true);

include APPRAIZ . "includes/controleAcessoSituacaoContrato.inc";
?>
<div class="portlet light bordered" id="page-body">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-calendar font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Agenda dos Pregões
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-3" style="margin-bottom: 8px;">
                <div class="row text-center">

                   <div class="col-xs-12 col-sm-12 col-lg-12" style="margin-bottom: 8px;">
                        <div class="text-center">
                            <div id="mini-calendar"></div>
                        </div>
                    </div>

                    <button type="button" class="btn btn-voltar">
                        <i class="fa fa-arrow-left"></i> Voltar
                    </button>
                </div>
            </div>
            <div class="col-lg-9">
                <div class="portlet light calendar bordered" style="min-height: 500px; background-color: #FFF;">
                    <div id="agenda" class="has-toolbar"></div>
                </div>
            </div>
        </div>

        <br/>
        <div class="form-actions">
        </div>
    </div>
</div>

<form method="post" class="form-excluir">
    <input type="hidden" name="act" value="excluir"/>
    <input type="hidden" name="p"/>
</form>

<form method="post" class="form-imprimir-protocolo" target="_blank" >
    <input type="hidden" name="act" value="imprimirProtocolo" />
    <input type="hidden" name="p" />
</form>

<div class="modal fade in" id="modal-abrir-agenda" tabindex="-1" role="modal-abrir-agenda" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <form id="form-abrir-agenda" name="form-abrir-agenda" method="post" enctype="multipart/form-data">
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

<div class="modal fade in" id="modal-marcar-consulta" tabindex="-1" role="modal-marcar-consulta" aria-hidden="true">
    <div class="modal-dialog" style="width: 80%;">
        <div class="modal-content">
            <form id="form-marcar-consulta" method="post">
                <div id="form-marcar-consulta-content"></div>
            </form>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-solicitacao-correcao" tabindex="-1" role="modal-solicitacao-correcao" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <form id="form-solicitacao-correcao" method="post">
                <div id="form-solicitacao-correcao-content"></div>
            </form>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-encaixe" tabindex="-1" role="modal-encaixe" aria-hidden="true">
    <div class="modal-dialog" style="width: 80%;">
        <div class="modal-content">
            <form id="form-encaixe" method="post">
                <div id="form-encaixe-content"></div>
            </form>
        </div>
    </div>
</div>

<div class="btn-group btn-group-sm">    <script type="text/javascript">

        function wf_atualizarTela(mensagem, janela) {
            janela.close();
            atualizarAgendaCalendario(mensagem);
        }

        function wf_alterarEstado(aedid, docid, esdid, acao) {
            if (!confirm('Deseja realmente ' + acao + ' ?')) {
                return;
            }
            var url = '/geral/workflow/alterar_estado.php' +
                '?aedid=' + aedid +
                '&docid=' + docid +
                '&esdid=' + esdid +
                '&verificacao=a%3A1%3A%7Bs%3A2%3A%22id%22%3Bs%3A5%3A%2216381%22%3B%7D';
            var janela = window.open(
                url,
                'alterarEstado',
                'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
            );
            janela.focus();
        }

        function wf_exibirHistorico(docid) {
            var url = '/geral/workflow/historico.php' +
                '?modulo=principal/tramitacao' +
                '&acao=C' +
                '&docid=' + docid
                + '&inicial=Agenda+do+profissional+disponibilizada';
            window.open(
                url,
                'alterarEstado',
                'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
            );
        }

    </script>

    <style type="text/css">
        .datepicker.datepicker-inline {
            width: 100% !important;
        }

        .datepicker.datepicker-inline .table-condensed {
            margin: auto !important;
        }

        .tabela-estados-container .fixed-table-container, .tabela-estados-container #tabela-estados,
        .tabela-estados-container #tabela-estados tbody, .tabela-estados-container #tabela-estados tr,
        .tabela-estados-container #tabela-estados td {
            border: 0 !important;
        }

        .tabela-estados-container #tabela-estados .bs-checkbox {
            padding: 8px 0 8px 8px !important;
        }

        .tabela-estados-container #tabela-estados > thead {
            display: none !important;
        }

        .tabela-estados-container #tabela-estados > tbody > tr {
            float: right !important;
        }

        .no-y-border td, .no-y-border th {
            border-right: 0 !important;
            border-left: 0 !important;
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

        .fc-sat, .fc-sun {
            background-color: #ededed;
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
            color: #FFF;
        }

        .tooltip-event label{
            /*width: 100px;*/
            /*margin: 0;*/
        }

        .fc-list-item .icone-agenda {
            display: none;
        }

        .portlet.calendar.light .fc-button {
            top: -1px;
        }

        .portlet.calendar .fc-event .fc-title {
            float: none;
        }

        .fc-event{
            border: 1px solid #FFF !important;
        }

        .tabela-agenda .btn{
            margin: 0;
        }
    </style>
    <link href="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.css" rel="stylesheet" type="text/css"/>
    <script src="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/fullcalendar/lang/pt-br.js" type="text/javascript"></script>
    <script type="text/javascript">
        var atualizarCalendario = true;
        var dataSelecionada = null;
        var tooltip = null;
        var Eventos = {
            mesAnoCarregados: <?= $dadosAgenda['mes_ano'] ?>,
            eventos: <?= $dadosAgenda['eventos'] ?>,
            eventosAgrupados: <?= $dadosAgenda['eventos_agrupados'] ?>,
            isCarregado: function (mes, ano) {
                return this.mesAnoCarregados.includes(ano + '-' + mes);
            },
            getEventos: function () {
                return this.eventos;
            },
            setEventos: function (eventos) {
                return this.eventos = eventos;
            },
            getEventosAgrupados: function () {
                return this.eventosAgrupados;
            },
            setEventosAgrupados: function (eventosAgrupados) {
                return this.eventosAgrupados = eventosAgrupados;
            },
            setMesAnoCarregados(mesAno) {
                this.mesAnoCarregados = mesAno;
            },
            addMesAnoCarregados: function (mes, ano) {
                return this.mesAnoCarregados.push(ano + '-' + mes);
            },
            addEventos: function (eventos) {
                $('#agenda').fullCalendar('addEventSource', eventos);
            },
            addEventosAgrupados: function (eventos) {
                this.eventosAgrupados = Object.assign(this.eventosAgrupados, eventos);
                return this.eventosAgrupados;
            },
            resetCalendar() {
                Eventos.setEventos({});
                Eventos.setEventosAgrupados({});
                Eventos.setMesAnoCarregados([]);

                $('#agenda').fullCalendar('removeEvents');
                desenharMiniCalendario(null, false);
            }
        };

        $(function () {

            $('.btn-voltar').click(function () {
                window.history.back(-1);
            });

            $('#profissionais').selectize({
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
                options: <?= $mEstabelecimentoProfissional->getComboPessoasProfissionaisPorEstabelecimento($p['id'], true); ?>,
                items: <?= json_encode($filtroProfissionais) ?>,
                render: {
                    item: function (item, escape) {
                        var name = escape(item.descricao);
                        return '<div>' +
                            (name ? '<span class="name">' + name + '</span>' : '') +
                            '</div>';
                    },
                    option: function (item, escape) {
                        var name = escape(item.descricao);
                        var urlFoto = (item.sexo === 'F' ? '../imagens/woman.png' : '../imagens/man.png');
                        if (item.foto_arquivo_id) {
                            urlFoto = '../slideshow/slideshow/verimagem.php?arqid=' + item.foto_arquivo_id + '&newwidth=300';
                        }

                        return '<div>' +
                            '<img src="' + urlFoto + '" alt="" style="width: 30px; height: 30px; float: left; margin-right: 5px; border-radius: 100px !important;" />' +
                            '<span style="vertical-align: sub;">' + name + '</span>' +
                            '</div>';
                    }
                },
                onChange: function () {
                    filtrarEventos();
                }
            });

            $('#especialidades').selectize({
                persist: false,
                maxItems: null,
                placeholder: 'Todas',
                plugins: ['remove_button'],
                valueField: 'codigo',
                labelField: 'descricao',
                searchField: ['descricao'],
                sortField: [
                    {field: 'descricao', direction: 'asc'}
                ],
                options: <?= $mEstabelecimentoProfissional->getComboEspecialidadesPorEstabelecimento($p['id'], true); ?>,
                items: <?= json_encode($filtroEspecialidades) ?>,
                render: {
                    item: function (item, escape) {
                        var name = escape(item.descricao);
                        return '<div>' +
                            (name ? '<span class="name">' + name + '</span>' : '') +
                            '</div>';
                    },
                    option: function (item, escape) {
                        var name = escape(item.descricao);

                        return '<div>' +
                            '<span style="vertical-align: sub;">' + name + '</span>' +
                            '</div>';
                    }
                },
                onChange: function () {
                    filtrarEventos();
                }
            });


            $('#pacientes').selectize({
                valueField: 'id',
                placeholder: 'Todos',
                labelField: 'nome_completo',
                searchField: ['nome_completo'],
                plugins: ['remove_button'],
                options: [],
                render: {
                    option: function(item, escape) {
                        var name = escape(item.nome_completo);
                        var urlFoto = (item.sexo === 'F' ? '../imagens/woman.png' : '../imagens/man.png');
                        if (item.foto_arquivo_id) {
                            urlFoto = '../slideshow/slideshow/verimagem.php?arqid=' + item.foto_arquivo_id + '&newwidth=300';
                        }

                        return '<div>' +
                            '<img src="' + urlFoto + '" alt="" style="width: 30px; height: 30px; float: left; margin-right: 5px; border-radius: 100px !important;" />' +
                            '<span style="vertical-align: sub;">' + name + '</span>' +
                            '</div>';
                    }
                },
                load: function(query, callback) {
                    if (query.length < 4) return callback();

                    $.ajax({
                        url: window.location.href,
                        type: 'POST',
                        data : {
                            act : 'filtrarPacientesCombo',
                            busca : (query)
                        },
                        dataType : 'json',
                        error: function() {
                            callback();
                        },
                        success: function(res) {
                            callback(res.data);
                        }
                    });
                },
                onChange: function () {
                    filtrarEventos();
                }
            });

            isMarcarValorTodos();
            $("[name=situacoes]:not(#situacoes_)").change(function () {
                isMarcarValorTodos();
                filtrarEventos();
            });

            $("#situacoes_").change(function () {
                marcarTodasSituacoes();
                filtrarEventos();
            });

            setCalendar();

            $('#mini-calendar').datepicker().on('changeDate', function (e) {
                dataSelecionada = e.date;
                if (atualizarCalendario) {
                    var agenda = $('#agenda');
                    agenda.fullCalendar('gotoDate', new Date(Date.parse(e.date)));
                }
                dataSelecionada = null;
            }).on('changeMonth', function (e) {
                var date = e.date ? e.date : $('#mini-calendar').datepicker('getDate');
                buscarDadosCalendario(moment(date).format('MM'), moment(date).format('YYYY'))
            });

            $('#modal-abrir-agenda').on('hidden.bs.modal', function () {
                $('.timepicker-24').timepicker('remove');
                $(this).find('.modal-dialog').css({width: ''});
            });

            $('.modal').on('hidden.bs.modal', function () {
                $(this).find('.modal-body').html('');
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
            var estados = $("[name=situacoes]:checked:not(#situacoes_)").map(function () {
                return $(this).val();
            }).get();
            return estados;
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
                profissional_id : '<?= ($p['profissional_id'] ? $p['profissional_id'] : '') ?>'
            }, function () {
                App.unblockUI('#form-abrir-agenda-content');
            });

            $('#modal-abrir-agenda').modal({
                backdrop: 'static',
            }).modal('show');
        }

        function imprimirProtocolo(p) {
            $('form.form-imprimir-protocolo [name="p"]').val(p);
            if($('form.form-imprimir-protocolo [name="p"]').val() != '')
            {
                $('form.form-imprimir-protocolo').submit();
            }else{
                var erro = '<div class="alert alert-danger"><button class="close" data-close="alert"></button><span>Erro ao imprimir o protocolo! Tente novamente mais tarde.</span></div>';
                $('form.form-imprimir-protocolo').after(erro);
                $('html,body').animate({scrollTop: $("form.form-imprimir-protocolo").offset().top},'slow');
            }
        }

        function marcarConsulta() {
            var content = $('#form-marcar-consulta-content');
            content.html('<div style="height: 150px; width: 100%;"></div>');

            App.blockUI({
                message: 'Carregando...',
                target: '#form-marcar-consulta-content'
            });

            content.load(window.location.href, {
                act: 'formularioMarcarConsulta',
                pessoa_id : '<?= ($p['pessoa_id'] ? $p['pessoa_id'] : '') ?>'
            }, function () {
                App.unblockUI('#form-marcar-consulta-content');
            });

            $('#modal-marcar-consulta').modal({
                backdrop: 'static',
            }).modal('show');
        }

        function incluirEncaixe() {
            var content = $('#form-encaixe-content');
            content.html('<div style="height: 150px; width: 100%;"></div>');

            App.blockUI({
                message: 'Carregando...',
                target: '#form-encaixe-content'
            });

            content.load(window.location.href, {
                act: 'formularioEncaixe',
                profissional_id : '<?= ($p['profissional_id'] ? $p['profissional_id'] : '') ?>'
            }, function () {
                App.unblockUI('#form-encaixe-content');
            });

            $('#modal-encaixe').modal({
                backdrop: 'static',
            }).modal('show');
        }

        function filtrarEventos() {
            // Chama o eventRender
            $('#agenda').fullCalendar('rerenderEvents');

            // Filtra o mini calendário
            desenharMiniCalendario();
        }

        function situacaoWf(value, row) {
            return "<div style='width: 20px; height: 20px; float: left; margin-right: 5px; border-radius: 100px !important; background-color: " + row.cor + "'></div>" + value;
        }

        function profissionalFoto(value, row) {
            var foto = row.foto_arquivo_id ? '../slideshow/slideshow/verimagem.php?arqid=' + row.foto_arquivo_id + '&newwidth=300' : '/imagens/user.png';
            return "<img src='" + foto + "' style='width: 30px; height: 30px; float: left; margin-right: 5px; border-radius: 100px !important; ' title='Foto do profissional' /><span style='vertical-align: sub;'>" + value + "</span>";
        }

        function abrirFormularioSolicitacaoCorrecao(params) {
            var content = $('#form-solicitacao-correcao-content');

            App.blockUI({
                message: 'Carregando...',
                target: '#form-solicitacao-correcao-content'
            });

            content.load(window.location.href, params, function () {
                App.unblockUI('#form-solicitacao-correcao-content');
            });
        }

        function abrirFormularioPacienteConsulta(params) {
            var content = $('#form-marcar-consulta-content');

            App.blockUI({
                message: 'Carregando...',
                target: '#form-marcar-consulta-content'
            });

            content.load(window.location.href, params, function () {
                App.unblockUI('#form-marcar-consulta-content');
            });
        }

        function desenharMiniCalendario(currentdate, chamarChangeMonth) {
            var miniCalendar = $('#mini-calendar');
            currentdate = currentdate ? currentdate : miniCalendar.datepicker('getDate');

            miniCalendar.datepicker('destroy');
            miniCalendar.datepicker({
                language: 'pt-BR',
                todayHighlight: true,
                beforeShowDay: function (date) {
                    var data = moment(date).format('YYYYMMDD');
                    var eventosAgrupados = Eventos.getEventosAgrupados();

                    if(!eventosAgrupados[data]){
                        return {
                            content: '<div class="day-content">' + moment(date).format('D') + '</div>'
                        }
                    }

                    var qtdEventos = eventosAgrupados[data].filter(function (evento) {
                        return isEventoVisivel(evento);
                    }).length;

                    var bubleEvents = '';
                    if(qtdEventos){
                        bubleEvents = eventosAgrupados[data] && eventosAgrupados[data].length ? '<div class="buble-events">' + qtdEventos + '</div>' : '';
                    }

                    return {
                        content: '<div class="day-content">' + moment(date).format('D') + bubleEvents + '</div>'
                    }
                }
            });

            currentdate = dataSelecionada ? dataSelecionada : currentdate;
            miniCalendar.datepicker('setDate', new Date(currentdate));

            if(chamarChangeMonth !== false){
                miniCalendar.trigger('changeMonth');
            }
        }

        function isEventoVisivel(event) {
            var profissionais = $('#profissionais').val();
            var especialidades = $('#especialidades').val();
            var pacientes = $('#pacientes').val();
            var estados = getSituacoesSelecionadas();

            profissionais = profissionais ? profissionais : [];
            especialidades = especialidades ? especialidades : [];
            pacientes = pacientes ? pacientes : [];
            estados = estados ? estados : [];

            return !($.inArray(event.pes_id, profissionais) === -1 && profissionais.length !== 0)
                && !($.inArray(event.esd_id, estados) === -1)
                && !($.inArray(event.esp_id, especialidades) === -1 && especialidades.length !== 0)
                && !($.inArray(event.pac_id, pacientes) === -1 && pacientes.length !== 0);
        }

        function atualizarAgendaCalendario(){
            var dataCalendario = $('#mini-calendar').datepicker('getDate');
            buscarDadosCalendario(moment(dataCalendario).format('MM'), moment(dataCalendario).format('YYYY'), true);
        }

        function buscarDadosCalendario (mes, ano, forcarCarregamento) {
            var dataCalendario = new Date(ano + '-' + mes + '-' + '01 00:00:00');
            if(forcarCarregamento === true){
                Eventos.resetCalendar();
                dataCalendario = $('#mini-calendar').datepicker('getDate');
            }

            if (!Eventos.isCarregado(mes, ano)) {
                App.blockUI({
                    message: 'Carregando...',
                    target: '.portlet-body'
                });

                $.ajax({
                    type: "POST",
                    dataType: 'JSON',
                    url: window.location.href,
                    data: {
                        act: 'buscarDadosCalendario',
                        mes: mes,
                        ano: ano
                    },
                    success: function (retorno) {
                        var params = retorno.params;
                        Eventos.addEventos(params.eventos);
                        Eventos.addEventosAgrupados(params.eventos_agrupados);
                        Eventos.addMesAnoCarregados(mes, ano);

                        desenharMiniCalendario(dataCalendario);
                    },
                    complete: function () {
                        App.unblockUI('.portlet-body');
                    }
                });
            }
        }

        function setCalendar(){
            var FC = $.fullCalendar; // a reference to FullCalendar's root namespace
            var View = FC.View;      // the class that all views must inherit from
            var CustomView;          // our subclass

            CustomView = View.extend({ // make a subclass of View

                initialize: function() {
                    // called once when the view is instantiated, when the user switches to the view.
                    // initialize member variables or do other setup tasks.
                },

                render: function() {
                    // responsible for displaying the skeleton of the view within the already-defined
                    // this.el, a jQuery element.
                },

                setHeight: function(height, isAuto) {
                    // responsible for adjusting the pixel-height of the view. if isAuto is true, the
                    // view may be its natural height, and `height` becomes merely a suggestion.
                },

                renderEvents: function(events) {
                    var dataAgrupador = moment($('#agenda').fullCalendar('getDate')).format('YYYYMMDD');
                    var htmlView = "<table class='tabela-agenda table table-striped table-hover' data-toggle='table' data-pagination='true' data-page-size='20' data-search='false'>" +
                        "<thead>" +
                        "<th data-width='120px' data-align='center'>Status</th>" +
                        /*"<th>Tipo</th>" +*/
                        "<th data-align='center' data-width='75px'>Horário</th>" +
                        "<th>Paciente</th>" +
                        "<th>Telefone</th>" +
                        "<th>Sala</th>" +
                        "<th>Profissional</th>" +
                        /*"<th>Convênio</th>" +*/
                        "<th data-align='center'>Ações</th>" +
                        "</thead>";

                    htmlView += "<tbody>";

                    $(events).each(function(index, event){
                        if(event.data_agrupador !== dataAgrupador || !isEventoVisivel(event)){
                            return;
                        }

                        var btnMarcarConsulta = '';
                        if(event.marcar_consulta){
                            btnMarcarConsulta = '<div class="btn-group-sm">' +
                                '<button type="button" class="btn btn-sm green btn-block" onclick="marcarConsultaAgenda('+event.a_id+')">' +
                                '<i class="fa fa-check-circle"></i> Marcar consulta' +
                                '</button>' +
                                '</div>';
                        }

                        htmlView += "<tr data-id='"+index+"'>";
                        htmlView += "<td><span class='label label-sm' style='background-color: "+ event.color +"'>" + event.estado + "</span></td>";
                        /*htmlView += "<td>" + event.tipo_consulta + "</td>";*/
                        htmlView += "<td>" + event.hora_inicio + "</td>";

                        if(event.paciente){
                            htmlView += "<td><a title='Visualizar' href='javascript:abrirAgenda(\"" + event.p + "\");'>" + event.paciente  + "</a></td>";
                        }else{
                            htmlView += "<td>" + btnMarcarConsulta + "</td>";
                        }

                        htmlView += "<td>" + event.telefone_paciente + "</td>";
                        htmlView += "<td>" + event.sala + "</td>";
                        htmlView += "<td>" + event.nome_completo_medico + "</td>";
                        /*htmlView += "<td>" + event.convenio + "</td>";*/
                        htmlView += "<td>";
                        htmlView += '<div style="padding-right: 5px;" class="btn-group btn-group-sm">' +
                            "<button type='button' class='btn btn-sm blue' onclick='abrirAgenda(\""+event.p+"\")' title='Visualizar'>" +
                            "<i class='icon-magnifier'></i>" +
                            "</button>" +
                            "</div>";
                        htmlView += '<div style="padding-right: 5px;" class="btn-group btn-group-sm">' +
                            "<button type='button' class='btn btn-sm blue' onclick='imprimirProtocolo(\""+event.p+"\")' title='Imprimir Protocolo'>" +
                            "<i class='icon-printer'></i>" +
                            "</button>" +
                            "</div>";

                        event.next_esd_id.forEach(function(estadoWF){

                            switch (estadoWF.aeddscrealizar) {
                                case 'Cancelar':
                                    var cor = '#790026';
                                    var icon = 'fa fa-times-circle-o';
                                    break;
                                case 'Agendada':
                                    var cor = '#0D746B';
                                    var icon = 'fa fa-check-circle-o';
                                    break;
                                case 'Bloquear':
                                    var cor = '#94A0AF';
                                    var icon = 'fa fa-ban';
                                    break;
                                case 'Desbloquear':
                                    var cor = '#3AC386';
                                    var icon = 'fa fa-check-circle-o';
                                    break;
                                case 'Confirmar':
                                    var cor = '#0D746B';
                                    var icon = 'fa fa-check-circle-o';
                                    break;
                                case 'Registrar presença':
                                    var cor = '#662e91';
                                    var icon = 'fa fa-clock-o';
                                    break;
                                case 'Registrar falta':
                                    var cor = '#FB0019';
                                    var icon = 'fa fa-minus-circle';
                                    break;
                                default:
                                    var cor = '';
                                    var icon = '';
                            }

                            htmlView += '<div style="padding-right: 5px;" class="btn-group btn-group-sm">' +
                                "<button style='background-color: "+cor+"' type='button' class='btn' data-toggle='tooltip' onclick='wf_alterarEstado(\""+estadoWF.aedid+"\",\""+event.documento_id+"\",\""+estadoWF.esdid+"\",\""+estadoWF.aeddscrealizar+"\")' title=\""+estadoWF.aeddscrealizar+"\">" +
                                "<i style='color: #FFFF;' class='"+icon+"'></i>" +
                                "</button>" +
                                "</div>";
                        });

                        htmlView += "</td>";
                        htmlView += "</tr>";
                    });

                    htmlView += "</tbody>";
                    htmlView += "</table>";
                    $('.fc-view').html(htmlView);

                    var tabelaAgenda = $('.tabela-agenda');
                    tabelaAgenda.bootstrapTable({
                        onPostBody : function () {
                            tabelaAgenda.find('tbody tr').each(function(index, tr){
                                var id = $(tr).data('id');
                                if(id !== null){
                                    $(this).mouseover(function(){
                                        eventMouseOver(events[id]);
                                    }).mouseout(function(){
                                        eventMouseout();
                                    });
                                }
                            });
                        }
                    });
                },

                destroyEvents: function() {
                    // responsible for undoing everything in renderEvents
                    $('.fc-view').html('');
                },

                renderSelection: function(range) {
                    // accepts a {start,end} object made of Moments, and must render the selection
                },

                destroySelection: function() {
                    $('.fc-view').html('');
                    // responsible for undoing everything in renderSelection
                }

            });

            FC.views.custom = CustomView; // register our class with the view system

            $('#agenda').fullCalendar({
                header: {
                    left: 'title',
                    center: '',
                    right: 'prev,next,today,month,agendaWeek,agendaDay,customView'
                },
                buttonText: {
                    list: 'Lista'
                },
                views: {
                    // listWeek: {
                    //     buttonText: 'list week'
                    // },
                    customView: {
                        type: 'custom',
                        buttonText: 'Lista',
                        click:  $('#agenda').fullCalendar('changeView', 'CustomView')
                    }
                },
                scrollTime: '07:00:00',
                defaultView: 'customView',
                slotDuration: '00:05:00',
                eventLimit: true,
                editable: false,
                droppable: false,
                events: Eventos.getEventos(),
                eventClick: function (calEvent) {
                    abrirAgenda(calEvent.p);
                },
                dayClick: function (date) {
                    if (moment(date).format('YYYYMMDDHHmm') >= moment(new Date()).format('YYYYMMDDHHmm')) {
                        var data = moment(date).format('YYYY-MM-DD');
                        var hora = $('#agenda').fullCalendar('getView').name === 'month' ? '07:00' : moment(date).format('HH:mm');
                        abrirAgenda(null, data, hora);
                    } else {
                        exibirAviso('Só é possível abrir a agenda para datas futuras.');
                    }
                },
                viewRender: function (view, e) {
                    var currentdate = view.intervalEnd;
                    atualizarCalendario = false;

                    desenharMiniCalendario(currentdate);
                    atualizarCalendario = true;
                },
                eventRender: function (event, element) {
                    element.prepend( "<span class='icone-agenda' style='float: right; margin: 1px 1px 0 0;'><img src='" + event.icone + "' style='width: 15px; height: 15px;' /></span>" );
                    return isEventoVisivel(event);
                },
                eventAfterAllRender: function (view) {
                    if ('agendaDay' === view.name || 'agendaWeek' === view.name) {
                        if ($('.fc-time-grid-event').length > 0) {
                            // var renderedEvents = $('div.fc-event-container a');
                            // var firstEventOffsetTop = renderedEvents && renderedEvents.length > 0 ? renderedEvents[0].offsetTop : 0;
                            // $('div.fc-scroller').scrollTo(firstEventOffsetTop + 'px');
                        }
                    }
                },
                eventMouseover: function (row, event, view) {
                    eventMouseOver(row, event, view);
                },
                eventMouseout: function (data, event, view) {
                    eventMouseout(data, event, view);
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
            });
        }

        function eventMouseOver(row, event, view){
            row = row ? row : {};
            tooltip = '<div class="tooltip-event visible-lg" style="background-color: ' + row.color + ';">' + row.dsc + '</div>';

            $("body").append(tooltip);
            $(this).mouseover(function (e) {
                $(this).css('z-index', 10000);
                $('.tooltip-event').fadeIn('500').fadeTo('10', 1.9);
            }).mousemove(function (e) {
                var divContainer = $("#agenda").closest(".row");
                var conatinerPos = divContainer.position();
                var divTooltip = $('.tooltip-event');
                var tamanhoTela = $(window).width();

                divTooltip.css('top', e.pageY - (divTooltip.height()/2));

                if(tamanhoTela >= 1700) {
                    divTooltip.css('left', conatinerPos.left + 100);
                } else if(tamanhoTela >= 1500) {
                    divTooltip.css('left', conatinerPos.left + 40);
                } else if(tamanhoTela >= 1350) {
                    divTooltip.css('left', conatinerPos.left - 10);
                } else {
                    divTooltip.css('left', conatinerPos.left - 40);
                }
            });
        }

        function eventMouseout(data, event, view){
            $(this).css('z-index', 8);

            $('.tooltip-event').remove();
        }

        function marcarConsultaAgenda(agendaId) {
            $('#modal-abrir-agenda').modal('hide');

            abrirFormularioPacienteConsulta({
                agenda_id: agendaId,
                act: 'selecionarPacienteConsulta'
            });

            $('#modal-marcar-consulta').modal({
                backdrop: 'static',
            }).modal('show');
        }
    </script>