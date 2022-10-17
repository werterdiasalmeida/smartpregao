<?php
$mUsuario = new Usuario();
$usucpf = $_SESSION['usucpf'];
$validacao = false;

if(isset($_REQUEST['act']) && !empty($_REQUEST['act'])){
    switch ($_REQUEST['act']){
        case 'salvar':
            if($mUsuario->isEmailCadastrado($_REQUEST['usuemail'], $usucpf)){
                unset($_REQUEST['p']);
                addMsgDanger('O email informado já existe!');
                extract($_REQUEST);
                $validacao = true;
            } else {

                $dadosUsuario = $mUsuario->carregarPorId($usucpf)->getDados();

                if($dadosUsuario['usuchaveativacao'] && $dadosUsuario['usuchaveativacao'] == 'f'){
                    $senha = md5_decrypt_senha($dadosUsuario['ususenha'], '');
                    if ($senha <> $_REQUEST['senhaatual']) {
                        addMsgDanger('A senha atual não confere.');
                        extract($dadosUsuario);
                        extract($_REQUEST);
                        $validacao = true;
                    }else{
                        $_REQUEST['alterarSenha'] = true;
                        $_REQUEST['usuchaveativacao'] = 't';
                        $_REQUEST['ususenha'] = $_REQUEST['novasenha'];
                    }
                }

                if(!$validacao){
                    $mUsuario->alterarDadosUsuarioLogado($_REQUEST, $_FILES);
                    $mUsuario->commit();

                    $db->sucessom($_REQUEST['modulo'], '');
                }
            }
            break;
            case 'download' :
                $file = new FilesSimec();
                $params = getParam();
                $file->getDownloadArquivo($params['arqid']);
                die;
                break;
    }
}

if($usudatanascimento){
    $usudatanascimento = formata_data_sql($usudatanascimento);
}

if(!$validacao){
    $mUsuario->carregarPorId($usucpf);
    extract($mUsuario->getDados());
}

include APPRAIZ . "includes/cabecalhom.inc";

$subTitulo = "Detalhes do usuário";
if($usuchaveativacao && $usuchaveativacao == 'f'){
    $subTitulo = "Dados do Usuário";
}

