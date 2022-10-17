<?php
$p = getParam();
$mAgenda = new EstabelecimentoProfissionalAgenda();
$data_selecionada = $_REQUEST['data_selecionada'] ? $_REQUEST['data_selecionada'] : $data_selecionada;

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'agendaListaHorarios' :
        case 'selecionarPacienteConsulta' :
            include_once 'agendaListaHorarios.inc';
            die;
            break;
    }
}

$arWhere = $mAgenda->montaWhereAgendaDisponivel(array_merge($_REQUEST, $p));
$arWhere[] = "epa.inicio >= now()";
$qtdEventos = $mAgenda->getQtdAgendas($arWhere, true);
?>

<div class="row">
    <div class="col-md-3">
        <div class="text-center">
            <div id="agenda-mini-calendar"></div>
        </div>
    </div>
    <div class="col-md-9">
        <div class="tabela-agenda-horarios-container">
            <?php include_once 'agendaListaHorarios.inc'; ?>
        </div>
    </div>
</div>


<script type="text/javascript">
    $(function(){
        $('.agenda-content #agenda-mini-calendar').datepicker({
            language: 'pt-BR',
            todayHighlight : true,
            beforeShowDay : function (date){
                var qtdEventos = <?= $qtdEventos ?>;
                var data = moment(date).format('YYYYMMDD');
                var bubleEvents = qtdEventos[data] ? '<div class="buble-events">' + qtdEventos[data] + '</div>' : '';

                return {
                    content : '<div class="day-content">' + moment(date).format('D') + bubleEvents + '</div>'
                }
            }
        }).on('changeDate', function(e) {
            var content = $('.tabela-agenda-horarios-container');

            App.blockUI({
                message: 'Carregando...',
                target: '.tabela-agenda-horarios-container'
            });

            content.load(window.location.href, {
                act: 'agendaListaHorarios',
                data_selecionada : moment(e.date).format('YYYY-MM-DD'),
                reagenda_id : '<?= $_REQUEST['reagenda_id'] ?>',
                tipo_formulario : '<?= $_REQUEST['tipo_formulario'] ?>',
                pessoa_id : $('[name=pessoa_id]').val(),
                especialidade_id : $('[name=especialidade_id]').val(),
            }, function () {
                App.unblockUI('.tabela-agenda-horarios-container');
            });
        });

        <?php if($data_selecionada) : ?>
            $('.agenda-content #agenda-mini-calendar').datepicker("update", new Date('<?= $data_selecionada ?> 00:00:00'));
        <?php endif; ?>
    });
</script>