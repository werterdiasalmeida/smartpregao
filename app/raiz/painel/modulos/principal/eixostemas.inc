<?
if($_REQUEST['AjaxCarregaMapa'] == 1){
	header('content-type: text/html; charset=ISO-8859-1');
	exit;
}

function montaBox($arrEixos,$eixoid,$eixodsc,$estado_eixoid,$scroll_eixoid){
	global $db;
	//Estado da Coluna (Maximizada / Minimizada)
			(!$estado_eixoid)? $estado_eixoid = "max" : $estado_eixoid = $estado_eixoid;
			($estado_eixoid == 'max')? $style_est = "" : $style_est = "none";
			($estado_eixoid == 'max')? $src_img_est = "../imagens/menos.gif" : $src_img_est = "../imagens/mais.gif";
			(!$scroll_eixoid)? $scroll_eixoid = "scrollfalse" : $scroll_eixoid = $scroll_eixoid;
			?>
			<div id="colid_<?=$eixoid?>" class="groupItem">
			<input type="hidden" id="estado_<?=$eixoid?>" name="estado_<?=$eixoid?>" value="<?=$estado_eixoid?>"  />
			<input type="hidden" id="scroll_<?=$eixoid?>" name="scroll_<?=$eixoid?>" value="<?=$scroll_eixoid?>"  />
				<div style="-moz-user-select: none;" class="itemHeader"><div style="float:left;padding-left: 10px;" ><?=$eixodsc?></div><div id="edit_<?=$eixoid?>"  class="edit" ><img onclick="exibeEditar('<?=$eixoid?>')"  src="../imagens/ico_config.gif" border=1 /></div><a href="#" class="closeEl" id="<?=$eixoid?>" ><img src="<?=$src_img_est ?>" border=0 /></a></div>
					<div style="display:none"  id="editar_<?=$eixoid?>" class="editar" ><div style="padding: 3px 3px 3px 3px;" >
					<?if(is_numeric($eixoid)){?>
						<input type="radio" id="radio_<?=$eixoid?>" onclick="editaIndicador(<?=$eixoid?>);desmarcaRadio(<?=$eixoid?>);"  /><span onclick="editaIndicador('<?=$eixoid?>');desmarcaRadio('<?=$eixoid?>');" >Editar Indicador</span><br>
					<?} ?>
					<input type="checkbox" id="autoscroll_<?=$eixoid?>" name="autoscroll" value="<?=$eixoid?>" onclick="executarEditar('<?=$eixoid?>',false)" /> Rolagem
					</div></div>
					<div class="itemContent" style="display:<?=$style_est ?>" id="itemContent_<?=$eixoid?>"  >
					<?if($scroll_eixoid == "scrolltrue"){ ?>
					<script>
						document.getElementById( 'autoscroll_<?=$eixoid?>').checked = true;
						executarEditar(<?=$eixoid?>,true);
						document.getElementById( 'itemContent_<?=$eixoid?>').style.display = '<?=$style_est?>';
					</script>
					<?}?>
					
					<?if(is_numeric($eixoid)){
						################# Pega os Indicadores ################# //Filtro (indstatus = 'A')
						$sql = "select 
									distinct(indid) as indid 
								from 
									painel.indicador 
								where 
									exoid = '$eixoid'
								and
									indstatus = 'A'
								and
									indpublicado is true";
						$obj = $db->carregar($sql);
					}
					if($obj){	
						unset($indid);
						foreach($obj as $ob){
							$arrIndid[] = $ob['indid'];
						}
						$arrIndid = $arrIndid ? $arrIndid : array();
						
						$indicadores = implode(",",$arrIndid);
						
						if($indicadores){ // Se existir indicadores, lista as ações
						//Nivel 2 - Ações
						################# Pega as Ações de acordo com os Indicadores ################# //Filtro (array de $indicadores)
							$sql = "select 
										acaid as acaid 
									from 
										painel.acao a 
									where 
										a.acaid in(
													select 
														distinct(i.acaid) as acaid 
													from 
														painel.indicador i
													right join 
														painel.seriehistorica s ON s.indid = i.indid
													right join 
														painel.acao a ON a.acaid = i.acaid
													where 
														i.indid in ($indicadores)
													and
														i.indstatus = 'A'
													and
														i.indpublicado is true
													)
									order 
										by a.acadsc";
							
							$acaid = $db->carregar($sql);
							$acaid = $acaid ? $acaid : array();
						}
						if($acaid){ // Se existir ações, pega os dados das ações
						################# Pega os dados das Ações de acordo com as Ações filtradas pelos Indicadores #################
							foreach($acaid as $acid){ //Se existir ações ($acaid)
								
								$sql = "select 
											* 
										from 
											painel.acao 
										where 
											acaid = {$acid['acaid']}";
								$acaid = $db->pegaLinha($sql);
								
								echo "<table width=100% class=listagem>";
								echo "<tr><td style=\"padding:5px;\" colspan=\"3\" align=\"center\"><span style=\"padding-left:5px;font-weight:bold;font-size:13px;color:#005500;\" >".$acaid['acadsc']."</span><br /></td></tr>";
									
								//Nível 3
								################# Pega os Indicadores de acordo com as Ações encontradas ################# //Filtro (indstatus = 'A' / acaid)	
								$sql = "select 
											distinct(i.indid),
											i.indnome 
										from 
											painel.indicador i
										right join 
											painel.seriehistorica s ON s.indid = i.indid
										where 
											i.acaid = {$acid['acaid']}
										and
											i.indstatus = 'A'
										and
											i.indpublicado is true 
										order by 
											indnome asc";
											
								$ind2 = $db->carregar($sql);
								$ind2 = $ind2 ? $ind2 : array();
								
								foreach($ind2 as $ind){ // Exibe os Indicadores existentes
								
								################# Periodicidade ################# //Pega Periodicidade do Indicador
								$sql = "select 
											perdsc 
										from 
											painel.indicador i
										left join 
											painel.periodicidade p on i.perid = p.perid
										where 
											indid = {$ind['indid']}
										and
											indpublicado is true";
								$perdsc = $db->pegaUm($sql);
										
								################# Diária ################# 
								if($perdsc == "Diária"){ //Se a Periodicidade do Indicador for Diária, busca data e quantidade (sehdtcoleta / sehqtde) da tabela seriehistorica  
									$sql = "select 
											sehid,
											sehqtde,
											(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
										from 
											painel.seriehistorica 
										where 
											indid = {$ind['indid']}
										and 
											dpeid is null
										and
											sehstatus <> 'I'
										order by
											data desc
										limit
											1";
								}
								################# Se não for Diária ################# 
								if($perdsc != "Diária"){ //Se a Periodicidade do Indicador não for Diária, busca data (dpedatainicio) da tabela detalheperiodicidade e quantidade (sehdtcoleta) da tabela seriehistorica
									$sql = "select
												seh.sehid,
												seh.sehqtde,
												(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data 
											from 
												painel.seriehistorica  seh
											inner join
												painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid
											where  
												indid = {$ind['indid']}
											and
												seh.dpeid is not null
											and 
												sehstatus <> 'I'
											order by
												data desc
											limit
												1";
								}
										
								$seh = $db->pegaLinha($sql);
								
								################# Comparação do penúltimo valor / data para indicar a seta de tendência (cima / baixo) ################# 
								$seta = "";  //variável seta inicia vazia;
								
								################# Veirifica se existe a Série Histórica p/ comparar #################
								if($seh['sehid']){ // se existe, busca novamente a série histórica, com excessão do último sehid
									
									################# Diária ################# 
									if($perdsc == "Diária"){ //Se a Periodicidade do Indicador for Diária, busca data e quantidade (sehdtcoleta / sehqtde) da tabela seriehistorica  
										$sql = "select 
											sehid,
											sehqtde,
											(to_char(sehdtcoleta, 'YYYY-MM-DD')) as data 
										from 
											painel.seriehistorica 
										where 
											indid = {$ind['indid']}
										and 
											dpeid is null
										and
											sehstatus <> 'I'
										and
											sehid != {$seh['sehid']}
										order by
											data desc
										limit
											1";
									}
									
									################# Se não for Diária ################# 
									if($perdsc != "Diária"){ //Se a Periodicidade do Indicador não for Diária, busca data (dpedatainicio) da tabela detalheperiodicidade e quantidade (sehdtcoleta) da tabela seriehistorica
										$sql = "select
												seh.sehid,
												seh.sehqtde,
												(to_char(dpe.dpedatainicio, 'YYYY-MM-DD')) as data 
											from 
												painel.seriehistorica  seh
											inner join
												painel.detalheperiodicidade dpe ON seh.dpeid = dpe.dpeid
											where  
												indid = {$ind['indid']}
											and
												seh.dpeid is not null
											and 
												sehstatus <> 'I'
											and
												sehid != {$seh['sehid']}
											order by
												data desc
											limit
												1";
									}
											
									$seh2 = $db->pegaLinha($sql);
									
									################# Veirifica qual o tipo de tendência do indicador (Maior melhor, Menor melhor) #################

									$sql = "select estid from painel.indicador where indid = {$ind['indid']}";
									$estilo = $db->pegaUm($sql);
									$estilo = !$estilo ? 1 : $estilo;
									 
									################# Veirifica se existe a 2ª Série Histórica p/ comparar #################
									if($seh2['sehid']){ // se existe, faz a comparação para indicar a seta
										
										$valor1 = (float)$seh['sehqtde']; //seh
										$valor2 = (float)$seh2['sehqtde']; //seh2
			
										if($valor1 < $valor2 && $estilo == 1){ // Se o $valor 1 for menor q	o $valor 2 e a tendência for MAIOR MELHOR, a tendência indica queda (seta vermelha p/ baixo)
											$seta = " <img onmouseover=\"this; return escape('Tendência');\" style=\"cursor:pointer\" src=\"../imagens/indicador-vermelha2.png\" align=\"absmiddle\" >";
										}
										if($valor1 < $valor2 && $estilo == 2){ // Se o $valor 1 for menor q	o $valor 2 e a tendência for MENOR MELHOR, a tendência indica aumento (seta verde p/ baixo)
											$seta = " <img onmouseover=\"this; return escape('Tendência');\" style=\"cursor:pointer\" src=\"../imagens/indicador-verde2_para_baixo.png\" align=\"absmiddle\" >";
										}
										if($valor1 > $valor2 && $estilo == 1){ // Se o $valor 1 for maior q	o $valor 2 e a tendência for MAIOR MELHOR, a tendência indica aumento (seta verde p/ cima)
											$seta = " <img onmouseover=\"this; return escape('Tendência');\" style=\"cursor:pointer\"  src=\"../imagens/indicador-verde2.png\" align=\"absmiddle\" >";
										}
										if($valor1 > $valor2 && $estilo == 2){ // Se o $valor 1 for maior q	o $valor 2 e a tendência for MENOR MELHOR, a tendência indica queda (seta vermelha p/ cima)
											$seta = " <img onmouseover=\"this; return escape('Tendência');\" style=\"cursor:pointer\"  src=\"../imagens/indicador-vermelha2_para_cima.png\" align=\"absmiddle\" >";
										}
										if($valor1 > $valor2 && $estilo == 3){ // Se o $valor 1 for maior q	o $valor 2 e a tendência for N/A, a tendência indica aumento (seta cinza p/ cima)
											$seta = " <img style=\"cursor:pointer\"  src=\"../imagens/indicador-cinza.png\" align=\"absmiddle\" >";
										}
										if($valor1 < $valor2 && $estilo == 3){ // Se o $valor 1 for menor q	o $valor 2 e a tendência for N/A, a tendência indica queda (seta cinza p/ baixo)
											$seta = " <img style=\"cursor:pointer\"  src=\"../imagens/indicador-cinza2.png\" align=\"absmiddle\" >";
										}
									}
									
									################# Verifica o Tipo de unidade de medida do indicador (umedesc) na tabela unidademeta, p/ exibir no SuperTitle #################
									$sql = "select 
												umedesc 
											from 
												painel.indicador i
											left join 
												painel.unidademeta u on i.umeid = u.umeid
											where 
												indid = {$ind['indid']}";
										
									$umedesc = $db->pegaUm($sql);
									
									################# Verifica o Tipo de unidade de Medição do indicador (unmdesc) na tabela unidademedicao, p/ exibir as casas decimais corretas no sehqtde #################
									$sql = "select 
												unmdesc 
											from 
												painel.indicador i
											left join 
												painel.unidademedicao u on i.unmid = u.unmid
											where 
												indid = {$ind['indid']}";
									
									$unmedsc = $db->pegaUm($sql);
									
									//Verifica o tipo de medição e aplica as regras
									switch($unmedsc){
											case "Número inteiro":
												$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
												break;
											case "Percentual":
												$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'))."%";
												break;
											case "Razão":
												$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
												break;
											case "Número índice":
												$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
												break;
											case "Moeda":
												$seh['sehqtde'] = "R$ ".number_format($seh['sehqtde'],2,',','.');
												break;
											default:
												$seh['sehqtde'] = str_replace(',00','',number_format($seh['sehqtde'],2,',','.'));
												break;
										}
									
									################# Verifica a Periodicidade e aplica as regras de exibição de data #################
									switch($perdsc){
										
										case "Diária":
											$tp_data = "d/m/Y";
											break;
										
										case "Mensal":
											$tp_data = "m/Y";
											break;
										
										case "Anual":
											$tp_data = "Y";
											break;
										
										case "Semestral":
											$tp_data = "m/Y";
											break;
										
										case "Trimestral":
											$tp_data = "m/Y";
											break;
										
										case "Semanal":
											$tp_data = "d/m/Y";
											break;
										
										case "Bianual":
											$tp_data = "d/m/Y";
											break;
										
										default:
											$tp_data = "d/m/Y";
											break;
	
									}
									
									################# Pega o Responsável pelo Indicador para exbir no SuperTitle #################
									$responsaveis_ind = "<font size=2 ><b>Responsáveis</b></font>";
									
									//Coordenador
									$responsaveis = $db->carregar("SELECT ent.entnome ,ent.entnumcomercial FROM painel.responsavel resp 
											LEFT JOIN entidade.entidade ent ON ent.entid = resp.entid WHERE indid = {$ind['indid']} and resp.restipo = 'C' ");
									if($responsaveis){
										$responsaveis_ind .= "<br /><b>Coordenador:</b>";
										foreach($responsaveis as $respon){
											$responsaveis_ind .= "<br /> {$respon['entnome']} - {$respon['entnumcomercial']}";	
										}
									}else{
										$responsaveis_ind .= "<br /><b>Coordenador:</b> N/A";
									}
									
									//Diretor
									$responsaveis = $db->carregar("SELECT ent.entnome ,ent.entnumcomercial FROM painel.responsavel resp 
											LEFT JOIN entidade.entidade ent ON ent.entid = resp.entid WHERE indid = {$ind['indid']} and resp.restipo = 'D' ");
									if($responsaveis){
										$responsaveis_ind .= "<br /><b>Diretor:</b>";
										foreach($responsaveis as $respon){
											$responsaveis_ind .= "<br /> {$respon['entnome']} - {$respon['entnumcomercial']}";	
										}
									}else{
										$responsaveis_ind .= "<br /><b>Diretor:</b> N/A";
									}
									
									//Equipe de Apoio
									$responsaveis = $db->carregar("SELECT ent.entnome ,ent.entnumcomercial FROM painel.responsavel resp 
											LEFT JOIN entidade.entidade ent ON ent.entid = resp.entid WHERE indid = {$ind['indid']} and resp.restipo = 'E' ");
									if($responsaveis){
										$responsaveis_ind .= "<br /><b>Equipe de Apoio:</b>";
										foreach($responsaveis as $respon){
											$responsaveis_ind .= "<br />{$respon['entnome']} - {$respon['entnumcomercial']}";	
										}
									}else{
										$responsaveis_ind .= "<br /><b>Equipe de Apoio:</b> N/A";
									}
																		
									################# Verifica se o Indicador possui dados para exibição no Google Maps #################
									//Para exibição no Google Maps, pelo menos um dos campos (dshcod,dshmunicipio,dshuf) da tabela detalheseriehistorica deve estar preenchido
									// 1 - dshcod => Regionalização = Escola => indica uma FK para a tabela painel.escola (campo esccodinep);
									// 2 - dshcod => Regionalização = IES => indica uma FK para a tabela painel.ies (campo iesid);
									// 3 - dshmunicipio => Regionalização = Municipal => indica uma FK para a tabela territorio.municipio (campo muncod);
									// 4 - dshuf => Regionalização = Estadual => indica uma FK para a tabela territorio.estado (campo estuf) (vincular capital muncodcapital => municipio.muncod);
									$sql = "select
												dshcod,
												dshuf,
												dshcodmunicipio,
												regid
											from 
												painel.detalheseriehistorica det
											inner join
												painel.seriehistorica seh ON seh.sehid = det.sehid
											inner join
												painel.indicador ind ON ind.indid = seh.indid
											where 
												ind.indid = {$ind['indid']}
											limit 1";
									$googleMaps = $db->pegaLinha($sql);
									
									if(($googleMaps['dshuf'] != "" || $googleMaps['dshcod'] != "" || $googleMaps['dshcodmunicipio'] != "") && ($googleMaps['regid'] != "null" && $googleMaps['regid'] != "" && $googleMaps['regid'] != 1 && $googleMaps['regid'] != 3)){
											$mapa = "<img src=\"../imagens/globo_terrestre.png\" style=\"cursor:pointer;margin-left:3px;margin-right:3px;\" onclick=\"ExibeMapa({$ind['indid']},{$seh['sehid']})\" onmouseover=\"this; return escape('Visualizar no mapa.');\" />";
									}else{
											$mapa = "";
									}
									
									//Início da Exibição dos elementos na tela
									echo "<tr onmouseout=\"this.bgColor='';\" onmouseover=\"this.bgColor='#ffffcc';\">";
									
									//Se existir quantidade, exibe o gráfico e habilita a comparação
									if($seh['sehqtde']){
											//Campo check para selecionar a comparação do gráfico
											echo "<td valign=\"top\" ><input onclick=\"comparaGrafico()\" type=\"checkbox\" name=\"comp_graf\" value=\"{$ind['indid']}\"  id=\"comp_ind_{$ind['indid']}\" /></td>";
											
											//Exibe o nome do Indicador e monta o link para exibição do gráfico com a Ação (acaid) e o Indicador (indid)
											echo "<td ><span style=\"font-size:12px\" ><a href=\"painel.php?modulo=principal/serie_historica&acao=A&acaid={$acid['acaid']}&indid={$ind['indid']}\" rel=\"lyteframe\" rev=\"width: 800px; height: 450px; scrolling: auto;\"  >".$ind['indnome']."</a></span></td>";
									}
									//Se não existir quantidade, desabilita a comparação e a exibição do gráfico
									else{
											//Campo check para selecionar a comparação do gráfico DESABILITADO
											echo "<td valign=\"top\" ><input disabled=\"disabled\"  type=\"checkbox\" name=\"comp_graf\" value=\"{$ac2['indid']}\"  id=\"comp_ind_{$ind['indid']}\" /></td>";
											
											//Exibe o nome do Indicador e um alert informando que ñ existe série histórica para a formação de gráfico
											echo "<td ><span style=\"font-size:12px;\" ><a style=\"cursor:pointer\" onclick=\"alert('Série Histórica Não Atribuída.')\" >".$ind['indnome']."</a></span></td>";
									}
									
									//Exibe a data e o mapa
									echo "<td width=20% align=\"left\" ><div style=\"margin-left:10px\" >".(!$seh['data']? "<span style=\"color:#AA0000;\" >N/A</span>" : "<span onmouseover=\"this; return escape('$responsaveis_ind');\" >".date($tp_data,strtotime($seh['data']))."</span>")." $mapa </div></td>";
									
									//Exibe a quantidade
									echo "<td width=30% align=\"right\" >".(!$seh['sehqtde']? "<span style=\"color:#AA0000;\" >N/A</span>" : "<span onmouseover=\"this; return escape('{$seh['sehqtde']} $umedesc');\" >".$seh['sehqtde'])."</span> $seta</td>";
									
									echo "<tr>";
									
								
								}
								
									}
									echo "<tr><td colspan=3>&nbsp;</td></tr></table>";
							}
						}
						elseif(!$acaid && is_numeric($eixoid)){
							echo "<div style=\"color: #AA0000;text-align:center;padding-top:10px;width:100%\" >Indicadores não informados.</div>";
						}
					}
					elseif(!is_numeric($eixoid)){
						if($eixoid == 'box1'){
							echo "<div><div id=\"carregando_mapas\"  style=\"position:relative;top:140px;width:90%;margin:2px;text-align:center;float:left\" >Visualização não disponível.</div><div id=\"mapa_indicadores\"  style=\"height:300px;margin:2px;\" ></div></div>";
						}
						else{
							echo "<div style=\"color: #AA0000;text-align:center;padding-top:10px;width:100%\" >BOX Personalizado.</div>";
						}
					}?>
				</div>
			</div>
<? }

function salvaParametros($arrSor1,$arrSor2,$estados,$scroll,$usucpf){
	global $db;
	
	$sql = "select usucpf from public.parametros_tela
			where usucpf = '$usucpf' and mnuid = '{$_SESSION['mnuid']}'";
	$parm = $db->pegaUm($sql);
		
	($arrSor1)? $sor1 = implode(';',$arrSor1) : '';
	($arrSor2)? $sor2 = implode(';',$arrSor2) : '';
	
	($estados) ? $estados = "/".$estados : $estados = "";
	($scroll) ? $scroll = "/".$scroll : $scroll = "";
	
	$prtbj = "sor1=$sor1/sor2=$sor2$estados$scroll";
	
	if($parm == ""){ //insert
		$sql = "insert into public.parametros_tela
			(prtdsc,prtobj,prtpublico,prtdata,usucpf,mnuid) values
			('Eixo - Parametros de Colunas','$prtbj','FALSE',now(),'$usucpf','{$_SESSION['mnuid']}')";
	}
	if($parm != ""){ //update
		$sql = "update public.parametros_tela set
			prtobj = '$prtbj',
			prtdata = now()
			where usucpf = '$usucpf'
			and mnuid = '{$_SESSION['mnuid']}'";
	}
	$db->executar($sql);	
	$db->commit();
}


//Salva parametros.
$sql = "SELECT temid, temdsc FROM painel.tema WHERE temstatus = 'A'";
$dados_tema = $db->carregar($sql);

//Salva os Parametros da Tela
if($_REQUEST['salvaParametros']){
	header('content-type: text/html; charset=ISO-8859-1');
	salvaParametros($_REQUEST['sort1'],$_REQUEST['sort2'],$_REQUEST['estados'],$_REQUEST['scroll'],$_SESSION['usucpf']);
	exit;
}

//Pega Parametros da Tela
$sql = "select * from public.parametros_tela where usucpf = '{$_SESSION['usucpf']}' and mnuid = '{$_SESSION['mnuid']}'";
$parm = $db->pegaLinha($sql);

$param = $parm['prtobj'];

if($param){
	$sort = explode("/",$param);
	$sort1 = $sort[0]; //coluna 1
	$sort2 = $sort[1]; //coluna 2
	$est = $sort[2]; //estados
	$srl = $sort[3]; //scroll
	$arrSort1 = explode(";",str_replace('sor1=','',$sort1));
	$arrSort2 = explode(";",str_replace('sor2=','',$sort2));
	$estados = explode(",",$est);
	$scroll = explode(",",$srl);
	$tipo = "update";	
}
else{
	$tipo = "insert";
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

//API Google Maps
include (APPRAIZ.'www/painel/_funcoes_mapa_indicadores.php');

$titulo = "Painel de Controle";
monta_titulo( $titulo, '' );
?>
<style type="text/css">	
	/*Drag adn Drop Google*/
	
		.groupWrapper
	{
		width: 47%;
		float: left;
		margin-right: 5px;
		min-height: 400px;
	}
		
	.groupItem
	{
		margin-bottom: 20px;
		border: #e0e0e0 solid 2px;
	}
	.groupItem .itemHeader
	{
		line-height: 28px;
		background-color: #e0e0e0;
		color: #000;
		cursor: move;
		font-weight: bold;
		font-size: 16px;
		height: 28px;
		position: relative;
		width: 100%;
	}
	
	.groupItem .itemHeader a
	{
		position: absolute;
		right: 10px;
		top: 0px;
		font-weight: normal;
		font-size: 11px;
		text-decoration: none;
	}
	.sortHelper
	{
		border: 3px dashed #666;
		width: auto !important;
	}
	.groupWrapper p
	{
		height: 1px;
		overflow: hidden;
		margin: 0;
		padding: 0;
		width: 100%;
	}
	.itemContent{
		overflow: auto;
		width: 100%;
	}
	.edit{
	cursor: pointer;
	float:right;
	font-size:12px;
	font-weight:normal;
	padding-right: 30px;
	padding-top: 5px;
	}
	.edit img{width:12px ; height:10px; border: #000000 solid 1px}
	
	.editar{
		font-size:12px;
		width: 100%;
		border-top: #000000 solid 1px;
		background-color: #DFDFDF;
		cursor: default;
	}
	.closeEl{
		padding-top: 5px;
	}
	.closeEl img{
		width:12px ; height:12px; border: none;
	}
	
	
	</style>
<link rel="StyleSheet" href="/includes/dtree/dtree.css" type="text/css" />
<link rel="stylesheet" href="/includes/LyteBox/lytebox.css" type="text/css" media="screen" />
<script type="text/javascript" src="/includes/dtree/dtree.js">window.open('/www/includes/dtree/dtree.js');</script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/superTitle.js"></script>
<script type="text/javascript" src="/includes/LyteBox/lytebox.js" ></script>
<script type="text/javascript" src="/includes/prototype.js"></script>

<script type="text/javascript">

<!--
 function exibeComparacao(arrID){
 	
 	document.getElementById( 'link_comparar' ).href = 'painel.php?modulo=principal/compara_serie&acao=A&indid=' + arrID;
 	document.getElementById( 'botao_comparar' ).style.display = '';
 }

 function comparaGrafico(){
 	
 	var checks = document.getElementsByTagName("input");
	var num = checks.length;
 	var ckeckados = 0;
 	var arrID = new Array();
 	
 	for(var i=0; i < num; i++){
		if(document.getElementsByTagName('input')[i].type == 'checkbox'){
        	if(document.getElementsByTagName('input')[i].checked){
        		arrID[ckeckados] = document.getElementsByTagName('input')[i].value;
        		ckeckados ++;
        		}
        	}
     }
    if(ckeckados <= 1){
    	document.getElementById( 'link_comparar' ).href = '#';
    	document.getElementById( 'botao_comparar' ).style.display = 'none';
    }
    if(ckeckados >= 2){
    	exibeComparacao(arrID);
    }
    
//-->
}

function exibeEditar(id){
	var eixoid = id;
	document.getElementById( 'editar_' + eixoid ).style.display = '';
	document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="escondeEditar(\'' + eixoid + '\');executarEditar(\'' + eixoid + '\',true)" />';
}

function escondeEditar(id){
	var eixoid = id;
	document.getElementById( 'editar_' + eixoid ).style.display = 'none';
	document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="exibeEditar(\'' + eixoid + '\');executarEditar(\'' + eixoid + '\',true)" />';
}

function executarEditar(id,display){
	var eixoid = id;
	if(document.getElementById( 'autoscroll_' + eixoid)){
		if(document.getElementById( 'autoscroll_' + eixoid).checked == true){
			document.getElementById( 'itemContent_' + eixoid ).style.height = '300px';
			document.getElementById( 'itemContent_' + eixoid ).style.overflow = 'auto';
			document.getElementById( 'scroll_' + eixoid ).value = 'scrolltrue';
		}
		if(document.getElementById( 'autoscroll_' + eixoid).checked == false){
			document.getElementById( 'itemContent_' + eixoid ).style.height = 'auto';
			document.getElementById( 'itemContent_' + eixoid ).style.overflow = 'hidden';
			document.getElementById( 'scroll_' + eixoid ).value = 'scrollfalse';
		}
	}
	if(display == true){
		document.getElementById( 'editar_' + eixoid ).style.display = 'none';
	}
	document.getElementById( 'edit_' + eixoid ).innerHTML = '<img src="../imagens/ico_config.gif" onclick="exibeEditar(\'' + eixoid + '\');" />';
}

function desmarcaRadio(id){
	var eixoid = id;
	document.getElementById( 'radio_' + eixoid ).selected = false;
}

function editaIndicador(id){
	var eixoid = id;
	//alert('Selecione os Indicadores: para eixoid = ' + eixoid);
	
}


function limpaComparacao(){
	var checks = document.getElementsByTagName("input");
	var num = checks.length;
 	var ckeckados = 0;
 	
	for(var i=0; i < num; i++){
		if(document.getElementsByTagName('input')[i].type == 'checkbox'){
        	if(document.getElementsByTagName('input')[i].checked){
        		document.getElementsByTagName('input')[i].checked = false;
        		ckeckados ++;
        		}
        	}
     }
    document.getElementById( 'botao_comparar' ).style.display = 'none';
}

</script>


<form method="POST"  name="formulario" id="formulario">
<table class="tabela" width="95%" bgcolor="FFFFFF" cellSpacing="1" border=0 cellPadding="3" align="center">
<tr style="font-weight: bold;" >
	<td width="255px"  valign="top" >
	<div class="dtree" style="width:250px;height:600px;border:#fcfcfc solid 1px;overflow:auto;">
	<p><a href="javascript: d.openAll();">Abrir Todos</a> &nbsp;|&nbsp; <a
					href="javascript: d.closeAll();">Fechar Todos</a></p>
			<div id="_arvore" style="overflow:hidden;width: 250px;" >
			</div>
		</div></td>
	<td colspan="3" valign="top">
	<!-- inicio da div de conteudo flutuante  -->
	<div id="conteudo_flutuante" style="width: 100%;"  >
	<!-- Início do Drag and Drop  -->
	
	<?
	//Nivel 1
			if($tipo == "update"){
				$arrEixosSor1 = array();
				$arrEixosSor2 = array();
				$est = 0;
				$exi = 1;
				foreach($arrSort1 as $colid){
					if($colid != ""){
						$id = str_replace("colid_","",$colid);
						$id = trim($id);
						if($id != 'box1'){
							$id = (int)$id;
							$sql = "select * from painel.eixo where exoid = $id";
							$eixo = $db->pegaLinha($sql);
							$arrEixosSor1[$exi] = $eixo['exoid']."_".$eixo['exodsc']."_".$estados[$est]."_".$scroll[$est];
						}
						if($id == 'box1'){
							//BOX Personalizados
							$arrEixosSor1[$exi] = "box1_Mapa de Indicadores_".$estados[$est]."_".$scroll[$est];
						}
						$exi++;
						$est++;
					}
					
				}
				foreach($arrSort2 as $colid){
				if($colid != ""){
						$id = str_replace("colid_","",$colid);
						$id = trim($id);
						if($id != 'box1'){
							$id = (int)$id;
							$sql = "select * from painel.eixo where exoid = $id";
							$eixo = $db->pegaLinha($sql);
							$arrEixosSor2[$exi] = $eixo['exoid']."_".$eixo['exodsc']."_".$estados[$est]."_".$scroll[$est];
						}
						if($id == 'box1'){
							//BOX Personalizados
							$arrEixosSor2[$exi] = "box1_Mapa de Indicadores_".$estados[$est]."_".$scroll[$est];
						}
						$exi++;
						$est++;
					}
				}
				$parametros_tela = true;
				
			}if($tipo == "insert"){
				$sql = "select * from painel.eixo";
				$eixos = $db->carregar($sql);
				$eixos = $eixos ? $eixos : array();
				$exi = 1;
				foreach($eixos as $ex){
					$arrEixos[$exi] = $ex['exoid']."_".$ex['exodsc'];
					$exi++;
				}
				//BOX Personalizados
				$arrEixos[$exi] = "box1_Mapa de Indicadores_max_scrollfalse";
				$parametros_tela = false;
			}
	//Exibição Padrão
	if($parametros_tela == false){
		$key = 1;
		?>
		<div id="sort1" class="groupWrapper">
		<?
		while($key <= (count($arrEixos) / 2 )){
			$item = explode("_",$arrEixos[$key]);
			$eixoid = $item[0];
			$eixodsc =  $item[1];
			$estado_eixoid =  $item[2];
			$scroll_eixoid =  $item[3];
			montaBox($arrEixos,$eixoid,$eixodsc,$estado_eixoid,$scroll_eixoid);					
			$key++;
		}
		?>
		</div>
		<div id="sort2" class="groupWrapper">
		<?
		while($key <= (count($arrEixos))){
			$item = explode("_",$arrEixos[$key]);
			$eixoid = $item[0];
			$eixodsc =  $item[1];
			$estado_eixoid =  $item[2];
			$scroll_eixoid =  $item[3];
			montaBox($arrEixos,$eixoid,$eixodsc,$estado_eixoid,$scroll_eixoid);	
			$key++;
		}
	}
	
	//Exibição Personalizada
	if($parametros_tela == true){
		$key = 1;
		?>
		<div id="sort1" class="groupWrapper">
		<?
		foreach($arrEixosSor1 as $key => $ex){
			$item = explode("_",$arrEixosSor1[$key]);
			$eixoid = $item[0];
			$eixodsc =  $item[1];
			$estado_eixoid =  $item[2];
			$scroll_eixoid =  $item[3];
			montaBox($arrEixos,$eixoid,$eixodsc,$estado_eixoid,$scroll_eixoid);	
			$key++;
		}
		?>
		</div>
		<div id="sort2" class="groupWrapper">
		<?
		foreach($arrEixosSor2 as $key => $ex){
		$item = explode("_",$arrEixosSor2[$key]);
			$eixoid = $item[0];
			$eixodsc =  $item[1];
			$estado_eixoid =  $item[2];
			$scroll_eixoid =  $item[3];
			montaBox($arrEixos,$eixoid,$eixodsc,$estado_eixoid,$scroll_eixoid);	
			$key++;
		}
	}
	?>
	</div>
	</div>
	<!-- fim da div de conteudo flutuante  -->
	</td>
</tr>
<tr>
<td colspan="4" id="botoes_acao" style="text-align:right;padding-right: 35px;"><input type="button" onclick="window.location.href='painel.php?modulo=inicio&acao=C'"  value="Voltar" /> <input onclick="salvaColunas();" type="button" value="Salvar" /> <a id="link_comparar"  href="#" rel="lyteframe" rev="width: 800px; height: 450px; scrolling: auto;"><input id="botao_comparar" style="display:none" type="button" onclick="limpaComparacao();" value="Comparar" /></a></td>
</tr>
</table>
</form>
<div id="erro" ></div>
<script type="text/javascript">
		<!--

		d = new dTree('d');

		d.config.folderLinks = true;
		d.config.useIcons = true;
		d.config.useCookies = true; 
		
		//alert(screen.width);
		var texto = "";

		d.add(0,-1,'<b>Eixos</b>');
		<?php
		
		$sql = "select * from painel.eixo order by exodsc";
		$eixos = $db->carregar($sql);
		$eixos = $eixos ? $eixos : array();
		
		foreach ( $eixos as $eixo ) :?>
	
			//formação do eixo
		    texto = '<a style="cursor:pointer"  onmouseover="this; return escape(\'<?=$eixo["exodsc"]?>\');"><b><?=strlen($eixo["exodsc"]) > 25 ? substr($eixo["exodsc"],0,25)."..." : $eixo["exodsc"]?></b></a>';
			//imprime eixo
			d.add(<?=$eixo["exoid"]?>,<?=$estrutura["exodsc"] ? $estrutura["exodsc"] : 0;?>,' '+texto,'','','','../imagens/eixos-mini2.png','../imagens/eixos-mini2.png');
		    <?
		    //Nível 2
		    $sql = "select distinct
						aca.acadsc,
						aca.acaid
					from
						painel.acao aca
					inner join
						painel.indicador ind ON ind.acaid = aca.acaid
					where
						ind.exoid = {$eixo["exoid"]}
					and
						ind.indpublicado is true
					and
						aca.acastatus = 'A'
					order by
						aca.acadsc";
			$acao = $db->carregar($sql);
			
			!$acao? $acao = array() : $acao = $acao;
			
			foreach($acao as $ac):
				?>
				//formação da ação
			    texto = '<a style="cursor:pointer"  onmouseover="this; return escape(\'<?=$ac["acadsc"]?>\');"><?=strlen($ac["acadsc"]) > 25 ? substr($ac["acadsc"],0,25)."..." : $ac["acadsc"]?></a>';
				//imprime o programa
				d.add(999<?=$ac["acaid"]?>,<?=$eixo["exoid"]?>,' '+texto,'','','','../includes/dtree/img/folder.gif','');
				<?
				//Nível 3
				$sql = "select distinct
							ind.indnome,
							ind.indid
						from
							painel.acao aca
						inner join
							painel.indicador ind ON ind.acaid = aca.acaid
						where
							ind.exoid = {$eixo['exoid']}
						and
							ind.indpublicado is true
						and
							aca.acastatus = 'A'
						and
							aca.acaid = {$ac['acaid']}
						and 
							ind.indstatus = 'A'
						order by
							ind.indnome";
				$indicador = $db->carregar($sql);
				!$indicador? $indicador = array() : $indicador = $indicador; 
					foreach($indicador as $ind):

					//Verifica se o Indicador possui Série Histórica
						$sql = "select
									coalesce(sehqtde,0) as sehqtde
								from
									painel.seriehistorica
								where
									indid = {$ind['indid']}";
					
						$sehqtde = $db->pegaUm($sql);
						if($sehqtde){
							$img = "../imagens/odometro.png";
							$link = '<a rev="width: 800px; height: 450px; scrolling: auto;" rel="lyteframe" href="painel.php?modulo=principal/serie_historica&amp;acao=A&amp;acaid='.$ac['acaid'].'&amp;indid='.$ind["indid"].'">'.(strlen($ind["indnome"]) > 25 ? substr($ind["indnome"],0,25)."..." : $ind["indnome"]).'</a>';
						}else{
							$img = "../imagens/odometro_01.png";
							$link = "<a style=\"cursor:pointer;\" onclick=\"alert(\'Série Histórica Não Atribuída.\')\" >".(strlen($ind["indnome"]) > 25 ? substr($ind["indnome"],0,25)."..." : $ind["indnome"])."</a>";
						}
						?>
						//formação do indicador
					    texto = '<span style="cursor:pointer"  onmouseover="this; return escape(\'<?=$ind["indnome"]?>\');"><i><?=$link?></i></span>';
						//imprime o programa
						d.add(99999<?=$ind["indid"]?>,999<?=$ac["acaid"]?>,' '+texto,'','','','<?=$img;?>','');
						<?
					endforeach;
			endforeach;
		endforeach; ?>
		elemento = document.getElementById( '_arvore' );
		elemento.innerHTML = d;

		//-->
</script>
<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
<script type="text/javascript" src="/includes/JQuery/interface.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(
	function () {
		jQuery('a.closeEl').bind('click', toggleContent);
		jQuery('div.groupWrapper').Sortable(
			{
				accept: 'groupItem',
				helperclass: 'sortHelper',
				activeclass : 	'sortableactive',
				hoverclass : 	'sortablehover',
				handle: 'div.itemHeader',
				tolerance: 'pointer',
				onChange : function(ser)
				{
				},
				onStart : function()
				{
					jQuery.iAutoscroller.start(this, document.getElementsByTagName('body'));
				},
				onStop : function()
				{
					jQuery.iAutoscroller.stop();
				}
			}
		);
	}
);
var toggleContent = function(e)
{
	var targetContent = jQuery('div.itemContent', this.parentNode.parentNode);
	if (targetContent.css('display') == 'none') {
		
		if(document.getElementById( 'autoscroll_' + this.id).checked == true){
			targetContent.css({'display' : ''});
			jQuery(this).html('<img src="../imagens/menos.gif" border=0 />');
			mundaEstado(this.id,'max');
			executarEditar(this.id,true);
		}
		if(document.getElementById( 'autoscroll_' + this.id).checked == false){
			targetContent.slideDown(300);
			jQuery(this).html('<img src="../imagens/menos.gif" border=0 />');
			mundaEstado(this.id,'max');
		}		
	} else {
		targetContent.slideUp(300);
		executarEditar(this.id,true);
		mundaEstado(this.id,'min');
		jQuery(this).html('<img src="../imagens/mais.gif" border=0 />');
	}
	return false;
};

function mundaEstado(estid,str){
	var id = estid;
	var estado = str;
	document.getElementById( 'estado_' + id ).value = estado; 
}

function mundaScroll(estid,str){
	var id = estid;
	var scroll = str;
	document.getElementById( 'scroll_' + id ).value = scroll; 
}


function salvaColunas(s){
			
			//Estado das CXs
		 	var estados = document.getElementsByTagName("input");
			var num = estados.length;
		 	var ckeckados = 0;
		 	var arrID = new Array();
		 	
		 	for(var i=0; i < num; i++){
				if(document.getElementsByTagName('input')[i].type == 'hidden'){
		        	if(document.getElementsByTagName('input')[i].value == 'max' || document.getElementsByTagName('input')[i].value == 'min'){
		        		arrID[ckeckados] = document.getElementsByTagName('input')[i].value;
		        		ckeckados ++;
		       		}
		       	}
		     }
		     
		     //Estado das CXs em Scroll
		 	var scroll = document.getElementsByTagName("input");
			var num2 = scroll.length;
		 	var ckeckados2 = 0;
		 	var arrID2 = new Array();
		 	
		 	for(var i=0; i < num2; i++){
				if(document.getElementsByTagName('input')[i].type == 'hidden'){
		        	if(document.getElementsByTagName('input')[i].value == 'scrolltrue' || document.getElementsByTagName('input')[i].value == 'scrollfalse'){
		        		arrID2[ckeckados2] = document.getElementsByTagName('input')[i].value;
		        		ckeckados2 ++;
		       		}
		       	}
		     }
		     
			
			var div = document.getElementById('erro'); 
			
			serial = jQuery.SortSerialize(s);
			// Faz uma requisição ajax, passando o parametro 'ordid', via POST
			var req = new Ajax.Request('painel.php?modulo=principal/eixostemas&acao=A', {
					        method:     'post',
					        parameters: '&salvaParametros=1&' + serial.hash + '&estados=' + arrID + '&scroll=' + arrID2,
					        onComplete: function (res)
					        {	
								alert('Operação Realizada com Sucesso!');
								div.innerHTML = res.responseText;
					        }
					  });
		}
		
		function carregaMapa(){
			var req = new Ajax.Request('painel.php?modulo=principal/eixostemas&acao=A', {
					        method:     'post',
					        parameters: '&AjaxCarregaMapa=1&',
					        onComplete: function (res)
					        {	
								montaMapa();
								disponibilizaMapa();
					        }
					  });
		}
</script>
<script type="text/javascript">/* carregaMapa() */</script>