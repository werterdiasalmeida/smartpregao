<?php
$mCargaEditaisCaptacao = new CargaEditaisCaptacao();
$pesquisa = $_POST['pesquisa'];

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'pesquisar':

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
                Cargas Externas de Editais (PNCP - Portal Nacional de Compras Públicas)
            </span>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <form role="form" name="listar" class="edital-form" method="POST" enctype="multipart/form-data">
                <input type="hidden" name="act" value="baixar-comprasnet" />
                <label>Pesquisa</label>
                <?= campo_textom('pesquisa', 'S', 'S', $pesquisa, 90, 90, '', '', '', '', 0); ?>
                <br />

                <button type="submit" class="btn blue">
                    <i class="bi bi-search"></i> Pesquisar
                </button>

                <button type="button" class="btn green" onclick="window.open('/_roda_carga_pncp.php')">
                    <i class="bi bi-download"></i> Rodar carga do PNCP
                </button>
            </form>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <form role="form" name="gravar" class="edital-form" method="POST" enctype="multipart/form-data">
                <?php
                unset($arConfig, $acao);
                $rs = $mCargaEditaisCaptacao->getList($arrWhere);
                $arConfig = array();
                $mCargaEditaisCaptacao = new Listam(array("style" => "width: 100%;"));
                $dado = array_map("utf8_decode", $dados);
                $mCargaEditaisCaptacao->setCabecalho(array(
                    'Código Externo',
                    'Código PNCP',
                    'Portal',
                    'Data Limite',
                    'Orgão',
                    'UASG',
                    'Pregão',
                    'Título',
                    'Descrição',
                    'Publicação',
                    'Situação'
                ));
                $mCargaEditaisCaptacao->setCorpo($rs, $arConfig);
                $mCargaEditaisCaptacao->setAcao($acao);
                $mCargaEditaisCaptacao->show();
                ?>

            </form>
        </div>
    </div>
</div>
</div>
<script>
    $('input[name="tipo_pesquisa"]').click(function() {

        $('#pesquisa').val(this.value);

    });
</script>