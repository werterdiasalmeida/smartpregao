<? 

/*
   Sistema Simec
   Setor responsável: DTI-MEC
   Desenvolvedor: Equipe Simec
   Analista: Luiz Fernando Carvalho Mendonça
   Programador: Felipe Evangelista dos Santos
   Módulo:cadastraGestores.inc
   Finalidade: permitir o cadastro dos Gestores
   */

include APPRAIZ . "includes/cabecalho.inc";
print "<br/>";

$titulo_modulo	= 'Cadastro de Gestores';
$subtitulo 		= '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';

monta_titulo($titulo_modulo, $subtitulo);
	
$sisid = $_REQUEST['sisid'];

if($_REQUEST["submetido"]) {
	// delete
	if($_REQUEST["excluir"]) {
		$sql = "DELETE FROM 
   					seguranca.gestores 
   				WHERE
		   			gesid = ".$_REQUEST ["excluir"];
		$msg = "excluídos";
	}
	// update
	else if($_REQUEST["alterar"]) {
		$sql = "UPDATE 
		   			seguranca.gestores 
		   		SET
				    gesnome 	= '".pg_escape_string($_REQUEST['gnome'])."',
				  	gesemail 	= '".pg_escape_string($_REQUEST['gemail'])."',
				  	gestelefone = '".pg_escape_string($_REQUEST['gtelefone'])."',
				  	gescelular  = '".pg_escape_string($_REQUEST['gcelular'])."',
				  	gesfax 		= '".pg_escape_string($_REQUEST['gfax'])."'
			  	WHERE
			  		gesid 		= ".$_REQUEST['alterar'];
		$msg = "alterados";
	}
	// insert
	else {
		$sql = "INSERT INTO 
   					seguranca.gestores 
	   				(
				 	 gesnome,
				  	 gesemail,
				  	 gestelefone,
				  	 gescelular,
				  	 gesfax,
				  	 sisid
		  	 		)
	  	 		VALUES
			  		(
			  	 	 '".pg_escape_string($_REQUEST['gnome'])."',
				  	 '".pg_escape_string($_REQUEST['gemail'])."',
				  	 '".pg_escape_string($_REQUEST['gtelefone'])."',
				  	 '".pg_escape_string($_REQUEST['gcelular'])."',
				  	 '".pg_escape_string($_REQUEST['gfax'])."',
				  	 ".$sisid."
				 	)";
		$msg = "inseridos";
	}
	
	$db->executar($sql);
	$db->commit();
	echo '<script> alert("Dados '.$msg.' com sucesso."); </script>';
}

?>

<form method="post" id="formulario" action="">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="5" align="center">
	<tr>
		<td bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Sistema:</td>
		<td>
		<?
			$sql = "SELECT 
	    				sisid as codigo, 
	    				sisabrev as descricao
	    			FROM 
	    				seguranca.sistema
	    			ORDER BY
	    				sisabrev ASC";
			$db->monta_combo("sisid", $sql, 'S', "Selecione...", 'submeteSistema', '', '', '300', 'S','');
		?>
		</td>
	</tr>
</table>
<br />

