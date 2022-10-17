<?php
require 'vendor/autoload.php';

define('MERCHANT_ID', 'd7689028-abff-4034-a442-045bad6e8ba1');
define('MERCHANT_KEY', 'KACkesku5lIFdRKnghtGl5GTvY57LgyPZMtUGWTo');

use Cielo\API30\Merchant;

use Cielo\API30\Ecommerce\Environment;
use Cielo\API30\Ecommerce\Sale;
use Cielo\API30\Ecommerce\CieloEcommerce;
use Cielo\API30\Ecommerce\Payment;

use Cielo\API30\Ecommerce\Request\CieloRequestException;

// ...
// Configure o ambiente
$environment = $environment = Environment::sandbox();

// Configure seu merchant
$merchant = new Merchant(MERCHANT_ID, MERCHANT_KEY);

// Crie uma instância de Sale informando o ID do pagamento
$sale = new Sale('123');

// Crie uma instância de Customer informando o nome do cliente
$customer = $sale->customer('Fulano de Tal');

// Crie uma instância de Payment informando o valor do pagamento
$payment = $sale->payment(15700);

// Crie uma instância de Credit Card utilizando os dados de teste
// esses dados estão disponíveis no manual de integração
$payment->setType(Payment::PAYMENTTYPE_CREDITCARD)
    ->creditCard("123", "Visa")
    ->setExpirationDate("12/2018")
    ->setCardNumber("0000000000000001")
    ->setHolder("Fulano de Tal");

// Crie o pagamento na Cielo
try {
// Configure o SDK com seu merchant e o ambiente apropriado para criar a venda
    $sale = (new CieloEcommerce($merchant, $environment))->createSale($sale);

// Com a venda criada na Cielo, já temos o ID do pagamento, TID e demais
// dados retornados pela Cielo
    $paymentId = $sale->getPayment()->getPaymentId();
    echo '<pre>'; var_dump($paymentId);

// Com o ID do pagamento, podemos fazer sua captura, se ela não tiver sido capturada ainda
    $sale = (new CieloEcommerce($merchant, $environment))->captureSale($paymentId, 15700, 0);
    echo '<pre>'; var_dump($sale);

// E também podemos fazer seu cancelamento, se for o caso
    $sale = (new CieloEcommerce($merchant, $environment))->cancelSale($paymentId, 15700);
    echo '<pre>'; var_dump($sale);
} catch (CieloRequestException $e) {
// Em caso de erros de integração, podemos tratar o erro aqui.
// os códigos de erro estão todos disponíveis no manual de integração.
    $error = $e->getCieloError();
    echo '<pre>'; var_dump($error);
}