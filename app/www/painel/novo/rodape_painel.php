</td></tr></table>
</div>
<?
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral
   Módulo:rodape.inc
   Finalidade: permitir o fechamento da conexão
*/

//monta link manual
$linkmanual = montaLinkManual( $_SESSION['sisid'] );

//Loga estatistica
$sql = "Insert into seguranca.estatistica (mnuid,usucpf,esttempoexec,estsession,sisid) VALUES(" . $_SESSION['mnuid'] . ",'" . $_SESSION['usucpforigem'] . "'," . ( getmicrotime() - $Tinicio ) . ",'" . session_id() . "'," . $_SESSION['sisid'] . ")";
$db->executar($sql,false);
$db->commit();


//Fecha conexões DB
$db->close();
?>

<!-- cantos arredondados - base-->
<div id="geral_base_esq">
	<div id="geral_base_dir"></div>
</div>

<!-- fim DIV GERAL-->



<div id="rodape"><font color="#ffffff">
									&nbsp;&nbsp;Data: <?= date("d/m/Y - H:i:s") ?>&nbsp;/
									Último acesso (<?= formata_data( $_SESSION['usuacesso'] ); ?>) -
									<a href="javascript:abrirUsuariosOnline();" style="color: #ffffff">
										<span id="rdpUsuariosOnLine"><?= $_SESSION['qtdusuariosonline'][$_SESSION['sisid']];?></span>
										Usuários On-Line
									</a>
								</font></div>


</body>
</html>
<?php include_once APPRAIZ . "includes/estouvivo.php"; ?>

					<script type="text/javascript" language="javascript">
						function abrirUsuariosOnline()
						{
							window.open(
								'../geral/usuarios_online.php',
								'usuariosonline',
								'height=500,width=600,scrollbars=yes,top=50,left=200'
							);
						}
						</script>
<div id="avisochat">
			<div style="padding: 0px; width: 110px; padding: 0; margin: 0; height: 100px; overflow: none; position-y: absolute;display:none;" id="avisochat_lista">
				<table border="0" cellpadding="2" cellspacing="2">
				<tr>
					<td  align="left" width="110" bgcolor="#FFF7AF" style="border-top: 1px solid #808080; border-right: 1px solid #808080;border-bottom: 1px solid #808080;">
						<span onclick="avisoChatMostrarEsconder();"><img src="/imagens/balaochat.gif" border="0" align="absmiddle"><font color="#000000"><strong>&nbsp;Mensagens</strong></font><br></span>
						<div style="overflow: auto; position-y: absolute;border-top: 1px solid #808080; height: 100px;">
							<table id="avisochat_tabela" width="100%" cellpadding="0" cellspacing="0"></table>
						</div>
					</td>
				</tr>
				</table>
			</div>
		</div>
