<?php

$unicod = "";
if($_REQUEST["entid"]) {
	$sql = "SELECT entunicod FROM entidade.entidade WHERE entid = ".$_REQUEST["entid"];
	$unicod = $db->pegaUm($sql);
	
	if($unicod) {
		$unicod = "where rssunicod = '".$unicod."'";
	}
}



if($unicod == "") {
	$sql = "select
	  sum(round(rssdotacaoautorizadacusteio,0)) as rssdotacaoautorizadacusteio,
	  sum(round(rssdotacaoautorizadainvest,0)) as rssdotacaoautorizadainvest,
	  sum(round(rssdotacaoautorizadainversao,0)) as rssdotacaoautorizadainversao,
	  
	  sum(round(rsscreditoespecialcusteio,0)) as rsscreditoespecialcusteio,
	  sum(round(rsscreditoespecialinvest,0)) as rsscreditoespecialinvest,
	  sum(round(rsscreditoespecialinversao,0)) as rsscreditoespecialinversao,
	  
	  sum(round(rssemendascusteio,0)) as rssemendascusteio,
	  sum(round(rssemendasinvest,0)) as rssemendasinvest,
	  sum(round(rssemendasinversao,0)) as rssemendasinversao,
	  
	  sum(round(rssportariacusteio,0)) as rssportariacusteio,
	  sum(round(rssportariainvest,0)) as rssportariainvest,
	  sum(round(rssportariainversao,0)) as rssportariainversao,
	  
	  sum(round(rssdevolucaocusteio,0)) as rssdevolucaocusteio,
	  sum(round(rssdevolucaoinvest,0)) as rssdevolucaoinvest,
	  sum(round(rssdevolucaoinversao,0)) as rssdevolucaoinversao,
	  
	  sum(round(rssdescentralizacaoliqcusteio,0)) as rssdescentralizacaoliqcusteio,
	  sum(round(rssdescentralizacaoliqinvest,0)) as rssdescentralizacaoliqinvest,
	  sum(round(rssdescentralizacaoliqinversao,0)) as rssdescentralizacaoliqinversao,
	  
	  sum(round(rssempenhadodiretouocusteio,0)) as rssempenhadodiretouocusteio,
	  sum(round(rssempenhadodiretouoinvest,0)) as rssempenhadodiretouoinvest,
	  sum(round(rssempenhadodiretouoinversao,0)) as rssempenhadodiretouoinversao,
	  
	  sum(round(rssempenhadodescentralizadocusteio,0)) as rssempenhadodescentralizadocusteio,
	  sum(round(rssempenhadodescentralizadoinvest,0)) as rssempenhadodescentralizadoinvest,
	  sum(round(rssempenhadodescentralizadoinversao,0)) as rssempenhadodescentralizadoinversao,
	  
	  sum(round(rsspactuadocusteio,0)) as rsspactuadocusteio,
	  sum(round(rsspactuadoinvest,0)) as rsspactuadoinvest,
	  sum(round(rsspactuadobolsas,0)) as rsspactuadobolsas,
	  
	  sum( round( ( coalesce(rssdotacaoautorizadacusteio,0) - coalesce(rsscreditoespecialcusteio,0) - coalesce(rssemendascusteio,0) ), 0) + round( ( coalesce(rssportariacusteio,0) - coalesce(rssdevolucaocusteio,0) ), 0) ) as rssdisponibilizadocusteio,
	  sum( round( ( coalesce(rssdotacaoautorizadainvest,0) - coalesce(rsscreditoespecialinvest,0) - coalesce(rssemendasinvest,0) ), 0) + round( ( coalesce(rssportariainvest,0) - coalesce(rssdevolucaoinvest,0) ), 0) ) as rssdisponibilizadoinvest,
	  sum(rssdisponibilizadobolsas) as rssdisponibilizadobolsas
	  
	from financeiro.relatoriosesu s
	--inner join entidade.entidade e ON e.entunicod = s.rssunicod
	--inner join entidade.funcaoentidade fe ON fe.entid = e.entid
	--where fe.funid = 12";
        
	$dados1 = $db->carregar($sql);
}

