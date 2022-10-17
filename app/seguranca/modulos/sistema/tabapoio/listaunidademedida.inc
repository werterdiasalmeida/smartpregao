<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listunidade.inc
   Finalidade: permitir o controle da lista de unidades
   */

$modulo=$_REQUEST['modulo'] ;//
$sql= "select u.unicod as codigo, u.unidsc as descricao, o.orgdsc from unidade u left join orgao o on u.orgcod = o.orgcod ";
if ($_REQUEST['orgcod'] ) { 
    $sql = $sql . "  where u.orgcod ='".$_REQUEST['orgcod']."'";
    $res = $db->recuperar($sql);
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
}
$sql = $sql. "  order by u.unidsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
include APPRAIZ."includes/cabecalho.inc";
?>
<title>Lista de Unidades</title>

<body>

<div align="center">
<table  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr align="center">
    <td width="100%"><strong>Lista de Unidades (<?=$nlinhas+1?>)
    <? if ($_REQUEST['orgcod'] ) print ' Subordinadas a(o) '.$orgdsc; ?>
    </strong>
    </td>
  </tr>
</table>
<table  align='center'  width='100%' border="0" cellspacing="0" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
  <tr>
    <td width="100%"><font color=red><strong><?print $_SESSION['msgsistema']; $_SESSION['msgsistema'] = '' ?></strong></font></td>
  </tr>
</table>
<table width='100%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"><strong>Código</strong></td>
      <td valign="top" class="title"><strong>Descrição</strong></td>
	<td valign="top" class="title"><strong>Órgão superior</strong></td>
      
    </tr>
</thead>
<tbody>
<?
for ($i=0; $i<=$nlinhas;$i++){
  $res = $db->carrega_registro($RS,$i);
// a linha abaixo transforma em variáveis todos os campos do array
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
     print '<tr class="'.$marcado.'"><td><b>'.$codigo.'</b></td><td>'.$descricao.'</td>';
     print '<td>'.$orgdsc.'</td></tr>';
}
?>
</tbody>
</table>

      </td>
    </tr>
  </table>
  </center>
</div>
