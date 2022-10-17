<?php

	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	//$coordenador = $db->testa_responsavel_projespec($_SESSION['pjeid']);
	//$ehdocumentador = $db->testa_documentador_projespec($_SESSION['pjeid']);

	
	function passa_sigilo($sigilo,$docid,$donoarquivo)
	{
		// 		
		global $db;
		$ok=false;
		if ($sigilo=='Ostensivo') $ok= true;
		if ($sigilo=='Confidencial' )
		{
			if ($donoarquivo==$_SESSION['usucpf']) $ok= true;
		}
		if ($sigilo=='Reservado' )
		{
				
			// verifica a tabela de documento_acesso
     	 $sql = "select * from documento_acesso where docid=$docid and usucpf='".$_SESSION['usucpf']."' and doastatus='A'"	;
     	 $lista_acesso = $db->pegaLinha($sql);
     	 if (is_array($lista_acesso)) $ok= true;

		}
		return $ok;
		
	}
	
	
	if ( !isset( $_SESSION['indice_sessao_popup_arquivo'][$_REQUEST['nome']] ) )
	{
		print '<html><script language="javascript"> alert( "Dados da sessão perdidos. Possivelmente sua sessão expirou." ); window.close(); </script></hmtl>';
		exit();
	}
	
	$dados_sessao = $_SESSION['indice_sessao_popup_arquivo'][$_REQUEST['nome']];
	$tipo_vinculo = $dados_sessao[ 'campo_id_vinculo' ];
	$valor_vinculo = $dados_sessao[ 'valor_id_vinculo' ];
	$tabela_vinculo = $dados_sessao[ 'tabela_vinculo' ];
  
    $sql = "select usucpf from agenda_reuniao where agrid=".$valor_vinculo;
	$marcoureuniao = $db->pegaum($sql);


	if( $_REQUEST[ 'act' ] == 'inserir' )
	{
		$posPonto = strrpos( $_FILES[ 'arquivo' ][ 'name' ], '.' );
		$nomeOriginal = substr( $_FILES[ 'arquivo' ][ 'name' ], 0, $posPonto );
		$ext = substr( $_FILES[ 'arquivo' ][ 'name' ], $posPonto );
		$sql = "select nextval('documento_docid_seq'::regclass)";
		$id = $db->pegaUm( $sql );
		$nomeFisico = $nomeOriginal . "_" . $id . $ext;
		//$dir = "../upload/" . $nomeFisico;
		$dir = APPRAIZ ."arquivos/anexos/" . $nomeFisico;
		if (! $_REQUEST[ 'docsigilo' ]) $_REQUEST[ 'docsigilo' ]='O';
		
		$sql = " insert into public.documento( docid, docdsc, tpdcod, docnomefisico,docsigilo,usucpf )" .
			   " values(" .
			   $id . ", " .
			   "'" . $_REQUEST[ 'docdsc' ] . "', " .
			   $_REQUEST[ 'tpdcod' ] . ", " .
			   "'" . $nomeFisico . "', '" .$_REQUEST[ 'docsigilo' ]."',".
			   "'" . $_SESSION[ 'usucpf' ] . "'" .
			   ")";
		$db->executar( $sql );

		$sql = "insert into public.documento_processo(proid,docid,tabela_processo) values ($valor_vinculo, $id,'$tabela_vinculo')";
		$db->executar( $sql );
		
        if ($_REQUEST[ 'docsigilo' ]=='R')
        {
        	// é necessário considerar a equipe do projeto, ação, programa, etc
        	if ($_SESSION['sisid']==6)
        	{
        		// projeto especial
        		 $sql = "select distinct usu.usucpf from seguranca.perfil pfl left join monitora.usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.pjeid = ".$_SESSION['pjeid']." and rpu.rpustatus='A' inner join seguranca.usuario usu on usu.usucpf=rpu.usucpf where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('E')";
        		 $permissoes = $db->carregar($sql);
        		 foreach ($permissoes as $permissao) 
        		 {
        		 	$sql = "insert into documento_acesso (docid,usucpf) values ($id,'".$permissao['usucpf']."')";
        		 	$db->executar($sql);
        		 }
        	}
        	if ($_SESSION['sisid']==1)
        	{
        		// monitoramento e avaliação
  			   $sql = "select distinct usu.usucpf	from perfil p 
				inner join monitora.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
				inner join usuario usu on usu.usucpf = ur.usucpf 
				where p.pflstatus = 'A' and ur.acaid = ".$_SESSION['acaid']." and ur.rpustatus = 'A' and ur.prsano = '".$_SESSION['exercicio']."'";
           		 $permissoes = $db->carregar($sql);
        		 foreach ($permissoes as $permissao) 
        		 {
        		 	$sql = "insert into documento_acesso (docid,usucpf) values ($id,'".$permissao['usucpf']."')";
        		 	$db->executar($sql);
        		 }
        	}        	
        	
        }
		
		
		
		
		
		$sql = "select 'Assunto:'||agrassunto ||'--- Data da reunião:'|| to_char(agrdata,'dd/mm/yyyy') as reuniao from agenda_reuniao where agrid = $valor_vinculo";
		$assuntoreuniao = $db->pegaUm($sql);
		//dbg($sql,1);
				
		if ($_REQUEST[ 'tpdcod' ]==3) // o documento é uma ata
		{
			
			// verificar os participantes da reunião
			$sql = "select distinct arp.usucpf, u.usuemail from agenda_reuniao_participante arp inner join $tabela_vinculo tv on tv.agrid=arp.agrid inner join documento_processo dp on dp.proid=tv.agrid and tv.agrid=$valor_vinculo inner join seguranca.usuario u on u.usucpf=arp.usucpf ";
			
	        $participantes = $db->carregar($sql);

			if ($_REQUEST[ 'ata' ]==3) // já existia ata válida e está sendo substituída temporariamente
		    {
		    	// atualiza os registros existentes tornando apenas histórico
		    	$sql = "update public.ata_acompanhamento set aacsituacao = 'T' where aacsituacao='A' and docid in (select d.docid from documento d inner join documento_processo dp on dp.docid=d.docid and dp.proid=$valor_vinculo where d.tpdcod=3 ) ";
			    $db->executar( $sql );
		    }
		    
            foreach ($participantes as $participante)
            {		    
		       $sql = "insert into public.ata_acompanhamento(docid,usucpf) values($id,'" . $participante['usucpf']."')";
		       $saida=$db->executar($sql);
            }

            if ($_REQUEST[ 'ata' ]==3) // já existia ata válida e os paraceres precisam ser migrados
		    {
		    	foreach ($participantes as $participante)
           		 {		 
           		 	$sql = "select aacparecer , aacparecertexto, usucpf from ata_acompanhamento where aacsituacao='T' and usucpf='".$participante['usucpf']."' and docid in (select d.docid from documento d inner join documento_processo dp on dp.docid=d.docid and dp.proid=$valor_vinculo where d.tpdcod=3 ) limit 1";
		    	    $dados_parecer = $db->carregar($sql); 
		       		$sql = "update ata_acompanhamento set aacparecer = '".$dados_parecer[0]['aacparecer']."' , aacparecertexto ='".$dados_parecer[0]['aacparecertexto']."' where docid = $id and usucpf = '".$participante['usucpf']."'";

		            $saida=$db->executar($sql);
           		 }
		    	 
		    }
		    if ($_REQUEST[ 'ata' ]==3) // já existia ata válida e está sendo substituída
		    {
		    	
		    	 	// atualiza os registros existentes tornando apenas histórico
		    	$sql = "update public.ata_acompanhamento set aacsituacao = 'S' where aacsituacao='T' and docid in (select d.docid from documento d inner join documento_processo dp on dp.docid=d.docid and dp.proid=$valor_vinculo where d.tpdcod=3 ) ";
			    $db->executar( $sql );
		    }   
		    // envia e-mail avisando os participantes
     		
            if ($_REQUEST[ 'ata' ]==3) 
               $assunto = 'Substituição de Ata de reunião';
            else 
               $assunto = 'Inclusão de Ata de reunião';
               $usuemail= '';
            foreach ($participantes as $participante)
            {
            	$usuemail .= $participante['usuemail'].',';
            }
 $usuemail='gilbertocerqueira@mec.gov.br';
		    $mensagem = "Sr(a) Usuário. <p>Reportamos que foi inserida ou substituída uma Ata relativa à reunião com o $assuntoreuniao da qual VSa. participou. Pedimos que entre no sistema e dê o seu parecer sobre a Ata, concordando ou discordando de seu conteúdo. Em caso de discordância, pedimos que apresente também a sua justificativa. <p> Grato";
           // email("Partcipantes de reunião agendada no Simec", $usuemail, $assunto, $mensagem);
		}
		if( move_uploaded_file( $_FILES[ "arquivo" ]["tmp_name"], $dir ) )
		{
			$msg = 'Arquivo inserido com sucesso';
			$db->commit();
		}
		//else 
		//dbg('erro de upload',1);
	}
	
	