<?
if( $_REQUEST["sisid"]  ) {
?>

<input type='hidden' name="submetido" value="1">
<input type='hidden' name="alterar" value="">
<input type='hidden' name="excluir" value="">

<center>
    <table width='95%' id="tabelaCadastroGestores" align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
		<thead>
		    <tr>
		      <td valign="top" align="center" class="title"><strong>Nome do Gestor</strong></td>     
		      <td valign="top" align="center" class="title"><strong>Email do Gestor</strong></td>
		      <td valign="top" align="center" class="title"><strong>Telefone do Gestor</strong></td>
		      <td valign="top" align="center" class="title"><strong>Celular do Gestor</strong></td>
		      <td valign="top" align="center" class="title"><strong>Fax do Gestor</strong></td>
		      <td valign="top" align="center" class="title"><strong>Ação</strong></td>
		    </tr>
		</thead>
		<tbody>
			<tr bgcolor="#c0c0c0">
		    	<td align='center'>
		    		<?=campo_texto('gnome','S','S','',50,100,'','');?>
		    	</td>
		        <td align='center'>
		        	<?=campo_texto('gemail','S','S','',50,100,'','');?>
		        </td>
		        <td align='center'>
		        	<?=campo_texto('gtelefone','S','S','',20,25,'##-####-####','');?>
		        	</td>
		        <td align='center'>
		        	<?=campo_texto('gcelular','S','S','',20,25,'##-####-####','');?>
		        </td>
		        <td align='center'>
		        	<?=campo_texto('gfax','S','S','',20,25,'##-####-####','');?>
		        </td>
		       	<td align='center'>
		       		<img src="/imagens/gif_inclui.gif" border="0" title="Incluir" style="cursor:pointer;" onclick="incluirGestor();">
					<img src="/imagens/excluir.gif" border="0" title="Cancelar" style="cursor:pointer;" onclick="form.reset();">
		       	</td>
			</tr>
			<?
				$sql= "select  
							gesid as gid, 
							gesnome as nome, 
							gesemail as email, 
							gestelefone as telefone, 
							gescelular as celular, 
							gesfax as fax, 
							sisid as sid 
					   from 
					   		seguranca.gestores
					   where
					   		sisid = ".$sisid."
					  order by
					  		gesnome ASC";
				$dadosGestores = $db->carregar($sql);
								
				if($dadosGestores) {
					for($i=0; $i<count($dadosGestores); $i++) {					  		
						$cor = (fmod($i,2) == 0) ? '#f4f4f4' : '#e0e0e0';
						
			  			print '<tr bgcolor="'.$cor.'">
			  					 <td>'.$dadosGestores[$i]["nome"].'</td>
			  					 <td>'.$dadosGestores[$i]["email"].'</td>
			  					 <td>'.$dadosGestores[$i]["telefone"].'</td>
			  					 <td>'.$dadosGestores[$i]["celular"].'</td>
			  					 <td>'.$dadosGestores[$i]["fax"].'</td>
			  					 <td align="center">
			  					 	<img border="0" src="../imagens/alterar.gif" title="Editar" style="cursor:pointer;" onclick="alterarCadastroGestor(this.parentNode.parentNode.rowIndex, '.$dadosGestores[$i]["gid"].', \''.$dadosGestores[$i]["nome"].'\', \''.$dadosGestores[$i]["email"].'\', \''.$dadosGestores[$i]["telefone"].'\', \''.$dadosGestores[$i]["celular"].'\', \''.$dadosGestores[$i]["fax"].'\');" />
			  					 	<img border="0" src="../imagens/excluir.gif" title="Excluir" style="cursor:pointer;"  onclick="excluirCadastroGestor('.$dadosGestores[$i]["gid"].')" />
			  					 </td>
			  				   </tr>';
					} 
				}
			?>
		</tbody>
	</table>
</center>
<? } ?>  
</form>

<script>

var form = document.getElementById("formulario");

function submeteSistema(value) {
	location.href = "seguranca.php?modulo=sistema/cadastro/cadastraGestores&acao=A&sisid=" + value;
}

function incluirGestor() {
	if( validarFormulario() ) {
		form.submit();	
	}
}

function validarFormulario() {
	var gnome 		= document.getElementsByName("gnome")[0];
	var gemail 		= document.getElementsByName("gemail")[0];
	var gtelefone	= document.getElementsByName("gtelefone")[0];
	var gcelular 	= document.getElementsByName("gcelular")[0];
	var gfax 		= document.getElementsByName("gfax")[0];

	if(gnome.value == "") {
		alert("O campo 'Nome do Gestor' deve ser preenchido.");
		gnome.focus();
		return false;
	}
	if(gemail.value == "") {
		alert("O campo 'Email do Gestor' deve ser preenchido.");
		gemail.focus();
		return false;
	}
	if ( !(/^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(gemail.value)) ) {
		alert("O valor campo 'Email do Gestor' está incorreto.");
		gemail.focus();
		return false;
	}
	if(gtelefone.value == "") {
		alert("O campo 'Telefone do Gestor' deve ser preenchido.");
		gtelefone.focus();
		return false;
	}
	if(gcelular.value == "") {
		alert("O campo 'Celular do Gestor' deve ser preenchido.");
		gcelular.focus();
		return false;
	}
	if(gfax.value == "") {
		alert("O campo 'Fax do Gestor' deve ser preenchido.");
		gfax.focus();
		return false;
	}

	return true;
}

function alterarCadastroGestor(index, gid, nome, email, telefone, celular, fax) {
	var tabela 		= document.getElementById("tabelaCadastroGestores");
	var alterar		= document.getElementsByName("alterar")[0];
	// campos do formulário
	var gnome 		= document.getElementsByName("gnome")[0];
	var gemail 		= document.getElementsByName("gemail")[0];
	var gtelefone	= document.getElementsByName("gtelefone")[0];
	var gcelular 	= document.getElementsByName("gcelular")[0];
	var gfax 		= document.getElementsByName("gfax")[0];

	// exclui a linha da tabela
	tabela.deleteRow(index);
	// seta o id do item a ser alterado
	alterar.value = gid;
	// preenche os campos do form
	gnome.value 	= nome;
	gemail.value 	= email;
	gtelefone.value = telefone;
	gcelular.value 	= celular;
	gfax.value 		= fax;
}

function excluirCadastroGestor(gid) {
	if( confirm("Deseja realmente excluir este Gestor?") ) {
		var excluir = document.getElementsByName("excluir")[0];

		excluir.value = gid;
		form.submit();
	}
}


</script>