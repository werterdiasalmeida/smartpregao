<?
//$_REQUEST['indid'] = '1';
//$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

define("REGIONALIZACAO_ESCOLA",2);
define("REGIONALIZACAO_POSGRADUACAO",7);

define("REGIONALIZACAO_CAMPUS_SUPERIOR",8);
define("REGIONALIZACAO_CAMPUS_PROFISSIONAL",9);
define("REGIONALIZACAO_UNIVERSIDADE",10);
define("REGIONALIZACAO_INSTITUTO",11);
define("REGIONALIZACAO_HOSPITAL",12);

define("REGIONALIZACAO_IES",5);
define("REGIONALIZACAO_MUN",4);
define("REGIONALIZACAO_UF",6);
define("REGIONALIZACAO_BRASIL",1);

$v = explode(";", $_REQUEST['variaveis']);

$_REQUEST['indid']=$v[0];
$_REQUEST['estuf']=$v[1];
$_REQUEST['muncod']=$v[2];
$_REQUEST['dpeid_inicio']=$v[3];
$_REQUEST['dpeid_fim']=$v[4];
$_REQUEST['tpmid']=$v[5];
$_REQUEST['tidid1']=$v[6];
$_REQUEST['tidid2']=$v[7];
$_REQUEST['regcod']=$v[8];

$db = new cls_banco();

$xml .= "<?xml version=\"1.0\" encoding=\"utf-8\"?>";

$xml .= "<countrydata>";

$sql = "SELECT * FROM territorios.estado est 
		LEFT JOIN territorios.regiao reg ON est.regcod = reg.regcod";
$estados = $db->carregar($sql);

$cores = array(1 => 'FFE4B5',2 => 'E6E6FA',3 => '87CEFA',4 => '98FB98',5 => 'FFFFE0');

foreach($estados as $estreg) {
	$estregconsolidado[$estreg['regcod']][] = $estreg;
}

foreach($estregconsolidado as $key => $estados) {
	$xml .= "<state id=\"range\"><data>".($key*100)." - ".(($key*100)+count($estados))."</data><color>".$cores[trim($key)]."</color></state>";
}

$xml .= "<state id=\"zoom_out_button\"><name>Zoom Out</name><data>SE</data><font_size>14</font_size><font_color>ffffff</font_color><background_color>990000</background_color></state>";


/*
 * outline_color:   This is the color of the borders between states and provinces. In this case, it will display a dark gray:

	<state id="outline_color">
		<color>666666</color>
	</state>

	If you do not want to display the state borders at all, add <opacity>0</opacity>
 */
$xml .= "<state id=\"outline_color\"><color>003300</color></state>";
/*
 *  default_point:   This sets the default attributes for all points on the map. Set the default color in the <color> field. In this case, it will display a bright red. The <size> attribute sets the default size. The <opacity> attribute sets the default opacity. A <size> of 10 will show a dot 10 pixel diameter dot when zoomed out at 100%. <src> loads a custom icon for the default point. All default_point attributes are overridden attributes specified in a point_range or individual point configuration block.

	<state id="default_point">
		<color>ffff00</color>
		<size>20</size>
		<opacity>70</opacity>
		<src>lighthouse.swf</src>
	</state>
 * 
 */
$xml .= "<state id=\"default_point\"><size>5</size></state>";
/*
 * default_color:   This is the color of the states that do not have any data associated with them. In this case, it will display them in a light gray:

	<state id="default_color">
		<color>bbbbbb</color>
	</state>
 * 
 */
$xml .= "<state id=\"default_color\"><color>339933</color></state>";
/*
 * background_color:   This is the color of the map background, the ocean and other bodies of water. In this case, it will display a white background:

	<state id="background_color">
		<color>ffffff</color>
	</state>

	To make the background transparent, use:

	<state id="background_color">
		<opacity>0</opacity>
	</state>
 * 
 */
$xml .= "<state id=\"background_color\"><color>cceeff</color></state>";

