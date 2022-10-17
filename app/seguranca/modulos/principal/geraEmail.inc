<?php
			$remetente = array('nome'=>'Sistema', 'email'=>'');
//			$assunto  = "URGENTE - Reabertura de prazo para envio de obras do PAC 2";   CASO 1 e 2
//			$assunto  = "[SIMEC]Informativo PAR 2010 - Obras aprovadas (PAC 2)";        CASO 3 e 4
			$assunto  = "Informativo PAR 2010 - Obras arquivadas (PAC 2)";
			$caso = 6;
			
			/* CASO 1
			$sql = "select distinct
						ent.entemail,
						mun.muncod,
						mun.mundescricao
					from 
						entidade.entidade ent
					inner join entidade.endereco ende on ende.entid = ent.entid
					inner join entidade.funcaoentidade fun on fun.entid = ent.entid and fun.funid = 1
					inner join obras.preobra pre ON pre.muncod = ende.muncod AND pre.prestatus = 'A' AND pre.ptoid in (1,2,3,6,7)
					INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid not in (228,229)
					inner join territorios.municipio mun on mun.muncod = ende.muncod
					INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150)
			--		where
			--			ende.muncod = '3136702'
					";
			
			CASO 2
			$sql = "select distinct
						ent.entemail,
						mun.muncod,
						mun.mundescricao
					from 
						entidade.entidade ent
					inner join entidade.endereco ende on ende.entid = ent.entid
					inner join entidade.funcaoentidade fun on fun.entid = ent.entid and fun.funid = 1
					inner join obras.preobra pre ON pre.muncod = ende.muncod AND pre.prestatus = 'A' AND pre.ptoid in (5)
					INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid not in (228,229)
					inner join territorios.municipio mun on mun.muncod = ende.muncod
					INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
			--		where
			--			ende.muncod = '3136702'
					";

			CASO 3
			$sql = "select distinct
						ent.entemail,
						mun.muncod,
						mun.mundescricao
					from 
						entidade.entidade ent
					inner join entidade.endereco ende on ende.entid = ent.entid
					inner join entidade.funcaoentidade fun on fun.entid = ent.entid and fun.funid = 1
					inner join obras.preobra pre ON pre.muncod = ende.muncod AND pre.prestatus = 'A' AND pre.ptoid in (1,2,3,6,7)
					INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid in (228)
					inner join territorios.municipio mun on mun.muncod = ende.muncod
					INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
					";

			CASO 4
			$sql = "select distinct
						ent.entemail,
						mun.muncod,
						mun.mundescricao
					from 
						entidade.entidade ent
					inner join entidade.endereco ende on ende.entid = ent.entid
					inner join entidade.funcaoentidade fun on fun.entid = ent.entid and fun.funid = 1
					inner join obras.preobra pre ON pre.muncod = ende.muncod AND pre.prestatus = 'A' AND pre.ptoid in (5)
					INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid in (228)
					inner join territorios.municipio mun on mun.muncod = ende.muncod
					INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
					";

			CASO 5
			$sql = "select distinct
						ent.entemail,
						mun.muncod,
						mun.mundescricao
					from 
						entidade.entidade ent
					inner join entidade.endereco ende on ende.entid = ent.entid
					inner join entidade.funcaoentidade fun on fun.entid = ent.entid and fun.funid = 1
					inner join obras.preobra pre ON pre.muncod = ende.muncod AND pre.prestatus = 'A' AND pre.ptoid in (1,2,3,6,7)
					INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid in (229)
					inner join territorios.municipio mun on mun.muncod = ende.muncod
					INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
					";
			*/
			$sql = "select distinct
						ent.entemail,
						mun.muncod,
						mun.mundescricao
					from 
						entidade.entidade ent
					inner join entidade.endereco ende on ende.entid = ent.entid
					inner join entidade.funcaoentidade fun on fun.entid = ent.entid and fun.funid = 1
					inner join obras.preobra pre ON pre.muncod = ende.muncod AND pre.prestatus = 'A' AND pre.ptoid in (5)
					INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid in (229)
					inner join territorios.municipio mun on mun.muncod = ende.muncod
					INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
					";
			
			$arrDados = $db->carregar( $sql );
			
			foreach( $arrDados as $dados ){
					
			/*	CASO 1
			 * $sql = "SELECT
							pre.predescricao
						FROM
							obras.preobra pre
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid not in (228,229)
						INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150)
						WHERE
							pre.muncod = '{$dados['muncod']}'
							AND pre.prestatus = 'A'
							AND pre.ptoid in (1,2,3,6,7)";

				CASO 2
				$sql = "SELECT
							pre.predescricao
						FROM
							obras.preobra pre
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid not in (228,229)
						INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
						WHERE
							pre.muncod = '{$dados['muncod']}'
							AND pre.prestatus = 'A'
							AND pre.ptoid in (5)";

				CASO 3				
				$sql = "SELECT
							pre.predescricao
						FROM
							obras.preobra pre
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid in (228)
						INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
						WHERE
							pre.muncod = '{$dados['muncod']}'
							AND pre.prestatus = 'A'
							AND pre.ptoid in (1,2,3,6,7)";
		
				CASO 4
				$sql = "SELECT
							pre.predescricao
						FROM
							obras.preobra pre
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid in (228)
						INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
						WHERE
							pre.muncod = '{$dados['muncod']}'
							AND pre.prestatus = 'A'
							AND pre.ptoid in (5)";

				CASO 5
				$sql = "SELECT
							pre.predescricao
						FROM
							obras.preobra pre
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid in (229)
						INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
						WHERE
							pre.muncod = '{$dados['muncod']}'
							AND pre.prestatus = 'A'
							AND pre.ptoid in (1,2,3,6,7)";
			*/
				$sql = "SELECT
							pre.predescricao
						FROM
							obras.preobra pre
						INNER JOIN workflow.documento doc ON doc.docid = pre.docid and doc.esdid in (229)
						INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = pre.muncod and mtm.tpmid in (150, 151)
						WHERE
							pre.muncod = '{$dados['muncod']}'
							AND pre.prestatus = 'A'
							AND pre.ptoid in (5)";
				
				$arrObras = $db->carregarColuna( $sql );
				$i = 0;
				$obras = "";
				
				foreach( $arrObras as $obra ){
					$i++;
					$obras.= "Obra: ".$obra."<br><br>";
				}
				
				if( $caso == 1 ){
					$conteudo = "
						Senhor(a) Dirigente Municipal
						<br><br>
						Informamos que o município de {$dados['mundescricao']} apresentou propostas de obras para construção de unidades de educação infantil no âmbito do PAC 2 que não foram selecionadas tecnicamente, ou não foram enviadas dentro do prazo estabelecido, conforme relação abaixo:
						<br><br>
						{$obras}
						Os motivos que determinaram a não seleção das obras estão detalhados em http://simec.mec.gov.br, módulo PAR 2010, na aba análise de engenharia.
						<br><br>
						As propostas que não tramitaram no prazo poderão ser alteradas e enviadas para análise. 
						<br><br>
						O município poderá ajustar todos os campos das propostas, inclusive alterar o terreno, e enviar pelo SIMEC para nova análise até 31/01/2011.
					";
				} elseif( $caso == 2 ){
					$conteudo = "
						Senhor(a) Dirigente Municipal
						<br><br>
						Informamos que o município de {$dados['mundescricao']} apresenta propostas de obras para construção de quadras escolares poliesportivas no âmbito do PAC 2 que não foram selecionadas tecnicamente, ou não foram enviadas dentro do prazo estabelecido, conforme relação abaixo:
						<br><br>
						{$obras}
						Os motivos que determinaram a não seleção das obras estão detalhados em http://simec.mec.gov.br, módulo PAR 2010, na aba análise de engenharia.
						<br><br>
						As propostas que não tramitaram no prazo poderão ser alteradas e enviadas para análise. 
						<br><br>
						O município poderá ajustar todos os campos das propostas, inclusive alterar o terreno, e enviar pelo SIMEC para nova análise até 31/01/2011.
					";
				} elseif( $caso == 3 ){
					$conteudo = "
						Senhor(a) Dirigente Municipal
						<br><br>
						Informamos que o município de {$dados['mundescricao']} apresentou propostas de obras para construção de unidades de educação infantil no âmbito do PAC 2 selecionadas tecnicamente, conforme relação abaixo:
						<br><br>
						{$obras}
						As próximas etapas serão divulgadas no sítio do MEC/FNDE ou no http://simec.mec.gov.br.
					";
				} elseif( $caso == 4 ){
					$conteudo = "
						Senhor(a) Dirigente Municipal
						<br><br>
						Informamos que o município de {$dados['mundescricao']} apresentou propostas de obras para construção de quadras escolares poliesportivas no âmbito do PAC 2 selecionadas tecnicamente, conforme relação abaixo:
						<br><br>
						{$obras}
						As próximas etapas serão divulgadas no sítio do MEC/FNDE ou no http://simec.mec.gov.br.
					";
				} elseif( $caso == 5 ){
					$conteudo = "
						Senhor(a) Dirigente Municipal
						<br><br>
						Informamos que o município de {$dados['mundescricao']} apresentou propostas de obras para construção de unidades de educação infantil no âmbito do PAC 2 que não foram selecionadas por excederem o limite de atendimento previsto para o município nessa seleção, conforme relação abaixo:
						<br><br>
						{$obras}
					";
				} elseif( $caso == 6 ){
					$conteudo = "
						Senhor(a) Dirigente Municipal
						<br><br>
						Informamos que o município de {$dados['mundescricao']} apresentou propostas de obras para construção de quadras escolares poliesportivas no âmbito do PAC 2 que não foram selecionadas tecnicamente, conforme relação abaixo:
						<br><br>
						{$obras}
					";
				}
				
			//	ver($arrDados, $obras, $conteudo, d);
			/*	$conteudo = "
					<font size='2'><b>Atividade:</b> <u>1111</u><font>
					<br><br>
					<b><font size='2'>A data final da atividade 333 foi alterada para uma data posterior a data da atividade pai pelo usuário 333. Por favor verifique.<font></b>
					<br><br>
					<br>	
					$texto	
					<br>		
					Atenciosamente,
					<br>	
					$assunto";*/
					
			
				if( $dados['entemail'] ){
					enviar_email($remetente, $dados['entemail'], $assunto, $conteudo );
			//		enviar_email($remetente, 'victor.benzi@mec.gov.br', $assunto, $conteudo );
				} else {
					echo $dados['mundescricao']." <br>";
				}
					
			}


// CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
print 'E-mail enviado com Sucesso!';



?>
<script language="JavaScript"></script>
<script type="text/javascript" 	src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript"></script>