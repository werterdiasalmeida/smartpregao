<?php

class Montatabela {
	
	private $htm;
	
	public function montaNovaTabela( $perid, $qrpid, $percent = 90 ){
		
		//global $db;
		
		//DADOS DA TABELA
		$obTabela 		= new Tabela( );
		$arrDadosTabela = $obTabela->pegaDados( $perid );
		$tabela = $arrDadosTabela['tblid'];
	
		//RESPOSTAS
		$obTabelaResposta = new Tabelaresposta( );
		$arrResp 		  = $obTabelaResposta->recuperarResposta( $tabela, $qrpid );
		
		//SUBTITULO
		$obGrupoColuna  = new Grupocoluna( );
		$arrGrupoColuna = $obGrupoColuna->pegaDados( $tabela );
	
		//LINHAS
		$obLinha  		= new Linha( );
		$obGrupolinha  	= new Grupolinha( );
		$arrLinha 		= $obLinha->pegaDados( $tabela );
		
		//COLUNAS
		$obColuna  = new Coluna( );
		$arrColuna = $obColuna->pegaDados( $tabela );
		
		$l = 0;
		
		$bgcolor1 = "#A8A8A8";
		$bgcolor2 = "#CCCCCC";
					
		//Montando a Tabela
		$htm .= "<input type='hidden' name='idTabela' id='idTabela' value='{$tabela}'>";
		$htm .= "<input type='hidden' name='tipoTabela' id='tipoTabela' value='{$arrDadosTabela['tbltipo']}'>";
		$htm .= "<table class='tabela' style='width:{$percent}%;' align='center'>";
		$htm .= "<tr>";
		$htm .= "<td bgcolor='$bgcolor1' colspan='50' align='center'><b>{$arrDadosTabela['descricao']}</b></td>";
		$htm .= "</tr>";
		$htm .= "<tr>";
		$grupo = $obGrupolinha->conta( $tabela );

		$rolspan = $obGrupoColuna->verificaNiveis( $tabela );
		$colspan = $grupo > 1 ? 2 : 1;
		
		if( $arrDadosTabela['colunas'] && $arrDadosTabela['linhas'] ){
			$htm .= "<td bgcolor='$bgcolor1' align='center' colspan='$colspan' rowspan='{$rolspan}'><b>{$arrDadosTabela['colunas']} / {$arrDadosTabela['linhas']}</b></td>";
		} else {
			$flag = 2;
		} 
		foreach( $arrGrupoColuna as $subTitulo ){
			$arrGrcid = $obGrupoColuna->pegaArrUltimoNivel( $subTitulo['codigo'] );
			
			$col = $obColuna->conta( $arrGrcid );

			if( $subTitulo['descricao'] <> '&nbsp;' ){
				if( $arrDadosTabela['colunas'] == '' && $arrDadosTabela['linhas'] == '' ){
					$htm .= "<td bgcolor='$bgcolor1' align='center'></td>";
				}
				if( $grupai <> $subTitulo['grupai'] ){
					$htm .= "</tr><tr><td bgcolor='$bgcolor1' align='center' colspan='{$col}'><b>{$subTitulo['descricao']}</b></td>";
				} else {
					$htm .= "<td bgcolor='$bgcolor1' align='center' colspan='{$col}'><b>{$subTitulo['descricao']}</b></td>";
				}
			}
			$grupai = $subTitulo['grupai'];
		}
		$htm .= "</tr>";
		//Monta as colunas
		$htm .= "<tr>";
		if( $flag == 2 ){
			$htm .= "<td bgcolor='$bgcolor1' align='center' width='40%'><b>{$arrLinha[0]['grupolinha']}</b></td>";
		}
		foreach( $arrColuna as $coluna ){
			$htm .= " <td bgcolor='$bgcolor2' align='center'><b>{$coluna['descricao']}</b></td>";
		}
		$htm .= "</tr>";
		
		$grp = 0;
		$bgcolor = "$bgcolor2";
		foreach( $arrLinha as $linha ){
			$htm .= "<tr>";
			if( $grp <> $linha['grupoid'] ){
				if( $linha['grupolinha'] <> '&nbsp;' && $linha['grupopai'] || $grupo > 1 ){
					$numero = $obLinha->conta( $linha['grupoid'] );
					$htm .= "<td bgcolor='$bgcolor2' rowspan='{$numero}' width='10%' align='center'><b>{$linha['grupolinha']}</b></td>";
				}
			}
			if( $linha['lindsc'] == 't' ){
				$resposta = isset($arrResp[$linha['codigo']]['']) ? $arrResp[$linha['codigo']][''] : '';
				$htm .= "<td bgcolor='$bgcolor'><b>{$linha['descricao']}: ".campo_texto('campo_'.$linha['codigo'],'N','S','',20,60,'','','','','','campo_'.$linha['codigo'], '', $resposta)."</b></td>";
			} else {
				$htm .= "<td bgcolor='$bgcolor'><b>{$linha['descricao']}</b></td>";
			}
			$l++;
			$grp = $linha['grupoid'];
			foreach( $arrColuna as $coluna ){
				$c++;
				if ($l % 2){
					$bgcolor = $bgcolor1;
				}else{
					$bgcolor = $bgcolor2;
				}
				// O miolo da Tabela
				$resposta = isset($arrResp[$linha['codigo']][$coluna['codigo']]) ? $arrResp[$linha['codigo']][$coluna['codigo']] : '';
				switch ($arrDadosTabela['tbltipo']){
	 			case 'CK':
	 				$checked = $resposta == 1 ? 'checked="checked"' : '';
					$htm .= "<td align='center'><input type=\"checkbox\" name=\"campo_" . $linha['codigo'].'_'.$coluna['codigo'] . "\" value='1' " . $checked . " />";
	 				break;
	 			case 'TX':
					$htm .= "<td align='center'>".campo_texto('campo_'.$linha['codigo'].'_'.$coluna['codigo'],'N','S','',10,60,'','','','','','campo_'.$linha['codigo'].'_'.$coluna['codigo'], '', $resposta)."</td>";
	 				break;
	 			}
			}
			$c = 0;
			$htm .= "</tr>";
		}
		$htm .= "</table>";
		$this->htm = $htm;
	}
	
	function salvar() {
		
		$qrpid = $_POST['qrpid'];
		
		$obTabelaResposta = new Tabelaresposta( );
		$obTabelaResposta->excluirPorQuestionarioTabela( $qrpid, $_POST['idTabela'] );
		$obTabelaResposta->commit();
		
		foreach ($_POST as $chave => $values)
		{
			if($values <> '' && strstr($chave,"campo_")){	
				$obTabelaResposta = new Tabelaresposta( );
				list( , $linha, $coluna ) = explode("_",$chave);;
				
				$arrCampos = array('tblid', 'linid', 'colid', 'qrpid', 'tbrdsc');
				$arrDados  = array("tblid" => $_POST['idTabela'],
									  "linid" => $linha,
									  "colid" => $coluna,
									  "qrpid" => $qrpid,
									  "tbrdsc" => $values);
				$obTabelaResposta->popularObjeto( $arrCampos, $arrDados );
				$obTabelaResposta->salvar();
				$obTabelaResposta->commit();
			}
		}
	}
	
	function show(){
		echo $this->htm;
	}
}

?>