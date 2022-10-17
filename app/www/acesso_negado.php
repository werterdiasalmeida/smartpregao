<?
//Habilitar quando estiver em Produção
?>
<html>
	<head>
		<title>Acesso Negado</title>
	</head>
	<body>
		<script type="text/javascript">
		   alert ( 'Acesso Negado!\nVocê não tem permissão para acessar esta tela.' );
		   if (navigator.appName.indexOf('Netscape') == 0)
		   		history.go(-1);
		   else
		   		history.go(-2);
		</script>
	</body>
</html>
