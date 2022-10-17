<?php

//header("Cache-Control: no-cache, must-revalidate");
//header("Expires: Sat, 26 Jul 1997 05:00:00 GMT");
/*
 * Funções do cadastro de atributo
*/


/*
 * Legenda das Sessões
 * 
 * $_SESSION['insereOpcao']      => Esta sessão guarda array de registro cadastrados
 * $_SESSION['insereOpcaoBanco'] => Nesta sessão fica armazenado todos os registro que serão cadastrados no banco
 * 
*/

//carrega dados para a tabela opções do atributo

function carregaOpcoesAtributoAjax($atrid){
	global $db;
	
	if(!$_SESSION['insereOpcao']){
		$_SESSION['insereOpcao'] =  array();
	}else{
		unset($_SESSION['insereOpcao']);
	}
	
	if(!$_SESSION['insereOpcaoFalse']){
		$_SESSION['insereOpcaoFalse'] = array();
	}else{
		unset($_SESSION['insereOpcaoFalse']);
	}
	
	$sql = "SELECT opaid, atrid, opadescricao, opavalor,
				   opavalorrelacionado, opaordem, opastatus
  			  FROM formulario.opcoesatributo
			WHERE atrid = $atrid";
	
	$array = $db->carregar($sql);
	echo $sql;
	if($array){
		
		foreach($array as $key => $value){
			
			if($value['opastatus'] == "t") {
			
				$registro = Array("opaid" => $value['opaid'],
					         "atrid" => $value['atrid'],
					         "opadescricao" => $value['opadescricao'],
					         "opavalor" => $value['opavalor'],
					         "opavalorrelacionado" => $value['opavalorrelacionado'],
							 "ordem" => $value['opaordem']
						 );
				
				array_push($_SESSION['insereOpcao'], $registro);
			}else{
				$registro = Array("opaid" => $value['opaid'],
					         "atrid" => $value['atrid'],
					         "opadescricao" => $value['opadescricao'],
					         "opavalor" => $value['opavalor'],
					         "opavalorrelacionado" => $value['opavalorrelacionado'],
							 "ordem" => $value['opaordem']
						 );
				
				array_push($_SESSION['insereOpcaoFalse'], $registro);
			}
		}
		echo "true";
	}else{
		echo "false";
	}	
	//$_SESSION['insereOpcaoBanco'] = $_SESSION['insereOpcao'];
	
}

function listaArrayOpcoesAjax(){
		
	if( !empty($_SESSION['insereOpcao']) ){
		$arDados = array();
		$arDados = $_SESSION['insereOpcao'];
		unset($_SESSION['insereOpcao']);
	}else{
		$arDados = $_SESSION['insereOpcaoBanco'];
	}

	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<table id="opaAtributo"  width=100% class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="10%" valign="top" class="title">
						Opções
					</td>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="40%" valign="top" class="title">
						Texto
					</td>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="25%" valign="top" class="title">
						Valor
					</td>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="20%" valign="top" class="title">
						Valor Relacionado
					</td>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" valign="top" class="title">
						Ordem
					</td>
				</tr>
				<?php
				   
		if($arDados){

			    function cmp($a, $b){  
			       
			        if ($a["ordem"] == $b["ordem"]) {
			            return 0;
			        }
			        return ($a["ordem"] < $b["ordem"]) ? -1 : 1;
			       
			    }
			    //ordenar dados do array pelo campo ordem
			    usort($arDados, "cmp");
			    
			    //$_SESSION['insereOpcao'] = $arDados;
			    
			    if( empty($_SESSION['insereOpcaoBanco']) ){
			    	$_SESSION['insereOpcaoBanco'] = array();
			    }else{
			    	unset($_SESSION['insereOpcaoBanco']);
			    }
				
				$_SESSION['insereOpcaoBanco'] = $arDados;
				
				//ver($_SESSION['insereOpcaoBanco']);
							
				$arDados = ($arDados) ? $arDados : array();				
				$codigo = 0;

				foreach($arDados as $chave => $detInd):
					
					$codigo % 2 ? $cor = "#fcfcfc" : $cor = "";
					?>
					<tr id="tr_<?php echo $detInd['opaid'];?>" bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td align="center"><img src="/imagens/alterar.gif" style="cursor: pointer;" onclick="visualizaOpcao('<?php echo $chave ?>','<?php echo $detInd['opaid'];?>')" border=0 alt="Ir" title="Alterar">
										   <img src="/imagens/excluir.gif" style="cursor: pointer;" onclick="excluiOpcaoAtributo('<?php echo $chave ?>','<?php echo $detInd['opaid'];?>')" border=0 alt="Ir" title="Excluir"></td>
						<td><input type="hidden" name="opadescricao_<?php echo $codigo?>" id="opadescricao_<?php echo $codigo?>" value="<?php echo $detInd["opadescricao"] ?>"><?php echo $detInd["opadescricao"] ?></td>
						<td><input type="hidden" name="opavalor_<?php echo $codigo?>" id="opavalor_<?php echo $codigo?>" value="<?php echo $detInd["opavalor"] ?>"><?php echo $detInd["opavalor"] ?></td>
						<td><input type="hidden" name="opavalorrelacionado_<?php echo $codigo?>" id="opavalorrelacionado_<?php echo $codigo?>" value="<?php echo $detInd["opavalorrelacionado"] ?>"><?php echo $detInd["opavalorrelacionado"] ?></td>
						<td><input type="hidden" name="ordem_<?php echo $detInd["ordem"];?>" value="<?php echo $detInd["ordem"];?>" >
							<?if(count($arDados) == ($codigo + 1)){ ?>
								<img src="../imagens/seta_baixod.gif" />
							<?}else{ ?>
								<img onclick="mudaPosicaoArray('opaAtributo', 'baixo', <?php echo $detInd["ordem"]; ?>);" style="cursor: pointer;" src="../imagens/seta_baixo.gif" />
							<?}
							
							if($codigo == 0){?>
								<img src="../imagens/seta_cimad.gif" />
							<?}else {?>
								<img onclick="mudaPosicaoArray('opaAtributo', 'cima', <?php echo $detInd["ordem"]; ?>);" style="cursor: pointer;" src="../imagens/seta_cima.gif" />	
							<?} ?> 
							</td>
					</tr>
					<?php
					$codigo++;
				endforeach;
				
				?>
				</table>
			</td>
		</tr>
	</table><?
	}
}

