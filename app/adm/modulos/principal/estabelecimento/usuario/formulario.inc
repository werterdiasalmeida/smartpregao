<?php
$p = getParam();
$usucpf = isset($p['usucpf']) ? $p['usucpf'] : '';
$mUsuario = new UsuarioEstabelecimento();
$mPerfil = new Perfil();
$alteracao = false;
$validacao = false;
$perfisVinculados = array();

$p = getParam();

if (!isset($p['id'])) {
    addMsgInfo('Por favor, selecione um estabelecimento.');
    header('Location: ?modulo=principal/estabelecimento/lista&acao=C');
    die;
}

UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);

if(isset($_REQUEST['act']) && !empty($_REQUEST['act'])){
    switch ($_REQUEST['act']){
        case 'salvar':
            if($_REQUEST['alteracao'] === 'f' && $mUsuario->isCpfCadastrado($_REQUEST['usucpf'])){
                unset($_REQUEST['p']);
                addMsgDanger('O CPF informado já existe!');
                extract($_REQUEST);
                $perfisVinculados = $_REQUEST['perfil'];
                $validacao = true;
            } else if($mUsuario->isEmailCadastrado($_REQUEST['usuemail'], $_REQUEST['usucpf'])){
                unset($_REQUEST['p']);
                addMsgDanger('O email informado já existe!');
                extract($_REQUEST);
                $perfisVinculados = $_REQUEST['perfil'];
                $validacao = true;
            } else {
                $params = array_merge($_POST, getParam());
                $params['estabelecimento_id'] = $params['id'];
                $usucpf = $mUsuario->salvarUsuarioEstabelecimento($params);
                $mUsuario->commit();

                $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'], 'usucpf' => $usucpf)));
                die;
            }
            break;
        case 'buscar-usuario':
            $usucpf = str_replace(array('.', '-'), '', $_REQUEST['usucpf']);
            $mUsuario->carregarPorId($usucpf);
            $dadosUsuario = array(
                'usunome' => $mUsuario->usunome,
                'usudatanascimento' => $mUsuario->usudatanascimento ? formata_data($mUsuario->usudatanascimento) : '',
                'usufonenum' => $mUsuario->usufonenum,
                'usuemail' => $mUsuario->usuemail,
                'ususexo' => $mUsuario->ususexo,
            );

            $db->sucessomAjax('', $dadosUsuario);
            die;
            break;
    }
}

$alteracao = !is_bool($alteracao) ? $alteracao === 't' : $alteracao;

if($usudatanascimento){
    $usudatanascimento = formata_data_sql($usudatanascimento);
}

$habilitado = 'S';
if(isset($p['usucpf']) && !is_null($p['usucpf']) && !$validacao){
    $usuariosResp = UsuarioResponsabilidadeEstabelecimento::getUsuariosPossuemResponsabilidadeEstabelecimento($p['id'], true);

    if(!in_array($p['usucpf'], $usuariosResp) && !$db->testa_superuser()){
        addMsgInfo('Você não pode gerenciar o usuário informado.');
        header('Location: ?modulo=principal/estabelecimento/usuario/lista&acao=C' . setParam(array('id' => $p['id'])));
        die;
    }

    $mUsuario->carregarPorId($usucpf);
    extract($mUsuario->getDados());

    $mUsuarioResponsabilidade = new UsuarioResponsabilidadeEstabelecimento();
    $perfisVinculados = $mUsuarioResponsabilidade->getPerfisComResponsabilidadeEstabelecimento($usucpf, $p['id']);

    $alteracao = true;
    $habilitado = 'N';
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true, true);

