<?php
function exibeNovosIndicadores(){
	global $db;
	//Pega os indicadores do usuário 
	$meusIndicadores = pegaMeusIndicadores();
	
	if(count($meusIndicadores) != 0){
		//Lista os novos indicadores atribuidos pelo usuário
		$sql = "select 
					indid,indnome 
				from 
					painel.indicador
				where 
					indid in (".implode(",",$meusIndicadores).")";
		$indicadores = $db->carregar($sql);
	}
	$indicadores = !$indicadores? array() : $indicadores;
	
	echo "<div id=\"0\" class=\"groupItem\">";
		echo "<div style=\"-moz-user-select: none;\" class=\"itemHeader\">Meus Indicadores</div>";
			echo "<div class=\"itemContent\">";
			echo "<table cellSpacing=0 cellPadding=3 style=\"width:100%\">";
			echo "<tr bgcolor='#D9D9D9' ><td style=\"font-weight:bold\" >&nbsp;&nbsp;Ações&nbsp;&nbsp;</td><td style=\"font-weight:bold\" >indid</td><td style=\"font-weight:bold\"  >Nome do Indicador</td></tr>";
			$i = 0;
			foreach($indicadores as $ind){
				$cor = $i%2 ? "#FCFCFC" : "#f5f5f5";
				echo "<tr id=\"tr_{$ind['indid']}\" bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='$cor';\" >
					<td><span id=\"acao_editar_{$ind['indid']}\" onclick=\"editarMeuIndicador({$ind['indid']})\" style=\"cursor:pointer\">[ v ]</span> <span onclick=\"removerMeuIndicador({$ind['indid']})\" style=\"cursor:pointer\" >[ x ]</span>
						<div id=\"div_editar_{$ind['indid']}\" style=\"display:none\" class=\"div_editar\" >
							Enviar para 
									<select>
										<option>Selecione...</option>
										<option>Box 1</option>
										<option>Box 2</option>
									</select>
						</div>
					</td>
					<td>".$ind['indid']."</td>
					<td>".$ind['indnome']."</td></tr>";
				$i++;
			}
			
			if(count($indicadores) == 0){
				echo "<tr><td colspan=3 style=\"text-align:center;color:#990000\" >Não existem indicadores atribuídos pelo usuário.</td></tr>";
			}
			
			echo "</table>";
		echo "</div>";
	echo "</div>";
}
?>