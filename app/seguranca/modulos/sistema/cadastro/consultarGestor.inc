<? 

/*
   Sistema Simec
   Setor responsável: DTI-MEC
   Desenvolvedor: Equipe Simec
   Analista: Luiz Fernando Carvalho Mendonça
   Programador: Afonso Alves Ribeiro
   Módulo:SIS
   Finalidade: efetuar uma consulta dos gestores de sistemas no SIS e em outros modulos.
   */


include  APPRAIZ."includes/cabecalho.inc";
if($_REQUEST['requisicao'] == 'excluir') {
	$gesid = $_REQUEST['gesid']; 
	$sql = "UPDATE seguranca.gestorsistema SET
			   gesativo = 'I'   
			WHERE gesid={$gesid}";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert ('Gestor excluído com sucesso.');
			window.location = 'seguranca.php?modulo=sistema/cadastro/consultarGestor&acao=A';
		  </script>";
}

?>
	<br/>
	<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
	<script type="text/javascript" src="../includes/remedial.js"></script>
	<script type="text/javascript" src="../includes/superTitle.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
	
	<script type="text/javascript">
		function alterarGestor(gesid) {
			window.location.href = "seguranca.php?modulo=sistema/cadastro/cadastrarGestor&acao=A&gesid=" +gesid;	
		}

		function excluirGestor(gesid) {
			if (confirm ('Deseja realmente excluir este gestor?')){
				window.location = "seguranca.php?modulo=sistema/cadastro/consultarGestor&acao=A&requisicao=excluir&gesid=" +gesid;
			}
		}
			
	</script>

	<?php
		$db->cria_aba( $abacod_tela, $url, '' );
		$titulo_modulo = "Consulta de Gestores";
		monta_titulo( $titulo_modulo, '<div style="margin:3px;" align="right" class="' . $class . '"Gestor&acao=A"></div>' );
	?>
	
	<form method="post" name="filtro">
		<input type="hidden" name="filtro" value="1"/>
		<table align="center" width="95%" cellSpacing="1" cellPadding="3" class="tabela" bgcolor="#f5f5f5" >
	      <tr>
	        <td align='center' class="SubTituloDireita">Sistema:</td>
	        <td>
				<?
				$sistema = $_POST['sisid'] ? $_POST['sisid'] : '';
				$sql = "SELECT sisid as codigo, sisdsc as descricao FROM seguranca.sistema ORDER BY sisdsc ";
				$db->monta_combo("sisid", $sql, 'S' , 'Escolha um sistema','','Todos os sistemas','','','','','',$sistema);
				?>	        
	        </td>
	      </tr>			
	      <tr>
	        <td align='center' class="SubTituloDireita">Nome:</td>
			<?
				$gestor = $_POST['gesnome'] ? $_POST['gesnome'] : '';
			?>	        
	        <td><?=campo_texto('gesnome','N','S','',60,50,'','','','','','','',$gestor);?></td>
	      </tr>
	      </table>
	      <div style="margin:3px;" align="center">
	      <input type="Button" style="width:80px;" name="Consultar" value="Consultar" onclick="document.filtro.submit();">
	      <input type="Button" style="width:80px;" name="Incluir" value="Incluir" onclick="window.location.href='seguranca.php?modulo=sistema/cadastro/cadastrarGestor&acao=A'">
	      </div>
	</form>

	<table align="right" width="95%" border="0" cellpadding="2" cellspacing="0" class="listagem" style="color:#333333">
		<tbody>
			<?
			if ( isset($_POST['sisid']) &&  $_POST['sisid'] != 'x'){
			$sql_lista = "
				select distinct
 				   '<img 	src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"  
							id=\"img ' ||  gestores.gesid || '\" name=\"+\" 
							onclick=\"javascript:alterarGestor(' || gestores.gesid || ');\"/>'
	
					|| '&nbsp;'
							
 				   '<img 	src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"  
							id=\"img ' ||  gestores.gesid || '\" name=\"+\" 
							onclick=\"javascript:excluirGestor(' || gestores.gesid || ');\"/>'							
							
							as acoes,
				   gestores.gesnome  as gestor,
				   gestores.gessetor as setor,
				   gestores.gesfone as telefone,
				   gestores.gesemail as email,
				   gestores.gesativo as status

				from 
				   seguranca.gestorsistema as gestores
				inner join
				   seguranca.sistemapossuigestor possui
				on gestores.gesid = possui.gesid
				
				where
				   possui.sisid = $_POST[sisid]";
			
			}
			else {
			$sql_lista = "
				select distinct
 				   '<img 	src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"  
							id=\"img ' ||  gestores.gesid || '\" name=\"+\" 
							onclick=\"javascript:alterarGestor(' || gestores.gesid || ');\"/>'
	
					|| '&nbsp;'
							
 				   '<img 	src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"  
							id=\"img ' ||  gestores.gesid || '\" name=\"+\" 
							onclick=\"javascript:excluirGestor(' || gestores.gesid || ');\"/>'							
							
							as acoes,
				   gestores.gesnome  as gestor,
				   gestores.gessetor as setor,
				   gestores.gesfone as telefone,
				   gestores.gesemail as email,
				   gestores.gesativo as status

				from 
				   seguranca.gestorsistema as gestores
				inner join
				   seguranca.sistemapossuigestor possui
				on gestores.gesid = possui.gesid";	
			}
			$cabecalho = array ("Ação", "Nome do Gestor", "Setor", "Telefone", "Email", "Status");
			$db->monta_lista($sql_lista,$cabecalho,60,20,'','','');
			?>
		</tbody>
	</table>	
<? 
	function is_combo_set($campo='') {
	$auxiliar = $_REQUEST[$campo];
	return isset($auxiliar) && $auxiliar != "" && $auxiliar != "x";
} 

function add_clausula_select($campo_formulario,$campo_base_dados,$is_string=true) {
	if ( is_combo_set($campo_formulario) ) {
		$parametro = $_REQUEST[$campo_formulario];
		return " and $campo_base_dados " . ($is_string ? " like '%":" = ") . $parametro . ($is_string ? "%'":"");
	} else {
		return " ";
	}
}?>