<?php
$mProcedimento = new Procedimento();
$mProfissionalEstabelecimentoProcedimento = new ProfissionalEstabelecimentoProcedimento();
$mEstabelecimentoEspecialidadeProfissional = new EstabelecimentoEspecialidadeProfissional();
$params = getParam();
$pUrlParam = (isset($params['id']) && !empty($params['id']) ? setParam(array('id' => $params['id'])) : '');

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            $paramsSalvar = array_merge($_POST, getParam());
            $paramsSalvar['id'] = $paramsSalvar['procedimento_id'];
            $procedimentoId = $mProcedimento->salvarProcedimento($paramsSalvar);

            $profissionais = $_POST['profissional'] ? $_POST['profissional'] : array();
            $mProfissionalEstabelecimentoProcedimento = new ProfissionalEstabelecimentoProcedimento();
            $mProfissionalEstabelecimentoProcedimento->salvarProfissionalProcedimento($profissionais, $procedimentoId, $params['id']);
            $mProcedimento->commit();

            $db->sucessom('principal/estabelecimento/procedimento/lista', $pUrlParam);
            break;
        case 'buscarTuss' :
            $mTuss = new Tuss();
            $dsc = $_POST['descricao'];
            $rs = $mTuss->getList(["(procedimento ILIKE '%{$dsc}%' OR cod_tuss ILIKE '%{$dsc}%')"]);
            die(json_encode(utf8ArrayEncode($rs)));
            break;
    }
}

$profissionais = $mEstabelecimentoEspecialidadeProfissional->getListProfissionais(array("est.id = {$params['id']}"), false);
$profissionaisVinculados = array();
$tussSelecionadosJson = '[]';
if (isset($params['procedimento_id']) && !empty($params['procedimento_id'])) {
    $mProcedimento->carregarPorId($params['procedimento_id']);
    $dados = $mProcedimento->getDados();
    extract($dados);

    $profissionaisVinculados = $mProfissionalEstabelecimentoProcedimento->getProfissionaisIdPorEstabelecimentoProcedimento($params['id'], $params['procedimento_id']);

    if($codigo_ans) {
        $mTuss = new Tuss();
        $tussSelecionados = $mTuss->getList(["cod_tuss IN ('" . implode("', '", explode(',', $codigo_ans)) . "')"]);
        $tussSelecionadosJson = json_encode(utf8ArrayEncode($tussSelecionados));
    }

    if(!$db->testa_superuser()){
        UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($estabelecimento_id);
    }
}
include APPRAIZ . "includes/cabecalhom.inc";

echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $params['id'])), null, array(), true, true);
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-check font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do procedimento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="procedimento-form" method="POST">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Descrição</label>
                            <?= campo_textom('descricao', 'S', 'S', '', 90, 200, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Tipo de guia</label>
                            <?php
                            $mTipoGuia = new TipoGuia();
                            $tiposGuia = $mTipoGuia->recuperarTodos("id AS codigo, descricao, tipo", ["excluido IS FALSE"], 2);
                            $db->monta_combom("tipo_guia_id", $tiposGuia, 'S', 'Selecione...', '', '', '', '', 'S');
                            ?>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Estabelecimento</label>
                            <?php
                            $mEstabelecimento = new Estabelecimento();
                            $estabelecimentos = $mEstabelecimento->getComboPorResponsabilidade(array("(est.id = {$params['id']} OR est.id IS NULL)"));
                            $opcaoSelecione = '';
                            $estabelecimentoObrigatorio = 'S';
                            if($db->testa_superuser()){
                                $opcaoSelecione = 'Padrão';
                                $estabelecimentoObrigatorio = 'N';
                            }

                            if(empty($id) && empty($estabelecimento_id)){
                                $estabelecimento_id = count($estabelecimentos) === 1 ? $estabelecimentos[0]['codigo'] : '';
                            }

                            $comboEstabelecimentoHabilitada = $abaEstabelecimento ? 'N' : 'S';
                            $db->monta_combom("estabelecimento_id", $estabelecimentos, $comboEstabelecimentoHabilitada, $opcaoSelecione, '', '', '', '', $estabelecimentoObrigatorio);
                            ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Código ANS (TUSS)</label>
                            <?= campo_textom('codigo_ans', 'N', 'S', '', 90, 50, '', '', '', '', 0); ?>
                        </div>
                    </div>
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Código SUS</label>
                            <?= campo_textom('codigo_sus', 'N', 'S', '', 90, 50, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>
            </div>

            <div class="form-group">
                <label>Profissionais que executam este procedimento</label>
                <select id="profissional" name="profissional[]" class="form-control select2-multiple" multiple>
                    <?php foreach ($profissionais as $profissional): ?>
                        <option value="<?= $profissional['profissional_id'] ?>" <?= (in_array($profissional['profissional_id'], $profissionaisVinculados) ? 'selected' : '') ?>>
                            <?= $profissional['nome_completo'] ?> (<?= $profissional['tipo_profissional'] ?>)
                        </option>
                    <?php endforeach;?>
                </select>

            </div>

            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar"/>
        </form>
    </div>
</div>

<script type="text/javascript">
    $(function () {
        $(".procedimento-form").validateForm();
        $(".select2, .select2-multiple").select2({
            language : 'pt-BR',
            placeholder: 'Selecione...',
            width: null
        });

        $('[name=codigo_ans]').selectize({
            plugins: ['remove_button'],
            delimiter: ',',
            persist: false,
            valueField: 'codigo',
            labelField: 'descricao',
            searchField: ['descricao', 'codigo'],
            options: <?= $tussSelecionadosJson ?>,
            render: {
                item: function(item, escape) {
                    return '<div>' +
                               item.codigo +
                           '</div>';
                },
                option: function(item, escape) {
                    return '<div>' +
                               '<strong>' + escape(item.codigo) + '</strong> - ' +
                               escape(item.descricao) +
                           '</div>';
                }
            },
            load: function(query, callback) {
                if (!query.length) return callback();
                $.ajax({
                    url: window.location.href,
                    type: 'POST',
                    dataType : 'json',
                    data : {
                        descricao : query,
                        act : 'buscarTuss'
                    },
                    error: function() {
                        callback();
                    },
                    success: function(response) {
                        callback(response);
                    }
                });
            }
        });

        $('.btn-voltar').click(function () {
            window.location = '?modulo=principal/estabelecimento/procedimento/lista&acao=C<?= $pUrlParam ?>';
        });
    });
</script>
