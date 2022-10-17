<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../../includes/jquery-mask/jquery.maskedinput-1.1.4.pack.js"></script>


<link href="../includes/uberuploadcropper/css/default.css" rel="stylesheet" type="text/css" />
<link href="../includes/uberuploadcropper/css/uploadify.css" rel="stylesheet" type="text/css" />
<link href="../includes/uberuploadcropper/scripts/Jcrop/css/jquery.Jcrop.css" rel="stylesheet" type="text/css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/uberuploadcropper/scripts/swfobject.js"></script>
<script type="text/javascript" src="../includes/uberuploadcropper/scripts/jquery.uploadify.v2.1.0.min.js"></script>
<script type="text/javascript" src="../includes/uberuploadcropper/scripts/jquery-impromptu.3.1.min.js"></script>
<script type="text/javascript" src="../includes/uberuploadcropper/scripts/Jcrop/js/jquery.Jcrop.min.js"></script>
<script type="text/javascript" src="../includes/uberuploadcropper/scripts/jquery-uberuploadcropper.js"></script>
<script type="text/javascript" src="../../includes/agendaentidades.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form method="post" id="formulario" name="formulario" >
	<input type="hidden" name="op" id="op" value="<?php echo $this->op; ?>"/>
	<input type="hidden" name="entid" id="entid" value="<?php echo $this->entid;?>"/>
	<input type="hidden" name="entcpfcnpj_original" id="entcpfcnpj_original" value="<?= $this->entcpfcnpj;?>"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan='2' class="SubTituloDireita" style="text-align:center; font-weight:bold;" >
				Dados Pessoais
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
				CPF:
			</td>
			<td>
				<?php
				$c = new TEntry('entcpfcnpj');
				$c->setSize(110)
//				  ->setMaxlength(14)
				  ->setValue( $this->entcpfcnpj )
