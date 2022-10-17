<?php header("Content-Type: text/html; charset=ISO-8859-1"); ?>
<?php
//
// $Id
//


if (!defined('APPRAIZ')) {
    define('APPRAIZ', realpath('../../') . "/");
}

require_once APPRAIZ . 'global/config.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'seguranca/modulos/sistema/geral/endereco.inc';

global $db;
$db = new cls_banco();

$_REQUEST = array_map(function ($req) use ($db) {
    return pg_escape_string($db->link, htmlentities($req));
}, $_REQUEST);

switch ($_REQUEST['opt']) {
    case 'dne':
        dne();
        break;

    case 'cepUF':
    	cepUF();
    	break;
        
    case 'municipio':
        municipio();
        break;

    case 'endereco':
        salvarEndereco();
        break;

    case 'excluirEndereco':
        excluirEntidadeEndereco($_REQUEST['endid']);
        break;
}


function cepUF()
{
    global $db;

    try{
        $cp = str_replace(array('.', '-'), '', $_REQUEST['endcep']);
        $rs = $db->carregar('SELECT ufesg as estado FROM cep.logfaixauf WHERE \'' . $cp . '\' BETWEEN ufecepini and ufecepfim');

        //$ws  = new SoapClient('http://dne.mec.gov.br/server/service.php?WSDL');
        //$res = $ws->getDNE(null, 0, 0, 0, str_replace(array('.', '-'), '', $_REQUEST['endcep']));
        //$xml = simplexml_load_string(utf8_encode(html_entity_decode($res))); // returns UTF-8

      //  echo "var DNE = new Array();" , "\n";

        if (!is_array($rs)) {
            echo 'DNE.push({ \'cep\'           : \'' , $cp , '\',
                             \'cidade\'        : \'\',
                             \'estado\'        : \'\',
                             \'latitude\'      : \'\',
                             \'hemisferio\'    : \'\',
                             \'longitude\'     : \'\',
                             \'meridiano\'     : \'\',
                             \'altitude\'      : \'\',
                             \'medidaarea\'    : \'\',
                             \'medidaraio\'    : \'\',
                             \'muncod\'        : \'\',
                             \'muncodcompleto\': \'\'
        					});';
        } else {
            foreach ($rs as $cepUF) {
                echo 'DNE.push({';
                foreach ($cepUF as $node => $value) {
                    echo "'" , $node , "': '" , addslashes(trim($value)) , "',";
                }
                echo "'time':'" , time() , "'});\n";
            }
        }
    } catch (Exception $e) {
        echo ($e->getMessage());
    }
}




function dne()
{
    global $db;

    try{
        $cp = str_replace(array('.', '-'), '', $_REQUEST['endcep']);
        $rs = $db->carregar('select * from cep.v_endereco where cep = \'' . $cp . '\' order by cidade asc');

        //$ws  = new SoapClient('http://dne.mec.gov.br/server/service.php?WSDL');
        //$res = $ws->getDNE(null, 0, 0, 0, str_replace(array('.', '-'), '', $_REQUEST['endcep']));
        //$xml = simplexml_load_string(utf8_encode(html_entity_decode($res))); // returns UTF-8

        echo "var DNE = new Array();" , "\n";

        if (!is_array($rs)) {
            echo 'DNE.push({\'cep\'           : \'' , $cp , '\',
                            \'logradouro\'    : \'\',
                            \'bairro\'        : \'\',
                            \'cidade\'        : \'\',
                            \'estado\'        : \'\',
                            \'latitude\'      : \'\',
                            \'hemisferio\'    : \'\',
                            \'longitude\'     : \'\',
                            \'meridiano\'     : \'\',
                            \'altitude\'      : \'\',
                            \'medidaarea\'    : \'\',
                            \'medidaraio\'    : \'\',
                            \'muncod\'        : \'\',
                            \'muncodcompleto\': \'\'});';
        } else {
            foreach ($rs as $dne) {
                echo 'DNE.push({';
                foreach ($dne as $node => $value) {
                    echo "'" , $node , "': '" , addslashes(trim($value)) , "',";
                }
                echo "'time':'" , time() , "'});\n";
            }
        }
    } catch (Exception $e) {
        echo ($e->getMessage());
    }
}


function municipio()
{
    global $db;

    if (array_key_exists('complete', $_REQUEST))
        die();

    if (trim($_REQUEST['regcod'] == ''))
        die();

    $res       = $db->carregar("SELECT estuf, muncod, mundescricao as mundsc FROM territorios.municipio WHERE estuf = '" . $_REQUEST['regcod'] . "' ORDER BY mundescricao");
    $ultimoCod = null;

    echo "var listaMunicipios = new Array();\n";

    foreach ($res as $unidade) {
        if ($ultimoCod != $unidade['estuf']) {
            echo "listaMunicipios['" , $unidade['estuf'] , "'] = new Array();";
            $ultimoCod = $unidade['estuf'];
        }

        echo "listaMunicipios['" , $unidade['estuf'] , "'].push(new Array('" , $unidade['muncod'] , "', '" , addslashes(trim($unidade['mundsc'])) , "'));";
    }
}





