<?php
include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QArvore.class.inc";

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";


$obArvore = new QArvore( /*array("questionario" => new Questionario(10))*/ );

//array("questionario" => new Questionario(10) )
if ( $_GET['instrucao'] == 'excluir' ){
	//	$s  = new Sistema();
	$q  = new QQuestionario($_GET['queid']);
	$a  = new QGrupo($_GET['grpid']);
	$p  = new QPergunta($_GET['perid']);
	//	$it = new Itempergunta();

	if ($_GET['queid']){
		if( $q->excluir() ){
			$msg = 'Opera��o realizada com sucesso!';
			$q->commit();
		}else{
			$msg = 'Opera��o n�o realizada!\nExistem "Grupos/Perguntas" associadas a este Question�rio.';
		}
	}elseif ($_GET['grpid']){
		if($a->gru_grpid){
			$where = array("gru_grpid" => $a->gru_grpid);
		} elseif ($a->queid){
			$where = array("queid" => $a->queid);
		} else {
			$where = array("itpid" => $a->itpid);
		}
		if( $a->excluir() ){
			$msg = 'Opera��o realizada com sucesso!';
			$a->commit();
			$a->ordena($where);
		}else{
			$msg = 'Opera��o n�o realizada!\nExistem "Grupos/Perguntas" associadas a este grupo.';
		}
	}elseif ($_GET['perid']){
		if($p->grpid){
			$where = array("grpid" => $p->grpid);
		} elseif ($p->queid){
			$where = array("queid" => $p->queid);
		} else {
			$where = array("itpid" => $p->itpid);
		}
		if($p->perguntaRespondida()){
			$msg = 'Opera��o n�o realizada!\nEssa pergunta j� possui resposta.';
		} else {
			if( $p->excluir() ){
				$msg = 'Opera��o realizada com sucesso!';
				$p->commit();
				$p->ordena($where);
			}else{
				$msg = 'Opera��o n�o realizada!\nExistem "Sub-perguntas" associadas a esta pergunta.';
			}
		}
	}

	die("<script>alert('$msg'); window.location = 'seguranca.php?modulo=principal/questionario/arvore&acao=A&sisidFiltro=".$_GET['sisidFiltro']."';</script>");
}



include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( '�rvore de Question�rios', '' );

?>
<script
	language="javascript" type="text/javascript"
	src="../includes/dtree/dtree.js"></script>
	<script>
		function montarA( sisid ){
			if( sisid ){
				window.location = '?modulo=principal/questionario/arvore&acao=A&sisidFiltro=' + sisid;
			}
		}
	</script>
<link
	rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td><font>Filtre o sistema:</font> 
		<? 
		$sql = "SELECT
					sisid as codigo,
					sisdsc as descricao
				FROM
					seguranca.sistema
				WHERE
					sisstatus = 'A'";
		$db->monta_combo('sisfiltro',$sql,'',"Selecione",'montarA','','','300','S', 'sisfiltro', '', $_GET['sisidFiltro']) 
		?>
		</td>
	</tr>
<?php 
	if ($_GET['sisidFiltro']):
?>	
	<tr>
		<td>
			<div id="bloco" style="overflow: hidden;">
				<p>
					<a href="javascript: arvore.openAll();">Abrir Todos</a>
					&nbsp;|&nbsp; <a href="javascript: arvore.closeAll();">Fechar Todos</a>
				</p>
				<div id="_arvore"></div>
			</div>
		</td>
	</tr>
<?= 
	$obArvore->montaArvore( $_GET['sisidFiltro'] );		
	endif;
?>	
</table>