<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type="text/css" href="../../includes/listagem.css"/>
<table width="95%" cellpadding="0" cellspacing="0" id="main" align="center">
<tr>
	<td><img src="/includes/layout/azul/img/logo.png" border="0" /></td>
	<td width="5%"><a href="../../treinamento/manual_usuario_depreciacao.pdf" target="_blank"><img src="/imagens/pdf_adobe.jpg" border="0" align="absmiddle" /></a></td>
	<td width="20%"><a href="../../treinamento/manual_usuario_depreciacao.pdf" target="_blank">Guia do usuário - Simulador de Depreciação WEB</a></td>
</tr>
</table>
<?php


/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "4000M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


if($_POST['enviar']) {
	

	// Pull in the NuSOAP code
	require_once('nusoap.php');

	// Create the client instance
	$client = new soapcliente('http://simec.mec.gov.br/webservice/depreciacao/server.php?wsdl', true);

	// Check for an error
	$err = $client->getError();
	
	if($err) {
	    // Display the error
	    die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
	    // At this point, you know the call that follows will fail
	}
	
	$csvarray = file($_FILES["arquivo"]["tmp_name"]);
	
	echo '<table class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td>';
	
	if($csvarray) {
		
		$dados['csvarray'] = $csvarray;
				   			   
		// Call the SOAP method
		$result = $client->call('aplicardepreciacao', array('informacoes' => $dados));
		
		// Check for a fault
		if ($client->fault) {
		    echo '<h2>Fault</h2><pre>';
		    print_r($result);
		    echo '</pre>';
		} else {
		    // Check for errors
		    $err = $client->getError();
		    if ($err) {
		        // Display the error
		        echo '<h2>Error</h2><pre>' . $err . '</pre>';
				echo '<h2>Response</h2>';
				echo '<pre>' . htmlspecialchars($client->response, ENT_QUOTES) . '</pre>';
		        
		    } else {
		    	ob_clean();
		    	header("Content-Type: application/csv") ;
				header("Content-disposition: attachment; filename=lista_depreciada.csv") ;
				if($result) echo implode("\n", $result['csvarray']);
				exit;
		    }
		}
		
	} else {
		echo 'Arquivo vazio';
	}
	
	echo '</td></tr>';
	echo '<tr><td><input type=button value=Voltar onclick="window.location=\'client.php\';"></td></tr>';
	echo '</table>';
	
	exit;
			   			   
	
}



?>
<script>

function validarFormulario() {

	if(document.getElementById('arquivocsv').value.substr(document.getElementById('arquivocsv').value.length-3) != "csv") {
		alert("Arquivo deve ser CSV");
		return false;
	}
	
}

