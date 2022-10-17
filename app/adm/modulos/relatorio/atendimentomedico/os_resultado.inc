<?php
ini_set("memory_limit", "1024M");

$_REQUEST['titulo'] = "Relatório de Atendimento";

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
$r->setBrasao(true);
$r->setTolizadorLinha(false);
$r->setMonstrarTolizadorNivel(true);
$r->setEspandir($_REQUEST['expandir']);

?>

    <html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>

    <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">

    <? echo $r->getRelatorio(); ?>

    </body>
    </html>