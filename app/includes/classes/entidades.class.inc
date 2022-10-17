<?php
/*
 * Classe   Entidades
 * @author  Alexandre Dourado
 * @since   18/05/2009
 */
class Entidades {

	private $entid;
	private $njuid;
	private $entnumcpfcnpj;
	private $entnome;
	private $entemail;
	private $entnuninsest;
	private $entobs;
	private $entstatus;
	private $entnumrg;
	private $entorgaoexpedidor;
	private $entsexo;
	private $entdatanasc;
	private $entdatainiass;
	private $entdatafimass;
	private $entnumdddresidencial;
	private $entnumresidencial;
	private $entnumdddcomercial;
	private $entnumramalcomercial;
	private $entnumcomercial;
	private $entnumdddfax;
	private $entnumramalfax;
	private $entnumfax;
	private $tpctgid;
	private $tpcid;
	private $tplid;
	private $tpsid;
	private $entcodentsup;
	private $entcodent;
	private $entescolanova;
	private $entdatainclusao;
	private $entsig;
	private $entunicod;
	private $entungcod;
	private $entproep;
	private $entnumdddcelular;
	private $entnumcelular;
	private $entorgcod;
	private $entsede;
	private $db;
	private $funcoes;
	private $funcoesadicionar;
	private $_listatipoendereco;
	private $buscarentidadepornome;
	
	/**
	 * Função __construct
	 * Método construtor do PHP
	 * 
	 * @author Alexandre Dourado
	 * @since  18/05/2009
	 * @return void
	 */	
	function __construct($dados = false){
		global $db;
		$this->db = (($db)?$db:die("Conexão com o BD"));
		$this->carregarTipoEndereco();
		if($dados['entid']) $this->dados = $this->db->pegaLinha("SELECT * FROM entidade.entidade WHERE entid='".$dados['entid']."'");
	}
	/**
	 * Método que carrega os endereços por tipo de endereço
	 * 
	 * @author Alexandre Dourado
	 * @since  18/05/2009
	 * @return void
	 */	
	function carregarTipoEndereco() {
		$tiposendereco = $this->db->carregar("SELECT tpeid,UPPER(tpedsc) AS tpedsc,tpestatus FROM entidade.tipoendereco");
		if($tiposendereco[0]) {
			foreach($tiposendereco as $tendereco) {
				$this->_listatipoendereco[$tendereco['tpeid']] = $tendereco; 
			}
		}
	}
	/**
	 * Carregar objeto de entidade
	 * 
	 * @author Alexandre Dourado
	 * @param array $entidade contendo dados da entidade
	 * @version v1.0 17/12/2008
	 */
	function carregarEntidade($entidade) {
		$this->entid 			 	= $entidade['entid'];
		$this->njuid 			 	= $entidade['njuid'];
		$this->entnumcpfcnpj 	 	= $entidade['entnumcpfcnpj'];
		$this->entnome 			 	= $entidade['entnome'];
		$this->entemail 		 	= $entidade['entemail'];
		$this->entnuninsest 	 	= $entidade['entnuninsest'];
		$this->entobs 			 	= $entidade['entobs'];
		$this->entstatus 		 	= $entidade['entstatus'];
		$this->entnumrg 		 	= $entidade['entnumrg'];
		$this->entorgaoexpedidor 	= $entidade['entorgaoexpedidor'];
		$this->entsexo 			 	= $entidade['entsexo'];
		$this->entdatanasc 		 	= $entidade['entdatanasc'];
		$this->entdatainiass 	 	= $entidade['entdatainiass'];
		$this->entdatafimass 	 	= $entidade['entdatafimass'];
		$this->entnumdddresidencial = $entidade['entnumdddresidencial'];
		$this->entnumresidencial 	= $entidade['entnumresidencial'];
		$this->entnumdddcomercial 	= $entidade['entnumdddcomercial'];
		$this->entnumramalcomercial = $entidade['entnumramalcomercial'];
		$this->entnumcomercial 		= $entidade['entnumcomercial'];
		$this->entnumdddfax 		= $entidade['entnumdddfax'];
		$this->entnumramalfax 		= $entidade['entnumramalfax'];
		$this->entnumfax 			= $entidade['entnumfax'];
		$this->tpctgid 				= $entidade['tpctgid'];
		$this->tpcid 				= $entidade['tpcid'];
		$this->tplid 				= $entidade['tplid'];
		$this->tpsid 				= $entidade['tpsid'];
		$this->entcodentsup 		= $entidade['entcodentsup'];
		$this->entcodent 			= $entidade['entcodent'];
		$this->entescolanova 		= $entidade['entescolanova'];
		$this->entdatainclusao 		= $entidade['entdatainclusao'];
		$this->entsig 				= $entidade['entsig'];
		$this->entunicod 			= $entidade['entunicod'];
		$this->entungcod 			= $entidade['entungcod'];
		$this->entproep 			= $entidade['entproep'];
		$this->entnumdddcelular 	= $entidade['entnumdddcelular'];
		$this->entnumcelular 		= $entidade['entnumcelular'];
		$this->entorgcod 			= $entidade['entorgcod'];
		$this->entsede 				= $entidade['entsede'];
		$this->entrazaosocial		= $entidade['entrazaosocial'];
		$this->funcoes 				= $this->carregarFuncoesEntidade();
		$this->enderecos			= $this->carregarEnderecosEntidade($entidade['endereco']);
	}
	
	function setBuscarEntidadePorNome($tipo) {
		$this->buscarentidadepornome = $tipo;
	}
	
