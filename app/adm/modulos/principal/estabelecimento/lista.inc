<?php
$mEstabelecimento = new Estabelecimento();
$arWhere = array();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'excluir':
            $arParams = getParam();
            UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($arParams['id']);

            $mEstabelecimento->carregarPorId($arParams['id']);
            $qtdAgendas = $mEstabelecimento->getQtdAgendaByEstabelecimento($arParams['id']);

            if ($qtdAgendas == 0) {
                $mEstabelecimento->excluirEstabelecimento($arParams['id']);
                $mEstabelecimento->commit();
                $db->sucessom($_REQUEST['modulo'], '', 'Estabelecimento exclu�do com sucesso');
            } else {
                $db->insucessom($_REQUEST['modulo'], '', 'O estabelecimento informado possui agenda e n�o pode ser exclu�do.');
            }
            break;
        case 'pesquisa_simples':
            $pesquisaAvancada = false;

            if (!empty($_REQUEST['pesquisa_simples'])) {
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
                $arWhere[] = "(
                    (pj.razao_social ILIKE '%{$pesquisa_simples}%') OR
                    (pj.nome_fantasia ILIKE '%{$pesquisa_simples}%') OR
                    (pj.cnpj ILIKE '%{$pesquisa_simples}%') OR
                    (tes.descricao ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
        case 'pesquisa_avancada':
            $pesquisaAvancada = true;

            if (!empty($_REQUEST['razao_social'])) {
                $razao_social = trim(addslashes($_REQUEST['razao_social']));
                $arWhere['razao_social'] = "pj.razao_social ILIKE '%{$razao_social}%' ";
            }

            if (!empty($_REQUEST['nome_fantasia'])) {
                $nome_fantasia = trim(addslashes($_REQUEST['nome_fantasia']));
                $arWhere['nome_fantasia'] = "pj.nome_fantasia ILIKE '%{$nome_fantasia}%' ";
            }

            if (!empty($_REQUEST['possuiagenda'])) {
                $sinal = $_REQUEST['possuiagenda'] == 'S' ? '>' : '=';
                $possuiagenda = $_REQUEST['possuiagenda'];
                $arWhere['possuiagenda'] = "((  SELECT 
                                                    COUNT(epa.id) 
                                                FROM 
                                                    estabelecimento.tb_estabelecimento_profissional_agenda epa
                                                INNER JOIN
                                                    estabelecimento.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE AND eep.estabelecimento_id = est.id
                                                WHERE
                                                    epa.excluido IS FALSE) {$sinal} 0)";
            }

            if (!empty($_REQUEST['cnpj'])) {
                $cnpj = trim(addslashes($_REQUEST['cnpj']));
                $arWhere['cnpj'] = "pj.cnpj ILIKE '%{$cnpj}%' ";
            }

            if (!empty($_REQUEST['tipo_estabelecimento_id'])) {
                $tipo_estabelecimento_id = (int) $_REQUEST['tipo_estabelecimento_id'];
                $arWhere['tipo_estabelecimento_id'] = "est.tipo_estabelecimento_id = " . $tipo_estabelecimento_id;
            }
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
?>
<style type="text/css">
    .campos-pesquisa {
        border-bottom: 1px solid #eef1f5;
        padding: 10px 0px 10px 0px;
        margin-bottom: 10px;
    }
