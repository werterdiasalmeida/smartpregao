<?php
$p = getParam();
UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);

$mAnexo = new AnexoEstabelecimento();
$arWhere = array("ane.estabelecimento_id = {$p['id']}");

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'salvar' :
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            unset($params['id']);
            $id = $mAnexo->salvarAnexo($params, $_FILES);
            $mAnexo->commit();

            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;
        case 'download' :
            $file = new FilesSimec();
            $params = getParam();
            $file->getDownloadArquivo($params['arqid']);
            die;
            break;
        case 'excluir' :
            $mAnexo->excluirAnexo($p['anexo_id']);
            $mAnexo->commit();

            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])), 'Anexo exclu?do com sucesso');
            die;
            break;
    }
}


include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(getParam()), null, array(), true);

?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-paperclip font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Documentos do estabelecimento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="anexo-form" method="POST" enctype="multipart/form-data">
            <div class="form-body">
                <div class="row">

                    <div class="col-md-6">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Arquivo</label>
                                <input type="file" name="arquivo" class="form-control" required />
                            </div>
                        </div>

                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Tipo</label>
                                <?php
                                $mTipo = new TipoAnexo();
                                $tipos = $mTipo->recuperarTodos('id AS codigo, descricao', array('excluido IS FALSE'), 2);
                                $db->monta_combom("tipo_anexo_id", $tipos, 'S', 'Selecione...', '', '', '', '', 'S', '', false);
                                ?>
                            </div>
                        </div>
                    </div>

                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Observa??o</label>
                            <?= campo_textaream('observacao', 'N', 'S', '', '10', '5', '400', '', 0, '') ?>
                        </div>
                    </div>
                </div>

            </div>
            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar"/>
        </form>
    </div>

    <?php
    $acao = <<<ACAO
            <div style="min-width: 100px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/documento&acao=C&p=<php> return setParam(array('arqid' => '{arqid}', 'id' => {$p['id']}), false); </php>&act=download" data-toggle="tooltip" title="Download">
                    <i class="fa fa-download"></i>
                </a>
                <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao('<php> return setParam(array('anexo_id' => '{anexo_id}', 'id' => {$p['id']}), false); </php>', '{descricao}');" data-toggle="tooltip"  title="Excluir">
                    <i class="icon-trash"></i>
                </a>
            </div>
ACAO;

    $arConfig = array();
    $arConfig[0]['html'] = <<<HTML
    <a href="?modulo=principal/estabelecimento/documento&acao=C&p=<php> return setParam(array('arqid' => '{arqid}', 'id' => {$p['id']}), false); </php>&act=download" data-toggle="tooltip" title="{observacao}">
        {descricao}
    </a>
HTML;
    $arConfig[1]['style'] = "text-align: center;";
    $arConfig[3]['style'] = "text-align: center;";

    $rs = $mAnexo->getList($arWhere);

    $mLista = new Listam(array(
        "style" => "width: 100%;"
    ));

    $mLista->setCabecalho( array(
        'Arquivo',
        'Tamanho',
        'Tipo',
        'Data',
        'A??o'
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
                <h4 class="modal-title">Excluir anexo</h4>
            </div>
            <div class="modal-body">
                Voc? tem certeza que deseja excluir o anexo <strong id="nome"></strong>?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="excluirRegistro();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">N?o</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<script type="text/javascript">
    $(function () {
        $(".anexo-form").validateForm();
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