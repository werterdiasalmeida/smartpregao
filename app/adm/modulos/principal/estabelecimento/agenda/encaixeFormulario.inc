<?php
$mEstabelecimentoEspecialidadeProfissional = new EstabelecimentoEspecialidadeProfissional();
$mAgenda = new EstabelecimentoProfissionalAgenda();
$p = getParam();
$profissional_id = $_REQUEST['profissional_id'] ? $_REQUEST['profissional_id'] : '';

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'incluirEncaixe' :
            try {
                $params = array_merge($_POST, getParam());
                $params = utf8ArrayDecode($params);

                $estabelecimentoId = $params['id'];
                $params['id'] = isset($params['agenda_id']) ? $params['agenda_id'] : null;

                $mAgenda->salvarEncaixe($params);
                $mAgenda->commit();

                $db->sucessomAjax($_REQUEST['modulo'], array(
                    'id' => $estabelecimentoId
                ));

            } catch (AgendaIndisponivel $e) {
                $db->insucessomAjax($_REQUEST['modulo'], array(), $e->getMessage());
            } catch (ValorProcedimentoInvalido $e) {
                $db->insucessomAjax($_REQUEST['modulo'], array(), $e->getMessage());
            } catch (ProfissionalIndisponivel $e) {
                $db->insucessomAjax($_REQUEST['modulo'], array(), $e->getMessage());
            } catch (SalaIndisponivel $e) {
                $db->insucessomAjax($_REQUEST['modulo'], array(), $e->getMessage());
            }
            break;
    }
}

$data_agenda = $_POST['data_agenda'];
$hora_inicio = substr($_POST['hora_inicio'], 0, 5);
$alteracao = false;
$habilitado = 'S';
?>
<style type="text/css">
    .input-group-addon-desabilitado{
        display: table-cell;
        padding: 0 5px;
    }

    .input-group-addon-desabilitado:first-child{
        padding-left: 0;
    }

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

