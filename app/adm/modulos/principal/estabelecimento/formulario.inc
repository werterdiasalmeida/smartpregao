<?php
$mEstabelecimento = new Estabelecimento();
$params = getParam();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar':
            try {
                $params = array_merge($_POST, getParam());
                $id = $mEstabelecimento->salvarEstabelecimento($params, $_FILES);
                $mEstabelecimento->commit();

                $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $id)));
            } catch (CnpjIndisponivel $e) {
                addMsgDanger($e->getMessage());
                extract($_POST);
            }
            break;
        case 'carregarMunicipios':
            require APPRAIZ . "adm/modulos/principal/includes/endereco/modal.inc";
            break;
        case 'verificarCnpj':
            if ($params['id']) {
                $mEstabelecimento->carregarPorId($params['id']);
                $_REQUEST['p'] = setParam(array('id' => $mEstabelecimento->pessoajuridica_id), false);
            }
            require APPRAIZ . "adm/modulos/principal/includes/pessoajuridica/formulario.inc";
            break;
    }
}

$aba = false;

$enderecos = null;
if (isset($params['id']) && !empty($params['id'])) {
    UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($params['id']);

    $aba = true;
    $dados = $mEstabelecimento->getTodosDados($params['id']);
    extract($dados);

    $mEnderecoPessoaJuridica = new EnderecoPessoaJuridica();
    $enderecos = $mEnderecoPessoaJuridica->listarPorPessoaJuridica($dados['pessoajuridica_id'], true);

    $mContatoPessoaJuridica = new ContatoPessoaJuridica();
    $contatos = $mContatoPessoaJuridica->listarEspecificosPorPessoaJuridica($dados['pessoajuridica_id']);
    extract($contatos);

    $mRepresentantePessoaJuridica = new RepresentantePessoaJuridica();
    $representantes = $mRepresentantePessoaJuridica->listarPorPessoaJuridica($dados['pessoajuridica_id'], true);
}

include APPRAIZ . "includes/cabecalhom.inc";

if ($aba) {
    echo Estabelecimento::gerarCabecalho();
}
?>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do cliente
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="estabelecimento-form" method="POST" enctype="multipart/form-data">
            <input type="hidden" id="remove_logo" name="remove_logo" />
            <div class="form-body">
                <?php require APPRAIZ . "adm/modulos/principal/includes/pessoajuridica/formulario.inc"; ?>

                <div class="row">
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Seguimento</label>
                            <?php
                            $mTipoEstabelecimento = new TipoEstabelecimento();
                            $tiposEstabelecimento = $mTipoEstabelecimento->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                            $db->monta_combom("tipo_estabelecimento_id", $tiposEstabelecimento, 'S', 'Selecione...', '', '', '', '', 'S');
                            ?>
                        </div>
                    </div>

                    <div class="col-md-8">
                        <?php require APPRAIZ . "adm/modulos/principal/includes/contato/formularioEstabelecimento.inc"; ?>
                    </div>
                </div>

                <h4 class="form-section">Contatos</h4>
                <div class="row">
                    <div class="col-md-12">
                        <a class="btn green btn-incluir-contato">
                            <i class="fa fa-plus-circle"></i>
                            Incluir contato
                        </a>
                        <br />
                        <br />
                        <table id="tabela-representantes" data-toggle="table" class="table table-striped" data-unique-id="id">
                            <thead>
                                <tr>
                                    <th data-field="nome">Nome</th>
                                    <th data-field="cargo">Cargo</th>
                                    <th data-field="contato">Contato</th>
                                    <th data-formatter="formatContatoPrincipal" data-field="flg_principal">Principal</th>
                                    <th data-formatter="formatAcaoRepresentante" data-align="center">Ação</th>
                                </tr>
                            </thead>
                            <tbody>
                            </tbody>
                        </table>
                    </div>
                </div>
                <input type="hidden" name="representantes" value="<?= $representantes ?>" />
            </div>
            <h4 class="form-section">Endereços</h4>
            <?php require APPRAIZ . "adm/modulos/principal/includes/endereco/lista.inc"; ?>

            <h4 class="form-section">Documentos da Empresa</h4>
            <div class="row">
                <div class="col-md-12">
                    <a class="btn green btn-incluir-documento">
                        <i class="fa fa-plus-circle"></i>
                        Incluir Documento
                    </a>
                    <br />
                    <br />
                    <table id="tabela-documento" data-toggle="table" class="table table-striped" data-unique-id="id">
                        <thead>
                            <tr>
                                <th data-field="nome">Nome</th>
                                <th data-field="cargo">Tipo</th>
                                <th data-field="contato">Validade</th>
                                <th data-formatter="formatContatoPrincipal" data-field="flg_principal">Detalhado</th>
                                <th data-formatter="formatAcaoRepresentante" data-align="center">Ação</th>
                            </tr>
                        </thead>
                        <tbody>
                        </tbody>
                    </table>
                </div>
            </div>

            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar" />
        </form>
    </div>
