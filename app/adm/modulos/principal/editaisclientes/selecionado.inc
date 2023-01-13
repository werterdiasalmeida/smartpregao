<?php
/* Objetos usados na tela */
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$mEditalClienteMonitoramento = new EditalClienteMonitoramento();
$mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
$fileSimec = new FilesSimec();
$p = getParam();
/* Ações da tela */
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
            /* Acompanhamentos 
        - Habilitação / Documentação
        - Pedido de vistoria
        - Pedido de esclarecimentos
        - Pedido de impugnação
        - Avaliação dos concorrentes
        - Pedido de recurso
        - POC
        - Documentação para Contratação
        - Documentação do Contrato e Aditivos
        - Acompanhamento (problemas e soluções)
        - Evidências e Entregas
        */
        case 'salvarAcompanhamento':
            $params = $_POST;
            $mEditalClienteAcompanhamento->salvarAcompanhamento($params, $_FILES);
            $mEditalClienteAcompanhamento->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;
        case 'excluirAcompanhamento':
            $params = $_REQUEST;
            $mEditalClienteAcompanhamento->excluir(intval($params['id_acompanhamento']));
            $mEditalClienteAcompanhamento->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;
            /* Monitoramentos 
            - Monitoramento do Pregão
            */
        case 'salvarMonitoramento':
            $params = $_POST;
            $mEditalClienteMonitoramento->salvarMonitoramento($params);
            $mEditalClienteMonitoramento->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $params['editalcliente_id'])));
            break;
        case 'excluirMonitoramento':
            $params = $_GET;
            $mEditalClienteMonitoramento->excluir(intval($params['id_monitoramento']));
            $mEditalClienteMonitoramento->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;
        case 'baixarAnexo':
            $fileSimec->getDownloadArquivo($_REQUEST['arqid']);
            die;
            break;
    }
}
/* Carrega os dados da tela */
$editalCliente = $_REQUEST['p'];
if (!is_int($editalCliente)) {
    $editalCliente = getParam();
    $editalCliente = $editalCliente['id'];
}
$mEditalCliente->carregarPorId($editalCliente);
$dadosEditalCliente = $mEditalCliente->getDados();

/* Verifica se já possui um DOCID, senão, atribui um */
if (empty($dadosEditalCliente['docid'])) {
    $dadosEditalCliente['docid'] = wf_cadastrarDocumento(17, 'Fluxo do Edital');
    $docid = $mEditalCliente->popularDadosObjeto($dadosEditalCliente)->salvar(true, true, $camposNulos);
} else {
    $docid = $dadosEditalCliente['docid'];
}
$dadosCliente = $mEstabelecimento->getTodosDados($dadosEditalCliente['estabelecimento_id']);
$dados = $mEdital->getDadosCompletos($editalCliente);
$editalCliente = setParam(array('id' => $editalCliente), false);
$param['id'] = $dados['id_edital'];
$p = setParam($param, true);

/* Escolhe qual a aba será selecionada ao abrir */
$abaSelecionada = 'btn_tab_pre_pregao';
switch (true) {
    case ($dados['situacao'] == 'Participação Realizada'
        || $dados['situacao'] == '<i>Participação Não Realizada</i>'
        || $dados['situacao'] == '<i> Ajustar a DOCUMENTAÇÃO </i>'):
        $abaSelecionada = 'btn_tab_pos_pregao';
        break;
    case ($dados['situacao'] == 'Em Monitoramento'):
        $abaSelecionada = 'btn_tab_pos_pregao';
        break;
    case ($dados['situacao'] == 'Em processo de contratação'):
        $abaSelecionada = 'btn_tab_contratacao';
        break;
    case ($dados['situacao'] == 'Contratado'):
        $abaSelecionada = 'btn_tab_gestao_contrato';
        break;
}
/* Caeçalho */
include APPRAIZ . "includes/cabecalhom.inc";
getStrMsgDanger();
getStrMsgInfo();
getStrMsgWarning();
?>
<style>
    .card-header {
        cursor: pointer;
        margin: 0;
        display: -webkit-box;
        display: -ms-flexbox;
        -webkit-box-pack: start;
        -ms-flex-pack: start;
        justify-content: flex-start;
        -webkit-box-align: center;
        -ms-flex-align: center;
        -webkit-transition: all 0.15s ease;
        transition: all 0.15s ease;
        position: relative;
        display: -webkit-box;
        display: -ms-flexbox;
        display: flex;
        -ms-flex-direction: column;
        flex-direction: column;
        min-width: 0;
        background-color: #B0E0E6;
        background-clip: border-box;
        border: 1px solid #EBEDF3;
        padding: 8px;
    }

    .accordion_com_conteudo {
        color: green;
    }
