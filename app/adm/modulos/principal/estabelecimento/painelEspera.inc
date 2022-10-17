<?php
$p = getParam();

if($_POST['act']){
    switch ($_POST['act']) {
        case 'painelEsperaList':
            painelEsperaList($p['id']);
            die;
            break;
    }
}

$mEstabelecimento = new Estabelecimento();
$estabelecimento = $mEstabelecimento->getTodosDados($p['id']);

?>
<!DOCTYPE html>
<html lang="pt">
<!--<![endif]-->
<!-- BEGIN HEAD -->

<head>
    <meta charset="ISO-8859-1"/>
    <title><?php echo ($moduloTitle ? "{$moduloTitle} - " : '') . $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] ?></title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1"/>
    <meta content="width=device-width, initial-scale=1" name="viewport"/>
    <meta content="" name="description"/>
    <meta content="" name="author"/>

    <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css?v=<?= VERSAO ?>" rel="stylesheet" type="text/css"/>
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

    <!-- BEGIN CORE PLUGINS -->
    <script src="/includes/metronic/global/plugins/jquery.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js?v=<?= VERSAO ?>" type="text/javascript"></script>
</head>
<body style="background-color: #FFF;">
    <div class="page-container">
        <!-- BEGIN CONTENT -->
        <div class="page-content-wrapper">
            <!-- BEGIN CONTENT BODY -->
            <div class="page-content" style="margin-left: 0;">
                <div class="row">
                    <div class="col-sm-4">
                        <img src="/imagens/logo2.png?v=<?= VERSAO ?>" alt="logo" class="logo-default" />
                    </div>
                    <div class="col-sm-4 text-center">
                        <h1 style="margin-bottom: 0;"><?= $estabelecimento['nome_fantasia'] ?></h1>
                        <small>Painel gerencial</small>
                        <h3>Fila de espera de atendimento</h3>
                    </div>
                    <div class="col-sm-4 text-right">
                        <h2 class="relogio"></h2>
                    </div>
                </div>

                <div class="painel-espera-list">
                    <?php painelEsperaList($p['id']); ?>
                </div>

            </div>
        </div>
    </div>
    <script type="text/javascript">
        $(function(){
            iniciarRelogio();
            iniciarAtualizacaoDados();
        });

        function iniciarRelogio() {
            setHora(new Date());

            function setHora(data) {
                function formataHora(num) {
                    return num.toString().padStart(2, '0');
                }

                $('.relogio').text(formataHora(data.getHours()) + ':' + formataHora(data.getMinutes()) + ':' + formataHora(data.getSeconds()));
            }

            setInterval(function() {
                setHora(new Date());
            }, 1000);
        }

        function iniciarAtualizacaoDados() {
            function atualizarDados() {
                $('.painel-espera-list').load(window.location.href, {act : 'painelEsperaList', p : '<?= $_REQUEST['p'] ?>'});
            }

            setInterval(function() {
                atualizarDados();
            }, 5000);
        }
    </script>
</body>
</html>
<?php
function painelEsperaList($estabelecimentoId) {
    $dataAtual = date('Y-m-d');
    $mAgenda = new EstabelecimentoProfissionalAgenda();

    $atendimentoPorEspecialidade = $mAgenda->getDadosPainelFilaAtendimento($estabelecimentoId, $dataAtual);

    if(count($atendimentoPorEspecialidade) == 0) :
        die('<h3 class="text-center">Nenhum atendimento realizado.</h3>');
    endif;

    echo "<div class='row'>";

    foreach($atendimentoPorEspecialidade as $atendimentoPorEspecialidade) :
        ?>
            <div class="col-md-3">
                <div class="portlet light bordered bg-blue-hoki" style="color: #FFF;">
                    <div class="portlet-title">
                        <div class="caption text-center" style="color: #FFF">
                            <?= $atendimentoPorEspecialidade['especialidade'] ?>
                        </div>
                    </div>
                    <div class="portlet-body">
                        <div class="row">
                            <div class="col-sm-5 text-right">
                                <label>Em atendimento</label>
                            </div>
                            <div class="col-sm-7">
                                <?= formata_valor($atendimentoPorEspecialidade['qtd_em_atendimento'], 0); ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-sm-5 text-right">
                                <label>Aguardando</label>
                            </div>
                            <div class="col-sm-7">
                                <?= formata_valor($atendimentoPorEspecialidade['qtd_aguardando'], 0); ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-sm-5 text-right">
                                <label>Atendidos</label>
                            </div>
                            <div class="col-sm-7">
                                <?= formata_valor($atendimentoPorEspecialidade['qtd_atendido'], 0); ?>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-sm-5 text-right">
                                <label>Espera média</label>
                            </div>
                            <div class="col-sm-7">
                                <?= formata_valor($atendimentoPorEspecialidade['tempo_medio_espera'], 0); ?>
                                <small>minutos</small>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        <?php
    endforeach;

    echo "</div>";

    $filaAtendimento = $mAgenda->getFilaAtendimento($estabelecimentoId, $dataAtual);

    if(count($filaAtendimento) > 0) :
        ?>
        <div class="row">
            <div class="col-md-12">
                <h3 class="text-center">Últimas chamadas</h3>
                <table class="table">
                    <thead>
                        <tr>
                            <th>Paciente</th>
                            <th>Especialidade</th>
                            <th>Profissional</th>
                            <th>Sala</th>
                            <th>Hora</th>
                        </tr>
                    </thead>
                    <tbody>
                    <?php foreach ($filaAtendimento as $fila) :?>
                        <tr>
                            <td>
                                <?= $fila['paciente'] ?>
                            </td>
                            <td>
                                <?= $fila['especialidade'] ?>
                            </td>
                            <td>
                                <?= $fila['profissional'] ?>
                            </td>
                            <td>
                                <?= $fila['sala'] ?>
                            </td>
                            <td>
                                <?= $fila['hora'] ?>
                            </td>
                        </tr>
                    <?php endforeach; ?>
                    </tbody>
                </table>
            </div>
        </div>
    <?php endif;
}