<?php
$moduloLista = $abaMedico ? 'principal/medico/modeloDocumento/lista' : 'principal/estabelecimento/modeloDocumento/lista';
$moduloFormulario = $abaMedico ? 'principal/medico/modeloDocumento/formulario' : 'principal/estabelecimento/modeloDocumento/formulario';

$mModeloDocumento = new ModeloDocumento();
$mTipoProntuario = new TipoProntuario();

$tiposSemFormatacao = $mTipoProntuario->getTiposSemFormatacao();

$params = getParam();

$pUrlParam = '';
$pUrlParam = (isset($params['id']) && !empty($params['id']) ? setParam(array('id' => $params['id'])) : '');

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            $paramsSalvar = array_merge($_POST, getParam());
            $paramsSalvar['descricao'] = in_array($paramsSalvar['tipo_prontuario_id'], $tiposSemFormatacao) ? $paramsSalvar['descricao_noformat'] : $paramsSalvar['descricao_format'];
            $paramsSalvar['descricao'] = str_replace('\"', '"', $paramsSalvar['descricao']);
            $paramsSalvar['id'] = $paramsSalvar['modelo_documento_id'];
            $paramsSalvar['privado'] = $abaMedico ? 't' : 'f';
            $paramsSalvar['estabelecimento_id'] = $abaMedico ? null : $paramsSalvar['estabelecimento_id'];

            $modeloDocumentoId = $mModeloDocumento->salvarModeloDocumento($paramsSalvar);
            $mModeloDocumento->commit();

            $pUrlParam = setParam(array('id' => $params['id'], 'modelo_documento_id' => $modeloDocumentoId));

            $db->sucessom($moduloFormulario, $pUrlParam);
            break;
        case 'visualizar' :
            include_once 'visualizar.inc';
            die;
            break;
    }
}

if (isset($params['modelo_documento_id']) && !empty($params['modelo_documento_id'])) {
    $mModeloDocumento->carregarPorId($params['modelo_documento_id']);
    $dados = $mModeloDocumento->getDados();
    extract($dados);

    if (!$db->testa_superuser() && $dados['criado_por'] !== $_SESSION['usuario_id']) {
        UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($params['id']);
    }
}
include APPRAIZ . "includes/cabecalhom.inc";
echo !$abaMedico ? Estabelecimento::gerarCabecalho() : '';

