<?php
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$mServicoAnaliseContratado = new ServicoAnaliseContratado();
$params = getParam();
$filtroPadrao = true;
$atalho_pesquisa = $_REQUEST['atalho_pesquisa'];
if (!$atalho_pesquisa)
    $atalho_pesquisa = 'todos_futuros';
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'participar':
            $params = array_merge($_REQUEST, getParam());
            $params['edital_id'] = $params['id'];
            $mEditalCliente->salvarEditalCliente($params);
            $mEditalCliente->commit();
            $db->sucessom($_REQUEST['modulo'], '&p=' . $params['id'], 'Participação Vinculada com Sucesso!');
            break;
        case 'pesquisa_simples':
            $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));
            $arWhereEditais[] = "(
                        (edi.objeto ILIKE '%{$pesquisa_simples}%') OR
                        (edi.orgao ILIKE '%{$pesquisa_simples}%') OR
                        (TO_CHAR(edi.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                    )";

            break;
        case 'pesquisa_meus_editais':
            $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));
            $esdid = $_REQUEST['esdid'];
            if ($esdid) $arWhereEditaisCliente[] = " esd.esdid = {$esdid} ";
            $arWhereEditaisCliente[] = "(
                            (edi.objeto ILIKE '%{$pesquisa_simples}%') OR
                            (edi.orgao ILIKE '%{$pesquisa_simples}%') OR
                            (TO_CHAR(edi.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                        )";
            $filtroPadrao = false;

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
getStrMsgDanger();
getStrMsgInfo();
getStrMsgWarning();
?>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Painel de Pregões da <?= $dados['nome_fantasia']; ?>
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
            <div class="col-md-12">
                <h4>Meus Editais</h4>
                <form role="form" name="formulario" id="edital-cliente-form" method="POST" enctype="multipart/form-data">
                    <input type="hidden" name="act" value="pesquisa_meus_editais">
                    <div class="form-group">
                        <div class="input-group">
                            <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por objeto, data ou órgão'); ?>
                            <span class="input-group-btn">
                                <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                    <i class="fa fa-search"></i>
                                </button>
                            </span>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Situação Atual</label>
                                <?php
                                $esdid_arr = $mEditalCliente->retornaListaStatusWF();
                                $db->monta_combom("esdid", $esdid_arr, 'S', '-Todas-', '', '', '', '', 'N', 'esdid');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <br/>
                            <?php
                            $opcoes = array(
                                array('codigo' => 'proximos_7', 'descricao' => 'Próxima Semana'),
                                array('codigo' => 'todos_futuros', 'descricao' => 'Próximos'),
                                array('codigo' => 'monitorados', 'descricao' => 'Monitorados'),
                                array('codigo' => 'contatados', 'descricao' => 'Contratados'),
                                array('codigo' => 'todos', 'descricao' => 'Todos'),
                            );
                            echo $db->monta_radiom('atalho_pesquisa', $opcoes, 'S', 'S', '');
                            ?>
                        </div>
                    </div>
                </form>
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
                    <a href="?modulo=principal/editaisclientes/selecionado&acao=C&p=' . <php> return setParam(array('id' => '{id}'), false); </php> . '" data-toggle="tooltip" title="Serviços do Edital">
                        {orgao}
                    </a>
                    HTML;
                $arConfig[3]['html'] = <<<HTML
                        <spam class="badge badge-primary"><b>{dias}</b></spam>
                    HTML;
                $arConfig[3]['style'] = "width: 10%; text-align: center;";
                /* Filtro do Período (padrão próximos 7 dias e não descartados)*/
                if ($filtroPadrao) {
                    $arWhereEditaisCliente[] = " edi.data_limite >= current_date";
                    $arWhereEditaisCliente[] = " (esd.esdid IS NULL OR esd.esdid NOT IN (95)) ";
                }
                $arWhereEditaisCliente[] = " estabelecimento_id = {$dados['id']}";
                $rs = $mEditalCliente->getList($arWhereEditaisCliente);
                if ($ordernar) {
                    $rs = $mEditalCliente->getList($arWhereEditaisCliente, true, $ordernar);
                }

                $mEditalCliente = new Listam(array("style" => "width: 100%;"));
                $mEditalCliente->setCabecalho(array(
                    'Categoria',
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
                unset($arConfig, $acao);
                $arWhereMinhasSolicitacoes[] = " estabelecimento_id = {$dados['id']}";
                $arConfig[0]['html'] = <<<HTML
                        <spam><a href="?modulo=principal/editaisclientes/selecionado&acao=C&p=' . <php> return setParam(array('id' => '{id}'), false); </php> . '" data-toggle="tooltip" title="Serviços do Edital">
                            {edital}
                        </a></spam>

HTML;
                $rs = $mServicoAnaliseContratado->getList($arWhereMinhasSolicitacoes);
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
                $arWhereEditais[] = "edi.data_limite >= NOW() AND edi.id not in (SELECT edital_id FROM edital.editalcliente WHERE estabelecimento_id = {$dados['id']} )";
                $arWhereEditais[] = "edi.categoria IN ( SELECT categoria FROM estabelecimentosaude.tb_estabelecimento_categoria WHERE estabelecimento_id = {$dados['id']} )";
                $rs = $mEdital->getList($arWhereEditais);
                $mEdital = new Listam(array("style" => "width: 100%;"));
                $mEdital->setCabecalho(array(
                    'Categoria',
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
    $("#esdid").change(function() {
        $('#edital-cliente-form').submit();
    });


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
        window.location = '?modulo=principal/editaisclientes/selecionado&acao=C&p=' + id;
    }

    function participarEdital() {
        $('form.form-participar').submit();
    }

    function solicitarServico(id) {
        window.location = '?modulo=principal/editaisclientes/servicos_pregao&acao=C&p=' + id;
    }
</script>