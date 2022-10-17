<?php
/**
 * Sistema Integrado de Monitoramento do Ministério da Educação
 * Setor responsvel: SPO/MEC
 * Analistas: Gilberto Arruda Cerqueira Xavier <gacx@ig.com.br>, Cristiano Cabral <cristiano.cabral@gmail.com>
 * Programadores: Renan de Lima Barbosa <renandelima@gmail.com>, Renê de Lima Barbosa <renedelima@gmail.com>
 * Módulo: Usuário
 * Finalidade: Permitir o controle de cadastro de usuários.
 * Data de criação: 19/11/2006
 * Última modificação: 21/11/2006
 */



// Carrega a combo com os orgãos do tipo selecionado
if( $_REQUEST["ajax"] == 1 ){

	// Se for estadual verifica se existe estado selecionado
	if ( $_REQUEST["tpocod"] == 2 && !$_REQUEST["regcod"]  ){
			
		echo '<font style="color:#909090;">
					Favor selecionar um Estado.
				  </font>';
		die;
			
	}

	// Se for municipal verifica se existe estado selecionado
	if ( $_REQUEST["tpocod"] == 3 && !$_REQUEST["muncod"]  ){
			
		echo '<font style="color:#909090;">
					Favor selecionar um município.
				  </font>';
		die;
			
	}

	$tpocod =  $_REQUEST["tpocod"];
	$muncod =  $_REQUEST["muncod"];

	carrega_orgao($editavel, $usucpf);
	die;

}

// Carrega a combo com as unidades do orgão selecionado
if( $_REQUEST["ajax"] == 2 ){

	carrega_unidade($_REQUEST["entid"], $editavel, $usuario->usucpf);	
	die;
	
}

// Carrega a combo com as unidades gestoras da undiade selecionada
if( $_REQUEST["ajax"] == 3 ){

	carrega_unidade_gestora($_REQUEST["unicod"], $editavel, $usuario->usucpf);
	die;
	
}