?>
<html>
	<head>
		<title><?= $_REQUEST[ 'acao' ] == 'I' ? 'Inclusão de novo arquivo' : 'Listagem de arquivos';?></title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<style type="text/css">
			body{margin:0; padding:0;}
		</style>
		<script type="text/javascript">
			function inserir_arquivo()
			{
				if( !validaBranco( document.formulario.tpdcod, "Tipo do arquivo" ) ) return;
				if( !validaBranco( document.formulario.docdsc, "Título do arquivo" ) ) return;
				if( !validaBranco( document.formulario.arquivo, "Arquivo" ) ) return;
				
				if( document.formulario.permiteata.value != '1' && document.formulario.tpdcod==3)
				{
					alert ('Você não tem direito a incluir Ata neste processo!');
					return;
				}
				
				
				if( document.formulario.ata.value == 'A' &&  document.formulario.tpdcod.value == 3) 
				{
					if( window.confirm( "Já existe Ata cadastrada neste processo. A inclusão de nova Ata irá tornar as anteriores em histórico. Confirma a inclusão de nova Ata?") )
     				{
						document.formulario.ata.value = 3;
						document.formulario.act.value = "inserir";
						document.formulario.submit();
    				} else return;	 		
				}
				document.formulario.act.value = "inserir";
				document.formulario.submit();
			}
			<? if( $msg ): ?>
				alert( '<?=$msg?>' );
			<? endif; ?>
		</script>
	</head>
	<body>
		<form name="formulario" method="post" enctype="multipart/form-data">
		
