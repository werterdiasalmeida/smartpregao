<?php
$mEstabelecimento = new Estabelecimento();
$mCotacaoDispensa = new CotacaoDispensa();
$mCDCCliente = new CDCCliente();
$params = getParam();
$filtroPadrao = true;
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'participar':
            $params = array_merge($_REQUEST, getParam());
            $params['edital_id'] = $params['id'];
            $mCDCCliente->salvarEditalCliente($params);
            $mCDCCliente->commit();
            $db->sucessom($_REQUEST['modulo'], '&p=' . $params['id'], 'Participação Vinculada com Sucesso!');
            break;
        case 'pesquisa_meus_registros':
            $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));
            $arWhereEditaisCliente[] = "(
                            (edi.objeto ILIKE '%{$pesquisa_simples}%') OR
                            (edi.orgao ILIKE '%{$pesquisa_simples}%') OR
                            (TO_CHAR(edi.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                        )";

            break;
        case 'pesquisa_oportunidades':
            $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
            $arWhereEditais[] = "(
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
$dadoBignumber = $mCDCCliente->retornaPainelCliente($params['id']);
include APPRAIZ . "includes/cabecalhom.inc";
getStrMsgDanger();
getStrMsgInfo();
getStrMsgWarning();
if ($aba) {
    echo Estabelecimento::gerarCabecalho();
}
?>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Painel de Cotação | Dispensa eletrônica | Credenciamento da <?= $dados['nome_fantasia']; ?>
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
                        <i class="fa fa-money"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup" data-value="<?php echo intval($dadoBignumber['cotacoes_recebidas']) ?>"><?php echo intval($dadoBignumber['cotacoes_recebidas']) ?></span>
                        </div>
                        <div class="desc"><b>Cotações Recebidas</b></div>
                        <div class="desc"> Enviadas: <?php echo intval($dadoBignumber['cotacoes_enviadas']) ?></div>
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
                        <div class="desc"> <b>Dispensas eletrônicas</b></div>
                        <div class="desc"> Enviadas: <?php echo intval($dadoBignumber['dispensas_enviadas']) ?></div>
                    </div>
                </a>
            </div>
            <div class="col-lg-4 col-md-4 col-sm-4 col-xs-12" style="margin-top: 10px;">
                <a class="dashboard-stat dashboard-stat-v2 blue">
                    <div class="visual">
                        <i class="fa fa-check"></i>
                    </div>
                    <div class="details">
                        <div class="number">
                            <span data-counter="counterup" data-value="<?php echo intval($dadoBignumber['credenciamentos_cadastrados']) ?>"><?php echo intval($dadoBignumber['credenciamentos_cadastrados']) ?></span>
                        </div>
                        <div class="desc"> <b>Credenciamentos Realizados</b></div>
                        <div class="desc"> Pendentes de resposta: <?php echo intval($dadoBignumber['credenciamentos_pendentes']) ?></div>
                    </div>
                </a>
            </div>
        </div>
        <form role="form" name="formulario" class="form" method="POST" enctype="multipart/form-data">
            <div class="row">
                <div class="col-md-12">
                    <br />
                    <form role="form" name="formulario" class="edital-cliente-form" method="POST" enctype="multipart/form-data">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por objeto, data ou órgão'); ?>
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
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Situação Atual</label>
                                    <?php
                                    $esdid_arr = $mCDCCliente->retornaListaStatusWF();
                                    $db->monta_combom("esdid", $esdid_arr, 'S', '-Todas-', '', '', '', '', 'N');
                                    ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Tipo de Oportunidade</label>
                                    <?
                                    $opcoes = array(
                                        "Todas" => array("valor" => '', 'default' => true,),
                                        "Cotação" => array("valor" => 'Cotação'),
                                        "Dispensa eletrônica" => array("valor" => 'Dispensa eletrônica'),
                                        "Credenciamento" => array("valor" => 'Credenciamento')
                                    );
                                    echo campo_radiom('tipo', $opcoes, 'S'); ?>
                                </div>
                            </div>
                        </div>
                    </form>
                    <a style="float: right;" onclick="cadastraroportunidade('<?= $dados['id'] ?>')"><i style="cursor: pointer;" title="Cadastrar oportunidade" class="bi bi-plus-square"></i>
                        Cadastrar oportunidade</a>
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
                    $arConfig[1]['html'] = <<<HTML
                    <a href="?modulo=principal/cdcclientes/selecionado&acao=C&p=' . <php> return setParam(array('id' => '{id}'), false); </php> . '" data-toggle="tooltip" title="Serviços do Edital">
                        {orgao}
                    </a>
                    HTML;
                    $arConfig[3]['html'] = <<<HTML
                        <spam class="badge badge-primary"><b>{dias}</b></spam>
                    HTML;
                    $arConfig[3]['style'] = "width: 10%; text-align: center;";
                    $arWhereEditaisCliente[] = " estabelecimento_id = {$dados['id']}";
                    $rs = $mCDCCliente->getList($arWhereEditaisCliente);
                    if ($ordernar) {
                        $rs = $mCDCCliente->getList($arWhereEditaisCliente, true, $ordernar);
                    }
                    $mCDCCliente = new Listam(array("style" => "width: 100%;"));
                    $mCDCCliente->setCabecalho(array(
                        'Tipo',
                        'Órgão',
                        'Data Limite',
                        'Dias restantes',
                        'Objeto',
                        'Local',
                        'Situação',
                        'Ação'
                    ));
                    $mCDCCliente->setCorpo($rs, $arConfig);
                    $mCDCCliente->setAcao($acao);
                    $mCDCCliente->show();
                    ?>
                </div>
            </div>
            <div class="row">
                <div class="col-md-12">
                    <h4>Todas as Oportunidades</h4>
                    <form role="form" name="formulario" class="editais-form" method="POST" enctype="multipart/form-data">
                        <div class="form-group">
                            <div class="input-group">
                                <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por objeto, data ou órgão'); ?>
                                <span class="input-group-btn">
                                    <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                        <i class="fa fa-search"></i>
                                    </button>
                                </span>
                                <input type="hidden" name="act" value="pesquisa_simples">

                            </div>
                        </div>
                    </form>

                    <?php
                    unset($arConfig, $acao);
                    $acao = <<<ACAO
                    <div style="min-width: 20px;">
                        <a class="btn btn-circle btn-icon-only btn-default yellow" href="javascript:exibirModalParticipar('<php> return setParam(array('id' => '{id}'), false); </php>', '{orgao}');" data-toggle="tooltip"  title="Quero Participar!">
                            <i class="glyphicon glyphicon-star-empty"></i>
                        </a>
                    </div>
                    ACAO;
                    $arConfig[4]['html'] = <<<HTML
                        <spam class="badge badge-primary"><b>{dias}</b></spam>
                    HTML;
                    $arConfig[4]['style'] = "width: 10%; text-align: center;";
                    // Filtro para não trazer as que já estão vinculadas
                    $arWhereEditais[] = "edi.data_limite >= NOW() AND edi.id not in (SELECT cdc_id FROM edital.cotacao_dispensa_credenciamentoempresa WHERE estabelecimento_id = {$dados['id']} )";
                    $arWhereEditais[] = "edi.categoria IN ( SELECT categoria FROM estabelecimentosaude.tb_estabelecimento_categoria WHERE estabelecimento_id = {$dados['id']} )";
                    $rs = $mCotacaoDispensa->getList($arWhereEditais);
                    $mCotacaoDispensa = new Listam(array("style" => "width: 100%;"));
                    $mCotacaoDispensa->setCabecalho(array(
                        'Tipo',
                        'Portal',
                        'Órgão',
                        'Data Limite',
                        'Dias restantes',
                        'Objeto',
                        'Local',
                        'Ação'
                    ));
                    $mCotacaoDispensa->setCorpo($rs, $arConfig);
                    $mCotacaoDispensa->setAcao($acao);
                    $mCotacaoDispensa->show();
                    ?>
                </div>
            </div>
            <input type="hidden" name="act" value="salvar" />
        </form>
    </div>
</div>
<div class="modal fade in" id="modal-participar" tabindex="-1" role="modal-participar" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Quero participar da oportunidade</h4>
            </div>
            <div class="modal-body">
                Confirma a participação na Oportunidade <strong id="nome"></strong>?
                <form role="form" name="formulario" class="form-participar" method="POST" enctype="multipart/form-data">
                    <input type="hidden" name="act" value="participar">
                    <input type="hidden" name="p" value="">
                    <input type="hidden" name="estabelecimento_id" value="<?= $dados['id'] ?>">
                </form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="participarEdital();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">Não</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>
<div class="modal fade in" id="modal-detalhe-edital" tabindex="-1" role="modal-detalhe-edital" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Detalhes do Edital</h4>
            </div>
            <div class="modal-body">
                Órgão Licitante: <strong id="nome"></strong>
                <div id="div_detalhe_edital"></div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn blue" data-dismiss="modal">Fechar</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
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