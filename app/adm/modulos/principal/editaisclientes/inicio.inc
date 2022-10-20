<?php
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$mServicoAnaliseContratado = new ServicoAnaliseContratado();

$params = getParam();

// if(!isset($params['id'])){
//     $params['id'] = $_SESSION['id_empresa'];
// } else {
//     $_SESSION['id_empresa'] = $params['id'];
// }

// ver($_SESSION['id_empresa']);

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'participar':
            $params = array_merge($_POST, getParam());
            $params['edital_id'] = $params['id'];
            $mEditalCliente->salvarEditalCliente($params);
            $mEditalCliente->commit();
            $db->sucessom($_REQUEST['modulo'], '&p=' . $params['id'], 'Participação Vinculada com Sucesso!');
            break;
        case 'pesquisa_simples':
            $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
            $arWhereEditais[] = "(
                        (edi.objeto ILIKE '%{$pesquisa_simples}%') OR
                        (edi.orgao ILIKE '%{$pesquisa_simples}%') OR
                        (TO_CHAR(edi.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                    )";

            break;
        case 'pesquisa_simples_cliente':
            $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
            $arWhereEditaisCliente[] = "(
                            (edi.objeto ILIKE '%{$pesquisa_simples}%') OR
                            (edi.orgao ILIKE '%{$pesquisa_simples}%') OR
                            (TO_CHAR(edi.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                        )";

            break;
        case 'carregarDetalhedital':

            require APPRAIZ . "adm/modulos/principal/editaisclientes/detalhe_edital.inc";
            die;

            break;
    }
}

$aba = false;

$enderecos = null;
if (isset($params['id']) && !empty($params['id'])) {
    UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($params['id']);

    $aba = true;

    $dados = $mEstabelecimento->getTodosDados($params['id']);

    extract($dados);
}

include APPRAIZ . "includes/cabecalhom.inc";

if ($aba) {
    echo Estabelecimento::gerarCabecalho();
}
?>
<br />
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Painel <?= $dados['nome_fantasia']; ?>
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="form" method="POST" enctype="multipart/form-data">
            <div class="row">
                <div class="col-md-4">

                    <h4>Minhas solicitações de Serviço</h4>
                    <form role="form" name="formulario" class="edital-cliente-form" method="POST" enctype="multipart/form-data">
                        <div class="form-group">
                            <div class="input-group">
                                <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por objeto, data ou órgão'); ?>
                                <span class="input-group-btn">
                                    <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                        <i class="fa fa-search"></i>
                                    </button>
                                </span>
                                <input type="hidden" name="act" value="pesquisa_simples_cliente">

                            </div>
                        </div>
                    </form>

                    <?php

                    $arConfig = array();

                    $arWhereEditaisCliente[] = " estabelecimento_id = {$dados['id']}";
                    $rs = $mServicoAnaliseContratado->getList($arWhereEditaisCliente);

                    $mServicoAnaliseContratado = new Listam(array("style" => "width: 100%;"));
                    $mServicoAnaliseContratado->setCabecalho(array(
                        'Edital',
                        'Situação'
                    ));
                    $mServicoAnaliseContratado->setCorpo($rs, $arConfig);
                    $mServicoAnaliseContratado->setAcao($acao);
                    $mServicoAnaliseContratado->show();
                    ?>

                </div>
                <div class="col-md-8">
                    <h4>Meus Editais</h4>

                    <form role="form" name="formulario" class="edital-cliente-form" method="POST" enctype="multipart/form-data">
                        <div class="form-group">
                            <div class="input-group">
                                <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por objeto, data ou órgão'); ?>
                                <span class="input-group-btn">
                                    <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                        <i class="fa fa-search"></i>
                                    </button>
                                </span>
                                <input type="hidden" name="act" value="pesquisa_simples_cliente">
                            </div>
                        </div>
                        <div class="form-group">
                            <button class="btn blue" type="button" data-toggle="tooltip" title=" Já realizados">
                                <i class="icon-arrow-left"></i>
                                Já realizados
                            </button>
                            <button class="btn blue" type="button" data-toggle="tooltip" title="Próximos 7 Dias">
                                <i class="icon-arrow-right"></i>
                                Próximos 7 Dias
                            </button>
                            <button class="btn blue" type="button" data-toggle="tooltip" title="Todos os próximos">
                                <i class="icon-arrow-right"></i>
                                Todos os próximos
                            </button>
                            <button class="btn blue" type="button" data-toggle="tooltip" title="Descartados">
                                <i class="icon-trash"></i>
                                Descartados
                            </button>
                        </div>
                    </form>
                    <?php
                    $acao = <<<ACAO
            <div style="min-width: 20px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:exibirModalDetalhe('<php> return setParam(array('id' => '{id}'), false); </php>', '{orgao}');" data-toggle="tooltip"  title="Detalhes do Edital">
                    <i class="glyphicon glyphicon-search"></i>
                </a>
                <a class="btn btn-circle btn-icon-only btn-default yellow" href="javascript:solicitarServico('<php> return setParam(array('id' => '{id}'), false); </php>', '{orgao}');" data-toggle="tooltip"  title="Solicitar um Serviço">
                    <i class="fa fa-star"></i>
                </a>    
            </div>