$sql = "select
  rssunicod,
--  entsig as rssunidsc,
  rssunidsc,
  
  sum(rssdotacaoautorizadacusteio) as rssdotacaoautorizadacusteio,
  sum(rssdotacaoautorizadainvest) as rssdotacaoautorizadainvest,
  sum(rssdotacaoautorizadainversao) as rssdotacaoautorizadainversao,
  
  sum(rsscreditoespecialcusteio) as rsscreditoespecialcusteio,
  sum(rsscreditoespecialinvest) as rsscreditoespecialinvest,
  sum(rsscreditoespecialinversao) as rsscreditoespecialinversao,
  
  sum(rssemendascusteio) as rssemendascusteio,
  sum(rssemendasinvest) as rssemendasinvest,
  sum(rssemendasinversao) as rssemendasinversao,
  
  sum(rssportariacusteio) as rssportariacusteio,
  sum(rssportariainvest) as rssportariainvest,
  sum(rssportariainversao) as rssportariainversao,
  
  sum(rssdevolucaocusteio) as rssdevolucaocusteio,
  sum(rssdevolucaoinvest) as rssdevolucaoinvest,
  sum(rssdevolucaoinversao) as rssdevolucaoinversao,
  
  sum(rssdescentralizacaoliqcusteio) as rssdescentralizacaoliqcusteio,
  sum(rssdescentralizacaoliqinvest) as rssdescentralizacaoliqinvest,
  sum(rssdescentralizacaoliqinversao) as rssdescentralizacaoliqinversao,
  
  sum(rssempenhadodiretouocusteio) as rssempenhadodiretouocusteio,
  sum(rssempenhadodiretouoinvest) as rssempenhadodiretouoinvest,
  sum(rssempenhadodiretouoinversao) as rssempenhadodiretouoinversao,
  
  sum(rssempenhadodescentralizadocusteio) as rssempenhadodescentralizadocusteio,
  sum(rssempenhadodescentralizadoinvest) as rssempenhadodescentralizadoinvest,
  sum(rssempenhadodescentralizadoinversao) as rssempenhadodescentralizadoinversao,
  
  sum(rsspactuadocusteio) as rsspactuadocusteio,
  sum(rsspactuadoinvest) as rsspactuadoinvest,
  sum(rsspactuadobolsas) as rsspactuadobolsas,
  
  sum( ( coalesce(rssdotacaoautorizadacusteio,0) - coalesce(rsscreditoespecialcusteio,0) - coalesce(rssemendascusteio,0) ) + ( coalesce(rssportariacusteio,0) - coalesce(rssdevolucaocusteio,0) ) ) as rssdisponibilizadocusteio,
  sum( ( coalesce(rssdotacaoautorizadainvest,0) - coalesce(rsscreditoespecialinvest,0) - coalesce(rssemendasinvest,0) ) + ( coalesce(rssportariainvest,0) - coalesce(rssdevolucaoinvest,0) ) ) as rssdisponibilizadoinvest,
  sum(rssdisponibilizadobolsas) as rssdisponibilizadobolsas
  
from financeiro.relatoriosesu s
--inner join entidade.entidade e ON e.entunicod = s.rssunicod
--inner join entidade.funcaoentidade fe ON fe.entid = e.entid
--where fe.funid = 12
 ".$unicod." 
group by 
  rssunicod,
   rssunidsc,ordem
order by ordem,
  rssunidsc";

$dados = $db->carregar($sql);

?>

<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<style type="text/css">
@media screen {
.div_rolagem{ overflow: auto; width:100%; height:400px; }
}

.noPadding{padding:0;}

TD
{
    FONT: 8pt Arial,verdana
}
TH
{
	text-align: center;
    FONT: 8pt Arial,verdana;
    font-weight: bold;
}
</style>

