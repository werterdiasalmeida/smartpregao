<? 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:associa_usuario.inc
   Finalidade: permitir a associação de um perfil ao usuario
   */
$modulo=$_REQUEST['modulo'] ;//
if ($_REQUEST['act']=='cadastra' and $_POST['usucpf']<>'') {
  $sql = 'SELECT p.*, s.sisdsc, s.sisid 
			FROM perfil AS p INNER JOIN sistema AS s ON p.sisid = s.sisid  
			ORDER BY s.sisdsc, p.pfldsc;';
  $RS = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RS);
  if ($nlinhas >= 0)
   {
   	
   	 $sisid_chk = 'inicio';
     for ($i=0; $i<=$nlinhas;$i++)
     {
       $res = $db->carrega_registro($RS,$i);
       // a linha abaixo transforma em variáveis todos os campos do array
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       if ($_POST[$pflcod]<>'')
       {
         // então marcou a opção
         // procurar se já foi marcada antes. se não foi, então inserir.
           $sql = 'select * from perfilusuario where pflcod='.$pflcod." and usucpf='".$_POST['usucpf']."'";
            $RS2 = $db->record_set($sql);
            $nlinhas2 = $db->conta_linhas($RS2);
            if ($nlinhas2 == -1) {
              // incluir
                $sql = 'insert into perfilusuario (pflcod, usucpf) values ('.$pflcod.",'".$_POST['usucpf']."')";
                 $saida = $db->executar($sql);
            }
       } else
       {
         // caso não tenha marcado, procurar se existe no banco. Se existir, então apagar
            $sql = 'select * from perfilusuario where pflcod='.$pflcod." and usucpf='".$_POST['usucpf']."'";
            $RS3 = $db->record_set($sql);
            $nlinhas3 = $db->conta_linhas($RS3);
            if ($nlinhas3 == 0)
            { // excluir
                 $sql = 'delete from perfilusuario where pflcod='.$pflcod."  and usucpf='".$_POST['usucpf']."'";
                 $saida = $db->executar($sql);
            }
       }
       
       
       
       //verifica se o acesso do sistema foi marcado
       if ($sisid != $sisid_chk)
       {
       	   
       	
       		if 	($_POST['rd_sis'.$sisid] != '')
       		{
	       	   $sql = "select count(*) as total from usuario_sistema us left join sistema s on us.sisid = s.sisid where s.sisstatus = 'A' and s.sisid=".$sisid." and us.usucpf='".$_POST['usucpf']."'";
	           $nlinhas4 = $db->pegaum($sql);
	
	           if ($nlinhas4 >= 0)
	           { 
	           		$sql = "UPDATE usuario_sistema SET suscod = '".$_POST['rd_sis'.$sisid]."' 
							WHERE (((usucpf)='".$_POST['usucpf']."') AND ((sisid)=".$sisid."));";
				
	           		//$sql = 'delete from perfilusuario where pflcod='.$pflcod."  and usucpf='".$_POST['usucpf']."'";
	           		//substr(teste,1) = este
	           } 
	           else 
	           {
	           		$dtatual = getdate();           
	           		$sql = "INSERT INTO usuario_sistema ( usucpf, sisid, suscod,susdataultacesso ) values ('".$_POST['usucpf']."',".$sisid.",'".$_POST['rd_sis'.$sisid]."','".$dtatual['year'].'-'.$dtatual['mon'].'-'.$dtatual['mday']."');";
	           		
	           }
	       	                            
	           $saida = $db->executar($sql);
       		}
           $sisid_chk = $sisid;
       	   //print $sql;
       }
       
        
           
     }

     $db -> commit();    
 	
     //$db->sucesso($modulo.'&acao=A&usucpf='.$_POST['usucpf']);
	print '<script language="JavaScript">window.parent.opener.location.reload();self.close();</script>';
   }
}
?>
<html>
<head>
<META http-equiv="Pragma" content="no-cache">
<title>Perfil do Usuário</title>
<script language="JavaScript" src="includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script type="text/javascript">self.focus();</script>
</head>
<body LEFTMARGIN="0" TOPMARGIN="0" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">
<table  align="center" width='100%' border="0" cellspacing="0" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center'>
  <tr>
    <td width="100%"><strong>Associação de Usuário aos Perfis</strong></td>
  </tr>
