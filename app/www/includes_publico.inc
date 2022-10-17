<?php
// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();