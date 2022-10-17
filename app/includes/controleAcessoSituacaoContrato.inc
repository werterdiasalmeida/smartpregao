<?
/**
 * Para verificar se uma tela precisa estar com o contrato ativo para ter acesso, basta incluir este arquivo.
 */
$mControleAcessoEstabelecimentoContrato = new ControleAcessoEstabelecimentoContrato();
$possuiAcessoTelaContratoAtivo = $mControleAcessoEstabelecimentoContrato->isPossuiAcessoTela();

if (!$possuiAcessoTelaContratoAtivo) :
    ?>
    <div id="page-body" style="width:100%; margin-top: 35px;">
        <div class="portlet light bordered" style="display: table; margin: 0 auto;">
            <div class="portlet-title">
                <div class="caption font-red">
                    <i class="icon-lock font-red"></i>
                    <span class="caption-subject bold uppercase"> Acesso Negado</span>
                </div>
            </div>
            <div class="portlet-body">
                <p>Para acessar essa funcionalidade é necessário que o </br>contrato do <b>Smart Pregão</b> esteja ativo.
                </p>
            </div>
        </div>
    </div>
    <?php
    die;
endif;
