//
// $Id$
//

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
	var entid = window.document.getElementById("entid").value;
	var janela=window.open('../apigoogle/php/mapa_padrao.php?longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&entid='+entid, 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();

}



var PESSOA_JURIDICA = 0;
var PESSOA_FISICA   = 1;


/**
 * 
 */
var Entidade = Class.create(
{
    //------------------------------------------------------- propriedades


    //------------------------------------------------------------- public
    /**
     * constructor
     */
    initialize: function (url, entidade, camposObrigatorios)
    {
        this._entidadePrototype = {
            entid:                  '',
            funid:                  '',
            njuid:                  '',
            entidassociado:         '',
            entnumcpfcnpj:          '',
            entnome:                '',
            entemail:               '',
            entnuninsest:           '',
            entobs:                 '',
            entstatus:              '',
            entnumrg:               '',
            entorgaoexpedidor:      '',
            entsexo:                '',
            entdatanasc:            '',
            entdatainiass:          '',
            entdatafimass:          '',
            entnumdddresidencial:   '',
            entnumresidencial:      '',
            entnumdddcomercial:     '',
            entnumramalcomercial:   '',
            entnumcomercial:        '',
            entnumdddfax:           '',
            entnumramalfax:         '',
            entnumfax:              '',
            tpctgid:                '',
            tpcid:                  '',
            tplid:                  '',
            tpsid:                  '',
            enderecos:               [{
                endid: '',
                entid: '',
                tpeid: '',
                endcep: '',
                endlog: '',
                endcom: '',
                endbai: '',
                muncod: '',
                estuf: '',
                endnum: '',
                endstatus: ''
            }]
        };

        this._camposObrigatorios = ['entnome', 'entnumdddcomercial', 'entnumcomercial', 'endcep'];
        this._entidassociado     = '',
        this._entidades          = [];
        this._removidos          = [];
        this._entidade           = entidade;
        this._total              = 0;
        this._url                = url;
    },


    /**
     * 
     */
    setEntidade: function (entidade)
    {
        this._entidade = entidade;
    },


    /**
     * 
     */
    setFormAction: function (url)
    {
        this._url = url;
    },


    /**
     * 
     */
    setCamposObrigatorios: function (camposObrigatorios)
    {
        this._camposObrigatorios = camposObrigatorios;
    },


    /**
     * 
     */
    setAssociacao: function (entidassociado)
    {
        this._entidassociado = entidassociado;
    },


    /**
     * 
     */
    adicionarEndereco: function (endereco)
    {
        if (endereco.endcep)
            this._entidade.enderecos[this._entidade.enderecos.length] = endereco;
    },


    /**
     * Carrega as entidades já gravadas no banco de dados.
     *
     * @param JSON Array JSON contendo as entidades a serem carregadas
     */
    carregarEntidade: function (JSon)
    {
        this._entidadePrototype = JSon;

        for (i in this._entidadePrototype) {
            if ($(i))
                $(i).value = this._entidadePrototype[i];
        }
    },


    /**
     * Salva os registros de uma entidade. O backend em PHP se responsabilizará
     * pela função a ser executada (INSERT ou UPDATE).
     *
     * @param Object Formulario do qual as entidades serao enviadas
     * @param Array Array contendo todos os itens marcados como
     *      obrigatórios no formulário
     * @return void
     */
    salvarEntidade: function (frmEntidade, camposObrigatorios)
    {
        if (this.validateForm(frmEntidade, camposObrigatorios)) {
            if ($('entidassociado')) {
                if (this._entidassociado)
                    $('entidassociado').value = this._entidassociado;
                else if (this._entidade && this._entidade.entidassociado)
                    $('entidassociado').value = this._entidade.entidassociado;
                else
                    $('entidassociado').value = '';
            }

            //if ($('muncod'))
            //    $('muncod').enable();

            return true;
        }

        return false;
    },


    /**
     * Exclui uma entidade e todas as suas dependencias
     *
     * @param integer entid ID da entidade a ser excluída
     * @return void
     */
    excluirEntidade: function (entid)
    {
    },


    /**
     * 
     */
    validarData: function (dt)
    {
        if (!validaData(dt)) {
            alert('Data inválida!');
            return false;
        } else {
            return true;
        }
    },


    /**
     * @todo Fazer validação do dígito verificador
     */
    validarCpfCnpj: function (val)
    {
        return val.length == 18 || val.length == 14;
    },


    /**
     * 
     */
    inlineAddEntidade: function (tipoEntidade, container, entid)
    {
        if (!entid) entid = '';

        if (!tipoEntidade || tipoEntidade == 0) {
            this._inlineFormEntidadePessoaJuridica(container, entid);
        } else {
            this._inlineFormEntidadePessoaFisica(container, entid);
        }
    },


    /**
     * 
     */
    buildInlineForm: function (container, tipoEntidade)
    {
        var form = ''
                 + '<form id="frmEntidade" method="post" action="' + this._url + '">'
                 + '  <div style="margin: 0; padding: 0; height: 200px; width: 100%; overflow-x: auto; overflow-y: scroll;" id="frmEntidadeContainer">'
                 + '    <table class="tabela" bgcolor="#fafafa" cellSpacing="1" cellPadding="3" align="center">'
                 + '      <tbody id="tableEntidade">'
                 + '      </tbody>'
                 + '    </table>'
                 + '  </div>'
                 + '  <div id="frmEntidadeIncluir">'
                 + '    <span class="labelIncluirEntidadePessoaFisica"><input type="button" id="buttonIncluirEntidadePessoaFisica" onclick="return Entidade.inlineAddEntidade(PESSOA_FISICA, \'tableEntidade\');" title="Adicionar formulário para cadastro de pessoa física." value="Adicionar" /></span>'
                 + '    <span class="labelIncluirEntidadePessoaJuridica"><input type="button" id="buttonIncluirEntidadePessoaJuridica" href="#" onclick="return Entidade.inlineAddEntidade(PESSOA_JURIDICA, \'tableEntidade\');" title="Adicionar formulário para cadastro de pessoa jurídica." value="Adicionar" /></span>'
                 + '    <input id="buttonSubmitFrmEntidade" type="submit" value="Salvar" />'
                 + '  </span>'
                 + '</form>';

        Element.insert(container, {bottom: form});
    },


    /**
     * 
     */
    validateForm: function (frmEntidade, camposObrigatorios)
    {
        var errors      = 0;
        var frmElements = Form.getElements(frmEntidade);

        if (camposObrigatorios)
            this._camposObrigatorios = camposObrigatorios;

        for (var i = 0; i < frmElements.length; i++) {
            frmElements[i].style.backgroundColor = '#fff';

            if (this._camposObrigatorios.length > 0) {
                for (var z = 0; z < this._camposObrigatorios.length; z++) {
                    if (frmElements[i].getAttribute('id') == this._camposObrigatorios[z] && trim(frmElements[i].value) == '') {
                        frmElements[i].style.backgroundColor = '#ffd';
                        errors++;
                    }
                }
            } else {
                if (frmElements[i].getAttribute('type') != 'hidden' && trim(frmElements[i].value) == '') {
                    frmElements[i].style.backgroundColor = '#ffd';
                    errors++;
                }
            }
        }

        if (errors > 0) {
            alert('Favor preencher os campos em amarelo.\nCampos obrigatórios!');
            return false;
        }

        return true;
    },


    //---------------------------------------------------------- protected
    /**
     * 
     */
    _removerEntidade: function (entid)
    {
        var tr = $('__ent_' + entid);
        Element.remove(tr);

        this._removidos.push(entid);
    },


    /**
     * 
     */
    _inlineFormEntidadePessoaFisica: function (container, index, entid)
    {
        if (!$(container)) {
            alert('O container informado não existe.');
            return false;
        }

        var newTr = $(container).insertRow($(container).numRows);
            newTr.setAttribute('id', '__ent_' + this._total);

        if (!index)
            index = this._total;

        if (!this._entidades[index]) {
            this._entidades[index] = this._entidadePrototype;
        }

        var inline = ''
                   + '  <td>\n'
                   + '    <a href="#" onclick="return Entidade._removerEntidade(' + this._total + ');">Excluir</a>\n'
                   + '  </td>\n'
                   + '  <td align="right" class="SubTituloDireita entnumcpfcnpj"><label>CPF:</label></td>\n'
                   + '  <td>\n'
                   + '    <input maxlength="14" class="CampoEstilo entnumcpfcnpj" type="text" name="entnumcpfcnpj[]" id="entnumcpfcnpj_' + this._total + '" value="' + this._entidades[index].entnumcpfcnpj + '" onkeyup="this.value = mascaraglobal(\'###.###.###-##\', this.value);" />\n'
                   + '  </td>\n'
                   + '  <td align="right" class="SubTituloDireita entnome"><label>Nome:</label></td>\n'
                   + '  <td>\n'
                   + '    <input maxlength="99" class="CampoEstilo entnome" type="text" name="entnome[]" id="entnome_' + this._total + '" value="' + this._entidades[index].entnome + '" style="width: 250px;"/>\n'
                   + '  </td>\n'
                   + '  <td align="right" class="SubTituloDireita entdatanasc"><label>Data de nascimento:</label></td>\n'
                   + '  <td>\n'
                   + '    <input maxlength="10" class="CampoEstilo entdatanasc" type="text" name="entdatanasc[]" id="entdatanasc_' + this._total + '" value="' + this._entidades[index].entdatanasc + '" onkeyup="this.value = mascaraglobal(\'##/##/####\',this.value);" onblur="Entidade.validarData(this);" />\n'
                   + '    <input type="hidden" name="entnumdddcomercial[]" id="entnumdddcomercial_' + this._total + '" value="' + this._entidades[index].entnumdddcomercial + '" />\n'
                   + '    <input type="hidden" name="entnumcomercial[]" id="entnumcomercial_' + this._total + '" value="' + this._entidades[index].entnumcomercial + '" />\n'
                   + '    <input type="hidden" name="entid[]" id="entid_' + this._total + '" value="' + entid + '" />\n'
                   + '  </td>';

        Element.insert(newTr,     {bottom: inline});
        Element.insert(container, {bottom: newTr });

        this._total++;
    },


    /**
     * 
     */
    _inlineFormEntidadePessoaJuridica: function (container, index, entid)
    {
        if (!$(container)) {
            alert('O container informado não existe.');
            return false;
        }

        var newTr = $(container).insertRow($(container).numRows);
            newTr.setAttribute('id', '__ent_' + this._total);

        if (!index)
            index = this._total;

        if (!this._entidades[index]) {
            this._entidades[index] = this._entidadePrototype;
        }

        var inline = ''
                   + '  <td>\n'
                   + '    <a class="excluirEntidade" href="#" onclick="return Entidade._removerEntidade(' + this._total + ');"><span>Excluir</span></a>\n'
                   + '  </td>\n'
                   + '  <td align="right" class="SubTituloDireita"><label>CNPJ:</label></td>\n'
                   + '  <td>\n'
                   + '    <input maxlength="18" class="CampoEstilo" type="text" name="entnumcpfcnpj[]" id="entnumcpfcnpj_' + this._total + '" value="' + this._entidades[index].entnumcpfcnpj + '" onkeyup="this.value = mascaraglobal(\'##.###.###/####-##\', this.value);" />\n'
                   + '  </td>\n'
                   + '  <td align="right" class="SubTituloDireita"><label>Nome:</label></td>\n'
                   + '  <td>\n'
                   + '    <input maxlength="99" class="CampoEstilo" type="text" name="entnome[]" id="entnome_' + this._total + '" value="' + this._entidades[index].entnome + '" style="width: 250px;"/>\n'
                   + '  </td>\n'
                   + '  <td align="right" class="SubTituloDireita"><label>(DDD) Telefone:</label></td>\n'
                   + '  <td>\n'
                   + '    ( <input size="2" class="CampoEstilo" maxlength="2" type="text" name="entnumdddcomercial[]" id="entnumdddcomercial_' + this._total + '" value="' + this._entidades[index].entnumdddcomercial + '" /> )\n'
                   + '    <input maxlength="9" class="CampoEstilo" type="text" name="entnumcomercial[]" id="entnumcomercial_' + this._total + '" value="' + this._entidades[index].entnumcomercial + '" onkeyup="this.value = mascaraglobal(\'####-####\', this.value);" />\n'
                   + '    <input type="hidden" name="entdatanasc[]" id="entdatanasc_' + this._total + '" value="' + this._entidades[index].entdatanasc + '"  />\n'
                   + '    <input type="hidden" name="entid[]" id="entid_' + this._total + '" value="' + entid + '" />\n'
                   + '  </td>';

        Element.insert(newTr,     {bottom: inline});
        Element.insert(container, {bottom: newTr });

        this._total++;
    },


    //------------------------------------------------------------ private
    /**
     * 
     */
    __carregarFuncoesJson: function (urlFuncoes, funid)
    {
        if (!$('funid') || $('funid').tagName.toUpperCase() != 'SELECT')
            return false;

        new Ajax.Request(urlFuncoes, {
                         onComplete: function (res)
                         {
                             eval(res.responseText);

                             for (i = 0; i < funcoes.length; i++) {
                                 $('funid').options[i + 1] = new Option(funcoes[i].fundsc,
                                                                        funcoes[i].funid,
                                                                        false,
                                                                        funcoes[i].funid == funid);
                             }
                         }
        });
    },


    /**
     * 
     */
    __carregarFuncoes: function (funid)
    {
        if (!$('funid') || $('funid').tagName.toUpperCase() != 'SELECT')
            return false;

        for (i = 0; i < funcoes.length; i++) {
            $('funid').options[i + 1] = new Option(funcoes[i].fundsc,
                                                   funcoes[i].funid,
                                                   false,
                                                   funcoes[i].funid == funid);
        }
    },


    /**
     * 
     */
    __carregarTiposClassificacaoAJAX: function (tpcid)
    {
    },


    /**
     * 
     */
    __carregarTiposClassificacao: function (tpcid)
    {
        for (i = 0; i < tiposClassificacao.length; i++) {
            $('tpcid').options[i + 1] = new Option(tiposClassificacao[i].tpcdesc,
                                                   tiposClassificacao[i].tpcid,
                                                   false,
                                                   tiposClassificacao[i].tpcid == tpcid);
        }
    },


    /**
     * 
     */
    __carregarTiposLocalizacao: function (tplid)
    {
        for (i = 0; i < tiposLocalizacao.length; i++) {
            $('tplid').options[i + 1] = new Option(tiposLocalizacao[i].tpldesc,
                                                   tiposLocalizacao[i].tplid,
                                                   false,
                                                   tiposLocalizacao[i].tplid == tplid);
        }
    },


    /**
     * 
     */
    __copyAttributes: function(oldElem, newElem)
    {
        if (oldElem.attributes.length == 0)
            return false;
    
        if (Prototype.Browser.IE) {
            newElem.setAttribute('name',  oldElem.getAttribute('name' ));
            newElem.setAttribute('class', oldElem.getAttribute('class'));
            newElem.setAttribute('style', oldElem.getAttribute('style'));
        } else {
            for (var i = 0; i < oldElem.attributes.length; i++) {
                var attr = oldElem.attributes.item(i);

                if (!/id|size|type|readonly/i.test(attr.name))
                    newElem.setAttribute(attr.name, attr.value);
            }
        }

        return newElem;
    },

    __getSelectMunUf: function (uf){
    	var retorno;
        var req3 = new Ajax.Request('/geral/dne.php?opt=municipio&regcod=' + uf, {
            method: 'post',
            asynchronous: false,
            onComplete: function (res3)
            {
        		retorno = res3.responseText;
            }
        });
        return retorno;
    },
    
    __getEnderecoFaixaUfCEP: function (cep){	
    	    var d = document;
    		var DNE = new Array();
	        var req2 = new Ajax.Request('/geral/dne.php?opt=cepUF&endcep=' + cep, {
	            method: 'post',
	            asynchronous: false,
	            onComplete: function (res2)
	            {
	          		eval(res2.responseText);
	            }
	        });  
	        if (DNE[0].estado){
	        	var mun = d.createElement('select');
	        	mun.setAttribute('name', 'mundescricao');	        	
	        	mun.setAttribute('id', 'mundescricao');
	        	mun.onchange = function(){
	        								$('muncod').value = this.value 
	        							 };
	            lista = Entidade.__getSelectMunUf(DNE[0].estado);
	        	eval(lista);
	        	
                mun.options[0] = new Option('Selecione',
					                        '',
					                        false,
					                        false);

	        	
	        	for(i=0; i < listaMunicipios[DNE[0].estado].length; i++){
	                mun.options[i+1] = new Option(listaMunicipios[DNE[0].estado][i][1],
	                							  listaMunicipios[DNE[0].estado][i][0],
						                          false,
						                          false);

	        	}	
	        	
                var parentNode = $('mundescricao').parentNode;
                parentNode.replaceChild(mun, $('mundescricao'));
                
                $('endlogradouro').removeAttribute('readOnly');
                $('endbai').removeAttribute('readOnly');
                $('estuf').value = DNE[0].estado;
		        return true;	        	
	        }else{	
	        	return false;
	        }
    },
    
    
    /**
     * 
     */
    __getEnderecoPeloCEP: function (endcep)
    {
        if (!endcep)
            var endcep = $('endcep');

        if (!endcep || endcep.value == '' || endcep.value.replace(/[^0-9]/ig, '').length != 8)
            return false;

        var req = new Ajax.Request('/geral/dne.php?opt=dne&endcep=' + endcep.value, {
                                       method: 'post',
                                       onComplete: function (res)
                                       {
                                           eval(res.responseText);

                                           if (DNE[0].muncod == '') {
                                        	   if (Entidade.__getEnderecoFaixaUfCEP(DNE[0].cep)){
                                        		   return;
                                        	   }else{
	                                               alert('CEP não encontrado!');
	                                               endcep.value = '';
	                                               endcep.select();
	                                               return false;
	                                        	   }
                                           }

                                           if (DNE.length > 1) {
                                               $('escolha_logradouro_id').show();

                                               var endlog = document.createElement('select');
                                               //var endbai = document.createElement('select');
                                               var endbai = document.createElement('input');

                                               Entidade.__copyAttributes($('endlog'), endlog);
                                               Entidade.__copyAttributes($('endbai'), endbai);

                                               endlog.options[0] = new Option('Selecione',
                                                                              '',
                                                                              false,
                                                                              false);

                                               var bairros = [''];

                                               for (var i = 0; i < DNE.length; i++) {
                                                   if (DNE[i].logradouro != '')
                                                       endlog.options[i + 1] = new Option(DNE[i].logradouro,
                                                                                          DNE[i].logradouro,
                                                                                          false,
                                                                                          false);
                                                       bairros[i + 1] = DNE[i].logradouro;
                                               }

                                               for (var i = 0; i < DNE.length; i++) {
                                                   if (DNE[i].endbai != '')
                                                       bairros[i + 1] = DNE[i].bairro;
                                               }

                                               endlog.onchange = function(e)
                                               {
                                                   $('endlogradouro').value = this.value;

                                                   if (this.value == '') {
                                                       $('endlogradouro').removeAttribute('readOnly');
                                                       $('endlogradouro').select();

                                                       endbai.removeAttribute('readOnly');
                                                       endbai.value = '';

                                                   } else {
                                                       $('endlogradouro').setAttribute('readOnly', 'readOnly');
                                                       endbai.setAttribute('readOnly', 'readOnly');
                                                       endbai.value = bairros[this.selectedIndex];
                                                   }
                                               }
                                           } else {
                                               $('escolha_logradouro_id').hide();

                                               var endlog = document.createElement('input');
                                               var endbai = document.createElement('input');

                                               Entidade.__copyAttributes($('endlog'), endlog);
                                               Entidade.__copyAttributes($('endbai'), endbai);

                                               endlog.setAttribute('size', '48');
                                               endbai.setAttribute('size', '48');
                                           }

                                           var parentNode = $('endlog').parentNode;
                                               parentNode.replaceChild(endlog, $('endlog'));
                                               endlog.setAttribute('id', 'endlog');

                                           var parentNode = $('endbai').parentNode;
                                               parentNode.replaceChild(endbai, $('endbai'));
                                               endbai.setAttribute('id', 'endbai');

                                           delete parentNode;
                                           delete endlog;
                                           delete endbai;

                                           $('endlog').value = $('endlogradouro').value = DNE[0].logradouro;
                                           $('endbai').value = DNE[0].bairro;
                                           $('estuf').value  = DNE[0].estado;

                                           if (trim($('endlog').value) != '')
                                               $('endlog').setAttribute('readOnly', 'readOnly');

                                           if (trim($('endbai').value) != '')
                                               $('endbai').setAttribute('readOnly', 'readOnly');

                                           //while ($('muncod').options[0]) {
                                           //    $('muncod').options[0] = null;
                                           //}

                                           //$('muncod').options[0] = new Option(DNE[0].cidade,
                                           //                                    DNE[0].muncod,
                                           //                                    false,
                                           //                                    false);

                                           //$('muncod').select();
                                           
                                           if ($('mundescricao').type != 'text'){
                                           		var mun = document.createElement('input');
                                           		mun.setAttribute('name'  	 , 'mundescricao');
                                           		mun.setAttribute('id'    	 , 'mundescricao');
                                           		mun.setAttribute('class' 	 , 'CampoEstilo');
                                           		mun.setAttribute('readonly' , 'readonly');
                                           		
                                                var parentNode = $('mundescricao').parentNode;
                                                parentNode.replaceChild(mun, $('mundescricao'));
                                                
//                                                $('endlogradouro').setAttribute('readOnly', 'readOnly');
                                           }
                                           $('endlogradouro').removeAttribute('readOnly');
                                           $('endbai').removeAttribute('readOnly');
                                           
                                           
                                           $('muncod').value       = DNE[0].muncod;
                                           $('mundescricao').value = DNE[0].cidade;
                                           
                                           //Verifica se é indígena
                                           if( $('cloid') ){
												if ( $('cloid').value == 4 ){
													$('lbLogadouro').innerHTML = 'Terra Indígena';
													$('trComunidadeIndigena').show();
												}else{
													$('trComunidadeIndigena').hide();
												}
                                           }
                                           if (endcep.value.substr( endcep.value.length - 3 ) === '000' || endcep.value.substr(5) === '0-000'){
                                               $('endlogradouro').removeAttribute('readOnly');
                                           }
                                       }
        });
    },


    /**
     * 
     */
    carregarMunicipios: function(estuf)
    {
        if ($('estuf').value == '')
            return false;

        var estuf = estuf ? estuf : $('estuf').value;
        var req   = new Ajax.Request('/geral/dne.php?opt=municipio&regcod=' + estuf, {
                                        method: 'post',
                                        onComplete: function (res)
                                        {
                                            while ($('muncod').options[0]) {
                                                $('muncod').options[0] = null;
                                            }

                                            eval(res.responseText);

                                            for (var i = 0; i < listaMunicipios[estuf].length; i++) {
                                                $('muncod').options[i] = new Option(listaMunicipios[estuf][i][1],
                                                                                    listaMunicipios[estuf][i][0],
                                                                                    false,
                                                                                    false);
                                            }
                                        }
        });
    },


    /**
     * 
     */
    cadastrarEscolaNova: function(element)
    {
        if (element.checked || element.checked == 'checked') {
            var req   = new Ajax.Request('/brasilpro/brasilpro.php?modulo=principal/cadastrarescola&acao=A&opt=gerarEntcodent', {
                                            method: 'post',
                                            onComplete: function (res)
                                            {
                                                $('entcodent').value = res.responseText;
                                            }
            });

            $('entcodent')  .setAttribute('readOnly', 'readOnly');
            $('entcodent')  .setAttribute('disabled', 'disabled');
            $('entcodent_0').setAttribute('disabled', 'disabled');
            $('entcodent_1').setAttribute('disabled', 'disabled');
            $('entnome')    .focus();
        } else {
            $('entcodent')  .value = '';
            $('entcodent')  .removeAttribute('readOnly');
            $('entcodent')  .removeAttribute('disabled');
            $('entcodent_0').removeAttribute('disabled');
            $('entcodent_1').removeAttribute('disabled');
            $('entcodent')  .focus();
        }
    },


    /**
     * 
     */
    submeterFrmEntidade: function(frm)
    {
        if (!frm)
            frm = $('frmEntidade');

        if (!Boolean($F('editavel')) || !this.validateForm(frm))
            return false;

        var entnumcpfcnpj = $F('entnumcpfcnpj').replace(/[^0-9]/ig, '');

        if (!validarCnpj(entnumcpfcnpj) && !validar_cpf(entnumcpfcnpj)) {
            alert('CPF ou CNPJ inválido.\nPor favor verifique os dados informados.');
            $('entnumcpfcnpj').focus();
            return false;
        }

        if (this.onSubmit()) {
            var frmElements = Form.getElements(frmEntidade);

            for (var i = 0; i < frmElements.length; i++)
                frmElements[i].removeAttribute('disabled');

            return true;
        }

        return false;
    },


    /**
     * 
     */
    onSubmit: function()
    {
        return true;
    },

    __getEnderecoPeloCEPKeyDown: function(event)
    {
        if (event.keyCode == Event.KEY_RETURN) {
            var element = Event.element(event);
            Entidade.__getEnderecoPeloCEP(element);
            return false;
        }

        return true;
    }

});


var Entidade = new Entidade();
var muncod   = $('muncod');
if (muncod && muncod.tagName.toUpperCase() == 'SELECT')
    muncod.setAttribute('disabled', 'disabled');

/*
var endcep   = $('endcep');
if (endcep)
    Event.observe(endcep, 'keyup', __getEnderecoPeloCEPKeyDown);
*/
