<?php
$p = getParam();
UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);
$arWhere = array("con.estabelecimento_id = {$p['id']}");
$mConvenio = new ConvenioEstabelecimento();

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'excluir' :
            $arParams = getParam();

            $mConvenio->excluirLogicamente($arParams['convenio_id']);
            $mConvenio->commit();

            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])), 'Convênio excluído com sucesso');
            break;
        case 'pesquisa_simples' :
            $pesquisaAvancada = false;

            if(!empty($_REQUEST['pesquisa_simples'])){
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));
                $arWhere[] = "(
                    (con.nome ILIKE '%{$pesquisa_simples}%') OR
                    (con.cnpj ILIKE '%{$pesquisa_simples}%') OR
                    (con.codigo_operadora ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
        case 'pesquisa_avancada' :
            $pesquisaAvancada = true;

            if(!empty($_REQUEST['nome'])){
                $nome = trim(addslashes($_REQUEST['nome']));
                $arWhere[] = "con.nome ILIKE '%{$nome}%' ";
            }

            if(!empty($_REQUEST['cnpj'])){
                $cnpj = trim(addslashes($_REQUEST['cnpj']));
                $arWhere[] = "con.cnpj ILIKE '%{$cnpj}%' ";
            }

            if(!empty($_REQUEST['codigo_operadora'])){
                $codigo_operadora = trim(addslashes($_REQUEST['codigo_operadora']));
                $arWhere[] = "con.codigo_operadora ILIKE '%{$codigo_operadora}%' ";
            }
            break;
        case 'subir' :
        case 'descer' :
            if($_REQUEST['act'] === 'subir'){
                $mConvenio->subir($p['convenio_id']);
                $mConvenio->commit();
            }else if($_REQUEST['act'] === 'descer'){
                $mConvenio->descer($p['convenio_id']);
                $mConvenio->commit();
            }

            desenharTabelaConvenios();
            die;
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true);

