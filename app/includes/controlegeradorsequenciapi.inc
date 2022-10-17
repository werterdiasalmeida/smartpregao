<?
function controlegeradorsequencialpi($identificador, $qtdnumeroseq) {
	global $db;
	$sequencial = array("0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","W","X","Y","Z");
	$sequencial_flipped = array_flip($sequencial);
	$seq = $db->pegaUm("SELECT gspseq FROM public.geradorsequencialpi WHERE gspidentificador = '".$identificador."' ORDER BY gspid DESC");
	//tratando sequencial de 4 digitos
	if($seq) {
		$keyseq4 = $sequencial_flipped[substr($seq,3,1)];
		$keyseq3 = $sequencial_flipped[substr($seq,2,1)];
		$keyseq2 = $sequencial_flipped[substr($seq,1,1)];
		$keyseq1 = $sequencial_flipped[substr($seq,0,1)];
		
		$passaUm = false;
		// tratando o digito 4
		if(substr($seq,3,1) == end($sequencial)) {
			$keyseq4 = 0;
			$passaUm = true;
		} else {
			$keyseq4 = $keyseq4 + 1;
		}
		// tratando o digito 3
		if(substr($seq,2,1) == end($sequencial)) {
			if($passaUm) {
				$keyseq3 = 0;
				$passaUm = true;
			}
		} else {
			if($passaUm) {
				$keyseq3 = $keyseq3 + 1;
				$passaUm = false;
			}
		}
		// tratando o digito 2
		if(substr($seq,1,1) == end($sequencial)) {
			if($passaUm) {
				$keyseq2 = 0;
				$passaUm = true;
			}
		} else {
			if($passaUm) {
				$keyseq2 = $keyseq2 + 1;
				$passaUm = false;
			}
		}
		// tratando o digito 1
		if(substr($seq,0,1) == end($sequencial)) {
			if($passaUm) {
				echo "FIM DAS SEQUENCIAS";
				exit;
			}
		} else {
			if($passaUm) {
				$keyseq4 = $keyseq4 + 1;
				$passaUm = false;
			}
		}
		$gspseq = $sequencial[$keyseq1].$sequencial[$keyseq2].$sequencial[$keyseq3].$sequencial[$keyseq4];
		$sql = "INSERT INTO geradorsequencialpi(gspseq, gspidentificador)
    			VALUES ('".$gspseq."', '".$identificador."');";
		$db->executar($sql);
		$db->commit();
		return substr($gspseq,(-1*$qtdnumeroseq));
	} else {
		$gspseq = $sequencial[0].$sequencial[0].$sequencial[0].$sequencial[1];
		$sql = "INSERT INTO geradorsequencialpi(gspseq, gspidentificador)
    			VALUES ('".$gspseq."', '".$identificador."');";
		$db->executar($sql);
		$db->commit();
		return substr($gspseq,(-1*$qtdnumeroseq));
	}
}
?>