// controle o cache do navegador
header( "Cache-Control: no-store, no-cache, must-revalidate" );
header( "Cache-Control: post-check=0, pre-check=0", false );
header( "Cache-control: private, no-cache" );   
header( "Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT" );
header( "Pragma: no-cache" );



		define( 'SENHA_PADRAO', 'adm123' );

		if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'listar_mun'){	
			$sql = "SELECT muncod, mundescricao as mundsc 
				FROM territorios.municipio 
				WHERE estuf = '".$_REQUEST['estuf']."' ORDER BY mundsc";
			$dados = $db->carregar($sql);
			
			$enviar = '';
			if($dados) $dados = $dados; else $dados = array();
			foreach ($dados as $data) {
				$enviar .= "<option value= ".$data['muncod'].">  ".htmlentities($data['mundsc'])." </option> \n";
			}
					
			die($enviar);
		}
		
		$_REQUEST['acao'] = "A";
		$acao = $_REQUEST['acao'];
		$usucpf = '';
		$_REQUEST['usucpf'] = $_SESSION['usucpf'];
		if($acao == 'U'){
				
			$usucpf = $_SESSION['usucpf'];
			$titulo_pagina = "Atualização de Dados Cadastrais";
				
		}else{
				
			$usucpf = $_REQUEST['usucpf'];
			$titulo_pagina = "Cadastro de Usuários";
		}


		// carrega os dados da conta do usuário
		$sql = sprintf("SELECT 
							u.*,
							e.entid 
						FROM 
							seguranca.usuario u 
						LEFT JOIN
							entidade.entidade e ON
							u.entid = e.entid
						WHERE 
							usucpf = '%s'",
		$usucpf
		);

		$usuario = $db->pegaLinha( $sql );
        
        if(!$usuario)
        {
            $_REQUEST['acao'] = "A";
            $db->insucesso("Usuário Não Encontrado","&acao=A","sistema/usuario/consusuario");
        }

		extract( $usuario );

		$usuario = (object) $usuario;
		$cpf = formatar_cpf( $usuario->usucpf );
		
		
		
		/**
		 * Verifica se usuário só pode consultar
		 * 
		 */	
		$habilitar_edicao = 'S';
		if($acao == 'C') $habilitar_edicao = 'N';		
			
		
		/*
		 * Verifica se o usuário deve ou não visializar todos os campos
		 *
		 */		
		
		$data_atual = NULL;
		$permissao = true;		
		if( $acao == 'U') {
			$permissao = false;
			$data_atual = date("Y-m-d H:i:s");
		}
		
		$usudatanascimento = formata_data($usuario->usudatanascimento);
		//-------------------------------------------------------------------------------
		
		if ( $_REQUEST['formulario']) {
			//$tpocod_banco = $_REQUEST['tpocod'] ? (integer) $_REQUEST['tpocod'] : "null";
			
			//data de nascimento
			$dataBanco = formata_data_sql( $_REQUEST['usudatanascimento'] );
			$dataBanco = $dataBanco ? "'" . $dataBanco . "'" : "null";
			
			// Caso tenha entidade
			$entid = isset($_REQUEST['entid']) ? $_REQUEST['entid'] : 'null';
			$entid = $entid == 999999 ? 'null' : $entid; 
			
			//arrumando problema de slashes excessivos causados pela diretiva magic codes do php
			$orgao = (str_replace( "\\", "",$_REQUEST['orgao']));		
			$orgao = stripcslashes( $orgao );
			$orgao = str_replace( "'", "", $orgao );
			
			/*
			 * Integração com o SSD
			 * Atualizando os possiveis novos dados
			 * Desenvolvido por Alexandre Dourado
			 */
			if(AUTHSSD) {
				// Definindo o local dos certificados
				//define("PER_PATH", "../");
				
				include_once(APPRAIZ."www/connector.php");
				$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
				// 	Efetuando a conexão com o servidor (produção/homologação)
				if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
					$SSDWs->useProductionSSDServices();
				} else {
					$SSDWs->useHomologationSSDServices();
				}
				
				$SSD_tipo_pessoa = @utf8_encode("F");
				$SSD_nome = @utf8_encode($_POST["usunome"]);
				$SSD_cpf = @utf8_encode($usuario->usucpf);
				$SSD_data_nascimento = @utf8_encode(formata_data_sql($_POST["usudatanascimento"]));
				if($_POST['usuemailconfssd'] != $_POST["usuemail"]) {
					$SSD_email = @utf8_encode($_POST["usuemail"]);
				}
				$SSD_ddd_telefone = @utf8_encode($_POST["usufoneddd"]);
				$SSD_telefone = @utf8_encode($_POST["usufonenum"]);
				
				$userInfo = "$SSD_tipo_pessoa||$SSD_nome||$nome_mae||$SSD_cpf||$rg||$sigla_orgao_expedidor||$orgao_expedidor||$nis||" .
							"$SSD_data_nascimento||$codigo_municipio_naturalidade||$codigo_nacionalidade||$SSD_email||$email_alternativo||" .
							"$cep||$endereco||$sigla_uf_cep||$localidade||$bairro||$complemento||$endereco||$SSD_ddd_telefone||$SSD_telefone||" .
							"$ddd_telefone_alternativo||$telefone_alternativo||$ddd_celular||$celular||$instituicao_trabalho||$lotacao||" .
							"$cpf_responsavel||ssd";
				
				$resposta = $SSDWs->updateUser($userInfo);
				
				if($resposta != "true") {
					echo "<script>
							alert('".addslashes($resposta["erro"])."');
							window.location = '?modulo=sistema/usuario/consusuario&acao=A';
		  			  	  </script>";
					exit;
				}
			}
			/*
			 * FIM
			 * Integração com o SSD
			 * Atualizando os possiveis novos dados
			 * Desenvolvido por Alexandre Dourado
			 */
			
			
			if ($permissao) {
				$sql = sprintf("UPDATE seguranca.usuario SET
                                    usunome           = '".$_POST['usunome']."',
                                    usuemail          = '%s',
                                    usufoneddd        = '%s',
                                    usufonenum        = '%s',
                                    usufuncao         = '%s',
                                    carid         	  = '%s',
                                    entid             = %s,
                                    unicod            = '%s',
                                    regcod            = '%s',
                                    ungcod            = '%s',
                                    ususexo           = '%s',
                                    usudatanascimento =  %s,
                                    usunomeguerra     = '%s',
                                    muncod            = '%s',
                                    orgao             = '%s',
                                    tpocod            = '%s'
                                WHERE
                                    usucpf            = '%s'",                                
                                pg_escape_string($_REQUEST['usuemail']),
                                pg_escape_string($_REQUEST['usufoneddd']),
                                str_replace( "\\", "", substr( $_REQUEST['usufonenum'], 0, 10 ) ),
                                pg_escape_string($_REQUEST['usufuncao']),
                                pg_escape_string($_REQUEST['carid']),
                                pg_escape_string($entid),
                                pg_escape_string($_REQUEST['unicod']),
                                pg_escape_string($_REQUEST['regcod']),
                                pg_escape_string($_REQUEST['ungcod']),
                                pg_escape_string($_REQUEST['ususexo']),
                                $dataBanco,
                                pg_escape_string($_REQUEST['usunomeguerra']),
                                pg_escape_string($_REQUEST['muncod']),
                                str_replace("'","",$orgao),
                                pg_escape_string($_REQUEST['tpocod']),
                                pg_escape_string($usuario->usucpf));
			} else {
				$data_atual = $data_atual ? "'" . $data_atual . "'" : "null";
				// atualiza dados gerais do usuário
				$sql = sprintf("UPDATE seguranca.usuario SET
                                    usuemail = '%s',
                                    usufoneddd = '%s',
                                    usufonenum = '%s',
                                    usufuncao = '%s',
                                    carid = '%s',
                                    entid = '%s',
                                    unicod = '%s',
                                    regcod = '%s',
                                    ungcod = '%s',
                                    ususexo = '%s',
                                    usudatanascimento = %s,
                                    usunomeguerra = '%s',
                                    muncod = '%s',
                                    orgao = '%s',
                                    tpocod = '%s',
                                    usudataatualizacao = %s
                                WHERE
                                    usucpf = '%s'",
                                    pg_escape_string($_REQUEST['usuemail']),
                                    pg_escape_string($_REQUEST['usufoneddd']),
                                    str_replace( "\\", "", substr( $_REQUEST['usufonenum'], 0, 10 ) ),
                                    pg_escape_string($_REQUEST['usufuncao']),
                                    pg_escape_string($_REQUEST['carid']),
                                    pg_escape_string($_REQUEST['entid']),
                                    pg_escape_string($_REQUEST['unicod']),
                                    pg_escape_string($_REQUEST['regcod']),
                                    pg_escape_string($_REQUEST['ungcod']),
                                    pg_escape_string($_REQUEST['ususexo']),
                                	$dataBanco,
                                    pg_escape_string($_REQUEST['usunomeguerra']),
                                    pg_escape_string($_REQUEST['muncod']),
                                    str_replace("'", "",$orgao),
                                    pg_escape_string($_REQUEST['tpocod']),
                                    $data_atual,
                                    pg_escape_string($usuario->usucpf));

			}

			$db->executar($sql);

			// altera a senha do usuário com o valor padrão
			if ($_REQUEST['senha']) {
				
				/*
			 	 * Integração com o SSD
			 	 * Atualizando nova senha por padrão
			 	 * Desenvolvido por Alexandre Dourado
			 	 */
				
				if(AUTHSSD) {
					// 	Definindo o local dos certificados
					//define("PER_PATH", "../");
					
					include_once(APPRAIZ."www/connector.php");
					$SSDWs = new SSDWsUser($tmpDir, $clientCert, $privateKey, $privateKeyPassword, $trustedCaChain);
					// 	Efetuando a conexão com o servidor (produção/homologação)
					if ($GLOBALS['USE_PRODUCTION_SERVICES']) {
						$SSDWs->useProductionSSDServices();
					} else {
						$SSDWs->useHomologationSSDServices();
					}
					$cpfOrCnpj = $usuario->usucpf;
					$oldPassword = base64_encode(md5_decrypt_senha( $usuario->ususenha, '' ));
					$newPassword = base64_encode(SENHA_PADRAO);
					$resposta = $SSDWs->changeUserPasswordByCPFOrCNPJ($cpfOrCnpj, $oldPassword, $newPassword);
					if($resposta != "true") {
						echo "<script>
								alert('".addslashes($resposta["erro"])."');
								window.location = '?modulo=sistema/usuario/consusuario&acao=A';
		  			  	  	  </script>";
						exit;
					}
				}
				
				/*
				 * FIM
			 	 * Integração com o SSD
			 	 * Atualizando nova senha por padrão
			 	 * Desenvolvido por Alexandre Dourado
			 	 */
				
				
				$sql = sprintf("UPDATE
                                    seguranca.usuario
                                SET
                                    ususenha         = '%s',
                                    usuchaveativacao = 'f'
                                WHERE
                                    usucpf = '%s'",
                                md5_encrypt_senha(SENHA_PADRAO, ''),
                                $usucpf);

				$db->executar($sql);
			}


			$db->commit();
			$parametros = '&usucpf='. $_REQUEST['usucpf'];
			die('<script>
					alert(\'Operação realizada com sucesso!\');
					location.href = window.location;
				 </script>');
		}
		
		include_once( APPRAIZ . "www/includes/webservice/cpf.php" );
		include APPRAIZ ."includes/cabecalho.inc";
		print '<br>';


		monta_titulo( $titulo_pagina, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

		?>


<body>
<form method="post" name="formulario">
    <input type="hidden" name="formulario" value="1" />
    <input type="hidden" name="ssd" value="<? echo (($_REQUEST['ssd'])?"true":""); ?>" />
    
    <script type="text/javascript" src="../includes/prototype.js"></script>
	<script src="../includes/calendario.js"></script>
	<script language="JavaScript" src="/includes/funcoes.js"></script>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

	<?php
        if ($permissao) { // : 270
    ?>
	<tr id="tr_cpf">
		<td align='right' class="SubTituloDireita">CPF:</td>
		<td>
			<?=campo_texto( 'cpf', 'S', 'N', '', 19, 14, '###.###.###-##', '', '', '', '', 'id="cpf"', '', '', 'mostraNomeReceita(this.value)' ); ?>
			<?
				$sql = "SELECT
							count(*)
						FROM
							siape.tb_siape_cadastro_servidor
						WHERE
							nu_cpf = '".str_replace(array(".","-"),"",$cpf)."'";
				
				$qtd = $db->pegaUm($sql);
				
				if((integer)$qtd > 0) {
			 ?>
			<? } ?>
		</td>
	</tr>
	<tr id="tr_usunome">
		<td align='right' class="SubTituloDireita">Nome:</td>
		<td><?= campo_texto( 'usunome', 'S', $habilitar_edicao, '', 50, 50, '', '', '', '', '', 'id="usunome"' ); ?></td>
	</tr>
	<?php
        } // if ($permissao): 259
    ?>

	<tr>
		<td align='right' class="SubTituloDireita">Apelido:</td>
		<td><?= campo_texto( 'usunomeguerra', 'S', $habilitar_edicao, '', 20, 20, '', '' ); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Sexo:</td>
		<td><input id="sexo_masculino" type="radio" name="ususexo" value="M" <? if($habilitar_edicao == 'N' ) echo("disabled='disabled'"); ?>
		<?=($ususexo=='M'?"CHECKED":"")?>
		<?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> /> <label
			for="sexo_masculino">Masculino</label> <input id="sexo_feminino"
			type="radio" name="ususexo" value="F"
			<?=($ususexo=='F'?"CHECKED":"")?>
			<?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> /> <label
			for="sexo_feminino">Feminino</label> <?= obrigatorio(); ?></td>
	</tr>

	<tr>
	<?
		$obrigatorio = "N";
		if( ! $permissao){
			$obrigatorio = "S";
		}
	?>
		<td align='right' class="SubTituloDireita">Data de Nascimento:</td>
		<td>
			<?//= campo_texto( 'usudatanascimento', $obrigatorio, $habilitar_edicao, '', 20, 20, '##/##/####', '' ); ?>
			<?= campo_data( 'usudatanascimento', 'S', $habilitar_edicao, '', '' ); ?>
		</td>
	</tr>

	<tr>
		<td align='right' class="SubTituloDireita">Unidade Federal:</td>
		<td><?php
		$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
		$db->monta_combo( 'regcod', $sql, $habilitar_edicao, '&nbsp;', 'listar_municipios', '', '', '', 'S', 'regcod');
		?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Município:</td>
		<td>
		<div id="muncod_on" style="display: none;">
		<select id="muncod"  <? if($habilitar_edicao == 'N' ) echo("disabled='disabled'"); ?>
			name="muncod" onchange="" class="CampoEstilo"></select> <?= obrigatorio(); ?>
		</div>
		<div id="muncod_off" style="color: #909090;">A Unidade Federal
		selecionada não possui municípios.</div>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Tipo do Órgão:</td>
		<td>
			<?php
			
			$tpocod = $usuario->tpocod;

			$sql = "SELECT
						tpocod as codigo, 
						tpodsc as descricao 
					FROM 
						public.tipoorgao 
					WHERE 
						tpostatus='A'";
	
	
			$db->monta_combo("tpocod",$sql,'S','','ajax_carrega_orgao','','','170','S','tpocod');
			
			?>
		</td>
	</tr>	
	<tr>
		<td align='right' class="subtitulodireita">Órgão:</td>
		<td>
			<span id="spanOrgao">
			 	<?php
			 		$entid = $usuario->entid;
			 		
					if ( $tpocod == 3 && !empty($usuario->orgao) ){
			 			$entid = 999999;
			 		}
			 		carrega_orgao($editavel, $usuario->usucpf); 
			 	?>
			</span>
		</td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td align='right' class="subtitulodireita">Unidade Orçamentária:</td>
		<td>
			<span id="unidade"> 
				<?php
					$unicod = $usuario->unicod;
					
					if ( $entid == 'null' ){
						$entid = '';
					}
					carrega_unidade($entid, $editavel, $usuario->usucpf); 
				?>
			</span>
		</td>
	</tr>
	<tr bgcolor="#F2F2F2">
		<td align='right' class="subtitulodireita">Unidade Gestora:</td>
		<td>
			<span id="unidade_gestora"> 
				<?php
					carrega_unidade_gestora($unicod, $editavel, $usuario->usucpf); 
				?>
			</span>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Telefone (DDD) + Telefone:</td>
		<td><?= campo_texto( 'usufoneddd', 'S', $habilitar_edicao, '', 3, 2, '##', '' ); ?>
		<?= campo_texto( 'usufonenum', 'S', $habilitar_edicao, '', 18, 15, '###-####|####-####|#####-####', '' ); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">E-Mail:</td>
		<td><?= campo_texto( 'usuemail', $habilitar_edicao, $habilitar_edicao, '', 50, 100, '', '' ); ?><input type="hidden" name="usuemailconfssd" value="<? echo $usuemail; ?>"></td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Função/Cargo:</td>
		<td>

			<?php
				$sql = "select carid as codigo, cardsc as descricao from public.cargo";
				$db->monta_combo( "carid", $sql, 'S', 'Selecione', 'alternarExibicaoCargo', '', '', '', 'N', "carid", '' );
			?>
			<?= campo_texto( 'usufuncao', $habilitar_edicao, $habilitar_edicao, '', 50, 100, '','', '', '', '', 'id="usufuncao" style="display: none;"' ); ?>
			<a href="javascript: alternarExibicaoCargo( 'exibirOpcoes' )" id="linkVoltar" style="display: none;" > Exibir Opções</a>		
		</td>
	</tr>



	<?php
	if(!AUTHSSD) {
	?>
		<tr id="tr_senha">
			<td align='right' class="subtitulodireita">Senha:</td>
			<td><input id="senha" type="checkbox" name="senha" /> <label
				for="senha">Alterar a senha do usuário para a senha padrão: <b>adm123</b>.</label>
			</td>
		</tr>
	<?php
	} 
	?>	
			

			


	<tr bgcolor="#C0C0C0">
		<td width="15%">&nbsp;</td>
		<td>
			<input type="button" class="botao" name="btalterar" value="Salvar" onclick="enviar_formulario();">
			<input type="button" class="botao" name="btvoltar" value="Voltar" onclick="voltar();">
		</td>
	</tr>
	
</table>
</form>
</body>

<script type="text/javascript" defer="defer"><!--

	document.getElementById('aguarde').style.display = 'none';


	function mostraNomeReceita(cpf){

		var comp = new dCPF();
		comp.buscarDados(cpf);
		$('usunome').value = comp.dados.no_pessoa_rf;
		$('usunome').readOnly = true;
	}

	<?=$script; ?>

    var permissao = <?php echo $permissao ? 'true' : 'false' ?>;

	<?php
		//$sql = "SELECT estuf, muncod, estuf || ' - ' || mundescricao as mundsc FROM territorios.municipio ORDER BY 3 ";
	?>
	//var lista_mun = new Array();
	<?// $ultimo_cod = null; ?>
	<?// foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<?// if ( $ultimo_cod != $unidade['estuf'] ) : ?>
			//lista_mun['<?= $unidade['estuf'] ?>'] = new Array();
			<?// $ultimo_cod = $unidade['estuf']; ?>
		<?// endif; ?>
		//lista_mun['<?= $unidade['estuf'] ?>'].push( new Array( '<?= $unidade['muncod'] ?>', '<?= addslashes( trim( $unidade['mundsc'] ) ) ?>' ) );
	<? //endforeach; ?>


	<?php
		$sql =
		"select " .
			" orgcod, " .
			" unicod, " .
			" unicod || ' - ' || unidsc as unidsc " .
		" from unidade " .
		" where " .
			" unistatus = 'A' and " .
			" unitpocod = 'U' " .
		" order by orgcod, unidsc ";
	?>
	var lista_uo = new Array();
	<? $ultimo_cod = null; ?>
	<? foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<? if ( $ultimo_cod != $unidade['orgcod'] ) : ?>
			lista_uo['<?= $unidade['orgcod'] ?>'] = new Array();
			<? $ultimo_cod = $unidade['orgcod']; ?>
		<? endif; ?>
		lista_uo['<?= $unidade['orgcod'] ?>'].push( new Array( '<?= $unidade['unicod'] ?>', '<?= addslashes( trim( $unidade['unidsc'] ) ) ?>' ) );
	<? endforeach; ?>
	
	<?php
		$sql = "SELECT unicod, ungcod, ungcod||' - '||ungdsc as ungdsc FROM unidadegestora WHERE ungstatus = 'A' AND unitpocod = 'U' ORDER BY unicod, ungdsc";
	?>
	var lista_ug = new Array();
	<? $ultimo_cod = null; ?>
	<? foreach ( $db->carregar( $sql ) as $unidade ) :?>
		<? if ( $ultimo_cod != $unidade['unicod'] ) : ?>
			lista_ug['<?= $unidade['unicod'] ?>'] = new Array();
			<? $ultimo_cod = $unidade['unicod']; ?>
		<? endif; ?>
		lista_ug['<?= $unidade['unicod'] ?>'].push( new Array( '<?= $unidade['ungcod'] ?>', '<?= addslashes( trim( $unidade['ungdsc'] ) ) ?>' ) );
	<? endforeach; ?>
	



	
	function enviar_formulario(){
		if ( validar_formulario() ) {
			prepara_formulario();
			document.formulario.submit();
		}
	}

	var validar_uo = false;
	function listar_unidades_orcamentarias( orgcod )
	{
		var outros = ( orgcod == '99999' );
		document.formulario.orgao.disabled = !outros;
		var sDisplayOn = document.all ? 'block' : 'table-row';
		var sDisplayOff = 'none';
		if ( outros ) {
			document.getElementById( 'nomeorgao' ).style.display = sDisplayOn;
			document.getElementById( 'tipoorgao' ).style.display = sDisplayOn;
			document.getElementById( 'linha_uo' ).style.display = sDisplayOff;
			document.getElementById( 'linha_ug' ).style.display = sDisplayOff;
		} else {
			document.getElementById( 'nomeorgao' ).style.display = sDisplayOff;
			document.getElementById( 'tipoorgao' ).style.display = sDisplayOff;
			document.getElementById( 'linha_uo' ).style.display = sDisplayOn;
			document.getElementById( 'linha_ug' ).style.display = sDisplayOn;
		}
		
		var campo_select = document.getElementById( 'unicod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		
		var div_on = document.getElementById( 'unicod_on' );
		var div_off = document.getElementById( 'unicod_off' );
		if ( !lista_uo[orgcod] )
		{
			validar_uo = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			listar_unidades_gestoras( '' );
			return;
		}
		validar_uo = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_uo[orgcod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_uo[orgcod][i][1], lista_uo[orgcod][i][0], false, lista_uo[orgcod][i][0] == '<?= $unicod ?>' );
		}
		if ( navigator.appName == 'Microsoft Internet Explorer' ) {
			for ( i=0; i< campo_select.length; i++ )
			{
				if ( campo_select.options(i).value == '<?= $unicod ?>' ) {
					campo_select.options(i).selected = true;
				}
			}
		}
	}
	
	
	function listar_municipios( estuf )
    {
    	validar_mun = true;    
        var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );        
		div_on.style.display = 'block';
		div_off.style.display = 'none';
				
         //return new Ajax.Updater('muncod', '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=sistema/usuario/cadusuario&acao=<?=$_REQUEST['acao'] ?>',
        return new Ajax.Updater('muncod', '<?=$_SESSION['sisarquivo'] ?>.php?modulo=sistema/usuario/cadusuario&acao=<?=$_REQUEST['acao'] ?>',
         {
            method: 'post',
            parameters: '&servico=listar_mun&estuf=' + estuf,
            onComplete: function(res)
            {
           	 atualiza_mun();                    
            }
        }); 
		
    }

	function listar_municipios2( regcod )
	{
		var campo_select = document.getElementById( 'muncod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		
		var div_on = document.getElementById( 'muncod_on' );
		var div_off = document.getElementById( 'muncod_off' );

		if ( !lista_mun[regcod] )
		{
			validar_mun = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			return;
		}
		
		validar_mun = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_mun[regcod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_mun[regcod][i][1], lista_mun[regcod][i][0], false, lista_mun[regcod][i][0] == '<?= $muncod ?>' );
		}
		if ( navigator.appName == 'Microsoft Internet Explorer' ) {
			for ( i=0; i< campo_select.length; i++ )
			{
				if ( campo_select.options(i).value == '<?= $muncod ?>' ) {
					campo_select.options(i).selected = true;
				}
			}
		}
	}


	var validar_ug = false;
	function listar_unidades_gestoras( unicod )
	{
		var campo_select = document.getElementById( 'ungcod' );
		while( campo_select.options.length )
		{
			campo_select.options[0] = null;
		}
		campo_select.options[0] = new Option( '', '', false, true );
		var div_on = document.getElementById( 'ungcod_on' );
		var div_off = document.getElementById( 'ungcod_off' );
		if ( !lista_ug[unicod] )
		{
			validar_ug = false;
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			return;
		}
		validar_ug = true;
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		var j = lista_ug[unicod].length;
		for ( var i = 0; i < j; i++ )
		{
			campo_select.options[campo_select.options.length] = new Option( lista_ug[unicod][i][1], lista_ug[unicod][i][0], false, lista_ug[unicod][i][0] == '<?= $ungcod ?>' );
		}
		if ( navigator.appName == 'Microsoft Internet Explorer' ) {
			for ( i=0; i < campo_select.length; i++ )
			{
				if ( campo_select.options(i).value == '<?= $ungcod ?>' ) {
					campo_select.options(i).selected = true;
				}
			}
		}
	}

	function validar_formulario(){	
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		
		if ( document.formulario.muncod.value == '' ) {
			mensagem += '\nMunicípio';
			validacao = false;
		}
		
		if(permissao){
			if ( document.formulario.usunome.value == '' ) {
				mensagem += '\nNome';
				validacao = false;
			}
		}
		
		if ( !validar_radio( document.formulario.ususexo, 'Sexo' ) ) {
			mensagem += '\nSexo';
			validacao = false;
		}
		
		if( ! permissao){
			if ( document.formulario.usudatanascimento.value == '' ) {
				mensagem += '\nData de Nascimento';
				validacao = false;
			}
		}
				
		if ( document.formulario.regcod.value == '' ) {
			mensagem += '\nUnidade Federal';
			validacao = false;
		}
		
		if ( document.formulario.tpocod.value == '' ) {
				mensagem += '\n\tTipo do Órgão';
				validacao = false;
			}
		
		if ( document.formulario.entid ){
			if ( document.formulario.entid.value == '' ) {
				mensagem += '\nÓrgão';
				validacao = false;
			}
		} 

		if ( validar_uo == true && document.formulario.unicod.value == '' ) {
			mensagem += '\nUnidade Orçamentária';
			validacao = false;
		} else if ( validar_ug == true && document.formulario.ungcod.value == '' ) {
				mensagem += '\nUnidade Gestora';
				validacao = false;
		}
		
		if ( document.formulario.orgao ){
			document.formulario.orgao.value = trim( document.formulario.orgao.value );
			if ( document.formulario.orgao.value == '' ||
				 document.formulario.orgao.value.length < 5 ){
					mensagem += '\n\tNome do Órgão';
					validacao = false;
			}
		}
		
		if ( document.formulario.usufoneddd.value == '' || document.formulario.usufonenum.value == '' ) {
			mensagem += '\nTelefone';
			validacao = false;
		}
		
		if ( !validaEmail( document.formulario.usuemail.value ) ) {
			mensagem += '\nE-mail';
			validacao = false;
		}
		
		if ( document.formulario.carid ) {
			if ( document.formulario.carid.value == '' ) {
				mensagem += '\n\tFunção/Cargo';
				validacao = false;
			}
			else{
				if( document.formulario.carid.value == 9 ){
					if ( document.formulario.usufuncao.value == '' ) {
						mensagem += '\nFunção';
						validacao = false;
					}
				}
			}
		}		
		
		
		if ( !validacao ) {
			alert( mensagem );
		}
		
		return validacao;
	}

	function voltar(){
		window.location.href = '?modulo=sistema/usuario/consusuario&acao=<?= $_REQUEST['acao'] ?>';
	}

	<?php if ( $regcod ): ?>
		listar_municipios( '<?= $regcod ?>' );
	<?php endif; ?>
	
	
		


	
	function atualiza_mun(){
		var campo_select = document.getElementById( 'muncod' );						 		
 		for ( i=0; i< campo_select.length; i++ )
		{
			if ( campo_select.options[i].value == <?=$usuario->muncod ?> ) {			
				campo_select.options[i].selected = 1;
			}
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
	
--></script>

<?php if( $carid == 9 ){
	echo "<script>alternarExibicaoCargo( 9 );</script>";		
}?>