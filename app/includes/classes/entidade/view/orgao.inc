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
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan='2' class="SubTituloDireita" style="text-align:center; font-weight:bold;" >
				Dados Corporativos
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >
				CNPJ:
			</td>
			<td>
				<input type="hidden" name="entcpfcnpj_original" id="entcpfcnpj_original" value="<?=$this->entcpfcnpj;?>"/>
				<?php
				$c = new TEntry('entcpfcnpj');
				$c->setSize(150)
				  ->setMaxlength(18)
				  ->setValue( $this->entcpfcnpj )
				  ->setMascara('99.999.999/9999-99')
				  ->setOnblur((empty($this->entid) ? 'Entidade.buscarEntidadePorCNPJ(this.value);' : 'verificaCpfCnpj(this.value)'))
				  ->show();
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
				Data Fundação
			</td>
			<td>
				<?php
				$c = new TData('entdtnascimento');
				$c->setValue( $this->entdtnascimento )
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
				Referência (órgão):
			</td>
			<td>
				<input type="hidden" id="entidpai" name="entidpai" value="<?php echo $this->entidpai?>">
				<label id="texto_entidpai"><?php echo $this->entidadenomepai ?></label>
				<input type="button" name="btnrefe" value="Pesquisar" onclick="janela('?modulo=principal/popup/arvoreReferencia&acao=A&entid=' + $('#entidpai').val(),600,600,'popup_refe')">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">
				Tipo:
			</td>
			<td>
				<?php
				$c = new TCombo('tpeid');
				$c->addItems( $this->itens['tipo'] )
				  ->setValue( $this->tpeid )
				  ->show();
				?>
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
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->graulatitude )
				  ->show();
				echo "º ";
				$c = new TEntry('minlatitude');
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->minlatitude )
				  ->show();
				echo "' ";
				$c = new TEntry('seglatitude');
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
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->graulongitude )
				  ->show();
				echo "º ";
				$c = new TEntry('minlongitude');
				$c->setSize(30)
				  ->setMaxlength(2)
				  ->setValue( $this->minlongitude )
				  ->show();
				echo "' ";
				$c = new TEntry('seglongitude');
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
				$c->setMascara('(99)9999-9999')
				  ->setSize(92)
				  ->setMaxlength(15)
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
				Celular:
			</td>
			<td>
				<?php
				$c = new TEntry('entcelular');
				$c->setMascara('(99)9999-9999')
				  ->setSize(92)
				  ->setMaxlength(15)
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
				$c->setMascara('(99)9999-9999')
				  ->setSize(92)
				  ->setMaxlength(15)
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

function salvar(){
	if ( validar_campos() ) {
		document.formulario.submit();
	}
}

function validar_campos(){
	var validacao = true;
	var mensagem = 'Os seguintes campos não foram preenchidos corretamente:\n';


		if ( document.formulario.tpeid.value == "" ) {
			mensagem += '\n\tTipo';
			validacao = false;
		}

	document.formulario.entnome.value = trim( document.formulario.entnome.value );
	if ( ( document.formulario.entnome.value == '')  || (document.formulario.entnome.value.length < 5 )) {
		mensagem += '\n\tNome';
		validacao = false;
	}


	if (document.formulario.entcpfcnpj.value != '') {
		if(!validarCnpj(document.formulario.entcpfcnpj.value)){
            mensagem += '\n\tCNPJ informado não é válido.';
            validacao = false;
		}
        }

	if (document.formulario.entdtnascimento.value != '') {
		if(!validaData(document.formulario.entdtnascimento)){
			mensagem += '\n\tData fundação informada não é válida.';
            validacao = false;
		}
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

//function limpaCombo(){
//	$('#entidpai').val('-1').change();
//}

function complementa(elem, character){
	if($(elem).val().length == new Number($(elem).attr('maxlength')-1))
		$(elem).val(character + $(elem).val());
}

</script>

<?php
//if($_REQUEST['acao'] == 'B' || $_REQUEST['acao'] == 'D')
//	montaComboEntidades();
?>
