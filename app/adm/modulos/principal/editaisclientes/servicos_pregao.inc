<?php
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
//ver($_REQUEST,d);
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'carregarServicoEdital':
            $params['servico'] = $_REQUEST['servico'];
            $params['edital_cliente_id'] = $_REQUEST['edital_cliente_id'];
            require APPRAIZ . "adm/modulos/principal/editaisclientes/detalhe_servico.inc";
            die;
            break;
    }
}

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

ver($dados);

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
                            <?php echo $dados['valor_estimado'] ?>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label><b>Temperatura</b></label>
                            <br />
                            <?php echo $dados['temperatura'] ?>
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
                                <div class="card-header" id="headingOne3">
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
                                <div class="card-header" id="headingTwo3">
                                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseTwo3">
                                    <br/><b>+ Comentários</b>
                                    </div>
                                </div>
                                <div id="collapseTwo3" class="collapse" data-parent="#accordionExample3">
                                    <div class="card-body">
                                        <?php echo nl2br($dados['comentarios']) ?>
                                    </div>
                                </div>
                            </div>
                            <div class="card">
                                <div class="card-header" id="headingThree3">
                                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseThree3">
                                    <br/> <b>+ Habilitação Técnica</b>
                                    </div>
                                </div>
                                <div id="collapseThree3" class="collapse" data-parent="#accordionExample3">
                                    <div class="card-body">
                                    <?php echo nl2br($dados['habilitacao_tecnica']) ?>
                                    </div>
                                </div>
                            </div>
                            <div class="card">
                                <div class="card-header" id="headingThree4">
                                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseThree4">
                                    <br/> <b>+ Serviço a ser executado</b>
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
                ?>

                <h4>Ações</h4>
                <a href="#" class="btn btn-danger" onclick="editar('<?= $p ?>')"> <i class="fa fa-edit"></i> Editar Informações do Pregão</a>
                </br>
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
            <!-- <div class="col-md-8">
                <div class="container-fluid">
                    <div class="row">
                        <div class="col-md-3">
                            <div class="card" style="width: 150px;">
                                <img width="150px" height="100px" class="card-img-top" src="/imagens/edital/verifica-habilitacao.jpg" alt="Verificação de Habilitação">
                                <div class="card-body">
                                    <center>
                                        <h5 class="card-title"><b>Verificação de Habilitação <i class="fa fa-info-circle" title="Mais informações"></i></b></h5>
                                        <p class="card-text">Saiba se a sua empresa pode participar deste pregão.</p>
                                        <a href="#" class="btn btn-primary" onclick="exibirModalServico(1,<?= $editalCliente ?>)">Contratar Serviço</a>
                                    </center>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="card" style="width: 150px;">
                                <img width="150px" height="100px" class="card-img-top" src="/imagens/edital/checklist.png" alt="Ckecklist Personalizado">
                                <div class="card-body">
                                    <center>
                                        <h5 class="card-title"><b>Ckecklist Personalizado <i class="fa fa-info-circle" title="Mais informações"></i></b></h5>
                                        <p class="card-text">Um resumo do edital (pontos importantes sobre o edital e a sua empresa).</p>
                                        <a href="#" class="btn btn-primary" onclick="exibirModalServico(2,<?= $editalCliente ?>)">Contratar Serviço</a>
                                    </center>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="card" style="width: 150px;">
                                <img width="150px" height="100px" class="card-img-top" src="/imagens/edital/documentacao.jpg" alt="Documentação de Habilitação">
                                <div class="card-body">
                                    <center>
                                        <h5 class="card-title"><b>Documentação de Habilitação <i class="fa fa-info-circle" title="Mais informações"></i></b></h5>
                                        <p class="card-text">Documentos para sua empresa participar do pregão (habilitação e proposta financeira) </p>
                                        <a href="#" class="btn btn-primary" onclick="exibirModalServico(3,<?= $editalCliente ?>)">Contratar Serviço</a>
                                    </center>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="card" style="width: 150px;">
                                <img width="150px" height="100px" class="card-img-top" src="/imagens/edital/cadastro-portal.jpg" alt="Documentação de Habilitação">
                                <div class="card-body">
                                    <center>
                                        <h5 class="card-title"><b>Cadastro no portal do Licitante <i class="fa fa-info-circle" title="Mais informações"></i></b></h5>
                                        <p class="card-text">Realizar e apoiar o cadastramento da empresa no portal do licitante. </p>
                                        <a href="#" class="btn btn-primary" onclick="exibirModalServico(4,<?= $editalCliente ?>)">Contratar Serviço</a>
                                    </center>
                                </div>
                            </div>
                        </div>
                    </div>
                    <br />
                    <div class="row">
                        <div class="col-md-3">
                            <div class="card" style="width: 150px;">
                                <img width="150px" height="100px" class="card-img-top" src="/imagens/edital/monitorar.jpg" alt="Monitoramento do Edital">
                                <div class="card-body">
                                    <center>
                                        <h5 class="card-title"><b>Monitoramento <i class="fa fa-info-circle" title="Mais informações"></i></b></h5>
                                        <p class="card-text">Monitorar o que acontece com o edital até o fim do Pregão.</p>
                                        <a href="#" class="btn btn-primary" onclick="exibirModalServico(5,<?= $editalCliente ?>)">Contratar Serviço</a>
                                    </center>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="card" style="width: 150px;">
                                <img width="150px" height="100px" class="card-img-top" src="/imagens/edital/suporte-pregao.jpg" alt="Suporte no Pregão">
                                <div class="card-body">
                                    <center>
                                        <h5 class="card-title"><b>Suporte no Pregão <i class="fa fa-info-circle" title="Mais informações"></i></b></h5>
                                        <p class="card-text">Suporte especializado por vídeo-chamada no momento do pregão.</p>
                                        <a href="#" class="btn btn-primary" onclick="exibirModalServico(6,<?= $editalCliente ?>)">Contratar Serviço</a>
                                    </center>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="card" style="width: 150px;">
                                <img width="150px" height="100px" class="card-img-top" src="/imagens/edital/suporte-pos-pregao.jpg" alt="Suporte Pós-pregão">
                                <div class="card-body">
                                    <center>
                                        <h5 class="card-title"><b>Suporte Pós-pregão <i class="fa fa-info-circle" title="Mais informações"></i></b></h5>
                                        <p class="card-text">Suporte após a finalização do pregão, para documentação.</p>
                                        <a href="#" class="btn btn-primary" onclick="exibirModalServico(7,<?= $editalCliente ?>)">Contratar Serviço</a>
                                    </center>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="card" style="width: 150px;">
                                <img width="150px" height="100px" class="card-img-top" src="/imagens/edital/suporte-juridico.jpg" alt="Suporte Jurídico">
                                <div class="card-body">
                                    <center>
                                        <h5 class="card-title"><b>Suporte Jurídico <i class="fa fa-info-circle" title="Mais informações"></i></b></h5>
                                        <p class="card-text">Suporte jurídico através de parceiros especializados.</p>
                                        <a href="#" class="btn btn-primary" onclick="exibirModalServico(8,<?= $editalCliente ?>)">Contratar Serviço</a>
                                    </center>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div> -->
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-detalhe-edital" tabindex="-1" role="modal-detalhe-edital" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Contratar o Serviço</h4>
            </div>
            <div class="modal-body">
                <div id="div_detalhe_edital"></div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn blue" data-dismiss="modal">Fechar</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>
<script>
    function editar(id) {
        url = '?modulo=principal/editais/formulario&acao=C' + id;
        window.open(url);
    }


    function exibirModalServico(servico, edital_licitante_id) {
        var modal = $('#modal-detalhe-edital');
        //carregando
        App.blockUI({
            message: 'Carregando...',
            target: '#div_detalhe_edital'
        });
        var data = {
            act: 'carregarServicoEdital',
            servico: servico,
            edital_cliente_id: edital_licitante_id,
        }
        $('#div_detalhe_edital').load('?modulo=principal/editaisclientes/servicos_pregao&acao=C', data, function() {
            App.unblockUI('#div_detalhe_edital');
        });
        modal.modal('show');
    }
</script>