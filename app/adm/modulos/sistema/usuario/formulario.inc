<?php
$p = getParam();
$usucpf = isset($p['usucpf']) ? $p['usucpf'] : '';
$mUsuario = new Usuario();
$mPerfil = new Perfil();
$alteracao = false;
$validacao = false;

if(isset($_REQUEST['act']) && !empty($_REQUEST['act'])){
    switch ($_REQUEST['act']){
        case 'salvar':
            if($_REQUEST['alteracao'] === 'f' && $mUsuario->isCpfCadastrado($_REQUEST['usucpf'])){
                unset($_REQUEST['p']);
                addMsgDanger('O CPF informado já existe!');
                extract($_REQUEST);
                $validacao = true;
            } else if($mUsuario->isEmailCadastrado($_REQUEST['usuemail'], $_REQUEST['usucpf'])){
                unset($_REQUEST['p']);
                addMsgDanger('O email informado já existe!');
                extract($_REQUEST);
                $validacao = true;
            } else {
                $mUsuario->salvarUsuario($_REQUEST);
                $mUsuario->commit();

                $db->sucessom($_REQUEST['modulo'], setParam(getParam()));
                die;
            }
            break;
        case 'selecionarResponsabilidade':
        case 'salvarResponsabilidade':
            include_once 'selecionarResponsabilidade.inc';
            die;
            break;
    }
}

$alteracao = !is_bool($alteracao) ? $alteracao === 't' : $alteracao;

if($usudatanascimento){
    $usudatanascimento = formata_data_sql($usudatanascimento);
}

$perfisVinculados = array();
if(isset($p['usucpf']) && !is_null($p['usucpf']) && !$validacao){
    $mUsuario->carregarPorId($usucpf);
    extract($mUsuario->getDados());

    $mPerfilUsuario = new PerfilUsuario();
    $perfisVinculados = $mPerfilUsuario->recuperarColuna('pflcod', array("usucpf = '{$usucpf}'"));

    $alteracao = true;
}

if($perfil){
    $perfisVinculados = $perfil;
}