function insereOpcoesArray($opaid, $opaidA, $atrid, $opavalor, $opadescricao, $opavalorrelacionado, $opaordem){
	
	if($_SESSION['insereOpcaoBanco']){
		$_SESSION['insereOpcao'] =  array();
		$_SESSION['insereOpcao'] = $_SESSION['insereOpcaoBanco'];
	}else{
		$_SESSION['insereOpcao'] =  array();
	}
	
	$insere = "true";
	
	if($opaid == ""){
		$ordem = 0;
		$i = 0;

		
		//este for verifica se já existe registro cadastrado no array
		foreach ($_SESSION['insereOpcao'] as $id => $reg) {
			if( ($reg["opadescricao"] == $opadescricao) || ($reg["opavalor"] == $opavalor) ){
				$insere = "false";
				break;
			}else{
				$insere = "true";
			}
		}
		
		$ordem = 0;
		$texto = "";
		$valor = "";
		$pes = "0";
		
		$texto = array();
		$valor = array();
		
		foreach($_SESSION['insereOpcaoFalse'] as $value ){
			if( ($value['opadescricao'] == $opadescricao) || ($value['opavalor'] == $opavalor) ){
				echo "Não é possível salvar as opção de atributo pois esta opção já existe na base de dados!
Este registro não aparece na listagem da tela porque está inativo.
Para regularizar a situação do registro, entre em contato com os desenvolvedores do sistema";
				exit();
			}
		}
	
		$arOrdem = array();
		foreach( $_SESSION['insereOpcao'] as $arDados ){
			$arOrdem[] = $arDados["ordem"];
		}
		
		$boExisteValor = true;
		while( $boExisteValor ){
			if( !in_array($i, $arOrdem ) ) {
				$ordem = $i;
				$boExisteValor = false;
			}
			$i++;
		}
		
		if($insere == "true"){
			$dados = Array("opaid" => $opaid,
					         "atrid" => $atrid,
					         "opadescricao" => $opadescricao,
					         "opavalor" => $opavalor,
					         "opavalorrelacionado" => $opavalorrelacionado,
							 "ordem" => $ordem
						 );
		    array_push($_SESSION['insereOpcao'],$dados);
		    
		    echo "Operação realizada com sucesso!";
		}else{
			echo "Não é possível salvar a opção de Valor '".$opavalor."' pois esta opção já existe na base de dados!";
		}
	    
	    
	}else{

		unset($_SESSION['insereOpcao'][$opaid]);
		
		$dados = Array("opaid" => $opaidA,
				         "atrid" => $atrid,
				         "opadescricao" => $opadescricao,
				         "opavalor" => $opavalor,
				         "opavalorrelacionado" => $opavalorrelacionado,
						 "ordem" => $opaordem
					 );
	    array_push($_SESSION['insereOpcao'],$dados);
	    
	    echo "Operação realizada com sucesso!";
	}
	$_SESSION['insereOpcaoBanco'] = $_SESSION['insereOpcao'];
}

function recuperaValorSessionAjax($id){
	$arrayDados = $_SESSION['insereOpcaoBanco'][$id];
	print_r($arrayDados["opaid"]. "|" . $arrayDados["atrid"]. "|" . $arrayDados["opadescricao"]. "|" . $arrayDados["opavalor"] . "|" . $arrayDados["opavalorrelacionado"] . "|". $arrayDados["ordem"]);
}

function mudaPosicaoArrayAjax($atributo, $movimento, $posicao){
	
	$array = $_SESSION['insereOpcaoBanco'];
	
	$arOrdem = array();
	foreach( $array as $Dados ){
		$arOrdem[] = $Dados["ordem"];
	}

	$end = end($arOrdem);

	if($movimento == "cima"){
		if(count($arOrdem) < 3 ){
			$atual = prev($arOrdem);
			$ordem = next($arOrdem);
		}else{
			foreach($arOrdem as $key => $value) {
				if($value == $posicao) {

					if( $posicao == $end ){
						//$proximo = prev($arOrdem);
						$atual = $end;
						$ordem = $end - 1;//prev($arOrdem);
					}else{
						$proximo = $value + 1;
						$atual = $value;
						$ordem = $value - 1;
					}
				}
			}
		}
	}else{
		if(count($arOrdem) < 3 ){
				$atual = prev($arOrdem);
				$ordem = next($arOrdem);
		}else{
			foreach($arOrdem as $key => $value) {
			
				if($value == $posicao) {
					
					$anterior  = $value - 1;
					$atual = $value;
					$ordem  = $value + 1;
				}
			}
		}
	}
	
	foreach ($array as $chave => $value){
		if($value["ordem"] == $atual)	{
			$opaid		 = $value['opaid'];
			$atrid		 = $value['atrid'];
			$descricao 	 = $value["opadescricao"];
			$valor		 = $value["opavalor"];
			$relacionado = $value["opavalorrelacionado"];
			$codigo		 = $chave;
		}
		if($value["ordem"] == $ordem){
			$opaid1		  = $value['opaid'];
			$atrid1		  = $value['atrid'];
			$descricao1	  = $value["opadescricao"];
			$valor1		  = $value["opavalor"];
			$relacionado1 = $value["opavalorrelacionado"];
			$codigo1      = $chave;
		}
	}
	
	if($movimento == "cima"){

		unset($_SESSION['insereOpcaoBanco'][$codigo]);
		unset($_SESSION['insereOpcaoBanco'][$codigo1]);
		
		$registro = Array("opaid" => $opaid1,
				         "atrid" => $atrid1,
				         "opadescricao" => $descricao1,
				         "opavalor" => $valor1,
				         "opavalorrelacionado" => $relacionado1,
						 "ordem" => $atual
					 );

		if( !is_null($codigo) && !is_null($codigo1) ){ //esta codição evita a inserção de registro na session em branco
	    	array_push($_SESSION['insereOpcaoBanco'],$registro);
		}
	    
	    $registro1 = Array("opaid" => $opaid,
				         "atrid" => $atrid,
				         "opadescricao" => $descricao,
				         "opavalor" => $valor,
				         "opavalorrelacionado" => $relacionado,
						 "ordem" => $ordem
					 );
					 
		if( !is_null($codigo) && !is_null($codigo1) ){ //esta codição evita a inserção de registro na session em branco
	    	array_push($_SESSION['insereOpcaoBanco'],$registro1);
		}
	}else{
		unset($_SESSION['insereOpcaoBanco'][$codigo]);
		unset($_SESSION['insereOpcaoBanco'][$codigo1]);

		$registro = Array("opaid" => $opaid1,
				         "atrid" => $atrid1,
				         "opadescricao" => $descricao1,
				         "opavalor" => $valor1,
				         "opavalorrelacionado" => $relacionado1,
						 "ordem" => $atual
					 );
					 
		if( !is_null($codigo) && !is_null($codigo1) ){ //esta codição evita a inserção de registro na session em branco
	    	array_push($_SESSION['insereOpcaoBanco'],$registro);
		}
	    
	    $registro1 = Array("opaid" => $opaid,
				         "atrid" => $atrid,
				         "opadescricao" => $descricao,
				         "opavalor" => $valor,
				         "opavalorrelacionado" => $relacionado,
						 "ordem" => $ordem
					 );
					 
		if( !is_null($codigo) && !is_null($codigo1) ){ //esta codição evita a inserção de registro na session em branco
	    	array_push($_SESSION['insereOpcaoBanco'],$registro1);
		}
	}
}

function verificaOpcoesAtributoAjax(){
	if( !empty($_SESSION['insereOpcaoBanco']) ){
		echo "1";
	}
}

function limpaSessionDadosAjax(){
	unset($_SESSION['insereOpcao']);
	unset($_SESSION['insereOpcaoBanco']);
	unset($_SESSION['insereOpcaoFalse']);
	unset($_SESSION['excluiOpcao']);
}

