<?php
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mProfissional = new Profissional();
$mPessoa = new PessoaFisica();
$mProntuario = new Prontuario();
$mTipoConsulta = new TipoConsulta();
$mConvenio = new ConvenioEstabelecimento();
$mValorProcedimento = new ValorProcedimento();
$mProcedimento = new Procedimento();
$mValorPago = new ValorPago();
$mValorAPagar = new ValorAPagar();
$mArquivo = new Arquivo();
$fileSimec = new FilesSimec();
$mSolicitacaoCorrecaoAgenda = new SolicitacaoCorrecaoAgenda();

$p = getParam();

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'filtrar-extrato' :
            $html = carregarExtrato($_REQUEST['estabelecimento_id'], $_REQUEST['tipo'], formata_data_sql($_REQUEST['inicio']), formata_data_sql($_REQUEST['fim']));
            $db->sucessomAjax("", array('html' => $html));
            break;
        case 'imprimir-extrato' :
            include 'meuExtratoImprimir.inc';
            die;
            break;
        case 'downloadAnexo' :
            $fileSimec->getDownloadArquivo($_REQUEST['arquivo_id']);
            die;
            break;
        case 'exibeInformacoesAgenda':
            $dados = $mAgenda->carregarPorId($p['agenda_id'])->getDados();
            $htmlPaciente = $mPessoa->getDivInfoPaciente($dados['pessoa_paciente_id']);
            $dadosTipoConsulta = $mTipoConsulta->carregarPorId($dados['tipo_consulta_id'])->getDados();
            $dadosConvenio = $mConvenio->carregarPorId($dados['convenio_id'])->getDados();
            $dadosValorProcedimento = $mValorProcedimento->carregarPorId($dados['valor_procedimento_id'])->getDados();
            $dadosProcedimento = $mProcedimento->carregarPorId($dadosValorProcedimento['procedimento_id'])->getDados();

            $dadosValorAPagar = $mValorAPagar->carregarPorId($p['valor_a_pagar_id'])->getDados();
            $descricaoSolicitacaoAjuste = "";
            if($dadosValorAPagar['solicitacao_correcao_agenda_id']){
                $dadoSSolicitacaoCorrecaoAgenda = $mSolicitacaoCorrecaoAgenda->carregarPorId($dadosValorAPagar['solicitacao_correcao_agenda_id'])->getDados();
                $descricaoSolicitacaoAjuste = $dadoSSolicitacaoCorrecaoAgenda['descricao_solicitacao'];
            }

            $dataInicio = new DateTime($dados['inicio']);
            $dataTermino = new DateTime($dados['fim']);

            $arRetorno = array(
                'data' => $dataInicio->format('d/m/Y'),
                'inicio' => $dataInicio->format('H:i'),
                'fim' => $dataTermino->format('H:i'),
                'tipoConsulta' => $dadosTipoConsulta['descricao'],
                'procedimento' => $dadosProcedimento['descricao'],
                'convenio' => $dadosConvenio['nome'],
                'htmlPaciente' => $htmlPaciente,
                'solicitacaoAjuste' => $descricaoSolicitacaoAjuste,
            );

            $db->sucessomAjax("", $arRetorno);
            break;
        case 'exibeInformacoesPagamento':
            $params = array_merge($_POST, getParam());

            $valorPago = $mValorPago->carregarPorId($params['valor_pago_id'])->getDados();
            $arquivo = $mArquivo->carregarPorId($valorPago['arquivo_comprovante_id'])->getDados();
            $tamanhoArquivo = formatarTamanhoArquivo($arquivo['arqtamanho']);
            list($tamanho, $escala) = explode(' ', $tamanhoArquivo);

            $valorTotal = 0;
            $registros = $mValorAPagar->getByValorPago($params['valor_pago_id']);
            foreach ($registros AS $chave => $registro) {
                $valorTotal += $registro['valorpagamento'];
                $registros[$chave]['data'] = (new DateTime($registro['data_operacao']))->format('d/m/Y  H:i:s');
                $registros[$chave]['valorprocedimento'] = formata_valor($registro['valorprocedimento']);
                $registros[$chave]['percentualrepasse'] = formata_valor($registro['percentualrepasse']);
                $registros[$chave]['valorpagamento'] = formata_valor($registro['valorpagamento']);
            }

            $arRetorno = array(
                'valorTotal' => formata_valor($valorTotal),
                'registros' => $registros,
                'arquivo' => array(
                    'arquivo' => $arquivo,
                    'tamanho' => $tamanho,
                    'escala' => $escala,
                    'isImagem' => isImagem($arquivo['arqextensao']),
                )
            );

            $db->sucessomAjax("", $arRetorno);
            break;
    }
}


