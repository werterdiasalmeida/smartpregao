<?php
$p = getParam();
include_once APPRAIZ . "adm/servicos/ProntuarioServico.class.inc";
$prontuarioServico = new ProntuarioServico();
$mProntuarioImpressao = $prontuarioServico->getProntuarioImpressaoFromId($p['prontuario_id']);
$arquivo = "";
if ($mProntuarioImpressao->getArqid() != null) {
    $fileSimec = new FilesSimec();
    $arquivo = $fileSimec->obterArquivoBase64($mProntuarioImpressao->getArqid());
} else {
    $arquivo = $mProntuarioImpressao->getBase64();
}
die (
json_encode(
    utf8ArrayEncode(
        array(
            'arquivo' => $arquivo,
        )
    )
)
);


