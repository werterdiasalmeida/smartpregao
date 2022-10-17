<?
function deletarAgendamentoScripts($dados) {
	global $db;
	$sql = "UPDATE seguranca.agendamentoscripts SET agsstatus='I' WHERE agsid='".$dados['agsid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Agendamento deletado com sucesso');
			window.location='seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A';
		  </script>";	
}

function pegarDetalhesPeriodicidade($dados) {
	global $db;
	if($dados['agsid']) {
		$agsperdetalhes = $db->pegaUm("SELECT agsperdetalhes FROM seguranca.agendamentoscripts WHERE agsid='".$dados['agsid']."'");
		$agsperdetalhes = explode(";",$agsperdetalhes);
	} else {
		$agsperdetalhes = array();
	}
	
	switch($dados['agsperiodicidade']) {
		case 'diario':
			$html .= "<table>";
			$html .= "<tr>";
			
			$html .= "<td><input type=checkbox name=agsperdetalhes[] value=00 ".((in_array("00",$agsperdetalhes))?"checked":"")."> 00:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=04 ".((in_array("04",$agsperdetalhes))?"checked":"")."> 04:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=08 ".((in_array("08",$agsperdetalhes))?"checked":"")."> 08:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=12 ".((in_array("12",$agsperdetalhes))?"checked":"")."> 12:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=16 ".((in_array("16",$agsperdetalhes))?"checked":"")."> 16:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=20 ".((in_array("20",$agsperdetalhes))?"checked":"")."> 20:00<br/>
					</td>";
			
			$html .= "<td><input type=checkbox name=agsperdetalhes[] value=01 ".((in_array("01",$agsperdetalhes))?"checked":"")."> 01:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=05 ".((in_array("05",$agsperdetalhes))?"checked":"")."> 05:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=09 ".((in_array("09",$agsperdetalhes))?"checked":"")."> 09:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=13 ".((in_array("13",$agsperdetalhes))?"checked":"")."> 13:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=17 ".((in_array("17",$agsperdetalhes))?"checked":"")."> 17:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=21 ".((in_array("21",$agsperdetalhes))?"checked":"")."> 21:00<br/>
					</td>";
			
			$html .= "<td><input type=checkbox name=agsperdetalhes[] value=02 ".((in_array("02",$agsperdetalhes))?"checked":"")."> 02:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=06 ".((in_array("06",$agsperdetalhes))?"checked":"")."> 06:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=10 ".((in_array("10",$agsperdetalhes))?"checked":"")."> 10:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=14 ".((in_array("14",$agsperdetalhes))?"checked":"")."> 14:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=18 ".((in_array("18",$agsperdetalhes))?"checked":"")."> 18:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=22 ".((in_array("22",$agsperdetalhes))?"checked":"")."> 22:00<br/>
					</td>";
			
			$html .= "<td><input type=checkbox name=agsperdetalhes[] value=03 ".((in_array("03",$agsperdetalhes))?"checked":"")."> 03:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=07 ".((in_array("07",$agsperdetalhes))?"checked":"")."> 07:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=11 ".((in_array("11",$agsperdetalhes))?"checked":"")."> 11:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=15 ".((in_array("15",$agsperdetalhes))?"checked":"")."> 15:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=19 ".((in_array("19",$agsperdetalhes))?"checked":"")."> 19:00<br/>
						  <input type=checkbox name=agsperdetalhes[] value=23 ".((in_array("23",$agsperdetalhes))?"checked":"")."> 23:00<br/>
					</td>";
			
			
			$html .= "</tr>";
			$html .= "</table>";
			echo $html;
			break;
		case 'semanal':
			echo "<input type=checkbox name=agsperdetalhes[] value=1 ".((in_array('1',$agsperdetalhes))?"checked":"")."> Segunda-feira <input type=checkbox name=agsperdetalhes[] value=2 ".((in_array('2',$agsperdetalhes))?"checked":"")."> Terça-feira <input type=checkbox name=agsperdetalhes[] value=3 ".((in_array('3',$agsperdetalhes))?"checked":"")."> Quarta-feira <input type=checkbox name=agsperdetalhes[] value=4 ".((in_array('4',$agsperdetalhes))?"checked":"")."> Quinta-feira <input type=checkbox name=agsperdetalhes[] value=5 ".((in_array('5',$agsperdetalhes))?"checked":"")."> Sexta-feira <input type=checkbox name=agsperdetalhes[] value=6 ".((in_array('6',$agsperdetalhes))?"checked":"")."> Sabado <input type=checkbox name=agsperdetalhes[] value=0 ".((in_array('0',$agsperdetalhes))?"checked":"")."> Domingo";
			break;
		case 'mensal':
			for($i=1;$i<=31;$i++) {
				echo "<input type=checkbox name=agsperdetalhes[] value=".$i." ".((in_array($i,$agsperdetalhes))?"checked":"")."> ".$i; 
			}
			break;
			
	}
	
}

function inserirAgendamentoScripts($dados) {
	global $db;
	
	$sql = "INSERT INTO seguranca.agendamentoscripts(
            agsfile, agsperiodicidade, agsperdetalhes, agsstatus)
    		VALUES ('".$dados['agsfile']."', '".$dados['agsperiodicidade']."', ".(($dados['agsperdetalhes'])?"'".implode(";",$dados['agsperdetalhes'])."'":"NULL").", 'A');";
	
	$db->executar($sql);
	
	$db->commit();
	
	echo "<script>
			alert('Agendamento inserido com sucesso');
			window.location='seguranca.php?modulo=principal/agendamentoscriptslistar&acao=A';
		  </script>";
}

function atualizarAgendamentoScripts($dados) {
	global $db;
	
	$sql = "UPDATE seguranca.agendamentoscripts
	   		SET agsfile='".$dados['agsfile']."', agsperiodicidade='".$dados['agsperiodicidade']."', agsperdetalhes=".(($dados['agsperdetalhes'])?"'".implode(";",$dados['agsperdetalhes'])."'":"NULL")." 
	   		WHERE agsid='".$dados['agsid']."';";
	
	$db->executar($sql);
	
	$db->commit();
	
	echo "<script>
			alert('Agendamento atualizado com sucesso');
			window.location='seguranca.php?modulo=principal/agendamentoscriptsgerenciar&acao=A&agsid=".$dados['agsid']."';
		  </script>";
	
	
}
?>