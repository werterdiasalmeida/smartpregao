<?php

include_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

$db = new cls_banco();
$_SESSION['superuser'] = true;

function __autoload($class_name) {
    $arCaminho = array(
        APPRAIZ . "adm/classes/",
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/mensageria/",
        APPRAIZ . "includes/classes/",
    );

    foreach($arCaminho as $caminho){
        $arquivo = $caminho . $class_name . '.class.inc';
        if ( file_exists( $arquivo ) ){
            require_once( $arquivo );
            break;
        }
    }
}

//$texto = json_encode(utf8ArrayEncode($_REQUEST));
//$sql = "INSERT INTO seguranca.logexecucaoscripts(leslog) VALUES('{$texto}')";
//$db->pegaUm($sql);
//$db->commit();
//die();


/*
 * data_callback = Conteúdo das mensagens via método POST
 * Parâmetros contidos no em data_callback
 * codigo_campanha;celular;resposta;data_resposta;status;interno_key
 *
 * [0] codigo_campanha  = Código da campanha retornado pela função sendsms (Tipo de dados Inteiro)
 * [1] celular          = Número do celular de retonro (Tipo de dados String)
 * [2] resposta         = Mensagem SMS retornada (Tipo de Dados String)
 * [3] data_resposta    = Data de recebimento da mensagem (Tipo de Dados String - Formato: yyyy-MM-dd HH:mm:ss)
 * [4] status           = Status da entrega (Tipo de Dados String)
 */
try{
    if (isset($_REQUEST['data_callback']))
    {
        $linhas =  preg_split( '/\r\n|\r|\n/', $_REQUEST['data_callback']);

        $mMensagemResposta = new MensagemResposta();
        $mEstabelecimentoMensagem = new EstabelecimentoMensagem();

        foreach ($linhas as $linha)
        {
            if ($linha != '')
            {
                $conteudo = explode(';', $linha);

                if (!empty($conteudo[0]) && !empty($conteudo[2]) && $conteudo[4] == 'RECEBIDO SUCESSO')
                {
                    $codigo_campanha = $conteudo[0];
                    $resposta        = $conteudo[2];

                    $mMensagem = new Mensagem();
                    $mensagemOriginal = $mMensagem->recuperarUm('id',["codigo = '{$codigo_campanha}'", 'excluido IS FALSE']);

                    if($mensagemOriginal){
                        $arDados = [
                            'mensagem_id'   => $mensagemOriginal,
                            'resposta'      => $resposta,
                            'excluido'      => 'false'
                        ];

                        $mMensagemResposta->setDadosNull();
                        $idResposta = $mMensagemResposta->manter($arDados);
                        $mMensagemResposta->commit();


                        $mEstabelecimentoMensagem->processaResposta($mensagemOriginal,$idResposta);
                    }
                }
            }
        }
    }
} catch (Exception $ex) {
    echo $ex->getMessage();
}






