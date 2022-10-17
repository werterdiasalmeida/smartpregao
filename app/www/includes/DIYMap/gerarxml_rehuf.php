<?
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

$xml .= "<?xml version=\"1.0\" encoding=\"utf-8\"?>";

$xml .= "<countrydata>";
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
$xml .= "<state id=\"default_point\">
		<src>../../imagens/hospital_icon.jpg</src>
		<size>20</size></state>";
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
$xml .= "<state id=\"scale_points\"><data>100</data></state>";



$sql = "SELECT ent.entnome as entnome, en.medlatitude, en.medlongitude 
		FROM entidade.entidade ent 
		LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
		LEFT JOIN entidade.funentassoc fue ON fue.fueid = fen.fueid 
		LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
		INNER JOIN entidade.endereco en ON en.entid = ent.entid 
		WHERE fen.funid = '16' AND 
		(esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND
		en.medlatitude IS NOT NULL AND en.medlongitude IS NOT NULL
		GROUP BY ent.entnome, en.medlatitude, en.medlongitude";

$pontos = $db->carregar($sql);

if($pontos[0]) {
	foreach($pontos as $p) {
		unset($laitude, $longitude);
		$lat = explode(".", $p['medlatitude']);
		if($lat[0]) {
			$latitude = -1*($lat[0]+round($lat[1]/60, 5)+round($lat[2]/3600)); 
		}
		$lon = explode(".", $p['medlongitude']);
		if($lon[0]) {
			$longitude = -1*($lon[0]+round($lon[1]/60, 5)+round($lon[2]/3600)); 
		}
		if($latitude && $longitude) {
			$xml .= "<state id=\"point\"><name>".$p['entnome']."</name><loc>".$latitude.",".$longitude."</loc></state>";
		}
	}
}

$xml .= "<state id='AC'><name>Acre</name></state>";
$xml .= "<state id='AL'><name>Alagoas</name></state>";
$xml .= "<state id='AP'><name>Amapá</name></state>";
$xml .= "<state id='AM'><name>Amazonas</name></state>";
$xml .= "<state id='AMPA'><name>Amazonas / Para</name></state>";
$xml .= "<state id='BA'><name>Bahia</name></state>";
$xml .= "<state id='CE'><name>Ceará</name></state>";
$xml .= "<state id='CEPI'><name>Ceará / Piaui</name></state>";
$xml .= "<state id='DF'><name>Distrito Federal</name></state>";
$xml .= "<state id='ES'><name>Espírito Santo</name></state>";
$xml .= "<state id='GO'><name>Goiás</name></state>";
$xml .= "<state id='MA'><name>Maranhão</name></state>";
$xml .= "<state id='MT'><name>Mato Grosso</name></state>";
$xml .= "<state id='MS'><name>Mato Grosso do Sul</name></state>";
$xml .= "<state id='MG'><name>Minas Gerais</name></state>";
$xml .= "<state id='PA'><name>Pará</name></state>";
$xml .= "<state id='PB'><name>Paraíba</name></state>";
$xml .= "<state id='PR'><name>Paraná</name></state>";
$xml .= "<state id='PE'><name>Pernambuco</name></state>";
$xml .= "<state id='PI'><name>Piauí</name></state>";
$xml .= "<state id='RJ'><name>Rio de Janeiro</name></state>";
$xml .= "<state id='RN'><name>Rio Grande do Norte</name></state>";
$xml .= "<state id='RS'><name>Rio Grande do Sul</name></state>";
$xml .= "<state id='RO'><name>Rondônia</name></state>";
$xml .= "<state id='RR'><name>Roraima</name></state>";
$xml .= "<state id='SC'><name>Santa Catarina</name></state>";
$xml .= "<state id='SP'><name>São Paulo</name></state>";
$xml .= "<state id='SE'><name>Sergipe</name></state>";
$xml .= "<state id='TO'><name>Tocantins</name></state>";


$xml .= "</countrydata>";




echo utf8_encode($xml);
?>