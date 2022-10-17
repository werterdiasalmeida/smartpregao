<?php
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
$titulo = "Tabelas de Apoio";
monta_titulo( $titulo, '&nbsp;' );

?>	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td>
		Escolha a tabela para administrar:<br>
		</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=unidademedicao&nome_descricao=unmdesc&nome_status=unmstatus&nome_id=unmid&novo_registro=true&nome_tabela=Unidade de Medição"> Unidade de medição</a><br>		</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=classeindicador&nome_descricao=clidsc&nome_status=clistatus&nome_id=cliid&novo_registro=true&nome_tabela=Classe do Indicador"> Classe do Indicador</a>	<br>	</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=tipoindicador&nome_descricao=tpidsc&nome_status=tpistatus&nome_id=tpiid&novo_registro=true&nome_tabela=Tipo do Indicador"> Tipo do Indicador</a>	<br>	</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=secretaria&nome_descricao=secdsc&nome_status=secstatus&nome_id=secid&novo_registro=true&nome_tabela=Secretaria/autarquia responsável"> Secretaria/autarquia responsável</a><br>	</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=acao&nome_descricao=acadsc&nome_status=acastatus&nome_id=acaid&novo_registro=true&nome_tabela=Ação"> Ação </a>	<br>	</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=periodicidade&nome_descricao=perdsc&nome_status=perstatus&nome_id=perid&novo_registro=true&nome_tabela=Periodicidade"> Periodicidade</a>	<br>	</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=estilo&nome_descricao=estdsc&nome_status=eststatus&nome_id=estid&novo_registro=true&nome_tabela=Estilo"> Estilo</a>	<br>	</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=coleta&nome_descricao=coldsc&nome_status=colstatus&nome_id=colid&novo_registro=true&nome_tabela=Coleta"> Coleta</a><br>		</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=perspectiva&nome_descricao=pesdsc&nome_status=pesstatus&nome_id=pesid&novo_registro=true&nome_tabela=Perspectiva estratégica"> Perspectiva estratégica</a><br>		</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=tema&nome_descricao=temdsc&nome_status=temstatus&nome_id=temid&novo_registro=true&nome_tabela=ema Estratégico"> Tema Estratégico</a><br>		</td>
	</tr>
	<tr>
		<td>
		- <a href="painel.php?modulo=principal/tabelas_de_apoio&acao=A&tabela=mapa&nome_descricao=mapdsc&nome_status=mapstatus&nome_id=mapid&novo_registro=true"> Mapa Estratégico</a><br>		</td>
	</tr>
</table>
