<?php
if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'agendarConsulta' :
            try {
                $mAgenda = new EstabelecimentoProfissionalAgenda();
                $params = array_merge($_POST, getParam());
                $params = utf8ArrayDecode($params);

                if(empty($params['reagenda_id'])){
                    $mAgenda->agendarConsulta($params);
                }else{
                    $mAgenda->reagendarConsulta($params);
                }

                $mAgenda->commit();

                $db->sucessomAjax($_REQUEST['modulo']);
            }catch (AbstractException $e) {
                $db->insucessomAjax($_REQUEST['modulo'], array(), $e->getMessage());
            }

            break;
        case 'alterarDadosConsulta' :
            try {
                $mAgenda = new EstabelecimentoProfissionalAgenda();
                $mValorPagar = new ValorAPagar();
                $mSolicitacaoCorrecao = new SolicitacaoCorrecaoAgenda();

                $params = array_merge($_POST, getParam());
                $params = utf8ArrayDecode($params);

                //gerando o valor negativo no pagamento, caso seja uma solicitação de correção
                $solicitacaoCorrecao = false;
                if($params['agenda_id']){
                    $solicitacaoCorrecao = $mSolicitacaoCorrecao->getSolicitacaoEmAberto($params['agenda_id']);
                    if($solicitacaoCorrecao){
                        $mValorPagar->setDadosNull();
                        $mValorPagar->gerarValorNegativoAgenda($params['agenda_id'], $solicitacaoCorrecao['id']);
                    }
                }

                //Salvando os dados da consulta
                $mAgenda->salvarDadosConsulta($params);

                //gerando o valor da agenda, caso seja uma solicitação de correção
                if($solicitacaoCorrecao){
                    $mValorPagar->setDadosNull();
                    $mValorPagar->gerarValorAgenda($params['agenda_id']);
                    $mSolicitacaoCorrecao->finalizarSolicitacao($solicitacaoCorrecao['id']);
                }

                $mGuia = new Guia();
                $mGuia->arquivarGuiaByAgenda($params['agenda_id']);
                $mGuia->gerarGuiaByAgenda($params['agenda_id']);

                $mAgenda->commit();
                $db->sucessomAjax($_REQUEST['modulo'], setParam(['agenda_id' => $_POST['agenda_id'], 'id' => $p['id']], false));
            }catch (AbstractException $e) {
                $db->insucessomAjax($_REQUEST['modulo'], array(), $e->getMessage());
            }

            break;
        case 'verificarCpf':
            $mPessoaFisica = new PessoaFisica();

            die(json_encode(array(
                'disponivel' => $mPessoaFisica->isCpfDisponivel($_POST['cpf'], $_POST['paciente_id'])
            )));
            break;
        case 'buscarValoresProcedimento':
            include_once 'valoresProcedimento.inc';
            die;
            break;
        case 'carregarNumeroCartaoConvenio':
            $valorProcedimentoId = $_REQUEST['valor_procedimento_id'];
            $pacienteId = $_REQUEST['paciente_id'];
            $numeroCartao = '';
            if($valorProcedimentoId && $pacienteId) {
                $mValorProcedimento = new ValorProcedimento();
                $valorProcedimento = $mValorProcedimento->getTodosDados($valorProcedimentoId);

                $mConvenioPaciente = new ConvenioEstabelecimentoPaciente();
                $convenio = $mConvenioPaciente->getByPessoaConvenio($pacienteId, $valorProcedimento['convenio_id']);
                $numeroCartao = $convenio ? $convenio['numero'] : '';
                $validadeCartao = $convenio ? $convenio['validade'] : '';
            }

            die(json_encode([
                'numero_convenio_plano' => $numeroCartao,
                'validade_convenio_plano' => $validadeCartao,
            ]));
            break;
    }
}
$reagenda_id = $_REQUEST['reagenda_id'] ? $_REQUEST['reagenda_id'] : '';
$tipo_formulario = $_REQUEST['tipo_formulario'] ? $_REQUEST['tipo_formulario'] : '';

