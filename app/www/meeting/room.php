<?php
include_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/SimecHashids.class.inc";

$db = new cls_banco();
$roomId = $_GET['r'];
$mensagemAvaliacao = "";
$tipoValidacao = "success";
$habilitaSala = false;

function __autoload($class_name)
{
    $arCaminho = array(
        APPRAIZ . "adm/classes/",
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/mensageria/",
        APPRAIZ . "includes/classes/",
    );

    foreach ($arCaminho as $caminho) {
        $arquivo = $caminho . $class_name . '.class.inc';
        if (file_exists($arquivo)) {
            require_once($arquivo);
            break;
        }
    }
}

if($_POST['act']) {
    switch ($_POST['act']) {
        case 'convidar-participante' :
            $mAgenda = new EstabelecimentoProfissionalAgenda();
            $telefone = $_POST['telefone'];

            if(empty($telefone)) {
                throw new Exception('Informe o telefone.');
            }

            $mSMS = new SimecSMS();
            $retorno = null;
            $destinatario = preg_replace("/[^0-9,]/", "", $_POST['telefone']);
            $mensagem = "Você foi convidado para participar de uma consulta no ClubVida em: {$mAgenda->getUrlAtendimentoRemotoByRoomId($roomId)}";
            $retorno = $mSMS->enviarSMS($mensagem, $destinatario, null, false);

            die(json_encode(utf8ArrayEncode([
                'msg' => 'Convite enviado.'
            ])));
            break;
    }
}

if (empty($roomId)) {
    $mensagemAvaliacao = "Nenhuma sala informada.";
    $tipoValidacao = 'warning';
} else {
    $hashids = new SimecHashids();
    $agendaId = $hashids->decode($roomId);

    if(empty($agendaId)) {
        $mensagemAvaliacao = "Nenhuma agenda encontrada.";
        $tipoValidacao = 'warning';
    } else {
        $mAgenda = new EstabelecimentoProfissionalAgenda();
        $dadosAgenda = $mAgenda->getTodosDados($agendaId);

        if(empty($dadosAgenda) || $dadosAgenda['pessoa_paciente_id'] == null) {
            $mensagemAvaliacao = "Nenhuma agenda encontrada.";
            $tipoValidacao = 'warning';
        } else {
            $mPessoa = new PessoaFisica();
            $dadosPaciente = $mPessoa->getTodosDados($dadosAgenda['pessoa_paciente_id']);
            $habilitaSala = true;
        }
    }
}

$acesso_medico = false;
if(preg_replace('/\D/', '', $_SESSION['usucpf']) == preg_replace('/\D/', '', $dadosAgenda['cpf_profissional']))
{
    $acesso_medico = true;
}

$sala_virtual = null;
if($dadosAgenda)
{
    $sala_virtual = new Jitsi($roomId, $acesso_medico);
}

?>
<html>
<head>
    <meta charset="ISO-8859-1"/>
    <title>ClubVida</title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1"/>
    <meta content="width=device-width, initial-scale=1" name="viewport"/>
    <meta content="" name="description"/>
    <meta content="" name="author"/>
    <!-- BEGIN GLOBAL MANDATORY STYLES -->
    <link href="//fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet"
          type="text/css"/>
    <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css" rel="stylesheet"
          type="text/css"/>
    <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css" rel="stylesheet"
          type="text/css"/>
    <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css" rel="stylesheet"
          type="text/css"/>
    <!-- END GLOBAL MANDATORY STYLES -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <link href="/includes/metronic/global/plugins/select2/css/select2.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/select2/css/select2-bootstrap.min.css" rel="stylesheet"
          type="text/css"/>
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL STYLES -->
    <link href="/includes/metronic/global/css/components.min.css" rel="stylesheet" id="style_components"
          type="text/css"/>
    <link href="/includes/metronic/global/css/plugins.min.css" rel="stylesheet" type="text/css"/>
    <!-- END THEME GLOBAL STYLES -->
    <!-- BEGIN PAGE LEVEL STYLES -->
    <link href="/includes/metronic/pages/css/login.min.css" rel="stylesheet" type="text/css"/>
    <!-- END PAGE LEVEL STYLES -->
    <!-- BEGIN THEME LAYOUT STYLES -->
    <!-- END THEME LAYOUT STYLES -->
    <link rel="shortcut icon" href="/favicon.ico"/>
