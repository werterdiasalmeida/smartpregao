<?php

class IndexApi extends AbstractApi
{
    public function routes()
    {
        return [
            ['route' => 'GET /', 'callback' => 'index', 'auth' => false]
        ];
    }

    public function index()
    {
        $this->app->render('index', ['routes' => $this->allRoutes()]);
    }
}