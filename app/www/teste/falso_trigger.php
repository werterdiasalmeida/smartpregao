<?php

header( 'Content-Type: text/plain;' );

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

include APPRAIZ . "includes/jpgraph/jpgraph.php";
include APPRAIZ . "includes/jpgraph/jpgraph_gantt.php";

include APPRAIZ . "pde/www/_constantes.php";
include APPRAIZ . "pde/www/_funcoes.php";

/*
$nome_bd     = 'simec_desenvolvimento';
$servidor_bd = 'simec-d';
$porta_bd    = '5432';
$usuario_db  = 'seguranca';
$senha_bd    = 'phpseguranca';
*/

$db = new cls_banco();

atividade_calcular_dados( 3 );

//$db->rollback();
$db->commit();
