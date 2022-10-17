<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:listaorgao.inc
   Finalidade: permitir o controle da lista de programas
   */
$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$sql= "select '<a href=seguranca.php?modulo=sistema/tabapoio/listaunidade&acao=A&orgcod=' || o.orgcod || '>' || o.orgcod || '</a>' as codigo, o.orgdsc as descricao, '(' || count(*) || ')' as total from orgao o inner join unidade u on o.orgcod=u.orgcod group by o.orgcod,o.orgdsc order by o.orgdsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista Órgãos Cadastrados.';
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('Código','Descrição','Total de Unidades');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>
