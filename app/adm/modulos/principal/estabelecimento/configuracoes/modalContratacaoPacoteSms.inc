<?php
$mEstabelecimentoPacoteMensagem = new EstabelecimentoPacoteMensagem();

$mIugu = new SimecIugu();
$urlBoletoIugu = $mIugu->getUrl();

$p = getParam();

$pacotesPendentes = $mEstabelecimentoPacoteMensagem->getPacotesPagamentoPendente($p['id'], 'SMS');
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
        <? foreach ($pacotesPendentes AS $pacotePendente): ?>
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
                        <button id="btn-gerar-boleto" type="button" class="btn green-jungle" style="margin-top: 10px; width: 150px;" onclick="confirmGerarBoleto('<?= setParam(['id' => $p['id'], 'estabelecimento_pacote_mensagem_id' => $pacotePendente['estabelecimento_pacote_mensagem_id'] ]) ?>');"">Gerar Nova Fatura</button><br>
                    <? else: ?>
                        <button id="btn-gerar-boleto" type="button" class="btn green-jungle" style="margin-top: 10px; width: 150px;" onclick="confirmGerarBoleto('<?= setParam(['id' => $p['id'], 'estabelecimento_pacote_mensagem_id' => $pacotePendente['estabelecimento_pacote_mensagem_id'] ]) ?>');"">Gerar Fatura</button><br>
                    <? endif; ?>

                    <button id="btn-cancelar-boleto" type="button" class="btn red" style="margin-top: 10px; width: 150px;" onclick="confirmCancelarSolicitacaoPacote('<?= setParam(['id' => $p['id'], 'estabelecimento_pacote_mensagem_id' => $pacotePendente['estabelecimento_pacote_mensagem_id'] ]) ?>');">Cancelar Solicitação</button>
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
                            $planosPacoteMensagem = $mPlanoPacoteMensagem->getComboPacotesMensagemByTipo('SMS');
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
                $('#modal-generica #btn-salvar-modal').html('Contratar').show().click(function(){
                    confirmSolicitacaoPacote();
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
                        $('.info-pacote').show();
                        $('.info-pacote .valor').html(formataValor(result.valor));
                        $('.info-pacote .quantidade').html(result.quantidade_mensagens);
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

    function contratarPacote(){
        var plano = $('[name=plano_pacote_mensagem_id]').val();
        if(!plano){
            return false;
        }

        $('#modal-confimacao').modal('hide');
        $('#modal-generica ').modal('hide');

        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType : 'json',
            data: {
                act : 'contratarPacoteSMS',
                plano_pacote_mensagem_id: plano
            },
            beforeSend: function( xhr ) {
                App.blockUI({message: 'Carregando...',target: '.page-container'});
            },
            complete: function( data ) {
                App.unblockUI('.page-container');

                setTimeout(function(){
                    exibeModalRenovacaoPacote();
                },1000);
            },
            success: function(result){
                exibirSucesso('Solicitação de contratação de pacote de mensagens realizada com sucesso!');
            },
            error: function (result) {
                exibirAlert('Não foi possível contratar o pacote!');
            }
        });
    }

    function confirmCancelarSolicitacaoPacote(param){
        //Redefinindo a ação e o texto da modal de confirmação
        $('#modal-confimacao .modal-body').html('Deseja realmente cancelar a solicitação de pacote?');
        $('#modal-confimacao #parametro').val(param);
        $('#modal-confimacao #btn-confirm-modal').show().click(function(){
            cancelarSolicitacaoPacote();
        });

        $('#modal-confimacao').modal('show');
    }

    function confirmSolicitacaoPacote(){
        //Redefinindo a ação e o texto da modal de confirmação
        $('#modal-confimacao .modal-body').html('Deseja realmente contratar esse plano de SMS?');
        $('#modal-confimacao #btn-confirm-modal').show().click(function(){
            contratarPacote();
        });
        $('#modal-confimacao').modal('show');
    }

    function confirmGerarBoleto(param){
        //Redefinindo a ação e o texto da modal de confirmação
        $('#modal-confimacao .modal-body').html('Deseja realmente gerar a fatura?');
        $('#modal-confimacao #parametro').val(param);
        $('#modal-confimacao #btn-confirm-modal').show().click(function(){
            gerarBoleto();
        });

        $('#modal-confimacao').modal('show');
    }

    function gerarBoleto(){
        var param = $('#modal-confimacao #parametro').val();
        if(!param){
            return false;
        }

        $('#modal-generica ').modal('hide');
        $('#modal-confimacao').modal('hide');

        $.ajax({
            type: "POST",
            url: window.location.href + param,
            dataType : 'json',
            data: {
                act : 'gerarFaturaContratacaoPacoteSMS'
            },
            beforeSend: function( xhr ) {
                App.blockUI({message: 'Carregando...',target: '.page-container'});
            },
            complete: function( data ) {
                App.unblockUI('.page-container');
                setTimeout(function(){
                    exibeModalRenovacaoPacote();
                },100);
            },
            success: function(result){
                exibirSucesso('Fatura gerada com sucesso, clique no botão para visualiza-la!');
            },
            error: function (result) {
                exibirAlert('Não foi possível gerar a fatura!');
            }
        });
    }

    function cancelarSolicitacaoPacote(){
        var param = $('#modal-confimacao #parametro').val();
        if(!param){
            exibirAlert('Não foi possível cancelar o pacote (parâmetros necessários não definidos)');
            return false;
        }

        $('#modal-confimacao').modal('hide');
        $('#modal-generica ').modal('hide');

        $.ajax({
            type: "POST",
            url: '?modulo=principal/estabelecimento/configuracoes/lista&acao=C' + param,
            dataType : 'json',
            data: {
                act : 'cancelarSolicitacaoPacote'
            },
            beforeSend: function( xhr ) {
                App.blockUI({message: 'Carregando...',target: '.page-container'});
            },
            complete: function( data ) {
                App.unblockUI('.page-container');
            },
            success: function(result){
                exibirSucesso('Cancelamento da solicitação de pacote efetuado com sucesso!');
            },
            error: function (result) {
                exibirAlert('Não foi possível cancelar o pacote!');
            }
        });
    }
</script>