if($abaMedico) {
    monta_titulom('Insira os dados do modelo de documento');
    $db->cria_abam($abacod_tela, $url, '', array(), false, true);
} else {
    UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $params['id'])), null, array(), true, true);
}
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-check font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do modelo de documento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="modelo-documento-form" method="POST">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Título</label>
                            <?= campo_textom('titulo', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Tipo de pedido</label>
                            <?php
                            $tiposProntuario = $mTipoProntuario->recuperarTodos('id AS codigo, descricao AS descricao', array("excluido IS FALSE"), 2);
                            $db->monta_combom("tipo_prontuario_id", $tiposProntuario, 'S', 'Selecione...', '', '', '', '', 'S');
                            ?>
                        </div>
                    </div>
                </div>
                <div class="row dsc-format" style="display: none;">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Descrição </label>
                            <?= campo_textaream('descricao_format', 'S', 'S', '', '10', '5', '10000', '', 0, '', false, NULL, $descricao) ?>
                        </div>
                    </div>
                </div>
                <div class="row dsc-no-format" style="display: none;">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Descrição</label>
                            <?= campo_textaream('descricao_noformat', 'S', 'S', '', '10', '5', '10000', '', 0, '', false, NULL, trim(html_entity_decode(strip_tags($descricao)))) ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Variáveis</label>
                            <div class="form-group">
                                <div class="btn-group">
                                    <?php foreach ($mModeloDocumento->getVariaveis() as $valor => $descricao) : ?>
                                        <button type="button" class="btn btn-default"
                                                onclick="adicionarVariavel('<?= $valor ?>')"><?= $descricao ?></button>
                                    <?php endforeach; ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <?php if (!$abaMedico): ?>
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Estabelecimento</label>
                                <?php
                                $mEstabelecimento = new Estabelecimento();
                                $estabelecimentos = $mEstabelecimento->getComboPorResponsabilidade(array("(est.id = {$params['id']} OR est.id IS NULL)"));
                                $opcaoSelecione = '';
                                $estabelecimentoObrigatorio = 'S';
                                if ($db->testa_superuser()) {
                                    $opcaoSelecione = 'Padrão';
                                    $estabelecimentoObrigatorio = 'N';
                                }

                                if (empty($id) && empty($estabelecimento_id)) {
                                    $estabelecimento_id = count($estabelecimentos) === 1 ? $estabelecimentos[0]['codigo'] : '';
                                }

                                $comboEstabelecimentoHabilitada = $abaEstabelecimento ? 'N' : 'S';
                                $db->monta_combom("estabelecimento_id", $estabelecimentos, $comboEstabelecimentoHabilitada, $opcaoSelecione, '', '', '', '', $estabelecimentoObrigatorio);
                                ?>
                            </div>
                        </div>
                    </div>
                <?php endif; ?>
            </div>

            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
                <button type="button" class="btn green btn-print">
                    <i class="fa fa-print"></i> Visualizar impressão
                </button>
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar"/>
        </form>
    </div>
</div>

<form role="form" name="formulario-print" class="modelo-documento-form-print" method="POST" target="_blank"
      style="display: none;">
    <textarea name="descricao"></textarea>
    <input type="hidden" name="tipo_prontuario_id" value=""/>
    <input type="hidden" name="act" value="visualizar"/>
</form>

<script type="text/javascript" src="../includes/tinymce_4.7/tinymce.min.js"></script>
<script type="text/javascript">
    var posicaoCursorDescricao = {
        inicial: 0,
        final: 0
    };
    $(function () {
        $(".modelo-documento-form").validateForm();

        $('.btn-voltar').click(function () {
            window.location = '?modulo=<?= $moduloLista ?>&acao=C<?= $pUrlParam ?>';
        });

        $('#descricao_format').focusout(function () {
            posicaoCursorDescricao.inicial = $(this).prop("selectionStart");
            posicaoCursorDescricao.final = $(this).prop("selectionEnd");
        });

        $('.modelo-documento-form  [name=tipo_prontuario_id]').change(gerenciarCampoFormatacao).change();
        $('.btn-print').click(function () {
            var formPrint = $('.modelo-documento-form-print');
            formPrint.find('[name=descricao]').val(gerenciarCampoFormatacao());
            formPrint.find('[name=tipo_prontuario_id]').val($('.modelo-documento-form  [name=tipo_prontuario_id]').val());
            formPrint.submit();
        });
    });

    function gerenciarCampoFormatacao() {
        var tiposSemFormatacao = <?= json_encode($tiposSemFormatacao) ?>;
        var tipoProntuarioId = $('.modelo-documento-form [name=tipo_prontuario_id]').val();

        if (tiposSemFormatacao.includes(tipoProntuarioId)) {
            $('.dsc-no-format').show().find('textarea').prop('required', true);
            $('.dsc-format').hide().find('textarea').prop('required', false);

            return $('[name=descricao_noformat]').val();
        } else {
            $('.dsc-format').show().find('textarea').prop('required', true);
            $('.dsc-no-format').hide().find('textarea').prop('required', false);

            return $('[name=descricao_format]').val();
        }
    }

    function setFocusVariavelCampoDescricao() {
        var input = document.getElementById("descricao_format");
        input.setSelectionRange(posicaoCursorDescricao.inicial, posicaoCursorDescricao.inicial);
        input.focus();
    }

    function adicionarVariavel(variavel) {
        var campoDescricao = $('[name^=descricao_]:visible');
        var valorCampo = campoDescricao.val();
        var strVariavel = '[' + variavel + ']';

        valorCampo = valorCampo.substr(0, posicaoCursorDescricao.inicial) + strVariavel + valorCampo.substr(posicaoCursorDescricao.final);
        campoDescricao.val(valorCampo);

        posicaoCursorDescricao.inicial = posicaoCursorDescricao.inicial + strVariavel.length;
        posicaoCursorDescricao.final = posicaoCursorDescricao.inicial;

        setFocusVariavelCampoDescricao();
    }
</script>
