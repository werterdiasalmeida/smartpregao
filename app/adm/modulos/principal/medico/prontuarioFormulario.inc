<?php


$params = getParam();
$sigla = $_REQUEST['tipo'];
$mTipo = new TipoProntuario();
$tipo = $mTipo->getPorSigla($sigla);
$estiloTipo = TipoProntuario::getEstilo();
$estabelecimentoId = $params['estabelecimento_id'];
if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'salvarProntuario' :

            $paramsPost = utf8ArrayDecode($_POST);
            $params = array_merge($paramsPost, getParam());

            $params['descricao'] = str_replace('\"', '"', $params['descricao']);

            unset($params['id']);
            $params['id'] = $params['prontuario_id'];
            $msgSucesso = $params['id'] ? 'Registro alterado no prontuário' : 'Registro adicionado ao prontuário';

            $mProntuario = new Prontuario();
            $params['descricao'] = stripslashes($params['descricao']);
            $prontuarioId = $mProntuario->salvarProntuario($params);
            $db->commit();
            $base64Pdf = '';
            if ($params['assinar_digitalmente'] == 't') {
                include_once APPRAIZ . "adm/servicos/ProntuarioServico.class.inc";
                $prontuarioServico = new ProntuarioServico();
                $base64Pdf = $prontuarioServico->getProntuarioImpressaoFromId($prontuarioId, true)->getBase64();
            }
            global $url_servidor_assinatura;

            $db->sucessomAjax('', array('p' => setParam(array(
                'pessoa_paciente_id' => $params['pessoa_paciente_id'],
                'agenda_id' => $params['agenda_id'],
                'estabelecimento_id' => $params['estabelecimento_id'],
                'prontuario_id' => $prontuarioId,
            ), false),
                'prontuario_id' => $prontuarioId,
                'base64Pdf' => $base64Pdf,
                'receituarioEspecial' => $paramsPost['receituario_especial'] == 't',
                'urlServidorAssinatura' => $url_servidor_assinatura), $msgSucesso);
            die;
            break;
        case 'salvarAnexo' :
            $_FILES = utf8ArrayDecode($_FILES);
            $fileSimec = new FilesSimec();
            $arqid = $fileSimec->setUploadArquivoEspecifico('', 'anexo');
            $db->commit();
            $db->sucessomAjax('', array('arquivo_id' => $arqid));
            die;
            break;
        case 'downloadAnexo' :
            $fileSimec = new FilesSimec();
            $fileSimec->getDownloadArquivo($_REQUEST['arquivo_id']);
            die;
            break;
        case 'filtrarCIDCombo' :
            $busca = $_REQUEST['busca'];
            $mCid10 = new VwCid10();

            die($mCid10->getListByCampoDeBusca($busca));
            break;
    }
}
$anexos = array();
$cids = array();
if ($params['prontuario_id']) {
    $mProntuario = new Prontuario();
    $dadosProntuario = $mProntuario->getTodosDados($params['prontuario_id']);
    extract($dadosProntuario);

    $mAnexo = new AnexoProntuario();
    $anexos = $mAnexo->listarAgrupadosProntuarioPorPessoa($params['pessoa_paciente_id'], array($params['prontuario_id']));

    $mProntuarioCid = new ProntuarioCid();
    $cids = $mProntuarioCid->getCidsPorProntuario($params['prontuario_id']);

    $tipo = $mTipo->getPorSigla($dadosProntuario['sigla']);
    $estabelecimentoId = $dadosProntuario['estabelecimento_id'];
}

$isPrescricao = $sigla == TipoProntuario::PRESCRICAO;
$classePrescricao = $isPrescricao ? 'memed-autocomplete' : '';

$mPessoa = new PessoaFisica();
$dadosPaciente = $mPessoa->getTodosDados($params['pessoa_paciente_id']);

