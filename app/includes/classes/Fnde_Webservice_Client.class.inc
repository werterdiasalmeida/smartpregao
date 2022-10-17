<?php
/**
 * @package FNDE
 * @category Webservice
 * @name Client
 * @author Walker de Alencar Oliveira <walker.oliveira@fnde.gov.br>
 * @since 2009-10-02
 */

/**
 * Classe básica de consumo de servicos do FNDE
 * Depende do arquivo: http_codes.ini
 *
 * @author Walker de Alencar Oliveira <walker.oliveira@fnde.gov.br>
 * @version v0.0.2
 */
class Fnde_Webservice_Client {

    const CURL_EXTENSION_REQUIRED = "A extensão [CURL] não está carregada! \n Verifique o link: [http://br2.php.net/manual/pt_BR/curl.installation.php].";
    const URL_REQUIRED = 'É obrigatório infomar uma URL!';

    protected $params = array();
    protected $url = null;
    protected $proxy = array();

    private function __construct(){}

    /**
     * Cria uma instancia uma requisição
     *
     * @return Fnde_Webservice_Client
     */
    public static function CreateRequest(){
        return new self();
    }
    /**
     * Define um ou vários parametros através de um array, que deve conter Key => Value.
     * Ex.: $client->setParams( array( 'usuario' => 'teste', 'email' => 'email@teste.gov.br' )  );
     *
     * @param array $params
     * @return Fnde_Webservice_Client
     */
    public function setParams( array $params ){
        $this->params = $params;
        return $this;
    }
    /**
     * Define configurações de Proxy.
     *
     * @param string  $server
     * @param integer $port
     * @param string  $username
     * @param string  $password
     * @return Fnde_Webservice_Client
     */
    public function setProxy( $server, $username = null, $password = null ){
        $this->proxy = $proxy = array(
            'server'   => $server,
            'username' => $username,
            'password' => $password
        );
        return $this;
    }

    /**
     * Definir a URL que será acessada.
     * @param string $url
     * @return Fnde_Webservice_Client
     */
    public function setURL( $url ){
        $this->url = (string) $url;
        return $this;
    }

    /**
     * Pega os parametros que estão definidos.
     *
     * @return array
     */
    public function getParams(){
        return $this->params;
    }

    /**
     * Pega a URL que está definida
     *
     * @return string
     */
    public function getURL(){
        return $this->url;
    }

    /**
     * Checa os requerimentos necessários para proceder o método $this->execute();
     */
    private function checkRequirements(){
        if ( !extension_loaded('CURL') ){
            throw new Exception(self::CURL_EXTENSION_REQUIRED, E_ERROR);
        }
        if ( is_null($this->url) ) {
            throw new Exception( self::URL_REQUIRED, E_ERROR );
        }
    }

    /**
     * Executa o método POST e retorna a resposta da URL.
     *
     * @param bool $verbose
     *      Se o valor for true, gera um log do processo de requisição no arquivo: log.txt
     * @return string
     *      Retorno padrão é uma string xml.
     */
    public function execute( $verbose = false ){
        $this->checkRequirements();
		//echo ('URL:'. print_r($this->url,true));
        $handler = curl_init();
        curl_setopt($handler, CURLOPT_URL, $this->url );
        curl_setopt($handler, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; pl; rv:1.9) Gecko/2008052906 Firefox/3.0');
        curl_setopt($handler, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($handler, CURLOPT_RETURNTRANSFER, true);
        if ($verbose) {
        	//chmod(APPRAIZ.'arquivos/log.txt', 0755);
        	//arquivos
            $hf = fopen(APPRAIZ.'arquivos/' .'log.txt','w+');
            curl_setopt($handler, CURLOPT_VERBOSE, '2');
            curl_setopt($handler, CURLOPT_STDERR, $hf );
        }

        if ( count( $this->proxy ) > 0 ){
            curl_setopt($handler, CURLOPT_HTTPPROXYTUNNEL, true );
            curl_setopt($handler, CURLOPT_PROXYAUTH, CURLAUTH_BASIC );
            curl_setopt($handler, CURLOPT_PROXYTYPE, CURLPROXY_HTTP );
            curl_setopt($handler, CURLOPT_PROXY, $this->proxy['server'] );
            if ( !is_null($this->proxy['username']) ){
                curl_setopt($handler, CURLOPT_PROXYUSERPWD, "{$this->proxy['username']}:{$this->proxy['password']}" );
            }
        }

        if ( count( $this->params ) > 0 ){
        	//echo ('Parametros:'. print_r($this->params,true));
        	curl_setopt($handler, CURLOPT_POST, true);
            curl_setopt($handler, CURLOPT_POSTFIELDS, $this->params );
        }

        if( ( $result = curl_exec($handler) ) ) {
            $info = curl_getinfo($handler);
            curl_close($handler);
            if ( $info['http_code'] != 200 ) {
                $http_codes = parse_ini_file("http_codes.ini");
                throw new exception("Erro de Página: {$http_codes[$info['http_code']]}", $info['http_code'] );
            }
            return $result;
        } else {
        	$info = curl_getinfo($handler);
            $errorMessage = curl_error($handler);
            $errorCode = curl_errno($handler);
            //echo (print_r($info) . $errorMessage .  $errorCode);
            curl_close($handler);
            throw new Exception( $errorMessage, $errorCode );
        }
    }
}