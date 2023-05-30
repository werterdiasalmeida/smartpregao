<div class="row">
    <div class="col-md-6">
        <div class="form-group">
            <label><b>Portal</b></label>
            <br />
            <?php echo $dados['portal'] ?>
        </div>
    </div>
    <div class="col-md-6">
        <div class="form-group">
            <label><b>Chamamento/Edital/Referência</b></label>
            <br />
            <?php echo $dados['edital'] ?>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-6">
        <div class="form-group">
            <label><b>Data Limite</b></label>
            <br />
            <?php echo formata_data($dados['data_limite']) ?>
        </div>
    </div>
    <div class="col-md-6">
        <div class="form-group">
            <label><b>Hora Limite</b></label>
            <br />
            <?php echo $dados['hora_limite'] ?>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-6">
        <div class="form-group">
            <label><b>Valor Total (R$)</b></label>
            <br />
            <div class="input-group">
                <?= campo_textom('valor_estimado', 'N', 'S', '', 255, 255, '', '', '', '', 0); ?>
                <span class="input-group-btn">
                    <button class="btn blue" type="submit" data-toggle="tooltip" title="Salvar">
                        <i class="bi bi-save"></i>
                    </button>
                </span>
            </div>
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
    <div class="col-md-4">
        <div class="form-group">
            <label><b>Categoria</b></label>
            <br />
            <?php echo $dados['categoria'] ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label><b>Estado</b></label>
            <br />
            <?php echo $dados['uf'] ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label><b>Município</b></label>
            <br />
            <?php echo $dados['municipio'] ?>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <b>Objeto</b> <br />
        <?php echo nl2br($dados['objeto']) ?>
        <br /><br />
    </div>
</div>
<script>
    $('#valor_estimado').mask('#.##0,00', {
        reverse: true
    });
</script>