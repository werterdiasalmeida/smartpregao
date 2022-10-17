<?php
$p = getParam();
$habilitado = 'S';
$mLote = new Lote();

if($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'carregarGuias' :
            $mGuia = new Guia();

            $where = [
                "gui.convenio_estabelecimento_id = {$_POST['convenio_estabelecimento_id']}",
                "gui.tipo_guia_id = {$_POST['tipo_guia_id']}"
            ];
            if($_POST['habilitado'] === 'N' && $p['lote_id']) {
                $mLoteGuia = new LoteGuia();
                $guiasSelecionadas = $mLoteGuia->getIdGuiasByLote($p['lote_id']);
                $where[] = "gui.id IN (" . implode(', ', $guiasSelecionadas) .  ")";
            }

            $response = $mGuia->getListAjaxSemLote($where, $_POST['offset'], $_POST['limit'], $p['lote_id']);
            $db->responseAjax($response);
            break;
        case 'formularioGuia':
        case 'salvarGuia':
        case 'imprimirGuia':
        case 'buscarTuss':
            $exibirValorProfissional = false;
            include_once APPRAIZ . 'adm/modulos/principal/estabelecimento/agenda/guia/formulario.inc';
            die;
            break;
        case 'salvar' :
            $params = array_merge($_POST, $p);
            $params['id'] = $params['lote_id'];
            $params['estabelecimento_id'] = $p['id'];

            $loteId = $mLote->salvarLote($params);
            $mLote->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(['id' => $p['id'], 'lote_id' => $loteId]));
            break;
    }
}

