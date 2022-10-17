<?php

ini_set("memory_limit", "1024M");

// Inclui componente de relatórios
include APPRAIZ . 'includes/classes/relatorio.class.inc';

$agrup = $mRelatorio->monta_agp($_REQUEST['agrupador']);
$col = $mRelatorio->monta_coluna($_REQUEST['coluna']);

$sql = monta_sql();
$dados = $db->carregar($sql);

$total_minuto = 0;
$total_minuto_conclusao = 0;

$colunax = (array)$_REQUEST['coluna'];


$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados);
$r->setColuna($col);
$r->setTotNivel(true);
$r->setTolizadorLinha(false);

$nomeDoArquivoXls = "relatorio_atendimento" . date("YmdHis");
echo $r->getRelatorioXls();