<?php
$mCargaEstabelecimento = new CargaEstabelecimento();
$mCargaEditaisCaptacao = new CargaEditaisCaptacao();
$data_referencia =  date('Y-m-d');
if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {

        case 'baixar':
            $params['prgcod'] = $_POST['prgcod'];
            $mCargaEstabelecimento->baixarGravarEmpresasPregao($params);
            break;
        case 'baixar-completo':
            $data_referencia = $_POST['data_referencia'];

            $arrWhere[] = " substring(aberturaformatada from 0 for 11)  =  '{$data_referencia}'";
            $rs = $mCargaEditaisCaptacao->getList($arrWhere);
            $resultadoCarga = '';
            foreach ($rs['dados'] as  $result) {
                $params['prgcod'] = $result['codigo'];
                $resultadoCarga .= $mCargaEstabelecimento->baixarGravarEmpresasPregao($params);
            }
            if ($resultadoCarga) {
                $db->sucessom($_REQUEST['modulo'], '', $resultadoCarga);
            } else {
                $db->insucessom($_REQUEST['modulo'], 'Sem dados para Baixar.', 'Sem dados para Baixar.');
            }
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
                Cargas Externas de Potenciais Clientes
            </span>
        </div>
    </div>
    <div class="row">
        <div class="col-md-6">
            <form role="form" name="listar" class="edital-form" method="POST" enctype="multipart/form-data">
                <input type="hidden" name="act" value="baixar" />
                <div class="row">
                    <div class="col-md-8">
                        <label>PRGCOD ComprasNet</label>
                        <?= campo_textom('prgcod', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        <br />
                        <button type="submit" class="btn blue">
                            <i class="bi bi-download"></i> Baixar dados do ComprasNet
                        </button>
                    </div>
                </div>
            </form>
        </div>
        <div class="col-md-6">
            <form role="form" name="listar" class="completo-form" method="POST" enctype="multipart/form-data">
                <input type="hidden" name="act" value="baixar-completo" />
                <div class="row">
                    <div class="col-md-8">
                        <label>Data de Referênica</label>
                        <?= campo_datam('data_referencia', 'S', 'S', 'S', 'S'); ?>
                        <br />
                        <button type="submit" class="btn red">
                            <i class="bi bi-download"></i> Baixar participantes de TODOS OS Pregões do dia
                        </button>
                    </div>
                </div>
            </form>
        </div>
        <div class="row">
            <div class="col-md-12">
                <form role="form" name="gravar" class="edital-form" method="POST" enctype="multipart/form-data">
                    <input type="hidden" name="act" value="gravar-selecionados" />
                    <textarea name="json_resultados" style="display: none;"><?php echo $response; ?></textarea>
                    <?php
                    unset($arConfig, $acao);
                    $rs = $mCargaEstabelecimento->getList($arWhere);
                    $arConfig = array();
                    $mCargaEditais = new Listam(array("style" => "width: 100%;"));
                    $dado = array_map("utf8_decode", $dados);
                    $mCargaEditais->setCabecalho(array(
                        'ID',
                        'CNPJ',
                        'Razão Social',
                        'Nome Fantasia',
                        'CNAE',
                        'Município',
                        'Email',
                        'Telefone',
                        'Situação',
                        'Pregões',
                    ));
                    $mCargaEditais->setCorpo($rs, $arConfig);
                    $mCargaEditais->setAcao($acao);
                    $mCargaEditais->show();
                    ?>
                    <br>

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