<?php
$mCargaPalavraChave = new CargaPalavraChave();
$pesquisa = $_REQUEST['pesquisa'];

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'pesquisar':
            $arrWhere[] = " (tipo_carga ilike '%{$pesquisa}%' OR palavra ilike '%{$pesquisa}%') ";
            break;

        case 'salvar-cadastro':

            $mCargaPalavraChave->popularDadosObjeto($_REQUEST)->salvar(true, true, array());
            $salvo = $mCargaPalavraChave->salvar();
            $mCargaPalavraChave->commit();
            $db->sucessom($_REQUEST['modulo'], '', 'Carga salva com Sucesso!');

            break;
        case 'excuir-cadastro':

            $mCargaPalavraChave->popularDadosObjeto($_REQUEST)->excluir($_REQUEST['id']);
            $mCargaPalavraChave->commit();
            $db->sucessom($_REQUEST['modulo'], '', 'Carga Excluida com Sucesso!');
            break;
    }
}
include APPRAIZ . "includes/cabecalhom.inc";
getStrMsgDanger();
getStrMsgInfo();
getStrMsgWarning();
?>
<br />
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-list"></i>
            <span class="caption-subject bold uppercase">
                Cadastro de Cargas
            </span>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <form role="form" name="listar" class="edital-form" method="POST" enctype="multipart/form-data">
                <input type="hidden" name="act" value="pesquisar" />
                <label>Pesquisa</label>
                <?= campo_textom('pesquisa', 'N', 'S', $pesquisa, 90, 90, '', '', '', '', 0); ?>
                <br />

                <button type="submit" class="btn blue">
                    <i class="bi bi-search"></i> Pesquisar
                </button>

                <button type="button" class="btn green" onclick="exibirModalCadastro();">
                    <i class="bi bi-file-earmark-plus"></i> Adicionar
                </button>
            </form>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <form role="form" name="gravar" class="edital-form" method="POST" enctype="multipart/form-data">
                <?php
                unset($arConfig, $acao);
                $rs = $mCargaPalavraChave->getList($arrWhere);
                $arConfig = array();
                $acao = <<<ACAO
                <div style="min-width: 20px;">
                   <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirCadastro({id});" data-toggle="tooltip"  title="Excluir">
                   <i class="bi bi-x-square"></i>
                   </a>
                </div> 
                ACAO;
                $mCargaPalavraChave = new Listam(array("style" => "width: 100%;"));
                $dado = array_map("utf8_decode", $dados);
                $mCargaPalavraChave->setCabecalho(array(
                    'ID',
                    'Carga',
                    'Palavra-CHave',
                    ''
                ));
                $mCargaPalavraChave->setCorpo($rs, $arConfig);
                $mCargaPalavraChave->setAcao($acao);
                $mCargaPalavraChave->show();
                ?>

            </form>
        </div>
    </div>
</div>
</div>
<!-- MODAL de Cadastro -->
<div class="modal fade in" id="modal-novo-cadastro" tabindex="-1" role="modal-novo-cadastro" aria-hidden="true">
    <div class="modal-dialog">
        <form role="form" name="formulario" id="form-cadastro" method="POST" enctype="multipart/form-data">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Novo Registro</h4>
                </div>
                <div class="modal-body">
                    <input type="hidden" name="act" value="salvar-cadastro" />
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Carga</label>
                                <?= campo_textom('tipo_carga', 'S', 'S', '',  '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Palavra</label>
                                <?= campo_textom('palavra', 'S', 'S', '',  '', '', '', '', 0); ?>
                            </div>
                        </div>

                    </div>
                </div>
                <div class="modal-footer">
                    <button type="submit" onclick="$('#form-cadastro').submit();" class="btn green" data-dismiss="modal">Salvar</button>
                    <button type="button" class="btn red" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </form>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>
<script>
    $('input[name="tipo_pesquisa"]').click(function() {

        $('#pesquisa').val(this.value);

    });

    function exibirModalCadastro() {
        var modal = $('#modal-novo-cadastro');
        modal.modal('show');
    }

    function excluirCadastro(id) {
        if (confirm('Confirma a exclusão do registro: ' + id)) {
            location.href = '/adm/adm.php?modulo=principal/cargas/carga_cadastro&acao=C&act=excuir-cadastro&id=' + id;
        }
    }
</script>