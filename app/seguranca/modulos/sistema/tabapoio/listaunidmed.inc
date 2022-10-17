<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:listaunidmed.inc
   Finalidade: permitir o controle da lista de unidades de medida
   */

$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$sql= "select u.unmcod as codigo, u.unmdsc as descricao from unidademedida u  order by u.unmdsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista de Unidades de Medida';
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('Código','Descrição');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>
