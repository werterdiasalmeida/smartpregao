<?
set_time_limit(30000);
$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';


$db = new cls_banco();

$escola[15138658] = array("name" => "E.M.E.F ITACY SANTA ROSA LIMA III", "inep" => "15138658", "mun" => "1503705");
$escola[15138232] = array("name" => "E.M.E.F COELHO E SILVA", "inep" => "15138232", "mun" => "1503705");
$escola[15112780] = array("name" => "E.M.E.F MÁRIO DE ANDRADE", "inep" => "15112780", "mun" => "1503705");
$escola[15138275] = array("name" => "E.M.E.F RUITAMAR MENDES SILVA", "inep" => "15138275", "mun" => "1503705");
$escola[15538389] = array("name" => "E.M.E.F ODILAR BARRETO", "inep" => "15538389", "mun" => "1503705");
$escola[15138755] = array("name" => "E.M.E.F SANTA JOANA D'ARC", "inep" => "15138755", "mun" => "1503705");
$escola[15138259] = array("name" => "E.M.E.F ANISIO TEIXEIRA II", "inep" => "15138259", "mun" => "1503705");
$escola[15573484] = array("name" => "E.M.E.F CECILIA MEIRELES", "inep" => "15573484", "mun" => "1506195");
$escola[15153825] = array("name" => "E.M.E.F NICÉIA", "inep" => "15153825", "mun" => "1506195");
$escola[15534252] = array("name" => "E.M.E.F SEMEANDO O FUTURO", "inep" => "15534252", "mun" => "1506195");
$escola[15096717] = array("name" => "E.M.E.F ESCREVENDO O FUTURO", "inep" => "15096717", "mun" => "1506195");
$escola[15101192] = array("name" => "E.M.E.F SÃO SEBASTIÃO", "inep" => "15101192", "mun" => "1503077");
$escola[29050715] = array("name" => "Escola Municipal de Boa Vista do Cajuí", "inep" => "29050715", "mun" => "2906006");
$escola[12133221] = array("name" => "ESCOLA NOVA QUINTAIS FLORESTAIS /CODIGO", "inep" => "12133221", "mun" => "1200252");
$escola[15217027] = array("name" => "E M E F. PIMENTA", "inep" => "15217027", "mun" => "1508050");
$escola[11114835] = array("name" => "IRMÃ DOROTHY MAE STANG", "inep" => "11114835", "mun" => "1100130");
$escola[11114827] = array("name" => "ANÍSIO SPINOLA TEIXEIRA", "inep" => "11114827", "mun" => "1100130");
$escola[11114843] = array("name" => "ZILDA ARNS NEUMANN", "inep" => "11114843", "mun" => "1100130");
$escola[11115807] = array("name" => "CACILDA BECKER", "inep" => "11115807", "mun" => "1100130");
$escola[11117818] = array("name" => "EMEF 15 DE NOVEMBRO", "inep" => "11117818", "mun" => "1100205");
$escola[11117834] = array("name" => "EMEF ENGENHO DO MADEIRA", "inep" => "11117834", "mun" => "1100205");
$escola[15217019] = array("name" => "E M E F. TUCANO", "inep" => "15217019", "mun" => "1508050");
$escola[15217035] = array("name" => "E M E F. NOSSA SENHORA APARECIDA", "inep" => "15217035", "mun" => "1508050");
$escola[21608660] = array("name" => "Escola Municipal João Alves Pinto", "inep" => "21608660", "mun" => "2106300");

$escola[15555518] = array("name" => "E M E F JOSÉ NUNES", "inep" => "15555518", "mun" => "1504950");
$escola[15577414] = array("name" => "E M E F JOSÉ MENDES", "inep" => "15577414", "mun" => "1504950");


