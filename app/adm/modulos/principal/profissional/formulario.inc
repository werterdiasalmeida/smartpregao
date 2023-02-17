<?php
$mPessoaFisica = new PessoaFisica();
$mProfissional = new Profissional();
$mUsuario = new Usuario();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            try {
                $params = array_merge($_POST, getParam());
                $paramsPessoa = $params;
                $paramsPessoa['id'] = $params['pessoafisica_id'];
                $params['pessoafisica_id'] = $mPessoaFisica->salvarPessoaFisica($paramsPessoa, $_FILES);
                $mProfissional->salvarProfissional($params);
                $mProfissional->commit();

                if ($paramAba) {
                    $paramAba = getParam($paramAba);
                    $paramAba['pessoa_id'] = $params['pessoafisica_id'];
                    $paramAba = setParam($paramAba);
                }

                $db->sucessom(($moduloRetorno ? $moduloRetorno : $_REQUEST['modulo']), ($paramAba ? '&p=' . $paramAba : setParam(array('id' => $params['pessoafisica_id']))));
            } catch (CpfIndisponivel $e) {
                $mProfissional->rollback();
                addMsgDanger($e->getMessage());
                extract($_POST);
            } catch (Exception $e) {
                $mProfissional->rollback();
                addMsgDanger($e->getMessage());
                extract($_POST);
            }

            $infoprofissionais = str_replace('\"', '"', $infoprofissionais);
            $enderecos = str_replace('\"', '"', $enderecos);
            break;
        case 'carregarMunicipios':
            require APPRAIZ . "adm/modulos/principal/includes/endereco/modal.inc";
            break;
        case 'carregaEspecialidades':
            $tipo = (int)$_REQUEST['tipo_profissional_id'];
            $mEspecialidade = new Especialidade();
            $especialidades = $mEspecialidade->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE", "tipo_profissional_id = {$tipo}"));

            $readonly = "";
            if (!is_array($especialidades) || count($especialidades) == 0) {
                $readonly = ' readonly="readonly" ';
            }
            $html = '<select name="especialidades" required="" multiple="multiple" ' . $readonly . ' class="CampoEstilo form-control" style="" tabindex="-98">';

            if ($tipo) {
                if (is_array($especialidades) && count($especialidades) > 0) {
                    foreach ($especialidades AS $especialidade) {
                        $html .= '<option value="' . $especialidade['codigo'] . '">' . $especialidade['descricao'] . '</option>';
                    }
                } else {
                    $html .= '<option value="">Nenhuma especialidade encontrada</option>';
                }
            } else {
                $html .= '<option value="">Selecione um tipo</option>';
            }

            $html .= '</select>';
            die($html);
            break;
        case 'verificarCpf':
            require APPRAIZ . "adm/modulos/principal/includes/pessoafisica/formulario.inc";
            break;
        case 'verificarCpfEmailUsuario':
            $p = getParam();
            $mPessoaFisica = new PessoaFisica();
            $id = $_POST['pessoa_id'] ? $_POST['pessoa_id'] : $p['id'];
            $usucpf = str_replace(array('-', '.'), '', $_POST['cpf']);
            $arValidacao = array();

            //Validando o cpf
            if (!$mPessoaFisica->isCpfDisponivel($_POST['cpf'], $id)) {
                $arValidacao[] = array('campo' => 'cpf', 'mensagem' => " O CPF informado não está disponível.");
            }

