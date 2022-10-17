<?php
/*
 * Classe MontaListaAjax
 * Classe para criaÁ„o monta lista com Ajax (Prototype)
 * @author Gustavo Fernandes da Guarda
 * @since 25/01/2010
 */
class MontaListaAjax extends cls_banco {

	public $db;

	public $sql;

	public $cabecalho;

	public function __construct($db = null, $boJQuery = true, $boMontaScript = true){
		
		$this->db = $db;

		if($boJQuery){
			echo "<script type=\"text/javascript\" src=\"../includes/JQuery/jquery.js\"></script>";
			echo "<script type=\"text/javascript\"> jQuery.noConflict(); </script>";		
		}

		if($boMontaScript){
			?>
			<center>
			<div id="aguarde_"
				style="display: none; position: absolute; color: #000033; top: 50%; left: 35%; width: 300; font-size: 12px; z-index: 0;">
			<br>
			<img src="../imagens/carregando.gif" border="0" align="middle"><br>
			Carregando...<br>
			</div>
			</center>
			<script type="text/javascript">
				function listaPaginacao(paginaAtual, url, registroPorPagina, posicao, totalNumerosBarrNavegacao, totalRegistro, ordemlista, ordemlistadir) {
					var data 	  = new Array();

					url = '/includes/montaListaAjax/montaListaAjax.php';
					if(posicao == ''){
						posicao = 0;
					}					
					

					if(ordemlista != undefined){
					data.push({name : 'requisicao', value : 'listaPaginacao'}, 
							  {name : 'paginaAtual', value : paginaAtual}, 
							  {name : 'registroPorPagina', value : registroPorPagina},
							  {name : 'posicao', value : posicao},
							  {name : 'totalNumerosBarrNavegacao', value : totalNumerosBarrNavegacao},
							  {name : 'totalRegistro', value : totalRegistro},						  
							  {name : 'ordemListaAjax', value : ordemlista},
							  {name : 'ordemListaDirAjax', value : ordemlistadir}
							  
							 );	
					} else {
					data.push({name : 'requisicao', value : 'listaPaginacao'}, 
							  {name : 'paginaAtual', value : paginaAtual}, 
							  {name : 'registroPorPagina', value : registroPorPagina},
							  {name : 'posicao', value : posicao},
							  {name : 'totalNumerosBarrNavegacao', value : totalNumerosBarrNavegacao},
							  {name : 'totalRegistro', value : totalRegistro}
							  							  
							 );	
					}
					
					 //jQuery('#paginacao').html("<div style=\"width:100%;text-align:center;margin-top:15%\" ><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Carregando...</div>");

					 jQuery.ajax({
						   type		: "POST",
						   url		: url,
						   data		: data,
						   success	: function(msg){
													 //divPaginacao.innerHTML = msg;
													 jQuery('#paginacao').html(msg);
												   }
						 });					


				}

			</script>
			<?php
		}

	}

