<?php
ini_set("memory_limit", "10240M");
/* Objetos usados na tela */
$mEstabelecimento = new Estabelecimento();
$mCotacaoDispensa = new CotacaoDispensa();
$mCDCCliente = new CDCCliente();
$mCDCClienteAcompanhamento = new CDCClienteAcompanhamento();
$fileSimec = new FilesSimec();
$p = getParam();
/* Carrega os dados da tela */
$CDCCliente = $_REQUEST['p'];
if (!is_int($CDCCliente)) {
    $CDCCliente = getParam();
    $CDCCliente = $CDCCliente['id'];
}
$mCDCCliente->carregarPorId($CDCCliente);
$dadosCDCCliente = $mCDCCliente->getDados();
$dadosCliente = $mEstabelecimento->getTodosDados($dadosCDCCliente['estabelecimento_id']);
$dados = $mCotacaoDispensa->getDadosCompletos($CDCCliente);
extract($dados);
$valor_estimado = formata_valor($valor_estimado);
/* Ações da tela */

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {

        case 'salvarCadastro':
            $params = array_merge($_POST, getParam());
            $params['valor_estimado'] = floatval(formata_valor_sql($params['valor_estimado']));
            $params['id'] = $mCDCCliente->salvarCDCCliente($params);
            $mCDCCliente->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $params['id'])));
            break;
        
            /* Acompanhamentos */
        case 'salvarAcompanhamento':
            $params = $_POST;
            $mCDCClienteAcompanhamento->salvarAcompanhamento($params, $_FILES);
            $mCDCClienteAcompanhamento->commit();

            /* Adiciona agenda, caso esteja preenchido o horário de início */
            if ($params['hora_inicio']) {
                $params['estabelecimento_id'] = $dadosCDCCliente['estabelecimento_id'];
                $params['data_agenda'] = $params['data'];
                if (!$params['hora_fim']) {
                    $params['hora_fim'] = substr($params['hora_inicio'], 0, 2) . ':59';
                }
                $params['compromisso'] = $dados['orgao'] . ' [' . $params['tipo_acompanhamento'] . ']';
                $params['url'] = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER["REQUEST_URI"];
                $mEstabelecimentoAgenda->salvarAgenda($params);
                $mEstabelecimentoAgenda->commit();
            }

            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;
        case 'excluirAcompanhamento':
            $params = $_REQUEST;
            $mCDCClienteAcompanhamento->excluir(intval($params['id_acompanhamento']));
            $mCDCClienteAcompanhamento->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;

        case 'baixarAnexo':
            $fileSimec->getDownloadArquivo($_REQUEST['arqid']);
            die;
            break;
    }
}

/* Verifica se já possui um DOCID, senão, atribui um */
if (empty($dadosCDCCliente['docid'])) {
    $dadosCDCCliente['docid'] = wf_cadastrarDocumento(18, 'Fluxo do CDC');
    $docid = $mCDCCliente->popularDadosObjeto($dadosCDCCliente)->salvar(true, true, $camposNulos);
} else {
    $docid = $dadosCDCCliente['docid'];
}
$CDCCliente = setParam(array('id' => $CDCCliente), false);
$param['id'] = $dados['id_edital'];
$p = setParam($param, true);

/* Escolhe qual a aba será selecionada ao abrir */
$abaSelecionada = 'btn_tab_cadastro';

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

    .listagem-marcado {
        background-color: #B4EEB4;
        display: block;
    }

    .listagem-remover {
        display: none;
    }

    .modal-dialog {
        width: 95%;
        margin: 30px auto;
    }
