<?php
$mPessoaFisica = new PessoaFisica();
$arWhere = array();
$pesquisou = false;
$temErro = false;
if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
        case 'excluir' :
            $arParams = getParam();
            $mPessoaFisica->carregarPorId($arParams['id']);

            $mPessoaFisica->excluirPessoaFisica($arParams['id']);
            $mPessoaFisica->commit();

            $db->sucessom($_REQUEST['modulo'], '', 'Pessoa excluída com sucesso');

            break;
        case 'pesquisa_simples' :
            $pesquisaAvancada = false;

            if(!empty($_REQUEST['pesquisa_simples'])){
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
                $arWhere[] = "(
                    (pes.nome_completo ILIKE '%{$pesquisa_simples}%') OR
                    (pes.nome_social ILIKE '%{$pesquisa_simples}%') OR
                    (pes.cpf ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
        case 'pesquisa_avancada' :
            $pesquisou = true;
            $pesquisaAvancada = true;
            if(!empty($_REQUEST['nome_completo']) ){
                $nome_completo = trim(addslashes($_REQUEST['nome_completo']));
                if (strlen($_REQUEST['nome_completo']) >= 4) {
                    $arWhere['nome_completo'] = "pes.nome_completo ILIKE '%{$nome_completo}%' ";
                } else {
                    $temErro = true;
                    addMsgWarning('Nome deve possuir ao menos 4 caracteres.');
                }
            }

            if(!empty($_REQUEST['nome_social'])){
                $nome_social = trim(addslashes($_REQUEST['nome_social']));
                $arWhere['nome_social'] = "pes.nome_social ILIKE '%{$nome_social}%' ";
            }

            if(!empty($_REQUEST['cpf']) ){
                $cpf = trim(addslashes($_REQUEST['cpf']));
                if (strlen($_REQUEST['cpf']) == 14 && validaCPF($_REQUEST['cpf'])) {
                    $arWhere['cpf'] = "pes.cpf ILIKE '%{$cpf}%' ";
                } else {
                    $temErro = true;
                    addMsgWarning('CPF inválido.');
                }
            }

            if(!empty($_REQUEST['sexo'])){
                $sexo = trim(addslashes($_REQUEST['sexo']));
                $arWhere['sexo'] = "pes.sexo = '{$sexo}'" ;
            }
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Pessoas');
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
            <i class="fa icon-user-following"></i>
            <span class="caption-subject bold uppercase">
                Lista de pessoas
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" onsubmit="" class="estabelecimento-form" method="POST">
                <div class="form-body">
                    <div class="pesquisa-simples <?= (true ? 'hidden' : '') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '','', null, '', null, null, 'Pesquise por nome ou CPF'); ?>
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

                    <div class="pesquisa-avancada <?= (true ? '' : 'hidden') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label>Nome completo</label>
                                    <?= campo_textom('nome_completo', 'N', 'S', '', 190, 190, '', '', '', '', 0); ?>
                                </div>
                            </div>
                            <!--<div class="col-md-6">
                                <div class="form-group">
                                    <label>Nome social</label>
                                    <?= campo_textom('nome_social', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                </div>
                            </div>-->
                        </div>
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>CPF</label>
                                    <?= campo_textom('cpf', 'N', 'S', '', 19, 19, '999.999.999-99', '', '', '', 0); ?>
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
                            <div class="pesquisa-avancada <?= (true ? '' : 'hidden') ?>">
                                <button type="submit" class="btn blue">
                                    <i class="fa fa-search"></i> Pesquisar
                                </button>
                                <!--                                <button type="button" class="btn green-jungle btn-pesquisa-simples">-->
                                <!--                                    <i class="fa fa-search-minus"></i> Pesquisa simples-->
                                <!--                                </button>-->
                            </div>
                        </div>
                        <div class="col-xs-5 col-sm-3 text-right">
                            <!--<a class="btn green" href="?modulo=principal/pessoafisica/formulario&acao=C">
                                <i class="fa fa-plus-circle"></i>
                                Cadastrar nova pessoa
                            </a>-->
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="<?= (true ? 'pesquisa_avancada' : 'pesquisa_simples') ?>"/>
            </form>
        </div>
    </div>

    <?php
    $acao = <<<ACAO
            <div style="min-width: 5px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/pessoafisica/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar pessoa">
                    <i class="icon-pencil"></i>
                </a>
                <a class="btn btn-circle btn-icon-only btn-default green" target="_blank" href="?modulo=principal/medico/atendimento&acao=C&p=<php> return setParam(array('pessoa_paciente_id' => '{id}'), false); </php>" data-toggle="tooltip" title="Prontuário">
                    <i class="icon-notebook"></i>
                </a>
                <!--<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao('<php> return setParam(array('id' => '{id}'), false); </php>', '{nome_completo}');" data-toggle="tooltip"  title="Excluir pessoa">
                     <i class="bi bi-x-square"></i>
                </a>-->
            </div>
ACAO;

    $arConfig = array();
    $arConfig[0]['html'] = <<<HTML
    <a href="?modulo=principal/pessoafisica/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar pessoa">
        {nome_completo}
    </a>
HTML;
    $arConfig[2]['style'] = 'text-align: center;';
    $arConfig[2]['style'] = 'text-align: center;';
    $arConfig[3]['style'] = 'text-align: center;';
    $arConfig[3]['html'] = '<php>return formata_data("{data_nascimento}");</php>';
    $arConfig[4]['style'] = 'text-align: left;';
    $arConfig[4]['html'] = '<php>
        $telefones = array_filter(["{telefone}", "{telefone_fixo}"]);
        return implode(" / ", $telefones);
    </php>';

    if (!empty($arWhere)) {
        $rs = $mPessoaFisica->getList($arWhere);
    } else if ($pesquisou && !$temErro) {
        addMsgWarning('Nenhum filtro informado');
    }

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho( array(
        'Nome completo',
//        'Nome social',
        'CPF',
        'Sexo',
        'Data de nascimento',
        'Telefone',
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
                Você tem certeza que deseja excluir a pessoa <strong id="nome"></strong>?
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
            window.location = '?modulo=principal/pessoafisica/lista&acao=C';
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
