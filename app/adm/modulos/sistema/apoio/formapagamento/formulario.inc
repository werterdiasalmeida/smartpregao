<?php
$mEntidade = new FormaPagamento();
$moduloLista = $abaEstabelecimento ? 'principal/estabelecimento/formapagamento/lista' : 'sistema/apoio/formapagamento/lista';

$params = getParam();
$pUrlParam = (isset($params['id']) && !empty($params['id']) ? setParam(array('id' => $params['id'])) : '');

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            $params = array_merge($_POST, getParam());
            $params['id'] = $params['forma_pagamento_id'];
            $mEntidade->salvarFormaPagamento($params);
            $mEntidade->commit();

            $db->sucessom($moduloLista, $pUrlParam);
            break;
    }
}

if (isset($params['forma_pagamento_id']) && !empty($params['forma_pagamento_id'])) {
    $mEntidade->carregarPorId($params['forma_pagamento_id']);
    $dados = $mEntidade->getDados();
    extract($dados);

    if(!$db->testa_superuser()){
        UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimento($estabelecimento_id);
    }
}

include APPRAIZ . "includes/cabecalhom.inc";

if($abaEstabelecimento){
    echo Estabelecimento::gerarCabecalho();
}


if($abaEstabelecimento){
    UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $params['id'])), null, array(), true, true);

} else {
    monta_titulom('Insira os dados da forma de pagamento');
}
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-check font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações da forma de pagamento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="forma-pagamento-form" method="POST">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Descrição</label>
                            <?= campo_textom('descricao', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>
                <div class="row">
                    <div class="col-md-12">
                        <div class="form-group">
                            <label>Estabelecimento</label>
                            <?php
                            $mEstabelecimento = new Estabelecimento();
                            $estabelecimentos = $mEstabelecimento->getComboPorResponsabilidade($abaEstabelecimento ? array("(est.id = {$params['id']} OR est.id IS NULL)") : array());
                            $opcaoSelecione = '';
                            $estabelecimentoObrigatorio = 'S';

                            if($db->testa_superuser()){
                                $opcaoSelecione = 'Padrão';
                                $estabelecimentoObrigatorio = 'N';
                            }

                            if(empty($id) && empty($estabelecimento_id)){
                                $estabelecimento_id = count($estabelecimentos) === 1 ? $estabelecimentos[0]['codigo'] : '';
                            }

                            $comboEstabelecimentoHabilitada = 'S';
                            $db->monta_combom("estabelecimento_id", $estabelecimentos, $comboEstabelecimentoHabilitada, $opcaoSelecione, '', '', '', '', $estabelecimentoObrigatorio);
                            ?>
                        </div>
                    </div>
                </div>
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
        $(".forma-pagamento-form").validateForm();

        $('.btn-voltar').click(function () {
            window.location = '?modulo=<?= $moduloLista ?>&acao=C<?= $pUrlParam ?>';
        });
    });
</script>
