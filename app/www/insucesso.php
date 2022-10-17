<html>
<head>
<script language="Javascript">
	alert( '<?= $_REQUEST['mensagem'] ?>' );
	window.location.href="<?=urldecode($_REQUEST['saida'])?>";
</script>
<title>Operação não pode ser realizada</title>
</head>
<body>
</body>
</html>


