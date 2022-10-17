<?
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";


// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if(!$_SESSION['usucpf'])
	echo "<script>
			alert('Problemas com autenticação.');
			window.close();
		  </script>";


if($_REQUEST['entid'])
	$entidade = $db->pegaLinha("SELECT ent.entnome, ent.entemail, ent.entnumdddcomercial, ent.entnumcomercial, ende.endlog, ende.endnum, ende.endbai, mun.mundescricao, ende.estuf, ende.endcep FROM entidade.entidade ent 
								LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid 
								LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod 
								WHERE ent.entid='".$_REQUEST['entid']."'");



?>
<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
<link rel='stylesheet' type='text/css' href='../../includes/Estilo.css'/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
<title>Dados da entidade</title>
</head>
<body>
<table class=tabela width=300>
	<?
	if($entidade) {
	?>
	<tr>
		<td class='SubTituloDireita'>Nome:</td>
		<td><? echo $entidade['entnome']; ?></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Telefone:</td>
		<td><? echo "(".$entidade['entnumdddcomercial'].") ".$entidade['entnumcomercial']; ?></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>E-mail:</td>
		<td><? echo $entidade['entemail']; ?></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>Endereço:</td>
		<td><? echo $entidade['endlog'].", número ".$entidade['endnum'].", ".$entidade['endbai'].", ".$entidade['mundescricao'].", ".$entidade['estuf'].", CEP ".$entidade['endcep']; ?></td>
	</tr>
	<?
	} else {
	?>
	<tr>
		<td colspan='2'>Entidade não cadastrada</td>
	</tr>
	<?
	}
	?>
</table>	
</body>
</html>



