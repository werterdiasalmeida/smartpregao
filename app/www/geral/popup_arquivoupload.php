<?php

	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	
	
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

	
		

				
		
		if( move_uploaded_file( $_FILES[ "arquivo" ]["tmp_name"], $dir ) )
		{
			$db->commit();
			$msg = 'Arquivo inserido com sucesso';
			print "alert ('$msg');location.href='../geral/popup_aqruivoupload.php';";
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
	</body>
</html>
