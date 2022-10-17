<?php
if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'selecionarPacienteConsulta' :
            include_once 'pacienteConsultaFormulario.inc';
            die;
            break;
    }
}

$p = getParam();
$mAgenda = new EstabelecimentoProfissionalAgenda();
$_REQUEST['data_selecionada'] = $_REQUEST['data_selecionada'] ? $_REQUEST['data_selecionada'] : date('Y-m-d');

$arWhere = $mAgenda->montaWhereAgendaDisponivel(array_merge($_REQUEST, $p));
$arWhere[] = "epa.inicio >= now()";
$horarios = $mAgenda->getHorarios($arWhere, true);
?>

<table id="tabela-agenda-horarios" data-toggle="table" data-unique-id="id"
       data-pagination="true" data-height="500">
    <thead>
    <tr>
        <th data-field="selecionado" data-radio="true"></th>
        <th data-field="data_agenda" data-align="center">Data</th>
        <th data-field="hora_inicio" data-align="center">Horário</th>
        <th data-field="profissional">Profissional</th>
        <th data-field="especialidade">Especialidade</th>
        <th data-field="sala">Sala</th>
    </tr>
    </thead>
</table>

<script type="text/javascript">
    $(function(){
        var tabelaHorarios = $('.agenda-content #tabela-agenda-horarios');
        tabelaHorarios.bootstrapTable({data : <?= $horarios ?>});

        $('.btn-selecionar-horario').unbind('click').click(selecionarPacienteConsulta);

        <?php if($hora_selecionada) : ?>
            var horarios = tabelaHorarios.bootstrapTable('getData').map(function(row){
                row.selecionado = '<?= substr($hora_selecionada, 0, 5) ?>' === row.hora_inicio;
                return row;
            });

            tabelaHorarios.bootstrapTable("load", horarios);
        <?php endif; ?>
    });

    function selecionarPacienteConsulta(){
        var content = $('#modal-marcar-consulta');

        var horarioSelecionado = content.find('#tabela-agenda-horarios').bootstrapTable('getData').filter(function(row){
            return row.selecionado;
        }).shift();

        if(!horarioSelecionado){
            exibirAviso('Selecione um horário');
            return;
        }

        var paciente_id = content.find('[name=paciente_id]').val();
        if(!paciente_id){
            exibirAviso('Informe o paciente');
            return;
        }

        abrirFormularioPacienteConsulta({
            agenda_id : horarioSelecionado.id,
            act: 'selecionarPacienteConsulta',
            reagenda_id : '<?= $_REQUEST['reagenda_id']; ?>',
            tipo_formulario : '<?= $_REQUEST['tipo_formulario'] ?>',
            data_selecionada : '<?= $_REQUEST['data_selecionada']; ?>',
            pessoa_id : content.find('[name=pessoa_id]').val(),
            paciente_id : paciente_id,
            especialidade_id : content.find('[name=especialidade_id]').val(),
        });
    }
</script>