if (isset($_SESSION['estabelecimento_selecionado']) && !empty($_SESSION['estabelecimento_selecionado'])) {
    $estabelecimento_id = $_SESSION['estabelecimento_selecionado'];
}

include APPRAIZ . "includes/cabecalhom.inc";
EstabelecimentoProfissionalAgenda::montaAvisoAtendimento();
monta_titulom('Meu extratro');
$db->cria_abam(ABA_MEDICO, $url, array(), array(), false);
?>

    <div class="portlet light bordered">
        <div class="portlet-title">
            <div class="caption font-blue-sharp">
                <i class="icon-bar-chart font-blue-sharp"></i>
                <span class="caption-subject bold uppercase">
                Extrato Completo
            </span>
            </div>
        </div>
        <div class="portlet-body form">
            <div class="row">
                <div class="col-sm-12">
                    <div class="lista-extrato">
                        <?= carregarExtrato($estabelecimento_id) ?>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="modal fade in" id="modal-dados-agenda" tabindex="-1" role="modal-dados-agenda"
         aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Resumo do atendimento</h4>
                </div>
                <div class="modal-body">
                    <div class="row">
                        <div class='resumo-paciente col-sm-4'>

                        </div>
                        <div class='resumo-agenda col-sm-8'>
                            <div><label>Data</label><br/><span class="data"></span></div>
                            <div><label>Hora</label><br/><span class="hora"></span></div>
                            <div><label>Procedimento</label><br/><span class="procedimento"></span></div>
                            <div><label>Convênio</label><br/><span class="convenio"></span></div>
                            <div><label>Tipo de consulta</label><br/><span class="tipo_consulta"></span></div>
                            <div class="descricao_solicitacao_correcao"><label>Descrição da solicitação de correção</label><br /><p style="text-align: justify; padding-right: 10px; margin-top: 0px;  margin-bottom: 0px;"></p></div>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                </div>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>

<? require_once APPRAIZ . 'adm/modulos/principal/estabelecimento/financeiro/modalPagamento.inc'; ?>

    <style type="text/css">
        .data-dropdown {
            width: 100px;
            display: inline;
        }

        .valor-positivo {
            color: #0a6aa1;
        }

        .valor-negativo {
            color: #FF0000;
        }

        .saldo-anterior {
            font-weight: normal !important;
        }

        .resumo-agenda {
            text-align: left;
            padding: 5px;
        }

        .resumo-agenda.resumo-agenda-tooltip > div {
            line-height: 60%;
        }

        .resumo-agenda > div:not(:last-child) {
            margin-bottom: 10px;
        }

        .resumo-agenda label {
            font-weight: bold;
        }
    </style>
    <script type="text/javascript">
        function imprimirExtrato() {
            var estabelecimento_id = $('[name=estabelecimento_id]').val();
            var tipo = $('[name=tipoPeriodo]').val();
            var inicio = $('[name=inicio]').val();
            var fim = $('[name=fim]').val();

            windowOpen(window.location.href
                + '&act=imprimir-extrato' + '&tipo=' + tipo + '&estabelecimento_id=' + estabelecimento_id
                + '&inicio=' + inicio + '&fim=' + fim)
        }

        function filtrarExtrato(tipo) {


            //sucesso
            App.blockUI({message: 'Carregando...', target: '.page-content'});

            $.ajax({
                type: "POST",
                url: window.location.href,
                dataType: 'json',
                data: {
                    act: 'filtrar-extrato',
                    estabelecimento_id: $('[name=estabelecimento_id]').val(),
                    tipo: tipo || $('[name=tipoPeriodo]').val(),
                    inicio: $('[name=inicio]').val(),
                    fim: $('[name=fim]').val()
                },
                success: function (retorno) {
                    $('.lista-extrato').html(retorno.params.html);

                    App.unblockUI('.page-content');
                },
                error: function (result) {
                    App.unblockUI('.page-content');
                    exibirAlert('Não foi possível realizar a operação');
                }
            });
        }

        function exibeInfoAgenda(rash) {
            $.ajax({
                type: "POST",
                url: window.location.href + rash,
                dataType: 'json',
                data: {
                    act: 'exibeInformacoesAgenda'
                },
                success: function (retorno) {
                    var modal = $('#modal-dados-agenda');
                    $(modal).find('.resumo-paciente').html(retorno.params.htmlPaciente);
                    $(modal).find('.data').html(retorno.params.data);
                    $(modal).find('.hora').html(retorno.params.inicio + ' - ' + retorno.params.fim);
                    $(modal).find('.procedimento').html(retorno.params.procedimento);
                    $(modal).find('.convenio').html(retorno.params.convenio);
                    $(modal).find('.tipo_consulta').html(retorno.params.tipoConsulta);

                    if(retorno.params.solicitacaoAjuste && retorno.params.solicitacaoAjuste != ''){
                        $(modal).find('.descricao_solicitacao_correcao').show();
                        $(modal).find('.descricao_solicitacao_correcao p').html(retorno.params.solicitacaoAjuste);
                    }else{
                        $(modal).find('.descricao_solicitacao_correcao').hide();
                        $(modal).find('.descricao_solicitacao_correcao p').html('');
                    }


                    $('#modal-dados-agenda').modal('toggle');
                    App.unblockUI('.page-content');
                },
                error: function (result) {
                    App.unblockUI('.page-content');
                    exibirAlert('Não foi possível realizar a operação');
                }
            });
        }
    </script>

