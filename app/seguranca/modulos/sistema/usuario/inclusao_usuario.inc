<? 
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Alexandre Soares Diniz
   Programador: Alexandre Soares Diniz
   Módulo:inclusao_usuario.inc
   Finalidade: permitir a inclusão de usuários
 */

 include "config.inc";
 include APPRAIZ."includes/classes_simec.inc";
 include APPRAIZ."includes/funcoes.inc";
 $db = new cls_banco();
 include "cabecalho.php";

  
//Recarrega variávei postadas
$codigo = $_POST['codigo'];	
$usucpf = $_POST['usucpf'];	
$usunome = $_POST['usunome'];
$usuemail = $_POST['usuemail'];
$usufoneddd = $_POST['usufoneddd'];
$usufonenum = $_POST['usufonenum'];
$orgcod = $_POST['orgcod'];
$usufuncao = $_POST['usufuncao'];
$unicod = $_POST['unicod'];
$regcod = $_POST['regcod'];
$ususexo = $_POST['ususexo'];

//variaveis de controle, usado na auditoria.
$_SESSION['usucpf'] = corrige_cpf($_REQUEST['usucpf']);
$_SESSION['usucpforigem'] = corrige_cpf($_REQUEST['usucpf']);
$_SESSION['mnuid'] = 10;
$_SESSION['sisid']=4;



//Verifica se o usuario esta cadastrado
$sql = "select usunome from usuario where usucpf = '".corrige_cpf($usucpf)."'";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS); 
if ($nlinhas >= 0) {
    $res = $db->carrega_registro($RS,0);
    if(is_array($res)) {foreach($res as $k=>$v) {${$k}=$v;} $habil='N';} 
} else $habil='S';

 
//Pesquisa se o usuário já está cadastrado no sistema solicitado
$sql = "select count(*) as total from usuario_sistema where usucpf = '".corrige_cpf($usucpf)."' and sisid = ".$codigo;
$total = $db->pegaUm($sql);
if ($total > 0) $dados_sistema =  'N'; else $dados_sistema = 'S';



if (($_REQUEST['act'] == 'inserir') and (! is_array($msgerro)))
{

	
	if ($habil=='S')
	{
    	// obter os dados da instituição
    	$sql = "select ittemail_inclusao_usuario, ittemail, itttelefone1, itttelefone2, ittddd, ittfax from instituicao where ittstatus = 'A'";
    	$saida = $db->recuperar($sql);
    	if(is_array($saida)) {
		foreach($saida as $k=>$v) ${$k}=$v;}
    	// fazer inserção de usuário na base de dados.
    	$senha = senha();
    	$sql = "insert into usuario (usucpf,usunome, usuemail, usustatus, usufoneddd, usufonenum, usufuncao, orgcod, unicod, usuchaveativacao,regcod,ususexo,usuobs,ungcod,ususenha) values (".
    	"'".corrige_cpf($_REQUEST['usucpf'])."',".
    	"'".str_to_upper($_REQUEST['usunome'])."',".
    	"'".$_REQUEST['usuemail']."',".
    	"'X',".
    	"'".$_REQUEST['usufoneddd']."',".
    	"'".$_REQUEST['usufonenum']."',".
    	"'".$_REQUEST['usufuncao']."',".
    	"'".$_REQUEST['orgcod']."',".
    	"'".$_REQUEST['unicod']."',".
    	"'f',".
    	"'".$_REQUEST['regcod']."',".
    	"'".$_REQUEST['ususexo']."',".
    	"'".$_REQUEST['usuobs']."',".
    	"'".$_REQUEST['ungcod']."',".
    	"'".md5_encrypt($senha,'')."')";
 		//print $sql."<br>";
 		$db->executar($sql);
    	$db -> commit();
	}
	
	// Adiciona include de inserção de acordo com o sistema
	 if (($codigo != "") and ($dados_sistema =='S'))
 	{
 		$diretoriochecar = "";
 		$sql = "select sisdiretorio from sistema where sisid= ".$_REQUEST['codigo'];
		
 		$diretoriochecar = $db->pegaUm($sql);
 		if ($diretoriochecar != "")
		{
			include (APPRAIZ."/".$diretoriochecar."/modulos/sistema/usuario/incusuariosql.inc");
		}
 	}
 	
	
		
   
    // envia email
    $assunto = 'Inscrição no cadastro';
	$sexo = 'Prezado Sr.  ';
	if ($_REQUEST['ususexo'] == 'F') $sexo = 'Prezada Sra. ';
        $mensagem = $sexo. strtoupper($_REQUEST['usunome']).',<br><br>'.$ittemail_inclusao_usuario.' '.$ittemail.' ou nos telefones:'.$ittddd.' - '.$itttelefone1.' ou '.$itttelefone2. ' Fax '.$ittfax.'<br><br>';
        email(strtoupper($_REQUEST['usunome']), $_REQUEST['usuemail'], $assunto, $mensagem);
        email('Administrador-UMA',$GLOBALS["email_sistema"],'Solicitação de cadastro','O usuário <br> CPF:'.corrige_cpf($_REQUEST['usucpf']).'  '.str_to_upper($_REQUEST['usunome']).'<br>E-mail:'.$_REQUEST['usuemail'].'<br>Telefone: '.$_REQUEST['usufoneddd'].'-'.$_REQUEST['usufonenum'].'<br>Órgão:'.$registro['orgdsc'].' / '.$registro['unidsc'].' / '.$registro['ungdsc'].'<br> Acaba de solicitar sua inclusão no cadastro');
   
        ?><script>
        	alert ('Sua solicitação foi enviada com sucesso, aguarde contato do administrador do sistema.');
        	location.href=".";
	    </script><?
  		exit();
}

	
	
	

	 
//include APPRAIZ."includes/cabecalho.inc";    
?>
<br>
<?
$titulo_modulo='Ficha de Solicitação de Cadastro de Usuários';
$subtitulo_modulo='Preencha os Dados Abaixo e clique no botão: "Enviar Solicitação".<br>'.obrigatorio().' Indica Campo Obrigatório.';
monta_titulo($titulo_modulo,$subtitulo_modulo);
?>
<table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" style="border: 1px Solid Silver; background-color:#f5f5f5;">
<form method="POST" name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="usuarionaocadastrado" value="<?=$habil?>">
<input type=hidden name="act" value=0>
<tr bgcolor="#F2F2F2">
    <td align = 'right' class="subtitulodireita">Sistema:</td>
    <td >
    <?	  
        $sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A'";
        $db->monta_combo("codigo",$sql,'N',"Selecione o sistema desejado",'','');
    ?>
  &nbsp;<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigat&oacute;rio.'> </td>
  </tr>    