//            //Validando o email dos usuários
//            $dadosUsarios = $mUsuario->recuperarTodos('*', array("(usucpf = '{$usucpf}' OR usuemail = '{$_POST['email']}')"));
//            if((count($dadosUsarios) == 1 && $usucpf != $dadosUsarios[0]['usucpf']) || count($dadosUsarios) > 1){
//                $arValidacao[] = array('campo' => 'contato_email', 'mensagem' => ' Já existe um usuário vinculado ao e-mail informado.');
//            }

            die(json_encode(utf8ArrayEncode($arValidacao)));
            break;
        case 'carregarProfissionalPorCpf':
            $p = getParam();
            $mProfissional = new Profissional();
            $mPessoaFisica = new PessoaFisica();
            $mContatoPessoaFisica = new ContatoPessoaFisica();
            $mEnderecoPessoaFisica = new EnderecoPessoaFisica();

            $retorno = array('disponivel' => false);

            /*if(!$mProfissional->isProfissional($_POST['cpf'])){*/
            $dadosPessoa = $mPessoaFisica->recuperarLinha('*', array(
                "cpf = '{$_POST['cpf']}'",
                "excluido IS FALSE"
            ), false);

            if ($dadosPessoa) {
                $contatos = $mContatoPessoaFisica->listarEspecificosPorPessoaFisica($dadosPessoa['id']);
                $enderecos = $mEnderecoPessoaFisica->listarPorPessoaFisica($dadosPessoa['id']);
                $dadosProfissional = $mProfissional->listarPorPessoaFisica($dadosPessoa['id'], false, $p['estabelecimento_id']);

                $retorno = array(
                    'disponivel' => true,
                    'nome_completo' => $dadosPessoa['nome_completo'],
                    'data_nascimento' => $dadosPessoa['data_nascimento'],
                    'pessoafisica_id' => $dadosPessoa['id'],
                    'sexo' => $dadosPessoa['sexo'],
                    'contatos[CL][contato]' => $contatos['contato_celular'],
                    'contatos[TF][contato]' => $contatos['contato_telefone'],
                    'contatos[EM][contato]' => $contatos['contato_email'],
                    'profissional' => $dadosProfissional,
                    'enderecos' => $enderecos
                );
            } else {
                $retorno = array('disponivel' => true);
            }
            /*}*/

            die(json_encode(utf8ArrayEncode($retorno)));
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

    $infoprofissionais = $mProfissional->listarPorPessoaFisica($params['id'], false, $params['estabelecimento_id']);

    if ($infoprofissionais[0]) {
        $curriculo = $infoprofissionais[0]['curriculo'];
    }

    $infoprofissionais = json_encode(utf8ArrayEncode($infoprofissionais));

    $mContatoPessoaFisica = new ContatoPessoaFisica();
    $contatos = $mContatoPessoaFisica->listarEspecificosPorPessoaFisica($params['id'], true);
    extract($contatos);
}

include APPRAIZ . "includes/cabecalhom.inc";
if ($paramAba) {
    echo Estabelecimento::gerarCabecalho($params['estabelecimento_id']);
}

if ($paramAba) {
    $notMenu = array();

    if (empty($params['id'])) {
        $notMenu = $db->carregarColuna("SELECT mnuid FROM seguranca.menu WHERE mnulink = 'adm.php?modulo=principal/estabelecimento/corpoClinico/anexoProfissional&acao=C' AND sisid = {$_SESSION['sisid']}");
    }

    UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, '&p=' . $paramAba, null, $notMenu, true, true);
} else if (isset($params['id']) && !empty($params['id'])) {
    monta_titulom('Insira os dados do profissional');
    $db->cria_abam($abacod_tela, $url, setParam(getParam()), array());
}
?>

<style type="text/css">
    .div-especialidades {
        height: auto;
    }
