<?php
$mConvenio = new ConvenioEstabelecimento();

$p = getParam();
UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            $params['id'] = $params['convenio_id'];

            $mConvenio->salvarConvenio($params);
            $mConvenio->commit();

            $db->sucessom('principal/estabelecimento/convenio', setParam(array('id' => $p['id'])));
            break;
    }
}

if (isset($p['convenio_id']) && !empty($p['convenio_id'])) {
    $mConvenio->carregarPorId($p['convenio_id']);
    extract($mConvenio->getDados());

    $mContato = new ContatoConvenio();
    $contatos = $mContato->listarEspecificosPorConvenio($p['convenio_id']);
    extract($contatos);
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();
monta_titulom('Insira os dados do convênio/plano');

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true, true);
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-building-o font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do convênio/plano
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="convenio-form" method="POST" enctype="multipart/form-data">
            <div class="form-body">

                <div class="row">
                    <div class="col-md-6">
                        <div class="form-group">
                            <label>Nome</label>
                            <?= campo_textom('nome', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                        </div>
                    </div>
                    <div class="col-md-3">
                        <div class="form-group">
                            <label>CNPJ</label>
                            <?= campo_textom('cnpj', 'N', 'S', '', 19, 19, '99.999.999/9999-99', '', '', '', 0); ?>
                        </div>
                    </div>
                    <div class="col-md-3">
                        <div class="form-group">
                            <label>Código na operadora</label>
                            <?= campo_textom('codigo_operadora', 'N', 'S', '', 20, 20, '', '', '', '', 0); ?>
                        </div>
                    </div>
                </div>

                <div class="row">
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Telefone principal</label>
                            <div class="input-group">
                                <span class="input-group-addon">
                                    <i class="fa fa-phone-square"></i>
                                </span>
                                <?= campo_textom("contatos[principal][contato]", 'N', 'S', '', 19, 19, '', '', '', '', 0, null, null, $contato_principal, null, null, 'contato_principal'); ?>
                                <input type="hidden" name="contatos[principal][tipo_contato_sigla]" value="principal" />
                            </div>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Telefone secundário</label>
                            <div class="input-group">
                                <span class="input-group-addon">
                                    <i class="fa fa-phone-square"></i>
                                </span>
                                <?= campo_textom("contatos[".TipoContato::TIPO_TELEFONE_FIXO."][contato]", 'N', 'S', '', 19, 19, '', '', '', '', 0, null, null, $contato_telefone, null, null, 'contato_telefone'); ?>
                                <input type="hidden" name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][tipo_contato_sigla]" value="<?= TipoContato::TIPO_TELEFONE_FIXO ?>" />
                            </div>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Email</label>
                            <div class="input-group">
                                <span class="input-group-addon">
                                    <i class="fa fa-envelope"></i>
                                </span>
                                <?= campo_textom("contatos[".TipoContato::TIPO_EMAIL."][contato]", 'N', 'S', '', 70, 70, '', '', '', '', 0, null, null, $contato_email, null, null, 'contato_email'); ?>
                                <input type="hidden" name="contatos[<?= TipoContato::TIPO_EMAIL ?>][tipo_contato_sigla]" value="<?= TipoContato::TIPO_EMAIL ?>" />
                            </div>
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
        $(".convenio-form").validateForm({
            rules : {
                cnpj: {cnpj: true},
                "contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]" : {email : true}
            }
        });

        $("#contato_principal, #contato_telefone").inputmask({
            mask: ["(99) 9999-9999", "(99) 99999-9999"],
        });

        $('.btn-voltar').click(function () {
            window.location = '?modulo=principal/estabelecimento/convenio&acao=C<?= setParam(array('id' => $p['id'])) ?>';
        });
    });
</script>