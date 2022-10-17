<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/TarefaProject.php";
$db = new cls_banco();

//$caminho = 'tarefas_project.csv';
//$arquivo = fopen( $caminho, 'r' );


//header( 'Content-Type: text/plain;' );

echo TarefaProject::exportarDoPDE();


//$db->commit();
