</style>
<script>
    $(document).ready(function() {
        $("#<?= $abaSelecionada ?>").click();
    });

    function editar(id) {
        url = '?modulo=principal/editais/formulario&acao=C' + id;
        window.open(url);
    }
    // Monitoramento
    function excluirMonitoramento(id) {
        if (confirm('Deseja apagar este monitoramento?')) {
            url = '?modulo=principal/editaisclientes/selecionado&acao=C&p=<?= $_REQUEST['p'] ?>&act=excluirMonitoramento&id_monitoramento=' + id;
            location.href = url;
        }
    }

    function exibirModalNovoMonitoramento() {
        var modal = $('#modal-novo-monitoramento');
        modal.modal('show');
    }
    // Acompanhamento
    function excluirAcompanhamento(id) {
        if (confirm('Deseja apagar este registro?')) {
            url = '?modulo=principal/editaisclientes/selecionado&acao=C&p=<?= $_REQUEST['p'] ?>&act=excluirAcompanhamento&id_acompanhamento=' + id;
            location.href = url;
        }
    }
    function exibirModalNovoAcompanhamento(tipo) {
        $('#titulo_acompanhamento').html(tipo);
        $('#tipo_acompanhamento').val(tipo);
        var modal = $('#modal-novo-acompanhamento');
        modal.modal('show');
    }

    function baixarAnexo(arqid){
        url = '?modulo=principal/editaisclientes/selecionado&acao=C&p=<?= $_REQUEST['p'] ?>&act=baixarAnexo&arqid=' + arqid; 
        location.href = url;
    }
</script>
<h4>
    <button type="button" class="btn btn-voltar" onclick="history.go(-1)">
        <i class="fa fa-arrow-left"></i> Voltar
    </button>
    <b><?php echo $dados['orgao'] ?></b>
    <i style="cursor: pointer;" title="Alterar informações do Edital" class="fa fa-edit" onclick="editar('<?= $p ?>')"></i>
    <spam class="badge badge-primary"><b><?php echo $dados['situacao'] ?></b></spam>
</h4>
<ul class="nav nav-tabs nav-fill">
    <li class="nav-item">
        <a class="nav-link active" data-toggle="tab" href="#kt_tab_pre_pregao" id="btn_tab_pre_pregao">
            <span class="nav-text"> Pré Pregão</span>
        </a>
    </li>
    <li class="nav-item">
        <a class="nav-link" data-toggle="tab" href="#kt_tab_pos_pregao" id="btn_tab_pos_pregao">
            <span class="nav-text">Pós Pregão</span>
        </a>
    </li>
    <li class="nav-item">
        <a class="nav-link" data-toggle="tab" href="#kt_tab_contratacao" id="btn_tab_contratacao">
            <span class="nav-text">Contratação</span>
        </a>
    </li>
    <li class="nav-item">
        <a class="nav-link" data-toggle="tab" href="#kt_tab_gestao_contrato" id="btn_tab_gestao_contrato">
            <span class="nav-text"> Gestão do contrato</span>
        </a>
    </li>
    <li class="nav-item">
        <a class="nav-link" data-toggle="tab" href="#kt_tab_tramitacao_anexos">
            <span class="nav-text">Tramitações e Anexos</span>
        </a>
    </li>
</ul>
<div class="tab-content bordered">
    <?php
    require_once('selecionado/pre_pregao.inc');
    require_once('selecionado/pos_pregao.inc');
    require_once('selecionado/contatacao.inc');
    require_once('selecionado/gestao_contato.inc');
    require_once('selecionado/tramitacoes_anexos.inc');
    ?>
</div>
<!-- MODAL de Monitoramento -->
<div class="modal fade in" id="modal-novo-monitoramento" tabindex="-1" role="modal-novo-monitoramento" aria-hidden="true">
    <div class="modal-dialog">
        <form role="form" name="formulario" id="form-monitoramento" class="edital-monitoramento-form" method="POST" enctype="multipart/form-data">
            <input type="hidden" name="act" value="salvarMonitoramento" />
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Novo Monitoramento</h4>
                </div>
                <div class="modal-body">
                    <input type="hidden" name="editalcliente_id" value="<?php echo $dadosEditalCliente['id'] ?>" />
                    <div id="div_novo-monitoramento">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Data da ação</label>
                                <?php
                                $data = date("m/d/Y");
                                echo campo_datam('data', 'S', 'S', 'S', 'S', '', '', $data); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Horário da ação</label>
                                <?php
                                $hora = '00:00';
                                echo campo_textom('hora', 'S', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Comentários</label>
                                <?= campo_textaream('comentario', 'S', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="submit" onclick="$('#form-monitoramento').submit();" class="btn green" data-dismiss="modal">Salvar</button>
                    <button type="button" class="btn red" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </form>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>
<!-- MODAL de Acompanhamento -->
<div class="modal fade in" id="modal-novo-acompanhamento" tabindex="-1" role="modal-novo-acompanhamento" aria-hidden="true">
    <div class="modal-dialog">
        <form role="form" name="formulario" id="form-acompanhamento" method="POST" enctype="multipart/form-data">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title" id="titulo_acompanhamento"></h4>
                </div>
                <div class="modal-body">
                    <input type="hidden" name="editalcliente_id" value="<?php echo $dadosEditalCliente['id'] ?>" />
                    <input type="hidden" name="tipo_acompanhamento" id="tipo_acompanhamento" value="" />
                    <input type="hidden" name="act" value="salvarAcompanhamento" />
                    <div id="div_novo-acompanhamento">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Data de referência</label>
                                <?php
                                $data = date("m/d/Y");
                                echo campo_datam('data', 'S', 'S', 'S', 'S', '', '', $data); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Arquivo</label>
                                <input type="file" id="arquivo" name="arquivo" class="form-control" />
                            </div>
                        </div>
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Comentários</label>
                                <?= campo_textaream('comentario', 'S', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="submit" onclick="$('#form-acompanhamento').submit();" class="btn green" data-dismiss="modal">Salvar</button>
                    <button type="button" class="btn red" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </form>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>