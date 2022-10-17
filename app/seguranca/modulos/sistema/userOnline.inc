<?php
//header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
//header("Cache-Control: post-check=0, pre-check=0", false);
//header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
//header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past
//

//include APPRAIZ . 'includes/cabecalho.inc';
#include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';
monta_titulo('Usuários Online', '');


$sql = "SELECT
			'<a href=\"javascript:void(0);\" onclick=\"abreChat( '|| s.sisid ||' );\">' || sisdsc || '</a>' AS sisdsc,
			qtdonline
		FROM
			seguranca.qtdusuariosonline u 
			INNER JOIN seguranca.sistema s ON s.sisid = u.sisid
		ORDER BY
			qtdonline DESC";

include APPRAIZ. 'includes/classes/relatorio.class.inc';

$dados = $db->carregar($sql);

$agrup = array(
				"agrupador" => array(
									 array(
											"campo" => "sisdsc",
									  		"label" => "Sistema"										
							   			   )
									),
				"agrupadoColuna" => array(
									   		"qtdonline", 
										  )	  
				);
$coluna = array(
				array(
					  "campo" 	 => "qtdonline",
				   	  "label" 	 => "Usuários Online",
					  "type"     => "numeric"
					 )
				);
//dbg($sql,1);
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($coluna);
$r->setTotalizador(false);
echo $r->getRelatorio();

//$cabecalho = array("Sistema", "Usuários Online");
//$db->monta_lista($sql, $cabecalho, 50, 10, '','','');
?>
<script>
function abreChat(sisid){
	var j = window.open('/geral/usuarios_online.php?sisid=' + sisid, 'chat', 'location=0,status=1,scrollbars=0,width=600,height=500');
	j.focus();
	j.moveTo(150,100);;
}
</script>