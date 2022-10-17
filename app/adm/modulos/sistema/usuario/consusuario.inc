<?php


	$configuracao = array();
	array_push( $configuracao, $_SESSION['sisid'] == 1 ? 'monitoramento' : 'projetos_especiais'  );
	
	$perfilcad	= arrayPerfil();
	
 	if( (count($perfilcad) == 1 && in_array(DEMANDA_PERFIL_DEMANDANTE, $perfilcad)) 
 		|| in_array(DEMANDA_PERFIL_TECNICO, $perfilcad) ){
 	    include APPRAIZ .'demandas/modulos/sistema/usuario/cadastro_usuario_demanda.php';
 	}
 	else{
		include APPRAIZ .'seguranca/modulos/sistema/usuario/consusuario.inc';
 	}	
	
?>