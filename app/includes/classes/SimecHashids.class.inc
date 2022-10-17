<?php

use Hashids\Hashids;

class SimecHashids
{
    private $salt = 'ClubeVida Hashid';

    public function __construct()
    {
        require_once(APPRAIZ . "includes/hashids/Math/MathInterface.php");
        require_once(APPRAIZ . "includes/hashids/Math/Bc.php");
        require_once(APPRAIZ . "includes/hashids/Math/Gmp.php");
        require_once(APPRAIZ . "includes/hashids/HashidsInterface.php");
        require_once(APPRAIZ . "includes/hashids/HashidsException.php");
        require_once(APPRAIZ . "includes/hashids/Hashids.php");
    }

    public function encode($value, $length = null)
    {
        $hashids = new Hashids($this->salt, $length);
        return $hashids->encode($value);
    }

    public function decode($value, $length = null)
    {
        $length = $length ? $length : strlen($value);
        $hashids = new Hashids($this->salt, $length);
        $retorno = $hashids->decode($value);
        return count($retorno) == 1 ? $retorno[0] : $retorno;
    }
}