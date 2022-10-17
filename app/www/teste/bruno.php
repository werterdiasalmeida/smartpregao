<?
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
restore_error_handler();
restore_exception_handler();
error_reporting( E_ALL );
//?p_ano=2008&p_uf=Acre&p_municipio=1200104&p_tp_entidade=&p_cgc=04508933000145
$ano = '2008';
$uf = 'AC';
$municipio = '1200104';
$cnpj = '04508933000145';
require_once APPRAIZ . "includes/Snoopy.class.php";
$conexao = new Snoopy;
$urlReferencia = "http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=%s&p_uf=%s&p_municipio=%s&p_tp_entidade=&p_cgc=%s";
$url = sprintf($urlReferencia, $ano, $uf, $municipio, $cnpj);
//$url = "http://www.google.com";		
//$conexao->submit($url, $postdata);
		$conexao->fetch($url);
		$resultado = $conexao->results;
		print $resultado;

die();
		$resultado = str_replace('#000099','#7E8E47',$resultado);
		$resultado = str_replace('#006699','#acbc73',$resultado);
		$resultado = str_replace('#F8C400','#ccd7a4',$resultado);
		$resultado = str_replace('#FFCC66','#ccd7a4',$resultado);


		$resultado = str_replace('<font face="Tahoma,Arial" size="2" color="#acbc73">','<font face="Tahoma,Arial" color="#333333" size="2">',$resultado);
		$resultado = str_replace('<font face="Tahoma,Arial" size="2" color="#FFFFFF">','<font face="Tahoma,Arial" color="#000000" size="2">',$resultado);

		$resultado = str_replace('Volta a consulta de liberações','',$resultado);



		print $resultado;
		
die();

require_once "config.inc";
//date_default_timezone_set('');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

//restore_error_handler();
//restore_exception_handler();
//error_reporting( E_ALL );
$_SESSION['usucpf'] = '70183040163';
$_SESSION['usucpforigem'] = '70183040163';
//$_SESSION['transacao'] = '1';

//$nome_bd     = 'simec_desenvolvimento';
//$servidor_bd = 'mecsrv168';
//$porta_bd    = '5432';
//$usuario_db  = 'simec';
//$senha_bd    = 'phpsimecao';
$db          = new cls_banco();

$con = pg_connect("host=".$servidor_bd." port=".$porta_bd." dbname=".$nome_bd."  user=".$usuario_db." password=".$senha_bd ."");
//pg_set_client_encoding($con,'LATIN5');

if ($_REQUEST['teste']) { 
	$teste = $_REQUEST['teste'];
}
else {
	$teste = 'TesteTeste';
}

$sql = "insert into carga.testedata (descricao,datatz,datasemtz) values('".$teste."', now(), now())";
pg_query($con, "insert into carga.testedata (descricao) values('".$teste."')");
//$db->executar($sql);

//$db->commit;

$sql = "select * from carga.testedata";

	$dados = $db->carregar( $sql );
	$dados = $dados ? $dados : array();	

foreach ( $dados as &$dado )
	{
		$dataHora = explode( ' ', $dado['datatz'] );
		$hora = substr( $dataHora[1], 0, 8 );
		$data = explode( '-', $dataHora[0] );
		$data = $data[2] . "/" . $data[1] . "/" . $data[0];
		$dado['datatz'] = $data . " " . $hora;
	}
foreach ( $dados as &$dado )
	{
		$dataHora = explode( ' ', $dado['datasemtz'] );
		$hora = substr( $dataHora[1], 0, 8 );
		$data = explode( '-', $dataHora[0] );
		$data = $data[2] . "/" . $data[1] . "/" . $data[0];
		$dado['datasemtz'] = $data . " " . $hora;
	}

	foreach ($dados as $item){
			dbg($item);	
		}
	


?>