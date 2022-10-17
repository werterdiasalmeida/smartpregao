<?php
$mEstabelecimentoPacoteMensagem = new EstabelecimentoPacoteMensagem();

$mIugu = new SimecIugu();
$urlBoletoIugu = $mIugu->getUrl();

$p = getParam();

$mTipoMensagem = new TipoMensagem($_REQUEST['tipo_mensagem_id']);
$dadosTipoMensagem = $mTipoMensagem->getDados();

$pacotesPendentes = $mEstabelecimentoPacoteMensagem->getPacotesPagamentoPendente($p['id'], $dadosTipoMensagem['codigo']);
$paramPadrao      = setParam(['id' => $p['id'], 'tipo_mensagem_id' => $_REQUEST['tipo_mensagem_id']], false);
?>
<style type="text/css">
    .info-pacote{
        display: none;
    }
</style>
<div class="">
    <? if(is_array($pacotesPendentes) && count($pacotesPendentes) > 0): ?>
        <div class="alert alert-warning">
            <strong>Atenção</strong> Você possui um ou mais pacotes pendentes de pagamento
        </div>
        <?
            foreach ($pacotesPendentes AS $pacotePendente):
                $paramPadraoPacote = setParam(['id' => $p['id'], 'tipo_mensagem_id' => $_REQUEST['tipo_mensagem_id'], 'estabelecimento_pacote_mensagem_id' => $pacotePendente['estabelecimento_pacote_mensagem_id'] ], false);
        ?>
            <div class="row" style="border: 1px solid #bfb8b8; margin: 10px;">
                <div class="col-md-6">
                    <h4>Informações do pacote</h4>
                    <label class="bold">Data contratação: </label> <?= formata_data($pacotePendente['data_contratacao']) ?><br>
                    <label class="bold">Situação: </label> <label class="bold font-yellow-gold">
                        <? if(!$pacotePendente['fatura_id']): ?>
                            Fatura não gerada
                        <? elseif($pacotePendente['vencida'] == 't'): ?>
                            Fatura vencida
                        <? else: ?>
                            Fatura pendente
                        <? endif; ?>
                    </label><br>
                    <? if($pacotePendente['fatura_id'] && $pacotePendente['vencida'] == 'f'): ?>
                        <label class="bold">Data vencimento: </label> <?= formata_data($pacotePendente['data_vencimento']) ?><br>
                    <? endif; ?>
                    <label class="bold">Valor (R$): </label> <?= formata_valor($pacotePendente['valor']) ?><br>
                    <label class="bold">Quantidade de mensagens: </label> <?= $pacotePendente['quantidade_mensagens'] ?><br><br>
                </div>
                <div class="col-md-6 text-center" style="padding-top: 35px;">
                    <? if($pacotePendente['fatura_id'] && $pacotePendente['vencida'] == 'f'): ?>
                        <a id="btn-abrir-boleto" type="button" class="btn blue" target="_blank" style="width: 150px;" href="<?= $urlBoletoIugu . $pacotePendente['secure_id_iugu'] ?>">Visualizar Boleto</a>
                    <? elseif($pacotePendente['vencida'] == 't'): ?>
                        <button id="btn-gerar-boleto" type="button" class="btn green-jungle" style="margin-top: 10px; width: 150px;" onclick="confirmGerarBoleto('<?= $paramPadraoPacote ?>');"">Gerar Nova Fatura</button><br>
                    <? else: ?>
                        <button id="btn-gerar-boleto" type="button" class="btn green-jungle" style="margin-top: 10px; width: 150px;" onclick="confirmGerarBoleto('<?= $paramPadraoPacote ?>');"">Gerar Fatura</button><br>
                    <? endif; ?>

                    <button id="btn-cancelar-boleto" type="button" class="btn red" style="margin-top: 10px; width: 150px;" onclick="confirmCancelarSolicitacaoPacote('<?= $paramPadraoPacote ?>');">Cancelar Solicitação</button>
                </div>
            </div>

        <? endforeach;  ?>
    <?  else: ?>
        <div class="form-body">
            <div class="row">
                <div class="col-md-12">
                    <div class="form-group">
                        <label>Plano</label>
                        <?php
                            $mPlanoPacoteMensagem = new PlanoPacoteMensagem();
                            $planosPacoteMensagem = $mPlanoPacoteMensagem->getComboPacotesMensagemByTipo($dadosTipoMensagem['codigo']);
                            $db->monta_combom("plano_pacote_mensagem_id", $planosPacoteMensagem, 'S', 'Selecione...', '', '', '', '', 'S');
                        ?>
                    </div>
                </div>
                <div class="col-md-12 info-pacote">
                    <div>
                        <h4>Informações do pacote</h4>
                        <label class="bold">Valor (R$): </label> <span class="valor"></span><br>
                        <label class="bold">Quantidade de mensagens: </label> <span class="quantidade"></span><br><br>
                    </div>
                </div>
            </div>
        </div>
    <? endif; ?>
</div>
<script type="text/javascript">
    $(function () {
        // Rotinas executadas no javascript do simec, como é uma solicitação ajax é necessário chamar manualmente
        if($().selectpicker){
            $('.bs-select').selectpicker({
                iconBase: 'fa',
                tickIcon: 'fa-check'
            });
        }
        adicionarLabelObrigatorio($(':required'));
        adicionarObrigatorio($('.obrigatorio'));

        $('[name=plano_pacote_mensagem_id]').change(function () {
            var selecionado = $(this).val();
            $('.info-pacote').hide();

            if(selecionado){
                //Redefinindo a ação e o texto do botão de salvar
                $('#modal-generica #btn-salvar-modal').html('Contratar').show().unbind( "click" ).click(function(){
                    confirmSolicitacaoPacote('<?= $paramPadrao ?>');
                });

                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    dataType : 'json',
                    data: {
                        act : 'carregaInformacoesPlanoPagamento',
                        plano_pacote_mensagem_id: selecionado
                    },
                    beforeSend: function( xhr ) {
                        App.blockUI({message: 'Carregando...',target: '#modal-generica .modal-dialog'});
                    },
                    complete: function( data ) {
                        App.unblockUI('#modal-generica .modal-dialog');
                    },
                    success: function(result){

                        var qtdMensagens = result.quantidade_mensagens;
                        if(!qtdMensagens){
                            qtdMensagens = 'Plano ilimitado';
                        }

                        $('.info-pacote').show();
                        $('.info-pacote .valor').html(formataValor(result.valor));
                        $('.info-pacote .quantidade').html(qtdMensagens);
                    },
                    error: function (result) {
                        exibirAlert('Não foi possível obter as informações do plano');
                    }
                });

            }else{
                $('#btn-salvar-modal').hide();
                $('#btn-salvar-modal').unbind( "click" );
            }
        });

    });
</script>