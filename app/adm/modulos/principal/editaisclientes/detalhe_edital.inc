<?php
$mEstabelecimento = new Estabelecimento();
$mEdital = new Edital();
$mEditalCliente = new EditalCliente();
$params = getParam();
$mEdital->carregarPorId($params['id']);
$dados = $mEdital->getDados();
extract($dados);

    if ($municipio_id) {
        $mMunicipio = new Municipio($municipio_id);
        $mUf = new UnidadeFederacao($mMunicipio->uf_id);
        $uf_sigla = $mUf->uf_sigla;
    }
?>
<br />
<!-- <center>
<button class="btn yellow" onclick="solicitarServico(<?= $params['id'] ?>)" type="button" data-toggle="tooltip" title="Quero Solicitar um Serviço!">
    <i class="fa fa-star"></i> Quero Solicitar um Serviço para este Pregão!
</button>
</center> -->

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
    <div class="col-md-4">
        <div class="form-group">
            <label><b>Valor Estimado</b></label>
            <br />
            <?php echo $dados['valor_estimado'] ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label><b>Temperatura</b></label>
            <br />
            <?php echo $dados['temperatura'] ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label><b>UF</b></label>
            <br />
            <?php echo $uf_sigla ?>
        </div>
    </div>
</div>
<div class="row">
    <div class="col-md-12">
        <div class="accordion accordion-solid accordion-toggle-plus" id="accordionExample3">
            <div class="card">
                <div class="card-header" style="cursor: pointer ;" id="headingOne3">
                    <div class="card-title" data-toggle="collapse" data-target="#collapseOne3">
                        <b>Objeto</b>
                    </div>
                </div>
                <div id="collapseOne3" class="collapse show" data-parent="#accordionExample3">
                    <div class="card-body">
                        <?php echo nl2br($dados['objeto']) ?>
                    </div>
                </div>
            </div>
            <div class="card">
                <div class="card-header" style="cursor: pointer ;" id="headingTwo3">
                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseTwo3">
                        <br /><b>+ Comentários</b>
                    </div>
                </div>
                <div id="collapseTwo3" class="collapse" data-parent="#accordionExample3">
                    <div class="card-body">
                        <?php echo nl2br($dados['comentarios']) ?>
                    </div>
                </div>
            </div>
            <div class="card">
                <div class="card-header" style="cursor: pointer ;" id="headingThree3">
                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseThree3">
                        <br /> <b>+ Habilitação Técnica</b>
                    </div>
                </div>
                <div id="collapseThree3" class="collapse" data-parent="#accordionExample3">
                    <div class="card-body">
                        <?php echo nl2br($dados['habilitacao_tecnica']) ?>
                    </div>
                </div>
            </div>
            <div class="card">
                <div class="card-header" style="cursor: pointer ;" id="headingThree4">
                    <div class="card-title collapsed" data-toggle="collapse" data-target="#collapseThree4">
                        <br /> <b>+ Serviço a ser executado</b>
                    </div>
                </div>
                <div id="collapseThree4" class="collapse" data-parent="#accordionExample4">
                    <div class="card-body">
                        <?php echo nl2br($dados['resumo_servico']) ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<br />
</div>
<div class="row">
    <div class="col-md-12">
        <div class="form-group">
            <label><b>Pasta do Google Drive</b></label>
            <br />
            <?php
            if ($dados['pasta_g_drive']) {
                echo "<a href=\"{$dados['pasta_g_drive']}\"> Abrir </a>";
            } else {
                echo " - ";
            }
            ?>
        </div>
    </div>
</div>
<script>
    function solicitarServico(id) {
        window.location = '?modulo=principal/editaisclientes/servicos_pregao&acao=C&p=' + id;
    }
</script>