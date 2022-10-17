<?php

	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	
	if (!$_SESSION['usucpf']){
		die('<script>
				alert(\'Acesso negado!\');
				window.close();
			 </script>');
	}
	
	$sisid = (integer) ($_SESSION['sisid'] == 4 && $_GET['sisid']) ? $_GET['sisid'] : $_SESSION['sisid'];
	
	$sql = "SELECT
			 sisdsc
			FROM
			 seguranca.sistema
			WHERE
			 sisid = $sisid";
	$sisdsc = $db->pegaUm($sql);

	$where = '';
	$where .= $_POST['fusucpf'] ? " AND usucpf = '" . str_replace(array(".","-"), '', $_POST['fusucpf']) . "' " : '';
	$where .= $_POST['fusunome'] ? " AND usunome ILIKE '" . $_POST['fusunome'] . "%'" : '';
	
	$sql =
		" select" .
			" case when usucpf = '" . $_SESSION['usucpf'] . "' then " .
				" '<a name=\'' || usunome || '\'  href=\"#\" \">' || usunome || '</a>' " .
			" else " .
				" '<a name=\'' || usunome || '\'  href=\"#\" onclick=\"window.opener.abrirChat( \'' || usucpf || '\', \'' || usunome || '\' );\">' || usunome || '</a>' " .
			" end as usunome, " .
			" unidsc, " .
			" mundescricao, " .
			" estuf, " .
			" tempologado, " .
			" mnudsc, " .
			" usufoneddd || ' ' || usufonenum as telefone, " .
			" usucpf " .
		" from seguranca.usuariosonline " .
		" where " .
			"sisid = " . ( $sisid ) .  
		$where;

	$registro = $db->carregar( $sql );
	$registro = $registro ? $registro : array();			
	
	extract($_POST);
?>
<html>
	<head>
		<META http-equiv="Pragma" content="no-cache"/>
		<title>Usuários Online</title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript">
			window.focus();	
		</script>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
	<?= monta_titulo( 'Usuários Online', $sisdsc ); ?>
	<?
//	if (count($registro) > 30){
	?>
	<form method="POST"  name="formulario">	
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td bgcolor="#DFDFDF" style="font-family: verdana; font-size: 16px; text-align: center;" colspan="4"><center>Filtro</center></td>
      </tr>    
      <tr>
        <td align='right' class="SubTituloDireita">CPF:</td>
        <td>
			<?=campo_texto('fusucpf','S',$habil,'',17,14,'###.###.###-##','');?>
        </td>
        <td align='right' class="SubTituloDireita">Nome:</td>
        <td>
			<?=campo_texto('fusunome','S',$habil,'',30,200,'','');?>
        </td>
      </tr>		
      <tr>
        <td bgcolor="#DFDFDF" colspan="4">
        	<center><input name="filtrar" type="submit" value="Filtrar"></center>
        </td>
      </tr>          
    </table>
    </form>  
    <?
//	}
    ?>
		<?php 			
			$a = 0;
			foreach ($registro as $item){
//				$texto = htmlentities( $item['descricao'] ); 
				$dados[$a]['usunome'] 	   = $item['usunome'];
				$dados[$a]['unidsc']  	   = htmlentities( $item['unidsc'] );
				$dados[$a]['mundescricao'] = htmlentities( $item['mundescricao'] );
				$dados[$a]['estuf'] 	   = $item['estuf'];
				$dados[$a]['tempologado']  = $item['tempologado'];
				$dados[$a]['mnudsc']  	   = htmlentities( $item['mnudsc'] );
				$dados[$a]['telefone']     = $item['telefone'];
				$dados[$a]['usucpf']     = $item['usucpf'];
				
				if ( isset($item['value']) ){
					$dados[$a]['value']	= $item['value'];
					$existValue = 1;
				}
				$a++;
			}

			$html 	   = array("<img src='../imagens/email.gif' title='Enviar e-mail' border='0' onclick='emailPara(\"{campo[8]}\");' style='cursor:pointer;'>&nbsp; {campo[1]}");
			$indices = array( 'Nome', 'Unidade', 'Cidade', 'UF', 'Tempo Logado', 'Módulo Atual', 'Telefone' );
			echo "<div style='overflow:auto; height:373px;'>";
		 	$db->monta_lista_array( $dados, $indices, 30, 10, 'N', '', $html);
		 	echo "</div>";		 	
		?>
				
		<?//= $db->monta_lista( $sql, $indices, 20, 20, '', '', '' ); ?>
	</body>
</html>