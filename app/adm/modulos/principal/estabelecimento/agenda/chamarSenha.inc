<?php

$mPainelSenhaRegistro   = new PainelSenhaRegistro();
$mPainelSenha           = new PainelSenha();

if ($_REQUEST['act']) {
    switch ($_REQUEST['act']) {
        case 'excluirPainelSenha' :
            $parametros = getParam($_REQUEST['p']);
            $mPainelSenha->excluirLogicamenteByEstabelecimento($parametros['id']);
            $mPainelSenha->commit();
            $db->sucessomAjax($_REQUEST['modulo'], []);
            break;
        case 'criarPainelSenha' :
            $mPainelSenha->criarPainelSenha(array_merge($_POST, getParam()));
            $db->sucessomAjax($_REQUEST['modulo'], []);
            break;
        case 'criarNumeroSenha' :
            $mPainelSenhaRegistro->chamarSenha(array_merge($_POST, getParam()));
            $mPainelSenhaRegistro->commit();
            $db->sucessomAjax($_REQUEST['modulo'], []);
            break;
    }
}

$painel     = $mPainelSenha->getByEstabelecimento($p['id']);
$registros  = $mPainelSenhaRegistro->getListByEstabelecimento($p['id'],true);

$painelAtendimento = $mPainelSenha->getByEstabelecimento($p['id'],'A');

if ($painel['data_validade']) {
    $painel['data_validade'] = (new DateTime($painel['data_validade']))->format('d/m/Y H:i');
} else {
    $data = new DateTime();
    $painel['data_validade'] = $data->format('d/m/Y 23:59');
}

$urlSistema = $_SERVER['HTTP_ORIGIN'];
?>

<style type="text/css">
    .swal2-container {
        z-index: 99999;
    }
</style>