	/**
	 * MÈtodo para criaÁ„o de listas.
	 * @param 
	 * @param  
	 * @return void
	 * @access public
	 * @author 
	 * @since 
	 * @final	 
	 */
	public function montaLista($sql, $cabecalho = "", $registroPorPagina, $totalNumerosBarrNavegacao, $soma, $alinha, $par2, $nomeformulario = "", $celWidth = "", $celAlign = "", $funcoes = "", $parametrosFuncoes = "") {
		self::sessaoCabecalhoSql($sql,$cabecalho, $funcoes, $parametrosFuncoes);
		
		//Registro Atual (instanciado na chamada)
		if ($_REQUEST['numero']=='') 
			$numero = 0; 
		else 
			$numero = intval($_REQUEST['numero']);

		// CARREGA DADOS		 
		if (is_array($sql)){
			
			$posicao = $numero;						
			$arDados = $sql;			
			$totalRegistro = count($sql);			

		} else {
			$posicao = $numero;				
			$sql = trim($sql);

			$sqlCount = "select
							count(1)
						 from (" . $sql . ") rs";
			$totalRegistro = $this->db->pegaUm($sqlCount);
			$sql = $sql . " LIMIT {$registroPorPagina} offset ".($posicao);

			$arDados = $this->db->carregar($sql);		
			
		} // fim _CARREGA DADOS		

//		if(!$arDados){
//			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr><td><center>N„o existem registros.</center></td></tr></table>';
//			die();
//		}
		
		$nlinhas = count($arDados);
		$RS = $arDados;
		
		if (!$arDados) $nl = 0; else $nl=$nlinhas;
		if (($numero+$registroPorPagina)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$registroPorPagina-1;
		$paginaAtualNova = $_REQUEST['paginaAtual'] ? $_REQUEST['paginaAtual'] : 1;

		echo "<div id=\"paginacao\">";
		
		if ($nl > 0) {
			$ordenador = array_keys($RS[0]);
			
			//monta o formulario da lista mantendo os parametros atuais da pgina
			echo '<form name="formlista" id="formlista" method="post"><input type="Hidden" name="posicao" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';
			foreach($_POST as $k=>$v){if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'posicao') echo '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';}
			echo '</form>';
				
			if($nomeformulario != "") {
				echo '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'">';
			}

			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
				
			//Monta Cabealho
			if(is_array($cabecalho)) {
				echo '<thead><tr>';
				for ($i=0;$i<count($cabecalho);$i++)
				{
					if ($_REQUEST['ordemListaAjax'] == ($i+1)) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';
					}
					echo '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="listaPaginacao(\''.$paginaAtualNova.'\',\''.$url.'\',\''.$registroPorPagina.'\',\''.$posicao.'\',\''.$totalNumerosBarrNavegacao.'\',\''.$totalRegistro.'\', \''.$ordenador[$i].'\', \''.$ordemlistadirnova.'\')" title="Ordenar por '.strip_tags($cabecalho[$i]).'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';			
				}
				echo '</tr> </thead>';
			} elseif( $cabecalho === null) {
				echo '<thead><tr>'; $i=0;
				foreach($arDados[0] as $k=>$v) {
					if ($_REQUEST['ordemListaAjax'] == ($i+1)) {
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						$ordemlistadirnova = 'ASC';
						$imgordem = '';
					}
					echo '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="listaPaginacao(\''.$paginaAtualNova.'\',\''.$url.'\',\''.$registroPorPagina.'\',\''.$posicao.'\',\''.$totalNumerosBarrNavegacao.'\',\''.$totalRegistro.'\', \''.$ordenador[$i].'\', \''.$ordemlistadirnova.'\')" title="Ordenar por '.strip_tags($k).'">'.$imgordem.'<strong>'.$k.'</strong></label>';
					$i=$i+1;}
					echo '</tr> </thead>';
			}
			
			//Monta Listagem
			$totais = array();
			$tipovl = array();			
			
			for ($i=$numero; $i < $reg_fim; $i++) {
				$c = 0;
				if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
				echo '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';

				$numCel = 0;				
				
				// EXECUTA PARAMETRO FUN«’ES				 
				if(is_array($funcoes)){
					
					foreach($parametrosFuncoes as $dados){
							
						$parametro[] =  $arDados[$i][$dados];						
					}
					
					foreach($funcoes as $chave => $valor){						
						
						$parametros = "'".implode("', '", $parametro)."'";											
						$arDados[$i][$chave] = $valor($parametros);							
					}					
					
					foreach($parametrosFuncoes as $dados){	
						
						unset($arDados[$i][$dados]);
						unset($parametro);						
					}
					
				}

				foreach($arDados[$i] as $k=>$v) {
					// Setando o alinhamento da cÈlula usando o array $celAlign.
					// Se n„o for passado o par‚metro, usa o padr„o do componente.
					if(is_array($celAlign)) {
						$alignNumeric = $alignNotNumeric = $celAlign[$numCel];
					} else {
						$alignNumeric 		= 'right';
						$alignNotNumeric	= 'left';
					}
					
					// Setando o tamanho da cÈlula usando o array $celWidth.
					// Se n„o for passado o par‚metro, usa o padr„o do componente.
					$width = (is_array($celWidth)) ? 'width="'.$celWidth[$numCel].'"' : '';
						
					if (is_numeric($v)) {
						//cria o array totalizador
						if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
						//Mostra o resultado
						if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
						if ($v<0) echo '<td align="'.$alignNumeric.'" '.$width.' style="color:#cc0000;" title="'.strip_tags($cabecalho[$c]).'">('.$v.')'; else echo '<td align="'.$alignNumeric.'" '.$width.' style="color:#0066cc;" title="'.strip_tags($cabecalho[$c]).'">'.$v;
						echo ('<br>'.$totais[$c]);
					} else {
						echo '<td align="'.$alignNotNumeric.'" '.$width.' title="'.strip_tags($cabecalho[$c]).'">'.$v;
					}
					echo '</td>';
					$c = $c + 1;
					$numCel++;
				}
				
				echo '</tr>';
	
			}
		
			if ($soma=='S'){
				
				//totaliza (imprime totais dos campos numericos)
				echo '<thead><tr>';
				for ($i=0;$i<$c;$i++) {
					echo '<td align="right" title="'.strip_tags($cabecalho[$i]).'">';

					if ($i==0) echo 'Totais:   ';
					if (is_numeric($totais['0'.$i])) echo number_format($totais['0'.$i], 2, ',', '.'); else echo $totais['0'.$i];
					echo '</td>';
				}
				echo '</tr>';
				//fim totais
			}

			echo '</table>';

			if($nomeformulario != "") {
				echo '</form>';
			}

			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';

			$url = str_replace('/simec/','',$_SERVER['REQUEST_URI']);
				
			if (fmod($totalRegistro, $registroPorPagina)==0) $totalRegistro = $totalRegistro - 1;
				
			//P·gina atual
			$pag_atu = intval(($posicao-1)/$registroPorPagina)+1;
				
			//P·gina Max
			$pag_max = intval(($totalRegistro+$registroPorPagina)/$registroPorPagina);
				
			//Pagina Inicial
			if ($pag_max<$totalNumerosBarrNavegacao) $p_ini = 1; else $p_ini = (intval(($pag_atu-1)/$totalNumerosBarrNavegacao)*$totalNumerosBarrNavegacao)+1;
				
			//Pagina Final
			if  ($pag_max > ($p_ini+$totalNumerosBarrNavegacao)) $p_fim = $p_ini + $totalNumerosBarrNavegacao - 1; else $p_fim = $pag_max;
				
			//Montando a Barrinha de NavegaÁ„o			
			if ($totalRegistro>$registroPorPagina) {
				
				echo "<div align='right'>P·ginas: ";
				
				for($n = $p_ini; $n < $p_fim+1; $n++) {
					
					if (($n*$registroPorPagina)>=$totalRegistro) {
						
						$reg_fim = $nlinhas;
					} else {
						
						$reg_fim = $n*$registroPorPagina;
					}

					$alt_txt=  "De ".($n*$registroPorPagina-$registroPorPagina+1)." atÈ ".$reg_fim;
					if ($n == $pag_atu) {
						
						echo"<strong style=\"background-color:#000000;color:#ffffff;BORDER-RIGHT: #a0a0a0 1px solid;\">&nbsp;".$n."&nbsp;</strong>";
					} else {
						
						$posicao = ($n*$registroPorPagina-$registroPorPagina);						
						echo "<a href=\"javascript:void(0);\" onclick=\"listaPaginacao(".($n).",'".$url."','".$registroPorPagina."','".$posicao."','".$totalNumerosBarrNavegacao."','".$totalRegistro."');\" title=\"".strip_tags($alt_txt)."\" style=\"background-color:#f5f5f5;color:#006699;BORDER-RIGHT: #a0a0a0 1px solid;text-decoration:none;\">&nbsp;<u>".$n."</u>&nbsp;</a>";
					}
				}
				if (intval($posicao)<($totalRegistro-$registroPorPagina+1)){
					
					echo "<a href=\"javascript:void(0);\" onclick=\"listaPaginacao(".($n).",'".$url."','".$registroPorPagina."','".(intval($posicao)+$registroPorPagina)."','".$totalNumerosBarrNavegacao."','".$totalRegistro."');\" style=\"color:#000000\" title=\"PrÛxima P·gina\"> <b>ª</b> </a>";			
				}
				echo "</div>";
			}
			echo '</td></tr></table>';

		} else {
			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			echo '<tr><td align="center" style="color:#cc0000;">N„o foram encontrados Registros.</td></tr>';
			echo '</table>';
		}
		echo "</div>";
	}

	public function listaPaginacao(&$post){
		extract($post);
		$sql = $_SESSION['montaListaAjax']['sql'];
		$cabecalho = $_SESSION['montaListaAjax']['cabecalho'];	
		
		// CONTROLA O ORDER BY		
		if ($_REQUEST['ordemListaAjax'] != '' && is_array($sql)) {		
		
			if ($_REQUEST['ordemListaDirAjax'] <> 'DESC') {
				$ordemlistadir = 'ASC';
				$ordemlistadir2 = 'DESC';
			} else {
				$ordemlistadir = 'DESC'; 
				$ordemlistadir2 = 'ASC';
			}
			// Obter uma lista de colunas
			foreach ($sql as $key => $row) {
				
		    	$row = ereg_replace("[^a-zA-Z0-9_]", "", strtr($row[($_REQUEST['ordemListaAjax'])], "·‡„‚ÈÍÌÛÙı˙¸Á¡¿√¬… Õ”‘’⁄‹«", "aaaaeeiooouucAAAAEEIOOOUUC"));
		    	//$crt[$key]  = $row[$_REQUEST['ordemListaAjax']];		    				    	
		    	$crt[$key]  = $row;
			}

			switch($ordemlistadir) {
				case 'ASC':
					array_multisort($crt,SORT_ASC, $sql);					
					break;
				case 'DESC':
					array_multisort($crt,SORT_DESC, $sql);				
					break;
			}
			
		} elseif($_REQUEST['ordemListaAjax'] != '' && !is_array($sql)) {
			
			if ($_REQUEST['ordemListaDirAjax'] <> 'DESC') {
				$ordemlistadir = 'ASC';
				$ordemlistadir2 = 'DESC';
			} else {
				$ordemlistadir = 'DESC'; 
				$ordemlistadir2 = 'ASC';
			}    	

			//$subsql = substr($sql,0,strpos(trim(strtoupper($sql)),'ORDER '));			
			//$sql = (!$subsql ? $sql : $subsql).' order by '.$campo.' '.$ordemlistadir;	
			$sql = $sql.' order by '.$ordemListaAjax.' '.$ordemlistadir;
		} // fim _ORDER BY
	
		// CARREGA DADOS
		if (is_array($sql)){			
						
			$arDados = $sql;			
			$tipoDados = 'array';			
			$totalRegistro = count($sql);

		} else {
				
			$sql = trim($sql);			
			$tipoDados = 'sql';				
			$sqlCount = "select
							count(1)
						 from (" . $sql . ") rs";
			$totalRegistro = $this->db->pegaUm($sqlCount);
			$sql = $sql . " LIMIT {$registroPorPagina} offset ".($posicao);

			$arDados = $this->db->carregar($sql);						
		}		
		  
		$nlinhas = count($arDados);
		$RS = $arDados;
		
		if (!$arDados) $nl = 0; else $nl=$nlinhas;		
		if (($posicao+$registroPorPagina)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $posicao+$registroPorPagina;
		$paginaAtualNova = $_REQUEST['paginaAtual'] ? $_REQUEST['paginaAtual'] : 1;
		
		$nTemp = $p_ini - $totalNumerosBarrNavegacao;
		
		echo "<div id=\"paginacao\">";
		
		if ($nl > 0) {
			
			$ordenador = array_keys($RS[0]);
			
			//monta o formulario da lista mantendo os parametros atuais da pgina
			echo '<form name="formlista" method="post"><input type="Hidden" name="posicao" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';
			foreach($_POST as $k=>$v){if ($k<>'ordemlista' && $k<>'ordemlistadir' && $k<>'posicao') echo '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';}
			echo '</form>';
				
			if($nomeformulario != "")
			echo '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'">';

			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';			
			
			// Monta Cabealho
			if ( $cabecalho === null ) {

			} else if(is_array($cabecalho)) {
				
				echo '<thead><tr>';
				
				for ($i=0;$i<count($cabecalho);$i++) {					
					
					if ($_REQUEST['ordemListaAjax'] == ($ordenador[$i])) {
						
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
						
					} else {
						
						$ordemlistadirnova = 'ASC';
						$imgordem = '';
					}

					echo '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="listaPaginacao(\''.$paginaAtualNova.'\',\''.$url.'\',\''.$registroPorPagina.'\',\''.$posicao.'\',\''.$totalNumerosBarrNavegacao.'\',\''.$totalRegistro.'\', \''.$ordenador[$i].'\', \''.$ordemlistadirnova.'\')" title="Ordenar por '.strip_tags($cabecalho[$i]).'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
				}
				
				echo '</tr> </thead>';
				
			} else {
				
				echo '<thead><tr>'; $i=0;
				foreach($arDados[0] as $k=>$v)
				{
					if ($_REQUEST['ordemListaAjax'] == ($i)) {
						
						$ordemlistadirnova = $ordemlistadir2;
						$imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
					} else {
						
						$ordemlistadirnova = 'ASC';
						$imgordem = '';}
						echo '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="listaPaginacao(\''.$paginaAtualNova.'\',\''.$url.'\',\''.$registroPorPagina.'\',\''.$posicao.'\',\''.$totalNumerosBarrNavegacao.'\',\''.$totalRegistro.'\', \''.$ordenador[$i].'\', \''.$ordemlistadirnova.'\')" title="Ordenar por '.strip_tags($k).'">'.$imgordem.'<strong>'.$k.'</strong></label>';
						$i=$i+1;}
						echo '</tr> </thead>';
			}

			//Monta Listagem
			$totais = array();
			$tipovl = array();

			$posAtual = intval($posicao);
			
			if($tipoDados == 'array'){				
				if($posicao != 0) 
					$posicao = intval($posicao);
			} else {
				$posicao = 0;
				$reg_fim = $reg_fim-1;				
			}
			
			for ($i=$posicao; $i <= $reg_fim; $i++){
				$c = 0;
				if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
				echo '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
				
				// contador -> posicao de celulas
				$numCel = 0;				
				
				// EXECUTA PARAMETRO FUN«’ES				 
				include_once APPRAIZ . 'www/cte/_funcoes.php';
				$funcoes = $_SESSION['montaListaAjax']['funcoes'];
				$parametrosFuncoes = $_SESSION['montaListaAjax']['parametrosFuncoes'];

				if(is_array($funcoes)){
					
					foreach($parametrosFuncoes as $dados){
							
						$parametro[] =  $arDados[$i][$dados];						
					}
					
					foreach($funcoes as $chave => $valor){						
						
						$parametros = "'".implode("', '", $parametro)."'";											
						$arDados[$i][$chave] = $valor($parametros);							
					}					
					
					foreach($parametrosFuncoes as $dados){	
						
						unset($arDados[$i][$dados]);
						unset($parametro);						
					}
					
				}

				foreach($arDados[$i] as $k=>$v) {				
					
					// Setando o alinhamento da cÈlula usando o array $celAlign.
					// Se n„o for passado o par‚metro, usa o padr„o do componente.
					if(is_array($celAlign)) {
						$alignNumeric = $alignNotNumeric = $celAlign[$numCel];
					} else {
						$alignNumeric 		= 'right';
						$alignNotNumeric	= 'left';
					}
					// Setando o tamanho da cÈlula usando o array $celWidth.
					// Se n„o for passado o par‚metro, usa o padr„o do componente.
					$width = (is_array($celWidth)) ? 'width="'.$celWidth[$numCel].'"' : '';
						
					if (is_numeric($v))	{
						//cria o array totalizador
						if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
						
						//Mostra o resultado
						if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
						if ($v<0) echo '<td align="'.$alignNumeric.'" '.$width.' style="color:#cc0000;" title="'.strip_tags($cabecalho[$c]).'">('.$v.')'; else echo '<td align="'.$alignNumeric.'" '.$width.' style="color:#0066cc;" title="'.strip_tags($cabecalho[$c]).'">'.$v;
						echo ('<br>'.$totais[$c]);
					} else {
						
						echo '<td align="'.$alignNotNumeric.'" '.$width.' title="'.strip_tags($cabecalho[$c]).'">'.$v;
					}
					echo '</td>';
					$c = $c + 1;
					$numCel++;
				}
				
				echo '</tr>';
			}

			if ($soma=='S'){
				
				//totaliza (imprime totais dos campos numericos)
				echo '<thead><tr>';
				for ($i=0;$i<$c;$i++)
				{
					echo '<td align="right" title="'.strip_tags($cabecalho[$i]).'">';
					if ($i==0) echo 'Totais:   ';
					if (is_numeric($totais['0'.$i])) echo number_format($totais['0'.$i], 2, ',', '.'); else echo $totais['0'.$i];
					echo '</td>';
				}
				echo '</tr>';
				//fim totais
			}

			echo '</table>';

			if($nomeformulario != ""){
				echo '</form>';
			}
				
			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';
				
			$url = str_replace('/simec/','',$_SERVER['REQUEST_URI']);

			//P·gina Max
			$pag_max = intval(($totalRegistro+$registroPorPagina)/$registroPorPagina);

			//Pagina Inicial
			if ($pag_max<$totalNumerosBarrNavegacao) $p_ini = 1; else $p_ini = (intval(($paginaAtual-1)/$totalNumerosBarrNavegacao)*$totalNumerosBarrNavegacao)+1;

			//Pagina Final
			if  ($pag_max > ($p_ini+$totalNumerosBarrNavegacao)) $p_fim = $p_ini + $totalNumerosBarrNavegacao - 1; else $p_fim = $pag_max;

			//Montando a Barrrinha de NavegaÁ„o
			if ($totalRegistro>$registroPorPagina) {
				
				echo "<div align='right'>P·ginas:";
				
				if ($p_ini>$totalNumerosBarrNavegacao){
					$nTemp = $p_ini - $totalNumerosBarrNavegacao;
			 		echo "<a href=\"javascript:void(0);\" onclick=\"listaPaginacao(".$nTemp.",'".$url."','".$registroPorPagina."','".($posAtual-$registroPorPagina)."','".$totalNumerosBarrNavegacao."','".$totalRegistro."','".$_REQUEST['ordemlistanova']."','".$_REQUEST['ordemlistadirnova']."'); return void(0); \" style=\"BORDER-RIGHT:#a0a0a0 1px solid;color:#000000\" title=\"P·gina Anterior\"> <b>´</b> </a>";
				}
				
				for($n = $p_ini; $n < $p_fim+1; $n++)
				{
					if (($n*$registroPorPagina)>=$totalRegistro) {
						$reg_fim = $nlinhas;
					} else {
						$reg_fim = $n*$registroPorPagina;
					}
						
					$alt_txt=  "De ".($n*$registroPorPagina-$registroPorPagina+1).' atÈ '.$reg_fim;
					if ($n == $paginaAtual) {
						echo"<strong style=\"background-color:#000000;color:#ffffff;BORDER-RIGHT: #a0a0a0 1px solid;\">&nbsp;".$n."&nbsp;</strong>";
						$posicaoAtual = ($n*$registroPorPagina-$registroPorPagina); 
					} else {
						$posicao = ($n*$registroPorPagina-$registroPorPagina);
						echo "<a href=\"javascript:void(0);\" onclick=\"listaPaginacao(".($n).",'".$url."','".$registroPorPagina."','".$posicao."','".$totalNumerosBarrNavegacao."','".$totalRegistro."','".$_REQUEST['ordemListaAjax']."','".$_REQUEST['ordemListaDirAjax']."'); return void(0);\" title=\"".strip_tags($alt_txt)."\" style=\"background-color:#f5f5f5;color:#006699;BORDER-RIGHT: #a0a0a0 1px solid;text-decoration:none;\">&nbsp;<u>".$n."</u>&nbsp;</a>";
					}
				
				}
									
				if($p_fim != $pag_max){
					echo "<a href=\"javascript:void(0);\" onclick=\"listaPaginacao(".($n).",'".$url."','".$registroPorPagina."','".(intval($posicao)+$registroPorPagina)."','".$totalNumerosBarrNavegacao."','".$totalRegistro."','".$_REQUEST['ordemListaAjax']."','".$_REQUEST['ordemListaDirAjax']."'); return void(0); \" style=\"color:#000000\" title=\"PrÛxima P·gina\"> <b>ª</b> </a>";																					
				}
			}
				
			echo '</td></tr></table>';
			echo "</div>";
			
		} else {
			
			echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
			echo '<tr><td align="center" style="color:#cc0000;">N„o foram encontrados Registros.</td></tr>';
			echo '</table>';
		}
				
		die;
	}

	private function sessaoCabecalhoSql(&$sql,&$cabecalho,&$funcoes, &$parametrosFuncoes){
		$_SESSION['montaListaAjax']['sql'] 		 			= $sql;
		$_SESSION['montaListaAjax']['cabecalho'] 			= $cabecalho;
		$_SESSION['montaListaAjax']['funcoes']				= $funcoes;
		$_SESSION['montaListaAjax']['parametrosFuncoes']	= $parametrosFuncoes;		
	}

}