<?php
include "includes_publico.inc";

// Carrega a combo com os municípios
if ($_REQUEST["ajaxRegcod"]) {
    header('content-type: text/html; charset=ISO-8859-1');

    $sql = "SELECT
				muncod AS codigo,
				mundescricao AS descricao
			FROM
				territorios.municipio
			WHERE
				estuf = '{$_POST['ajaxRegcod']}'
			ORDER BY
				mundescricao ASC";
    die($db->monta_combo("muncod", $sql, 'S', 'Selecione um município', '', '', '', '271', 'S', 'muncod'));
}

// Carrega a combo com os orgãos do tipo selecionado
if ($_REQUEST["ajax"] == 1) {
    // Se for estadual verifica se existe estado selecionado
    if ($_REQUEST["tpocod"] == 2 && !$_REQUEST["regcod"]) {
        echo '<font style="color:#909090;">
					Favor selecionar um Estado.
				  </font>';
        die;
    }
    // Se for municipal verifica se existe estado selecionado
    if ($_REQUEST["tpocod"] == 3 && !$_REQUEST["muncod"]) {
        echo '<font style="color:#909090;">
				Favor selecionar um município.
			  </font>';
        die;
    }
    $tpocod = $_REQUEST["tpocod"];
    $muncod = $_REQUEST["muncod"];
    $regcod = $_REQUEST["regcod"];

    carrega_orgao($editavel, $usucpf);
    die;
}

$_SESSION['mnuid'] = 10;
$_SESSION['sisid'] = 4;

// captura os dados informados no primeiro passo
$sisid = $_REQUEST['sisid'];
$usucpf = $_REQUEST['usucpf'];

