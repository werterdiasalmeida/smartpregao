<?php
ini_set("memory_limit", "1024M");
/* Objetos usados na tela */
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$mEstabelecimentoAgenda = new EstabelecimentoAgenda();
$p = getParam();
if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'salvarAgenda':
            $params = array_merge($_POST, getParam());
            $params = utf8ArrayDecode($params);
            $estabelecimentoId = $params['id'];
            $mEstabelecimentoAgenda->salvarAgenda($params);
            $mEstabelecimentoAgenda->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $estabelecimentoId)));
            break;
        case 'excluirAgenda':
            $params = $_GET;
            $mEstabelecimentoAgenda->excluir(intval($params['id_agenda']));
            $mEstabelecimentoAgenda->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;
    }
}
include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();
getStrMsgDanger();
getStrMsgInfo();
getStrMsgWarning();
$dadosCliente = $mEstabelecimento->getTodosDados($p['id']);
$where = "estabelecimento_id = {$p['id']}";
$dadosAgenda = $mEstabelecimentoAgenda->getDadosCalendario($where);
?>
<script src="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.css" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/fullcalendar/lang/pt-br.js" type="text/javascript"></script>
<div class="portlet light bordered" id="page-body">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-calendar font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Agenda da <?= $dadosCliente['razao_social'] ?>
            </span>
        </div>
        <br><br>
        <a onclick="exibirModalNovaAgenda()">
            <i class="bi bi-plus-square" title="Nova Agenda"></i>
            Nova Agenda
        </a>
        <br><br>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-lg-12">
                <div class="portlet light calendar bordered" style="min-height: 500px; background-color: #FFF;">
                    <div id="div_calendario" class="has-toolbar"></div>
                </div>
            </div>
        </div>
        <br />
    </div>
</div>
<script>
    $(document).ready(function() {
        $('#div_calendario').fullCalendar({
            header: {
                right: 'month,agendaWeek,agendaDay,listWeek'
            },
            defaultDate: '<? echo $dadosAgenda['hoje'] ?>',
            navLinks: true,
            editable: false,
            eventLimit: true,
            events: <? echo $dadosAgenda['agenda'] ?>
        });
    });
    // Excluir Agenda
    function excluirAgenda(id) {
        if (confirm('Deseja apagar esta Agenda?')) {
            url = '?modulo=principal/estabelecimento/agenda&acao=C&p=<?= $_REQUEST['p'] ?>&act=excluirAgenda&id_agenda=' + id;
            location.href = url;
        }
    }
    // Nova Agenda
    function exibirModalNovaAgenda() {
        var modal = $('#modal-nova-agenda');
        modal.modal('show');
    }
</script>
<!-- MODAL de Agenda -->
<div class="modal fade in" id="modal-nova-agenda" tabindex="-1" role="modal-nova-agenda" aria-hidden="true">
    <div class="modal-dialog">
        <form role="form" name="formulario" id="form-agenda" method="POST" enctype="multipart/form-data">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title" id="titulo_agenda"></h4>
                </div>
                <div class="modal-body">
                    <input type="hidden" name="estabelecimento_id" value="<?php echo $dadosCliente['id'] ?>" />
                    <input type="hidden" name="act" value="salvarAgenda" />
                    <div id="div_nova-agenda">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Data</label>
                                <?php
                                $data = date("m/d/Y");
                                echo campo_datam('data_agenda', 'S', 'S', 'S', 'S', '', '', $data); ?>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="form-group">
                                <label>Horário Início</label>
                                <?php
                                $hora = '08:00';
                                echo campo_textom('hora_inicio', 'S', 'S', '', 5, 5, '##:##', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="form-group">
                                <label>Horário Fim</label>
                                <?php
                                $hora = '09:00';
                                echo campo_textom('hora_fim', 'S', 'S', '', 5, 5, '##:##', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Compromisso</label>
                                <?php
                                echo campo_textom('compromisso', 'S', 'S', '', 100, 100, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Link</label>
                                <?php
                                $url = 'http://';
                                echo campo_textom('url', 'N', 'S', '', 100, 100, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="submit" onclick="$('#form-agenda').submit();" class="btn green" data-dismiss="modal">Salvar</button>
                    <button type="button" class="btn red" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </form>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>