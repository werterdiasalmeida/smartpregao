
<?php

class IdiomaEntidade extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "agenda.idiomaentidade";

	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array( "ideid" );

	/**
	 * Atributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos     = array(
									  	'ideid'	 => null, 
    									'ideleitura' => null,
    									'ideescrita' => null,
    									'ideconversacao' => null,
    									'idecompreensao' => null,
    									'idiid' => null,
    									'entid' => null,
    									'idestatus' => null
	);
		
		
	function listarPorEntidade(  $entid ){
			
		$sql = "SELECT
		    		idi.ididsc,
    				CASE ide.ideleitura 
    					WHEN 'F' THEN 'Facilidade'
             		    WHEN 'D' THEN 'Dificuldade'
       				END AS ideleitura,
    				CASE ide.ideescrita 
    					WHEN 'F' THEN 'Facilidade'
             		    WHEN 'D' THEN 'Dificuldade'
       				END AS ideescrita,
     				CASE ide.ideconversacao
    					WHEN 'F' THEN 'Facilidade'
             		    WHEN 'D' THEN 'Dificuldade'
       				END AS ideconversacao,
     				CASE ide.idecompreensao
    					WHEN 'F' THEN 'Facilidade'
             		    WHEN 'D' THEN 'Dificuldade'
       				END AS idecompreensao, 		
					ide.ideid
				 FROM agenda.idiomaentidade ide 
				 JOIN 
					agenda.idioma idi ON idi.idiid = ide.idiid	
										AND idi.idistatus = 'A'							
				 WHERE 
					ide.idestatus = 'A' AND ide.entid ={$entid}";

		$rs = $this->carregar($sql);
		return $rs;
	}

	function verificaVinculo(  $entid , $idiid){
			
		$sql = "SELECT
		    		ideid
				 FROM 
				 	agenda.idiomaentidade 							
				 WHERE 
					idestatus = 'A' AND entid ={$entid} AND idiid = {$idiid}";

		$rs = $this->pegaUm($sql);
		return $rs;
	}
	
	public function idiomasFormatadosPorEntidade($entidade){

		$out = array();
		$sql = "SELECT DISTINCT
				idi.idiid AS codigo,
				idi.ididsc AS descricao
			FROM 
				agenda.idioma idi
			INNER JOIN
				agenda.idiomaentidade ide ON ide.idiid = idi.idiid
											AND ide.idestatus ='A'
			WHERE
				ide.entid = {$entidade} AND idi.idistatus='A'";

		$rsIdioma = $this->carregar($sql);

		if($rsIdioma){
			foreach ($rsIdioma as $valueIdioma){
				$out[] = "<b>{$valueIdioma['descricao']}</b>";
				$sql = "SELECT DISTINCT
							CASE ide.ideleitura 
								WHEN 'D' THEN '<b>Leitura:</b> Com Dificuldade'
			            		WHEN 'F' THEN '<b>Leitura:</b> Com Facilidade'
			       			END AS ideleitura,
							CASE ide.ideescrita
								WHEN 'D' THEN '<b>Escrita:</b> Com Dificuldade'
			            		WHEN 'F' THEN '<b>Escrita:</b> Com Facilidade'
			       			END AS ideescrita,
							CASE ide.ideconversacao
								WHEN 'D' THEN '<b>Conversação:</b> Com Dificuldade'
			            		WHEN 'F' THEN '<b>Conversação:</b> Com Facilidade'
			       			END AS ideconversacao,
							CASE ide.idecompreensao
								WHEN 'D' THEN '<b>Compreensão:</b> Com Dificuldade'
			            		WHEN 'F' THEN '<b>Compreensão:</b> Com Facilidade'
			       			END AS idecompreensao
					FROM 
						agenda.idiomaentidade ide
					WHERE 
						ide.idiid = {$valueIdioma['codigo']} AND ide.entid = {$entidade}  AND ide.idestatus ='A'"; 

				$rsSituacao = $this->pegaLinha($sql);
				if(is_array($rsSituacao)){
					foreach ($rsSituacao as $situacao){
						$out[] = "&nbsp;&nbsp;&nbsp;&nbsp; {$situacao}";
					}
				}
			}
		}
		if (is_array($out))
		$out = implode('<br>', $out);
		return $out;
	}
}
