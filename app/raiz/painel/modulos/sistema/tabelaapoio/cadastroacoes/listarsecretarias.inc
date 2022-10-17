<? 

	$sql_lista = "SELECT 
					'<input type=\"checkbox\" id=\"chk_'||sec.secid||'\" onclick=\"resultado(this,\''||sec.secid||'\',\''||sec.secdsc||'\');\">' as checkbox,
					sec.secdsc 
				FROM painel.secretaria sec";


?>
<html>
<head>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php 

monta_titulo($entnome, '&nbsp');

$cabecalho = array ("Selecione", "Secretaria" );
$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');

?>
<script type="text/javascript">
/* CARREGANDO OS DADOS DE PTRES */
var tabelaorigem = window.opener.document.getElementById('orcamento');
var idck;
for(i=1;i<tabelaorigem.rows.length-1;i++) {
	
	idck = tabelaorigem.rows[i].id.split("_");
	
	if(document.getElementById("chk_"+idck[1])) {
		document.getElementById("chk_"+idck[1]).checked=true;
	}
}
/* FIM CARREGANDO OS DADOS DE PTRES */

function resultado(dados, ptrid, ptres){


	if(!ptres) {
		alert('Não existe PTRES. Entre em contato com o administrador do sistema.');
		return false;
	}

	if(dados.checked) {

	var linhaTbl = dados.parentNode.parentNode;
	var tabelaorigem = window.opener.document.getElementById('orcamento');
	if(eval(tabelaorigem.rows.length%2)) {
		var cor = "";
	} else {
		var cor = "#DCDCDC";
	}
	var linha = tabelaorigem.insertRow(1);
	linha.id = "ptrid_"+ptrid;
	linha.style.backgroundColor = cor;
	linha.style.height = '30px';
	
	// setando o ptres
	var celula1 = tabelaorigem.rows[1].insertCell(0);
	celula1.style.textAlign = "left";
	celula1.innerHTML = "<input type='checkbox' checked id='check' onclick='resultado("+ptrid+");'>"+ptres+"<input type='hidden' name='ptrid[]' value='"+ptrid+"'>";
			
	} else {

	var tabelaorigem = window.opener.document.getElementById('orcamento');
	tabelaorigem.deleteRow(window.opener.document.getElementById('ptrid_'+ptrid).rowIndex);
	//window.opener.calculovalorPI();

	}

}

</script>
<form action="" method="post" name="formulario">
<table width="100%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
<tr style="background-color: #cccccc">
	<td align="right">
	<input type="button" name="close" value="Fechar" onclick="window.close();">	
	</td>
</tr>		
</table>

</form>
</body>
</html>

