<?php

class GuiaValidacao
{
    protected $erros = [];

    public function validarObrigatorio($valor, $label)
    {
        if(!isset($valor) || empty($valor)) {
            $this->erros[] = "O campo <strong>{$label}</strong> ? obrigat?rio.";
        }
    }

    public function validarTamanhoMinimo($valor, $label, $tamanhoMinimo)
    {
        if(!empty($valor) && strlen($valor) < $tamanhoMinimo) {
            $this->erros[] = "O tamanho do campo <strong>{$label}</strong> n?o pode ser menor que {$tamanhoMinimo}.";
        }
    }

    public function validarTamanhoMaximo($valor, $label, $tamanhoMaximo)
    {
        if(!empty($valor) && strlen($valor) > $tamanhoMaximo) {
            $this->erros[] = "O tamanho do campo <strong>{$label}</strong> n?o pode ser maior que {$tamanhoMaximo}.";
        }
    }

    public function validarNumero($valor, $label)
    {
        if(!empty($valor) && !is_numeric($valor)) {
            $this->erros[] = "O campo <strong>{$label}</strong> deve ser num?rico.";
        }
    }

    public function validarDominio($valor, $label, array $dominio)
    {
        $valorDominio = array_map(function($valor){
            return $valor['codigo'];
        }, $dominio);

        if(!empty($valor) && !in_array($valor, $valorDominio)) {
            $this->erros[] = "O campo <strong>{$label}</strong> n?o possui um valor permitido.";
        }
    }

    public function validarData($valor, $label)
    {
        $data = explode('-', $valor);
        if(!empty($valor) && !checkdate($data[1], $data[2], $data[0])) {
            $this->erros[] = "O campo <strong>{$label}</strong> deve ser uma data v?lida.";
        }
    }

    public function validarDinheiro($valor, $label)
    {
        $this->validarNumero($valor ,$label);
    }

    public function validarHora($valor, $label)
    {
        if(!empty($valor) && !preg_match("/^(?:2[0-3]|[01][0-9]):[0-5][0-9]$/", $valor)) {
            $this->erros[] = "O campo <strong>{$label}</strong> deve ser uma hora v?lida.";
        }
    }

    public function getErros()
    {
        return $this->erros;
    }
}