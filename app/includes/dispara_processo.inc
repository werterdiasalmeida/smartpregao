<?php

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>
	 * Módulo: dispara_processo.inc
	 * Finalidade: dispara todos os processos automatizados no simec.
	 * Data de criação:
	 * Última modificação: 26/10/2006
	 */
function visao_gerencial($valor)
 {
 	global $db;
$pjeid=$valor; //$copvalorcampo;
$sql= "select ug.ungabrev,p.pjecod,p.pjedsc,to_char(p.pjedataini,'dd/mm/yyyy') as dataini, to_char(p.pjedatafim,'dd/mm/yyyy') as datafim, p.* , m.unmdsc, pr.prodsc from monitora.projetoespecial p inner join unidademedida m on p.unmcod=m.unmcod inner join produto pr on pr.procod=p.procod  inner join unidadegestora ug on ug.ungcod=p.ungcod where p.tpscod=11 and p.pjeid=".$pjeid;
// 

$res =  $db->pegalinha($sql);

// Transforma em variáveis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
$visao = "<center>
<table valign='top' >
  <tr bgcolor='#E9E9E9'>
    <td valign='top' align='center' width='35%'><B>SIMEC - VISÃO GERENCIAL DE PROJETO</td>
  </tr>
</table>
    
<table valign='top' >
  <tr bgcolor='#E9E9E9'>
    <td valign='top' width='35%'>
	  <table   width='100%' bgcolor='#f5f5f5' cellSpacing='0' cellPadding='0' border='1' bordercolor='#000000' style='border-collapse: collapse' >
        <tr >
          <td colspan='2' align='center' ><b>Dados do Projeto</b></td>
        </tr>	
        <tr>
          <td align='left' width='40%' ><b>Denominação:</b></td>
          <td><i><font face='Verdana' size='2'>$ungabrev$pjecod-$pjedsc</font></i></td>
      </tr>    ";

   $sql = " SELECT DISTINCT 
   				pfl.pflcod,
   				pfl.pfldsc AS descricao,
   				pfl.pflsncumulativo AS mostra,
   				usu.usucpf AS membro, 
   				usu.usuemail, 
   				usu.usunome || ' ('|| ee.entnome ||')' AS usuario,
   				usu.usufoneddd||'-'||usu.usufonenum AS fone 
   			FROM 
   				seguranca.perfil pfl 
   			LEFT JOIN 
   				monitora.usuarioresponsabilidade rpu ON rpu.pflcod = pfl.pflcod AND 
   														rpu.pjeid = " . $pjeid . " AND 
   														rpu.rpustatus='A' 
   			INNER JOIN 
   				seguranca.usuario usu ON usu.usucpf = rpu.usucpf 
   			LEFT JOIN 
   				entidade.entidade ee ON ee.entid = usu.entid 
   			WHERE
   				pfl.pflstatus = 'A' AND 
   				pfl.pflresponsabilidade IN ('E') 
   			ORDER BY 
   				pfl.pflcod";

 $rseq = @$db->carregar( $sql );
 if (  $rseq && count($rseq) > 0 )
	{
	 foreach ( $rseq as $linharesp )
		{
		 foreach($linharesp as $k=>$v) ${$k}=$v;
         if ($usuario ){
	        $linhar .= "<tr><td align='left' class='SubTituloDireita'><b>".$descricao.":</b></td><td><i><font face='Verdana' size='2'>".$usuario.' Telefone:'.$fone;
	        $linhar .= "</font></i></td></tr>";
		}
	}
}

    
$visao .= $linhar;

$visao .= "
   </table>
 </td>

 <td width='35%' valign='top'>
   <table valign='top' bordercolor='#000000' style='border-collapse: collapse' cellpadding='0' cellspacing='0' border='1'>
     <tr>
       <td colspan='2' align='center'>
        <b>Acompanhamento Físico e Financeiro</b></td>
     </tr>
";

         $sql = "select avgrealizado from monitora.avaliacaogenerico where pjeid=".$pjeid." order by avgdata desc limit 1";
         $realizado = $db->pegaUm($sql);
		 $percent='0';
		 if ($pjeprevistoano) $percent = number_format(($realizado/$pjeprevistoano)*100,0,',','.');
		 $sql = "select c.corsignificado as desc , trim(c.corimgav) as imagem, u.usunome,avgtexto,s.tpsdsc,s.tpscor from monitora.avaliacaogenerico ap inner join cor c on c.corcod=ap.corcod inner join seguranca.usuario u on u.usucpf=ap.usucpf inner join tiposituacao s on s.tpscod=ap.tpscod where avgliberada='t' and avgstatus='A' and pjeid = $pjeid order by avgdata desc limit 1";
		 //dbg($sql,1);
    	$respe=$db->pegalinha($sql);
		if(is_array($respe)) foreach($respe as $k=>$v) ${$k}=$v; 
		if ($tpsdsc) {
			$situacao = "$desc - $tpsdsc";	 
		}
		else $situacao='Sem avaliação!';
		if (! $avgtexto) $avgtexto='Sem comentários!';
$visao .= "
     <tr>
        <td align='left' width='50%' ><b>Período:</b></td>
        <td align='right'><i><font face='Verdana' size='2'>$dataini a $datafim</font></i></td>
     </tr>      
     <tr>
        <td align='left' width='50%' ><b>Produto:</b></td>
        <td align='right'><i><font face='Verdana' size='2'>$prodsc</font></i></td>
     </tr> 
   	 <tr>
        <td align='left' width='50%' ><b>Unidade de Medida:</b></td>
        <td align='right'><i><font face='Verdana' size='2'>$unmdsc</font></i></td>
     </tr>
   	 <tr>
        <td align='left' width='50%' ><b>Meta:</b></td>
        <td align='right'><i><font face='Verdana' size='2'>";
$visao .= number_format($pjeprevistoano,0,',','.');

$visao .= "</font></i></td>
     </tr>     
     <tr>
        <td align='left' width='50%'><b>Realizado até o momento / (Percentual):</b></td>
        <td align='right'><i><font face='Verdana' size='2'>$realizado / ($percent%)</font></i></td>
     </tr> ";

	  $sql="	select p.pjevlrano as vlrano,sum(e.expfinanceiro) as gasto 
          from monitora.projetoespecial p
           left join monitora.execucaopto e on p.pjeid=e.pjeid
         where p.pjeid=$pjeid group by p.pjevlrano";
       $res=$db->pegalinha($sql);
		if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v; 
       $financeiro="Total do projeto: R$ ".number_format($vlrano,2,',','.').
                   "<br>Gasto: R$ ".number_format($gasto,2,',','.').
                   "<br>Saldo: R$ ".number_format($vlrano-$gasto,2,',','.'); 

$visao .= "
    <tr>
       <td align='left' width='50%'><b>Financeiro:</b>
       </td><td align='right'><i><font face='Verdana' size='2'>$financeiro</font></i></td>
    </tr>
    <tr>
       <td align='left' width='50%'><b>Situação atual:</b>
       </td><td align='right'><i><font face='Verdana' size='2'>$situacao</font></i></td>
    </tr>   
         <tr>
       <td align='left' width='50%'><b>Comentário:</b>
       </td><td align='right'><i><font face='Verdana' size='2'>$avgtexto</font></i></td>
    </tr> 
  </table>
  </td>


  </tr>

</table>
<DIV><SPAN><A href=\"http://simec.mec.gov.br/geral/ganttgraph.php?cp='".md5_encrypt($pjeid,'')."'\"><B>GRÁFICO DE GANTT</A></SPAN></DIV>";
  return $visao;
 }
 
 
