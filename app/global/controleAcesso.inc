<?
// carrega a p�gina solicitada pelo usu�rio
$sql = sprintf( "select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf'] );
$chave = $db->pegaUm( $sql );
$deveAlterarSenha = false;

register_shutdown_function( 'gravarEstatistica' );

if ( $chave == 'f' )
{
	$deveAlterarSenha = true;
	// leva o usu�rio para o formul�rio de troca de senha
	include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/sistema/usuario/dados.inc";
	include APPRAIZ . "includes/rodape.inc";
	include APPRAIZ . "includes/rodapem.inc";
}
else if ( $_REQUEST['modulo'] )
{
    include APPRAIZ . $_SESSION['sisdiretorio'] . "/modulos/" . $_REQUEST['modulo'] . ".inc";
	include APPRAIZ . "includes/rodape.inc";
	include APPRAIZ . "includes/rodapem.inc";

}
else
{
	// leva o usu�rio para o formul�rio de login
	header( "Location: /login.php" );
}