<?php

include_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/SimecHashids.class.inc";

$db = new cls_banco();
$_SESSION['superuser'] = true;

function __autoload($class_name) {
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

    foreach($arCaminho as $caminho){
        $arquivo = $caminho . $class_name . '.class.inc';
        if ( file_exists( $arquivo ) ){
            require_once( $arquivo );
            break;
        }
    }
}

$hashids = new SimecHashids();
$agenda_id = $hashids->decode($_GET['p']); //11199

$mAgenda = new EstabelecimentoProfissionalAgenda();
$mEstabelecimento = new Estabelecimento();

$dadosAgenda = [];
$mensagemAvaliacao = "";
$tipoValidacao = "success";
if(!$agenda_id){
    $mensagemAvaliacao = '<strong>Atenção</strong> Nenhum atendimento informado!';
    $tipoValidacao = "warning";
}else{
    $dadosAgenda = $mAgenda->getTodosDados($agenda_id);
    $dadosEstabelecimento = $mEstabelecimento->getTodosDados($dadosAgenda['estabelecimento_id']);

    if(!$dadosAgenda){
        $mensagemAvaliacao = '<strong>Atenção</strong> Nenhum atendimento encontrado!';
        $tipoValidacao = "warning";
    }else{
        if($dadosAgenda['avaliacao_id']){
            $mensagemAvaliacao = '<strong>Atenção</strong> O atendimento já foi avaliado!';
            $tipoValidacao = "warning";
        }
    }
}


