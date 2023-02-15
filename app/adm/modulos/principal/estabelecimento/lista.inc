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
                $db->sucessom($_REQUEST['modulo'], '', 'Estabelecimento excluído com sucesso');
            } else {
                $db->insucessom($_REQUEST['modulo'], '', 'O estabelecimento informado possui agenda e não pode ser excluído.');
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
                                                    estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                                                INNER JOIN
                                                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.id = epa.estabelecimento_especialidade_profissional_id AND eep.excluido IS FALSE AND eep.estabelecimento_id = est.id
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
<br />
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
                    <div class="pesquisa-simples <?= ($pesquisaAvancada ? 'hidden' : '') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por razão social, CNPJ, nome fantasia ou tipo'); ?>
                                        <span class="input-group-btn">
                                            <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                                <i class="fa fa-search"></i>
                                            </button>
                                        </span>
                                        <span class="input-group-btn" data-toggle="tooltip" title="Pesquisa avançada">
                                            <button class="btn green-jungle btn-pesquisa-avancada" type="button">
                                                <i class="fa fa-search-plus"></i>
                                            </button>
                                        </span>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>


                    <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Razão social</label>
                                    <?= campo_textom('razao_social', 'N', 'S', '', 190, 190, '', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Nome fantasia</label>
                                    <?= campo_textom('nome_fantasia', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>CNPJ</label>
                                    <?= campo_textom('cnpj', 'N', 'S', '', 19, 19, '99.999.999/9999-99', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Tipo</label>
                                    <?php
                                    $mTipoEstabelecimento = new TipoEstabelecimento();
                                    $tiposEstabelecimento = $mTipoEstabelecimento->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                                    $db->monta_combom("tipo_estabelecimento_id", $tiposEstabelecimento, 'S', 'Selecione...', '', '', '', '', 'N');
                                    ?>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Possui agenda</label>
                                    <div class="form-select-content">
                                        <?php
                                        $db->monta_radiom('possuiagenda', array(
                                            array('codigo' => 'S', 'descricao' => 'Sim'),
                                            array('codigo' => 'N', 'descricao' => 'Não'),
                                            array('codigo' => '', 'descricao' => 'Todos'),
                                        ), 'S', 'N')
                                        ?>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-actions">
                    <div class="row">
                        <div class="col-md-6">
                            <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                                <button type="submit" class="btn blue">
                                    <i class="fa fa-search"></i> Pesquisar
                                </button>
                                <button type="button" class="btn green-jungle btn-pesquisa-simples">
                                    <i class="fa fa-search-minus"></i> Pesquisa simples
                                </button>
                            </div>
                        </div>
                        <div class="col-md-6 text-right">
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

    </div>

    <?php
   
    $acaoMonitoramento = <<<ACAO
    <php>
        if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/editaisclientes/monitoramento&acao=C')) {
            return '<a class="btn btn-circle btn-icon-only btn-default yellow" href="?modulo=principal/editaisclientes/monitoramento&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Abrir Monitoramento do Cliente">
                        <i class="icon-eye"></i>
                    </a>';
        }
    </php>
ACAO;

$acaoPainelCDCEmpresa = <<<ACAO
<php>
    if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/cdcclientes/inicio&acao=C')) {
        return '<a class="btn btn-circle btn-icon-only btn-default red" href="?modulo=principal/cdcclientes/inicio&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Abrir Painel de Cotações do Cliente">
                    <i class="icon-arrow-up"></i>
                </a>';
    }
</php>
ACAO;


    $acaoPainelEmpresa = <<<ACAO
    <php>
        if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/editaisclientes/inicio&acao=C')) {
            return '<a class="btn btn-circle btn-icon-only btn-default purple" href="?modulo=principal/editaisclientes/inicio&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Abrir Painel do Cliente">
                        <i class="icon-arrow-right"></i>
                    </a>';
        }
    </php>
ACAO;

    $acaoEdicao = <<<ACAO
        <php>
            if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/formulario&acao=C')) {
                return '<a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/formulario&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Editar estabelecimento">
                <i class="fa fa-edit"></i>
                        </a>';
            }
        </php>
ACAO;

    $acaoFinanceiro = <<<ACAO
    <php>
            if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/financeiro/extrato&acao=C')) {
                return '<a class="btn btn-circle btn-icon-only btn-default red" href="?modulo=principal/estabelecimento/agenda&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Agenda do estabelecimento">
                <i class="icon-calendar"></i>
                        </a>';
            }
        </php>
ACAO;

    $acaoExclusao = <<<ACAO
            <php>
                if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/formulario&acao=C')) {
                    return '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao(\'' . setParam(array('id' => '{id}'), false) . '\', \'{nome_fantasia}\');" data-toggle="tooltip"  title="Excluir estabelecimento">
                                <i class="icon-trash"></i>
                            </a>';
                }
            </php>
ACAO;

    $acao = <<<ACAO
            <div style="min-width: 200px;">
            {$acaoPainelEmpresa} 
            {$acaoPainelCDCEmpresa}
            {$acaoMonitoramento}
            {$acaoFinanceiro}
            {$acaoEdicao}
            {$acaoExclusao}
            </div>
ACAO;

    $arConfig = array();
    $arConfig[0]['html'] = <<<HTML
        <php>
            if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/formulario&acao=C')) {
                return '<a href="?modulo=principal/editaisclientes/inicio&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Abrir Painel">
                            {nome_fantasia}
                        </a>';
            }elseif(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade('{id}', false, 'adm.php?modulo=principal/estabelecimento/agenda&acao=C')){
                return '<a href="?modulo=principal/estabelecimento/agenda&acao=C&p=' . setParam(array('id' => '{id}'), false) . '" data-toggle="tooltip" title="Agenda">
                            {nome_fantasia}
                        </a>';
            }else{
                return '{nome_fantasia}';
            }
        </php>

HTML;

    $arConfig[2]['style'] = "text-align: center;";
    $arConfig[2]['style'] = "width: 10%;";

    $rs = $mEstabelecimento->getList($arWhere);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho(array(
        'Razão social',
        'Nome fantasia',
        'CNPJ',
        'Ação'
    ));
    $mLista->setCorpo($rs, $arConfig);
    $mLista->setAcao($acao);
    $mLista->show();
    ?>
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
                Você tem certeza que deseja excluir o estabelecimento <strong id="nome"></strong>?
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

                exibirSucesso(retorno.msg ? retorno.msg : 'Migração realizada com sucesso!');
            },
            error: function(result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a migração');
            }
        });
    }
</script>