//verifica se atributo é uma combo ou uma lista
function VerificaAtributoAjax($valor){
	global $db;
	
	$sql = "SELECT tiaid
			 FROM formulario.tipoatributo
			WHERE tiaopcoes = true
			 AND tiaid = $valor";
	
	$res = $db->pegaUm($sql);
	if($res != "")
		echo "true";
	else
		echo "false";	
}

function insereAtributoAjax($request){
	global $db;
	
	//ver($_SESSION['insereOpcaoBanco'], d);
	
	$atrid 			  = $request['atrid'];
	$atrnome 		  = $request['atrnome'];
	$atrdescricao 	  = $request['atrdescricao']; 
	$atrtipodados 	  = $request['atrtipodados'];
	$tiaid 			  = $request['tiaid'];
	$atrtamanhomax 	  = $request['atrtamanhomax'];
	$atrcasasdecimais = $request['atrcasasdecimais']; 
	$atrmascara 	  = $request['atrmascara'];
	$atridrelacionado = $request['atridrelacionado']; 
	$ratid 			  = $request['ratid'];
	$atrsqlopcoes 	  = $request['atrsqlopcoes'];

	if(validaMascara($atrtipodados, $atrmascara) == "true"){
	
		if($atrid == "0"){
			$sql = "SELECT atrid FROM formulario.atributo
					WHERE atrnome = '$atrnome'";
		
			$res = $db->pegaUm($sql);
			
			if(empty($res)){
				
				$stSql = strtolower( $atrsqlopcoes );
				$stSql = str_replace("{valorvinculado}", "\'".$atridrelacionado."\'", $stSql);
			
				$sql = sprintf("INSERT INTO formulario.atributo(atrnome, atrdescricao, atrtipodado, 
									tiaid, atrtamanhomax, atrcasasdecimais, atrmascara, atridrelacionado,
									ratid, atrsqlopcoes, atrstatus)
								VALUES('%s', '%s', '%s', %s, %s, %s, '%s', %s, %s, '%s', '1') RETURNING atrid;", 
							($atrnome ? iconv( "UTF-8", "ISO-8859-1",  $atrnome ) : ''), 
							($atrdescricao ? iconv( "UTF-8", "ISO-8859-1",  $atrdescricao ) : ''),
							($atrtipodados ? $atrtipodados : ''),
							($tiaid ? $tiaid : 'NULL'),
							($atrtamanhomax ? $atrtamanhomax : 'NULL'),
							($atrcasasdecimais ? $atrcasasdecimais : 'NULL'),
							($atrmascara ? $atrmascara : ''),
							($atridrelacionado ? $atridrelacionado : 'NULL'),
							($ratid ? $ratid : 'NULL'),
							($stSql ? $stSql : '')
							);
				
				$atrid = $db->pegaUm($sql);
				$res = $db->commit();
				
				if($res == "1"){
					if($_SESSION['insereOpcaoBanco']){
						insereOpcoesAjax($atrid, $atrtipodados, $atrmascara);
					}
					echo $atrid;
				}else
					echo "false";
			}else{
				echo "Não é possível salvar o registro, pois já existe na base de dados!";
			}
		}else{
			$stSql = strtolower( $atrsqlopcoes );
			$stSql = str_replace("{valorvinculado}", "\'".$atridrelacionado."\'", $stSql);
				
			$sql = sprintf("UPDATE formulario.atributo SET atrnome = '%s', atrdescricao = '%s', atrtipodado = '%s', 
								tiaid = %s, atrtamanhomax = %s, atrcasasdecimais = %s, atrmascara = '%s', atridrelacionado = %s,
								ratid = %s, atrsqlopcoes = '%s'
							WHERE atrid = $atrid",
						($atrnome ? iconv( "UTF-8", "ISO-8859-1", $atrnome ) : ''), 
						($atrdescricao ? iconv( "UTF-8", "ISO-8859-1", $atrdescricao ) : ''),
						($atrtipodados ? $atrtipodados : ''),
						($tiaid ? $tiaid : 'NULL'),
						($atrtamanhomax ? $atrtamanhomax : 'NULL'),
						($atrcasasdecimais ? $atrcasasdecimais : 'NULL'),
						($atrmascara ? $atrmascara : ''),
						($atridrelacionado ? $atridrelacionado : 'NULL'),
						($ratid ? $ratid : 'NULL'),
						($stSql ? $stSql : '')
						);
			
			//echo $sql;
			
			$db->executar($sql);
			$res = $db->commit();
			
			if($res == "1"){
				if($_SESSION['insereOpcaoBanco']){
					insereOpcoesAjax($atrid, $atrtipodados, $atrmascara);
				}
				echo "true";
			}else{
				echo "false";
			}
		}
	}//fim da validação
}

function validaMascara($atrtipodados, $atrmascara){
	$registro = array();
	$registro = $_SESSION['insereOpcaoBanco'];
	
	foreach ($registro as $key => $value) {
		$valor = $value['opavalor'];
		$valor = str_replace(".", "", $valor);
		$valor = str_replace(",", "", $valor);
		
		if(!$atrtipodados){
			echo "É necessário informar o tipo de dados!";
			break;
			exit();
		}else{
			if($atrtipodados == "N"){
				if(!is_numeric($valor)){
					echo "O campo valor deve ser preenchido com valor tipo numerico!";
					return false;
					break;
					exit();
				}
			}elseif($atrtipodados == "B"){
				if( !is_bool($valor) ){
					echo "O campo valor deve ser preenchido com valor tipo boleano!";
					return false;
					break;
					exit();
				}
			}elseif($atrtipodados == "D"){
				if( (count($valor) > 7) && (count($valor) < 11) ){
					$arData = explode("/", $valor);

					if( checkdate($arData['1'], $arData['0'], $arData['2']) ){
						echo "O campo valor deve ser preenchido com valor tipo date!";
						return false;
						break;
						exit();
					}	
				}else{
					echo "O valor está com o formato da data invalido";
					return false;
					break;					
					exit();					
				}
			}
		}
		
		if($atrmascara){
			if( !validaMascaraAjax($value['opavalor'], $atrmascara) ){
				echo "O valor informado não está no padrão da máscara digitada!";
				return false;
				break;
				exit();
			}
		}
	}
	return true;
}

