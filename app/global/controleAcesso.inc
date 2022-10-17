<?
// carrega a página solicitada pelo usuário
$sql = sprintf( "select u.usuchaveativacao from seguranca.usuario u where u.usucpf = '%s'", $_SESSION['usucpf'] );
$chave = $db->pegaUm( $sql );
$deveAlterarSenha = false;

register_shutdown_function( 'gravarEstatistica' );

if ( $chave == 'f' )
{
	$deveAlterarSenha = true;
	// leva o usuário para o formulário de troca de senha
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
	// leva o usuário para o formulário de login
	header( "Location: /login.php" );
}