<?php
/**
 * Classe ManipulaTabela
 * Classe para manipulação de campos da tabela
 * @author Juliano Meinen
 * @since 20/08/2010
 */
class ManterTabela extends cls_banco
	{

	private $sisid;
	private $tableName;
	private $schemaName;
	private $pk;
	private $initSizeTextArea = 150;
	public 	$sql;
	private	$sqlOption;
	private $permission = array(
								"inserir", //Permite inserir registros na tabela
								"alterar", //Permite alterar registros da tabela
								"excluir", //Permite excluir registros da tabela
								"comentar_tabela", //Permite adicionar / editar comentários sobre a tabela
								"comentar_coluna", //Permite adicionar / editar comentários sobre a coluna da tabela
								"campo_descricao", //Permite informar qual o campo descritivo da tabela em casso de FK
								"campo_lista", //Permite definir quais os campos aparecerão na lista dos registros da tabela
								"inserir_tabela_apoio" //Permite inserir novas tabelas de apoio em seguranca.tblapoio e seguranca.tbasistema
								);
	private $codigoSqlOptions;
	private $descricaoSqlOptions;
	private $sqlOptionsTable;
	private $campoTabela;
	private $arrError = array();
	private $arrMessage = array();
	private $arrTableFK = array();
	private $boUpdate;
		
	public function __construct( $sisid = null)
	{
		parent::__construct();
		
		if( is_integer($sisid) ){
			$this->setSisid($sisid);
		}elseif(!is_null($sisid)){
			$this->setSchemaName($sisid);
		}

	}
	
	/**
	 * Função setSisid
	 * Método usado setar o sisid
	 * @access public
	 * @author Juliano Meinen
	 * @since 20/08/2010
	 */
	public function setSisid($sisid)
	{
		$this->sisid = $sisid;
		$this->setSchemaNamePorSisid($this->sisid);
	}
	
	/**
	 * Função setSchemaNamePorSisid
	 * Método usado setar o schema pelo sisid
	 * @access public
	 * @author Juliano Meinen
	 * @since 09/09/2010
	 */
	public function setSchemaNamePorSisid($sisid)
	{
		$sql = sprintf("select sisdiretorio from seguranca.sistema where sisid = %s",$sisid);
		$this->setSchemaName( $this->pegaUm($sql) );
	}
	
	/**
	 * Função setSchema
	 * Método usado setar o schema do banco de dados
	 * @access public
	 * @author Juliano Meinen
	 * @since 20/08/2010
	 */
	public function setSchemaName($schemaName)
	{
		$this->schemaName = $schemaName;
	}
	
	/**
	 * Função getSchemaName
	 * Método que retorna o nome do shcema 
	 * @access public
	 * @author Juliano Meinen
	 * @since 20/08/2010
	 */
	public function getSchemaName()
	{
		return $this->schemaName;
	}
	
	/**
	 * Função setTable
	 * Método usado setar o nome da tabela 
	 * @access public
	 * @author Juliano Meinen
	 * @since 20/08/2010
	 */
	public function setTableName($tableName)
	{
		$this->tableName = $tableName;
	}
	
	/**
	 * Função getTableName
	 * Método que retorna o nome da tabela 
	 * @access public
	 * @author Juliano Meinen
	 * @since 20/08/2010
	 */
	public function getTableName()
	{
		return $this->tableName;
	}
	
	/**
	 * Função getPK
	 * Método seta a PRIMARY KEY 
	 * @access private
	 * @author Juliano Meinen
	 * @since 24/08/2010
	 */
	public function getPK($schema = null,$table = null)
	{
		$sql = "SELECT
					pg_attribute.attname AS coluna
				FROM 
					pg_class
				JOIN 
					pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
				JOIN 
					pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = 'f'
				JOIN
					pg_type ON pg_type.oid = pg_attribute.atttypid
				JOIN 
					pg_index ON pg_index.indrelid=pg_class.oid
				LEFT JOIN
					pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum)
				WHERE 
					pg_class.relname = '".($table ? $table : $this->getTableName())."'
				AND
					pg_namespace.nspname = '".($schema ? $schema : $this->getSchemaName())."'
				AND 
					pg_attribute.attnum > 0
				AND 
					pg_attribute.attrelid = pg_class.oid
				AND 
					pg_attribute.atttypid = pg_type.oid
				AND
					pg_constraint.contype = 'p'";

		if($schema && $table){
			return $this->pegaUm($sql);
		}else{
			$this->pk = !$this->pk ? $this->pegaUm($sql) : $this->pk;
			return $this->pk;
		}
	}
	
	/**
	 * Função getSQL
	 * Método que retorna o SQL 
	 * @access public
	 * @author Juliano Meinen
	 * @since 23/08/2010
	 */
	public function getSQL()
	{
		return $this->sql;
	}
	
	/**
	 * Função getShemasDB
	 * Método usado para recuperar os schema do BD
	 * @return array|bool - Retorna um array com o nome dos schemas ou false se hover erro 
	 * @access public
	 * @author Juliano Meinen
	 * @since 26/08/2010
	 */
	public function getShemasDB($sisid = null)
	{
		$this->sisid = !$sisid ? $_SESSION['sisid'] : $sisid;
		
//		$sql = "SELECT 
//									lower(n.nspname) codigo,
//									lower(n.nspname) as descricao  
//								FROM 
//									pg_namespace n
//								WHERE
//									n.nspname not like 'pg\\_%' -- no catalogs
//								AND
//									n.nspname != 'information_schema' -- no information_schema
//								ORDER BY 
//									nspname";	
		$sql = sprintf("SELECT DISTINCT
									lower(tba.tbaschema) codigo,
									lower(tba.tbaschema) as descricao  
								FROM 
									seguranca.tblapoio tba
								INNER JOIN
									seguranca.tblsistema tbs ON tbs.tbaid = tba.tbaid
								WHERE
									sisid = %s
								AND
									tbastatus = 'A'
								",$this->sisid);
		
		return $this->carregar($sql);
	}
	
	/**
	 * Função getShemaTablesPorSisid
	 * Método usado para recuperar as tabelas do schema por meio do sisid
	 * @return array|bool - Retorna um array com o nome das tabelas ou false se hover erro 
	 * @access public
	 * @author Juliano Meinen
	 * @since 31/08/2010
	 */
	public function getShemaTablesPorSisid($sisid = null)
	{
		$this->sisid = !$sisid ? $_SESSION['sisid'] : $sisid;
		
		if( !in_array("inserir_tabela_apoio",$this->getPermission() ) ):
			$arrWhere[] = "tba.tbatabela != 'tblsistema'";
			$arrWhere[] = "tba.tbatabela != 'tblapoio'";
		endif;
		
		$sql = sprintf("SELECT DISTINCT
									lower(tba.tbaschema) || '.' || lower(tba.tbatabela) codigo,
									lower(tba.tbatabela) as descricao
								FROM 
									seguranca.tblapoio tba
								INNER JOIN
									seguranca.tblsistema tbs ON tbs.tbaid = tba.tbaid
								WHERE
									sisid = %s
								AND
									tba.tbaschema = '%s'
								AND
									tbastatus = 'A'
								".($arrWhere ? " AND ".implode(" AND ", $arrWhere) : "")."
								", $this->sisid, $this->getSchemaName());

		return $this->carregar($sql);
		//return $this->getShemaTables();
	}
	
    /**
	 * Função getShemaTables
	 * Método usado para recuperar as tabelas do schema
	 * @return array|bool - Retorna um array com o nome das tabelas ou false se hover erro 
	 * @access public
	 * @author Juliano Meinen
	 * @since 20/08/2010
	 */
	public function getShemaTables($arrWhere = null)
	{
		$sql = "SELECT 
									lower(n.nspname) || '.' || lower(c.relname) as codigo,
									lower(c.relname) as descricao  
								FROM 
									pg_namespace n, pg_class c
								WHERE 
									n.oid = c.relnamespace
								AND
									c.relkind = 'r'     -- no indices
								AND
									n.nspname not like 'pg\\_%' -- no catalogs
								AND
									n.nspname != 'information_schema' -- no information_schema
									"
								 .(($arrWhere && is_array($arrWhere)) ? " AND ".implode(" AND ",$arrWhere) : " AND n.nspname = '".$this->getSchemaName()."' ")."
								ORDER BY 
									nspname,
									relname";
		return $this->carregar($sql);
	}
	
	/**
	 * Função getTableColumns
	 * Método usado para recuperar as colunas da tabela
	 * @return array|bool - Retorna um array com o nome das colunas ou false se hover erro 
	 * @access public
	 * @author Juliano Meinen
	 * @since 20/08/2010
	 */
	public function getTableColumns($schema = null,$table = null,$arrWhere = null,$arrOrder = null,$setSql = true)
	{
		$sql = "SELECT DISTINCT
					pg_attribute.attnum as numero,
					pg_namespace.nspname AS esquema,
					pg_class.relname AS tabela,
					pg_attribute.attname AS coluna,
					pg_type.typname as tipo,
					pg_attribute.atttypmod - 4 as tamanho,
					(SELECT description FROM pg_description pd WHERE pg_attribute.attrelid = pd.objoid AND pg_attribute.attnum = pd.objsubid LIMIT 1) AS comentario,
					pg_attribute.attnotnull as naonulo,
					--pg_constraint.*,
					pg_get_constraintdef(pg_constraint.oid) as restricao
				FROM 
					pg_class
				JOIN 
					pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
				JOIN 
					pg_attribute ON pg_attribute.attrelid = pg_class.oid AND pg_attribute.attisdropped = 'f'
				JOIN
					pg_type ON pg_type.oid = pg_attribute.atttypid
				JOIN 
					pg_index ON pg_index.indrelid=pg_class.oid
				LEFT JOIN
					pg_constraint ON (pg_attribute.attrelid = pg_constraint.conrelid AND pg_constraint.conkey[1] = pg_attribute.attnum AND pg_constraint.contype != 'u')
				WHERE 
					pg_class.relname = '".($table ? $table : $this->getTableName())."'
				AND
					pg_namespace.nspname = '".($schema ? $schema : $this->getSchemaName())."'
				AND 
					pg_attribute.attnum > 0
				AND 
					pg_attribute.attrelid = pg_class.oid
				AND 
					pg_attribute.atttypid = pg_type.oid
				".(($arrWhere && is_array($arrWhere)) ? " AND ".implode(" AND ",$arrWhere) : "")."
				ORDER BY
					".(($arrOrder && is_array($arrOrder)) ? implode(",",$arrOrder)."," : "")."
					pg_attribute.attnum";
		$setSql ? $this->sql = $sql : "";
		return $this->carregar($sql);
	}
	
	
	/**
	 * Função getInputColumn
	 * Método usado contruir o campo input de edição da coluna de acordo com o tipo e tamanho
	 * @return array|bool - Retorna um array com com o tipo do campo e seuas atributos 
	 * @access public
	 * @author Juliano Meinen
	 * @since 20/08/2010
	 */
	public function getInputColumn($arrColuna,$value = null,$editPK = false)
	{
		extract($arrColuna);
		if($tipo == "varchar" || $tipo == "bpchar"){
			$arrAttb["type"] = "text";
		}
		
		if($tipo == "timestamp" || $tipo == "date"){
			$arrAttb["type"] = "data";
		}
		
		
		if($tipo == "text"){
			$arrAttb["type"] = "textarea";
		}
		
		if($tipo == "bool"){
			$arrAttb["type"] = "select";
			$arrAttb["options"] = array( array("codigo" => "t" ,"descricao" => "Sim") , array("codigo" => "f" ,"descricao" => "Não"));
		}
				
		if(strstr($tipo,"int")){
			$arrAttb["type"] = "text";
			$tamanho = str_replace("int","",$tipo);
			$tamanho = !$tamanho ? 10 : $tamanho;
			$arrAttb["mascara"] = "[.###]";
			if(!$restricao && $value)
				$value = number_format($value,2,'','.');
		}
		
		if(strstr($tipo,"float")){
			$arrAttb["type"] = "text";
			$tamanho = str_replace("float","",$tipo);
			$tamanho = !$tamanho ? 10 : $tamanho;
			$arrAttb["mascara"] = "[.###],##";
			if($value)
				$value = number_format($value,2,',','.');
		}
		
		if(strstr($tipo,"numeric")){
			$arrAttb["type"] = "text";
			$tamanho = str_replace("numeric","",$tipo);
			$tamanho = !$tamanho ? 10 : $tamanho;
			$arrAttb["mascara"] = "[.###],##";
			if($value)
				$value = number_format($value,2,',','.');
		}

		if($tamanho > 0){
				$arrAttb["size"] = $tamanho + 5;
				$arrAttb["maxlength"] = $tamanho;
				if($tamanho > $this->initSizeTextArea)
					$arrAttb["type"] = "textarea";
		}
		if(strstr($restricao,"PRIMARY KEY")){
			if($editPK){
				if($_POST['inp_hdn_acao'] == "carregar"){
					$arrAttb["readonly"] = "readonly";
				}
				//unset($arrAttb["mascara"]);	
			}else{
				$arrAttb["readonly"] = "readonly";	
			}
		}
		
		if(strstr($restricao,"FOREIGN KEY")):
			unset($arrAttb);
			$arrAttb['type'] = "select";
			$arrAttb['options'] = $this->getOptionsForeingKey($restricao);
			$schema = $this->getSchemaForeingKey($restricao);
			$tbl = $this->getTableForeingKey($restricao);
			if( count($arrAttb['options']) >= 30 ):
				$arrAttb['sqlOptions'] = $this->sqlOption;
				$arrAttb['sqlOptionsTable'] = $this->sqlOptionsTable;
				$arrAttb['codigoSqlOptions'] = $this->codigoSqlOptions;
				$arrAttb['descricaoSqlOptions'] = $this->descricaoSqlOptions;
			endif;
			
			if(in_array("campo_descricao",$this->permission)):
			
				$arrWhere[] = "pg_get_constraintdef(pg_constraint.oid) is null";
				
				$arrColumns = $this->getTableColumns($schema,$tbl,$arrWhere,null,false);
							
				if(is_array($arrColumns)):
					foreach($arrColumns as $column):
						$arrCol[] = array("codigo" => $column['coluna'] ,"descricao" => $column['coluna']);
					endforeach;
				endif;
				$campoDesc = $this->getColumnTableDescription($schema,$tbl);
				$arrCampoDesc = explode(",",$campoDesc);
				if(is_array($arrCol)):
					$combo = $this->monta_combo_multiplo("inp_desc_".$schema."_".$tbl,$arrCol,"S","",'','','',4,'200px',$arrCampoDesc,"inp_desc_".$schema."_".$tbl,true);
					$div = '<div id="div_desc_'.$schema.'_'.$tbl.'" class="hidden absolute" >
									'.$combo.'
									<input type="button" onclick="salvarCampoDescTabela(\''.$coluna.'\',\''.$schema.'\',\''.$tbl.'\',\''.$value.'\')" name="btn_desc_'.$schema.'_'.$tbl.'" value="OK" />
									<img class="link fechar" onclick="fechar(this)" title="Fechar" src="../imagens/sair.gif" />
								</div>';
					
					
						$arrAttb['campoTabela'] = !$campoDesc ? " <img class=\"link img_middle\" id=\"img_".$schema."_".$tbl."\" src=\"../imagens/gif_inclui.gif\" onclick=\"addTableColumnDesc('$schema','$tbl')\" title=\"Clique aqui para adicionar o campo de descrição da tabela.\" />" : " <img class=\"link img_middle\" id=\"img_".$schema."_".$tbl."\"  onclick=\"addTableColumnDesc('$schema','$tbl')\" src=\"../imagens/alterar.gif\" style=\"background-color:#FFFFFF\" title=\"Clique aqui para alterar o campo de descrição da tabela.\" />";
						$arrAttb['campoTabela'].= $div;
					
				endif;
			
			endif;
			
		endif;
		
		if(strstr($coluna,"status")):
			unset($arrAttb);
			$arrAttb['type'] = "select";
			$arrAttb['options'] = array( array("codigo" => "A" ,"descricao" => "Ativo"), array("codigo" => "I" ,"descricao" => "Inativo"),array("codigo" => "H" ,"descricao" => "Histórico") );
			$arrAttb['defaultValue'] = "A";
		endif;
			
		if($naonulo == "t"){
			$arrAttb["class"] = strlen($arrAttb["class"]) > 0 ? $arrAttb["class"]." required" : "required";
		}
		
		$arrAttb["id"] = $coluna;
		$arrAttb["name"] = $coluna;
		$arrAttb["comentario"] = htmlspecialchars( str_replace( array(chr(13),chr(10)),"<br>",$comentario) ); 
		$required = "N";

		if(in_array("data",$arrAttb)):
			foreach($arrAttb as $at => $vl){
				if($at == "class" && strstr($vl,"required")){
					$required = "S";
				}
			}
			$value = $value ? date("d/m/Y",strtotime($value)) : null;
			return campo_data2($arrAttb['name'],$required,($arrAttb['readonly'] ? "N" : "S"),'','##/##/####',$arrAttb["comentario"],'',$value);
		elseif(in_array("textarea",$arrAttb)):
			foreach($arrAttb as $at => $vl){
				if($at == "class" && strstr($vl,"required")){
					$required = "S";
				}
			}
			return campo_textarea($arrAttb['name'],$required,($arrAttb['readonly'] ? "N" : "S"),'',($arrAttb["cols"] ? $arrAttb["cols"] : "80"),($arrAttb["rows"] ? $arrAttb["rows"] : "5"),$arrAttb['maxlength'],'','',$arrAttb["comentario"],'','',$value);		
		elseif(in_array("select",$arrAttb)):
			foreach($arrAttb as $at => $vl){
				if($at == "id" || $at == "name" || $at == "size" || $at == "maxlength" || $at == "class" || $at == "readonly"){
					$arrAtributos[] = "$at=\"$vl\"";
				}
				if($at == "class" && strstr($vl,"required")){
					$required = "S";
				}
			}
			if(is_array($arrAttb['options']) && !$arrAttb['sqlOptions']):
				$select = $this->monta_combo($arrAttb['name'],$arrAttb['options'],($arrAttb['readonly'] ? 'N' : 'S'),"Selecione...",'',"",$arrAttb["comentario"],$arrAttb['size'],$required,$arrAttb['id'],'', ($value ? $value : $arrAttb['defaultValue']) );
			elseif(is_array($arrAttb['options']) && $arrAttb['sqlOptions']):
				global ${$arrAttb['name']};
				${$arrAttb['name']} = array("value" => $value, "descricao" => $this->getDescricaoValue($arrAttb['sqlOptionsTable'],$arrAttb['descricaoSqlOptions'],$arrAttb['name'],$value));
				campo_popup($arrAttb['name'],$arrAttb['sqlOptions'],'Selecione','','400x400','30',
					array(
						array("codigo" => $arrAttb['codigoSqlOptions'],"descricao" => "Código","numeric" => "1"),
						array("codigo" => $arrAttb['descricaoSqlOptions'],"descricao" => "Descricão","numeric" => "0"))
									,1);
			elseif(!is_array($arrAttb['options']) && $required):
				$this->setError("Tabela '{$arrAttb['options']}' referente ao campo '{$arrAttb['name']}' não possui registros.");
			endif;
			return $select.$arrAttb['campoTabela'];
		else:
			foreach($arrAttb as $att => $valor):
				if($att == "class" && strstr($valor,"required"))
					$required = "S";
			endforeach;
			return campo_texto($arrAttb['name'],$required,($arrAttb['readonly'] ? "N" : "S"),'',$arrAttb['size'],$arrAttb['maxlength'],$arrAttb['mascara'],$arrAttb['hidden'],'',$arrAttb["comentario"],'',"id='".$arrAttb['name']."'",$arrAttb['onkeypress'],$value);
		endif;
	}
	
	/**
	 * Função getHtmlRequired
	 * Método usado para contruir a indicação de campo obrigatório
	 * @return string - Retorna uma string com o html 
	 * @access public
	 * @author Juliano Meinen
	 * @since 23/08/2010
	 */
	public function getHtmlRequired()
	{
		return "<img class=\"img_required\" src=\"../imagens/obrig.gif\" />";
	}
	
	/**
	 * Função getOptionsForeingKey
	 * Método usado para obter os valores do combo de FKs
	 * @return array|bool - Retorna um array com as opções ou falso se houver erro 
	 * @access public
	 * @author Juliano Meinen
	 * @since 23/08/2010
	 */
	private function getOptionsForeingKey($restricao)
	{
		$restricao = str_replace(array("FOREIGN KEY","REFERENCES"," "),"",$restricao);
		$restricao = substr($restricao,1,strpos($restricao, "ON")-1);
		
		$fk = substr($restricao,0,strpos($restricao, ")"));
		$tbl = str_replace("$fk)","",substr($restricao,0,strpos($restricao,"(")));
		$fk2 = str_replace( array( "$fk)$tbl(" ,")" ),"",$restricao);

		if(!strstr($tbl,".")){
			$arrWhere1[] = "n.nspname = '".$this->getSchemaName()."'";
			$arrWhere1[] = "c.relname = '$tbl'";
			$arrWhere2[] = "n.nspname = 'public'";
			$arrWhere2[] = "c.relname = '$tbl'";
			if(is_array($this->getShemaTables($arrWhere1))){
				$tbl = $this->getSchemaName().".".$tbl;
			}elseif(is_array($this->getShemaTables($arrWhere2))){
				$tbl = "public.".$tbl;
			}else{
				$this->setError("Tabela '$tbl' não existente no schema 'public' ou '".$this->getSchemaName()."'.");
				return false;
			}
		}

		$dado = explode(".",$tbl);

		$sql = sprintf("select tbmcampodesc from seguranca.tblmapa where tbmschema = '%s' and tbmtabela = '%s' and tbmstatus = 'A'",$dado[0],$dado[1]);
		
		$campos = $this->pegaUm($sql);

		if($campos){
			$arrCol = explode(",",$campos);
		}else{
			$arrWhereDesc[] = "pg_attribute.attname != '$fk2'";
			$arrWhereDesc[] = "pg_get_constraintdef(pg_constraint.oid) IS NULL";
			$arrWhereDesc[] = "( pg_type.typname like ('%varchar%') OR pg_type.typname like ('%text%') )";
			$arrColunas = $this->getTableColumns($dado[0],$dado[1],$arrWhereDesc);
			if(is_array($arrColunas)):
				foreach($arrColunas as $col):
					$arrCol[] = $col['coluna'];
				endforeach;
			else:
				$arrCol[] = $fk2;
			endif;
			$this->setError("A tabela '".$dado[0].".".$dado[1]."' não possui campo descritivo informado em 'seguranca.tblmapa'.");
			$this->campoTabela = false;
		}
		
		$sql = "select
					$fk2,
					".(is_array($arrCol) ? implode(" || ' - ' ||",$arrCol) : "*")."
				from
					$tbl
				".(is_array($arrCol) && !in_array("*",$arrCol) ? " ORDER BY ".implode(",",$arrCol) : "")."
				limit
					50";
		$this->sqlOption = "select
								$fk2 as codigo,
								".(is_array($arrCol) ? implode(" || ' - ' ||",$arrCol) : "*")." as descricao
							from
								$tbl
							where 1=1	
							".(is_array($arrCol) && !in_array("*",$arrCol) ? " ORDER BY ".implode(",",$arrCol) : "")." LIMIT 2000";
		$this->codigoSqlOptions = $fk2;
		$this->descricaoSqlOptions = (is_array($arrCol) ? implode(" || ' - ' ||",$arrCol) : "*");
		$this->sqlOptionsTable = $tbl;
								
		$arrOptions = $this->carregar($sql);

		if(is_array($arrOptions)):
			foreach($arrOptions as $option):
				$arrOp[] = array( "codigo" => $option[$fk2] , "descricao" => str_replace($option[$fk2]." - ","",implode(" - ",$option)) );
			endforeach;
			return $arrOp;
		else:
			return $tbl;
		endif;
		
		
	}
	
	/**
	 * Função setError
	 * Método usado para empilhar os erros 
	 * @access private
	 * @author Juliano Meinen
	 * @since 23/08/2010
	 */
	private function setError($error)
	{
		$this->arrError[] = $error;
	}
	
	/**
	 * Função getError
	 * Método usado para retornar a pilha de erros 
	 * @access public
	 * @author Juliano Meinen
	 * @since 23/08/2010
	 */
	public function getError()
	{
		if(count($this->arrError)){
			foreach($this->arrError as $chave => $erro):
				$html .= "<p class=\"red bold\">".($chave + 1)." - ".( !is_array($erro) ? $erro : implode(" , ",$erro))."</p>";
			endforeach;
			return $html;
		}else{
			return false;
		}
	}

	
	/**
	 * Função montaListaTabelaAjax
	 * Método usado montar a lista de registros da tabela
	 * @access public
	 * @author Juliano Meinen
	 * @since 23/08/2010
	 */
	public static function montaListaTabelaAjax($arrParams = array())
	{
		include APPRAIZ."includes/classes/html/TElement.class.inc";
		include APPRAIZ."includes/classes/html/TTable.class.inc";
		include APPRAIZ."includes/classes/html/TTableRow.class.inc";
		include APPRAIZ."includes/classes/html/TTableCell.class.inc";
		include APPRAIZ."includes/classes/view/Lista.class.inc";
		include APPRAIZ."includes/classes/PaginacaoAjax.class.inc";
		
		extract($arrParams);

		$tbl = new ManterTabela();
		$tbl->setSchemaName($schema);
		$tbl->setTableName($table);
		$arrColumn = $tbl->getTableColumns();
		$tbl->getTableColumns();
		$sql = $tbl->getSQL();

		if(!is_array($arrColumn))
			return false;
			
		foreach($arrColumn as $arrColumn):
			$arrCabecalho[] = $arrColumn['coluna'];
		endforeach;
				
		$sql = "select ".implode(",",$arrCabecalho)." from ".$schema.".".$table." order by ".implode(",",$arrCabecalho);
		
		$arParamCol[0] = array("type" => Lista::TYPESTRING,
							   "html" => "<img src=\"../imagens/alterar.gif\" alt=\"Alterar\"  title=\"Alterar\" class=\"link\" onclick=\"editarTabela('{$arrCabecalho[0]}','".$arrCabecalho[0]."')\" />",
							   "align" => "left");
		
		// ARRAY de parametros de configuração da tabela
		$arConfig = array(//"style" => "width:95%;",
						  "totalLinha" => false,
						  "totalRegistro" => false);
		
		$oPaginacaoAjax = new PaginacaoAjax();
		$oPaginacaoAjax->setNrPaginaAtual($nrPaginaAtual);
		$oPaginacaoAjax->setNrRegPorPagina($nrRegPorPagina);
		$oPaginacaoAjax->setNrBlocoPaginacaoMaximo($nrBlocoPaginacaoMaximo);
		$oPaginacaoAjax->setNrBlocoAtual($nrBlocoAtual);
		$oPaginacaoAjax->setDiv( 'divLista' );
		$oPaginacaoAjax->setCabecalho( $arrCabecalho );
		$oPaginacaoAjax->setParamCol( $arParamCol );
		$oPaginacaoAjax->setSql( $sql );
		$oPaginacaoAjax->setConfig( $arConfig );
		$oPaginacaoAjax->show();
	}
	
	/**
	 * Função montaListaTabela
	 * Método usado montar a lista de registros da tabela
	 * @access public
	 * @author Juliano Meinen
	 * @since 23/08/2010
	 */
	public function montaListaTabela($arrPesquisa = null)
	{
		$busca = $_POST['txt_busca'] ? $_POST['txt_busca'] : false;
		
		if(is_array($arrPesquisa)){
			$arrBusca2 = $arrPesquisa;	
		}elseif($arrPesquisa){
			$arrBusca2[0] = $arrPesquisa;
		}
		
		$sql = sprintf("select tbacampolista from seguranca.tblapoio where tbaschema = '%s' and tbatabela = '%s' and tbastatus = 'A'",$this->getSchemaName(),$this->getTableName());
		$campos = $this->pegaUm($sql);
		$campos = str_replace(" ", "", $campos);
		
		if($campos){
			$arrCol = explode(",",$campos);
			$arrWhere[] = "pg_attribute.attname in ('".implode("','",$arrCol)."')";
		}
						
		$arrColumn = $this->getTableColumns(null,null,$arrWhere);
		if(!is_array($arrColumn))
			return false;
		$cont = 1;
		
		$arrCabecalho[0] = "Ação";
		$arrCampos[0] = str_replace($this->getTableName().".","",$this->getPK($this->getSchemaName(),$this->getTableName()));
		$arrCampos[0] = "'<div style=\"white-space: nowrap\" ><img src=\"../imagens/".( in_array("alterar",$this->permission) ? "alterar" : "alterar_01").".gif\" alt=\"Alterar\"  title=\"Alterar\" class=\"link\" ".(in_array("alterar",$this->permission) ? " onclick=\"editarTabela(\'{$arrCampos[0]}\',\'' || ".$this->getTableName().".".$arrCampos[0]." || '\')\" " : " onclick=\"alert(\'Operação Não Permitida!\')\" "). "/> <img src=\"../imagens/".(in_array("excluir",$this->permission) ? "excluir" : "excluir_01").".gif\" alt=\"Excluir\"  title=\"Excluir\" class=\"link\" ".(in_array("excluir",$this->permission) ? " onclick=\"excluirTabela(\'{$arrCampos[0]}\',' || ".$this->getTableName().".".$arrCampos[0]." || ')\"" : " onclick=\"alert(\'Operação Não Permitida!\')\" " ). " /></div>' as acao";
		
		foreach($arrColumn as $arrColumn):
			$arrWhere1 = null;
			$arrWhere2 = null;
			$arrColumn['coluna'] = $this->getTableName().".".$arrColumn['coluna'];
			$cabecalho[] = $arrColumn['coluna'];
			
			if(strstr($arrColumn['comentario'],"<label>") && strstr($arrColumn['comentario'],"</label>")){
				$arrColumn['label'] = null;
				$arrCabecalho[] = $this->getLabel($arrColumn['comentario']);
			}else{
				$arrCabecalho[] = $arrColumn['coluna'];
			}
			
			if($arrColumn['tipo'] == "bool"){
				$arrColumn['coluna'] = "( CASE WHEN ".$arrColumn['coluna']." is true THEN 'Sim' ELSE 'Não' END) AS ".str_replace($this->getTableName().".","",$arrColumn['coluna']);
			}
			if($arrColumn['tipo'] == "date" || $arrColumn['tipo'] == "timestamp"){
				$arrColumn['coluna'] = "to_char( ".$arrColumn['coluna']." , 'DD/MM/YYYY') AS ".str_replace($this->getTableName().".","",$arrColumn['coluna']);
			}
			if(strstr($arrColumn['restricao'],"FOREIGN KEY")){
				$restricao = str_replace(array("FOREIGN KEY","REFERENCES"," "),"",$arrColumn['restricao']);
				$restricao = substr($restricao,1,strpos($restricao, "ON")-1);
				$fk = substr($restricao,0,strpos($restricao, ")"));
				$tbl = str_replace("$fk)","",substr($restricao,0,strpos($restricao,"(")));
				$fk2 = str_replace( array( "$fk)$tbl(" ,")" ),"",$restricao);
				
				if(!strstr($tbl,".")){
					$arrWhere1[] = "n.nspname = '".$this->getSchemaName()."'";
					$arrWhere1[] = "c.relname = '$tbl'";
					$arrWhere2[] = "n.nspname = 'public'";
					$arrWhere2[] = "c.relname = '$tbl'";
					if(is_array($this->getShemaTables($arrWhere1))){
						$schema = $this->getSchemaName();
					}elseif(is_array($this->getShemaTables($arrWhere2))){
						$schema = "public";
					}else{
						$schema = false;
					}
				}else{
					$dado = explode(".",$tbl);
					$schema = $dado[0];
					$tbl = $dado[1];
				}
				
				if($schema){
					$sql = sprintf("select tbmcampodesc from seguranca.tblmapa where tbmschema = '%s' and tbmtabela = '%s' and tbmstatus = 'A'",$schema,$tbl);
					$campos = $this->pegaUm($sql);
					if($campos){
						$arrCol = explode(",",$campos);
						$arrJoin[] = "".$schema.".".$tbl." as tbl$cont ON tbl$cont.".$fk2." = ".$this->getTableName().".".$fk;
						$arrColumn['coluna'] = "tbl$cont.".implode(" || ' - ' || tbl$cont.",$arrCol);
					}
				}
				$arrColumn['tipo'] = "varchar";
				$cont++;
			}
			
			$arrCampos[] = $arrColumn['coluna'];
			
			if($busca && ($arrColumn['tipo'] == "varchar" || $arrColumn['tipo'] == "bpchar" || $arrColumn['tipo'] == "text")){
				$arrBusca[] = "public.removeacento(".$arrColumn['coluna']."::text) ilike public.removeacento(('%$busca%'))";
			}
			if($busca && (strstr($arrColumn['tipo'],"numeric") || strstr($arrColumn['tipo'],"float") || strstr($arrColumn['tipo'],"int"))){
				$busca = str_replace(".","",$busca);
				$busca = str_replace(",",".",$busca);
				$arrBusca[] = $arrColumn['coluna']."::text = '$busca'";
			}
		endforeach;
		
		//Verifica de a tabela possui campo de Status
		if($columnStatus = $this->getTableColumns(null,null,array("pg_attribute.attname ilike ('%status%')"))){
			if($columnStatus[0]['coluna']){
				$arrBusca2[] = $columnStatus[0]['coluna']." = 'A'";
			}
		}
				
		$sql = "select ".implode(",",$arrCampos)." from ".$this->getSchemaName().".".$this->getTableName().( $arrJoin ? " left join ".implode(" left join ",$arrJoin) : "" )." ".($busca && is_array($arrBusca) ? " where ( ".implode(" OR ",$arrBusca).") " : " where 1=1 " )." ".(is_array($arrBusca2) ? " and ( ".implode(" AND ",$arrBusca2).") " : "" )."order by ".implode(",",$cabecalho);
			
		$cabecalho = str_replace($this->getTableName().".","",$arrCabecalho);
		if(!$this->getPK($this->getSchemaName(),$this->getTableName())){
			$this->setError("A tabela ".$this->getTableName()." do schema ".$this->getSchemaName()." não possui PRIMARY KEY atribuída!");
		}else{
			$this->monta_lista($sql,$cabecalho,10,10,'N','center');
		}
	}
	
	private function isUpdate($pk = null,$schema = null,$table = null)
	{
		if(!$pk || !$schema || !$table){
			return false;
		}
		$sql = "select count(".$this->pk.") from ".$schema.".".$table." where ".$this->pk." = '$pk'";
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função salvarDados
	 * Método usado para persistir os dados da tabela no DB
	 * @access public
	 * @author Juliano Meinen
	 * @since 24/08/2010
	 */
	public function salvarDados($schema = null,$table = null,$arrDados = null,$retorno = true, $editPK = false)
	{
		$schema = !$schema ? $this->getSchemaName() : $schema;
		$table = !$table ? $this->getTableName() : $table;
		$arrDados = !$arrDados ? $_REQUEST : $arrDados;
		$this->pk = $this->getPK($schema,$table);
		$this->boUpdate = $this->isUpdate($arrDados[$this->pk],$schema,$table);
		
		$arrWhere[] = "pg_attribute.attname in ('".implode("','",(array_keys($arrDados)))."')";

		if(is_array($arrDados) && array_key_exists($this->pk,$arrDados) && $this->boUpdate){//Update
			foreach($this->getTableColumns($schema,$table,$arrWhere,null,false) as $campo ):
				
				unset($_POST[$campo['coluna']]);
								
				if($campo['tipo'] == "timestamp" || $campo['tipo'] == "date"):
						if ($arrDados[$campo['coluna']]) {
							$data = explode("/",$arrDados[$campo['coluna']]);
							$dia = $data[0];
							$mes = $data[1];
							$ano = $data[2];
							$arrDados[$campo['coluna']] = $ano."-".$mes."-".$dia;
						}else{
							$arrDados[$campo['coluna']] = null;
						}
				endif;
				
				if(strstr($campo['tipo'],"int") || strstr($campo['tipo'],"float") || strstr($campo['tipo'],"numeric")):
					$arrDados[$campo['coluna']] = str_replace(".","",$arrDados[$campo['coluna']]);
					$arrDados[$campo['coluna']] = str_replace(",",".",$arrDados[$campo['coluna']]);
				endif;
			
				if( $campo['coluna'] != $this->pk && (!array_key_exists($campo['coluna'],$arrDados) OR is_null($arrDados[$campo['coluna']])) && $campo['naonulo'] == "t" ){
						$this->setError("O array de dados não possui o campo {$campo['coluna']} atribuído como NOT NULL na tabela '".$table."' do schema '".$schema."'.");
						return false;
				}elseif($campo['coluna'] != $this->pk){
					$arrUpdate[] = $campo['coluna']." = ".($arrDados[$campo['coluna']] ? "'".$arrDados[$campo['coluna']]."'" : "null");
				}
				
			endforeach;
			$sql = sprintf("update ".$schema.".".$table." set ".implode(",",$arrUpdate)." where ".$this->pk." = '%s'",$arrDados[$this->pk]);
			$this->executar($sql);
			if($this->commit($sql)):
				if($retorno){
					$this->setMessage("Registro alterado com sucesso!");
				}
				return $arrDados[$this->pk];
			endif;
		}elseif(is_array($arrDados) && array_key_exists($this->pk,$arrDados) && !$this->boUpdate){//Insert
			
			foreach($this->getTableColumns($schema,$table,$arrWhere,null,false) as $campo ):
					
					unset($_POST[$campo['coluna']]);
					
					if($campo['tipo'] == "timestamp" || $campo['tipo'] == "date"):
						if ($arrDados[$campo['coluna']]) {
							$data = explode("/",$arrDados[$campo['coluna']]);
							$dia = $data[0];
							$mes = $data[1];
							$ano = $data[2];
							$arrDados[$campo['coluna']] = $ano."-".$mes."-".$dia;
						}else{
							$arrDados[$campo['coluna']] = null;
						}
					endif;
					
					if(strstr($campo['tipo'],"int") || strstr($campo['tipo'],"float") || strstr($campo['tipo'],"numeric")):
						$arrDados[$campo['coluna']] = str_replace(".","",$arrDados[$campo['coluna']]);
						$arrDados[$campo['coluna']] = str_replace(",",".",$arrDados[$campo['coluna']]);
					endif;
			
					if( $campo['coluna'] != $this->pk && (!array_key_exists($campo['coluna'],$arrDados) OR is_null($arrDados[$campo['coluna']])) && $campo['naonulo'] == "t" ){
						$this->setError("O array de dados não possui o campo {$campo['coluna']} atribuído como NOT NULL na tabela '".$table."' do schema '".$schema."'.");
						return false;
					}elseif($campo['coluna'] != $this->pk){
						$arrCampos[] = $campo['coluna'];
						$arrValores[] = $arrDados[$campo['coluna']] ? "'".$arrDados[$campo['coluna']]."'" : "null";
					}
			endforeach;
			if($editPK){
				$arrCampos[] = $this->pk;
				$arrValores[] = "'".$arrDados[$this->pk]."'";	
			}
			$sql = "insert into ".$schema.".".$table." (".implode(",",$arrCampos).") values (".implode(",",$arrValores).") returning ".$this->pk;

			$return = $this->pegaUm($sql);
			if($this->commit($sql)):
				if($retorno){
					$this->setMessage("Registro inserido com sucesso!");	
				}
				return $return;
			endif;
		}else{
			$this->setError("O array de dados não possui os campos necessários da tabela '".$table."' do schema '".$schema."'.");
			$this->setError($arrDados);
			return false;
		}
		
	}
	
	/**
	 * Função popularDados
	 * Método usado popular os dados
	 * @access public
	 * @author Juliano Meinen
	 * @since 24/08/2010
	 */
	public function popularDados()
	{
		$sql = sprintf("select * from ".$this->getSchemaName().".".$this->getTableName()." where ".$_POST['inp_hdn_campo']." = '%s'",$_POST['inp_hdn_campo_valor']);
		return $this->pegaLinha($sql);
	}
	
	/**
	 * Função excluirDados
	 * Método usado excluir o registro da tabela
	 * @access public
	 * @author Juliano Meinen
	 * @since 26/08/2010
	 */
	public function excluirDados()
	{
		
		$arrColumn = $this->getTableColumns();
		
		if(!is_array($arrColumn))
			return false;
			
		foreach($arrColumn as $arrColumn):
			if(strstr($arrColumn['coluna'],"status")):
				$sql = "update ".$this->getSchemaName().".".$this->getTableName()." set ".$arrColumn['coluna']." = 'I' where ".$_REQUEST['inp_hdn_campo']." = '{$_REQUEST['inp_hdn_campo_valor']}'";
				break;
			else:
				$sql = "delete from ".$this->getSchemaName().".".$this->getTableName()." where ".$_REQUEST['inp_hdn_campo']." = '{$_REQUEST['inp_hdn_campo_valor']}'";
			endif;
		endforeach;
		$this->executar($sql);
		if($this->commit($sql)){
			$this->setMessage("Registro excluído com sucesso!");
			return true;
		}
	}
	
	/**
	 * Função setMessage
	 * Método usado para incluir as mensagems de sucesso
	 * @access private
	 * @author Juliano Meinen
	 * @since 26/08/2010
	 */
	private function setMessage($message)
	{
		$this->arrMessage[] = $message;
	}
	
	/**
	 * Função getMessage
	 * Método usado para retornar a pilha de mensagens de sucesso 
	 * @access public
	 * @author Juliano Meinen
	 * @since 26/08/2010
	 */
	public function getMessage()
	{
		if(count($this->arrMessage)){
			foreach($this->arrMessage as $message):
				$html .= "<p class=\"sucess_msg bold\">".$message."</p>";
			endforeach;
			return $html;
		}else{
			return false;
		}
	}
	
	/**
	 * Função getDescricaoValue
	 * Método usado para retornar a descricao do campo para a função combo_popup
	 * @access private
	 * @author Juliano Meinen
	 * @since 02/08/2010
	 */
	private function getDescricaoValue($table,$col,$pk,$value)
	{
		if($table && $col && $pk && $value):
			$sql = sprintf("select %s from %s where %s = '%s'",$col,$table,$pk,$value);
			return $this->pegaUm($sql);
		else:
			return false;
		endif;
	}
	
	/**
	 * Função getDescriptionTable
	 * Método usado para retornar a descricao da tabela
	 * @access public
	 * @author Juliano Meinen
	 * @since 02/08/2010
	 */
	public function getDescriptionTable($schema = null,$table = null)
	{
		$sql = "SELECT
					(SELECT description FROM pg_description pd WHERE pg_class.oid = pd.objoid LIMIT 1) AS comentario
				FROM 
					pg_class
				join
					pg_namespace ON pg_namespace.oid = pg_class.relnamespace AND pg_namespace.nspname NOT LIKE 'pg_%'
				where
					pg_class.relname = '".(!$table ? $this->getTableName() : $table)."'
				and
					pg_namespace.nspname = '".(!$schema ? $this->getSchemaName() : $schema)."';";
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função saveTableDescription
	 * Método usado para salvar a descrição da tabela.
	 * @access public
	 * @author Juliano Meinen
	 * @since 03/08/2010
	 */
	public function saveTableDescription()
	{
		if($_POST['combo_tbl'] && $_POST['params']):
			$sql = sprintf("COMMENT ON TABLE %s IS '%s';",utf8_decode($_POST['combo_tbl']),utf8_decode($_POST['params']));
			$this->executar($sql);
			return $this->commit($sql);
		else:
			return false;
		endif;
	}
	
	/**
	 * Função getSchemaForeingKey
	 * Método usado retornar o schema da FK.
	 * @access private
	 * @author Juliano Meinen
	 * @since 08/09/2010
	 */
	private function getSchemaForeingKey($restricao)
	{
		$restricao = str_replace(array("FOREIGN KEY","REFERENCES"," "),"",$restricao);
		$restricao = substr($restricao,1,strpos($restricao, "ON")-1);
		
		$fk = substr($restricao,0,strpos($restricao, ")"));
		$tbl = str_replace("$fk)","",substr($restricao,0,strpos($restricao,"(")));
		$fk2 = str_replace( array( "$fk)$tbl(" ,")" ),"",$restricao);

		if(!strstr($tbl,".")){
			$arrWhere1[] = "n.nspname = '".$this->getSchemaName()."'";
			$arrWhere1[] = "c.relname = '$tbl'";
			$arrWhere2[] = "n.nspname = 'public'";
			$arrWhere2[] = "c.relname = '$tbl'";
			if(is_array($this->getShemaTables($arrWhere1))){
				$tbl = $this->getSchemaName().".".$tbl;
			}elseif(is_array($this->getShemaTables($arrWhere2))){
				$tbl = "public.".$tbl;
			}
		}
		
		$dado = explode(".",$tbl);
		return $dado[0];
	}
	
	/**
	 * Função getTableForeingKey
	 * Método usado retornar a tabela da FK.
	 * @access private
	 * @author Juliano Meinen
	 * @since 08/09/2010
	 */
	private function getTableForeingKey($restricao)
	{
		$restricao = str_replace(array("FOREIGN KEY","REFERENCES"," "),"",$restricao);
		$restricao = substr($restricao,1,strpos($restricao, "ON")-1);
		
		$fk = substr($restricao,0,strpos($restricao, ")"));
		$tbl = str_replace("$fk)","",substr($restricao,0,strpos($restricao,"(")));
		$fk2 = str_replace( array( "$fk)$tbl(" ,")" ),"",$restricao);
		
		if(!strstr($tbl,".")){
			$arrWhere1[] = "n.nspname = '".$this->getSchemaName()."'";
			$arrWhere1[] = "c.relname = '$tbl'";
			$arrWhere2[] = "n.nspname = 'public'";
			$arrWhere2[] = "c.relname = '$tbl'";
			if(is_array($this->getShemaTables($arrWhere1))){
				$tbl = $this->getSchemaName().".".$tbl;
			}elseif(is_array($this->getShemaTables($arrWhere2))){
				$tbl = "public.".$tbl;
			}
		}
		
		$this->setTableFK($tbl);
		
		$dado = explode(".",$tbl);
		return $dado[1];
	}
	
	/**
	 * Função saveColumnTableDescription
	 * Método usado salvar a coluna de descrição da tabela quando FK
	 * @access public
	 * @author Juliano Meinen
	 * @since 08/09/2010
	 */
	public function saveColumnTableDescription()
	{
		
		$sql = sprintf("select tbmid, tbmcampodesc, tbmstatus from seguranca.tblmapa where tbmschema = '%s' AND tbmtabela = '%s'",$_POST['schema'],$_POST['table']);
		$tbm = $this->pegaLinha($sql);
		$arrDados['tbmid'] = $tbm['tbmid'];
		$arrDados['tbmschema'] = $_POST['schema'];
		$arrDados['tbmtabela'] = $_POST['table'];
		$arrDados['tbmcampodesc'] = $_POST['column'];
		$arrDados['tbmstatus'] = $tbm['tbmstatus'] ? $tbm['tbmstatus'] : "A";
		if($_POST['column'] != $tbm['tbmcampodesc']){
			$this->salvarDados("seguranca","tblmapa",$arrDados);
		}

		$arrWhere[] = "pg_attribute.attname = '".$_POST['columnTable']."'";
		$arrColuna = $this->getTableColumns($this->getSchemaName(),$this->getTableName(),$arrWhere,null,false);
		echo $this->getInputColumn($arrColuna[0],$_POST['valueColumnTable']);
		
	}
	
	/**
	 * Função getColumnTableDescription
	 * Método usado para retornar a coluna de descrição da tabela quando FK
	 * @access public
	 * @author Juliano Meinen
	 * @since 09/09/2010
	 */
	public function getColumnTableDescription($schema = null,$table = null)
	{
		$schema = !$schema ? $this->getSchemaName() : $schema;
		$table = !$table ? $this->getTableName() : $table;
		
		$sql = sprintf("select tbmcampodesc from seguranca.tblmapa where tbmschema = '%s' AND tbmtabela = '%s' AND tbmstatus = 'A'",$schema,$table);
		return $this->pegaUm($sql);

	}
	
	/**
	 * Função getListColumnTableDescription
	 * Método usado para retornar a lista de colunas da tabela
	 * @access public
	 * @author Juliano Meinen
	 * @since 09/09/2010
	 */
	public function getListColumnTableDescription()
	{
		$sql = sprintf("select tbacampolista from seguranca.tblapoio where tbaschema = '%s' AND tbatabela = '%s' AND tbastatus = 'A'",$this->getSchemaName(),$this->getTableName());
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função saveListColumnTable
	 * Método usado salvar as colunas da lista da tabela
	 * @access public
	 * @author Juliano Meinen
	 * @since 09/09/2010
	 */
	public function saveListColumnTable()
	{
		
		$sql = sprintf("select * from seguranca.tblapoio where tbaschema = '%s' AND tbatabela = '%s' AND tbastatus = 'A'",$this->getSchemaName(),$this->getTableName());
		$arrDados = $this->pegaLinha($sql);
		if($_POST['column'] != $arrDados['tbacampolista']){
			$arrDados['tbaid'] = !$arrDados['tbaid'] ? null : $arrDados['tbaid'];
			$arrDados['tbaschema'] = $this->getSchemaName();
			$arrDados['tbatabela'] = $this->getTableName();
			$arrDados['tbacampolista'] = $_POST['column'];
			$arrDados['tbastatus'] = "A";
			$this->salvarDados("seguranca","tblapoio",$arrDados);
		}
		unset($_POST['ajaxMethod']);
		$tbl = new ManterTabela();
		$tbl->setSchemaName( $this->getSchemaName() );
		$tbl->setTableName( $this->getTableName() );
		$tbl->montaListaTabela($_POST['txt_busca']);
	}
	
	/**
	 * Função setPermission
	 * Método usado atribuir permissão
	 * @access public
	 * @author Juliano Meinen
	 * @since 09/09/2010
	 */
	public function setPermission(array $permission)
	{
		$this->permission = $permission;
	}
	
	/**
	 * Função getPermission
	 * Método usado para retornar as permissões
	 * @access public
	 * @author Juliano Meinen
	 * @since 09/09/2010
	 */
	public function getPermission()
	{
		return $this->permission;
	}
	
	/**
	 * Função saveColumnComment
	 * Método usado para salvar o comentário da coluna
	 * @access public
	 * @author Juliano Meinen
	 * @since 13/09/2010
	 */
	public function saveColumnComment()
	{
		if($_POST['combo_tbl'] && $_POST['column']):
			$sql = sprintf("COMMENT ON COLUMN %s IS '%s';",utf8_decode($_POST['combo_tbl']).".".utf8_decode($_POST['column']),utf8_decode($_POST['columnComment']));
			$this->executar($sql);
			return $this->commit($sql);
		else:
			return false;
		endif;
	}
	
	/**
	 * Função setTableFK
	 * Método usado para registrar as tabelas envolvidas no processo
	 * @access public
	 * @author Juliano Meinen
	 * @since 13/09/2010
	 */
	public function setTableFK($tbl)
	{
		$this->arrTableFK[] = $tbl;
	}
	
	/**
	 * Função getTableFK
	 * Método usado para retornar as tabelas envolvidas no processo
	 * @access public
	 * @author Juliano Meinen
	 * @since 13/09/2010
	 */
	public function getTableFK($sisid = null)
	{
		$sisid = !$sisid ? $_SESSION['sisid'] : $sisid;

		if(count($this->arrTableFK) > 0):
			foreach($this->arrTableFK as $table):
				$dado = explode(".",$table);
				$arrWhere[] = "(tbaschema = '{$dado[0]}' AND tbatabela = '{$dado[1]}')
				";
				$arrTabelas[$table] = $table;
			endforeach;
				
			$sql = "select 
						tbaschema,
						tbatabela
					from
						seguranca.tblapoio tba
					inner join
						seguranca.tblsistema tbl ON tba.tbaid = tbl.tbaid
					where
						sisid = $sisid
						".($arrWherer ? " and " . implode(" or ",$arrWhere) : "")."
					and
						tbastatus = 'A'";
			$arrTbl = $this->carregar($sql);
			if(is_array($arrTbl)):
				foreach($arrTbl as $tabela):
					if(in_array($tabela['tbaschema'].".".$tabela['tbatabela'],$arrTabelas)):
						unset($arrTabelas[$tabela['tbaschema'].".".$tabela['tbatabela']]);
					endif;
					$arrTabelasPresentes[] = $tabela['tbaschema'].".".$tabela['tbatabela'];
				endforeach;
			endif;
			return $arrTabelas;
		else:
			return false;
		endif;
	}
	
	/**
	 * Função saveTableApoio
	 * Método usado para registrar as tabelas em seguranca.tblapoio e seguranca.tblsistema
	 * @access public
	 * @author Juliano Meinen
	 * @since 14/09/2010
	 */
	public function saveTableApoio($sisid = null,$retorno = true)
	{
		$sisid = !$sisid ? $_SESSION['sisid'] : $sisid;
		
		$arrTbl = explode(",",$_POST['tbls']);
		
		foreach($arrTbl as $tbl):
			$dado = explode(".",$tbl);
			$sql = sprintf("select tbaid from seguranca.tblapoio where tbaschema = '%s' and tbatabela = '%s' and tbastatus = 'A'",$dado[0],$dado[1]);
			$tbaid = $this->pegaUm($sql);

			if($tbaid):
				$sql = sprintf("select tbsid from seguranca.tblsistema where tbaid = '%s' and sisid = '%s'",$tbaid,$sisid);
				$tbsid = $this->pegaUm($sql);
				if(!$tbsid):
					$this->salvarDados("seguranca","tblsistema",array("tbsid" => null,"tbaid" => $tbaid, "sisid" => $sisid),$retorno);	
				endif;
			else:
				$pk = $this->salvarDados("seguranca","tblapoio",array("tbaid" => null,"tbaschema" => $dado[0], "tbatabela" => $dado[1],"tbacampolista" => null,"tbastatus" => "A"),$retorno);
				if($pk){
					$this->salvarDados("seguranca","tblsistema",array("tbsid" => null,"tbaid" => $pk, "sisid" => $sisid),$retorno);
				}
			endif;
		endforeach;
		
		$tbl = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
		$tbl.= '<tr class="center SubtituloTabela" >';
		$tbl.= '<td>';
		$tbl.= '<p class="sucess_msg bold">Tabela(s) inserida(s) com sucesso!</p>';
		$tbl.= '</td>';
		$tbl.= '</tr></table>';
		if($retorno){
			echo $tbl;
		}
		
	}
	
	/**
	 * Função reloadComboSchema
	 * Método usado para carregar o combo de Schemas
	 * @access public
	 * @author Juliano Meinen
	 * @since 14/09/2010
	 */
	public function reloadComboSchema()
	{
		$combo_sch = $_POST['combo_sch'];
		$this->monta_combo('combo_sch',$this->getShemasDB(),'S','Selecione o Schema','selecionaSchema','','','','','','',$combo_sch);
	}

	/**
	 * Função reloadComboTable
	 * Método usado para carregar o combo de Tabelas
	 * @access public
	 * @author Juliano Meinen
	 * @since 14/09/2010
	 */
	public function reloadComboTable()
	{
		$combo_tbl = $_POST['combo_tbl'];
		$this->monta_combo('combo_tbl',$this->getShemaTablesPorSisid(),'S','Selecione a Tabela','selecionaTabelaApoio','','','','','combo_tbl','',$combo_tbl);
	}
	
	public function getLabel($comentario = null)
	{
		$posLabel1 = strpos($comentario,"<label>");
		$posLabel2 = strpos($comentario,"</label>");
		$arrColumn['label'] = substr($comentario,$posLabel1,$posLabel2);
		$arrColumn['label'] = str_replace(array("<label>","</label>"),"",$arrColumn['label']);
		return $arrColumn['label'];
	}

}
