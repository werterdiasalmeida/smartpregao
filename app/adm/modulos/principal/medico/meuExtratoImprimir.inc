<?php
$estabelecimentoId = $_REQUEST['estabelecimento_id'];
$tipo = $_REQUEST['tipo'];
$data_inicio = formata_data_sql($_REQUEST['inicio']);
$data_termino = formata_data_sql($_REQUEST['fim']);

$usucpfFormatado = formatar_cpf($_SESSION['usucpf']);

$mProfissional = new Profissional();
$mValorAPargar = new ValorAPagar();

//Define o tipo
$textoTipoPadrao = "Últimos 30 dias";
$dataInicio = (new DateTime())->sub(new DateInterval('P30D'));
$dataFim = (new DateTime());
if ($tipo == 'H') {
    $textoTipoPadrao = "Hoje";
    $dataInicio = (new DateTime());
    $dataFim = (new DateTime());
} elseif ($tipo == 'E') {
    $textoTipoPadrao = "Esta semana";
    $dataInicio = (new DateTime())->format('N') == 0 ? (new DateTime()) : (new DateTime())->modify('last sunday');
    $dataFim = clone $dataInicio;
    $dataFim = $dataFim->add(new DateInterval('P6D'));

} elseif ($tipo == 'D') {
    $dataInicio = new DateTime($data_inicio);
    $dataFim = new DateTime($data_termino);
    $textoTipoPadrao = $dataInicio->format('d/m/Y') . ' até ' . $dataFim->format('d/m/Y');
}

$arWhere = array(
    'estabelecimento_id' => $estabelecimentoId,
    'inicio' => $dataInicio->format('Y-m-d'),
    'fim' => $dataFim->format('Y-m-d'),
);

$saldoAnterior = $mValorAPargar->getSaldoAnteriorProfissionalByWhere($usucpfFormatado, $estabelecimentoId, $dataInicio->format('Y-m-d'));
$saldoAnteriorFormatado = formata_valor($saldoAnterior) . ' <i class="fa ' . ($saldoAnterior >= 0 ? 'fa-plus-circle' : 'fa-minus-circle') . '" aria-hidden="true"></i>';
$classSaldoAnterior = $saldoAnterior >= 0 ? 'valor-positivo' : 'valor-negativo';

$data = $mValorAPargar->getExtratoCompletoProfissionalByWhere($usucpfFormatado, $arWhere);

//Define os itens da tabela
$tabela = "";
$saldo = $saldoAnterior;
$valorAReceber = 0;
$valorRecebido = 0;
foreach ($data AS $indice => $registro) {

    $data = (new DateTime($registro['data_operacao']))->format('d/m/Y  H:i:s');
    $valor = formata_valor($registro['valor']);

    if ($registro['tipo'] == 'pagar') {
        $valorAReceber += $registro['valor'];
        $saldo += $registro['valor'];

        if($registro['valor'] >= 0){
            $iconeValor = 'fa-plus-circle';
            $classPagamento = "valor-positivo";
            $registro['estorno'] = false;
            $tituloIcone = "Informações sobre o crédito";
        } else {
            $iconeValor = 'fa-minus-circle';
            $classPagamento = "valor-negativo";
            $registro['estorno'] = true;
            $tituloIcone = "Informações sobre o estorno";
        }

        $rashAgenda = setParam(array('agenda_id' => $registro['agenda_id']));
    } else {
        $iconeValor = 'fa-minus-circle';
        $classPagamento = "valor-negativo";
        $valorRecebido += $registro['valor'];
        $saldo -= $registro['valor'];
    }

    if ($registro['estorno'] === true) {
        $registro['descricao'] .= " - ESTORNO";
        $registro['percentual_repasse'] = null;
        $registro['valor_total'] = null;
    }

    $saldoFormatado = formata_valor($saldo);
//    $valorProcedimentoFormatado = $registro['valor_total'] ? formata_valor($registro['valor_total']) : '-';
//    $percentualRepasseFormatado = $registro['percentual_repasse'] ? formata_valor($registro['percentual_repasse']) : '-';
    $classSaldo = $saldo >= 0 ? 'valor-positivo' : 'valor-negativo';
    $iconeSaldo = $saldo >= 0 ? 'fa-plus-circle' : 'fa-minus-circle';

    if ($registro['tipo'] == 'pagar' && $registro['remunerada'] == 'f' && $registro['valor'] == '0') {
        $tabela .= <<<HTML
                        <tr>
                            <td>{$data}</td>
                            <td>{$registro['descricao']}</td>
                            <td style="text-align: right">-</td>
                            <td class="{$classSaldo}" style="text-align: right"><span style="white-space: nowrap;" >{$saldoFormatado} <i class="fa {$iconeSaldo}" aria-hidden="true"></i></span></td>
                        </tr>
HTML;
    } else {
        $tabela .= <<<HTML
                    <tr>
                        <td>{$data}</td>
                        <td>{$registro['descricao']}</td>
                        <td class="{$classPagamento}" style="text-align: right"><span style="white-space: nowrap" >{$valor}  <i class="fa {$iconeValor}" aria-hidden="true"></i></span></td>
                        <td class="{$classSaldo}" style="text-align: right"><span style="white-space: nowrap;" >{$saldoFormatado} <i class="fa {$iconeSaldo}" aria-hidden="true"></i></span></td>
                    </tr>
HTML;
    }


}

