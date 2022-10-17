<?php
/*
 * Classe AbaAjax
 * Classe para criação de Abas com exibição de conteúdo em Ajax (Prototype)
 * @author Gustavo Fernandes e Juliano Meinen
 * @since 22/09/2009
 */
class AbaAjax extends cls_banco {
	
	public $tabelaAba;
	
	public $db;
	
	public $boJqueryNoConflict;
	
	public function __construct($tabela_aba = null, $boPrototype = false, $boJquery = false, $boJqueryNoConflict = false, $boCache = false){
		$this->db = $db;
		$this->boJqueryNoConflict = $boJqueryNoConflict;
		$this->boCache = $boCache;
		
		if($tabela_aba){
			$this->tabelaAba = $tabela_aba;
		} else {
			$this->tabelaAba = 'tabela_aba';			
		}
		
		if($boPrototype)
			echo "<script type=\"text/javascript\" src=\"../includes/prototype.js\"></script>";
		if($boJquery)
			echo "<script type=\"text/javascript\" src=\"../includes/JQuery/jquery-1.4.2.js\"></script>";	
	}
		
	/**
	 * Método para criação das abas.
	 * @param array $arAbas - Array composto pelas chaves 'descricao', 'padrao','url','parametro'.
	 * @param string $idDiv - ID da div que retornará o conteudo das abas.  
	 * @return void
	 * @access public
	 * @author Gustavo Fernandes
	 * @since 22/09/2009
	 * @final
	 *
	 * @example:
	 * $arAba = array(
	 *		  		array(	 "descricao" => "Teste", 
	 *	   				 	 "padrao" => false,
	 *		   				 "url" => "ajax.php",
	 *		   				 "parametro" => "id=1",
	 * 						 "elemento" => array( "id2" ) 
	 * 				)
	 * 		    );
	 * 
	 */
	public function criaAba($arAbas = array() , $idDiv){
		
		if (!is_array($arAbas)) {
	        $arAbas = $this->db->carregar($arAbas);
	    }
		$menu    = "<table width=\"95%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" align=\"center\" class=\"notprint\">"
	             . "<tr>"
	             . "<td>"
	             . "<table cellpadding=\"0\" cellspacing=\"0\" align=\"left\" id=\"{$this->tabelaAba}\">"
	             . "<tr>";
		?>
		<style>
			.fundoSel{
				background: url( '../imagens/aba_fundo_sel.gif' );
			}
			.fundoNosel{
				background: url( '../imagens/aba_fundo_nosel.gif' );
			}
		</style>  
	  <script>
	  
	  /* FUNÇAO QUE EXECUTA JS NO AJAX */
	  function extraiScriptAbaAjax(texto){  
				//desenvolvido por Skywalker.to, Micox e Pita.  
				//http://forum.imasters.uol.com.br/index.php?showtopic=165277  
				var ini, pos_src, fim, codigo;  
				var objScript = null;  
				ini = texto.indexOf('<script', 0)  
				while (ini!=-1){  
					var objScript = document.createElement("script");  
					//Busca se tem algum src a partir do inicio do script  
					pos_src = texto.indexOf(' src', ini)  
					ini = texto.indexOf('>', ini) + 1;
			
					//Verifica se este e um bloco de script ou include para um arquivo de scripts  
					if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script  
						//Marca como sendo o inicio do nome do arquivo para depois do src  
						ini = pos_src + 4;  
						//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele  
						fim = texto.indexOf('.', ini)+4;  
						//Pega o nome do arquivo  
						codigo = texto.substring(ini,fim);  
						//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano  
						codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");  
						// Adiciona o arquivo de script ao objeto que sera adicionado ao documento  
						objScript.src = codigo;  
					}else{
					//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script  
						// Procura o final do script
						fim = texto.indexOf('</script', ini);  
						// Extrai apenas o script
						codigo = texto.substring(ini,fim);  
						// Adiciona o bloco de script ao objeto que sera adicionado ao documento  
						objScript.text = codigo;
					}
					
					//Adiciona o script ao documento  
					document.body.appendChild(objScript);
					
					// Procura a proxima tag de <script>  
					ini = texto.indexOf('<script', fim);
					
					//Limpa o objeto de script  
					objScript = null;  
				}  
		}
	  
	  
	  /**
	  * FUNÇAO CHAMADA DO LINK DA ABA
	  */
		function clicaAba(obj,url,parametros){
			if($('<?php echo $this->tabelaAba; ?>').rows.length > 0){
				for (var i = 0; i < $('<?php echo $this->tabelaAba; ?>').rows.length; i++) {
					var tr = $('<?php echo $this->tabelaAba; ?>').rows[i];
					if(tr.cells.length > 0){
						var gifaba = '';
						var giffundo_aba = '';
						var cor_fonteaba = '';
						for (var j = 0; j < tr.cells.length; j++) {
						
							<?if($this->boCache){ ?>
								//alert('fez cache');
								$('<? echo $idDiv; ?>').show();
								if($( '<?php echo $this->tabelaAba; ?>_div_nome_' + j ) ){
									$( '<?php echo $this->tabelaAba; ?>_div_nome_' + j ).hide();
								}
							<? } ?>
						
							var td = tr.cells[j];
							var tdAnteriorObj = obj.previousSibling
							var tdAnteriorCorrente = td.previousSibling
							if(td.id.search("<?php echo $this->tabelaAba; ?>_td_nome_") >= 0) {
								if(td.id == obj.id){
									tdAnteriorObj.innerHTML = "";
									
									if((tdAnteriorObj.id.replace('<?php echo $this->tabelaAba; ?>_td_imgini_', '') + 1) == j ){
										gifaba = "aba_esq_sel_ini.gif";
									} else {
										gifaba = "aba_esq_sel.gif";									
									}
							        giffundo_aba = 'aba_fundo_sel.gif';
									cor_fonteaba = '#000055';
								
									var img = document.createElement( "img" );
										img.setAttribute( "src", "/imagens/"+gifaba+"" );
										img.setAttribute( "width", "11" );
										img.setAttribute( "height", "24" );
									tdAnteriorObj.appendChild( img );
									
			            			td.style.color  = cor_fonteaba;
			            			td.className  = 'fundoSel';
								} else {
									tdAnteriorCorrente.innerHTML = "";
									
									if(gifaba == 'aba_esq_sel_ini.gif' || gifaba == 'aba_esq_sel.gif') {
										gifaba = 'aba_dir_sel.gif';						
									} else if((tdAnteriorCorrente.id.replace('<?php echo $this->tabelaAba; ?>_td_imgini_', '') + 1) == j ){
										gifaba = "aba_nosel_ini.gif";
									} else {
										gifaba = "aba_nosel.gif";									
									}
							        giffundo_aba = 'aba_fundo_nosel.gif';
									cor_fonteaba = '#4488cc';
								
									var img = document.createElement( "img" );
										img.setAttribute( "src", "/imagens/"+gifaba+"" );
										img.setAttribute( "width", "11" );
										img.setAttribute( "height", "24" );
									tdAnteriorCorrente.appendChild( img );
									
			            			td.style.color  = cor_fonteaba;
			            			td.className  = 'fundoNosel';			            			
								}
							}
						}
					}
				}
				/**
				* SETANDO IMAGEM PARA A ULTIMA TD
				*/
				if (gifaba == 'aba_esq_sel_ini.gif' || gifaba == 'aba_esq_sel.gif'){
			        gifaba = 'aba_dir_sel_fim.gif';
			    } else {
			        gifaba = 'aba_nosel_fim.gif';
			    }
			    
			    $('<?php echo $this->tabelaAba; ?>_td_imgfim').innerHTML = "";					    
			    var img = document.createElement( "img" );
					img.setAttribute( "src", "/imagens/"+gifaba+"" );
					img.setAttribute( "width", "11" );
					img.setAttribute( "height", "24" );
				$('<?php echo $this->tabelaAba; ?>_td_imgfim').appendChild( img );
			}
			
			var div = obj.id.replace('td','div');
			
			if( $( div ) && $( div ).innerHTML != "" ){
				$('<? echo $idDiv; ?>').hide();
				$( div ).show();
			}
			else{
				new Ajax.Request(url,
				{  
					method: 'post',   
					parameters: parametros,
					onLoading: $('<? echo $idDiv; ?>').update("<div style=\"width:100%;text-align:center;margin-top:15%\" ><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Carregando...</div>"),  
					asynchronous: false, 
					onComplete: function(r)
					{
						
						<?if($this->boCache){ ?>
							extraiScriptAbaAjax( r.responseText ); 
							var div = obj.id.replace('td','div');
							$('<? echo $idDiv; ?>').hide();
							$( div ).update("<div style=\"width:100%;text-align:center;margin-top:15%\" ><img src=\"../imagens/carregando.gif\" border=\"0\" align=\"middle\"><br />Carregando...</div>"),
							$( div ).show();
							$( div ).update(r.responseText);
							
						<? }else{ ?>
							extraiScriptAbaAjax( r.responseText );
							$('<? echo $idDiv; ?>').update(r.responseText);
						<? } ?>
						
					}
				});
			}
		}
	 </script>
	    <?php
	    
