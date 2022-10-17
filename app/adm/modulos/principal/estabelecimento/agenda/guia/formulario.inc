<?php
$mGuia = new Guia();
$mTipoGuia = new TipoGuia();
$exibirValorProfissional = $exibirValorProfissional !== false;
$p = getParam();

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'salvarGuia' :
            $mGuia->salvarGuia(utf8ArrayDecode($_REQUEST));
            $mGuia->commit();
            $db->sucessomAjax('', '', 'Guia salva com sucesso!');
            break;
        case 'imprimirGuia' :
            include_once 'imprimir.inc';
            die;
            break;
        case 'buscarTuss' :
            $mTuss = new Tuss();
            $dsc = $_POST['descricao'];
            $rs = $mTuss->getList(["(procedimento ILIKE '%{$dsc}%' OR cod_tuss ILIKE '%{$dsc}%')"]);
            die(json_encode(utf8ArrayEncode($rs)));
            break;
    }
}

$guia = $mGuia->getGuiaByAgenda($_REQUEST['agenda_id']);
extract($guia ? $guia['guia'] : []);
$habilitado = $mGuia->isGuiaHabilitada($guia['id']) ? 'S' : 'N';
$mAgenda = new EstabelecimentoProfissionalAgenda();
$dadosAgenda = $mAgenda->getTodosDados($_REQUEST['agenda_id']);
$valor_profissional = $dadosAgenda['valor_profissional'];

$abrirAgendaParam = ['agenda_id' => $_REQUEST['agenda_id'], 'id' => $dadosAgenda['estabelecimento_id']];
?>

<div class="modal-header">
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
    <h4 class="modal-title">Guia</h4>
</div>
<div class="modal-body">
    <div class="form-body">
        <?php
        switch ($mTipoGuia->getTipoById($guia['tipo_guia_id'])) {
            case TipoGuia::TIPO_GUIA_EXAME :
                $executado = is_array($executado) ? array_map(function ($exec) {
                    $exec['qtd_execucao'] = formata_valor($exec['qtd_execucao'], 0);
                    $exec['fator_red_acresc'] = formata_valor($exec['fator_red_acresc'], 2);
                    $exec['valor_unitario'] = formata_valor($exec['valor_unitario'], 2);
                    $exec['valor_total'] = formata_valor($exec['valor_total'], 2);
                    return $exec;
                }, $executado) : [];

                $total_procedimentos = formata_valor($total_procedimentos, 2);
                $total_geral = formata_valor($total_geral, 2);

                include_once 'formExame.inc';
                break;
            default :
                $tussSelecionadosJson = '[]';
                if($codigo_procedimento && is_string($codigo_procedimento)) {
                    $mTuss = new Tuss();
                    $tussSelecionados = $mTuss->getList(["cod_tuss IN ('" . implode("', '", explode(',', $codigo_procedimento)) . "')"]);
                    $tussSelecionadosJson = json_encode(utf8ArrayEncode($tussSelecionados));
                }

                include_once 'formConsulta.inc';
                break;
        }
        ?>
    </div>
</div>
<div class="modal-footer">
    <div class="row">
        <div class="col-sm-4 text-left">
            <button type="button" class="btn green btn-voltar-consulta" onclick="voltarDadosConsulta();">
                <i class="fa fa-arrow-left"></i>
                Voltar
            </button>
        </div>
        <div class="col-sm-8 text-right">
            <?php if($habilitado === 'S') : ?>
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
            <?php endif; ?>
            <a class="btn green-jungle" type="button" target="_blank" href="?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?><?= setParam(array('agenda_id' => $_REQUEST['agenda_id'], 'id' => $p['id'])) ?>&act=imprimirGuia">
                <i class="fa fa-print"></i>
                Imprimir
            </a>
            <button type="button" class="btn" data-dismiss="modal">Fechar</button>
        </div>
    </div>

    <?php if($habilitado === 'S') : ?>
        <input type="hidden" name="act" value="salvarGuia"/>
        <input type="hidden" name="agenda_id" value="<?= $_REQUEST['agenda_id'] ?>"/>
        <input type="hidden" name="tipo_guia_id" value="<?= $guia['tipo_guia_id'] ?>" />
    <?php endif; ?>
</div>

<script type="text/javascript">
    $(function() {
        if(typeof abrirAgenda === "undefined") {
            $('.btn-voltar-consulta').hide();
        }
    });

    function voltarDadosConsulta() {
        $('#modal-guia').modal('hide');
        abrirAgenda('<?= setParam($abrirAgendaParam, false) ?>');
    }
</script>