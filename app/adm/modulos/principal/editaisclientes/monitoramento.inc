<?php
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$mEditalClienteMonitoramento = new EditalClienteMonitoramento();
$params = getParam();
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'pesquisa':
            $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
            $arWhereEditais[] = "(
                        (edi.objeto ILIKE '%{$pesquisa_simples}%') OR
                        (edi.orgao ILIKE '%{$pesquisa_simples}%') OR
                        (TO_CHAR(edi.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                    )";

            break;
    }
}
if (isset($params['id']) && !empty($params['id'])) {
    UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($params['id']);
    $aba = true;
    $dados = $mEstabelecimento->getTodosDados($params['id']);
    extract($dados);
}
include APPRAIZ . "includes/cabecalhom.inc";
$estabelecimentoId = $_REQUEST['p'];
?>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="bi bi-exclamation-octagon font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Central de Alertas da <?= $dados['nome_fantasia']; ?>
            </span>
        </div>
        <?php
        if (isset($params['id']) && !empty($params['id'])) {
            echo $mEstabelecimento->retornaAtalhosCliente($params['id']);
        } else {
            echo "Cliente n�o encontrado.";
        }
        ?>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="form" method="POST" enctype="multipart/form-data">
            <div class="row">
                <div class="col-md-12">
                    <h4>Monitoramento Ativo</h4>
                    <?php
                    $arConfig = array();
                    $where = " data >= CURRENT_DATE AND edc.estabelecimento_id = {$dados['id']}";
                    $id = $params['id'];
                    $arConfig[1]['html'] = <<<HTML
                    <a href="?modulo=principal/editaisclientes/selecionado&acao=C&p=' . <php> return setParam(array('id' => '{id}'), false); </php> . '" data-toggle="tooltip" title="Servi�os do Edital">
                        {edital}
                    </a>
HTML;

                    $arConfig[0]['html'] = <<<HTML
                    <center><spam class="badge badge-primary"><b>{falta}</b></spam></center>
HTML;
                    $rs = $mEditalClienteMonitoramento->getListCliente($where);
                    $mEditalClienteMonitoramento = new Listam(array("style" => "width: 100%;"));
                    $mEditalClienteMonitoramento->setCabecalho(array(
                        'Falta',
                        'Edital',
                        'Data/Hora da A��o',
                        'Monitoramento'
                    ));
                    $mEditalClienteMonitoramento->setCorpo($rs, $arConfig);
                    $mEditalClienteMonitoramento->setAcao($acao);
                    $mEditalClienteMonitoramento->show();
                    ?>
                </div>
                <h4>Hist�rico</h4>
                <?php
                $arConfig = array();
                $where = " data < CURRENT_DATE AND edc.estabelecimento_id = {$dados['id']}";
                $id = $params['id'];
                $arConfig[0]['html'] = <<<HTML
                    <a href="?modulo=principal/editaisclientes/selecionado&acao=C&p=' . <php> return setParam(array('id' => '{id}'), false); </php> . '" data-toggle="tooltip" title="Servi�os do Edital">
                        {edital}
                    </a>

HTML;
                $mEditalClienteMonitoramento = new EditalClienteMonitoramento();
                $rs = $mEditalClienteMonitoramento->getListCliente($where);
                $mEditalClienteMonitoramento = new Listam(array("style" => "width: 100%;"));
                $mEditalClienteMonitoramento->setCabecalho(array(
                    'Edital',
                    'Data/Hora',
                    'Monitoramento'
                ));
                $mEditalClienteMonitoramento->setCorpo($rs, $arConfig);
                $mEditalClienteMonitoramento->setAcao($acao);
                $mEditalClienteMonitoramento->show();
                ?>
            </div>

    </div>


    <div class="form-actions">
        <button type="button" class="btn btn-voltar">
            <i class="fa fa-arrow-left"></i> Voltar
        </button>
    </div>

    <input type="hidden" name="act" value="salvar" />
    </form>
</div>
</div>


<script type="text/javascript">
    $(function() {
        $('.btn-voltar').click(function() {
            history.go(-1);
        });
    });

    function filtrarMeusEditais(tipo) {

        location.href = location.href + '&act=pesquisa_meus_editais&filtrar=' + tipo;

    }

    function exibirModalParticipar(p, nome) {
        var modal = $('#modal-participar');

        $('form.form-participar [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.modal('show');
    }

    function exibirModalDetalhe(p, nome) {
        var modal = $('#modal-detalhe-edital');
        modal.find('strong#nome').html(nome);
        App.blockUI({
            message: 'Carregando...',
            target: '#div_detalhe_edital'
        });
        // Carregar os dados do Edital
        $('div#div_detalhe_edital').load(window.location.href, {
            act: 'carregarDetalhedital',
            p: p
        }, function() {
            App.unblockUI('#div_detalhe_edital');
        });
        //modal.find('#div_detalhe_edital').html();
        modal.modal('show');
    }

    function participarEdital() {
        $('form.form-participar').submit();
    }

    function solicitarServico(id) {
        window.location = '?modulo=principal/editaisclientes/servicos_pregao&acao=C&p=' + id;
    }
</script>