//				  ->setMascara('###.###.###-##')
				  ->setMascara('999.999.999-99')
				  ->setOnblur((empty($this->entid) ? 'Entidade.buscarEntidadePorCPF(this.value);' : 'verificaCpfCnpj(this.value)'))
				  ->show();
				echo obrigatorio();
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Nome:
			</td>
			<td>
				<?php
				$c = new TEntry('entnome');
				$c->setSize(310)
				  ->setMaxlength(60)
				  ->setValue( $this->entnome )
				  ->show();
				echo obrigatorio();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Nome social:
			</td>
			<td>
				<?php
				$c = new TEntry('entnomesocial');
				$c->setSize(310)
				  ->setMaxlength(60)
				  ->setValue( $this->entnomesocial )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Data Nascimento
			</td>
			<td>
				<?php
				$c = new TData('entdtnascimento');
				$c->setValue( $this->entdtnascimento, false )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				RG:
			</td>
			<td>
				<?php
				$c = new TEntry('entrg');
				$c->setSize(80)
				  ->setMaxlength(10)
				  ->setValue( $this->entrg )
				  ->show();
				?>
				&nbsp;&nbsp;
				Órgão emissor:
				&nbsp;
				<?php
				$c = new TEntry('entorgaoemissorrg');
				$c->setSize(50)
				  ->setMaxlength(10)
				  ->setValue( $this->entorgaoemissorrg )
				  ->show();
				?>
				&nbsp;&nbsp;
				Data de emissão:
				&nbsp;
				<?php
				$c = new TData('entdtemissaorg');
				$c->setValue( $this->entdtemissaorg, false )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Título de Eleitor:
			</td>
			<td>
				<?php
				$c = new TEntry('enttituloeleitor');
				$c->setSize(110)
				  ->setMaxlength(14)
				  ->setValue( $this->enttituloeleitor )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				PIS/PASEP:
			</td>
			<td>
				<?php
				$c = new TEntry('entpispasep');
				$c->setSize(110)
				  ->setMascara('99999999999')
				  ->setMaxlength(11)
				  ->setValue( $this->entpispasep )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Nome Mãe:
			</td>
			<td>
				<?php
				$c = new TEntry('entnomemae');
				$c->setSize(300)
				  ->setMaxlength(60)
				  ->setValue( $this->entnomemae )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Nome Pai:
			</td>
			<td>
				<?php
				$c = new TEntry('entnomepai');
				$c->setSize(300)
				  ->setMaxlength(60)
				  ->setValue( $this->entnomepai )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Vínculo funcional
			</td>
			<td>
				<?php
					$dados = new VinculoFuncional();
					$itens = $dados->recuperarTodos('vifid AS codigo, vifdsc AS descricao', array("vifstatus = 'A'" ), 'vifdsc' );

					$c = new TCombo('vifid');
					$c->addItems($itens)
					  ->setValue( $this->vifid )
					  ->setOnChange('verificaVinculo(this.value)')
					  ->show();
				?>
			</td>
		</tr>

		<tr id="tr_siape" style="<?php echo ($this->entsiape ? '' : 'display:none;'); ?>">
			<td class="SubTituloDireita">
				Identificação Única:
			</td>
			<td>
				<?php
				$c = new TEntry('entsiape');
				$c->setSize(75)
				  ->setMaxlength(9)
				  ->setValue( $this->entsiape )
				  ->show();

				echo obrigatorio();
				?>
			</td>
		</tr>

		<tr id="tr_cargo" style="<?php echo ($this->cefid ? '' : 'display:none;'); ?>">
				<td class="SubTituloDireita">
					Cargo Efetivo:
				</td>
				<td>
					<?php
						$dados = new CargoEfetivo();
						$itens = $dados->recuperarTodos('cefid AS codigo, cefdsc AS descricao', array("cefstatus = 'A'" ), 'cefdsc' );

						$c = new TCombo('cefid');
						$c->addItems($itens)
						  ->setValue( $this->cefid )
						  ->show();

						echo obrigatorio();
					?>
				</td>
			</tr>
			<tr id="tr_origem" style="<?php echo ($this->oroid ? '' : 'display:none;'); ?>">
				<td class="SubTituloDireita">
					Órgão de Origem:
				</td>
				<td>
					<?php
						$dados = new OrgaoOrigem();
						$itens = $dados->recuperarTodos('oroid AS codigo, orodsc AS descricao', array("orostatus = 'A'" ), 'orodsc' );

						$c = new TCombo('oroid');
						$c->addItems($itens)
						  ->setValue( $this->oroid )
						  ->show();

						echo obrigatorio();
					?>
				</td>
			</tr>

		<tr>
			<td class="SubTituloDireita">
				Escolaridade:
			</td>
			<td>
				<?php
				$dados = new Escolaridade();
				$itens = $dados->recuperarTodos('escid AS codigo, escdsc AS descricao', array("escstatus = 'A'" ), 'escdsc' );
//				echo $db->monta_combo('escid', $rs, 'S', 'Selecione...', '', '', '', '', 'N', 'escid');
				$c = new TCombo('escid');
				$c->addItems($itens)
				  ->setValue( $this->escid )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Graduação:
			</td>
			<td>
				<?php
				$c = new TEntry('entgraduacao');
				$c->setSize(300)
				  ->setMaxlength(80)
				  ->setValue( $this->entgraduacao )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Sexo:
			</td>
			<td>
				<?php
				$itens =  array( array('codigo'=>'M', 'descricao'=>'Masculino'),
								 array('codigo'=>'F', 'descricao'=>'Feminino'));
				$c = new TCombo('entsexo');
				$c->addItems($itens)
				  ->setValue( $this->entsexo )
				  ->show();
				  echo obrigatorio();
				?>
			</td>
		</tr>


		<tr>
			<td class="SubTituloDireita">
				Orientação Sexual:
			</td>
			<td>
				<?php
				$dados = new OrientacaoSexual();
				$itens = $dados->recuperarTodos('orsid AS codigo, orsdsc AS descricao', array("orsstatus = 'A'" ), 'orsdsc' );
