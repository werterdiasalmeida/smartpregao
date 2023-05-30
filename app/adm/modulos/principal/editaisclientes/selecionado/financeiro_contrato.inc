    <div class="row">
        <div class="col-md-4">
            <div class="form-group">
                <label><b>Total de Recebimentos (R$)</b></label>
                <br />
                <p class="font-blue-sharp text-right"><b><?php echo formata_valor($dadosFinanceiroGLobal['total_recebimentos']) ?></b></p>
            </div>
        </div>
        <div class="col-md-4">
            <div class="form-group">
                <label><b>Total de Pagamentos (R$)</b></label>
                <br />
                <p class="font-red text-right"><b><?php echo formata_valor($dadosFinanceiroGLobal['total_pagamentos']) ?></b></p>
            </div>
        </div>
        <div class="col-md-4">
            <div class="form-group">
                <label><b>Saldo Total (R$)</b></label>
                <br />
                <p class="font-blue-sharp text-right"><b><?php echo formata_valor($dadosFinanceiroGLobal['saldo_total']) ?></b></p>
            </div>
        </div>
    </div>
    
    <a onclick="exibirModalNovoFinanceiro()">
        <i class="bi bi-plus-square" title="Novo Documento"></i>
        Novo Lançamento Financeiro
    </a>
    <?php
    $mEditalClienteFinanceiro = new EditalClienteFinanceiro();
    $where = " editalcliente_id = {$dadosEditalCliente['id']}";
    $rs = $mEditalClienteFinanceiro->getList($where);
    unset($arConfig, $acoes);
    $acao = <<<ACAO
                            <div style="min-width: 20px;">
                               <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirFinanceiro({id});" data-toggle="tooltip"  title="Excluir">
                               <i class="bi bi-x-square"></i>
                               </a>
                            </div> 
                            ACAO;
    $arConfig[5]['html'] = <<<HTML
                                <a onclick="baixarAnexo({arqid})">{arquivo}</a>
                            HTML;
    $arConfig[2]['html'] = <<<HTML
                            <a onclick="abrirFinanceiro({id})">{titulo}</a>
                        HTML;
    $mEditalClienteFinanceiro = new Listam(array("style" => "width: 100%;"));
    $mEditalClienteFinanceiro->setCabecalho(array(
        'Transação',
        'Vencimento',
        'Titulo',
        'Valor (R$)',
        'Situação',
        'Documento',
        'Ação'
    ));
    $mEditalClienteFinanceiro->setCorpo($rs, $arConfig);
    $mEditalClienteFinanceiro->setAcao($acao);
    $mEditalClienteFinanceiro->show();
    ?>
    <!-- MODAL de Financeiro -->
    <div class="modal fade in" id="modal-novo-financeiro" tabindex="-1" role="modal-novo-financeiro" aria-hidden="true">
        <div class="modal-dialog">
            <form role="form" name="formulario" id="form-financeiro" class="edital-financeiro-form" method="POST" enctype="multipart/form-data">
                <input type="hidden" name="act" value="salvarFinanceiro" />
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                        <h4 class="modal-title">Novo Lançamento Financeiro</h4>
                    </div>
                    <div class="modal-body">
                        <input type="hidden" name="editalcliente_id" value="<?php echo $dadosEditalCliente['id'] ?>" />
                        <div id="div_novo-financeiro">
                            <div class="col-md-6">
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Titulo</label>
                                            <?= campo_textom('titulo', 'S', 'S', '', 100, 100, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-6">
                                        <div class="form-group">
                                            <label>Vencimento</label>
                                            <?php
                                            $data = date("m/d/Y");
                                            echo campo_datam('data_vencimento', 'S', 'S', 'S', 'S', '', '', $data); ?>
                                        </div>
                                    </div>
                                    <div class="col-md-6">
                                        <div class="form-group">
                                            <label>Competência</label>
                                            <?php
                                            $data = date("m/d/Y");
                                            echo campo_datam('data_competencia', 'S', 'S', 'S', 'S', '', '', $data); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Transação</label>
                                            <?
                                            $opcoes = array(
                                                "Entrada" => array("valor" => 'Entrada'),
                                                "Saída" => array("valor" => 'Saída'),
                                                "A Receber" => array("valor" => 'A Receber'),
                                                "A Pagar" => array("valor" => 'A Pagar', 'default' => true)
                                            );
                                            echo campo_radiom('tipo', $opcoes, 'S'); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Descrição</label>
                                            <?= campo_textaream('descricao', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Origem / Destino</label>
                                            <?
                                            $opcoes = array(
                                                "Cliente" => array("valor" => 'Cliente', 'default' => true),
                                                "Fornecedor" => array("valor" => 'Fornecedor'),
                                                "Colaborador" => array("valor" => 'Colaborador')
                                            );
                                            echo campo_radiom('origem_destino', $opcoes, 'S'); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Nome</label>
                                            <?= campo_textom('nome_origem_destino', 'S', 'S', '', 100, 100, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Valor (R$)</label>
                                            <br />
                                            <?= campo_textom('valor', 'S', 'S', '', 255, 255, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Tipo de Documento</label>
                                            <?
                                            $opcoes = array(
                                                "Boleto" => array("valor" => 'Boleto', 'default' => true),
                                                "Nota Fiscal" => array("valor" => 'Nota Fiscal'),
                                                "Fatura" => array("valor" => 'Fatura'),
                                                "Nota de Pagamento" => array("valor" => 'Nota de Pagamento'),
                                                "Outros" => array("valor" => 'Outros')
                                            );
                                            echo campo_radiom('tipo_documento', $opcoes, 'S'); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Situação</label>
                                            <?
                                            $opcoes = array(
                                                "Cadastrado" => array("valor" => 'Cadastrado', 'default' => true),
                                                "Em processamento" => array("valor" => 'Em processamento'),
                                                "Pago/Recebido" => array("valor" => 'Pago/Recebido')
                                            );
                                            echo campo_radiom('situacao', $opcoes, 'S'); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="row">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <label>Arquivo</label>
                                            <input type="file" id="arquivo" name="arquivo" class="form-control" />
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="clearfix"></div>
                    <div class="modal-footer">
                        <button type="submit" onclick="$('#form-financeiro').submit();" class="btn green" data-dismiss="modal">Salvar</button>
                        <button type="button" class="btn red" data-dismiss="modal">Fechar</button>
                    </div>
                </div>
            </form>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>
    <script>
        $('#valor').mask('#.##0,00', {
            reverse: true
        });

        function exibirModalNovoFinanceiro() {
            var modal = $('#modal-novo-financeiro');
            modal.modal('show');
        }

        function excluirFinanceiro(id) {
            if (confirm('Deseja apagar esta transação?')) {
            url = '?modulo=principal/editaisclientes/selecionado&acao=C&p=<?= $_REQUEST['p'] ?>&act=excluirFinanceiro&id_financeiro=' + id;
            location.href = url;
        }
        }

        function abrirFinanceiro(id) {

        }
    </script>