<?

	// verifico se para o processo já existe ata gravada.
	// caso exista ata o sistema deverá alertar que as demais serão alteradas como substituídas
	$sql = "select d.usucpf, d.docid, aa.aacsituacao from documento d inner join documento_processo dp on dp.proid = $valor_vinculo and d.docid=dp.docid inner join ata_acompanhamento aa on aa.docid=dp.docid and aa.aacsituacao in ('A','F') where d.tpdcod=3 limit 1";
// dbg($sql);
 //   $ExisteAta = $db->pegalinha($sql);
    if ($coordenador or $marcoureuniao==$_SESSION['usucpf'] or $ehdocumentador)
    print "<input type='hidden' name='permiteata' value='1'>";	
    else print "<input type='hidden' name='permiteata' value='0'>";
   if (is_array($ExisteAta)) { ?>
   <input type="hidden" name="ata" value="<?=$ExisteAta['aacsituacao']?>" >	
   <?} else {?>	
   <input type="hidden" name="ata" value="0" >	 
   <?}?>  
			<input type="hidden" name="act" value="" >
			<table cellpadding="0" cellspacing="0" border="0" width="100%" class="listagem">
				<? if( $_REQUEST[ 'acao' ] == 'I' ): ?>
					<tr>
						<td colspan="2" bgcolor="#adadad" class="TituloTela" style="color:#000000;border:none;padding:10px;">Inclusão de arquivo</td>
					</tr>
					<tr bgcolor="#cdcdcd">
						<td class="SubTituloDireita" style="border:none; padding:3px;">Tipo do arquivo:</td>
						<td style="border:none; padding:3px;">
						<?
							if 	($tabela_vinculo == 'agenda_reuniao')
								$sql = "select tpdcod as codigo, tpddsc as descricao from public.tipodocumento where tpdstatus='A' order by tpddsc asc";
								else 
								$sql = "select tpdcod as codigo, tpddsc as descricao from public.tipodocumento where tpdstatus='A' and tpdcod <> 3 order by tpddsc asc";								
							$db->monta_combo( "tpdcod", $sql, 'S', "Selecione o tipo do arquivo", '', '', 'Selecione o tipo do arquivo', 200 );
						?>
						</td>
					</tr>
					<tr bgcolor="#cdcdcd">
						<td class="SubTituloDireita">Título:</td>
						<td style="border:none; padding:3px;">
							<?=campo_texto('docdsc','S','S','',40,150,'','','','Entre com uma breve descrição sobre o arquivo.');?>
						</td>
					</tr>
