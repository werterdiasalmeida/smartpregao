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
            <b class="font-blue-sharp"><?php echo $dados['valor_estimado'] ?></b>
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
        <b>Objeto</b> <br />
        <?php echo nl2br($dados['objeto']) ?>
        <br /><br />
    </div>
</div>
