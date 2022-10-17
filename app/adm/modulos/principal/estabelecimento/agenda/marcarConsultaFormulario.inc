<?php
$mEstabelecimentoProfissional = new EstabelecimentoEspecialidadeProfissional();
$especialidade_id = $_REQUEST['especialidade_id'] ? $_REQUEST['especialidade_id'] : '';
$pessoa_id = $_REQUEST['pessoa_id'] ? $_REQUEST['pessoa_id'] : '';
$paciente_id = $_REQUEST['paciente_id'] ? utf8_decode($_REQUEST['paciente_id']) : '';
$agenda_id = $_REQUEST['agenda_id'] ? $_REQUEST['agenda_id'] : '';
$reagenda_id = $_REQUEST['reagenda_id'] ? $_REQUEST['reagenda_id'] : '';
$tipo_formulario = $_REQUEST['tipo_formulario'] ? $_REQUEST['tipo_formulario'] : '';

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'agendaMarcarConsulta' :
        case 'agendaListaHorarios' :
        case 'selecionarPacienteConsulta' :
            include_once 'agendaMarcarConsulta.inc';
            die;
            break;
    }
}

$paciente = null;

if($reagenda_id){
    $mAgenda = new EstabelecimentoProfissionalAgenda();
    $dadosAgenda = $mAgenda->getTodosDados($reagenda_id);

    $pessoa_id = $dadosAgenda['pessoa_id'];
    $especialidade_id = $dadosAgenda['especialidade_id'];
    $data_selecionada = $dadosAgenda['data_inicio'];
    $hora_selecionada = $dadosAgenda['hora_inicio'];
    $paciente_id = $dadosAgenda['pessoa_paciente_id'];

    $_REQUEST['especialidade_id'] = $especialidade_id;
}

if($paciente_id){
    $mPessoa = new PessoaFisica();

    if(is_numeric($paciente_id)){
        $paciente = $mPessoa->getList(array("pes.id = {$paciente_id}"), false, null, false, false);
    }

    if($paciente){
        $paciente = array_shift($paciente);
    }else{
        $paciente = array('nome_completo' => $paciente_id, 'id' => $paciente_id);
    }
}

if($agenda_id){
    $mAgenda = new EstabelecimentoProfissionalAgenda();
    $dadosAgenda = $mAgenda->getTodosDados($agenda_id);

    $pessoa_id = $dadosAgenda['pessoa_id'];
    $especialidade_id = $dadosAgenda['especialidade_id'];
    $data_selecionada = $dadosAgenda['data_inicio'];
    $hora_selecionada = $dadosAgenda['hora_inicio'];

    $_REQUEST['especialidade_id'] = $especialidade_id;
    $_REQUEST['data_selecionada'] = $data_selecionada;
    $_REQUEST['hora_selecionada'] = $hora_selecionada;
}

$tituloModal = 'Marcar agenda/procedimento';

switch ($tipo_formulario) {
    case 'reagendar':
        $tituloModal = 'Reagendar consulta/procedimento';
        break;
}
?>

<style type="text/css">
    .paciente .selectize-control.single .selectize-input:after {
        display: none;
    }

    .paciente label {
        width: 120px;
        font-weight: bold;
    }
</style>
<div id="agenda-formulario">
    <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
        <h4 class="modal-title"><?= $tituloModal ?></h4>
    </div>

    <div class="modal-body">
        <div class="row">
            <div class="form-group col-md-6">
                <label>Especialidade</label>
                <?php
                $especialidades = $mEstabelecimentoProfissional->getComboEspecialidadesPorEstabelecimento($p['id']);
                $especialidades = array_merge(array(array('codigo' => '-1', 'descricao' => 'Todos')), $especialidades);
                $db->monta_combom("especialidade_id", $especialidades, 'S', '', '', '', '', '', 'N');
                ?>
            </div>

            <div class="form-group col-md-6">
                <label>Profissional</label>
                <?php
                $profissionais = $mEstabelecimentoProfissional->getComboPessoasProfissionaisPorEstabelecimento($p['id']);
                $profissionais = array_merge(array(array('codigo' => '-1', 'descricao' => 'Todos')), $profissionais);
                $db->monta_combom("pessoa_id", $profissionais, 'S', '', '', '', '', '', 'N');
                ?>
            </div>
        </div>

        <div class="row">
            <div class="col-md-12">
                <label class="obrigatorio">Paciente:</label>
                <div class="paciente">
                    <?= $db->monta_combom("paciente_id", array(), 'S', '', '', '', '', '', 'S'); ?>
                </div>
            </div>
        </div>

        <div class="row" style="margin-top: 20px;">
            <div class="col-md-12">
                <div class="agenda-content">
                    <?php include_once 'agendaMarcarConsulta.inc'; ?>
                </div>
            </div>
        </div>
    </div>
    <div class="modal-footer">
        <button type="button" class="btn green btn-selecionar-horario">
            <i class="fa fa-check-circle"></i>
            Confirmar dados
        </button>
        <button type="button" class="btn" data-dismiss="modal">Fechar</button>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        $('[name=pessoa_id], [name=especialidade_id]').selectize({
            onChange : function(){
                carregarAgendaMarcacao();
            }
        });

        $('[name=paciente_id]').selectize({
            valueField: 'id',
            labelField: 'nome_completo',
            searchField: ['nome_completo', 'cpf', 'cartao_nacional_saude', 'data_nascimento_formatado'],
            plugins: ['restore_on_backspace'],
            options: [],
            create: true,
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
            load: function(query, callback) {
                if (query.length < 4) return callback();

                $.ajax({
                    url: window.location.href,
                    type: 'POST',
                    data : {
                        act : 'filtrarPacientesCombo',
                        busca : (query)
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

        <?php if($tipo_formulario == 'reagendar') : ?>
            $('[name=paciente_id]').selectize()[0].selectize.disable();
        <?php endif; ?>

        adicionarObrigatorio($('#agenda-formulario .obrigatorio'));

        <?php if($paciente_id) : ?>
            var selectizePaciente = $('[name=paciente_id]')[0].selectize;
            selectizePaciente.clearOptions();

            var data = <?= json_encode(utf8ArrayEncode($paciente)) ?>;
            selectizePaciente.addOption(data);
            selectizePaciente.setValue('<?= $paciente_id; ?>');
        <?php endif; ?>
    });

    function carregarAgendaMarcacao(){
        var content = $('.agenda-content');

        App.blockUI({
            message: 'Carregando...',
            target: '.agenda-content'
        });

        content.load(window.location.href, {
            act: 'agendaMarcarConsulta',
            pessoa_id : $('[name=pessoa_id]').val(),
            reagenda_id : '<?= $_REQUEST['reagenda_id'] ?>',
            tipo_formulario : '<?= $tipo_formulario ?>',
            especialidade_id : $('[name=especialidade_id]').val(),
        }, function () {
            App.unblockUI('.agenda-content');
        });
    }
</script>