<?
	if ($_SESSION['sisid']==6)
	{
		?>
					<tr bgcolor="#cdcdcd">
						<td class="SubTituloDireita">Grau de sigilo:</td>
						<td style="border:none; padding:3px;">
							<?

				$opcoes = array
				(
					"Ostensivo" => array
					(
							"valor" => "O",
							"id"    => "O"
					),
					"Reservado" => array
					(
							"valor" => "R",
							"id"    => "R"
					),
					"Confidencial" => array
					(
							"valor" => "C",
							"id"    => "C"
					)
				);

				campo_radio( 'docsigilo', $opcoes, 'h' );				
			?>
                       
						</td>
					</tr>
<?}?>
					<tr bgcolor="#cdcdcd">
						<td class="SubTituloDireita" style="border:none; padding:3px;">Arquivo:</td>
						<td style="border:none; padding:3px;"><input type="file" name="arquivo" /></td>
					</tr>
					<tr bgcolor="#adadad">
						<td></td>
						<td style="border:none; padding:3px;"><input type="button" value="Inserir" onclick="inserir_arquivo();" /></td>
					</tr>
				<? else: ?>
					<tr>
						<td colspan="2" bgcolor="#adadad" class="TituloTela" style="color:#000000;border:none;padding:10px;">Listagem de arquivos</td>
					</tr>
					<?
						$rs = $db->carregar( $dados_sessao[ 'sql' ] );
						//dbg($dados_sessao[ 'sql' ]);
						
						$i = 0;
						if( $rs ):
							foreach( $rs as $linha ):
								foreach( $linha as $k => $v )
								{
									${$k} = $v;
								}
								$cor = $i % 2 == 0 ? "#f4f4f4" : "#e0e0e0";
					?>
					<? if (passa_sigilo($sigilo,$docid,$donoarquivo)) {?>
								<tr bgcolor="<?=$cor?>">
								<td width="70%" style="padding:10px;"><a href="../geral/baixaranexo.php?arquivo=<?=$docnomefisico;?>&id=<?=$docid?>" target="_blank"><?=$descricao?></a></td>
								<td bgcolor="<?=$cor?>" style="padding:10px;"
								<?
                               if ($tpdcod==3) {
                               	// é uma ata. Preciso verificar se é uma Ata aberta, substituída, ou fechada
                               $sql = "select aacsituacao from ata_acompanhamento where docid = $docid limit 1";
                                $situacaoata = $db->pegaum($sql);
                               	if ($situacaoata=='A')
                               	{
                               	?>	
                                title="Clique aqui para Dar/Consultar parecer" onclick="parecer_ata(<?=$docid;?>)">Clique aqui para Dar/Consultar o parecer                             
								
                               <?
                               	} 
								else if ($situacaoata=='S' or $situacaoata=='F' )
								{
								?>
                                title="Clique aqui para Consultar parecer"  onclick="parecer_ata(<?=$docid;?>)">Clique aqui para Consultar o parecer   
								<?
								}
                               	}
                               	else print '>';?>
                               </td>															
						</tr>
					<?}?>
					<?  
							$i++;
							endforeach;
						else :
					?>
							<tr bgcolor="<?=$cor?>">
								<td colspan="2" style="padding:10px;">Nenhum arquivo cadastrado</td>
							</tr>
					<?
						endif;
					?>
				<? endif; ?>
			</table>
		</form>
		<script language="JavaScript" src="../includes/wz_tooltip.js">
		</script> 
		<script>
		   function parecer_ata(cod)
		   {
		   	  	 e = "parecer_ata.php?arquivo="+cod;
                 window.open(e,"janela","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
		   }
		      
		</script>
	</body>
</html>
