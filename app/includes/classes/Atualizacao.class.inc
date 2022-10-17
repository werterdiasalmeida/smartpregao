<?php
/*
 * Classe   Noticias
 * @author  Rodrigo Pereira de Souza Silva
 * @since   18/10/2010
 */
class Atualizacao {

	/**
	 * Atributos
	 */
	private $atuid;
	private $atudscsimplificada;
	private $atudsccompleta;
	private $atusisid;
	private $atudtinclusao;
	private $atustatus;
	private $db;
	
	/**
	 * Função __construct
	 * Método construtor do PHP
	 * 
	 * @author Rodrigo Pereira de Souza Silva
	 * @since  18/10/2010
	 * @return void
	 */	
	function __construct($css = '', $mensagem = '', $width = '550', $height = '280'){
		global $db;
		$this->db = (($db) ? $db : die("Falha na conexão com o Banco de Dados."));
		
		// Exibindo o ícone da interrogação para que o usuário possa ler as atualizações		
		echo '<div style="'.$css.'">
				  <table>
					<tr>
						<td align="right">
							<a style="text-decoration:none;" onclick="window.open( \'../geral/lista_atualizacoes.php\', \'visualizarExtrato\', \'width=780, height=465, status=1, menubar=0, toolbar=0, scrollbars=1, resizable=1\' );" href="#">
								<img alt="Atualizações" title="Atualizações" border="0" src="../includes/layout/fab/img/bt_help.png" align="right">
							</a>
						</td>
						<td align="left">
							<a style="text-decoration:none;" onclick="window.open( \'../geral/lista_atualizacoes.php\', \'visualizarExtrato\', \'width=780, height=465, status=1, menubar=0, toolbar=0, scrollbars=1, resizable=1\' );" href="#">
								<font color="#dd0000"><strong>Atualizações</strong></font>
							</a>
						</td>
					</tr>
				  </table>
				</div>';
		
		// PopUp
		//if( $this->pegaAtualizacaoDeHoje() && $popup ) {
			?>
			<!-- <link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
			<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
			<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
			<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
			<script type="text/javascript">
			/*** INICIO SHOW MODAL ***/
			
			function montaShowModal() {
				var aler='';
				aler += '<?php //echo $mensagem; ?>';
				aler += '<p align="center"><input type="button" value="Fechar" onclick="closeMessage();"></p>';
				displayStaticMessage(aler,false);
				return false;
			}
			
			function displayStaticMessage(messageContent,cssClass) {
				messageObj = new DHTML_modalMessage();	// We only create one object of this class
				messageObj.setShadowOffset(5);	// Large shadow
				
				messageObj.setHtmlContent(messageContent);
				messageObj.setSize(<?php //echo $width; ?>,<?php //echo $height; ?>);
				messageObj.setCssClassMessageBox(cssClass);
				messageObj.setSource(false);	// no html source since we want to use a static message here.
				messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
				messageObj.display();
			}
			
			function closeMessage() {
				messageObj.close();	
			}

			jQuery(window).load(function($){
				montaShowModal();
			});
			
			</script>-->
			<?php
			//}// fim do if
		
	}// fim do método construtor
	
	function pegaAtualizacaoDeHoje(){
		
		global $db;
		
		$sql = "SELECT 
					atudtinclusao
  				FROM 
  					public.atualizacao
  				WHERE
  					atustatus = 'A'
  				AND atusisid = {$_SESSION["sisid"]}
  				AND atudtinclusao = '".date('Y-m-d')."'";
		
		if( $db->pegaUm($sql) ){
			return true;
		}else{
			return false;
		}
		
	}
	
}// fim da classe
?>