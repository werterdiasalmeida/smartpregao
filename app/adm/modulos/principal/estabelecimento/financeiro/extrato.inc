<?php
$p = getParam();
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mProfissional = new Profissional();
$mPessoa = new PessoaFisica();
$mProntuario = new Prontuario();
$mTipoConsulta = new TipoConsulta();
$mConvenio = new ConvenioEstabelecimento();
$mValorProcedimento = new ValorProcedimento();
$mSolicitacaoCorrecaoAgenda = new SolicitacaoCorrecaoAgenda();
$mProcedimento = new Procedimento();
$mEstabelecimento = new Estabelecimento();
$mValorPago = new ValorPago();
$mValorAPagar = new ValorAPagar();
$fileSimec = new FilesSimec();
$mArquivo = new Arquivo();

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'filtrarExtrato' :
            $html = carregarExtrato($_REQUEST['profissional_id'],$_REQUEST['tipo'],formata_data_sql($_REQUEST['inicio']), formata_data_sql($_REQUEST['fim']));
            $db->sucessomAjax("",array('html' => $html));
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

            $dataInicio  = new DateTime($dados['inicio']);
            $dataTermino = new DateTime($dados['fim']);

            $arRetorno = array(
                'data'              => $dataInicio->format('d/m/Y'),
                'inicio'            => $dataInicio->format('H:i'),
                'fim'               => $dataTermino->format('H:i'),
                'tipoConsulta'      => $dadosTipoConsulta['descricao'],
                'procedimento'      => $dadosProcedimento['descricao'],
                'convenio'          => $dadosConvenio['nome'],
                'htmlPaciente'      => $htmlPaciente,
                'solicitacaoAjuste' => $descricaoSolicitacaoAjuste,
            );

            $db->sucessomAjax("",$arRetorno);
            break;
        case 'salvarPagamento':
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            $params['data_operacao'] = 'now()';
            $params['responsavel_operacao'] = $_SESSION['usuario_id'];
            $params['excluido'] = 'false';
            unset($params['id']);

            $valorPagoId = $mValorPago->salvarRegistro($params);

            $itens = json_decode(str_replace('\"','"',$params['jsonItens']));
            if(is_array($itens) && count($itens) > 0){
                $mValorAPagar->efetuaPagamentoEmLote($itens,$valorPagoId);
            }
            $db->commit();
            $db->sucessomAjax();
            break;
        case 'salvarAnexo' :
            $_FILES = utf8ArrayDecode($_FILES);
            $arqid = $fileSimec->setUploadArquivoEspecifico('', 'anexo');
            $db->commit();
            $db->sucessomAjax('' , array('arquivo_id' => $arqid));
            die;
            break;
        case 'downloadAnexo' :
            $fileSimec->getDownloadArquivo($_REQUEST['arquivo_id']);
            die;
            break;
        case 'exibeInformacoesPagamento':
            $params = array_merge($_POST, getParam());

            $valorPago = $mValorPago->carregarPorId($params['valor_pago_id'])->getDados();
            $arquivo = $mArquivo->carregarPorId($valorPago['arquivo_comprovante_id'])->getDados();
            $tamanhoArquivo = formatarTamanhoArquivo($arquivo['arqtamanho']);
            list($tamanho, $escala) = explode(' ', $tamanhoArquivo);

            $valorTotal = 0;
            $registros = $mValorAPagar->getByValorPago($params['valor_pago_id']);
            foreach($registros AS $chave => $registro){
                $valorTotal += $registro['valorpagamento'];
                $registros[$chave]['data'] = (new DateTime($registro['data_operacao']))->format('d/m/Y  H:i:s');
                $registros[$chave]['valorprocedimento'] = formata_valor($registro['valorprocedimento']);
                $registros[$chave]['percentualrepasse'] = formata_valor($registro['percentualrepasse']);
                $registros[$chave]['valorpagamento'] = formata_valor($registro['valorpagamento']);
            }

            $arRetorno = array(
                'valorTotal'    => formata_valor($valorTotal),
                'registros'     => $registros,
                'arquivo'       => array(
                        'arquivo'   => $arquivo,
                        'tamanho'   => $tamanho,
                        'escala'    => $escala,
                        'isImagem'  => isImagem($arquivo['arqextensao']),
                )
            );

            $db->sucessomAjax("",$arRetorno);
            break;
    }
}


UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);
include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();
monta_titulom('Extratro financeiro');
?>

    <div class="portlet light bordered">
        <div class="portlet-title">
            <div class="caption font-blue-sharp">
                <i class="icon-bar-chart font-blue-sharp"></i>
                <span class="caption-subject bold uppercase">
                Extratro Financeiro Completo
            </span>
            </div>
        </div>
        <div class="portlet-body form">
            <div class="row">
                <div class="col-sm-12">
                    <div class="lista-extrato">
                        <?= carregarExtrato() ?>
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
                            <div><label>Data</label><br /><span class="data"></span></div>
                            <div><label>Hora</label><br /><span class="hora"></span></div>
                            <div><label>Procedimento</label><br /><span class="procedimento"></span></div>
                            <div><label>Convênio</label><br /><span class="convenio"></span></div>
                            <div><label>Tipo de consulta</label><br /><span class="tipo_consulta"></span></div>
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

    <div class="modal fade in" id="modal-pagamento" tabindex="-1" role="modal-pagamento"
         aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <div class="modal-content">
                <form role="form" name="formulario-pagamento" id="formulario-pagamento" class="formulario-pagamento" method="POST" enctype="multipart/form-data">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                        <h4 class="modal-title">Pagamento</h4>
                    </div>
                    <div class="modal-body">
                        <div class="row">
                            <div class='col-sm-12'>
                                <div class="row">
                                    <div class="col-md-6">
                                        <div class="form-group">
                                            <label>Profissional</label>
                                            <div>
                                                <strong><span class="nome-profissional"></span></strong>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="col-md-6">
                                        <div class="form-group">
                                            <label>Valor total do repasse</label>
                                            <div>
                                                <strong><span class="valor-repasse"></span></strong>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Comprovante de pagamento</label>
                                            <div id="file-form-anexo" class="dropzone text-center">
                                                <div class="fallback">
                                                    <input type="file" id="anexo" name="anexo" class="form-control" />
                                                </div>
                                                <div class="adiciona-arquivos">
                                                    <div style="font-size: 18px; margin-bottom: 4px;">
                                                        Arraste o arquivo para cá
                                                    </div>
                                                    <div style="font-size: 18px; margin-bottom: 4px;">
                                                        ou
                                                    </div>
                                                    <button type="button" id="btn-anexo" class="btn green">Selecione o arquivo do seu computador</button>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class='col-sm-12'>
                                <div class="tabela-container"></div>
                            </div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="submit" id="btn-salvar-pagamento" class="btn blue"><i class="fa fa-save"></i> Salvar </button>
                        <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                    </div>
                </form>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>

    <? include_once 'modalPagamento.inc'; ?>

    <style type="text/css">
        .dropzone {
            min-height: 0;
            border: 1px solid #c2c6c8;
        }

        .dropzone .dz-message {
            margin: 0;
        }

        .data-dropdown{
            width: 100px ;
            display: inline;
        }

        .valor-positivo{
            color: #0a6aa1;
        }

        .valor-negativo{
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

        .resumo-agenda label{
            font-weight: bold;
        }

        .checkbox_item{
            cursor: pointer;
        }
    </style>
    <script type="text/javascript">
        var dropzoneControl = null;

        $(function(){

            $('#modal-pagamento').on('hidden.bs.modal', function (e) {
                dropzoneControl.removeAllFiles(true);
                $('#formulario-pagamento #file-form-anexo .adiciona-arquivos').show();
            })

            $("#formulario-pagamento #file-form-anexo").dropzone({
                url: window.location.href,
                params : {
                    act : 'salvarAnexo'
                },
                maxFiles: 1,
                maxFilesize: 2, // MB
                acceptedFiles: 'image/*,application/pdf',
                paramName : 'anexo',
                clickable : '#btn-anexo',
                dictDefaultMessage : '',
                dictMaxFilesExceeded : 'Você não pode enviar mais arquivos.',
                dictFileTooBig: 'Arquivo muito grande para ser enviado (tamanho máximo {{maxFilesize}} mb).',
                dictInvalidFileType : 'Tipo de arquivo inválido.',
                init: function() {
                    $('#formulario-pagamento #file-form-anexo .adiciona-arquivos').show();

                    if(!dropzoneControl){
                        dropzoneControl = this;
                    }

                    this.on("addedfile", function(file) {
                        // Create the remove button
                        var removeButton = Dropzone.createElement("<a href='javascript:;' class='btn red btn-sm btn-block'>Remover</a>");
                        var downloadButton = Dropzone.createElement("<a href='javascript:;' class='btn blue btn-sm btn-block'>Download</a>");

                        // Capture the Dropzone instance as closure.
                        var _this = this;

                        // Listen to the click event
                        removeButton.addEventListener("click", function(e) {
                            // Make sure the button click doesn't submit the form:
                            e.preventDefault();
                            e.stopPropagation();
                            // Remove the file preview.
                            _this.removeFile(file);
                            // If you want to the delete the file on the server as well,
                            // you can do the AJAX request here.
                            $('#formulario-pagamento #file-form-anexo .adiciona-arquivos').show();
                        });

                        // Listen to the click event
                        downloadButton.addEventListener("click", function(e) {
                            downloadAnexo(this);
                        });

                        // Add the button to the file preview element.
                        file.previewElement.appendChild(removeButton);
                        file.previewElement.appendChild(downloadButton);
                    });

                    this.on("dragover", function(file) {
                        $('#file-form-anexo').css('background-color', '#DDDDDD');
                    });

                    var removerFundoDropzone = function(file) {
                        $('#file-form-anexo').css('background-color', 'transparent');
                    };

                    this.on("dragleave", removerFundoDropzone);
                    this.on("drop", removerFundoDropzone);

                    this.on("error", function(file,msg){
                        dropzoneControl.removeFile(file);
                        exibirAlert(msg);
                    });

                    this.on("success", function(file, response) {
                        try {
                            var responseObj = JSON.parse(response);
                            var hidden = Dropzone.createElement('<input type="hidden" name="arquivo_id" class="arquivo" value="'+responseObj.params.arquivo_id+'" />');
                            file.previewElement.appendChild(hidden);
                            $('#formulario-pagamento #file-form-anexo .adiciona-arquivos').hide();
                        }catch (e) {
                            exibirAlert('Não foi possível enviar o arquivo.');
                            this.removeFile(file);
                        }
                    });
                },
                accept: function(file, done) {
                    done();
                }
            });

            $(".formulario-pagamento").validateForm({
                submitHandler: function (e) {
                    $(e).find('input[type=submit], button[type=submit]').prop('disabled', true).html('Enviando...');

                    //sucesso
                    App.blockUI({
                        message: 'Carregando...',
                        target: '.portlet-body .form'
                    });

                    if(!$('[name^=arquivo_id]').val() || $('[name^=arquivo_id]').val() == ''){
                        exibirAlert('O comprovante de pagamento é obrigatório.');
                        $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                        return false;
                    }

                    $('#modal-pagamento').modal('toggle');

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType : 'json',
                        data: {
                            act : 'salvarPagamento',
                            jsonItens: $('#modal-pagamento #jsonItens').val(),
                            valor_pagamento: $('#modal-pagamento #valorRepasse').val(),
                            profissional_id: $('[name=profissional_id]').val(),
                            arquivo_comprovante_id : $('[name^=arquivo_id]').val()
                        },
                        success: function (retorno) {
                            App.unblockUI('.portlet-body .form');
                            exibirSucesso(retorno.msg ? retorno.msg : 'Operação realizada com sucesso!');
                            filtrarExtrato();
                            $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                        },
                        error: function (result) {
                            App.unblockUI('.portlet-body .form');
                            exibirAlert('Não foi possível realizar a operação');
                            $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                        }
                    });

                }
            });
        });

        function filtrarExtrato(tipo) {
            //sucesso
            App.blockUI({message: 'Carregando...',target: '.page-content'});

            $.ajax({
                type: "POST",
                url: window.location.href,
                dataType : 'json',
                data: {
                    act : 'filtrarExtrato',
                    profissional_id: $('[name=profissional_id]').val(),
                    tipo: tipo || $('[name=tipoPeriodo]').val(),
                    inicio: $('[name=inicio]').val(),
                    fim:  $('[name=fim]').val()
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

        function downloadAnexo(downloadButton){
            var campoArquivo = $(downloadButton).closest('.dz-preview').find('.arquivo');
            if(campoArquivo && campoArquivo.val()){
                window.location.href = window.location + '&act=downloadAnexo&arquivo_id=' + campoArquivo.val();
            }else{
                exibirAviso('Nenhum arquivo encontrado.');
            }
        }


        function exibeInfoAgenda(rash){
            $.ajax({
                type: "POST",
                url: "?modulo=principal/estabelecimento/financeiro/extrato&acao=C" + rash,
                dataType : 'json',
                data: {
                    act : 'exibeInformacoesAgenda'
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

        function selecionaTodos(){
            var todos = $('[id^=check_item_]').length;
            var checkados = $('[id^=check_item_]:checked').length;

            if(todos > 0 && todos == checkados){
                $('[id^=check_item_]').prop('checked',false);
            }else{
                $('[id^=check_item_]').prop('checked','checked');
            }

            verificaBtnPagamento();
        }

        function verificaBtnPagamento(){
            $('.btn-pagamento').unbind('click');

            if($('[id^=check_item_]:checked').length > 0){
                $('.btn-pagamento').click(function () {
                    if($('[name=profissional_id]').val() == ''){
                        exibirAlert('Filtre um profissional específico para efetuar o pagamento');
                    }else{
                        exibeModalPagamentos();
                    }
                });
            }else{
                $('.btn-pagamento').click(function () {
                    exibirAlert('Selecione um ou mais itens para efetuar o pagamento');
                });
            }

            gerenciarCheckboxTodos();
        }

        function gerenciarCheckboxTodos(){
            var todos = $('[id^=check_item_]').length;
            var checkados = $('[id^=check_item_]:checked').length;
            if(todos > 0 && todos == checkados){
                $('#checkTodos').prop('checked','checked');
            }else{
                $('#checkTodos').prop('checked',false);
            }
        }

        function exibeModalPagamentos(){
            var checkados = $('[id^=check_item_]:checked');

            $('#modal-pagamento').find('.tabela-container').html(
                '<table class="tabela-itens-selecionados table table-stripped" data-pagination="true" data-page-size="5">' +
                '<thead>' +
                '<tr>' +
                '<th data-align="center" data-field="data">Data</th>' +
                '<th data-field="descricao">Descrição</th>' +
                '<th data-align="right" data-field="valorProcedimento">Valor do procedimento R$</th>' +
                '<th data-align="right" data-field="percentualRepasse">Percentual de repasse %</th>' +
                '<th data-align="right" data-field="valorPagamento">Valor de repasse R$</th>' +
                '</tr>' +
                '</thead>' +
                '<tfoot>' +
                '<tr>' +
                '<td data-align="right" style="text-align: right;" colspan="4">Valor total</td>' +
                '<td data-align="right" style="text-align: right;">R$ <span class="valor-repasse"></span></td>' +
                '</tr>' +
                '</tfoot>' +
                '</table>' +
                '<input type="hidden" id="jsonItens" value="">' +
                '<input type="hidden" id="valorRepasse" value="">'
            );

            var registros = [];
            var ids = [];
            valorPagamentoTotal = 0;
            $.each(checkados, function(){
                var item = $(this).closest('tr');
                var data = item.find('.td-data .valor').html();
                var descricao = item.find('.td-descricao .valor').html();
                var valorProcedimento = item.find('.td-valor-procedimento .valor').html();
                var percentualRepasse = item.find('.td-percentual-repasse .valor').html();
                var valorPagamento = item.find('.td-valor-pagamento .valor').html();

                valorPagamentoTotal += desformataValor(valorPagamento);
                registros.push({data: data,descricao: descricao, valorProcedimento: valorProcedimento, percentualRepasse:percentualRepasse, valorPagamento:valorPagamento});
                ids.push($(this).val());

            });

            //nomeProfissional valorRepasse
            $('#modal-pagamento .nome-profissional').html($('[name=profissional_id] option:selected').text());
            $('#modal-pagamento .valor-repasse').html(formataValor(valorPagamentoTotal));
            $('#modal-pagamento #jsonItens').val(JSON.stringify(ids));
            $('#modal-pagamento #valorRepasse').val(valorPagamentoTotal);


            $('.tabela-itens-selecionados').bootstrapTable({
                data: registros
            });

            $('#modal-pagamento').modal('toggle');
        }
    </script>
<?php
function carregarExtrato($profissionalId = null, $tipo = 'U', $data_inicio = null, $data_termino = null)
{
    global $db;
    $p = getParam();

    $mProfissional = new Profissional();
    $mEstabelecimentoEspecialidadeProfissional = new EstabelecimentoEspecialidadeProfissional();
    $mValorAPargar = new ValorAPagar();

    //Define o tipo
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

    $pessoas = $mEstabelecimentoEspecialidadeProfissional->getComboProfissionaisPorEstabelecimento($p['id']);

    $campoProfissional = $db->monta_combom("profissional_id", $pessoas, 'S', 'Todos', '', '', '', '', 'N','',true, $profissionalId);

    $arWhere = array(
        'profissional_id'       => $profissionalId,
        'inicio'                => $dataInicio->format('Y-m-d'),
        'fim'                   => $dataFim->format('Y-m-d'),
    );

    $saldoAnterior  = $mValorAPargar->getSaldoAnteriorEstabelecimentoByWhere($p['id'],$profissionalId,$dataInicio->format('Y-m-d'));
    $data           = $mValorAPargar->getExtratoCompletoEstabelecimentoByWhere($p['id'],$arWhere);

    $saldoAnteriorFormatado = formata_valor($saldoAnterior) . ' <i class="fa ' . ($saldoAnterior >= 0 ? 'fa-plus-circle' : 'fa-minus-circle') . '" aria-hidden="true"></i>';
    $classSaldoAnterior     = $saldoAnterior >= 0 ? 'valor-positivo' : 'valor-negativo';

    //Define os itens da tabela
    $tabela = "";
    $saldo = $saldoAnterior;
    $valorARepassar = 0;
    $valorRepassado = 0;


    $colspanProfissional = '7';
    $tituloProfissional = '<th>Profissional</th>';
    if(!empty($profissionalId)){
        $colspanProfissional = '6';
        $tituloProfissional = "";
    }

    foreach($data AS $indice => $registro){
        $dataRegistro = (new DateTime($registro['data_operacao']))->format('d/m/Y  H:i:s');
        $valor = formata_valor($registro['valor']);

        if($registro['tipo'] == 'pagar'){
            $valorARepassar += $registro['valor'];
            $saldo += $registro['valor'];

            if($registro['valor'] >= 0){
                $iconeValor = 'fa-plus-circle';
                $classPagamento = "valor-positivo";
                $registro['estorno'] = false;
                $tituloIcone = "Informações sobre o crédito";
            }else{
                $iconeValor = 'fa-minus-circle';
                $classPagamento = "valor-negativo";
                $registro['estorno'] = true;
                $tituloIcone = "Informações sobre o estorno";
            }

            $rashAgenda = setParam(array('agenda_id' => $registro['agenda_id'], 'valor_a_pagar_id' => $registro['id']));
            $icone = "<i class='fa fa-info-circle' style='color: #000000 !important; cursor: pointer;' onclick='exibeInfoAgenda(\"{$rashAgenda}\")' data-toggle='tooltip' title='{$tituloIcone}' aria-hidden='true'></i>";
        }else{
            $iconeValor = 'fa-minus-circle';
            $classPagamento = "valor-negativo";
            $valorRepassado += $registro['valor'];
            $saldo -= $registro['valor'];
            $registro['estorno'] = false;
            $icone = "<i class='fa fa-arrow-circle-down' style='color: #000000 !important; cursor: pointer;' aria-hidden='true' onclick='exibeModalPagamentoView(\"{$registro['id']}\")' data-toggle='tooltip' title='Visualizar pagamento'></i>";
        }

        if($registro['estorno'] === true){
            $registro['descricao'] .= " - ESTORNO";
            $registro['percentual_repasse'] = null;
            $registro['valor_total'] = null;

            if($registro['solicitacao_correcao_agenda_id']){
                $registro['descricao'] .= " (Solicitação de alteração de agenda)";
            }
        }

        $saldoFormatado = formata_valor($saldo);
        $valorProcedimentoFormatado = $registro['valor_total'] ? formata_valor($registro['valor_total']) : '-';
        $percentualRepasseFormatado = $registro['percentual_repasse'] ? formata_valor($registro['percentual_repasse']) : '-';
        $classSaldo = $saldo >= 0 ? 'valor-positivo' : 'valor-negativo';
        $iconeSaldo = $saldo >= 0 ? 'fa-plus-circle' : 'fa-minus-circle';

        $linhaCheckbox = "-";
        if($registro['tipo'] == 'pagar'){
            if(!empty($registro['referencia'])){
                $linhaCheckbox = '<input type="checkbox" checked disabled="disabled" data-toggle="tooltip" title="Registro pago">';
            }else{
                $linhaCheckbox = '<input class="checkbox_item" type="checkbox" id="check_item_' . $registro['id'] . '" value="'. $registro['id'] .'">';
            }
        }

        $tdProfissional = '<td class="td-profissional">' . $registro['profissional'] . '</td>';
        if(!empty($profissionalId)){
            $tdProfissional = "";
        }

        if($registro['tipo'] == 'pagar' && $registro['remunerada'] == 'f' && $registro['valor'] == '0'){
            $tabela .= <<<HTML
                        <tr>
                            <td>-</td>
                            <td class="td-data"><span class="valor" >{$dataRegistro}</span></td>
                            {$tdProfissional}
                            <td class="td-descricao"><span style="padding: 0 5px 0 5px;">$icone</span><span class="valor">{$registro['descricao']}</span></td>
                            <td>-</td>
                            <td>-</td>
                            <td>-</td>
                            <td class="{$classSaldo}">{$saldoFormatado} <i class="fa {$iconeSaldo}" aria-hidden="true"></i></td>
                        </tr>
HTML;
        }else{
            $tabela .= <<<HTML
                    <tr>
                        <td>{$linhaCheckbox}</td>
                        <td class="td-data"><span class="valor" >{$dataRegistro}</span></td>
                        {$tdProfissional}
                        <td class="td-descricao"><span style="padding: 0 5px 0 5px;">$icone</span><span class="valor">{$registro['descricao']}</span></td>
                        <td class="td-valor-procedimento valor-positivo"><span class="valor">{$valorProcedimentoFormatado}</span></td>
                        <td class="td-percentual-repasse"><span class="valor">{$percentualRepasseFormatado}</span></td>
                        <td class="td-valor-pagamento {$classPagamento}"><span class="valor">{$valor}</span> <i class="fa {$iconeValor}" aria-hidden="true"></i></td>
                        <td class="{$classSaldo}">{$saldoFormatado} <i class="fa {$iconeSaldo}" aria-hidden="true"></i></td>
                    </tr>
HTML;

        }
    }

    if($saldoAnterior > 0){
        $valorARepassar +=$saldoAnterior;
    }else{
        $valorRepassado +=$saldoAnterior;
    }

    $valorARepassarFormatado = formata_valor($valorARepassar);
    $valorRepassadoFormatado = formata_valor($valorRepassado);

    $inicioFormatada = $dataInicio->format('d/m/Y');
    $fimFormatada = $dataFim->format('d/m/Y');


    if($saldo){
        $classSaldo = $saldo >= 0 ? 'valor-positivo' : 'valor-negativo';
        $saldoFormatado = formata_valor($saldo);
    }else{
        $classSaldo = "";
        $saldoFormatado = formata_valor(0);
    }

    $html = <<<HTML
                <div class="row" xmlns="http://www.w3.org/1999/html">
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
                                                    <li><a href="javascript: filtrarExtrato('A')">Este ano</a></li>
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
                                                <input type="hidden" name="tipoPeriodo" value="">
                                            </div>
                                        </div>
                                    </div>
                                    <div class="row" style="margin-top: 10px;">
                                        <div class="col-sm-3" style="padding-top: 5px; padding-right: 5px;">
                                            Profissional:
                                        </div>
                                        <div class="col-sm-9">
                                            {$campoProfissional}
                                        </div>
                                    </div>
                                    <div class="row" style="padding: 15px;">
                                        <table class="table table-striped" style="border: 1px solid;">
                                            <tr>
                                                <td style="font-size: 20px">Valor a repassar</td>
                                                <td style="font-size: 20px">R$ {$valorARepassarFormatado}</td>
                                            </tr>
                                            <tr>
                                                <td style="font-size: 20px">Valor repassado</td>
                                                <td style="font-size: 20px">R$ {$valorRepassadoFormatado}</td>
                                            </tr>
                                            <tr>
                                                <td style="font-size: 25px">Saldo</td>
                                                <td class="{$classSaldo}" style="font-size: 20px;">R$ {$saldoFormatado}</td>
                                            </tr>
                                        </table>
                                    </div>
                                </div>
                                <div class="col-md-9">
                                    <table id="tabela-extrato" class="table table-striped" data-unique-id="id" data-pagination="true" data-page-size="50">
                                        <thead>
                                            <tr>
                                                <th data-align="center">
                                                <input style="cursor: pointer;" type="checkbox" id="checkTodos" data-toggle='tooltip' title='Selecionar todos'>
                                                </th>
                                                <th data-align="center">Data</th>
                                                {$tituloProfissional}
                                                <th>Descrição</th>
                                                <th data-align="right">Valor do procedimento R$</th>
                                                <th data-align="right">Percentual de repasse %</th>
                                                <th data-align="right">Valor de repasse R$</th>
                                                <th data-align="right">Saldo R$</th>
                                            </tr>
                                            <tr class="saldo-anterior">
                                                <th colspan="{$colspanProfissional}" data-align="right">Saldo anterior</th>
                                                <th class="{$classSaldoAnterior}" data-align="right">{$saldoAnteriorFormatado}</th>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            {$tabela} 
                                        </tbody>
                                    </table>
                                </div>
                                <div class="col-md-12 text-center">
                                    <button type="button" class="btn green-jungle btn-pagamento">
                                        <i class="fa fa-usd"></i> Efetuar pagamento
                                    </button>
                                    <button type="button" class="btn btn-voltar">
                                        <i class="fa fa-arrow-left"></i> Voltar
                                    </button>
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
                            
                            $('[name=profissional_id]').change(function () {
                                filtrarExtrato();
                            });
                            
                            $('.bs-select').selectpicker({
                                iconBase: 'fa',
                                tickIcon: 'fa-check'
                            });
                            
                            $('[data-toggle="tooltip"]').tooltip();
                            
                            $('.btn-voltar').click(function () {
                                window.location = '?modulo=principal/estabelecimento/lista&acao=C';
                            });
                            
                            $('.checkbox_item').click(function () {
                                verificaBtnPagamento();
                            });
                            
                            $('#checkTodos').click(function () {
                                selecionaTodos();
                            });
                            
                            verificaBtnPagamento();
                        });
                    </script>
                </div>
HTML;



//ver($tdProfissional,$tituloProfissional,$colspanProfissional,d);



    return $html;
}
