<?
//Recupera tudo que foi passado por REQUEST e instancia as vari�veis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;
//Montando a cl�usula WHERE
if ($usucpf) $wh .= " and usu.usucpf like '%".$usucpf."%' ";
if ($usunome) $wh .= " and usu.usunome like '%".str_to_upper($usunome)."%' ";
if ($audtipo) $wh .= " and aud.audtipo = '".$audtipo."' ";
if ($audtabela) $wh .= " and aud.audtabela like '%".$audtabela."%' ";
if ($audsql) $wh .= " and aud.audsql like '%".$audsql."%' ";
if ($auddata_ini and $auddata_fim) $wh .=  " and aud.auddata<='".formata_data_sql($auddata_fim)." 23:59:59' and aud.auddata>='".formata_data_sql($auddata_ini)." 00:00:00' ";
if ($wh) $wh = " where ".substr($wh,4,strlen($wh)); else $wh = "";

include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$titul = 'Auditoria Interna';
monta_titulo($titulo_modulo,$titul);
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<form name="formulario" method="post">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  
  <tr>
    <td align='right' class="SubTituloDireita">CPF:</td>
    <td><?=campo_texto('usucpf','','','',16,14,'###########','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Nome:</td>
    <td><?=campo_texto('usunome','','','',50,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Opera��o:</td>
    <td><select name="audtipo" class="CampoEstilo">
								<option value="">Todas</option>
								<option value="I">Inclus�o</option>
								<option value="U">Altera��o</option>
								<option value="D">Exclus�o</option>
								<option value="X">Erros</option>
		</select></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Tabela:</td>
    <td><?=campo_texto('audtabela','','','',30,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Express�o na Query:</td>
    <td><?=campo_texto('audsql','','','',30,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Data:</td>
    <td><?=campo_data('auddata_ini', '','S','','N');?> at� <?=campo_data('auddata_fim', '','S','','N');?></td>
  </tr>
  <tr bgcolor="#C0C0C0">
    <td ></td>
    <td><input type='button' class="botao" name='consultar' value='Consultar' onclick="Envia();"></td>
  </tr>
</table>  
</form>


<?
   $cabecalho = array("CPF", "NOME", "E.MAIL", "UF DE LOTA��O", "DDD", "TELEFONE", "FUN��O", "TIPO DE EVENTO", "DATA", "MENU UTILIZADO");
   
	if ($wh){
	//$sql = "select a.usucpf as CPF, u.usunome as NOME, a.audtipo as operacao, a.audtabela as tabela, '<text>'||a.audsql||'</text>' as query, m.mnucod||' - '||m.mnutransacao as transacao, to_char(a.auddata,'DD/MM/YYYY HH24:MI:SS') as data from auditoria a inner join usuario u on a.usucpf=u.usucpf inner join menu m on a.mnuid=m.mnuid " . $wh . " and m.sisid = " . $_SESSION['sisid'] . " order by m.sisid, auddata desc";
	//$sql = "select ss.sisabrev, a.usucpf as CPF, u.usunome as NOME, a.audtipo as operacao, a.audtabela as tabela, '<text>'||a.audsql||'</text>' as query, m.mnucod||' - '||m.mnutransacao as transacao, to_char(a.auddata,'DD/MM/YYYY HH24:MI:SS') as data from auditoria a inner join usuario u on a.usucpf=u.usucpf inner join menu m on a.mnuid=m.mnuid inner join seguranca.sistema ss on ss.sisid=m.sisid " . $wh . "  order by m.sisid, auddata desc";
	
	$sql = "select usu.usucpf as CPF, usu.usunome as Nome, usu.usuemail as Email,
					usu.regcod as UF, usu.usufoneddd as DDD, usu.usufonenum as Telefone,
					usu.usufuncao as Fun��o,

					case  aud.audtipo        when 'I' then 'Inclu��o' 
                	when 'U' then 'Altera��o' 
                	when 'D' then 'Exclus�o' 
                	when 'X' then 'Erro do Sistema' end as \"Tipo da A��o\",                	
					to_char(aud.auddata ,'dd/MM/yyyy HH:MM:SS') as \"Data do Evento\",					
					aba.abadsc as MenuUtilizado	

			from seguranca.usuario usu									

			inner join seguranca.auditoria aud on usu.usucpf = aud.usucpf

			inner join seguranca.aba aba on aud.sisid = aba.sisid
			
			" . $wh . "
			
						
	";		
//	dbg($sql);
	$db->monta_lista($sql,$cabecalho,50,20,'','','');
	}
	?>

<script language="JavaScript">
function Envia()
{
document.formulario.submit();
}
</script>