$mAgenda = new EstabelecimentoProfissionalAgenda($_REQUEST['agenda_id']);
$agenda = $mAgenda->getTodosDados($_REQUEST['agenda_id']);
$_REQUEST['paciente_id'] = utf8_decode($_REQUEST['paciente_id']);

if($tipo_formulario == 'alterar-dados-consulta') {
    extract($agenda);
    $_REQUEST['paciente_id'] = $agenda['pessoa_paciente_id'];
}

$paciente_id = $_REQUEST['paciente_id'];

if($paciente_id){
    $mPessoa = new PessoaFisica();

    if(is_numeric($paciente_id)){
        $mPessoaFisica = new PessoaFisica($paciente_id);
        $dadosPaciente = $mPessoaFisica->getDados();

        if($dadosPaciente){
            extract($dadosPaciente);

            $mContatoPessoaFisica = new ContatoPessoaFisica();
            $contatos = $mContatoPessoaFisica->listarEspecificosPorPessoaFisica($paciente_id);
            extract($contatos);
        }
    }else{
        $nome_completo = $paciente_id;
        $paciente_id = null;
    }
}

$tituloModal = 'Marcar consulta';
$tituloBtnSubmit = "Agendar";
$possuiGuia = false;
$msgGuia = 'Ao alterar os dados da consulta será gerada uma nova guia e a atual será arquivada.';
switch ($tipo_formulario) {
    case 'reagendar':
        $tituloModal = 'Reagendar consulta';
        $tituloBtnSubmit = 'Reagendar';
        break;
    case 'alterar-dados-consulta':
        $tituloModal = 'Alterar dados da consulta';
        $tituloBtnSubmit = 'Alterar dados';
        $mGuia = new Guia();
        $possuiGuia = $mGuia->isAgendaPossuiGuia($_REQUEST['agenda_id']);
        break;
}
?>
<input type="hidden" name="act" value="agendarConsulta" />
<input type="hidden" name="agenda_id" value="<?= $_REQUEST['agenda_id']; ?>" />
<input type="hidden" name="paciente_id" value="<?= $paciente_id; ?>" />
<input type="hidden" name="reagenda_id" value="<?= $reagenda_id; ?>" />
<input type="hidden" name="tipo_formulario" value="<?= $tipo_formulario; ?>" />