// este modulo foi criado para disparar os processos de todos os módulos que não devem depender da ação humana
		// os processos são disparado por módulos
		//inicialmente é necessário atualizar a tabela de controle processo
		$sql= "insert into controleprocesso (ctrdata, usucpf) values (current_date,'".$_SESSION['usucpf']."')";
		$db->executar($sql);
		$db->commit();
		
		// PROCESSOS DO MÓDULO DE PROJETO ESPECIAL
		
		// controle de riscos
		// nos projetos os riscos devem ser monitorados conforme a periodicidade
		$sql = "select pe.pjedsc,pe.pjeid,rp.rpeid, rp.rpedsc,rp.rpemonitoramento,rp.rpedatamonitorado from monitora.risco_projeto rp inner join monitora.planotrabalho p on p.ptoid=rp.ptoid inner join monitora.projetoespecial pe on pe.pjeid=p.pjeid and pe.pjestatus='A' and pe.tpscod in (1,4,9,10,11) where rp.rpestatus='A'";		
        $rs=$db->carregar($sql);
   		if (  $rs && count($rs) > 0 )
		{
	 		foreach ( $rs as $linha )
			{
		 		foreach($linha as $k=>$v) ${$k}=$v;
		 		// para cada risco verifico a diferença da data de monitoramento e a periodicidade planejada
		 		switch($rpemonitoramento )
				{
					case 'S': $periodo=7;
						break;
					case 'Q': $periodo=15;
						break;
					case 'M': $periodo=30;
						break;
					case 'T': $periodo=45;
						break;
					case 'Z': $periodo=180;
						break;
				}
		 		
		 		if (((date('y-m-a')-$rpedatamonitorado) >$periodo) or (! $rpedatamonitorado)) 
		 		{
		 			// então precisa gravar nova data
		 			$sql = "update monitora.risco_projeto set rpedatamonitorado=current_date+$periodo where rpeid=$rpeid";
		 			$db->executar($sql);	
		 			//neste ponto deve ser criada uma mensagem para os usuários envolvidos
		 			$sql = "insert into mensagem (msgassunto,msgconteudo,usucpf,sisid) values ('Monitoramento de Risco em Projeto','Monitorar o risco $rpedsc no Projeto $pjedsc','00000000191',6)";
		 			$saida=$db->executar($sql);
            		$sql =  "Select msgid from mensagem where oid = ".pg_last_oid($saida);		 			    $ultimomsgid = $db->pegaUm( $sql );
		 					 			
		 			$sql = "select distinct ur.usucpf,u.usuemail from monitora.usuarioresponsabilidade ur inner join seguranca.usuario u on u.usucpf=ur.usucpf where pjeid=$pjeid and ur.pflcod in (12,47,51,57)";
		 			$rsm=$db->carregar($sql);
		 			$usuemails='';
   					if (  $rsm && count($rs) > 0 )
					{
	 					foreach ( $rsm as $linha )
						{
		 					foreach($linha as $k=>$v) ${$k}=$v;
		 					$sql = "insert into mensagemusuario (msgid,usucpf) values ($ultimomsgid,'$usucpf')";
		 					$saida=$db->executar($sql);
		 					$usuemails .= $usuemail.',';
		 					
						}
					}
					if ($usuemails)
					{
						// envia email 
						email_pessoal('Sistema','Sistema', $usuemails, 'Monitoramento de Risco em Projeto', "O Sistema está lhe informando que é necessário monitorar o risco $rpedsc no Projeto $pjedsc", '','');
					}
					$db->commit();					
		 		}		 		
			}
		}

		// controle de plano gerencial
		// nos projetos o plano gerencial deve ser acompanhado conforme a data de inicio e o aviso de antecedencia
		// ou seja, para cada atividade verificar a data de inicio menso a antecednecia e ver se está na hora de avisar, desde que a atividade esteja aprovada e o projeto esteja em execução.
		$sql = "select pe.pjedsc,pe.pjeid,pg.ptoid, pg.ptocod||'-'||pg.ptodsc as descricao,pg.ptosntemdono as temdono,pg.usucpf as dono, to_char(pg.ptodata_ini,'YYYY-MM-DD') as ptodata_ini,pg.ptoavisoantecedencia from monitora.planotrabalho pg inner join monitora.projetoespecial pe on pe.pjeid=pg.pjeid and pe.pjestatus='A' and pe.tpscod in (11) where pg.ptostatus='A' and pg.ptosnaprovado='t'";		
        $rs=$db->carregar($sql);
   		if (  $rs && count($rs) > 0 )
		{			
			// verifico quem é o coordenador
	 		foreach ( $rs as $linha )
			{
		 		foreach($linha as $k=>$v) ${$k}=$v;
		 		$sql = "select u.usucpf as cpfcoord, u.usuemail as emailcoord from monitora.projetoespecial pe inner join monitora.usuarioresponsabilidade ur on ur.pjeid=pe.pjeid and ur.pflcod=47 inner join seguranca.usuario u on u.usucpf=ur.usucpf where pe.pjeid=$pjeid";
			   $resw=$db->pegaLinha($sql);
			   foreach( (array) $resw as $k=>$v) ${$k}=$v;
		 		
    	 		if (strtotime($ptodata_ini) <= strtotime(date('Y-m-d')) - $ptoavisoantecedencia)
		 		{	 			
		 		// para cada item do plano de trabalho que pode ser monitorado verifico se já foi feito algum acompanhamento dentro do período de antecedencia
                $sql = "select expid from monitora.execucaopto where ptoid=$ptoid ";
              
                if (! $db->pegaUm($sql))
                {
                	// então precisa avisar
                	// para avisar precisa ver se pode avisar a qualquer um ou a apenas o coordenador                    	// verifica se a mensagem já foi criada
                	$sql = "select msgid from mensagem where msgconteudo = 'Acompanhar a atividade $descricao no Projeto $pjedsc'";

                 	if (! $db->pegaUm($sql))
                 	{
                 		// neste ponto deve ser criada uma mensagem para os usuários envolvidos
                	$sql = "insert into mensagem (msgassunto,msgconteudo,usucpf,sisid) values ('Acompanhamento de atividade em Projeto','Acompanhar a atividade $descricao no Projeto $pjedsc','00000000191',6)";
		 				$saida=$db->executar($sql);
            			$sql =  "Select msgid from mensagem where oid = ".pg_last_oid($saida);		 			    
            			$ultimomsgid = $db->pegaUm( $sql );
                		$usuemails='';
						$sql = "insert into mensagemusuario (msgid,usucpf) values ($ultimomsgid,'$cpfcoord')";
						$saida=$db->executar($sql);
		 				$usuemails .= $emailcoord.',';                	 
                		if ($temdono=='t' and $dono)
                		{
                	  			// avisa ao coordenador e ao dono
		 					$sql = "select u.usuemail as emaildono from seguranca.usuario u where u.usucpf='$dono'";
							$emaildono=$db->pegaUm($sql);
							$sql = "insert into mensagemusuario (msgid,usucpf) values ($ultimomsgid,'$dono')";
                			$saida=$db->executar($sql);
		 					$usuemails .= $emaildono.',';  
                	  
                		}
                		// envia msg por email
						if ($usuemails)
						{
							// envia email 
							email_pessoal('Sistema','Sistema', $usuemails, 'Acompanhamento de atividade em Projeto', "Acompanhar a atividade $descricao no Projeto $pjedsc", '','');
						}
						$db->commit();                	
                 	}
                }
			}
		 		
			}
		}
		// controle de comunicações padronizadas
		// nos projetos as comunicações padronizadas são por tipo
		// ou seja, para cada atividade verificar a data de inicio menso a antecednecia e ver se está na hora de avisar, desde que a atividade esteja aprovada e o projeto esteja em execução.

		$sql = "select cp.copvalorcampo, c.comid, c.comperiodicidade, to_char(c.comdataini,'YYYY-MM-DD') as dataini,to_char(c.comdatafim,'YYYY-MM-DD') as datafim, c.asscod from comunicacao c inner join comunicacao_processo cp on cp.comid=c.comid and trim(cp.copcampo)='pjeid' inner join monitora.projetoespecial pe on pe.pjeid = cp.copvalorcampo and pe.pjestatus='A' and pe.tpscod in (11) where c.comstatus='A'";

        $rs=$db->carregar($sql);
   		if (  $rs && count($rs) > 0 )
		{			
	 		foreach ( $rs as $linha )
			{
		 		foreach($linha as $k=>$v) ${$k}=$v;
		 		// verifico se está dentro do prazo de mandar a comunicação

    	 		if (strtotime($dataini) <= strtotime(date('Y-m-d')) and strtotime($datafim) >= strtotime(date('Y-m-d')))
		 		{	
 			     // então está dentro do prazo de aviso
 			     // verifico os usuários da comunicação
 			     $sql = "select * from comunicacao_destinatario where comid=$comid";
 			     $rsd=$db->carregar($sql);
 			     $visao='';
   				 if (  $rsd && count($rsd) > 0 )
				 { 
				 	foreach ( $rsd as $linha )
					{
						foreach($linha as $k=>$v) ${$k}=$v;
						if ($pflcod)
						{
							// então vai se comunicar pelo perfil
							if (! $cdsdatainc ) $cdsdatainc='1999-01-01';
							
							if (strtotime($cdsdatainc) <= strtotime(date('Y-m-d'))-$comperiodicidade)
							{
								// então está na hora de comunicar
								// atualiza o registro
								$sql= "update comunicacao_destinatario set cdsdatainc=current_date + $comperiodicidade where cdsid=$cdsid";
								$saida=$db->executar($sql);
								// busca todos os interessados com o perfil definido
								$sql = "select ur.usucpf, u.usunome,u.usuemail as emaildest from monitora.usuarioresponsabilidade ur inner join seguranca.usuario u on u.usucpf=ur.usucpf where ur.pflcod=$pflcod and pjeid=$copvalorcampo";

								$rsu=$db->carregar($sql);
   				 				if (  $rsu && count($rsu) > 0 )
				 				{ 
				 					foreach ( $rsu as $linha )
									{
										foreach($linha as $k=>$v) ${$k}=$v;
										// verifico o tipo de comunicação

										if ($asscod=='01')
										{
											// visão gerencial
											// envia email com a visão gerencial
											$visao=visao_gerencial($copvalorcampo);
											 email_pessoal('Sistema','Sistema', $emaildest, 'Visão Gerencial em Projeto', "$visao", '','');
											
										}
									}
				 				}
								
							}
						}
						else 
						{
							// será por pessoa
							if (! $cdsdatainc ) $cdsdatainc='1999-01-01';
							if (strtotime($cdsdatainc) <= strtotime(date('Y-m-d'))-$comperiodicidade)
							{
								// então está na hora de comunicar
								// atualiza o registro
								$sql= "update comunicacao_destinatario set cdsdatainc=current_date + $comperiodicidade where cdsid=$cdsid";
								$saida=$db->executar($sql);
								// busca todos os interessados com o perfil definido
								$sql = "select u.usunome,u.usuemail as emaildest from seguranca.usuario u where u.usucpf='$usucpf'";
								
								$rsu=$db->carregar($sql);
   				 				if (  $rsu && count($rsu) > 0 )
				 				{ 
				 					foreach ( $rsu as $linha )
									{
										foreach($linha as $k=>$v) ${$k}=$v;
										// verifico o tipo de comunicação

										if ($asscod=='01')
										{
											// visão gerencial
											// envia email com a visão gerencial
											$visao=visao_gerencial($copvalorcampo);
											 email_pessoal('Sistema','Sistema', $emaildest, 'Visão Gerencial em Projeto', "$visao", '','');
										}
									}
				 				}
								
							}							
							
						}
						$db->commit();
					}
				 	
				 }

              }
		 		
			}
		}		
		// PROCESSOS DO MÓDULO DE ELABORAÇÃO E REVISÃO
		// PROCESSOS DO MÓDULO MONITORAMENTO
		// PROCESSOS DO MÓDULO DE ORÇAMENTO
		// PROCESSOS DO MODULO FINANCEIRO
		
	?>