//				echo $db->monta_combo('orsid', $rs, 'S', 'Selecione...', '', '', '', '', 'N', 'orsid');
				$c = new TCombo('orsid');
				$c->addItems($itens)
				  ->setValue( $this->orsid )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Identidade de Gênero:
			</td>
			<td>
				<?php
				$dados = new IdentidadeGenero();
				$itens = $dados->recuperarTodos('idgid AS codigo, idgdsc AS descricao', array("idgstatus = 'A'" ), 'idgdsc' );
//				echo $db->monta_combo('idgid', $rs, 'S', 'Selecione...', '', '', '', '', 'N', 'idgid');
				$c = new TCombo('idgid');
				$c->addItems($itens)
				  ->setValue( $this->idgid )
				  ->show();
				?>
			</td>

		<tr>
			<td class="SubTituloDireita">
				Cor / Raça:
			</td>
			<td>
				<?php
				$dados = new CorRaca();
				$itens = $dados->recuperarTodos('corid AS codigo, cordsc AS descricao', array("corstatus = 'A'" ), 'cordsc' );
//				echo $db->monta_combo('corid', $rs, 'S', 'Selecione...', '', '', '', '', 'N', 'corid');
				$c = new TCombo('corid');
				$c->addItems($itens)
				  ->setValue( $this->corid )
				  ->show();
				?>
			</td>
		</tr>


		<tr>
			<td class="SubTituloDireita">
				Observação:
			</td>
			<td>
				<?php
					$c = new TText('entobservacao');
					$c->setValue( $this->entobservacao )
					  ->setSize(400, 60)
					  ->setMaxLength(250)
					  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Arquivo:
			</td>
			<td>
				<div style="border:2px dashed #CCCCCC; padding:2px; margin-bottom:5px; width:100px;">
				<?php if ($this->arqid): ?>
					<img id="PhotoPrev" src="../slideshow/slideshow/verimagem.php?&arqid=<?php echo $this->arqid;?>"/>
				<?php else: ?>
					<img id="PhotoPrev" src="" style="visibility:hidden; "/>
				<?php endif; ?>
				</div>
				<input type="hidden" name="arqidRefe" id="arqidRefe" value="<?php echo $this->arqid;?>" />
				<input type="hidden" name="arqid" id="arqid" value="<?php echo $this->arqid;?>" />
				<input type="file" name="UploadPhoto" id="UploadPhoto" />
			</td>
		</tr>

		<tr>
			<td colspan='2' class="SubTituloDireita" style="text-align:center; font-weight:bold;">
				Endereço
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				CEP:
			</td>
			<td>
				<?php
				$c = new TEntry('entcep');
//				$c->setMascara('#####-###')
				$c->setMascara('99999-999')
				  ->setOnBlur('Entidade.buscarEnderecoCEP(this.value);')
				  ->setSize(80)
				  ->setMaxlength(10)
				  ->setValue( $this->entcep )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Bairro:
			</td>
			<td>
				<?php
				$c = new TEntry('entbairro');
				$c->setSize(200)
				  ->setMaxlength(100)
				  ->setValue( $this->entbairro )
				  ->show();
				?>
			</td>
		</tr>

		<tr id="escolha_logradouro_id" style="display:none">
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Logradouro:
			</td>
			<td>
				<?php
				$c = new TEntry('entlog');
				$c->setSize(300)
				  ->setMaxlength(200)
				  ->setValue( $this->entlog )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Número:
			</td>
			<td>
				<?php
				$c = new TEntry('entnumero');
				$c->setSize(50)
				  ->setMaxlength(10)
				  ->setValue( $this->entnumero )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Complemento:
			</td>
			<td>
				<?php
				$c = new TEntry('entcomplemento');
				$c->setSize(300)
				  ->setMaxlength(200)
				  ->setValue( $this->entcomplemento )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Município/Estado:
			</td>
			<td>
				<input type="hidden" name="muncod" id="muncod" value="<?= $this->muncod ?>"/>
				<?php
				$c = new TEntry('cidade');
				$c->setEditable(false)
				  ->setSize(150)
				  ->setMaxlength(25)
				  ->setValue( $this->cidade )
				  ->show();

				$c = new TEntry('estuf');
				$c->setEditable(false)
				  ->setSize(50)
				  ->setMaxlength(2)
				  ->setValue( $this->estuf )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td colspan='2' class="SubTituloDireita" style="text-align:center; font-weight:bold;">
				Coordenadas Geográficas
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Latitude:
			</td>
			<td>
			<?php
				$c = new TEntry('graulatitude');