<div id="agenda-formulario">
    <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
        <h4 class="modal-title"><?= $tituloModal ?></h4>
    </div>
    <div class="modal-body">
        <div class="form">
            <?php if($possuiGuia) : ?>
                <div class="alert alert-warning">
                    <span>
                        <strong>Atenção!</strong>
                        <?= $msgGuia ?>
                    </span>
                </div>
            <?php endif; ?>

            <h4 class="form-section" style="margin-top: 0;">Dados da consulta</h4>
            <div class="row">
                <div class="col-sm-4">
                    <div class="form-group">
                        <label>Especialidade:</label>
                        <strong><?= $agenda['especialidade'] ?></strong>
                    </div>
                </div>
                <div class="col-sm-4">
                    <div class="form-group">
                        <label>Profissional:</label>
                        <strong><?= $agenda['profissional'] ?></strong>
                    </div>
                </div>
                <div class="col-sm-4">
                    <div class="form-group">
                        <label>Horário:</label>
                        <strong><?= substr($agenda['hora_inicio'], 0, 5) ?></strong>
                    </div>
                </div>
            </div>
            <div class="row">
                <div class="col-sm-12">
                    <div class="form-group">
                        <label>Procedimento</label>
                        <?php
                        $mProcedimento = new ProfissionalEstabelecimentoProcedimento();
                        $procedimentos = $mProcedimento->getComboProcedimentosPorProfissionalEstabelecimento($p['id'], $agenda['profissional_id']);
                        $db->monta_combom("profissional_estabelecimento_procedimento_id", $procedimentos, 'S', 'Selecione...', '', '', '', '', 'S');
                        ?>
                    </div>
                </div>
            </div>
            <div class="row">
                <div class="col-sm-12 form-group">
                    <label>Tipo de serviço</label>
                    <?php
                    $mTipoConsulta = new TipoConsulta();
                    $tiposConsulta = $mTipoConsulta->getComboPorEstabelecimento($p['id']);
                    $db->monta_combom("tipo_consulta_id", $tiposConsulta, 'S', 'Selecione...', '', '', '', '', 'S');
                    ?>
                </div>
            </div>

            <h4 class="form-section">Dados do paciente</h4>

            <div class="row">
                <div class="col-md-12">
                    <div class="form-group">
                        <label>Nome completo</label>
                        <div class="paciente">
                            <?= campo_textom('nome_completo', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>
            </div>

            <div class="row">
                <div class="col-md-3">
                    <div class="form-group">
                        <label>CPF</label>
                        <?= campo_textom('cpf', 'N', 'S', '', 19, 19, '999.999.999-99', '', '', '', 0); ?>
                    </div>
                </div>

                <div class="col-sm-3">
                    <div class="form-group">
                        <label>Cartão nacional de saúde</label>
                        <?= campo_textom('cartao_nacional_saude', 'N', 'S', '', 50, 40, '', '', '', '', 0); ?>
                    </div>
                </div>
                <div class="col-md-3">
                    <div class="form-group">
                        <label>Data de nascimento</label>
                        <?= campo_datam('data_nascimento', 'S', 'S', '', 'S'); ?>
                    </div>
                </div>
                <div class="col-md-3">
                    <div class="form-group">
                        <label>Sexo</label>
                        <?= campo_radiom('sexo', array("Masculino" => array("valor" => "M"), "Feminino" => array("valor" => "F")), 'S'); ?>
                    </div>
                </div>
            </div>

            <div class="row">
                <div class="col-sm-4 form-group">
                    <div class="valores-procedimento input-container">

                    </div>
                </div>
                <div class="col-sm-4 form-group numero-convenio-plano" style="display: none;">
                    <div class="input-container">
                        <label>Número do cartão do convênio/plano</label>
                        <?= campo_textom('numero_convenio_plano', 'N', 'S', '', 50, 40, '', '', '', '', 0); ?>
                    </div>
                </div>
                <div class="col-sm-4 form-group numero-convenio-plano" style="display: none;">
                    <div class="input-container">
                        <label>Validade do cartão do convênio/plano</label>
                        <?= campo_datam('validade_convenio_plano', 'N', 'S', '', 'S'); ?>
                    </div>
                </div>
            </div>

            <h4 class="form-section">Contatos</h4>
            <?php require APPRAIZ . "adm/modulos/principal/includes/contato/formulario.inc"; ?>

            <h4 class="form-section">Observação</h4>
            <div class="row">
                <div class="col-sm-12">
                    <div class="form-group">
                        <?= campo_textaream('observacao', 'N', 'S', '', '', '3', '1000', '', 0, '') ?>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div class="modal-footer">
        <div class="row">
            <div class="col-sm-4 text-left">
                <?php if( $_REQUEST['agenda_id'] && $_REQUEST['data_selecionada']
                    && $_REQUEST['pessoa_id'] && $_REQUEST['paciente_id']
                    && $_REQUEST['especialidade_id'] ) : ?>
                    <button type="button" class="btn green btn-selecionar-horario">
                        <i class="fa fa-arrow-left"></i>
                        Selecionar outro horário
                    </button>
                <?php endif; ?>
            </div>
            <div class="col-sm-8">
                <button type="submit" class="btn blue btn-marcar-consulta">
                    <i class="fa fa-check-circle"></i>
                    <?= $tituloBtnSubmit ?>
                </button>
                <button type="button" class="btn" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<style type="text/css">
    .selectize-control.single .selectize-input:after{
        display: none;
    }

    .selectize-control.single .selectize-input, .selectize-control.single .selectize-input input{
        padding: 6px 12px;
    }

    .control-label .required, .form-group .required{
        color: #555;
        font-size: 14px;
    }

    .paciente .selectize-control.single .selectize-input:after {
        display: none;
    }

    .paciente label {
        width: 120px;
        font-weight: bold;
    }
</style>

<script type="text/javascript">
    Selectize.define('paste_load', function(options) {
        var self = this;

        this.onPaste = (function() {
            var original = self.onPaste;
            return function(e) {
                if(e.type === "paste" ) {
                    this.refreshOptions(true);
                    return;
                }
                return original.apply(this, arguments);
            };
        })();
    });

    $(function(){
        var formMarcarConsulta = $("#form-marcar-consulta");
        var valorProcedimentoId = '<?= $agenda['valor_procedimento_id'] ?>';

        formMarcarConsulta.validateForm({
            rules : {
                cpf: {cpf: true},
                data_nascimento: {dateITA : true},
                "contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]" : {email : true},
            },
            submitHandler: function (e) {
                App.blockUI({
                    message: 'Carregando...',
                    target: '#form-marcar-consulta'
                });

                verificarCpf(formMarcarConsulta.find('#cpf').val(), function(){
                    var tipoFormulario = formMarcarConsulta.find("[name=tipo_formulario]").val();
                    if(tipoFormulario === 'alterar-dados-consulta') {
                        formMarcarConsulta.find("[name=act]").val("alterarDadosConsulta");
                    }

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType : 'json',
                        data: {
                            act : formMarcarConsulta.find('[name=act]').val(),
                            agenda_id : formMarcarConsulta.find('[name=agenda_id]').val(),
                            paciente_id : formMarcarConsulta.find('[name=paciente_id]').val(),
                            reagenda_id : formMarcarConsulta.find('[name=reagenda_id]').val(),
                            convenio_id : formMarcarConsulta.find('[name=convenio_id]').val(),
                            nome_completo : formMarcarConsulta.find('[name=nome_completo]').val(),
                            tipo_consulta_id : formMarcarConsulta.find('[name=tipo_consulta_id]').val(),
                            cartao_nacional_saude : formMarcarConsulta.find('[name=cartao_nacional_saude]').val(),
                            numero_convenio_plano : formMarcarConsulta.find('[name=numero_convenio_plano]').val(),
                            validade_convenio_plano : formMarcarConsulta.find('[name=validade_convenio_plano]').val(),
                            cpf : formMarcarConsulta.find('[name=cpf]').val(),
                            data_nascimento : formMarcarConsulta.find('[name=data_nascimento]').val(),
                            sexo : formMarcarConsulta.find('[name=sexo]:checked').val(),
                            "contatos[<?= TipoContato::TIPO_CELULAR ?>][contato]" : formMarcarConsulta.find('[name="contatos[<?= TipoContato::TIPO_CELULAR ?>][contato]"]').val(),
                            "contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][contato]" : formMarcarConsulta.find('[name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][contato]"]').val(),
                            "contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]" : formMarcarConsulta.find('[name="contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]"]').val(),
                            "contatos[<?= TipoContato::TIPO_CELULAR ?>][tipo_contato_sigla]" : formMarcarConsulta.find('[name="contatos[<?= TipoContato::TIPO_CELULAR ?>][tipo_contato_sigla]"]').val(),
                            "contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][tipo_contato_sigla]" : formMarcarConsulta.find('[name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][tipo_contato_sigla]"]').val(),
                            "contatos[<?= TipoContato::TIPO_EMAIL ?>][tipo_contato_sigla]" : formMarcarConsulta.find('[name="contatos[<?= TipoContato::TIPO_EMAIL ?>][tipo_contato_sigla]"]').val(),
                            observacao : formMarcarConsulta.find('[name=observacao]').val(),
                            valor_procedimento_id : formMarcarConsulta.find('[name=valor_procedimento_id]').val()
                        },
                        success: function (retorno) {
                            if(retorno.codigo === 1){
                                App.unblockUI('#form-marcar-consulta');

                                exibirMsg(retorno.msg);
                                $('#modal-marcar-consulta').modal('hide');
                                if(tipoFormulario === 'alterar-dados-consulta') {
                                    abrirAgenda(retorno.params);
                                } else {
                                    atualizarAgendaCalendario();
                                }
                            }else{
                                this.error(retorno);
                            }
                        },
                        error: function (result) {
                            App.unblockUI('#form-marcar-consulta');
                            exibirAlert(result.msg ? result.msg : 'Não foi possível incluir na agenda');
                        }
                    });
                }, function(){
                    App.unblockUI('#form-marcar-consulta');
                });
            }
        });

        $('.btn-selecionar-horario').click(function(){
            var content = $('#form-marcar-consulta-content');

            App.blockUI({
                message: 'Carregando...',
                target: '#form-marcar-consulta-content'
            });

            content.load(window.location.href, {
                agenda_id : '<?= $_REQUEST['agenda_id'] ?>',
                reagenda_id : '<?= $_REQUEST['reagenda_id'] ?>',
                act: 'formularioMarcarConsulta',
                data_selecionada : '<?= $_REQUEST['data_selecionada']; ?>',
                tipo_formulario : '<?= $_REQUEST['tipo_formulario']; ?>',
                pessoa_id : '<?= $_REQUEST['pessoa_id'] ?>',
                paciente_id : '<?= $_REQUEST['paciente_id'] ?>',
                especialidade_id : '<?= $_REQUEST['especialidade_id'] ?>',
            }, function () {
                App.unblockUI('#form-marcar-consulta-content');
            });
        });

        formMarcarConsulta.find('#cpf').change(function(){
            verificarCpf($(this).val());
        });

        adicionarDatePicker();
        // Destaca os campos required
        adicionarLabelObrigatorio(formMarcarConsulta.find(':required'));
        adicionarObrigatorio(formMarcarConsulta.find('.obrigatorio'));


        $('[name=nome_completo]').selectize({
            valueField: 'nome_completo',
            labelField: 'nome_completo',
            searchField: ['nome_completo', 'cpf', 'cartao_nacional_saude', 'data_nascimento_formatado'],
            plugins: ['restore_on_backspace', 'paste_load'],
            options: [],
            create: true,
            maxItems: 1,
            createOnBlur : true,
            persist : false,
            selectOnTab : true,
            render: {
                option_create: function(data, escape) {
                    return '<div class="create">Novo paciente: <strong>' + escape(data.input) + '</strong>&hellip;</div>';
                },
                option: function(item, escape) {
                    return '<div>' +
                        (item.cpf ? '<div><label>CPF: </label>' + escape(item.cpf) + '</div>' : '') +
                        (item.cartao_nacional_saude ? '<div><label>CNS: </label>' + escape(item.cartao_nacional_saude) + '</div>' : '') +
                        (item.nome_completo ? '<div><label>Nome: </label>' + escape(item.nome_completo) + '</div>' : '') +
                        (item.telefone ? '<div><label>Telefone: </label>' + escape(item.telefone) + '</div>' : '') +
                        (item.data_nascimento_formatado ? '<div><label>Nascimento: </label>' + escape(item.data_nascimento_formatado) + '</div>' : '') +
                        (item.nome_mae ? '<div><label>Nome da mãe: </label>' + escape(item.nome_mae) + '</div>' : '') +
                        '</div>';
                }
            },
            onChange : function (id){
                var container = $('#form-marcar-consulta');
                var selectize = $(container).find('[name=nome_completo]')[0].selectize;
                var pessoaSelecionada = selectize.options[id];

                if(pessoaSelecionada){
                    container.find('[name=paciente_id]').val(pessoaSelecionada.id);
                    container.find('[name=cpf]').val(pessoaSelecionada.cpf);

                    pessoaSelecionada.data_nascimento ?
                        container.find('[name=data_nascimento]').datepicker('setDate', new Date(pessoaSelecionada.data_nascimento + ' 00:00:00')) :
                        container.find('[name=data_nascimento]').val('');

                    container.find('[name=cartao_nacional_saude]').val(pessoaSelecionada.cartao_nacional_saude);
                    container.find('[id=radio_opcao_sexo_' + pessoaSelecionada.sexo_sigla + ']').prop('checked', true);
                    container.find('[name="contatos[<?= TipoContato::TIPO_CELULAR ?>][contato]"]').val(pessoaSelecionada.telefone);
                    container.find('[name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO?>][contato]"]').val(pessoaSelecionada.telefone_fixo);
                    container.find('[name="contatos[<?= TipoContato::TIPO_EMAIL?>][contato]"]').val(pessoaSelecionada.email);
                }else{
                    container.find('[name=paciente_id]').val();
                    container.find('[name=cpf]').val();
                    container.find('[name=data_nascimento]').val();
                    container.find('[name=sexo]').prop('checked', false);
                    container.find('[name=cartao_nacional_saude]').val();
                    container.find('[name="contatos[<?= TipoContato::TIPO_CELULAR ?>][contato]"]').val();
                    container.find('[name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO?>][contato]"]').val();
                    container.find('[name="contatos[<?= TipoContato::TIPO_EMAIL?>][contato]"]').val();
                }

                carregarNumeroCartaoConvenio();
            },
            load: function(query, callback) {
                if (query.length < 4) return callback();

                $.ajax({
                    url: window.location.href,
                    type: 'POST',
                    data : {
                        act : 'filtrarPacientesCombo',
                        busca : query
                    },
                    dataType : 'json',
                    error: function() {
                        callback();
                    },
                    success: function(res) {
                        callback(res.data);
                    }
                });
            }
        });

        var elemProfissionalEstabelecimentoProcedimento = $('[name=profissional_estabelecimento_procedimento_id]');
        elemProfissionalEstabelecimentoProcedimento.change(function(){
            var content = $('.valores-procedimento');
            $('#valor_procedimento_id').val('').trigger('change');

            App.blockUI({
                message: 'Carregando...',
                target: '.valores-procedimento'
            });

            content.load(window.location.href, {
                act: 'buscarValoresProcedimento',
                profissional_estabelecimento_procedimento_id : $(this).val(),
                valor_procedimento_id: valorProcedimentoId
            }, function () {
                App.unblockUI('.valores-procedimento');
            });
        });

        var profissionalEstabelecimentoProcedimentoId = elemProfissionalEstabelecimentoProcedimento.val();
        if(profissionalEstabelecimentoProcedimentoId != "") {
            elemProfissionalEstabelecimentoProcedimento.trigger('change');
        }
    });

    function verificarCpf(cpf, success, error){
        success = success ? success : function (result) { };
        error = error ? error : function (result) { };

        if(cpf === ''){
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
            dataType : 'json',
            data: {act : 'verificarCpf', cpf: cpf, paciente_id : $('[name=paciente_id]').val()},
            success: function (retorno) {
                App.unblockUI('.form-body');
                if(!retorno.disponivel){
                    var formMarcarConsulta = $("#form-marcar-consulta");

                    addMsgErroCampo(formMarcarConsulta.find('#cpf'), 'O CPF informado não está disponível.');
                    error();
                }else {
                    success(retorno);
                }
            },
            error: function (result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a operação');
                error();
            }
        });
    }

    function carregarNumeroCartaoConvenio() {
        let pacienteId = $('[name=paciente_id]').val();
        let valorProcedimentoId = $('#valor_procedimento_id').val();
        let campoNumeroConvenioPlano = $('#numero_convenio_plano');
        let campoValidadeConvenioPlano = $('#validade_convenio_plano');

        campoNumeroConvenioPlano.val('');
        campoValidadeConvenioPlano.val('');

        App.blockUI({target : '.numero-convenio-plano', message : 'Carregando...'});
        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType : 'json',
            data: {act : 'carregarNumeroCartaoConvenio', valor_procedimento_id: valorProcedimentoId, paciente_id : pacienteId},
            success: function (retorno) {
                App.unblockUI('.numero-convenio-plano');
                if(retorno && retorno.numero_convenio_plano) {
                    campoNumeroConvenioPlano.val(retorno.numero_convenio_plano);
                    campoValidadeConvenioPlano.datepicker("update", toDateObj(retorno.validade_convenio_plano));
                }
            },
            error: function (result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a operação');
            }
        });
    }
</script>