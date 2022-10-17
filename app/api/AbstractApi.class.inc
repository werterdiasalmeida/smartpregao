<?php

abstract class AbstractApi
{
    /**
     * @var flight\Engine
     */
    protected $app;

    private $usuario;

    private $allRoutes;

    public function __construct(flight\Engine $app)
    {
        $this->app = $app;
    }

    public function autoloader($pathsAdicionais = [])
    {
        if(count($pathsAdicionais) === 0) {
            return;
        }

        spl_autoload_register(function ($className) use ($pathsAdicionais) {
            foreach ($pathsAdicionais as $caminho) {
                $arquivo = $caminho . $className . '.class.inc';
                if (!file_exists($arquivo)) {
                    continue;
                }

                require_once $arquivo;
                break;
            }
        });
    }

    protected function json($data, $code = 200)
    {
        $this->app->json(utf8ArrayEncode($data), $code);
    }

    protected function getData()
    {
        return $this->app->request()->data->getData();
    }

    protected function serverUrl($returnRequestUri = true)
    {
        return 'http' . (isset($_SERVER['HTTPS']) ? 's' : '') . "://{$_SERVER['HTTP_HOST']}"
            . ($returnRequestUri ? "{$_SERVER['REQUEST_URI']}" : '');
    }

    public function setUsuario($usuario)
    {
        $this->usuario = $usuario;
    }

    public function getUsuario()
    {
        return $this->usuario;
    }

    public function setAllRoutes($allRoutes)
    {
        $this->allRoutes = $allRoutes;
    }

    public function allRoutes()
    {
        return $this->allRoutes;
    }

    abstract public function routes();
}
