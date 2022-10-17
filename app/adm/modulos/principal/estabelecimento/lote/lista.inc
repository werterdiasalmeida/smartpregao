<?php
$p = getParam();
$mLote = new Lote();

$where = ["lot.estabelecimento_id = {$p['id']}"];
if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'excluir' :
            $mLote->carregarPorId($p['lote_id']);

            $mLote->excluirLogicamente($p['lote_id']);
            $mLote->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])), 'Lote excluído com sucesso');
            break;
        case 'pesquisar' :
            if (!empty($_REQUEST['descricao'])) {
                $descricao = trim(addslashes($_REQUEST['descricao']));
                $where[] = "(
                    lot.numero ILIKE '%{$descricao}%'
                    OR
                    lot.descricao ILIKE '%{$descricao}%'
                    OR
                    con.nome ILIKE '%{$descricao}%'
                    OR
                    esd.esddsc ILIKE '%{$descricao}%'
                    OR
                    tgu.descricao ILIKE '%{$descricao}%'
                )";
            }
            break;
        case 'gerarTiss' :
            $mTiss = new Tiss();
            $xmlTiss = $mTiss->gerar($p['lote_id']);

            $dadosLote = $mLote->getTodosDados($p['lote_id']);
            $dataAtual = date('YmdHis');

            $isValido = $mTiss->isValido();

            if($isValido === true) {
                $nomeArquivo = "lote_{$dadosLote['numero']}_{$dadosLote['sequencial']}_{$dataAtual}.xml";
                header("Content-disposition: attachment; filename={$nomeArquivo}");
                header("Content-type: text/xml");

                die($xmlTiss->saveXML());
            } else {
                $db->insucessom($_REQUEST['modulo'], setParam($p), 'Erro ao gerar o TISS: <br />' . implode('<br />', array_map(function($erro) {
                        return $erro->message;
                    }, $isValido)));
            }

            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();
UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true);

?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-archive"></i>
            <span class="caption-subject bold uppercase">
                Lista de lotes
            </span>
        </div>
    </div>
    <div class="portlet-body">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" method="POST">
                <div class="form-body">
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <div class="input-group">
                                    <?= campo_textom('descricao', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por número, descrição, convênio ou situação'); ?>
                                    <span class="input-group-btn">
                                        <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                            <i class="fa fa-search"></i>
                                        </button>
                                    </span>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-actions">
                    <div class="row">
                        <div class="col-xs-7 col-sm-9">
                        </div>
                        <div class="col-xs-5 col-sm-3 text-right">
                            <a class="btn green"
                               href="?modulo=principal/estabelecimento/lote/formulario&acao=C<?= setParam(array('id' => $p['id'])) ?>">
                                <i class="fa fa-plus-circle"></i>
                                Incluir lote
                            </a>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="pesquisar"/>
            </form>
        </div>
    </div>

    <?php
    $acao = <<<'ACAO'
                <php>
                    global $p;
                    $param = setParam(['lote_id' => '{lote_id}', 'id' => ('{estabelecimento_id}' ? '{estabelecimento_id}' : $p["id"])], false);
                    if('{esdid}' == Lote::WF_ESTADO_CORRECAO) :
                        $html = '<div style="min-width: 85px;">
                                    <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/lote/formulario&acao=C&p=' . $param . '" data-toggle="tooltip" title="Editar">
                                        <i class="icon-pencil"></i>
                                    </a>
                                    
                                    <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao(\'' . $param . '\', \'{numero}\');" data-toggle="tooltip"  title="Excluir">
                                        <i class="icon-trash"></i>
                                    </a>
                                </div>';
                    else :
                        $cnpj = '{cnpj}';
                        $acaoGerar = '<a class="btn btn-circle btn-icon-only btn-default purple" href="?modulo=principal/estabelecimento/lote/lista&acao=C&p=' . $param . '&act=gerarTiss" data-toggle="tooltip" title="Gerar TISS">
                                        <i class="fa fa-paste"></i>
                                    </a>'; 
                                    
                        if(empty($cnpj)) :
                            $acaoGerar = '<a class="btn btn-circle btn-icon-only btn-default gray" href="javascript:;" onclick="exibirAviso(\'Não é possível gerar o TISS pois o convênio selecionado não possui CNPJ.\')" data-toggle="tooltip" title="Gerar TISS">
                                            <i class="fa fa-paste"></i>
                                        </a>'; 
                        endif;
                                                
                        $html = '<div style="min-width: 85px;">
                                    <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/lote/formulario&acao=C&p=' . $param . '" data-toggle="tooltip" title="Visualizar">
                                        <i class="icon-magnifier"></i>
                                    </a>
                                    ' . $acaoGerar . '
                                </div>';
                    endif;
                    
                    return $html;
                </php>
ACAO;

    $arConfig = array();
    $arConfig[0]['type'] = Listam::TYPESTRING;
    $rs = $mLote->getList($where, true, $_REQUEST['ordem']);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho(array(
        'Número',
        'Descrição',
        'Convênio/Plano',
        'Tipo',
        'Valor',
        'Situação',
        'Ação'
    ));
    $mLista->setCorpo($rs, $arConfig);
    $mLista->setAcao($acao);
    $mLista->show();
    ?>
</div>

<form method="post" class="form-excluir">
    <input type="hidden" name="act" value="excluir"/>
    <input type="hidden" name="p"/>
</form>

<div class="modal fade in" id="modal-excluir" tabindex="-1" role="modal-excluir" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Excluir registro</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja excluir o lote Nº <strong id="nome"></strong>?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="excluirRegistro();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">Não</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<script type="text/javascript">
    $(function () {
    });

    function exibirModalExclusao(p, nome) {
        let modal = $('#modal-excluir');

        $('form.form-excluir [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.modal('show');
    }

    function excluirRegistro() {
        $('form.form-excluir').submit();
    }
</script>