/*
 * scale_points:   This sets the size of the points when zooming into the overall map. If you leave this out, the points will retain their size relative to the map, i.e. a small point when zoomed out will appear to be a large point when you zoom in to the map. If you set the <data> to 100, the point will not change visible size when you zoom in to the map, i.e. a small point when zoomed out will still appear small when you zoom in to the map. If you set the <data> to 50, the point will change size a little bit when you zoom in to the map, i.e. a small point when zoomed out will appear little larger when you zoom in to the map, but will still shrink along the way. Try it out and find the setting that works for you. <data> can be any number between 25 and 100. 
 */
$xml .= "<state id=\"scale_points\"><data>50</data></state>";
/*
 * hover:   This is where we set the font size, text color, and background color of the text field that displays the state name and hover data. A size of 14 will show text 14 pixels tall. If none of this information specified, the default font is 11, with white text on black. 
 */
$xml .= "<state id=\"hover\"><font_size>14</font_size><font_color>ffffff</font_color><background_color>990000</background_color></state>";

if($_REQUEST['indid']) {
	$indicador = $db->pegaLinha("SELECT * FROM painel.indicador i 
								 LEFT JOIN painel.unidademeta u ON i.umeid=u.umeid WHERE indid='".$_REQUEST['indid']."'");
	if($indicador) {
		$regid   = $indicador['regid'];
		$umedesc = $indicador['umedesc'];
		switch($indicador['unmid']) {
			case '5':
				$formatoinput = array('mascara'             => '###.###.###.###,##',
									  'label'               => 'R$');
				break;
			case '3':
				$formatoinput = array('mascara'             => '##########',
									  'label'               => 'Qtde');
				
				if($indicador['indqtdevalor'] == "t") {
					// mostar os dois campos (quantidade e valor)
					$formatoinput['campovalor'] = array('mascara'             => '###.###.###.###,##',
									  					'label'               => 'R$');
				}
				break;
			default:
				$formatoinput = array('mascara'             => '##########',
									  'label'               => 'Qtde');
		}
		
		if($_REQUEST['dpeid_inicio'] && $_REQUEST['dpeid_fim']){
			$sql = "select dpedatainicio from painel.detalheperiodicidade where dpeid = {$_REQUEST['dpeid_inicio']}";
			$data1 = $db->pegaUm($sql);
			$sql = "select dpedatainicio from painel.detalheperiodicidade where dpeid = {$_REQUEST['dpeid_fim']}";
			$data2 = $db->pegaUm($sql);
			$filtroseh .= "AND
							dpedatainicio between '{$data1}' and '{$data2}' ";
		}
		
		
		if($indicador['indcumulativo'] == 'f') {
			if($_REQUEST['sehid']) {
				$filtro  = explode(";",$_REQUEST['sehid']);
				$dpeid_fim = end($filtro);
				$filtroseh .= "AND sehid='".$dpeid_fim."'";
			} else {
				$filtroseh .= "AND sehstatus='A'";
			}
		} else {
			if($sehid) {
				$filtro  = explode(";",$_REQUEST['sehid']);
				$filtroseh .= "AND seh.sehid IN('".implode("','",$filtro)."')";
			}
		}
		
	}
}


switch($regid) {
case REGIONALIZACAO_ESCOLA:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}
	
	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio 
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";
	
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'><name>".$dadosest['estdescricao']."</name><data>".(($regcod*100)+$id)."</data></state>";		
			}
		}
	}
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\"><name>".$p['mundescricao']."".(($p['qtde'] && $indicador['unmid'] != 1)? "(".$p['qtde'].")":"")."</name><loc>".$latitude.",".$longitude."</loc><url>javascript:amun('".$p['muncod']."');</url></state>";
			}
		}
	}
	
	break;
	