if(empty($mensagemAvaliacao) && $_POST && $_POST['act'] == 'salvar'){
    $mAvaliacao = new AvaliacaoAtendimento();
    $arDados = [
        'agenda_id'     => $dadosAgenda['estabelecimento_profissional_agenda_id'],
        'nota'          => $_POST['rating'],
        'melhorias'     => json_encode($_POST['melhorias']),
        'comentario'    => $_POST['comentario'],
        'excluido'      => 'FALSE'
    ];

    $mAvaliacao->manter($arDados);
    $mAvaliacao->commit();

    $mensagemAvaliacao = '<strong>Avaliação cadastrada com sucesso!</strong> O ClubVida agradece sua opinião, volte sempre...';
    $tipoValidacao = "info";
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
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css" rel="stylesheet"
          type="text/css"/>
    <!-- END GLOBAL MANDATORY STYLES -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <link href="/includes/metronic/global/plugins/select2/css/select2.min.css" rel="stylesheet" type="text/css"/>
    <link href="/includes/metronic/global/plugins/select2/css/select2-bootstrap.min.css" rel="stylesheet" type="text/css"/>
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL STYLES -->
    <link href="/includes/metronic/global/css/components.min.css" rel="stylesheet" id="style_components" type="text/css"/>
    <link href="/includes/metronic/global/css/plugins.min.css" rel="stylesheet" type="text/css"/>
    <!-- END THEME GLOBAL STYLES -->
    <!-- BEGIN PAGE LEVEL STYLES -->
    <link href="/includes/metronic/pages/css/login.min.css" rel="stylesheet" type="text/css"/>
    <!-- END PAGE LEVEL STYLES -->
    <!-- BEGIN THEME LAYOUT STYLES -->
    <!-- END THEME LAYOUT STYLES -->
    <link rel="shortcut icon" href="/favicon.ico?v=<?= VERSAO ?>"/>
    <style type="text/css">

        fieldset, label {
            margin: 0; padding: 0;
        }

        body{
            margin: 20px;
            background-color: #f1f4f7;
        }

        /****** Style Star Rating Widget *****/

        .rating {
            border: none;
            float: left;
        }

        .rating > input { display: none; }
        .rating > label:before {
            margin: 5px;
            font-size: 2.00em;
            font-family: FontAwesome;
            display: inline-block;
            content: "\f005";
        }

        .rating > .half:before {
            content: "\f089";
            position: absolute;
        }

        .rating > label {
            color: #ddd;
            float: right;
        }

        /***** CSS Magic to Highlight Stars on Hover *****/

        .rating > input:checked ~ label, /* show gold star when clicked */
        .rating:not(:checked) > label:hover, /* hover current star */
        .rating:not(:checked) > label:hover ~ label { color: #FFD700;  } /* hover previous stars in list */

        .rating > input:checked + label:hover, /* hover current star when changing rating */
        .rating > input:checked ~ label:hover,
        .rating > label:hover ~ input:checked ~ label, /* lighten current selection */
        .rating > input:checked ~ label:hover ~ label { color: #FFED85;  }

        .content {
            padding: 20px;
        }

        .content .stars{
            margin: auto;
            display: inline-block;
        }

        .content .subtitulo{
            color: #666;
            font-size: 14px;
        }

        .btn-group .btn {
            margin: 5px !important;
        }
        .form-actions{
            margin-top:15px !important;
        }

        .qtd-caracteres-comentario {
            color: #666 !important;
            font-style: italic;
        }

        .descricao-estrela{
            font-weight: bolder;
            font-style: italic;
        }

        .avaliacao-melhorias, .avaliacao-comentario {
            display: none;
        }
    </style>
</head>
<body>
    <div class="content">
        <div class="row">
            <div class="col-md-4 col-md-offset-4" style="text-align: center; padding: 15px 0 35px 0;">
                <img src="/imagens/logo_assinatura_email.png?v=<?= VERSAO ?>" style="height: 65px;" alt=""/>
            </div>
        </div>
        <div class="row">
            <div class="col-md-4 col-md-offset-4">
                <? if(empty($mensagemAvaliacao)): ?>
                    <div class="portlet light bordered">
                        <div class="portlet-body form">
                            <div>
                                <div class="col-md-12">
                                    <div class="col-xs-12" style="text-align: center; padding: 15px;">
                                        <?php montaImgFoto($dadosAgenda['foto_arquivo_id_profissional'], $dadosAgenda['sexo_profissional'], '60px', '60px', '100', '100', 'bg-white img-circle') ?>
                                    </div>
                                </div>
                            </div>

                            <form id="form-avaliacao" name="form-avaliacao" method="post">
                                <div class="text-center">
                                    <div class="row">
                                        <h4>Como foi sua consulta no estabelecimento <strong><?= $dadosEstabelecimento['nome_fantasia'] ?></strong> com <strong><?= $dadosAgenda['profissional'] ?></strong> em <?= formata_data($dadosAgenda['data_inicio']) ?>?</h4>
                                        <div class="stars">
                                            <fieldset class="rating">
                                                <input type="radio" class="star" id="star3" name="rating" value="3" /><label class="star-label full" for="star3"></label>
                                                <input type="radio" class="star" id="star2" name="rating" value="2" /><label class="star-label full" for="star2"></label>
                                                <input type="radio" class="star" id="star1" name="rating" value="1" /><label class="star-label full" for="star1"></label>
                                            </fieldset>
                                        </div>
                                    </div>
                                    <div class="row">
                                        &nbsp;<label class="descricao-estrela"></label>&nbsp;
                                    </div>
                                    <div class="row avaliacao-melhorias" style="padding: 0 10px 0 10px">
                                        <h4>O que poderia melhorar?</h4>
                                        <div class="col-md-12">
                                            <div class="btn-group" data-toggle="buttons">
                                                <label class="btn btn-default" style="width: 100%"><input name="melhorias[]" type="checkbox" class="toggle" value="AM"> Atendimento médico </label>
                                            </div>
                                            <div class="btn-group" data-toggle="buttons">
                                                <label class="btn btn-default" style="width: 100%"><input name="melhorias[]" type="checkbox" class="toggle" value="AR"> Atendimento recepção </label>
                                            </div>
                                        </div>
                                        <div class="col-md-12">
                                            <div class="btn-group" data-toggle="buttons">
                                                <label class="btn btn-default" style="width: 100%"><input name="melhorias[]" type="checkbox" class="toggle"  value="MC"> Marcação </label>
                                            </div>
                                            <div class="btn-group" data-toggle="buttons">
                                                <label class="btn btn-default" style="width: 100%"><input name="melhorias[]" type="checkbox" class="toggle" value="TE"> Tempo de espera </label>
                                            </div>
                                            <div class="btn-group" data-toggle="buttons">
                                                <label class="btn btn-default" style="width: 100%"><input name="melhorias[]" type="checkbox" class="toggle" value="OT"> Outros </label>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="row avaliacao-comentario" style="text-align: left !important; margin-top: 10px;">
                                        <div class="col-md-12">
                                            <div class="form-group form-md-line-input has-success form-md-floating-label">
                                                <div class="input-group right-addon">
                                                    <input type="text" name="comentario" class="form-control" maxlength="120" placeholder="Deixe seu comentário">
                                                    <label for="form_control_1"></label>
                                                    <span class="help-block">Deixe seu comentário</span>
                                                    <span class="input-group-addon qtd-caracteres-comentario">0/120</span>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="form-actions">
                                        <input type="hidden" name="act" value="salvar">
                                        <input type="submit" class="btn green-jungle" value="Enviar">
                                    </div>
                                </div>
                            </form>
                        </div>
                    </div>
                <? else: ?>
                    <div class="alert alert-<?= $tipoValidacao; ?>">
                        <?= $mensagemAvaliacao ?>
                    </div>
                <? endif; ?>
            </div>
        </div>
    </div>
    <!--[if lt IE 9]>
    <script src="/includes/metronic/global/plugins/respond.min.js"></script>
    <script src="/includes/metronic/global/plugins/excanvas.min.js"></script>
    <![endif]-->
    <!-- BEGIN CORE PLUGINS -->
    <script src="/includes/metronic/global/plugins/jquery.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/js.cookie.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-hover-dropdown/bootstrap-hover-dropdown.min.js"
            type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-slimscroll/jquery.slimscroll.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery.blockui.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-switch/js/bootstrap-switch.min.js" type="text/javascript"></script>
    <!-- END CORE PLUGINS -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <script src="/includes/metronic/global/plugins/jquery-validation/js/jquery.validate.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-validation/js/additional-methods.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/select2/js/select2.full.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-inputmask/jquery.inputmask.bundle.min.js" type="text/javascript"></script>
    <script src="/includes/funcoes.js" type="text/javascript"></script>
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN PAGE LEVEL SCRIPTS -->
    <script type="text/javascript">
        $(function(){
            $("#star3").attr('checked', true);
            defineTextoDescricaoEstrela($("#star3").val());

            $('[name=comentario]').keyup(function(){
                $('.qtd-caracteres-comentario').html($(this).val().length + '/' + $(this).prop('maxlength'))
            });

            // $('.star, .star-label').hover(function(){
            //     var valor = new Number($('#' + $(this).prop('for')).val());
            //     defineTextoDescricaoEstrela(valor);
            // });

            $('.star').click(function(){
                var valor = $(this).val();
                defineTextoDescricaoEstrela(valor);
            });
        });

        function defineTextoDescricaoEstrela(valor){
            var texto = "";
            $('.avaliacao-comentario [name=comentario]').prop('placeholder','Deixe seu comentário');
            $('.avaliacao-comentario .help-block').html('Deixe seu comentário');
            valor = parseInt(valor);
            switch (valor) {
                case 1:
                    texto = "<span style='color:#e7505a;'>Ruim</span>";
                    $('.avaliacao-melhorias').fadeIn();
                    $('.avaliacao-comentario').fadeIn();

                    break;
                case 2:
                    texto = "<span style='color:#3598dc;'>Bom</span>";
                    $('.avaliacao-melhorias').fadeIn();
                    $('.avaliacao-comentario').fadeIn();
                    break;
                case 3:
                    texto = "<span style='color:#26C281;'>Ótimo</span>";
                    $('.avaliacao-melhorias').fadeOut();
                    $('.avaliacao-comentario').fadeIn();

                    $('.avaliacao-comentario [name=comentario]').prop('placeholder','Quer fazer um elogio?');
                    $('.avaliacao-comentario .help-block').html('Quer fazer um elogio?');
                    break;
            }

            $('.descricao-estrela').html(texto);
        }
    </script>
    <!-- END PAGE LEVEL SCRIPTS -->
    <!-- BEGIN THEME LAYOUT SCRIPTS -->
    <!-- END THEME LAYOUT SCRIPTS -->
</body>
</html>