ACAO;
                    $arConfig = array();
                    $arConfig[2]['html'] = <<<HTML
    <spam class="badge badge-primary"><b>{dias}</b></spam>
HTML;

                    $arConfig[2]['style'] = "width: 10%; text-align: center;";
                    /* Filtro do Período (padrão próximos 7 dias e não descartados)*/
                    $periodo = '';
                    if(!$periodo){
                        $arWhereEditaisCliente[] = " edi.data_limite between current_date and current_date + 8 ";
                        $arWhereEditaisCliente[] = " (esd.esdid IS NULL OR esd.esdid NOT IN (95)) ";
                    }
                    $arWhereEditaisCliente[] = " estabelecimento_id = {$dados['id']}";
                    $rs = $mEditalCliente->getList($arWhereEditaisCliente);

                    $mEditalCliente = new Listam(array("style" => "width: 100%;"));
                    $mEditalCliente->setCabecalho(array(
                        'Órgão Licitante',
                        'Data do pregão',
                        'Dias restantes',
                        'Objeto',
                        'Local',
                        'Situação',
                        'Ação'
                    ));
                    $mEditalCliente->setCorpo($rs, $arConfig);
                    $mEditalCliente->setAcao($acao);
                    $mEditalCliente->show();
                    ?>
                </div>
            </div>
            <div class="row">
                <div class="col-md-12">
                    <h4>Todos os Editas</h4>
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
                    $acao = <<<ACAO
            <div style="min-width: 20px;">
                <a class="btn btn-circle btn-icon-only btn-default yellow" href="javascript:exibirModalParticipar('<php> return setParam(array('id' => '{id}'), false); </php>', '{orgao}');" data-toggle="tooltip"  title="Quero Participar!">
                    <i class="glyphicon glyphicon-star-empty"></i>
                </a>
                
                <a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:exibirModalDetalhe('<php> return setParam(array('id' => '{id}'), false); </php>', '{orgao}');" data-toggle="tooltip"  title="Detalhes do Edital">
                    <i class="glyphicon glyphicon-search"></i>
                </a>
            </div>
ACAO;

                    $arConfig = array();

                    $arConfig[3]['html'] = <<<HTML
    <spam class="badge badge-primary"><b>{dias}</b></spam>
HTML;


                    $arConfig[3]['style'] = "width: 10%; text-align: center;";

                    // Filtro para não trazer as que já estão vinculadas
                    $arWhereEditais[] = "edi.data_limite >= NOW() AND edi.id not in (SELECT edital_id FROM edital.editalcliente WHERE estabelecimento_id = {$dados['id']} )";

                    $rs = $mEdital->getList($arWhereEditais);

                    $mEdital = new Listam(array("style" => "width: 100%;"));
                    $mEdital->setCabecalho(array(
                        'Portal',
                        'Órgão Licitante',
                        'Data do pregão',
                        'Dias restantes',
                        'Objeto',
                        'Local',
                        'Ação'
                    ));
                    $mEdital->setCorpo($rs, $arConfig);
                    $mEdital->setAcao($acao);
                    $mEdital->show();
                    ?>
                </div>

            </div>

            <div class="form-actions">
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
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
                <h4 class="modal-title">Quero participar do edital</h4>
            </div>
            <div class="modal-body">
                Confirma a participação no Edital <strong id="nome"></strong>?
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
    $(function() {
        $('.btn-voltar').click(function() {
            history.go(-1);
        });
    });

    function exibirModalParticipar(p, nome) {
        var modal = $('#modal-participar');

        $('form.form-participar [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.modal('show');
    }

    function exibirModalDetalhe(p, nome) {
        var modal = $('#modal-detalhe-edital');
        modal.find('strong#nome').html(nome);
        App.blockUI({
            message: 'Carregando...',
            target: '#div_detalhe_edital'
        });
        // Carregar os dados do Edital
        $('div#div_detalhe_edital').load(window.location.href, {
            act: 'carregarDetalhedital',
            p: p
        }, function() {
            App.unblockUI('#div_detalhe_edital');
        });
        //modal.find('#div_detalhe_edital').html();
        modal.modal('show');
    }

    function participarEdital() {
        $('form.form-participar').submit();
    }

    function solicitarServico(id) {
        window.location = '?modulo=principal/editaisclientes/servicos_pregao&acao=C&p=' + id;
    }
</script>