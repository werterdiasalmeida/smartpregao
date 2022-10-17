<?php

use Simec\Adm\Cliente;

class ClienteApi extends AbstractApi
{
    public function routes()
    {
        return [
            ['route' => 'GET /cliente/consultar/@cpf', 'callback' => 'consultar', 'auth' => true],
        ];
    }

    public function autoloader($pathsAdicionais = [])
    {
        include_once APPRAIZ . '/www/adm/_constantes.php';
        parent::autoloader($pathsAdicionais);
    }

    public function consultar($cpf)
    {
        $mCartaoAderencia = new CartaoAderencia();
        $beneficiario = $mCartaoAderencia->getByCPF($cpf);

        if (!$beneficiario) {
            $this->json([
                'message' => 'Beneficiario não encontrado.',
                'code' => 404
            ], 404);
            return;
        }

        $dependentes = json_decode(utf8_encode($beneficiario['dependentes']), true);
        $beneficiario['dependentes'] = [];

        $beneficiario['nome'] = ucwords(strtolower($beneficiario['nome']));

        foreach ($dependentes as $dependente){
            $beneficiario['dependentes'][] = utf8ArrayDecode([
                'cpf'=> $dependente['cpf'],
                'nome'=> ucwords(strtolower($dependente['nome'])),
                'parentesco'=> ($dependente['parentesco'] == 1) ? utf8_encode('Cônjuge') : 'Filho',
                'dt_nascimento'=> formata_data($dependente['dt_nascimento'])
            ]);
        }

        $this->json($beneficiario);
    }

}
