<?php
$permissaoTela = 'S';
$mContrato = new Contrato();
$params = getParam();
$contratoId = isset($params['contrato_id']) ? $params['contrato_id'] : null;

if ($_POST['act']) {
    switch ($_POST['act']) {
        case 'salvar' :
            $contratoId = $mContrato->salvarContrato($_POST);
            $db->commit();
            $db->sucesso('financeiro/contrato/formulario&acao=C', setParam(array('contrato_id' => $contratoId)));
        case 'getDadosPessoaJuridica' :
            $mPessoaJuridica = new Estabelecimento();
            $dadosPessoaJuridica = $mPessoaJuridica->getByCnpj($_POST['cnpj']);
            die(json_encode(utf8ArrayEncode($dadosPessoaJuridica)));
        case 'getValorContratado' :
            $mTipoContrato = new TipoContrato();
            $mTipoContrato->recuperarLinha("*", array("id = '{$_POST['tipoContratoId']}'"));
            $dadosTipoContrato = $mTipoContrato->getDados();
            die(json_encode(utf8ArrayEncode($dadosTipoContrato)));
        case 'getPrazoContrato' :
            $mPeriodoContrato = new PeriodoContrato();
            $mPeriodoContrato->recuperarLinha("*", array("id = '{$_POST['periodoContratoId']}'"));
            $dadosPeriodoContrato = $mPeriodoContrato->getDados();
            die(json_encode(utf8ArrayEncode($dadosPeriodoContrato)));
        case 'buscarPessoaJuridica' :
        case 'filtrarEstabelecimento' :
            include_once 'buscaEstabelecimento.inc';
            die();
            break;
        case 'validarPeriodoContrato' :
            $possuiContratoPeriodo = $mContrato->isPossuiContratoParaPessoaJuridicaNoPeriodo(
                $_POST['estabelecimento_id'], $_POST['inicio_vigencia'], $_POST['fim_vigencia'], $_POST['id']);

            if (!$possuiContratoPeriodo) {
                $db->sucessomAjax();
            } else {
                $db->insucessomAjax('', '', 'Já existe um contrato em vigência no mesmo período para o estabelecimento');
            }
            break;
        case 'carregarMunicipios':
            $uf_sigla = $_POST['uf_sigla'];
            $municipio_id = $_POST['municipio_id'];
            $munHabilitado = 'N';
            $municipios = array();

            if ($uf_sigla) {
                $munHabilitado = $contratoId ? 'N' : 'S';

                $mUnidadeFederacao = new UnidadeFederacao();
                $ufId = $mUnidadeFederacao->getIdBySigla($uf_sigla);

                $mMunicipio = new Municipio();
                $municipios = $mMunicipio->recuperarTodos('id AS codigo, municipio AS descricao', array("uf_id = '{$ufId}'"), 2);
            }

            $db->monta_combo("municipio_id", $municipios, $munHabilitado, 'Selecione...', '', '', '', '', 'S');
            die;
            break;
    }
}

if (!empty($contratoId)) {
    $dados = $mContrato->getTodosDadosPorId($contratoId);
    $permissaoTela = 'N';
    extract($dados);
}

include APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
if (!empty($contratoId)) {
    $codFuncFormularioFatura = getMnuidPorMnulink("seguranca.php?modulo=financeiro/fatura/formulario&acao=C");
    $db->cria_aba($abacod_tela, $url, setParam(array('contratoId' => $contratoId)), array($codFuncFormularioFatura));
}
monta_titulo($titulo_modulo, '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica os campos obrigatórios.');
?>

