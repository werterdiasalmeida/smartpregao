<?php
$mCotacaoDispensa = new CotacaoDispensa();
$arWhere = array();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'excluir':
            $arParams = getParam();
            $mCotacaoDispensa->carregarPorId($arParams['id']);

            $mCotacaoDispensa->excluirCD($arParams['id']);
            $mCotacaoDispensa->commit();

            $db->sucessom($_REQUEST['modulo'], '');
            break;
        case 'carregarMunicipios':
            montaComboMunicipio($_POST['uf_sigla'], $_POST['municipio_id']);
            die;
        case 'pesquisa_simples':
            $pesquisaAvancada = false;

            if (!empty($_REQUEST['pesquisa_simples'])) {
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));;
                $arWhere[] = "(
                    (edi.objeto ILIKE '%{$pesquisa_simples}%') OR
                    (edi.orgao ILIKE '%{$pesquisa_simples}%') OR
                    (TO_CHAR(edi.data_limite, 'DD/MM/YYYY') ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
?>
<br />
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="fa fa-list"></i>
            <span class="caption-subject bold uppercase">
            Cotação | Dispensa eletrônica | Credenciamento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="edital-form" method="POST">
                <div class="form-body">
                    <div class="pesquisa-simples">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 100, 100, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por objeto, data ou órgão'); ?>
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
                            <div class="col-md-12 text-right">
                                <a class="btn green" href="?modulo=principal/cotacao-dispensa/formulario&acao=C">
                                    <i class="fa fa-plus-circle"></i>
                                    Incluir Novo
                                </a>
                            </div>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="<?= ($pesquisaAvancada ? 'pesquisa_avancada' : 'pesquisa_simples') ?>" />
            </form>
        </div>
    </div>

    <?php
    $acao = <<<ACAO
            <div style="min-width: 20px;">
                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/cotacao-dispensa/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Editar">
                    <i class="icon-pencil"></i>
                </a>
                <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:exibirModalExclusao('<php> return setParam(array('id' => '{id}'), false); </php>', '{orgao}');" data-toggle="tooltip"  title="Excluir">
                <i class="bi bi-x-square"></i>
                </a>
            </div>
ACAO;

    $arConfig = array();
    $arConfig[2]['html'] = <<<HTML
    <a href="?modulo=principal/cotacao-dispensa/formulario&acao=C&p=<php> return setParam(array('id' => '{id}'), false); </php>" data-toggle="tooltip" title="Alterar dados">
        {orgao}
    </a>
HTML;
    $arConfig[4]['html'] = <<<HTML
    <spam class="badge badge-primary"><b>{dias}</b></spam>
HTML;


    $arConfig[4]['style'] = "width: 10%; text-align: center;";
    // Filtro para não trazer a velhas
    $arWhere[] = "data_limite >= NOW() ";

    $rs = $mCotacaoDispensa->getList($arWhere);

    $mCotacaoDispensa = new Listam(array("style" => "width: 100%;"));
    $mCotacaoDispensa->setCabecalho(array(
        'Tipo',
        'Portal',
        'Órgão',
        'Data Limite',
        'Dias restantes',
        'Objeto',
        'Local',
        'Ação'
    ));
    $mCotacaoDispensa->setCorpo($rs, $arConfig);
    $mCotacaoDispensa->setAcao($acao);
    $mCotacaoDispensa->show();
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
                <h4 class="modal-title">Excluir edital</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja excluir o Registro <strong id="nome"></strong>?
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
            window.location = '?modulo=principal/cotacao-dispensa/lista&acao=C';
        });

        $('select[name=uf_sigla]').change(function() {
            carregarMunicipio();
        });

        $('#btn-pesquisa-avancada, #btn-pesquisa-simples').click(function() {
            gerenciarPesquisas();
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
        }, function() {
            App.unblockUI('#cidades');
        });
    }
</script>