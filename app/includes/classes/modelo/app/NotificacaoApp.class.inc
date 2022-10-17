<?php

class NotificacaoApp extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "app.notificacaoapp";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("noaid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'noaid' => null,
        'noatitulo' => null,
        'noamensagem' => null,
        'noastatus' => null,
        'noadtcadastro' => null,
        'usucpf' => null,
        'appid' => null
    );

    protected $arConfiguracoesPush = array();

    public function __construct($id = null)
    {
        parent::__construct($id);

        $this->carregarConfiguracoesPush();
    }

    /**
     * Envia uma notificação do banco para os dispositivos selecionados
     * @param null $noaid
     * @return array
     */
    public function enviar($noaid = null)
    {
        $noaid = $noaid ? $noaid : $this->noaid;
        $this->carregarPorId($noaid);

        // Não envia mais para todos os dispositivos, agora só para os selecionados
        /*
        $mDispositivo = new Dispositivo();
        $arDispositivos = $mDispositivo->recuperarTodos('disid, distoken, displataforma', array("disstatus = 'A'"));
        */

        $mDispositivo = new NotificacaoUsuario();
        $arDispositivos = $mDispositivo->getDispositivosByNotificacao($noaid);

        $arTokenDispositivos = array();
        foreach ($arDispositivos as $dispositivo) {
            $arTokenDispositivos[] = $dispositivo['distoken'];
        }

        $mNotificacaoEnviada = new NotificacaoAppEnviada();
        $arDadosNotificacaoEnviada = array(
            'noaid' => $noaid,
            'naedtenviado' => 'now()',
            'usucpf' => $_SESSION['usucpf']
        );

        $naeid = $mNotificacaoEnviada->popularDadosObjeto($arDadosNotificacaoEnviada)->salvar();

        $arEnviouNotificacoes = $this->enviarNotificacao('Smart Pregão', $this->noamensagem, $arTokenDispositivos, $this->recuperarPayload($naeid));

        $this->gerarLogEnvio($naeid, $arDispositivos, $arTokenDispositivos, $arEnviouNotificacoes);

        $arLogEnvio = array();
        foreach ($arEnviouNotificacoes as $plataforma) {
            if (is_array($plataforma)) {
                $arLogEnvio = array_merge(array_filter($plataforma, function ($dispositivo) {
                    return $dispositivo;
                }), $arLogEnvio);
            }
        }

        return $arEnviouNotificacoes;
    }

    /**
     * Envia uma notificação qualquer para os dispositivos
     * @param $titulo
     * @param $mensagem
     * @param $arTokenDispositivo
     * @param array $arPayload
     * @return array
     */
    public function enviarNotificacao($titulo, $mensagem, $arTokenDispositivo, $arPayload = array())
    {
        $mDispositivo = new Dispositivo();
        $arDispositivos = $mDispositivo->recuperarTodos('disid, distoken, displataforma', array("disstatus = 'A'", "distoken IN ('" . implode("', '", $arTokenDispositivo) . "')"));
        $arDispositivos = $arDispositivos ? $arDispositivos : array();

        $arTokenDispositivo = array(
            'android' => array(),
            'ios' => array()
        );

        foreach ($arDispositivos as $dispositivo) {
            $arTokenDispositivo[strtolower($dispositivo['displataforma'])][] = $dispositivo['distoken'];
        }

        $arEnviouNotificacoes = array();
        foreach ($arTokenDispositivo as $plataforma => $tokenDispositivo) {
            if (is_array($tokenDispositivo) && count($tokenDispositivo) > 0) {
                /*switch ($plataforma) {
                    case 'android':*/
                        $arEnviouNotificacoes[$plataforma] = $this->realizarPushAndroid($titulo, $mensagem, $tokenDispositivo, $arPayload);
                  /*      break;
                    case 'ios':
                        $arEnviouNotificacoes[$plataforma] = $this->realizarPushIos($mensagem, $tokenDispositivo, $arPayload);
                        break;
                    default:
                        $arEnviouNotificacoes['ionic'] = $this->realizarPushIonic($titulo, $mensagem, $tokenDispositivo, $arPayload);
                        break;
                }*/
            }
        }

        return $arEnviouNotificacoes;
    }

    private function realizarPushIonic($titulo, $mensagem, $arTokenDispositivo, $arPayload = array())
    {
        $ionicProfile = $this->arConfiguracoesPush['ionic']['profile'];
        $ionicAuthenticationToken = $this->arConfiguracoesPush['ionic']['authtoken'];
        $urlPushIonic = $this->arConfiguracoesPush['ionic']['push_url'];
        $msgResumida = strlen($mensagem) > 100 ? substr($mensagem, 0, 100) . '...' : $mensagem;

        $arMensagem = array(
            'tokens' => $arTokenDispositivo,
            'profile' => $ionicProfile,
            'notification' => array(
                'title' => utf8_encode($titulo),
                'message' => utf8_encode($msgResumida),
                'payload' => $arPayload,
                'ios' => array(
                    'priority' => 10
                ),
                'android' => array(
                    'priority' => 'high',
                )
            )
        );

        $arHeader = array(
            'Content-Type: application/json',
            "Authorization: Bearer {$ionicAuthenticationToken}"
        );

        $curlRequest = curl_init();
        curl_setopt($curlRequest, CURLOPT_URL, $urlPushIonic);
        curl_setopt($curlRequest, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($curlRequest, CURLOPT_FOLLOWLOCATION, true);
        curl_setopt($curlRequest, CURLOPT_POST, 1);

        curl_setopt($curlRequest, CURLOPT_POSTFIELDS, json_encode($arMensagem));
        curl_setopt($curlRequest, CURLOPT_HTTPHEADER, $arHeader);
        curl_setopt($curlRequest, CURLOPT_SSL_VERIFYPEER, 0); // On dev server only!
        $resultadoCurl = curl_exec($curlRequest);
        $resultadoCurl = json_decode($resultadoCurl, true);

        // Ocorreu algum erro na requisição
        if (!$resultadoCurl || isset($resultadoCurl['error']) || curl_error($curlRequest) != "") {
            return false;
        }

        return true;
    }

    private function realizarPushAndroid($titulo, $mensagem, $arTokenDispositivo, $arPayload = array())
    {
        $googleAuthenticationToken = $this->arConfiguracoesPush['android']['authtoken'];
        $urlPushGoogle = $this->arConfiguracoesPush['android']['push_url'];

        if (is_array($googleAuthenticationToken)) {
            if (!is_null($this->appid) && isset($googleAuthenticationToken[$this->appid])) {
                $googleAuthenticationToken = $googleAuthenticationToken[$this->appid];
            } else {
                $googleAuthenticationToken = array_shift($googleAuthenticationToken);
            }
        }

        $arBody = array(
            'notification' => array(
                'title' => utf8_encode($titulo),
                'body' => utf8_encode($mensagem),
                'sound' => 'default',
                'click_action' => 'FCM_PLUGIN_ACTIVITY',
                'icon' => 'fcm_push_icon',
            ),
            'data' => $arPayload,
            'priority' => 'high'
        );

        $arHeader = array(
            'Authorization: key=' . $googleAuthenticationToken,
            'Content-Type: application/json'
        );

        $curlRequest = curl_init();
        curl_setopt($curlRequest, CURLOPT_URL, $urlPushGoogle);
        curl_setopt($curlRequest, CURLOPT_POST, true);
        curl_setopt($curlRequest, CURLOPT_HTTPHEADER, $arHeader);
        curl_setopt($curlRequest, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($curlRequest, CURLOPT_SSL_VERIFYPEER, false);

        $arEnviou = array();
        foreach ($arTokenDispositivo as $token) {
            $arBody['registration_ids'] = array($token);
            curl_setopt($curlRequest, CURLOPT_POSTFIELDS, json_encode($arBody));
            $resultadoCurl = curl_exec($curlRequest);
            $resultadoCurl = json_decode($resultadoCurl, true);

            // Ocorreu algum erro na requisição
            if (!$resultadoCurl || $resultadoCurl["failure"] > 0 /* || curl_error($curlRequest) != "" */) {
                if ($resultadoCurl["results"][0]["error"] == 'NotRegistered') {
                    $this->desabilitarDispositivoPorToken($token);
                } else {
                    $arEnviou[$token] = false;
                }
            } else {
                $arEnviou[$token] = true;
            }
        }

        curl_close($curlRequest);
        return $arEnviou;
    }

    private function realizarPushIos($mensagem, $arTokenDispositivo, $arPayload = array())
    {
        $passPhrase = $this->arConfiguracoesPush['ios']['passphrase'];
        $certificado = $this->arConfiguracoesPush['ios']['certificado'];
        $urlPushIos = $this->arConfiguracoesPush['ios']['push_url'];

        $streamContext = stream_context_create();
        stream_context_set_option($streamContext, 'ssl', 'local_cert', $certificado);
        stream_context_set_option($streamContext, 'ssl', 'passphrase', $passPhrase);

        $connection = stream_socket_client($urlPushIos, $err, $errstr, 60, STREAM_CLIENT_CONNECT | STREAM_CLIENT_PERSISTENT, $streamContext);

        if (!$connection) {
            return false;
        }

        $body = array(
            'aps' => array(
                'alert' => utf8_encode($mensagem),
                'sound' => 'default',
                'content-available' => 1
            ),
            'payload' => $arPayload
        );

        $payload = json_encode($body);
        $arEnviou = array();
        foreach ($arTokenDispositivo as $token) {
            $mensagem = chr(0) . pack('n', 32) . pack('H*', $token) . pack('n', strlen($payload)) . $payload;

            $enviado = fwrite($connection, $mensagem, strlen($mensagem));

            if (!$enviado) {
                $arEnviou[$token] = false;
            } else {
                $arEnviou[$token] = true;
            }
        }

        fclose($connection);
        return $arEnviou;
    }

    private function recuperarPayload($naeid)
    {
        return array(
            'titulo' => utf8_encode($this->noatitulo),
            'mensagem' => utf8_encode($this->noamensagem),
            'notificacao' => $naeid,
        );
    }

    private function gerarLogEnvio($naeid, array $arDispositivos, array $arTokenDispositivosLogar, array $arEnviouNotificacoes)
    {
        $mDispositivoNotificacaoApp = new DispositivoNotificacaoApp();
        foreach ($arDispositivos as $dispositivo) {
            if (is_array($arEnviouNotificacoes[$dispositivo['displataforma']]) // se a posição da plataforma é um array
                && isset($arEnviouNotificacoes[$dispositivo['displataforma']][$dispositivo['distoken']]) // se o valor do token existe no array
                && $arEnviouNotificacoes[$dispositivo['displataforma']][$dispositivo['distoken']] // se enviou a notificação com sucesso
                && in_array($dispositivo['distoken'], $arTokenDispositivosLogar)) { // se está no array de dispositivos para logar
                $arDadosDispositivoNotificacao = array(
                    'disid' => $dispositivo['disid'],
                    'naeid' => $naeid
                );

                $mDispositivoNotificacaoApp->popularDadosObjeto($arDadosDispositivoNotificacao)->salvar();
                $mDispositivoNotificacaoApp->setDadosNull();
            }
        }
    }

    public function getLista($arWhere = array(), $paginacao = false)
    {
        $arWhere[] = "noa.noastatus = 'A'";

        $sql = "SELECT
					noa.noatitulo,
					noa.noamensagem,
					noa.noadtcadastro,
					usu.usunome,
					noa.noaid
				FROM
					{$this->stNomeTabela} noa
				INNER JOIN
					seguranca.usuario usu ON usu.usucpf = noa.usucpf
				WHERE
					" . implode(' AND ', $arWhere);

        if (!$_POST["ordem"]) {
            $sql .= " ORDER BY noa.noadtcadastro DESC";
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 50);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    private function carregarConfiguracoesPush()
    {
        global $ionic_profile, $ionic_authtoken, $ionic_push_url, $android_authtoken, $android_push_url,
               $ios_passphrase, $ios_certificado, $ios_push_url;

        $this->arConfiguracoesPush = array(
            'ionic' => array(
                'profile' => $ionic_profile,
                'authtoken' => $ionic_authtoken,
                'push_url' => $ionic_push_url,
            ),
            'android' => array(
                'authtoken' => $android_authtoken,
                'push_url' => $android_push_url,
            ),
            'ios' => array(
                'passphrase' => $ios_passphrase,
                'certificado' => $ios_certificado,
                'push_url' => $ios_push_url,
            ),
        );
    }

    private function desabilitarDispositivoPorToken($token)
    {
        $mDispositivo = new Dispositivo();
        $mDispositivo->recuperarLinha('*', array("distoken = '{$token}'"));

        if ($mDispositivo->disid) {
            $mDispositivo->disstatus = 'I';
            $mDispositivo->salvar();
        }
    }
}

