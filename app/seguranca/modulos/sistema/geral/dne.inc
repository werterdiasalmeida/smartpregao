<?php
//
// $Id
//


// http://10.220.5.106/dne/dev/server/service.php?WSDL

try{
    $ws  = new SoapClient('http://dne.mec.gov.br/server/service.php?WSDL');
    $res = $ws->getDNE(null, 0, 0, 0, str_replace(array('.', '-'), '', $_REQUEST['endcep']));

    $xml = simplexml_load_string(utf8_encode(html_entity_decode($res)));

    echo "var DNE = new Array();";

    foreach ($xml as $node => $value) {
        echo "DNE['" , $node , "'] = '" , utf8_decode(addslashes($value)) , "';";
    }
} catch (Exception $e){
    echo ($e->getMessage());
}



//print_r($res);





die();

//return XmlLoader::getXmlByString( $strResult );


//getEnderecoPeloCEP