<tr>
        <td align='right' class="subtitulodireita">CPF:</td>
	<td>
		<? $obrig='S';?>
		<?=campo_texto('usucpf',$obrig,'N','',19,14,'###.###.###-##','');?>
	</td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Nome completo:</td>
        <td>
		<?=campo_texto('usunome','S',$habil,'',50,50,'','');?>
	    </td>
      </tr>
      <? 
      //caso cpf já existe, oculta os dados pessoais - inicio
      if ($habil == 'S') {
      ?>
      <tr>
        <td align = 'right' class="subtitulodireita">Sexo:</td>
        <td>
                <input type="radio" name="ususexo" value="M" <?=($ususexo=='M'?"CHECKED":"")?>>  Masculino
                &nbsp;<input type="radio" name="ususexo" value="F" <?=($ususexo=='F'?"CHECKED":"")?>> Feminino
         <?=obrigatorio();?>
         </td>
       </tr>
      
      <tr>
        <td align = 'right' class="subtitulodireita">Orgão:</td>
        <td >
	<?$sql = "select orgcod as CODIGO,orgcod||' - '||orgdsc as DESCRICAO from orgao order by orgdsc ";
	  $db->monta_combo("orgcod",$sql,'S',"Selecione o órgão",'atualizaComboUnidade()','');
	 print obrigatorio();?></td>
      </tr>
	<?if ($orgcod) {?>
      <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">Unidade Orçamentária (UO):</td>
         <td >
	<?
	  $sql = "select unicod as CODIGO,unicod||' - '||unidsc as DESCRICAO from unidade where unistatus='A' and unitpocod='U' and orgcod ='".$orgcod."' order by unidsc ";
	  $db->monta_combo("unicod",$sql,'S',"Selecione a Unidade Orçamentária",'atualizaComboUnidade','');
	   print obrigatorio();
	?>
	</td>
      </tr>
	  <?}?>
	  	  <?
	  if ($unicod == '26101' and $orgcod=='26000') {?>

      <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">Unidade Gestora (UG):</td>
         <td >
	<?
	  $sql = "select ungcod as CODIGO,ungcod||' - '||ungdsc as DESCRICAO from unidadegestora where ungstatus='A' and unitpocod='U' and unicod ='".$unicod."' order by ungdsc ";
	  $db->monta_combo("ungcod",$sql,'S',"Selecione a Unidade Gestora",'','');
	   print obrigatorio();
	?>
	</td>
      </tr>
	  <?}?>
    <tr bgcolor="#F2F2F2">
        <td align = 'right' class="subtitulodireita">UF do órgão:</td>
        <td >
	<?
	  $sql = "select regcod as codigo, regcod||' - '||descricaouf as descricao from uf where codigoibgeuf is not null order by 2";
	  $db->monta_combo("regcod",$sql,'S',"Selecione a UF",'','');
	  print obrigatorio();
	?>
	</td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Telefone (DDD) + Telefone:</td>
        <td>
		<?=campo_texto('usufoneddd','','','',3,2,'##','');?>
		<?=campo_texto('usufonenum','S','','',18,15,'###-####|####-####','');?>
	</td>
      </tr>
      <tr >
        <td align = 'right' class="subtitulodireita">Seu E-Mail:</td>
        <td ><?=campo_texto('usuemail','S','','',50,100,'','');?></td>
      </tr>
      <tr >
        <td align = 'right' class="subtitulodireita">Confirme o Seu E-Mail:</td>
        <td ><?=campo_texto('usuemail_c','S','','',50,100,'','');?><br>
		<font color="#006666">Obs: O Campo E-Mail é para uso individual. <b>Não utilizar e-mails coletivos</b>. Utilizar PREFERENCIALMENTE e-mail funcional.</font></td>
      </tr>
      <tr>
        <td align='right' class="subtitulodireita">Função/Cargo:</td>
        <td>
		<?=campo_texto('usufuncao','S','','',50,100,'','');?>
	    </td>
      </tr>
		<tr>
        <td align='right' class="subtitulodireita">Observações:</td>
        <td>
		<?=campo_textarea('usuobs','N','S','',100,3,'');?><br>
		<font color="#006666">Se desejar, informe acima Observações: Ex.: motivo do seu cadastramento, suas atribuições, etc...</font>
	    </td>
      </tr>
      <?}
     //caso cpf já existe, oculta os dados pessoais - fim
     
     
     
     //Oculta dados do sistema caso já exista uma solicitação.
 	if ($dados_sistema == 'S')
 	{
 		
 		// Exibe os dados de acordo com o sistema - incio
    	//dados perfil desejado
 	
 	
 		if ($codigo != "")
 		{
	 		$diretoriochecar = "";
	 		$sql = "select sisdiretorio from sistema where sisid= ".$_REQUEST['codigo'];
			$diretoriochecar = $db->pegaUm($sql);
	 		if ($diretoriochecar != "")
			{
				include (APPRAIZ."/".$diretoriochecar."/modulos/sistema/usuario/incusuario.inc");
			}
 		}
 	?>
 	<tr bgcolor="#C0C0C0">
 		<td></td>
    	<td><input type="button" name="btinserir" value="Enviar Solicitação"  onclick=<? print "\"validar_cadastro('I','".$habil."')\"";?>>&nbsp;&nbsp;&nbsp;<input type="Button" value="Voltar" onclick="history.back();"></td>
 	</tr>
 	<?
 	}else 
 	{?>
 	<tr bgcolor="#C0C0C0">
 		<td></td>
		<td><b>Já existe uma solicitação deste usuário para este sistema, aguarde contato do administrador.</b></td>
 	</tr>
 	<tr bgcolor="#C0C0C0">
 		<td></td>
 		<td><input type="Button" value="Voltar" onclick="history.back();"></td>
 	</tr>
 	<?}
	?>
 </form>
