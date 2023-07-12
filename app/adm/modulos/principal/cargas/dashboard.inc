<?php
$mCargaEstabelecimento = new CargaEstabelecimento();
$dados = $mCargaEstabelecimento->retornaDadosDashboard();
include APPRAIZ . "includes/cabecalhom.inc";
getStrMsgDanger();
getStrMsgInfo();
getStrMsgWarning();
?>
<br />
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-list"></i>
            <span class="caption-subject bold uppercase">
                Dashboard de controle das Cargas Externas
            </span>
        </div>
    </div>
    <div class="row">
        <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12 " style="margin-top: 10px;">
            <a class="dashboard-stat dashboard-stat-v2 purple">
                <div class="visual">
                    <i class="fa fa-money"></i>
                </div>
                <div class="details">
                    <div class="number">
                        <span data-counter="counterup" data-value="<?php echo formata_valor($dados['editais_cadastrados'], 0, false) ?>"><?php echo $dados['editais_cadastrados'] ?></span>
                    </div>
                    <div class="desc"><b>Editais Cadastrados</b></div>
                    <div class="desc"> Atualizado em: <?php echo formata_data($dados['editais_cadastrados_update']) ?></div>
                </div>
            </a>
            <button type="button" class="btn purple" onclick="window.open('?modulo=principal/cargas/editais_internos&acao=C')">
                <i class="bi bi-download"></i> Carga de Editais Internos
            </button>
        </div>
        <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
            <a class="dashboard-stat dashboard-stat-v2 green">
                <div class="visual">
                    <i class="fa fa-cloud"></i>
                </div>
                <div class="details">
                    <div class="number">
                        <span data-counter="counterup" data-value="<?php echo formata_valor($dados['editais_captacao'], 0, false) ?>"><?php echo $dados['editais_captacao'] ?></span>
                    </div>
                    <div class="desc"> <b>Editais para Captação de Clientes</b></div>
                    <div class="desc"> Atualizado em: <?php echo formata_data($dados['editais_captacao_update']) ?></div>
                </div>
            </a>
            <button type="button" class="btn green" onclick="window.open('?modulo=principal/cargas/editais_captacao_clientes&acao=C')">
                <i class="bi bi-download"></i> Carga de Editais (Captação de Clientes)
            </button>
        </div>
        <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
            <a class="dashboard-stat dashboard-stat-v2 blue">
                <div class="visual">
                    <i class="fa fa-user"></i>
                </div>
                <div class="details">
                    <div class="number">
                        <span data-counter="counterup" data-value="<?php echo formata_valor($dados['clientes_captacao'], 0, false) ?>"><?php echo $dados['clientes_captacao'] ?></span>
                    </div>
                    <div class="desc"> <b>Cientes cadastrados na Captação</b></div>
                    <div class="desc"> 
                        Atualizados API: <?php echo formata_valor($dados['clientes_autalizados_api'], 0, false) ?>
                        | Falta: <?php echo formata_valor($dados['clientes_captacao'] - $dados['clientes_autalizados_api'], 0, false) ?>
                </div>
                </div>
            </a>
            <button type="button" class="btn blue" onclick="window.open('?modulo=principal/cargas/clientes_potencial&acao=C')">
                <i class="bi bi-download"></i> Carga de Clientes (Captação de Clientes)
            </button>
        </div>
    </div>
    <div class="row">
        <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12 " style="margin-top: 10px;">
            <a class="dashboard-stat dashboard-stat-v2 red">
                <div class="visual">
                    <i class="fa fa-download"></i>
                </div>
                <div class="details">
                    <div class="number">
                        <span data-counter="counterup" data-value="<?php echo formata_valor($dados['editais_comprasnet'], 0, false) ?>"><?php echo $dados['editais_cadastrados'] ?></span>
                    </div>
                    <div class="desc"><b>Editais ComprasNet Direto</b></div>
                    <div class="desc"> Atualizado em: <?php echo formata_data($dados['editais_comprasnet_update']) ?></div>
                </div>
            </a>
            <button type="button" class="btn red" onclick="window.open('?modulo=principal/cargas/comprasnet&acao=C')">
                <i class="bi bi-download"></i> Carga ComprasNet Direto
            </button>
        </div>
    </div>
</div>