$guiasSelecionadas = [];
if($p['lote_id']) {
    $dadosLote = $mLote->getTodosDados($p['lote_id']);
    $guiasSelecionadas = $dadosLote['guias_selecionadas'];
    extract($dadosLote);

    $habilitado = $mLote->isPodeAlterar($p['lote_id']) ? $habilitado : 'N';
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();
UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(['id' => $p['id']]), null, [], true, true);
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-archive font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do lote
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <?php if($habilitado === 'S') : ?>
            <form role="form" name="formulario" class="lote-form" method="POST">
        <?php endif; ?>
            <div class="form-body">
                <?php if(!empty($p['lote_id'])) : ?>
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Número</label>
                                <?= campo_textom('numero', 'S', 'N', '', 90, 250, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <?php wf_desenhaBarraNavegacaoHorizontalM($documento_id, ['id' => $p['lote_id']]) ?>
                            </div>
                        </div>
                    </div>
                <?php endif; ?>

                <div class="row">
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Descrição</label>
                            <?= campo_textom('descricao', 'S', $habilitado, '', 90, 250, '', '', '', '', 0); ?>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Convênio/Plano</label>
                            <?php
                            $mConvenio = new ConvenioEstabelecimento();
                            $convenios = $mConvenio->getConveniosComGuiaByEstabelecimento($p['id'], $convenio_estabelecimento_id);
                            $db->monta_combom('convenio_estabelecimento_id', $convenios, $habilitado, 'Selecione...', '', '', '', '', 'S');
                            ?>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Tipo</label>
                            <?php
                            $mTipoGuia = new TipoGuia();
                            $tiposGuia = $mTipoGuia->getCombo();
                            $db->monta_combom('tipo_guia_id', $tiposGuia, $habilitado, 'Selecione...', '', '', '', '', 'S');
                            ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-12">
                        <h4>Guias</h4>

                        <table
                            id="tabela-guias"
                            data-pagination="true"
                            data-page-size="100"
                            data-side-pagination="server"
                            data-height="650"
                            data-ajax="carregarGuias"
                            data-response-handler="responseHandler"
                            data-page-list="[10, 20, 50, 100]">
                            <thead>
                                <tr>
                                    <th colspan="2"></th>
                                    <th colspan="2" data-align="center">
                                        Guia
                                    </th>
                                    <th colspan="2" data-align="center">
                                        Paciente
                                    </th>
                                    <th colspan="2" data-align="center">
                                        Atendimento
                                    </th>
                                </tr>
                                <tr>
                                    <?php if($habilitado === 'S') : ?>
                                        <th data-field="selecionado" data-checkbox="true" data-formatter="formataCheckSelecione"></th>
                                    <?php endif; ?>
                                    <th data-field="agenda_id" data-align="center" data-formatter="formataAcao" data-width="5%">Ação</th>
                                    <th data-field="guia_numero" data-align="center">Número</th>
                                    <th data-field="guia_tipo">Tipo</th>
                                    <th data-field="paciente_nome">Nome</th>
                                    <th data-field="paciente_num_carteira">Nº Carteira</th>
                                    <th data-field="atendimento_data" data-align="center" data-formatter="formataData">Data</th>
                                    <th data-field="atendimento_valor" data-align="right" data-formatter="formataValor">Valor (R$)</th>
                                </tr>
                            </thead>
                        </table>
                    </div>
                </div>
            </div>

            <div class="form-actions">
                <?php if($habilitado === 'S') : ?>
                    <button type="submit" class="btn blue">
                        <i class="fa fa-save"></i> Salvar
                    </button>
                    <input type="hidden" name="guias_selecionadas" />
                    <input type="hidden" name="act" value="salvar"/>
                <?php endif; ?>
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
            </div>
        <?php if($habilitado === 'S') : ?>
            </form>
        <?php endif; ?>
    </div>
</div>

<div class="modal fade in" id="modal-guia" tabindex="-1" role="modal-guia" aria-hidden="true">
    <div class="modal-dialog" style="width: 90%;">
        <div class="modal-content">
            <form name="formulario-guia" class="guia-form" method="POST">
                <p style="height: 500px;">Carregando...</p>
            </form>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-erros-guia" tabindex="-1" role="modal-erros-guia" aria-hidden="true">
    <div class="modal-dialog modal-lg" >
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Erros da guia Nº <span class="numero_guia"></span></h4>
            </div>
            <div class="modal-body">
                <div class="form-body">
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <ul class="erros"></ul>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="modal-footer">
                <div class="row">
                    <div class="col-sm-12">
                        <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    let FormularioLote = function () {
        let self = this;
        let tabelaGuias = null;
        let guiasSelecionadas = <?= json_encode($guiasSelecionadas) ?>;

        this.init = function () {
            <?php if($habilitado === 'S') : ?>
                $(".lote-form").validateForm({
                    submitHandler : function (e) {
                        let guias = tabelaGuias.bootstrapTable('getData');
                        let guiasSelecionadas = [];
                        guias.forEach(function(guia){
                            if(guia.selecionado) {
                                guiasSelecionadas.push(guia.id);
                            }
                        });

                        if(guiasSelecionadas.length === 0) {
                            exibirAviso('Por favor, selecione ao menos uma guia.');
                            return;
                        } else if (guiasSelecionadas.length > 100) {
                            exibirAviso('Por favor, selecione no máximo 100 guias.');
                            return;
                        }

                        $('[name=guias_selecionadas]').val(JSON.stringify(guiasSelecionadas));
                        this.submitForm(e);
                    }
                });
            <?php endif; ?>

            $('.btn-voltar').click(function () {
                window.location.href = '?modulo=principal/estabelecimento/lote/lista&acao=C<?= setParam(['id' => $p['id']]) ?>';
            });

            tabelaGuias = $('#tabela-guias');
            tabelaGuias.bootstrapTable();
            tabelaGuias.on('check.bs.table check-all.bs.table uncheck.bs.table uncheck-all.bs.table', function () {
                tabelaGuias.bootstrapTable('getData').map(function (row) {
                    if(row.selecionado && $.inArray(row.id, guiasSelecionadas) === -1) {
                        guiasSelecionadas.push(row.id);
                    } else if (!row.selecionado && $.inArray(row.id, guiasSelecionadas) !== -1) {
                        guiasSelecionadas.splice(guiasSelecionadas.indexOf(row.id), 1);
                    }
                });
            });

            $('[name=convenio_estabelecimento_id], [name=tipo_guia_id]').change(function(){
                guiasSelecionadas = [];
                tabelaGuias.bootstrapTable('refresh', {pageNumber : 1});
            });

            $('#modal-guia').on('hidden.bs.modal', function () {
                tabelaGuias.bootstrapTable('refresh');
            });
        };

        this.carregarGuias = function (params) {
            $.ajax(window.location.href, {
                type : 'POST',
                dataType : 'json',
                data : Object.assign(params.data, {
                    act : 'carregarGuias',
                    habilitado : '<?= $habilitado ?>',
                    convenio_estabelecimento_id : $('[name=convenio_estabelecimento_id]').val(),
                    tipo_guia_id : $('[name=tipo_guia_id]').val()
                }),
                success : function (response) {
                    params.success(response);
                },
                error : function (response) {
                    exibirAlert('Não foi possível carregar as guias.');
                    params.error(response);
                    console.error(response);
                }
            });
        };

        this.selecionarMarcados = function (res) {
            $.each(res.rows, function (i, row) {
                row.selecionado = $.inArray(row.id, guiasSelecionadas) !== -1;
            });

            return res;
        };

        this.getGuiaByIndex = function (index) {
            let rows = tabelaGuias.bootstrapTable('getData');
            return rows[index];
        };

        return this;
    };

    let formLote = new FormularioLote();
    $(function(){
        formLote.init();
    });

    function carregarGuias(params) {
        let convenioId = $('[name=convenio_estabelecimento_id]').val();
        let tipoGuiaId = $('[name=tipo_guia_id]').val();

        if(convenioId && tipoGuiaId) {
            formLote.carregarGuias(params);
        } else {
            params.success({
                total: 0,
                totalNotFiltered: 0,
                rows: []
            });
        }
    }

    function responseHandler(rows)
    {
        return formLote.selecionarMarcados(rows);
    }

    function formataData(value)
    {
        return formatarData(value);
    }

    function formataValor(value)
    {
        return 'R$ ' + mascaraglobal('[###.]###,##', value);
    }

    function formataCheckSelecione(value, row)
    {
        if(!row.is_valida) {
            return {
                disabled: true
            }
        }

        return value;
    }

    function formataAcao(value, row, index)
    {
        let label = {
            title : 'Visualizar guia',
            icon : 'icon-magnifier'
        };

        if('<?= $habilitado ?>' === 'S') {
            label = {
                title : 'Alterar guia',
                icon : 'icon-pencil',
            };
        }

        let htmlErros = '';
        if(row.erros && row.erros.length > 0) {
            htmlErros = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:;" onclick="exibirErros('+ index + ');" data-toggle="tooltip" title="A guia possui erros.">' +
                        '    <i class="fa fa-warning"></i>' +
                        '</a>';
        }

        return '<div style="min-width: 100px;">' +
                   '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:;" onclick="carregarGuia(' + value + ');" data-toggle="tooltip" title="' + label.title + '">' +
                   '    <i class="' + label.icon + '"></i>' +
                   '</a>' + htmlErros +
               '</div>';
    }

    function carregarGuia (agendaId) {
        $('#modal-guia').modal({
            backdrop: 'static',
        }).modal('show');

        let guiaForm = $('form.guia-form');
        App.blockUI({
            target : guiaForm,
            message : 'Carregando...'
        });

        guiaForm.load(window.location.href, {
            act : 'formularioGuia',
            agenda_id : agendaId
        }, function() {
            App.unblockUI(guiaForm);
        });
    }

    function exibirErros(index)
    {
        let guia = formLote.getGuiaByIndex(index);
        let modalErros = $('#modal-erros-guia');

        modalErros.find('.numero_guia').html(guia.guia_numero);
        modalErros.find('.erros').html('<li>' + guia.erros.join('</li><li>') + '</li>');

        modalErros.modal({
            backdrop: 'static',
        }).modal('show');
    }
</script>