function insereOpcoesAjax($atrid, $atrtipodados, $atrmascara){
	//header('content-type: text/html; charset=ISO-8859-1');
	global $db;
	
	$reg = array();
	
	if($_SESSION['excluiOpcao']){
		$arDados = array();
		$arDados = $_SESSION['excluiOpcao'];
		foreach ($arDados as $key => $value) {
			$opaidE = $value['opaid'];
			
			if($opaidE){
				$sql = "UPDATE formulario.opcoesatributo SET opastatus = false
						WHERE opaid = ". $opaidE;
			
				$db->executar($sql);
				$res = $db->commit();
			}
			
		}
		unset($_SESSION['excluiOpcao']);		
	}
	
	$reg = $_SESSION['insereOpcaoBanco'];
		
		
	$sql = "SELECT opaid FROM formulario.opcoesatributo
			WHERE atrid = $atrid";
			
	$array = $db->carregar($sql);	
	
	if($array){
		
		foreach ($reg as $key => $value) {
			$opaid = $value['opaid'];
			
			if($opaid){
				$sql = sprintf("UPDATE formulario.opcoesatributo
						   SET opavalor='%s', opadescricao='%s', opaordem='%s', opavalorrelacionado=%s
						WHERE atrid = $atrid
						  AND opaid = $opaid",
						($value["opavalor"] ?  $value["opavalor"] : ''),
						($value["opadescricao"] ? $value["opadescricao"] : ''),
						$value["ordem"],
						($value["opavalorrelacionado"] ? $value["opavalorrelacionado"] : 'NULL')
						);
				
				$db->executar($sql);
				$res = $db->commit();
			}else{
				$sql = sprintf("INSERT INTO formulario.opcoesatributo(atrid, opavalor, opadescricao, 
								opavalorrelacionado, opaordem)
							VALUES(%s, '%s', '%s', %s, '%s') RETURNING opaid;", 
						($atrid ? $atrid : 'NULL'), 
						($value["opavalor"] ? iconv( "UTF-8", "ISO-8859-1", $value["opavalor"] ) : ''),
						($value["opadescricao"] ? iconv( "UTF-8", "ISO-8859-1", $value["opadescricao"] ) : ''),
						($value["opavalorrelacionado"] ? $value["opavalorrelacionado"] : 'NULL'),
						$value["ordem"]
						);
						
				$opaid = $db->pegaUm($sql);
				$res = $db->commit();
			}
		}		
	}else{
		
		foreach ($reg as $key => $value) {
		
			$sql = sprintf("INSERT INTO formulario.opcoesatributo(atrid, opavalor, opadescricao, 
								opavalorrelacionado, opaordem)
							VALUES(%s, '%s', '%s', %s, '%s') RETURNING opaid;", 
						($atrid ? $atrid : 'NULL'), 
						($value["opavalor"] ? iconv( "UTF-8", "ISO-8859-1", $value["opavalor"] ) : ''),
						($value["opadescricao"] ? iconv( "UTF-8", "ISO-8859-1", $value["opadescricao"] ) : ''),
						($value["opavalorrelacionado"] ? $value["opavalorrelacionado"] : 'NULL'),
						$value["ordem"]
						);
			//echo $sql."<br/>";
			$opaid = $db->pegaUm($sql);
			$res = $db->commit();
		}
		
		if($res == "1")
			echo $opaid;
		else
			echo "false";
	}
}

function excluiOpcaoAtributoAjax($id, $opaid){
	global $db;
	$msg = "";
	$arDados = array();
	
	if( empty($_SESSION['excluiOpcao']) ){
		$_SESSION['excluiOpcao'] = array();
		$registro = array();
	}
	
	if($id != ""){
		
		$arDados = $_SESSION['insereOpcaoBanco'];
		
		foreach ($arDados as $key => $value) {
			
			if($id == $key){	

				$registro = Array("opaid" => $value['opaid'],
					         "atrid" => $value['atrid'],
					         "opadescricao" => $value['opadescricao'],
					         "opavalor" => $value['opavalor'],
					         "opavalorrelacionado" => $value['opavalorrelacionado'],
							 "ordem" => $value['opaordem']
						 );
				
				array_push($_SESSION['excluiOpcao'], $registro);
	    		unset($_SESSION['insereOpcaoBanco'][$id]);		
			}
		}		
		$msg = "Operação realizada com sucesso!";
	}
} 

function geraComboOpcoes($atrid){
	global $db;
	$array = Array();
	
	if($atrid){
		$sql = "SELECT opaid as Codigo, opavalor as Descricao 
				   FROM formulario.opcoesatributo
				WHERE atrid = $atrid 
				and opastatus = true 
				order by opavalor";
		
		$dados = $db->carregar($sql);
		
		if($dados){
			$db->monta_combo("opavalorrelacionado", $sql, 'S','-- Selecione um relacionamento --','', '', '',250,'N','opavalorrelacionado');?>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			<input type="hidden" value="sim" id="obrigatorio" name="obrigatorio"><?php
		}else{
			$sql = "SELECT atrid, atrnome, atrdescricao, atrtipodado, tiaid, atrtamanhomax, 
					       atrcasasdecimais, atrmascara, atridrelacionado, ratid, atrsqlopcoes, 
					       atrstatus
					  FROM formulario.atributo
					WHERE atrid = $atrid";
			
			$dados = $db->pegaLinha($sql);

			if($dados && $dados['atrsqlopcoes']){
				$db->monta_combo("opavalorrelacionado", $dados['atrsqlopcoes'], 'S','-- Selecione um relacionamento --','', '', '',250,'N','opavalorrelacionado');?>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
				<input type="hidden" value="sim" id="obrigatorio" name="obrigatorio"><?php
			}else{
				$db->monta_combo("opavalorrelacionado", $array, 'S','-- Selecione um relacionamento --','', '', '',250,'N','opavalorrelacionado');?>
				<input type="hidden" value="nao" id="obrigatorio" name="obrigatorio"><?php
			}			
		}

	}else{	
		$db->monta_combo("opavalorrelacionado", $array, 'S','-- Selecione um relacionamento --','', '', '',250,'N','opavalorrelacionado');?>
		<input type="hidden" value="nao" id="obrigatorio" name="obrigatorio"><?php
	}
}

function validaMascaraAjax($opavalor, $atrmascara){
	if($atrmascara){
		echo eregi($atrmascara, $opavalor);	
	}
}

/*
 * Funções da listagem de atributos
*/

