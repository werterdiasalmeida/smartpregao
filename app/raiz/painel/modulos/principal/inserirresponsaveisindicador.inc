<?

require_once APPRAIZ . "includes/classes/entidades.class.inc";

// inserindo entidade e inserindo na tabela da página pai (via javascript)
if ($_REQUEST['opt'] == 'salvarRegistro') {
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	// inserindo no formulario pai (opener)
    $botoes = "<input type=\"hidden\" name=\"entid[]\" value=\"".$entidade->getEntid()."\"><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" border=\"0\" title=\"Editar\" onclick=editarresponsavel(\'".$entidade->getEntid()."\');>&nbsp<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\"  border=\"0\" title=\"Excluir\" onclick=removerresponsavel(this);>"; 
    if(!$_REQUEST['tr'] || $_REQUEST['tr'] == ''){
    	echo '<script type="text/javascript">
				var tabela = window.opener.document.getElementById("listaresponsavel");
				if (window.opener.document.getElementById("linha_'.$entidade->getEntid().'")){
    				tabela.deleteRow(window.opener.document.getElementById("linha_'.$entidade->getEntid().'").rowIndex);
    			}
				var tamanho = tabela.rows.length;
				
				if(window.opener.document.getElementById("linha_'.$entidade->getEntid().'") == null){
					var tr = tabela.insertRow(tamanho-1);	
					tr.id = "linha_'.$entidade->getEntid().'";
				
					var colAcao = tr.insertCell(0);
					var colNome = tr.insertCell(1);
					var colResp = tr.insertCell(2);
					
					colAcao.style.textAlign = "center";
					colResp.style.textAlign = "center";
					
					colAcao.innerHTML = \''.$botoes.'\';
					colNome.innerHTML = \'' . $_REQUEST['entnome'] . '\';
					colResp.innerHTML = \'<select name="entidcargo['.$entidade->getEntid().']"><option value="C" '.(($resp['restipo']=='C')?"selected":"").'>Coordenador</option><option value="D" '.(($resp['restipo']=='D')?"selected":"").'>Diretor</option><option value="E" '.(($resp['restipo']=='E')?"selected":"").'>Equipe de Apoio</option></select>\'; 
				}
				window.close();

			</script>';
    	
	} else {
	
    	echo '<script type="text/javascript">
				window.close();
			  </script>';
	}
	exit;

}

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
<?


$entidade = new Entidades();

if($_REQUEST['entid']) {
	$entidade->carregarPorEntid($_REQUEST['entid']);
}
echo $entidade->formEntidade("painel.php?modulo=principal/inserirresponsaveisindicador&acao=A&opt=salvarRegistro",
							 array("funid" => FUN_RESPONSAVELINDICADOR, "entidassociado" => null),
							 array("enderecos"=>array(1))
							 );

?>
    </div>
</body>
</html>