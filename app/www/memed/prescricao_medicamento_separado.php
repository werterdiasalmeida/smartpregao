<?php
// Substitua as variáveis abaixo
$apiKey = '$2y$10$r75QRT0YCGXn3QI8qdL/w.YL3B0GWk7Fn1BCYLD071CF7tdlGaWOm';
$secretKey = '$2y$10$i1C7FXIA.a8WE3DsiZROJOCBmmFB6nJFU.Il9J5w5Ar9TpUSkSH2K';
$medicamentos = array(
    // Um medicamento da Memed possui ID, ao envia-lo para a Memed, será identificado e ajudará
    // no melhoramento da busca para o usuário
    array(
        'id' => 'a123123123',
        'posologia' => 'Tomar 2x ao dia',
        'quantidade' => 2
    ),
    // O usuário pode criar um medicamento (chamamos de Medicamento Custom), sendo assim, ele não possui ID,
    // mas possui nome
    array(
        'nome' => 'Vitamina C, comprimido (100un) Sundown',
        'posologia' => 'Tomar 1x ao dia',
        'quantidade' => 1
    )
);

$usuarioId = '-1';
$usuarioCidade = 'Brasília';
$usuarioEstado = 'DF';
$usuarioDataNascimento = '30/12/1900';
$usuarioEspecialidade = 'Dermatologia';
$usuarioSexo = 'M';
$usuarioProfissao = 'Médico';

// Início do código para envio do request para a API da Memed
$jsonData = array(
    'data' => array(
        'type' => 'prescricoes',
        'attributes' => array(
            'medicamentos' => $medicamentos,
            'usuario' => array(
                'id' => $usuarioId,
                'cidade' => $usuarioCidade,
                'data_nascimento' => $usuarioDataNascimento,
                'estado' => $usuarioEstado,
                'especialidade' => $usuarioEspecialidade,
                'sexo' => $usuarioSexo,
                'profissao' => $usuarioProfissao
            )
        )
    )
);

$curl = curl_init();
curl_setopt_array($curl, array(
    CURLOPT_URL => 'https://api.memed.com.br/v1/prescricoes?api-key=' . $apiKey . '&secret-key=' . $secretKey,
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_TIMEOUT => 30,
    CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
    CURLOPT_CUSTOMREQUEST => 'POST',
    CURLOPT_POSTFIELDS => json_encode(utf8ArrayEncode($jsonData)),
    CURLOPT_HTTPHEADER => array(
        'accept: application/vnd.api+json',
        'cache-control: no-cache',
        'content-type: application/json',
    ),


    // Teste SSL
    CURLOPT_SSL_VERIFYPEER => 0,
    CURLOPT_SSL_VERIFYHOST => 0
));
$response = curl_exec($curl);
$err = curl_error($curl);
curl_close($curl);

// Para desenvolvimento, deixamos o código abaixo, assim é possível conferir a resposta da API da Memed
if ($err) {
    echo 'cURL Error #:' . $err;
} else {
    echo $response;
}

function utf8ArrayEncode($param){
    return is_array($param) ? array_map("utf8ArrayEncode", $param) : utf8_encode($param);
}