$escola[13059564] = array("name" => "Afonso Lima", "inep" => "13059564", "mun" => "1302553");
$escola[13081896] = array("name" => "Cristo Redentor", "inep" => "13081896", "mun" => "1302553");
$escola[13059564] = array("name" => "Justiniano Bezerra", "inep" => "13057910", "mun" => "1302553");
$escola[13081896] = array("name" => "Maria Rosa de Oliveira", "inep" => "13081896", "mun" => "1302553");
$escola[13059564] = array("name" => "Ouro Verde", "inep" => "13057812", "mun" => "1302553");
$escola[13081896] = array("name" => "Sagrado Coração de Jesus", "inep" => "13020358", "mun" => "1302553");
$escola[13059564] = array("name" => "Ouro Verde", "inep" => "13057812", "mun" => "1302553");

$escola[17115809] = array("name" => "Benedito Póvoa", "inep" => "17115809", "mun" => "1720937");

$escola[29966728] = array("name" => "Escola Municipal Alvino Santiago", "inep" => "29966728", "mun" => "2920502");
$escola[29964725] = array("name" => "Escola Municipal Ana Rocha", "inep" => "29964725", "mun" => "2920502");
$escola[29964733] = array("name" => "Escola Municipal Antonio Fernandes de Souza", "inep" => "29964733", "mun" => "2920502");
$escola[29963753] = array("name" => "Escola Municipal Gersínio dos Anjos", "inep" => "29963753", "mun" => "2920502");
$escola[29963737] = array("name" => "Escola Municipal José Marques de Oliveira", "inep" => "29963737", "mun" => "2920502");
$escola[29963770] = array("name" => "Escola Municipal Nelito Evangelista", "inep" => "29963770", "mun" => "2920502");
$escola[29963745] = array("name" => "Escola Municipal Gervásio Soares", "inep" => "29963745", "mun" => "2920502");
$escola[29963761] = array("name" => "Escola Municipal Maria Amália de Novaes", "inep" => "29963761", "mun" => "2920502");
$escola[29965721] = array("name" => "Escola Municipal Santa Inês", "inep" => "29965721", "mun" => "2920502");
$escola[29963788] = array("name" => "Escola Municipal Santa Rosa", "inep" => "29963788", "mun" => "2920502");
$escola[29964741] = array("name" => "Escola Municipal Urbano Gomes", "inep" => "29964741", "mun" => "2920502");

$es = array("15093441",
			"15174026",
			"15555518",
			"15178005",
			"15174034",
			"15577414",
			"13059564",
			"13081896",
			"13057910",
			"13081896",
			"13057812",
			"13020358",
			"17115809",
			"29966728",
			"29964725",
			"29964733",
			"29963753",
			"29963737",
			"29963770",
			"29963745",
			"29963761",
			"29965721",
			"29963788",
			"29964741"
);

/*
 * Relação executada no dia 22/09 
$es = array("33052522",	
			"15138658",	
			"15138232",	
			"15112780",	
			"15138275",	
			"15538389",	
			"15138755",	
			"15138259",	
			"15573484",	
			"15153825",	
			"15534252",	
			"15096717",	
			"15105040",	
			"15096530",	
			"15101192",	
			"21608660",	
			"29050715",	
			"12133221",	
			"15217027",	
			"15217019",	
			"15217035",	
			"11114835",	
			"11114827",	
			"11114843",	
			"11115807",	
			"11117818",	
			"11117834");	
 */
