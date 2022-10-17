<?php
$mEntidade = new TipoProfissional();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            $params = array_merge($_POST, getParam());

            $arrCamposNulos = array();

            foreach ($params as $k => $v){
                if(empty($v)){
                    $params[$k] = null;
                    $arrCamposNulos[] = $k;
                }
            }

            $mEntidade->salvarTipoProfissional($params, $arrCamposNulos);
            $mEntidade->commit();

            $db->sucessom("sistema/apoio/tipoprofissional/lista", '');
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
monta_titulom('Insira os dados do tipo de profissional');
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do tipo de profissional
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="tipo-profissional-form" method="POST">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Descrição</label>
                            <?= campo_textom('descricao', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Sigla</label>
                            <?= campo_textom('sigla_tipo_registro', 'S', 'S', '', 10, 10, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-3">
                        <div class="form-group">
                            <label>Pronome de tratamento masculino</label>
                            <?= campo_textom('pron_trat_masc', 'N', 'S', '', 10, 50, '', '', '', '', 0); ?>
                        </div>
                    </div>
                    <div class="col-md-3">
                        <div class="form-group">
                            <label>Pronome de tratamento feminino</label>
                            <?= campo_textom('pron_trat_fem', 'N', 'S', '', 10, 50, '', '', '', '', 0); ?>
                        </div>
                    </div>
                    <div class="col-md-3">
                        <div class="form-group">
                            <label>Ordem Pronome</label>
                            <?= campo_textom('ordem_pronome', 'N', 'S', '', 10, 2, '99', '', '', '', 0); ?>
                        </div>
                    </div>
                    <div class="col-md-3">
                        <div class="form-group">
                            <label>Código conselho</label>
                            <?= campo_textom('codigo_conselho', 'N', 'S', '', 2, 2, '99', '', '', '', 0); ?>
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
        $(".tipo-profissional-form").validateForm();

        $('.btn-voltar').click(function () {
            window.location = '?modulo=sistema/apoio/tipoprofissional/lista&acao=C';
        });
    });
</script>
