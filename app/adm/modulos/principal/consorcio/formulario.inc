<?php
$mConsorcio = new Consorcio();
$mConsorcioMunicipio = new ConsorcioMunicipio();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            try {
                $params = array_merge($_POST, getParam());
                $id = $mConsorcio->salvarConsorcio($params, $_FILES);
                $mConsorcio->commit();
                $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $id)));
            }catch (CnpjIndisponivel $e){
                addMsgDanger($e->getMessage());
                extract($_POST);
            }catch (MunicipioConsorcioIndisponivel $e){
                addMsgDanger($e->getMessage());
                extract($_POST);
            }
            break;
        case 'carregarMunicipios':
            require APPRAIZ . "adm/modulos/principal/includes/endereco/modal.inc";
            break;
        case 'verificarCnpj':
            $p = getParam();
            if($p['id']){
                $mConsorcio->carregarPorId($p['id']);
                $_REQUEST['p'] = setParam(array('id' => $mConsorcio->pessoajuridica_id), false);
            }else{
                unset($_REQUEST['p']);
            }

            require APPRAIZ . "adm/modulos/principal/includes/pessoajuridica/formulario.inc";
            break;
    }
}

$params = getParam();
$enderecos = null;
if (isset($params['id']) && !empty($params['id'])) {
    $dados = $mConsorcio->getTodosDados($params['id']);
    extract($dados);

    $mEnderecoPessoaJuridica = new EnderecoPessoaJuridica();
    $enderecos = $mEnderecoPessoaJuridica->listarPorPessoaJuridica($pessoajuridica_id, true);

    $mContatoPessoaJuridica = new ContatoPessoaJuridica();
    $contatos = $mContatoPessoaJuridica->listarPorPessoaJuridica($pessoajuridica_id, true);

    $mRepresentantePessoaJuridica = new RepresentantePessoaJuridica();
    $representantes = $mRepresentantePessoaJuridica->listarPorPessoaJuridica($pessoajuridica_id, true);

    $municipiosJson = $mConsorcioMunicipio->getPorConsorcio($id, true);
}

//Recupera o json de municípios para a combo
$jsonMunicipios = $mConsorcioMunicipio->getMunicipiosDisponiveis($id,true, true);
$jsonIdMunicipiosIndisponivels = $mConsorcioMunicipio->getIdMunicipiosIndisponiveis($id, true);

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Insira os dados do consórcio');
?>
<style type="text/css">
    .div-municipios{
        height: auto;
    }
