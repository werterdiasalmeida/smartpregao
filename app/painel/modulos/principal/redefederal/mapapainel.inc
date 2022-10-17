<div style="position: relative; z-index:999; width:100%;" id="containerMapa" >
	<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
	<map name="mapaBrasil" id="mapaBrasil">
		<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="redefederal_listaUnidades('todos', <?=$_SESSION['redefederal']['orgid']?>);" title="Brasil"/>							
		<area shape="rect" coords="48,124,74,151"   style="cursor:pointer;" onclick="redefederal_listaUnidades('AC', <?=$_SESSION['redefederal']['orgid']?>);" title="Acre"/>
		<area shape="rect" coords="364,147,432,161" style="cursor:pointer;" onclick="redefederal_listaUnidades('AL', <?=$_SESSION['redefederal']['orgid']?>);" title="Alagoas"/>
		<area shape="rect" coords="202,27,233,56"   style="cursor:pointer;" onclick="redefederal_listaUnidades('AP', <?=$_SESSION['redefederal']['orgid']?>);" title="Amapá"/>
		<area shape="rect" coords="89,76,133,107"   style="cursor:pointer;" onclick="redefederal_listaUnidades('AM', <?=$_SESSION['redefederal']['orgid']?>);" title="Amazonas"/>
		<area shape="rect" coords="294,155,320,183" style="cursor:pointer;" onclick="redefederal_listaUnidades('BA', <?=$_SESSION['redefederal']['orgid']?>);" title="Bahia"/>
		<area shape="rect" coords="311,86,341,114"  style="cursor:pointer;" onclick="redefederal_listaUnidades('CE', <?=$_SESSION['redefederal']['orgid']?>);" title="Ceará"/>
		<area shape="rect" coords="244,171,281,197" style="cursor:pointer;" onclick="redefederal_listaUnidades('DF', <?=$_SESSION['redefederal']['orgid']?>);" title="Distrito Federal"/>
		<area shape="rect" coords="331,215,369,242" style="cursor:pointer;" onclick="redefederal_listaUnidades('ES', <?=$_SESSION['redefederal']['orgid']?>);" title="Espírito Santo"/>
		<area shape="rect" coords="217,187,243,218" style="cursor:pointer;" onclick="redefederal_listaUnidades('GO', <?=$_SESSION['redefederal']['orgid']?>);" title="Goiás"/>
		<area shape="rect" coords="154,155,210,186" style="cursor:pointer;" onclick="redefederal_listaUnidades('MT', <?=$_SESSION['redefederal']['orgid']?>);" title="Mato Grosso"/>
		<area shape="rect" coords="156,219,202,246" style="cursor:pointer;" onclick="redefederal_listaUnidades('MS', <?=$_SESSION['redefederal']['orgid']?>);" title="Mato Grosso do Sul"/>
		<area shape="rect" coords="248,80,301,111"  style="cursor:pointer;" onclick="redefederal_listaUnidades('MA', <?=$_SESSION['redefederal']['orgid']?>);" title="Maranhão"/>
		<area shape="rect" coords="264,206,295,235" style="cursor:pointer;" onclick="redefederal_listaUnidades('MG', <?=$_SESSION['redefederal']['orgid']?>);" title="Minas Gerais"/>
		<area shape="rect" coords="188,84,217,112"  style="cursor:pointer;" onclick="redefederal_listaUnidades('PA', <?=$_SESSION['redefederal']['orgid']?>);" title="Pará"/>
		<area shape="rect" coords="368,112,433,130" style="cursor:pointer;" onclick="redefederal_listaUnidades('PB', <?=$_SESSION['redefederal']['orgid']?>);" title="Paraíba"/>
		<area shape="rect" coords="201,262,231,289" style="cursor:pointer;" onclick="redefederal_listaUnidades('PR', <?=$_SESSION['redefederal']['orgid']?>);" title="Paraná"/>
		<area shape="rect" coords="369,131,454,147" style="cursor:pointer;" onclick="redefederal_listaUnidades('PE', <?=$_SESSION['redefederal']['orgid']?>);" title="Pernambuco"/>
		<area shape="rect" coords="285,116,313,146" style="cursor:pointer;" onclick="redefederal_listaUnidades('PI', <?=$_SESSION['redefederal']['orgid']?>);" title="Piauí"/>
		<area shape="rect" coords="349,83,383,108"  style="cursor:pointer;" onclick="redefederal_listaUnidades('RN', <?=$_SESSION['redefederal']['orgid']?>);" title="Rio Grande do Norte"/>
		<area shape="rect" coords="189,310,224,337" style="cursor:pointer;" onclick="redefederal_listaUnidades('RS', <?=$_SESSION['redefederal']['orgid']?>);" title="Rio Grande do Sul"/>
		<area shape="rect" coords="302,250,334,281" style="cursor:pointer;" onclick="redefederal_listaUnidades('RJ', <?=$_SESSION['redefederal']['orgid']?>);" title="Rio de Janeiro"/>
		<area shape="rect" coords="98,139,141,169"  style="cursor:pointer;" onclick="redefederal_listaUnidades('RO', <?=$_SESSION['redefederal']['orgid']?>);" title="Rondônia"/>
		<area shape="rect" coords="112,24,147,56"   style="cursor:pointer;" onclick="redefederal_listaUnidades('RR', <?=$_SESSION['redefederal']['orgid']?>);" title="Roraima"/>
		<area shape="rect" coords="228,293,272,313" style="cursor:pointer;" onclick="redefederal_listaUnidades('SC', <?=$_SESSION['redefederal']['orgid']?>);" title="Santa Catarina"/>
		<area shape="rect" coords="233,243,268,270" style="cursor:pointer;" onclick="redefederal_listaUnidades('SP', <?=$_SESSION['redefederal']['orgid']?>);" title="São Paulo"/>
		<area shape="rect" coords="337,161,401,178" style="cursor:pointer;" onclick="redefederal_listaUnidades('SE', <?=$_SESSION['redefederal']['orgid']?>);" title="Sergipe"/>
		<area shape="rect" coords="227,130,270,163" style="cursor:pointer;" onclick="redefederal_listaUnidades('TO', <?=$_SESSION['redefederal']['orgid']?>);" title="Tocantins"/>
		<area shape="rect" coords="17,264,85,282"   style="cursor:pointer;" onclick="redefederal_listaUnidades('norte',       <?=$_SESSION['redefederal']['orgid']?>);" title="Norte" />
		<area shape="rect" coords="16,281,94,296"   style="cursor:pointer;" onclick="redefederal_listaUnidades('nordeste',    <?=$_SESSION['redefederal']['orgid']?>);" title="Nordeste" />
		<area shape="rect" coords="15,296,112,312"  style="cursor:pointer;" onclick="redefederal_listaUnidades('centrooeste', <?=$_SESSION['redefederal']['orgid']?>);" title="Centro-Oeste" />
		<area shape="rect" coords="14,312,100,329"  style="cursor:pointer;" onclick="redefederal_listaUnidades('sudeste',     <?=$_SESSION['redefederal']['orgid']?>);" title="Sudeste" />
		<area shape="rect" coords="13,329,68,344"   style="cursor:pointer;" onclick="redefederal_listaUnidades('sul', 		<?=$_SESSION['redefederal']['orgid']?>);" title="Sul" />
	</map>
	<div style="position: relative; top:-32px; left:410px; z-index:1000;">
		<img src="/imagens/excluir_2.gif" border="0" onclick="closeMessage();" title="Fechar Mapa" style="cursor:pointer;"/>
	</div>
</div>
