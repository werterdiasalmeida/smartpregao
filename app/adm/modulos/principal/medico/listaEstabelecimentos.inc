<?php
include APPRAIZ . "includes/cabecalhom.inc";

$usucpfFormatado = formatar_cpf($_SESSION['usucpf']);
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mProfissional = new Profissional();
$estabelecimentos = $mProfissional->listaEstabelecimentosPorCpfProfissional($usucpfFormatado);

monta_titulom('Selecionar estabelecimento', 'Selecione o estabelecimento que deseja visualizar sua agenda');
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Estabelecimentos
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form method="post" name="form-estabelecimento">
            <input type="hidden" name="estabelecimento_id" />
        </form>
        <?php
        $acao = <<<HTML
                <div style="text-align: center;">
                    <a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:;" onclick="selecionarEstabelecimento('{codigo}');" data-toggle="tooltip" title="Selecionar estabelecimento">
                        <i class="fa fa-arrow-circle-left"></i>
                    </a>
                </div>
HTML;
        $arConfig[0]['style'] = "text-align: left;";
        $arConfig[0]['html'] = <<<HTML
            <a href="javascript:;" onclick="selecionarEstabelecimento('{codigo}');" data-toggle="tooltip" title="Selecionar estabelecimento">
                <img src="../slideshow/slideshow/verimagem.php?arqid={estabelecimento_logo}&newheight=50&newwidth=50" class="img-circle" />
                {descricao}
            </a>
HTML;

        $mLista = new Listam(array("style" => "width: 100%;"));
        $mLista->getTable()->set('data-search', 'true');
        $mLista->getTable()->set('data-toggle', 'table');
        $mLista->setCabecalho(array(
            'Estabelecimento',
            'Ação'
        ));
        $mLista->setCorpo( $estabelecimentos, $arConfig );
        $mLista->setAcao( $acao );
        $mLista->show();
        ?>
    </div>
</div>

<script type="application/javascript">
    function selecionarEstabelecimento(codigo){
        $('[name=estabelecimento_id]').val(codigo);
        $('[name=form-estabelecimento]').submit();
    }
</script>