<form id="formulario" name="formulario" method="post">
    <input type="hidden" id="act" name="act" value="salvar"/>
    <input type="hidden" id="id" name="id" value="<?= $id ?>"/>
    <input type="hidden" id="estabelecimento_id" name="estabelecimento_id"/>
    <input type="hidden" id="pessoajuridica_id" name="pessoajuridica_id" value="<?= $pessoajuridica_id ?>"/>
    <input type="hidden" id="razao_social" name="razao_social" value="<?= $razao_social ?>"/>
    <input type="hidden" id="nome_fantasia" name="nome_fantasia" value="<?= $nome_fantasia ?>"/>
    <input type="hidden" id="periodo_contrato" name="periodo_contrato"/>
    <input type="hidden" id="endereco_pessoajuridica_id" name="endereco_pessoajuridica_id" value="<?= $endereco_pessoajuridica_id ?>"/>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <td colspan="2" class="SubTituloDireita" style="text-align: center; font-weight: bold;">
                1. Dados do Contratante
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                CNPJ:
            </td>
            <td>
                <?= campo_texto('cnpj', 'S', $permissaoTela, '', 50, 100, '', ''); ?>
                <?php if (!$contratoId): ?>
                    <a href="javascript:buscarPessoaJuridica()" title="Buscar Pessoa Jurídica">
                        <img src="/imagens/consultar.gif">
                    </a>
                <?php endif; ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Razão Social:
            </td>
            <td>
                <label id="razao_social_lbl">Digite o CNPJ</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Nome Fantasia:
            </td>
            <td>
                <label id="nome_fantasia_lbl">Digite o CNPJ</label>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloDireita" style="text-align: center; font-weight: bold;">
                2. Dados de Contato
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Email:
            </td>
            <td>
                <?= campo_texto('contato_email', 'S', $permissaoTela, '', 35, 80, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                CEP:
            </td>
            <td>
                <?= campo_texto('cep', 'S', $permissaoTela, '', 10, 10, '##.###-###', '', '', '', 0); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Estado:
            </td>
            <td>
                <?php
                $mUnidadeFederacao = new UnidadeFederacao();
                $estados = $mUnidadeFederacao->recuperarTodos('uf_sigla AS codigo, uf_nome_estado AS descricao', array(), 2);
                $db->monta_combo("uf_sigla", $estados, $permissaoTela, 'Selecione...', '', '', '', '', 'S');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Cidade:
            </td>
            <td id="cidades">
                <?php
                $munHabilitado = 'N';
                $municipios = array();
                $db->monta_combo("municipio_id", $municipios, $munHabilitado, 'Selecione...', '', '', '', '', 'S');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Logradouro:
            </td>
            <td>
                <?= campo_texto('logradouro', 'S', $permissaoTela, '', 50, 200, '', '', '', '', 0); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Bairro:
            </td>
            <td>
                <?= campo_texto('bairro', 'S', $permissaoTela, '', 50, 200, '', '', '', '', 0); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Número/Complemento:
            </td>
            <td>
                <?= campo_texto('complemento', 'S', $permissaoTela, '', 50, 200, '', '', '', '', 0); ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloDireita" style="text-align: center; font-weight: bold;">
                3. Dados do Contrato
            </td>
        </tr>
        <?php if ($numero): ?>
            <tr>
                <td class="SubTituloDireita">
                    Número do Contrato:
                </td>
                <td>
                    <label><?= $numero ?></label>
                </td>
            </tr>
        <?php endif; ?>
        <tr>
            <td class="SubTituloDireita">
                Tipo de Contrato:
            </td>
            <td>
                <?php
                $mTipoContrato = new TipoContrato();
                $arTipoContrato = $mTipoContrato->recuperarTodos('id AS codigo, nome AS descricao', array("excluido IS FALSE"), 1);
                $db->monta_combo('tipo_contrato_id', $arTipoContrato, $permissaoTela, 'Selecione...', 'getValorContratado();', '', '', '350', 'S');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Período de Contrato:
            </td>
            <td>
                <?php
                $mPeriodoContrato = new PeriodoContrato();
                $arPeriodoContrato = $mPeriodoContrato->recuperarTodos('id AS codigo, nome AS descricao', array("excluido IS FALSE"), 1);
                $db->monta_combo('periodo_contrato_id', $arPeriodoContrato, $permissaoTela, 'Selecione...', 'getPrazoContrato();', '', '', '350', 'S');
                ?>
            </td>
        </tr>
        <tr class="campo-tipo-contrato">
            <td class="SubTituloDireita" id="valor_contratado_lbl">
                Valor Contratado (R$):
            </td>
            <td>
                <?= campo_texto('valor_contratado', 'S', $permissaoTela, '', 20, 16, '', ''); ?>
            </td>
        </tr>
        <tr class="campo-valor-parcela">
            <td class="SubTituloDireita">
                Quantidade de Parcelas (em meses):
            </td>
            <td>
                <?= campo_texto('quantidade_parcelas', 'S', $permissaoTela, '', 10, 3, '', ''); ?>
            </td>
        </tr>
        <tr class="campo-valor-parcela">
            <td class="SubTituloDireita">
                Valor da Parcela (R$):
            </td>
            <td>
                <label id="valor_parcela">Selecione o Período do Contrato</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data Inicial da Vigência:
            </td>
            <td>
                <?= campo_data2('inicio_vigencia', 'S', $permissaoTela, '', 'S', '', '', null, 'setDataFinalVigencia();'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data Final da Vigência:
            </td>
            <td>
                <?= campo_data2('fim_vigencia', 'S', 'N', '', 'S'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Dia do Pagamento:
            </td>
            <td>
                <?php
                $db->monta_combo('dia_pagamento', Contrato::getComboDataDePagamento(), $permissaoTela, 'Selecione...', '', '', '', '', 'S');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloEsquerda">&nbsp;</td>
            <td class="subtituloEsquerda" colspan="2">
                <?php if ($permissaoTela == 'S'): ?>
                    <input type="button" value="Salvar" id="btn-salvar" onclick="enviarFormulario();"/>
                <?php endif; ?>
                <input type="button" value="Voltar" id="btn-voltar"
                       onclick="window.location = '?modulo=financeiro/contrato/lista&acao=C';"/>
            </td>
        </tr>
    </table>
</form>

<!-- Modal -->
<div class="modal fade" id="modal-estabelecimento" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document" style="width: 70%;">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
                <h4 class="modal-title" id="myModalLabel">Busca de Estabelecimento</h4>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<link rel="stylesheet" href="../includes/bootstrap/3.0.0/css/bootstrap.min-simec.css">
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css?v=<?= VERSAO_SISTEMA; ?>" type="text/css"
      rel="stylesheet"></link>
<script type="text/javascript"
        src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js?v=<?= VERSAO_SISTEMA; ?>"></script>
<script type="text/javascript" src="../includes/moment/moment.min.js?v=<?= VERSAO_SISTEMA; ?>"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script type="text/javascript" src="../includes/bootstrap/3.0.0/js/bootstrap.min.js"></script>
<script type="text/javascript" src="../includes/jquery-mask/jquery-mask-1.13.3.js"></script>
<script type="text/javascript">
    var tipoContratoPorValorFixo = "<?= TipoContrato::TIPO_POR_VALOR_FIXO ?>";
    var tipoContratoPorAtendimento = "<?= TipoContrato::TIPO_POR_ATENDIMENTO_REALIZADO ?>";

    $(function () {
        $("#cnpj").mask('00.000.000/0000-00', {reverse: true});
        $('#valor_contratado').mask("#.##0,00", {reverse: true});
        $('#quantidade_parcelas').mask("###", {reverse: true});

        $("#cnpj").change(function () {
            $("#estabelecimento_id").val('');
            $("#razao_social").val('Digite o CNPJ');
            $("#nome_fantasia").val('Digite o CNPJ');

            if ($(this).val().length === 18) {
                getDadosPessoaJuridica();
            } else {
                setDadosLabelCnpj();
            }
        });

        $("#valor_contratado").change(function () {
            setValorParcela();
        });

        $("#quantidade_parcelas").change(function () {
            setValorParcela();
        });

        $('[name=cep]').change(function () {
            recuperarEnderecoPorCep();
        });

        $('select[name=uf_sigla]').change(function () {
            carregarMunicipio();
        });

        setDadosLabelCnpj();
        carregarMunicipio('<?= $municipio_id ?>');
        showCampoValorParcela();
        setLabelValorContratado();
        setValorParcela();
    });

    function setDadosLabelCnpj() {
        var razaoSocial = $("#razao_social").val();
        var nomeFantasia = $("#nome_fantasia").val();

        if (razaoSocial != "") {
            $("#razao_social_lbl").html(razaoSocial);
        }

        if (nomeFantasia != "") {
            $("#nome_fantasia_lbl").html(nomeFantasia);
        }
    }

    function showCamposTipoContrato() {
        var tipoContrato = $("[name=tipo_contrato_id]").val();
        var camposTipoContrato = $(".campo-tipo-contrato");

        camposTipoContrato.hide();
        if (tipoContrato !== "") {
            camposTipoContrato.show();
        }
    }

    function showCampoValorParcela() {
        var tipoContrato = $("[name=tipo_contrato_id]").val();
        var campoValorParcela = $(".campo-valor-parcela");

        campoValorParcela.hide();
        if (tipoContrato !== "" && tipoContrato == tipoContratoPorValorFixo) {
            campoValorParcela.show();
        }
    }

    function setDataFinalVigencia() {
        var dataInicialVigencia = $("[name=inicio_vigencia]").val();
        var periodoContrato = $("#periodo_contrato").val();

        if (periodoContrato != '' && dataInicialVigencia != "") {
            var dataFinalVigencia = moment(dataInicialVigencia, "DD/MM/YYYY").add(periodoContrato, 'months').format('DD/MM/YYYY');
            $("[name=fim_vigencia]").val(dataFinalVigencia);
        }
    }

    function setLabelValorContratado() {
        var tipoContrato = $("[name=tipo_contrato_id]").val();
        showCamposTipoContrato();
        var lblValorContratado = "Valor Contratado (R$):";

        switch (tipoContrato) {
            case tipoContratoPorAtendimento:
                lblValorContratado = "Valor por Atendimento";
                break;
            case tipoContratoPorValorFixo:
                lblValorContratado = "Valor do Contrato";
                break;
        }

        lblValorContratado = lblValorContratado + ' (R$):';

        $("#valor_contratado_lbl").html(lblValorContratado);

        return lblValorContratado;
    }

    function getValorContratado() {
        setLabelValorContratado();
        var elemValorContratado = $("#valor_contratado");
        var tipoContrato = $("[name=tipo_contrato_id]").val();

        elemValorContratado.val(null);

        if (tipoContrato != "") {
            var params = {
                'act': 'getValorContratado',
                'tipoContratoId': tipoContrato
            };
            divCarregando();

            $.ajax({
                type: "POST",
                dataType: 'JSON',
                url: window.location.href,
                data: params,
                success: function (retorno) {
                    elemValorContratado.val(retorno.valor);
                    elemValorContratado.keyup();
                    setReadOnlyCampoQtdParcelas();
                    showCampoValorParcela();
                },
                complete: function () {
                    divCarregado();
                }
            });
        }
    }

    function setReadOnlyCampoQtdParcelas() {
        var tipoContrato = $("[name=tipo_contrato_id]").val();
        var elemQtdParcelas = $("#quantidade_parcelas");

        elemQtdParcelas.attr('readOnly', false);

        if (tipoContrato == tipoContratoPorAtendimento) {
            elemQtdParcelas.attr('readOnly', true);
        }
    }

    function setValorParcela() {
        $("#valor_parcela").html('Selecione o Período do Contrato');
        var vlrContratado = $("#valor_contratado").val();
        var qtdParcelas = $("#quantidade_parcelas").val();

        if (vlrContratado != "" && qtdParcelas != "") {
            vlrContratado = desformataValor(vlrContratado);
            var vlrParcela = parseFloat(vlrContratado) / qtdParcelas;
            $("#valor_parcela").html(formataValor(vlrParcela));
        }
    }

    function getPrazoContrato() {
        var periodoContrato = $("[name=periodo_contrato_id]").val();

        if (periodoContrato != "") {
            var params = {
                'act': 'getPrazoContrato',
                'periodoContratoId': periodoContrato
            };
            divCarregando();

            $.ajax({
                type: "POST",
                dataType: 'JSON',
                url: window.location.href,
                data: params,
                success: function (retorno) {
                    setReadOnlyCampoQtdParcelas();
                    $("#quantidade_parcelas").val(retorno.quantidade_parcelas);
                    $("#periodo_contrato").val(retorno.quantidade_parcelas);
                    setValorParcela();
                    setDataFinalVigencia();
                },
                complete: function () {
                    divCarregado();
                }
            });
        }
    }

    function getDadosPessoaJuridica() {
        var cnpj = $("#cnpj").val();
        var params = {
            'act': 'getDadosPessoaJuridica',
            'cnpj': cnpj
        };
        divCarregando();

        $.ajax({
            type: "POST",
            dataType: 'JSON',
            url: window.location.href,
            data: params,
            success: function (retorno) {
                var razaoSocial = "CNPJ não encontrado";
                var nomeFantasia = "CNPJ não encontrado";
                limparDadosEndereco();

                if (retorno.id !== undefined) {
                    $("#estabelecimento_id").val(retorno.id);
                    $("#pessoajuridica_id").val(retorno.pessoajuridica_id);
                    razaoSocial = retorno.razao_social;
                    nomeFantasia = retorno.nome_fantasia;

                    $('[name=contato_email]').val(retorno.contato_email);

                    $('[name=endereco_pessoajuridica_id]').val(retorno.endereco_pessoajuridica_id);
                    $('[name=cep]').val(retorno.cep);
                    $('[name=logradouro]').val(retorno.logradouro);
                    $('[name=bairro]').val(retorno.bairro);
                    $('[name=uf_sigla]').val(retorno.uf_sigla);
                    $('[name=complemento]').val(retorno.complemento);
                    carregarMunicipio(retorno.municipio_id);
                }

                $("#razao_social").val(razaoSocial);
                $("#nome_fantasia").val(nomeFantasia);

                setDadosLabelCnpj();
            },
            complete: function () {
                divCarregado();
            }
        });
    }

    function enviarFormulario(tipo) {
        if (!validaBranco(document.formulario.estabelecimento_id, 'CNPJ')) return;
        if (!validaBranco(document.formulario.contato_email, 'Email')) return;
        if (!validaBranco(document.formulario.cep, 'CEP')) return;
        if (!validaBranco(document.formulario.uf_sigla, 'Estado')) return;
        if (!validaBranco(document.formulario.municipio_id, 'Cidade')) return;
        if (!validaBranco(document.formulario.logradouro, 'Logradouro')) return;
        if (!validaBranco(document.formulario.bairro, 'Bairro')) return;
        if (!validaBranco(document.formulario.complemento, 'Número/Complemento')) return;
        if (!validaBranco(document.formulario.tipo_contrato_id, 'Tipo de Contrato')) return;
        if (!validaBranco(document.formulario.periodo_contrato_id, 'Período de Contrato')) return;
        if (!validaBranco(document.formulario.valor_contratado, setLabelValorContratado())) return;
        if (!validaBranco(document.formulario.quantidade_parcelas, 'Quantidade de Parcelas (em meses)')) return;
        if (!validaBranco(document.formulario.inicio_vigencia, 'Data Inicial da Vigência')) return;
        if (!validaBranco(document.formulario.fim_vigencia, 'Data Final da Vigência')) return;
        if (!validaBranco(document.formulario.dia_pagamento, 'Dia do Pagamento')) return;

        var id = $("#id").val();
        var inicioVigencia = $("#inicio_vigencia").val();
        var fimVigencia = $("#fim_vigencia").val();
        var estabelecimentoId = $("#estabelecimento_id").val()
        var params = {
            'act': 'validarPeriodoContrato',
            'id': id,
            'estabelecimento_id': estabelecimentoId,
            'inicio_vigencia': inicioVigencia,
            'fim_vigencia': fimVigencia
        };
        divCarregando();

        $.ajax({
            type: "POST",
            dataType: 'JSON',
            url: window.location.href,
            data: params,
            success: function (retorno) {
                if (retorno.codigo == 1) {
                    document.formulario.submit();
                } else {
                    divCarregado();
                    alert(retorno.msg);
                }
            }
        });


    }

    function buscarPessoaJuridica() {
        $('#modal-estabelecimento .modal-body').load(window.location.href, {act: 'buscarPessoaJuridica'}, function () {
            $('#modal-estabelecimento').modal('show');
        });
    }

    function recuperarEnderecoPorCep() {
        var campoCep = $('[name=cep]');
        var cep = campoCep[0];

        if (!cep || cep.value == '' || cep.value.replace(/[^0-9]/ig, '').length != 8) {
            limparDadosEndereco();
            return false;
        }

        divCarregando();
        $.ajax({
            type: "POST",
            url: "/geral/dne.php",
            data: {opt: 'dne', endcep: cep.value},
            success: function (retorno) {
                eval(retorno);

                if (DNE[0] && DNE.length >= 1) {
                    $('[name=logradouro]').val(DNE[0].logradouro);
                    $('[name=bairro]').val(DNE[0].bairro);
                    $('[name=uf_sigla]').val(DNE[0].estado);

                    carregarMunicipio(DNE[0].muncod);
                } else {
                    limparDadosEndereco();
                }
                divCarregado();
            },
            error: function (result) {
                divCarregado();
            },
        });
    }

    function carregarMunicipio(municipio_id) {
        var uf_sigla = $('[name=uf_sigla]').val();

        $('#cidades').load(window.location.href, {
            act: 'carregarMunicipios',
            uf_sigla: uf_sigla,
            municipio_id: municipio_id
        });
    }

    function limparDadosEndereco() {
        var form = $('[name=formulario]');

        $(form).find('[name=cep]').val('');
        $(form).find('[name=logradouro]').val('');
        $(form).find('[name=complemento]').val('');
        $(form).find('[name=bairro]').val('');
        $(form).find('[name=uf_sigla]').val('');
        $(form).find('[name=municipio_id]').val('');
        $(form).find('[name=endereco_pessoa_juridica_id]').val('');

        carregarMunicipio('');
    }

</script>


