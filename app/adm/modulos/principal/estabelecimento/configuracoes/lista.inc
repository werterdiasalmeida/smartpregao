<?php
$mTipoMensagem                  = new TipoMensagem();
$mPlanoPacoteMensagem           = new PlanoPacoteMensagem();
$mEstabelecimentoPacoteMensagem = new EstabelecimentoPacoteMensagem();
$mContatoPessoaJuridica         = new ContatoPessoaJuridica();
$mEnderecoPessoaJuridica        = new EnderecoPessoaJuridica();
$mFatura                        = new Fatura();
$mIugu                          = new SimecIugu();
$mEstabelecimento               = new Estabelecimento();
$mPessoaJuridica                = new PessoaJuridica();
$mIugu                          = new SimecIugu();

$urlBoletoIugu = $mIugu->getUrl();

$p = getParam();
UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);

$mEstabelecimentoConfiguracao = new EstabelecimentoConfiguracao();
$dadosConfiguracao = $mEstabelecimentoConfiguracao->getByEstabelecimento($p['id']);

$dadosEstabelecimento = $mEstabelecimento->getTodosDados($p['id']);
$dadosEstabelecimento = $mPessoaJuridica->getDadosEnderecoEContatoPrincipal($dadosEstabelecimento);

$possuiEndereco = false;
if( !empty($dadosEstabelecimento['logradouro']) && !empty($dadosEstabelecimento['bairro']) &&
    !empty($dadosEstabelecimento['bairro']) && !empty($dadosEstabelecimento['complemento']) &&
    !empty($dadosEstabelecimento['cep'])  && !empty($dadosEstabelecimento['uf_sigla'])){
    $possuiEndereco = true;
}

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'carregaModalRenovacaoPacote' :
            include "modalContratacaoPacoteMensagem.inc";
            die();
            break;
        case 'contratarPacoteMensagem' :
            $params = array_merge($_POST, getParam());
            $id = $mEstabelecimentoPacoteMensagem->salvarDados($params);
            $mEstabelecimentoPacoteMensagem->commit();
            $db->sucessomAjax("",array('estabelecimento_pacote_mensagem_id' => $id));
            break;
        case 'cancelarSolicitacaoPacote' :
            $params = array_merge($_POST, getParam());
            $id = $mEstabelecimentoPacoteMensagem->inativar($params['estabelecimento_pacote_mensagem_id']);
            $mEstabelecimentoPacoteMensagem->commit();
            $db->sucessomAjax("",array('estabelecimento_pacote_mensagem_id' => $id));
            break;
        case 'carregarDadosListaHistorico' :
            $params = array_merge($_POST, getParam());

            $historicoSolicitacoes = $mEstabelecimentoPacoteMensagem->getListByEstabelecimentoAndTipo($params['id'],$params['tipo_mensagem_id'], false);
            foreach ($historicoSolicitacoes AS $chave => $dado){
                $historicoSolicitacoes[$chave]['p'] = setParam(['id' => $params['id'], 'tipo_mensagem_id' => $dado['tipo_mensagem_id'],  'estabelecimento_pacote_mensagem_id' => $dado['estabelecimento_pacote_mensagem_id'] ], false);
            }

            $historicoSolicitacoes = json_encode(utf8ArrayEncode($historicoSolicitacoes));
            $db->sucessomAjax("",array('tipo' => $params['tipo_mensagem_id'], 'json' => $historicoSolicitacoes));
            break;
        case 'gerarFaturaContratacaoPacoteMensagem' :
            $params = array_merge($_POST, getParam());
            $dadosPacote = $mEstabelecimentoPacoteMensagem->getDadosCompletosById($params['estabelecimento_pacote_mensagem_id']);
            $contatoPessoaJuridica = $mContatoPessoaJuridica->listarEspecificosPorPessoaJuridica($dadosPacote['pessoa_juridica_id']);
            $listEnderecoPessoaJuridica = $mEnderecoPessoaJuridica->listarPorPessoaJuridica($dadosPacote['pessoa_juridica_id'], false, 1);
            $enderecoPessoaJuridica = $listEnderecoPessoaJuridica[0];

            //Gerando a fatura do Iugo e recuperando os dados da mesma para jogar em tela
            $arDados = [
                'data_vencimento'   => (new DateTime())->add(new DateInterval('P2D'))->format('Y-m-d'),
                'valor'             => $dadosPacote['plano_valor'],
                'descricao'         => "Contratação pacote de {$dadosPacote['quantidade_mensagens']} mensagens: {$dadosPacote['descricao_tipo_mensagem']}.",
                'cnpj'              => $dadosPacote['cnpj'],
                'razao_social'      => $dadosPacote['razao_social'],
                'email'             => $contatoPessoaJuridica['contato_email'],
                'telefone'          => isset($contatoPessoaJuridica['contato_telefone']) ? $contatoPessoaJuridica['contato_telefone'] : null,
                'cep'               => $enderecoPessoaJuridica['cep'],
                'numero'            => $enderecoPessoaJuridica['complemento']
            ];
            $faturaId = $mFatura->salvarEGerarFaturaIugu($arDados);
            $mFatura->carregarPorId($faturaId);
            $arDadosFatura = $mFatura->getDados();
            $arDadosFatura['url'] = $mIugu->getUrl() . $arDadosFatura['secure_id_iugu'];

            //Adicionando a fatura
            $mEstabelecimentoPacoteMensagem->manter([
                     'id'           => $dadosPacote['estabelecimento_pacote_mensagem_id'],
                     'fatura_id'    => $faturaId
             ]);

            $db->sucessomAjax("", $arDadosFatura,'Fatura gerada com sucesso!');
            break;
        case 'carregaInformacoesPlanoPagamento' :
            $params = array_merge($_POST, getParam());
            $mPlanoPacoteMensagem->carregarPorId($params['plano_pacote_mensagem_id']);
            $dadosPlano = $mPlanoPacoteMensagem->getDados();
            die(json_encode(utf8ArrayEncode($dadosPlano)));
            break;
        case 'salvar' :
            $params = array_merge($_POST, getParam());

            // Salvando as configurações de tipo de mensagem
            $tiposMensagem = $mTipoMensagem->recuperarColuna('id');
            $tiposMensagem = $tiposMensagem ? $tiposMensagem : [];
            $arSalvarTipoMensagem = [];

            foreach ($tiposMensagem AS $tipo_mensagem_id){
                //verifica se o tipo de mensagem possui um pacote ilimitado, se sim faz o vinculo


                $arSalvarTipoMensagem[] = [
                    'tipo_mensagem_id' => $tipo_mensagem_id,
                    'habilitado'       => ($params['check_tipo_mensagem'][$tipo_mensagem_id] ? true : false)
                ];
            }

            $dadosConfiguracao['configuracao_disparo_mensagens'] = json_encode($arSalvarTipoMensagem);
            $dadosConfiguracao['inicial_lote'] = $params['inicial_lote'];
            $dadosConfiguracao['estabelecimento_id'] = $params['id'];
            $dadosConfiguracao['excluido'] = 'false';
            $mEstabelecimentoConfiguracao->manter($dadosConfiguracao);
            $db->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(getParam()));
            break;
    }
}

