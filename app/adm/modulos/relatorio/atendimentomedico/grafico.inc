<?php
ini_set("memory_limit", "1024M");

$sql = monta_sql();
$dados = $db->carregar($sql);

$arrumado =array();

ver($_REQUEST, $dados);


function montra_grafico(&$arrumado, $dados, $agrupador, $profundidade = 0){
$niveis = count($agrupador) - $profundidade;
$profundidade++;


}
