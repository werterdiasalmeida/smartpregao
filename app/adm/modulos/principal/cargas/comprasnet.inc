<?php
$mCargaEditaisCaptacao = new CargaEditaisCaptacao();
$dt_entrega =  date('Y-m-d');

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {

        case 'baixar-comprasnet':

            // $url = 'http://comprasnet.gov.br/livre/pregao/dados_pregao.asp?prgcod=1085142&Tipo=E';

            $url = 'https://comprasnet.gov.br/livre/Pregao/avisos1.asp?prgCod=1140960&Origem=Avisos&Tipo=A';

            $ch = curl_init($url);
            
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
            
            $content = curl_exec($ch);
            
            curl_close($ch);
            
            if ($content !== false) {
                // Faça algo com o conteúdo recuperado
                echo $content;
            } else {
                // Ocorreu um erro ao recuperar o conteúdo
                echo "Erro ao recuperar o conteúdo da URL.";
            }
            
            ver($content, d);

            // Inicializa o cURL
            $ch = curl_init();

            // Configura as opções do cURL
            curl_setopt($ch, CURLOPT_URL, $url);
            curl_setopt($ch, CURLOPT_POST, true);
            curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($data));
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

            // Executa a requisição e obtém a resposta
            $response = curl_exec($ch);

            // Fecha a conexão cURL
            curl_close($ch);

            ver($response, d);

            curl_close($curl);
            $return = json_decode($response, true);
            $totalPaginas = $return['totalPages'];
            $reg = 0;
            for ($i = 0; $i < $totalPaginas; $i++) {
                $url = "http://app.sigapregao.com.br/app/api/oportunidades?tipoPeriodo=abertura&orgao=&pesquisa=&excluir=&limitar=&portal=&abertura=&dataInicial={$data_inicio}&dataFinal={$data_fim}&modalidade=&dataEncerramento=&tipoFiltroEncerramento=ate&horaEncerramento=&statusItem=&situacao=&faseCotacao=&ufs=&portais=CN&modalidades=1&superoportunidades=false&page={$i}";
                $curl = curl_init();
                curl_setopt_array($curl, array(
                    CURLOPT_URL => $url,
                    CURLOPT_RETURNTRANSFER => true,
                    CURLOPT_MAXREDIRS => 10,
                    CURLOPT_TIMEOUT => 0,
                    CURLOPT_FOLLOWLOCATION => true,
                    CURLOPT_CUSTOMREQUEST => 'GET',
                    CURLOPT_POSTFIELDS => '{}',
                    CURLOPT_COOKIE => $txt_cookie,
                    CURLOPT_HTTPHEADER => array(
                        'Content-Type: application/json; charset=UTF-8',
                        'Accept: application/json'
                    ),
                ));
                $response = curl_exec($curl);
                curl_close($curl);
                $return = json_decode($response, true);
                foreach ($return['content'] as $dados) {
                    $dado = array_map("utf8_decode", $dados);
                    $dado['id_siga'] = $dado['id'];
                    $dado['nomeportal'] = $dado['nomePortal'];
                    $dado['nomemodalidade'] = $dado['nomeModalidade'];
                    $dado['aberturaformatada'] = $dado['aberturaFormatada'];
                    $dado['encerramentoformatado'] = $dado['encerramentoFormatado'];
                    $dado['datapublicacaoformatada'] = $dado['dataPublicacaoFormatada'];
                    $existe = $mCargaEditaisCaptacao->pegaLinha(" SELECT id_siga FROM cargas.tb_carga_editais_captacao  WHERE id_siga = '{$dado['id_siga']}'");
                    if (!$existe) {
                        $gravou = $mCargaEditaisCaptacao->salvarEditalCaptacao($dado);
                    }
                    $reg++;
                }
            }
            $resultadoCarga = "Foram processados <b>{$reg}</b> registros !";
            $db->sucessom($_REQUEST['modulo'], '', $resultadoCarga);
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
                <input type="hidden" name="act" value="baixar-comprasnet" />

                <div class="row">
                    <div class="col-md-2">
                        <label>Data Início da Abertura</label>
                        <?= campo_datam('dt_entrega', 'S', 'S', 'S', 'S'); ?>
                    </div>
                    <div class="col-md-2">
                        <br>
                        <button type="submit" class="btn blue">
                            <i class="bi bi-download"></i> Baixar dados do ComprasNet
                        </button>
                    </div>
                </div>
        </div>

        </form>
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
                    'Comprasnet (prgid)',
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