//				$c->setMascara('##')
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->graulatitude )
				  ->show();
				echo "º ";
				$c = new TEntry('minlatitude');
//				$c->setMascara('##')
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->minlatitude )
				  ->show();
				echo "' ";
				$c = new TEntry('seglatitude');
//				$c->setMascara('##')
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->seglatitude )
				  ->show();
				echo "''";

				$c = new TCombo('pololatitude');
				$c->setItemBranco(false)
				  ->addItems(array(array('codigo'=>'S','descricao'=>'S'),array('codigo'=>'N','descricao'=>'N')))
				  ->setValue( $this->pololatitude )
				  ->show();
			?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Longitude:
			</td>
			<td>
			<?php
				$c = new TEntry('graulongitude');
//				$c->setMascara('##')
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->graulongitude )
				  ->show();
				echo "º ";
				$c = new TEntry('minlongitude');
//				$c->setMascara('##')
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->minlongitude )
				  ->show();
				echo "' ";
				$c = new TEntry('seglongitude');
//				$c->setMascara('##')
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->seglongitude )
				  ->show();
				echo "'' &nbsp; W";
			?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita"></td>
			<td><a href="javascript:abreMapa();" >Visualizar / Buscar No Mapa</a>
			<input type="hidden" name="entzoom" id="entzoom" value="<?= $entzoom;?>"/>
			</td>
		</tr>

		<tr>
			<td colspan='2' class="SubTituloDireita" style="text-align:center; font-weight:bold;">
				Contato
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Telefone:
			</td>
			<td>
				<?php
				$c = new TEntry('enttelfone');
				$c//->setOnKeyUp("complementa(this,'(')")
//				  ->setMascara('(##)####-####')
				  ->setMascara('(99)9999-9999')
				  ->setSize(100)
				  ->setMaxlength(13)
				  ->setValue( $this->enttelfone )
				  ->show();
				?>
				&nbsp;&nbsp;
				Ramal:
				&nbsp;
				<?php
				$c = new TEntry('entramal');
				$c->setSize(60)
				  ->setMaxlength(4)
				  ->setValue( $this->entramal )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Telefone Profissional:
			</td>
			<td>
				<?php
				$c = new TEntry('enttelfoneprofissional');
				$c//->setOnKeyUp("complementa(this,'(')")
//				  ->setMascara('(##)####-####')
				  ->setMascara('(99)9999-9999')
				  ->setSize(100)
				  ->setMaxlength(13)
				  ->setValue( $this->enttelfoneprofissional )
				  ->show();
				echo obrigatorio();
				?>
				&nbsp;&nbsp;
				Ramal:
				&nbsp;
				<?php
				$c = new TEntry('entramalprofissional');
				$c->setSize(60)
				  ->setMaxlength(4)
				  ->setValue( $this->entramalprofissional )
				  ->show();
				echo obrigatorio();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Celular:
			</td>
			<td>
				<?php
				$c = new TEntry('entcelular');
//				$c->setMascara('(##)####-####')
				$c->setMascara('(99)9999-9999')
				  ->setSize(90)
				  ->setMaxlength(13)
				  ->setValue( $this->entcelular )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Fax:
			</td>
			<td>
				<?php
				$c = new TEntry('entfax');
