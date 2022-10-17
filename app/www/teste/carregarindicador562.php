<?
$_REQUEST['baselogin'] = "simec_espelho_producao";


// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

set_time_limit(30000);
ini_set("memory_limit", "3000M");


// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';


$db = new cls_banco();


$csv = file("carga562fev2009.csv");


$sql = "INSERT INTO painel.agendamentocarga(
            agddata, agddataprocessamento, usucpf, indid, agdstatus, 
            agdprocessado)
    	VALUES (NOW(), '2010-09-22', '91112796134', '562', 'A', false) RETURNING agdid;";

$agdid = $db->pegaUm($sql);

foreach($csv as $cs) {
	
	$dados = explode(";", $cs);
	
	$dadoster = $db->pegaLinha("SELECT m.muncod, m.estuf FROM painel.escola e 
								LEFT JOIN territorios.municipio m ON e.escmuncod=m.muncod 
								WHERE e.esccodinep='".$dados[4]."'");
	
	$sql = "INSERT INTO painel.agendamentocargadados(
            agdid, indid, dpeid, acdqtde, acdmuncod, 
            acduf, acdesciescod, tidid1)
		    VALUES ('".$agdid."', '562', '".$dados[1]."', '".$dados[2]."', 
		    		'".$dadoster['muncod']."', '".$dadoster['estuf']."', 
		    		'".$dados[4]."', '".$dados[3]."');";
	
	$db->executar($sql);
}

$db->commit();

echo "fim";
?>