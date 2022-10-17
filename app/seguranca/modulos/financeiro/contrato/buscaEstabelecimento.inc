<?php
if ($_POST['act']) {
    switch ($_POST['act']) {
        case 'filtrarEstabelecimento' :
            $arWhere = array();
            $mEstabelecimento = new Estabelecimento();

            if(!empty($_REQUEST['razao_social'])){
                $razao_social = trim(addslashes($_REQUEST['razao_social']));
                $arWhere['razao_social'] = "pj.razao_social ILIKE '%{$razao_social}%' ";
            }

            if(!empty($_REQUEST['nome_fantasia'])){
                $nome_fantasia = trim(addslashes($_REQUEST['nome_fantasia']));
                $arWhere['nome_fantasia'] = "pj.nome_fantasia ILIKE '%{$nome_fantasia}%' ";
            }

            if(!empty($_REQUEST['cnpj'])){
                $cnpj = trim(addslashes($_REQUEST['cnpj']));
                $arWhere['cnpj'] = "pj.cnpj ILIKE '%{$cnpj}%' ";
            }

            $rsListaEnvio = $mEstabelecimento->listarComLimit($arWhere);

            $acao = <<<ACAO
            <div style="min-width: 20px;">
                <a href="javascript:selecionarCnpj('{cnpj}')" title="Selecionar Estabelecimento">
                     <img src="/imagens/check.png">
                </a>
            </div>
ACAO;

            $arParamCol = array();

            $arCabecalho = array(
                'Ação',
                'Razão social',
                'Nome fantasia',
                'CNPJ'
            );

            $oLista = new Lista();
            $oLista->setCabecalho($arCabecalho);
            $oLista->setCorpo($rsListaEnvio, $arParamCol);
            $oLista->setAcao($acao);
            $oLista->show();

            die();
    }
}
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <td class="SubTituloDireita">
            CNPJ:
        </td>
        <td>
            <?= campo_texto('cnpj', 'N', 'S', '', 50, 100, '', ''); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Razão Social:
        </td>
        <td>
            <?= campo_texto('razao_social', 'N', 'S', '', 50, 100, '', ''); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">
            Nome Fantasia:
        </td>
        <td>
            <?= campo_texto('nome_fantasia', 'N', 'S', '', 50, 100, '', ''); ?>
        </td>
    </tr>
    <tr>
        <td class="subtituloEsquerda">&nbsp;</td>
        <td class="subtituloEsquerda" colspan="2">
            <input type="button" id="btn-pesquisar" name="Pesquisar" value="Pesquisar"/>
        </td>
    </tr>
</table>
<div id="content-lista">
        <div style="text-align: center; padding: 10px; font-weight: bold;">
            Entre com o filtro desejado e clique em 'Pesquisar'.
        </div>
</div>

<script type="text/javascript">

    $(function () {
        $("#modal-estabelecimento #cnpj").mask('00.000.000/0000-00', {reverse: true});

        $("#modal-estabelecimento #btn-pesquisar").click(function () {
            var params = {
                act : 'filtrarEstabelecimento',
                cnpj: $("#modal-estabelecimento #cnpj").val(),
                razao_social: $("#modal-estabelecimento #razao_social").val(),
                nome_fantasia: $("#modal-estabelecimento #nome_fantasia").val()
            };

            $('#modal-estabelecimento #content-lista').load( window.location.href, params);
        });
    });

    function selecionarCnpj(cnpj) {
        $("#cnpj").val(cnpj);
        $("#cnpj").trigger('change');
        $('#modal-estabelecimento').modal('hide');
    }
</script>