</style>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-share font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do consórcio
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="consorcio-form" method="POST" enctype="multipart/form-data">
            <div class="form-body">

                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Nome do consórcio</label>
                            <?= campo_textom('descricao', 'S', 'S', '', 190, 190, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>
                <?php require APPRAIZ . "adm/modulos/principal/includes/pessoajuridica/formulario.inc"; ?>

                <h4 class="form-section">Municípios</h4>
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <div class="btn-group bootstrap-select CampoEstilo form-control div-municipios">
                                <select name="municipios[]" multiple="multiple" class="CampoEstilo form-control" style="" tabindex="-98"></select>
                            </div>
                        </div>
                    </div>
                </div>

                <h4 class="form-section">Endereços</h4>
                <?php require APPRAIZ . "adm/modulos/principal/includes/endereco/lista.inc"; ?>

                <h4 class="form-section">Contatos</h4>
                <?php require APPRAIZ . "adm/modulos/principal/includes/contato/lista.inc"; ?>

                <h4 class="form-section">Representantes</h4>
                <div class="row">
                    <div class="col-md-12">
                        <a class="btn green btn-incluir-representante">
                            <i class="fa fa-plus-circle"></i>
                            Incluir representante
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
                <input type="hidden" name="representantes" value="<?= $representantes ?>"/>
            </div>

            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar"/>
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
                    <h4 class="modal-title">Representante</h4>
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

                                echo $db->monta_radiom('flg_principal',$opcoes,'S','S', 'f');
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

<style type="text/css">
    .selectize-control .option-disabled{
        color: #94989f !important;
        background-color: #f4f8ff !important;
    }
</style>
<script type="text/javascript">
    $(function () {
        $('[name="municipios[]"]').selectize({
            plugins: {
                'remove_button' : 'remove_button' ,
                'disable_options' : {
                    disableOptions : <?= $jsonIdMunicipiosIndisponivels ?>
                }
            },
            persist: false,
            create: false,
            labelField: 'descricao',
            valueField: 'codigo',
            searchField: ['descricao','estado','Sigla'],
            preload: true,
            options : <?= $jsonMunicipios ?>,
            render: {
                item: function(data, escape) {
                    return '<div>' + escape(data.descricao) + '/' + escape(data.sigla) + '</div>';
                },
                option: function(data, escape) {
                    var indisponivel = '';
                    var classIndisponivel = '';
                    if(data.disponivel === 'f'){
                        indisponivel = '<br /><span style="font-style: italic;">' +
                            'Este município já está vinculado a outro consórcio.</span>';
                        classIndisponivel = 'option-disabled';
                    }

                    return '<div class="' + classIndisponivel + '">' +
                                '<strong>Estado: </strong>' + escape(data.estado) + ' - ' + escape(data.sigla) + '<br />' +
                                '<strong>Município: </strong>' + escape(data.descricao) + '' +
                                indisponivel +
                            '</div>';
                }
            },
            onInitialize : function (){
                <?php if($municipiosJson) : ?>
                this.setValue(<?= $municipiosJson ?>);
                <?php endif; ?>
            }
        });

        $(".consorcio-form").validateForm({
            rules : {
                cpf: {cpf: true},
                foto: {accept: "image/*"}
            },
            submitHandler: function (e) {
                var submit = this;
                verificarCnpj($('#cnpj').val(), function(){
                    submit.submitHandlerFile(e,$('input[type=file][name=logo]'));
                });
            },
            beforeSubmitForm: function (e) {
                var dadosEndereco = $('#tabela-enderecos').bootstrapTable("getData");
                $('input[type=hidden][name=enderecos]').val(JSON.stringify(dadosEndereco));

                var dadosContato = $('#tabela-contatos').bootstrapTable("getData");
                $('input[type=hidden][name=contatos]').val(JSON.stringify(dadosContato));

                var dadosRepresentante = $('#tabela-representantes').bootstrapTable("getData");
                $('input[type=hidden][name=representantes]').val(JSON.stringify(dadosRepresentante));
            }
        });

        $(".endereco-form").validateForm({
            submitHandler: function (e) {
                incluirEndereco();
            }
        });

        $(".contato-form").validateForm({
            submitHandler: function (e) {
                incluirContato();
            }
        });

        $(".representante-form").validateForm({
            submitHandler: function (e) {
                incluirRepresentante();
            }
        });

        $('.btn-voltar').click(function () {
            window.location = '?modulo=principal/consorcio/lista&acao=C';
        });

        $('.btn-incluir-representante').click(function(){
            $('#modal-representante').modal({
                backdrop: 'static',
            }).modal('show');
        });

        <?php if($representantes) : ?>
        $('#tabela-representantes').bootstrapTable('load', <?= $representantes ?>);
        <?php endif; ?>
    });


    function formatAcaoRepresentante(value, row, index) {
        var btnEditar = '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarRepresentante(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>';

        var btnExcluir = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirRepresentante(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';

        return btnEditar + btnExcluir;
    }

    function formatRepresentantePrincipal(value, row, index) {
        var retorno = 'Sim';
        if(row.flg_principal == 'f'){
            retorno = 'Não';
        }

        return retorno;
    }

    function excluirRepresentante(index){
        removerItemTabela(index, $('#tabela-representantes'));
    }

    function limparDadosRepresentante() {
        var form = $('[name=representante-form]');
        $(form).find('[name=contato]').val('');
        $(form).find('[name=nome]').val('');
        $(form).find('[name=cargo]').val('');
        $(form).find('[name=index]').val('');
        $(form).find('#flg_principal_f').prop('checked',true);
    }

    function incluirRepresentante(){
        var form = $('[name=representante-form]');

        //Se for principal procura todos os outros que forem principais e altera para não
        if($(form).find('[name=flg_principal]:checked').val() == 't'){
            var dados = $('#tabela-representantes').bootstrapTable("getData");
            $.each(dados, function(index,item){
                if(item.flg_principal == 't') {
                    item.flg_principal = 'f';
                    $('#tabela-representantes').bootstrapTable('updateRow', {
                        index: index,
                        row: item
                    });
                }
            });
        }

        var representante = {
            contato : $(form).find('[name=contato]').val(),
            nome : $(form).find('[name=nome]').val(),
            cargo : $(form).find('[name=cargo]').val(),
            flg_principal : $(form).find('[name=flg_principal]:checked').val(),
            id : $(form).find('[name=id]').val()
        };

        adicionarItemTabela(representante, $('#tabela-representantes'), $(form).find('#index').val());
        $('#modal-representante').modal('hide');
        limparDadosRepresentante();
    }

    function alterarRepresentante(index){
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

        if(representante.flg_principal == 'f'){
            $(form).find('#flg_principal_f').prop('checked',true);
        }else{
            $(form).find('#flg_principal_t').prop('checked',true);
        }

        $(form).find('[name=id]').val(representante.id);
    }
</script>
