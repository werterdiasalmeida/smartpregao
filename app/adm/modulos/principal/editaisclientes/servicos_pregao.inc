<?php
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$mEditalClienteMonitoramento = new EditalClienteMonitoramento();
if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar':
            $params = $_POST ;
            $mEditalClienteMonitoramento->salvarMonitoramento($params);
            $mEditalClienteMonitoramento->commit();
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

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
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
include APPRAIZ . "includes/cabecalhom.inc";

?>
<br />
<div class="portlet light bordered">
    <div class="portlet-title">
        <button type="button" class="btn btn-voltar" onclick="history.go(-1)">
            <i class="fa fa-arrow-left"></i> Voltar
        </button>
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                <?= $dadosCliente['nome_fantasia'] ?>, Escolha um serviço para solicitar
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="row">
            <div class="col-md-6">
                <h5>Situação: <spam class="badge badge-primary"><b><?php echo $dados['situacao'] ?></b></spam>
                </h5>
                <b><?php echo $dados['orgao'] ?></b>
                <br><br>
                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>Portal do Pregoeiro</b></label>
                            <br />
                            <?php echo $dados['portal'] ?>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>Edital</b></label>
                            <br />
                            <?php echo $dados['edital'] ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>Data do pregão</b></label>
                            <br />
                            <?php echo formata_data($dados['data_limite']) ?>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>Hora do pregão</b></label>
                            <br />
                            <?php echo $dados['hora_limite'] ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>Valor Estimado</b></label>
                            <br />
                            <b class="font-blue-sharp"><?php echo $dados['valor_estimado'] ?></b>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>Temperatura</b></label>
                            <br />
                            <spam class="badge badge-primary"><?php echo $dados['temperatura'] ?></spam>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>UF</b></label>
                            <br />
                            <?php echo $dados['uf'] ?>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>Município</b></label>
                            <br />
                            <?php echo $dados['municipio'] ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-12">
                        <div class="accordion accordion-solid accordion-toggle-plus" id="accordionExample3">
                            <div class="card">
                                <div class="card-header" style="cursor: pointer ;" id="headingOne3">
                                    <div class="card-title" data-toggle="collapse" data-target="#collapseOne3">
                                        <b>Objeto</b>
                                    </div>
                                </div>
                                <div id="collapseOne3" class="collapse show" data-parent="#accordionExample3">
                                    <div class="card-body">
                                        <?php echo nl2br($dados['objeto']) ?>
                                    </div>
                                </div>
                            </div>
                            <div class="card">
                                <div class="card-header" style="cursor: pointer ;" id="headingTwo3">
                                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseTwo3">
                                        <br /><b>+ Comentários</b>
                                    </div>
                                </div>
                                <div id="collapseTwo3" class="collapse" data-parent="#accordionExample3">
                                    <div class="card-body">
                                        <?php echo nl2br($dados['comentarios']) ?>
                                    </div>
                                </div>
                            </div>
                            <div class="card">
                                <div class="card-header" style="cursor: pointer ;" id="headingThree3">
                                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseThree3">
                                        <br /> <b>+ Habilitação Técnica</b>
                                    </div>
                                </div>
                                <div id="collapseThree3" class="collapse" data-parent="#accordionExample3">
                                    <div class="card-body">
                                        <?php echo nl2br($dados['habilitacao_tecnica']) ?>
                                    </div>
                                </div>
                            </div>
                            <div class="card">
                                <div class="card-header" style="cursor: pointer ;" id="headingThree4">
                                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseThree4">
                                        <br /> <b>+ Serviço a ser executado</b>
                                    </div>
                                </div>
                                <div id="collapseThree4" class="collapse" data-parent="#accordionExample4">
                                    <div class="card-body">
                                        <?php echo nl2br($dados['resumo_servico']) ?>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="col-md-6">
                <?php
                wf_desenhaBarraNavegacaoHorizontalM($docid, array('id' => $editalCliente), null, null, null, null);
                $param['id'] = $dados['id_edital'];
                $p = setParam($param, true);

                if ($dados['situacao'] == 'Em Monitoramento') {

                    echo '<h4>Monitoramento</h4>';
                    echo "<a style=\"float:right;\" class='btn btn-primary' onclick=\"exibirModalNovoMonitoramento()\" > <i class=\"fa fa-plus\"></i> Adicionar</a> <br><br>";

                    $arConfig = array();
                    $$arWhere[] = " editalcliente_id = {$dadosEditalCliente['id']}";

                    $rs = $mEditalClienteMonitoramento->getList($arWhere);
                    $mEditalClienteMonitoramento = new Listam(array("style" => "width: 100%;"));
                    $mEditalClienteMonitoramento->setCabecalho(array(
                        'Data/Hora',
                        'Monitoramento'
                    ));
                    $mEditalClienteMonitoramento->setCorpo($rs, $arConfig);
                    $mEditalClienteMonitoramento->setAcao($acao);
                    $mEditalClienteMonitoramento->show();
                } ?>

                <h4>Ações</h4>
                <a href="#" class="btn btn-danger" onclick="editar('<?= $p ?>')"> <i class="fa fa-edit"></i> Editar Informações do Pregão</a>
                </br>
                </br>
                <a href="#" class="btn btn-success" onclick="adicionarCalendario('<?= $editalCliente ?>')"> <i class="fa fa-calendar"></i> Adicionar ao Calendário</a>
                <br />
                <?php
                if ($dados['link_g_drive']) {
                    echo "<br><a class='btn btn-warning' href=\"{$dados['link_g_drive']}\" target=\"_blank\"> <i class=\"fa fa-google\"></i> Google Drive</a>";
                }

                switch ($dados['portal']) {
                    case 'ComprasNet':
                        $num_pregao = preg_replace('/[^0-9]/', '', $dados['edital']);
                        $num_uasg = substr($num_pregao, -6);
                        $num_pregao = substr($num_pregao, 0, -6);
                        $linkDownload_comprasnet = "https://www.comprasnet.gov.br/ConsultaLicitacoes/Download/Download.asp?coduasg={$num_uasg}&numprp={$num_pregao}&modprp=5&bidbird=N&idLetra=us2gcz&idSom=&Submit=Confirmar";
                        $link = "<br><br>
                                    <a class='btn btn-primary' href=\"{$linkDownload_comprasnet}\" target=\"_blank\"> <i class=\"fa fa-cloud\"></i> Download ComprasNet</a>";
                        echo $link;
                        $linkDownload_comprasnet = "http://www.comprasnet.gov.br/livre/Pregao/lista_pregao.asp?Opc=2&txtlstUasg=&rdTpPregao=E&lstSrp=T&lstICMS=T&lstSituacao=5&lstTipoSuspensao=0&uf=&co_uasg={$num_uasg}&numprp={$num_pregao}&dt_entrega=&dt_abertura=";
                        $link = "<br><br>
                                    <a class='btn btn-primary' href=\"{$linkDownload_comprasnet}\" target=\"_blank\"> <i class=\"fa fa-search\"></i> Acompanhar no ComprasNet</a>";
                        echo $link;
                        break;
                    case 'BB':
                        $num_pregao = preg_replace('/[^0-9]/', '', $dados['edital']);
                        $link =  '<br><br> <a class="btn btn-primary"  href="https://www.licitacoes-e.com.br/aop/consultar-detalhes-licitacao.aop?numeroLicitacao=' . $num_pregao . '&opcao=consultarDetalhesLicitacao" target="_blank"> <i class="fa fa-cloud"></i> Licitações-e (BB)</a>';
                        echo $link;
                        break;
                }
                ?>

            </div>

        </div>
    </div>
</div>
<div class="modal fade in" id="modal-novo-monitoramento" tabindex="-1" role="modal-novo-monitoramento" aria-hidden="true">
    <div class="modal-dialog">
        <form role="form" name="formulario" id="form-monitoramento" class="edital-monitoramento-form" method="POST" enctype="multipart/form-data">
            <input type="hidden" name="act" value="salvar" />
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Adicionar Monitoramento</h4>
                </div>
                <div class="modal-body">
                <input type="hidden" name="editalcliente_id" value="<?php echo $dadosEditalCliente['id']?>" />
                    <div id="div_novo-monitoramento">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Data da ação</label>
                                <?= campo_datam('data', 'S', 'S', '', 'S'); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Horário da ação</label>
                                <?= campo_textom('hora', 'S', 'S', '', 5, 5, '', '', '', '', 0); ?>
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
<script>
    function editar(id) {
        url = '?modulo=principal/editais/formulario&acao=C' + id;
        window.open(url);
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