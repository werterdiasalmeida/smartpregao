<?php
$mUsuario = new Usuario();
$p = getParam();

if (!isset($p['id'])) {
    addMsgInfo('Por favor, selecione um estabelecimento.');
    header('Location: ?modulo=principal/estabelecimento/lista&acao=C');
    die;
}

UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);

if($_POST && isset($_POST['act'])){
    switch($_POST['act']){
        case 'redefenir-senha' :
            $mUsuario->redefinirSenha($p);
            $db->commit();
            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])), 'A senha do usuário foi redefinida com sucesso.');
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();
UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true);
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="icon-user"></i>
            <span class="caption-subject bold uppercase">
                Lista de Usuários
            </span>
        </div>
    </div>
    <div class="portlet-body">
        <a class="btn green" href="?modulo=principal/estabelecimento/usuario/formulario&acao=C<?= setParam(array('id' => $p['id'])) ?>">
            <i class="fa fa-plus-circle"></i>
            Novo usuário
        </a>
        <br />

        <?php
        $arCabecalho = array(
            "Nome",
            "CPF",
            "Data de nascimento",
            "Sexo",
            "Telefone",
            "Email",
            "Data de cadastro",
            "Ação"
        );

        $acaoEditar = <<<ACAO
            <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/usuario/formulario&acao=C&p=<php> return setParam(array('id' => {$p['id']}, 'usucpf' => '{usucpf}'), false); </php>" data-toggle="tooltip" title="Editar usuário">
                <i class="icon-pencil"></i>
            </a>
ACAO;

        $acaoRefinirSenha = <<<ACAO
            <a class="btn btn-circle btn-icon-only btn-default yellow-gold" href="javascript:exibirModalRedefinirSenha('<php> return setParam(array('id' => {$p['id']}, 'usucpf' => '{usucpf}'), false); </php>', '<php>return '{usunome}'</php>')" data-toggle="tooltip" title="Redefenir Senha">
                <i class="icon-key"></i>
            </a>
ACAO;
        $acao = <<<ACAO
            <div style="min-width: 50px;">
                {$acaoEditar}
                {$acaoRefinirSenha}
            </div>
ACAO;

        $arConfig = array();
        $arConfig[1]['html'] = "<php>return formatar_cpf('{usucpf}');</php>";
        $arConfig[1]['style'] = "text-align: center;";
        $arConfig[2]['style'] = "text-align: center;";
        $arConfig[4]['style'] = "text-align: center;";
        $arConfig[6]['style'] = "text-align: center;";

        $usucpfResponsabilidade = UsuarioResponsabilidadeEstabelecimento::getUsuariosPossuemResponsabilidadeEstabelecimento($p['id'], true);
        $arWhere = count($usucpfResponsabilidade) > 0 ? array("usucpf IN ('" . implode("', '", $usucpfResponsabilidade) . "')") : array('false');
        $rs = $mUsuario->getList($arWhere);

        $mLista = new Listam(array("style" => "width: 100%;"));
        $mLista->setCabecalho( $arCabecalho );
        $mLista->setCorpo( $rs, $arConfig );
        $mLista->setAcao( $acao );
        $mLista->show();
        ?>
    </div>
</div>

<form method="post" class="form-redefenir-senha">
    <input type="hidden" name="act" value="redefenir-senha" />
    <input type="hidden" name="p" />
</form>

<div class="modal fade in" id="modal-redefenir-senha" tabindex="-1" role="modal-redefenir-senha" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Redefinir Senha</h4>
            </div>
            <div class="modal-body">
                A Senha do(a) usuário(a) <b class="nome-usuario"></b> será alterada para <b><?= Usuario::SENHA_PADRAO ?></b>.<br>
                Deseja continuar?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="redefenirSenha();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">Não</button>
            </div>
        </div>
        <!-- /.modal-content -->
    </div>
    <!-- /.modal-dialog -->
</div>

<script type="text/javascript">

    function exibirModalRedefinirSenha(p, nome){
        var modal = $('#modal-redefenir-senha');

        $('form.form-redefenir-senha [name=p]').val(p);
        modal.find('.nome-usuario').html(nome);
        modal.modal('show');
    }

    function redefenirSenha(){
        $('form.form-redefenir-senha').submit();
    }
</script>

