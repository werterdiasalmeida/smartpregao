<div class="row">
    <div class="col-md-4">
        <div class="form-group">
            <label>1 - Registro ANS</label>
            <?= campo_textom('registro_ans', 'S', $habilitado, '', '', '6', '999999', ''); ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label>2 - Nº Guia no Prestador</label>
            <?= campo_textom('numero_guia_prestador', 'S', 'N', '', '', '20', '', ''); ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label>3 - Nº Guia na Operadora</label>
            <?= campo_textom('numero_guia_operadora', 'N', $habilitado, '', '', '20', '', ''); ?>
        </div>
    </div>
</div>

<h4 class="form-section">Dados do Beneficiário</h4>

<div class="row">
    <div class="col-md-6">
        <div class="form-group">
            <label>4 - Número da Carteira</label>
            <?= campo_textom('numero_carteira', 'S', $habilitado, '', '', '20', '', ''); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>5 - Validade da Carteira</label>
            <?= campo_datam('validade_carteira', 'N', $habilitado, '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>6 - Atendimento a RN</label>
            <?= campo_radiom('atendimento_rn', $mGuia->getDominioRecemNascidoRadio(), 'S', false, $habilitado); ?>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-8">
        <div class="form-group">
            <label>7 - Nome</label>
            <?= campo_textom('nome_beneficiario', 'S', $habilitado, '', '', '70', '', ''); ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label>8 - Cartão Nacional de Saúde</label>
            <?= campo_textom('cartao_nacional_saude', 'N', $habilitado, '', '', '15', '', ''); ?>
        </div>
    </div>
</div>

<h4 class="form-section">Dados do Contratado</h4>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>9 - Código na Operadora</label>
            <?= campo_textom('codigo_operadora', 'S', $habilitado, '', '', '14', '', ''); ?>
        </div>
    </div>
    <div class="col-md-6">
        <div class="form-group">
            <label>10 - Nome do Contratado</label>
            <?= campo_textom('nome_contratado', 'S', $habilitado, '', '', '70', '', ''); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>11 - Código CNES</label>
            <?= campo_textom('cnes', 'S', $habilitado, '', '', '7', '', ''); ?>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-4">
        <div class="form-group">
            <label>12 - Nome do Profissional Executante</label>
            <?= campo_textom('nome_profissional', 'S', $habilitado, '', '', '70', '', ''); ?>
        </div>
    </div>
    <div class="col-md-2">
        <div class="form-group">
            <label>13 - Conselho</label>
            <?= $db->monta_combom('conselho_profissional', $mGuia->getDominioConselho(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-2">
        <div class="form-group">
            <label>14 - Nº Conselho</label>
            <?= campo_textom('numero_conselho_profissional', 'S', $habilitado, '', '', '15', '', ''); ?>
        </div>
    </div>
    <div class="col-md-2">
        <div class="form-group">
            <label>15 - UF</label>
            <?= $db->monta_combom('uf_conselho_profissional', $mGuia->getDominioUf(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-2">
        <div class="form-group">
            <label>16 - Código CBO</label>
            <?= $db->monta_combom('cbos', $mGuia->getDominioCBO(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
</div>


<h4 class="form-section">Dados do Atendimento / Procedimento Realizado</h4>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>17 - Indicação de Aciente</label>
            <?= $db->monta_combom('indicacao_acidente', $mGuia->getDominioIndicacaoAcidente(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>18 - Data do Atendimento</label>
            <?= campo_datam('data_atendimento', 'S', $habilitado, '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>19 - Tipo de Consulta</label>
            <?= $db->monta_combom('tipo_consulta', $mGuia->getDominioTipoConsulta(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>20 - Tabela</label>
            <?= $db->monta_combom('codigo_tabela', $mGuia->getDominioTipoTabela(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-6">
        <div class="form-group">
            <label>21 - Código do Procedimento</label>
            <?= campo_textom('codigo_procedimento', 'S', $habilitado, '', '', '10', '9999999999', ''); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>22 - Valor do Procedimento</label>
            <?php
            $valor_procedimento = formata_valor((!empty($valor_procedimento) ? $valor_procedimento : null ), 2);
            echo campo_textom('valor_procedimento', 'S', $habilitado, '', '', '10', '', '');
            ?>
        </div>
    </div>
    <?php if($exibirValorProfissional) : ?>
        <div class="col-md-3">
            <div class="form-group">
                <label class="label-vlr-profissional">
                    Valor do profissional
                </label>
                <?php
                $valor_profissional = formata_valor((!empty($valor_profissional) ? $valor_profissional : null ), 2);
                echo campo_textom('valor_profissional', 'S', $habilitado, '', '', '10', '', '', 'right', '', 0, '', '', null, '', null, null, null, 'monetario', true);
                ?>
            </div>
        </div>
    <?php endif; ?>
</div>

<div class="row">
    <div class="col-md-12">
        <div class="form-group">
            <label>23 - Observação / Justificativa</label>
            <?= campo_textaream('observacao', 'N', $habilitado, '', 5, 5, 1000); ?>
        </div>
    </div>
</div>

<style type="text/css">
    .control-label .required, .form-group .required {
        color: #000;
    }
</style>

<script type="text/javascript">
    $(function () {
        let guiaForm = $('.guia-form');

        adicionarLabelObrigatorio(guiaForm.find(':required'));
        adicionarDatePicker(guiaForm);

        guiaForm.find('[name=valor_procedimento], [name=valor_profissional]').inputmask('decimal', {
            integerDigits: 6,
            autoGroup: true,
            digits: 2,
            groupSeparator: '.',
            radixPoint: ',',
            rightAlign: true,
            digitsOptional: false,
            placeholder: '0,00',
            allowMinus: false,
            allowPlus: false
        });

        guiaForm.find('.bs-select').selectpicker('refresh');
        guiaForm.validate().destroy();
        <?php if($habilitado === 'S') : ?>
            guiaForm.validateForm({
                rules: {},
                submitHandler: function (e) {
                    App.blockUI({
                        message: 'Carregando...',
                        target: guiaForm
                    });

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType: 'json',
                        data: guiaForm.serialize(),
                        success: function (retorno) {
                            exibirMsg(retorno.msg);
                            carregarGuia(<?= $_REQUEST['agenda_id'] ?>);
                        },
                        error: function (result) {
                            App.unblockUI(guiaForm);
                            exibirAlert('Não foi possível salvar a guia');
                        }
                    });
                }
            });
            setTussAutocomplete();
        <?php endif; ?>

        $('[name=valor_procedimento]').change(calcularValorProfissional);
        $('.label-vlr-profissional').tooltip({
            html : true,
            title : 'Total de Procedimentos x <?= formata_valor($dadosAgenda['percentual_profissional'], 0) ?>% = Valor do profissional'
        });
    });

    function setTussAutocomplete () {
        $('[name=codigo_procedimento]').selectize({
            persist: false,
            maxItems: 1,
            valueField: 'codigo',
            labelField: 'descricao',
            searchField: ['descricao', 'codigo'],
            options: <?= $tussSelecionadosJson ?>,
            render: {
                item: function(item, escape) {
                    return '<div>' + escape(item.codigo) + '</strong> - ' +
                        escape(item.descricao) +
                        '</div>';
                },
                option: function(item, escape) {
                    return '<div>' +
                        '<strong>' + escape(item.codigo) + '</strong> - ' +
                        escape(item.descricao) +
                        '</div>';
                }
            },
            load: function(query, callback) {
                if (!query.length) {
                    return callback();
                }

                $.ajax({
                    url: window.location.href,
                    type: 'POST',
                    dataType : 'json',
                    data : {
                        descricao : query,
                        act : 'buscarTuss'
                    },
                    error: function() {
                        callback();
                    },
                    success: function(response) {
                        callback(response);
                    }
                });
            }
        });
    }

    function calcularValorProfissional() {
        let percentual = parseFloat('<?= $dadosAgenda['percentual_profissional'] ?>');
        let totalProcedimentos = valueToFloat($('[name=valor_procedimento]').val());
        $('[name=valor_profissional]').val(mascaraglobal('[###.]###,##', (totalProcedimentos * (percentual / 100)).toFixed(2)));
    }
</script>