<?php
$mUsuario = new Usuario();

if($_POST && isset($_POST['act'])){
    switch($_POST['act']){
        case 'alterar_senha' :

            $db->sucessom($_REQUEST['modulo'], '', 'A senha do usuário foi alterada com sucesso.');
            break;
        case 'pesquisa_simples' :

            if(!empty($_REQUEST['pesquisa_simples'])){
                $pesquisa_simples = trim(addslashes($_REQUEST['pesquisa_simples']));
                $arWhere[] = "(
                    (usunome ILIKE '%{$pesquisa_simples}%') OR
                    (usucpf ILIKE '%{$pesquisa_simples}%') OR
                    (usuemail ILIKE '%{$pesquisa_simples}%')
                )";
            }

            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Usuários');
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
    <div class="portlet-body form">
        <div class="campos-pesquisa">
            <form role="form" name="formulario" class="pesquisa-form" method="POST">
                <div class="form-body">
                    <div class="pesquisa-simples">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <div class="input-group">
                                        <?= campo_textom('pesquisa_simples', 'N', 'S', '', 90, 90, '', '', '', '', 0, '', '', null, '', null, null, 'Pesquise por nome de usuário, CPF ou Email'); ?>
                                        <span class="input-group-btn">
                                            <button class="btn blue" type="submit" data-toggle="tooltip" title="Pesquisar">
                                                <i class="fa fa-search"></i>
                                            </button>
                                        </span>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="form-actions">
                    <div class="row">
                        <div class="col-xs-7 col-sm-5">
                        </div>
                        <div class="col-xs-5 col-sm-7 text-right">
                            <?php if(possuiAcesso('adm.php?modulo=sistema/usuario/formulario&acao=A')) : ?>
                                <a class="btn green" href="?modulo=sistema/usuario/formulario&acao=A">
                                    <i class="fa fa-plus-circle"></i>
                                    Cadastrar novo usuário
                                </a>
                            <?php endif; ?>
                        </div>
                    </div>
                </div>
                <input type="hidden" name="act" value="pesquisa_simples"/>
            </form>
        </div>

    </div>
    <div class="portlet-body">
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

        $acao = <<<ACAO
            <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=sistema/usuario/formulario&acao=A&p=<php> return setParam(array('usucpf' => '{usucpf}'), false); </php>" data-toggle="tooltip" title="Editar usuário">
                <i class="icon-pencil"></i>
            </a>
ACAO;

        $arConfig = array();
        $arConfig[1]['html'] = "<php>return formatar_cpf('{usucpf}');</php>";
        $arConfig[1]['style'] = "text-align: center;";
        $arConfig[2]['style'] = "text-align: center;";
        $arConfig[4]['style'] = "text-align: center;";
        $arConfig[6]['style'] = "text-align: center;";

        $rs = $mUsuario->getList($arWhere);

        $mLista = new Listam(array("style" => "width: 100%;"));
        $mLista->setCabecalho( $arCabecalho );
        $mLista->setCorpo( $rs, $arConfig );
        $mLista->setAcao( $acao );
        $mLista->show();
        ?>
    </div>
</div>

<script type="text/javascript">

</script>
