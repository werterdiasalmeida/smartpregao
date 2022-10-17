<?

$sql = "SELECT
			les.*,
			ags.*,
			TO_CHAR(les.lesdtexecucao, 'DD/MM/yyyy HH12:MI:SS') AS lesdtexecucao
		FROM
			seguranca.logexecucaoscripts les
		JOIN
			seguranca.agendamentoscripts ags ON ags.agsid = les.agsid
		WHERE
			lesid = " . (int) $_REQUEST['lesid'];

$dados = $db->pegaLinha($sql);

// dbg($dados);

monta_titulo( $titulo_modulo, '' );
?>

<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Expires" content="-1">
    <title> </title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/jquery.1.3.2.min.js"></script>
    <script src="../includes/jquery.tabs.js"></script>
    <script src="../includes/calendario.js"></script>
    <script src="../includes/micoxAjax.js"></script>


    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <style type="text/css">
		.principal { width:645px;}
    </style>
    <style type="text/css" media="print">
		.principal {width:100%;}
		.noPrint {display:none;}
    </style>
  </head>
  <body>
  		<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
 			<tr>
                <td class="SubTituloDireita" style="width: 30%;">
                    Script execução:
                </td>
                <td >
                    <?= $dados['agsfile']; ?>
                </td>
            </tr>
 			<tr>
                <td class="SubTituloDireita" style="width: 30%;">
                    Data de execução:
                </td>
                <td >
                    <?= $dados['lesdtexecucao']; ?>
                </td>
            </tr>
  			<tr>
	            <td class="SubTituloEsquerda" style="text-align: center;" colspan="2">
	                    Log
	            </td>
	        </tr>
  			<tr>
	            <td colspan="2">
	            		<br>
	                    <?= $dados['leslog']; ?>
	                    <br>
	            </td>
	        </tr>
  			<tr>
	            <td colspan="2" style="text-align:center; background-color:#DCDCDC;">
	                    <input type="button" name=""Fechar"" value="Fechar" onclick="window.close();">
	            </td>
	        </tr>
        </table>

  </body>
</html>