</style>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-briefcase"></i>
            <span class="caption-subject bold uppercase">
                Lista de clientes
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="pesquisa-form" method="POST">
                <div class="form-body">
                    <div class="col-md-12">
                        <div class="form-group">
                            <div class="input-group">
                                <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por raz�o social, CNPJ, nome fantasia ou tipo'); ?>
                                <span class="input-group-btn">
                                    <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                        <i class="fa fa-search"></i>
                                    </button>
                                </span>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-actions">
                    <div class="col-md-12 text-right">
                        <?php if (possuiAcesso('adm.php?modulo=principal/estabelecimento/formulario&acao=C')) : ?>
                            <a class="btn green" href="?modulo=principal/estabelecimento/formulario&acao=C">
                                <i class="fa fa-plus-circle"></i>
                                Cadastrar novo cliente
                            </a>
                        <?php endif; ?>
                    </div>
                </div>
        </div>
        <input type="hidden" name="act" value="<?= ($pesquisaAvancada ? 'pesquisa_avancada' : 'pesquisa_simples') ?>" />
        </form>
    </div>
    <div class="row">
        <div class="col-md-12">
            <?php

            $acaoEdicao = <<<ACAO
                                    <php>
                                        if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/formulario&acao=C')) {
                                            return '<a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/formulario&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Editar estabelecimento">
                                            <i class="bi bi-gear"></i>
                                                    </a>';
                                        }
                                    </php>
                            ACAO;

            $acaoExclusao = <<<ACAO
                                            <php>
                                                if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/formulario&acao=C')) {
                                                    return '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao(\'' . setParam(array('id' => '{id}'), false) . '\', \'{nome_fantasia}\');" data-toggle="tooltip"  title="Excluir estabelecimento">
                                                <i class="bi bi-x-square"></i>
                                            </a>';
                                                }
                                            </php>
                                ACAO;

            $acao = <<<ACAO
                                    {$acaoEdicao}
                                    {$acaoExclusao}
                        ACAO;

            $arConfig = array();
            $arConfig[0]['html'] = <<<HTML
        <php>
            if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/formulario&acao=C')) {
                return '<a href="?modulo=principal/editaisclientes/inicio&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Abrir Painel">
                            {empresa}
                        </a>';
            }elseif(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/agenda&acao=C')){
                return '<a href="?modulo=principal/estabelecimento/agenda&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Agenda">
                            {empresa}
                        </a>';
            }else{
                return '{empresa}';
            }
        </php>

HTML;

            $arConfig[2]['style'] = "text-align: center;";
            $arConfig[2]['style'] = "width: 10%;";
            $mEstabelecimento = new Estabelecimento();
            $rs = $mEstabelecimento->getList($arWhere);

            $mLista = new Listam(array("style" => "width: 100%;"));
            $mLista->setCabecalho(array(
                'Empresa',
                'CNPJ',
                'A��o'
            ));
            $mLista->setCorpo($rs, $arConfig);
            $mLista->setAcao($acao);
            $mLista->show();
            ?>
        </div>
    </div>
</div>
</div>
<form method="post" class="form-excluir">
    <input type="hidden" name="act" value="excluir" />
    <input type="hidden" name="p" />
</form>

<div class="modal fade in" id="modal-excluir" tabindex="-1" role="modal-excluir" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Excluir estabelecimento</h4>
            </div>
            <div class="modal-body">
                Voc� tem certeza que deseja excluir o estabelecimento <strong id="nome"></strong>?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="excluirRegistro();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">N�o</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<script type="text/javascript">
    $(function() {
        $('.btn-limpar').click(function() {
            window.location = '?modulo=principal/estabelecimento/lista&acao=C';
        });
    });

    function exibirModalExclusao(p, nome) {
        var modal = $('#modal-excluir');

        $('form.form-excluir [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.modal('show');
    }

    function excluirRegistro() {
        $('form.form-excluir').submit();
    }

    function migrarEstabelecimentos(origem, destino, desabilita) {
        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType: 'json',
            data: {
                act: 'migrar_informacoes_estabelecimentos',
                origem: origem,
                destino: destino,
                desabilita: desabilita,
            },
            success: function(retorno) {
                console.log(retorno);

                exibirSucesso(retorno.msg ? retorno.msg : 'Migra��o realizada com sucesso!');
            },
            error: function(result) {
                App.unblockUI('.form-body');
                exibirAlert('N�o foi poss�vel realizar a migra��o');
            }
        });
    }
</script>