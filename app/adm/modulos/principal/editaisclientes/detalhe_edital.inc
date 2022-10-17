<?php
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$params = getParam();
$mEditalCliente->carregarPorId($params['id']);
$dados = $mEditalCliente->getDados();
$mEdital->carregarPorId($dados['id']);
$dados = $mEdital->getDados();
?>
<br />
<center>
<button class="btn yellow" onclick="solicitarServico(<?= $params['id']?>)" type="button" data-toggle="tooltip" title="Quero Solicitar um Serviço!">
    <i class="fa fa-star"></i> Quero Solicitar um Serviço para este Pregão!
</button>
</center>
<br />
<br />
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
        <div class="form-group">
            <label><b>Objeto</b></label>
            <br />
            <br />
            <?php echo $dados['objeto'] ?>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div class="form-group">
            <label><b>Comentários</b></label>
            <br />
            <br />
            <?php echo $dados['comentarios'] ?>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-6">
        <div class="form-group">
            <label><b>Pasta do Google Drive</b></label>
            <br />
            <?php echo $dados['pasta_g_drive'] ?>
        </div>
    </div>
    <div class="col-md-6">
        <div class="form-group">
            <label><b>Links de acesso</b></label>
            <br />

        </div>
    </div>
</div>
<script>

function solicitarServico(id){
    window.location = '?modulo=principal/editaisclientes/servicos_pregao&acao=C&p='+id;
}

</script>