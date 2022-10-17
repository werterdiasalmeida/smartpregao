<?php
	$inner = '';
	if( $_REQUEST['modid'] )
	{
		$inner = 'inner join seguranca.moduloperfil mp ON mp.pflcod = p.pflcod and mp.modid = '.$_REQUEST['modid'].' ';
	}
	
	$sql = "select
				p.pflcod as codigo,
				p.pfldsc as descricao
			from
				seguranca.perfil p
			{$inner}
			where
				p.pflstatus='A'
				and p.pflnivel > 1 
				and p.sisid=". $sisid ."
			order by
				descricao";
	$db->monta_combo( "pflcod", $sql,  'S', "Selecione o perfil desejado", 'selecionar_perfil', '', '', '', 'S' );
?>

<script type="text/javascript">

	function selecionar_perfil(){
//		document.formulario.formulario.value = "";
//		document.formulario.submit();
	}

//	var especifica_perfil;
//	/**
//	 * Exibe opções relacionadas ao perfil. (ação, programa e projeto especial )
//	 */
//	function especificar_perfil( tipo ){
//		document.getElementById( "proposto_"+ tipo ).selectedIndex = -1;
//		switch ( tipo ) {
//			case 'E':
//				var paiid = $('[name=paiid]').val();
//				var param = "?campo=proposto_"+ tipo +"&paiid="+ paiid;
//				especifica_perfil = window.open( "../sigoei/geral/seleciona_escola.php"+param, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=500" );
//				break;
//			case 'A':
//				var paiid = $('[name=paiid]').val();
//				var param = "?campo=proposto_"+ tipo +"&paiid="+ paiid;
//				especifica_perfil = window.open( "../sigoei/geral/seleciona_acordo.php"+param, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=500" );
//				break;
//			case 'U':
//			case 'E':
//			case 'N':
//			default:
//				break;
//		}
//		especifica_perfil.focus();
//	}

	</script>