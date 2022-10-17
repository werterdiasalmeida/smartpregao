<?php
ini_set( "memory_limit", "300M" );

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?
// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select 
            m.estuf,
            m.mundescricao,
            inucadcmt AS rspdsc
		from 
			cte.instrumentounidade iu	
			inner join territorios.municipio m ON m.muncod = iu.muncod and 
												  m.estuf = iu.mun_estuf	
		where 
			itrid = 2 and 
			trim(inucadcmt) <> '' 
		order by 
			m.estuf,
			m.mundescricao
		--limit 10;";

$dados = $db->carregar($sql);

echo '<table cellspacing="0" cellpadding="2" border="1" align="center" width="95%" style="color: rgb(51, 51, 51);">';

foreach ($dados as $dado):
	echo '<tr>
			<td valign="top">'.$dado['estuf'].'</td>
			<td valign="top">'.$dado['mundescricao'].'</td>
			<td align="left">'.$dado['rspdsc'].'</td>
		  </tr>';
endforeach;

echo '</table>';
?>