	    $div = "<div class=\"div_abas_ajax\" id=\"$idDiv\" ></div>";	
	    
	    $i = 0; 
	    $abaInicio = "";
	    foreach ($arAbas as $atributo => $aba){
	        extract($aba);
	        
		     if($this->boCache){
		    	$div .= "<div class=\"div_abas_ajax\" id=\"{$this->tabelaAba}_div_nome_$i\" ></div>";	
		    }
	        
		    if($funcao){
		    	$arrExecutaFuncaoJS[] = "jQuery(\"#{$this->tabelaAba}_td_nome_$i\").click(function () {
								        	$funcao;
								        	return false;
								        });";
		    }
		    		    
	        $arElementos = array();
	        if($elemento){
	        	if(is_array($elemento)){
	        		foreach($elemento as $idElemento){
	        			if(count($arElementos)){
	        				$arElementos[] = "'&".$idElemento."='+$('$idElemento').value";
	        			} else {
	        				$arElementos[] = $idElemento."='+$('$idElemento').value";	        				
	        			}
	        		}
	        	}
	        }
	
			$gifaba = 'aba_nosel_ini.gif';
			$giffundo_aba = 'aba_fundo_nosel.gif';
	        $cor_fonteaba = '#4488cc';
	        
	        if($arElementos){
	        	$parametro = $parametro."&".implode(" + ",$arElementos)."+'";	
	        }
	        
	        $evento = "clicaAba(this,'$url','$parametro')";
	        
	        $menu .= "<td height=\"20\" valign=\"top\" id=\"{$this->tabelaAba}_td_imgini_$i\">
	        			<img src=\"../imagens/$gifaba\" width=\"11\" height=\"20\" alt=\"\" border=\"0\">
	        		  </td>"
	               . "<td height=\"20\" id=\"{$this->tabelaAba}_td_nome_$i\" align=\"center\" valign=\"middle\" background=\"../imagens/$giffundo_aba\" style=\"color:$cor_fonteaba; padding-left: 10px; padding-right: 10px;cursor:pointer;\" onclick=\"$evento\">";
	
			if($aba['padrao']){
				$eventoFuncao = "<script>";
				if($this->boJqueryNoConflict){
					$eventoFuncao .= "jQuery.noConflict();";
				}
				$eventoFuncao .= "
								jQuery(document).ready(
								function () {
										var obj = document.getElementById('{$this->tabelaAba}_td_nome_$i');
										clicaAba(obj,'$url','$parametro');		
								});
								</script>";
			} 
	               
	        $menu .= $aba['descricao'] . "</td>";
	        $i++;
	    }
		$gifaba = 'aba_nosel_fim.gif';
		
	    $menu .= "<td height=\"20\" valign=\"top\" id=\"{$this->tabelaAba}_td_imgfim\">
					<img src=\"../imagens/$gifaba\" width=\"11\" height=\"20\" alt=\"\" border=\"0\">
				  </td>
				  </tr></table></td></tr></table>";
	    
	    echo $script;
	    echo $menu;
	    echo $div;
	    echo $eventoFuncao;
	    
	    if(count($arrExecutaFuncaoJS)){
	    	$ExecutaJS = "<script>";
	    	foreach($arrExecutaFuncaoJS as $funcaoJS){
				$ExecutaJS .= $funcaoJS." ; ";
	    	}
	    	$ExecutaJS .= "</script>";
	    	
	    	echo $ExecutaJS;
	    }
	    
	}
}