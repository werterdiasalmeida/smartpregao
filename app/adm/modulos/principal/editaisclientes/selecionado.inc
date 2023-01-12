<?php
/* Objetos usados na tela */
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$mEditalClienteMonitoramento = new EditalClienteMonitoramento();

/* Ações da tela */
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'salvarMonitoramento':
            $params = $_POST;
            $mEditalClienteMonitoramento->salvarMonitoramento($params);
            $mEditalClienteMonitoramento->commit();
            unset($_POST);
            break;
        case 'excluirMonitoramento':
            $params = $_REQUEST;
            $mEditalClienteMonitoramento->excluir(intval($params['id_monitoramento']));
            $mEditalClienteMonitoramento->commit();
            break;
        case 'carregarServicoEdital':
            $params['servico'] = $_REQUEST['servico'];
            $params['edital_cliente_id'] = $_REQUEST['edital_cliente_id'];
            require APPRAIZ . "adm/modulos/principal/editaisclientes/detalhe_servico.inc";
            die;
            break;
        case 'adicionarCalendario':
            $horainicio = explode(':', $dados['hora_limite']);
            $horainicio = $horainicio[0] + 3;
            $dados['hora_limite'] = $horainicio . ':00';
            $dadosAgenda['inicio'] = $dados['data_limite'] . ' ' . $dados['hora_limite'];
            $horafim = explode(':', $dados['hora_limite']);
            $dadosAgenda['fim'] = $dados['data_limite'] . ' ' . $horafim[0] . ':30';
            $dadosAgenda['titulo'] = 'Pregão - ' . $dados['orgao'];
            $dadosAgenda['descricao'] = 'Pregão - ' . $dados['orgao'] . ' - ' . $dados['pregao'] . ' - ' . $dados['objeto'];
            $dadosAgenda['local'] = $dados['portal'];
            $event = new ICS(
                $dadosAgenda['inicio'],
                $dadosAgenda['fim'],
                $dadosAgenda['titulo'],
                $dadosAgenda['descricao'],
                $dadosAgenda['local']
            );
            $event->show();
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

/* Caeçalho */
include APPRAIZ . "includes/cabecalhom.inc";
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
        $("#btn_tab_pre_pregao").click();
    });

    function editar(id) {
        url = '?modulo=principal/editais/formulario&acao=C' + id;
        window.open(url);
    }

    function excluirMonitoramento(id) {
        if (confirm('Deseja apagar este monitoramento?')) {
            url = '?modulo=principal/editaisclientes/servicos_pregao&acao=C&p=<?= $_REQUEST['p'] ?>&act=excluirMonitoramento&id_monitoramento=' + id;
            location.href = url;
        }
    }


    function adicionarCalendario(id) {
        url = '?modulo=principal/editaisclientes/servicos_pregao&acao=C&act=adicionarCalendario&p=' + id;;
        location.href = url;
    }


    function exibirModalNovoMonitoramento() {
        var modal = $('#modal-novo-monitoramento');
        modal.modal('show');
    }
</script>
<div class="modal fade in" id="modal-novo-monitoramento" tabindex="-1" role="modal-novo-monitoramento" aria-hidden="true">
    <div class="modal-dialog">
        <form role="form" name="formulario" id="form-monitoramento" class="edital-monitoramento-form" method="POST" enctype="multipart/form-data">
            <input type="hidden" name="act" value="salvarMonitoramento" />
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Adicionar Monitoramento</h4>
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
                    <button type="submit" onclick="$('#form-monitoramento').submit();" class="btn red" data-dismiss="modal">Salvar</button>
                    <button type="button" class="btn blue" data-dismiss="modal">Fechar</button>
                </div>
            </div>
        </form>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>
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
        <a class="nav-link" data-toggle="tab" href="#kt_tab_pregao">
            <span class="nav-text">Pregão</span>
        </a>
    </li>
    <li class="nav-item">
        <a class="nav-link" data-toggle="tab" href="#kt_tab_pos_pregao">
            <span class="nav-text">Pós Pregão</span>
        </a>
    </li>
    <li class="nav-item">
        <a class="nav-link" data-toggle="tab" href="#kt_tab_contratacao">
            <span class="nav-text">Contratação</span>
        </a>
    </li>
    <li class="nav-item">
        <a class="nav-link" data-toggle="tab" href="#kt_tab_gestao_contrato">
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
    require_once('selecionado/pregao.inc');
    require_once('selecionado/pos_pregao.inc');
    require_once('selecionado/contatacao.inc');
    require_once('selecionado/gestao_contato.inc');
    require_once('selecionado/tramitacoes_anexos.inc');
    ?>
</div>