<div id="agenda-formulario">
    <input type="hidden" name="p" value="<?= setParam(getParam(),false); ?>" />
    <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
        <h4 class="modal-title">Incluir encaixe</h4>
    </div>
    <div class="modal-body">

        <h4 class="form-section" style="margin-top: 0;">Dados da consulta</h4>

        <div class="row">
            <div class="col-sm-6">
                <div class="form-group">
                    <label>Profissional</label>
                    <?php
                    $profissionais = $mEstabelecimentoEspecialidadeProfissional->getComboProfissionaisPorEstabelecimento($p['id']);
                    $db->monta_combom("profissional_id", $profissionais, $habilitado, 'Selecione...', '', '', '', '', 'S');
                    ?>
                </div>
            </div>
            <div class="col-sm-6">
                <div class="form-group">
                    <label class="obrigatorio">Especialidade</label>
                    <div id="combo-especialidade-container">
                        -
                    </div>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-sm-12">
                <div class="form-group">
                    <label class="obrigatorio">Procedimento</label>
                    <div id="combo-procedimento-container">
                        -
                    </div>
                </div>
            </div>
        </div>
        <div class="row">
            <div class="col-sm-12 form-group">
                <label>Tipo de serviço</label>
                <?php
                $mTipoConsulta = new TipoConsulta();
                $tiposConsulta = $mTipoConsulta->getComboPorEstabelecimento($p['id']);
                $db->monta_combom("tipo_consulta_id", $tiposConsulta, 'S', 'Selecione...', 'exibeSala', '', '', '', 'S');
                ?>
            </div>
        </div>
        <script>
            function exibeSala(tipo_consulta)
            {
                if(tipo_consulta == '<?=TIPO_CONSULTA_REMOTA?>' || tipo_consulta == '<?=TIPO_CONSULTA_RETORNO_REMOTO?>')
                {
                    $('#div_select_sala').hide();
                }else{
                    $('#div_select_sala').show();
                }
            }
            
        </script>

        <?php
        $mSala = new SalaEstabelecimento();
        $comboSalas = $mSala->getComboPorEstabelecimento($p['id']);
        if(count($comboSalas) > 0): ?>
            <div class="row" id="div_select_sala">
                <div class="col-sm-12">
                    <div class="form-group">
                        <label>Sala</label>
                        <?php
                        $db->monta_combom("sala_estabelecimento_id", $comboSalas, $habilitado, 'Selecione...', '', '', '', '', 'N', '', false);
                        ?>
                    </div>
                </div>
            </div>
        <?php endif; ?>
        <div class="row">
            <div class="col-sm-4">
                <div class="form-group">
                    <label>Data</label>
                    <?php
                    echo campo_datam('data_agenda', 'S', $habilitado, '', 'S');
                    ?>
                </div>
            </div>

            <div class="col-sm-8">
                <div class="form-group">
                    <div class="form-group horario">
                        <label>
                            Horário
                        </label>
                        <div class="input-group">
                            <span class="<?= $habilitado == 'S' ? 'input-group-addon' : 'input-group-addon-desabilitado' ?>">De</span>
                            <div>
                                <?= campo_textom('hora_inicio', 'S', $habilitado, '', 10, 10, '99:99', '', '', '', 0, '', '', $hora_inicio, '', null, null, null, 'timepicker timepicker-24 startdate'); ?>
                            </div>
                            <span class="<?= $habilitado == 'S' ? 'input-group-addon' : 'input-group-addon-desabilitado' ?>">Até</span>
                            <div>
                                <?= campo_textom('hora_fim', 'S', 'S', '', 10, 10, '99:99', '', '', '', 0, '', '', $hora_fim, '', null, null, null, 'timepicker timepicker-24 enddate', $habilitado == 'S'); ?>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <h4 class="form-section">Dados do paciente</h4>

        <div class="row">
            <div class="col-md-12">
                <div class="form-group">
                    <label>Nome completo</label>
                    <div class="paciente">
                        <?= campo_textom('nome_completo', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        <input type="hidden" name="paciente_id" value="" />
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

        </div> <!-- row -->
    </div>
    <div class="modal-footer">
        <?php if($habilitado === 'S') : ?>
            <button type="submit" class="btn blue btn-incluir">
                <i class="fa fa-plus"></i>
                Incluir encaixe
            </button>
        <?php endif; ?>
        <button type="button" class="btn" data-dismiss="modal">Fechar</button>
    </div>
</div>
<input type="hidden" name='act' value="incluirEncaixe" />

<script type="text/javascript">
    $(function(){
        var formEncaixe = $("#form-encaixe");

        adicionarLabelObrigatorio(formEncaixe.find(':required, .obrigatorio'));
        adicionarDatePicker(formEncaixe);

        formEncaixe.find('[name=profissional_id]').change(function(){
            App.blockUI({
                message: 'Carregando...',
                target: '#form-encaixe #combo-especialidade-container'
            });

            formEncaixe.find('#combo-especialidade-container').load(window.location.href, {
                act: 'filtrarEspecialidades',
                profissional_id: $(this).val(),
            }, function () {
                App.unblockUI('#form-encaixe #combo-especialidade-container');
            });

            App.blockUI({
                message: 'Carregando...',
                target: '#form-encaixe #combo-procedimento-container'
            });

            $('#form-encaixe').find('.valores-procedimento').html('');

            formEncaixe.find('#combo-procedimento-container').load(window.location.href, {
                act: 'filtrarProcedimentos',
                profissional_id: $(this).val(),
            }, function () {
                App.unblockUI('#form-encaixe #combo-procedimento-container');
                formEncaixe.find('[name=profissional_estabelecimento_procedimento_id]').change(function(){
                    var content = $('#form-encaixe').find('.valores-procedimento');

                    App.blockUI({
                        message: 'Carregando...',
                        target: '#form-encaixe .valores-procedimento'
                    });

                    content.load(window.location.href, {
                        act: 'buscarValoresProcedimento',
                        profissional_estabelecimento_procedimento_id : $(this).val(),
                    }, function () {
                        App.unblockUI('#form-encaixe .valores-procedimento');
                    });
                });
            });
        });

        setTimePicker();

        formEncaixe.validateForm({
            rules : {
                cpf: {cpf: true},
                data_agenda: {dateITA: true},
                data_nascimento: {dateITA: true},
                "contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]" : {email : true},
            },
            submitHandler: function (e) {
                var form = formEncaixe;
                $('div.horario.hidden').remove();

                App.blockUI({
                    message: 'Carregando...',
                    target: '#form-encaixe-content'
                });

                verificarCpf(formEncaixe.find('#cpf').val(), function(){
                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType : 'json',
                        data: {
                            act : formEncaixe.find('[name=act]').val(),
                            profissional_id : form.find('[name=profissional_id]').val(),
                            estabelecimento_especialidade_profissional_id : form.find('[name=estabelecimento_especialidade_profissional_id]').val(),
                            sala_estabelecimento_id : form.find('[name=sala_estabelecimento_id]').val(),
                            data_agenda : form.find('[name=data_agenda]').val(),
                            hora_inicio : form.find('[name="hora_inicio"]').val(),
                            hora_fim : form.find('[name="hora_fim"]').val(),
                            paciente_id : formEncaixe.find('[name=paciente_id]').val(),
                            reagenda_id : formEncaixe.find('[name=reagenda_id]').val(),
                            convenio_id : formEncaixe.find('[name=convenio_id]').val(),
                            nome_completo : formEncaixe.find('[name=nome_completo]').val(),
                            tipo_consulta_id : formEncaixe.find('[name=tipo_consulta_id]').val(),
                            cartao_nacional_saude : formEncaixe.find('[name=cartao_nacional_saude]').val(),
                            numero_convenio_plano : formEncaixe.find('[name=numero_convenio_plano]').val(),
                            validade_convenio_plano : formEncaixe.find('[name=validade_convenio_plano]').val(),
                            cpf : formEncaixe.find('[name=cpf]').val(),
                            data_nascimento : formEncaixe.find('[name=data_nascimento]').val(),
                            sexo : formEncaixe.find('[name=sexo]:checked').val(),
                            "contatos[<?= TipoContato::TIPO_CELULAR ?>][contato]" : formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_CELULAR ?>][contato]"]').val(),
                            "contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][contato]" : formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][contato]"]').val(),
                            "contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]" : formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]"]').val(),
                            "contatos[<?= TipoContato::TIPO_CELULAR ?>][tipo_contato_sigla]" : formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_CELULAR ?>][tipo_contato_sigla]"]').val(),
                            "contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][tipo_contato_sigla]" : formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][tipo_contato_sigla]"]').val(),
                            "contatos[<?= TipoContato::TIPO_EMAIL ?>][tipo_contato_sigla]" : formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_EMAIL ?>][tipo_contato_sigla]"]').val(),
                            observacao : formEncaixe.find('[name=observacao]').val(),
                            valor_procedimento_id : formEncaixe.find('[name=valor_procedimento_id]').val()
                        },
                        success: function (retorno) {
                            if(retorno.codigo === 1){
                                App.unblockUI('#form-encaixe-content');

                                fecharModalAlterarEncaixe(retorno.msg);
                            }else{
                                this.error(retorno);
                            }
                        },
                        error: function (result) {
                            App.unblockUI('#form-encaixe-content');
                            exibirAlert(result.msg ? result.msg : 'Não foi possível incluir na agenda');
                        }
                    });
                }, function(){
                    App.unblockUI('#form-encaixe-content');
                });
            }
        });

        $.validator.addMethod("enddate", function(value, element) {
            var starttime = $('.startdate').val();
            var endtime = value;

            var dt = new Date();
            //convert both time into timestamp
            var stt = new Date((dt.getMonth() + 1) + "/" + dt.getDate() + "/" + dt.getFullYear() + " " + starttime);
            var endt = new Date((dt.getMonth() + 1) + "/" + dt.getDate() + "/" + dt.getFullYear() + " " + endtime);

            return endt > stt;
        }, "O horário final deve ser depois do horário inicial.");

        formEncaixe.find('[name=data_agenda]:not(.disabled)').datepicker('destroy').datepicker({
            orientation: "left",
            autoclose: true,
            language: 'pt-BR',
            startDate: new Date()
        });

        formEncaixe.find('#cpf').change(function(){
            verificarCpf($(this).val());
        });

        adicionarDatePicker();

        formEncaixe.find('[name=nome_completo]').selectize({
            valueField: 'nome_completo',
            labelField: 'nome_completo',
            searchField: ['nome_completo', 'cpf', 'cartao_nacional_saude', 'data_nascimento_formatado'],
            plugins: ['restore_on_backspace'],
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
                var formEncaixe = $('#form-encaixe');
                var selectize = $(formEncaixe).find('[name=nome_completo]')[0].selectize;
                var pessoaSelecionada = selectize.options[id];

                if(pessoaSelecionada){
                    formEncaixe.find('[name=paciente_id]').val(pessoaSelecionada.id);
                    formEncaixe.find('[name=cpf]').val(pessoaSelecionada.cpf);
                    pessoaSelecionada.data_nascimento ?
                        formEncaixe.find('[name=data_nascimento]').datepicker('setDate', new Date(pessoaSelecionada.data_nascimento + ' 00:00:00')) :
                        formEncaixe.find('[name=data_nascimento]').val('');
                    formEncaixe.find('[name=cartao_nacional_saude]').val(pessoaSelecionada.cartao_nacional_saude);
                    formEncaixe.find('[id=radio_opcao_sexo_' + pessoaSelecionada.sexo_sigla + ']').prop('checked', true);
                    formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_CELULAR ?>][contato]"]').val(pessoaSelecionada.telefone);
                    formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO?>][contato]"]').val(pessoaSelecionada.telefone_fixo);
                    formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_EMAIL?>][contato]"]').val(pessoaSelecionada.email);
                }else{
                    formEncaixe.find('[name=paciente_id]').val();
                    formEncaixe.find('[name=cpf]').val();
                    formEncaixe.find('[name=data_nascimento]').val();
                    formEncaixe.find('[name=sexo]').prop('checked', false);
                    formEncaixe.find('[name=cartao_nacional_saude]').val();
                    formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_CELULAR ?>][contato]"]').val();
                    formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO?>][contato]"]').val();
                    formEncaixe.find('[name="contatos[<?= TipoContato::TIPO_EMAIL?>][contato]"]').val();
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
    });

    function setTimePicker(){
        $('.timepicker-24').timepicker({
            autoclose: true,
            minuteStep: 1,
            showSeconds: false,
            showMeridian: false,
            defaultTime : null,
            format : 'HH:mm'
        }).on('keydown', function(e) {
            if(e.keyCode === 9) {
                $(this).timepicker('hideWidget');
            }
        }).on('show.timepicker', function(e) {
            if(e.time.value === ''){
                $(this).timepicker('setTime', '08:00');
            }
        });
    }

    function fecharModalAlterarEncaixe(msg){
        if(msg){
            exibirMsg(msg);
        }

        $('#modal-encaixe').modal('hide');
        atualizarAgendaCalendario();
    }

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
                    var formEncaixe = $('#form-encaixe');
                    addMsgErroCampo(formEncaixe.find('#cpf'), 'O CPF informado não está disponível.');
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

    function calcularHorarioFinal(){
        // apenas para o método ser definido
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