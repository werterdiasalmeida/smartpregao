<?php
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
?>
<br /><br />