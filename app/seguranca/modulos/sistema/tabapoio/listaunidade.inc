<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:listunidade.inc
   Finalidade: permitir o controle da lista de unidades
   */

$modulo=$_REQUEST['modulo'] ;//

include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$sql= "select u.unicod as codigo, u.unidsc as descricao, o.orgdsc from unidade u left join orgao o on u.orgcod = o.orgcod ";
if ($_REQUEST['orgcod'] ) {
    $sql = $sql . "  where u.orgcod ='".$_REQUEST['orgcod']."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
}
$sql = $sql. "  order by u.unidsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista de Unidades cadastradas';
 if ($_REQUEST['orgcod'] ) $titul .= ' Subordinadas a(o) '.$orgdsc;
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('Código','Descrição','Órgão Superior');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>

