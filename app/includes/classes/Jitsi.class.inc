<?php
require_once APPRAIZ . 'includes/jitsi/vendor/autoload.php';

use Jose\Component\Core\AlgorithmManager;
use Jose\Component\Signature\Algorithm\RS256;
use Jose\Component\Signature\JWSBuilder;
use Jose\Component\KeyManagement\JWKFactory;
use Jose\Component\Signature\Serializer\CompactSerializer;

class Jitsi
{
    protected $API_KEY="vpaas-magic-cookie-6f6df2cd52674b19a48b58ad1f697379/6cdf3c";
    protected $APP_ID="vpaas-magic-cookie-6f6df2cd52674b19a48b58ad1f697379";
    protected $USER_EMAIL="";
    protected $USER_NAME="";
    protected $USER_IS_MODERATOR=false;
    protected $USER_AVATAR_URL="";
    protected $USER_ID="paciente";
    protected $LIVESTREAMING_IS_ENABLED=false;
    protected $RECORDING_IS_ENABLED=false;
    protected $OUTBOUND_IS_ENABLED=false;
    protected $TRANSCRIPTION_IS_ENABLED=false;
    protected $EXP_DELAY_SEC=7200;
    protected $NBF_DELAY_SEC=10;
    protected $ROOM_NAME='Sala';
    protected $TOKEN=null;
    protected $DOMAIN='8x8.vc';

    public function __construct($ROOM_NAME = null, $USER_IS_MODERATOR = false)
    {
        $this->ROOM_NAME = $ROOM_NAME ?? rand(1,2500) . date('d-m-Y-H-i');
        $this->USER_IS_MODERATOR = $USER_IS_MODERATOR ?? false;
        $this->USER_ID = $USER_IS_MODERATOR ? 'medico' : 'paciente';
        $this->criarToken();
    }

    private function criarToken()
    {
        /**
         * We read the JSON Web Key (https://tools.ietf.org/html/rfc7517) 
         * from the private key we generated at https://jaas.8x8.vc/#/apikeys .
         * 
         * @var \Jose\Component\Core\JWK jwk
         //*/
        $jwk = JWKFactory::createFromKeyFile(APPRAIZ . 'includes/jitsi/ClubVida.pk');

        /**
         * Setup the algoritm used to sign the token.
         * @var \Jose\Component\Core\AlgorithmManager $algorithm
         */
        $algorithm = new AlgorithmManager([
            new RS256()
        ]);

        /**
         * The builder will create and sign the token.
         * @var \Jose\Component\Signature\JWSBuilder $jwsBuilder
         */
        $jwsBuilder = new JWSBuilder($algorithm);

        /**
         * Must setup JaaS payload!
         * Change the claims below or using the variables from above!
         */
        $payload = json_encode([
            'iss' => 'chat',
            'aud' => 'jitsi',
            'exp' => time() + $this->EXP_DELAY_SEC,
            'nbf' => time() - $this->NBF_DELAY_SEC,
            'room'=> $this->ROOM_NAME,
            'sub' => $this->APP_ID,
            'context' => [
                'user' => [
                    'moderator' => $this->USER_IS_MODERATOR ? "true" : "false",
                    'email' => $this->USER_EMAIL,
                    'name' => $this->USER_NAME,
                    'avatar' => $this->USER_AVATAR_URL,
                    'id' => $this->USER_ID
                ],
                'features' => [
                    'recording' => $this->RECORDING_IS_ENABLED ? "true" : "false",
                    'livestreaming' => $this->LIVESTREAMING_IS_ENABLED ? "true" : "false",
                    'transcription' => $this->TRANSCRIPTION_IS_ENABLED ? "true" : "false",
                    'outbound-call' => $this->OUTBOUND_IS_ENABLED ? "true" : "false"
                ]
            ]
        ]);

        /**
         * Create a JSON Web Signature (https://tools.ietf.org/html/rfc7515)
         * using the payload created above and the api key specified for the kid claim.
         * 'alg' (RS256) and 'typ' claims are also needed.
         */
        $jws = $jwsBuilder
                ->create()
                ->withPayload($payload)
                ->addSignature($jwk, [
                    'alg' => 'RS256',
                    'kid' => $this->API_KEY,
                    'typ' => 'JWT'
                ])
                ->build();

        /**
         * We use the serializer to base64 encode into the final token.
         * @var \Jose\Component\Signature\Serializer\CompactSerializer $serializer
         */
        $serializer = new CompactSerializer();
        $token = $serializer->serialize($jws, 0);
        $this->TOKEN = $token;
    }

    public function getToken()
    {
        return $this->TOKEN;
    }

    public function getUserName()
    {
        return $this->USER_NAME;
    }

    public function getUserId()
    {
        return $this->USER_ID;
    }

    public function getUserEmail()
    {
        return $this->USER_EMAIL;
    }

    public function getRoom()
    {
        return $this->ROOM_NAME;
    }

    public function getDomain()
    {
        return $this->DOMAIN;
    }

    public function getAppID()
    {
        return $this->APP_ID;
    }

}