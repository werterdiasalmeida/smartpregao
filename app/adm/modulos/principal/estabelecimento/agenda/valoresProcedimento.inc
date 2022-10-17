<?php
$p = getParam();
$profissionalEstabelecimentoProcedimentoId = $_REQUEST['profissional_estabelecimento_procedimento_id'];
$valor_procedimento_id  = $_REQUEST['valor_procedimento_id'] ? $_REQUEST['valor_procedimento_id'] : '';
if (empty($profissionalEstabelecimentoProcedimentoId)) {
    die;
}

$mValorProcedimento = new ValorProcedimento();
$valores = $mValorProcedimento->getValoresVigentesPorProfissionalEstabelecimentoProcedimento($profissionalEstabelecimentoProcedimentoId);

$valoresJson = json_encode(utf8ArrayEncode(array_map(function ($valor) use ($p) {
    if (ValorProcedimento::isVisualizaValores($p['id'])) {
        $valor['valor'] = formata_valor($valor['valor']);
    } else {
        unset($valor['valor']);
    }
    return $valor;
}, $valores)));

$placeholderMsg = 'Selecione o convênio do procedimento';
if (count($valores) == 0) :
    $placeholderMsg = "Nenhum valor encontrado para o procedimento selecionado.";
endif;
?>
<div class="form-group">
    <label>Convênio/Plano </label>
    <select name="valor_procedimento_id" id="valor_procedimento_id" required></select>
</div>
<div class="form-group">
    <label>Forma de Pagamento </label>
    
    <?php
    $mFormaPagamento = new FormaPagamento();
    $formasPagamento = $mFormaPagamento->getComboPorEstabelecimento($convenio_estabelecimento_id);
    $db->monta_combom('forma_pagamento_id', $formasPagamento, $habilitado, 'Selecione...', '', '', '', '', 'S');
    ?>
</div>

<style type="text/css">
    .valor-procedimento {
        border: 1px solid #c2c6c8;
        text-align: center;
        padding: 10px;
        margin: 10px 10px 0 0;
        min-width: 230px;
    }

    .valor-procedimento:hover {
        background-color: #f1f4f9;
        cursor: pointer;
    }

    .valor-procedimento .convenio,
    .valor-procedimento .valor,
    .valor-procedimento .selecao {
        display: block;
        text-align: center;
        font-size: 18px;
        cursor: pointer;
    }

    .valor-procedimento input[type=radio] {
        cursor: pointer;
    }

    .valor-procedimento.selected {
        background-color: #5e9bcf;
        color: #FFF;
    }

    .valor-procedimento .valor {
        margin-bottom: 8px;
        font-weight: bold;
    }
</style>
<script type="text/javascript">
    $(function() {
        var valorProcedimentoId = '<?= $_REQUEST['valor_procedimento_id'] ?>';
        adicionarLabelObrigatorio($('.valores-procedimento'));

        $('#valor_procedimento_id').selectize({
            persist: false,
            valueField: 'id',
            labelField: 'convenio',
            placeholder: '<?= $placeholderMsg ?>',
            searchField: ['convenio'],
            options: <?= $valoresJson ?>,
            render: {
                item: function(item, escape) {
                    return '<div>' +
                        '<span>' + escape(item.convenio) + (item.valor ? ' (R$ ' + escape(item.valor) + ')' : '') + '</span>' +
                        '</div>';
                },
                option: function(item, escape) {
                    return '<div>' +
                        '<div><label style="width: 130px; font-weight: bold;">Convênio/Plano: </label>' + (item.convenio ? '<span>' + escape(item.convenio) + '</span>' : '') + '</div>' +
                        (item.valor ? '<div><label style="width: 130px; font-weight: bold;">Valor: </label>' + (item.valor ? '<span>R$ ' + escape(item.valor) + '</span>' : '') + '</div>' : '') +
                        '</div>';
                }
            },
        }).on('change', function() {
            if ($(this).val()) {
                $('.numero-convenio-plano').show();
                carregarNumeroCartaoConvenio();
            } else {
                $('.numero-convenio-plano').hide();
            }
        });

        if (valorProcedimentoId != "") {
            $('#valor_procedimento_id')[0].selectize.setValue(valorProcedimentoId);
        }

        /*$('[name=valor_procedimento_id]').click(function(){
            $('.valores-procedimento').find('.selected').removeClass('selected');
            $(this).closest('.valor-procedimento').addClass('selected');
        });*/
    });
</script>