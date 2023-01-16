<?php
$mEstabelecimento = new Estabelecimento();
$mAnexoEstabelecimento = new AnexoEstabelecimento();
$params = getParam();
if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar':
            try {
                $params = array_merge($_POST, getParam());
                $id = $mEstabelecimento->salvarEstabelecimento($params, $_FILES);
                $mEstabelecimento->commit();
                $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $id)));
            } catch (CnpjIndisponivel $e) {
                addMsgDanger($e->getMessage());
                extract($_POST);
            }
            break;
        case 'carregarMunicipios':
            require APPRAIZ . "adm/modulos/principal/includes/endereco/modal.inc";
            break;
        case 'verificarCnpj':
            if ($params['id']) {
                $mEstabelecimento->carregarPorId($params['id']);
                $_REQUEST['p'] = setParam(array('id' => $mEstabelecimento->pessoajuridica_id), false);
            }
            require APPRAIZ . "adm/modulos/principal/includes/pessoajuridica/formulario.inc";
            break;
        case 'salvar-documento':
            $params = getParam();
            $_POST['estabelecimento_id'] = $params ['id'];
            $id = $mAnexoEstabelecimento->salvarAnexo($_POST, $_FILES);
            $mAnexoEstabelecimento->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $id)));
            break;
    }
}

$aba = false;

$enderecos = null;
if (isset($params['id']) && !empty($params['id'])) {
    UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($params['id']);
    $aba = true;
    $dados = $mEstabelecimento->getTodosDados($params['id']);
    extract($dados);
}

include APPRAIZ . "includes/cabecalhom.inc";

if ($aba) {
    echo Estabelecimento::gerarCabecalho();
}
?>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-briefcase font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do cliente
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="estabelecimento-form" method="POST" enctype="multipart/form-data">
            <input type="hidden" id="remove_logo" name="remove_logo" />
            <div class="form-body">
                <?php require APPRAIZ . "adm/modulos/principal/includes/pessoajuridica/formulario.inc"; ?>

                <div class="row">
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Seguimento</label>
                            <?php
                            $mTipoEstabelecimento = new TipoEstabelecimento();
                            $tiposEstabelecimento = $mTipoEstabelecimento->recuperarTodos('id AS codigo, descricao', array("excluido IS FALSE"));
                            $db->monta_combom("tipo_estabelecimento_id", $tiposEstabelecimento, 'S', 'Selecione...', '', '', '', '', 'S');
                            ?>
                        </div>
                    </div>

                    <div class="col-md-8">
                        <?php require APPRAIZ . "adm/modulos/principal/includes/contato/formularioEstabelecimento.inc"; ?>
                    </div>
                </div>

                <h4 class="form-section">Documentos da Empresa</h4>

                <div class="row">
                    <div class="col-md-12">
                        <a class="btn green btn-incluir-documento">
                            <i class="fa fa-plus-circle"></i>
                            Incluir Documento
                        </a>
                        <br />
                        <br />
                        <?php

                        $arConfig = array();
                        $arWhere[] = " ane.estabelecimento_id = {$dados['id']}";
                        $acao = <<<ACAO
                        <div style="min-width: 20px;">
                            <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirAnexo({id});" data-toggle="tooltip"  title="Excluir">
                                <i class="icon-trash"></i>
                            </a>
                        </div>
ACAO;                        
                      
                        $rs = $mAnexoEstabelecimento->getList($arWhere);
                        $mAnexoEstabelecimento = new Listam(array("style" => "width: 100%;"));
                        $mAnexoEstabelecimento->setCabecalho(array(
                            'Nome',
                            'Arquivo',
                            'Tipo',
                            'Validade',                            
                            'Detalhade',
                            'Ação'
                        ));
                        $mAnexoEstabelecimento->setCorpo($rs, $arConfig);
                        $mAnexoEstabelecimento->setAcao($acao);
                        $mAnexoEstabelecimento->show();

                        ?>
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

                <input type="hidden" name="act" value="salvar" />
        </form>
    </div>
</div>

<div class="modal fade in" id="modal-documento" tabindex="-1" role="modal-documento" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <form role="form" name="documento-form" class="documento-form" method="POST" enctype='multipart/form-data'>
                <input type="hidden" id="act" name="act" value="salvar-documento" />
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Documento do estabelecimento</h4>
                </div>
                <div class="modal-body">
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Nome</label>
                                <?= campo_textom('nome', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Tipo</label>
                                <br />
                                <?php
                                $opcoes = array(
                                    array('codigo' => 'Habilitação Jurídica/Financeira', 'descricao' => 'Habilitação Jurídica/Financeira'),
                                    array('codigo' => 'Habilitação Técnica', 'descricao' => 'Habilitação Técnica'),
                                    array('codigo' => 'Contratação', 'descricao' => 'Contratação'),
                                    array('codigo' => 'Modelo de Documento', 'descricao' => 'Modelo de Documento'),
                                );
                                echo $db->monta_radiom('tipo', $opcoes, 'S', 'S', 'Habilitação Jurídica');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Validade</label>
                                <?= campo_datam('validade', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Texto Detalhe</label>
                                <br />
                                <?= campo_textaream('detalhe', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Arquivo</label>
                                <br />
                                <input type="file" id="arquivo" name="arquivo" class="form-control" />
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="submit" class="btn blue">
                        <i class="fa fa-plus-circle"></i>
                        Incluir
                    </button>
                    <button type="button" class="btn" data-dismiss="modal">Cancelar</button>
                </div>
                <input type="hidden" id="id" name="id" />
                <input type="hidden" id="id" name="id" />
            </form>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function() {
        $(".estabelecimento-form").validateForm({
            rules: {
                cnpj: {
                    cnpj: true
                },
                foto: {
                    accept: "image/*"
                },
                "contatos[<?= TipoContato::TIPO_EMAIL ?>][contato]": {
                    email: true
                }
            },
            submitHandler: function(e) {
                var submit = this;
                verificarCnpj($('#cnpj').val(), function() {
                    submit.submitHandlerFile(e, $('#file-logo'));
                });
            },
            beforeSubmitForm: function(e) {
                const remove_logo = localStorage.getItem('remove_logo');
                if (remove_logo === 'S') {
                    $('#remove_logo').val('S');
                }
            }
        });


        $('.btn-voltar').click(function() {
            window.location = '?modulo=principal/estabelecimento/lista&acao=C';
        });

        $('.btn-incluir-contato').click(function() {
            $('#modal-representante').modal({
                backdrop: 'static',
            }).modal('show');
        });
        $('.btn-incluir-documento').click(function() {
            $('#modal-documento').modal({
                backdrop: 'static',
            }).modal('show');
        });

        <?php if ($representantes) : ?>
            $('#tabela-representantes').bootstrapTable('load', <?= $representantes ?>);
        <?php endif; ?>
    });


</script>