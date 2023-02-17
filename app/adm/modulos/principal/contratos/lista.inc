<?php
$mEstabelecimento = new Estabelecimento();
$mEditalClienteContrato = new EditalClienteContrato();
$params = getParam();
$filtroPadrao = true;
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'pesquisa_meus_registros':
            $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));
            $arWhereEditaisCliente[] = "(
                            (edi.objeto ILIKE '%{$pesquisa_simples}%') OR
                            (edi.orgao ILIKE '%{$pesquisa_simples}%') OR
                            (TO_CHAR(edi.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                        )";

            break;

        case 'carregarDetalhedital':
            require APPRAIZ . "adm/modulos/principal/cdcclientes/detalhe_edital.inc";
            die;
            break;
    }
}
$aba = false;
$enderecos = null;
if (isset($params['id']) && !empty($params['id'])) {
    UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($params['id']);
    //$aba = true;
    $dados = $mEstabelecimento->getTodosDados($params['id']);
    extract($dados);
}
$dadoBignumber = $mEditalClienteContrato->retornaPainelCliente($params['id']);
include APPRAIZ . "includes/cabecalhom.inc";
getStrMsgDanger();
getStrMsgInfo();
getStrMsgWarning();
?>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Painel de Contratos da <?= $dados['nome_fantasia']; ?>
            </span>
        </div>
        <?php
        if (isset($params['id']) && !empty($params['id'])) {
            echo $mEstabelecimento->retornaAtalhosCliente($params['id']);
        } else {
            echo "Cliente não encontrado.";
        }
        ?>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12 " style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 purple">
                    <div class="visual">
                        <i class="fa fa-user"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup" data-value="<?php echo intval($dadoBignumber['cotacoes_recebidas']) ?>"><?php echo intval($dadoBignumber['cotacoes_recebidas']) ?></span>
                        </div>
                        <div class="desc"><b>Contratos Assinados</b></div>
                    </div>
                </a>
            </div>
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 green">
                    <div class="visual">
                        <i class="fa fa-cloud"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup" data-value="<?php echo intval($dadoBignumber['dispensas_recebidas']) ?>"><?php echo intval($dadoBignumber['dispensas_recebidas']) ?></span>
                        </div>
                        <div class="desc"> <b>Pontos de Atenção (contratos)</b></div>
                    </div>
                </a>
            </div>
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 blue">
                    <div class="visual">
                        <i class="fa fa-money"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup" data-value="<?php echo intval($dadoBignumber['credenciamentos_cadastrados']) ?>"><?php echo intval($dadoBignumber['credenciamentos_cadastrados']) ?></span>
                        </div>
                        <div class="desc"> <b>Pontos de Atenção (financeiro)</b></div>
                    </div>
                </a>
            </div>
        </div>
        <div class="row">
            <div class="col-md-12">
                <br />
                <form role="form" name="formulario" class="edital-cliente-form" method="POST" enctype="multipart/form-data">
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <div class="input-group">
                                    <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por objeto, data, órgão ou empresa'); ?>
                                    <span class="input-group-btn">
                                        <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                            <i class="fa fa-search"></i>
                                        </button>
                                    </span>
                                    <input type="hidden" name="act" value="pesquisa_meus_editais">
                                </div>
                            </div>
                        </div>
                    </div>
                </form>
                <a style="float: right;" onclick="cadastraroportunidade('<?= $dados['id'] ?>')"><i style="cursor: pointer;" title="Cadastrar oportunidade" class="bi bi-plus-square"></i>
                    Cadastrar contrato</a>
                <br />
                <?php
                unset($arConfig, $acao);
                $acao = <<<ACAO
                                <div style="min-width: 20px;">
                                    <a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:selecionarEdital('<php> return setParam(array('id' => '{id}'), false); </php>');" data-toggle="tooltip"  title="Detalhes do Edital">
                                        <i class="fa fa-edit"></i> 
                                </a>
                                </div>
                    ACAO;
                $arConfig = array();
                $arConfig[0]['html'] = <<<HTML
                    <a href="?modulo=principal/contratos/formulario&acao=C&p=' . <php> return setParam(array('id' => '{id}'), false); </php> . '" data-toggle="tooltip" title="Abrir contrato">
                        {empresa}
                    </a>
                    HTML;
                $arWhereEditaisCliente[] = "ecc. estabelecimento_id = {$dados['id']}";
                $rs = $mEditalClienteContrato->getList($arWhereEditaisCliente);
                if ($ordernar) {
                    $rs = $mEditalClienteContrato->getList($arWhereEditaisCliente, true, $ordernar);
                }
                $mEditalClienteContrato = new Listam(array("style" => "width: 100%;"));
                $mEditalClienteContrato->setCabecalho(array(
                    'Contratante',
                    'Número',
                    'Início',
                    'Término',
                    'Valor total (R$)',
                    'Situação',
                    'Ação'
                ));
                $mEditalClienteContrato->setCorpo($rs, $arConfig);
                $mEditalClienteContrato->setAcao($acao);
                $mEditalClienteContrato->show();
                ?>
            </div>
        </div>
    </div>
</div>


<script type="text/javascript">
    function filtrarMeusEditais(tipo) {

        location.href = location.href + '&act=pesquisa_meus_editais&filtrar=' + tipo;

    }

    function exibirModalParticipar(p, nome) {
        var modal = $('#modal-participar');

        $('form.form-participar [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.modal('show');
    }

    function selecionarEdital(id) {
        window.location = '?modulo=principal/cdcclientes/selecionado&acao=C&p=' + id;
    }

    function participarEdital() {
        $('form.form-participar').submit();
    }

    function cadastraroportunidade(id) {
        url = '?modulo=principal/cotacao-dispensa/formulario&acao=C&est_id=' + id;
        window.open(url);
    }
</script>