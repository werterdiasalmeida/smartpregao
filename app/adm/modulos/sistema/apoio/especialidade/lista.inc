<?php
$mEntidade = new Especialidade();
$arWhere   = array();

if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
        case 'excluir' :
            $arParams = getParam();
            $mEntidade->excluir($arParams['id']);
            $mEntidade->commit();
            $db->sucessom($_REQUEST['modulo'], '', 'Especialidade excluída com sucesso');
            break;
        case 'pesquisar' :
            if(!empty($_REQUEST['descricao'])){
                $descricao = trim(addslashes($_REQUEST['descricao']));
                $arWhere['descricao'] = "e.descricao ILIKE '%{$descricao}%' ";
            }

            if(!empty($_REQUEST['tipo_profissional_id'])){
                $arWhere['tipo_profissional_id'] = "e.tipo_profissional_id = " . (int) $_REQUEST['tipo_profissional_id'];
            }
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Especialidade');
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
            <i class="fa icon-user-following"></i>
            <span class="caption-subject bold uppercase">
                Especialidade
            </span>
        </div>
    </div>
    <div class="portlet-body">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="especialidade-form" method="POST">
                <div class="form-body">
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Descrição</label>
                                <?= campo_textom('descricao', 'N', 'S', '', 90, 90, '', '', '', '', 0,'','',$_REQUEST['descricao']); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Tipo</label>
                                <?php
                                $mTipo = new TipoProfissional();
                                $tipos = $mTipo->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                                $db->monta_combom("tipo_profissional_id", $tipos, 'S', 'Selecione...', '', '', '', '', 'N','', false,$_REQUEST['tipo_profissional_id']);
                                ?>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-actions">
                    <div class="row">
                        <div class="col-xs-7 col-sm-9">
                            <button type="submit" class="btn blue">
                                <i class="fa fa-search"></i> Pesquisar
                            </button>
                            <button type="button" class="btn btn-limpar">
                                <i class="fa fa-eraser"></i> Limpar
                            </button>
                        </div>
                        <div class="col-xs-5 col-sm-3 text-right">
                            <a class="btn green" href="?modulo=sistema/apoio/especialidade/formulario&acao=C">
                                <i class="fa fa-plus-circle"></i>
                                Cadastrar
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
            <div style="min-width: 5px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=sistema/apoio/especialidade/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar">
                    <i class="icon-pencil"></i>
                </a>
                <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao('<php> return setParam(array('id' => '{id}'), false); </php>', '{descricao}');" data-toggle="tooltip"  title="Excluir">
                    <i class="icon-trash"></i>
                </a>
            </div>
ACAO;

    $arConfig = array();
    $rs = $mEntidade->getList($arWhere);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho( array(
        'Descrição',
        'Tipo de Profissional',
        'CBOS',
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
                Você tem certeza que deseja excluir a especialidade <strong id="nome"></strong>?
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
            window.location = '?modulo=sistema/apoio/especialidade/lista&acao=C';
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
