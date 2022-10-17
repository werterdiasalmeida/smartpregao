<?php
$mConsorcio = new Consorcio();
$arWhere = array();

if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
        case 'excluir' :
            $arParams = getParam();
            $mConsorcio->carregarPorId($arParams['id']);

            $mConsorcio->excluir($arParams['id']);
            $mConsorcio->commit();

            $db->sucessom($_REQUEST['modulo'], '', 'Consórcio excluído com sucesso');
            break;
        case 'pesquisa_simples' :
            $pesquisaAvancada = false;

            if(!empty($_REQUEST['pesquisa_simples'])){
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
                $arWhere[] = "(
                    (c.descricao ILIKE '%{$pesquisa_simples}%') OR
                    (pj.razao_social ILIKE '%{$pesquisa_simples}%') OR
                    (pj.nome_fantasia ILIKE '%{$pesquisa_simples}%') OR
                    (pj.cnpj ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
        case 'pesquisa_avancada' :
            $pesquisaAvancada = true;

            if(!empty($_REQUEST['descricao'])){
                $descricao = trim(addslashes($_REQUEST['descricao']));
                $arWhere['descricao'] = "c.descricao ILIKE '%{$descricao}%' ";
            }

            if(!empty($_REQUEST['razao_social'])){
                $razao_social = trim(addslashes($_REQUEST['razao_social']));
                $arWhere['razao_social'] = "pj.razao_social ILIKE '%{$razao_social}%' ";
            }

            if(!empty($_REQUEST['nome_fantasia'])){
                $nome_fantasia = trim(addslashes($_REQUEST['nome_fantasia']));
                $arWhere['nome_fantasia'] = "pj.nome_fantasia ILIKE '%{$nome_fantasia}%' ";
            }

            if(!empty($_REQUEST['cnpj'])){
                $cnpj = trim(addslashes($_REQUEST['cnpj']));
                $arWhere['cnpj'] = "pj.cnpj ILIKE '%{$cnpj}%' ";
            }
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Consórcios');
?>
<style type="text/css">
    .campos-pesquisa{
        border-bottom: 1px solid #eef1f5;
        padding: 10px 0px 10px 0px;
        margin-bottom: 10px;
    }
</style>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-share-alt"></i>
            <span class="caption-subject bold uppercase">
                Lista de consórcios
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="consorcio-form" method="POST">
                <div class="form-body">
                    <div class="pesquisa-simples <?= ($pesquisaAvancada ? 'hidden' : '') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="input-group">
                                    <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por nome do consórcio, razão social, CNPJ ou nome fantasia'); ?>
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

                    <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Nome do consórcio</label>
                                    <?= campo_textom('descricao', 'N', 'S', '', 190, 190, '', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Razão social</label>
                                    <?= campo_textom('razao_social', 'N', 'S', '', 190, 190, '', '', '', '', 0); ?>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>CNPJ</label>
                                    <?= campo_textom('cnpj', 'N', 'S', '', 19, 19, '99.999.999/9999-99', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Nome fantasia</label>
                                    <?= campo_textom('nome_fantasia', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
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
                                <button type="button" class="btn green-jungle btn-pesquisa-simples <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                                    <i class="fa fa-search-minus"></i> Pesquisa simples
                                </button>
                            </div>
                        </div>
                        <div class="col-xs-5 col-sm-7 text-right">
                            <a class="btn green" href="?modulo=principal/consorcio/formulario&acao=C">
                                <i class="fa fa-plus-circle"></i>
                                Cadastrar novo consórcio
                            </a>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="<?= ($pesquisaAvancada ? 'pesquisa_avancada' : 'pesquisa_simples') ?>"/>
            </form>
        </div>
    </div>

    <?php
    $acao = <<<ACAO
            <div style="min-width: 5px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/consorcio/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar consórcio">
                    <i class="icon-pencil"></i>
                </a>
                <php>
                    if('{qtd_municipio}' === '0'){
                        return "<a class=\"btn btn-circle btn-icon-only btn-default red\" href=\"javascript:exibirModalExclusao('" . setParam(array('id' => '{id}'), false) . "', '{nome_fantasia}');\" data-toggle=\"tooltip\"  title=\"Excluir consórcio\">
                                    <i class=\"icon-trash\"></i>
                                </a>";
                    }else{
                        return "<a class=\"btn btn-circle btn-icon-only btn-default grey\" data-toggle=\"tooltip\"  title=\"Este consórcio não pode ser excluído pois possui municípios associados\">
                                    <i class=\"icon-trash\"></i>
                                </a>";
                    }
                </php>
            </div>
ACAO;

    $arConfig = array();
    $arConfig[0]['html'] = <<<HTML
    <a href="?modulo=principal/consorcio/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar consórcio">
        {descricao}
    </a>
HTML;
    $arConfig[3]['style'] = "text-align: center;";
    $arConfig[3]['style'] = "width: 10%;";

    $rs = $mConsorcio->getList($arWhere);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho( array(
        'Nome do consórcio',
        'Razão social',
        'Nome fantasia',
        'CNPJ',
        'Ação'
    ));
    $mLista->setCorpo( $rs, $arConfig );
    $mLista->setAcao( $acao );
    $mLista->show();
    ?>
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
                <h4 class="modal-title">Excluir consórcio</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja excluir o consórcio <strong id="nome"></strong>?
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
            window.location = '?modulo=principal/consorcio/lista&acao=C';
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
</script>