case REGIONALIZACAO_IES:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}
	

	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN painel.ies ies ON ies.iesid = CAST(dsh.dshcod as integer)
		   INNER JOIN territorios.municipio mun ON mun.muncod = ies.iesmuncod  
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";
	
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'><name>".$dadosest['estdescricao']."</name><data>".(($regcod*100)+$id)."</data></state>";		
			}
		}
	}
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\"><name>".$p['mundescricao']."".(($p['qtde'] && $indicador['unmid'] != 1)? "(".$p['qtde'].")":"")."</name><loc>".$latitude.",".$longitude."</loc><url>javascript:amun('".$p['muncod']."');</url></state>";
			}
		}
	}
	
	break;

case REGIONALIZACAO_POSGRADUACAO:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}
	

	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio 
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";
	
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'><name>".$dadosest['estdescricao']."</name><data>".(($regcod*100)+$id)."</data></state>";		
			}
		}
	}
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\"><name>".$p['mundescricao']."".(($p['qtde'] && $indicador['unmid'] != 1)? "(".$p['qtde'].")":"")."</name><loc>".$latitude.",".$longitude."</loc><url>javascript:amun('".$p['muncod']."');</url></state>";
			}
		}
	}
	
	break;
	
case REGIONALIZACAO_CAMPUS_SUPERIOR:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}
	

	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio 
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";
	
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'><name>".$dadosest['estdescricao']."</name><data>".(($regcod*100)+$id)."</data></state>";		
			}
		}
	}
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\"><name>".$p['mundescricao']."".(($p['qtde'] && $indicador['unmid'] != 1)? "(".$p['qtde'].")":"")."</name><loc>".$latitude.",".$longitude."</loc><url>javascript:amun('".$p['muncod']."');</url></state>";
			}
		}
	}
	
	break;

case REGIONALIZACAO_HOSPITAL:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}
	

	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio 
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";
	
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'><name>".$dadosest['estdescricao']."</name><data>".(($regcod*100)+$id)."</data></state>";		
			}
		}
	}
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\"><name>".$p['mundescricao']."".(($p['qtde'] && $indicador['unmid'] != 1)? "(".$p['qtde'].")":"")."</name><loc>".$latitude.",".$longitude."</loc><url>javascript:amun('".$p['muncod']."');</url></state>";
			}
		}
	}
	
	break;
	
case REGIONALIZACAO_UNIVERSIDADE:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}
	

	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio 
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";
	
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'><name>".$dadosest['estdescricao']."</name><data>".(($regcod*100)+$id)."</data></state>";		
			}
		}
	}
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\"><name>".$p['mundescricao']."".(($p['qtde'] && $indicador['unmid'] != 1)? "(".$p['qtde'].")":"")."</name><loc>".$latitude.",".$longitude."</loc><url>javascript:amun('".$p['muncod']."');</url></state>";
			}
		}
	}
	
	break;
	
case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}
	

	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio 
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";
	
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'><name>".$dadosest['estdescricao']."</name><data>".(($regcod*100)+$id)."</data></state>";		
			}
		}
	}
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\"><name>".$p['mundescricao']."".(($p['qtde'] && $indicador['unmid'] != 1)? "(".$p['qtde'].")":"")."</name><loc>".$latitude.",".$longitude."</loc><url>javascript:amun('".$p['muncod']."');</url></state>";
			}
		}
	}
	
	break;
	
	
case REGIONALIZACAO_INSTITUTO:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}
	

	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio 
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";
	
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'><name>".$dadosest['estdescricao']."</name><data>".(($regcod*100)+$id)."</data></state>";		
			}
		}
	}
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\"><name>".$p['mundescricao']."".(($p['qtde'] && $indicador['unmid'] != 1)? "(".$p['qtde'].")":"")."</name><loc>".$latitude.",".$longitude."</loc><url>javascript:amun('".$p['muncod']."');</url></state>";
			}
		}
	}
	
	break;
	
	
	
	
