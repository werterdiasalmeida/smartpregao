<?php
$p = getParam();

if (!isset($p['id'])) {
    addMsgInfo('Por favor, selecione um estabelecimento.');
    header('Location: ?modulo=principal/estabelecimento/lista&acao=C');
    die;
}

UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);
$mEstabelecimentoEspecialidadeProfissional = new EstabelecimentoEspecialidadeProfissional();
$mUsuario = new Usuario();
$arWhere = array(
    "est.id = {$p['id']}"
);

if ($_REQUEST['act']) {
    /*if(!possuiPerfil(array(ADM_PERFIL_GERAL, ADM_PERFIL_LOCAL))) {
        $db->insucessom($_REQUEST['modulo'], '', 'Acesso negado.');
    }*/

    switch ($_REQUEST['act']) {
        case 'alterar' :
            include_once 'corpoClinico/alterarCorpoClinico.inc';
            die;
            break;
        case 'abrir-agenda' :
            include_once 'corpoClinico/abrirAgendaCorpoClinico.inc';
            die;
            break;
        case 'previsualizar-agenda' :
            include_once 'corpoClinico/preVisualizarAgendaCorpoClinico.inc';
            die;
            break;
        case 'lembretes-tarefas' :
            include_once 'corpoClinico/lembretesTarefas.inc';
            die;
            break;
        case 'adicionar-lembrete-tarefa' :
            $mLembreteTarefa = new LembreteTarefa();
            $params = array_merge($_POST, getParam());
            $params = utf8ArrayDecode($params);
            $params['estabelecimento_id'] = $params['id'];
            $params['descricao'] = $params['lembrete_tarefa'];
            unset($params['id']);

            $mLembreteTarefa->salvarLembreteTarefa($params);
            $mLembreteTarefa->commit();

            $db->sucessomAjax();
            die;
            break;
        case 'listar-lembretes-tarefas':
            include_once 'corpoClinico/lembretesTarefasLista.inc';
            die;
            break;
        case 'mensagens-lembrete-tarefa':
            include_once 'corpoClinico/mensagensLembreteTarefa.inc';
            die;
            break;
        case 'salvar-agenda' :
            $mAgenda = new EstabelecimentoProfissionalAgenda();
            $eventos = str_replace('\"', '"', $_REQUEST['eventos']);
            $mAgenda->salvarEventosAgenda(utf8ArrayDecode(json_decode($eventos, true)));
            $mAgenda->commit();

            $db->sucessomAjax();
            die;
        case 'pesquisarProfissionais' :
            include_once 'corpoClinico/listaProfissionais.inc';
            die;
            break;
        case 'pesquisarEspecialidades' :
            include_once 'corpoClinico/listaEspecialidades.inc';
            die;
            break;
        case 'salvarEspecialidades' :
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            $mEstabelecimentoEspecialidadeProfissional->salvarEspecialidades($params);
            $mEstabelecimentoEspecialidadeProfissional->commit();

            $db->sucessomAjax();
            break;
        case 'excluir' :
            $mEstabelecimentoEspecialidadeProfissional->excluirPorProfissionalEstabelecimento($p['profissional_id'], $p['id']);
            $mEstabelecimentoEspecialidadeProfissional->commit();

            $db->sucessom($_REQUEST['modulo'], setParam(array(
                'id' => $p['id']
            )));
            break;
        case 'ativar' :
            $mEstabelecimentoEspecialidadeProfissional->ativarPorProfissionalEstabelecimento($p['profissional_id'], $p['id']);
            $mEstabelecimentoEspecialidadeProfissional->commit();

            $db->sucessom($_REQUEST['modulo'], setParam(array(
                'id' => $p['id']
            )));
            break;
        case 'listar' :
            $_REQUEST['pesquisa_simples'] = utf8_decode($_REQUEST['pesquisa_simples']);
            $_REQUEST['nome_completo'] = utf8_decode($_REQUEST['nome_completo']);
            // sem break de propósito apenas para decodificar as strings apenas no case 'listar'
        case 'pesquisa_simples' :
        case 'pesquisa_avancada' :

            if($_REQUEST['act'] === 'pesquisa_simples'
            || $_REQUEST['act_pesquisa'] === 'pesquisa_simples'){
                $pesquisaAvancada = false;

                if (!empty($_REQUEST['pesquisa_simples'])) {
                    $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));
                    $arWhere[] = "(
                        (pes.nome_completo ILIKE '%{$pesquisa_simples}%') OR
                        (tp.descricao ILIKE '%{$pesquisa_simples}%') OR
                        ((p.registro_profissional || '/' || p.uf_registro) ILIKE '%{$pesquisa_simples}%')
                    )";
                }
            }

            if($_REQUEST['act'] === 'pesquisa_avancada'
            || $_REQUEST['act_pesquisa'] === 'pesquisa_avancada') {
                $pesquisaAvancada = true;

                if (!empty($_REQUEST['nome_completo'])) {
                    $nome_completo = trim(addslashes($_REQUEST['nome_completo']));
                    $arWhere[] = "pes.nome_completo ILIKE '%{$nome_completo}%' ";
                }

                if (!empty($_REQUEST['tipo_profissional_id'])) {
                    $tipo_profissional_id = $_REQUEST['tipo_profissional_id'];
                    $arWhere[] = "tp.id = {$tipo_profissional_id}";
                }

                if (!empty($_REQUEST['registro'])) {
                    $registro = trim(addslashes($_REQUEST['registro']));
                    $arWhere[] = "((p.registro_profissional || '/' || p.uf_registro) ILIKE '%{$registro}%')";
                }
            }

            if($_REQUEST['act'] === 'listar'){
                lista();
                die;
            }
            break;
        case 'redefenir-senha' :
            $mUsuario->redefinirSenha($p);
            $db->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])), 'A senha do usuário foi redefinida com sucesso.');
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true);

