<?php
include "config.inc";
header('Content-Type: application/json; charset=iso-8859-1');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$tipo = $_REQUEST['tipo'];
if (!$tipo) $tipo = 'pregao_e_empresas';
$filtro = $_REQUEST['filtrar'];
if (!$filtro) $filtro = 'true';

switch ($tipo) {
    case 'pregao_e_empresas':

        $sql = "select distinct * 
                from cargas.tb_carga_editais_captacao ed
                join cargas.tb_carga_estabelecimento_pregao pr on pr.prgcod::integer = ed.codigo::integer 
                join cargas.tb_carga_estabelecimento es on es.cnpj = pr.cnpj
                where codigo is not null and codigo <>'' AND {$filtro}";

        break;
}

$result = $db->carregarArray($sql);
$saida .= "[";
foreach ($result as $item) {
    $saida .= "[{";
    foreach ($item as $key => $value) {

        $saida .= " \"{$key}\": \"{$value}\" ,";
    }
    $saida = substr($saida, 0, -1);
    $saida .= "}],";
}
$saida = substr($saida, 0, -1);
$saida .= "]";

echo $saida;
