<?
/**
 * Script para listar os arquivos de um dado diretório
 *
 * @author Adonias Malosso <malosso@gmail.com>
 * @link http://www.php.pro.br
 */
include APPRAIZ . "includes/cabecalho.inc";

function resolveNomeDir($dir) {
	global $db;

	$sql = "SELECT sisdsc FROM sistema WHERE sisdiretorio = '$dir'";
	$ret = $db->pegaUm($sql);
}

function carregaArquivos($dir) {
	
}
?>
<br>
<?
$titulo = 'Arquivos de exportação';
monta_titulo($titulo_modulo,$titul);

?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <form name="formulario" method="post">
  <tr>
    <td align='right' class="SubTituloDireita">CPF:</td>
    <td><?=campo_texto('usucpf','','','',16,14,'###########','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Nome:</td>
    <td><?=campo_texto('usunome','','','',50,50,'','');?></td>
  </tr>
  <tr>
     <td align='right' class="SubTituloDireita">Sistema:</td>
     <td>
     <?
       $sisid = $_REQUEST['sisid'];
  		$sql="SELECT sisid as codigo, sisdsc as descricao FROM sistema order by sisdsc";
		$db->monta_combo("sisid",$sql,'S',"Selecione o Sistema",'','');
	?>
    </td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Operação:</td>
    <td><select name="audtipo" class="CampoEstilo">
								<option value="">Todas</option>
								<option value="I">Inclusão</option>
								<option value="U">Alteração</option>
								<option value="D">Exclusão</option>
								<option value="X">Erros</option>
		</select></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Tabela:</td>
    <td><?=campo_texto('audtabela','','','',30,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Expressão na Query:</td>
    <td><?=campo_texto('audsql','','','',30,50,'','');?></td>
  </tr>
  <tr>
    <td align='right' class="SubTituloDireita">Data:</td>
    <td><?=campo_data('auddata_ini', '','S','','N');?> até <?=campo_data('auddata_fim', '','S','','N');?></td>
  </tr>
  <tr bgcolor="#C0C0C0">
    <td ></td>
    <td><input type='button' class="botao" name='consultar' value='Consultar' onclick="Envia();"</td>
  </tr>
</form>
</table>
<?



if ($wh){
$sql = "select a.usucpf as CPF, u.usunome as NOME, a.audtipo as operacao, a.audtabela as tabela, '<text>'||a.audsql||'</text>' as query, m.mnucod||' - '||m.mnutransacao as transacao, to_char(a.auddata,'DD/MM/YYYY HH24:MI:SS') as data from auditoria a inner join usuario u on a.usucpf=u.usucpf inner join menu m on a.mnuid=m.mnuid ".$wh." order by auddata desc";




$db->monta_lista($sql,$cabecalho,50,20,'','','');
}
?>
<script language="JavaScript">
function Envia()
{
document.formulario.submit();
}
</script>