<?php
$mPessoaFisica = new PessoaFisica();
$mDocumentos = new AnexoPessoaFisica();

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'salvar' :
            try {
                $params = array_merge($_POST, getParam());
                $id = $mPessoaFisica->salvarPessoaFisica($params, $_FILES);
                $mDocumentos->salvarDocumentosPessoa($params, $id);
                $mPessoaFisica->commit();

                $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $id)));
            }catch (CpfIndisponivel $e) {
                addMsgDanger($e->getMessage());
                extract($_POST);
            }
            break;
        case 'carregarMunicipios':
            require APPRAIZ . "adm/modulos/principal/includes/endereco/modal.inc";
            break;
        case 'verificarCpf':
            require APPRAIZ . "adm/modulos/principal/includes/pessoafisica/formulario.inc";
            break;
        case 'enviarArquivoDocumento':
            $arquivoId = null;
            $p = getParam();
            $_FILES = utf8ArrayDecode($_FILES);

            if (isset($_FILES['documento'])
            && isset($_FILES['documento']['name']) && !empty($_FILES['documento']['name'])
            && isset($_FILES['documento']['type']) && !empty($_FILES['documento']['type'])
            && isset($_FILES['documento']['size']) && !empty($_FILES['documento']['size'])) {
                $file = new FilesSimec();
                $arquivoId = $file->setUploadArquivoEspecifico("Documento da pessoa", 'documento');
                $db->commit();
            }

            if($p['id']){
                $mDocumentos->salvarDocumentosPessoa(array(
                    "arquivo_{$_REQUEST['tipo']}" => $arquivoId
                ), $p['id']);
                $mDocumentos->commit();
            }

            $db->sucessomAjax('', array('arqid' => $arquivoId));
            die;
            break;
        case 'downloadArquivoDocumento':
            $file = new FilesSimec();
            $file->getDownloadArquivo($_REQUEST['arqid']);
            die;
            break;
        case 'carregarConveniosPorEstabelecimento':
            $estabelecimentoId = $_REQUEST['estabelecimento_id'];
            if($estabelecimentoId) {
                $mConvenio = new ConvenioEstabelecimento();
                $convenios = $mConvenio->getComboPorEstabelecimento($estabelecimentoId);

                $db->monta_combom('convenio_estabelecimento_id', $convenios, 'S', 'Selecione...', '', '', '', '', 'S');
            } else {
                $db->monta_combom('convenio_estabelecimento_id', [], 'N', 'Selecione...', '', '', '', '', 'S');
            }

            die('
            <script type="text/javascript">
                $(function(){
                    $("[name=convenio_estabelecimento_id]").selectpicker();
                });
            </script>');
            break;
    }
}