<?php
function carregarExtrato($estabelecimentoId, $tipo = 'U', $data_inicio = null, $data_termino = null)
{
    global $db;
    $usucpfFormatado = formatar_cpf($_SESSION['usucpf']);

    $mProfissional = new Profissional();
    $mValorAPargar = new ValorAPagar();

    //Define o tipo
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
    }

    $arWhere = array(
        'estabelecimento_id' => $estabelecimentoId,
        'inicio' => $dataInicio->format('Y-m-d'),
        'fim' => $dataFim->format('Y-m-d'),
    );

    $saldoAnterior = $mValorAPargar->getSaldoAnteriorProfissionalByWhere($usucpfFormatado, $estabelecimentoId, $dataInicio->format('Y-m-d'));
    $saldoAnteriorFormatado = formata_valor($saldoAnterior) . ' <i class="fa ' . ($saldoAnterior >= 0 ? 'fa-plus-circle' : 'fa-minus-circle') . '" aria-hidden="true"></i>';
    $classSaldoAnterior = $saldoAnterior >= 0 ? 'valor-positivo' : 'valor-negativo';

    $data = $mValorAPargar->getExtratoCompletoProfissionalByWhere($usucpfFormatado, $arWhere);

    //Define os itens da tabela
    $tabela = "";
    $saldo = $saldoAnterior;
    $valorAReceber = 0;
    $valorRecebido = 0;
    foreach ($data AS $indice => $registro) {
        $dataRegistro = (new DateTime($registro['data_operacao']))->format('d/m/Y  H:i:s');
        $valor = formata_valor($registro['valor']);

        if ($registro['tipo'] == 'pagar') {
            $valorAReceber += $registro['valor'];
            $saldo += $registro['valor'];

            if($registro['valor'] >= 0){
                $iconeValor = 'fa-plus-circle';
                $classPagamento = "valor-positivo";
                $registro['estorno'] = false;
                $tituloIcone = "Informações sobre o crédito";
            } else {
                $iconeValor = 'fa-minus-circle';
                $classPagamento = "valor-negativo";
                $registro['estorno'] = true;
                $tituloIcone = "Informações sobre o estorno";
            }

            $rashAgenda = setParam(array('agenda_id' => $registro['agenda_id'], 'valor_a_pagar_id' => $registro['id']));
            $icone = "<i class='fa fa-info-circle' style='color: #000000 !important; cursor: pointer;' onclick='exibeInfoAgenda(\"{$rashAgenda}\")' data-toggle='tooltip' title='{$tituloIcone}' aria-hidden='true'></i>";
        } else {
            $iconeValor = 'fa-minus-circle';
            $classPagamento = "valor-negativo";
            $valorRecebido += $registro['valor'];
            $saldo -= $registro['valor'];
            $icone = "<i class='fa fa-arrow-circle-down' style='color: #000000 !important; cursor: pointer;' aria-hidden='true' onclick='exibeModalPagamentoView(\"{$registro['id']}\")' data-toggle='tooltip' title='Visualizar pagamento'></i>";
        }

        if ($registro['estorno'] === true) {
            $registro['descricao'] .= " - ESTORNO";
            $registro['percentual_repasse'] = null;
            $registro['valor_total'] = null;

            if($registro['solicitacao_correcao_agenda_id']){
                $registro['descricao'] .= " (Solicitação de alteração de agenda)";
            }
        }

        $saldoFormatado = formata_valor($saldo);
//        $valorProcedimentoFormatado = $registro['valor_total'] ? formata_valor($registro['valor_total']) : '-';
//        $percentualRepasseFormatado = $registro['percentual_repasse'] ? formata_valor($registro['percentual_repasse']) : '-';
        $classSaldo = $saldo >= 0 ? 'valor-positivo' : 'valor-negativo';
        $iconeSaldo = $saldo >= 0 ? 'fa-plus-circle' : 'fa-minus-circle';

        if ($registro['tipo'] == 'pagar' && $registro['remunerada'] == 'f' && $registro['valor'] == '0') {
            $tabela .= <<<HTML
                        <tr>
                            <td>{$dataRegistro}</td>
                            <td><span style="padding: 0 5px 0 5px;">$icone</span>{$registro['descricao']}</td>
                            <td>-</td>
                            <td class="{$classSaldo}"><span style="white-space: nowrap" >{$saldoFormatado} <i class="fa {$iconeSaldo}" aria-hidden="true"></i></span></td>
                        </tr>
HTML;
        } else {
            $tabela .= <<<HTML
                    <tr>
                        <td>{$dataRegistro}</td>
                        <td><span style="padding: 0 5px 0 5px;">$icone</span>{$registro['descricao']}</td>
                        <td class="{$classPagamento}"><span style="white-space: nowrap" >{$valor}  <i class="fa {$iconeValor}" aria-hidden="true"></i></span></td>
                        <td class="{$classSaldo}"><span style="white-space: nowrap" >{$saldoFormatado} <i class="fa {$iconeSaldo}" aria-hidden="true"></i></span></td>
                    </tr>
HTML;
        }


    }

    if ($saldoAnterior > 0) {
        $valorAReceber += $saldoAnterior;
    } else {
        $valorRecebido += $saldoAnterior;
    }

    $valorAReceberFormatado = formata_valor($valorAReceber);
    $valorRecebidoFormatado = formata_valor($valorRecebido);

    $estabelecimentos = $mProfissional->listaEstabelecimentosPorCpfProfissional($usucpfFormatado);
    $campoEstabelecimento = $db->monta_combom("estabelecimento_id", $estabelecimentos, 'S', '', '', '', '', '', 'N', '', true, $estabelecimentoId);

    $inicioFormatada = $dataInicio->format('d/m/Y');
    $fimFormatada = $dataFim->format('d/m/Y');


    if ($saldo) {
        $classSaldo = $saldo >= 0 ? 'valor-positivo' : 'valor-negativo';
        $saldoFormatado = formata_valor($saldo);
    } else {
        $classSaldo = "";
        $saldoFormatado = formata_valor(0);
    }

    $html = <<<HTML
                <div class="row">
                        <div class="portlet light calendar bordered" style="background-color: #FFF;">
                            <div class="row">
                                <div class="col-md-3">
                                    <div class="row" style="margin-top: 10px;">
                                        <div class="col-sm-3" style="padding-top: 5px; padding-right: 5px;">
                                            Período:
                                        </div>
                                        <div class="col-sm-9">
                                            <div class="dropdown" style="width: 100%">
                                                <button class="btn btn-default dropdown-toggle" type="button" id="dropdownMenu1" data-toggle="dropdown" aria-haspopup="true" aria-expanded="true" style="width: 100%">
                                                    <span class="label-periodo" style="float: left;">{$textoTipoPadrao}</span>
                                                    <span class="bs-caret" style="float: right;">
                                                        <i class="fa fa-angle-down" aria-hidden="true"></i>
                                                    </span>
                                                </button>
                                                <ul class="dropdown-menu" aria-labelledby="dropdownMenu1" style="width: 300px;">
                                                    <li><a href="javascript: filtrarExtrato('H')">Hoje</a></li>
                                                    <li><a href="javascript: filtrarExtrato('E')">Esta semana</a></li>
                                                    <li><a href="javascript: filtrarExtrato('U')">Últimos 30 dias</a></li>
                                                    <li role="separator" class="divider"></li>
                                                    <li>
                                                        <div style="padding: 5px">
                                                            <div class="">
                                                                Período de
                                                            </div>
                                                            <div class="input-group">
                                                                <div>
                                                                    <input type="text" id="inicio" name="inicio" value="{$inicioFormatada}" class="normal data-dropdown form-control" onchange="" data-date-format="dd/mm/yyyy">                                              
                                                                </div>
                                                                <span class="input-group-addon">Até</span>
                                                                <div>
                                                                    <input type="text" id="fim" name="fim" value="{$fimFormatada}" class="normal data-dropdown form-control" onchange="" data-date-format="dd/mm/yyyy">
                                                                </div>
                                                                <span class="input-group-btn">
                                                                   <button type="button" class="btn" onclick="filtrarExtrato('D')"><i class="fa fa-search" aria-hidden="true"></i></button>
                                                               </span>
                                                                
                                                            </div>
                                                        </div>
                
                                                    </li>
                                                </ul>
                                                <input type="hidden" name="tipoPeriodo" value="{$tipo}">
                                            </div>
                                        </div>
                                    </div>
                                    <div class="row" style="margin-top: 10px;">
                                        <div class="col-sm-3" style="padding-top: 5px; padding-right: 5px;">
                                            Estabelecimento:
                                        </div>
                                        <div class="col-sm-9">
                                            {$campoEstabelecimento}
                                        </div>
                                    </div>
                                    <div class="row" style="padding: 15px;">
                                        <table class="table table-striped" style="border: 1px solid;">
                                            <tr>
                                                <td style="font-size: 25px">Saldo</td>
                                                <td class="{$classSaldo}" style="font-size: 20px;">R$ {$saldoFormatado}</td>
                                            </tr>
                                            <tr>
                                                <td style="font-size: 20px">Créditos</td>
                                                <td style="font-size: 20px">R$ {$valorAReceberFormatado}</td>
                                            </tr>
                                            <tr>
                                                <td style="font-size: 20px">Repasses</td>
                                                <td style="font-size: 20px">R$ {$valorRecebidoFormatado}</td>
                                            </tr>
                                        </table>
                                    </div>
                                    <div class="row" style="padding: 15px; text-align: center">
                                            <a class="btn blue" href="javascript:imprimirExtrato()">
                                                <i class="fa fa-print"></i>
                                                Imprimir Extrato
                                            </a>
                                    </div>
                                </div>
                                <div class="col-md-9">
                                    <table id="tabela-extrato" class="table table-striped" data-unique-id="id" data-pagination="true" data-page-size="50">
                                        <thead>
                                            <tr>
                                                <th data-align="center">Data</th>
                                                <th>Descrição</th>
                                                <!--<th data-align="right">Valor do procedimento R$</th>-->
                                                <!--<th data-align="right">Percentual de repasse %</th>-->
                                                <th data-align="right">Valor R$</th>
                                                <th data-align="right">Saldo R$</th>
                                            </tr>
                                            <tr class="saldo-anterior">
                                                <th colspan="3" data-align="right">Saldo anterior</th>
                                                <th class="{$classSaldoAnterior}" data-align="right">{$saldoAnteriorFormatado}</th>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            {$tabela} 
                                        </tbody>
                                    </table>
                                </div>
                            </div>
                        </div>
                    </div>
                    <script type="text/javascript">
                        $(function () {
                            $('#tabela-extrato').bootstrapTable();
                            
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

                            //adicionarDatePicker();
                            
                            $('[name=estabelecimento_id]').change(function () {
                                filtrarExtrato();
                            });
                            
                            $('.bs-select').selectpicker({
                                iconBase: 'fa',
                                tickIcon: 'fa-check'
                            });
                            
                            $('[data-toggle="tooltip"]').tooltip();
                        });
                    </script>
HTML;

    return $html;
}