//				$c->setMascara('(##)####-####')
				$c->setMascara('(99)9999-9999')
				  ->setSize(90)
				  ->setMaxlength(13)
				  ->setValue( $this->entfax )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Email:
			</td>
			<td>
				<?php
				$c = new TEntry('entemail');
				$c->setSize(250)
				  ->setMaxlength(60)
				  ->setValue( $this->entemail )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Confirme o Email:
			</td>
			<td>
				<?php
				$c = new TEntry('entemail_confirmacao');
				$c->setSize(250)
				  ->setMaxlength(60)
				  ->setValue( $this->entemail_confirmacao )
				  ->show();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Email Profissional:
			</td>
			<td>
				<?php
				$c = new TEntry('entemailprofissional');
				$c->setSize(250)
				  ->setMaxlength(60)
				  ->setValue( $this->entemailprofissional )
				  ->show();
				echo obrigatorio();
				?>
			</td>
		</tr>

		<tr>
			<td class="SubTituloDireita">
				Confirme o Email Profissional:
			</td>
			<td>
				<?php
				$c = new TEntry('entemail_confirmacao_profissional');
				$c->setSize(250)
				  ->setMaxlength(60)
				  ->setValue( $this->entemail_confirmacao_profissional )
				  ->show();
				echo obrigatorio();
				?>
			</td>
		</tr>

		<tr bgcolor="#CCCCCC">
			<td>
				&nbsp;
			</td>
			<td>
				<input type="button" class="botao" value="Salvar" name="btnSalvar" onclick="salvar();">
				<input type="button" class="botao" value="Novo" name="btnNovo" onclick=""/>
			</td>
		</tr>

	</table>
</form>
<script type="text/javascript">

	var Entidade = new Entidade();

