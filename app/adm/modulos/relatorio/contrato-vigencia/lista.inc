<?php

$mContrato = new Contrato();
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'abrirListaDeFaturas' :
        case 'listarFaturas' :
        case 'enviarLembreteEmail' :
        case 'enviarEmailAvisoCancelamento' :
            include_once 'listaFaturas.inc';
            die;
            break;
        case 'carregarMunicipios':
            $uf_sigla = $_POST['uf_sigla'];
            $municipio_id = $_POST['municipio_id'];
            $munHabilitado = 'N';
            $municipios = array();

            if ($uf_sigla) {
                $munHabilitado = 'S';

                $mUnidadeFederacao = new UnidadeFederacao();
                $ufId = $mUnidadeFederacao->getIdBySigla($uf_sigla);

                $mMunicipio = new Municipio();
                $municipios = $mMunicipio->recuperarTodos('id AS codigo, municipio AS descricao', array("uf_id = '{$ufId}'"), 2);
            }

            $db->monta_combom("municipio_id", $municipios, $munHabilitado, 'Selecione...', '', '', '', '', 'S');
            echo "<script  type='text/javascript'>
                        $(function(){
                            $('[name=municipio_id]').selectpicker('refresh');
                        });
                  </script>";
            die;
            break;
        case 'pesquisa_simples' :
            $pesquisaAvancada = false;

            if (!empty($_REQUEST['pesquisa_simples'])) {
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
                $arWhere[] = "(
                    (pjr.nome_fantasia ILIKE '%{$pesquisa_simples}%') OR
                    (pjr.cnpj ILIKE '%{$pesquisa_simples}%') OR
                    (con.numero ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
        case 'pesquisa_avancada' :
            $pesquisaAvancada = true;

            if (!empty($_REQUEST['nome_fantasia'])) {
                $nome_fantasia = trim(addslashes($_REQUEST['nome_fantasia']));
                $arWhere[] = "pjr.nome_fantasia ILIKE '%{$nome_fantasia}%' ";
            }


            if (!empty($_REQUEST['uf_id'])) {
                $uf_id = $_REQUEST['uf_id'];
                $arrWhereEndereco = ["excluido IS FALSE", "uf_id = '{$_REQUEST['uf_id']}'"];

                if (!empty($_REQUEST['municipio_id'])) {
                    $municipio_id = $_REQUEST['municipio_id'];
                    $arrWhereEndereco[] = "municipio_id = {$_REQUEST['municipio_id']}";
                }

                $arWhere[] = "pjr.id IN ( SELECT 
                                    pessoajuridica_id
                                FROM 
                                    corporativo.tb_endereco_pessoajuridica
                                WHERE
                                    " . implode(" AND ", $arrWhereEndereco) . ")";
            }

            if (!empty($_REQUEST['cnpj'])) {
                $cnpj = trim(addslashes($_REQUEST['cnpj']));
                $arWhere[] = "pjr.cnpj ILIKE '%{$cnpj}%' ";
            }

            if (!empty($_REQUEST['situacoes'])) {
                $situacoes = $_REQUEST['situacoes'];
                $arWhere[] = "con.situacao IN ( '" . implode("', '", $situacoes) . "' ) ";
            }

            $arrWherePeriodo = [];
            if (!empty($_REQUEST['inicio_vigencia'])) {
                $inicio_vigencia = $_REQUEST['inicio_vigencia'];
                $inicio_vigencia_sql = formata_data_sql($inicio_vigencia);
                $arrWherePeriodo[] = "'{$inicio_vigencia_sql}'::date BETWEEN con.inicio_vigencia AND con.fim_vigencia";
            }

            if (!empty($_REQUEST['fim_vigencia'])) {
                $fim_vigencia = $_REQUEST['fim_vigencia'];
                $fim_vigencia_sql = formata_data_sql($fim_vigencia);
                $arrWherePeriodo[] = "'{$fim_vigencia_sql}'::date BETWEEN con.inicio_vigencia AND con.fim_vigencia";
            }

            if(count($arrWherePeriodo) > 0) {
                $arWhere[] = "(" . implode(" OR ", $arrWherePeriodo) . ")";
            }

            if($_REQUEST['numero']){
                $numero = trim($_POST['numero']);
                $arWhere[] = "con.numero ILIKE '%{$numero}%'";
            }
            break;
    }
}

