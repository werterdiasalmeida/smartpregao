<?php

// carrega as fun��es gerais
$pathConfig = "../../global/config.inc";
include_once(realpath($pathConfig) ? $pathConfig : "config.inc");
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "seguranca/www/_funcoes.php";
include_once APPRAIZ . "seguranca/www/_enviarEmailConfirmacaoAtendimento.php";

function __autoload($class_name)
{
    $arCaminho = array(
        APPRAIZ . "adm/classes/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/financeiro/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/mensageria/",
        APPRAIZ . "includes/classes/controller/",
        APPRAIZ . "includes/classes/",
        APPRAIZ . "includes/classes/exception/",
        APPRAIZ . "includes/classes/view/",
        APPRAIZ . "includes/classes/html/",
    );

    foreach ($arCaminho as $caminho) {
        $arquivo = $caminho . $class_name . '.class.inc';
        if (file_exists($arquivo)) {
            require_once($arquivo);
            break;
        }
    }
}

// CPF do administrador de sistemas
if (!$_SESSION['usucpf']) {
    $_SESSION['superuser'] = true;
}

// abre conex�o com o servidor de banco de dados
$db = new cls_banco();

$mEstabelecimentoProfissionalAgenda = new EstabelecimentoProfissionalAgenda();
$mNotificacaoEmailPaciente = new NotificacaoAtendimento();
$arrListPacientes = $mEstabelecimentoProfissionalAgenda->getAtendimentosAgendadosComInicioNoProximoDia();
$totalDePacientes = sizeof($arrListPacientes);

$html = "Enviando emails de confirma��o de atendimento.<br>";
foreach ($arrListPacientes as $numItem => $paciente) {
    $numOrdem = $numItem + 1;
    $html .= "Enviando email {$numOrdem} / {$totalDePacientes}</br>";
    $mNotificacaoEmailPaciente->enviarEmailParaConfirmacaoDeAtendimento($paciente['agenda_id']);
}
$html .= "Fim da Execu��o.";

die($html);