?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="icon-user"></i>
            <span class="caption-subject bold uppercase">
                Detalhes do usuário
            </span>
        </div>
    </div>
    <div class="portlet-body">
        <div class="portlet-body form">
            <form role="form" name="formulario" class="formulario-usuario" method="POST">
                <div class="form-body">
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>CPF</label>
                                <?= campo_textom('usucpf', 'S', $habilitado, '', '', 14, '###.###.###-##', '', '', '', 0, '', '', formatar_cpf($usucpf)); ?>
                                <input type="hidden" name="alteracao" value="<?= ($alteracao ? 't' : 'f') ?>" />
                            </div>

                            <div class="form-group">
                                <label>Nome</label>
                                <?= campo_textom('usunome', 'S', $habilitado, '', 50, 50, '', '', '', '', 0); ?>
                            </div>

                            <div class="form-group">
                                <label>Data de nascimento</label>
                                <?= campo_datam('usudatanascimento', 'N', $habilitado, '', 'S', '', ''); ?>
                            </div>

                            <div class="form-group">
                                <label>Perfil</label>
                                <div class="form-select-content">
                                    <?php
                                    $perfisDisponiveis = $mPerfil->recuperarTodos("pflcod AS codigo, pfldsc AS descricao", array(
                                        "pflcod IN (" . implode(', ', UsuarioEstabelecimento::perfisEstabelecimento()) . ")"
                                    ));
                                    $db->monta_checkboxm('perfil[]', $perfisDisponiveis, 'S', '', $perfisVinculados);
                                    ?>
                                </div>
                            </div>
                        </div>

                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Sexo</label>
                                <div class="form-select-content">
                                    <?php
                                    $db->monta_radiom('ususexo', array(
                                        array('codigo' => 'M', 'descricao' => 'Masculino'),
                                        array('codigo' => 'F', 'descricao' => 'Feminino'),
                                    ), $habilitado, 'S')
                                    ?>
                                </div>
                            </div>

                            <div class="form-group">
                                <label>Telefone</label>
                                <?= campo_textom('usufonenum', 'S', $habilitado, '', 15, 15, '', '', '', '', 0); ?>
                            </div>

                            <div class="form-group">
                                <label>Email</label>
                                <?= campo_textom('usuemail', 'S', $habilitado, '', 20, 50, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-actions">
                    <button type="submit" class="btn blue">
                        <i class="fa fa-save"></i> <?= (!$usucpf ? 'Incluir usuário' : 'Atualizar usuário'); ?>
                    </button>
                    <button type="button" class="btn btn-voltar" onclick="window.location.href = '?modulo=principal/estabelecimento/usuario/lista&acao=C<?= setParam(array('id' => $p['id'])) ?>'">
                        <i class="fa fa-arrow-left"></i> Voltar
                    </button>
                </div>

                <input type="hidden" name="act" value="salvar"/>
            </form>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        $("form.formulario-usuario [name=usufonenum]").inputmask({
            mask: ["(99) 9999-9999", "(99) 99999-9999"],
        });

        $(".select2, .select2-multiple").select2({
            language : 'pt-BR',
            placeholder: 'Selecione...',
            width: null
        });

        $(".formulario-usuario").validate({
            errorElement: "span",
            errorClass: "help-block",
            focusInvalid: true,
            ignore: "",
            rules: {
                usucpf: {cpf: true},
                usudatanascimento: {dateITA : true},
                usuemail: {email: true},
            },
            messages: {},
            invalidHandler: function (e, r) {
            },
            highlight: function (e) {
                $(e).closest(".form-group").addClass("has-error")
            },
            success: function (e) {
                e.closest(".form-group").removeClass("has-error");
                e.remove();
            },
            errorPlacement: function (e, r) {
                switch (r.attr('name')){
                    case 'ususexo' :
                        1 === r.closest("div").size() ? e.insertAfter(r.closest("div")) : e.insertAfter(r);
                        break;
                    default :
                        1 === r.closest(".input-icon").size() ? e.insertAfter(r.closest(".input-icon")) : e.insertAfter(r);
                }
            },
            submitHandler: function (e) {
                e.submit();
            }
        });

        $(".formulario-usuario input").keypress(function (e) {
            return 13 == e.which ? ($(".formulario-usuario").validate().form() && $(".formulario-usuario").submit(), false) : void 0;
        });

        $('.formulario-usuario [name=usucpf]').change(function(){
            var usucpf = $(this).val();

            if(usucpf){
                App.blockUI({
                    message: 'Carregando...',
                    target: '.formulario-usuario'
                });

                $.ajax({
                    type: "POST",
                    url: window.location.href,
                    dataType : 'json',
                    data: {
                        act : 'buscar-usuario',
                        p : '<?= setParam(array('id' => $p['id']), false) ?>',
                        usucpf: usucpf
                    },
                    success: function (retorno) {
                        App.unblockUI('.formulario-usuario');
                        var alteracao = false;
                        $.each(retorno.params, function(name, value){
                            if(value){
                                alteracao = true;
                            }

                            switch (name){
                                case 'ususexo':
                                    $('#ususexo_'+value).prop('checked', true);
                                    break;
                                default :
                                    $('[name='+name+']').val(value);
                                    break;
                            }
                        });

                        if(alteracao){
                            $('[name=alteracao]').val('t');
                            desabilitarFormulario();
                        }else{
                            $('[name=alteracao]').val('f');
                            habilitarFormulario();
                        }
                    },
                    error: function (result) {
                        App.unblockUI('.formulario-usuario');
                        exibirAlert('Não foi possível buscar os dados do usuário');
                    }
                });
            }else{
                habilitarFormulario();
            }
        });


    });

    function habilitarFormulario(){
        $('.formulario-usuario').find('input[type=text]:not([name=usucpf]), input[type=radio]').prop('disabled', false);
    }

    function desabilitarFormulario(){
        $('.formulario-usuario').find('input[type=text]:not([name=usucpf]), input[type=radio]').prop('disabled', true);
    }
</script>
