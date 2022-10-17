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

        if(empty($dadosAgenda)) {
            $mensagemAvaliacao = "Nenhuma agenda encontrada.";
            $tipoValidacao = 'warning';
        } else {
            $mPessoa = new PessoaFisica();
            $dadosPaciente = $mPessoa->getTodosDados($dadosAgenda['pessoa_paciente_id']);
            $habilitaSala = true;
        }
    }
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
        height: 616px;
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
                                            <br/>
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
                    <div class="col-md-9">
                        <div class="row">
                            <div class="col-md-12 videos">
                                <div class="video-destaque">
                                    <div class="video-cont remote"></div>
                                </div>

                                <div class="video-mini video-1">
                                    <div class="video-cont local"></div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    <?php endif; ?>
</div>

<script src="/meeting/js/RTCMultiConnection.min.js"></script>
<script src="/meeting/js/socket.io.js"></script>
<script src="/meeting/node_modules/webrtc-adapter/out/adapter.js"></script>
<script src="/meeting/node_modules/webrtc-screen-capturing/getScreenId.js"></script>
<script src="/includes/JQuery/jquery-1.9.1.min.js"></script>
<script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/jquery-inputmask/jquery.inputmask.bundle.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/moment.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap-datepicker/js/bootstrap-datepicker.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/bootstrap-growl/jquery.bootstrap-growl.min.js" type="text/javascript"></script>
<script src="/includes/funcoesm.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/jquery.blockui.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/scripts/app.js" type="text/javascript"></script>

