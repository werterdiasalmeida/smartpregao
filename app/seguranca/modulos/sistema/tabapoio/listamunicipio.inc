<?
 /*
   Sistema Simec
   Setor respons?vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   M?dulo:listamunicipio.inc
   Finalidade: permitir o controle da lista de municipios
   */
$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$sql= "select muncod as codigo, mundsc as descricao, regcod as estado, muncodibge as ibge from municipio order by regcod,mundsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista de Munic?pios Cadastrados.';
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('C?digo Interno','Descri??o','UF','C?digo IBGE');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>
