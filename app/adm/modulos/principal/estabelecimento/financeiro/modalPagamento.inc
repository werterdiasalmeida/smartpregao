<?php
$p = getParam();
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mProfissional = new Profissional();
$mPessoa = new PessoaFisica();
$mProntuario = new Prontuario();
$mTipoConsulta = new TipoConsulta();
$mConvenio = new ConvenioEstabelecimento();
$mValorProcedimento = new ValorProcedimento();
$mProcedimento = new Procedimento();
$mEstabelecimento = new Estabelecimento();
$mValorPago = new ValorPago();
$mValorAPagar = new ValorAPagar();

?>
    <div class="modal fade in" id="modal-pagamento-view" tabindex="-1" role="modal-pagamento-view"
         aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <div class="modal-content">
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
                                                <div class="dz-preview dz-processing dz-success dz-complete">
                                                    <div class="dz-image"></div>
                                                    <div class="dz-details">
                                                        <div class="dz-size"><span data-dz-size=""><strong class="tamanho"></strong> <span class="escala"></span></span></div>
                                                        <div class="dz-filename"><span data-dz-name="" class="nome-completo"></span></div>
                                                    </div>
                                                    <div class="dz-progress">
                                                        <span class="dz-upload" data-dz-uploadprogress="" style="width: 100%;"></span>
                                                    </div>
                                                    <div class="dz-error-message">
                                                        <span data-dz-errormessage=""></span>
                                                    </div>
                                                    <div class="dz-error-mark">
                                                    </div>
                                                    <a href="javascript:;" class="btn blue btn-sm btn-block btn-download">Download</a>
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
                    <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                </div>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>
    <style type="text/css">
        .dropzone {
          min-height: 0;
          border: 1px solid #c2c6c8;
        }

        .dropzone .dz-message {
            margin: 0;
        }
    </style>

    <script type="text/javascript">
        function exibeModalPagamentoView(id){

            App.blockUI({
                message: 'Carregando...',
                target: '#modal-pagamento-view'
            });

            $('#modal-pagamento-view').find('.tabela-container').html(
                '<table class="tabela-itens-selecionados table table-stripped" data-pagination="true" data-page-size="5">' +
                '<thead>' +
                '<tr>' +
                '<th data-align="center" data-field="data">Data</th>' +
                '<th data-field="descricao">Descrição</th>' +
                '<th data-align="right" data-field="valorprocedimento">Valor do procedimento R$</th>' +
                '<th data-align="right" data-field="percentualrepasse">Percentual de repasse %</th>' +
                '<th data-align="right" data-field="valorpagamento">Valor de repasse R$</th>' +
                '</tr>' +
                '</thead>' +
                '<tfoot>' +
                    '<tr>' +
                        '<td data-align="right" style="text-align: right;" colspan="4">Valor total</td>' +
                        '<td data-align="right" style="text-align: right;">R$ <span class="valor-repasse"></span></td>' +
                    '</tr>' +
                '</tfoot>' +
                '</table>'
            );


            $.ajax({
                type: "POST",
                url: window.location.href,
                dataType : 'json',
                data: {
                    act : 'exibeInformacoesPagamento',
                    valor_pago_id : id
                },
                success: function (retorno) {
                    App.unblockUI('#modal-pagamento-view');

                    //nomeProfissional valorRepasse
                    $('#modal-pagamento-view .nome-profissional').html($('[name=profissional_id] option:selected').text());
                    $('#modal-pagamento-view .valor-repasse').html(retorno.params.valorTotal);

                    $('.tabela-itens-selecionados').bootstrapTable({
                        data: retorno.params.registros
                    });

                    //Definições do arquivo
                    $('#modal-pagamento-view .dz-preview').removeClass('dz-image-preview');
                    $('#modal-pagamento-view .dz-preview').removeClass('dz-file-preview');

                    var nomeCompleto = retorno.params.arquivo.arquivo.arqnome + '.' + retorno.params.arquivo.arquivo.arqextensao;
                    if(retorno.params.arquivo.isImagem){
                        $('#modal-pagamento-view .dz-preview').addClass('dz-image-preview');
                        $('#modal-pagamento-view .dz-image').html('<img src="../slideshow/slideshow/verimagem.php?arqid=' + retorno.params.arquivo.arquivo.arqid + '&newwidth=300" alt="' + nomeCompleto + '">');
                    }else{
                        $('#modal-pagamento-view .dz-preview').addClass('dz-file-preview');
                        $('#modal-pagamento-view .dz-image').html('<div class="dz-image"><img data-dz-thumbnail="" alt=""></div>');
                    }

                    $('#modal-pagamento-view .dz-size .tamanho').html(retorno.params.arquivo.tamanho);
                    $('#modal-pagamento-view .dz-size .escala').html(retorno.params.arquivo.escala);
                    $('#modal-pagamento-view .nome-completo').html(nomeCompleto);

                    $(function(){
                        $('#modal-pagamento-view .dropzone').find('.btn-download').click(function(){
                            downloadArquivo(retorno.params.arquivo.arquivo.arqid);
                        });
                    });

                    $('#modal-pagamento-view').modal('toggle');
                },
                error: function (result) {
                    App.unblockUI('.portlet-body .form');
                    exibirAlert('Não foi possível realizar a operação');
                }
            });

            function downloadArquivo(arquivo_id){
                window.location.href = window.location + '&act=downloadAnexo&arquivo_id=' + arquivo_id;
            }

        }
    </script>