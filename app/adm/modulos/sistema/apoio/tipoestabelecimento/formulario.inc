<?php
$mEntidade = new TipoEstabelecimento();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            $params = array_merge($_POST, getParam());
            $mEntidade->salvarTipoEstabelecimento($params);
            $mEntidade->commit();

            $db->sucessom("sistema/apoio/tipoestabelecimento/lista", '');
            break;
    }
}

$params = getParam();
if (isset($params['id']) && !empty($params['id'])) {
    $mEntidade->carregarPorId($params['id']);
    $dados = $mEntidade->getDados();
    extract($dados);
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Insira os dados do tipo de estabelecimento');
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do tipo de estabelecimento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="tipo-estabelecimento-form" method="POST">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Descrição</label>
                            <?= campo_textom('descricao', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>
            </div>
            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar"/>
        </form>
    </div>
</div>


<script type="text/javascript">
    $(function () {
        $(".tipo-estabelecimento-form").validateForm();

        $('.btn-voltar').click(function () {
            window.location = '?modulo=sistema/apoio/tipoestabelecimento/lista&acao=C';
        });
    });
</script>