</script>
<form method="post" name="formulario" id="formulario" enctype="multipart/form-data" action="" onsubmit="return validarFormulario();">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloCentro" colspan="2">Reavaliação, Redução a valor recuperável, Depreciação, Amortização e Exaustão na administração direta da União, suas Autarquias e Fundações</td>
</tr>
<tr>
	<td colspan="2">
	<table width=100%>
	<tr>
	<td colspan=2>O arquivo a ser enviado deve estar num formato padrão. Este formato consiste nos seguintes dados:</td>
	</tr>
	<tr>
	<td class="SubTituloDireita"><b>UNIDADE ORÇAMENTÁRIA</b></td><td>Código da unidade orçamentária</td>
	</tr>
	<tr>
	<td class="SubTituloDireita"><b>UNIDADE GESTORA</b></td><td>Código da unidade gestora</td>
	</tr>
	<tr>
	<td class="SubTituloDireita"><b>CÓDIGO DO PRODUTO</b></td><td>Código de controle interno de cada instituição utilizado para controlar o tipo de produto</td>
	</tr>
	<tr>
	<td class="SubTituloDireita"><b>NÚMERO DE TOMBAMENTO*</b></td><td>Nesse caso, se for colocado o RGP, ele deve ter a quantidade obrigatória de 1 (um). Caso contrário, não é um campo obrigatório.</td>
	</tr>
	<tr>
	<td class="SubTituloDireita"><b>CONTA PATRIMONIAL</b></td><td>Padronizado dentre algumas áreas, define a taxa de depreciação do produto</td>
	</tr>
	<tr>
	<td class="SubTituloDireita">DATA DE TOMBAMENTO</td><td>Data de lançamento do bem no SIAFI, no formato dd/mm/aaaa</td>
	</tr>
	<tr>
	<td class="SubTituloDireita">QUANTIDADE</td><td>Quantidade de produtos a serem depreciados</td>
	</tr>
	<tr>
	<td class="SubTituloDireita">VALOR DE ENTRADA</td><td>Valor total dos produtos a serem depreciados, levando em conta a quantidade</td>
	</tr>
	</table>
	<br />
	O arquivo deve estar no formato csv, separados por ";" ponto-e-virgula.<br/><br/>
	
	Exemplo:<br/><br/>
	
	UNIDADE ORÇAMENTÁRIA(CÓDIGO); UNIDADE GESTORA(CÓDIGO); CÓDIGO DO PRODUTO; NÚMERO DE TOMBAMENTO*; CONTA PATRIMONIAL; DATA DE TOMBAMENTO; QUANTIDADE; VALOR DE ENTRADA<br/>
	26101; 150010; 123456; 54321MKT; 14212.04.00; 01/01/2010; 1; 10000<br/>
	26101; 150010; 1234567; ; 14212.06.00; 03/05/2010; 5; 20000<br/>
	<br/>
	
	Retorno:<br/><br/>
	
	UNIDADE ORÇAMENTÁRIA(CÓDIGO); UNIDADE GESTORA(CÓDIGO); CÓDIGO DO PRODUTO; NÚMERO DE TOMBAMENTO*; CONTA PATRIMONIAL; DATA DE TOMBAMENTO; QUANTIDADE; VALOR DE ENTRADA; VALOR ATUAL ACUMULADO; VALOR RESIDUAL; VALOR DEPRECIÁVEL; DEPRECIAÇÃO DO MÊS CORRENTE; DEPRECIAÇÃO, AMORTIZAÇÃO OU EXAUSTÃO ACUMULADA; VALOR LÍQUIDO CONTÁBIL; DATA DE DEPRECIAÇÃO; ERROS<br/>
	26101;150010;123456;54321MKT;14212.04.00;01/01/2010;1;10000;10000,00;1000,00;9000,00;50,00;521,67;9478,33;10/11/2010;<br/>
	26101;150010;1234567;;14212.06.00;03/05/2010;5;20000;20000,00;4000,00;16000,00;133,33;848,89;19151,11;10/11/2010;
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Tipos de contas patrimoniais</td>
</tr>
<tr>
<td colspan="2">
<div style="width:100%;height:80px;overflow:auto;">
<?
$_CONTAS['14212.04.00'] = array("codigo" => "14212.04.00", "descricao" => "APARELHOS DE MEDICAO E ORIENTACAO", 				"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.06.00'] = array("codigo" => "14212.06.00", "descricao" => "APARELHOS E EQUIPAMENTO DE COMUNICAÇÃO", 		"porcentagem" => "0.2",  "vidautil" => "10");
$_CONTAS['14212.08.00'] = array("codigo" => "14212.08.00", "descricao" => "APAR., EQUIP.E UTENS.MED.,ODONT.,LABOR.E HOSP.", "porcentagem" => "0.2",  "vidautil" => "15");
$_CONTAS['14212.10.00'] = array("codigo" => "14212.10.00", "descricao" => "APARELHOS E EQUIP. P/ESPORTES E DIVERSOES", 		"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.12.00'] = array("codigo" => "14212.12.00", "descricao" => "APARELHOS E UTENSILIOS DOMESTICOS", 				"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.13.00'] = array("codigo" => "14212.13.00", "descricao" => "ARMAZENS ESTRUTURAIS - COBERTURA DE LONA", 		"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.14.00'] = array("codigo" => "14212.14.00", "descricao" => "ARMAMENTOS", 									"porcentagem" => "0.15", "vidautil" => "20");
$_CONTAS['14212.18.00'] = array("codigo" => "14212.18.00", "descricao" => "COLECOES E MATERIAIS BIBLIOGRAFICOS",			"porcentagem" => "0",    "vidautil" => "10");
$_CONTAS['14212.19.00'] = array("codigo" => "14212.14.00", "descricao" => "DISCOTECAS E FILMOTECAS", 						"porcentagem" => "0.1",  "vidautil" => "5");
$_CONTAS['14212.22.00'] = array("codigo" => "14212.22.00", "descricao" => "EQUIPAMENTOS DE MANOBRAS E PATRULHAMENTO",		"porcentagem" => "0.1",  "vidautil" => "20");
$_CONTAS['14212.24.00'] = array("codigo" => "14212.24.00", "descricao" => "EQUIPAMENTO DE PROTEÇÃO, SEGURANÇA E SOCORRO",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.26.00'] = array("codigo" => "14212.26.00", "descricao" => "INSTRUMENTOS MUSICAIS E ARTISTICOS",				"porcentagem" => "0.1",  "vidautil" => "20");
$_CONTAS['14212.28.00'] = array("codigo" => "14212.28.00", "descricao" => "MAQUINAS E EQUIPAM. DE NATUREZA INDUSTRIAL",		"porcentagem" => "0.1",  "vidautil" => "20");
$_CONTAS['14212.30.00'] = array("codigo" => "14212.30.00", "descricao" => "MAQUINAS E EQUIPAMENTOS ENEGERTICOS",			"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.32.00'] = array("codigo" => "14212.32.00", "descricao" => "MAQUINAS E EQUIPAMENTOS GRAFICOS",				"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.33.00'] = array("codigo" => "14212.33.00", "descricao" => "EQUIPAMENTOS PARA AUDIO, VIDEO E FOTO",			"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.34.00'] = array("codigo" => "14212.34.00", "descricao" => "MAQUINAS, UTENSILIOS E EQUIPAMENTOS DIVERSOS",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.35.00'] = array("codigo" => "14212.35.00", "descricao" => "EQUIPAMENTOS DE PROCESSAMENTOS DE DADOS",		"porcentagem" => "0.1",  "vidautil" => "5");
$_CONTAS['14212.36.00'] = array("codigo" => "14212.36.00", "descricao" => "MAQUINAS, INSTALACOES E UTENS. DE ESCRITORIO",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.38.00'] = array("codigo" => "14212.38.00", "descricao" => "MAQUINAS, FERRAMENTAS E UTENSILIOS DE OFICINA",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.39.00'] = array("codigo" => "14212.39.00", "descricao" => "EQUIPAMENTOS HIDRAULICOS E ELETRICOS",			"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.40.00'] = array("codigo" => "14212.40.00", "descricao" => "MAQ.EQUIP.UTENSILIOS AGRI/AGROP.E RODOVIARIOS",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.42.00'] = array("codigo" => "14212.42.00", "descricao" => "MOBILIARIO EM GERAL",							"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.46.00'] = array("codigo" => "14212.46.00", "descricao" => "SEMOVENTES E EQUIPAMENTOS DE MONTARIA",			"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.48.00'] = array("codigo" => "14212.48.00", "descricao" => "VEICULOS DIVERSOS",								"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.49.00'] = array("codigo" => "14212.49.00", "descricao" => "EQUIPAMENTOS E MATERIAL SIGILOSO E RESERVADO",	"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.50.00'] = array("codigo" => "14212.50.00", "descricao" => "VEICULOS FERROVIARIOS",							"porcentagem" => "0.1",  "vidautil" => "30");
$_CONTAS['14212.51.00'] = array("codigo" => "14212.51.00", "descricao" => "PECAS NÃO INCORPORAVEIS A IMOVEIS",				"porcentagem" => "0.1",  "vidautil" => "10");
$_CONTAS['14212.52.00'] = array("codigo" => "14212.52.00", "descricao" => "VEICULOS DE TRACAO MECANICA",					"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.53.00'] = array("codigo" => "14212.53.00", "descricao" => "CARROS DE COMBATE",								"porcentagem" => "0.1",  "vidautil" => "30");
$_CONTAS['14212.54.00'] = array("codigo" => "14212.54.00", "descricao" => "EQUIPAMENTOS, PEÇAS E ACESSORIOS AERONAUTICOS",	"porcentagem" => "0.1",  "vidautil" => "30");
$_CONTAS['14212.56.00'] = array("codigo" => "14212.56.00", "descricao" => "EQUIPAMENTOS, PEÇAS E ACES.DE PROTECAO AO VOO",	"porcentagem" => "0.1",  "vidautil" => "30");
$_CONTAS['14212.57.00'] = array("codigo" => "14212.57.00", "descricao" => "ACESSORIOS PARA AUTOMOVEIS",						"porcentagem" => "0.1",  "vidautil" => "5");
$_CONTAS['14212.58.00'] = array("codigo" => "14212.58.00", "descricao" => "EQUIPAMENTO DE MERGULHO E SALVAMENTO",			"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.60.00'] = array("codigo" => "14212.60.00", "descricao" => "EQUIPAMENTOS,PECAS E ACESSORIOS MARITIMOS",		"porcentagem" => "0.1",  "vidautil" => "15");
$_CONTAS['14212.83.00'] = array("codigo" => "14212.83.00", "descricao" => "EQUIPAMENTOS E SISTEMA DE PROT.VIG. AMBIENTAL",	"porcentagem" => "0.1",  "vidautil" => "10");

echo "<table width=100%>";
echo "<tr><td class=SubTituloCentro>Código</td><td class=SubTituloCentro>Descrição</td></tr>";
foreach($_CONTAS as $cc) {
	echo "<tr><td>".$cc['codigo']."</td><td>".$cc['descricao']."</td></tr>";
}
echo "</table>";
?>
</div>
</td>
</tr>
<tr>
	<td class="SubTituloDireita">Arquivo :</td>
	<td><input type="file" name="arquivo" id="arquivocsv"> <input type="submit" name="enviar" value="Enviar"></td>
</tr>
</table>
</form>