?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-building-o font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Convênios/Planos do estabelecimento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="estabelecimento-form" method="POST">
                <div class="form-body">
                    <div class="pesquisa-simples <?= ($pesquisaAvancada ? 'hidden' : '') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por nome, CNPJ ou Código na operadora'); ?>
                                        <span class="input-group-btn">
                                            <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                                <i class="fa fa-search"></i>
                                            </button>
                                        </span>
                                        <span class="input-group-btn" data-toggle="tooltip" title="Pesquisa avançada">
                                            <button class="btn green-jungle btn-pesquisa-avancada" type="button">
                                                <i class="fa fa-search-plus"></i>
                                            </button>
                                        </span>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                        <div class="row">
                            <div class="col-md-4">
                                <div class="form-group">
                                    <label>Nome</label>
                                    <?= campo_textom('nome', 'N', 'S', '', 190, 190, '', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-4">
                                <div class="form-group">
                                    <label>CNPJ</label>
                                    <?= campo_textom('cnpj', 'N', 'S', '', 19, 19, '99.999.999/9999-99', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-4">
                                <div class="form-group">
                                    <label>Código na operadora</label>
                                    <?= campo_textom('codigo_operadora', 'N', 'S', '', 19, 19, '', '', '', '', 0); ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="form-actions">
                    <div class="row">
                        <div class="col-xs-7 col-sm-5">
                            <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                                <button type="submit" class="btn blue">
                                    <i class="fa fa-search"></i> Pesquisar
                                </button>
                                <button type="button" class="btn green-jungle btn-pesquisa-simples">
                                    <i class="fa fa-search-minus"></i> Pesquisa simples
                                </button>
                            </div>
                        </div>
                        <div class="col-xs-5 col-sm-7 text-right">
                            <a class="btn green" href="?modulo=principal/estabelecimento/formularioConvenio&acao=C<?= setParam(array('id' => $p['id'])) ?>">
                                <i class="fa fa-plus-circle"></i>
                                Cadastrar novo convênio/plano
                            </a>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="<?= ($pesquisaAvancada ? 'pesquisa_avancada' : 'pesquisa_simples') ?>"/>
            </form>
        </div>
    </div>

    <div class="tabela-convenios">
        <?php desenharTabelaConvenios(); ?>
    </div>
</div>

<form method="post" class="form-excluir">
    <input type="hidden" name="act" value="excluir" />
    <input type="hidden" name="p" />
</form>

<div class="modal fade in" id="modal-excluir" tabindex="-1" role="modal-excluir" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Excluir convênio/plano</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja excluir o convênio/plano <strong id="nome"></strong>?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="excluirRegistro();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">Não</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<script type="text/javascript">
    $(function () {
        $('.btn-limpar').click(function () {
            window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] . setParam(array('id' => $p['id'])) ?>';
        });
    });

    function exibirModalExclusao(p, nome){
        var modal = $('#modal-excluir');

        $('form.form-excluir [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.modal('show');
    }

    function excluirRegistro(){
        $('form.form-excluir').submit();
    }

    function subir(p){
        ordenar(p, 'subir');
    }

    function descer(p){
        ordenar(p, 'descer');
    }

    function ordenar(p, tipo){
        App.blockUI({
            message: 'Carregando...',
            target: '.tabela-convenios'
        });

        $('.tabela-convenios').load(window.location.href, {
            act: tipo,
            p: p,
        }, function () {
            App.unblockUI('.tabela-convenios');
        });
    }
</script>

<?php
function desenharTabelaConvenios(){
    global $mConvenio, $arWhere;

    $acao = <<<ACAO
            <div style="min-width: 100px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/formularioConvenio&acao=C&p=<php> return setParam(array('id' => '{estabelecimento_id}', 'convenio_id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar convênio/plano">
                    <i class="icon-pencil"></i>
                </a>
                <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao('<php> return setParam(array('id' => '{estabelecimento_id}', 'convenio_id' => '{id}'), false); </php>', '{nome}');" data-toggle="tooltip"  title="Excluir convênio/plano">
                    <i class="icon-trash"></i>
                </a>
            </div>
ACAO;

    $arConfig = array();
    $arConfig[1]['html'] = <<<HTML
    <a href="?modulo=principal/estabelecimento/formularioConvenio&acao=C&p=<php> return setParam(array('id' => '{estabelecimento_id}', 'convenio_id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar convênio/plano">
        {nome}
    </a>
HTML;
    $arConfig[0]['style'] = "text-align: center;";
    $arConfig[1]['style'] = "width: 50%;";
    $arConfig[2]['style'] = "width: 10%; text-align: center;";
    $arConfig[6]['html'] = <<<HTML
        <div style="min-width: 100px; text-align: center;">
            <php>
                if('{ordem}' == '{ordem_min}') {
                    return '<a class="btn btn-circle btn-icon-only btn-default grey" style="margin-right: 0; color: #fff">
                                <i class="fa fa-arrow-circle-up"></i>
                            </a>';
                } else {
                    return '<a class="btn btn-circle btn-icon-only btn-default green-haze" style="margin-right: 0;" href="javascript:subir(\'' . setParam(array('id' => '{estabelecimento_id}', 'convenio_id' => '{id}'), false) . '\');" data-toggle="tooltip"  title="Subir">
                                <i class="fa fa-arrow-circle-up"></i>
                            </a>';
                }
            </php>
            
            <php>
                if('{ordem}' == '{ordem_max}') {
                    return '<a class="btn btn-circle btn-icon-only btn-default grey" style="margin-right: 0; color: #fff">
                                <i class="fa fa-arrow-circle-down"></i>
                            </a>';
                } else {
                    return '<a class="btn btn-circle btn-icon-only btn-default yellow-mint" style="margin-right: 0;" href="javascript:descer(\'' . setParam(array('id' => '{estabelecimento_id}', 'convenio_id' => '{id}'), false) . '\');" data-toggle="tooltip"  title="Descer">
                                <i class="fa fa-arrow-circle-down"></i>
                            </a>';
                }
            </php>
        </div>
HTML;

    $rs = $mConvenio->getList($arWhere, false);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho( array(
        'Código',
        'Nome',
        'CNPJ',
        'Código na operadora',
        'Telefone principal',
        'Telefone secundário',
        'Email',
        'Ordem',
        'Ação'
    ));
    $mLista->setCorpo( $rs, $arConfig );
    $mLista->setAcao( $acao );
    $mLista->show();
}