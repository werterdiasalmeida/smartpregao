<?php
$mPessoaFisica = new PessoaFisica();
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mProfissional = new Profissional();
$arWhere = array();
$pesquisaAvancada = false;

if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
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
            break;
    }
}

$usucpfFormatado = formatar_cpf($_SESSION['usucpf']);
$estabelecimentos = $mProfissional->listaEstabelecimentosPorCpfProfissional($usucpfFormatado);

Estabelecimento::setEstabelecimentoDoMedicoNaSession();

if (isset($_SESSION['estabelecimento_selecionado']) && !empty($_SESSION['estabelecimento_selecionado'])) {
    $estabelecimento_id = $_SESSION['estabelecimento_selecionado'];
}

$arWhere[] = $mPessoaFisica->getWherePessoaAtendidaPeloProfissionalAndEstabelecimento($usucpfFormatado, $estabelecimento_id);

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalhoDoMedico();
EstabelecimentoProfissionalAgenda::montaAvisoAtendimento();

monta_titulom('Meus Pacientes');
$db->cria_abam(ABA_MEDICO, 'adm.php?modulo=principal/medico/meusPacientes&acao=C', array(), array(), false);
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
            <div class="caption font-blue-sharp">
                <i class="icon-users font-blue-sharp"></i>
                <span class="caption-subject bold uppercase">
                Pacientes
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

                        <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                            <div class="row">
                                <div class="col-md-12">
                                    <div class="form-group">
                                        <label>Nome completo</label>
                                        <?= campo_textom('nome_completo', 'N', 'S', '', 190, 190, '', '', '', '', 0); ?>
                                    </div>
                                </div>
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
                                <div class="pesquisa-avancada <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                                    <button type="submit" class="btn blue">
                                        <i class="fa fa-search"></i> Pesquisar
                                    </button>
                                    <button type="button" class="btn green-jungle btn-pesquisa-simples">
                                        <i class="fa fa-search-minus"></i> Pesquisa simples
                                    </button>
                                </div>
                            </div>
                            <div class="col-xs-5 col-sm-3 text-right">
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
                <a class="btn btn-circle btn-icon-only btn-default blue" target="_blank" href="?modulo=principal/medico/atendimento&acao=C&p=<php> return setParam(array('pessoa_paciente_id' => '{id}'), false); </php>" data-toggle="tooltip" title="Prontuário">
                    <i class="icon-notebook"></i>
                </a>
            </div>
ACAO;

        $arConfig = array();
        $arConfig[0]['html'] = <<<HTML
    <a href="?modulo=principal/medico/atendimento&acao=C&p=<php> return setParam(array('pessoa_paciente_id' => '{id}'), false); </php>" target="_blank" data-toggle="tooltip" title="Abrir Prontuário">
        {nome_completo}
    </a>
HTML;
        $arConfig[2]['style'] = 'text-align: center;';
        $arConfig[2]['style'] = 'text-align: center;';
        $arConfig[3]['style'] = 'text-align: center;';
        $arConfig[3]['html'] = '<php>return formata_data("{data_nascimento}");</php>';

        $rs = $mPessoaFisica->getList($arWhere, true, null, null, false);

        $mLista = new Listam(array("style" => "width: 100%;"));
        $mLista->setCabecalho( array(
            'Nome completo',
            'CPF',
            'Sexo',
            'Data de nascimento',
            'Ação'
        ));
        $mLista->setCorpo( $rs, $arConfig );
        $mLista->setAcao( $acao );
        $mLista->show();
        ?>
    </div>


    <script type="text/javascript">

        $(function () {
            $('#modal-abrir-agenda').on('hidden.bs.modal', function () {
                $('.timepicker-24').timepicker('remove');
                $(this).find('.modal-dialog').css({width: ''});
            });

            $('[name=estabelecimento_id]').change(function () {
                $('[name=form-estabelecimento]').submit();
            });
        });
    </script>