if ($saldoAnterior > 0) {
    $valorAReceber += $saldoAnterior;
} else {
    $valorRecebido += $saldoAnterior;
}

$valorAReceberFormatado = formata_valor($valorAReceber);
$valorRecebidoFormatado = formata_valor($valorRecebido);

$estabelecimentos = $mProfissional->listaEstabelecimentosPorCpfProfissional($usucpfFormatado);
$campoEstabelecimento = $db->monta_combom("estabelecimento_id", $estabelecimentos, 'S', '', '', '', '', '', 'N', '', true, $estabelecimentoId);

$inicioFormatada = $dataInicio->format('d/m/Y');
$fimFormatada = $dataFim->format('d/m/Y');


if ($saldo) {
    $classSaldo = $saldo >= 0 ? 'valor-positivo' : 'valor-negativo';
    $saldoFormatado = formata_valor($saldo);
} else {
    $classSaldo = "";
    $saldoFormatado = formata_valor(0);
}


?>
<head>
    <meta charset="ISO-8859-1"/>
    <title><?php echo ($moduloTitle ? "{$moduloTitle} - " : '') . $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] ?></title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1"/>
    <meta content="width=device-width, initial-scale=1" name="viewport"/>
    <meta content="" name="description"/>
    <meta content="" name="author"/>
    <!-- BEGIN GLOBAL MANDATORY STYLES -->
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet"
          type="text/css"/>
    <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css"/>


    <link href="/includes/estilom.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css" />

    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL STYLES -->
    <link href="/includes/metronic/global/css/components.min.css?v=<?= VERSAO ?>" rel="stylesheet" id="style_components" type="text/css"/>
    <link href="/includes/metronic/global/css/plugins.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css"/>
    <!-- END THEME GLOBAL STYLES -->
    <!-- BEGIN THEME LAYOUT STYLES -->
    <link href="/includes/metronic/layouts/layout/css/layout.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/layouts/layout/css/themes/lightblue.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css"
          id="style_color"/>
    <!-- END THEME LAYOUT STYLES -->
    <link rel="shortcut icon" href="favicon.ico?v=<?= VERSAO ?>"/>

    <!--[if lt IE 9]>
    <script src="/includes/metronic/global/plugins/respond.min.js?v=<?= VERSAO ?>"></script>
    <script src="/includes/metronic/global/plugins/excanvas.min.js?v=<?= VERSAO ?>"></script>
    <![endif]-->
    <!-- BEGIN CORE PLUGINS -->
    <script src="/includes/metronic/global/plugins/jquery.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/estouvivo.js?v=<?= VERSAO ?>" type="text/javascript"></script>
</head>

<style type="text/css">
    @page {
        size: portrait;
    }

    body {
        background-color: unset;
    }

    .table td, thead th {
        padding: 5px;
    }

    .table thead tr th {
        font-size: 12px;
    }

    .table td {
        font-size: 10px;
    }
</style>
<?php
$styleHeader = 'background-color: #5E9BCF; color: #FFFFFF;';
?>
<div class="row">
    <div class="col-md-12" style="text-align: center; margin-bottom: 15px;">
        <img src="/imagens/logo_assinatura_email.png?v=<?= VERSAO ?>" style="height: 35px;" alt=""/>
    </div>
    <div style="text-align: center; margin-bottom: 15px;">
        <h6 class="font-blue-sharp" style="font-weight: bold;">
            Extrato Completo<br>
            <?= $_REQUEST['inicio'] ?> - <?= $_REQUEST['fim'] ?>
        </h6>
    </div>
</div>
<table class="table table-striped" cellpadding="0" cellspacing="0" style="width: 100%;">
    <thead>
    <tr>
        <th align="left">Data</th>
        <th>Descrição</th>
<!--        <th width="100" align="right">Valor do procedimento R$</th>-->
<!--        <th width="100" align="right">Percentual de repasse %</th>-->
        <th width="100" align="right">Valor R$</th>
        <th width="100" align="right">Saldo R$</th>
    </tr>

    </thead>
    <tbody>
    <tr class="saldo-anterior">
        <td colspan="5" style="text-align: right">Saldo anterior</td>
        <td class="<?= $classSaldoAnterior ?>" style="text-align: right"><?= $saldoAnteriorFormatado ?></td>
    </tr>
    <?= $tabela ?>
    <?php if($tabela == ""): ?>
    <tr>
        <td colspan="6" style="text-align: center;">Nenhum registro encontrado</td>
    </tr>
    <?php endif; ?>
    </tbody>
</table>
<script type="text/javascript">
    $(function () {
        window.print();
        setTimeout('window.close()', 10);
    });
</script>