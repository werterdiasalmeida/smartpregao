<?php
$mCargaEditaisCaptacao = new CargaEditaisCaptacao();
$data_inicio =  date('Y-m-d');
$data_fim =  date('Y-m-d');
$txt_cookie = '_ga=GA1.3.165147413.1659353544; rdtrk=%7B%22id%22%3A%229bab0df0-d5d3-4d3e-9aae-6ee9770548c1%22%7D; stonlyWidget_triggerShown_13538=1; _ltuid=t1XwT; _fbp=fb.2.1681735616774.1080822722; _gid=GA1.3.1035722561.1683640216; _gcl_au=1.1.1122350250.1683724342; _gat=1; JSESSIONID=73EEC07BF93A5AC4D454B0DBEF13DD8D; remember-me=Y29tZXJjaWFsJTQwd2Vic2lzLmNvbS5icjoxNjg0OTMzOTUwNjg2OmQ4OWEzMzNjMzE4NzJiZGJlNWE3NzkzZDcxODViMWIx; __trf.src=encoded_eyJmaXJzdF9zZXNzaW9uIjp7InZhbHVlIjoiKG5vbmUpIiwiZXh0cmFfcGFyYW1zIjp7fX0sImN1cnJlbnRfc2Vzc2lvbiI6eyJ2YWx1ZSI6Iihub25lKSIsImV4dHJhX3BhcmFtcyI6e319LCJjcmVhdGVkX2F0IjoxNjgzNzI0MzUwNTk5fQ==';

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
                        <label>Data In�cio da Abertura</label>
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
                    <i class="bi bi-download"></i> Baixar dados do Siga Preg�o
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
                    'C�digo Externo',
                    'Comprasnet (prgid)',
                    'Portal',
                    'Data Limite',
                    'Org�o',
                    'UASG',
                    'Preg�o',
                    'T�tulo',
                    'Descri��o',
                    'Publica��o',
                    'Situa��o'
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