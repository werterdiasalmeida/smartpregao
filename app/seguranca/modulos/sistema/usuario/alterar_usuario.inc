<?php

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

	$_SESSION["cadastro"]["tpocod"] =  $_REQUEST["tpocod"];
	$_SESSION["cadastro"]["muncod"] =  $_REQUEST["muncod"];

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


		define( 'SENHA_PADRAO', 'adm123' );
		//implementando ação do ajax de montagem do combo de municipios
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
		
		$acao = $_REQUEST['acao'];
		$usucpf = '';

		if($acao == 'U'){
				
			$usucpf = $_SESSION['usucpf'];
			
			$titulo_pagina = "Atualização de Dados Pessoais";
				
		}else{
				
			$usucpf = $_REQUEST['usucpf'];
			$titulo_pagina = "Cadastro de Usuários";
		}


		// carrega os dados da conta do usuário
		$sql = sprintf(
		"SELECT u.* FROM seguranca.usuario u WHERE usucpf = '%s'",
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
			$orgao = $_REQUEST['orgao'];			
			$orgao = stripcslashes( $orgao );
			$orgao = str_replace( "'", "\'", $orgao );
			
		
			$data_atual = $data_atual ? "'" . $data_atual . "'" : "null";
			$sql = sprintf("UPDATE seguranca.usuario SET
                                  	usunome           = '".$_POST['usunome']."',
                                    usuemail          = '%s',
                                    usufoneddd        = '%s',
                                    usufonenum        = '%s',
                                    usufuncao         = '%s',
                                    entid             = %s,
                                    unicod            = '%s',
                                    regcod            = '%s',
                                    ungcod            = '%s',
                                    ususexo           = '%s',
                                    usudatanascimento =  %s,
                                    usunomeguerra     = '%s',
                                    muncod            = '%s',
                                    orgao             = '%s',
                                    tpocod            =  %s,
                                    usudataatualizacao = %s
                                WHERE
                                    usucpf            = '%s'",                                
                                pg_escape_string($_REQUEST['usuemail']),
                                pg_escape_string($_REQUEST['usufoneddd']),
                                pg_escape_string($_REQUEST['usufonenum']),
                                pg_escape_string($_REQUEST['usufuncao']),
                                $entid,
                                pg_escape_string($_REQUEST['unicod']),
                                pg_escape_string($_REQUEST['regcod']),
                                pg_escape_string($_REQUEST['ungcod']),
                                pg_escape_string($_REQUEST['ususexo']),
                                $dataBanco,
                                pg_escape_string($_REQUEST['usunomeguerra']),
                                pg_escape_string($_REQUEST['muncod']),
                                $orgao,
                                pg_escape_string($_REQUEST['tpocod']),
                                $data_atual,
                                pg_escape_string($usuario->usucpf));                               
			

			$db->executar($sql);
			
			
			$db->commit();
			$parametros = '&usucpf='. $_REQUEST['usucpf'];
			$db->sucesso( $modulo, $parametros );
			
			// altera a senha do usuário com o valor padrão
			if ($_REQUEST['senha']) {
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

		}

		include APPRAIZ ."includes/cabecalho.inc";
		print '<br>';


		monta_titulo( $titulo_pagina, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );

		?>


<body>

<form method="post" name="formulario">
    <input type="hidden" name="formulario" value="1" />
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">


	<tr id="tr_cpf">
		<td align='right' class="SubTituloDireita">CPF:</td>
		<td><?= campo_texto( 'cpf', 'S', 'N', '', 19, 14, '###.###.###-##', '' ); ?></td>
	</tr>
	<tr id="tr_usunome">
		<td align='right' class="SubTituloDireita">Nome:</td>
		<td><?= campo_texto( 'usunome', 'S', 'N', '', 50, 50, '', '' ); ?></td>
	</tr>
	

	<tr>
		<td align='right' class="SubTituloDireita">Apelido:</td>
		<td><?= campo_texto( 'usunomeguerra', 'N', $habilitar_edicao, '', 20, 20, '', '' ); ?></td>
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
		<?= campo_texto( 'usudatanascimento', $obrigatorio, $habilitar_edicao, '', 20, 20, '##/##/####', '' ); ?></td>
	</tr>

	<tr>
		<td align='right' class="SubTituloDireita">Unidade Federal:</td>
		<td><?php
		$sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
		$db->monta_combo( 'regcod', $sql, $habilitar_edicao, '&nbsp;', 'listar_municipios', '', '', '', '', 'regcod' );
		?> <?= obrigatorio(); ?></td>
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
						public.tipoorgao";
	
	
			$db->monta_combo("tpocod",$sql,'S','','ajax_carrega_orgao','','','170','S','tpocod');
			
			?>
		</td>
	</tr>	
	<tr>
		<td align='right' class="subtitulodireita">Órgão:</td>
		<td>
			<span id="spanOrgao">
			 	<?php
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
		<?= campo_texto( 'usufonenum', 'S', $habilitar_edicao, '', 18, 15, '###-####|####-####', '' ); ?>
		</td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">E-Mail:</td>
		<td><?= campo_texto( 'usuemail', $habilitar_edicao, $habilitar_edicao, '', 50, 100, '', '' ); ?></td>
	</tr>
	<tr>
		<td align='right' class="subtitulodireita">Função/Cargo:</td>
		<td><?= campo_texto( 'usufuncao', $habilitar_edicao, $habilitar_edicao, '', 50, 100, '', '' ); ?>
		</td>
	</tr>

	<?php
	    if( ($permissao == true) && ($habilitar_edicao == 'S') ) {
	?>

	<tr id="tr_senha">
		<td align='right' class="subtitulodireita">Senha:</td>
		<td><input id="senha" type="checkbox" name="senha" /> <label
			for="senha">Alterar a senha do usuário para a senha padrão: <b>adm123</b>.</label>
		</td>
	</tr>
	<?php if( in_array( 'geral', $configuracao ) ): ?>
	<tr id="tr_status">
		<td align='right' class="SubTituloDireita">Status Geral:</td>
		<td><input id="status_ativo" type="radio" name="suscod" value="A"
			onchange="alterar_status_geral();"
			<?= $suscod == 'A' ? 'checked="checked"' : "" ?> /> <label
			for="status_ativo">Ativo</label> <input id="status_pendente"
			type="radio" name="suscod" value="P"
			onchange="alterar_status_geral();"
			<?= $suscod == 'P' ? 'checked="checked"' : "" ?> /> <label
			for="status_pendente">Pendente</label> <input id="status_bloqueado"
			type="radio" name="suscod" value="B"
			onchange="alterar_status_geral();"
			<?= $suscod == 'B' ? 'checked="checked"' : "" ?> /> <label
			for="status_bloqueado">Bloqueado</label>
		&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a
			href="javascript: exibir_ocultar_historico('historico_geral');"><img
			src="/imagens/mais.gif" style="border: 0" /> histórico</a>
		<div id="historico_geral" style="width: 500px; display: none">
		<p><?php
		$cabecalho = array(
								"Data",
								"Status",
								"Descrição",
		);
		$sql = sprintf(
								"SELECT to_char( hu.htudata, 'dd/mm/YYYY' ) as data, hu.suscod, hu.htudsc FROM seguranca.historicousuario hu WHERE usucpf = '%s' AND sisid IS NULL ORDER BY hu.htudata DESC",
		$usucpf
		);
		$db->monta_lista_simples( $sql, $cabecalho, 25, 0 );
		?></p>
		</div>
		</td>
	</tr>
	<tr id="tr_justificativa">
		<td align='right' class="SubTituloDireita">Justificativa:</td>
		<td>
		<div id="justificativa_on" style="display: none;"><?= campo_textarea( 'htudsc', 'N', 'S', '', 100, 3, '' ); ?>
		</div>
		<div id="justificativa_off" style="display: block; color: #909090;">
		Status não alterado.</div>
		</td>
	</tr>
	<?php else: ?>
	<tr id="tr_status_geral">
		<td align='right' class="SubTituloDireita">Status Geral:</td>
		<td><?= $status[$suscod] ?></td>
	</tr>
	<?php endif; ?>



	<?php
	}	
	?>

	<tr bgcolor="#C0C0C0">
		<td width="15%">&nbsp;</td>
		<td>
	
		<input type="button" class="botao" name="btalterar" value="Salvar"
			onclick="enviar_formulario();">
		
	</tr>
	
</table>
</form>
</body>

<script type="text/javascript" defer><!--

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
				
         //return new Ajax.Updater('muncod', '<?=$_SESSION['sisdiretorio'] ?>.php?modulo=sistema/usuario/alterar_usuario&acao=<?=$_REQUEST['acao'] ?>',
        return new Ajax.Updater('muncod', '<?=$_SESSION['sisarquivo'] ?>.php?modulo=sistema/usuario/alterar_usuario&acao=<?=$_REQUEST['acao'] ?>',
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
		
		if( !permissao ){
			if ( document.formulario.usudatanascimento.value == '' ) {
				mensagem += '\nData de Nascimento';
				validacao = false;
			}
			
		}
		if( (document.formulario.usudatanascimento.value != '') && (!validaData(document.formulario.usudatanascimento)) ){
				alert('Data inválida');
				document.formulario.usudatanascimento.focus();
				return false;
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
		if ( document.formulario.usufuncao.value == '' ) {
			mensagem += '\nFunção';
			validacao = false;
		}
				
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

	function voltar(){
		window.history.back();
	}

	<?php if ( $regcod ): ?>
		listar_municipios( '<?= $regcod ?>' );
	<?php endif; ?>	
	

	function exibir_ocultar_historico( id ){
		div = document.getElementById( id );
		if ( div.style.display == 'none' ) {
			div.style.display = 'block';
		} else {
			div.style.display = 'none'
		}
	}
	
	function atualiza_mun(){
		var campo_select = document.getElementById( 'muncod' );						 		
 		for ( i=0; i< campo_select.length; i++ )
		{
			if ( campo_select.options[i].value == <?=$usuario->muncod ?> ) {			
				campo_select.options[i].selected = 1;
			}
		}
	}
--></script>
