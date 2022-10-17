<?php
$mFila = new Fila();
$arWhere = array();

if($_POST && isset($_POST['act'])){
    switch($_POST['act']) {
        case 'excluir' :
            $arParams = getParam();
            $mFila->carregarPorId($arParams['id']);

            $mFila->excluirFila($arParams['id']);
            $mFila->commit();

            $db->sucessom($_REQUEST['modulo'], '');
            break;
        case 'carregarMunicipios' :
            montaComboMunicipio($_POST['uf_sigla'], $_POST['municipio_id']);
            die;
        case 'pesquisa_simples' :
            $pesquisaAvancada = false;

            if(!empty($_REQUEST['pesquisa_simples'])){
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
                $arWhere[] = "(
                    (fil.descricao ILIKE '%{$pesquisa_simples}%') OR
                    (mun.municipio ILIKE '%{$pesquisa_simples}%') OR
                    (uf.uf_sigla ILIKE '%{$pesquisa_simples}%') OR
                    (esp.descricao ILIKE '%{$pesquisa_simples}%') OR
                    (TO_CHAR(fil.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
        case 'pesquisa_avancada' :
            $pesquisaAvancada = true;

            if(!empty($_REQUEST['municipio_id'])){
                $municipio_id = $_REQUEST['municipio_id'];
                $arWhere[] = "fil.municipio_id = {$municipio_id}";
            }

            if(!empty($_REQUEST['uf_sigla'])){
                $uf_sigla = $_REQUEST['uf_sigla'];
                $arWhere[] = "uf.uf_sigla = '{$uf_sigla}'";
            }

            if(!empty($_REQUEST['especialidade_id'])){
                $especialidade_id = $_REQUEST['especialidade_id'];
                $arWhere[] = "fil.especialidade_id = {$especialidade_id}";
            }

            if(!empty($_REQUEST['data_limite'])){
                $data_limite = formata_data_sql($_REQUEST['data_limite']);
                $arWhere[] = "fil.data_limite = '{$data_limite}'";
            }

            if(!empty($_REQUEST['descricao'])){
                $descricao = trim(addslashes($_REQUEST['descricao']));
                $arWhere[] = "fil.descricao ILIKE '%{$descricao}%' ";
            }

            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
?>
<br/>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-list"></i>
            <span class="caption-subject bold uppercase">
                Resultados das cargas automáticas
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
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '','', null, '', null, null, 'Pesquise por descrição ou órgão'); ?>
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
                            <div class="col-md-4">
                                <div class="form-group">
                                    <label>Descrição</label>
                                    <?= campo_textom('descricao', 'N', 'S', '', 40, 40, '', '', '', '', 0); ?>
                                </div>
                            </div>
                            <div class="col-md-4">
                                <div class="form-group">
                                    <label>Especialidade</label>
                                    <?php
                                    $mEspecialidade = new Especialidade();
                                    $especialidades = $mEspecialidade->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                                    $db->monta_combom("especialidade_id", $especialidades, 'S', 'Selecione...', '', '', '', '', 'N');
                                    ?>
                                </div>
                            </div>
                            <div class="col-md-4">
                                <div class="form-group">
                                    <label>Data limite</label>
                                    <?= campo_datam('data_limite', 'N', 'S', '', 'S'); ?>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>UF</label>
                                    <?php
                                    $mUnidadeFederacao = new UnidadeFederacao();
                                    $estados = $mUnidadeFederacao->recuperarTodos('uf_sigla AS codigo, uf_nome_estado AS descricao', array(), 2);
                                    $db->monta_combom("uf_sigla", $estados, 'S', 'Selecione...', '', '', '', '', 'N');
                                    ?>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Município</label>
                                    <div id="cidades">
                                        <?php montaComboMunicipio($uf_sigla, $municipio_id); ?>
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
                                    <button type="button" class="btn green-jungle btn-pesquisa-simples <?= ($pesquisaAvancada ? '' : 'hidden') ?>">
                                        <i class="fa fa-search-minus"></i> Pesquisa simples
                                    </button>
                                </div>
                            </div>
                            
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="<?= ($pesquisaAvancada ? 'pesquisa_avancada' : 'pesquisa_simples') ?>"/>
            </form>
        </div>
    </div>

    <?php
    $acao = <<<ACAO
            <div style="min-width: 20px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/fila/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar fila">
                    <i class="icon-pencil"></i>
                </a>
                <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao('<php> return setParam(array('id' => '{id}'), false); </php>', '{descricao}');" data-toggle="tooltip"  title="Excluir fila">
                    <i class="icon-trash"></i>
                </a>
            </div>
ACAO;

    $arConfig = array();
    $arConfig[0]['html'] = <<<HTML
    <a href="?modulo=principal/fila/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar fila">
        {descricao}
    </a>
HTML;
    $arConfig[3]['style'] = "width: 10%; text-align: center;";

    $rs = $mFila->getList($arWhere);

    $mLista = new Listam(array("style" => "width: 100%;"));
    $mLista->setCabecalho(array(
        'Descrição',
        'Município',
        'Especialidade',
        'Data limite',
        'Valor',
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
                <h4 class="modal-title">Excluir fila</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja excluir a fila <strong id="nome"></strong>?
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
            window.location = '?modulo=principal/fila/lista&acao=C';
        });

        $('select[name=uf_sigla]').change(function () {
            carregarMunicipio();
        });

        $('#btn-pesquisa-avancada, #btn-pesquisa-simples').click(function(){
            gerenciarPesquisas();
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

    function carregarMunicipio(municipio_id) {
        var uf_sigla = $('select[name=uf_sigla]').val();

        App.blockUI({
            message: 'Carregando...',
            target: '#cidades'
        });

        $('div#cidades').load(window.location.href, {
            act: 'carregarMunicipios',
            uf_sigla: uf_sigla,
            municipio_id: municipio_id
        }, function () {
            App.unblockUI('#cidades');
        });
    }
</script>