<?
 /*
   Sistema Simec
   Setor responsбvel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Mуdulo:simec.php
   Finalidade: permitir a abertura de todas as pбginas do sistema com seguranзa
   */

//Mede o tempo de execuзгo da pбgina em microsegundos
function getmicrotime()
{
	list( $usec, $sec ) = explode( " ", microtime() );
	return ( (float) $usec + (float) $sec );
}
$Tinicio = getmicrotime();

//controla cache de navegaзгo
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );
header( "Last-Modified: " . gmdate( "D, d M Y H:i:s" ) . " GMT" );
header( "Pragma: no-cache" );

/* Define o limite de tempo do cache em 100 minutos */

include "config.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();
//Emula outro usuбrio

if ( $_POST['usucpf_simu'] and ( $_SESSION['superuser'] or $db->testa_uma() or $_SESSION['usuuma'] ) )
{
	$_SESSION['usucpf'] = $_POST['usucpf_simu'];
}

if( ! isset( $_SESSION['usucpf'] ) )
{
    $_SESSION = array();
	$_SESSION['MSG_AVISO'] = 'Sua sessгo expirou ou nгo foi autorizada. Faзa novo acesso.';
	header('Location: login.php');
    exit();
}
$modulo = $_REQUEST['modulo'];

if ( ! $_SESSION['exercicio'] )
{
	//$sql= "select prsano,prsativo from ".$_SESSION['sisdiretorio'].".programacaosimec where prsexerccorrente='t' order by prsano desc";
	$sql= "select prsano,prsativo,prsexercicioaberto from ".$_SESSION['sisdiretorio'].".programacaoexercicio where prsexerccorrente='t' order by prsano desc";
	$RS = $db->record_set($sql);
	$res = $db->carrega_registro($RS,0);
	$_SESSION['exercicio']= $res['prsano'];
	$_SESSION['anoexercicio']= $res['prsano'];
	$_SESSION['exercicioaberto']= $res['prsexercicioaberto'];
}
else 
{
	//$sql= "select prsano,prsativo from ".$_SESSION['sisdiretorio'].".programacaosimec where prsexerccorrente='t' order by prsano desc";
	$sql= "select prsano,prsativo,prsexercicioaberto from ".$_SESSION['sisdiretorio'].".programacaoexercicio where prsexerccorrente='t' order by prsano desc";	
	$RS = $db->record_set($sql);
	$res = $db->carrega_registro($RS,0);
}

if ( ! $res['prsativo'] and ! $db->testa_superuser() )
{
	   header("Location: manutencao.php");
}

$sql = "select ittemail, orgcod,ittabrev from instituicao where ittstatus='A'";
$RS = $db->record_set( $sql );
$res = $db->carrega_registro( $RS, 0 );
$_SESSION['ittemail'] = trim( $res['ittemail'] );
$_SESSION['ittorgao'] = trim( $res['orgcod'] );
$_SESSION['ittabrev'] = trim( $res['ittabrev'] );
$_SESSION['sigla'] = trim( $res['ittsistemasigla'] );

$sql = "select u.usuchaveativacao from seguranca.usuario u where u.usucpf='" . $_SESSION['usucpf'] . "'";
$chave = $db->recuperar( $sql );

// include "includes/erros.inc";

if ( $chave['usuchaveativacao'] == 'f' )
{
    include "monitora/modulos/sistema/usuario/altsenha.inc";
	include APPRAIZ."includes/rodape.inc";
}
else
{
    if ( $_REQUEST['modulo'] )
    {
        include APPRAIZ . 'includes/testa_acesso.inc';
        include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
	   	include APPRAIZ . "includes/rodape.inc";
    }
    else
    {
	   header( "Location: login.php" );
    }
}

?>