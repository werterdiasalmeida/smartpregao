<?php
echo "\n" . date('d/m/Y H:i:s') . ' - ';

if(getenv("APP_ENV") == 'local')
{
    die('CRON desabilitada no ambiente local. Para habilitar altere o arquivo ' . __FILE__ );
}

function getmicrotime()
{
    list($usec, $sec) = explode(" ", microtime());
    return ((float)$usec + (float)$sec);
}

$Tinicio = getmicrotime();

/* configurações */
ini_set("memory_limit", "3000M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "../../global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

//Definindo como superuser para que o commit ocorra
$_SESSION['superuser'] = true;
if (!$_SESSION['usucpf']) {
    $_SESSION['usucpforigem'] = '03700155689';
    $_SESSION['usucpf'] = '03700155689';
}

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$tm = time();

if (!is_dir('./scripts_exec/')) {
    mkdir(APPRAIZ . 'seguranca/www/scripts_exec/', 0777);
}

if (!is_dir('./scripts_exec/scripts_logs/')) {
    mkdir(APPRAIZ . 'seguranca/www/scripts_exec/scripts_logs/', 0777);
}

$horainicio = date("d/m/Y H:i:s");

$sql = "SELECT * FROM seguranca.agendamentoscripts WHERE agsstatus='A'";
$agendamentos = $db->carregar($sql);


if ($agendamentos[0]) {
    foreach ($agendamentos as $agen) {
        switch ($agen['agsperiodicidade']) {
            case 'diario':
                $agen['agsperdetalhes'] = str_replace(";", ":00;", $agen['agsperdetalhes']) . ":00";
                $diahor = explode(";", $agen['agsperdetalhes']);

                if (in_array(date("H:i"), $diahor)) {
                    $_LISTAGEN[$agen['agsid']] = $agen;
                }
                break;
            case 'semanal':
                if ($agen['agsdataexec'] != date("Y-m-d")) {
                    $diasem = explode(";", $agen['agsperdetalhes']);
                    if (in_array(date("w"), $diasem)) {
                        $_LISTAGEN[$agen['agsid']] = $agen;
                        $sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec='" . date("Y-m-d") . "' WHERE agsid='" . $agen['agsid'] . "';";
                    }

                }
                break;
            case 'mensal':
                if ($agen['agsdataexec'] != date("Y-m-d")) {
                    $diamen = explode(";", $agen['agsperdetalhes']);
                    if (in_array(date("d"), $diamen)) {
                        $_LISTAGEN[$agen['agsid']] = $agen;
                        $sqls[] = "UPDATE seguranca.agendamentoscripts SET agsdataexec='" . date("Y-m-d") . "' WHERE agsid='" . $agen['agsid'] . "';";
                    }
                }
                break;
        }
    }
}

$out = array();
$baseUrl = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://{$_SERVER['HTTP_HOST']}";
if ($_LISTAGEN) {
    foreach ($_LISTAGEN as $agsid => $agen) {
        $file = $agen['agsfile'];

        if (is_file('./scripts_exec/' . $file)) {
            switch ($agen['agstipo']) {
                case 'http' :
                    $arquivo = "{$baseUrl}/seguranca/scripts_exec/{$file}";
                    $ch = curl_init();
                    curl_setopt($ch, CURLOPT_URL, $arquivo);
                    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
                    $logBanco = curl_exec($ch);
                    curl_close($ch);
                    break;
                case 'cli' :
                default :
                    $arquivo = realpath(APPRAIZ . "seguranca/www/scripts_exec/" . $file);
                    $logBanco = shell_exec("php {$arquivo} --executado_agendamento_simec &");
                    break;
            }

            $resexec .= $logBanco;
            $resexec .= " - ";
            $log .= "Script executado: {$arquivo} & - ";
        } else {
            $logBanco = "Não foi encontrado o arquivo '" . $file . "' - ";
            $log .= $logBanco;
        }

        //gravando o log da operação
        $sql = "INSERT INTO
						seguranca.logexecucaoscripts (agsid,leslog)
					VALUES
						('{$agsid}','" . addslashes($logBanco) . "');";

        $db->executar($sql);
        $db->commit();
    }
} else {
    $log .= "Nenhum agendamento encontrado - ";
}

if ($sqls) {
    $db->executar(implode("", $sqls));
    $db->commit();
    $log .= "Atualizações efetuadas com sucesso - ";
} else {
    $log .= "Nenhuma atualização efetuada - ";
}

// dbg($log);
// dbg($resexec);
echo ($log);
echo ($resexec);

//Limpando o superuser
$_SESSION['superuser'] = null;
$_SESSION['usucpforigem'] = null;
$_SESSION['usucpf'] = null;