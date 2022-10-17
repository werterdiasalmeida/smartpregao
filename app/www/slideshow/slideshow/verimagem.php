<?php
include_once "config.inc";
include_once APPRAIZ . "includes/VerImagem.php";
include_once APPRAIZ . 'includes/VerImagemException.php';
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$newwidth = (int)$_REQUEST['newwidth'];
$newheight = (int)$_REQUEST['newheight'];
$arqid = $_REQUEST['arqid'];

try {
    $verImagem = new VerImagem($arqid);

    if ($newwidth || $newheight) {
        $verImagem->setAltura($newheight);
        $verImagem->setLargura($newwidth);
        $verImagem->redimenciona();
    }

    $verImagem->redimencionarOuSelecionar();
    $verImagem->exibir();

    unset($verImagem);
} catch (VerImagemException $e) {
    if ($e->getCode() == VerImagem::ERRO_FORMATO_NAO_SUPORTADO) {
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($arqid);
    }
}