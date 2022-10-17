<?php
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mEstabelecimentoProfissional = new EstabelecimentoEspecialidadeProfissional();
$mValorPagar = new ValorAPagar();

$arWhere[] = "es.excluido IS FALSE";
$arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento('es');

$whereCombos = $arWhere;

//ver($arWhere,d);
if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
        case 'formulario' :
        case 'adicionarObservacao' :
        case 'adicionarObservacaoPessoa' :
            $iniciarAtendimento = true;
            include_once APPRAIZ . 'adm/modulos/principal/estabelecimento/agenda/agendaFormulario.inc';
            die;
            break;
        case 'gerar-estorno' :
            $arParams = array_merge(getParam(), $_POST);
            $dadosAgenda = $mAgenda->getTodosDados($arParams['estabelecimento_profissional_agenda_id']);
            $mValorPagar->gerarValorNegativoAgenda($arParams['estabelecimento_profissional_agenda_id']);

            $retorno = wf_alterarEstado(
                    $dadosAgenda['documento_id'],
                    EstabelecimentoProfissionalAgenda::WF_ACAO_ESTORNAR_CONSULTA,
                    $arParams['justificativa'],
                    $arParams);

            if($retorno){
                $db->sucessom($_REQUEST['modulo'], '', 'Consulta estornada com sucesso!');
            }else{
                $db->insucessom($_REQUEST['modulo'], '', 'Não foi possível estornar a consulta!');
            }

            break;
        case 'pesquisa_simples' :
            $pesquisaAvancada = false;

            if(!empty($_REQUEST['pesquisa_simples'])){
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
                $arWhere[] = "(
                    (pac.nome_completo ILIKE '%{$pesquisa_simples}%') OR
                    (pac.cpf ILIKE '%{$pesquisa_simples}%') OR
                    (p.nome_completo ILIKE '%{$pesquisa_simples}%') OR
                    (pj.razao_social ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
        case 'pesquisa_avancada' :
            $pesquisaAvancada = true;

            if(!empty($_REQUEST['nome_completo'])){
                $nome_completo = trim(addslashes($_REQUEST['nome_completo']));
                $arWhere['nome_completo'] = "pac.nome_completo ILIKE '%{$nome_completo}%' ";
            }

            if(!empty($_REQUEST['cpf'])){
                $cpf = trim(addslashes($_REQUEST['cpf']));
                $arWhere['cpf'] = "pac.cpf ILIKE '%{$cpf}%' ";
            }

            if(!empty($_REQUEST['sexo'])){
                $sexo = trim(addslashes($_REQUEST['sexo']));
                $arWhere['sexo'] = "pac.sexo = '{$sexo}' ";
            }

            if(!empty($_REQUEST['profissionais']) && is_array($_REQUEST['profissionais'])){
                $profissionais = $_REQUEST['profissionais'];
                $arWhere['profissionais'] = "p.id IN ('" . implode("','", $_REQUEST['profissionais']). "') ";
            }

            if(!empty($_REQUEST['especialidades']) && is_array($_REQUEST['especialidades'])){
                $especialidades = $_REQUEST['especialidades'];
                $arWhere['especialidades'] = "e.id IN ('" . implode("','", $_REQUEST['especialidades']). "') ";
            }

            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('');

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
            <i class="fa fa-refresh"></i>
            <span class="caption-subject bold uppercase">
                Lista de consultas que podem ser estornadas
            </span>
        </div>
    </div>
    <div class="alert alert-warning">
        <strong>Atenção</strong> Só é possível estornar consultas em até 7 dias após sua finalização
    </div>
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="form-pesquisa" method="POST">
                <div class="form-body">
                    <div class="pesquisa-simples <?= ($pesquisaAvancada ? 'hidden' : '') ?>">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '','', null, '', null, null, 'Pesquise por estabelecimento, paciente ou profissional'); ?>
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
                        <div class="row">
                            <div class="col-sm-12 col-md-12 col-lg-6">
                                <div class="form-group">
                                    <label class="control-label">Profissionais</label>
                                    <select id="profissionais" name="profissionais[]" multiple style="display: none;"></select>
                                </div>
                            </div>
                            <div class="col-sm-12 col-md-12 col-lg-6">
                                <div class="form-group">
                                    <label class="control-label">Especialidades</label>
                                    <select id="especialidades" name="especialidades[]" multiple style="display: none;"></select>
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
            <div style="min-width: 100px;">
                 <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:abrirModalConfirmEstorno('<php> return setParam(array('estabelecimento_profissional_agenda_id' => '{id}'), false); </php>');" data-toggle="tooltip" title="Estornar consulta">
                    <i class="icon-refresh"></i>
                </a>
                <a class="btn btn-circle btn-icon-only btn-default green" href="javascript:abrirAgenda('<php> return setParam(array('agenda_id' => {id}, 'id' => '{estabelecimento_id}'), false); </php>');" title="Visualizar" style="margin: 2px;">
                    <i class="icon-magnifier"></i>
                </a>
                
            </div>
ACAO;



    $arConfig = array();
    $arConfig[3]['style'] = "text-align: center;";
    $arConfig[4]['style'] = "text-align: center;";

    $rs = $mAgenda->getListPassiveisEstorno($arWhere);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho( array(
        'Estabelecimento',
        'Especialidade',
        'Paciente',
        'Profissional',
        'Data',
        'Horário',
        'Tipo de serviço',
        'Procedimento',
        'Sala',
        'Ação'
    ));
    $mLista->setCorpo( $rs, $arConfig );
    $mLista->setAcao( $acao );
    $mLista->show();
    ?>
</div>

<div class="modal fade in" id="modal-estorno" tabindex="-1" role="modal-estorno" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Estornar consulta</h4>
            </div>
            <div class="modal-body">
                <form method="post" class="form-estorno">
                    <strong><span style="color: red;">Atenção</span> Ao concluir essa operação o valor gerado pela consulta será zerado e seu estado passará a ser 'Estornada'!</strong><br><br>
                    Deseja realmente estornar essa consulta?<br><br>
                    <input type="hidden" name="act" value="gerar-estorno" />
                    <input type="hidden" name="p" />
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Justificativa</label>
                                <?= campo_textaream('justificativa', 'S', 'S', '', '10', '5', '500', '', 0, '') ?>
                            </div>
                        </div>
                    </div>
                </form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="estornarRegistro();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">Não</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>
<div class="modal fade in" id="modal-abrir-agenda" tabindex="-1" role="modal-abrir-agenda" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <form id="form-abrir-agenda" method="post">
                <div id="form-abrir-agenda-content"></div>
            </form>
        </div>
    </div>
</div>
<script type="text/javascript">
    $(function () {
        $(".form-estorno").validateForm();

        $('.btn-limpar').click(function () {
            window.location = '?modulo=principal/estabelecimento/lista&acao=C';
        });

        $('#profissionais').selectize({
            persist: false,
            maxItems: null,
            placeholder: 'Todos',
            plugins: ['remove_button'],
            valueField: 'codigo',
            labelField: 'descricao',
            searchField: ['descricao'],
            sortField: [
                {field: 'descricao', direction: 'asc'}
            ],
            items: <?= json_encode($profissionais) ?>,
            options: <?= $mEstabelecimentoProfissional->getComboPessoasProfissionaisByWhere($whereCombos, true); ?>,
            render: {
                item: function (item, escape) {
                    var name = escape(item.descricao);
                    return '<div>' +
                        (name ? '<span class="name">' + name + '</span>' : '') +
                        '</div>';
                },
                option: function (item, escape) {
                    var name = escape(item.descricao);
                    var urlFoto = (item.sexo === 'F' ? '../imagens/woman.png' : '../imagens/man.png');
                    if (item.foto_arquivo_id) {
                        urlFoto = '../slideshow/slideshow/verimagem.php?arqid=' + item.foto_arquivo_id + '&newwidth=300';
                    }

                    return '<div>' +
                        '<img src="' + urlFoto + '" alt="" style="width: 30px; height: 30px; float: left; margin-right: 5px; border-radius: 100px !important;" />' +
                        '<span style="vertical-align: sub;">' + name + '</span>' +
                        '</div>';
                }
            }
        });

        $('#especialidades').selectize({
            persist: false,
            maxItems: null,
            placeholder: 'Todas',
            plugins: ['remove_button'],
            valueField: 'codigo',
            labelField: 'descricao',
            searchField: ['descricao'],
            sortField: [
                {field: 'descricao', direction: 'asc'}
            ],
            items: <?= json_encode($especialidades) ?>,
            options: <?= $mEstabelecimentoProfissional->getComboEspecialidadesByWhere($whereCombos, true); ?>,
            render: {
                item: function (item, escape) {
                    var name = escape(item.descricao);
                    return '<div>' +
                        (name ? '<span class="name">' + name + '</span>' : '') +
                        '</div>';
                },
                option: function (item, escape) {
                    var name = escape(item.descricao);

                    return '<div>' +
                        '<span style="vertical-align: sub;">' + name + '</span>' +
                        '</div>';
                }
            }
        });

    });

    function abrirAgenda(p) {
        var content = $('#form-abrir-agenda-content');
        content.html('<div style="height: 150px; width: 100%;"></div>');
        p = p ? p : '<?= setParam(getParam(), false) ?>';

        App.blockUI({
            message: 'Carregando...',
            target: '#form-abrir-agenda-content'
        });

        content.load(window.location.href, {
            act: 'formulario',
            p: p,
            data_agenda: null,
            hora_inicio: null,
            profissional_id: '<?= ($p['profissional_id'] ? $p['profissional_id'] : '') ?>'
        }, function () {
            App.unblockUI('#form-abrir-agenda-content');
        });

        $('#modal-abrir-agenda').modal({
            backdrop: 'static',
        }).modal('show');
    }

    function abrirModalConfirmEstorno(p, nome){
        var modal = $('#modal-estorno');

        $('form.form-estorno [name=p]').val(p);
        $('form.form-estorno [name=justificativa]').val('');
        modal.modal('show');
    }

    function estornarRegistro(){
        $('form.form-estorno').submit();
    }
</script>
