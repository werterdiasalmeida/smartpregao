<form role="form" name="formulario" id="form-contrato" method="POST" enctype="multipart/form-data">
    <input type="hidden" name="act" value="salvarContrato" />
    <input type="hidden" name="editalcliente_contrato_id" value="<?php echo $editalcliente_contrato_id?>" />
    <div class="row">
        <div class="col-md-6">
            <div class="form-group">
                <label><b>N�mero do Contrato</b></label>
                <br />
                <?= campo_textom('numero_contrato', 'S', 'S', '', 255, 255, '', '', '', '', 0); ?>
            </div>
        </div>
        <div class="col-md-6">
            <div class="form-group">
                <label><b>Processo Adminsitrativo</b></label>
                <br />
                <?= campo_textom('processo_adminitrativo', 'S', 'S', '', 255, 255, '', '', '', '', 0); ?>
            </div>
        </div>
    </div>
    <div class="row">
    <div class="col-md-3">
            <div class="form-group">
                <label><b>In�cio</b></label>
                <br />
                <?= campo_datam('inicio_contrato', 'S', 'S', '', 'S'); ?>
            </div>
        </div>
        <div class="col-md-3">
            <div class="form-group">
                <label><b>T�rmino</b></label>
                <br />
                <?= campo_datam('vencimento_contrato', 'S', 'S', '', 'S'); ?>
            </div>
        </div>
        <div class="col-md-6">
            <div class="form-group">
                <label><b>Edital</b></label>
                <br />
                <?php echo $dados['portal'] ?>
                <br>
                <?php echo $dados['edital'] ?>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-md-6">
            <div class="form-group">
                <label><b>Respons�vel Interno</b></label>
                <br />
                <?= campo_textom('responsavel_interno', 'S', 'S', '', 255, 255, '', '', '', '', 0); ?>
            </div>
        </div>
        <div class="col-md-6">
            <div class="form-group">
                <label><b>Valor total (R$)</b></label>
                <br />
                <b class="font-blue-sharp">
                    <?= campo_textom('valor_total', 'S', 'S', '', 255, 255, '', '', '', '', 0); ?>
                </b>
            </div>
        </div>
    </div>
    <button type="submit" class="btn green">Salvar</button>
</form>
<script>
    $('#valor_total').mask('#.##0,00', {
        reverse: true
    });
</script>