<?php

header( "Content-Type: text/plain" );

// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// abre conexуo com o servidor de banco de dados
$db = new cls_banco();

$_SESSION['usucpf'] = '00000000191';
$_SESSION['usucpforigem'] = '00000000191';

$pflcodCoor = 1; // perfil Coordenador de Aчуo
$pflcodCons = 7; // perfil Consulta
$tprcodAcao = 8; // responsabilidade do tipo Aчуo

$sql = "
	select
		pu.usucpf,
		us.usunome
	from seguranca.perfilusuario pu
		inner join seguranca.usuario us on
			us.usucpf = pu.usucpf
	where
		pu.pflcod = '" . $pflcodCoor . "' and
		pu.usucpf not in (
			select
				ur.usucpf
			from monitora.tprperfil tp
				inner join monitora.usuarioresponsabilidade ur on
					ur.pflcod = tp.pflcod
			where
				tp.tprcod = " . $tprcodAcao . " and -- esponsabilidade do tipo Aчуo
				tp.pflcod = '" . $pflcodCoor . "' and -- perfil Coordenador de Aчуo
				ur.acaid is not null -- com responsabilidade em uma aчуo
			group by
				ur.usucpf
		)
	group by
		pu.usucpf,
		us.usunome
";

$listaCpf = $db->carregar( $sql );
$listaCpf = $listaCpf ? $listaCpf : array();

$sqlBaseRemovePerfilCoor = "
	delete from seguranca.perfilusuario
	where
		usucpf = '%s' and
		pflcod = '" . $pflcodCoor . "'
";

$sqlBaseAdicionaPerfilCons = "
	insert into seguranca.perfilusuario
	(
		usucpf,
		pflcod
	)
	values
	(
		'%s',
		'" . $pflcodCons . "'
	)
";

$sqlBaseVerificaPerfilCons = "
	select
		count(*)
	from seguranca.perfilusuario
	where
		usucpf = '%s' and
		pflcod = '" . $pflcodCons . "'
";

$removidos = 0;

foreach ( $listaCpf as $linha )
{
	$usucpf = $linha['usucpf'];
	$usunome = $linha['usunome'];
	
	echo $usucpf . " " . $usunome . "\n";
	
	// remove perfil de coordenador
	$sql = sprintf( $sqlBaseRemovePerfilCoor, $usucpf );
	//echo $sql . ";\n";
	$db->executar( $sql );
	
	// adicionar perfil de consulta
	$sql = sprintf( $sqlBaseVerificaPerfilCons, $usucpf );
	$possuiPerfilCons = (boolean) $db->pegaUm( $sql );
	if ( !$possuiPerfilCons )
	{
		$sql = sprintf( $sqlBaseAdicionaPerfilCons, $usucpf );
		//echo $sql . ";\n\n\n\n\n\n\n\n";
		$db->executar( $sql );
	}
	
	$removidos++;
}

echo "\nTOTAL " . $removidos;

$db->commit();

?>