<?

$expires = 3600;
$cache_time = mktime(0,0,0,date('m'),date('d')+1,date('Y'));

header("Expires: " . date("D, d M Y H:i:s",$cache_time) . " GMT");
header("Cache-Control: max-age=$expires, must-revalidate");
header('Content-type: image/jpeg');

require_once("./ClassImage.php");

$imagem = new ClassImage();

if($_REQUEST["w"] > 800 || $_REQUEST["h"] > 600){
	$_REQUEST["w"] = 800;
	$_REQUEST["h"] = 600;
}
	
$imagem->ResizeImage($_REQUEST["img"],$_REQUEST["w"],$_REQUEST["h"]);

?>