	function carregarFuncoesEntidade() {
		if($this->entid) {
			$funcoes = $this->db->carregar("SELECT * FROM entidade.funcaoentidade fen 
											LEFT JOIN entidade.funcao fun ON fen.funid = fun.funid 
											LEFT JOIN entidade.funentassoc fea ON fen.fueid = fea.fueid  
											LEFT JOIN entidade.entidade ent ON ent.entid = fea.entid 
							 				WHERE fen.entid='".$this->entid."'");
			if($funcoes[0])
				return $funcoes;
			else
				return false;
		}
		return false;
	}
	
	function adicionarFuncoesEntidade($dados) {
		$this->funcoesadicionar[] = $dados;
	}
	
	function carregarEnderecosEntidade($dados = null) {
		if($this->entid && !$dados) {
			$enderecos =  $this->db->carregar("SELECT edo.*, tpo.tpedsc, mun.mundescricao FROM entidade.endereco edo 
											   LEFT JOIN entidade.tipoendereco tpo ON tpo.tpeid = edo.tpeid 
											   LEFT JOIN territorios.municipio mun ON mun.muncod = edo.muncod  
							 				   WHERE edo.entid='".$this->entid."'");
			if($enderecos[0]) {
				foreach($enderecos as $endereco) {
					if($endereco['tpeid']) // somente carregar os endereços com tipo
						$enderecof[$endereco['tpeid']] = $endereco; 
				}
				return $enderecof;
			}
			return false;
		} else {
			if($dados) {
				foreach($dados as $tpeid => $endereco) {
					$enderecof[$tpeid] = $endereco;
				}
				return $enderecof;
			}
			return false;
		}
	}
	
	
	function carregarPorEntid($entid) {
		$this->carregarEntidade($this->db->pegaLinha("SELECT ent.* FROM entidade.entidade ent WHERE entid='".$entid."'"));
	}
	
	function carregarPorFuncaoEntAssociado($funid, $entidassociado) {
		$this->carregarEntidade($this->db->pegaLinha("SELECT ent.* FROM entidade.entidade ent 
										  LEFT JOIN entidade.funcaoentidade fen ON ent.entid = fen.entid 
							 			  LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
							 			  WHERE fen.funid='".$funid."' AND fea.entid='".$entidassociado."'"));
	}
	
	function carregarCombosFormulario() {
		$naturezajuridica = $this->db->carregar("SELECT '<option value=\"'||njuid||'\" ".(($this->njuid)?" '||CASE WHEN njuid='".$this->njuid."' THEN 'selected' ELSE '' END||'":"").">'||njudsc||'</option>' as option FROM entidade.naturezajuridica ORDER BY njudsc");
		if($naturezajuridica[0]) {
			foreach($naturezajuridica as $n) {
				$dados['opcnaturezajuridica'][] = $n['option'];
			}
		}
		$tipocatescolaprivada = $this->db->carregar("SELECT '<option value=\"'||tpctgid||'\" ".(($this->tpctgid)?" '||CASE WHEN tpctgid='".$this->tpctgid."' THEN 'selected' ELSE '' END||'":"").">'||tpctgdesc||'</option>' as option FROM entidade.tipocategoriaescolaprivada WHERE tpctgstatus = 'A' ORDER BY tpctgdesc");
		if($tipocatescolaprivada[0]) {
			foreach($tipocatescolaprivada as $tcep) {
				$dados['opctipocatescolaprivada'][] = $tcep['option'];
			}
		}
		$tipoclassificacao = $this->db->carregar("SELECT '<option value=\"'||tpcid||'\" ".(($this->tpcid)?" '||CASE WHEN tpcid='".$this->tpcid."' THEN 'selected' ELSE '' END||'":"").">'||tpcdesc||'</option>' as option FROM entidade.tipoclassificacao WHERE tpcstatus = 'A' OR tpcstatus IS NULL ORDER BY tpcdesc");
		if($tipoclassificacao[0]) {
			foreach($tipoclassificacao as $tc) {
				$dados['opctipoclassificacao'][] = $tc['option'];
			}
		}
		$tipolocalizacao = $this->db->carregar("SELECT '<option value=\"'||tplid||'\" ".(($this->tplid)?" '||CASE WHEN tplid='".$this->tplid."' THEN 'selected' ELSE '' END||'":"").">'||tpldesc||'</option>' as option FROM entidade.tipolocalizacao WHERE tplstatus = 'A' ORDER BY tpldesc");
		if($tipolocalizacao[0]) {
			foreach($tipolocalizacao as $tl) {
				$dados['opctipolocalizacao'][] = $tl['option'];
			}
		}
		$tiposituacao = $this->db->carregar("SELECT '<option value=\"'||tpsid||'\" ".(($this->tpsid)?" '||CASE WHEN tpsid='".$this->tpsid."' THEN 'selected' ELSE '' END||'":"").">'||tpsdesc||'</option>' as option FROM entidade.tiposituacao WHERE tpsstatus = 'A' ORDER BY tpsdesc");
		if($tiposituacao[0]) {
			foreach($tiposituacao as $ts) {
				$dados['opctiposituacao'][] = $ts['option'];
			}
		}
		return $dados;
	}
	
	
	function formEntidade($formaction, $dadosfuncao = null, $carregarcomplementos = false, $funtipo = null, $bloq = '', $hide = '') {
		
		if($dadosfuncao){
			
			$funcaodados = $this->db->pegaLinha("SELECT funid,UPPER(fundsc) as fundsc,funstatus,funtipo,funcumulativo FROM entidade.funcao WHERE funid='".$dadosfuncao['funid']."'");
			
			if($dadosfuncao['entidassociado'])
				$entidassociadodados = $this->db->pegaLinha("SELECT * FROM entidade.entidade WHERE entid='".$dadosfuncao['entidassociado']."'");
		
		} else {
			
			if($funtipo){
				
				$funcaodados['funtipo'] = $funtipo;	

			} else {
				
				$funcaodados['funtipo'] = "F";
				
			}
			
		}
		
		$html .= "<script type=\"text/javascript\" src=\"/includes/prototype.js\"></script>
				  <script type=\"text/javascript\" src=\"/includes/ModalDialogBox/modal-message.js\"></script>
				  <script type=\"text/javascript\" src=\"/includes/ModalDialogBox/ajax-dynamic-content.js\"></script>
				  <script type=\"text/javascript\" src=\"/includes/ModalDialogBox/ajax.js\"></script>
				  <link rel=\"stylesheet\" href=\"/includes/ModalDialogBox/modal-message.css\" type=\"text/css\" media=\"screen\" />
				  <link rel=\"stylesheet\" href=\"/includes/entidadesn.css\" type=\"text/css\" media=\"screen\" />
            	  <script type=\"text/javascript\" src=\"/includes/entidadesn.js\"></script>
            	  <script>
				  function enviaForm()
				  	{
				  		document.frmEntidade.submit();
					}
				  var funidEspecifico = '" . ($carregarcomplementos['funidEspecifico'] ? json_encode($carregarcomplementos['funidEspecifico']) : '') . "';	            	  
				  </script>            	  
				  <form name=\"frmEntidade\" id=\"frmEntidade\" method=\"post\" action=\"".$formaction."\" onsubmit=\"\">
				  <input type=\"hidden\" id=\"entid\" name=\"entid\" value=\"".$this->entid."\">";
		
		if($dadosfuncao){
			$html .= "<input type=\"hidden\" id=\"funid\" name=\"funcoes[funid]\" value=\"".$dadosfuncao['funid']."\">
					  <input type=\"hidden\" id=\"entidassociado\" name=\"funcoes[entidassociado]\" value=\"".$dadosfuncao['entidassociado']."\">";
		}
		
		$html .= "<table  class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"tblentidade\">";
		
		
		if($carregarcomplementos["painel"]['secretaria']){
			$html .= $carregarcomplementos["painel"]['secretaria'];
		}
		
		$html .= "<tr id=\"msg_buscacnpj_receita\" style=\"display: none\"><td colspan=\"2\"  style=\"background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\"><img src=\"../imagens/restricao.png\"> <b>Essa não é a entidade que procura?</b> <a href=\"#\" onclick=\"confirmaBuscaReceitaPeloCNPJ($('entnumcpfcnpj').value)\">Clique aqui</a> para consultar a base da Receita Federal. </td></tr>";
		if($dadosfuncao){
			$html .= "<tr id=\"tr_titulo\"><td class=\"SubTituloCentro\" colspan=\"2\">".strtoupper($funcaodados['fundsc'])." ".(($entidassociadodados['entnome'])?"(".strtoupper($entidassociadodados['entnome']).")":"")."</td></tr>";
		}
		if($this->funcoes[0]) {
			foreach($this->funcoes as $funcao) {
				if ( is_array($carregarcomplementos['funidEspecifico']) && !in_array($funcao['funid'] , $carregarcomplementos['funidEspecifico']) ) {
					continue;
				}
				
				$optfuncoescadastradas .= "<option>".$funcao['fundsc'].(($funcao['entnome'])?" (".$funcao['entnome'].")":"")."</option>";
			}
		}// else {
		//	$optfuncoescadastradas = "<option>Não existem funções cadastradas</option>";
		//}
		$optfuncoescadastradas = empty($optfuncoescadastradas) ? "<option>Não existem funções cadastradas</option>" : $optfuncoescadastradas;
		( $bloq != '' ) ? $bloqclass = 'disabled' : $bloqclass = 'disabled' ;
		switch($funcaodados['funtipo']) {
			case 'F':
				
				require APPRAIZ . "www/includes/webservice/cpf.php";

				$html .= "<tr id=\"tr_entnumcpfcnpj\"><td class=\"SubTituloDireita\">CPF :</td><td><input ".$bloq."type=\"text\" name=\"entnumcpfcnpj\" id=\"entnumcpfcnpj\" size=\"17\" maxlength=\"14\" value=\"".$this->mascaraglobal($this->entnumcpfcnpj,"###.###.###-##")."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('###.###.###-##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);getEntidadePeloCPF(this.value);\"></td></tr>
						  <tr id=\"tr_entnome\"><td class=\"SubTituloDireita\">Nome :</td><td><input ".$bloq."type=\"text\" name=\"entnome\" id=\"entnome\" size=\"45\" maxlength=\"255\" value=\"".$this->entnome."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);".(($this->buscarentidadepornome)?"getEntidadePeloEntnome(this.value, '".$this->buscarentidadepornome."');":"")."\"></td></tr>
						  <tr id=\"tr_entemail\"><td class=\"SubTituloDireita\">Email :</td><td><input ".$bloq."type=\"text\" name=\"entemail\" id=\"entemail\" size=\"45\" maxlength=\"100\" value=\"".$this->entemail."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entnumrg\"><td class=\"SubTituloDireita\">Registro Geral (RG) :</td><td><input ".$bloq."type=\"text\" name=\"entnumrg\" id=\"entnumrg\" size=\"11\" maxlength=\"10\" value=\"".$this->entnumrg."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entorgaoexpedidor\"><td class=\"SubTituloDireita\">Orgão Expedidor :</td><td><input ".$bloq."type=\"text\" name=\"entorgaoexpedidor\" id=\"entorgaoexpedidor\" size=\"15\" maxlength=\"10\" value=\"".$this->entorgaoexpedidor."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entsexo\"><td class=\"SubTituloDireita\">Sexo :</td><td><select ".$bloq."name=\"entsexo\" class=\"CampoEstilo\" id=\"entsexo\"><option value=\"\" >Selecione</option><option value=\"M\" ".(($this->entsexo=="M")?"selected":"").">Masculino</option><option value=\"F\" ".(($this->entsexo=="F")?"selected":"").">Feminino</option></select></td></tr>
						  <tr id=\"tr_entdatanasc\"><td class=\"SubTituloDireita\">Data de Nascimento :</td><td><input ".$bloq."type=\"text\" name=\"entdatanasc\" id=\"entdatanasc\" size=\"12\" maxlength=\"10\" value=\"".(($this->entdatanasc && $pc = explode("-",$this->entdatanasc))?$pc[2]."/".$pc[1]."/".$pc[0]:"")."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##/##/####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entnumdddresidencial\"><td class=\"SubTituloDireita\">Telefone residencial :</td><td>( <input ".$bloq."type=\"text\" name=\"entnumdddresidencial\" id=\"entnumdddresidencial\" size=\"3\" maxlength=\"2\" value=\"".$this->entnumdddresidencial."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> ) <input ".$bloq."type=\"text\" name=\"entnumresidencial\" id=\"entnumresidencial\" size=\"11\" maxlength=\"9\" value=\"".$this->entnumresidencial."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('####-####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entnumdddcomercial\"><td class=\"SubTituloDireita\">Telefone comercial :</td><td>( <input ".$bloq."type=\"text\" name=\"entnumdddcomercial\" id=\"entnumdddcomercial\" size=\"3\" maxlength=\"2\" value=\"".$this->entnumdddcomercial."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> ) <input ".$bloq."type=\"text\" name=\"entnumcomercial\" id=\"entnumcomercial\" size=\"11\" maxlength=\"9\" value=\"".$this->entnumcomercial."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('####-####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> <strong>Ramal :</strong> <input ".$bloq."type=\"text\" name=\"entnumramalcomercial\" id=\"entnumramalcomercial\" size=\"5\" maxlength=\"4\" value=\"".$this->entnumramalcomercial."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##########',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entnumdddfax\"><td class=\"SubTituloDireita\">Fax :</td><td>( <input ".$bloq."type=\"text\" name=\"entnumdddfax\" id=\"entnumdddfax\" size=\"3\" maxlength=\"2\" value=\"".$this->entnumdddfax."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> ) <input ".$bloq."type=\"text\" name=\"entnumfax\" id=\"entnumfax\" size=\"11\" maxlength=\"9\" value=\"".$this->entnumfax."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('####-####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> <strong>Ramal :</strong> <input ".$bloq."type=\"text\" name=\"entnumramalfax\" id=\"entnumramalfax\" size=\"5\" maxlength=\"4\" value=\"".$this->entnumramalfax."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entnumdddcelular\"><td class=\"SubTituloDireita\">Celular :</td><td>( <input ".$bloq."type=\"text\" name=\"entnumdddcelular\" id=\"entnumdddcelular\" size=\"3\" maxlength=\"2\" value=\"".$this->entnumdddcelular."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> ) <input ".$bloq."type=\"text\" name=\"entnumcelular\" id=\"entnumcelular\" size=\"11\" maxlength=\"10\" value=\"".$this->entnumcelular."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('####-####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entobs\"><td class=\"SubTituloDireita\">Observação :</td><td><textarea  ".$bloq."id=\"entobs\" name=\"entobs\" cols=\"45\" rows=\"5\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);textCounter(this.form.entobs,this.form.no_entobs,500);\"  style=\"width:45 ex;\"  onkeydown=\"textCounter(this.form.entobs,this.form.no_entobs,500);\" onkeyup=\"textCounter(this.form.entobs,this.form.no_entobs,500);\">".$this->entobs."</textarea><br><input ".$bloq."readonly style=\"text-align:right;border-left:#888888 3px solid;color:#808080;\" type=\"text\" name=\"no_entobs\" size=\"6\" maxlength=\"6\" value=\"500\" class=\"CampoEstilo\"><font color=\"red\" size=\"1\" face=\"Verdana\"> máximo de caracteres</font></div><div id=\"print_entobs\" class=\"notscreen\" style=\"text-align: left;\"></div></td></tr>
						  <tr id=\"tr_funid\"><td class=\"SubTituloDireita\">Funções cadastradas:</td><td><select ".$bloq."size=\"5\" style=\"width:450px\" id=\"funcoescadastradas\">".$optfuncoescadastradas."</select></td></tr>";
				
				break;
			case 'J':
				require APPRAIZ . "www/includes/webservice/pj.php";
				$dados = $this->carregarCombosFormulario();
				
				$entescolanovaCheck = ($this->entescolanova=='t' ? 'checked="checked"' : "");				
				
				if($carregarcomplementos['novaEscola']) {
					$html .= "<tr id=\"tr_entescolanova\"><td class=\"SubTituloDireita\">Nova escola (não possui código INEP) :</td><td><input ".$bloq."type=\"checkbox\" onclick=\"gerarEntcodent( this.checked );\" name=\"entescolanova\" id=\"entescolanova\" value=\"".$this->entescolanova."\" ".$entescolanovaCheck."  />			
					</td></tr>";
				}
				
				$html .= "<tr id=\"tr_entnumcpfcnpj\"><td class=\"SubTituloDireita\">CNPJ :</td><td><input ".$bloq."type=\"text\" name=\"entnumcpfcnpj\" id=\"entnumcpfcnpj\" size=\"22\" maxlength=\"18\" value=\"".$this->mascaraglobal($this->entnumcpfcnpj,"##.###.###/####-##")."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##.###.###/####-##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);getEntidadePeloCNPJ(this.value);\"></td></tr>
						  <tr id=\"tr_entcodent\"><td class=\"SubTituloDireita\">Código da escola (INEP) :</td><td><input ".$bloq."type=\"text\" name=\"entcodent\" id=\"entcodent\" size=\"12\" maxlength=\"10\" value=\"".$this->entcodent."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);getEntidadePeloEntcodent(this.value);\"></td></tr>
						  <tr id=\"tr_entnuninsest\"><td class=\"SubTituloDireita\">Inscrição Estadual :</td><td><input ".$bloq."type=\"text\" name=\"entnuninsest\" id=\"entnuninsest\" size=\"16\" maxlength=\"14\" value=\"".$this->entnuninsest."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entunicod\"><td class=\"SubTituloDireita\">Código da unidade :</td><td><input ".$bloq."type=\"text\" name=\"entunicod\" id=\"entunicod\" size=\"6\" maxlength=\"5\" value=\"".$this->entunicod."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);getEntidadePeloUnicod(this.value);\"></td></tr>
						  <tr id=\"tr_entungcod\"><td class=\"SubTituloDireita\">Código da unidade gestora :</td><td><input ".$bloq."type=\"text\" name=\"entungcod\" id=\"entungcod\" size=\"16\" maxlength=\"14\" value=\"".$this->entungcod."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entnome\"><td class=\"SubTituloDireita\">Nome :</td><td><input ".$bloq."type=\"text\" name=\"entnome\" id=\"entnome\" size=\"90\" maxlength=\"255\" value=\"".$this->entnome."\" class=\"disabled\" onfocus=\"\" onmouseout=\"\" onblur=\"\" readonly=\"readonly\"></td></tr>
						  <tr id=\"tr_entrazaosocial\"><td class=\"SubTituloDireita\">Razão Social :</td><td><input ".$bloq."type=\"text\" name=\"entrazaosocial\" id=\"entrazaosocial\" size=\"90\" maxlength=\"255\" value=\"".$this->entrazaosocial."\" class=\"disabled\" onfocus=\"\" onmouseout=\"\" onblur=\"\" readonly=\"readonly\">".(($this->entnumcpfcnpj)?"<script>getRazaoSocialReceitaPeloCNPJ('".$this->entnumcpfcnpj."');</script>":"")."</td></tr>
						  <tr id=\"tr_entemail\"><td class=\"SubTituloDireita\">Email :</td><td><input ".$bloq."type=\"text\" name=\"entemail\" id=\"entemail\" size=\"45\" maxlength=\"100\" value=\"".$this->entemail."\" class=\"{$bloqclass}\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entsig\"><td class=\"SubTituloDireita\">Sigla :</td><td><input ".$bloq."type=\"text\" name=\"entsig\" id=\"entsig\" size=\"45\" maxlength=\"50\" value=\"".$this->entsig."\" class=\"normal\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entnumdddcomercial\"><td class=\"SubTituloDireita\">Telefone comercial :</td><td>( <input ".$bloq."type=\"text\" name=\"entnumdddcomercial\" id=\"entnumdddcomercial\" size=\"3\" maxlength=\"2\" value=\"".$this->entnumdddcomercial."\" class=\"{$bloqclass}\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> ) <input ".$bloq."type=\"text\" name=\"entnumcomercial\" id=\"entnumcomercial\" size=\"11\" maxlength=\"9\" value=\"".$this->entnumcomercial."\" class=\"{$bloqclass}\" onKeyUp=\"this.value=mascaraglobal('####-####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> <strong>Ramal :</strong> <input ".$bloq."type=\"text\" name=\"entnumramalcomercial\" id=\"entnumramalcomercial\" size=\"5\" maxlength=\"4\" value=\"".$this->entnumramalcomercial."\" class=\"{$bloqclass}\" onKeyUp=\"this.value=mascaraglobal('##########',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_entnumdddfax\"><td class=\"SubTituloDireita\">Fax :</td><td>( <input ".$bloq."type=\"text\" name=\"entnumdddfax\" id=\"entnumdddfax\" size=\"3\" maxlength=\"2\" value=\"".$this->entnumdddfax."\" class=\"{$bloqclass}\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> ) <input ".$bloq."type=\"text\" name=\"entnumfax\" id=\"entnumfax\" size=\"11\" maxlength=\"9\" value=\"".$this->entnumfax."\" class=\"{$bloqclass}\" onKeyUp=\"this.value=mascaraglobal('####-####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> <strong>Ramal :</strong> <input ".$bloq."type=\"text\" name=\"entnumramalfax\" id=\"entnumramalfax\" size=\"5\" maxlength=\"4\" value=\"".$this->entnumramalfax."\" class=\"{$bloqclass}\" onKeyUp=\"this.value=mascaraglobal('####',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
						  <tr id=\"tr_njuid\"><td class=\"SubTituloDireita\">Natureza Jurídica :</td><td><select ".$bloq."name=\"njuid\" class=\"CampoEstilo\" style=\"width: auto\" id=\"njuid\"><option value=\"\">Selecione</option>".(($dados['opcnaturezajuridica'])?implode("",$dados['opcnaturezajuridica']):"")."</select></td></tr>
						  <tr id=\"tr_tpctgid\"><td class=\"SubTituloDireita\">Categoria da escola privada :</td><td><select ".$bloq."name=\"tpctgid\" class=\"CampoEstilo\" style=\"width: auto\" id=\"tpctgid\"><option value=\"\">Selecione</option>".(($dados['opctipocatescolaprivada'])?implode("",$dados['opctipocatescolaprivada']):"")."</select></td></tr>
						  <tr id=\"tr_tpcid\"><td class=\"SubTituloDireita\">Classificação :</td><td><select ".$bloq."name=\"tpcid\" class=\"CampoEstilo\" style=\"width: auto\" id=\"tpcid\"><option value=\"\">Selecione</option>".(($dados['opctipoclassificacao'])?implode("",$dados['opctipoclassificacao']):"")."</select></td></tr>
						  <tr id=\"tr_tplid\"><td class=\"SubTituloDireita\">Localização :</td><td><select ".$bloq."name=\"tplid\" class=\"CampoEstilo\" style=\"width: auto\" id=\"tplid\"><option value=\"\">Selecione</option>".(($dados['opctipolocalizacao'])?implode("",$dados['opctipolocalizacao']):"")."</select></td></tr>
						  <tr id=\"tr_tpsid\"><td class=\"SubTituloDireita\">Situação :</td><td><select ".$bloq."name=\"tpsid\" class=\"CampoEstilo\" style=\"width: auto\" id=\"tpsid\"><option value=\"\">Selecione</option>".(($dados['opctiposituacao'])?implode("",$dados['opctiposituacao']):"")."</select></td></tr>
						  <tr id=\"tr_entobs\"><td class=\"SubTituloDireita\">Observação :</td><td><textarea  ".$bloq." id=\"entobs\" name=\"entobs\" cols=\"45\" rows=\"5\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);textCounter(this.form.entobs,this.form.no_entobs,500);\"  style=\"width:45 ex;\"  onkeydown=\"textCounter(this.form.entobs,this.form.no_entobs,500);\" onkeyup=\"textCounter(this.form.entobs,this.form.no_entobs,500);\">".$this->entobs."</textarea><br><input ".$bloq."readonly style=\"text-align:right;border-left:#888888 3px solid;color:#808080;\" type=\"text\" name=\"no_entobs\" size=\"6\" maxlength=\"6\" value=\"500\" class=\"CampoEstilo\"><font color=\"red\" size=\"1\" face=\"Verdana\"> máximo de caracteres</font></div><div id=\"print_entobs\" class=\"notscreen\" style=\"text-align: left;\"></div></td></tr>
						  <tr id=\"tr_funcoescadastradas\"><td class=\"SubTituloDireita\">Funções cadastradas:</td><td><select ".$bloq."size=\"5\" style=\"width:450px\" id=\"funcoescadastradas\">".$optfuncoescadastradas."</select></td></tr>";
				break;
		}
		if($carregarcomplementos['enderecos']) {
			foreach($carregarcomplementos['enderecos'] as $tendereco) {
				// verifica se existe o tipo de endereço
				if($this->_listatipoendereco[$tendereco]) {
					$latitude = explode(".",$this->enderecos[$tendereco]['medlatitude']);
					$longitude = explode(".",$this->enderecos[$tendereco]['medlongitude']);
					$html .= "<tr id=\"tr_endereco\"><td class=\"SubTituloCentro\" colspan=\"2\">".strtoupper($this->_listatipoendereco[$tendereco]['tpedsc'])."<input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][endid]\" id=\"endid".$this->_listatipoendereco[$tendereco]['tpeid']."\" value=\"".$this->enderecos[$tendereco]['endid']."\"></td></tr>
							  <tr id=\"tr_endcep\"><td class=\"SubTituloDireita\">CEP :</td><td><input ".$bloq."type=\"text\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][endcep]\" id=\"endcep".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"11\" maxlength=\"9\" value=\"".$this->mascaraglobal($this->enderecos[$tendereco]['endcep'],"#####-###")."\" class=\"{$bloqclass}\" onKeyUp=\"this.value=mascaraglobal('#####-###',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);getEnderecoPeloCEP(this.value,'".$this->_listatipoendereco[$tendereco]['tpeid']."');\"></td></tr>
							  <tr id=\"tr_endlog\"><td class=\"SubTituloDireita\">Logradouro :</td><td><input ".$bloq."type=\"text\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][endlog]\" id=\"endlog".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"80\" maxlength=\"200\" value=\"".$this->enderecos[$tendereco]['endlog']."\" class=\"{$bloqclass}\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
							  <tr id=\"tr_endcom\"><td class=\"SubTituloDireita\">Complemento :</td><td><input ".$bloq."type=\"text\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][endcom]\" id=\"endcom".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"80\" maxlength=\"200\" value=\"".$this->enderecos[$tendereco]['endcom']."\" class=\"{$bloqclass}\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
							  <tr id=\"tr_endnum\"><td class=\"SubTituloDireita\">Número :</td><td><input ".$bloq."type=\"text\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][endnum]\" id=\"endnum".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"11\" maxlength=\"10\" value=\"".$this->enderecos[$tendereco]['endnum']."\" class=\"{$bloqclass}\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
							  <tr id=\"tr_endbai\"><td class=\"SubTituloDireita\">Bairro :</td><td><input ".$bloq."type=\"text\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][endbai]\" id=\"endbai".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"80\" maxlength=\"100\" value=\"".$this->enderecos[$tendereco]['endbai']."\" class=\"{$bloqclass}\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"></td></tr>
							  <tr id=\"tr_estuf\"><td class=\"SubTituloDireita\">UF :</td><td><input ".$bloq."type=\"text\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][estuf]\" id=\"estuf".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"3\" maxlength=\"2\" value=\"".$this->enderecos[$tendereco]['estuf']."\" class=\"disabled\" readonly=\"readonly\"></td></tr>
							  <tr id=\"tr_mundescricao\"><td class=\"SubTituloDireita\">Munícipio :</td><td><input ".$bloq."type=\"text\" id=\"mundescricao".$this->_listatipoendereco[$tendereco]['tpeid']."\" name=\"mundescricao".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"30\" value=\"".$this->enderecos[$tendereco]['mundescricao']."\" class=\"disabled\" readonly=\"readonly\"><input ".$bloq."type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][muncod]\" id=\"muncod".$this->_listatipoendereco[$tendereco]['tpeid']."\" value=\"".$this->enderecos[$tendereco]['muncod']."\"></td></tr>
							  <tr id=\"tr_latitude\"><td class=\"SubTituloDireita\">Latitude :</td>
							  	  <td><span id=\"_graulatitude".$this->_listatipoendereco[$tendereco]['tpeid']."\">".((trim($latitude[0]))?trim($latitude[0]):"XX")."</span>º 
							  	  	  <span id=\"_minlatitude".$this->_listatipoendereco[$tendereco]['tpeid']."\">". ((trim($latitude[1]))?trim($latitude[1]):"XX")."</span>' 
							  	  	  <span id=\"_seglatitude".$this->_listatipoendereco[$tendereco]['tpeid']."\">". ((trim($latitude[2]))?trim($latitude[2]):"XX")."</span>\" 
							  	  	  <span id=\"_pololatitude".$this->_listatipoendereco[$tendereco]['tpeid']."\">".((trim($latitude[3]))?trim($latitude[3]):"XX")."</span>
							  	  	  <input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][medlatitude][0]\" id=\"graulatitude".$this->_listatipoendereco[$tendereco]['tpeid']."\" maxlength=\"2\" size=\"3\" value=\"".trim($latitude[0])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][medlatitude][1]\" id=\"minlatitude".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"3\" maxlength=\"2\" value=\"".trim($latitude[1])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][medlatitude][2]\" id=\"seglatitude".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"3\" maxlength=\"2\" value=\"".trim($latitude[2])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][medlatitude][3]\" id=\"pololatitude".$this->_listatipoendereco[$tendereco]['tpeid']."\" value=\"".trim($latitude[3])."\"></td></tr>
							  <tr id=\"tr_longitude\"><td class=\"SubTituloDireita\">Longitude :</td>
							  	  <td><span id=\"_graulongitude".$this->_listatipoendereco[$tendereco]['tpeid']."\">".((trim($longitude[0]))?trim($longitude[0]):"XX")."</span>º
							  	  	  <span id=\"_minlongitude".$this->_listatipoendereco[$tendereco]['tpeid']."\">".((trim($longitude[1]))?trim($longitude[1]):"XX")."</span>' 
							  	  	  <span id=\"_seglongitude".$this->_listatipoendereco[$tendereco]['tpeid']."\">".((trim($longitude[2]))?trim($longitude[2]):"XX")."</span>\" 
							  	  	  <span id=\"_pololongitude".$this->_listatipoendereco[$tendereco]['tpeid']."\">".((trim($longitude[3]))?trim($longitude[3]):"XX")."</span>
							  	  	  <input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][medlongitude][0]\" id=\"graulongitude".$this->_listatipoendereco[$tendereco]['tpeid']."\" maxlength=\"2\" size=\"3\" value=\"".trim($longitude[0])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][medlongitude][1]\" id=\"minlongitude".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"3\" maxlength=\"2\" value=\"".trim($longitude[1])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][medlongitude][2]\" id=\"seglongitude".$this->_listatipoendereco[$tendereco]['tpeid']."\" size=\"3\" maxlength=\"2\" value=\"".trim($longitude[2])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][medlongitude][3]\" id=\"pololongitude".$this->_listatipoendereco[$tendereco]['tpeid']."\" value=\"".trim($longitude[3])."\"></td></tr>
							  <tr id=\"tr_endmapa\"><td class=\"SubTituloDireita\">&nbsp;</td><td><a href=\"#\" onclick=\"abreMapaEntidade('".$this->_listatipoendereco[$tendereco]['tpeid']."');\">Visualizar / Buscar No Mapa</a> <input ".$bloq."style=\"display:none;\" type=\"text\" name=\"endereco[".$this->_listatipoendereco[$tendereco]['tpeid']."][endzoom]\" id=\"endzoom".$this->_listatipoendereco[$tendereco]['tpeid']."\" value=\"".$this->enderecos[$tendereco]['endzoom']."\"></td></tr>";
				}
			}
		}
		
		if($carregarcomplementos['fotos']) {
			$html .= "<tr><td class=\"SubTituloCentro\" colspan=\"2\">FOTOS</td></tr>
					  <tr><td style=\"text-align:center;\" colspan=\"2\">
					  <div id=\"thumbnails\">";
			
			$fotos = $this->carregarFotosEntidades();
			
			if(is_array($fotos)){
				$_SESSION['downloadfiles']['pasta'] = array("origem" => "entidades","destino" => "entidades");
				$_SESSION['imgparams'] = array("filtro" => "cnt.entid =".$this->entid, "tabela" => "entidade.fotoentidade");					
				for($k=1;$k<11;$k++){
					$html .= "<div class='imageBox' id='imageBox{$k}' >";
					if($fotos[$k]["arqid"]) {
						$html .= "<img id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?arqid=". $fotos[$k]["arqid"] ."&_sisarquivo=entidades' style='margin: 0px;opacity: 1' class='imageBox_theImage' title='". $fotos[$k]["arqdescricao"] ."' onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $pagina ."&arqid=". $fotos[$k]["arqid"] ."&_sisarquivo=entidades\",\"imagem\",\"width=850,height=600,resizable=yes\")'/>\n";
						$html .= "<input type='hidden' value='".$fotos[$k]["arqid"]."' id='".$fotos[$k]["fotbox"]."_".$fotos[$k]["arqid"]."' name='".$fotos[$k]["fotbox"]."' /><br />\n";
					}							
					$html .= "</div>";
				}
			} else {
				
				for($k=1;$k<11;$k++) {
					$html .= "<div class='imageBox' id='imageBox{$k}' ></div>";
				}
								
			}
	  
			$html .= "</div></td></tr>
					  <tr><td class=\"SubTituloEsquerda\" colspan=\"2\"><a style=\"cursor:pointer;\" onclick=\"carregarFotosEntidades('".(($this->entid)?"?funcao=AtualizaFotos&entid=".$this->entid:"")."');\"><img src=\"../imagens/gif_inclui.gif\" align=\"absmiddle\" border=\"0\"> Inserir Fotos</a></td></tr>";
		}
		
		if($carregarcomplementos["painel"]['vinculo_acoes']){
			$html .= $carregarcomplementos["painel"]['vinculo_acoes'];
		}
		
		$html .= "<tr id=\"tr_acoes\"><td class=\"SubTituloCentro\" colspan=\"2\">";
		
		if($dadosfuncao && $hide != 'hide' ){
			//$html .= "<input type='button' value='Gravar' id='btngravar' onclick='enviaForm()'>";
			$html .= "<input type='submit' id='btngravar' value=\"Gravar\" >";
		}
		
		/*if($_REQUEST['modulo'] == "meprincipal/dados_escola"){
			
			$imagemPst = "<div style=\"position:absolute; left:900px; top:270px;\"><a href=\"http://portal.esporte.gov.br/snee/segundotempo/maiseducacao/\" target=\"_blank\" alt=\"Clique aqui e saiba mais...\" title=\"Clique aqui e saiba mais...\"><img src=\"../imagens/Imagem_PST.jpg\" border=\"0\"></a></div>";
			
		} else {
			
			$imagemPst = "";
				 			
		}*/
		
		if($carregarcomplementos["painel"]['secretaria']){
			$html .= "<input type='button' value='Cancelar' id='btncancelar' onclick='history.back(-1);'>";
		}else{
			$html .= "<input type='button' value='Cancelar' id='btncancelar' onclick='window.close();'>";
		}
		
		$html .= "
		</td></tr>
				  </table>
				  {$imagemPst}
				  </form>
				  <script type=\"text/javascript\">
					messageObj = new DHTML_modalMessage();	// We only create one object of this class
					messageObj.setShadowOffset(5);	// Large shadow
					function displayMessage(url) {
						messageObj.setSource(url);
						messageObj.setCssClassMessageBox(false);
						messageObj.setSize(500,300);
						messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
						messageObj.display();
					}
					function displayStaticMessage(messageContent,cssClass) {
						messageObj.setHtmlContent(messageContent);
						messageObj.setSize(650,300);
						messageObj.setCssClassMessageBox(cssClass);
						messageObj.setSource(false);	// no html source since we want to use a static message here.
						messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
						messageObj.display();
					}
					function closeMessage() {
						messageObj.close();	
					}					
				</script>";
		
		return $html;
	}
	
	function carregarFotosEntidades() {
		if($this->entid) {
			
			$sql = "SELECT arq.arqid, arq.arqdescricao FROM entidade.fotoentidade fot
					LEFT JOIN public.arquivo arq ON arq.arqid = fot.arqid 
					WHERE entid='".$this->entid."' 
					ORDER BY fotordem";
			
			$fotos = $this->db->carregar($sql);
			
			if($fotos[0]) {
				$i=1;
				foreach($fotos as $fot) {
					$fotosp[$i] = $fot;
					$i++;
				}
			}
			return $fotosp;
			
		} else {
			return false;
		}
	}
	
	
	/**
	 * Função que valida a existencia de entidades baseados no nome e cpf/cnpj
	 * Caso tenha alguma entidade com nome e cpf/cnpj, o componente devera atualizar
	 * 
	 * @author Alexandre Dourado
	 * @version v1.0 24/09/2010
	 */
	function validarExistenciaEntidade() {
		
		$sql = "SELECT * FROM entidade.entidade WHERE entnome ilike '".$this->entnome."' AND entnumcpfcnpj='".$this->entnumcpfcnpj."' ORDER BY entdatainclusao DESC";
		$dentidade = $this->db->pegaLinha($sql);
		
		if($dentidade['entid']) {
			
			$this->entid 			 	= $dentidade['entid'];
			$this->njuid 			 	= (($this->njuid==$dentidade['njuid'])?$this->njuid:$dentidade['njuid']);
			$this->entnumcpfcnpj 	 	= (($this->entnumcpfcnpj==$dentidade['entnumcpfcnpj'])?$this->entnumcpfcnpj:$dentidade['entnumcpfcnpj']);
			$this->entnome 			 	= (($this->entnome==$dentidade['entnome'])?$this->entnome:$dentidade['entnome']);
			$this->entemail 		 	= (($this->entemail==$dentidade['entemail'])?$this->entemail:$dentidade['entemail']);
			$this->entnuninsest 	 	= (($this->entnuninsest==$dentidade['entnuninsest'])?$this->entnuninsest:$dentidade['entnuninsest']);
			$this->entobs 			 	= (($this->entobs==$dentidade['entobs'])?$this->entobs:$dentidade['entobs']);
			$this->entstatus 		 	= (($this->entstatus==$dentidade['entstatus'])?$this->entstatus:$dentidade['entstatus']);
			$this->entnumrg 		 	= (($this->entnumrg==$dentidade['entnumrg'])?$this->entnumrg:$dentidade['entnumrg']);
			$this->entorgaoexpedidor 	= (($this->entorgaoexpedidor==$dentidade['entorgaoexpedidor'])?$this->entorgaoexpedidor:$dentidade['entorgaoexpedidor']);
			$this->entsexo 			 	= (($this->entsexo==$dentidade['entsexo'])?$this->entsexo:$dentidade['entsexo']);$dentidade[''];
			$this->entdatanasc 		 	= (($this->entdatanasc==$dentidade['entdatanasc'])?$this->entdatanasc:$dentidade['entdatanasc']);$entidade[''];
			$this->entdatainiass 	 	= (($this->entdatainiass==$dentidade['entdatainiass'])?$this->entdatainiass:$dentidade['entdatainiass']);
			$this->entdatafimass 	 	= (($this->entdatafimass==$dentidade['entdatafimass'])?$this->entdatafimass:$dentidade['entdatafimass']);
			$this->entnumdddresidencial = (($this->entnumdddresidencial==$dentidade['entnumdddresidencial'])?$this->entnumdddresidencial:$dentidade['entnumdddresidencial']);
			$this->entnumresidencial 	= (($this->entnumresidencial==$dentidade['entnumresidencial'])?$this->entnumresidencial:$dentidade['entnumresidencial']);
			$this->entnumdddcomercial 	= (($this->entnumdddcomercial==$dentidade['entnumdddcomercial'])?$this->entnumdddcomercial:$dentidade['entnumdddcomercial']);
			$this->entnumramalcomercial = (($this->entnumramalcomercial==$dentidade['entnumramalcomercial'])?$this->entnumramalcomercial:$dentidade['entnumramalcomercial']);
			$this->entnumcomercial 		= (($this->entnumcomercial==$dentidade['entnumcomercial'])?$this->entnumcomercial:$dentidade['entnumcomercial']);
			$this->entnumdddfax 		= (($this->entnumdddfax==$dentidade['entnumdddfax'])?$this->entnumdddfax:$dentidade['entnumdddfax']);
			$this->entnumramalfax 		= (($this->entnumramalfax==$dentidade['entnumramalfax'])?$this->entnumramalfax:$dentidade['entnumramalfax']);
			$this->entnumfax 			= (($this->entnumfax==$dentidade['entnumfax'])?$this->entnumfax:$dentidade['entnumfax']);
			$this->tpctgid 				= (($this->tpctgid==$dentidade['tpctgid'])?$this->tpctgid:$dentidade['tpctgid']);
			$this->tpcid 				= (($this->tpcid==$dentidade['tpcid'])?$this->tpcid:$dentidade['tpcid']);
			$this->tplid 				= (($this->tplid==$dentidade['tplid'])?$this->tplid:$dentidade['tplid']);
			$this->tpsid 				= (($this->tpsid==$dentidade['tpsid'])?$this->tpsid:$dentidade['tpsid']);
			$this->entcodentsup 		= (($this->entcodentsup==$dentidade['entcodentsup'])?$this->entcodentsup:$dentidade['entcodentsup']);
			$this->entcodent 			= (($this->entcodent==$dentidade['entcodent'])?$this->entcodent:$dentidade['entcodent']);
			$this->entescolanova 		= (($this->entescolanova==$dentidade['entescolanova'])?$this->entescolanova:$dentidade['entescolanova']);
			$this->entdatainclusao 		= (($this->entdatainclusao==$dentidade['entdatainclusao'])?$this->entdatainclusao:$dentidade['entdatainclusao']);
			$this->entsig 				= (($this->entsig==$dentidade['entsig'])?$this->entsig:$dentidade['entsig']);
			$this->entunicod 			= (($this->entunicod==$dentidade['entunicod'])?$this->entunicod:$dentidade['entunicod']);
			$this->entungcod 			= (($this->entungcod==$dentidade['entungcod'])?$this->entungcod:$dentidade['entungcod']);
			$this->entproep 			= (($this->entproep==$dentidade['entproep'])?$this->entproep:$dentidade['entproep']);
			$this->entnumdddcelular 	= (($this->entnumdddcelular==$dentidade['entnumdddcelular'])?$this->entnumdddcelular:$dentidade['entnumdddcelular']);
			$this->entnumcelular 		= (($this->entnumcelular==$dentidade['entnumcelular'])?$this->entnumcelular:$dentidade['entnumcelular']);
			$this->entorgcod 			= (($this->entorgcod==$dentidade['entorgcod'])?$this->entorgcod:$dentidade['entorgcod']);
			$this->entsede 				= (($this->entsede==$dentidade['entsede'])?$this->entsede:$dentidade['entsede']);
			$this->entrazaosocial		= (($this->entrazaosocial==$dentidade['entrazaosocial'])?$this->entrazaosocial:$dentidade['entrazaosocial']);
			
			$this->funcoes 				= $this->carregarFuncoesEntidade();
			$this->enderecos			= ((!$this->enderecos)?$this->carregarEnderecosEntidade():$this->enderecos);
		}
	}
	
	function salvar() {
		$this->ajustarDadosEntidade();
		if($this->entid) {
			$this->atualizarEntidade();
			$this->salvarEndereco();
			$this->salvarFuncoes();
			$this->db->commit();
		} else {
			// validação para verificar se existe alguma entidade com nome e cpf/cnpj, caso exista, o componente
			// não devera inserir nova entidade, e sim, atualiza-la
			$this->validarExistenciaEntidade();
			if($this->entid) {
				$this->atualizarEntidade();
				$this->salvarEndereco();
				$this->salvarFuncoes();
			} else {
				$this->inserirEntidade();
				$this->salvarEndereco();
				$this->salvarFuncoes();
			}
			$this->db->commit();
		}
	}
	
	function salvarEndereco() {
		if($this->enderecos) {
			foreach($this->enderecos as $tpeid => $endereco) {
				
				// resolvendo problema não identificado no javascript
				if($endereco['medlatitude'][3] == "undefined") $endereco['medlatitude'][3] = "S";
				if($endereco['medlongitude'][3] == "undefined") $endereco['medlongitude'][3] = "W";
				
				if($endereco['medlatitude'][0] === "" || 
				   $endereco['medlatitude'][0] === "undefined" ||
				   $endereco['medlatitude'][1] === "" || 
				   $endereco['medlatitude'][1] === "undefined" ||
				   $endereco['medlatitude'][2] === "" || 
				   $endereco['medlatitude'][2] === "undefined" ||
				   $endereco['medlatitude'][3] === "") {
				   	
				   $endereco['medlatitude'] = false;	
				   	
				}
				
				if($endereco['medlongitude'][0] === "" || 
				   $endereco['medlongitude'][0] === "undefined" ||
				   $endereco['medlongitude'][1] === "" || 
				   $endereco['medlongitude'][1] === "undefined" ||
				   $endereco['medlongitude'][2] === "" || 
				   $endereco['medlongitude'][2] === "undefined" ||
				   $endereco['medlongitude'][3] === "") {
				   	
				   $endereco['medlongitude'] = false;	
				   	
				}
				
				if($endereco['endid']) {
					
					$this->atualizarEndereco($endereco);
					// código para coletar dados sobre mudanças na tabela de entidade
					$sql = "INSERT INTO entidade.historico(
			            	entid, endid, sisid, usucpf, hstdata, hsturl, hstacao)
			    			VALUES ('".$this->entid."', '".$endereco['endid']."', '".$_SESSION['sisid']."', '".$_SESSION['usucpf']."', NOW(), '".$_SERVER['REQUEST_URI']."', 'U');";
					$this->db->executar($sql);
					
				} else {
					$endereco['tpeid'] = $tpeid;
					
					// verificando realmente nao existe
					$sql = "SELECT endid FROM entidade.endereco WHERE entid='".$this->entid."' AND tpeid='".$tpeid."'";
					$end = $this->db->pegaUm($sql);
					
					if($end) {
						$endereco['endid'] = $end;
						$this->atualizarEndereco($endereco);
						// código para coletar dados sobre mudanças na tabela de entidade
						$sql = "INSERT INTO entidade.historico(
				            	entid, endid, sisid, usucpf, hstdata, hsturl, hstacao)
				    			VALUES ('".$this->entid."', '".$endereco['endid']."', '".$_SESSION['sisid']."', '".$_SESSION['usucpf']."', NOW(), '".$_SERVER['REQUEST_URI']."', 'U');";
						$this->db->executar($sql);
					} else {
						$this->enderecos[$tpeid]['endid'] = $this->inserirEndereco($endereco);
						// código para coletar dados sobre mudanças na tabela de entidade
						$sql = "INSERT INTO entidade.historico(
				            	entid, endid, sisid, usucpf, hstdata, hsturl, hstacao)
				    			VALUES ('".$this->entid."', '".$this->enderecos[$tpeid]['endid']."', '".$_SESSION['sisid']."', '".$_SESSION['usucpf']."', NOW(), '".$_SERVER['REQUEST_URI']."', 'I');";
						$this->db->executar($sql);
					}
					
				}
			}
		}
	}
	
	function salvarFuncoes() {
		if($this->funcoesadicionar) {
			foreach($this->funcoesadicionar as $funcao) {
				$f = $this->db->pegaLinha("SELECT fun.funcumulativo, fen.fueid ".(($funcao['entidassociado'])?", fea.feaid":"")." FROM entidade.funcao fun 
										   LEFT JOIN entidade.funcaoentidade fen ON fun.funid = fen.funid AND fen.entid='".$this->entid."'   
			 						   	   ".(($funcao['entidassociado'])?"LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid AND fea.entid='".$funcao['entidassociado']."'":"")." 
				 						   WHERE fun.funid='".$funcao['funid']."'");
				if($f['funcumulativo']=="f")
					$this->db->executar("DELETE FROM entidade.funentassoc fea WHERE ".($f['feaid']?"fea.feaid!='".$f['feaid']."' AND":"")." fea.feaid IN( SELECT fea2.feaid FROM entidade.funcaoentidade fen LEFT JOIN entidade.funentassoc fea2 ON fen.fueid = fea2.fueid WHERE fen.funid='".$funcao['funid']."' ".(($funcao['entidassociado'])?"AND fea2.entid='".$funcao['entidassociado']."'":"").")");
				
				if(!$f['fueid']) {
					$fueid = $this->db->pegaUm("INSERT INTO entidade.funcaoentidade(funid, entid, fuedata, fuestatus)
	    							  			VALUES ('".$funcao['funid']."', '".$this->entid."', NOW(), 'A') RETURNING fueid;");
					if($funcao['entidassociado'])
						$this->db->executar("INSERT INTO entidade.funentassoc(entid, fueid, feadata) VALUES ('".$funcao['entidassociado']."', '".$fueid."', NOW());");
    			} else {
					if($funcao['entidassociado'] && !$f['feaid'])
						$this->db->executar("INSERT INTO entidade.funentassoc(entid, fueid, feadata) VALUES ('".$funcao['entidassociado']."', '".$f['fueid']."', NOW());");
    			}
			}
		}
	}
	
	
	function ajustarDadosEntidade() {
		$this->entnumcpfcnpj = str_replace(array(".","-","/"),"",$this->entnumcpfcnpj);
		if($this->entdatanasc) {
			$data = explode("/", $this->entdatanasc);
			$this->entdatanasc = $data[2]."-".$data[1]."-".$data[0];
		}
	}
	
	function atualizarEndereco($dados) {

		$this->db->executar("UPDATE entidade.endereco SET endcep='".str_replace("-","",$dados['endcep'])."', endlog=".(($dados['endlog'])?"'".$dados['endlog']."'":"NULL").", endcom=".(($dados['endcom'])?"'".$dados['endcom']."'":"NULL").", endbai=".(($dados['endbai'])?"'".$dados['endbai']."'":"NULL").", 
						     muncod='".$dados['muncod']."', estuf='".$dados['estuf']."', endnum=".(($dados['endnum'])?"'".$dados['endnum']."'":"NULL").", medlatitude=".((is_array($dados['medlatitude']))?"'".str_replace(" ", "", implode(".",$dados['medlatitude']))."'":"NULL").", medlongitude=".((is_array($dados['medlongitude']))?"'".str_replace(" ", "", implode(".",$dados['medlongitude']))."'":"NULL").", 
						     endcomunidade=".(($dados['endcomunidade'])?"'".$dados['endcomunidade']."'":"NULL").", endzoom=".(($dados['endzoom'])?"'".$dados['endzoom']."'":"NULL")."
						 	 WHERE endid='".$dados['endid']."';");
	
	}
	
	function inserirEndereco($dados) {
		
		return $this->db->pegaUm("INSERT INTO entidade.endereco(
							              entid, tpeid, endcep, endlog, endcom, endbai, muncod, 
							              estuf, endnum, endstatus, medlatitude, medlongitude, endcomunidade, endzoom)
							      VALUES ('".$this->entid."', '".$dados['tpeid']."', '".str_replace("-","",$dados['endcep'])."', ".(($dados['endlog'])?"'".$dados['endlog']."'":"NULL").", 
							      		  ".(($dados['endcom'])?"'".$dados['endcom']."'":"NULL").", ".(($dados['endbai'])?"'".$dados['endbai']."'":"NULL").", ".(($dados['muncod'])?"'".$dados['muncod']."'":"NULL").", ".(($dados['estuf'])?"'".$dados['estuf']."'":"NULL").", ".(($dados['endnum'])?"'".$dados['endnum']."'":"NULL").", 
							      		  'A', ".((is_array($dados['medlatitude']))?"'".str_replace(" ", "", implode(".",$dados['medlatitude']))."'":"NULL").", ".((is_array($dados['medlongitude']))?"'".str_replace(" ", "", implode(".",$dados['medlongitude']))."'":"NULL").", ".(($dados['endcomunidade'])?"'".$dados['endcomunidade']."'":"NULL").", ".(($dados['endzoom'])?"'".$dados['endzoom']."'":"NULL").") RETURNING endid;");
	
	}
	
	function carregarFuncaoEntidadeAssociada($funid, $entidassociado) {
		$this->funcoes[] = array("funid" => $funid, "entidassociado" => $entidassociado);
	}
	
	function inserirEntidade() {

		$this->entid = $this->db->pegaUm("INSERT INTO entidade.entidade(
								            njuid, entnumcpfcnpj, entnome, entemail, entnuninsest, 
								            entobs, entstatus, entnumrg, entorgaoexpedidor, entsexo, entdatanasc, 
								            entdatainiass, entdatafimass, entnumdddresidencial, entnumresidencial, 
								            entnumdddcomercial, entnumramalcomercial, entnumcomercial, entnumdddfax, 
								            entnumramalfax, entnumfax, tpctgid, tpcid, tplid, tpsid, entcodentsup, 
								            entcodent, entescolanova, entdatainclusao, entsig, entunicod, 
								            entungcod, entproep, entnumdddcelular, entnumcelular, entorgcod, 
								            entsede, entrazaosocial)
								    VALUES (".(($this->njuid)?"'".$this->njuid."'":"NULL").", ".(($this->entnumcpfcnpj)?"'".$this->entnumcpfcnpj."'":"NULL").", ".(($this->entnome)?"'".$this->entnome."'":"NULL").", 
								    		".(($this->entemail)?"'".$this->entemail."'":"NULL").", ".(($this->entnuninsest)?"'".$this->entnuninsest."'":"NULL").", ".(($this->entobs)?"'".substr($this->entobs,0,500)."'":"NULL").", 
								    		'A', ".(($this->entnumrg)?"'".$this->entnumrg."'":"NULL").", ".(($this->entorgaoexpedidor)?"'".$this->entorgaoexpedidor."'":"NULL").", ".(($this->entsexo)?"'".$this->entsexo."'":"NULL").", ".(($this->entdatanasc && checkdate(substr($this->entdatanasc,3,2),substr($this->entdatanasc,0,2),substr($this->entdatanasc,6,4)))?"'".$this->entdatanasc."'":"NULL").", 
								            ".(($this->entdatainiass)?"'".$this->entdatainiass."'":"NULL").", ".(($this->entdatafimass)?"'".$this->entdatafimass."'":"NULL").", ".(($this->entnumdddresidencial)?"'".substr($this->entnumdddresidencial,0,3)."'":"NULL").", ".(($this->entnumresidencial)?"'".$this->entnumresidencial."'":"NULL").", 
								            ".(($this->entnumdddcomercial)?"'".substr($this->entnumdddcomercial,0,3)."'":"NULL").", ".(($this->entnumramalcomercial)?"'".$this->entnumramalcomercial."'":"NULL").", ".(($this->entnumcomercial)?"'".$this->entnumcomercial."'":"NULL").", ".(($this->entnumdddfax)?"'".substr($this->entnumdddfax,0,3)."'":"NULL").", 
								            ".(($this->entnumramalfax)?"'".$this->entnumramalfax."'":"NULL").", ".(($this->entnumfax)?"'".$this->entnumfax."'":"NULL").", ".(($this->tpctgid)?"'".$this->tpctgid."'":"NULL").", ".(($this->tpcid)?"'".$this->tpcid."'":"NULL").", ".(($this->tplid)?"'".$this->tplid."'":"NULL").", ".(($this->tpsid)?"'".$this->tpsid."'":"NULL").", ".(($this->entcodentsup)?"'".$this->entcodentsup."'":"NULL").", 
								            ".(($this->entcodent)?"'".$this->entcodent."'":"NULL").", ".(($this->entescolanova)?"'".$this->entescolanova."'":"NULL").", NOW(), ".(($this->entsig)?"'".$this->entsig."'":"NULL").", ".(($this->entunicod)?"'".$this->entunicod."'":"NULL").", 
								            ".(($this->entungcod)?"'".$this->entungcod."'":"NULL").", ".(($this->entproep)?"'".$this->entproep."'":"NULL").", ".(($this->entnumdddcelular)?"'".substr($this->entnumdddcelular,0,3)."'":"NULL").", ".(($this->entnumcelular)?"'".$this->entnumcelular."'":"NULL").", ".(($this->entorgcod)?"'".$this->entorgcod."'":"NULL").", ".(($this->entsede)?"'".$this->entsede."'":"NULL").", ".(($this->entrazaosocial)?"'".$this->entrazaosocial."'":"NULL").") RETURNING entid;");
		
		// código para coletar dados sobre mudanças na tabela de entidade
		$sql = "INSERT INTO entidade.historico(
            	entid, sisid, usucpf, hstdata, hsturl, hstacao)
    			VALUES ('".$this->entid."', '".$_SESSION['sisid']."', '".$_SESSION['usucpf']."', NOW(), '".$_SERVER['REQUEST_URI']."', 'I');";
		$this->db->executar($sql);

	}
	
	function atualizarEntidade() {
		
		// regra    -> Nunca atualizar o nome da entidade
		// 24/08/09 -> Mario, a pedido do cristiano mudou a regra e solicitou para atualizar nome da entidade
		// 03/09/09 -> Não deixar atualizar o entcodent
		// 22/09/09 -> Nova regra criada pelo Cristiano: if !entungcod && !entnumcpfcnpj && !entcodent && !entunicod then 'pode alterar nome' else 'não pode alterar'
		// 25/02/10 -> Nova regra solicitada pelo Mário, permite quando "Super-User" que o usuário altere os dados "entnome".
		// 04/05/10 -> Nova regra, se não houver nome para a entidade permite atualizar "entnome". 

		$arEntidade = $this->db->pegaLinha("SELECT ent.* FROM entidade.entidade ent WHERE entid='".$this->entid."'");
		
		if( (!$this->entungcod && !$this->entnumcpfcnpj && !$this->entcodent && $this->entunicod) || ($this->db->testa_superuser()) )  {
			$entidadenome = "entnome=".(($this->entnome)?"'".$this->entnome."'":"NULL").", ";
		}else if($arEntidade['entnome'] == null){
			$entidadenome = "entnome=".(($this->entnome)?"'".$this->entnome."'":"NULL").", ";
		}
	
		$this->db->executar("UPDATE entidade.entidade SET ".$entidadenome." njuid=".(($this->njuid)?"'".$this->njuid."'":"NULL").", entnumcpfcnpj=".(($this->entnumcpfcnpj)?"'".$this->entnumcpfcnpj."'":"NULL").", entemail=".(($this->entemail)?"'".$this->entemail."'":"NULL").", entnuninsest=".(($this->entnuninsest)?"'".$this->entnuninsest."'":"NULL").", 
					       	 entobs=".(($this->entobs)?"'".$this->entobs."'":"NULL").", entnumrg=".(($this->entnumrg)?"'".$this->entnumrg."'":"NULL").", entorgaoexpedidor=".(($this->entorgaoexpedidor)?"'".$this->entorgaoexpedidor."'":"NULL").", entsexo=".(($this->entsexo)?"'".$this->entsexo."'":"NULL").", entdatanasc=".(($this->entdatanasc)?"'".$this->entdatanasc."'":"NULL").", 
					       	 entdatainiass=".(($this->entdatainiass)?"'".$this->entdatainiass."'":"NULL").", entdatafimass=".(($this->entdatafimass)?"'".$this->entdatafimass."'":"NULL").", entnumdddresidencial=".(($this->entnumdddresidencial)?"'".$this->entnumdddresidencial."'":"NULL").", entnumresidencial=".(($this->entnumresidencial)?"'".$this->entnumresidencial."'":"NULL").", 
					       	 entnumdddcomercial=".(($this->entnumdddcomercial)?"'".$this->entnumdddcomercial."'":"NULL").", entnumramalcomercial=".(($this->entnumramalcomercial)?"'".$this->entnumramalcomercial."'":"NULL").", entnumcomercial=".(($this->entnumcomercial)?"'".$this->entnumcomercial."'":"NULL").", entnumdddfax=".(($this->entnumdddfax)?"'".$this->entnumdddfax."'":"NULL").", 
					       	 entnumramalfax=".(($this->entnumramalfax)?"'".$this->entnumramalfax."'":"NULL").", entnumfax=".(($this->entnumfax)?"'".$this->entnumfax."'":"NULL").", tpctgid=".(($this->tpctgid)?"'".$this->tpctgid."'":"NULL").", tpcid=".(($this->tpcid)?"'".$this->tpcid."'":"NULL").", tplid=".(($this->tplid)?"'".$this->tplid."'":"NULL").", 
					       	 tpsid=".(($this->tpsid)?"'".$this->tpsid."'":"NULL").", entcodentsup=".(($this->entcodentsup)?"'".$this->entcodentsup."'":"NULL").", entescolanova=".(($this->entescolanova)?"'".$this->entescolanova."'":"NULL").", entsig=".(($this->entsig)?"'".$this->entsig."'":"NULL").", entunicod=".(($this->entunicod)?"'".$this->entunicod."'":"NULL").", 
					       	 entungcod=".(($this->entungcod)?"'".substr($this->entungcod,0,6)."'":"NULL").", entproep=".(($this->entproep)?"'".$this->entproep."'":"NULL").", entnumdddcelular=".(($this->entnumdddcelular)?"'".$this->entnumdddcelular."'":"NULL").", entnumcelular=".(($this->entnumcelular)?"'".$this->entnumcelular."'":"NULL").", entorgcod=".(($this->entorgcod)?"'".$this->entorgcod."'":"NULL").", entsede=".(($this->entsede)?"'".$this->entsede."'":"NULL").",
					       	 entrazaosocial=".(($this->entrazaosocial)?"'".$this->entrazaosocial."'":"NULL")."
					       	 WHERE entid='".$this->entid."';");
		
		// código para coletar dados sobre mudanças na tabela de entidade
		$sql = "INSERT INTO entidade.historico(
            	entid, sisid, usucpf, hstdata, hsturl, hstacao)
    			VALUES ('".$this->entid."', '".$_SESSION['sisid']."', '".$_SESSION['usucpf']."', NOW(), '".$_SERVER['REQUEST_URI']."', 'U');";
		$this->db->executar($sql);
		
	}
	
	function removerEntidade() {
		
	}
	
	function mascaraglobal($value, $mask) {
		$casasdec = explode(",", $mask);
		// Se possui casas decimais
		if($casasdec[1])
			$value = sprintf("%01.".strlen($casasdec[1])."f", $value);
	
		$value = str_replace(array("."),array(""),$value);
		if(strlen($mask)>0) {
			$masklen = -1;
			$valuelen = -1;
			while($masklen>=-strlen($mask)) {
				if(substr($mask,$masklen,1) == "#") {
					$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
					$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
				$masklen--;
			}
		}
		return $valueformatado;
	}
	
	function getEntId() {
		return $this->entid;
	}
	

	function getEntnome() {
		return $this->entnome;
	}	
	
	function getEntNumCpfCnpj() {
		return $this->entnumcpfcnpj;
	}
	
	function getEntEmail() {
		return $this->entemail;
	}
	
	function getEntNumDddComercial() {
		return $this->entnumdddcomercial;
	}
	
	function getEntNumComercial() {
		return $this->entnumcomercial;
	}
	
	function getEntNumDddCelular() {
		return $this->entnumdddcelular;
	}
	
	function getEntNumCelular() {
		return $this->entnumcelular;
	}
	
	
	
}
?>