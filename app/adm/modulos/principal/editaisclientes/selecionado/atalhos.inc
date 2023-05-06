<?php
// Gera o link para a agenda
//ver($dados);
$agAssunto = " [PREGÃO] " . $orgao;
$agAssunto = removeAcentos($agAssunto);
$agAssunto = strtoupper($agAssunto);
$agLink = 'Pregão Eletrônico';
$agInicio = $data_limite . 'T' . $hora_limite;
$agInicio = str_replace('-', '', $agInicio);
$agInicio = str_replace(':', '', $agInicio);
$hora_fim = explode(':', $hora_limite);
$hora_fim = $hora_fim[0];
$agFim = $data_limite . 'T' . $hora_fim . ':59';
$agFim = str_replace('-', '', $agFim);
$agFim = str_replace(':', '', $agFim);
$url = "https://calendar.google.com/calendar/u/0/r/eventedit?dates={$agInicio}/{$agFim}&text={$agAssunto}&add=werter@gmail.com,malosso@gmail.com&location=http://sistema.smartpregao.com.br/";
$link =  "<br><br> <a class=\"btn btn-success\"  href=\"{$url}\" target=\"_blank\"> <i class=\"fa fa-calendar\"></i> Criar Agenda para o Pregão</a>";
echo $link . '<br/>';

if ($dados['link_g_drive']) {
    echo "<br><a class='btn btn-warning' href=\"{$dados['link_g_drive']}\" target=\"_blank\"> <i class=\"bi bi-google\"></i> Google Drive</a>";
}

switch ($dados['portal']) {
    case 'ComprasNet':
        $num_pregao = preg_replace('/[^0-9]/', '', $dados['edital']);
        $num_uasg = substr($num_pregao, -6);
        $num_pregao = substr($num_pregao, 0, -6);
        $linkDownload_comprasnet = "https://www.comprasnet.gov.br/ConsultaLicitacoes/Download/Download.asp?coduasg={$num_uasg}&numprp={$num_pregao}&modprp=5&bidbird=N&idLetra=us2gcz&idSom=&Submit=Confirmar";
        $link = "<br><br>
                                    <a class='btn btn-primary' href=\"{$linkDownload_comprasnet}\" target=\"_blank\"> <i class=\"bi bi-cloud\"></i> Download ComprasNet</a>";
        echo $link;
        $linkDownload_comprasnet = "http://www.comprasnet.gov.br/livre/Pregao/lista_pregao.asp?Opc=2&txtlstUasg=&rdTpPregao=E&lstSrp=T&lstICMS=T&lstSituacao=5&lstTipoSuspensao=0&uf=&co_uasg={$num_uasg}&numprp={$num_pregao}&dt_entrega=&dt_abertura=";
        $link = "<br><br>
                                    <a class='btn btn-primary' href=\"{$linkDownload_comprasnet}\" target=\"_blank\"> <i class=\"bi bi-search\"></i> Acompanhar no ComprasNet</a>";
        echo $link;
        $linkDownload_comprasnet = "http://comprasnet.gov.br/acesso.asp?url=/livre/pregao/ata0.asp?co_uasg={$num_uasg}&numprp={$num_pregao}";
        $link = "<br><br>
                                    <a class='btn btn-primary' href=\"{$linkDownload_comprasnet}\" target=\"_blank\"> <i class=\"bi bi-activity\"></i> ATA 0 do ComprasNet</a>";
        echo $link;
        break;
    case 'BB':
        $num_pregao = preg_replace('/[^0-9]/', '', $dados['edital']);
        $link =  '<br><br> <a class="btn btn-primary"  href="https://www.licitacoes-e.com.br/aop/consultar-detalhes-licitacao.aop?numeroLicitacao=' . $num_pregao . '&opcao=consultarDetalhesLicitacao" target="_blank"> <i class="fa fa-cloud"></i> Licitações-e (BB)</a>';
        echo $link;
        break;
}

if ($dados['id_externo']) {
    $link =  '<br><br> <a class="btn btn-danger"  href="https://app.sigapregao.com.br/oportunidade/' . $dados['id_externo'] . '" target="_blank"> <i class="fa fa-cloud"></i> Siga Pregão</a>';
    echo $link;
}


?>
<br /><br />