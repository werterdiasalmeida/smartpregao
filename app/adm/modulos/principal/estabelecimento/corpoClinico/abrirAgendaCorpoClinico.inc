<?php
$p = getParam();
$mProfissional = new Profissional();
$profissional = $mProfissional->getTodosDados($p['profissional_id']);

$mSala = new SalaEstabelecimento();
$comboSalas = $mSala->getComboPorEstabelecimento($p['id']);
$possuiSalas = count($comboSalas) > 0;

$especialidade_profissional_id = $_REQUEST['especialidade_profissional_id'];
$mes = $_REQUEST['mes'];
$ano = $_REQUEST['ano'];
$dias = $_REQUEST['dias'] ? json_encode(array_map(function($dia){
    $dia['selecionado'] = $dia['selecionado'] === 'true';
    return $dia;
}, $_REQUEST['dias'])) : null;
?>
<div class="row">
    <div class="col-sm-2 text-right">
        <?php montaImgFoto($profissional['foto_arquivo_id'], $profissional['sexo'], '150px', '', '200', '200', 'img-circle bg-white') ?>
    </div>

    <div class="col-sm-10 text-left">
        <div class="row">
            <div class="col-md-12">
                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Nome</label>
                            <br>
                            <strong><?= $profissional['nome_completo'] ?></strong>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-md-8">
                        <div class="form-group">
                            <label class="obrigatorio">Especialidade</label>
                            <?php
                            $mEspecialidadePro = new EspecialidadeProfissional();
                            $especialidades = $mEspecialidadePro->listarPorProfissional($profissional['id'], false, $p['id']);
                            $especialidadesJson = json_encode(utf8ArrayEncode($especialidades));
                            $especialidades = array_map(function($especialidade){
                                return array(
                                    'codigo' => $especialidade['id'],
                                    'descricao' => $especialidade['especialidade']
                                );
                            }, $especialidades);

                            if(count($especialidades) == 1){
                                $especialidade_profissional_id = $especialidades[0]['codigo'];
                            }

                            $db->monta_combom("especialidade_profissional_id", $especialidades, 'S', 'Selecione...', '', '', '', '', 'N');
                            ?>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Duração da consulta</label>
                            <br>
                            <strong id="duracao-consulta">-</strong>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-8">
                        <div class="form-group">
                            <label class="obrigatorio">Mês</label>
                            <?php
                            if(empty($mes)){
                                $mes = $mes ? $mes : date('m');
                                $mes = $mes + 1 > 12 ? 1 : $mes + 1;
                            }
                            $db->monta_combom("mes", getComboMes(), 'S', '', '', '', '', '', 'N');
                            ?>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label class="obrigatorio">Ano</label>
                            <?php
                            if(empty($ano)){
                                $ano = $ano ? $ano : date('Y');
                                $ano = date('m') == 12 ? $ano + 1 : $ano;
                            }

                            echo campo_textom('ano', 'N', 'S', '', 100, 4, '9999', '');
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-12">
        <table id="tabela-dias" data-toggle="table" class="table table-striped" data-unique-id="id"
               data-pagination="false">
            <thead>
                <tr>
                    <th colspan="2"></th>
                    <th colspan="<?= ($possuiSalas ? 3 : 2) ?>" data-align="center">Manhã</th>
                    <th colspan="<?= ($possuiSalas ? 3 : 2) ?>" data-align="center">Tarde</th>
                </tr>
                <tr>
                    <th data-field="selecionado" data-checkbox="true"></th>
                    <th data-field="dia_semana">Dia da semana</th>
                    <th data-field="hora_inicio_manha" data-align="center" data-formatter="campoHoraInicioManha" data-width="12%">Hora início</th>
                    <th data-field="hora_fim_manha" data-align="center" data-formatter="campoHoraFimManha" data-width="12%">Hora fim</th>
                    <?php if($possuiSalas) : ?>
                        <th data-field="sala_manha" data-align="center" data-formatter="campoSalaManha" data-width="12%">Sala</th>
                    <?php endif; ?>
                    <th data-field="hora_inicio_tarde" data-align="center" data-formatter="campoHoraInicioTarde" data-width="12%">Hora início</th>
                    <th data-field="hora_fim_tarde" data-align="center" data-formatter="campoHoraFimTarde" data-width="12%">Hora fim</th>
                    <?php if($possuiSalas) : ?>
                        <th data-field="sala_tarde" data-align="center" data-formatter="campoSalaTarde" data-width="12%">Sala</th>
                    <?php endif; ?>
                </tr>
            </thead>
            <tbody>
            </tbody>
        </table>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        var container = $('.abrir-agenda-form');
        adicionarObrigatorio(container.find('.obrigatorio'));

        container.find('[name=especialidade_profissional_id]').change(function(){
            var duracaoConsulta = getDuracaoConsulta();
            container.find('#duracao-consulta').html(duracaoConsulta === 0 ? '-' : duracaoConsulta + ' <small>minutos</small>');
        }).change();

        container.find('#tabela-dias').bootstrapTable({
            <?php if($dias) : ?>
                data: <?= $dias ?>,
            <?php else : ?>
                data: [
                    {id: 0, selecionado : false, dia_semana : 'Domingo', dia_semana_orig : 'Domingo'},
                    {id: 1, selecionado : false, dia_semana : 'Segunda', dia_semana_orig : 'Segunda'},
                    {id: 2, selecionado : false, dia_semana : 'Terça', dia_semana_orig : 'Terça'},
                    {id: 3, selecionado : false, dia_semana : 'Quarta', dia_semana_orig : 'Quarta'},
                    {id: 4, selecionado : false, dia_semana : 'Quinta', dia_semana_orig : 'Quinta'},
                    {id: 5, selecionado : false, dia_semana : 'Sexta', dia_semana_orig : 'Sexta'},
                    {id: 6, selecionado : false, dia_semana : 'Sábado', dia_semana_orig : 'Sábado'}
                ],
            <?php endif; ?>
            onCheck : function (){
                atualizarValoresTabela();
                refreshTable();
            },
            onUncheck : function (item){
                item = limparItem(item);
                adicionarItemTabela(item, container.find('#tabela-dias'), item.id);
                atualizarValoresTabela();
                refreshTable();
            },
            onCheckAll : function (){
                atualizarValoresTabela();
                refreshTable();
            },
            onUncheckAll : function (itens){
                itens.forEach(function(item){
                    item = limparItem(item);
                    adicionarItemTabela(item, container.find('#tabela-dias'), item.id);
                });
                atualizarValoresTabela();
                refreshTable();
            },
            onPostBody : function (){
                $('.timepicker-24').timepicker({
                    autoclose: true,
                    minuteStep: 5,
                    showSeconds: false,
                    showMeridian: false,
                    defaultTime : null,
                    format : 'HH:mm'
                }).on('keydown', function(e) {
                    if(e.keyCode === 9) {
                        $(this).timepicker('hideWidget');
                    }
                }).inputmask("99:99", {placeholder: ''});

                $('.hora_inicio_manha, .hora_fim_manha')
                    .on('show.timepicker', function(e) {
                        if(e.time.value === ''){
                            $(this).timepicker('setTime', '08:00');
                        }
                    })
                    .on('changeTime.timepicker', function(e) {
                        var h = e.time.hours;
                        var m = e.time.minutes;

                        //convert hours into minutes
                        m += h*60;

                        //720 minutes = 12h*60m
                        if(m > 720) {
                            $(this).timepicker('setTime', '12:00');
                        }

                        atualizarQtdConsultas(this);
                    });

                $('.hora_inicio_tarde, .hora_fim_tarde').on('changeTime.timepicker', function(e) {
                    var h = e.time.hours;
                    var m = e.time.minutes;

                    //convert hours into minutes
                    m += h*60;

                    //720 minutes = 12h*60m
                    if(m < 720) {
                        $(this).timepicker('setTime', '12:00');
                    }

                    atualizarQtdConsultas(this);
                });

                $('.timepicker-24, .sala_manha, .sala_tarde').focus(function(){
                    var id = $(this).prop('id').split('_').pop();
                    var table = $('#tabela-dias');
                    var dados = table.bootstrapTable('getData');
                    var tr = table.find('tr[data-index=' + id + ']');

                    if(!dados[id].selecionado){
                        table.bootstrapTable('getData')[id].selecionado = true;
                        tr.addClass('selected').find('input[type=checkbox]').prop('checked', true);
                    }

                    verificarCheckAll(table);
                }).blur(function(){
                    var id = $(this).prop('id').split('_').pop();
                    var table = $('#tabela-dias');
                    var dados = table.bootstrapTable('getData');
                    var tr = table.find('tr[data-index=' + id + ']');

                    if(dados[id].selecionado
                        && tr.find('.hora_inicio_manha').val() === ''
                        && tr.find('.hora_fim_manha').val() === ''
                        && tr.find('.sala_manha').val() === ''
                        && tr.find('.hora_inicio_tarde').val() === ''
                        && tr.find('.hora_fim_tarde').val() === ''
                        && tr.find('.sala_tarde').val() === ''
                    ){
                        table.bootstrapTable('getData')[id].selecionado = false;
                        tr.removeClass('selected').find('input[type=checkbox]').prop('checked', false);
                    }

                    verificarCheckAll(table);
                });
            }
        }).bootstrapTable('refresh');

        container.find('.btn-voltar').addClass('hidden');
        container.find('.btn-salvar')
            .removeClass('hidden')
            .html('<i class="fa fa-search"></i> Visualizar')
            .unbind('click').click(function(){
                var especialidadeProfissionalId = container.find('[name=especialidade_profissional_id]').val();
                var mes = container.find('[name=mes]').val();
                var ano = container.find('[name=ano]').val();

                atualizarValoresTabela();
                var dias = container.find('#tabela-dias').bootstrapTable("getData");

                if(especialidadeProfissionalId === ''){
                    addMsgErroCampo(container.find('[name=especialidade_profissional_id]'), 'Campo obrigatório.');
                }

                if(mes === ''){
                    addMsgErroCampo(container.find('[name=mes]'), 'Campo obrigatório.');
                }

                if(ano === ''){
                    addMsgErroCampo(container.find('[name=ano]'), 'Campo obrigatório.');
                }

                var dataValida = true;
                var dia = null;
                for (var i = 0; i < dias.length; i++) {
                    dia = dias[i];
                    if((dia.hora_inicio_manha && dia.hora_fim_manha && isHoraInicioMaior(dia.hora_inicio_manha, dia.hora_fim_manha))
                    || (dia.hora_inicio_tarde && dia.hora_fim_tarde && isHoraInicioMaior(dia.hora_inicio_tarde, dia.hora_fim_tarde))){
                        dataValida = false;
                        exibirAviso('A hora início deve ser anterior a fim.');
                        break;
                    }
                }

                if(especialidadeProfissionalId && mes && ano && dias.length && dataValida){
                    App.blockUI({
                        message: 'Carregando...',
                        target: '.abrir-agenda-form'
                    });

                    $('#abrir-agenda').load(window.location.href, {
                        p: '<?= setParam(getParam(), false) ?>',
                        act: 'previsualizar-agenda',
                        especialidade_profissional_id : especialidadeProfissionalId,
                        mes : mes,
                        ano : ano,
                        dias : dias
                    }, function () {
                        App.unblockUI('.abrir-agenda-form');
                    });
                }
        });

        container.find('[name=especialidade_profissional_id]').change(function(){
            atualizarValoresTabela();
        });
    });

    function getDuracaoConsulta(){
        var container = $('.abrir-agenda-form');
        var especialidadeProfissional = container.find('[name=especialidade_profissional_id]').val();
        var especialidades = <?= $especialidadesJson ?>;

        if(especialidadeProfissional === ''){
            return 0;
        }

        var especialidadeSelecionada = especialidades.filter(function(especialidade){
            return especialidade.id === especialidadeProfissional;
        }).shift();

        return especialidadeSelecionada.duracao_consulta
    }

    function refreshTable(){
        atualizarTabela($('#tabela-dias'));
    }

    function campoHora(value, row, index, campo){
        // var disabled = !row.selecionado ? 'disabled' : '';
        var disabled = '' ;
        value = value ? value : '';

        return '<input type="text" id="' + campo + '_' + row.id + '" name="' + campo + '[' + row.id + ']" '
            + disabled + ' value="' + value + '" class="normal  form-control hora timepicker timepicker-24 ' + campo + '" />';
    }

    function campoSala(value, row, index, campo){
        // var disabled = !row.selecionado ? 'disabled' : '';
        var disabled = '' ;
        value = value ? value : '';

        return '<select id="' + campo + '_' + row.id + '" name="' + campo + '[' + row.id + ']" class="CampoEstilo form-control bs-select ' + campo + '" ' + disabled + '>' +
                   '<option value=""></option>' +
                   <?php foreach($comboSalas as $sala) : ?>
                       '<option value="<?= $sala['codigo'] ?>" ' + ('<?= $sala['codigo'] ?>' === value ? 'selected' : '' ) + ' ><?= $sala['descricao'] ?></option>' +
                   <?php endforeach; ?>
               '</select>';
    }

    function campoHoraInicioManha(value, row, index){
        return campoHora(value, row, index, 'hora_inicio_manha');
    }

    function campoHoraFimManha(value, row, index){
        return campoHora(value, row, index, 'hora_fim_manha');
    }

    function campoHoraInicioTarde(value, row, index){
        return campoHora(value, row, index, 'hora_inicio_tarde');
    }

    function campoHoraFimTarde(value, row, index){
        return campoHora(value, row, index, 'hora_fim_tarde');
    }

    function campoSalaManha(value, row, index){
        return campoSala(value, row, index, 'sala_manha');
    }

    function campoSalaTarde(value, row, index){
        return campoSala(value, row, index, 'sala_tarde');
    }

    function atualizarValoresTabela(){
        var table = $('#tabela-dias');
        var dados = table.bootstrapTable('getData').map(function(item){
            if(item.selecionado){
                item.hora_inicio_manha = $('#hora_inicio_manha_' + item.id).val();
                item.hora_fim_manha = $('#hora_fim_manha_' + item.id).val();
                item.sala_manha = $('#sala_manha_' + item.id).val();

                item.hora_inicio_tarde = $('#hora_inicio_tarde_' + item.id).val();
                item.hora_fim_tarde = $('#hora_fim_tarde_' + item.id).val();
                item.sala_tarde = $('#sala_tarde_' + item.id).val();
            }

            item.dia_semana = getLabelQtdConsultas(item);
            return item;
        });

        table.bootstrapTable("load", dados);
    }

    function isHoraInicioMaior(horaInicio, horaFim){
        var inicio = horaInicio;
        var dataInicio = new Date();
        dataInicio.setHours(inicio[0]);
        dataInicio.setMinutes(inicio[1]);

        var fim = horaFim;
        var dataFim = new Date();
        dataFim.setHours(fim[0]);
        dataFim.setMinutes(fim[1]);

        return inicio > fim;
    }

    function verificarCheckAll(table){
        if(table.find('input[type=checkbox][name=btSelectItem]').length === table.find('input[type=checkbox][name=btSelectItem]:checked').length){
            table.find('input[type=checkbox][name=btSelectAll]').prop('checked', true);
        }else{
            table.find('input[type=checkbox][name=btSelectAll]').prop('checked', false);
        }
    }

    function calcularQtdConsultas(horaInicial, horaFinal){
        var dataAtual = new Date();
        var inicio = new Date(moment(dataAtual).format('YYYY-MM-DD') + ' ' + horaInicial + ':00');
        var fim = new Date(moment(dataAtual).format('YYYY-MM-DD') + ' ' + horaFinal + ':00');
        var duracaoConsulta = getDuracaoConsulta();

        if(fim > inicio && duracaoConsulta){
            var diferencaMinutos = (fim - inicio)/ 60000;
            return Math.trunc(diferencaMinutos / duracaoConsulta);
        }

        return 0;
    }

    function atualizarQtdConsultas(elem){
        var id = $(elem).prop('id').split('_').pop();
        var table = $('#tabela-dias');
        var dados = table.bootstrapTable('getData');
        var tr = table.find('tr[data-index=' + id + ']');

        dados[id].hora_inicio_manha = tr.find('.hora_inicio_manha').val();
        dados[id].hora_fim_manha = tr.find('.hora_fim_manha').val();
        dados[id].hora_inicio_tarde = tr.find('.hora_inicio_tarde').val();
        dados[id].hora_fim_tarde = tr.find('.hora_fim_tarde').val();

        var labelQtdConsultas = getLabelQtdConsultas(dados[id]);
        tr.find('td:eq(1)').html(labelQtdConsultas);
    }

    function getLabelQtdConsultas(item){
        var qtdConsultasManha = 0;
        if(item.hora_inicio_manha && item.hora_fim_manha){
            qtdConsultasManha = calcularQtdConsultas(item.hora_inicio_manha, item.hora_fim_manha);
        }

        var qtdConsultasTarde = 0;
        if(item.hora_inicio_tarde && item.hora_fim_tarde){
            qtdConsultasTarde = calcularQtdConsultas(item.hora_inicio_tarde, item.hora_fim_tarde);
        }

        var qtdTotalConsultas = qtdConsultasManha + qtdConsultasTarde;

        if(qtdTotalConsultas){
            item.dia_semana = item.dia_semana_orig + ' <br />' +
                '<small>' + qtdTotalConsultas + (qtdTotalConsultas === 1 ? ' consulta' : ' consultas') + '</small>';
        }else{
            item.dia_semana = item.dia_semana_orig;
        }

        return item.dia_semana;
    }

    function limparItem(item){
        item.hora_inicio_manha = '';
        item.hora_fim_manha = '';
        item.hora_inicio_tarde = '';
        item.hora_fim_tarde = '';
        item.sala_manha = '';
        item.sala_tarde = '';

        return item;
    }
</script>