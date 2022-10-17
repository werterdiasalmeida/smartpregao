<?php

while ( ob_get_level() > 1 )
{
	ob_end_clean();
}



$sql_nome = "SELECT a.aledtinic, a.aledtfim
			FROM seguranca.alerta a 						
			WHERE a.sisid = '".$_SESSION['sisid']."' ";

$dados = $db->recuperar($sql_nome);
$data_atual = date("Y-m-d");

if($dados){	
	if( ( verificaData($data_atual, $dados['aledtinic'], 0 )) && (verificaData($dados['aledtfim'], $data_atual, 0 ) ) ){
		echo("
		<script type='text/javascript'> 
		
		alert('teste');		
		window.open('http://simec-d/seguranca/seguranca.php?modulo=sistema/geral/telaAlerta&acao=I','Janela','height=380,width=480,status=yes,toolbar=no,menubar=no,location=no, left=200, top=200, scrollbars=yes');
		
		</script>	
		");
	}
}

function verificaData($dtmaior,$dtmenor,$dif) {
		
	if (
	(mktime (0,0,0,substr($dtmaior,5,2),substr($dtmaior,8,2),substr($dtmaior,0,4))) >=
	(mktime (0,0,0,substr($dtmenor,5,2), substr($dtmenor,8,2), substr($dtmenor,0,4)))	
	)
	{		
		return true;		
	}
	else {	
		return false;
	}
	
}


?>