</table>
<br>
 

<script>
function atualizaComboUnidade() {
	 document.formulario.submit();
}
	
function validar_cadastro(cod,sistema) {
		
    	if (sistema != 'N')
    	{
    		if (!validaBranco(document.formulario.usucpf, 'CPF')) return;
			if (! DvCpfOk(document.formulario.usucpf))
			{
		    	document.formulario.usucpf.focus();
		    	return;
			}
			if (!validaBranco(document.formulario.usunome, 'Nome')) return;
			if (!validaRadio(document.formulario.ususexo,'Sexo')) return;
			if (!validaBranco(document.formulario.regcod, 'UF')) return;
			if (!validaBranco(document.formulario.orgcod, 'Órgão')) return;
			if (document.formulario.unicod.options[1].value){if (!validaBranco(document.formulario.unicod, 'Unidade Orçamentária (UO)')) return;}
			if (document.formulario.ungcod){if (!validaBranco(document.formulario.ungcod, 'Unidade Gestora (UG)')) return;}
			if (!validaBranco(document.formulario.usufoneddd, 'DDD')) return;
			if (!validaBranco(document.formulario.usufonenum, 'Telefone')) return;
			if (!validaBranco(document.formulario.usuemail, 'Email')) return;
			if (document.formulario.usuemail_c.value != document.formulario.usuemail.value)
        	{
            	alert ("A confirmação do E-mail não coincide!. Verifique o E-mail.");
            	document.formulario.usuemail.setfocus();
            	return;
        	}
		
			if(! validaEmail(document.formulario.usuemail.value))
			{
				alert("Email Inválido.");
				document.formulario.usuemail.focus();
				return;
			}
       		if (!validaBranco(document.formulario.usufuncao, 'Função/Cargo')) return;
    	}
	   		
		if (cod == 'I') document.formulario.act.value = 'inserir'; else document.formulario.act.value = 'alterar';
		document.formulario.submit();
     }
	 


</script>
</body>
</html>
