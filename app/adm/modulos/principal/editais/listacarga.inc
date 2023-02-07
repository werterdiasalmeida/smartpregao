<?php
$mCargaEditais = new CargaEditais();
$mEdital = new Edital();
$data_inicio =  date('Y-m-d');
$data_fim =  date('Y-m-d');
$pagina = '0';
$txt_cookie = '_ga=GA1.3.165147413.1659353544; rdtrk={"id":"9bab0df0-d5d3-4d3e-9aae-6ee9770548c1"}; stonlyWidget_autolaunchTriggered_13538=1; stonlyWidget_triggerShown_13538=1; _ltuid=t1XwT; _gcl_au=1.1.426049715.1675162045; remember-me=Y29tZXJjaWFsJTQwd2Vic2lzLmNvbS5icjoxNjc2MzcxNzA2NjY5OjY2NGU1NmM4ZjZhMjFmYzI4NDA3MzRkNTVjYmY4ZWEz; _gid=GA1.3.968366638.1675250563; JSESSIONID=0468EB1F011AD8A417A8E2158CC07543; __trf.src=encoded_eyJmaXJzdF9zZXNzaW9uIjp7InZhbHVlIjoiKG5vbmUpIiwiZXh0cmFfcGFyYW1zIjp7fX0sImN1cnJlbnRfc2Vzc2lvbiI6eyJ2YWx1ZSI6Iihub25lKSIsImV4dHJhX3BhcmFtcyI6e319LCJjcmVhdGVkX2F0IjoxNjc1MjU5MTY1NTUzfQ==; _gat=1';
$tipo_pesquisa = $_POST['tipo_pesquisa'];
if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'gravar-selecionados':
            if (is_array($_POST['selecionado_gravar'])) {
                $_POST['json_resultados'] = str_replace('\\', '', $_POST['json_resultados']);
                $return = json_decode($_POST['json_resultados'], true);
                $resultadoCarga = '';
                foreach ($_POST['selecionado_gravar'] as $selecionado) {
                    foreach ($return['content'] as $dados) {
                        $dado = array_map("utf8_decode", $dados);
                        if ($dado['id'] == $selecionado) {
                            if ($dado['nomePortal'] == 'COMPRASNET') $dado['nomePortal'] = 'ComprasNet';
                            if ($dado['nomePortal'] == 'LICITAÇÕES-E') $dado['nomePortal'] = 'BB';
                            if ($dado['nomePortal'] <> 'LICITAÇÕES-E' && $dado['nomePortal'] <> 'COMPRASNET') {
                                 $salvar['comentarios'] = 'Baixar em: '.$dado['nomePortal'];
                                 $dado['nomePortal'] = 'Outros';
                                }
                            $salvar['portal'] = $dado['nomePortal'];
                            $salvar['orgao'] = $dado['orgao'];
                            $salvar['edital'] = 'Pregão Eletrônico Nº 0' . $dado['numero'] . ' UASG: ' . $dado['uasg'];
                            $dataHora = explode(' às ', $dado['aberturaFormatada']);
                            $salvar['data_limite'] = $dataHora[0];
                            $salvar['hora_limite'] = $dataHora[1];
                            $salvar['objeto'] = str_replace('"', '', $dado['descricao']);
                            $salvar['objeto'] = str_replace('<strong>', '', $salvar['objeto']);
                            $salvar['objeto'] = str_replace('</strong>', '', $salvar['objeto']);
                            $salvar['valor_estimado'] = 'R$ -';
                            $salvar['id_externo'] = $dado['id'];
                            $salvar['id_origem'] = $dado['codigo'];
                            if (!$salvar['id_origem']) $salvar['id_origem'] = NULL;
                            $salvar['categoria'] = $_POST['categoria'];
                            $existe = $mEdital->pegaLinha(" SELECT id FROM edital.edital  WHERE id_externo = {$salvar['id_externo']}");
                            if (!$existe) {
                                $params['id'] = $mEdital->salvarEdital($salvar);
                                $mEdital->commit();
                                $resultadoCarga .= 'Carregado: ' .  $dado['titulo'] . ' <br/>';
                            } else {
                                $resultadoCarga .= 'Já esta cadastrado: ' .  $dado['titulo'] . ' <br/>';
                            }
                        }
                    }
                }
                $db->sucessom($_REQUEST['modulo'], '', $resultadoCarga);
            } else {
                $db->insucessom($_REQUEST['modulo'], '', 'Nenhum registro selecionado');
            }
            break;
        case 'baixar-siga-pregao':
            $data_inicio = formata_data_sql($_POST['data_inicio']);
            $data_fim = formata_data_sql($_POST['data_fim']);
            $txt_cookie = str_replace('\\', '', $_POST['txt_cookie']);
            $pagina = $_POST['pagina'];
            $pesquisa = $_POST['pesquisa'];
            $pesquisa = str_replace(' ', '+', $pesquisa);
            $pesquisa = str_replace(';', '%3B', $pesquisa);
            $url = "http://app.sigapregao.com.br/app/api/oportunidades?tipoPeriodo=publicacao&orgao=&excluir=&limitar=&portal=&abertura=&dataInicial={$data_inicio}&dataFinal={$data_fim}&modalidade=&dataEncerramento=&tipoFiltroEncerramento=ate&horaEncerramento=&statusItem=&situacao=&faseCotacao=&ufs=&portais=&modalidades=&superoportunidades=false&page={$pagina}&pesquisa={$pesquisa}";
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
            $rsDadosBaixados = array();
            $i = 0;
            foreach ($return['content'] as $dados) {
                $dado = array_map("utf8_decode", $dados);
                $rsDadosBaixados[$i]['id'] = $dado['id'];
                $rsDadosBaixados[$i]['nomePortal'] = $dado['nomePortal'];
                $rsDadosBaixados[$i]['aberturaFormatada'] = $dado['aberturaFormatada'];
                $rsDadosBaixados[$i]['orgao'] = $dado['orgao'];
                $rsDadosBaixados[$i]['uasg'] = $dado['uasg'];
                $rsDadosBaixados[$i]['numero'] = $dado['numero'];
                $rsDadosBaixados[$i]['codigo'] = $dado['codigo'];
                $rsDadosBaixados[$i]['titulo'] = $dado['titulo'];
                $rsDadosBaixados[$i]['descricao'] = $dado['descricao'];
                $rsDadosBaixados[$i]['publicacao'] = formata_data($dado['publicacao']);
                $i++;
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
                <input type="hidden" name="act" value="baixar-siga-pregao" />
                <div class="row">
                    <div class="col-md-2">
                        <label>Data Início da Publicação</label>
                        <?= campo_datam('data_inicio', 'S', 'S', 'S', 'S'); ?>
                    </div>
                    <div class="col-md-2">
                        <label>Data Fim da Publicação</label>
                        <?= campo_datam('data_fim', 'S', 'S', 'S', 'S'); ?>
                    </div>
                    <div class="col-md-8">
                        <label>Pesquisa</label>
                        <br /><input type="radio" name="tipo_pesquisa" value="desenvolvimento de solucoes;desenvolvimento de solucao;manutencao em software;fabrica de software;manutencaoo evolutiva;manutencaoo corretiva;desenvolvimento de sistema; desenvolvimento de software; website; portal institucional; Java; PHP; DOTNET; Python">
                        <label for="html">Desenvolvimento de Softwares</label>
                        <input type="radio" name="tipo_pesquisa" value="manutencao de servidores; manutencao de computadores; devops; formatacao; infraestrutura tecnologica; infraestrutura de tecnologia">
                        | <label for="html">Manutenção / Infraetrutura de Tecnologia</label>
                        <input type="radio" name="tipo_pesquisa" value="mensuracao de pontos de funcao; mensuracao de pf; pontos de funcao">
                        | <label for="html">Contagem de PF</label>
                        <br>
                        <?= campo_textaream('pesquisa', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                        <br />
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
                    <div class="col-md-4">
                        <label>Página</label>
                        <br />
                        <input type="radio" name="pagina" value="0" <?php if ($pagina == '0') echo 'checked=checked;' ?>>
                        <label for="html">0</label>
                        <input type="radio" name="pagina" value="1" <?php if ($pagina == '1') echo 'checked=checked;' ?>>
                        <label for="html">1</label>
                        <input type="radio" name="pagina" value="2" <?php if ($pagina == '2') echo 'checked=checked;' ?>>
                        <label for="html">2</label>
                        <input type="radio" name="pagina" value="3" <?php if ($pagina == '3') echo 'checked=checked;' ?>>
                        <label for="html">3</label>
                        <input type="radio" name="pagina" value="4" <?php if ($pagina == '4') echo 'checked=checked;' ?>>
                        <label for="html">4</label>
                        <input type="radio" name="pagina" value="5" <?php if ($pagina == '5') echo 'checked=checked;' ?>>
                        <label for="html">5</label>
                        <input type="radio" name="pagina" value="6" <?php if ($pagina == '6') echo 'checked=checked;' ?>>
                        <label for="html">6</label>
                        <input type="radio" name="pagina" value="7" <?php if ($pagina == '7') echo 'checked=checked;' ?>>
                        <label for="html">7</label>
                        <input type="radio" name="pagina" value="8" <?php if ($pagina == '8') echo 'checked=checked;' ?>>
                        <label for="html">8</label>
                        <input type="radio" name="pagina" value="9" <?php if ($pagina == '9') echo 'checked=checked;' ?>>
                        <label for="html">9</label>
                        <input type="radio" name="pagina" value="10" <?php if ($pagina == '10') echo 'checked=checked;' ?>>
                        <label for="html">10</label>

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
                    $acao = <<<ACAO
                <div style="min-width: 20px;">
                    <input class="form-check-input" type="checkbox" value="{id}" name="selecionado_gravar[]">
                </div> 
                ACAO;
                    $arConfig = array();
                    $mCargaEditais = new Listam(array("style" => "width: 100%;"));
                    $dado = array_map("utf8_decode", $dados);
                    $mCargaEditais->setCabecalho(array(
                        'Código Externo',
                        'Portal',
                        'Data Limite',
                        'Orgão',
                        'UASG',
                        'Pregão',
                        'Código Origem',
                        'Título',
                        'Descrição',
                        'Publicação',
                        'Selecionar'
                    ));
                    $mCargaEditais->setCorpo($rsDadosBaixados, $arConfig);
                    $mCargaEditais->setAcao($acao);
                    $mCargaEditais->show();
                    ?>
                    <label>Categorizar como :</label>
                    
                    <?php
                    $opcoes = array(
                        array('codigo' => 'Desenvolvimento de Softwares', 'descricao' => 'Desenvolvimento de Softwares'),
                        array('codigo' => 'Manutenção / Infraetrutura de Tecnologia', 'descricao' => 'Manutenção / Infraetrutura de Tecnologia'),
                        array('codigo' => 'Contagem de PF', 'descricao' => 'Contagem de PF'),
                    );
                    echo $db->monta_radiom('categoria', $opcoes, 'S', 'S', 'Habilitação Jurídica');
                    ?>
                    <br>
                    <button type="submit" class="btn red">
                        <i class="bi bi-save"></i> Gravar selecionados
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