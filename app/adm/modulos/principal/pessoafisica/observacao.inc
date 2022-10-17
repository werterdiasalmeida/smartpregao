<?php
$p = getParam();
$mObservacao = new ObservacaoPessoaFisica();

if(empty($p['id'])){
    header("Location: ?modulo=principal/pessoafisica/lista&acao=C");
}

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'salvar' :
            $params = array_merge($_POST, getParam());
            $params['pessoafisica_id'] = $params['id'];
            unset($params['id']);
            $id = $mObservacao->salvarObservacao($params);
            $mObservacao->commit();

            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Observações');

UsuarioResponsabilidadeEstabelecimento::criaAbam($abacod_tela, $url, setParam(getParam()), array(), true);
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-user font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Observações da pessoa
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="observacao-form" method="POST" enctype="multipart/form-data">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Observação</label>
                            <?= campo_textaream('descricao', 'S', 'S', '', '10', '5', '1000', '', 0, '') ?>
                        </div>
                    </div>
                </div>

            </div>
            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-plus"></i> Adicionar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar"/>
        </form>
    </div>

    <?php
    $arConfig = array();
    $arConfig[0]['style'] = "text-align: center; width: 15%;";
    $arConfig[2]['style'] = "width: 15%;";

    $rs = $mObservacao->listarPorPessoa($p['id']);

    $mLista = new Listam(array(
        "style" => "width: 100%;"
    ));

    $mLista->setCabecalho( array(
        'Data',
        'Observação',
        'Incluído por',
    ));

    $mLista->setCorpo( $rs, $arConfig );
    $mLista->setAcao( $acao );
    $mLista->show();
    ?>
</div>

<script type="text/javascript">
    $(function () {
        $(".observacao-form").validateForm();
    });
</script>