</head>
<style type="text/css">
    fieldset, label {
        margin: 0;
        padding: 0;
    }

    body {
        background-color: #f1f4f7;
        overflow-x: hidden;
    }

    .content {
        padding: 10px;
    }

    .video-cont {
        height: 706px;
        border: 1px solid #e7ecf1;
        /*background-image: url('/meeting/img/no-video.png');*/
        background: rgb(0,0,0);
        background: linear-gradient(0deg, rgba(0,0,0,1) 0%, rgba(0,0,0,1) 36%, rgba(3,81,140,1) 100%);
        background-position: center;
        text-align: center;
    }

    .video-mini .video-cont {
        background-size: 70%;
        width: 200px;
        height: 200px;
        border: 0;
    }

    .video-mini {
        position: absolute;
        bottom: 5px;
        right: 25px;
        min-height: 0;
    }

    .video-mini .commands {
        position: absolute;
        top: 5px;
        left: 5px;
        width: 100%;
        height: 50px;
    }

    .media-container {
        display: inline-block;
        width: 100%;
        height: 100%;
        text-align: center;
    }

    .media-box {
        display: inline-flex;
        width: 100%;
        height: 100%;
        padding: 20px;
    }

    .video-mini .media-box {
        padding: 0;
        display: flex;
    }

    .media-box video {
        width: 100%;
    }

    .media-controls {
        /*display: none;*/
    }

    .media-controls, .volume-control {
        margin-top: 2px;
        position: absolute;
        margin-left: 5px;
        z-index: 100;
        opacity: 0;
        bottom: 0;
        text-align: center;
        margin-bottom: 40px;
        left: 50%;
        transform: translate(-50%, 8px);
    }

    .media-controls .control, .volume-control .control {
        width: 35px;
        height: 35px;
        background-position: center center;
        background-repeat: no-repeat;
        background-color: rgba(255, 255, 255, 0.84);
        border-radius: 20px !important;
        float: left;
        margin-left: 5px;
        cursor: pointer;
    }

    .video-mini .video-cont div.control {
        width: 30px;
        height: 30px;
    }

    .video-mini .video-cont div.media-controls {
        width: 105px;
    }

    .video-destaque .change-screen {
        display: none;
    }

    .share-screen {
        background-image: url('/meeting/img/share-screen.png');
        background-size: 70%;
    }

    .change-screen {
        background-image: url('/meeting/img/retweet.png');
        background-size: 70%;
    }

    .zoom-in {
        background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAadEVYdFNvZnR3YXJlAFBhaW50Lk5FVCB2My41LjExR/NCNwAAAe1JREFUWEftl8tqwkAUhn2DPoV3vEaNikENaHAruhLUbUC87FwIbly5cKGbgNBdu3GbF+kTTf2HOSVTx7SL0Aj1wE+mkzP/+RzN5DQSZjDGxCikkACi0SjLZrO+isfjvsj7/f4ln8+zTCajXE9CLeRLAJqmsWKxyIUxyft3qVTyBdjtdma5XGaFQkHygMgbSiQS/gAQimEOVxLMRboyDoeDqeu6tI7G5I8dGgwGagBKzOVyH4vFYrNarSRdt3gj0u/Ger3eLJdLaR28KpUK9x4OhyydTqsBCMJxnDcxHVgYhuH2+32+C6lU6u8BRqORC+/QAEzT5ACoEQpAq9VyqcZjAuC7wWOGx+hyuThiOrCYz+fv8IdisdgtQBjxBHgCPAEeCwAHBL2vT6dT4Cdhr9fjJyFO2uvrnleWAOgmII7HY+AAjUbDhTeUTCbVAATxP19GuIHtAcD5fA4coNls/twPAAA/xmtzGThAu93+3Q5AaKun0ymbzWZsMpnwMVSr1V5F+t0Yj8df+SR4UPG7ALT9SEAHa1kWfyS9PT7ui3Rl0P8F8KI1JJrDVQmAhSiGK4p7F5EAJtKVsd1uTW8+BA+vD8bKthxFq9Uq63a7/JOiGAlQUKfT8QVA1Ot13lXRmu+Ct23btwBhxAMAsMgnlwgSabRVBN4AAAAASUVORK5CYII=');
        background-size: 70%;
    }

    .zoom-out {
        background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAAadEVYdFNvZnR3YXJlAFBhaW50Lk5FVCB2My41LjExR/NCNwAAAglJREFUWEftV8lqAkEQzYd4EuK47ytueHA5OPgFjoIHEVzwYOIl4NWf8JKDl/xXPqPSr+nSUceZEYfEgA8Kpal686yqqWpf/gWCwSAlEgmKxWK02+3e1LEl4BsOh6W/OrofmqZRKpUiXdepXq9/qGNLpNNpgmWzWRoMBpo6vg+RSIS63a4krVQqtgKSySRlMhlvBdRqNcrn85K0XC7/vgCQ5nI5mVqnEsAX5YKAfr/vjQA0Hwjxy9wI4B7wTABn4BYB8C+VSu4FiDrrSB0eIuosDWR4nXAGUjcCuFnhz0IQZzbRyJev6Hg8lrUrFAqEpuNaMhEHu3kNO52O9OVYsyAYZoVyPwIC0OmNRkMGczbMRG4EcAO2Wq1DDMezRaPRSwGr1cq32WxoOp3SbDaztPl87jjdDMP4WiwWtFwuEfN9zgETz/pU7k888cQDYb1e+8SEMuwsFAoZyv0qxCS1jDVbr9e75JlMJofJd82wF5zuAzz97AwXHOV+BEaxlTOPT/5erVZtBZzHmj/ZLEex2ANyG55bPB4/IXCzjhGHfcCrGXFmzmKx6DjST4DUM5FbAfAX5fLmQoKa3ZIB+CEDYjE9BXgjQLy7khR1bbfbtgLQL7jYwH84HHojIBAIHDq72Ww6ZgB+EDAajbwR4Pf7pQCQbrfbd3VsCfwv5Gzt9/tXdfzAIKI/NHr5AU4kDfWD0WSsAAAAAElFTkSuQmCC');
        background-size: 70%;
    }

    .light {
        margin: 0;
    }

    .video-2 {
        bottom: 210px;
    }

    .video-3 {
        bottom: 414px;
    }

    @media (max-width:629px) {
        .share-screen {
            display: none;
        }
    }
