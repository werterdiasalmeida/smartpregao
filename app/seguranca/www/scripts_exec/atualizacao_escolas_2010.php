<?php
function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "3096M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "seguranca/www/_funcoesmonitoramento.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$sql = "SELECT \"PK_COD_ENTIDADE\",
			   \"NUM_CNPJ_ESCOLA_PRIVADA\",
			   \"NUM_CNPJ_UNIDADE_EXECUTORA\",
			   \"Nome_Escola\",
			   \"Correio_Eletronico\", 
			   \"DDD\",
			   \"Telefone\",
			   \"Fax\",
			   \"CATESCPRIVADA\", 
			   \"Dependencia_Administrativa\",
			   \"Localizacao\", 
			   \"CEP\",
			   \"Endereco\", 
			   \"Complemento_Endereco\",
			   \"Bairro\",
			   \"Cod_Municipio\",
			   \"SIGLA\", 
			   \"Nr_Endereco\"
		FROM carga.escolas";

$escolas_novas = $db->carregar($sql);

$entidades_atu = 0;
$entidades_ins = 0;
$enderecos_atu = 0;
$enderecos_ins = 0;

if($escolas_novas[0]) {
	foreach($escolas_novas as $esc) {
		
		$sql = "SELECT e.entid, en.endid FROM entidade.entidade e 
				LEFT JOIN entidade.endereco en ON en.entid = e.entid AND tpeid=1
				WHERE e.entcodent = '".$esc['PK_COD_ENTIDADE']."'";
		
		$escola_rec = $db->pegaLinha($sql);
		
		/*
		 * Testando se existe a escola
		 */
		if($escola_rec['entid']) {
			
			$sql = "UPDATE entidade.entidade
   					SET entnumcpfcnpj='".(($esc['NUM_CNPJ_ESCOLA_PRIVADA'])?$esc['NUM_CNPJ_ESCOLA_PRIVADA']:$esc['NUM_CNPJ_UNIDADE_EXECUTORA'])."', 
   						entnome='".$esc['Nome_Escola']."', 
   						entemail='".$esc['Correio_Eletronico']."', 
					    entnumdddcomercial='".$esc['DDD']."', 
					    entnumcomercial='".$esc['Telefone']."', 
					    entnumdddfax='".$esc['DDD']."', 
					    entnumfax='".$esc['Fax']."', 
					    entrazaosocial='".$esc['Nome_Escola']."'
					 WHERE entcodent='".$esc['PK_COD_ENTIDADE']."';";
			
			//die($sql);
			
			$db->executar($sql);
			$entidades_atu++;
			
		} else {
			/*
			 1;"Comunitária";"A"
			 2;"Confessional";"A"
			 3;"Filantropica";"A"
			 4;"Particular";"A"
			 */
			switch($esc['CATESCPRIVADA']) {
				case 'Comunitaria':
					$tpctgid = 1;
					break;
				case 'Particular':
					$tpctgid = 4;
					break;
				case 'Confessional':
					$tpctgid = 2;
					break;
				case 'Filantropica':
					$tpctgid = 3;
					break;
				default:
					$tpctgid = "NULL";
			}
			
			/*
			1;"Estadual";""
			2;"Federal";""
			3;"Municipal";""
			4;"Privada";""
			 */
			switch($esc['Dependencia_Administrativa']) {
				case 'Estadual':
					$tpcid = 1;
					break;
				case 'Federal':
					$tpcid = 2;
					break;
				case 'Municipal':
					$tpcid = 3;
					break;
				case 'Privada':
					$tpcid = 4;
					break;
				default:
					$tpcid = "NULL";
			}

			/*
			1;"Urbana";"A"
			2;"Rural";"A"
			 */
			switch($esc['Localizacao']) {
				case 'Rural':
					$tplid = 1;
					break;
				case 'Urbana':
					$tplid = 2;
					break;
				default:
					$tplid = "NULL";
			}
			
			$sql = "INSERT INTO entidade.entidade(
				            entnumcpfcnpj, 
				            entnome, 
				            entemail, 
				            entstatus,  
				            entnumdddcomercial, 
				            entnumcomercial, 
				            entnumdddfax, 
				            entnumfax, 
				            tpctgid, 
				            tpcid, 
				            tplid, 
				            tpsid, 
				            entcodent, 
				            entdatainclusao, 
				            entrazaosocial)
				    VALUES ('".(($esc['NUM_CNPJ_ESCOLA_PRIVADA'])?$esc['NUM_CNPJ_ESCOLA_PRIVADA']:$esc['NUM_CNPJ_UNIDADE_EXECUTORA'])."', 
				    		'".$esc['Nome_Escola']."', 
				    		'".$esc['Correio_Eletronico']."', 
				    		'A', 
				    		'".$esc['DDD']."', 
				    		'".$esc['Telefone']."', 
				            '".$esc['DDD']."', 
				            '".$esc['Fax']."', 
				            ".$tpctgid.", 
				            ".$tpcid.", 
				            ".$tplid.", 
				            1, 
				            '".$esc['PK_COD_ENTIDADE']."', 
				            NOW(), 
				            '".$esc['Nome_Escola']."') RETURNING entid;";
			
			//die($sql);
			$escola_rec['entid'] = $db->pegaUm($sql);
			$entidades_ins++;
			
		}
		/*
		 * FIM - Testando se existe a escola
		 */
		
		
		if($escola_rec['endid']) {
			
			$sql = "UPDATE entidade.endereco
   					SET endcep='".$esc['CEP']."', 
   					    endlog='".$esc['Endereco']."', 
   					    endcom='".$esc['Complemento_Endereco']."', 
   					    endbai='".$esc['Bairro']."', 
       					muncod='".$esc['Cod_Municipio']."', 
       					estuf='".$esc['SIGLA']."', 
       					endnum='".$esc['Nr_Endereco']."' 
 					WHERE endid='".$escola_rec['endid']."';";
			
			//die($sql);
			
			$db->executar($sql);
			$enderecos_atu++;
			
		} else {
			
			$sql = "INSERT INTO entidade.endereco(
				            entid, tpeid, endcep, endlog, 
				            endcom, endbai, muncod, 
				            estuf, endnum, endstatus)
				    VALUES ('".$escola_rec['entid']."', 1, '".$esc['CEP']."', '".$esc['Endereco']."', 
				    		'".$esc['Complemento_Endereco']."', '".$esc['Bairro']."', '".$esc['Cod_Municipio']."', 
				    		'".$esc['SIGLA']."', '".$esc['Nr_Endereco']."', 'A') RETURNING endid;";
			
			//die($sql);
			
			$escola_rec['endid'] = $db->pegaUm($sql);
			$enderecos_ins++;
			
		}

	}
	
	$db->commit();
}

echo "RESUMO<br/>";
echo "Entidades atualizadas:".$entidades_atu."<br>";
echo "Entidades inseridas:".$entidades_ins."<br>";
echo "Endereços atualizados:".$enderecos_atu."<br>";
echo "Endereços inseridos:".$enderecos_ins."<br>";

?>