<!-- custom layout for HTML5 audio/video elements -->
<script src="/meeting/node_modules/recordrtc/RecordRTC.js"></script>
<script type="text/javascript">
    // ......................................................
    // ..................RTCMultiConnection Code.............
    // ......................................................
    let maximoConexoes = 4;
    let connection = new RTCMultiConnection();
    connection.socketURL = 'https://socketio.clubvida.com.br:9001/';
    connection.socketMessageEvent = 'video-conference';
    connection.session = {
        audio: true,
        video: true
    };
    connection.sdpConstraints.mandatory = {
        OfferToReceiveAudio: true,
        OfferToReceiveVideo: true
    };
    connection.maxParticipantsAllowed = maximoConexoes;

    let shareScreen = false;

    let RMCMediaTrack = {
        selfVideo: null,
        screen: null,
        cameraStream : null,
        cameraTrack : null
    };

    connection.onstream = function (event) {
        connection.videosContainer = $('div.video-cont.' + event.type + ':last')[0];

        if(connection.videosContainer.innerHTML.length > 0) {
            let videosContent = $('.videos');
            let videoId = (videosContent.find('.video-mini').length + 1);
            videosContent.append('<div class="video-mini video-' + videoId + '">' +
                                     '<div class="video-cont ' + event.type + '"></div>' +
                                 '</div>');
            connection.videosContainer = videosContent.find('.video-' + videoId).find('.video-cont')[0];
        }

        let existing = document.getElementById(event.streamid);
        if (existing && existing.parentNode) {
            existing.parentNode.removeChild(existing);
        }
        event.mediaElement.removeAttribute('src');
        event.mediaElement.removeAttribute('srcObject');
        event.mediaElement.muted = true;
        event.mediaElement.volume = 0;
        let video = document.createElement('video');
        try {
            video.setAttributeNode(document.createAttribute('autoplay'));
            video.setAttributeNode(document.createAttribute('playsinline'));
        } catch (e) {
            video.setAttribute('autoplay', true);
            video.setAttribute('playsinline', true);
        }
        if (event.type === 'local') {
            video.volume = 0;
            try {
                video.setAttributeNode(document.createAttribute('muted'));
            } catch (e) {
                video.setAttribute('muted', true);
            }
        }
        video.srcObject = event.stream;
        let mediaElement = getHTMLMediaElement(video, {
            // title: event.userid,
            buttons: ['full-screen'],
            /*width: '100%',
            height: '100%',*/
            showOnMouseEnter: false,
            class: event.type,
            type : event.type
        });
        connection.videosContainer.appendChild(mediaElement);
        setTimeout(function () {
            mediaElement.media.play();
        }, 5000);
        mediaElement.id = event.streamid;

        if(event.type === 'local') {
            RMCMediaTrack.selfVideo = mediaElement.media;

            if(event.stream.isVideo) {
                RMCMediaTrack.cameraStream = event.stream;
                RMCMediaTrack.cameraTrack = event.stream.getVideoTracks()[0];
            }

            connection.socket.on('disconnect', function () {
                if (!connection.getAllParticipants().length) {
                    location.reload();
                }
            });
        }
    };

    connection.onstreamended = function (event) {
        let mediaElement = document.getElementById(event.streamid);

        if (mediaElement) {
            mediaElement.parentNode.removeChild(mediaElement);
        }
    };
    connection.onMediaError = function (e) {
        if (e.message === 'Concurrent mic process limit.') {
            if (DetectRTC.audioInputDevices.length <= 1) {
                alert('Please select external microphone. Check github issue number 483.');
                return;
            }
            let secondaryMic = DetectRTC.audioInputDevices[1].deviceId;
            connection.mediaConstraints.audio = {
                deviceId: secondaryMic
            };
            connection.join(connection.sessionid);
        }
    };

    let roomid = '<?= $roomId; ?>';
    if (roomid && roomid.length) {
        connection.openOrJoin(roomid, function (isRoomExist, roomid, error) {
            if (error) {
                alert(error);
            }
        });
    }

    // detect 2G
    if (navigator.connection &&
        navigator.connection.type === 'cellular' &&
        navigator.connection.downlinkMax <= 0.115) {
        alert('2G is not supported. Please use a better internet service.');
    }

    function getScreenStream(callback) {
        getScreenId(function(error, sourceId, screen_constraints) {
            navigator.mediaDevices.getUserMedia(screen_constraints).then(function(screen) {
                RMCMediaTrack.screen = screen.getVideoTracks()[0];

                RMCMediaTrack.selfVideo.srcObject = screen;

                // in case if onedned event does not fire
                (function looper() {
                    // readyState can be "live" or "ended"
                    if (RMCMediaTrack.screen.readyState === 'ended') {
                        RMCMediaTrack.screen.onended();
                        return;
                    }
                    setTimeout(looper, 1000);
                })();

                var firedOnce = false;
                RMCMediaTrack.screen.onended = RMCMediaTrack.screen.onmute = RMCMediaTrack.screen.oninactive = function() {
                    if (firedOnce) return;
                    firedOnce = true;

                    if (RMCMediaTrack.cameraStream.getVideoTracks()[0].readyState) {
                        RMCMediaTrack.cameraStream.getVideoTracks().forEach(function(track) {
                            RMCMediaTrack.cameraStream.removeTrack(track);
                        });
                        RMCMediaTrack.cameraStream.addTrack(RMCMediaTrack.cameraTrack);
                    }

                    RMCMediaTrack.selfVideo.srcObject = RMCMediaTrack.cameraStream;

                    connection.socket && connection.socket.emit(connection.socketCustomEvent, {
                        justStoppedMyScreen: true,
                        userid: connection.userid
                    });

                    // share camera again
                    replaceTrack(RMCMediaTrack.cameraTrack);

                    // now remove old screen from "attachStreams" array
                    connection.attachStreams = [RMCMediaTrack.cameraStream];
                    shareScreen = false;
                };

                connection.socket && connection.socket.emit(connection.socketCustomEvent, {
                    justSharedMyScreen: true,
                    userid: connection.userid
                });

                callback(screen);
            });
        });
    }

    function getHTMLMediaElement(mediaElement, config) {
        config = config || {};

        if (!mediaElement.nodeName || (mediaElement.nodeName.toLowerCase() != 'audio' && mediaElement.nodeName.toLowerCase() != 'video')) {
            if (!mediaElement.getVideoTracks().length) {
                return getAudioElement(mediaElement, config);
            }

            var mediaStream = mediaElement;
            mediaElement = document.createElement(mediaStream.getVideoTracks().length ? 'video' : 'audio');

            try {
                mediaElement.setAttributeNode(document.createAttribute('autoplay'));
                mediaElement.setAttributeNode(document.createAttribute('playsinline'));
            } catch (e) {
                mediaElement.setAttribute('autoplay', true);
                mediaElement.setAttribute('playsinline', true);
            }

            if ('srcObject' in mediaElement) {
                mediaElement.srcObject = mediaStream;
            } else {
                mediaElement[!!navigator.mozGetUserMedia ? 'mozSrcObject' : 'src'] = !!navigator.mozGetUserMedia ? mediaStream : (window.URL || window.webkitURL).createObjectURL(mediaStream);
            }
        }

        if (mediaElement.nodeName && mediaElement.nodeName.toLowerCase() == 'audio') {
            return getAudioElement(mediaElement, config);
        }

        var buttons = config.buttons || ['mute-audio', 'mute-video', 'full-screen', 'volume-slider', 'stop'];
        buttons.has = function (element) {
            return buttons.indexOf(element) !== -1;
        };

        config.toggle = config.toggle || [];
        config.toggle.has = function (element) {
            return config.toggle.indexOf(element) !== -1;
        };

        var mediaElementContainer = document.createElement('div');
        mediaElementContainer.className = 'media-container';

        var mediaControls = document.createElement('div');
        mediaControls.className = 'media-controls';
        mediaElementContainer.appendChild(mediaControls);

        if (buttons.has('mute-audio')) {
            var muteAudio = document.createElement('div');
            muteAudio.className = 'control ' + (config.toggle.has('mute-audio') ? 'unmute-audio selected' : 'mute-audio');
            mediaControls.appendChild(muteAudio);

            muteAudio.onclick = function () {
                if (muteAudio.className.indexOf('unmute-audio') != -1) {
                    muteAudio.className = muteAudio.className.replace('unmute-audio selected', 'mute-audio');
                    mediaElement.muted = false;
                    mediaElement.volume = 1;
                    if (config.onUnMuted) config.onUnMuted('audio');
                } else {
                    muteAudio.className = muteAudio.className.replace('mute-audio', 'unmute-audio selected');
                    mediaElement.muted = true;
                    mediaElement.volume = 0;
                    if (config.onMuted) config.onMuted('audio');
                }
            };
        }

        if (buttons.has('mute-video')) {
            var muteVideo = document.createElement('div');
            muteVideo.className = 'control ' + (config.toggle.has('mute-video') ? 'unmute-video selected' : 'mute-video');
            mediaControls.appendChild(muteVideo);

            muteVideo.onclick = function () {
                if (muteVideo.className.indexOf('unmute-video') != -1) {
                    muteVideo.className = muteVideo.className.replace('unmute-video selected', 'mute-video');
                    mediaElement.muted = false;
                    mediaElement.volume = 1;
                    mediaElement.play();
                    if (config.onUnMuted) config.onUnMuted('video');
                } else {
                    muteVideo.className = muteVideo.className.replace('mute-video', 'unmute-video selected');
                    mediaElement.muted = true;
                    mediaElement.volume = 0;
                    mediaElement.pause();
                    if (config.onMuted) config.onMuted('video');
                }
            };
        }

        if (buttons.has('take-snapshot')) {
            var takeSnapshot = document.createElement('div');
            takeSnapshot.className = 'control take-snapshot';
            mediaControls.appendChild(takeSnapshot);

            takeSnapshot.onclick = function () {
                if (config.onTakeSnapshot) config.onTakeSnapshot();
            };
        }

        if (buttons.has('stop')) {
            var stop = document.createElement('div');
            stop.className = 'control stop';
            mediaControls.appendChild(stop);

            stop.onclick = function () {
                mediaElementContainer.style.opacity = 0;
                setTimeout(function () {
                    if (mediaElementContainer.parentNode) {
                        mediaElementContainer.parentNode.removeChild(mediaElementContainer);
                    }
                }, 800);
                if (config.onStopped) config.onStopped();
            };
        }

        var volumeControl = document.createElement('div');
        volumeControl.className = 'volume-control';

        if (buttons.has('record-audio')) {
            var recordAudio = document.createElement('div');
            recordAudio.className = 'control ' + (config.toggle.has('record-audio') ? 'stop-recording-audio selected' : 'record-audio');
            volumeControl.appendChild(recordAudio);

            recordAudio.onclick = function () {
                if (recordAudio.className.indexOf('stop-recording-audio') != -1) {
                    recordAudio.className = recordAudio.className.replace('stop-recording-audio selected', 'record-audio');
                    if (config.onRecordingStopped) config.onRecordingStopped('audio');
                } else {
                    recordAudio.className = recordAudio.className.replace('record-audio', 'stop-recording-audio selected');
                    if (config.onRecordingStarted) config.onRecordingStarted('audio');
                }
            };
        }

        if (buttons.has('record-video')) {
            var recordVideo = document.createElement('div');
            recordVideo.className = 'control ' + (config.toggle.has('record-video') ? 'stop-recording-video selected' : 'record-video');
            volumeControl.appendChild(recordVideo);

            recordVideo.onclick = function () {
                if (recordVideo.className.indexOf('stop-recording-video') != -1) {
                    recordVideo.className = recordVideo.className.replace('stop-recording-video selected', 'record-video');
                    if (config.onRecordingStopped) config.onRecordingStopped('video');
                } else {
                    recordVideo.className = recordVideo.className.replace('record-video', 'stop-recording-video selected');
                    if (config.onRecordingStarted) config.onRecordingStarted('video');
                }
            };
        }

        if (buttons.has('volume-slider')) {
            var volumeSlider = document.createElement('div');
            volumeSlider.className = 'control volume-slider';
            volumeControl.appendChild(volumeSlider);

            var slider = document.createElement('input');
            slider.type = 'range';
            slider.min = 0;
            slider.max = 100;
            slider.value = 100;
            slider.onchange = function () {
                mediaElement.volume = '.' + slider.value.toString().substr(0, 1);
            };
            volumeSlider.appendChild(slider);
        }

        if (buttons.has('full-screen')) {
            var zoom = document.createElement('div');
            zoom.className = 'control ' + (config.toggle.has('zoom-in') ? 'zoom-out selected' : 'zoom-in');
            zoom.title = 'Tela cheia';

            if (!slider && !recordAudio && !recordVideo && zoom) {
                mediaControls.insertBefore(zoom, mediaControls.firstChild);
            } else volumeControl.appendChild(zoom);

            zoom.onclick = function () {
                if (zoom.className.indexOf('zoom-out') != -1) {
                    zoom.className = zoom.className.replace('zoom-out selected', 'zoom-in');
                    exitFullScreen();
                } else {
                    zoom.className = zoom.className.replace('zoom-in', 'zoom-out selected');
                    launchFullscreen(mediaElementContainer);
                }
            };

            function launchFullscreen(element) {
                if (element.requestFullscreen) {
                    element.requestFullscreen();
                } else if (element.mozRequestFullScreen) {
                    element.mozRequestFullScreen();
                } else if (element.webkitRequestFullscreen) {
                    element.webkitRequestFullscreen(Element.ALLOW_KEYBOARD_INPUT);
                }
            }

            function exitFullScreen() {
                if (document.cancelFullScreen) {
                    document.cancelFullScreen();
                } else if (document.mozCancelFullScreen) {
                    document.mozCancelFullScreen();
                } else if (document.webkitCancelFullScreen) {
                    document.webkitCancelFullScreen();
                }
            }

            function screenStateChange(e) {
                if (e.srcElement != mediaElementContainer) return;

                var isFullScreeMode = document.webkitIsFullScreen || document.mozFullScreen || document.fullscreen;

                /*mediaElementContainer.style.width = (isFullScreeMode ? (window.innerWidth - 20) : config.width) + 'px';*/
                mediaElementContainer.style.display = isFullScreeMode ? 'block' : 'inline-block';

                // if (config.height) {
                //     mediaBox.style.height = (isFullScreeMode ? (window.innerHeight - 20) : config.height) + 'px';
                // }

                if (!isFullScreeMode && config.onZoomout) config.onZoomout();
                if (isFullScreeMode && config.onZoomin) config.onZoomin();

                if (!isFullScreeMode && zoom.className.indexOf('zoom-out') != -1) {
                    zoom.className = zoom.className.replace('zoom-out selected', 'zoom-in');
                    if (config.onZoomout) config.onZoomout();
                }
                setTimeout(adjustControls, 1000);
            }

            document.addEventListener('fullscreenchange', screenStateChange, false);
            document.addEventListener('mozfullscreenchange', screenStateChange, false);
            document.addEventListener('webkitfullscreenchange', screenStateChange, false);
        }

        if(config.type === 'local') {
            var btnShareScreen = document.createElement('div');
            btnShareScreen.className = 'control share-screen';
            btnShareScreen.id = 'share-screen';
            btnShareScreen.title = 'Compartilhar tela';
            mediaControls.insertBefore(btnShareScreen, mediaControls.firstChild);
            btnShareScreen.onclick = function() {
                if(!shareScreen) {
                    getScreenStream(function(screen) {
                        var isLiveSession = connection.getAllParticipants().length > 0;
                        if (isLiveSession) {
                            replaceTrack(RMCMediaTrack.screen);
                        }

                        // now remove old video track from "attachStreams" array
                        // so that newcomers can see screen as well
                        connection.attachStreams.forEach(function(stream) {
                            stream.getVideoTracks().forEach(function(track) {
                                stream.removeTrack(track);
                            });

                            // now add screen track into that stream object
                            stream.addTrack(RMCMediaTrack.screen);
                        });
                    });
                } else {
                    if (RMCMediaTrack.cameraStream.getVideoTracks()[0].readyState) {
                        RMCMediaTrack.cameraStream.getVideoTracks().forEach(function(track) {
                            RMCMediaTrack.cameraStream.removeTrack(track);
                        });
                        RMCMediaTrack.cameraStream.addTrack(RMCMediaTrack.cameraTrack);
                    }

                    RMCMediaTrack.selfVideo.srcObject = RMCMediaTrack.cameraStream;

                    connection.socket && connection.socket.emit(connection.socketCustomEvent, {
                        justStoppedMyScreen: true,
                        userid: connection.userid
                    });

                    // share camera again
                    replaceTrack(RMCMediaTrack.cameraTrack);

                    // now remove old screen from "attachStreams" array
                    connection.attachStreams = [RMCMediaTrack.cameraStream];
                }

                shareScreen = !shareScreen;
            };
        }

        var btnChangeScreen = document.createElement('div');
        btnChangeScreen.className = 'control change-screen';
        btnChangeScreen.id = 'change-screen';
        btnChangeScreen.title = 'Destacar tela';
        $(btnChangeScreen).click(function(){
            let container = $(this).closest('.video-mini');
            let classes = container.prop('class');
            $('.video-destaque').addClass(classes).removeClass('video-destaque');
            container.removeAttr('class');
            container.addClass('video-destaque');
        });
        mediaControls.insertBefore(btnChangeScreen, mediaControls.firstChild);

        if (buttons.has('volume-slider') || buttons.has('full-screen') || buttons.has('record-audio') || buttons.has('record-video')) {
            mediaElementContainer.appendChild(volumeControl);
        }

        var mediaBox = document.createElement('div');
        mediaBox.className = 'media-box' + (config.class ? ' ' + config.class : '');
        mediaElementContainer.appendChild(mediaBox);

        if (config.title) {
            var h2 = document.createElement('h2');
            h2.innerHTML = config.title;
            h2.setAttribute('style', 'position: absolute;color:white;font-size:17px;text-shadow: 1px 1px black;padding:0;margin:0;text-align: left; margin-top: 10px; margin-left: 10px; display: block; border: 0;line-height:1.5;z-index:1;');
            mediaBox.appendChild(h2);
        }

        mediaBox.appendChild(mediaElement);

        // if (!config.width) config.width = (innerWidth / 2) - 50;

        mediaElementContainer.style.width = config.width;

        if (config.height) {
            mediaBox.style.height = config.height;
        }

        // mediaBox.querySelector('video').style.maxHeight = innerHeight + 'px';

        var times = 0;

        function adjustControls() {
            // mediaControls.style.marginLeft = (mediaElementContainer.clientWidth - mediaControls.clientWidth - 2) + 'px';
            //
            // if (slider) {
            //     slider.style.width = (mediaElementContainer.clientWidth / 3) + 'px';
            //     volumeControl.style.marginLeft = (mediaElementContainer.clientWidth / 3 - 30) + 'px';
            //
            //     if (zoom) zoom.style['border-top-right-radius'] = '5px';
            // } else {
            //     volumeControl.style.marginLeft = (mediaElementContainer.clientWidth - volumeControl.clientWidth - 2) + 'px';
            // }
            //
            // volumeControl.style.marginTop = (mediaElementContainer.clientHeight - volumeControl.clientHeight - 2) + 'px';
            //
            // if (times < 10) {
            //     times++;
            //     setTimeout(adjustControls, 1000);
            // } else times = 0;
        }

        if (config.showOnMouseEnter || typeof config.showOnMouseEnter === 'undefined') {
            mediaElementContainer.onmouseenter = mediaElementContainer.onmousedown = function () {
                adjustControls();
                mediaControls.style.opacity = 1;
                volumeControl.style.opacity = 1;
            };

            mediaElementContainer.onmouseleave = function () {
                mediaControls.style.opacity = 0;
                volumeControl.style.opacity = 0;
            };
        } else {
            setTimeout(function () {
                adjustControls();
                setTimeout(function () {
                    mediaControls.style.opacity = 1;
                    volumeControl.style.opacity = 1;
                }, 300);
            }, 700);
        }

        adjustControls();

        mediaElementContainer.toggle = function (clasName) {
            if (typeof clasName != 'string') {
                for (var i = 0; i < clasName.length; i++) {
                    mediaElementContainer.toggle(clasName[i]);
                }
                return;
            }

            if (clasName == 'mute-audio' && muteAudio) muteAudio.onclick();
            if (clasName == 'mute-video' && muteVideo) muteVideo.onclick();

            if (clasName == 'record-audio' && recordAudio) recordAudio.onclick();
            if (clasName == 'record-video' && recordVideo) recordVideo.onclick();

            if (clasName == 'stop' && stop) stop.onclick();

            return this;
        };

        mediaElementContainer.media = mediaElement;

        return mediaElementContainer;
    }

    function getAudioElement(mediaElement, config) {
        config = config || {};

        if (!mediaElement.nodeName || (mediaElement.nodeName.toLowerCase() != 'audio' && mediaElement.nodeName.toLowerCase() != 'video')) {
            var mediaStream = mediaElement;
            mediaElement = document.createElement('audio');

            try {
                mediaElement.setAttributeNode(document.createAttribute('autoplay'));
                mediaElement.setAttributeNode(document.createAttribute('controls'));
            } catch (e) {
                mediaElement.setAttribute('autoplay', true);
                mediaElement.setAttribute('controls', true);
            }

            if ('srcObject' in mediaElement) {
                mediaElement.mediaElement = mediaStream;
            } else {
                mediaElement[!!navigator.mozGetUserMedia ? 'mozSrcObject' : 'src'] = !!navigator.mozGetUserMedia ? mediaStream : (window.URL || window.webkitURL).createObjectURL(mediaStream);
            }
        }

        config.toggle = config.toggle || [];
        config.toggle.has = function (element) {
            return config.toggle.indexOf(element) !== -1;
        };

        var mediaElementContainer = document.createElement('div');
        mediaElementContainer.className = 'media-container';

        var mediaControls = document.createElement('div');
        mediaControls.className = 'media-controls';
        mediaElementContainer.appendChild(mediaControls);

        var muteAudio = document.createElement('div');
        muteAudio.className = 'control ' + (config.toggle.has('mute-audio') ? 'unmute-audio selected' : 'mute-audio');
        mediaControls.appendChild(muteAudio);

        muteAudio.style['border-top-left-radius'] = '5px';

        muteAudio.onclick = function () {
            if (muteAudio.className.indexOf('unmute-audio') != -1) {
                muteAudio.className = muteAudio.className.replace('unmute-audio selected', 'mute-audio');
                mediaElement.muted = false;
                if (config.onUnMuted) config.onUnMuted('audio');
            } else {
                muteAudio.className = muteAudio.className.replace('mute-audio', 'unmute-audio selected');
                mediaElement.muted = true;
                if (config.onMuted) config.onMuted('audio');
            }
        };

        if (!config.buttons || (config.buttons && config.buttons.indexOf('record-audio') != -1)) {
            var recordAudio = document.createElement('div');
            recordAudio.className = 'control ' + (config.toggle.has('record-audio') ? 'stop-recording-audio selected' : 'record-audio');
            mediaControls.appendChild(recordAudio);

            recordAudio.onclick = function () {
                if (recordAudio.className.indexOf('stop-recording-audio') != -1) {
                    recordAudio.className = recordAudio.className.replace('stop-recording-audio selected', 'record-audio');
                    if (config.onRecordingStopped) config.onRecordingStopped('audio');
                } else {
                    recordAudio.className = recordAudio.className.replace('record-audio', 'stop-recording-audio selected');
                    if (config.onRecordingStarted) config.onRecordingStarted('audio');
                }
            };
        }

        var volumeSlider = document.createElement('div');
        volumeSlider.className = 'control volume-slider';
        volumeSlider.style.width = 'auto';
        mediaControls.appendChild(volumeSlider);

        var slider = document.createElement('input');
        slider.style.marginTop = '11px';
        slider.style.width = ' 200px';

        if (config.buttons && config.buttons.indexOf('record-audio') == -1) {
            slider.style.width = ' 241px';
        }

        slider.type = 'range';
        slider.min = 0;
        slider.max = 100;
        slider.value = 100;
        slider.onchange = function () {
            mediaElement.volume = '.' + slider.value.toString().substr(0, 1);
        };
        volumeSlider.appendChild(slider);

        var stop = document.createElement('div');
        stop.className = 'control stop';
        mediaControls.appendChild(stop);

        stop.onclick = function () {
            mediaElementContainer.style.opacity = 0;
            setTimeout(function () {
                if (mediaElementContainer.parentNode) {
                    mediaElementContainer.parentNode.removeChild(mediaElementContainer);
                }
            }, 800);
            if (config.onStopped) config.onStopped();
        };

        stop.style['border-top-right-radius'] = '5px';
        stop.style['border-bottom-right-radius'] = '5px';

        var mediaBox = document.createElement('div');
        mediaBox.className = 'media-box' + (config.class ? ' ' + config.class : '');
        mediaElementContainer.appendChild(mediaBox);

        var h2 = document.createElement('h2');
        h2.innerHTML = config.title || 'Audio Element';
        h2.setAttribute('style', 'position: absolute;color: rgb(160, 160, 160);font-size: 20px;text-shadow: 1px 1px rgb(255, 255, 255);padding:0;margin:0;');
        mediaBox.appendChild(h2);

        mediaBox.appendChild(mediaElement);

        mediaElementContainer.style.width = '329px';
        mediaBox.style.height = '90px';

        h2.style.width = mediaElementContainer.style.width;
        h2.style.height = '50px';
        h2.style.overflow = 'hidden';

        var times = 0;

        function adjustControls() {
            // mediaControls.style.marginLeft = (mediaElementContainer.clientWidth - mediaControls.clientWidth - 7) + 'px';
            // mediaControls.style.marginTop = (mediaElementContainer.clientHeight - mediaControls.clientHeight - 6) + 'px';
            // if (times < 10) {
            //     times++;
            //     setTimeout(adjustControls, 1000);
            // } else times = 0;
        }

        if (config.showOnMouseEnter || typeof config.showOnMouseEnter === 'undefined') {
            mediaElementContainer.onmouseenter = mediaElementContainer.onmousedown = function () {
                adjustControls();
                mediaControls.style.opacity = 1;
            };

            mediaElementContainer.onmouseleave = function () {
                mediaControls.style.opacity = 0;
            };
        } else {
            setTimeout(function () {
                adjustControls();
                setTimeout(function () {
                    mediaControls.style.opacity = 1;
                }, 300);
            }, 700);
        }

        adjustControls();

        mediaElementContainer.toggle = function (clasName) {
            if (typeof clasName != 'string') {
                for (var i = 0; i < clasName.length; i++) {
                    mediaElementContainer.toggle(clasName[i]);
                }
                return;
            }

            if (clasName == 'mute-audio' && muteAudio) muteAudio.onclick();
            if (clasName == 'record-audio' && recordAudio) recordAudio.onclick();
            if (clasName == 'stop' && stop) stop.onclick();

            return this;
        };

        mediaElementContainer.media = mediaElement;

        return mediaElementContainer;
    }

    function replaceTrack(videoTrack) {
        if (!videoTrack) return;
        if (videoTrack.readyState === 'ended') {
            alert('Can not replace an "ended" track. track.readyState: ' + videoTrack.readyState);
            return;
        }
        connection.getAllParticipants().forEach(function(pid) {
            var peer = connection.peers[pid].peer;
            if (!peer.getSenders) return;

            var trackToReplace = videoTrack;

            peer.getSenders().forEach(function(sender) {
                if (!sender || !sender.track) return;

                if (sender.track.kind === 'video' && trackToReplace) {
                    sender.replaceTrack(trackToReplace);
                    trackToReplace = null;
                }
            });
        });
    }

    $(function() {
        $('.btn-convidar-participante').popover({
            placement: 'top',
            title: 'Informe o número do participante:',
            html: true,
            popout : true,
            content: function() {
                return '<div class="input-group" style="width: 100%; white-space: nowrap;">' +
                       '     <input type="text" id="telefone-convidar" name="telefone_convidar" class="form-control" placeholder="(00)00000-0000" aria-label="(00)00000-0000" style="width: 150px">' +
                       '     <div class="input-group-append">' +
                       '         <button class="btn btn-outline-secondary btn-info btn-enviar-convite" type="button">Enviar</button>' +
                       '     </div>' +
                       '</div>';
            }
        }).on('click', function() {
            $("#telefone-convidar").inputmask({
                mask: ["(99) 9999-9999", "(99) 99999-9999"],
            });

            $('.btn-enviar-convite').click(function () {
                let telefone = $('#telefone-convidar').val();
                if(!telefone) {
                    exibirAviso('Por favor, informe o telefone.');
                    return;
                }

                App.blockUI({target : '.agenda-info', 'message' : 'Enviando...'});
                $.ajax({
                    url: window.location.href,
                    type: 'POST',
                    data : {
                        act : 'convidar-participante',
                        telefone : telefone
                    },
                    dataType : 'json',
                    error: function(error) {
                        exibirAlert('Não foi possível enviar o convite.');
                        console.error(error);
                        App.unblockUI('.agenda-info');
                    },
                    success: function(res) {
                        exibirSucesso(res.msg);
                        $('.btn-convidar-participante').trigger('click');
                        App.unblockUI('.agenda-info');
                    }
                });
            });
        });

        setInterval(function(){
            let videoMini = $('.video-mini');
            let totalConexoes = videoMini.length;
            if(totalConexoes.length > 1) {
                videoMini.each(function(){
                    if($(this).find('video').length === 0) {
                        $(this).remove();
                    } else {
                        for(let i = 3; i >= 1; i--) {
                            if($('.video-mini.video-' + i).length === 0){
                                $(this).removeAttr('class');
                                $(this).addClass('video-mini video-' + i);
                            }
                        }
                    }
                });
            }

            $('button.btn-convidar-participante').prop('disabled', $('div.videos > div.video-destaque, div.videos > div.video-mini').length >= maximoConexoes);
        }, 500);
    });
</script>
</body>
</html>