<?php
$mAgenda = new EstabelecimentoProfissionalAgenda();
$mSolicitacaoCorrecao = new SolicitacaoCorrecaoAgenda();

$p = getParam();
$arParam = array('id' => $p['id'], 'agenda_id' => ($_REQUEST['agenda_id'] ? $_REQUEST['agenda_id'] : ''));

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'salvarSolicitacaoCorrecao' :
            try {
                $params = array_merge($_POST, getParam());
                $params = utf8ArrayDecode($params);

                $mSolicitacaoCorrecao->salvarSolicitacao(['descricao_solicitacao' => $params['descricao_solicitacao'],'agenda_id' => $params['agenda_id']]);
                $mSolicitacaoCorrecao->commit();

                $db->sucessomAjax($_REQUEST['modulo'], array(
                    'id' => $estabelecimentoId
                ));

            } catch (Exception $e) {
                $db->insucessomAjax($_REQUEST['modulo'], array(), $e->getMessage());
            }
            break;
        case 'cancelarSolicitacaoCorrecao' :
            try {
                $params = array_merge($_POST, getParam());
                $params = utf8ArrayDecode($params);

                $mSolicitacaoCorrecao->cancelarSolicitacao($params['solicitacao_correcao_id']);
                $mSolicitacaoCorrecao->commit();

                $db->sucessomAjax($_REQUEST['modulo'], array(
                    'id' => $estabelecimentoId
                ));

            } catch (Exception $e) {
                $db->insucessomAjax($_REQUEST['modulo'], array(), $e->getMessage());
            }

            break;
    }
}

$dadosAgenda = $mAgenda->getTodosDados($arParam['agenda_id']);
$dadosSolicitacao = $mSolicitacaoCorrecao->getSolicitacaoEmAberto($arParam['agenda_id']);

if($dadosSolicitacao){
    extract($dadosSolicitacao);
    $habilitado = 'N';
}else{
    $habilitado = 'S';
}

$permissaoSolicitacaoCorrecao = false;
$permissaoEfetuarCorrecao     = false;
if($dadosAgenda['wf_estado_id'] == EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA){
    $permissaoSolicitacaoCorrecao = true;
    if($arParam['agenda_id'] && possuiPerfil(ADM_PERFIL_CORRETOR_AGENDA) && $dadosSolicitacao){
        $permissaoEfetuarCorrecao = true;
    }
}


?>
<style type="text/css">
    .control-label .required, .form-group .required{
        color: #555;
        font-size: 14px;
    }
</style>

    <div id="agenda-formulario">
        <input type="hidden" name="p" value="<?= setParam($arParam,false); ?>" />
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
            <h4 class="modal-title">Solicitação de correção</h4>
        </div>
        <div class="modal-body">
            <label class="form-section bold">Descreva com detalhes a alteração necessária <i class="fa fa-asterisk" style="color: red; font-size: 6px; vertical-align: text-top;"></i></label>
            <div class="row">
                <div class="col-sm-12">
                    <div class="form-group">
                        <?= campo_textaream('descricao_solicitacao', 'S', $habilitado, '', '', '9', '2000', '', 0, '') ?>
                    </div>
                </div>
            </div>

        </div> <!-- row -->
    </div>
    <div class="modal-footer">
        <?php if($habilitado === 'S' && $permissaoSolicitacaoCorrecao) : ?>
            <button type="submit" class="btn blue btn-incluir">
                <i class="fa fa-save"></i>
                 Salvar
            </button>
        <?php endif; ?>
        <?php if($dadosSolicitacao && $dadosSolicitacao['finalizada'] == 'f' && $permissaoEfetuarCorrecao) : ?>
            <button class="btn blue" type="button" title="Alterar dados da consulta" onclick="corrigirDadosConsulta();">
                <i class="fa fa-edit"></i>
                Alterar dados da consulta
            </button>
            <button type="button" class="btn red btn-cancelar">
                <i class="fa fa-ban"></i>
                Cancelar
            </button>
        <?php endif; ?>
        <button type="button" class="btn" data-dismiss="modal">Fechar</button>
    </div>
    <input type="hidden" name='act' value="salvarSolicitacaoCorrecao" />
<script type="text/javascript">
    $(function(){
        var form = $("#form-solicitacao-correcao");

        adicionarLabelObrigatorio(form.find(':required, .obrigatorio'));

        form.validateForm({
            rules : {},
            submitHandler: function (e) {
                App.blockUI({
                    message: 'Carregando...',
                    target: '#form-solicitacao-correcao-content'
                });

                $('[name=act]').val('salvarSolicitacaoCorrecao');
                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    dataType : 'json',
                    data: {
                        act : form.find('[name=act]').val(),
                        descricao_solicitacao : form.find('[name=descricao_solicitacao]').val(),
                        p : form.find('[name=p]').val(),
                    },
                    success: function (retorno) {
                        if(retorno.codigo === 1){
                            App.unblockUI('#form-solicitacao-correcao-content');

                            fecharModalSolicitacaoCorrecao(retorno.msg);
                        }else{
                            this.error(retorno);
                        }
                    },
                    error: function (result) {
                        App.unblockUI('#form-solicitacao-correcao-content');
                        exibirAlert(result.msg ? result.msg : 'Não foi possível incluir a solicitação de correção de agenda');
                    }
                });

            }, function(){
                App.unblockUI('#form-solicitacao-correcao-content');
            }
        });

        form.find('.btn-cancelar').click(function () {
            $('[name=act]').val('cancelarSolicitacaoCorrecao');

            $.ajax({
                type: "POST",
                url: window.location.href,
                dataType : 'json',
                data: {
                    act : form.find('[name=act]').val(),
                    solicitacao_correcao_id : '<?= ($dadosSolicitacao && $dadosSolicitacao['id'] ? $dadosSolicitacao['id'] : '') ?>',
                },
                success: function (retorno) {
                    if(retorno.codigo === 1){
                        App.unblockUI('#form-solicitacao-correcao-content');

                        fecharModalSolicitacaoCorrecao(retorno.msg);
                    }else{
                        this.error(retorno);
                    }
                },
                error: function (result) {
                    App.unblockUI('#form-solicitacao-correcao-content');
                    exibirAlert(result.msg ? result.msg : 'Não foi possível cancelar a solicitação de correção de agenda');
                }
            });
        })
    });

    function fecharModalSolicitacaoCorrecao(msg){
        if(msg){
            exibirMsg(msg);
        }

        $('#modal-solicitacao-correcao').modal('hide');
        atualizarAgendaCalendario();
    }

    function corrigirDadosConsulta(){
        $('#modal-solicitacao-correcao').modal('hide');
        alterarDadosConsulta();
    }
</script>