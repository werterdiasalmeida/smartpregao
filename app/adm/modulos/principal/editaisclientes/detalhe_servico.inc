<?php
$mTipoServicoAnalise = new TipoServicoAnalise();
$TipoServicoAnalise = $mTipoServicoAnalise->carregarPorId($_REQUEST['servico']);
$dados = $TipoServicoAnalise->getDados();

?>
<div class="row">
    <div class="col-md-12">
        <div class="form-group">
            <label><b>Serviço a ser solicitado</b></label>
            <br />
            <?php echo $dados['descricao'] ?>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div class="form-group">
        <a href="#" class="btn btn-warning" onclick="confirmarServico(<?= $dados['id']?>)">Confirmar Contratação</a>
        </div>
    </div>
</div>

<script>

function confirmarServico(id){
    url = location.href+'&act=contrataServico&servico_id='+id;
    alert(url);
    window.open(location.href+'&act=contrataServico&servico_id='+id);
}

</script>