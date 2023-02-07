<?php

$mCargaEstabelecimento = new CargaEstabelecimento();
if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {

        case 'baixar':
            $prgcod = $_POST['prgcod'];
            $url = "http://comprasnet.gov.br/livre/pregao/anexosPropostaHabilitacao.asp?prgCod={$prgcod}";
            $curl = curl_init();
            curl_setopt_array($curl, array(
                CURLOPT_URL => $url,
                CURLOPT_RETURNTRANSFER => true,
                CURLOPT_MAXREDIRS => 10,
                CURLOPT_TIMEOUT => 0,
                CURLOPT_FOLLOWLOCATION => true,
                CURLOPT_CUSTOMREQUEST => 'GET',
            ));
            $response = curl_exec($curl);
            curl_close($curl);
            $return = explode('<tr class="tex3" bgcolor="#FFFFFF">', $response);
            $resultadoCarga = '';
            if (is_array($return)) {
                foreach ($return as $dados) {
                    $rsDadosBaixados = array();
                    $dado  = explode('<td>', $dados);
                    $texto = $dado[1];
                    $saida = explode(' - ', $texto);
                    $saida[0] = str_replace(' ', '', $saida[0]);

                    if ($saida[1]) {
                        $rsDadosBaixados['cnpj'] = $saida[0];
                        $texto2 = explode('   ', $saida[1]);
                        $rsDadosBaixados['razao_social'] = $texto2[0];
                        $rsDadosBaixados['situacao_carga'] = 'dados-basicos';
                        $rsDadosBaixados['prgcod'] = $prgcod;
                        //Grava no Banco
                        $existe = $mCargaEstabelecimento->pegaLinha(" SELECT id FROM carga_clientes.tb_carga_estabelecimento  WHERE cnpj = '{$rsDadosBaixados['cnpj']}'");
                        if (!$existe) {
                            $mCargaEstabelecimento->salvarBasico($rsDadosBaixados);
                            $resultadoCarga .= 'Carregado: ' .  $rsDadosBaixados['cnpj'] . ' ( ' . $rsDadosBaixados['razao_social']  . ') <br/>';
                        } else {
                            $resultadoCarga .= 'Já esta cadastrado: ' .  $rsDadosBaixados['cnpj'] . ' ( ' . $rsDadosBaixados['razao_social']  . ') <br/>';
                        }
                        unset($rsDadosBaixados);
                    }
                }
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
                Cargas Externas de Editais
            </span>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
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
                    <button type="submit" class="btn red">
                        <i class="bi bi-save"></i> Gravar
                    </button>
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