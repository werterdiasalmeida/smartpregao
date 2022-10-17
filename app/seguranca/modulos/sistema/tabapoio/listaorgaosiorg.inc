<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:listaorgaosiorg.inc
   Finalidade: permitir o controle da lista de orgaos do siorg
   */


$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$sql= "select osicod as codigo, osinome as descricao, orgcod from orgaosiorg order by osinome";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista de Órgãos no Cadastro Siorg';
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('Código SIORG','Descrição','Código');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>
