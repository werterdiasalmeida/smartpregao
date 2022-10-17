<?php

/**
 * Classe responsável por guardar os métodos que serão acessados pelo webservice
 */
class SegurancaWs extends AbstractWs
{
    const SISID_APP_CLIENTE = 72;
    const PFLCOD_APP_CLIENTE = 437;
    const PFLCOD_OPERADOR = 438;

    public function __construct()
    {
        $_SESSION['sisid'] = 4;
    }

    public static function getFuncoesPermitidasSemLogar(){
        return array(
            'cadastrarUsuario',
            'recuperarSenha'
        );
    }

    public function cadastrarUsuario($dadosUsuario){
        $mUsuario = new Usuario();
        $dadosUsuario['cpf'] = str_replace(array('.', '-'), '', $dadosUsuario['cpf']);

        // Verifica se o cpf está cadastrado
        if($mUsuario->recuperarUm('count(*)', array("usucpf = '{$dadosUsuario['cpf']}'")) > 0){
            return array(
                "codigo" => 1,
                "mensagem" => "O CPF informado já está cadastrado",
                "dados" => array()
            );
        }

        // Verifica se o cpf está cadastrado
        if($mUsuario->recuperarUm('count(*)', array("usuemail = '{$dadosUsuario['email']}'")) > 0){
            return array(
                "codigo" => 1,
                "mensagem" => "O Email informado já está cadastrado",
                "dados" => array()
            );
        }

        // Coloca os dados do usuário na sessão
        $_SESSION['usucpf'] = $_SESSION['usucpforigem'] = $dadosUsuario['cpf'];

        // Ajusta o telefone
        switch(strlen($dadosUsuario['telefone'])){
            case 10 :
                $telefone = vsprintf('(%s%s) %s%s%s%s-%s%s%s%s', str_split($dadosUsuario['telefone']));
                break;
            case 11 :
                $telefone = vsprintf('(%s%s) %s%s%s%s%s-%s%s%s%s', str_split($dadosUsuario['telefone']));
                break;
            default :
                $telefone = $dadosUsuario['telefone'];
                break;
        }

        $sql = sprintf(
            "INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, usuchaveativacao, regcod,
					ususexo, ususenha, suscod, orgao,
					muncod, usudatanascimento, usustatus
				) values (
					'%s', '%s', '%s', '%s',
					'%s', '%s', '%s',
					'%s', '%s', '%s', '%s',
					'%s', '%s', '%s', '%s'
				)",
            $dadosUsuario['cpf'],
            utf8_decode($dadosUsuario['nome']),
            strtolower( $dadosUsuario['email'] ),
            '',
            $telefone,
            '',
            'f',
            '',
            $dadosUsuario['sexo'],
            md5_encrypt_senha( $dadosUsuario['senha'], '' ),
            'A',
            '',
            '',
            $dadosUsuario['nascimento'],
            'A'
        );

        $mUsuario->executar( $sql );

        $sql = "INSERT INTO seguranca.usuario_sistema(
                        usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
                VALUES ('{$dadosUsuario['cpf']}', ".self::SISID_APP_CLIENTE.", 'A', ".self::PFLCOD_APP_CLIENTE.", now(), 'A');";
        $mUsuario->executar( $sql );

        $sql = "INSERT INTO seguranca.perfilusuario(
                        usucpf, pflcod)
                VALUES ('{$dadosUsuario['cpf']}', ".self::PFLCOD_APP_CLIENTE.");";
        $mUsuario->executar( $sql );

        $mUsuario->commit();
        return $this->gerarRespostaSucesso(array());
    }

    public function recuperarSenha($dadosUsuario){
        $usucpf = $dadosUsuario['usucpf'];
        $mensagem = "Enviamos uma nova senha para seu email.";
        $retorno = true;

        if(empty($usucpf)){
            $mensagem = "CPF ou Email inválido";
            $retorno  = false;
        }

        $mUsuario = new Usuario();
        $arDadosUsuario = $mUsuario->recuperarLinha('*', array("(usucpf = '{$usucpf}' OR usuemail = '{$usucpf}')"), false);

        if(is_array($arDadosUsuario) && count($arDadosUsuario) > 0 && ($arDadosUsuario['usucpf'] == $usucpf || $arDadosUsuario['usuemail'] == $usucpf)){
            $senhaProvisoriaPlain = gerarSenhaProvisoriaApp();
            $senhaProvisoria = md5_encrypt_senha( $senhaProvisoriaPlain, '' );
            $arDadosUsuarioSenha = array(
                'usucpf' => $arDadosUsuario['usucpf'],
                'ususenha' => $senhaProvisoria,
                'usuchaveativacao' => 'f'
            );
            $mUsuario->popularDadosObjeto($arDadosUsuarioSenha)->alterar();

            // envia email de confirmação
            $destinatario = $arDadosUsuario['usuemail'];
            $assunto = "Recuperação de Senha";
            $conteudo = "Olá {$arDadosUsuario['usunome']}!
                        <br/>Sua senha foi alterada para <strong>{$senhaProvisoriaPlain}</strong>.
                        <br />Ao se conectar, altere esta senha para uma de sua preferência.";

            global $email_from;
            $remetente = array();
            $remetente['email'] = $email_from;
            $remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];
            enviar_email($remetente, $destinatario, $assunto, $conteudo);

            $mUsuario->commit();
        }else{
            $mensagem = "CPF ou Email inválido";
            $retorno = false;
        }

        $arRetorno = array("codigo" => ($retorno ? '0' : '1'), "mensagem" => $mensagem);
        return $arRetorno;
    }

    public function alterarSenha($dadosNovaSenha){
        $usucpf = $_SESSION['usucpf'];

        $mUsuario = new Usuario();
        $arDadosUsuario = $mUsuario->recuperarLinha('*', array("usucpf = '{$usucpf}'"), false);

        if (md5(md5_decrypt_senha( $arDadosUsuario['ususenha'], '' )) != $dadosNovaSenha['atual'] ) {
            $mensagem = "A senha atual não confere.";
            $retorno = false;
        }else if(strlen($dadosNovaSenha['nova']) < 8){
            $mensagem = "A nova senha deve conter pelo menos 8 caracteres.";
            $retorno = false;
        }else if($dadosNovaSenha['nova'] != $dadosNovaSenha['confirmacao']){
            $mensagem = "Sua nova senha e a confirmação são diferentes.";
            $retorno = false;
        }else{
            $novaSenha = md5_encrypt_senha($dadosNovaSenha['nova'], '');
            $arDadosUsuarioSenha = array(
                'usucpf' => "'{$usucpf}'",
                'ususenha' => $novaSenha,
                'usuchaveativacao' => 't'
            );
            $mUsuario->popularDadosObjeto($arDadosUsuarioSenha)->salvar();
            $mUsuario->commit();

            $arRetorno = $mUsuario->loginWebService($usucpf, md5($dadosNovaSenha['nova']));
            $arRetorno['mensagem'] = "Senha alterada com sucesso.";
            return $arRetorno;
        }

        $arRetorno = array("codigo" => ($retorno ? '0' : '1'), "mensagem" => $mensagem);
        return $arRetorno;
    }
}