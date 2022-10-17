<?php
$mTabelaProcedimentoConvenio = new TabelaProcedimentoConvenio();
$mTabelaPreco = new TabelaPreco();
$p = getParam();
$arWhere = array(" tp.estabelecimento_id = {$p['id']}");

if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
        case 'excluir' :
            $p = getParam();

            $mTabelaPreco->excluirLogicamente($p['tabela_preco_id']);
            $mTabelaPreco->commit();

            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])), 'Tabelas de preço excluídas com sucesso');
            break;
    }
}

UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);
include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();


UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true);

?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-table"></i>
            <span class="caption-subject bold uppercase">
                Lista de tabela de preços
            </span>
        </div>
    </div>
    <div class="portlet-body">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" method="POST">
                <div class="form-actions">
                    <div class="row">
                        <div class="col-xs-7 col-sm-9">
                        </div>
                        <div class="col-xs-5 col-sm-3 text-right">
                            <a class="btn green" href="?modulo=principal/estabelecimento/tabelaprecos/formulario&acao=C<?= setParam(array('id' => $p['id'])) ?>">
                                <i class="fa fa-plus-circle"></i>
                                Incluir tabela de preço
                            </a>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="pesquisar"/>
            </form>
        </div>
    </div>

    <?php
    $acao = <<<ACAO
                <div style="min-width: 85px;">
                    <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/tabelaprecos/formulario&acao=C&p=<php>return setParam(array('tabela_preco_id' => '{id}', 'id' => {estabelecimento_id}), false); </php>" data-toggle="tooltip" title="Editar">
                        <i class="icon-pencil"></i>
                    </a>
                    <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao('<php>return setParam(array('tabela_preco_id' => '{id}', 'id' => {estabelecimento_id}), false); </php>', '<php>return formata_data('{inicio_vigencia}') . ' a ' . formata_data('{fim_vigencia}') </php>');" data-toggle="tooltip"  title="Excluir">
                        <i class="icon-trash"></i>
                    </a>
                </div>
ACAO;

    $arConfig = array();
    $arConfig[0] =  array(
        'style' => 'width: 10%; text-align: center;',
        'html' => '<php>return formata_data("{inicio_vigencia}");</php>'
    );
    $arConfig[1] =  array(
        'style' => 'width: 10%; text-align: center;',
        'html' => '<php>return formata_data("{fim_vigencia}");</php>'
    );
    $arConfig[2] =  array(
        'style' => 'width: 80%; text-align: left;',
        'html' => '<php>return getLabels("{convenios}");</php>'
    );

    $rs = $mTabelaPreco->getList($arWhere);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho(array(
        'Início da vigência',
        'Fim da vigência',
        'Convênios/Planos',
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
                <h4 class="modal-title">Excluir registro</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja excluir a tabela de preços de <strong id="nome"></strong>?
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