</style>

<?php if($sala_virtual) : ?>
    <script src='https://8x8.vc/external_api.js' async></script>
    <script type="text/javascript">
    let api_jitsi;

    const initIframeAPI = () => {
        const domain = '<?php echo $sala_virtual->getDomain() ?>';
        const options = {
            roomName: '<?php echo $sala_virtual->getAppID() . '/' . $sala_virtual->getRoom() ?>',
            jwt: '<?php echo $sala_virtual->getToken() ?>',
            lang: 'ptBR',
            userInfo: {
                displayName: '<?php echo $acesso_medico ? $dadosAgenda['profissional'] : $dadosPaciente['nome_completo'] ?>'
            },
            parentNode: document.querySelector('#meet'),
            configOverwrite: { 
                toolbarButtons: ['microphone','camera','fullscreen','hangup','settings','chat'], 
                enableWelcomePage: true, 
                disableInviteFunctions: <?= $acesso_medico ? 'false' : 'true' ?>,
                disableModeratorIndicator: <?= $acesso_medico ? 'false' : 'true' ?>,
                prejoinPageEnabled: true,
                autoKnockLobby: true,
                },
            interfaceConfigOverwrite: { LANG_DETECTION: false, },
        };
        api_jitsi = new JitsiMeetExternalAPI(domain, options);

        if(api_jitsi == null)
        {
            api_jitsi = new JitsiMeetExternalAPI(domain, options);
        }
        
        api_jitsi.addListener("videoConferenceLeft", () => {
            finishJitsiIframeAPI();
        });
    }

    const finishJitsiIframeAPI = () => {
        $(".div_jitsi, #btn_jitsi_finalizar").hide();
        $("#div_foto_paciente, #btn_jitsi_iniciar").show();
        $("#jitsi_room_consulta").html('');
        $("#hidden_jitsi_link").val('');
        <?php if($acesso_medico) : ?>
            $.each(api_jitsi.getParticipantsInfo(), function (index,value) {
                if(value.displayName != '<?=$dadosAgenda['profissional']?>')
                {
                    api_jitsi.executeCommand('kickParticipant', value.participantId);
                }
            });
        <?php endif; ?>        
        api_jitsi.executeCommand('hangup');
        $('.video-destaque').html('<center><h2 style="margin-top:15%" >Consulta encerrada</h2></center>');
    }

    window.onload = () => {
        initIframeAPI();
    }
    </script>
<?php endif; ?>

<body>