if (!$situacoes) {
    $situacoes = [
        Contrato::SITUACAO_CONTRATO_ATIVO,
        Contrato::SITUACAO_CONTRATO_PENDENTE_PAGAMENTO
    ];
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Contratos');
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-file-text-o font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Relatório de Contratos
            </span>
        </div>
    </div>

    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="pesquisa-form" method="POST">
                <div class="form-body">
                    <div class="pesquisa-simples <?= ($pesquisaAvancada ? 'hidden' : '') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por CNPJ, nome fantasia ou número do contrato'); ?>
                                        <span class="input-group-btn">
                                            <button class="btn blue" type="submit" data-toggle="tooltip"
                                                    title="Pesquisar">
                                                <i class="fa fa-search"></i>
                                            </button>
                                        </span>
                                        <span class="input-group-btn" data-toggle="tooltip" title="Pesquisa avançada">
                                            <button class="btn green-jungle btn-pesquisa-avancada" type="button">
                                                <i class="fa fa-search-plus"></i>
                                            </button>
                                        </span>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>


                    <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>CNPJ</label>
                                    <?= campo_textom('cnpj', 'N', 'S', '', 19, 19, '99.999.999/9999-99', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Nome fantasia</label>
                                    <?= campo_textom('nome_fantasia', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Estado</label>
                                    <?php
                                    $mUnidadeFederacao = new UnidadeFederacao();
                                    $estados = $mUnidadeFederacao->recuperarTodos('id AS codigo, uf_nome_estado AS descricao', array(), 2);
                                    $db->monta_combom("uf_id", $estados, 'S', 'Selecione...', '', '', '', '', 'N');
                                    ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Cidade</label>
                                    <div id="cidades">
                                        <?php
                                        $munHabilitado = 'N';
                                        $municipios = array();
                                        $db->monta_combom("municipio_id", $municipios, $munHabilitado, 'Selecione...', '', '', '', '', 'S');
                                        ?>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-6" style="padding-bottom: 10px;">
                                <div class="control-label">
                                    Período de
                                </div>
                                <div class="input-group" style="padding-top: 6px;">
                                    <div>
                                        <input type="text" id="inicio_vigencia" name="inicio_vigencia"
                                               value="<?= $inicio_vigencia ?>"
                                               class="normal data-dropdown form-control" onchange=""
                                               data-date-format="dd/mm/yyyy">
                                    </div>
                                    <span class="input-group-addon">Até</span>
                                    <div>
                                        <input type="text" id="fim_vigencia" name="fim_vigencia"
                                               value="<?= $fim_vigencia ?>"
                                               class="normal data-dropdown form-control" onchange=""
                                               data-date-format="dd/mm/yyyy">
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label class="control-label">Número de Contrato</label>
                                    <?= campo_textom('numero', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                </div>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-sm-6">
                                <div class="form-group">
                                    <label class="control-label">Situações</label>
                                    <div class="col-sm-12 mt-checkbox-inline container-combo-situacao-prossional-agenda">
                                        <?= $db->monta_checkboxm('situacoes[]', Contrato::getSituacoesContratoParaCheckbox(), 'S', '', $situacoes); ?>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-actions">
                    <div class="row">
                        <div class="col-xs-7 col-sm-5">
                            <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                                <button type="submit" class="btn blue">
                                    <i class="fa fa-search"></i> Pesquisar
                                </button>
                                <button type="button" class="btn green-jungle btn-pesquisa-simples">
                                    <i class="fa fa-search-minus"></i> Pesquisa simples
                                </button>
                            </div>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act"
                       value="<?= ($pesquisaAvancada ? 'pesquisa_avancada' : 'pesquisa_simples') ?>"/>
            </form>
        </div>
    </div>

    <?php
    $situacaoFaturaPaga = Fatura::SITUACAO_FATURA_PAGA;
    $situacaoFaturaVencida = Fatura::SITUACAO_FATURA_VENCIDA;
    $situacaoFaturaGerada = Fatura::SITUACAO_FATURA_GERADA;

    $arConfig = array();

    $arConfig[0]['style'] = "width: 150px;";
    $arConfig[1]['style'] = "width: 150px;";
    $arConfig[4]['style'] = "text-align: center;";
    $arConfig[4]['html'] = "<php>return htmlLabelSituacao('{situacao}');</php>";
    $arConfig[5]['style'] = "text-align: right;";
    $arConfig[6]['style'] = $arConfig[5]['style'];
    $arConfig[7]['style'] = $arConfig[5]['style'];

    $arConfig[5]['html'] = <<<HTML
    <php>
        if ('{qtd_parcelas_pagas}' > 0) {
            return '{qtd_parcelas_pagas} 
                    <a href="javascript:abrirListaDeFaturas(\'' . setParam(array('contrato_id' => '{id}', 'situacao' => '{$situacaoFaturaPaga}'), false) . '\');" class="label label-sm label-success" data-toggle="tooltip" title="Vizualisar Faturas Pagas">
                        <i class="fa fa-search"></i>
                    </a>';
        } else {
            return '{qtd_parcelas_pagas}';
        }
    </php>
HTML;

    $arConfig[6]['html'] = <<<HTML
    <php>
        if ('{qtd_parcelas_vencidas}' > 0) {
            return '{qtd_parcelas_vencidas} 
                    <a href="javascript:abrirListaDeFaturas(\'' . setParam(array('contrato_id' => '{id}', 'situacao' => '{$situacaoFaturaVencida}'), false) . '\');" class="label label-sm label-success" data-toggle="tooltip" title="Vizualisar Faturas Vencidas">
                        <i class="fa fa-search"></i>
                    </a>';
        } else {
            return '{qtd_parcelas_vencidas}';
        }
    </php>
HTML;

    $arConfig[7]['html'] = <<<HTML
    <php>
        if ('{qtd_parcelas_a_vencer}' > 0) {
            return '{qtd_parcelas_a_vencer} 
                    <a href="javascript:abrirListaDeFaturas(\'' . setParam(array('contrato_id' => '{id}', 'situacao' => '{$situacaoFaturaGerada}'), false) . '\');" class="label label-sm label-success" data-toggle="tooltip" title="Vizualisar Faturas à Vencer">
                        <i class="fa fa-search"></i>
                    </a>';
        } else {
            return '{qtd_parcelas_a_vencer}';
        }
    </php>
HTML;

    $rs = $mContrato->getListaRelatorio($arWhere);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho(array(
        'Número do Contrato',
        'CNPJ',
        'Nome fantasia',
        'Vigência',
        'Situação',
        'Qtd. Faturas Pagas',
        'Qtd. Faturas Vencidas',
        'Qtd. Faturas à Vencer'
    ));
    $mLista->setCorpo($rs, $arConfig);
    $mLista->setAcao($acao);
    $mLista->show();
    ?>

    <form id="form" method="post">
        <input type="hidden" id="act" name="act"/>
        <input type="hidden" id="p" name="p"/>
    </form>
</div>

<div class="modal fade" id="faturas-modal" tabindex="-1" role="basic" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h2 class="modal-title">Lista de faturas</h2>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn dark btn-outline" data-dismiss="modal">Fechar</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<script type="text/javascript">
    $(function () {
        $('#inicio_vigencia').datepicker({
            orientation: "left",
            autoclose: true,
            language: 'pt-BR'
        }).on('changeDate', function () {
            $('#fim_vigencia').datepicker('setStartDate', new Date($(this).data('datepicker').getFormattedDate('yyyy-mm-dd 00:00:00')));
        });

        $('#fim_vigencia').datepicker({
            orientation: "left",
            autoclose: true,
            language: 'pt-BR'
        }).on('changeDate', function () {
            $('#inicio_vigencia').datepicker('setEndDate', new Date($(this).data('datepicker').getFormattedDate('yyyy-mm-dd 00:00:00')));
        });

        isMarcarValorTodos();
        $("[name='situacoes[]']:not([id='situacoes[]_'])").change(function () {
            isMarcarValorTodos();
        });

        $("[id='situacoes[]_']").change(function () {
            marcarTodasSituacoes();
        });

        $('select[name=uf_sigla]').change(function () {
            carregarMunicipio();
        });
    });

    function carregarMunicipio(municipio_id) {
        var uf_id = $('select[name=uf_id]').val();
        App.blockUI({
            message: 'Carregando...',
            target: '#cidades'
        });

        $('div#cidades').load(window.location.href, {
            act: 'carregarMunicipios',
            uf_id: uf_id,
            municipio_id: municipio_id
        }, function () {
            App.unblockUI('#cidades');
        });
    }

    function abrirListaDeFaturas(pParams) {
        var modal = $('#faturas-modal');
        modal.find('.modal-body').html('');

        modal.modal({
            backdrop: 'static'
        }).modal('show');

        App.blockUI({
            message: 'Carregando...',
            target: '#faturas-modal .modal-body'
        });

        modal.find('.modal-body').load(window.location.href, {
            act: 'abrirListaDeFaturas',
            p: pParams
        }, function () {
            App.unblockUI('#faturas-modal .modal-body');
        });
    }

    function isMarcarValorTodos() {
        var isTodasOpcoesMarcadas =
            $("[name='situacoes[]']:not([id='situacoes[]_'])").length === $("[name='situacoes[]']:checked:not([id='situacoes[]_'])").length;

        if (isTodasOpcoesMarcadas) {
            $("[id='situacoes[]_']").prop('checked', true);
        } else {
            $("[id='situacoes[]_']").prop('checked', false);
        }
    }

    function marcarTodasSituacoes() {
        var flgTodasSituacoes = $("[id='situacoes[]_']").prop('checked');

        if (flgTodasSituacoes) {
            $("[name='situacoes[]']").prop('checked', true);
        } else {
            $("[name='situacoes[]']").prop('checked', false);
        }
    }
</script>

<?php
function htmlLabelSituacao($situacao) {
    $arrSitaucaoContrato = Contrato::getArraySituacaoContrato();

    return "<span class='label label-sm' style='background-color: {$arrSitaucaoContrato[$situacao]['cor']};'>{$arrSitaucaoContrato[$situacao]['descricao']}</span>";
}