?>
<br/>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption">
            <i class="icon-user"></i>
            <span class="caption-subject bold uppercase">
                <?= $subTitulo ?>
            </span>
        </div>
    </div>
    <div class="portlet-body">
        <div class="portlet-body form">
            <form role="form" name="formulario" class="formulario-usuario" method="POST" enctype="multipart/form-data">
                <div class="form-body">
                    <div class="row">
                        <div class="col-md-10">
                            <div class="row">
                                <div class="col-md-6">
                                    <div class="form-group">
                                        <label>CPF</label>
                                        <br />
                                        <strong><?= formatar_cpf($usucpf) ?></strong>
                                    </div>
                                </div>
                            </div>
                            <div class="row">
                                <div class="col-md-12">
                                    <div class="form-group">
                                        <label>Nome</label>
                                        <?= campo_textom('usunome', 'S', 'S', '', 50, 50, '', '', '', '', 0); ?>
                                    </div>
                                </div>
                            </div>
                            <div class="row">
                                <div class="col-md-2">
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
                                </div>
                                <div class="col-md-5">
                                    <div class="form-group">
                                        <label>Data de nascimento</label>
                                        <?= campo_datam('usudatanascimento', 'N', 'S', '', 'S', '', ''); ?>
                                    </div>
                                </div>
                                <div class="col-md-5">
                                    <div class="form-group">
                                        <label>Email</label>
                                        <div class="input-group">
                                            <span class="input-group-addon">
                                                <i class="fa fa-envelope"></i>
                                            </span>
                                            <?= campo_textom('usuemail', 'S', 'S', '', 20, 50, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="row">
                                <div class="col-md-4">
                                    <div class="form-group">
                                        <label>Celular/Whatsapp</label>
                                        <div class="input-group">
                                            <span class="input-group-addon">
                                                <i class="fa fa-whatsapp"></i>
                                            </span>
                                            <?= campo_textom('usufonenum', 'S', 'S', '', 15, 15, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="col-md-4">
                                    <div class="form-group">
                                        <label>Instagram</label>
                                        <div class="input-group">
                                            <span class="input-group-addon">
                                                <i class="fa fa-instagram"></i>
                                            </span>
                                            <?= campo_textom('usuinst', 'N', 'S', '', 20, 100, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                </div>
                                <div class="col-md-4">
                                    <div class="form-group">
                                        <label>Facebook</label>
                                        <div class="input-group">
                                            <span class="input-group-addon">
                                                <i class="fa fa-facebook-square"></i>
                                            </span>
                                            <?= campo_textom('usuface', 'N', 'S', '', 20, 100, '', '', '', '', 0); ?>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class="row">
                                <div class="col-md-12">
                                    <div class="form-group">
                                        <label><b>Certificado Digital</b> <?= $arqid_cd ?></label>
                                        <div class="input-group">
                                            <span class="input-group-addon">
                                            <i class="fa fa-lock"></i>
                                            </span>
                                            <input type="file" name="arquivo_cd" class="form-control"  />
                                            <input type="hidden" name="arqid_cd" value="<?= $arqid_cd ?>" />
                                            
                                        </div>
                                        <?php
                                            if($arqid_cd){
                                            ?>
                                            <a href="#" data-toggle="tooltip" title="Baixar o arquivo do Certificado Digital">
                                            <i class="fa fa-download"></i> Baixar Arquivo do Certificado
                                            </a>
                                            <?php
                                            }
                                            ?>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-2">
                            <div class="form-group">
                                <label>Foto</label>
                                <br />
                                <div class="fileinput fileinput-new" data-provides="fileinput">
                                    <div class="fileinput-new thumbnail" style="width: 150px; height: 100px;">
                                        <?php montaImgFoto($arqid, $ususexo); ?>
                                        <input type="hidden" name="arqid" value="<?= $arqid ?>" />
                                    </div>
                                    <div class="fileinput-preview fileinput-exists thumbnail"
                                         style="max-width: 150px; max-height: 100px;"></div>
                                    <div>
                                    <span class="btn default btn-file">
                                        <span class="fileinput-new"> Selecionar imagem </span>
                                        <span class="fileinput-exists"> Alterar </span>
                                        <input type="file" name="foto"/>
                                    </span>
                                        <a href="javascript:;" class="btn red fileinput-exists"
                                           data-dismiss="fileinput">
                                            Remover </a>
                                    </div>
                                </div>
                                <br />
                                <small>(Imagem com tamanho mínimo de 250x250)</small>
                            </div>
                        </div>
                    </div>
                    <?php if($usuchaveativacao && $usuchaveativacao == 'f'): ?>
                        <h4 class="form-section">
                            <div class="caption font-red-sunglo">
                                <i class="icon-key font-red-sunglo"></i>
                                <span class="caption-subject bold uppercase">
                                    <?= ($deveAlterarSenha ? "Por favor, efetue a alteração da sua senha" : 'Realizar alteração de senha' )   ?>
                                </span>
                            </div>
                        </h4>
                        <div class="form-group">
                            <label class="bold">Senha atual <i class="fa fa-asterisk" style="color: red; font-size: 6px; vertical-align: text-top;"></i></label>
                            <input class="form-control spinner" type="password" placeholder="Informe a sua senha atual" name="senhaatual" />
                        </div>
                        <div class="form-group">
                            <label class="bold">Nova senha <i class="fa fa-asterisk" style="color: red; font-size: 6px; vertical-align: text-top;"></i></label>
                            <input class="form-control spinner" type="password" placeholder="Informe a nova senha" name="novasenha" id="novasenha" />
                        </div>
                        <div class="form-group">
                            <label>Confirmação de senha</label>
                            <input class="form-control spinner" type="password" placeholder="Confirme sua nova senha" name="confsenha" />
                        </div>
                    <?php endif; ?>

                </div>
                <div class="form-actions">
                    <button type="submit" class="btn blue">
                        <i class="fa fa-save"></i> Atualizar
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

        $(".formulario-usuario").validateForm({
            rules : {
                foto: {accept: "image/*"},
                usudatanascimento: {dateITA : true},
                usuemail: {email: true},
                <?php if($usuchaveativacao && $usuchaveativacao == 'f'): ?>
                    senhaatual: {required: true},
                    novasenha: {required: true, minlength: 8},
                    confsenha: {equalTo: "#novasenha"}
                <?php endif; ?>
            }
        });

        $(".formulario-usuario input").keypress(function (e) {
            return 13 == e.which ? ($(".formulario-usuario").validate().form() && $(".formulario-usuario").submit(), false) : void 0;
        });
    });
</script>
