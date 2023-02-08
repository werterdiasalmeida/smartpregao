<?php
$mCargaEditaisCaptacao = new CargaEditaisCaptacao();
$data_inicio =  date('Y-m-d');
$data_fim =  date('Y-m-d');
$txt_cookie = '_ga=GA1.3.165147413.1659353544; rdtrk={"id":"9bab0df0-d5d3-4d3e-9aae-6ee9770548c1"}; stonlyWidget_autolaunchTriggered_13538=1; stonlyWidget_triggerShown_13538=1; _ltuid=t1XwT; _gcl_au=1.1.426049715.1675162045; remember-me=Y29tZXJjaWFsJTQwd2Vic2lzLmNvbS5icjoxNjc2MzcxNzA2NjY5OjY2NGU1NmM4ZjZhMjFmYzI4NDA3MzRkNTVjYmY4ZWEz; _gid=GA1.3.968366638.1675250563; JSESSIONID=0468EB1F011AD8A417A8E2158CC07543; __trf.src=encoded_eyJmaXJzdF9zZXNzaW9uIjp7InZhbHVlIjoiKG5vbmUpIiwiZXh0cmFfcGFyYW1zIjp7fX0sImN1cnJlbnRfc2Vzc2lvbiI6eyJ2YWx1ZSI6Iihub25lKSIsImV4dHJhX3BhcmFtcyI6e319LCJjcmVhdGVkX2F0IjoxNjc1MjU5MTY1NTUzfQ==; _gat=1';

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {

        case 'baixar-siga-pregao':
            $data_inicio = formata_data_sql($_POST['data_inicio']);
            $data_fim = formata_data_sql($_POST['data_fim']);
            $txt_cookie = str_replace('\\', '', $_POST['txt_cookie']);
            $url = "http://app.sigapregao.com.br/app/api/oportunidades?tipoPeriodo=abertura&orgao=&pesquisa=&excluir=&limitar=&portal=&abertura=&dataInicial={$data_inicio}&dataFinal={$data_fim}&modalidade=&dataEncerramento=&tipoFiltroEncerramento=ate&horaEncerramento=&statusItem=&situacao=&faseCotacao=&ufs=&portais=CN&modalidades=1&superoportunidades=false&page=0";
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
                <input type="hidden" name="act" value="baixar-siga-pregao" />

                <div class="row">
                    <div class="col-md-2">
                        <label>Data Início da Abertura</label>
                        <?= campo_datam('data_inicio', 'S', 'S', 'S', 'S'); ?>
                    </div>
                    <div class="col-md-2">
                        <label>Data Fim da Abertura</label>
                        <?= campo_datam('data_fim', 'S', 'S', 'S', 'S'); ?>
                    </div>
                </div>
        </div>
        <div class="row">
            <div class="col-md-8">
                <label>Cookie</label>
                <?= campo_textaream('txt_cookie', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                <br />
                <button type="submit" class="btn blue">
                    <i class="bi bi-download"></i> Baixar dados do Siga Pregão
                </button>
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
                    'Publicação'
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