function carregaAtributosAjax($registro_array){
	global $db;
	if(!$registro_array){
		$registro_array = Array();
	}
	$f = $registro_array;
	
	$sql = "SELECT distinct 
			( '<center><a href=\"seguranca.php?modulo=principal/cadAtributo&acao=A&atrid='|| a.atrid ||'\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
			      '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiAtributo('||a.atrid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ) as acao,
			 a.atrid, a.atrnome, a.atrdescricao, ta.tiacampo,
			 (CASE WHEN a.atrtipodado = 'C' THEN 'Caracter'
			       WHEN a.atrtipodado = 'N' THEN 'Número'
			       WHEN a.atrtipodado = 'D' THEN 'Data'
			       ELSE 'Boleano' END) as atrtipodado,
			 at.atrnome as atridrelacionado, 
			 '<center><img src=\"/imagens/consultar.gif \" border=0 alt=\"Ir\" title=\"Opções do Atributo \" onclick=\"verificaOpcao('||				
			   (SELECT opa.atrid FROM formulario.atributo atr
			      inner join formulario.opcoesatributo opa
			      ON (atr.atrid = opa.atrid)
			    WHERE opa.opastatus = true
			      AND atr.atrid = a.atrid
			    group by opa.atrid, atr.atrnome ) ||', '''||a.atrnome||''');\" style=\"cursor: pointer\"></center>' as opaid
			 FROM formulario.atributo a left JOIN formulario.atributo at 
			   ON (a.atridrelacionado = at.atrid) LEFT JOIN formulario.opcoesatributo oa
			   ON (a.atrid = oa.atrid) INNER JOIN  formulario.tipoatributo ta
			   ON (a.tiaid = ta.tiaid)
			WHERE a.atrstatus = true";
	
	if(!empty($f[0])){
		$sql.= " AND a.atrid = $f[0]"; 
	}
	if(!empty($f[1])){
		$sql.= " AND a.atrnome = '$f[1]'";
	}
	if(!empty($f[2])){
		$sql.= " AND a.atrtipodado = '$f[2]'";
	}
	if(!empty($f[3])){
		$sql.= " AND ta.tiaid = $f[3]";
	}
	if(!empty($f[4])){
		$sql.= " AND a.atridrelacionado = $f[4]";
	}
	if(!empty($f[5])){
		$sql.= " AND a.ratid = $f[5]";
	}
	if(!empty($f[6])){
		$sql.= " AND a.atrstatus = $f[6]";
	}

	$sql.= " ORDER BY a.atrnome";

	monta_titulo( '', 'Listagem de Atributos' );
	$cabecalho = array("Opções", "Identificador", "Nome", "Descrição", "Tipo de Dado", "Tipo de Atributo", "Vinculado A", "Opções do Atributo");
	
	$db->monta_lista($sql, $cabecalho, 8, 4, 'N','Center','');
	
}

function carregaTabelaOpcoes($atrid){
	global $db;
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<table id="opaAtributo"  width=100% class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="40%" valign="top" class="title">
						Texto
					</td>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="25%" valign="top" class="title">
						Valor
					</td>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="20%" valign="top" class="title">
						Valor Relacionado
					</td>
				</tr>
				<?php
				$sql = "SELECT 
							opaid, opadescricao, opavalor, opavalorrelacionado, opaordem
  						  FROM formulario.opcoesatributo
  						WHERE atrid = $atrid
  						AND opastatus = true
  						order by opaordem";
				
				$detalhes = $db->carregar($sql);
				
				!$detalhes? $detalhes = array() : $detalhes = $detalhes;
				
				foreach($detalhes as $chave => $detInd):

					$chave % 2 ? $cor = "#fcfcfc" : $cor = "";
					?>
					<tr id="tr_<?=$detInd['opaid'];?>" bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td><?=$detInd["opadescricao"] ?></td>
						<td><?=$detInd["opavalor"] ?></td>
						<td><?=$detInd["opavalorrelacionado"] ?></td>
					</tr>
					<?php
				
				endforeach;
				
				?>
				</table>
		</td>
	</tr>
</table>
	<?
	
}

function carregaAtributosAjaxPopUp($atrid, $atrnome){
	global $db;
	
	$sql = "SELECT distinct
			 ( '<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Alterar\" onclick=\"carregaAtributo('||a.atrid||')\">') as acao,
			 a.atrid, a.atrnome, a.atrdescricao, ta.tiacampo,
			 (CASE WHEN a.atrtipodado = 'C' THEN 'Caracter'
			       WHEN a.atrtipodado = 'N' THEN 'Número'
			       WHEN a.atrtipodado = 'D' THEN 'Data'
			       ELSE 'Boleano' END) as atrtipodado,
			 at.atrnome as atridrelacionado, 
			 '<center><img src=\"/imagens/consultar.gif \" border=0 alt=\"Ir\" title=\"Opções do Atributo \" onclick=\"verificaOpcao('||oa.atrid||','||a.atrid||', '''||a.atrnome||''');\" style=\"cursor: pointer\"></center>' as opaid
			 FROM formulario.atributo a left JOIN formulario.atributo at 
			   ON (a.atridrelacionado = at.atrid) LEFT JOIN formulario.opcoesatributo oa
			   ON (a.atrid = oa.atrid) INNER JOIN  formulario.tipoatributo ta
			   ON (a.tiaid = ta.tiaid)
			WHERE a.atrstatus = true";
	
	
	if(!empty($atrid)){
		$sql.= " AND a.atrid = $atrid"; 
	}
	if(!empty($atrnome)){
		$sql.= " AND a.atrnome = '$atrnome'";
	}
	
	$sql.= " ORDER BY a.atrnome";
	
	monta_titulo( '', 'Listagem de Atributos' );
	$cabexalho = array("Opções", "Identificador", "Nome", "Descrição", "Tipo de Dado", "Tipo de Atributo", "Vinculado A", "Opções do Atributo");
	
	$db->monta_lista($sql, $cabexalho, 8, 4, 'N','Center','');
	
}
function geraComboAtributoFormulario($atrid){
	global $db;
	
	$sql = "SELECT atrid as codigo, atrnome as descricao
					FROM formulario.atributo
				WHERE atrstatus = true";
	
	$dados = $db->carregar($sql);?>
	<select id="atrid" class="CampoEstilo" style="width: 250px;" name="atrid" onchange="montaCampoValorPadrao(this.value);">
	<option value="">-- Selecione um atributo --</option>
	<?php foreach ($dados as $key => $value){
		if($atrid == $value["codigo"]){?>
			<option selected="selected" value="<?php echo $value["codigo"]?>"><?php echo $value["descricao"]?></option><?php
		}?>
		<option value="<?php echo $value["codigo"]?>"><?php echo $value["descricao"]?></option>
	<?}?>
	</select><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/><?php
}

function excluirAtributoAjax($atrid){
	global $db;
	$sql = "UPDATE formulario.atributo SET atrstatus = false
			WHERE atrid = $atrid";

	$db->executar($sql);
	$res = $db->commit();
		
	if($res == "1")
		echo "Operação realizada com sucesso!";
	else
		echo "Operação não realizada!";
}

/*
 * 
 * mater dados do formulário
 * 
*/

function insereFormularioAjax($forid, $fornome, $fordetalhamento){
	global $db;
	
	if($forid == "0"){
		$sql = "SELECT forid 
				  FROM formulario.formulario
				WHERE fornome = '$fornome'
				AND forstatus = false";

		$res = $db->pegaUm($sql);
		
		if(!empty($res)){
			echo "Não é possível salvar as opção de atributo pois esta opção já existe na base de dados!
Este registro não aparece na listagem da tela porque está inativo.
Para regularizar a situação do registro, entre em contato com os desenvolvedores do sistema";
		}else{
		
			$sql = "SELECT forid 
					  FROM formulario.formulario
					WHERE fornome = '$fornome'
					AND forstatus = true";
			
			$res = $db->pegaUm($sql);
					
			if(empty($res)){
				$sql = sprintf("INSERT INTO formulario.formulario(fornome, fordetalhamento, forstatus)
								  VALUES('%s', '%s', '1') RETURNING forid;",
								($fornome ? iconv( "UTF-8", "ISO-8859-1",  $fornome ) : ''), 
								($fordetalhamento ? iconv( "UTF-8", "ISO-8859-1",  $fordetalhamento ) : '')
							  );
			
				$forid = $db->pegaUm($sql);
				$res = $db->commit();
				
				if($res == "1"){
					echo $forid;
					$_SESSION['forid']   = $forid;
					$_SESSION['fornome'] = $fornome;
				}else
					echo "false";
			}else{
				echo "Não é possível salvar o registro, pois já existe na base de dados!";
			}
		}
	}else{
		$sql = sprintf("UPDATE formulario.formulario set fornome = '%s', fordetalhamento = '%s'
						 WHERE forid = $forid",
						($fornome ? iconv( "UTF-8", "ISO-8859-1",  $fornome ) : ''), 
						($fordetalhamento ? iconv( "UTF-8", "ISO-8859-1",  $fordetalhamento ) : '')
						);
		$db->executar($sql);
		$res = $db->commit();
			
		if($res == "1"){
			echo "Operação realizada com sucesso!";
		}else{
			echo "false";
		}
	}
}

function listarFormularioAjax($forid, $fornome, $fordetalhamento){
	global $db;
	
	$sql = "SELECT
				'<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarFormulario('||forid||');\" border=0 alt=\"Ir\" title=\"Alterar\"> </a>'|| 
			 		    '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiFormulario('||forid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' as acao, 
				forid, fornome, fordetalhamento 
			  FROM formulario.formulario
			WHERE forstatus = true";
	
	if($forid != ""){
		$sql.= " AND forid = $forid";
	}
	if($fornome != ""){
		$sql.= " AND fornome = $fornome";
	}
	if($fordetalhamento != ""){
		$sql.= " AND fordetalhamento = $fordetalhamento";
	}
	$sql.= " ORDER by fornome";
	monta_titulo( '', 'Listagem de Formulário' );
	$cabecalho = array("Opções", "Identificador", "Nome", "Detalhamento");
 
	$db->monta_lista($sql, $cabecalho, 8, 4, 'N','Center','');
}

function excluiFormularioAjax($forid){
	global $db;
	$sql = "UPDATE formulario.formulario SET forstatus = false
			WHERE forid = $forid";

	$db->executar($sql);
	$res = $db->commit();
		
	if($res == "1")
		echo "Operação realizada com sucesso!";
	else
		echo "Operação não realizada!";
}

/*
 * 
 * vincular dados do atributo formulario
 *
*/

function geraComboGrupoAtributoAjax($forid){
	global $db;

	$sql = "SELECT gafid as codigo, gafnome as descricao
				FROM formulario.grupoatributoformulario";
	
	if($forid){
		$sql.= " WHERE forid = $forid";
	}

	$dados = $db->carregar($sql);?>
	<select id="gafid" class="CampoEstilo" style="width: 250px;" name="gafid">
	<option value="">-- Selecione um grupo do atributo --</option>
	<?php
	if($dados){ 
		foreach ($dados as $key => $value):?>
		<option value="<?php echo $value["codigo"]?>"><?php echo $value["descricao"]?></option>
	<?php
	endforeach;
	}?></select><?php
}

function insereAtributoFormularioAjax($request){
	global $db;
	
	if($request['afoid'] == 0){
		$sql = "SELECT afoid FROM formulario.atributoformulario
				WHERE atrid = ".$request['atrid'].
				" AND forid = ".$request['forid'];
		
		$afoid = $db->pegaUm($sql);
		
		if($afoid){
			echo "Não é possível salvar o registro, pois já existe na base de dados!";
			exit();
		}
	}
	
	if($request['afoid'] == 0){
		$sql = "SELECT max(afoordem) as maximo 
				   FROM formulario.atributoformulario
				WHERE forid = ".$request['forid'];
		
		$ordem = $db->pegaUm($sql);
		
		if($ordem){
			$ordem++;
		}else{
			$ordem = 1;
		}
		
		$sql = sprintf("INSERT INTO formulario.atributoformulario(
					            forid, atrid, gafid, afonome, afodescricao, afotextoajuda, 
					            afoobrigatorio, afovalorpadrao, afoordem, afostatus, afodatainclusao)
					    VALUES (%s, %s, %s, '%s', '%s', '%s', %s, 
					            '%s', $ordem, '1', '%s') RETURNING afoid;",
						($request['forid']? $request['forid'] : 'NULL'),
						($request['atrid']? $request['atrid'] : 'NULL'),
						($request['gafid']? $request['gafid'] : 'NULL'),
						($request['afonome']? iconv( "UTF-8", "ISO-8859-1",  $request['afonome'] ) : ''),
						($request['afodescricao']? iconv( "UTF-8", "ISO-8859-1",  $request['afodescricao'] ) : ''),
						($request['afotextoajuda']? iconv( "UTF-8", "ISO-8859-1",  $request['afotextoajuda'] ) : ''),
						($request['afoobrigatorio']? $request['afoobrigatorio'] : ''),
						($request['afovalorpadrao']? iconv( "UTF-8", "ISO-8859-1",  $request['afovalorpadrao'] ) : ''),
						(date('Y-m-d'))
					);
					
		$afoid = $db->pegaUm($sql);
		$res = $db->commit();
		
		if($res == "1"){
			echo $afoid;
		}else
			echo "false";
					
	}else{
		$sql = sprintf("UPDATE formulario.atributoformulario
						   SET forid=%s, atrid=%s, gafid=%s, afonome='%s', afodescricao='%s', 
						       afotextoajuda='%s', afoobrigatorio=%s, afovalorpadrao='%s'
						 WHERE afoid=".$request['afoid'],
						($request['forid']? $request['forid'] : 'NULL'),
						($request['atrid']? $request['atrid'] : 'NULL'),
						($request['gafid']? $request['gafid'] : 'NULL'),
						($request['afonome']? iconv( "UTF-8", "ISO-8859-1",  $request['afonome'] ) : ''),
						($request['afodescricao']? iconv( "UTF-8", "ISO-8859-1",  $request['afodescricao'] ) : ''),
						($request['afotextoajuda']? iconv( "UTF-8", "ISO-8859-1",  $request['afotextoajuda'] ) : ''),
						($request['afoobrigatorio']? $request['afoobrigatorio'] : ''),
						($request['afovalorpadrao']? iconv( "UTF-8", "ISO-8859-1",  $request['afovalorpadrao'] ) : '')
					  );
		
		$db->executar($sql);
		$res = $db->commit();
		
		if($res == "1"){
			echo "true";
		}else
			echo "false";
	}
}

function carregaDadosVinculoFormularioAjax($afoid){
	global $db;
	$sql = "SELECT afoid, forid, atrid, gafid, afonome, afodescricao, afotextoajuda, 
			       afoobrigatorio, afovalorpadrao, afoordem, afostatus, afodatainclusao
			  FROM formulario.atributoformulario
			WHERE afoid = $afoid";
	
	$dados = $db->pegaLinha($sql);
	
	echo json_encode($dados);
}
 
function listarAtributoFormularioAjax($forid){
	global $db;	
	
				
	$sql = "SELECT
				'<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarFormulario('||af.afoid||');\" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
			 		    '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiFormulario('||af.afoid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' as acao, 
				 af.atrid, a.atrnome, af.afodescricao, af.afovalorpadrao,
			       (CASE WHEN af.afoobrigatorio = true THEN 'SIM'
    					 ELSE 'NÃO' END) as afoobrigatorio, to_char(af.afodatainclusao, 'DD/MM/YYYY') as afodatainclusao, 
    			af.afoid as afoordem, gf.gafnome
			  FROM formulario.atributoformulario af 
				inner join formulario.atributo a 
			     on (af.atrid = a.atrid) left join formulario.grupoatributoformulario gf
     			 on (af.gafid = gf.gafid)
			  WHERE afostatus = true
			   AND af.forid = $forid
			  order by gf.gafnome, af.afoordem";
	
	$cabecalho = array("Opções", "Identificador", "Nome do Atributo", "Descrição", "Valor Padrão", "Obrigatório", "Data do Cadastro", "Ordem");
	$ordem = array("formulario.atributoformulario", "afoordem", "afoid");
	
	$db->monta_lista_grupo($sql, $cabecalho, 10, 5, 'N','Center','','formListaGrupo', 'gafnome', $ordem);

}

/*
 * 
 * manter grupo de formulario
 *
*/

function excluiVinculoFormularioAjax($afoid){
	global $db;
	
	$sql = "UPDATE formulario.atributoformulario
			   SET afostatus= false
			 WHERE afoid = $afoid";
	
	$db->executar($sql);
	$res = $db->commit();
	
	if($res == "1"){
		echo "Operação realizada com sucesso!";
	}else{
		echo "Operação não realizada!";
	}
}

function listarGrupoFormularioAjax($forid){
	global $db;
	monta_titulo( '', 'Listagem de Grupos' ); 
	
	/*?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<table id="opaAtributo"  width=100% class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="10%" valign="top" class="title">
						Opções
					</td>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="40%" valign="top" class="title">
						Nome
					</td>
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" width="40%" valign="top" class="title">
						Descrição
					</td>					
					<td style="font-weight: bold;text-align:center" bgcolor="#dcdcdc" onmouseout="this.bgColor='#dcdcdc';" onmouseover="this.bgColor='#c0c0c0';" valign="top" class="title">
						Ordem
					</td>
				</tr>
				<?php*/
				/*$sql = "SELECT
							'<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarGrupo('||gafid||', '||forid||');\" border=0 alt=\"Ir\" title=\"Alterar\"></a>' ||
						 		    '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiGrupo('||gafid||', '||forid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' as acao, 
							 	gafid, forid, gafnome, gafdescricao, gafordem
					  			   FROM formulario.grupoatributoformulario
					  			WHERE forid = $forid";*/
	
				$sql = "SELECT
							'<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarGrupo('||g.gafid||', '||g.forid||');\" border=0 alt=\"Ir\" title=\"Alterar\"></a> ' ||
						 		    '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiGrupo('||g.gafid||', '||g.forid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' as acao, 
							 	g.gafnome, g.gafdescricao, g.gafid as gafordem
					  			   FROM formulario.grupoatributoformulario g
					  			WHERE g.forid = $forid
					  			  order by g.gafordem";
				
				$cabecalho = array("Opções", "Nome", "Descrição", "Ordem");
				$ordem = array("formulario.grupoatributoformulario", "gafordem", "gafid");
				$db->monta_lista_grupo($sql, $cabecalho, 5, 5, 'N','Center','','formGrupo', '', $ordem);
				
				/*$detalhes = $db->carregar($sql);
				
				!$detalhes? $detalhes = array() : $detalhes = $detalhes;

				foreach($detalhes as $chave => $detInd):
					
					$chave % 2 ? $cor = "#fcfcfc" : $cor = "";
					?>
					<tr id="tr_<?=$detInd['gafid'];?>" bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
						<td align="center"><?=$detInd["acao"] ?></td>
						<td align="center"><?=$detInd["gafnome"] ?></td>
						<td><?=$detInd["gafdescricao"] ?></td>					
						<td><input type="hidden" name="ordem_<?=$detInd['gafid'];?>_ordem" value="<?=$detInd['gafordem'] ?>" >
							<input type="hidden" value="<?=$chave ?>">
							<?if(count($detalhes) == ($chave + 1)){ ?>
								<img src="../imagens/seta_baixod.gif" />
							<?}else{ ?>
								<img onclick="mudaPosicao('opaAtributo','baixo',this.parentNode.parentNode.rowIndex, 'formulario.grupoatributoformulario', 'gafordem', 'gafid');" style="cursor: pointer;" src="../imagens/seta_baixo.gif" />
							<?}
							
							if($chave == 0){?>
								<img src="../imagens/seta_cimad.gif" />
							<?}else {?>
								<img onclick="mudaPosicao('opaAtributo','cima',this.parentNode.parentNode.rowIndex, 'formulario.grupoatributoformulario', 'gafordem', 'gafid');" style="cursor: pointer;" src="../imagens/seta_cima.gif" />	
							<?} ?> 
							</td>
					</tr>
					<?php
				
				endforeach;
				
				?>
				</table>
		</td>
	</tr>
</table>
	<?*/
}

function alterarGrupoFormularioAjax($gafid, $forid){
	global $db;
	
	$sql = "SELECT gafid, forid, gafnome, gafdescricao, gafordem
			  FROM formulario.grupoatributoformulario
			WHERE gafid = $gafid";
	
	$dados = $db->pegaLinha($sql);
	
	if($dados){
		echo $dados['gafid']."|".$dados['forid']."|".$dados['gafnome']."|".$dados['gafdescricao'];
	}
}

function excluiGrupoFormularioAjax($gafid, $forid){
	global $db;
	
	$sql = "SELECT af.forid, gf.gafnome, gf.gafid
			  FROM formulario.atributoformulario af
				inner join formulario.atributo a 
			     on (af.atrid = a.atrid) left join formulario.grupoatributoformulario gf
			     on (af.gafid = gf.gafid)
			  WHERE afostatus = true
			   AND af.forid = $forid
			   AND gf.gafid = $gafid
			  group by af.afoid, af.atrid, a.atrnome, af.afodescricao, af.afovalorpadrao, 
					af.afoobrigatorio, af.afodatainclusao, af.afoordem, gf.gafnome, gf.gafid, af.forid
			  order by gf.gafnome, af.afoordem";
	
	$dados = $db->carregar($sql);
	
	if($dados){
		echo "false";
	}else{
		$sql = "DELETE FROM formulario.grupoatributoformulario
				  WHERE gafid = $gafid";
		
		$db->executar($sql);
		$res = $db->commit();
		
		if($res == "1"){
			echo "true";
		}else{
			echo "false";
		}
	}
}

function insereGrupoAjax($gafid, $forid, $gafnome, $gafdescricao){
	global $db;
	if($gafid == ""){
		
		$sql = "SELECT max(gafordem) as maximo 
				   FROM formulario.grupoatributoformulario
				WHERE forid = ".$forid;
		
		$ordem = $db->pegaUm($sql);
		
		if($ordem){
			$ordem++;
		}else{
			$ordem = 1;
		}
				
		$sql = sprintf("INSERT INTO formulario.grupoatributoformulario(
					            forid, gafnome, gafdescricao, gafordem)
					    VALUES (%s, '%s', '%s', '$ordem')RETURNING gafid;",
						($forid ? $forid : 'NULL'),
						($gafnome ? iconv( "UTF-8", "ISO-8859-1", $gafnome ) : ''),
						($gafdescricao ? iconv( "UTF-8", "ISO-8859-1", $gafdescricao ) : '')
						);
		
		$atrid = $db->pegaUm($sql);
		$res = $db->commit();

		if($res == "1"){
			echo $atrid;
		}else
			echo "false";
	}else{
		$sql = sprintf("UPDATE formulario.grupoatributoformulario
						   SET gafnome='%s', gafdescricao='%s'
						 WHERE gafid = $gafid",
						($gafnome ? iconv( "UTF-8", "ISO-8859-1", $gafnome ) : ''),
						($gafdescricao ? iconv( "UTF-8", "ISO-8859-1", $gafdescricao ) : '')
						);
		
		$db->executar($sql);
		$res = $db->commit();
		
		if($res == "1"){
			echo "true";
		}else{
			echo "false";
		}
	}
	
	
}

/*
 * id1 			 => posição atual do registro 1
 * id2 			 => posição atual do registro 2
 * tabela 		 => tabela que receberá a alteração
 * atributoOrdem => nome do atributo ordem na tabela
 * id            => identificador da tabela
*/

/*function mudaPosicao($id1, $id2, $tabela, $atributoOrdem, $id){
	global $db;
	$sql = "";
	if(!$id1 || !$id2){
		return false;
	}else{
		$sql = "SELECT $atributoOrdem
				  FROM $tabela
				WHERE
				  $id = $id1";
		$ordem1 = $db->pegaUm($sql);

		$sql = "SELECT $atributoOrdem
				  FROM $tabela
				WHERE
				  $id = $id2";
		$ordem2 = $db->pegaUm($sql);

		$sql = "update 
					$tabela
					set
				$atributoOrdem = $ordem2
				where
					$id = $id1";

		$db->executar($sql);

		$sql = "update 
					$tabela
					set
				$atributoOrdem = $ordem1
				where
					$id = $id2";
		$db->executar($sql);

		$db->commit();
		
	}	
}*/

function montaCampoValorPadraoAjax($atrid, $valorpadrao){
	global $db;
	if($atrid){
		$sql = "SELECT a.atrid, ta.tiaopcoes, a.atrtipodado
				  FROM formulario.atributo a left join formulario.tipoatributo ta
				   ON (a.tiaid = ta.tiaid)
				 WHERE ta.tiaopcoes = false
				  AND a.atrid = $atrid";
		
		$dados = $db->pegaLinha($sql);
		
		if($dados){ /*
		             * O usuário deverá informar um valor quando o tipo de atributo não definir as Opções do Atributo (TipoAtributo.tiaOpcoes = false)
					*/
			echo campo_texto( 'afovalorpadrao', 'N', 'S', '', 80, 250, '', '','','','','id="afovalorpadrao"', '', '', 'validaTipoDado();');
			echo "<input type=\"hidden\" name=\"tipoDados\" id=\"tipoDados\" value=\"".$dados['atrtipodado']."\">";
		}else{
			$sql = "SELECT a.atrid, ta.tiaopcoes
					  FROM formulario.atributo a left join formulario.tipoatributo ta
					   ON (a.tiaid = ta.tiaid)
					 WHERE ta.tiaselecionavariasopcoes = true
					  AND a.atrid = $atrid";
		
			$dados = $db->pegaLinha($sql);
			//echo $sql;
			if($dados){
				/*
				 * Caso o tipo do atributo permita a seleção de mais de uma opção simultaneamente (TipoAtributo.tiaSelecionaVariasOpcoes = true) 
				 * o sistema não permitirá o preenchimento do campo 'Valor padrão'
				*/
				echo campo_texto( 'afovalorpadrao', 'N', 'N', '', 80, 250, '', '','','','','id="afovalorpadrao"');
			}else{/* 
			       * Para os atributos cujo tipo define Opções do Atributo (TipoAtributo.tiaOpcoes = true), 
				   * o sistema deverá exibir uma listagem com todas as opções cadastradas
				   */
				$sql = "SELECT opavalor as codigo, opadescricao as descricao       
						  FROM formulario.opcoesatributo
						 WHERE atrid = $atrid
						  AND opastatus = true";
						  
				$dados = $db->carregar($sql);
				
				?>
				<select id="afovalorpadrao" class="CampoEstilo" style="width: 250px;" name="afovalorpadrao">
				<option value="">-- Selecione um valor --</option>
				<?php
				if($dados){ 
					foreach ($dados as $key => $value){
						if($valorpadrao == $value["codigo"]){?>
							<option selected="selected" value="<?php echo $value["codigo"]?>"><?php echo $value["codigo"]?></option><?php
						}else{?>
							<option value="<?php echo $value["codigo"]?>"><?php echo $value["codigo"]?></option>
				<?		}
					}
				}?>
				</select><?php

				//echo $db->monta_combo("afovalorpadrao", $sql, 'S','-- Selecione um valor --','', '', '',250,'N','afovalorpadrao', '', $afovalorpadrao);
			}
		}
	}else{
		echo campo_texto( 'afovalorpadrao', 'N', 'S', '', 80, 250, '', '','','','','id="afovalorpadrao"');
	}
}

function montaComboTelaAjax($sisid){
	global $db;
	
	if($sisid){
		$sql = "SELECT mnucod as codigo, mnudsc as descricao
				  FROM seguranca.menu
				  WHERE sisid = $sisid";
				  			  
		$db->monta_combo("mnuid",$sql, 'S','-- Selecione a Tela --','', '', '','','N','mnuid');
	}else{
		$array = array();
		$db->monta_combo("mnuid",$array, 'S','-- Selecione a Tela --','', '', '','','N','mnuid');
	}
}

function carregaFormularioPreenchidoAjax($forid, $sisid, $mnuid){
	global $db;
	
	$sql = "SELECT '' as acao, fp.fprid, s.sisdsc, m.mnudsc, fp.fprobservacao, fp.fprdataalteracao, fp.usucpf
  			 FROM formulario.formulariopreenchido fp inner join seguranca.sistema s
  			  ON (fp.sisid = s.sisid) inner join seguranca.menu m
  			  ON (fp.mnuid = m.mnuid)
  			WHERE forid = $forid";

  	if($sisid){
  		$sql.= " AND fp.sisid = $sisid";
  	}
  	
	if($mnuid){
  		$sql.= " AND fp.mnuid = $mnuid";
  	}
  	
  	$sql.= " order by s.sisdsc";
	
	monta_titulo( '', 'Lista de Formulários Preenchidos' );
	$cabecalho = array("Opções", "Identificador", "Sistema", "Página", "Observação", "Data da última alteração", "Usuário da ultima alteração");
 
	$db->monta_lista($sql, $cabecalho, 8, 4, 'N','Center','');
}

?>