// Verifica se o CPF digitado é válido.
if (!validaCPF($usucpf)) {
    die('<script>
			alert(\'CPF inválido!\');
			history.go(-1);
		 </script>');
}

$remetente = array();
$remetente['email'] = $email_from;
$remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];

// atribui o ano atual para o exercício das tarefas
// ultima modificação: 05/01/2007
$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
$_SESSION['exercicio'] = $db->pega_ano_atual();

// captura os dados do formulário
$usunome = $_REQUEST['usunome'];
$usuemail = $_REQUEST['usuemail'];
$usuemail_c = $_REQUEST['usuemail_c'];
$usufoneddd = $_REQUEST['usufoneddd'];
$usufonenum = $_REQUEST['usufonenum'];
$ususexo = $_REQUEST['ususexo'];
$pflcod = $_REQUEST['pflcod'];
$muncod = $_REQUEST['muncod'];
$regcod = $_REQUEST['regcod'];
$carid = $_REQUEST['carid'];
$usufuncao = $_REQUEST['usufuncao'];
$usunomeguerra = $_REQUEST['usunomeguerra'];
$usumatricula = $_REQUEST['usumatricula'];

// prepara o cpf para ser usado nos comandos sql
$cpf = corrige_cpf($usucpf);

// verifica se o cpf já está cadastrado no sistema
$sql = sprintf(
    "SELECT
			u.ususexo,
			u.usucpf,
			u.regcod,
			u.usunome,
			u.usuemail,
			u.usustatus,
			u.usufoneddd,
			u.usufonenum,
			u.ususenha,
			u.usudataultacesso,
			u.usunivel,
			u.usufuncao,
			u.ususexo,
			u.entid,
			u.unicod,
			u.usuchaveativacao,
			u.usutentativas,
			u.usuobs,
			u.ungcod,
			u.usudatainc,
			u.usuconectado,
			u.suscod,
			u.muncod,
			u.carid
		FROM
			seguranca.usuario u
		WHERE
			u.usucpf = '%s'",
    $cpf
);

$usuario = (object)$db->pegaLinha($sql);


if ($usuario->usucpf) {
    foreach ($usuario as $atributo => $valor) {
        $$atributo = $valor;
    }
    $usucpf = formatar_cpf($usuario->usucpf);
    $cpf_cadastrado = true;
    $editavel = 'N';
} else {
    $cpf_cadastrado = false;
    $editavel = 'S';
}

// verifica se o usuário já está cadastrado no módulo selecionado
$sql = sprintf("SELECT usucpf, sisid, suscod FROM usuario_sistema WHERE usucpf = '%s' AND sisid = %d", $cpf, $sisid);

$usuario_sistema = (object)$db->pegaLinha($sql);
if ($usuario_sistema->sisid) {
    if ($usuario_sistema->suscod == 'B') {
        $_SESSION['MSG_AVISO'] = array("Sua conta está bloqueada neste sistema. Para solicitar a ativação da sua conta justifique o pedido no formulário abaixo.");
        header("Location: solicitar_ativacao_de_conta.php?sisid=$sisid&usucpf=$usucpf");
        exit();
    }
    $_SESSION['MSG_AVISO'] = array("Atenção. CPF já cadastrado no módulo solicitado.");
    header("Location: cadastrar_usuario.php?sisid=$sisid&usucpf=$usucpf");
    exit();
}
$cpf_cadastrado_sistema = (boolean)$db->pegaUm($sql);

$sql = sprintf("select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado, sisdiretorio from sistema where sisid = %d", $sisid);
$sistema = (object)$db->pegaLinha($sql);

// efetiva cadastro se o formulário for submetido
if ($_POST['formulario']) {
    // Gerando a senha que poderá ser usada no SSD e no simec
    $senhageral = $db->gerar_senha();

    // atribuições requeridas para que a auditoria do sistema funcione
    $_SESSION['sisid'] = 4; # seleciona o sistema de segurança
    $_SESSION['usucpf'] = $cpf;
    $_SESSION['usucpforigem'] = $cpf;

    $tpocod_banco = $tpocod ? (integer)$tpocod : "null";
    if (!$cpf_cadastrado) {

        // insere informações gerais do usuário
        $sql = sprintf(
            "INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, carid, entid, unicod, usuchaveativacao, regcod,
					ususexo, ungcod, ususenha, suscod, orgao,
					muncod, tpocod, usunomeguerra, usumatricula
				) values (
					'%s', '%s', '%s', '%s', '%s',
					'%s', '%s', %s, '%s', '%s',
					'%s', '%s', '%s', '%s', '%s',
					'%s', '%s', %s, '%s', '%s'
				)",
            $cpf,
            str_to_upper($usunome),
            strtolower($usuemail),
            $usufoneddd,
            $usufonenum,
            $usufuncao,
            $carid,
            'null',
            $unicod,
            'f',
            $regcod,
            $ususexo,
            $ungcod,
            md5_encrypt_senha($senhageral, ''),
            'P',
            $orgao,
            $muncod,
            $tpocod_banco,
            $usunomeguerra,
            $usumatricula
        );
//ver($sql,d);
        $db->executar($sql);
    }

    // vincula o usuário com o módulo
    $sql = sprintf(
        "INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '%s', %d, %d )",
        $cpf,
        $sisid,
        $pflcod
    );
    $db->executar($sql);

    // modifica o status do usuário (no módulo) para pendente
    $db->alterar_status_usuario($cpf, 'P', "", $sisid);

    //----------- verificando se o sistema deve inserir dados de proposta e os insere caso necessario

    $sql = "SELECT
			 s.sisid, lower(s.sisdiretorio) as sisdiretorio, s.sisdsc
			FROM
			 seguranca.sistema s
			WHERE
			 sisid = " . $sisid . "";

    $sistema = (object)$db->pegaLinha($sql);

    $sql = sprintf("SELECT
						CASE WHEN (SELECT
				   						true
				  				   FROM
				   						pg_tables
				  				   WHERE
				   						schemaname = '%s' AND
				   						tablename  = 'tiporesponsabilidade')
						THEN
							true
						WHEN
							  (SELECT
							   		true
							   FROM
							   		pg_tables
							   WHERE
								   schemaname='%s' AND
								   tablename = 'tprperfil')
				THEN true
				ELSE false
				END;",
        $sistema->sisdiretorio,
        $sistema->sisdiretorio);

    $existeTabela = $db->pegaUm($sql);

    if ($existeTabela == 't') {

        $propostos = (array)$_REQUEST["proposto"];

        foreach ($propostos as $chave => $valores) {

            $sql_tpr = "select tprcampo from " . $sistema->sisdiretorio . ".tiporesponsabilidade where tprsigla = '" . $chave . "'";

            $tprcampo = $db->pegaUm($sql_tpr);
            foreach ($valores as $chave => $valor) {
                $sql_proposta = "insert into
						seguranca.usuariorespproposta
						( urpcampoid, urpcampo, pflcod, usucpf, sisid )
					 	values
					 	( '" . $valor . "', '" . $tprcampo . "', '" . $pflcod . "', '" . $cpf . "', " . $sisid . " )";

                $db->executar($sql_proposta, false);
            }
        }
    }

    //---------------------------- fim da verificação e inserção


    //**************VERIFICA SE PERFIL POSSUI ATIVAÇÃO AUTOMATICA DO CADASTRO DE USUÁRIO********************

    //$sql = sprintf("SELECT pflcod FROM seguranca.perfil WHERE sisid= and pfpadrao = %s",$pflcod);
    $sql = sprintf("SELECT pflcod FROM seguranca.perfil WHERE sisid=%s and pflpadrao='t'", $sisid);
    $pflcodpadrao = (array)$db->carregarColuna($sql);

    if ($pflcodpadrao && in_array($pflcod, $pflcodpadrao)) { //ativação automatica
        // carrega os dados da conta do usuário
        $sql = sprintf("SELECT
							usucpf, usuemail, ususexo, usunome, ususenha
						FROM
							seguranca.usuario
						WHERE
							usucpf = '%s'",
            $cpf
        );
        $usuariod = (object)$db->pegaLinha($sql);

        $justificativa = "Ativação automática de usuário pelo sistema";
        $suscod = "A";
        $db->alterar_status_usuario($usuariod->usucpf, $suscod, $justificativa, $sisid);


        if (!$cpf_cadastrado) {

            $destinatario = $usuemail;
            $assunto = "Cadastro no sistema {$sistema->sisdsc}";

            $conteudo = "Prezado(a) Sr(a). {$usunome}<br /><br />
                		Seu Cadastro foi realizado com sucesso. Sua senha é <strong>{$senhageral}</strong> e deverá ser alterada no primeiro acesso. Para acessar o sistema utilize sempre o seu CPF";

            enviar_email($remetente, $destinatario, $assunto, $conteudo);
        }

        $sql = sprintf(
            "INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )",
            $usuariod->usucpf,
            $pflcod
        );
        $db->executar($sql);
        $db->commit();

        $_REQUEST['usucpf'] = formatar_cpf($usuariod->usucpf);
        $_POST['ususenha'] = md5_decrypt_senha($usuariod->ususenha, '');
        $_SESSION['logincadastro'] = true;
        include APPRAIZ . "includes/autenticar.inc";

        exit();
    } else { //solicitação de cadastro
        $sql = "SELECT DISTINCT
        		      usuemail
        		    FROM
        		      seguranca.usuario usu
        		    INNER JOIN
        		      seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf
        		    INNER JOIN
        		      seguranca.perfil pfl ON pfu.pflcod = pfl.pflcod AND pfl.sisid = {$sisid}  AND pfl.pflstatus = 'A' AND pfl.pflsuperuser = true
        		    INNER JOIN
        		      seguranca.usuario_sistema uss ON uss.usucpf = usu.usucpf AND uss.sisid = {$sisid} AND uss.suscod = 'A' AND uss.susstatus = 'A'";

        $emailsSuperUser = $db->carregarColuna($sql);
        $emailsSuperUser = $emailsSuperUser ? $emailsSuperUser : array();

        $destinatario = $usuemail;
        $assunto = "Solicitação de cadastro no sistema {$sistema->sisdsc}";
        $conteudo = "Prezado(a) Sr(a). {$usunome}<br /><br />
	                     Sua solicitação de cadastro para acesso ao módulo {$sistema->sisdsc} foi registrada e será analisada pelo setor responsável. Em breve você receberá maiores informações.";
        enviar_email($remetente, $destinatario, $assunto, $conteudo, null, $emailsSuperUser);

        $sisabrev = $db->pegaUm("SELECT sisabrev FROM seguranca.sistema WHERE sisid = " . $sisid);
        $mensagem = sprintf("Sua solicitação de cadastro para acesso ao módulo %s foi registrada e será analisada pelo setor responsável. Em breve você receberá maiores informações.", $sisabrev);
        $_SESSION['MSG_AVISO'][] = $mensagem;

        $db->commit();

        header("Location: login.php");

        exit();

    }

    //**************FIM VERIFICA SE PERFIL POSSUI ATIVAÇÃO AUTOMATICA DO CADASTRO DE USUÁRIO********************
}

