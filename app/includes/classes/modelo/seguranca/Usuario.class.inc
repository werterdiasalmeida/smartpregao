<?php

class Usuario extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.usuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "usucpf" );

    protected $tabelaAssociativa = true;

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
        'usucpf' => null,
        'regcod' => null,
        'usunome' => null,
        'usuemail' => null,
        'usustatus' => null,
        'usufoneddd' => null,
        'usufonenum' => null,
        'ususenha' => null,
        'usudataultacesso' => null,
        'usunivel' => null,
        'ususexo' => null,
        'orgcod' => null,
        'unicod' => null,
        'usuchaveativacao' => null,
        'usutentativas' => null,
        'usuprgproposto' => null,
        'usuacaproposto' => null,
        'usuobs' => null,
        'ungcod' => null,
        'usudatainc' => null,
        'usuconectado' => null,
        'pflcod' => null,
        'suscod' => null,
        'usunomeguerra' => null,
        'orgao' => null,
        'muncod' => null,
        'usudatanascimento' => null,
        'usudataatualizacao' => null,
        'entid' => null,
        'tpocod' => null,
        'carid' => null,
        'usumigrado' => null,
        'usudtsenhaprov' => null,
        'usucodigoindicacao' => null,
        'arqid' => null,
        'usuinst' => null,
        'usuface' => null,
        'token_api' => null,
        'arqid_cd' => null
    );

    const SENHA_PADRAO = 'vida12345';

    const PASSPHRASE = '@meso@tec@';

    public function loginWebService($usuario, $senha){
        // $usuario          = corrige_cpf( $usuario );
        $senha            = trim( $senha );
        $limiteTentativas = 4;
        $mensagem         = "Usuário autenticado com sucesso";
        $retorno          = true;
        $objUsuario       = new stdClass();

        if(empty($usuario) || empty($senha)){
            $mensagem = "Usuário ou senha inválidos";
            $retorno  = false;
        }


        if($retorno){
            $usuarioCpf = str_replace(array('.', '-'), '', $usuario);

            $sql = sprintf("SELECT
                                u.id,
                    			u.usucpf,
                    			u.ususenha,
                    			u.suscod,
                    			u.usutentativas,
                    			u.usunome,
                    			u.usuemail,
                    			u.ususexo,
                    			u.usumigrado,
                    			u.arqid,
                    			(CURRENT_DATE - u.usudtsenhaprov) AS qtd_dias_senha_provisoria
    		               FROM
    			                seguranca.usuario u
                		   WHERE
                			    u.usucpf = '%s' or usuemail = '%s'", $usuarioCpf, $usuario);
            $objUsuario = (object) $this->recuperar( $sql );

            if ( !$objUsuario->usucpf ) {
                $retorno  = false;
                $mensagem = "Usuário ou senha inválidos";
            }else{
                // verifica o status do usuário
                switch ( $objUsuario->suscod ) {
                    case 'A': // Ativo
                        break;
                    case 'P': // Pendente
                        $retorno  = false;
                        $mensagem = "Usuário pendente";
                        break;
                    case 'B': // Bloqueado
                    default:
                        $retorno  = false;
                        $mensagem = "Usuário bloqueado";
                }
            }
        }

        // Removida a verificação de módulo para o APP
        /*
        if($retorno){
            // verifica permissão de acesso aos módulos
            $sql = sprintf(
                "SELECT
            s.sisid, s.sisdiretorio, s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel AS usunivel, us.susdataultacesso
            FROM seguranca.sistema s
            INNER JOIN seguranca.usuario_sistema us USING ( sisid )
            INNER JOIN seguranca.usuario u USING ( usucpf )
            INNER JOIN seguranca.perfilusuario pu USING ( usucpf )
            INNER JOIN seguranca.perfil p ON pu.pflcod = p.pflcod AND p.sisid = s.sisid
            WHERE
            us.suscod = 'A' AND
            u.usucpf = '%s' AND
            u.suscod = 'A' AND
            p.pflstatus = 'A'
            GROUP BY s.sisid, s.sisdiretorio,  s.sisarquivo, s.sisdsc, s.sisurl, s.sisabrev, s.sisexercicio, s.paginainicial, p.pflnivel, us.susdataultacesso
            ORDER BY us.susdataultacesso DESC
            LIMIT 1",
                $objUsuario->usucpf
            );

            $sistema = (object) $this->pegaLinha( $sql );
            if ( !$sistema->sisid ) {
                $retorno  = false;
                $mensagem = "Você não possui acesso ao módulo solicitado.";
            }
        }
        */

        if($retorno){
            if($objUsuario->usumigrado == 't'
                && (is_null($objUsuario->qtd_dias_senha_provisoria)
                    || md5(md5_decrypt_senha( $objUsuario->ususenha, '' )) != $senha )){
                $senhaProvisoriaPlain = gerarSenhaProvisoriaApp();
                $senhaProvisoria = md5_encrypt_senha( $senhaProvisoriaPlain, '' );
                $sqlUpdate = "UPDATE seguranca.usuario SET ususenha = '{$senhaProvisoria}', usudtsenhaprov = now() WHERE usucpf = '{$objUsuario->usucpf}'";
                $this->executar($sqlUpdate);

                // envia email de confirmação
                $destinatario = $objUsuario->usuemail;
                $assunto = "Sua nova senha";
                $conteudo = "Olá {$objUsuario->usunome}!
                             <br />Sua nova senha é <strong>{$senhaProvisoriaPlain}</strong>.
                             <br />Por favor, ao se conectar altere esta senha para uma de sua preferência.";

                global $email_from;
                $remetente = array();
                $remetente['email'] = $email_from;
                $remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];
                enviar_email($remetente, $destinatario, $assunto, $conteudo);

                $retorno = false;
                $mensagem = 'Por favor, verifique seu email pois lhe enviamos uma nova senha de acesso.';

                $this->commit();
            }
        }

        if($retorno){
            // valida a senha informada
            if (md5(md5_decrypt_senha( $objUsuario->ususenha, '' )) != $senha ) {
                // Removido o bloqueio de senha
                /*
                $objUsuario->usutentativas += 1;

                if ( $objUsuario->usutentativas > $limiteTentativas ) {
                    $sql = sprintf( "UPDATE seguranca.usuario SET usutentativas = 0 WHERE usucpf = '%s'", $objUsuario->usucpf );
                    $this->executar( $sql );

                    $mensagem = "Usuário bloqueado por exceder a quantidade de tentativas de login com senha inválida.";
                    $retorno  = false;

                    $this->alterar_status_usuario( $objUsuario->usucpf, 'B', $mensagem );
                } else {
                    $mensagem = "A senha informada não é válida. Tentativas restantes: " . ($limiteTentativas - $objUsuario->usutentativas + 1);
                    $retorno  = false;

                    $sql = sprintf("UPDATE seguranca.usuario SET usutentativas = %d WHERE usucpf = '%s'",
                                    $objUsuario->usutentativas,
                                    $objUsuario->usucpf);
                    $this->executar( $sql );
                }
                */
                $mensagem = "Usuário ou senha inválidos.";
                $retorno  = false;
            }else{
                if ( $objUsuario->usutentativas > 0 || $objUsuario->usumigrado == 't' ) {
                    $sql = sprintf( "UPDATE seguranca.usuario SET usutentativas = 0, usumigrado = false WHERE usucpf = '%s'", $objUsuario->usucpf );
                    $this->executar( $sql );
                }
            }

            $this->commit();
        }

        //se chegou até esse ponto adiciona informações do usuário
        $arRetorno = array("codigo" => ($retorno ? 0 : 2), "mensagem" => $mensagem);
        if($retorno){
            $arRetorno['dados'] = array (
                'nome' => $objUsuario->usunome,
                'nomeResumido' => resumirNomeCompleto($objUsuario->usunome),
                'sexo' => $objUsuario->ususexo,
                'email' => $objUsuario->usuemail,
                'foto' => $objUsuario->arqid,
                't' => setParam(array(
                    'usuario' => $objUsuario->usucpf,
                    'senha' => $senha,
                    'data_hora' => date('Y-m-d H:i:s')
                ), false)
            );

            // Coloca os dados do usuário na sessão
            $_SESSION['usucpf'] = $_SESSION['usucpforigem'] = $objUsuario->usucpf;
            $_SESSION['usuario_id'] = $_SESSION['usuario_id'] = $objUsuario->id;
        }

        return $arRetorno;
    }

    public function logOffWebService($arDados){
        $mUsuarioDispositivo = new UsuarioDispositivo();
        $mUsuarioDispositivo->deletarPorUsuarioEToken($arDados['usuario'], $arDados['token']);
        return array("codigo" => 0, "mensagem" => 'LogOff Realizado com Sucesso!');
    }

    /**
     * Rertorna os dados do usuário
     *
     * @param $cpf cpf sem formatação
     * @return object
     */
    public function getDadosPorCpf($cpf){

        // verifica se o cpf já está cadastrado no sistema
        $sql = sprintf(
            "SELECT
			u.ususexo,
			u.usucpf,
			u.regcod,
			u.usunome,
			u.usuemail,
			u.usustatus,
			u.usufoneddd,
			u.usufonenum,
			u.ususenha,
			u.usudataultacesso,
			u.usunivel,
			u.usufuncao,
			u.ususexo,
			u.entid,
			u.unicod,
			u.usuchaveativacao,
			u.usutentativas,
			u.usuobs,
			u.ungcod,
			u.usudatainc,
			u.usuconectado,
			u.suscod,
			u.muncod,
			u.carid,
            u.arqid_cd
		FROM
			seguranca.usuario u
		WHERE
			u.usucpf = '%s'",
            $cpf
        );

        $usuario = (object) $this->pegaLinha( $sql );

        return $usuario;
    }

    public function cadastrarDadosGerais($dados){
        // Gerando a senha que poderá ser usada no SSD e no simec
        $senhageral = $this->gerar_senha();

        $cpf        = $dados['usucpf'];

        $usunome    = $dados['usunome'];
        $usuemail   = $dados['usuemail'];
        $usufoneddd = $dados['usufoneddd'];
        $usufonenum = $dados['usufonenum'];

        $regcod     = $dados['regcod'];
        $ususexo    = $dados['ususexo'];
        $muncod     = $dados['muncod'];
        $usufuncao  = $dados['usufuncao'];
        $orgao      = $dados['orgao'];

        // insere informações gerais do usuário
        $sql = sprintf(
            "INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, usuchaveativacao, regcod,
					ususexo, ususenha, suscod, orgao,
					muncod, usustatus
				) values (
					'%s', '%s', '%s', '%s',
					'%s', '%s', '%s',
					'%s', '%s', '%s', '%s',
					'%s', '%s', '%s'
				)",
            $cpf,
            str_to_upper( $usunome ),
            strtolower( $usuemail ),
            $usufoneddd,
            $usufonenum,
            $usufuncao,
            'f',
            $regcod,
            $ususexo,
            md5_encrypt_senha( $senhageral, '' ),
            'P',
            $orgao,
            $muncod,
            'A'
        );

        $this->executar( $sql );
    }

    public function getList($arWhere = array()){
        $arWhere[] = "usustatus = 'A'";

        $sql = "SELECT
                    usunome,
                    trim(usucpf) AS usucpf,
                    TO_CHAR(usudatanascimento, 'DD/MM/YYYY') AS usudatanascimento_formatado,
                    CASE 
                      WHEN ususexo = 'M' THEN 'Masculino'
                      WHEN ususexo = 'F' THEN 'Feminino'
                      ELSE ' - '
                    END ususexo,
                    usufonenum,
                    usuemail,
                    TO_CHAR(usudatainc, 'DD/MM/YYYY HH24:mi:ss') AS usudatainc_formatado,
                    usudatainc,
                    usudatainc,
                    usudatanascimento
                FROM
                    seguranca.usuario
                WHERE
                    " . implode(' AND ', $arWhere) . "
                ORDER BY
                    usunome ASC";

        $arRs = $this->carregar($sql);
        return $arRs ? $arRs : array();
    }

    public function atribuirPerfil($usucpf, $perfil)
    {
        $sql = "SELECT COUNT(*) FROM seguranca.perfilusuario  WHERE pflcod = {$perfil} AND usucpf = '{$usucpf}'";
        $qtdPerfil = $this->pegaUm($sql);

        if($qtdPerfil == 0){
            $sqlPerfil = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$usucpf}', {$perfil});";
            $this->executar($sqlPerfil);
        }
    }

    public function salvarUsuario($params)
    {
        $params['usucpf'] = str_replace(array('.', '-'), '', $params['usucpf']);
        // Busca CPF na base para ver se é uma alteração
        $usucpfBd = $this->recuperarUm('usucpf', array("usucpf = '{$params['usucpf']}'"));

        $camposNulos = array();
        if(empty($params['usudatanascimento'])){
            $params['usudatanascimento'] = null;
            $camposNulos[] = 'usudatanascimento';
        }else{
            $params['usudatanascimento'] = formata_data_sql($params['usudatanascimento']);
        }

        $dados = array(
            'usucpf' => $params['usucpf'],
            'usunome' => $params['usunome'],
            'usudatanascimento' => $params['usudatanascimento'],
            'ususexo' => $params['ususexo'],
            'usufonenum' => $params['usufonenum'],
            'usuemail' => $params['usuemail'],
            'usustatus' => 'A',
        );

        if(empty($usucpfBd) || $params['senha_padrao']){
            $dados['ususenha'] = md5_encrypt_senha(self::SENHA_PADRAO, '');
            $dados['usuchaveativacao'] = 'f';
        }

        if($usucpfBd){
            $this->popularDadosObjeto($dados)->alterar($camposNulos);
        }else{
            $this->popularDadosObjeto($dados)->inserir();
        }

        $this->atribuirPerfis($params['perfil'], $params['usucpf']);
        return $params['usucpf'];
    }

    public function alterarDadosUsuarioLogado($params, $arquivos = array())
    {
       
        
        $params['usucpf'] = $_SESSION['usucpf'];
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $arqid = $params['arqid'];
        if (isset($arquivos['foto'])
            && isset($arquivos['foto']['name']) && !empty($arquivos['foto']['name'])
            && isset($arquivos['foto']['type']) && !empty($arquivos['foto']['type'])
            && isset($arquivos['foto']['size']) && !empty($arquivos['foto']['size'])) {
            $file = new FilesSimec();
            $arqid = $file->setUploadArquivoEspecifico("Foto do usuário {$params['usunome']}", 'foto');
        }
       
        $arqid_cd = $params['arqid_cd'];
        if (isset($arquivos['arquivo_cd'] )
            && isset($arquivos['arquivo_cd']['name']) && !empty($arquivos['arquivo_cd']['name'])
            && isset($arquivos['arquivo_cd']['type']) && !empty($arquivos['arquivo_cd']['type'])
            && isset($arquivos['arquivo_cd']['size']) && !empty($arquivos['arquivo_cd']['size'])) {
            $file = new FilesSimec();
            $arqid_cd = $file->setUploadArquivoEspecifico('CD', 'arquivo_cd');  
        }

        $dados = array(
            'usucpf' => $params['usucpf'],
            'usunome' => $params['usunome'],
            'usudatanascimento' => formata_data_sql($params['usudatanascimento']),
            'ususexo' => $params['ususexo'],
            'usufonenum' => $params['usufonenum'],
            'usuemail' => $params['usuemail'],
            'usustatus' => 'A',
            'arqid' => $arqid,
            'usuinst' => $params['usuinst'],
            'usuface' => $params['usuface'],
            'arqid_cd' => $arqid_cd
        );

        ver($dados);

        if($params['alterarSenha'] && $params['alterarSenha'] === true){
            $dados['ususenha'] = md5_encrypt_senha($params['ususenha'], '');
            $dados['usuchaveativacao'] = $params['usuchaveativacao'];
        }

        $_SESSION['usuarqid'] = $arqid;
        $this->popularDadosObjeto($dados)->alterar($camposNulos);

        $this->atualizarPessoas($dados);
        return $params['usucpf'];
    }

    public function isEmailCadastrado($usuemail, $usucpf)
    {
        $usucpf = str_replace(array('.', '-'), '', $usucpf);
        return $this->recuperarUm('count(*)', array("usuemail = '{$usuemail}'", "usucpf != '{$usucpf}'")) > 0;
    }

    public function isCpfCadastrado($usucpf){
        $usucpf = str_replace(array('.', '-'), '', $usucpf);
        return $this->recuperarUm('*', array("usucpf = '{$usucpf}'"));
    }

    public function atribuirPerfis($perfis, $usucpf, $sisid = null)
    {
        $perfis = $perfis ? $perfis : array();
        $sisid = $sisid ? $sisid : $_SESSION['sisid'];

        $mPerfilUsuario = new PerfilUsuario();
        $mPerfilUsuario->excluirPerfisUsuarioPorSistema($usucpf, $sisid);

        $mUsuarioSistema = new UsuarioSistema();
        $mUsuarioSistema->excluirUsuarioSistemaPorSistema($usucpf, $sisid);

        foreach($perfis as $pflcod){
            $perfilUsuario = array(
                'usucpf' => $usucpf,
                'pflcod' => $pflcod,
            );

            $mPerfilUsuario->popularDadosObjeto($perfilUsuario)->inserir();

            $usuarioSistema = array(
                'usucpf' => $usucpf,
                'sisid' => $sisid,
                'susstatus' => 'A',
                'pflcod' => null,
                'susdataultacesso' => null,
                'suscod' => null,
            );

            $mUsuarioSistema->popularDadosObjeto($usuarioSistema)->salvar();
        }
    }

    public function atualizarPessoas($dadosUsuario)
    {
        $cpf = formatar_cpf($dadosUsuario['usucpf']);
        $mPessoa = new PessoaFisica();
        $mContatoPf = new ContatoPessoaFisica();
        $pessoasUsuario = $mPessoa->recuperarColuna('id', array("cpf = '{$cpf}'"));

        foreach($pessoasUsuario as $pessoaId){
            $dadosPessoa = array(
                'id' => $pessoaId,
                'nome_completo' => $dadosUsuario['usunome'],
                'nome_social' => $dadosUsuario['usunome'],
                'foto_arquivo_id' => $dadosUsuario['arqid'],
                'cpf' => $cpf,
                'data_nascimento' => $dadosUsuario['usudatanascimento'],
                'sexo' => $dadosUsuario['ususexo'],
            );

            $mPessoa->manter($dadosPessoa);
            $mPessoa->setDadosNull();

            $mContatoPf->salvarContatos(array(
                'pessoafisica_id' => $pessoaId,
                'tipo_contato_sigla' => TipoContato::TIPO_EMAIL,
                'contato' => $dadosUsuario['usuemail']
            ));
            $mContatoPf->setDadosNull();

            $mContatoPf->salvarContatos(array(
                'pessoafisica_id' => $pessoaId,
                'tipo_contato_sigla' => TipoContato::TIPO_CELULAR,
                'contato' => $dadosUsuario['usufonenum']
            ));
            $mContatoPf->setDadosNull();
        }
    }

    public function redefinirSenha($params)
    {
        $params['usucpf'] = str_replace(array('.', '-'), '', $params['usucpf']);
        $cpfUsuario = $this->recuperarUm('usucpf', array("usucpf = '{$params['usucpf']}'"));

        if($cpfUsuario) {
            $dados = array(
                'usucpf' => $cpfUsuario,
                'ususenha' => md5_encrypt_senha(self::SENHA_PADRAO, '')
            );

            $this->popularDadosObjeto($dados)->alterar();
        }
    }

    public function byTokenApi($tokenApi)
    {
        $usuario = [];

        if (empty($usuario)) {
            $usuario = $this->recuperarLinha('usucpf AS cpf, usuemail AS email, id, usucpf', ["token_api = '{$tokenApi}'"], false);
        }

        if (empty($usuario)) {
            return [];
        }

        return $usuario;
    }
}