</style>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-user-follow font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do profissional
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="profissional-form" method="POST" enctype="multipart/form-data">
            <div class="form-body">
                <?php
                $cpfObrigatorio = true;
                require APPRAIZ . "adm/modulos/principal/includes/pessoafisica/formulario.inc";
                ?>

                <h4 class="form-section">Contatos</h4>
                <?php require APPRAIZ . "adm/modulos/principal/includes/contato/formulario.inc"; ?>

                <h4 class="form-section">Currículo</h4>
                <?= campo_textaream('curriculo', 'N', 'S', '', '10', '5', '1800', '', 0, 'Forneça um resumo das informações curriculares e experiências profissionais') ?>

                <h4 class="form-section obrigatorio">Informações profissionais</h4>
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <a class="btn green btn-infoprofissional">
                                <i class="fa fa-plus-circle"></i>
                                Incluir informação profissional
                            </a>
                            <br/>
                            <br/>
                            <table id="tabela-infoprofissional" data-toggle="table" class="table table-striped"
                                   data-unique-id="id">
                                <thead>
                                <tr>
                                    <th data-field="tipo_profissional">Tipo</th>
                                    <th data-field="registro_profissional">Número de Registro</th>
                                    <th data-field="uf_descricao">UF Registro</th>
                                    <th data-field="data_registro">Data Registro</th>
                                    <th data-field="especialidades">Especialidade(s)</th>
                                    <th data-formatter="formatAcaoInfoProfissional" data-align="center">Ação</th>
                                </tr>
                                </thead>
                                <tbody>

                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="infoprofissionais" value="<?= $infoprofissionais ?>"/>
                <input type="hidden" name="pessoafisica_id" value="<?= $id ?>"/>

                <h4 class="form-section">Endereços</h4>
                <?php require APPRAIZ . "adm/modulos/principal/includes/endereco/lista.inc"; ?>

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
<div class="modal fade in" id="modal-infoprofissional" tabindex="-1" role="modal-infoprofissional" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <form role="form" name="infoprofissional-form" class="infoprofissional-form" method="POST">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Informações Profissionais</h4>
                </div>
                <div class="modal-body">
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Tipo</label>
                                <?php
                                $mTipo = new TipoProfissional();
                                $tipos = $mTipo->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                                $db->monta_combom("tipo_profissional_id", $tipos, 'S', 'Selecione...', '', '', '', '', 'S');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Número de Registro</label>
                                <?= campo_textom('registro_profissional', 'N', 'S', '', 20, 20, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>UF do registro</label>
                                <?php
                                $mUnidadeFederacao = new UnidadeFederacao();
                                $estados = $mUnidadeFederacao->recuperarTodos('uf_sigla AS codigo, uf_nome_estado AS descricao', array(), 2);
                                $db->monta_combom("uf_sigla", $estados, 'S', 'Selecione...', '', '', '', '', 'N');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Data do Registro</label>
                                <?= campo_datam('data_registro', 'N', 'S', '', 'S'); ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label class="bold">Especialidades</label>
                                <div class="btn-group bootstrap-select CampoEstilo form-control div-especialidades">
                                    <select name="especialidades" required="" readonly="readonly" multiple="multiple"
                                            class="CampoEstilo form-control" style="" tabindex="-98">
                                        <option value="">Selecione um tipo</option>
                                    </select>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-12">
                            <table id="tabela-rqe" data-toggle="table" class="table table-striped" data-unique-id="id"
                                   data-pagination="true">
                                <thead>
                                <tr>
                                    <th data-field="especialidade" data-width="50%">Especialidade</th>
                                    <th data-field="rqe" data-formatter="campoRqe" data-width="20%">RQE</th>
                                    <th data-field="duracao_consulta" data-formatter="campoDuracaoConsulta"
                                        data-width="30%">Duração das consultas
                                        <small>(em minutos)</small>
                                    </th>
                                </tr>
                                </thead>
                                <tbody>
                                </tbody>
                            </table>
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

                <input type="hidden" id="index" name="index"/>
                <input type="hidden" id="id" name="id"/>
            </form>
        </div>
    </div>
</div>

<script type="text/javascript" src="../includes/selectize-0.12.1/js//standalone/selectize.min.js"></script>
<link rel="stylesheet" href="../includes/selectize-0.12.1/css/selectize.bootstrap3.css"/>

<script type="text/javascript">
    $(function () {
        $('#cpf').change(function () {
            carregarProfissionalPorCpf($(this).val());
        });

        $('select[name=tipo_profissional_id]').change(function () {
            carregaEspecialidades();
        });

        defineRenderSelectizeEspecialidades();

        $(".profissional-form").validateForm({
            rules: {
                cpf: {cpf: true},
                foto: {accept: "image/*"},
                contato_email: {email: true}
            },
            submitHandler: function (e) {
                if ($('#tabela-infoprofissional').bootstrapTable("getData").length === 0) {
                    addMsgErroCampo($('#tabela-infoprofissional').closest('.bootstrap-table'), 'Adicione pelo menos uma informação do profissional.');
                    return;
                }

                var submit = this;
                verificarCpfEmailUsuario($('#cpf').val(), $('#contato_email').val(), function () {
                    submit.submitHandlerFile(e, $('input[type=file][name=foto]'));
                });
            },
            beforeSubmitForm: function (e) {
                var dadosEndereco = $('#tabela-enderecos').bootstrapTable("getData");
                $('input[type=hidden][name=enderecos]').val(JSON.stringify(dadosEndereco));

                var dadosInfoProfissionais = $('#tabela-infoprofissional').bootstrapTable("getData");
                $.each(dadosInfoProfissionais, function (index, item) {
                    dadosInfoProfissionais[index].especialidades = dadosInfoProfissionais[index].jsonEspecialidades;
                    dadosInfoProfissionais[index].jsonEspecialidades = "";
                });

                $('input[type=hidden][name=infoprofissionais]').val(JSON.stringify(dadosInfoProfissionais));
            }
        });

        $(".endereco-form").validateForm({
            submitHandler: function (e) {
                incluirEndereco();
            }
        });

        $(".infoprofissional-form").validateForm({
            submitHandler: function (e) {
                incluirInformacaoProfissional();
            }
        });

        $('.btn-voltar').click(function () {
            window.location = '?modulo=<?= ($moduloRetorno ? $moduloRetorno : 'principal/profissional/lista') ?>&acao=C<?= ($paramAba ? '&p=' . $paramAba : '') ?>';
        });

        $('#modal-infoprofissional').on('hidden.bs.modal', function () {
            limparDadosProfissional();
        });

        $('.btn-infoprofissional').click(function () {
            $('#modal-infoprofissional').modal({
                backdrop: 'static',
            }).modal('show');
        });

        <?php if($infoprofissionais) : ?>
        carregarInfoProfissional(<?= $infoprofissionais ?>);
        <?php endif; ?>
    });

    function carregarInfoProfissional(infoProfissionais) {
        $.each(infoProfissionais, function (index, value) {
            var especialidades = "";
            $.each(this.especialidades, function () {
                especialidades += '<span class="label label-sm label-success" style="margin-top: 3px !important; display: inline-block; padding: 4px;">' + this + '</span> &nbsp;';
            });
            infoProfissionais[index]['especialidades'] = especialidades;
        });

        $('#tabela-infoprofissional').bootstrapTable('load', infoProfissionais);
    }

    function formatAcaoInfoProfissional(value, row, index) {
        var btnEditar = '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarInformacaoProfissional(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>';

        var btnExcluir = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirInformacaoProfissional(' + index + ')" title="Excluir" style="margin: 2px;">'
            + ' <i class="bi bi-x-square"></i>'
            + '</a>';

        return btnEditar + btnExcluir;
    }

    function incluirInformacaoProfissional() {
        var form = $('[name=infoprofissional-form]');

        var jsonEspecialidades = [];
        var especialidades = "";
        $.each(form.find('[name=especialidades]').find('option:selected'), function () {
            especialidades += '<span class="label label-sm label-success" style="margin-top: 3px !important; display: inline-block; padding: 4px;">' + $(this).html() + '</span> &nbsp;';
            jsonEspecialidades.push({
                especialidade: $(this).val(),
                rqe: form.find('#rqe_' + $(this).val()).val(),
                duracao_consulta: form.find('#duracao_consulta_' + $(this).val()).val()
            });
        });

        var infoProfissional = {
            registro_profissional: form.find('[name=registro_profissional]').val(),
            data_registro: form.find('[name=data_registro]').val(),
            uf_sigla: form.find('[name=uf_sigla]').val(),
            uf_descricao: form.find('[name=uf_sigla]').val() ? form.find('[name=uf_sigla]').find('option:selected').html() : '',
            tipo_profissional_id: form.find('[name=tipo_profissional_id]').val(),
            tipo_profissional: form.find('[name=tipo_profissional_id]').find('option:selected').html(),
            jsonEspecialidades: jsonEspecialidades,
            especialidades: especialidades,
            id: form.find('[name=id]').val()
        };

        adicionarItemTabela(infoProfissional, $('#tabela-infoprofissional'), parseInt(form.find('#index').val()));
        $('#modal-infoprofissional').modal('hide');
    }

    function defineRenderSelectizeEspecialidades() {
        $('[name=especialidades]').selectize({
            plugins: ['remove_button'],
            persist: false,
            create: false,
            closeAfterSelect: true,
            render: {
                item: function (data, escape) {
                    return '<div>' + escape(data.text) + '</div>';
                }
            },
            onChange: function (value) {
                atualizarTabelaRqe();
            },
            onItemAdd: function () {
                $('.rqe:last').focus();
            }
        });
    }

    function carregaEspecialidades(ids) {
        var tipo_profissional_id = $('select[name=tipo_profissional_id]').val();
        App.blockUI({
            message: 'Carregando...',
            target: '.div-especialidades'
        });

        $('.div-especialidades').load(window.location.href, {
            act: 'carregaEspecialidades',
            tipo_profissional_id: tipo_profissional_id,
        }, function () {
            defineRenderSelectizeEspecialidades();
            App.unblockUI('.div-especialidades');

            if (ids && ids.length > 0) {
                var $select = $('[name=especialidades]').selectize();
                var selectize = $select[0].selectize;
                var especialidades = ids.map(function (especialidade) {
                    return especialidade.especialidade;
                });

                selectize.setValue(especialidades);

                var tabelaRqe = $('#tabela-rqe');
                ids.map(function (especialidade) {
                    tabelaRqe.find('#rqe_' + especialidade.especialidade).val(especialidade.rqe);
                    tabelaRqe.find('#duracao_consulta_' + especialidade.especialidade).val((especialidade.duracao_consulta ? especialidade.duracao_consulta : 5));
                });
            } else {
                atualizarTabelaRqe();
            }
        });
    }

    function limparDadosProfissional() {
        var form = $('[name=infoprofissional-form]');
        form.find('[name=registro_profissional]').val('');
        form.find('[name=data_registro]').val('');
        form.find('[name=tipo_profissional_id]').val('').selectpicker('refresh');
        form.find('[name=uf_sigla]').val('').selectpicker('refresh');

        carregaEspecialidades();

        var validator = form.validate();
        form.find(".has-error").removeClass("has-error");
        validator.resetForm();
        validator.reset();
    }

    function alterarInformacaoProfissional(index) {
        $('#modal-infoprofissional').modal({
            backdrop: 'static',
        }).modal('show');

        var dados = $('#tabela-infoprofissional').bootstrapTable("getData");
        var infoProfissional = dados[index];
        var form = $('[name=infoprofissional-form]');

        form.find('[name=tipo_profissional_id]').val(infoProfissional.tipo_profissional_id).selectpicker('refresh');
        form.find('[name=uf_sigla]').val(infoProfissional.uf_sigla).selectpicker('refresh');
        form.find('[name=registro_profissional]').val(infoProfissional.registro_profissional);
        form.find('[name=data_registro]').val(infoProfissional.data_registro);
        form.find('[name=index]').val(index);
        form.find('[name=id]').val(infoProfissional.id);

        carregaEspecialidades(infoProfissional.jsonEspecialidades);
    }

    function excluirInformacaoProfissional(index) {
        removerItemTabela(index, $('#tabela-infoprofissional'));
    }

    function atualizarTabelaRqe() {
        var tabela = $('#tabela-rqe');
        var campo = $('[name=especialidades]');

        var especialidades = campo[0].selectize.options;
        var especialidadesCampo = campo.val();
        especialidadesCampo = especialidadesCampo ? especialidadesCampo : [];

        var especialidadesSelecionadas = especialidadesCampo.map(function (index, value) {
            var especialidade = especialidades[index];
            return {
                rqe: $('#rqe_' + especialidade.value).val(),
                duracao_consulta: $('#duracao_consulta_' + especialidade.value).val(),
                especialidade: especialidade.text,
                id: especialidade.value,
            };
        });

        tabela.bootstrapTable('load', especialidadesSelecionadas);

        $('.duracao-consulta').TouchSpin({
            min: 5,
            max: 120,
            step: 5
        });
    }

    function campoRqe(value, row, index) {
        row.rqe = row.rqe ? row.rqe : '';

        return '<input type="text" id="rqe_' + row.id + '" name="rqe[' + row.id + ']" '
            + ' value="' + row.rqe + '" class="normal form-control rqe" />';
    }

    function campoDuracaoConsulta(value, row, index) {
        row.duracao_consulta = row.duracao_consulta ? row.duracao_consulta : 5;

        return '<input type="text" id="duracao_consulta_' + row.id + '" name="duracao_consulta[' + row.id + ']" '
            + ' value="' + row.duracao_consulta + '" class="normal  form-control duracao-consulta" '
            + 'required aria-required="true" />';
    }

    function carregarProfissionalPorCpf(cpf) {
        var container = $('.profissional-form');
        var limparCampos = function () {
            $(container).find('[name=nome_completo], [name=data_nascimento], [name=pessoafisica_id], ' +
                '[name="contatos[CL][contato]"], [name="contatos[TF][contato]"], [name="contatos[EM][contato]"]').val('');
            $(container).find('[name=sexo]').prop('checked', false);
        };
        if (cpf === '' || !validarCpfCnpj(container.find('#cpf')[0])) {
            limparCampos();
            return;
        }

        App.blockUI({
            message: 'Carregando...',
            target: '.form-body'
        });

        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType: 'json',
            data: {act: 'carregarProfissionalPorCpf', cpf: cpf},
            success: function (retorno) {
                App.unblockUI('.form-body');
                if (!retorno.disponivel) {
                    addMsgErroCampo($('#cpf'), 'O CPF informado não está disponível.');
                } else {
                    if (retorno.pessoafisica_id) {
                        if (retorno.enderecos) {
                            $('#tabela-enderecos').bootstrapTable('load', retorno.enderecos);
                        }

                        if (retorno.profissional) {
                            carregarInfoProfissional(retorno.profissional);
                        }

                        $.each(retorno, function (campo, valor) {
                            var elemCampo = container.find('[name="' + campo + '"]');

                            switch (elemCampo.prop('type')) {
                                case 'radio':
                                    container.find('[name="' + campo + '"][value=' + valor + ']').prop('checked', true);
                                    break;
                                default:
                                    switch (campo) {
                                        case 'data_nascimento':
                                            valor ? elemCampo.datepicker('setDate', new Date(valor + ' 00:00:00')) : elemCampo.val('');
                                            break;
                                        default:
                                            elemCampo.val(valor);
                                            break;
                                    }
                                    break;
                            }
                        });
                    } else {
                        $('#tabela-enderecos').bootstrapTable('load', []);
                        carregarInfoProfissional({});
                        // limparCampos();
                    }
                }
            },
            error: function (result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a operação');
            }
        });
    }


    function verificarCpfEmailUsuario(cpf, email, success, error) {
        success = success ? success : function (result) {
        };
        error = error ? error : function (result) {
        };

        if (cpf === '') {
            success();
            return;
        }

        App.blockUI({
            message: 'Carregando...',
            target: '.form-body'
        });

        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType: 'json',
            data: {
                act: 'verificarCpfEmailUsuario',
                cpf: cpf,
                email: email,
                pessoa_id: $('[name=pessoafisica_id]').val()
            },
            success: function (retorno) {
                App.unblockUI('.form-body');

                if (retorno.length > 0) {
                    $.each(retorno, function (index, item) {
                        addMsgErroCampo($('#' + item.campo), item.mensagem);
                    });
                } else {
                    success(retorno);
                }
            },
            error: function (result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a operação');
            }
        });
    }
</script>