$perfis = $mPerfil->recuperarTodos('pflcod, pfldsc', array("pflstatus = 'A'", "sisid = {$_SESSION['sisid']}"), 2);

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Usuário', 'Utilize esta página para gerenciar um usuário.');
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
                                <?php
                                $usucpf = $usucpf ? formatar_cpf($usucpf) : '';
                                echo campo_textom('usucpf', 'S', ($alteracao ? 'N' : 'S'), '', '', 14, '###.###.###-##', '', '', '', 0);
                                ?>
                                <input type="hidden" name="alteracao" value="<?= ($alteracao ? 't' : 'f') ?>" />
                            </div>

                            <div class="form-group">
                                <label>Nome</label>
                                <?= campo_textom('usunome', 'S', 'S', '', 50, 50, '', '', '', '', 0); ?>
                            </div>

                            <div class="form-group">
                                <label>Data de nascimento</label>
                                <?= campo_datam('usudatanascimento', 'N', 'S', '', 'S', '', ''); ?>
                            </div>

                            <div class="form-group">
                                <label>Perfil</label>
                                <select id="perfil" name="perfil[]" class="form-control select2-multiple" multiple>
                                    <?php foreach ($perfis as $perfil): ?>
                                        <option value="<?= $perfil['pflcod'] ?>" <?= (in_array($perfil['pflcod'], $perfisVinculados) ? 'selected' : '') ?>>
                                            <?= $perfil['pfldsc'] ?>
                                        </option>
                                    <?php endforeach;?>
                                </select>

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
                                    ), 'S', 'S')
                                    ?>
                                </div>
                            </div>

                            <div class="form-group">
                                <label>Telefone</label>
                                <?= campo_textom('usufonenum', 'S', 'S', '', 15, 15, '', '', '', '', 0); ?>
                            </div>

                            <div class="form-group">
                                <label>Email</label>
                                <?= campo_textom('usuemail', 'S', 'S', '', 20, 50, '', '', '', '', 0); ?>
                            </div>


                            <div class="form-group">
                                <label>Redefinir senha padrão</label>
                                <div class="form-select-content">
                                    <?php
                                    $senhaHabilitada = 'S';
                                    $senha_padrao = '';
                                    if(!$usucpf){
                                        $senhaHabilitada = 'N';
                                        $senha_padrao = Usuario::SENHA_PADRAO;
                                    }

                                    $db->monta_checkboxm('senha_padrao', array(
                                        array('codigo' => Usuario::SENHA_PADRAO, 'descricao' => Usuario::SENHA_PADRAO)
                                    ), $senhaHabilitada);
                                    ?>
                                </div>
                            </div>
                        </div>
                    </div>

                    <?php
                    $mPerfilResponsabilidade = new TipoResponsabilidadePerfil();
                    $mUsuarioResponsabilidade = new UsuarioResponsabilidade();
                    if(!empty($perfisVinculados) && $mPerfilResponsabilidade->isPerfisPossuemResponsabilidade($perfisVinculados)) :
                        ?>
                        <h4 class="form-section">Responsabilidade</h4>
                        <div class="row">
                            <div class="col-md-12">
                                <table class="table">
                                    <thead>
                                        <tr>
                                            <th>
                                                Perfil
                                            </th>
                                            <th>
                                                Estabelecimentos
                                            </th>
                                            <th>
                                                Ação
                                            </th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <?php foreach($mUsuarioResponsabilidade->listarPerfisResponsabilidade($p['usucpf'], $_SESSION['sisid']) as $perfilResponsabilidade) : ?>
                                            <tr>
                                                <td style="width: 30%;">
                                                    <?= $perfilResponsabilidade['pfldsc'] ?>
                                                </td>
                                                <td class="estabelecimentos_<?= $perfilResponsabilidade['pflcod'] ?>">
                                                    <?= getLabels($perfilResponsabilidade['estabelecimentos']) ?>
                                                </td>
                                                <td style="width: 5%;">
                                                    <a class="btn btn-circle btn-icon-only btn-default blue" href="#" data-toggle="tooltip" title="Atribuir responsabilidade" onclick="selecionarResponsabilidade(<?= $perfilResponsabilidade['pflcod']; ?>);">
                                                        <i class="icon-pencil"></i>
                                                    </a>
                                                </td>
                                            </tr>
                                        <?php endforeach; ?>
                                    </tbody>
                                </table>
                            </div>
                        </div>
                        <?php
                    endif;
                    ?>
                </div>
                <div class="form-actions">
                    <button type="submit" class="btn blue">
                        <i class="fa fa-save"></i> <?= (!$usucpf ? 'Incluir usuário' : 'Atualizar usuário'); ?>
                    </button>
                    <button type="button" class="btn btn-voltar" onclick="window.location.href = '?modulo=sistema/usuario/lista&acao=X'">
                        <i class="fa fa-arrow-left"></i> Voltar
                    </button>
                </div>

                <input type="hidden" name="act" value="salvar"/>
            </form>
        </div>
    </div>
</div>


<div class="modal fade in" id="modal-responsabilidade" tabindex="-1" role="modal-responsabilidade" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Selecionar responsabilidade</h4>
            </div>
            <div class="modal-body" id="responsabilidade">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn blue hidden btn-incluir">
                    <i class="fa fa-save"></i>
                    Salvar
                </button>
                <button type="button" class="btn" data-dismiss="modal">Fechar</button>
            </div>
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
    });

    function selecionarResponsabilidade(pflcod)
    {
        $('.btn-incluir').addClass('hidden').unbind('click');
        $('#modal-responsabilidade').modal('show');

        var responsabilidadeContainer = $('#responsabilidade');
        responsabilidadeContainer.html('<div style="padding: 20px;"></div>');

        App.blockUI({
            message: 'Carregando...',
            target: '#responsabilidade'
        });

        responsabilidadeContainer.load(window.location.href, {
            act: 'selecionarResponsabilidade',
            pflcod: pflcod,
        }, function () {
            App.unblockUI('#responsabilidade');
        });
    }
</script>