case REGIONALIZACAO_MUN:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND mun.estuf='".$_REQUEST['estuf']."'";
	}
	
	if($_REQUEST['muncod']) {
		$filtroseh .= " AND mun.muncod='".$_REQUEST['muncod']."'";
	}
	if($_REQUEST['tpmid']) {
		$filtroseh .= " AND mun.muncod in (select muncod from territorios.muntipomunicipio where tpmid =".$_REQUEST['tpmid'].") ";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}

	
	$sql = "SELECT 
			DISTINCT mun.muncod, 
			UPPER(mun.mundescricao) as mundescricao, 
			CASE WHEN (length (mun.munmedlat)=8) THEN 
	        	CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
	            	((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            	(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
	            END
	        ELSE
	            CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
	        	    ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
	            ELSE
	            0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
	            END
	       END as munmedlat, 
	       (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1) as munmedlog,
	       trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde
		   FROM painel.seriehistorica seh 
		   INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
		   LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
		   INNER JOIN territorios.municipio mun ON mun.muncod = dsh.dshcodmunicipio 
		   WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh."
		   GROUP BY mun.muncod, mun.mundescricao, mun.munmedlat, mun.munmedlog";

	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'>
							<name>".$dadosest['estdescricao']."</name>
							<data>".(($regcod*100)+$id)."</data>
						</state>";		
			}
		}
	}
	
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$latitude  = round($p['munmedlat'],5);
			$longitude = round($p['munmedlog'],5);
			if($latitude && $longitude) {
				$xml .= "<state id=\"point\">
							<name>".$p['mundescricao']."(".(($p['qtde'])?$p['qtde']:"-").")</name>
							<loc>".$latitude.",".$longitude."</loc>
							<url>javascript:amun('".$p['muncod']."');</url>
						 </state>";
			}
		}
	}
	
	break;
	
case REGIONALIZACAO_UF:
	if($_REQUEST['regcod'] && $_REQUEST['regcod'] != "todos")
		$filtroseh .= " AND mun.estuf in ( select estuf from territorios.estado where regcod = '{$_REQUEST['regcod']}' ) ";
	if($_REQUEST['estuf']) {
		$filtroseh .= " AND est.estuf='".$_REQUEST['estuf']."'";
	}
	//Adiciona filtro de Detalhes do Indicador
	if($_REQUEST['tidid1'] && $_REQUEST['tidid1'] != "todos"){
		$filtroseh .= " AND dsh.tidid1 = {$_REQUEST['tidid1']} ";
	}
	if($_REQUEST['tidid2'] && $_REQUEST['tidid2'] != "todos"){
		$filtroseh .= " AND dsh.tidid2 = {$_REQUEST['tidid2']} ";
	}

	$sql = "SELECT est.estuf, trim(to_char(SUM(dsh.dshqtde), '".str_replace(array(".",",","#"),array("g","d","9"),$formatoinput['mascara'])."')) as qtde  
			FROM painel.seriehistorica seh 
			INNER JOIN painel.detalheseriehistorica dsh ON dsh.sehid = seh.sehid
			LEFT JOIN painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid 
			INNER JOIN territorios.estado est ON est.estuf = dsh.dshuf 
			WHERE seh.indid='".$indicador['indid']."' AND seh.sehstatus!='I' ".$filtroseh." GROUP BY est.estuf";
	
	$pontos = $db->carregar($sql);
	
	if($pontos[0]) {
		foreach($pontos as $p) {
			$dadosconsolidados[$p['estuf']] = $p['qtde'];
		}
	}
	if($estregconsolidado) {
		foreach($estregconsolidado as $regcod => $estados) {
			foreach($estados as $id => $dadosest) {
				$xml .= "<state id='".$dadosest['estuf']."'>
							<name>".$dadosest['estdescricao']."(".(($dadosconsolidados[$dadosest['estuf']])?$dadosconsolidados[$dadosest['estuf']]:"-").")</name>
							<url>javascript:aest('".$dadosest['estuf']."');</url>
							<data>".(($regcod*100)+$id)."</data>
						</state>";		
			}
		}
	}
	break;

}
//dbg($sql);
//dbg($_REQUEST);
$xml .= "</countrydata>";

echo utf8_encode($xml);
?>