//Recuperando o registro de configuração do estabelecimento e organizando as informações
$configDisparoMensagens = [];
$dadosConfiguracao['configuracao_disparo_mensagens'] = !empty($dadosConfiguracao['configuracao_disparo_mensagens']) ? json_decode($dadosConfiguracao['configuracao_disparo_mensagens'],true) : [];
foreach ($dadosConfiguracao['configuracao_disparo_mensagens'] AS $confMensagem){
    $configDisparoMensagens[$confMensagem['tipo_mensagem_id']] = $confMensagem['habilitado'];
}
$inicial_lote = $dadosConfiguracao['inicial_lote'] ?: 1;

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true);

?>
<style type="text/css">
    .panel-default {
        border: 1px solid #ddd;
        margin-bottom: 10px !important;
    }

    .panel-default>.panel-heading {
        background-color: #ffffff;
    }

    .modal-grande {
        width: 90%;
    }
</style>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-cog font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Configurações
            </span>
        </div>
    </div>
    <form role="form" name="formulario" class="configuracao-form" method="POST" enctype="multipart/form-data">
        <div class="portlet-body form">
            <div class="row">
                <div class="col-md-6 col-xs-12">
                    <h4 class="form-section">
                        <i class="fa fa-comments-o"></i> Envio de mensagens
                    </h4>
                    <div class="">
                        <?php
                        $tiposMensagem = $mTipoMensagem->getInformacoesTipoMensagemByEstabelecimento($p['id']);

                        foreach ($tiposMensagem AS $tipoMensagem):
                            $ativo = $configDisparoMensagens[$tipoMensagem['id']];
                            $displayGrafico = $tipoMensagem['qtd_total'] == 0 && $tipoMensagem['qtd_disponivel'] == 0 ? 'none' : 'block' ;

                            $historicoSolicitacoes = $mEstabelecimentoPacoteMensagem->getListByEstabelecimentoAndTipo($p['id'],$tipoMensagem['id'], false);
                            $pHistoricoLista = setParam(['id' => $p['id'], 'tipo_mensagem_id' => $tipoMensagem['id']], false);

                            foreach ($historicoSolicitacoes AS $chave => $dado){
                                $historicoSolicitacoes[$chave]['p'] = setParam(['id' => $p['id'], 'tipo_mensagem_id' => $dado['tipo_mensagem_id'], 'estabelecimento_pacote_mensagem_id' => $dado['estabelecimento_pacote_mensagem_id'] ], false);
                            }
                            $historicoSolicitacoes = json_encode(utf8ArrayEncode($historicoSolicitacoes));

                            $disabled = $tipoMensagem['ilimitado'] == 'f' && $tipoMensagem['qtd_disponivel'] == 0 ? 'disabled' : '';
                            $ativo = !empty($disabled) ? false : $ativo;
                            ?>

                            <div class="panel panel-default">
                                <div class="panel-heading">
                                    <i class="<?= $tipoMensagem['icone'] ?>"></i> <?= $tipoMensagem['descricao'] ?>
                                    <input type="hidden" id="param-lista-<?= $tipoMensagem['id'] ?>" value="<?= $pHistoricoLista; ?>">
                                    <div style="margin-right: 10px; float: right;">
                                        <div class="bootstrap-switch bootstrap-switch-mini" style="margin-left: 10px;">
                                            <div class="bootstrap-switch-container">
                                                <input name="check_tipo_mensagem[<?= $tipoMensagem['id'] ?>]" <?= $disabled; ?> type="checkbox" <?= ($ativo === true ? 'checked' : '') ?> class="make-switch switch_tipo_mensagem" data-on-text="Sim" data-off-text="Não" data-on-color="success" data-off-color="danger" id="switch_tipo_mensagem_<?= $tipoMensagem['id'] ?>" data-size="mini">
                                            </div>
                                        </div>
                                    </div>
                                </div>
                                <? if($tipoMensagem['ilimitado'] == 'f'): ?>
                                    <? if(!$possuiEndereco || empty($dadosEstabelecimento['contato_email'])): ?>
                                        <div class="panel-body">
                                            <div class="portlet-body">
                                                <div class="row" style="padding: 15px; border: 1px solid #ddd;">
                                                    <p>Para contratar pacotes de SMS é necessário completar as informações de e-mail e endereço do estabelecimento!</p>
                                                    <div class="row text-center" style="padding: 10px;">
                                                        <a type="button" href="?modulo=principal/estabelecimento/formulario&acao=C<?= setParam($p); ?>" class="btn blue">Clique aqui para completar seu cadastro</a>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    <? else: ?>
                                        <div class="panel-body">
                                            <div class="portlet-body">
                                                <div class="row" style="padding: 15px; border: 1px solid #ddd;">
                                                    <div class="row">
                                                        <div class="row">
                                                            <div class="col-xs-12 col-sm-4 col-md-4 text-center"><h4>Total: <?= $tipoMensagem['qtd_total'] ?></h4></div>
                                                            <div class="col-xs-12 col-sm-4 col-md-4 text-center"><h4>Utilizados: <?= $tipoMensagem['qtd_total'] - $tipoMensagem['qtd_disponivel'] ?></h4></div>
                                                            <div class="col-xs-12 col-sm-4 col-md-4 text-center"><h4 class="bold">Disponíveis: <?= $tipoMensagem['qtd_disponivel'] ?></h4></div>
                                                        </div>
                                                        <div class="row text-center">
                                                            <div id="grafico-quantitativo-<?= $tipoMensagem['id'] ?>" style="height: 200px; margin: 0 auto; display:<?= $displayGrafico ?>;"></div>
                                                        </div>
                                                    </div>
                                                    <div class="row text-center" style="padding: 10px;">
                                                        <button type="button" class="btn blue btn-contratar-pacote" onclick="exibeModalRenovacaoPacote('<?= $tipoMensagem['id'] ?>')">Contratar Pacote</button>
                                                    </div>
                                                    <div class="row" style="padding: 10px;">
                                                        <table style="width: 100%" id="tabela-historico-pacotes-<?= $tipoMensagem['id'] ?>" class="table table-striped table-hover">
                                                            <thead>
                                                                <th data-field='estabelecimento_pacote_mensagem_id' data-formatter='formatAcaoHistorico' data-align='center'>Ação</th>
                                                                <th data-field="data_cadastro" data-align='center' data-formatter='formatDate'>Data</th>
                                                                <th data-field="fatura_situacao" data-formatter="formatSituacaoHistorico">Situação</th>
                                                                <th data-field='plano_valor' data-formatter='formataValor'>Valor (R$)</th>
                                                                <th data-field='quantidade_total'>Qtd.</th>
                                                            </thead>
                                                        </table>
                                                    </div>
                                                </div>
                                            </div>
                                            <script type="text/javascript">
                                                $(function () {
                                                    Highcharts.chart('grafico-quantitativo-<?= $tipoMensagem['id'] ?>', {
                                                        chart: {
                                                            plotBackgroundColor: null,
                                                            backgroundColor: null,
                                                            plotBorderWidth: 0,
                                                            plotShadow: false,
                                                            spacingBottom: 0,
                                                            spacingTop: 0,
                                                            spacingLeft: 0,
                                                            spacingRight: 0,
                                                            margin: [0,0,0,0]
                                                        },
                                                        title: {
                                                            text: null
                                                        },
                                                        legend: {
                                                            enabled: true
                                                        },
                                                        credits: {
                                                            enabled: false
                                                        },
                                                        tooltip: {
                                                            pointFormat: '<b>{point.y} ({point.percentage:.1f}%)</b>'
                                                        },
                                                        plotOptions: {
                                                            pie: {
                                                                dataLabels: {
                                                                    enabled: false
                                                                },
                                                                startAngle: -90,
                                                                endAngle: 90,
                                                                center: ['50%', '75%']
                                                            }
                                                        },
                                                        series: [{
                                                            type: 'pie',
                                                            name: 'Quantidade mensagens disponíveis',
                                                            innerSize: '50%',
                                                            data: [
                                                                {name: 'Disponível', y: <?= ($tipoMensagem['qtd_disponivel'] ?: 0 ) ?>, color: '#26C281'},
                                                                {name: 'Utilizado', y: <?= ($tipoMensagem['qtd_total'] - $tipoMensagem['qtd_disponivel'] ?: 0 ) ?>, color: '#ed6b75'}
                                                            ]
                                                        }]
                                                    });

                                                    $('#tabela-historico-pacotes-<?= $tipoMensagem['id'] ?>').bootstrapTable({
                                                        striped: true,
                                                        pagination: true,
                                                        pageSize: 3,
                                                        search: true,
                                                        paginationHAlign: 'left'
                                                    });
                                                    $('#tabela-historico-pacotes-<?= $tipoMensagem['id'] ?>').bootstrapTable('load', <?= $historicoSolicitacoes ?>);
                                                });
                                            </script>
                                        </div>
                                    <? endif; ?>
                                <? endif; ?>
                            </div>
                        <?php
                        endforeach;
                        ?>
                    </div>

                </div>
                <div class="col-md-6 col-xs-12">
                    <h4 class="form-section">
                        <i class="fa fa-gears"></i> Gerais
                    </h4>
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Número inicial dos lotes</label>
                                <?= campo_textom('inicial_lote', 'S', 'S', '', 10, 10, '##########', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <div class="form-actions">
            <button type="submit" class="btn blue">
                <i class="fa fa-save"></i> Salvar
            </button>
            <button type="button" class="btn btn-voltar">
                <i class="fa fa-arrow-left"></i> Voltar
            </button>
        </div>
        <input type="hidden" name="act" value="salvar"/>
    </form>
</div>
<div class="modal fade in" id="modal-generica" tabindex="-1" role="modal-generica" aria-hidden="true">
    <div class="modal-dialog modal-grande">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">titulo</h4>
            </div>
            <div class="modal-body">
                corpo
            </div>
            <div class="modal-footer">
                <button id="btn-salvar-modal" type="button" class="btn blue" style="display: none;">Salvar</button>
                <button id="btn-fechar-modal" type="button" class="btn default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
</div>
<div class="modal fade in" id="modal-confimacao" tabindex="-1" role="modal-confimacao" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Confirmação</h4>
                <input type="hidden" id="parametro" value="">
            </div>
            <div class="modal-body">
                Deseja realmente executar essa operação?
            </div>
            <div class="modal-footer">
                <button id="btn-confirm-modal" type="button" class="btn red">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">Não</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>
<script src="/includes/metronic/global/plugins/highcharts/js/highcharts.js?v=<?= VERSAO ?>" type="text/javascript"></script>
<script type="text/javascript">
    $(function () {
        $('.btn-limpar').click(function () {
            window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] . setParam(array('id' => $p['id'])) ?>';
        });

        //Ao fechar a modal esconde o botão de salvar, renomeia para o nome padrão e retira o evento que é executado
        $('#modal-generica').on('hidden.bs.modal', function () {
            $('#modal-generica #btn-salvar-modal').html('Salvar').hide().unbind( "click" );
        })

        //Ao fechar a modal esconde retira a ação do botão 'sim' e volta o texto para o padrão
        $('#modal-confimacao').on('hidden.bs.modal', function () {
            $('#modal-confimacao .modal-body').html('Deseja realmente executar essa operação?');
            $('#modal-confimacao #btn-confirm-modal').html('Sim').unbind( "click" );
            $('#modal-confimacao #parametro').val('');
        })
    });

    function exibeModalRenovacaoPacote(tipo_mensagem){
        var modal = $('#modal-generica');

        modal.find('.modal-title').html('Contratação de pacote de Mensagem');

        if(modal.find('.modal-dialog').hasClass('modal-grande')){
            modal.find('.modal-dialog').removeClass('modal-grande');
        }

        modal.find('.modal-body').load( location.href , { act: 'carregaModalRenovacaoPacote', tipo_mensagem_id: tipo_mensagem }, function() {
            //console.log('modal aberta');
        });
        modal.modal('show');
    }

    function formatDate(value, row, index) {
        var data = formatarData(value);
        return data;
    }

    function formatAcaoHistorico(value, row, index) {
        var strAcao = "-";
        if(index == 0 && (!row.fatura_situacao || row.fatura_situacao == 'A' || row.fatura_situacao == 'N' || row.fatura_situacao == 'G')) {
            if(row.secure_id_iugu && row.secure_id_iugu != ''){
                var btnGerar = '<a class="btn btn-circle btn-icon-only btn-default blue" target="_blank" href="<?= $urlBoletoIugu ?>' + row.secure_id_iugu + '" title="Visualizar Boleto" style="margin: 2px;">'
                    + '<i class=" icon-doc"></i>'
                    + '</a>';
            }else{
                var btnGerar = '<a class="btn btn-circle btn-icon-only btn-default green-jungle" href="javascript:;" onclick="confirmGerarBoleto(\'' + row.p + '\')" title="Gerar Fatura" style="margin: 2px;">'
                    + '<i class="icon-pencil"></i>'
                    + '</a>';
            }

            var btnCancelar = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:;" onclick="confirmCancelarSolicitacaoPacote(\'' + row.p + '\')" title="Cancelar Solicitação" style="margin: 2px;">'
                + '<i class="icon-ban"></i>'
                + '</a>';

            strAcao = btnGerar + btnCancelar;
        }

        return strAcao;
    }

    function formatSituacaoHistorico(value, row, index) {
        var strAcao = "Fatura não gerada";
        if(row.fatura_situacao == 'A') {
            strAcao = "Fatura aguardando fechamento"
        }else if(row.fatura_situacao == 'N') {
            strAcao = "Fatura pendente"
        }else if(row.fatura_situacao == 'G') {
            strAcao = "Fatura gerada"
        }else if(row.fatura_situacao == 'V') {
            strAcao = "Fatura vencida"
        }else if(row.fatura_situacao == 'C') {
            strAcao = "Fatura cancelada"
        }else if(row.fatura_situacao == 'P') {
            strAcao = "Fatura paga"
        }

        return strAcao;
    }

    function contratarPacote(param){
        var plano = $('[name=plano_pacote_mensagem_id]').val();
        if(!plano){
            return false;
        }

        $('#modal-confimacao').modal('hide');
        $('#modal-generica ').modal('hide');

        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType : 'json',
            data: {
                act : 'contratarPacoteMensagem',
                plano_pacote_mensagem_id: plano
            },
            beforeSend: function( xhr ) {
                App.blockUI({message: 'Carregando...',target: '.page-container'});
            },
            complete: function( data ) {
                App.unblockUI('.page-container');
                carregarListaHistorico(param);
            },
            success: function(result){
                exibirSucesso('Solicitação de contratação de pacote de mensagens realizada com sucesso!');
            },
            error: function (result) {
                exibirAlert('Não foi possível contratar o pacote!');
            }
        });
    }

    function confirmCancelarSolicitacaoPacote(param){
        //Redefinindo a ação e o texto da modal de confirmação
        $('#modal-confimacao .modal-body').html('Deseja realmente cancelar a solicitação de pacote?');
        $('#modal-confimacao #parametro').val(param);
        $('#modal-confimacao #btn-confirm-modal').show().unbind( "click" ).click(function(){
            cancelarSolicitacaoPacote();
        });

        $('#modal-confimacao').modal('show');
    }

    function confirmSolicitacaoPacote(param){
        //Redefinindo a ação e o texto da modal de confirmação
        $('#modal-confimacao .modal-body').html('Deseja realmente contratar esse plano de Mensagem?');
        $('#modal-confimacao #btn-confirm-modal').show().unbind( "click" ).click(function(){
            contratarPacote(param);
        });
        $('#modal-confimacao').modal('show');
    }

    function confirmGerarBoleto(param){
        //Redefinindo a ação e o texto da modal de confirmação
        $('#modal-confimacao .modal-body').html('Deseja realmente gerar a fatura?');
        $('#modal-confimacao #parametro').val(param);
        $('#modal-confimacao #btn-confirm-modal').show().unbind( "click" ).click(function(){
            gerarBoleto();
        });

        $('#modal-confimacao').modal('show');
    }

    function gerarBoleto(){
        var param = $('#modal-confimacao #parametro').val();
        if(!param){
            return false;
        }

        $('#modal-generica ').modal('hide');
        $('#modal-confimacao').modal('hide');

        $.ajax({
            type: "POST",
            url: window.location.href + '&p=' + param,
            dataType : 'json',
            data: {
                act : 'gerarFaturaContratacaoPacoteMensagem'
            },
            beforeSend: function( xhr ) {
                App.blockUI({message: 'Carregando...',target: '.page-container'});
            },
            complete: function( data ) {
                App.unblockUI('.page-container');
                carregarListaHistorico(param);
            },
            success: function(result){
                exibirSucesso('Fatura gerada com sucesso, clique no botão para visualiza-la!');
            },
            error: function (result) {
                exibirAlert('Não foi possível gerar a fatura!');
            }
        });
    }

    function cancelarSolicitacaoPacote(){
        var param = $('#modal-confimacao #parametro').val();
        if(!param){
            exibirAlert('Não foi possível cancelar o pacote (parâmetros necessários não definidos)');
            return false;
        }

        $('#modal-confimacao').modal('hide');
        $('#modal-generica ').modal('hide');

        $.ajax({
            type: "POST",
            url: '?modulo=principal/estabelecimento/configuracoes/lista&acao=C&p=' + param,
            dataType : 'json',
            data: {
                act : 'cancelarSolicitacaoPacote'
            },
            beforeSend: function( xhr ) {
                App.blockUI({message: 'Carregando...',target: '.page-container'});
            },
            complete: function( data ) {
                App.unblockUI('.page-container');
                carregarListaHistorico(param);
            },
            success: function(result){
                exibirSucesso('Cancelamento da solicitação de pacote efetuado com sucesso!');
            },
            error: function (result) {
                exibirAlert('Não foi possível cancelar o pacote!');
            }
        });
    }

    function carregarListaHistorico(param){
        $.ajax({
            type: "POST",
            url: '?modulo=principal/estabelecimento/configuracoes/lista&acao=C&p=' + param,
            dataType : 'json',
            data: {
                act : 'carregarDadosListaHistorico'
            },
            beforeSend: function( xhr ) {
                App.blockUI({message: 'Carregando...',target: '.page-container'});
            },
            complete: function( result ) {
                App.unblockUI('.page-container');
                $('#tabela-historico-pacotes-' + result.responseJSON.params.tipo).bootstrapTable('load', JSON.parse(result.responseJSON.params.json));
            },
            error: function (result) {
                exibirAlert('Não foi possível carregar a lista de histórico de transações!');
            }
        });
    }
</script>