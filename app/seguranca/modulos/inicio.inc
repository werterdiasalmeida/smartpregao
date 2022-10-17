<?
 /*
   Sistema Simec
   Setor responsvel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial so simec
    */
//recupera todas as variaveis que veio pelo post ou get
foreach($_REQUEST as $k=>$v) ${$k}=$v;

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>
<br>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<script language="JavaScript" src="../includes/funcoes.js"></script>
<?
$db->cria_aba($abacod_tela,$url, '');
//monta_titulo($titulo_modulo, '');
?>
<?php
include("sistema/userOnline.inc"); 
?>