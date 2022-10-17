<?php
$moduloFormulario = $abaEstabelecimento ? 'principal/estabelecimento/tiposervico/formulario' : 'sistema/apoio/tipoconsulta/formulario';
$p = getParam();
$pUrlParam = (isset($p['id']) && !empty($p['id']) ? setParam(array('id' => $p['id'])) : '');

$mEntidade = new TipoConsulta();
$arWhere   = array();

if($abaEstabelecimento){
    $arWhere[] = "(tc.estabelecimento_id = {$p['id']} OR tc.estabelecimento_id IS NULL)";
}

if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
        case 'excluir' :
            $p = getParam();
            $mEntidade->carregarPorId($p['tipo_consulta_id']);

            if(!$db->testa_superuser()){
                UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimento($mEntidade->estabelecimento_id);
            }

            $mEntidade->excluirLogicamente($p['tipo_consulta_id']);
            $mEntidade->commit();
            $db->sucessom($_REQUEST['modulo'], $pUrlParam, 'Tipo de serviço excluído com sucesso');
            break;
        case 'pesquisar' :
            if(!empty($_REQUEST['descricao'])){
                $descricao = trim(addslashes($_REQUEST['descricao']));
                $arWhere['descricao'] = "tc.descricao ILIKE '%{$descricao}%' ";
            }
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
if($abaEstabelecimento){
    echo Estabelecimento::gerarCabecalho();
}


if($abaEstabelecimento){
    UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true);
} else {
    monta_titulom('Tipos de serviço');
}
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
            <i class="fa icon-check"></i>
            <span class="caption-subject bold uppercase">
                Lista de tipos de serviço
            </span>
        </div>
    </div>
    <div class="portlet-body">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" method="POST">
                <div class="form-body">
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <div class="input-group">
                                    <?= campo_textom('descricao', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                    <span class="input-group-btn">
                                        <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                            <i class="fa fa-search"></i>
                                        </button>
                                    </span>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-actions">
                    <div class="row">
                        <div class="col-xs-7 col-sm-9">
                        </div>
                        <div class="col-xs-5 col-sm-3 text-right">
                            <a class="btn green" href="?modulo=<?= $moduloFormulario ?>&acao=C<?= $pUrlParam ?>">
                                <i class="fa fa-plus-circle"></i>
                                Incluir tipo de serviço
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
    <php>
        if(!'{$db->testa_superuser()}' && '{estabelecimento_id}' === '') :
            return '<div style="min-width: 5px;">
                        <a class="btn btn-circle btn-icon-only btn-default grey" data-toggle="tooltip" title="Não é possível alterar tipos de serviço padrões">
                            <i class="icon-pencil"></i>
                        </a>
                        <a class="btn btn-circle btn-icon-only btn-default grey" data-toggle="tooltip" title="Não é possível excluir tipos de serviço padrões">
                            <i class="icon-trash"></i>
                        </a>
                    </div>';
        else :
            return '<div style="min-width: 5px;">
                        <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo={$moduloFormulario}&acao=C&p=' . setParam(array('tipo_consulta_id' => '{tipo_consulta_id}', 'id' => '{estabelecimento_id}'), false) . '" data-toggle="tooltip" title="Editar">
                            <i class="icon-pencil"></i>
                        </a>
                        <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao(\'' . setParam(array('tipo_consulta_id' => '{tipo_consulta_id}', 'id' => '{estabelecimento_id}'), false). '\', \'{descricao}\');" data-toggle="tooltip"  title="Excluir">
                            <i class="icon-trash"></i>
                        </a>
                    </div>';
        endif;
    </php>
            
ACAO;

    if($abaEstabelecimento){
        $acao = <<<ACAO
                <php>
                    if(!'{$db->testa_superuser()}' && '{estabelecimento_id}' === '') :
                        return '<div style="min-width: 5px;">
                                    <a class="btn btn-circle btn-icon-only btn-default grey" data-toggle="tooltip" title="Não é possível alterar tipos de serviço padrões">
                                        <i class="icon-pencil"></i>
                                    </a>
                                    <a class="btn btn-circle btn-icon-only btn-default grey" data-toggle="tooltip" title="Não é possível excluir tipos de serviço padrões">
                                        <i class="icon-trash"></i>
                                    </a>
                                </div>';
                    else :
                        return '<div style="min-width: 5px;">
                                    <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo={$moduloFormulario}&acao=C&p=' . setParam(array('tipo_consulta_id' => '{tipo_consulta_id}', 'id' => ('{estabelecimento_id}' ? '{estabelecimento_id}' : '{$p['id']}')), false) . '" data-toggle="tooltip" title="Editar">
                                        <i class="icon-pencil"></i>
                                    </a>
                                    <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao(\'' . setParam(array('tipo_consulta_id' => '{tipo_consulta_id}', 'id' => ('{estabelecimento_id}' ? '{estabelecimento_id}' : '{$p['id']}')), false) . '\', \'{descricao}\');" data-toggle="tooltip"  title="Excluir">
                                        <i class="icon-trash"></i>
                                    </a>
                                </div>';
                    endif;
                </php>
ACAO;
    }

    $arConfig = array();
    $arConfig[2]['align'] = 'center';
    $arConfig[3]['style'] = 'width: 5%';

    $rs = $mEntidade->getList($arWhere);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho( array(
        'Descrição',
        'Estabelecimento',
        'Remunerado',
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
                Você tem certeza que deseja excluir o tipo de serviço <strong id="nome"></strong>?
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
