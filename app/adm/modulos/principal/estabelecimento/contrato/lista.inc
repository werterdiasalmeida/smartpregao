<?php
$p = getParam();
UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);
$estabelecimentoId = $p['id'];

$mContrato = new Contrato();
$mContratoFatura = new ContratoFatura();
$dadosContrato = $mContrato->getDadosContratoVigentePorEstabelecimento($estabelecimentoId);

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'visualizarFatura':
            $urlFatura = $mContratoFatura->getUrlFaturaIuguPorId($p['contrato_fatura_id']);
            $db->commit();

            header("location:{$urlFatura}");
            break;
        case 'enviarFaturaPorEmail' :
            $sucessoEnvio = $mContratoFatura->enviarFaturaIuguParaEmailPorId($p['contrato_fatura_id']);
            $db->commit();
            if ($sucessoEnvio) {
                $db->sucessom('principal/estabelecimento/contrato/lista', setParam(array('id' => $estabelecimentoId)), "Email enviado com sucesso");
            } else {
                $db->insucessom('principal/estabelecimento/contrato/lista', setParam(array('id' => $estabelecimentoId)), 'Não foi possível enviar o Email');
            }
            break;
        case 'gerarSegundaViaFatura' :
            $sucessoEnvio = $mContratoFatura->gerarSegundaViaDeFatura($p['contrato_fatura_id']);
            if ($sucessoEnvio) {
                $db->sucessom('principal/estabelecimento/contrato/lista', setParam(array('id' => $estabelecimentoId)), "Segunda via gerada com sucesso");
            } else {
                $db->insucessom('principal/estabelecimento/contrato/lista', setParam(array('id' => $estabelecimentoId)), 'Não foi possível gerar a fatura');
            }
            break;
        case 'download' :
            $file = new FilesSimec(null, null, 'seguranca');
            $params = getParam();
            $file->getDownloadArquivo($params['comprovante_pagamento']);
            die;
            break;
    }
}

$classContrato = '';
$styleSituacaoContrato = 'padding-left: 24px; padding-right: 24px;';
$dscLabelValorContrato = '';