$params = getParam();
if (isset($params['id']) && !empty($params['id'])) {
    $mPessoaFisica->carregarPorId($params['id']);
    $dados = $mPessoaFisica->getDados();
    extract($dados);

    $mEnderecoPessoaFisica = new EnderecoPessoaFisica();
    $enderecos = $mEnderecoPessoaFisica->listarPorPessoaFisica($params['id'], true);

    $mContatoPessoaFisica = new ContatoPessoaFisica();
    $contatos = $mContatoPessoaFisica->listarEspecificosPorPessoaFisica($params['id']);
    extract($contatos);

    $mConvenios = new ConvenioEstabelecimentoPaciente();
    $conveniosPlanos = $mConvenios->getConveniosByPessoa($params['id'], true);

    $documentoCns = $mDocumentos->getUltimoBySiglaPessoa(TipoAnexo::TIPO_CNS, $params['id']);
    $arquivo_cns = $documentoCns['arquivo_id'];

    $documentoCpf = $mDocumentos->getUltimoBySiglaPessoa(TipoAnexo::TIPO_CPF, $params['id']);
    $arquivo_cpf = $documentoCpf['arquivo_id'];
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Insira os dados da pessoa');

if (isset($params['id']) && !empty($params['id'])) {
    UsuarioResponsabilidadeEstabelecimento::criaAbam($abacod_tela, $url, setParam(getParam()), array(), true);
}
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-user-following font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações da pessoa
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="pessoafisica-form" method="POST" enctype="multipart/form-data">
            <div class="form-body">
                <?php
                $incluirAnexoDocumentos = true;
                require APPRAIZ . "adm/modulos/principal/includes/pessoafisica/formulario.inc";
                ?>

                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Cartão nacional de saúde</label>
                            <div class="input-group">
                                <?= campo_textom('cartao_nacional_saude', 'N', 'S', '', 50, 40, '', '', '', '', 0); ?>
                                <input type="hidden" name="arquivo_cns" value="<?= $arquivo_cns ?>" />
                                <span class="input-group-btn">
                                    <button class="btn btn-icon-only btn-default blue btn-anexo-documento btn-anexo-cns" data-type="cns" type="button" title="Enviar arquivo">
                                        <i class="icon-paper-clip"></i>
                                    </button>
                                </span>
                            </div>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Nome da mãe</label>
                            <?= campo_textom('nome_mae', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>

                <h4 class="form-section">Contatos</h4>
                <?php require APPRAIZ . "adm/modulos/principal/includes/contato/formulario.inc"; ?>

                <h4 class="form-section">Endereços</h4>
                <?php require APPRAIZ . "adm/modulos/principal/includes/endereco/lista.inc"; ?>

                <h4 class="form-section">Convênios / planos</h4>
                <div class="row">
                    <div class="col-md-12">
                        <a class="btn green btn-incluir-conveio-plano">
                            <i class="fa fa-plus-circle"></i>
                            Incluir convênio / plano
                        </a>
                        <br />
                        <br />
                        <table id="tabela-convenio-plano" data-toggle="table" class="table table-striped" data-unique-id="id">
                            <thead>
                            <tr>
                                <th data-field="estabelecimento">Estabelecimento</th>
                                <th data-field="convenio_plano">Convênio / plano</th>
                                <th data-field="numero">Número</th>
                                <th data-field="validade">Validade</th>
                                <th data-formatter="formatAcaoConvenioPlano" data-align="center">Ação</th>
                            </tr>
                            </thead>
                            <tbody>

                            </tbody>
                        </table>
                    </div>
                </div>
                <input type="hidden" name="convenios_planos" value="<?= $conveniosPlanos ?>"/>

                <input type="hidden" name="act" value="salvar"/>
            </div>

            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
            </div>
        </form>
    </div>
</div>

<?php require APPRAIZ . "adm/modulos/principal/includes/endereco/modal.inc"; ?>

<div class="modal fade in" id="modal-convenio-plano" tabindex="-1" role="modal-convenio-plano" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <form role="form" name="convenio-plano-form" class="convenio-plano-form" method="POST" enctype="multipart/form-data">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Convênio / plano</h4>
                </div>
                <div class="modal-body">
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Estabelecimento</label>
                                <?php
                                $mEstabelecimento = new Estabelecimento();
                                $estabelecimentos = $mEstabelecimento->getComboPorResponsabilidade();
                                $db->monta_combom('estabelecimento_id', $estabelecimentos, 'S', 'Selecione...', '', '', '', '', 'S');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Convênio / plano</label>
                                <div class="convenios_estabelecimento">
                                    <?php
                                    $db->monta_combom('convenio_estabelecimento_id', [], 'N', 'Selecione...', '', '', '', '', 'S');
                                    ?>
                                </div>
                                <input type="hidden" name="convenio_estabelecimento_id_alterar" />
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Número do cartão</label>
                                <?= campo_textom('numero', 'S', 'S', '', 50, 40, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Validade</label>
                                <?= campo_datam('validade', 'N', 'S', '', 'S'); ?>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="submit" class="btn blue">
                        <i class="fa fa-plus-circle"></i>
                        Incluir
                    </button>
                    <button type="button" class="btn" data-dismiss="modal">Cancelar</button>
                </div>

                <input type="hidden" id="index" name="index" />
                <input type="hidden" id="id" name="id" />
            </form>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function () {
        $(".pessoafisica-form").validateForm({
            rules : {
                cpf: {cpf: true},
                foto: {accept: "image/*"},
                "contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]" : {email : true}
            },
            submitHandler: function (e) {
                var submit = this;
                verificarCpf($('#cpf').val(), function(){
                    submit.submitHandlerFile(e,$('input[type=file][name=foto]'));
                });
            },
            beforeSubmitForm: function (e) {
                var dadosEndereco = $('#tabela-enderecos').bootstrapTable("getData");
                $('input[type=hidden][name=enderecos]').val(JSON.stringify(dadosEndereco));

                var dadosConvenioPlano = $('#tabela-convenio-plano').bootstrapTable("getData");
                $('input[type=hidden][name=convenios_planos]').val(JSON.stringify(dadosConvenioPlano));
            }
        });

        $(".endereco-form").validateForm({
            submitHandler: function (e) {
                incluirEndereco();
            }
        });

        $('.btn-voltar').click(function () {
            window.location = '?modulo=principal/pessoafisica/lista&acao=C';
        });

        <?php if($conveniosPlanos) : ?>
            $('#tabela-convenio-plano').bootstrapTable('load', <?= $conveniosPlanos ?>);
        <?php endif; ?>

        $('.btn-incluir-conveio-plano').click(function(){
            $('#modal-convenio-plano').modal({
                backdrop: 'static',
            }).modal('show');
        });

        $('#modal-convenio-plano [name=estabelecimento_id]').change(function() {
            let conveniosContainer = $('.convenios_estabelecimento');
            App.blockUI({target : conveniosContainer, message : 'Carregando...'});

            conveniosContainer.load(window.location.href, {act : 'carregarConveniosPorEstabelecimento', estabelecimento_id : $(this).val()}, function(){
                App.unblockUI(conveniosContainer);
                let convenioPlano = $('#modal-convenio-plano [name=convenio_estabelecimento_id_alterar]').val();

                if(convenioPlano) {
                    $('#modal-convenio-plano [name=convenio_estabelecimento_id]').val(convenioPlano).trigger('change');
                }
            });
        });

        $('#modal-convenio-plano').on('hidden.bs.modal', function () {
            limparDadosModalConvenioPlano();
        });

        $(".convenio-plano-form").validateForm({
            rules : {},
            submitHandler: function (e) {
                let form = $(".convenio-plano-form");

                adicionarItemTabela({
                    estabelecimento_id : form.find('[name=estabelecimento_id]').val(),
                    estabelecimento : form.find('[name=estabelecimento_id] option:selected').html(),
                    convenio_estabelecimento_id : form.find('[name=convenio_estabelecimento_id]').val(),
                    convenio_plano : form.find('[name=convenio_estabelecimento_id] option:selected').html(),
                    numero : form.find('[name=numero]').val(),
                    validade : form.find('[name=validade]').val(),
                    id : form.find('[name=id]').val()
                }, $('#tabela-convenio-plano'), form.find('#index').val());

                $('#modal-convenio-plano').modal('hide');
            },
        });
    });

    function formatAcaoEndereco(value, row, index) {
        var btnEditar = '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarConvenioPlano(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>';

        var btnExcluir = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirConvenioPlano(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';

        return btnEditar + btnExcluir;
    }

    function excluirConvenioPlano(index){
        removerItemTabela(index, $('#tabela-convenio-plano'));
    }

    function alterarConvenioPlano(index){
        $('#modal-convenio-plano').modal({
            backdrop: 'static',
        }).modal('show');

        let dados = $('#tabela-convenio-plano').bootstrapTable("getData");
        let convenioPlano = dados[index];
        let form = $(".convenio-plano-form");

        $(form).find('[name=estabelecimento_id]').val(convenioPlano.estabelecimento_id).trigger('change');
        $(form).find('[name=convenio_estabelecimento_id_alterar]').val(convenioPlano.convenio_estabelecimento_id);
        $(form).find('[name=numero]').val(convenioPlano.numero);
        $(form).find('[name=validade]').datepicker("update", toDateObj(convenioPlano.validade));
        $(form).find('[name=index]').val(index);
        $(form).find('[name=id]').val(convenioPlano.id);
    }

    function limparDadosModalConvenioPlano()
    {
        let form = $(".convenio-plano-form");
        form.find('[name=estabelecimento_id]').val('').trigger('change');
        form.find('[name=numero]').val('');
        form.find('[name=validade]').datepicker("update", null);
        form.find('[name=convenio_estabelecimento_id_alterar]').val('');
        form.find('[name=index]').val('');
        form.find('[name=id]').val('');
    }

    function formatAcaoConvenioPlano(value, row, index)
    {
        let btnEditar = '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarConvenioPlano(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>';

        let btnExcluir = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirConvenioPlano(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';

        return btnEditar + btnExcluir;
    }
</script>
