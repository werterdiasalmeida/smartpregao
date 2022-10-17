<?php

/*
 * O contador de tempo online na tela deve ser atualizado toda vez que o
 * usuário carregar uma tela do sistema. Ele é utilizado pelo "estou vivo"
 * de acordo com a constante MAXONLINETIME, definido no config.inc.
 */
$_SESSION["evHoraUltimoAcesso"] = time();

?>