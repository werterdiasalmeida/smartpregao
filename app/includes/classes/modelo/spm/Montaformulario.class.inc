<?php
	
class Montaformulario{
	
	private $htm;

	public function montaNovoFormulario( $perid, $qrpid, $percent = 90 ){
		
		$tp = $perid == 433 ? 1 : 2;
		
		//RESPOSTAS
		$obIdent = new Identificacao( );
		$arrResp = $obIdent->recuperarResposta( $perid, $qrpid );
		
		$bgcolor = "#A8A8A8";
		
		$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
		$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
		$htm .= "<input type='hidden' name='identFormulario' id='identFormulario' value='1'>";
		$htm .= "<table class='tabela' style='width:{$percent}%;' align='center'>";
		$htm .= "<tr>";
		$htm .= "<td bgcolor='$bgcolor' colspan='50' align='center'><b>Formulário de Identificação</b></td>";
		$htm .= "</tr>";
		if( $tp == 1 ){
			$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='2'><b>1.1 Município: ".campo_texto('idtmun','S','S','',50,100,'','','','','','idtmun', '', trim($arrResp['idtmun']))."</b></td>";
	    	$htm .= "<td nowrap><b>1.2 UF: ".campo_texto('idtuf','S','S','',10,2,'','','','','','idtuf', '', $arrResp['idtuf'])."</b></td>";
	    	$htm .= "<td nowrap><b>1.3 Data Preenchimento: ".campo_data2('idtdtpreench','S','S','','','','', formata_data($arrResp['idtdtpreench']))."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
			$htm .= "<td nowrap colspan='4'><b>1.4 Responsável pelo Organismo Municipal para Políticas para Mulheres: ".campo_texto('idtresp','S','S','',68,100,'','','','','','idtresp', '', $arrResp['idtresp'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='2'><b>1.5 Cargo: ".campo_texto('idtcargo','S','S','',55,50,'','','','','','idtcargo', '', $arrResp['idtcargo'])."</b></td>";
	    	$htm .= "<td nowrap colspan='2'><b>1.6 Formação: ".campo_texto('idtformacao','S','S','',54,50,'','','','','','idtformacao', '', $arrResp['idtformacao'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='2'><b>1.7 Endereço Eletrônico: ".campo_texto('idtemailorg','S','S','',40,20,'','','','','','idtemailorg', '', $arrResp['idtemailorg'])."</b></td>";
	    	$htm .= "<td nowrap colspan='2'><b>1.8 Telefone(s): ".campo_texto('idttelorg','S','S','',52,50,'','','','','','idttelorg', '', $arrResp['idttelorg'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='4'><b>1.9 Nome do Organismo: ".campo_texto('idtnomeorg','S','S','',121,100,'','','','','','idtnomeorg', '', $arrResp['idtnomeorg'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='4'><b>1.10 Endereço para correspondências: ".campo_texto('idtend','S','S','',106,100,'','','','','','idtend', '', $arrResp['idtend'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
			$htm .= "<td nowrap colspan='4'><b>1.11 Responsável pelo preenchimento do questionário: ".campo_texto('idtresppreench','S','S','',87,100,'','','','','','idtresppreench', '', $arrResp['idtresppreench'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap><b>1.12 Gerente: ".campo_texto('idtger','S','S','',35,100,'','','','','','idtger', '', $arrResp['idtger'])."</b></td>";
	    	$htm .= "<td nowrap colspan='2'><b>1.13 Telefone(s): ".campo_texto('idttel','S','S','',18,100,'','','','','','idttel', '', $arrResp['idttel'])."</b></td>";
	    	$htm .= "<td nowrap><b>1.14 Endereço Eletrônico: ".campo_texto('idtemail','S','S','',15,20,'','','','','','idtemail', '', $arrResp['idtemail'])."</b></td>";
		} else {
			$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='2'><b>1.1 UF: ".campo_texto('idtuf','S','S','',10,2,'','','','','','idtuf', '', $arrResp['idtuf'])."</b></td>";
	    	$htm .= "<td nowrap><b>1.2 Data Preenchimento: ".campo_data2('idtdtpreench','S','S','','','','', formata_data($arrResp['idtdtpreench']))."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
			$htm .= "<td nowrap colspan='4'><b>1.3 Responsável pelo Organismo Estadual de Políticas para Mulheres: ".campo_texto('idtresp','S','S','',68,100,'','','','','','idtresp', '', $arrResp['idtresp'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='2'><b>1.4 Cargo: ".campo_texto('idtcargo','S','S','',55,50,'','','','','','idtcargo', '', $arrResp['idtcargo'])."</b></td>";
	    	$htm .= "<td nowrap colspan='2'><b>1.5 Formação: ".campo_texto('idtformacao','S','S','',57,50,'','','','','','idtformacao', '', $arrResp['idtformacao'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='2'><b>1.6 Endereço Eletrônico: ".campo_texto('idtemailorg','S','S','',40,20,'','','','','','idtemailorg', '', $arrResp['idtemailorg'])."</b></td>";
	    	$htm .= "<td nowrap colspan='2'><b>1.7 Telefone(s): ".campo_texto('idttelorg','S','S','',56,50,'','','','','','idttelorg', '', $arrResp['idttelorg'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='4'><b>1.8 Nome do Organismo: ".campo_texto('idtnomeorg','S','S','',121,100,'','','','','','idtnomeorg', '', $arrResp['idtnomeorg'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='4'><b>1.9 Endereço para correspondências: ".campo_texto('idtend','S','S','',107,100,'','','','','','idtend', '', $arrResp['idtend'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
			$htm .= "<td nowrap colspan='4'><b>1.10 Responsável pelo preenchimento do questionário: ".campo_texto('idtresppreench','S','S','',87,100,'','','','','','idtresppreench', '', $arrResp['idtresppreench'])."</b></td>";
	  		$htm .= "</tr>";
	  		$htm .= "<tr>";
	    	$htm .= "<td nowrap colspan='2'><b>1.11 Telefone(s): ".campo_texto('idttel','S','S','',35,100,'','','','','','idttel', '', $arrResp['idttel'])."</b></td>";
	    	$htm .= "<td nowrap colspan='2'><b>1.12 Endereço Eletrônico: ".campo_texto('idtemail','S','S','',45,20,'','','','','','idtemail', '', $arrResp['idtemail'])."</b></td>";
		}
	    $htm .= "</tr>";
  		$htm .= "</table>";
  		
  		$this->htm = $htm;

	}
	
	function salvar() {
		
		$obIdent = new Identificacao( );
		$obIdent->excluirPorQuestionarioFormulario( $_POST['qrpid'], $_POST['perid'] );
		$obIdent->commit();
	
		$obIdent = new Identificacao( );
		
		$_REQUEST['idtdtpreench'] = formata_data_sql($_REQUEST['idtdtpreench']);
		
		$arrCampos = array('perid', 'qrpid', 'idtmun', 'idtuf', 'idtdtpreench', 'idtresp', 'idtcargo', 'idtformacao', 'idtemailorg', 
							  	'idttelorg', 'idtnomeorg', 'idtend', 'idtresppreench', 'idttel', 'idtemail', 'idtger');
		$obIdent->popularObjeto( $arrCampos );
		$obIdent->idtmun = trim($_POST['idtmun']);
		$obIdent->salvar();
		$obIdent->commit();
	}
	
	function show(){
		echo $this->htm;
	}
}