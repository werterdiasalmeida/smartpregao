<?php
abstract class AbstractException extends Exception
{
    public function __construct($message = "", $code = 0, Throwable $previous = null)
    {
        define('ERROR', true);
        $message = $message ? $message : $this->getDefaultErrorMessage();
        parent::__construct($message, $code, $previous);
    }

    abstract protected function getDefaultErrorMessage();
}