<table width="95%">
	<thead>
		<tr>
			<th class="noPadding" align="left">
				<table width="100%" border="0" cellpadding="0" cellspacing="0">
					<tr>
						<td valign="top" width="50" rowspan="2">
						</td>
						<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
							<font style="font-size:11px;">
								MEC - SESu - DIFES<br/>
								<b>REUNI - Acompanhamento Or�ament�rio<br/>
								Reestrutura��o e Expans�o das Universidades Federais - REUNI</b><br />
								Execu��o 2009
							</font>
						</td>
						<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
							<font style="font-size:11px;">
								Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
								<b>Hora da Impress�o: <?= date( 'd/m/Y - H:i:s' ) ?></b><br/>
							</font>
						</td>
						<tr><td>&nbsp;</td></tr>
				</table>
				<table style="width:98.8%;" align="left" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<thead>
						<tr style="height:50px;">
							<!-- Primeira Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" colspan="5" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PACTUADO</strong></td>
							<th valign="top" colspan="5" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>DISPONIBILIZADO</strong></td>
							<th valign="top" colspan="3" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>DIFEREN�A: Pactuado - Disponib.</strong></td>
							<th valign="top" colspan="3" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>% Pactuado Devido</strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Primeira Tabela -->
							<th width="4%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>UO</strong></td>
							<th width="4%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IFES</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio<br>AE + UBC</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa</font></strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">*</font> Bolsas<br>Rec. Capes</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br>com Bolsa Capes</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest. + Inv. Financ</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa</font></strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">*</font> Bolsas<br>Rec. Capes</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br>com Bolsa Capes</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa Capes</font></strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>C</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>I</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>T</strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Primeira Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">D<br>pactuado</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">E<br>pactuado</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">F<br>D + E</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">G<br>pactuado</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">H<br>F + G</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">I<br>AC + AO</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">J<br>AG + AH + AW + AX</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">K<br>I + J</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">L<br>capes</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">M<br>K + L</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">N<br>D - I</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">O<br>E - J</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">P<br>N + O</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">Q<br>N � D</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">R<br>O � E</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">S<br>P � F</strong></td>
						</tr>
					</thead>
				</table>
			</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td class="noPadding" align="center">
				<div class="div_rolagem">
					<table style="width:100%;" align="left" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<?
					if($unicod == "") {
						$D1 = (($dados1[0]["rsspactuadocusteio"] && $dados1[0]["rsspactuadocusteio"] != 0) ? number_format($dados1[0]["rsspactuadocusteio"], 0, ',', '.') : "-");
						$E1 = (($dados1[0]["rsspactuadoinvest"] && $dados1[0]["rsspactuadoinvest"] != 0) ? number_format($dados1[0]["rsspactuadoinvest"], 0, ',', '.') : "-");
						$F1 = ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"] != 0) ? number_format(($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]), 0, ',', '.') : "-";
						
						$G1 = (($dados1[0]["rsspactuadobolsas"] && $dados1[0]["rsspactuadobolsas"] != 0) ? number_format($dados1[0]["rsspactuadobolsas"], 0, ',', '.') : "-");
						$H1 = (($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) + $dados1[0]["rsspactuadobolsas"] != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) + $dados1[0]["rsspactuadobolsas"]), 0, ',', '.') : "-";
						
						$I1 = (($dados1[0]["rssdisponibilizadocusteio"] && $dados1[0]["rssdisponibilizadocusteio"] != 0) ? number_format($dados1[0]["rssdisponibilizadocusteio"], 0, ',', '.') : "-");
						$J1 = (($dados1[0]["rssdisponibilizadoinvest"] && $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format($dados1[0]["rssdisponibilizadoinvest"], 0, ',', '.') : "-");
						$K1 = ($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
						
						$L1 = (($dados1[0]["rssdisponibilizadobolsas"] && $dados1[0]["rssdisponibilizadobolsas"] != 0) ? number_format($dados1[0]["rssdisponibilizadobolsas"], 0, ',', '.') : "-");
						
						$M1 = (($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]) + $dados1[0]["rssdisponibilizadobolsas"] != 0) ? number_format((($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]) + $dados1[0]["rssdisponibilizadobolsas"]), 0, ',', '.') : "-";
						$N1 = ($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"] != 0) ? number_format(($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]), 0, ',', '.') : "-";
						$O1 = ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
						$P1 = (($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])), 0, ',', '.') : "-";
						
						$Q1 = ($dados1[0]["rsspactuadocusteio"]==0) ? "-" : (($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) / $dados1[0]["rsspactuadocusteio"] != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) / $dados1[0]["rsspactuadocusteio"]), 0, ',', '.') : "-";
						$R1 = ($dados1[0]["rsspactuadoinvest"]==0) ? "-" : (($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) / $dados1[0]["rsspactuadoinvest"] != 0) ? number_format((($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) / $dados1[0]["rsspactuadoinvest"]), 0, ',', '.') : "-";
						$S1 = ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]==0) ? "-" : ((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])) / ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) != 0) ? number_format(((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])) / ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"])), 0, ',', '.') : "-";
			
						echo '<tr bgcolor="#c0c0c0" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
								<td colspan="2" align="left"><b>TOTAL</b></td>
							  	<!-- Primeira Tabela -->
							  	<td align="right"><b>'.$D1.'</b></td>
							  	<td align="right"><b>'.$E1.'</b></td>
							  	<td align="right"><b>'.$F1.'</b></td>
							  	<td align="right"><b>'.$G1.'</b></td>
							  	<td align="right"><b>'.$H1.'</b></td>
							  	<td align="right"><b>'.$I1.'</b></td>
							  	<td align="right"><b>'.$J1.'</b></td>
							  	<td align="right"><b>'.$K1.'</b></td>
							  	<td align="right"><b>'.$L1.'</b></td>
							  	<td align="right"><b>'.$M1.'</b></td>
							  	<td align="right"><b>'.$N1.'</b></td>
							  	<td align="right"><b>'.$O1.'</b></td>
							  	<td align="right"><b>'.$P1.'</b></td>
							  	<td align="right"><b>'.$Q1.'</b></td>
							  	<td align="right"><b>'.$R1.'</b></td>
							  	<td align="right"><b>'.$S1.'</b></td>
							  </tr>';
					}
					
					for($z=0; $z<count($dados); $z++) {
						// Primeira Tabela
						$D = (($dados[$z]["rsspactuadocusteio"] && $dados[$z]["rsspactuadocusteio"] != 0) ? number_format($dados[$z]["rsspactuadocusteio"], 0, ',', '.') : "-");
						$E = (($dados[$z]["rsspactuadoinvest"] && $dados[$z]["rsspactuadoinvest"] != 0) ? number_format($dados[$z]["rsspactuadoinvest"], 0, ',', '.') : "-");
						$F = ($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"] != 0) ? number_format(($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"]), 0, ',', '.') : "-";
						
						$G = (($dados[$z]["rsspactuadobolsas"] && $dados[$z]["rsspactuadobolsas"] != 0) ? number_format($dados[$z]["rsspactuadobolsas"], 0, ',', '.') : "-");
						$H = (($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"]) + $dados[$z]["rsspactuadobolsas"] != 0) ? number_format((($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"]) + $dados[$z]["rsspactuadobolsas"]), 0, ',', '.') : "-";
						
						$I = (($dados[$z]["rssdisponibilizadocusteio"] && $dados[$z]["rssdisponibilizadocusteio"] != 0) ? number_format($dados[$z]["rssdisponibilizadocusteio"], 0, ',', '.') : "-");
						$J = (($dados[$z]["rssdisponibilizadoinvest"] && $dados[$z]["rssdisponibilizadoinvest"] != 0) ? number_format($dados[$z]["rssdisponibilizadoinvest"], 0, ',', '.') : "-");
						$K = ($dados[$z]["rssdisponibilizadocusteio"] + $dados[$z]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados[$z]["rssdisponibilizadocusteio"] + $dados[$z]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
						
						$L = (($dados[$z]["rssdisponibilizadobolsas"] && $dados[$z]["rssdisponibilizadobolsas"] != 0) ? number_format($dados[$z]["rssdisponibilizadobolsas"], 0, ',', '.') : "-");
						
						$M = (($dados[$z]["rssdisponibilizadocusteio"] + $dados[$z]["rssdisponibilizadoinvest"]) + $dados[$z]["rssdisponibilizadobolsas"] != 0) ? number_format((($dados[$z]["rssdisponibilizadocusteio"] + $dados[$z]["rssdisponibilizadoinvest"]) + $dados[$z]["rssdisponibilizadobolsas"]), 0, ',', '.') : "-";
						$N = ($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"] != 0) ? number_format(($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]), 0, ',', '.') : "-";
						$O = ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
						$P = (($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) + ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"]) != 0) ? number_format((($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) + ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"])), 0, ',', '.') : "-";
						
						$Q = ($dados[$z]["rsspactuadocusteio"]==0) ? "-" : (($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) / $dados[$z]["rsspactuadocusteio"] != 0) ? number_format(($dados[$z]["rsspactuadocusteio"]==0) ? "-" : (($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) / $dados[$z]["rsspactuadocusteio"]), 0, ',', '.') : "-";
						$R = ($dados[$z]["rsspactuadoinvest"]==0) ? "-" : (($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"]) / $dados[$z]["rsspactuadoinvest"] != 0) ? number_format(($dados[$z]["rsspactuadoinvest"]==0) ? "-" : (($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"]) / $dados[$z]["rsspactuadoinvest"]), 0, ',', '.') : "-";
						
						$S = (($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"])==0) ? "-" : (((($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) + ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"])) / ($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"]) != 0) ? number_format(((($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) + ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"])) / ($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"])), 0, ',', '.') : "-");
						
						echo '<tr bgcolor="#f9f9f9" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
								<td width="4%" align="left">'.$dados[$z]["rssunicod"].'</td>
							  	<td width="4%" align="left">'.$dados[$z]["rssunidsc"].'</td>
							  	<!-- Primeira Tabela -->
							  	<td width="5.75%" align="right">'.$D.'</td>
							  	<td width="5.75%" align="right">'.$E.'</td>
							  	<td width="5.75%" align="right">'.$F.'</td>
							  	<td width="5.75%" align="right">'.$G.'</td>
							  	<td width="5.75%" align="right">'.$H.'</td>
							  	<td width="5.75%" align="right">'.$I.'</td>
							  	<td width="5.75%" align="right">'.$J.'</td>
							  	<td width="5.75%" align="right">'.$K.'</td>
							  	<td width="5.75%" align="right">'.$L.'</td>
							  	<td width="5.75%" align="right">'.$M.'</td>
							  	<td width="5.75%" align="right">'.$N.'</td>
							  	<td width="5.75%" align="right">'.$O.'</td>
							  	<td width="5.75%" align="right">'.$P.'</td>
							  	<td width="5.75%" align="right">'.$Q.'</td>
							  	<td width="5.75%" align="right">'.$R.'</td>
							  	<td width="5.75%" align="right">'.$S.'</td>
							  </tr>';
					}
					?>
					</table>
				</div>
			</td>
		</tr>
	</tbody>
</table>
<br />
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left"><strong><font color="red">*</font> OBS:</strong> Os recursos REUNI pactuados para bolsas de Doutorado, Mestrado e P�s-Doutorado, foram alocados em a��o da CAPES.</td>
	</tr>
	<tr>
		<td align="left"><strong><font color="red">**</font> OBS:</strong> UNB-26271: A UNB emitiu em 2009 devolu��o referente ao projeto REUNI/2007 no valor de R$30.742.529,31 , conforme programa��o financeira (PF) emitida em 16/07/2009.</td>
	</tr>
	<tr>
		<td align="left"><strong><font color="red">***</font> OBS:</strong> Os valores de descentraliza��o neste exerc�cio est�o em andamento. Estes dados foram atualizados pelo SIAFI Base: 14-SET-2009</td>
	</tr>
</table>