include APPRAIZ . "includes/controleAcessoSituacaoContrato.inc";
?>

<style type="text/css">
    .mt-comments .mt-comment .mt-comment-body .mt-comment-details .mt-comment-actions {
        display: inline-block;
    }
</style>
<!--<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.12.0.min.js"></script>-->

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-users font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
            Informações do corpo clínico
        </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="fila-form" method="POST">
                <div class="form-body">

                    <div class="pesquisa-simples <?= ($pesquisaAvancada ? 'hidden' : '') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise pelo nome, registro ou tipo do profissional'); ?>
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
                            <div class="col-md-5">
                                <div class="form-group">
                                    <label>Nome</label>
                                    <?= campo_textom('nome_completo', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-3">
                                <div class="form-group">
                                    <label>Registro</label>
                                    <?= campo_textom('registro', 'N', 'S', '', 19, 19, '', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-4">
                                <div class="form-group">
                                    <label>Tipo</label>
                                    <?php
                                    $mTipo = new TipoProfissional();
                                    $tipos = $mTipo->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                                    $db->monta_combom("tipo_profissional_id", $tipos, 'S', 'Selecione...', '', '', '', '', 'N');
                                    ?>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div class="form-actions">
                        <div class="row">
                            <div class="col-xs-7 col-sm-5">
                                <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                                    <button type="submit" class="btn blue">
                                        <i class="fa fa-search"></i> Pesquisar
                                    </button>
                                    <button type="button"
                                            class="btn green-jungle btn-pesquisa-simples">
                                        <i class="fa fa-search-minus"></i> Pesquisa simples
                                    </button>
                                </div>
                            </div>
                            <div class="col-xs-5 col-sm-7 text-right">
                                <?php if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id'], false, 'adm.php?modulo=principal/estabelecimento/corpoClinico/formularioProfissional&acao=C')) : ?>
                                    <a class="btn green" href="?modulo=principal/estabelecimento/corpoClinico/formularioProfissional&acao=C<?= setParam(array('id' => $p['id'])) ?>">
                                        <i class="fa fa-plus-circle"></i>
                                        Incluir novo profissional
                                    </a>
                                <?php endif; ?>

                                <?php if(UsuarioResponsabilidadeEstabelecimento::isPossuiPerfilEstabelecimento($p['id'], array(ADM_PERFIL_GERAL, ADM_PERFIL_LOCAL))) : ?>
                                    <a class="btn green btn-selecionar-especialidades">
                                        <i class="fa fa-plus-circle"></i>
                                        Selecionar especialidades
                                    </a>
                                <?php endif; ?>
                            </div>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act"
                       value="<?= ($pesquisaAvancada ? 'pesquisa_avancada' : 'pesquisa_simples') ?>"/>
            </form>
        </div>
        <br/>

        <div id="lista">
            <?php lista(); ?>
        </div>
        <div class="form-actions">
            <button type="button" class="btn btn-voltar">
                <i class="fa fa-arrow-left"></i> Voltar
            </button>
        </div>
    </div>
</div>

<form method="post" class="form-excluir">
    <input type="hidden" name="act" value="excluir"/>
    <input type="hidden" name="p"/>
</form>

<form method="post" class="form-ativar">
    <input type="hidden" name="act" value="ativar"/>
    <input type="hidden" name="p"/>
</form>

<div class="modal fade in" id="modal-corpo-clinico" tabindex="-1" role="modal-corpo-clinico" aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 90%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Incluir corpo clínico</h4>
            </div>
            <div class="modal-body">
                <div class="portlet light bordered">
                    <div class="portlet-title">
                        <div class="caption font-blue-sharp">
                            <i class="icon-user-following font-blue-sharp"></i>
                            <span class="caption-subject bold uppercase">
                        Pesquisar profissional
                    </span>
                        </div>
                    </div>
                    <div class="portlet-body form">
                        <form role="form" name="formulario" class="corpo-clinico-form" method="POST"
                              enctype="multipart/form-data">
                            <div class="form-body">
                                <div class="row pesquisa-simples">
                                    <div class="col-md-12">
                                        <div class="form-group">
                                            <?= campo_textom('pesquisa', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise pelo nome, registro ou tipo do profissional'); ?>
                                        </div>
                                    </div>
                                    <div class="col-md-12">
                                        <div class="form-group text-center">
                                            <button type="submit" class="btn blue">
                                                <i class="fa fa-search"></i> Pesquisar
                                            </button>
                                            <button type="button" class="btn green-jungle btn-pesquisa-avancada">
                                                <i class="fa fa-search-plus"></i> Pesquisa avançada
                                            </button>
                                        </div>
                                    </div>
                                </div>
                                <div class="row pesquisa-avancada hidden">
                                    <div class="col-md-6">
                                        <div class="form-group">
                                            <label>Nome</label>
                                            <?= campo_textom('nome', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                    <div class="col-md-2">
                                        <div class="form-group">
                                            <label>Registro</label>
                                            <?= campo_textom('registro', 'N', 'S', '', 19, 19, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                    <div class="col-md-4">
                                        <div class="form-group">
                                            <label>Tipo</label>
                                            <?php
                                            $mTipo = new TipoProfissional();
                                            $tipos = $mTipo->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                                            $db->monta_combom("tipo_profissional_id", $tipos, 'S', 'Selecione...', '', '', '', '', 'N');
                                            ?>
                                        </div>
                                    </div>
                                    <div class="col-md-12">
                                        <div class="form-group text-center">
                                            <button type="submit" class="btn blue">
                                                <i class="fa fa-search"></i> Pesquisar
                                            </button>
                                            <button type="button" class="btn green-jungle btn-pesquisa-simples">
                                                <i class="fa fa-search-minus"></i> Pesquisa simples
                                            </button>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </form>
                    </div>
                </div>

                <div class="row">
                    <div class="col-md-6">
                        <div class="portlet light bordered" style="height: 365px;">
                            <div class="portlet-title">
                                <div class="caption font-blue-sharp">
                                    <i class="icon-users font-blue-sharp"></i>
                                    <span class="caption-subject bold uppercase">
                                Selecione um profissional
                            </span>
                                </div>
                            </div>
                            <div class="portlet-body form">
                                <div id="profissionais">
                                    <h4>Faça uma pesquisa para encontrar os profissionais.</h4>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div class="col-md-6">
                        <div class="portlet light bordered" style="height: 365px;">
                            <div class="portlet-title">
                                <div class="caption font-blue-sharp">
                                    <i class="fa fa-user-md font-blue-sharp"></i>
                                    <span class="caption-subject bold uppercase">
                                Selecione as especialidades
                            </span>
                                </div>
                            </div>
                            <div class="portlet-body form">
                                <div id="especialidades">
                                    <h4>Para selecionar uma especialidade você deve primeiro escolher um
                                        profissional.</h4>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn blue hidden btn-incluir">
                    <i class="fa fa-plus"></i>
                    Incluir
                </button>
                <button type="button" class="btn" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-alterar-corpo-clinico" tabindex="-1" role="modal-alterar-corpo-clinico"
     aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 90%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Alterar corpo clínico</h4>
            </div>
            <div class="modal-body">
                <div id="alterar-corpo-clinico" style="min-height: 100px;">

                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn blue hidden btn-incluir">
                    <i class="fa fa-plus"></i>
                    Incluir
                </button>
                <button type="button" class="btn" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-abrir-agenda" tabindex="-1" role="modal-abrir-agenda"
     aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 95%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Abrir agenda</h4>
            </div>

            <div class="abrir-agenda-form">
                <div class="modal-body">
                    <div id="abrir-agenda" style="min-height: 100px;">

                    </div>
                </div>
                <div class="modal-footer">
                    <div class="row">
                        <div class="col-sm-4 text-left">
                            <button type="button" class="btn green btn-voltar hidden">
                                <i class="fa fa-arrow-left"></i>
                                Voltar
                            </button>
                        </div>
                        <div class="col-sm-8 text-right">
                            <button type="button" class="btn blue hidden btn-salvar"></button>
                            <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-lembretes-tarefas" tabindex="-1" role="modal-lembretes-tarefas"
     aria-hidden="true">
    <div class="modal-dialog modal-lg" style="width: 95%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Lembretes e tarefas</h4>
            </div>

            <div class="lembretes-tarefas-form">
                <div class="modal-body">
                    <form id="form-lembretes-tarefas" method="post">
                        <div id="lembretes-tarefas" style="min-height: 100px;">

                        </div>
                    </form>
                </div>
                <div class="modal-footer">
                    <div class="row">
                        <div class="col-sm-4 text-left">
                            <button type="button" class="btn green btn-voltar hidden">
                                <i class="fa fa-arrow-left"></i>
                                Voltar
                            </button>
                        </div>
                        <div class="col-sm-8 text-right">
                            <button type="button" class="btn blue hidden btn-salvar"></button>
                            <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<div class="modal fade in" id="modal-excluir" tabindex="-1" role="modal-excluir" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Desligar corpo clínico</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja desligar o profissional <strong id="nome"></strong> do tipo <strong
                        id="tipo"></strong> do corpo clínico?
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

<div class="modal fade in" id="modal-ativar" tabindex="-1" role="modal-ativar" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Ativar corpo clínico</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja ativar o profissional <strong id="nome"></strong> do tipo <strong
                        id="tipo"></strong> no corpo clínico?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn green-turquoise" onclick="ativarRegistro();">Sim</button>
                <button type="button" class="btn red" data-dismiss="modal">Não</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<form method="post" class="form-redefenir-senha">
    <input type="hidden" name="act" value="redefenir-senha" />
    <input type="hidden" name="p" />
</form>

<div class="modal fade in" id="modal-redefenir-senha" tabindex="-1" role="modal-redefenir-senha" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Redefinir Senha</h4>
            </div>
            <div class="modal-body">
                A Senha do(a) usuário(a) <b class="nome-usuario"></b> será alterada para <b><?= Usuario::SENHA_PADRAO ?></b>.<br>
                Deseja continuar?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="redefenirSenha();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">Não</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<link href="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.css" rel="stylesheet" type="text/css"/>
<script src="/includes/metronic/global/plugins/fullcalendar/fullcalendar.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/fullcalendar/lang/pt-br.js" type="text/javascript"></script>

<script type="text/javascript">
    var defaultEspecialidades = $('#especialidades').html();
    var defaultProfissionais = $('#profissionais').html();

    $(function () {
        $(".corpo-clinico-form").validateForm({
            submitHandler: function (e) {
                var form = $(".corpo-clinico-form");
                $('#especialidades').html(defaultEspecialidades);
                $(".btn-incluir").addClass('hidden');

                App.blockUI({
                    message: 'Carregando...',
                    target: '#profissionais'
                });

                $('#profissionais').load(window.location.href, {
                    act: 'pesquisarProfissionais',
                    pesquisa: form.find('[name=pesquisa]:visible').val(),
                    nome: form.find('[name=nome]:visible').val(),
                    registro: form.find('[name=registro]:visible').val(),
                    tipo_profissional_id: form.find('[name=tipo_profissional_id]:visible').val(),
                }, function () {
                    App.unblockUI('#profissionais');
                });
            }
        });

        $('#modal-corpo-clinico, #modal-alterar-corpo-clinico').on('hidden.bs.modal', function () {
            limparForm();
            $('#alterar-corpo-clinico').html('');
        });

        $('.btn-voltar').click(function () {
            window.history.back(-1);
        });

        $('.btn-limpar').click(function () {
            window.location = window.location.href;
        });

        $('.btn-selecionar-especialidades').click(function(){
            $('#modal-corpo-clinico').modal({
                backdrop: 'static',
            }).modal('show');
        });
    });

    function exibirModalExclusao(p, nome, tipo) {
        var modal = $('#modal-excluir');

        $('form.form-excluir [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.find('strong#tipo').html(tipo);
        modal.modal('show');
    }

    function exibirModalAtivacao(p, nome, tipo) {
        var modal = $('#modal-ativar');

        $('form.form-ativar [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.find('strong#tipo').html(tipo);
        modal.modal('show');
    }

    function excluirRegistro() {
        $('form.form-excluir').submit();
    }

    function ativarRegistro() {
        $('form.form-ativar').submit();
    }

    function limparForm() {
        var container = $('#modal-corpo-clinico');

        container.find('button.btn-pesquisa-simples').trigger('click');

        container.find('[name=tipo_profissional_id]').selectpicker('refresh');
        container.find('#profissionais').html(defaultProfissionais);
        container.find('#especialidades').html(defaultEspecialidades);
        container.find(".btn-incluir").addClass('hidden');
        container.find('form.corpo-clinico-form')[0].reset();
    }

    function listar() {
        App.blockUI({
            message: 'Carregando...',
            target: '#lista'
        });

        $('#lista').load(window.location.href, {
            act: 'listar',
            act_pesquisa: $('[name=act]').val(),
            pesquisa_simples : $('[name=pesquisa_simples]').val(),
            nome_completo : $('[name=nome_completo]').val(),
            registro : $('[name=registro]').val(),
            tipo_profissional_id : $('[name=tipo_profissional_id]').val(),
        }, function () {
            App.unblockUI('#lista');
        });
    }

    function alterar(p) {
        App.blockUI({
            message: 'Carregando...',
            target: '#alterar-corpo-clinico'
        });

        $('#alterar-corpo-clinico').load(window.location.href, {
            p: p,
            act: 'alterar'
        }, function () {
            App.unblockUI('#alterar-corpo-clinico');
        });

        $('#modal-alterar-corpo-clinico').modal({
            backdrop: 'static',
        }).modal('show');
    }

    function exibirModalConfigurarAgenda(p) {
        App.blockUI({
            message: 'Carregando...',
            target: '#abrir-agenda'
        });

        $('#abrir-agenda').load(window.location.href, {
            p: p,
            act: 'abrir-agenda'
        }, function () {
            App.unblockUI('#abrir-agenda');
        });

        $('#modal-abrir-agenda').modal({
            backdrop: 'static',
        }).modal('show');
    }

    function exibirModalLembretesTarefas(p) {
        App.blockUI({
            message: 'Carregando...',
            target: '#lembretes-tarefas'
        });

        $('#lembretes-tarefas').load(window.location.href, {
            p: p,
            act: 'lembretes-tarefas'
        }, function () {
            App.unblockUI('#lembretes-tarefas');
        });

        $('#modal-lembretes-tarefas').modal({
            backdrop: 'static',
        }).modal('show');
    }

    function exibirModalRedefinirSenha(p, nome){
        var modal = $('#modal-redefenir-senha');

        $('form.form-redefenir-senha [name=p]').val(p);
        modal.find('.nome-usuario').html(nome);
        modal.modal('show');
    }

    function redefenirSenha(){
        $('form.form-redefenir-senha').submit();
    }
</script>
<?php
function lista()
{
    global $mEstabelecimentoEspecialidadeProfissional, $mUsuario, $arWhere, $p;

    $mContato = new ContatoPessoaFisica();
    $rs = $mEstabelecimentoEspecialidadeProfissional->getList($arWhere, false, null, true);
    if (count($rs) > 0) :
        foreach ($rs as $profissional) :
            $montarAcaoP = setParam(array(
                'id' => $profissional['estabelecimento_id'],
                'profissional_id' => $profissional['profissional_id'],
                'pessoa_id' => $profissional['pessoa_id']
            ), false);

            $usuarioExistente = $mUsuario->isCpfCadastrado($profissional['cpf']);
            $pPessoaCpf = '';

            if($usuarioExistente) {
                $pPessoaCpf = setParam(array(
                    'id' => $profissional['estabelecimento_id'],
                    'usucpf' => $profissional['cpf']
                ), false);
            }

            $contato = array_filter($mContato->listarEspecificosPorPessoaFisica($profissional['pessoa_id']));
            unset($contato['contato_email']);
            $profissionalExcluido = $profissional['estabelecimento_especialidade_profissional_excluido'] === 't';
            ?>
            <div class="mt-comments">
                <div class="mt-comment">
                    <div class="mt-comment-img" style="text-align: center;">
                        <?php montaImgFoto($profissional['foto_arquivo_id'], $profissional['sexo'], '55px', '55px', '100', '100', 'bg-white') ?>
                    </div>
                    <div class="mt-comment-body" style="min-height: 100px;">
                        <div class="mt-comment-info">
                            <span class="mt-comment-author">
                                <?= $profissional['nome_completo'] ?>
                            </span>
                        </div>
                        <div class="mt-comment-text">
                            <?= (strlen($profissional['curriculo']) > 500 ? substr($profissional['curriculo'], 0, 500) . '...' : $profissional['curriculo']) ?>
                        </div>
                        <div class="mt-comment-details">
                            <span class="mt-comment-status">
                                <?= $profissional['tipo_profissional'] ?>
                                -
                                <?= getLabels($profissional['especialidades']); ?>
                                <br />
                                <?php if($contato): ?>
                                    Tel.: <?= ($contato ? implode(' / ', $contato) : '-') ?>
                                <? endif; ?>
                            </span>
                            <ul class="mt-comment-actions">
                                <?php if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id'] , false, 'adm.php?modulo=principal/estabelecimento/corpoClinico/formularioProfissional&acao=C')) : ?>
                                    <li>
                                        <a class="btn btn-circle btn-icon-only btn-default <?= ($profissionalExcluido ? 'grey' : 'blue') ?>"
                                            <?= ($profissionalExcluido ? '' : 'href="?modulo=principal/estabelecimento/corpoClinico/formularioProfissional&acao=C&p=' . $montarAcaoP . '"') ?> title="Editar">
                                            <i class="icon-pencil" style="color: #FFF;"></i>
                                        </a>
                                    </li>
                                <?php endif; ?>
                                <?php if(UsuarioResponsabilidadeEstabelecimento::isPossuiPerfilEstabelecimento($p['id'], array(ADM_PERFIL_GERAL, ADM_PERFIL_LOCAL, ADM_PERFIL_RECEPCIONISTA))) : ?>
                                    <?php if(UsuarioResponsabilidadeEstabelecimento::isPossuiPerfilEstabelecimento($p['id'], array(ADM_PERFIL_GERAL, ADM_PERFIL_LOCAL))) : ?>
                                        <li>
                                            <a class="btn btn-circle btn-icon-only btn-default <?= ($profissionalExcluido ? 'grey' : 'green') ?>"
                                               <?= ($profissionalExcluido ? '' : 'href="javascript: alterar(\'' . $montarAcaoP . '\');"') ?> title="Selecionar especialidades">
                                                <i class="icon-users" style="color: #FFF;"></i>
                                            </a>
                                        </li>
                                    <?php endif; ?>
                                    <li>
                                        <a class="btn btn-circle btn-icon-only btn-default <?= ($profissionalExcluido ? 'grey' : 'green-jungle') ?>"
                                           <?= ($profissionalExcluido ? '' : 'href="javascript: exibirModalConfigurarAgenda(\'' . $montarAcaoP .'\');"') ?>
                                           title="Abrir agenda">
                                            <i class="fa fa-calendar-plus-o" style="color: #FFF;"></i>
                                        </a>
                                    </li>
                                    <li>
                                        <a class="btn btn-circle btn-icon-only btn-default <?= ($profissionalExcluido ? 'grey' : 'purple') ?>"
                                           <?= ($profissionalExcluido ? '' : 'href="?modulo=principal/estabelecimento/agenda&acao=C&p=' . $montarAcaoP . '"') ?>
                                           title="Agenda">
                                            <i class="icon-calendar" style="color: #FFF;"></i>
                                        </a>
                                    </li>
                                    <li>
                                        <a class="btn btn-circle btn-icon-only btn-default <?= ($profissionalExcluido ? 'grey' : 'yellow-casablanca') ?>"
                                           <?= ($profissionalExcluido ? '' : 'href="javascript: exibirModalLembretesTarefas(\'' . $montarAcaoP . '\');"') ?>
                                           title="Lembretes e Tarefas">
                                            <i class="fa fa-comments" style="color: #FFF;"></i>
                                        </a>
                                    </li>
                                    <?php if($usuarioExistente): ?>
                                        <li>
                                            <a class="btn btn-circle btn-icon-only btn-default <?= ($profissionalExcluido ? 'grey' : 'yellow-gold') ?>"
                                               <?= ($profissionalExcluido ? '' : 'href="javascript: exibirModalRedefinirSenha(\'' . $pPessoaCpf . '\', \'' . $profissional['nome_completo'] . '\');"') ?>
                                               title="Redefenir Senha">
                                                <i class="icon-key" style="color: #FFF;"></i>
                                            </a>
                                        </li>
                                    <?php endif; ?>
                                <?php endif; ?>
                                <li>
                                    <?php if(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id'] , false, 'adm.php?modulo=principal/estabelecimento/corpoClinico/formularioProfissional&acao=C') && $profissionalExcluido) : ?>
                                        <a class="btn btn-circle btn-icon-only btn-default green-turquoise"
                                           href="javascript: exibirModalAtivacao('<?= $montarAcaoP ?>', '<?= $profissional['nome_completo'] ?>', '<?= $profissional['tipo_profissional'] ?>');"
                                           title="Ativar profissional">
                                            <i class="icon-check" style="color: #FFF;"></i>
                                        </a>
                                    <?php elseif(UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id'] , false, 'adm.php?modulo=principal/estabelecimento/corpoClinico/formularioProfissional&acao=C')) : ?>
                                        <a class="btn btn-circle btn-icon-only btn-default red"
                                           href="javascript: exibirModalExclusao('<?= $montarAcaoP ?>', '<?= $profissional['nome_completo'] ?>', '<?= $profissional['tipo_profissional'] ?>');"
                                           title="Desligar profissional">
                                            <i class="icon-trash" style="color: #FFF;"></i>
                                        </a>
                                    <?php endif; ?>
                                </li>
                            </ul>
                        </div>
                    </div>
                </div>
            </div>
        <?php
        endforeach;
    else :
        ?>
        <h4 style="text-align: center;">Não há ninguém no seu corpo clínico.</h4>
        <?php
    endif;
}