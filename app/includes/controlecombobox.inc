<?
class ControleComboBox {
	
	var $idpadrao;
	var $funcaoJavascript;
	var $filtroSQL = array();
	
	function ControleComboBox($dados = array()) {
		$this->funcaoJavascript = $dados['funcaoJavascript'];
		$this->filtroSQL = $dados['filtroSQL'];
		$this->chamarFuncaoAjax($dados['funcao'],$dados['parametro']);
	}

	function chamarFuncaoAjax($nome,$parametros = array()) {
		if($nome) {
			$this->$nome($parametros);
			exit;
		}
	}

	function carregarCombinacao($cdcidpai) {
		global $db;
		
		$cdcidpai = explode("||", $cdcidpai);
		
		$sql = "SELECT * FROM public.combodadoscombinacao cdc
				LEFT JOIN public.combodadostabela cdt ON cdt.cdtid = cdc.cdcidfilho 
				WHERE cdcidpai = '".$cdcidpai[0]."'".(($cdcidpai[1])?" AND cdcidorigem='".$cdcidpai[1]."'":"")." ORDER BY cdtcod";
		$opcoes = $db->carregar($sql);
		
		if($opcoes[0]) {
			foreach($opcoes as $op) {
				echo $op['cdtid']."||".$op['cdcid']."##".$op['cdtcod']." - ".$op['cdtdsc']."&&";
			}
		}
	}

	function pegarComboHTML($id) {
		$this->idpadrao = $id;
	?>
	<script>
	function limparComboBox(idcombo) {
		for(i=1;i<document.getElementById(idcombo).options.length;i++) {
			document.getElementById(idcombo).remove(i);
		}
	}

	
	function executarChangeCombobox(combo) {
		var combodados = combo.id;
		combodados = combodados.split("_");
		var combonum = parseInt(combodados[1]);
		var combobase = combodados[0];
		var vNum = (combonum+1); 
		while(document.getElementById(combobase+'_'+vNum)) { 
			limparComboBox(combobase+'_'+vNum);
			document.getElementById(combobase+'_'+vNum).disabled = true;
			vNum = vNum+1;
		}
		for(i=1;i<document.getElementById(combobase+'_'+(combonum+1)).options.length;i++) {
			document.getElementById(combobase+'_'+(combonum+1)).remove(i);
		}
		if(combo.value) {
			var evXmlHttp;
			evXmlHttp = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
			evXmlHttp.onreadystatechange = function () {

			if(evXmlHttp.readyState == 4) {
				if(evXmlHttp.status == 200 && evXmlHttp.responseText != '') {
					var x = evXmlHttp.responseText.split("&&");
					for(i=0;i<(x.length-1);i++) {
						var dados = x[i].split("##");
						document.getElementById(combobase+'_'+(combonum+1)).options[i+1] = new Option(dados[1],dados[0]);
					}
				}
				if (evXmlHttp.dispose) {
					evXmlHttp.dispose();
				}
				evXmlHttp = null;
														}
			}
			evXmlHttp.open( 'GET', window.location.href+'&dados[funcao]=carregarCombinacao&dados[parametro]='+combo.value,true);
			evXmlHttp.send( null );
			document.getElementById(combobase+'_'+(combonum+1)).disabled = false;
		}
		<? 
		// incluir uma funcao javascript
		if($this->funcaoJavascript) {
			echo $this->funcaoJavascript."(combo);";
		} 
		?>
			
		
	}
	 </script>
	<?
	echo $this->pegarCombobox($id);
}

function pegarCombobox($id, $i=0) {
	global $db;
	$sql = "SELECT * FROM public.combotabela WHERE ctbid = '".$id."'";
	$combobox = $db->pegaLinha($sql);
	if($combobox) {
		if($i) {
			$sqlopcoes = array();
		} else {
			$sqlopcoes = "SELECT cdtid as codigo, cdtcod||' - '||cdtdsc as descricao FROM public.combodadostabela WHERE ctbid = '".$combobox['ctbid']."' ".(($this->filtroSQL[$id])?" AND ".$this->filtroSQL[$id]:"")." ORDER BY cdtcod";
		}
		$htmlcombo = "<tr><td class='SubTituloDireita'>".$combobox['ctbdsc']."</td><td>".$this->combobox('campo'.$id, $sqlopcoes, (($i)?'N':'S'), 'Selecione', (($combobox['ctbidfilho'])?'executarChangeCombobox':$this->funcaoJavascript), '', '', '', 'N', sprintf("%09s", $this->idpadrao).'_'.$i, true)."</td></tr>";
		if($combobox['ctbidfilho']) {
			$htmlcombo .= $this->pegarcombobox($combobox['ctbidfilho'], ($i+1));		
		}
		return $htmlcombo; 
	}
}

function combobox($var,$sql,$habil,$titulo='',$acao,$opc,$txtdica='',$size='',$obrig='', $id = '', $return = false, $value = null) {
	global ${$var}, $db;
	$value = $value ? $value : ${$var};
	
    if (is_array($sql)) $rescombo = $sql;
    else $rescombo = $db->carregar( $sql );
	
	$select = "";
	if ($txtdica) $select = "<span onmouseover=\"return escape('". $txtdica . "');\">";
	
	$select .= "<select name='" . strtolower( $var ) . "'";
	
	$select .= " class='CampoEstilo' ";
	
	if($size <> '') $select .= " style='width:".$size."px;'";
	else $select .= " style='width: auto'";
	
	if($habil == 'N') $select = $select. ' disabled="disabled" ';
	if (isset($tamanho) && $tamanho > 1) $select .= ' size="' . $tamanho . '" ';
	if ($acao) $select .= ' onchange="' . $acao . '(this)"';
	if ($id) $select .= ' id="' . htmlentities( $id ).  '"';

	$select .= ">";
	
	// cria os options
	if($titulo) $select .= '<option value="">'.$titulo.'</option>';
	if($rescombo) {
		for($i = 0; $i < count( $rescombo ); $i++) {
			if ($value == $rescombo[$i]['codigo'] ) $sel = 'selected="selected"';
			else $sel='';
			$select .= "<option value='".$rescombo[$i]['codigo']."' ".$sel.">".$rescombo[$i]['descricao']."</option>\n";
		}
	}
	
	if($value == 'x') $sel = 'selected="selected"';
	else $sel='';
	
	if($opc) $select .= "<option " . $sel . " value='x'>" . $opc . "</option>\n";

	// finaliza impressão do select
	$select .= '</select>';
	
	if ($txtdica) $select .= "</span>";
	

	if($obrig == 'S') $select .= obrigatorio();

	if ($return) return $select;
	else echo $select;
}
}
	


?>