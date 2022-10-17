<?php
$p = getParam();
$profissionalId = $_REQUEST['profissional'];
$procedimentoId = $_REQUEST['procedimento'];
$convenioId = $_REQUEST['convenio'];

$mValorProcedimento = new ValorProcedimento();
$mTabelaProcConvenio = new TabelaProcedimentoConvenio();
$tabelaPrecoId = $mTabelaProcConvenio->getIdByTabelaPrecoConvenio($p['tabela_preco_id'], $convenioId);

$estProcProfissionalId = null;
if($profissionalId) {
    $mProfissionalEstProc = new ProfissionalEstabelecimentoProcedimento();
    $estProcProfissionalId = $mProfissionalEstProc->getIdByEstabelecimentoProcedimentoProfissional($p['id'], $procedimentoId, $profissionalId);
}

if(empty($tabelaPrecoId)) {
    return;
}

$tabelaValorProcedimentoId = $mValorProcedimento->getIdByProcedimentoTabela($procedimentoId, $tabelaPrecoId, $estProcProfissionalId);

$mValorCodigo = new ValorProcedimentoCodigo();
$codigos = $mValorCodigo->getByProcedimentoValor($_REQUEST['procedimento'], $tabelaValorProcedimentoId);

if(count($codigos) > 0) : ?>
    <table class="valor-procedimento-codigo table table-bordered table-striped">
        <thead>
            <tr>
                <th style="width: 20%;">
                    Código
                </th>
                <th style="width: 60%;">
                    Procedimento
                </th>
                <th style="width: 20%;">
                    Valor (R$)
                </th>
            </tr>
        </thead>
        <tbody>
            <?php foreach($codigos as $codigo) : ?>
                <tr>
                    <td>
                        <?= $codigo['codigo'] ?>
                    </td>
                    <td>
                        <?= $codigo['procedimento'] ?>
                    </td>
                    <td>
                        <?= campo_textom("valor[{$codigo['codigo']}]", 'N', 'S', '', null, '25', '', '', 'left', '', 0, "data-codigo='{$codigo['codigo']}'", '', $codigo['valor'], '', null, null, '', 'monetario'); ?>
                    </td>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>
<?php endif; ?>
<script type="text/javascript">
    $(function() {
        setTimeout(function(){
            let tabelaValores = $('table.valor-procedimento-codigo:visible');
            let camposValor = tabelaValores.find('.monetario');
            let valorMonetario = $('[name=valorProcedimento]:visible');

            camposValor.inputmask('decimal', {
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

            valorMonetario.prop('disabled', camposValor.length > 0);

            camposValor.change(function(){
                let valorTotal = 0;
                camposValor.each(function(){
                    valorTotal += valueToFloat($(this).val());
                });

                valorMonetario.val(mascaraglobal('[###.]###,##', valorTotal.toFixed(2)));
                valorMonetario.trigger('keyup');
            });
        }, 300);
    });
</script>