<div id="agenda-formulario">
    <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
        <h4 class="modal-title"><span class="fa fa-ticket"></span> Painel de senhas</h4>
    </div>

    <div class="modal-body">
        <input type="hidden" name="p-painel-senha" value="<?= setParam(array('id' => $p['id']), false) ?>">
        <input type="hidden" name="id-painel-senha" value="<?= $painel['id'] ?>">
        <div class="row">
            <?php if($painel['id']): ?>
                <div class="col-xs-12">
                    <h4><b>Painel recepção</b></h4>
                </div>
                <div class="col-xs-4">
                    <div class="form-group">
                        <label>Código</label>
                        <div class="paciente">
                            <?= campo_textom('codigo-painel-senha', 'N', 'N', '', 90, 90, '', '', '', '', 0, "", "", $painel['codigo'], '',  null, null,  null, '', true); ?>
                        </div>
                    </div>
                </div>
                <div class="col-xs-8">
                    <div class="form-group">
                        <label>Link</label>
                        <div class="paciente">
                            <div class="input-group">
                                <input type="text" class="form-control" id="link-painel-senha" readonly value="<?= ($urlSistema . "/senhas/index.php?p=" . $painel['codigo']) ?>">
                                <span class="input-group-btn" data-toggle='tooltip' data-placement="left" title="Copiar link">
                                    <button class="btn btn-default btn-copy" type="button" data-clipboard-target="#link-painel-senha">
                                        <span class="fa fa-copy"></span>
                                    </button>
                                </span>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="col-xs-12">
                    <h4><b>Painel atendimento</b></h4>
                </div>
                <div class="col-xs-4">
                    <div class="form-group">
                        <label>Código</label>
                        <div class="paciente">
                            <?= campo_textom('codigo-painel-senha-atendimento', 'N', 'N', '', 90, 90, '', '', '', '', 0, "", "", $painelAtendimento['codigo'], '',  null, null,  null, '', true); ?>
                        </div>
                    </div>
                </div>
                <div class="col-xs-8">
                    <div class="form-group">
                        <label>Link</label>
                        <div class="paciente">
                            <div class="input-group">
                                <input type="text" class="form-control" id="link-painel-senha-atendimento" readonly value="<?= ($urlSistema . "/senhas/index.php?p=" . $painelAtendimento['codigo']) ?>">
                                <span class="input-group-btn" data-toggle='tooltip' data-placement="left" title="Copiar link">
                                    <button class="btn btn-default btn-copy" type="button" data-clipboard-target="#link-painel-senha-atendimento">
                                        <span class="fa fa-copy"></span>
                                    </button>
                                </span>
                            </div>
                        </div>
                    </div>
                </div>
            <?php endif; ?>
            <div class="col-xs-12">
                <div class="form-group">
                    <label>Validade dos paineis</label>
                    <div class="paciente">
                        <?= campo_textom('validade-painel-senha', ($painel['id'] ? 'N' : 'S'), ($painel['id'] ? 'N' : 'S'), '', 90, 90, '', '', '', '', 0, "", "", $painel['data_validade'], '',  null, null,  null, '', true); ?>
                    </div>
                </div>
            </div>

            <? if($painel['id']): ?>
                <div class="col-xs-6">
                    <div class="form-group">
                        <label>Número</label>
                        <div class="paciente">
                            <?= campo_textom('numero-painel-senha', 'S', 'S', '', 90, 10, '', '', '', '', 0, "", "", '', '',  null, null,  null, '', true); ?>
                        </div>
                    </div>
                </div>
                <div class="col-xs-6">
                    <div class="form-group">
                        <label>Guichê</label>
                        <div class="paciente">
                            <?= campo_textom('guiche-painel-senha', 'S', 'S', '', 90, 90, '9999999', '', '', '', 0, "", "",'', '',  null, null,  null, '', true); ?>
                        </div>
                    </div>
                </div>
            <?php endif; ?>
        </div>

        <?php if($painel['id']): ?>
            <h3>Lista de números chamados</h3>
            <table id="tabela-senhas-chamadas"
                   data-toggle="table"
                   class="table table-striped"
                   data-unique-id="painel_senha_registro_id"
                   data-pagination='true'
                   data-side-pagination="client"
                   data-page-size='5'
                   data-search='false'>
                <thead>
                <tr>
                    <th data-field="responsavel">Responsável</th>
                    <th data-field="data" data-align="center">Data e hora</th>
                    <th data-field="numero" data-align="center">Número</th>
                    <th data-field="guiche" data-align="center">Guichê</th>
                </tr>
                </thead>
                <tbody>

                </tbody>
            </table>
        <?php endif; ?>
    </div>
    <div class="modal-footer">
        <?php if($painel['id']): ?>
            <button type="button" class="btn red btn-excluir-painel-senha" style="float: left"><i class="fa fa-remove"></i> Excluir painel</button>
        <?php endif; ?>
        <button type="submit" class="btn blue btn-chamar-senha">
            <i class="fa fa-ticket"></i>
            <?php if($painel['id']): ?>
                Chamar senha
            <?php else: ?>
                Criar painel
            <?php endif; ?>
        </button>
        <button type="button" class="btn" data-dismiss="modal">Fechar</button>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        <?php if($painel['id']): ?>
          $('#tabela-senhas-chamadas').bootstrapTable({
            data : <?= $registros; ?>
          });
        <?php endif; ?>

        $('#validade-painel-senha').datetimepicker({locale: 'pt-br', format: 'DD/MM/YYYY HH:ss'});

        var form = $("#form-chamar-senha");
        adicionarLabelObrigatorio(form.find(':required, .obrigatorio'));
        form.validateForm({
          rules : {},
          submitHandler: function (e) {
            App.blockUI({
              message: 'Carregando...',
              target: "#form-chamar-senha"
            });

            let dataParam = {
                p   : form.find('[name=p-painel-senha]').val(),
            };

            if ($('[name=id-painel-senha]').val() != '') {
              dataParam.act               = 'criarNumeroSenha';
              dataParam.painel_senha_id   = form.find('[name=id-painel-senha]').val();
              dataParam.numero            = form.find('[name=numero-painel-senha]').val();
              dataParam.guiche            = form.find('[name=guiche-painel-senha]').val();
            } else {
              dataParam.act               = 'criarPainelSenha';
              dataParam.painel_senha_id   = form.find('[name=id-painel-senha]').val();
              dataParam.codigo            = form.find('[name=codigo-painel-senha]').val();
              dataParam.validade          = form.find('[name=validade-painel-senha]').val();
            }

            $.ajax({
              type: "POST",
              url: '?modulo=principal/estabelecimento/agenda&acao=C',
              dataType : 'json',
              data: dataParam,
              success: function (retorno) {
                if(retorno.codigo === 1){
                  App.unblockUI('#form-chamar-senha');
                  exibirSucesso(retorno.msg ? retorno.msg : 'Operação realizada com sucesso!');

                    <?php if($painel['id']): ?>
                        $('#modal-chamar-senha').modal('hide');
                    <?php else: ?>
                        chamarSenha("&p=" + form.find('[name=p-painel-senha]').val());
                    <?php endif; ?>

                }else{
                  this.error(retorno);
                }
              },
              error: function (result) {
                App.unblockUI('#form-chamar-senha');
                exibirAlert(result.msg ? result.msg : 'Não foi possível executar a operação');
              }
            });

          }, function(){
            App.unblockUI("#form-chamar-senha");
          }
        });

        // Recuperando o ultimo registro pra preencher
        let atual = (<?= $registros; ?>).shift();
        if (atual && atual.numero) {
            if (Number.isInteger(parseInt(atual.numero))) {
              $('#numero-painel-senha').val(parseInt(atual.numero) + 1);
            }
        }

        $('.btn-excluir-painel-senha').click(function(){
          Swal.fire({
            title: 'Deseja realmente excluir o painel de senhas?',
            text: "Essa ação não poderá ser revertida!",
            icon: 'warning',
            showCancelButton: true,
            confirmButtonColor: '#3085d6',
            cancelButtonColor: '#d33',
            confirmButtonText: 'Sim',
            cancelButtonText: 'Não'
          }).then((result) => {
            if(result.isConfirmed) {
              $('#modal-chamar-senha').modal('hide');
              $.ajax({
                type: "POST",
                url: '?modulo=principal/estabelecimento/agenda&acao=C',
                dataType : 'json',
                data: {
                  act: 'excluirPainelSenha',
                  p   : form.find('[name=p-painel-senha]').val(),
                  painel_senha_id: $('[name=id-painel-senha]').val()
                },
                success: function (retorno) {
                  if(retorno.codigo === 1){
                    exibirSucesso(retorno.msg ? retorno.msg : 'Operação realizada com sucesso!');
                  }else{
                    this.error(retorno);
                  }
                },
                error: function (result) {
                  exibirAlert(result.msg ? result.msg : 'Não foi possível executar a operação');
                }
              });
            }
          });
        });

      new ClipboardJS('.btn-copy');
      $('[data-toggle="tooltip"]').tooltip();
    });
</script>