include "cabecalho_publico.inc";
?>
    <style type="text/css">
        input[type=text]{
            width: 90%;
        }

        input[type=text]#usucpf_disabled{
            width: 90% !important;
        }

        select{
            width: 90% !important;
        }

        #usufoneddd{
            width: 20% !important;
        }

        #usufonenum{
            width: 70% !important;
        }
    </style>

    <div id="caixa-login">
        <div class="conteudo">
            <form method="post" name="formulario">
                <input type="hidden" name="formulario" value="1"/>

                <div class="col-md-12 col-sm-12">
                    <div class="col-md-3 col-sm-2">

                    </div>
                    <div id="bem_vindo_2" class="col-md-6 col-sm-8">
                        &nbsp;
                    </div>
                    <div class="col-md-3 col-sm-2">

                    </div>
                </div>
                <div class="col-md-2 col-sm-2 col-xm-0">

                </div>

                <div class="col-md-8 col-sm-8 col-xm-12">
                    <div id="container-publico">
                        <div id="caixa-formulario">
                            <table border="0" cellpadding="0" cellspacing="0" class="tabela_modulos">
                                <tr>
                                    <td class="td_bg">Solicitação de Cadastro de Usuários</td>
                                </tr>
                                <tr>
                                    <td>
                                        <!--Caixa de Login-->
                                        <table class="tabela" style="width: 100%;">
                                            <?php if ($sistema->sisid): ?>
                                                <tr>
                                                    <td class="SubTituloDireita" style="width: 20%;">
                                                        <strong>Módulo:</strong>
                                                    </td>
                                                    <td style="width: 80%;">
                                                        <?php
                                                        $sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t' AND sisid = {$sisid}";
                                                        $db->monta_combo("sisid", $sql, 'N', "Selecione o sistema desejado", '', '', '', null);
                                                        ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita">CPF:</td>
                                                    <td>
                                                        <?php $usucpf_disabled = $usucpf; ?>
                                                        <?= campo_texto('usucpf_disabled', $obrig, 'N', '', null, 14, '###.###.###-##', ''); ?>
                                                        <?= obrigatorio(); ?>
                                                        <input type="hidden" name="usucpf" value="<?= $usucpf ?>"/>
                                                    </td>
                                                </tr>
                                            <?php endif; ?>

                                            <?php if ($cpf_cadastrado_sistema): ?>
                                                <tr bgcolor="#C0C0C0">
                                                    <td>&nbsp;</td>
                                                    <td><input type="button" value="Voltar"
                                                               onclick="location.href='./cadastrar_usuario.php?sisid=<?= $sisid ?>&usucpf=<?= $usucpf ?>'">
                                                    </td>
                                                </tr>
                                            <?php else: ?>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita" width="150px">
                                                        Nome:
                                                    </td>
                                                    <td>
                                                        <?= campo_texto('usunome', $obrig, $editavel, '', null, 50, '', ''); ?>
                                                        <?= obrigatorio(); ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita" width="150px">
                                                        Nome de Guerra:
                                                    </td>
                                                    <td>
                                                        <?= campo_texto('usunomeguerra', 'N', $editavel, '', null, 50, '', ''); ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita" width="150px">
                                                        Matrícula:
                                                    </td>
                                                    <td>
                                                        <?= campo_texto('usumatricula', 'N', $editavel, '', null, 50, '', ''); ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="subtituloDireita" width="150px">Patente:</td>
                                                    <td>
                                                        <?php
                                                        $sql = "select carid as codigo, cardsc as descricao from public.cargo";
                                                        $db->monta_combo("carid", $sql, $editavel, '&nbsp;', 'alternarExibicaoCargo', '', '', '', 'N', "carid", '' );
                                                        ?>
                                                        <?= campo_texto( 'usufuncao', 'N', $editavel, '', 50, 100, '','', '', '', '', 'id="usufuncao" style="display: none;"' ); ?>
                                                        <?= obrigatorio(); ?>
                                                        <br />
                                                        <a href="javascript: alternarExibicaoCargo( 'exibirOpcoes' )" id="linkVoltar" style="display: none;" > Exibir Opções</a>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita">
                                                        Sexo:
                                                    </td>
                                                    <td>
                                                        <input id="sexo_masculino" type="radio" name="ususexo"
                                                               value="M" <?= ($ususexo == 'M' ? "CHECKED" : "") ?> <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> />
                                                        <label for="sexo_masculino">Masculino</label>
                                                        <input id="sexo_feminino" type="radio" name="ususexo"
                                                               value="F" <?= ($ususexo == 'F' ? "CHECKED" : "") ?>    <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> />
                                                        <label for="sexo_feminino">Feminino</label> <?= obrigatorio(); ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita">
                                                        UF:
                                                    </td>
                                                    <td>
                                                        <?php
                                                        $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
                                                        $db->monta_combo("regcod", $sql, $editavel, "&nbsp;", 'listar_municipios', '', '', null, 'S', 'regcod');
                                                        ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita">
                                                        Município:
                                                    </td>
                                                    <td>
                                                        <div id="muncod_on"
                                                             style="display:<?= (($regcod && $muncod) ? 'block' : 'none') ?>;">
                                                            <?php
                                                            if ($regcod && $muncod) {
                                                                $sql = "SELECT
                                                                    muncod AS codigo,
                                                                    mundescricao AS descricao
                                                                FROM
                                                                    territorios.municipio
                                                                WHERE
                                                                    estuf = '{$regcod}'
                                                                ORDER BY
                                                                    mundescricao ASC";
                                                                $db->monta_combo("muncod", $sql, 'S', 'Selecione um município', '', '', '', '271', 'S', 'muncod');
                                                            } else {
                                                                echo '<select name=\'muncod\' id=\'muncod\' class=\'CampoEstilo\' style=\'width:170px;\'>
                                                                <option value="">Selecione um município</option>
                                                              </select>';
                                                            }
                                                            ?>
                                                        </div>
                                                        <div id="muncod_off"
                                                             style="color: #909090; display:<?= (($regcod && $muncod) ? 'none' : 'block') ?>;">
                                                            Selecione uma UF
                                                        </div>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita">(DDD) + Telefone:</td>
                                                    <td><?= campo_texto('usufoneddd', '', $editavel, '', 3, 2, '##', ''); ?> <?= campo_texto('usufonenum', 'S', $editavel, '', 18, 15, '###-####|####-####|#####-####', ''); ?>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align='right' class="SubTituloDireita">E-Mail:</td>
                                                    <td><?= campo_texto('usuemail', 'S', $editavel, '', null, 100, '', '', 'left', '', 0, ''); ?>
                                                    </td>
                                                </tr>
                                                <?php if (!$cpf_cadastrado): ?>
                                                    <tr>
                                                        <td align='right' class="SubTituloDireita">Confirmação de<br/>E-Mail:
                                                        </td>
                                                        <td><?= campo_texto('usuemail_c', 'S', '', '', null, 100, '', ''); ?>
                                                            <br/>
                                                        </td>
                                                    </tr>
                                                <?php endif;

                                                // inclui campos requeridos pelo módulo no qual o usuário pretende se cadastrar
                                                $arquivo = APPRAIZ . $sistema->sisdiretorio . "/modulos/sistema/usuario/incusuario.inc";
                                                if (file_exists($arquivo)) {
                                                    include $arquivo;
                                                }
                                                ?>
                                                <tr>
                                                    <td colspan="2" class="text-center">
                                                        <a class="button" style=""
                                                           href="javascript:enviar_formulario()">Enviar</a>
                                                        <a class="button" style=""
                                                           href="./cadastrar_usuario.php?sisid=<?= $sisid ?>&usucpf=<?= $usucpf ?>">Voltar</a>
                                                    </td>
                                                </tr>
                                            <?php endif; ?>
                                        </table>
                                    </td>
                                </tr>
                            </table>
                        </div>
                        <!--fim Caixa de Login -->
                    </div>

                    <br style="clear: both;" />
                    <div class="col-md-3 col-sm-2">

                    </div>

                    <div id="bem_vindo" class="col-md-6 col-sm-8">
                        &nbsp;
                    </div>
                    <div class="col-md-3 col-sm-2">

                    </div>

                </div>
            </form>
        </div>
    </div>

    <script src="/includes/prototype.js"></script>
    <script type="text/javascript">
        function listar_municipios(regcod) {
            var div_on = document.getElementById('muncod_on');
            var div_off = document.getElementById('muncod_off');

            if (regcod) {
                var req = new Ajax.Request('cadastrar_usuario_2.php', {
                    method: 'post',
                    parameters: '&ajaxRegcod=' + regcod,
                    onComplete: function (res) {
                        div_on.style.display = 'block';
                        div_off.style.display = 'none';

                        div_on.innerHTML = res.responseText;
                    }
                });
            } else {
                div_on.style.display = 'none';
                div_off.style.display = 'block';
            }

        }

        function alternarExibicaoCargo( tipo ){

            var carid = document.getElementById( 'carid' );
            var usufuncao = document.getElementById( 'usufuncao' );
            var link = document.getElementById( 'linkVoltar' );

            if( tipo != 'exibirOpcoes' ){
                if( carid.value == 9 ){
                    usufuncao.style.display = "";
                    //usufuncao.className = "";
                    link.style.display = "";
                    carid.style.display = "none";
                    //link.className = "";
                }
            }
            else{
                usufuncao.style.display = "none";
                //usufuncao.value = "";
                link.style.display = "none";
                //link.className = "objetoOculto";
                carid.style.display = "";
                carid.value = "";
            }
        }

        function trim(valor) {
            //return valor.replace( /^\s*|\s*$/g, '' );
            return valor.replace(/^\s+|\s+$/g, "");
        }

        function selecionar_orgao(valor) {
            document.formulario.formulario.value = "";
            document.formulario.submit();
        }

        function selecionar_unidade_orcamentaria() {
            document.formulario.formulario.value = "";
            document.formulario.submit();
        }

        function enviar_formulario() {
            if (validar_formulario()) {
                document.formulario.submit();
            }
        }


        function validar_formulario() {

            //alert('tamanho do nome'+ document.formulario.usunome.value.length);

            var validacao = true;
            var mensagem = 'Os seguintes campos não foram preenchidos corretamente:\n';
            if (document.formulario.sisid.value == '' || !validar_cpf(document.getElementsByName("usucpf")[0].value)) {
                // TODO: voltar para o primeiro formulário
            }

            <?php if ( !$cpf_cadastrado ): ?>
            document.formulario.usunome.value = trim(document.formulario.usunome.value);
            if (( document.formulario.usunome.value == '') || (document.formulario.usunome.value.length < 5 )) {
                mensagem += '\nNome';
                validacao = false;
            }
            if (!validar_radio(document.formulario.ususexo, 'Sexo')) {
                mensagem += '\nSexo';
                validacao = false;
            }
            if (document.formulario.regcod.value == '') {
                mensagem += '\nUnidade Federal';
                validacao = false;
            } else if (document.formulario.muncod.value == '') {
                mensagem += '\nMunicípio';
                validacao = false;
            }

            /*** Tipo do Órgão / Instituição ***/
            if (document.formulario.tpocod) {
                if (document.formulario.tpocod.value == '') {
                    mensagem += '\nTipo do Órgão / Instituição';
                    validacao = false;
                }
            }
            /*** Órgão / Instituição ***/
            if (document.formulario.entid) {
                if (document.formulario.tpocod.value != 4 && document.formulario.entid.value == '') {
                    mensagem += '\nÓrgão / Instituição';
                    validacao = false;
                }
            }
            /*** Órgão / Instituição(Outros) ***/
            if (document.formulario.orgao) {
                if (document.formulario.tpocod.value == 4 && document.formulario.orgao.value == '') {
                    mensagem += '\nÓrgão / Instituição';
                    validacao = false;
                }
            }
            /*** Se for federal, valida o preenchimento da UO e UG ***/
            if (document.formulario.tpocod) {
                if (document.formulario.tpocod.value == 1) {
                    if (document.formulario.unicod) {
                        if (document.formulario.unicod.value == '') {
                            mensagem += '\nUnidade Orçamentária';
                            validacao = false;
                        }
                    }
                    if (document.formulario.ungcod) {
                        if (document.formulario.ungcod.value == '') {
                            mensagem += '\nUnidade Gestora';
                            validacao = false;
                        }
                    }
                }
            }

            <?php if ( $uo_total > 0 ): ?>
            /*if ( document.formulario.unicod.value == '' ) {
             mensagem += '\nUnidade Orçamentária';
             validacao = false;
             }*/
            <?php endif; ?>
            /*
             if ( document.formulario.orgao ){
             document.formulario.orgao.value = trim( document.formulario.orgao.value );
             if (    document.formulario.orgao.value == '' ||
             document.formulario.orgao.value.length < 5
             )
             {
             mensagem += '\nNome do Órgão';
             validacao = false;
             }
             }*/

            if (document.formulario.entid) {
                if (document.formulario.entid.value == '390360' && document.formulario.unicod.value == '26101') {
                    if (document.formulario.ungcod.value == '') {
                        mensagem += '\nUnidade Gestora';
                        validacao = false;
                    }
                }
            }

            document.formulario.usufoneddd.value = trim(document.formulario.usufoneddd.value);
            document.formulario.usufonenum.value = trim(document.formulario.usufonenum.value);
            if (
                document.formulario.usufoneddd.value == '' ||
                document.formulario.usufonenum.value == '' ||

                document.formulario.usufoneddd.value.length < 2 ||
                document.formulario.usufonenum.value.length < 7
            ) {
                mensagem += '\nTelefone';
                validacao = false;
            }
            document.formulario.usuemail.value = trim(document.formulario.usuemail.value);
            if (!validaEmail(document.formulario.usuemail.value)) {
                mensagem += '\nEmail';
                validacao = false;
            }
            document.formulario.usuemail_c.value = trim(document.formulario.usuemail_c.value);
            if (!validaEmail(document.formulario.usuemail_c.value)) {
                mensagem += '\nConfirmação do Email';
                validacao = false;
            }
            if (validaEmail(document.formulario.usuemail.value) && validaEmail(document.formulario.usuemail_c.value) && document.formulario.usuemail.value != document.formulario.usuemail_c.value) {
                mensagem += '\nOs campos Email e Confirmação do Email não coincidem.';
                validacao = false;
            }

            if (document.formulario.carid) {
                if (document.formulario.carid.value == '') {
                    mensagem += '\nFunção/Cargo';
                    validacao = false;
                }
                else {
                    if (document.formulario.carid.value == 9) {
                        document.formulario.usufuncao.value = trim(document.formulario.usufuncao.value);
                        if (
                            document.formulario.usufuncao.value == '' ||
                            document.formulario.usufuncao.value.length < 5
                        ) {
                            mensagem += '\nFunção';
                            validacao = false;
                        }
                    }
                }
            }
            <?php endif; ?>

            if (document.formulario.pflcod) {
                if (document.formulario.pflcod.value == '') {
                    mensagem += '\nPerfil';
                    validacao = false;
                }

                // seleciona todos as ações
                var acoes = document.getElementById("proposto_A");
                if (acoes) {
                    if (acoes.options.length == 1 && acoes.options[0].value == '') {
                        mensagem += '\nAções';
                        validacao = false;
                    } else {
                        for (var i = 0; i < acoes.options.length; i++) {
                            acoes.options[i].selected = true;
                        }
                    }
                }

                // seleciona todos os programas
                var programas = document.getElementById("proposto_P");
                if (programas) {
                    if (programas.options.length == 1 && programas.options[0].value == '') {
                        mensagem += '\nProgramas';
                        validacao = false;
                    } else {
                        for (var i = 0; i < programas.options.length; i++) {
                            programas.options[i].selected = true;
                        }
                    }
                }

                // seleciona todas as unidades
                var unidades = document.getElementById("proposto_U");
                if (unidades) {
                    if (unidades.options.length == 0 && unidades.options[0].value == '') {
                        mensagem += '\nUnidades';
                        validacao = false;
                    } else {
                        for (var i = 0; i < unidades.options.length; i++) {
                            unidades.options[i].selected = true;
                        }
                    }
                }
            }

            if (!validacao) {
                alert(mensagem);
            }
            return validacao;
        }

        function alternarExibicaoCargo(tipo) {

            var carid = document.getElementById('carid');
            var usufuncao = document.getElementById('usufuncao');
            var link = document.getElementById('linkVoltar');


            if (tipo != 'exibirOpcoes') {
                if (carid.value == 9 || carid.value == '') {
                    usufuncao.style.display = "";
                    //usufuncao.className = "";
                    link.style.display = "";
                    carid.style.display = "none";
                    //link.className = "";
                }
            }
            else {
                usufuncao.style.display = "none";
                usufuncao.value = "";
                link.style.display = "none";
                //link.className = "objetoOculto";
                carid.style.display = "";
                carid.value = "";
            }
        }

    </script>

<?php include "rodape_publico.inc"; ?>