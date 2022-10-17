<html>

<head>
<meta http-equiv="Content-Language" content="pt-br">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<meta name="ProgId" content="FrontPage.Editor.Document">
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>Manuais</title>
<style fprolloverstyle>A:hover {color: red; font-weight: bold}
</style>
</head>

<body bgproperties="fixed" background="../imagens/manual.jpg" bgcolor="#FFFFCC" text="#FFFFCC">
<p></p><p></p><p></p><p></p><p></p><p></p><p></p><p></p>
<form method="POST"  name="formulario">

  <input type="button" value="Apostila de Treinamento de Multiplicadores" name="B3" onclick="abre_apostila()"></p>
  <input type="button" value="Manual" name="B3" onclick="abre_manual()"></p>

</form>

</body>
<script>
function abre_apostila()
{
	window.open("../apostila_treinamento.pdf", "ApostilaTreinamento","menubar=no,toolbar=no,scrollbars=no,resizable=no,status=no,location=no,left=20,top=20,width=550,height=480");
	
	//menubar=yes,location=yes,resizable=yes,scrollbars=yes,status=yes
}
function abre_manual()
{
	window.open("../manual.pdf", "Manual","menubar=no,toolbar=no,scrollbars=no,resizable=no,status=no,location=no,left=20,top=20,width=550,height=480");
    //alert('O manual está sendo finalizado.\nDentro de alguns dias estará disponível!')
}
</script>

</html>