if (!empty($dadosContrato)) {
    switch ($dadosContrato['situacao']) {
        case Contrato::SITUACAO_CONTRATO_ATIVO:
            $classContrato = 'success';
            $styleSituacaoContrato = "background-color: #3AC386;" . $styleSituacaoContrato;
            break;
        case Contrato::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO:
            $classContrato = 'danger';
            break;
        case Contrato::SITUACAO_CONTRATO_INATIVO:
            $classContrato = 'warning';
            break;
    }

    switch ($dadosContrato['tipo_contrato_id']) {
        case TipoContrato::TIPO_POR_VALOR_FIXO:
            $dscLabelValorContrato = "Valor do Contrato (R$)";
            break;
        case TipoContrato::TIPO_POR_ATENDIMENTO_REALIZADO:
            $dscLabelValorContrato = "Valor pago por atendimento (R$)";
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(getParam()), null, array(), true);
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-file-text-o font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Detalhes do Contrato
            </span>
        </div>
    </div>

    <?php if (!empty($dadosContrato)): ?>
        <div class="portlet-body form">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-3">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Número de Contrato</label>
                                <div class="input-group">
                                    <div>
                                        <strong><?= $dadosContrato['numero'] ?></strong>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="col-md-3">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Tipo de Contrato</label>
                                <div class="input-group">
                                    <div>
                                        <strong><?= $dadosContrato['nome_tipo_contrato'] ?></strong>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="col-md-3">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Situação do Contrato</label>
                                <div class="input-group">
                                    <div>
                                        <span class="label label-sm label-<?= $classContrato ?>"
                                              style="<?= $styleSituacaoContrato ?>">
                                            <?= $dadosContrato['str_situacao'] ?></span>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-md-3">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Período de Contrato</label>
                                <div class="input-group">
                                    <div>
                                        <strong><?= $dadosContrato['nome_periodo'] ?></strong>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="col-md-3">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Vigência do Contrato</label>
                                <div class="input-group">
                                    <div>
                                        <strong><?= formata_data($dadosContrato['inicio_vigencia']) ?>
                                            à <?= formata_data($dadosContrato['fim_vigencia']) ?></strong>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-md-3">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label><?= $dscLabelValorContrato; ?></label>
                                <div class="input-group">
                                    <div>
                                        <strong><?= formata_valor($dadosContrato['valor_contratado']); ?></strong>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

            </div>
        </div>

        <?php
        $situacaoFaturaVencida = Fatura::SITUACAO_FATURA_VENCIDA;
        $situacaoFaturaGerada = Fatura::SITUACAO_FATURA_GERADA;
        $situacaoFaturaPendente = Fatura::SITUACAO_FATURA_PENDENTE;
        $situacaoFaturaPaga = Fatura::SITUACAO_FATURA_PAGA;

        $strVisualizarFatura = <<<ACAO
            <a class="btn btn-circle btn-icon-only btn-default green-jungle" href="javascript:visualizarFatura(\'' . setParam(array('contrato_fatura_id' => '{id}', 'id' => '{$estabelecimentoId}'), false) . '\');" data-toggle="tooltip" title="Visualizar Fatura">
                <i class="fa fa-print"></i>
            </a>
ACAO;

        $acao = <<<ACAO
            <div style="min-width: 100px;">
                <php>
                    switch ('{situacao}') {
                        case '{$situacaoFaturaPendente}':
                        case '{$situacaoFaturaGerada}':
                            return '$strVisualizarFatura
                            <a class="btn btn-circle btn-icon-only btn-default blue-steel" href="javascript:enviarFaturaPorEmail(\'' . setParam(array('contrato_fatura_id' => '{id}', 'id' => '{$estabelecimentoId}'), false) . '\');" data-toggle="tooltip" title="Enviar Fatura por Email">
                                <i class="fa fa-envelope"></i>
                            </a>'; 
                        case '{$situacaoFaturaVencida}':
                            return '<a class="btn btn-circle btn-icon-only btn-default yellow-crusta" href="javascript:gerarSegundaViaFatura(\'' . setParam(array('contrato_fatura_id' => '{id}', 'id' => '{$estabelecimentoId}'), false) . '\');" data-toggle="tooltip" title="Gerar segunda via de fatura">
                                <i class="fa fa-clone"></i>
                            </a>'; 
                        case '{$situacaoFaturaPaga}':
                            if('{comprovante_pagamento}' == '') {
                                return '$strVisualizarFatura';
                            } else {
                                return '<a class="btn btn-circle btn-icon-only btn-default green-jungle" href="javascript:downloadComprovante(\'' . setParam(array('comprovante_pagamento' => '{comprovante_pagamento}', 'id' => '{$estabelecimentoId}'), false) . '\');" data-toggle="tooltip" title="Download do comprovante">
                                    <i class="fa fa-download"></i>
                                </a>'; 
                            }
                    }
                </php>
            </div>
ACAO;

        $arConfig = array();
        $arConfig[1]['style'] = "text-align: right;";
        $arConfig[2]['html'] = '<php>return formata_data("{data_vencimento}");</php>';
        $arConfig[3]['html'] = '<php>return formata_data("{data_pagamento}");</php>';

        $arConfig[4]['style'] = "text-align: center; vertical-align: middle;";
        $arConfig[4]['html'] = <<<HTML
    <php>
        switch ('{situacao}') {
            case '{$situacaoFaturaPaga}':
                return '<span class="label label-sm label-success" style="background-color: #3AC386;padding-left: 24px; padding-right: 24px;">{str_situacao}</span>';
            case '{$situacaoFaturaPendente}':
            case '{$situacaoFaturaGerada}':
                return '<span class="label label-sm label-warning" style="padding-left: 13px; padding-right: 13px;">{str_situacao}</span>'; 
            case '{$situacaoFaturaVencida}':
                return '<span class="label label-sm label-danger" style="padding-left: 13px; padding-right: 13px;">{str_situacao}</span>'; 
            default:
                return '<span class="label label-sm label-info" style="padding-left: 13px; padding-right: 13px;">{str_situacao}</span>'; 
        }
    </php>
HTML;

        $arWhere = array(
            "cnf.contrato_id = {$dadosContrato['id']}"
        );
        $rs = $mContratoFatura->getListaVisaoEstabelecimento($arWhere);

        $mLista = new Listam(array(
            "style" => "width: 100%;"
        ));

        $mLista->setCabecalho(array(
            'Descrição',
            'Valor (R$)',
            'Data de vencimento',
            'Data de pagamento',
            'Situação',
            'Ação'
        ));

        $mLista->setCorpo($rs, $arConfig);
        $mLista->setAcao($acao);
        $mLista->show();
    endif;
    ?>

    <?php if (empty($dadosContrato)): ?>
        <div class="alert alert-warning">
            Nenhum contrato vinculado a este estabelecimento.
        </div>
    <?php endif; ?>

    <form id="form-fatura" method="post">
        <input type="hidden" id="act" name="act"/>
        <input type="hidden" id="p" name="p"/>
    </form>
</div>

<script type="text/javascript">
    function enviarFaturaPorEmail(pFatura) {
        $("#form-fatura #act").val('enviarFaturaPorEmail');
        $("#form-fatura #p").val(pFatura);
        $('#form-fatura').submit();
    }

    function gerarSegundaViaFatura(pFatura) {
        $("#form-fatura #act").val('gerarSegundaViaFatura');
        $("#form-fatura #p").val(pFatura);
        $('#form-fatura').submit();
    }

    function visualizarFatura(pFatura) {
        var url = location.pathname +
            '?modulo=principal/estabelecimento/contrato/lista&acao=C&act=visualizarFatura&p=' + pFatura;
        window.open(url, '_newtab');
    }

    function downloadComprovante(id) {
        window.location = "?modulo=principal/estabelecimento/contrato/lista&acao=C&act=download&p=" + id;
    }
</script>