</div>

<?php require APPRAIZ . "adm/modulos/principal/includes/endereco/modal.inc"; ?>
<?php require APPRAIZ . "adm/modulos/principal/includes/contato/modal.inc"; ?>

<div class="modal fade in" id="modal-representante" tabindex="-1" role="modal-representante" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <form role="form" name="representante-form" class="representante-form" method="POST">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Contato do estabelecimento</h4>
                </div>
                <div class="modal-body">
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Nome</label>
                                <?= campo_textom('nome', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Cargo</label>
                                <?= campo_textom('cargo', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Contato</label>
                                <?= campo_textom('contato', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Principal</label>
                                <br />
                                <?php
                                $opcoes = array(
                                    array('codigo' => 't', 'descricao' => 'Sim'),
                                    array('codigo' => 'f', 'descricao' => 'Não'),
                                );
                                echo $db->monta_radiom('flg_principal', $opcoes, 'S', 'S', 'f');
                                ?>
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
<div class="modal fade in" id="modal-documento" tabindex="-1" role="modal-documento" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <form role="form" name="documento-form" class="documento-form" method="POST">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Documento do estabelecimento</h4>
                </div>
                <div class="modal-body">
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Nome</label>
                                <?= campo_textom('nome', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Tipo</label>
                                <br />
                                <?php
                                $opcoes = array(
                                    array('codigo' => 'Habilitação Jurídica', 'descricao' => 'Habilitação Jurídica'),
                                    array('codigo' => 'Habilitação Financeira', 'descricao' => 'Habilitação Financeira'),
                                    array('codigo' => 'Habilitação Técnica', 'descricao' => 'Habilitação Técnica'),
                                );
                                echo $db->monta_radiom('tipo', $opcoes, 'S', 'S', 'Habilitação Jurídica');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Validade</label>
                                <?= campo_datam('contato', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Detalhado</label>
                                <br />
                                <?php
                                $opcoes = array(
                                    array('codigo' => 't', 'descricao' => 'Sim'),
                                    array('codigo' => 'f', 'descricao' => 'Não'),
                                );
                                echo $db->monta_radiom('flg_detalhado', $opcoes, 'S', 'S', 'f');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Arquivo (*.pdf)</label>
                                <br />
                                <input type="file">
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
    $(function() {
        $(".estabelecimento-form").validateForm({
            rules: {
                cnpj: {
                    cnpj: true
                },
                foto: {
                    accept: "image/*"
                },
                "contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]": {
                    email: true
                }
            },
            submitHandler: function(e) {
                var submit = this;
                verificarCnpj($('#cnpj').val(), function() {
                    submit.submitHandlerFile(e, $('#file-logo'));
                });
            },
            beforeSubmitForm: function(e) {
                const remove_logo = localStorage.getItem('remove_logo');
                if (remove_logo === 'S') {
                    $('#remove_logo').val('S');
                }
                var dadosEndereco = $('#tabela-enderecos').bootstrapTable("getData");
                $('input[type=hidden][name=enderecos]').val(JSON.stringify(dadosEndereco));

                var dadosRepresentante = $('#tabela-representantes').bootstrapTable("getData");
                $('input[type=hidden][name=representantes]').val(JSON.stringify(dadosRepresentante));

            }
        });

        $(".endereco-form").validateForm({
            submitHandler: function(e) {
                incluirEndereco();
            }
        });

        $(".contato-form").validateForm({
            submitHandler: function(e) {
                incluirContato();
            }
        });

        $(".representante-form").validateForm({
            submitHandler: function(e) {
                incluirRepresentante();
            }
        });

        $('.btn-voltar').click(function() {
            window.location = '?modulo=principal/estabelecimento/lista&acao=C';
        });

        $('.btn-incluir-contato').click(function() {
            $('#modal-representante').modal({
                backdrop: 'static',
            }).modal('show');
        });
        $('.btn-incluir-documento').click(function() {
            $('#modal-documento').modal({
                backdrop: 'static',
            }).modal('show');
        });
        3

        <?php if ($representantes) : ?>
            $('#tabela-representantes').bootstrapTable('load', <?= $representantes ?>);
        <?php endif; ?>
    });


    function formatAcaoRepresentante(value, row, index) {
        var btnEditar = '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarRepresentante(' + index + ')" title="Alterar" style="margin: 2px;">' +
            '<i class="icon-pencil"></i>' +
            '</a>';

        var btnExcluir = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirRepresentante(' + index + ')" title="Excluir" style="margin: 2px;">' +
            '<i class="icon-trash"></i>' +
            '</a>';

        return btnEditar + btnExcluir;
    }

    function formatRepresentantePrincipal(value, row, index) {
        var retorno = 'Sim';
        if (row.flg_principal == 'f') {
            retorno = 'Não';
        }

        return retorno;
    }

    function excluirRepresentante(index) {
        removerItemTabela(index, $('#tabela-representantes'));
    }

    function limparDadosRepresentante() {
        var form = $('[name=representante-form]');
        $(form).find('[name=contato]').val('');
        $(form).find('[name=nome]').val('');
        $(form).find('[name=cargo]').val('');
        $(form).find('[name=index]').val('');
        $(form).find('#flg_principal_f').prop('checked', true);
    }

    function incluirRepresentante() {
        var form = $('[name=representante-form]');

        //Se for principal procura todos os outros que forem principais e altera para não
        if ($(form).find('[name=flg_principal]:checked').val() == 't') {
            var dados = $('#tabela-representantes').bootstrapTable("getData");
            $.each(dados, function(index, item) {
                if (item.flg_principal == 't') {
                    item.flg_principal = 'f';
                    $('#tabela-representantes').bootstrapTable('updateRow', {
                        index: index,
                        row: item
                    });
                }
            });
        }

        var representante = {
            contato: $(form).find('[name=contato]').val(),
            nome: $(form).find('[name=nome]').val(),
            cargo: $(form).find('[name=cargo]').val(),
            flg_principal: $(form).find('[name=flg_principal]:checked').val(),
            id: $(form).find('[name=id]').val()
        };

        adicionarItemTabela(representante, $('#tabela-representantes'), $(form).find('#index').val());
        $('#modal-representante').modal('hide');
        limparDadosRepresentante();
    }

    function alterarRepresentante(index) {
        $('#modal-representante').modal({
            backdrop: 'static',
        }).modal('show');

        var dados = $('#tabela-representantes').bootstrapTable("getData");
        var representante = dados[index];
        var form = $('[name=representante-form]');

        $(form).find('[name=contato]').val(representante.contato);
        $(form).find('[name=nome]').val(representante.nome);
        $(form).find('[name=cargo]').val(representante.cargo);
        $(form).find('[name=index]').val(index);

        if (representante.flg_principal == 'f') {
            $(form).find('#flg_principal_f').prop('checked', true);
        } else {
            $(form).find('#flg_principal_t').prop('checked', true);
        }

        $(form).find('[name=id]').val(representante.id);
    }

    function formatContatoPrincipal(value, row, index) {
        var retorno = 'Sim';
        if (row.flg_principal == 'f') {
            retorno = 'Não';
        }
        return retorno;
    }
</script>