<div class="content">
    <div class="row">
        <div class="col-md-offset-3 col-md-6">
            <?php if ($mensagemAvaliacao) : ?>
                <div class="alert alert-<?= $tipoValidacao; ?>">
                    <?= $mensagemAvaliacao ?>
                </div>
            <?php endif; ?>
        </div>
    </div>
    <?php if ($habilitaSala) : ?>
        <div class="portlet light bordered">
            <div class="portlet-title">
                <div class="caption">
                    <i class="fa fa-video-camera"></i>
                    <span class="caption-subject bold uppercase">
                        Atendimento remoto
                    </span>
                </div>
            </div>
            <div class="portlet-body form">
                <div class="row">
                    <?php if($acesso_medico): ?>
                        <div class="col-md-3 agenda-info">
                            <div class="row">
                                <div class="col-md-12 text-center">
                                    <div class="text-center" style="padding: 15px 0 35px 0;">
                                        <img src="/imagens/logo_assinatura_email.png" style="height: 65px;" alt=""/>
                                    </div>
                                </div>
                            </div>
                            <div class="row">
                                <div class="col-md-12">
                                    <div class="mt-widget-1">
                                        <div class="mt-img">
                                            <?= montaImgFoto($dadosPaciente['foto_arquivo_id'], $dadosPaciente['sexo'], '180px', '', '200', '200', 'img-circle bg-white', true) ?>
                                        </div>
                                        <div class="mt-body">
                                            <h3 class="mt-username"><?= $dadosPaciente['nome_completo'] ?></h3>
                                            <p class="mt-user-title">
                                                <strong>Idade: </strong>
                                                <?= calcularIdade($dadosPaciente['data_nascimento']) ?>
                                                <br/>
                                                <strong>Nascimento: </strong>
                                                <?= formata_data($dadosPaciente['data_nascimento']) ?>
                                                <br/>
                                                <strong>Sexo: </strong>
                                                <?= (($dadosPaciente['sexo']) == 'F' ? 'Feminino' : 'Masculino') ?>
                                            </p>
                                        </div>
                                    </div>
                                </div>
                                <div class="col-md-12">
                                    <div class="mt-widget-1">
                                        <div class="mt-body" style="padding-top: 5px;">
                                            <h3 class="mt-username font-blue-steel">
                                                <i class="fa fa-calendar font-blue-steel"></i> Agenda
                                            </h3>
                                            <p class="mt-user-title">
                                                <strong>Data:</strong> <?= formata_data($dadosAgenda['data_inicio']) ?>
                                                <br/>
                                                <strong>Hora:</strong> <?= substr($dadosAgenda['hora_inicio'], 0, 5) ?>
                                                - <?= substr($dadosAgenda['hora_fim'], 0, 5) ?>
                                                <br/>
                                                <strong>Procedimento:</strong> <?= $dadosAgenda['procedimento'] ?>
                                                <br/>
                                                <strong>Tipo de
                                                    serviço:</strong> <?= ($dadosAgenda['tipo_consulta'] ? $dadosAgenda['tipo_consulta'] : '-') ?>
                                                <br/>
                                                <strong>Responsável:</strong> <?= $dadosAgenda['profissional'] ?>
                                                <br/>
                                                <?php if($sala_virtual) : ?>
                                                    <strong>Link:</strong> https://<?php echo $sala_virtual->getDomain() . '/'. $sala_virtual->getAppID() . '/' . $sala_virtual->getRoom() ?>
                                                    <br/>
                                                    <br/>
                                                <?php endif; ?>
                                                <button class="btn btn-default btn-convidar-participante">
                                                    <i class="fa fa-share"></i>
                                                    Convidar participante
                                                </button>
                                            </p>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    <?php endif ?>
                    <div class="col-md-<?=$acesso_medico ? 9 : 12 ?>">
                        <div class="row">
                            <div class="col-md-12 videos">
                                <div class="video-destaque">
                                    <div class="video-cont remote"><div id="meet"></div></div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    <?php endif; ?>
</div>

<script src="/includes/JQuery/jquery-1.9.1.min.js"></script>
<script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/jquery-inputmask/jquery.inputmask.bundle.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/moment.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap-datepicker/js/bootstrap-datepicker.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap-growl/jquery.bootstrap-growl.min.js" type="text/javascript"></script>
<script src="/includes/funcoesm.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/jquery.blockui.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/scripts/app.js" type="text/javascript"></script>

<script type="text/javascript">
    
</script>
</body>
</html>