foreach($es as $e) {
	
	$sql = "select e.*,f.*,en.entid,enn.endid,fen.fueid from educacenso_tmp.escola_inep_2009 e 
			left join educacenso_tmp.tmp_escola_inep_2009 f on f.pk_cod_entidade=e.pk_cod_entidade 
			left join entidade.entidade en on en.entcodent=e.pk_cod_entidade::varchar
			left join entidade.endereco enn on enn.entid=en.entid 
			left join entidade.funcaoentidade fen on fen.entid=en.entid and funid=3
			where e.pk_cod_entidade='".$e."'";
	
	$descola = $db->pegaLinha($sql);
	
	if($descola) {
		
		
		if(!$descola['entid']) {
			$sql = "INSERT INTO entidade.entidade(
					            entnome,  
					            entstatus,  
					            tpcid, tplid, tpsid,  
					            entcodent, entescolanova, entdatainclusao)
					    VALUES ('".$descola['nome_escola']."',
					    		'A','3','2','1','".$e."',  
					            false, NOW()) RETURNING entid;";
			
			
			$entid = $db->pegaUm($sql);
			
			$db->commit();
			
			//$sql = "INSERT INTO entidade.entidadedetalhe(entcodent, entid) VALUES ('".$e."', '".$entid."');";
			//$db->executar($sql);
			
		} else {
			$entid = $descola['entid'];
		}
		
		if(!$descola['fueid']) {
			$sql = "INSERT INTO entidade.funcaoentidade(
	            	funid, entid, fuedata, fuestatus)
	    			VALUES ('3', '".$entid."', NOW(), 'A');";
			
			$db->executar($sql);
		}
		
		if(!$descola['endid']) {
			$sql = "INSERT INTO entidade.endereco(
	            	entid, tpeid, endcep, endlog, endcom, endbai, muncod, 
	            	estuf, endnum, endstatus)
	    			VALUES ('".$entid."', '1', ".(($descola['num_cep'])?"'".$descola['num_cep']."'":"NULL").", ".(($descola['endereco'])?"'".$descola['endereco']."'":"NULL").", '".(($descola['endereco_complemento'])?"'".$descola['endereco_complemento']."'":"NULL")."', 
	    					".(($descola['endbai'])?"'".$descola['endbai']."'":"NULL").", '".$descola['cod_municipio']."', 
	            			".(($descola['sg_uf'])?"'".$descola['sg_uf']."'":"'".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$descola['cod_municipio']."'")."'").", '".(($descola['num_endereco'])?"'".$descola['num_endereco']."'":"NULL")."', 'A');";
			
			$db->executar($sql);
		}
		
		$db->commit();
		
		
		
	} else {
		
		if($escola[$e]) {
			
			$sql = "SELECT e.entid, en.endid, fen.fueid FROM entidade.entidade e 
					LEFT JOIN entidade.endereco en on e.entid=en.entid 
					LEFT JOIN entidade.funcaoentidade fen on fen.entid=e.entid and funid=3
					WHERE entcodent='".$e."'";
			
			$descola = $db->pegaLinha($sql);
			
		if(!$descola['entid']) {
				$sql = "INSERT INTO entidade.entidade(
						            entnome,  
						            entstatus,  
						            tpcid, tplid, tpsid,  
						            entcodent, entescolanova, entdatainclusao)
						    VALUES ('".addslashes($escola[$e]['name'])."',
						    		'A','3','2','1','".$e."',  
						            false, NOW()) RETURNING entid;";
				
				//echo $sql;
				//exit;
				
				$entid = $db->pegaUm($sql);
				
				$db->commit();
				
				//$sql = "INSERT INTO entidade.entidadedetalhe(entcodent, entid) VALUES ('".$e."', '".$entid."');";
				//$db->executar($sql);
				
			} else {
				$entid = $descola['entid'];
			}
			
			if(!$descola['fueid']) {
				$sql = "INSERT INTO entidade.funcaoentidade(
		            	funid, entid, fuedata, fuestatus)
		    			VALUES ('3', '".$entid."', NOW(), 'A');";
				
				//echo $sql;
				//exit;
				
				
				$db->executar($sql);
			}
			
			if(!$descola['endid']) {
				$sql = "INSERT INTO entidade.endereco(
		            	entid, tpeid, muncod, 
		            	estuf, endstatus)
		    			VALUES ('".$entid."', '1', '".$escola[$e]['mun']."', '".$db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$escola[$e]['mun']."'")."', 'A');";
				
				//echo $sql;
				//exit;
				
				
				$db->executar($sql);
			}
			
			$db->commit();
				
			
		} else {
			echo "<pre>";
			print_r("não encontrado: ".$e);
		}
		
	}
}


echo "fim";
?>