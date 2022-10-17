<?php
$mProfissional = new Profissional();
$arWhere = array();

if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
        case 'excluir' :
            $arParams = getParam();
            $mProfissional->carregarPorId($arParams['id']);

            $mProfissional->excluirPorPessoaFisica($arParams['id']);
            $mProfissional->commit();

            $db->sucessom($_REQUEST['modulo'], '', 'Profissional excluído com sucesso');
            break;
        case 'pesquisa_simples' :
            $pesquisaAvancada = false;

            if(!empty($_REQUEST['pesquisa_simples'])){
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
                $arWhere[] = "(
                    (pes.nome_completo ILIKE '%{$pesquisa_simples}%') OR
                    (pes.nome_social ILIKE '%{$pesquisa_simples}%') OR
                    (pes.cpf ILIKE '%{$pesquisa_simples}%') OR
                    (tp.descricao ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
        case 'pesquisa_avancada' :
            $pesquisaAvancada = true;
            if(!empty($_REQUEST['nome_completo'])){
                $nome_completo = trim(addslashes($_REQUEST['nome_completo']));
                $arWhere['nome_completo'] = "pes.nome_completo ILIKE '%{$nome_completo}%' ";
            }

            if(!empty($_REQUEST['nome_social'])){
                $nome_social = trim(addslashes($_REQUEST['nome_social']));
                $arWhere['nome_social'] = "pes.nome_social ILIKE '%{$nome_social}%' ";
            }

            if(!empty($_REQUEST['cpf'])){
                $cpf = trim(addslashes($_REQUEST['cpf']));
                $arWhere['cpf'] = "pes.cpf ILIKE '%{$cpf}%' ";
            }

            if(!empty($_REQUEST['sexo'])){
                $sexo = trim(addslashes($_REQUEST['sexo']));
                $arWhere['sexo'] = "pes.sexo = '{$sexo}'" ;
            }

            if(!empty($_REQUEST['tipo_profissional_id'])){
                $tipo_profissional_id = trim(addslashes($_REQUEST['tipo_profissional_id']));
                $arWhere['tipo_profissional_id'] = "p.tipo_profissional_id = '{$tipo_profissional_id}'" ;
            }
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Profissionais');
?>
<style type="text/css">
    .campos-pesquisa{
        border-bottom: 1px solid #eef1f5;
        padding: 10px 0px 10px 0px;
        margin-bottom: 10px;
    }
</style>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa icon-user-follow"></i>
            <span class="caption-subject bold uppercase">
                Lista de profissionais
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="estabelecimento-form" method="POST">
                <div class="form-body">
                    <div class="pesquisa-simples <?= ($pesquisaAvancada ? 'hidden' : '') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '','', null, '', null, null, 'Pesquise por nome ou CPF ou tipo'); ?>
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
                                    <label>Nome</label>
                                    <?= campo_textom('nome_completo', 'N', 'S', '', 190, 190, '', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>CPF</label>
                                    <?= campo_textom('cpf', 'N', 'S', '', 19, 19, '999.999.999-99', '', '', '', 0); ?>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Tipo</label>
                                    <?php
                                    $mTipo = new TipoProfissional();
                                    $tipos = $mTipo->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                                    $db->monta_combom("tipo_profissional_id", $tipos, 'S', 'Selecione...', '', '', '', '', 'N');
                                    ?>
                                </div>
                            </div>

                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Sexo</label>
                                    <?= campo_radiom('sexo', array("Todos" => array("valor" => ""),"Masculino" => array("valor" => "M"), "Feminino" => array("valor" => "F")), 'N'); ?>
                                </div>
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
                                    <button type="button" class="btn green-jungle btn-pesquisa-simples">
                                        <i class="fa fa-search-minus"></i> Pesquisa simples
                                    </button>
                            </div>
                        </div>
                        <div class="col-xs-5 col-sm-7 text-right">
                            <!--<a class="btn green" href="?modulo=principal/profissional/formulario&acao=C">
                                <i class="fa fa-plus-circle"></i>
                                Cadastrar novo profissional
                            </a>-->
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="<?= ($pesquisaAvancada ? 'pesquisa_avancada' : 'pesquisa_simples') ?>"/>
            </form>
        </div>
    </div>

    <?php
    $acao = <<<ACAO
            <div style="min-width: 5px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/profissional/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar">
                    <i class="icon-pencil"></i>
                </a>
                <!--<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao('<php> return setParam(array('id' => '{id}'), false); </php>', '{nome_completo}');" data-toggle="tooltip"  title="Excluir">
                    <i class="icon-trash"></i>
                </a>-->
            </div>
ACAO;

    $arConfig = array();
    $arConfig[0]['html'] = <<<HTML
    <a href="?modulo=principal/profissional/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar">
        {nome_completo}
    </a>
HTML;
    $arConfig[1]['style'] = 'text-align: center;';
    $arConfig[3]['html'] = <<<PHP
    <php>
        return '<span class="label label-sm label-success">'
            . implode('</span>&nbsp;<span class="label label-sm label-success">', explode(',', '{tipo}'))  .
            '</span>'
    </php>
PHP;

    $rs = $mProfissional->getList($arWhere, true, null, array(ADM_PERFIL_GERAL, ADM_PERFIL_LOCAL));

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho( array(
        'Nome',
        'CPF',
        'Sexo',
        'Tipo',
        'Ação'
    ));
    $mLista->setCorpo( $rs, $arConfig );
    $mLista->setAcao( $acao );
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
                <h4 class="modal-title">Excluir pessoa</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja excluir o profissional <strong id="nome"></strong>?
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
        $('.btn-limpar').click(function () {
            window.location = '?modulo=principal/profissional/lista&acao=C';
        });
    });

    function exibirModalExclusao(p, nome){
        var modal = $('#modal-excluir');

        $('form.form-excluir [name=p]').val(p);
        modal.find('strong#nome').html(nome);
        modal.modal('show');
    }

    function excluirRegistro(){
        $('form.form-excluir').submit();
    }
</script>