</style>
<script>
    /**
     * Maravilhoso Pesquisator! (werter@gmail.com)
     *
     */

    /**
     * Recebe um pesquisator que está na tela (class = busca-listagem-pesquisator)
     *
     * @param {htmlObject} InstÃ¢ncia do pesquisator.
     * @returns {jQuery} Tabela de lista mais prÃ³xima do pesquisator.
     */
    function getTabelaDoPesquisator(pesquisator) {
        return $(pesquisator).parent().find('table');
    }

    /**
     * Ativa o pesquisator relacionando-o Ã  tabela de listagem na sequencia.
     */
    function enablePesquisator() {
        $.expr[':'].contains = function(a, i, m) {
            return $(a).html().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $(document).on('keyup', 'input.busca-listagem-pesquisator', function() {
            var $tbody = getTabelaDoPesquisator(this).children('tbody');
            $('tr td', $tbody).removeClass('listagem-marcado');
            $('tr', $tbody).removeClass('listagem-remover');
            stringPesquisa = $(this).val();
            if (stringPesquisa) {
                $('tr td:contains(' + stringPesquisa + ')', $tbody).addClass('listagem-marcado');
                $('tr:not(:contains(' + stringPesquisa + '))', $tbody).addClass('listagem-remover');
            }
        }).on('focus', 'input.busca-listagem-pesquisator', function() {
            getTabelaDoPesquisator(this).addClass('table-listagem-filtrando');
        }).on('blur', 'input.busca-listagem-pesquisator', function() {
            getTabelaDoPesquisator(this).removeClass('table-listagem-filtrando');
        });
    }
    enablePesquisator();

    $(document).ready(function() {
        $("#<?= $abaSelecionada ?>").click();
    });

    // Acompanhamento
    function excluirAcompanhamento(id) {
        if (confirm('Deseja apagar este registro?')) {
            url = '?modulo=principal/cdcclientes/selecionado&acao=C&p=<?= $_REQUEST['p'] ?>&act=excluirAcompanhamento&id_acompanhamento=' + id;
            location.href = url;
        }
    }

    function exibirModalNovoAcompanhamento(tipo) {
        $('#titulo_acompanhamento').html(tipo);
        $('#tipo_acompanhamento').val(tipo);
        var modal = $('#modal-novo-acompanhamento');
        modal.modal('show');
    }

    function baixarAnexo(arqid) {
        url = '?modulo=principal/cdcclientes/selecionado&acao=C&p=<?= $_REQUEST['p'] ?>&act=baixarAnexo&arqid=' + arqid;
        location.href = url;
    }

    function editar(id) {
        url = '?modulo=principal/cotacao-dispensa/formulario&acao=C' + id + '&C&est_id=<?php echo $dadosCDCCliente['estabelecimento_id'] ?>';
        window.open(url);
    }
</script>
<h4>
    <button type="button" class="btn btn-voltar" onclick="history.go(-1)">
        <i class="fa fa-arrow-left"></i> Voltar
    </button>
    <b><?php echo $dados['orgao'] ?></b>
    <i style="cursor: pointer;" title="Alterar informações" class="fa fa-edit" onclick="editar('<?= $p ?>')"></i>
    <spam class="badge badge-primary"><b><?php echo $dados['situacao'] ?></b></spam>
</h4>
<ul class="nav nav-tabs nav-fill">
    <li class="nav-item">
        <a class="nav-link active" data-toggle="tab" href="#kt_tab_cadastro" id="btn_tab_cadastro">
            <span class="nav-text"> Cadastro</span>
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
    require_once('selecionado/cadastro.inc');
    require_once('selecionado/tramitacoes_anexos.inc');
    ?>
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
                    <input type="hidden" name="cdc_id" value="<?php echo $dadosCDCCliente['cdc_id'] ?>" />
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
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Adicionar a Agenda?</label>
                            </div>
                        </div>
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Horário Início</label>
                                <?php
                                echo campo_textom('hora_inicio', 'N', 'S', '', 5, 5, '##:##', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Horário Fim</label>
                                <?php
                                echo campo_textom('hora_fim', 'N', 'S', '', 5, 5, '##:##', '', '', '', 0); ?>
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