</table>
<?
    $usucpf =$_REQUEST['usucpf'];
	$sql = "select usucpf, usunome from usuario where usucpf='".$_REQUEST['usucpf']."'";
    $saida = $db->recuperar($sql);
        if(is_array($saida)) 	   foreach($saida as $k=>$v) ${$k}=$v;
	?>
<form method="POST" name="formulario">
<input type=hidden name="act" value=0>
<input type=hidden name='usucpf' value='<?=$usucpf?>'>
<table width='100%' align="center">
<tr><td>Selecione o Perfil Para o Usuário: <strong><?=$usunome?></strong>
</td></tr></table>
<table  width='100%' border="0" cellspacing="0" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;" align='center' >
<?
// listar apenas os perfis que são de nível menor do que o do usuário logado
// isto impede que ele possa cadastrar alguém com um perfil superior ao dele
	//$sql = "select p.pflnivel from perfil p inner join perfilusuario pu on pu.pflcod=p.pflcod and pu.usucpf='".$_SESSION['usucpf']."' order by p.pflnivel";
	//$RS = $db->record_set($sql);
    //$res = $db->carrega_registro($RS,0);

  //$sql = "select * from perfil where pflstatus='A' and pflnivel >= ".$res[0]." order by pfldsc";
  //FROM perfil AS p INNER JOIN sistema AS s ON p.sisid = s.sisid  
  $sql = "SELECT p.*, s.sisdsc, s.sisid 
			FROM sistema AS s Left JOIN perfil AS p ON p.sisid = s.sisid  
  			WHERE p.pflstatus='A' and p.pflnivel >= ".$_SESSION['usunivel']." ORDER BY s.sisdsc, p.pfldsc;";
  $RS = $db->record_set($sql);
  $nlinhas = $db->conta_linhas($RS);
  if ($nlinhas >= 0)
  {
  	$exibe_titulo = "";
  	
    for ($i=0; $i<=$nlinhas;$i++)
    {
    // monta a matriz de opções
       $res = $db->carrega_registro($RS,$i);
       // a linha abaixo transforma em variáveis todos os campos do array
       if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
       
       
       //verifica se já foi marcado o status no sistema
       $sql = "select suscod from usuario_sistema where sisid=".$sisid." and usucpf='".$usucpf."'";
       $suscod = $db->pegaUm($sql);
                
       
       // verifica se já foi marcado o perfil
       $sql = "select * from perfilusuario where pflcod=".$pflcod." and usucpf='".$usucpf."'";
       $RSC = $db->record_set($sql);
       $nlinhasC = $db->conta_linhas($RSC);
       if ($nlinhasC >= 0) $cb = '  checked ' ; else $cb = '';
                     
       //imprime titulo do sistema acima dos perfis especificos
       if ($exibe_titulo != $sisdsc)
       {
       		//1 linha é ignorada
       		if ($i!=0) print '<tr><td>&nbsp</td></tr>';
	   		      		      		
       		print '<tr><td align=\'center\'><strong>'.$sisdsc.'</strong></td></tr>
	   		       <tr><td align=\'left\' class="SubTituloEsquerda">Status:</td></tr>
        		   <tr><td>
            			<input type="radio" name="rd_sis'.$sisid.'" value="A" '. ($suscod=='A'?'CHECKED':'').'> Ativo
            			<input type="radio" name="rd_sis'.$sisid.'" value="P" '. ($suscod=='P'?'CHECKED':'').'> Pendente
            			<input type="radio" name="rd_sis'.$sisid.'" value="B" '. ($suscod=='B'?'CHECKED':'').'> Bloqueado
            			
        			</td></tr>
        			<tr><td align=\'left\' class="SubTituloEsquerda">Perfis Permitidos:</td></tr>';
	   		 print $status;
	   		$exibe_titulo = $sisdsc;
       }
       print '<tr><td><input type="checkbox" name="'.$pflcod.'" value="'.$pflcod.'"'.$cb.' >'.$pfldsc.'</td></tr>';
     }
     
  }
  else
  {
    print '<tr><td></td><td>Não há registros de Perfil</td></tr>';
  }
?>
<tr bgcolor="#cccccc"><td align="right"><input type='button' value='OK!' onclick="validar_cadastro()">

</td></tr>
</table>
</form>
</div>
<script language="JavaScript">
  function validar_cadastro() {
    document.formulario.act.value = 'cadastra';
	document.formulario.submit();
  }
  function submete_perfil(cod){
    if (document.formulario.pflcod.value != '')
    {
        document.formulario.submit();
    }
  }

</script>
