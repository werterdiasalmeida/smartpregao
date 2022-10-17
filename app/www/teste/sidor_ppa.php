<?
error_reporting( E_ALL );
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";



$db = new cls_banco();

pg_set_client_encoding( $db->link, 'UNICODE' );

header( 'Content-Type: text/plain; charset=utf-8' );

set_time_limit(0);

function getmicrotimesidor()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}

require_once APPRAIZ . "includes/Snoopy.class.php";
require_once APPRAIZ . "includes/Sidor.class.php";

parse_str( 'operacao=Opera%E7%E3o+Especial&tipoAcao=&codAcao=01810000&qtdFisPrevT1=1.000&qtdFisPrevT2=4429.000&qtdFisPrevT3=4474.000&qtdFisPrevT4=0.000', $_REQUEST["_dadosEnvio"] );
//dbg($_REQUEST["_dadosEnvio"],1);

$inicioTx = getmicrotimesidor();

if($_REQUEST["_dadosEnvio"]) {

	ini_set("output_buffering", 0);
	ini_set("implicit_flush", 1);

	$usuarioSidor = $_REQUEST["usuario_sidor"];
	$senhaSidor = $_REQUEST["senha_sidor"];
	//$dados = unserialize(base64_decode($_REQUEST["_dadosEnvio"]));
	//array_push($dados,array());
	
	$sidor = new Sidor();
	$usuarioSidor = 'ewg';
	$senhaSidor = 'orl001';
	try {
		$sidor->login($usuarioSidor, $senhaSidor);
	}
	catch(SidorLoginException $e) {
		$db->insucesso("Usuário ou senha inválidos!", '', true);
	}
	catch(SidorException $e) {
		$db->insucesso($e->message());
	}
	

	/*
	$idAtual = "";
	$dadosEnvio = array();
	$i=0;
	$totalAcao = 0;
	$linhaAnterior = array();
	?>
	<pre>Speed Includator Orçamentator - Módulo de exportação de dados orçamentários para SIDOR</pre>
	<?
	foreach($dados as $linha) {
		try {
			$linha["tipoDetalhamento"] = "1" . $linha["tdecod"];
			$id = $linha["unicod"].$linha["tipoDetalhamento"].$linha["prgcod"].$linha["acacod"].$linha["loccod"];
			$prefix = sprintf("%02d", $i);
	
			if($id != $idAtual && $i > 0) {
				//
				// enviar dados para o sidor
				$codref = $sidor->pegarCodReferenciaSidor($linhaAnterior["unicod"], $linhaAnterior["tipoDetalhamento"], $linhaAnterior["prgcod"], $linhaAnterior["acacod"], $linhaAnterior["loccod"]);
				$prod = $sidor->pegarProdutoUnidadeSidor($codref, $linhaAnterior["unicod"], $linhaAnterior["tipoDetalhamento"], $linhaAnterior["prgcod"], $linhaAnterior["acacod"], $linhaAnterior["loccod"]);
				$dadosEnvio = array_merge($dadosEnvio, array("flgAtualizacao$prefix"=>"N"
					,"natu$prefix"=>""
					,"iuso$prefix"=>""
					,"fonte$prefix"=>""
					,"idoc$prefix"=>""
					,"valor$prefix"=>""
				));
				
				$sidor->insereDadosProposta($codref, $linhaAnterior["unicod"], $linhaAnterior["tipoDetalhamento"], $linhaAnterior["prgcod"], $linhaAnterior["acacod"], $linhaAnterior["loccod"], $i+1, $dadosEnvio, $totalAcao, $linhaAnterior["acaqtdefisico"], $prod["codproduto"], $prod["desproduto"], $prod["unidademedida"], $linhaAnterior["justificativa"]);
				?>
				<pre>OK: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["tipoDetalhamento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - <?=$totalAcao?> - <?=$i?> registro(s) incluído(s)!</pre>
				<?
				flush(); flush();

				$i=0;
				$totalAcao = 0;
				$dadosEnvio = array();
			}
			$prefix = sprintf("%02d", $i); // calcular novamente porque o i pode ter sido zerado		
	
			$dadosEnvio = array_merge($dadosEnvio, array("flgAtualizacao$prefix"=>"N"
				,"natu$prefix"=>$linha["ndpcod"]
				,"iuso$prefix"=>$linha["iducod"]
				,"fonte$prefix"=>$linha["foncod"]
				,"idoc$prefix"=>$linha["idocod"]
				,"valor$prefix"=>$linha["valor"]
			));
			$linhaAnterior = $linha;
			$totalAcao += $linha["valor"];
			$idAtual = $id;
			$i++;
		}
		catch(SidorCodReferenciaNaoEncontradoException $e) {
			$i=0;
			$totalAcao = 0;
			$dadosEnvio = array();
			?>
			<pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["tipoDetalhamento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - <?=$totalAcao?> - Código de referência não encontrado!</pre>
			<?
		}
		catch(SidorCargaException $e) {
			?>
			<pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["tipoDetalhamento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - <?=$totalAcao?> - Erro ao inserir dados!</pre>
			<?
		}
		catch(SidorException $e) {
			?>
			<pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["tipoDetalhamento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - <?=$totalAcao?> - Genérico! Chame o suporte.</pre>
			<?
		}
	}
	$tx = getmicrotimesidor() - $inicioTx;
	?>
	<pre>Dados incluídos em <?=number_format($tx, "5", ",", ".")?> segundos :: Executado em <?=date("d/m/Y H:i:s")?></pre>
	<pre><a href="Javascript:history.back(-3)">Voltar para o SIMEC</a></pre>
	<?
	exit();
*/
}