//	function listar_municipios( estuf )
//		{
//			if (estuf == ''){
//				$('#mun_on').css({display : 'none'});
//				$('#mun_off').css({display : 'block'});
//				return false;
//			}
//			$('#mun_on').css({display : 'block'});
//			$('#mun_off').css({display : 'none'});
//			var url = location.href;
//			url += '&ajax=1&estuf=' + estuf;
//			$('#mun_on').load(url);
//		}

	function salvar(){
		if ( validar_campos() ) {
//			document.formulario.act.value = 'salvar';
			document.formulario.submit();
		}
	}

	function validar_campos(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos corretamente:\n';

		document.formulario.entcpfcnpj.value = trim( document.formulario.entcpfcnpj.value );
		if ( ( document.formulario.entcpfcnpj.value == '') ) {
			mensagem += '\n\tCPF';
			validacao = false;
		}

		document.formulario.entnome.value = trim( document.formulario.entnome.value );
		if ( ( document.formulario.entnome.value == '')  || (document.formulario.entnome.value.length < 5 )) {
			mensagem += '\n\tNome';
			validacao = false;
		}

		if ( $('[name=servidor]:checked').val() == 's' && $('[name=entsiape]').val() == '' ) {
			mensagem += '\n\tMatrícula SIAPE';
			validacao = false;
		}

		if( verificaVinculo($('#vifid').val()) ){
			if(document.formulario.entsiape.value == ''){
				mensagem += '\n\tMatrícula SIAPE';
				validacao = false;
			}
		}

		if( verificaServidorPublico($('#vifid').val()) ){
			if($('#cefid').val() == ''){
				mensagem += '\n\tCargo Efetivo';
				validacao = false;
			}
			if($('#oroid').val() == ''){
				mensagem += '\n\tÓrgão Origem';
				validacao = false;
			}
		}

		if ( ( document.formulario.entsexo.value == '') ) {
			mensagem += '\n\tSexo';
			validacao = false;
		}

		if (document.formulario.entcpfcnpj.value != '') {
			if(!validar_cpf(document.formulario.entcpfcnpj.value)){
	            mensagem += '\n\tCPF informado não é válido.';
	            validacao = false;
			}
        }

		if (document.formulario.entdtnascimento.value != '') {
			if(!validaData(document.formulario.entdtnascimento)){
				mensagem += '\n\tData de nascimento informada não é válida.';
	            validacao = false;
			}
		}

		if (document.formulario.entdtemissaorg.value != '') {
			if(!validaData(document.formulario.entdtemissaorg)){
				mensagem += '\n\tData de emissão informada não é válida.';
	            validacao = false;
			}
		}

        document.formulario.enttelfoneprofissional.value = trim( document.formulario.enttelfoneprofissional.value );
        if ( ( document.formulario.enttelfoneprofissional.value == '')  || (document.formulario.enttelfoneprofissional.value.length < 13 )) {
            mensagem += '\n\tTelefone Profissional';
            validacao = false;
        }

        document.formulario.entramalprofissional.value = trim( document.formulario.entramalprofissional.value );
        if ( ( document.formulario.entramalprofissional.value == '')) {
            mensagem += '\n\tRamal Profissional';
            validacao = false;
        }

		document.formulario.entemail.value = trim( document.formulario.entemail.value );
		if (document.formulario.entemail.value != ''){
			if ( !validaEmail( document.formulario.entemail.value ) ) {
				mensagem += '\n\tEmail';
				validacao = false;
			}
			document.formulario.entemail_confirmacao.value = trim( document.formulario.entemail_confirmacao.value );
			if ( !validaEmail( document.formulario.entemail_confirmacao.value ) ) {
				mensagem += '\n\tConfirmação do Email';
				validacao = false;
			}
			if ( validaEmail( document.formulario.entemail.value ) && validaEmail( document.formulario.entemail_confirmacao.value ) && document.formulario.entemail.value != document.formulario.entemail_confirmacao.value ) {
				mensagem += '\n\tOs campos Email e Confirmação do Email não coincidem.';
				validacao = false;
			}
		}

		document.formulario.entemailprofissional.value = trim( document.formulario.entemailprofissional.value );
		if ( !validaEmail( document.formulario.entemailprofissional.value ) ) {
			mensagem += '\n\tEmail Profissional';
			validacao = false;
		}
		document.formulario.entemail_confirmacao_profissional.value = trim( document.formulario.entemail_confirmacao_profissional.value );
		if ( !validaEmail( document.formulario.entemail_confirmacao_profissional.value ) ) {
			mensagem += '\n\tConfirmação do Email Profissional';
			validacao = false;
		}
		if ( validaEmail( document.formulario.entemailprofissional.value ) && validaEmail( document.formulario.entemail_confirmacao_profissional.value ) && document.formulario.entemailprofissional.value != document.formulario.entemail_confirmacao_profissional.value ) {
			mensagem += '\n\tOs campos Email Profissional e Confirmação do Email Profissional não coincidem.';
			validacao = false;
		}

		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

//	var obDepAjax = new CarregaDependenciaAjax();
//	obDepAjax.paramPadrao({ajax:1});
//	obDepAjax.addDependencia('entidpai', 'entidpai_1', {nivel:'1', padrao:'entidpai'}, {posicaoHTML:'depois', refHTML:'entidpai_tr'});


function verificaCpfCnpj(entcpfcnpj){
	if(entcpfcnpj && entcpfcnpj != $('#entcpfcnpj_original').val()){
		var url = location.href;
//		url += '&ajax=1&entcpfcnpj=' + entcpfcnpj;
		$.ajax({url		 : url,
			    data	 : {op:'ajax', entcpfcnpj:entcpfcnpj},
			    type 	 : "POST",
			    async	 : false,
			    success  : function(data)
			    	{
	    				if(data){
							alert(data);
							$('#entcpfcnpj').val($('#entcpfcnpj_original').val());
	    				}
						//eval(data);
					}
			});
	}
}
//
//	function montaComboEntidade(){
//		limpaCombo();
//		entid = $('#entid').val();
//		if(entid){
//			var url = location.href;
//			url += '&ajax=1&entidcombos=' + entid;
//			$.ajax({url		 : url,
//				    type 	 : "POST",
//				    async	 : false,
//				    success  : function(data)
//				    	{
//							eval(data);
//						}
//				});
//		}
//	}


	function abreMapa(){
		var graulatitude = window.document.getElementById("graulatitude").value;
		var minlatitude  = window.document.getElementById("minlatitude").value;
		var seglatitude  = window.document.getElementById("seglatitude").value;
		var pololatitude = window.document.getElementById("pololatitude").value;

		var graulongitude = window.document.getElementById("graulongitude").value;
		var minlongitude  = window.document.getElementById("minlongitude").value;
		var seglongitude  = window.document.getElementById("seglongitude").value;

		var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
		var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
		var entid = document.getElementById("entid").value;
		var janela=window.open('agenda.php?modulo=principal/mapa&acao=A&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&entid='+entid, 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

	}

//	function limpaCombo(){
//		$('#entidpai').val('-1').change();
//	}

	$(function() {

		$('#UploadPhoto').uberuploadcropper({
			//---------------------------------------------------
			// uploadify options..
			//---------------------------------------------------
			'uploader'  : '../includes/uberuploadcropper/scripts/uploadify.swf',
			'script'    : '/geral/upload/upload.php',
			'cancelImg' : '../includes/uberuploadcropper/cancel.png',
			'multi'     : false,
			'auto'      : true,
			'folder'    : '/geral/upload/temp/',
			'fileDesc'  : 'JPG/PNG/GIF',
			'fileExt'   : '*.jpg;*.jpeg;*.png;*.gif',
			'buttonText': 'Foto',
			//---------------------------------------------------
			//now the cropper options..
			//---------------------------------------------------
			//'aspectRatio': 1,
			'allowSelect': false,			//can reselect
			'allowResize' : true,			//can resize selection
			'setSelect': [ 0, 0, 300, 300 ],	//these are the dimensions of the crop box x1,y1,x2,y2
			//'minSize': [ 100, 100 ],		//if you want to be able to resize, use these
			//'maxSize': [ 100, 100 ],
			/*
			onError: function (a, b, c, d) {
				if (d.status == 404)
					alert('Could not find upload script. Use a path relative to: '+'<?= getcwd() ?>');
				else if (d.type === "HTTP")
					alert('error '+d.type+": "+d.status);
				else if (d.type ==="File Size")
					alert(c.name+' '+d.type+' Limit: '+Math.round(d.sizeLimit/1024)+'KB');
				else
					alert('error '+d.type+": "+d.text);
			},*/
			//---------------------------------------------------
			//now the uber options..
			//---------------------------------------------------
			'cropScript': '/geral/upload/crop.php',
			'onComplete': function(imgs,data)
			{
				if(data == '{arqid:}'){
					alert('O tipo da imagem não é suportado pelo sistema.\nPor favor escolha apenas imagens do tipo JPG, PNG ou GIF.');
				}else{
					var data = eval('('+data+');');
					$('#arqid').val(data.arqid);
					$('#PhotoPrev').attr('src', '../slideshow/slideshow/verimagem.php?&arqid=' + data.arqid)
								   .css({visibility:'visible'});
				}
			}
		});

	});

	function complementa(elem, character){
		if($(elem).val().length == new Number($(elem).attr('maxlength')-1))
			$(elem).val(character + $(elem).val());
	}

	function verificaVinculo(value){
		if( (verificaServidorPublico(value) )
		 || (value == <?= VINCULO_CARGO_COMISSIONADO ?>)
		 || (value == <?= VINCULO_ESTAGIARIO ?>) ){
			$('#tr_siape').show();
			return true;
		}else{
			$('#tr_siape').hide();
			$('#entsiape').val('');
			return false;
		}

	}

	function verificaServidorPublico(value){
		if( value == <?= VINCULO_SERVIDOR_PUBLICO ?>){
			$('#tr_cargo').show();
			$('#tr_origem').show();
			return true;
		}else{
			$('#tr_cargo').hide();
			$('#tr_origem').hide();
			$('#cefid').val('');
			$('#oroid').val('');
			return false;
		}
	}

</script>