$receituario_especial = $receituario_especial ?: 'f';
$assinar_digitalmente = $assinar_digitalmente ?: 'f';
?>
<script src="/includes/hwcrypto/hwcrypto-legacy.js"></script>
<script src="/includes/hwcrypto/hwcrypto.js"></script>
<script src="/includes/hwcrypto/hex2base.js"></script>
<script src="/includes/assinatura/assinatura.js"></script>
<div id="prontuario-formulario">
    <div id="prontuario">
        <input type="hidden" name="p" value="<?= setParam(getParam(), false); ?>"/>
        <input type="hidden" name="tipo" value="<?= $sigla; ?>"/>
        <input type="hidden" name="act" value="salvarProntuario"/>
        <input type="hidden" name="modelo_documento_id"/>

        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
            <h4 class="modal-title">
                <i class="<?= $estiloTipo[$sigla]['estilo']['classe_icone'] ?>"></i>
                Incluir <?= $tipo['descricao'] ?>
            </h4>
        </div>
        <div class="modal-body">
            <div class="row">
                <div class="col-xs-12 col-sm-12 col-md-12 col-lg-2" <?php echo $_REQUEST['jitsi_video_iframe'] ? 'style="display:none"' : ''  ?> >
                    <div class="row">
                        <div class="col-sm-12">
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
                                    <div class="text-center" style="padding: 10px;">
                                        <?= $mTag->getBadgeTagsPorPaciente($dadosPaciente['id'], ' '); ?>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="col-xs-12 col-sm-12 col-md-12 col-lg-8">
                    <div class="col-sm-12 col-md-12 col-lg-12">
                        <div class="form-group">
                            <label>Descrição</label>
                            <?= campo_textaream('descricao', 'N', 'S', '', '10', '13', '10000', '', 0, '', false, NULL, null, $classePrescricao, 'data-memed-categorias="composicoes,exames"') ?>
                        </div>
                    </div>
                    <div class="col-xs-12">
                        <div class="form-group">
                            <label class="control-label">CID</label>
                            <textarea id="cids" name="cids" multiple style="display: none;"></textarea>
                        </div>
                    </div>
                    <div class="row" style="margin: 0; padding: 0;">
                        <? if ($isPrescricao): ?>
                            <div class="col-xs-12 col-md-3">
                                <div class="form-group" style="margin-bottom: 0">
                                    <label>Controle especial</label>
                                    <div class="form-select-content" style="margin-bottom: 0">
                                        <?php
                                        $db->monta_radiom('receituario_especial', array(
                                            array('codigo' => 't', 'descricao' => 'Sim'),
                                            array('codigo' => 'f', 'descricao' => 'Não')
                                        ), 'S', 'N')
                                        ?>
                                    </div>
                                </div>
                                <br/>
                            </div>
                        <? endif; ?>
                        <div class="col-xs-12 col-md-3">
                            <div class="form-group" style="margin-left: 10px; margin-bottom: 0">
                                <label>Assinar digitalmente</label>
                                <div class="form-select-content" style="margin-bottom: 0">
                                    <?php
                                    $db->monta_radiom('assinar_digitalmente', array(
                                        array('codigo' => 't', 'descricao' => 'Sim'),
                                        array('codigo' => 'f', 'descricao' => 'Não')
                                    ), 'S', 'N')
                                    ?>
                                </div>
                            </div>
                        </div>
                        <div class="col-xs-12">
                            <a href="javascript: tutorial()">
                                Clique aqui para obter os recursos necessários para assinar digitalmente seus registros.
                            </a>
                        </div>
                    </div>


                </div>
                <div class="col-xs-12 col-sm-12 col-md-12 col-lg-<?php echo $_REQUEST['jitsi_video_iframe'] ? '3' : '2'  ?>">
                    <div class="row">
                        <div class="col-sm-12">

                            <div class="mt-element-list">
                                <div class="mt-list-head list-simple font-white bg-blue">
                                    <div class="list-head-title-container">
                                        <h4 class="list-title">Modelos</h4>
                                    </div>
                                </div>
                                <div class="mt-list-container list-simple" style="height: 327px; overflow: auto;">
                                    <div class="margin-bottom-20">
                                        <?= campo_textom('filtro_modelos', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', '', '', null, null, 'Pesquisar Modelos'); ?>
                                    </div>
                                    <?php
                                    $mModeloDoc = new ModeloDocumento();
                                    $modelos = $mModeloDoc->getComboPorEstabelecimento($estabelecimentoId, $tipo['id'], $params['agenda_id']);
                                    $jsonCompletoModelos = json_encode(utf8ArrayEncode($modelos));

                                    $modelosJson = $mModeloDoc->getModelosAssociativo($modelos, true);
                                    if (count($modelos) == 0):
                                        ?>
                                        <h5 style="text-align: center;">Nenhum modelo encontrado.</h5>
                                    <?php
                                    endif;

                                    foreach ($modelos as $modelo) :
                                        $dscTooltip = strlen($modelo['modelo']) > 250 ? substr($modelo['modelo'], 0, 250) . "..." : $modelo['modelo'];
                                        ?>
                                        <ul class="ul-modelo-<?= $modelo['codigo'] ?>">
                                            <li class="mt-list-item">
                                                <div style="border-bottom: 1px solid #e7ecf1; padding-bottom: 5px; margin-bottom: 10px;"
                                                     data-toggle="popover" title="Descrição do Modelo"
                                                     data-content="<?= strip_tags($dscTooltip) ?>" data-trigger="hover"
                                                     data-placement="bottom">
                                                    <a href="javascript:;"
                                                       onclick="carregarModelo(<?= $modelo['codigo']; ?>)">
                                                        <i class="fa fa-arrow-left hidden-xs hidden-sm hidden-md"
                                                           aria-hidden="true"
                                                           style="margin-left:-15px; color: #DDDDDD;"></i>
                                                        <i class="fa fa-arrow-up hidden-lg" aria-hidden="true"
                                                           style="margin-left:-15px; color: #DDDDDD;"></i>
                                                        <?= $modelo['descricao']; ?>
                                                    </a>
                                                </div>
                                            </li>
                                        </ul>
                                    <?php endforeach; ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="row">
                <div class="col-sm-12">
                    <div class="form-group">
                        <label>Anexos</label>
                        <div id="file-form-anexo" class="dropzone text-center">
                            <div class="fallback">
                                <input type="file" id="anexo" name="anexo" class="form-control"/>
                            </div>
                            <div>
                                <div style="font-size: 18px; margin-bottom: 4px;">
                                    Arraste os arquivos para cá
                                </div>
                                <div style="font-size: 18px; margin-bottom: 4px;">
                                    ou
                                </div>
                                <button type="button" id="btn-anexo" class="btn green">Selecione os arquivos do seu
                                    computador
                                </button>
                            </div>

                            <?php
                            $fileSimec = new FilesSimec();
                            $anexos = isset($anexos[$params['prontuario_id']]) ? $anexos[$params['prontuario_id']] : array();
                            foreach ($anexos as $anexo) :
                                $tamanhoArquivo = formatarTamanhoArquivo($anexo['arqtamanho']);
                                list($tamanho, $escala) = explode(' ', $tamanhoArquivo);

                                $isImagem = isImagem($anexo['arqextensao']);
                                ?>
                                <div class="dz-preview <?= ($isImagem ? 'dz-image-preview' : 'dz-file-preview') ?> dz-processing dz-success dz-complete arquivo_<?= $anexo['arquivo_id'] ?>">
                                    <?php if ($isImagem) :
                                        $caminhoArquivo = $fileSimec->getCaminhodArquivo($anexo['arquivo_id']);
                                        $base64Imagem = base64_encode(file_get_contents($caminhoArquivo));
                                        ?>
                                        <div class="dz-image"><img
                                                    src="../slideshow/slideshow/verimagem.php?arqid=<?= $anexo['arquivo_id'] ?>&newwidth=300"
                                                    alt="<?= "{$anexo['arqnome']}.{$anexo['arqextensao']}" ?>"></div>
                                    <?php else : ?>
                                        <div class="dz-image"><img data-dz-thumbnail="" alt=""></div>
                                    <?php endif; ?>

                                    <div class="dz-details">
                                        <div class="dz-size"><span
                                                    data-dz-size=""><strong><?= $tamanho ?></strong> <?= $escala ?></span>
                                        </div>
                                        <div class="dz-filename"><span
                                                    data-dz-name=""><?= "{$anexo['arqnome']}.{$anexo['arqextensao']}" ?></span>
                                        </div>
                                    </div>
                                    <div class="dz-progress">
                                        <span class="dz-upload" data-dz-uploadprogress="" style="width: 100%;"></span>
                                    </div>
                                    <div class="dz-error-message">
                                        <span data-dz-errormessage=""></span>
                                    </div>
                                    <div class="dz-error-mark">
                                    </div>
                                    <a href="javascript:;" class="btn red btn-sm btn-block btn-remover">Remover</a>
                                    <a href="javascript:;" class="btn blue btn-sm btn-block btn-download">Download</a>
                                    <input type="hidden" name="arquivo_id[]" class="arquivo"
                                           value="<?= $anexo['arquivo_id'] ?>"/>
                                    <script type="text/javascript">
                                        $(function () {
                                            var container = $('.arquivo_<?= $anexo['arquivo_id'] ?>');
                                            container.find('.btn-download').click(function () {
                                                downloadAnexo(this);
                                            });
                                            container.find('.btn-remover').click(function () {
                                                $(this).closest('.dz-preview').remove();
                                            });
                                        });
                                    </script>
                                </div>
                            <?php endforeach; ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <div class="modal-footer">
            <button type="submit" class="btn blue btn-incluir">
                <i class="fa fa-save"></i>
                Salvar
            </button>
            <button type="button" class="btn" data-dismiss="modal">Fechar</button>
        </div>
    </div>
    <div id="tutorial">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
            <h4 class="modal-title">
                <i class="fa fa-lock"></i>
                Como assinar digitalmente os meus registros?
            </h4>
        </div>
        <div class="modal-body">
            <p class="text-assinatura" style="text-align: center; font-size: 20px; font-weight: 500;">
                Para o funcionamento correto do processo de assinatura digital,
                precisamos que você antes instale alguns recursos.
            </p>
            <div class="row">

                <div class="col-md-2">

                    <img style="width: 100%;"
                         src="../imagens/tutorial/assinatura.png"/>
                </div>
                <div class="col-md-10">
                    <ul style=" margin-top: 2%; list-style: decimal">
                        <li class="text-assinatura">
                            O Java deve estar devidamente instalado e atualizado na sua máquina. Você pode obtê-lo em:
                            <a target="_blank" href="https://www.java.com/pt-BR/">https://www.java.com/pt-BR/</a>.
                        </li>
                        <li class="text-assinatura">
                            Instale o assinador digital que garantirá a segurança da comunicação
                            entre o Smart Pregão e o seu certificado.
                            Você pode fazer isso executando o instalador disponível neste
                            <a target="_blank" href="../imagens/tutorial/open-eid.exe">link.</a>
                        </li>
                        <li class="text-assinatura">
                            Garanta agora que o seu token está devidamente inserido e configurado no seu sistema operacional.
                            Isso pode ser feito através do site da sua certificadora.
                        </li>
                        <li class="text-assinatura">
                            Caso tudo esteja devidamente configurado você pode testar
                            <a href="javascript: testaCertificado()">clicando aqui</a>
                        </li>
                    </ul>
                </div>
            </div>

        </div>
        <div class="modal-footer">
            <button type="button" onclick="fimTutorial()" class="btn">Voltar</button>
        </div>
    </div>

</div>


<style type="text/css">
    .dropzone {
        min-height: 0;
        border: 1px solid #c2c6c8;
    }

    .dropzone .dz-message {
        margin: 0;
    }

    .text-assinatura {
        margin-bottom: 20px;
        font-weight: 400;
        font-size: 16px;
    }
</style>

<!--<script type="text/javascript" src="../includes/tinymce_4.7/tinymce.min.js"></script>-->
<script type="text/javascript" src="../includes/underscore/underscore-min.js"></script>
<script type="text/javascript">
    var autosave = null;
    var selectizeCid = null;
    var jsonModelos = <?= $jsonCompletoModelos ?>;
    var etapas = [0, 1, 2, 3];

    $(function () {
        adicionarLabelObrigatorio($('#prontuario-formulario :required'));

        $("#filtro_modelos").keyup(function () {
            filtrarModelos($(this).val());
        });

        $('[class^=ul-modelo-]').hover(function () {
            $(this).find('[class^="fa fa-arrow-"]').css('color', '#337ab7');
        }, function () {
            $(this).find('[class^="fa fa-arrow-"]').css('color', '#DDDDDD');
        });

        var formProntuario = $("#form-prontuario");
        formProntuario.validateForm({
            rules: {},
            submitHandler: function (e) {
                cancelarAutosave();
                enviarForm();
            }
        });
        formProntuario.find('[data-toggle="tooltip"]').tooltip();
        formProntuario.find('[data-toggle="popover"]').popover();

        $("#prontuario-formulario #file-form-anexo").dropzone({
            url: window.location.href,
            params: {
                act: 'salvarAnexo'
            },
            paramName: 'anexo',
            clickable: '#btn-anexo',
            dictDefaultMessage: '',
            init: function () {
                this.on("addedfile", function (file) {
                    // Create the remove button
                    var removeButton = Dropzone.createElement("<a href='javascript:;' class='btn red btn-sm btn-block'>Remover</a>");
                    var downloadButton = Dropzone.createElement("<a href='javascript:;' class='btn blue btn-sm btn-block'>Download</a>");

                    // Capture the Dropzone instance as closure.
                    var _this = this;

                    // Listen to the click event
                    removeButton.addEventListener("click", function (e) {
                        // Make sure the button click doesn't submit the form:
                        e.preventDefault();
                        e.stopPropagation();
                        // Remove the file preview.
                        _this.removeFile(file);
                        // If you want to the delete the file on the server as well,
                        // you can do the AJAX request here.
                    });

                    // Listen to the click event
                    downloadButton.addEventListener("click", function (e) {
                        downloadAnexo(this);
                    });

                    // Add the button to the file preview element.
                    file.previewElement.appendChild(removeButton);
                    file.previewElement.appendChild(downloadButton);
                });

                this.on("dragover", function (file) {
                    $('#file-form-anexo').css('background-color', '#DDDDDD');
                });

                var removerFundoDropzone = function (file) {
                    $('#file-form-anexo').css('background-color', 'transparent');
                };

                this.on("dragleave", removerFundoDropzone);
                this.on("drop", removerFundoDropzone);

                this.on("success", function (file, response) {
                    try {
                        var responseObj = JSON.parse(response);
                        var hidden = Dropzone.createElement('<input type="hidden" name="arquivo_id[]" class="arquivo" value="' + responseObj.params.arquivo_id + '" />');
                        file.previewElement.appendChild(hidden);
                    } catch (e) {
                        exibirAlert('Não foi possível enviar o arquivo.');
                        console.error(e);
                        this.removeFile(file);
                    }
                });
            },
            accept: function (file, done) {
                done();
            }
        });

        $('#modal-prontuario').on('hidden.bs.modal', function () {
            cancelarAutosave();
        });

        var selectCid = $('#cids').selectize({
            valueField: 'codigo',
            placeholder: "Selecione um ou mais CID's",
            labelField: 'codigo_formatado',
            searchField: ['descricao', 'descricao_abreviada', 'codigo', 'codigo_formatado'],
            plugins: ['remove_button'],
            options: [],
            render: {
                option: function (item, escape) {
                    return '<div>' +
                        '<div class="col-sm-2 col-md-2 col-lg-1">' +
                        '<h3><label class="italic">' + item.codigo_formatado + '</label></h3>' +
                        '</div>' +
                        '<div class="col-sm-10 col-md-10 col-lg-11">' +
                        '<label class="bold">Código: </label><span> ' + item.codigo_formatado + '</span><br />' +
                        '<label class="bold">Descrição: </label><span> ' + item.descricao + '</span><br />' +
                        '<label class="bold">Descrição abreviada: </label><span> ' + item.descricao_abreviada + '</span>' +
                        '</div>' +
                        '</div>';
                }
            },
            load: function (query, callback) {
                if (query.length < 2) return callback();

                $.ajax({
                    url: window.location.href,
                    type: 'POST',
                    data: {
                        act: 'filtrarCIDCombo',
                        busca: (query)
                    },
                    dataType: 'json',
                    error: function () {
                        callback();
                    },
                    success: function (res) {
                        callback(res.data);
                    }
                });
            }
        });
        selectizeCid = selectCid[0].selectize;

        <? foreach ($cids as $cid): ?>
        // Preenchendo os itens já salvos no banco
        selectizeCid.addOption({
            codigo: '<?= $cid['codigo'] ?>',
            codigo_formatado: '<?= $cid['codigo_formatado'] ?>',
            descricao: '<?= $cid['descricao'] ?>',
            descricao_abreviada: '<?= $cid['descricao_abreviada'] ?>'
        });
        selectizeCid.addItem('<?= $cid['codigo'] ?>');
        <? endforeach; ?>

        setProntuarioAutoSave();
        fimTutorial();
    });

    function cancelarAutosave() {
        clearInterval(autosave);
        autosave = null;
    }

    function downloadAnexo(downloadButton) {
        var campoArquivo = $(downloadButton).closest('.dz-preview').find('.arquivo');
        if (campoArquivo && campoArquivo.val()) {
            window.location.href = window.location + '&act=downloadAnexo&arquivo_id=' + campoArquivo.val();
        } else {
            exibirAviso('Nenhum arquivo encontrado.');
        }
    }

    function getModelo(id) {
        var modelos = <?= $modelosJson ?>;
        return modelos[id] ? modelos[id] : '';
    }

    function filtrarModelos(texto) {
        if (!texto || texto.length == 0 || texto == '') {
            $('[class^=ul-modelo]').show();
            return false;
        }

        if (jsonModelos) {
            $.each(jsonModelos, function (index, value) {
                if (value.descricao.toLocaleLowerCase().indexOf(texto.toLocaleLowerCase()) != -1 || value.modelo.toLocaleLowerCase().indexOf(texto.toLocaleLowerCase()) != -1) {
                    $('.ul-modelo-' + value.codigo).show();
                } else {
                    $('.ul-modelo-' + value.codigo).hide();
                }
            });
        }
    }

    function setProntuarioAutoSave() {
        autosave = setInterval(function () {
            enviarForm();
        }, 30000);
    }

    function carregarModelo(id) {
        var modelo = getModelo(id);
        $('#prontuario-formulario [name=modelo_documento_id]').val(id);
        $('#prontuario-formulario [name=descricao]').val(modelo);
        // tinyMCE.activeEditor.setContent(modelo);
    }

    function enviarForm() {
        var autosaveExecutando = autosave !== null;
        var formProntuario = $('#form-prontuario');

        var descricao = formProntuario.find('[name=descricao]');
        var arquivos = formProntuario.find('.arquivo');

        if (descricao.val() === '' && arquivos.length === 0) {
            if (!autosaveExecutando) {
                exibirAviso('Por favor, informe a descrição ou inclua pelo menos um anexo.');
            }
            return;
        }

        if (!autosaveExecutando) {
            App.blockUI({
                message: 'Carregando...',
                target: '#prontuario-formulario'
            });
        }

        var form = formProntuario[0];
        var formData = new FormData(form);
        $.ajax({
            url: window.location.href,
            type: 'POST',
            data: formData,
            dataType: 'json',
            success: function (data) {
                if (!autosaveExecutando) {
                    exibirSucesso(data.msg);
                    App.unblockUI('#prontuario-formulario');

                    if (data.params.base64Pdf !== '') {
                        console.log(data);
                        assinatura.assinar(
                            data.params.base64Pdf,
                            data.params.urlServidorAssinatura,
                            data.params.receituarioEspecial ? 2 : 1,
                            function (response) {
                                exibirSucesso('Prescrição assinada com sucesso!');
                                salvarArquivoAssinado(response.signedBase64, data.params);
                            },
                            function (error) {
                                App.unblockUI('#prontuario-formulario');
                                $('#modal-prontuario').modal('hide');
                                exibirAlert('Falha ao assinar documento.');
                                $('#modal-prontuario').modal('hide');
                            }
                        );
                    } else {
                        $('#modal-prontuario').modal('hide');
                    }
                } else {
                    formProntuario.find('input[type=hidden][name=p]').val(data.params.p);
                    $.bootstrapGrowl('<i class="fa fa-check"></i> Informação incluída automaticamente no prontuário.', {
                        ele: 'body', // which element to append to
                        type: 'success', // (null, 'info', 'danger', 'success', 'warning')
                        offset: {
                            from: 'bottom',
                            amount: 60
                        }, // 'top', or 'bottom'
                        align: 'right', // ('left', 'right', or 'center')
                        width: 'auto', // (integer, or 'auto')
                        delay: 2000, // Time while the message will be displayed. It's not equivalent to the *demo* timeOut!
                        allow_dismiss: true, // If true then will display a cross to close the popup.
                        stackup_spacing: 10 // spacing between consecutively stacked growls.
                    });
                }
                atualizarListaProntuario();
            },
            error: function (data) {
                if (!autosaveExecutando) {
                    exibirAlert('Não foi possível incluir as informações no prontuário');
                    App.unblockUI('#prontuario-formulario');
                } else {
                    $.bootstrapGrowl('<i class="fa fa-times"></i> Não foi possível incluir automaticamente as informações no prontuário', {
                        ele: 'body', // which element to append to
                        type: 'danger', // (null, 'info', 'danger', 'success', 'warning')
                        offset: {
                            from: 'bottom',
                            amount: 60
                        }, // 'top', or 'bottom'
                        align: 'right', // ('left', 'right', or 'center')
                        width: 'auto', // (integer, or 'auto')
                        delay: 2000, // Time while the message will be displayed. It's not equivalent to the *demo* timeOut!
                        allow_dismiss: true, // If true then will display a cross to close the popup.
                        stackup_spacing: 10 // spacing between consecutively stacked growls.
                    });
                }
            },
            cache: false,
            contentType: false,
            processData: false,
            xhr: function () {  // Custom XMLHttpRequest
                var myXhr = $.ajaxSettings.xhr();
                if (myXhr.upload) { // Avalia se tem suporte a propriedade upload
                    myXhr.upload.addEventListener('progress', function () {
                        /* faz alguma coisa durante o progresso do upload */
                    }, false);
                }

                return myXhr;
            }
        });
    }

    function salvarArquivoAssinado(pdfAssinadoBase64, entidade) {
        $.ajax({
            url: window.location.href,
            type: 'POST',
            data: {
                pdfAssinadoBase64: pdfAssinadoBase64,
                entidade: entidade,
                act: 'salvarAssinado'
            },
            dataType: 'json',
            success: function (data) {
                App.unblockUI('#prontuario-formulario');
                $('#modal-prontuario').modal('hide');
                window.location.reload();
            },
            error: function (error) {
                App.unblockUI('#prontuario-formulario');
                $('#modal-prontuario').modal('hide');
                exibirAlert('Falha ao assinar arquivo.');

            }
        });
    }

    function tutorial() {
        $('#prontuario').hide();
        $('#tutorial').show();
    }

    function fimTutorial() {
        $('#prontuario').show();
        $('#tutorial').hide();
        _hideFinish();
    }

    function _hideFinish() {
        $('#tutorial-finish').hide();
        $('#tutorial-next').show();
    }

    function _showFinish() {
        $('#tutorial-finish').show();
        $('#tutorial-next').hide();
    }
    function testaCertificado() {

        assinatura.testCertificate(
            function () {
                exibirSucesso('Sucesso, você já pode assinar digitalmente os seus registros!');
            },
            function (err) {
                if (!err || !err.message) {
                    exibirAlert('Falha ao obter certificado, verifique se todos os requisitos estão devidamente instalados.');
                    return;
                }
                if (err.message == 'no_certificates') {
                    exibirAlert('Nenhum certificado instalado. Verifique se seu token está devidamente configurado.');
                    return;
                }
                if (err.message == 'no_implementation') {
                    exibirAlert('A extensão necessária ainda não foi devidamente instalada. ' +
                        'Caso já tenha executado o instalador, reinicie o seu navegador.');
                    return;
                }
                exibirAlert('Falha ao obter certificado, verifique se todos os requisitos estão devidamente instalados.');
            }
        );
    }

</script>