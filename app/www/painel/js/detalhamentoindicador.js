/*
 * Abre a caixa mostrando as formas de detalhar a informação. 
 * Esta função é utilizada nas derivações dentro do indicador.
 */
function abreboxsub(iden) {
	if(document.getElementById('img_'+iden).title=="mais") {
		document.getElementById('box_'+iden).style.display='block';
	} else {
		var obj = document.getElementById('img_'+iden);
		var tabela = obj.parentNode.parentNode.parentNode;
		var linha  = obj.parentNode.parentNode; 
		obj.title="mais";
		obj.src="../imagens/mais.gif"
		tabela.deleteRow(linha.rowIndex+1);
	}
}
/*
 * Abre a caixa mostrando as formas de detalhar a informação. 
 * Esta função é utilizado na raiz do indicador.
 */
function abrebox(indid) {
	document.getElementById('exibeGrafico_' + indid).style.display = 'none';

	if(document.getElementById('img'+indid).title=="mais") {
		document.getElementById('box'+indid).style.display='block';
	} else {
		var obj = document.getElementById('img'+indid);
		var tabela = obj.parentNode.parentNode.parentNode;
		var linha  = obj.parentNode.parentNode; 
		obj.title="mais";
		obj.src="../imagens/mais.gif";
		var aux = document.getElementById('combo'+indid).value.split("&&");
		if(aux.length>1) {
			var tipo = aux[0];
			var tdiid = aux[1];
		} else {
			var tipo = document.getElementById('combo'+indid).value;
		}
		switch(tipo) {
			case 'detalhes':
				tabela.deleteRow(linha.rowIndex+1);
				break;
			default:
				tabela.deleteRow(linha.rowIndex+2);
		}
	}
}
/*
 * Processa a forma de detalhar indicada pelo box 
 * Esta função é utilizado na raiz do indicador.
 */
function processabox(acaind, params) {

	var acaind_t = acaind.split("_");
	var acaid = acaind_t[0];
	var indid = acaind_t[1];
	
	document.getElementById('box'+indid).style.display='none';
	var aux = document.getElementById('combo'+indid).value.split("&&");
	if(aux.length>1) {
		var tipo = aux[0];
		var tdiid = aux[1];
	} else {
		var tipo = document.getElementById('combo'+indid).value;
	}
	switch(tipo) {
		case 'detalhes':
			detalharPorDetalhes(document.getElementById('img'+indid), acaind, tdiid);
			break;
		default:
			detalhar(document.getElementById('img'+indid), acaind, params+'&tipo='+tipo);
	}
}
/*
 * Processa a forma de detalhar indicada pelo box.
 * Esta função é utilizada nas derivações dentro do indicador.
 */
function processaboxsub(inddet, params) {
	document.getElementById('box_'+inddet).style.display='none';
	var aux = document.getElementById('combo'+inddet).value.split("&&");
	if(aux.length>1) {
		var tipo = aux[0];
		var tdiid = aux[1];
	} else {
		var tipo = document.getElementById('combo'+inddet).value;
	}
	switch(tipo) {
		case 'detalhes':
			detalharPorDetalhesSub(document.getElementById('img_'+inddet), inddet, params+'&tdiid='+tdiid);
			break;
		default:
			detalharsub(document.getElementById('img_'+inddet), inddet, params+'&tipo='+tipo);
	}
}

function detalharsub(obj, inddet, params) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	
	inddet = inddet.split("_");
	var indid = inddet[0];
	var det = inddet[1];

	if(obj.title == "mais") {
		obj.title="menos";
		obj.src="../imagens/menos.gif";
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		nlinha.style.background = '#f5f5f5';
		nlinha.id="linha_"+det+"_"+indid;
		var col0 = nlinha.insertCell(0);
		col0.id="coluna"+det+"_"+indid;
		col0.colSpan=tabela.rows[0].cells.length;
		divCarregando(document.getElementById('coluna'+det+"_"+indid));
		ajaxatualizar('requisicao=detalhar&indid='+indid+params,'coluna'+det+"_"+indid);
		divCarregado(document.getElementById('coluna'+det+"_"+indid));
	} else {
		obj.title="mais";
		obj.src="../imagens/mais.gif"
		tabela.deleteRow(linha.rowIndex+2);
	}
}



function detalhar(obj, acaind, params) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	acaind = acaind.split("_");
	var acaid = acaind[0];
	var indid = acaind[1];
	

	if(obj.title == "mais") {
		obj.title="menos";
		obj.src="../imagens/menos.gif"
		var nlinha = tabela.insertRow(linha.rowIndex+2);
		nlinha.style.background = '#f5f5f5';
		nlinha.id="linha_"+acaid+"_"+indid;
		var col0 = nlinha.insertCell(0);
		col0.id="coluna"+indid+nlinha.rowIndex + 1;
		col0.colSpan=9;
		divCarregando(document.getElementById('coluna'+indid+nlinha.rowIndex + 1));
		ajaxatualizar('requisicao=detalhar&indid='+indid+params,'coluna'+indid+nlinha.rowIndex + 1);
		divCarregado(document.getElementById('coluna'+indid+nlinha.rowIndex + 1));
	} else {
		obj.title="mais";
		obj.src="../imagens/mais.gif"
		tabela.deleteRow(linha.rowIndex+2);
	}
}

function abrirAcao(acaid, obj) {
	var ped;
	var tabela = document.getElementById('tabela');
	if(obj.title == "mais") { 
		for(var i=0;i<tabela.rows.length;i++) {
			if(tabela.rows[i].id != "") {
				ped = tabela.rows[i].id.split("_");
				if(ped[1]==acaid && ped[3] != "grafico") {
					tabela.rows[i].style.display="";
				}
			}
		}
		obj.title = "menos";
		obj.src = "../imagens/menos.gif"
	} else {
		for(var i=0;i<tabela.rows.length;i++) {
			if(tabela.rows[i].id != "") {
				ped = tabela.rows[i].id.split("_");
				if(ped[1]==acaid) {
					tabela.rows[i].style.display="none";
				}
			}
		}
		obj.title = "mais";
		obj.src = "../imagens/mais.gif"
	}
}

function detalharPorSubDetalhes(obj, acaind, params, tidid) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	acaind = acaind.split("_");
	var acaid = acaind[0];
	var indid = acaind[1];
	divCarregando();
	if(obj.title == "mais") {
		obj.title="menos";
		obj.src="../imagens/menos.gif";
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		nlinha.style.background = '#f5f5f5';
		nlinha.id="linha_"+acaid+"_"+indid;
		var col0 = nlinha.insertCell(0);
		col0.id="subcoluna"+nlinha.rowIndex;
		col0.colSpan=tabela.rows[0].cells.length;
		ajaxatualizar('requisicao=detalharPorDetalhes&indid='+indid+'&tidid='+tidid+params,'subcoluna'+nlinha.rowIndex);
	} else {
		obj.title="mais";
		obj.src="../imagens/mais.gif"
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}

function detalharPorDetalhes(obj, acaind, tdiid) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	acaind = acaind.split("_");
	var acaid = acaind[0];
	var indid = acaind[1];
	divCarregando();
	if(obj.title == "mais") {
		obj.title="menos";
		obj.src="../imagens/menos.gif"
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		nlinha.style.background = '#f5f5f5';
		nlinha.id="linha_"+acaid+"_"+indid;
		var col0 = nlinha.insertCell(0);
		col0.id="coluna"+nlinha.rowIndex;
		col0.colSpan=9;
		ajaxatualizar('requisicao=detalharPorDetalhes&indid='+indid+'&tdiid='+tdiid,'coluna'+nlinha.rowIndex);
	} else {
		obj.title="mais";
		obj.src="../imagens/mais.gif"
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}

function detalharPorDetalhesSub(obj, inddet, params) {
	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	inddet = inddet.split("_");
	var indid = inddet[0];
	var det = inddet[1];
	divCarregando();	
	if(obj.title == "mais") {
		obj.title="menos";
		obj.src="../imagens/menos.gif"
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		nlinha.style.background = '#f5f5f5';
		nlinha.id="linha_"+det+"_"+indid;
		var col0 = nlinha.insertCell(0);
		col0.id="coluna"+det+"_"+indid;
		col0.colSpan=tabela.rows[0].cells.length;
		ajaxatualizar('requisicao=detalharPorDetalhes&indid='+indid+params,'coluna'+det+"_"+indid);
	} else {
		obj.title="mais";
		obj.src="../imagens/mais.gif"
		tabela.deleteRow(linha.rowIndex+1);
	}
	divCarregado();
}
var exibeGraficoAberto = 0;
function exibeGraficoIndicador(indid,request,detalhe,valor){
	if(exibeGraficoAberto != 0 && document.getElementById('exibeGrafico_' + exibeGraficoAberto)){
		document.getElementById('exibeGrafico_' + exibeGraficoAberto).style.display = "none";
	}
	document.getElementById('exibeGrafico_' + indid).style.display = "block";
	exibeGraficoAberto = indid;
}

//exibeGraficoPorIndicador('101','17','estado','estuf','AC',this.value)
function exibeGraficoPorIndicador(indid,acaid,reg,detalhe,valorDetalhe,tipografico,dpeid_inicio,dpeid_fim,estuf,muncod,indqtdevalor){
	if(tipografico){
		divCarregando();
		
		if(indqtdevalor == "t" && tipografico=="linha"){
			document.getElementById('grafico_linha_financeiro_' + indid).style.display = "";
		}else{
			document.getElementById('grafico_linha_financeiro_' + indid).style.display = "none";
		}
		
		if(!dpeid_inicio || dpeid_inicio == "undefined"){
			var dpeid_inicio = "";
		}
		if(!dpeid_fim || dpeid_fim == "undefined"){
			var dpeid_fim = "";
		}
		
		if(!estuf || estuf == ""){
			var estuf = "todos";
		}
		if(!muncod || muncod == ""){
			var muncod = "todos";
		}
		
		if(document.getElementById('finac_qtde_' + indid)){
			if(document.getElementById('finac_qtde_' + indid).checked == true)
				var finac_qtde = 1;
			else
				var finac_qtde = "";
		}else{
			var finac_qtde = "";
		}
		if(document.getElementById('finac_valor_' + indid)){
			if(document.getElementById('finac_valor_' + indid).checked == true)
				var finac_valor = 1;
			else
				var finac_valor = "";
		}else{
			var finac_valor = "";
		}
		
		if(document.getElementById('finac_ambos_' + indid)){
			
			if(document.getElementById('finac_qtde_' + indid).checked == false && document.getElementById('finac_valor_' + indid).checked == false){
				document.getElementById('finac_ambos_' + indid).checked = true;
				var finac_valor = 1;
				var finac_qtde = 1;
			}
			
			if(document.getElementById('finac_ambos_' + indid).checked == true){
				var finac_valor = 1;
				var finac_qtde = 1;
			}
		}
		
		var unidade_inteiro = "";
		var unidade_moeda = "";
		var indice_moeda = "";
		var gtmid = "";
		var tpmid = "";
		var tidid1 = "";
		var tidid2 = "";
		var regcod = "";
		var sehid = "";
		
		if(tipografico == "barra"){
			
			if(document.getElementById('opc_periodicidade_' + indid))
				document.getElementById('opc_periodicidade_' + indid).style.display = '';
			
			if(document.getElementById('sel_periodicidade_' + indid)){
				var periodicidade = document.getElementById('sel_periodicidade_' + indid).value;
			}else{
				var periodicidade = "";
			}
			
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";null;null;null;" + estuf + ";" + muncod + ";todos;todos;todos;todos;todos;" + reg + ";" + valorDetalhe + ";" + periodicidade;
			
			geraGrafico = "geraGrafico.php?tipo="  			 + tipografico     + 
										 ";indid=" 			 + indid 		   + 
										 ";dpeid=" 			 + dpeid_inicio    +
										 ";dpeid2=" 		 + dpeid_fim 	   +
										 ";unidade_inteiro=" + unidade_inteiro +
										 ";unidade_moeda="   + unidade_moeda   +
										 ";indice_moeda="    + indice_moeda    +
										 ";estuf="           + estuf    	   +
										 ";muncod="          + muncod    	   +
										 ";gtmid="           + gtmid    	   +
										 ";tpmid="           + tpmid    	   +
										 ";tidid1="          + tidid1    	   +
										 ";tidid2="          + tidid2    	   +
										 ";regcod="          + regcod    	   +
										 ";detalhe="         + detalhe    	   +
										 ";valorDetalhe="    + valorDetalhe    +
										 ";periodicidade="   + periodicidade
			
			var altura = "90%";
			var largura = "100%";
		}
		if(tipografico == "linha"){
			
			if(document.getElementById('opc_periodicidade_' + indid))
				document.getElementById('opc_periodicidade_' + indid).style.display = '';
			
			if(document.getElementById('sel_periodicidade_' + indid)){
				var periodicidade = document.getElementById('sel_periodicidade_' + indid).value;
			}else{
				var periodicidade = "";
			}
		
			//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tipografico + ";" + dpeid_inicio + ";" + dpeid_fim + ";null;null;null;" + estuf + ";" + muncod + ";todos;todos;todos;todos;todos;" + reg + ";" + valorDetalhe + ";" + finac_qtde + ";" + finac_valor + ";" + periodicidade;
			
			geraGrafico = "geraGrafico.php?tipo="  			 + tipografico     + 
										 ";indid=" 			 + indid 		   + 
										 ";dpeid=" 			 + dpeid_inicio    +
										 ";dpeid2=" 		 + dpeid_fim 	   +
										 ";unidade_inteiro=" + unidade_inteiro +
										 ";unidade_moeda="   + unidade_moeda   +
										 ";indice_moeda="    + indice_moeda    +
										 ";estuf="           + estuf    	   +
										 ";muncod="          + muncod    	   +
										 ";gtmid="           + gtmid    	   +
										 ";tpmid="           + tpmid    	   +
										 ";tidid1="          + tidid1    	   +
										 ";tidid2="          + tidid2    	   +
										 ";regcod="          + regcod    	   +
										 ";finac_qtde="      + finac_qtde      +
										 ";finac_valor="     + finac_valor     +
										 ";detalhe="         + detalhe    	   +
										 ";valorDetalhe="    + valorDetalhe    +
										 ";periodicidade="   + periodicidade
			
			var altura = "90%";
			var largura = "100%";
			
		}
		if(tipografico != "linha" && tipografico != "barra"){
			
			if(document.getElementById('opc_periodicidade_' + indid))
				document.getElementById('opc_periodicidade_' + indid).style.display = 'none';
			
			if(document.getElementById('sel_periodicidade' + indid)){
				var periodicidade = document.getElementById('sel_periodicidade' + indid).value;
			}else{
				var periodicidade = "";
			}
		
			var arrTipoGrafico = tipografico.split('_');
			var tpGrafico = arrTipoGrafico[0];
			var fatia =  arrTipoGrafico[1];
			var idFatia = arrTipoGrafico[2];
			var altura = "80%";
			var largura = "100%";
			if(tpGrafico == "barra"){
				//geraGrafico = "geraGrafico.php?tipo=" + indid + ";barra_fatia;;tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";null;null;null;" + estuf + ";" + muncod  + ";todos;todos;todos;todos;todos;" + reg + ";" + valorDetalhe;
				
				geraGrafico = "geraGrafico.php?tipo=barra_fatia"	           		  + 
										 ";indid=" 			 + indid 		   		  + 
										 ";sehid=" 			 + sehid 		   		  +
										 ";tidid=tdiid"		 + fatia + "_" +  idFatia +
										 ";dpeid=" 			 + dpeid_inicio    		  +
										 ";dpeid2=" 		 + dpeid_fim 	   		  +
										 ";unidade_inteiro=" + unidade_inteiro 		  +
										 ";unidade_moeda="   + unidade_moeda   		  +
										 ";indice_moeda="    + indice_moeda    		  +
										 ";estuf="           + estuf    	   		  +
										 ";muncod="          + muncod    	   		  +
										 ";gtmid="           + gtmid    	   		  +
										 ";tpmid="           + tpmid    	   		  +
										 ";tidid1="          + tidid1    	   		  +
										 ";tidid2="          + tidid2    	   		  +
										 ";regcod="          + regcod    	   		  +
										 ";finac_qtde="      + finac_qtde      		  +
										 ";finac_valor="     + finac_valor     		  +
										 ";detalhe="         + detalhe    	   		  +
										 ";valorDetalhe="    + valorDetalhe    		  +
										 ";periodicidade="   + periodicidade
				
			}
			if(tpGrafico == "pizza"){
				//geraGrafico = "geraGrafico.php?tipo=" + indid + ";" + tpGrafico + ";;tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";null;null;null;" + estuf + ";" + muncod + ";todos;todos;todos;todos;todos;" + reg + ";" + valorDetalhe;
				
				geraGrafico = "geraGrafico.php?tipo="        + tpGrafico			  +
										 ";indid=" 			 + indid 		   		  + 
										 ";sehid=" 			 + sehid 		   		  +
										 ";tidid=tdiid"		 + fatia + "_" +  idFatia +
										 ";dpeid=" 			 + dpeid_inicio    		  +
										 ";dpeid2=" 		 + dpeid_fim 	   		  +
										 ";unidade_inteiro=" + unidade_inteiro 		  +
										 ";unidade_moeda="   + unidade_moeda   		  +
										 ";indice_moeda="    + indice_moeda    		  +
										 ";estuf="           + estuf    	   		  +
										 ";muncod="          + muncod    	   		  +
										 ";gtmid="           + gtmid    	   		  +
										 ";tpmid="           + tpmid    	   		  +
										 ";tidid1="          + tidid1    	   		  +
										 ";tidid2="          + tidid2    	   		  +
										 ";regcod="          + regcod    	   		  +
										 ";finac_qtde="      + finac_qtde      		  +
										 ";finac_valor="     + finac_valor     		  +
										 ";detalhe="         + detalhe    	   		  +
										 ";valorDetalhe="    + valorDetalhe    		  +
										 ";periodicidade="   + periodicidade
										 
			}
			if(tpGrafico == "barracomp"){
				//geraGrafico = "geraGrafico.php?tipo=" + indid + ";barra_comp;;tdiid" + fatia + "_" + idFatia + ";" + dpeid_inicio + ";" + dpeid_fim + ";null;null;null;" + estuf + ";" + muncod  + ";todos;todos;todos;todos;todos;" + reg + ";" + valorDetalhe;
				
				geraGrafico = "geraGrafico.php?tipo=barra_comp"		           		  + 
										 ";indid=" 			 + indid 		   		  + 
										 ";sehid=" 			 + sehid 		   		  +
										 ";tidid=tdiid"		 + fatia + "_" +  idFatia +
										 ";dpeid=" 			 + dpeid_inicio    		  +
										 ";dpeid2=" 		 + dpeid_fim 	   		  +
										 ";unidade_inteiro=" + unidade_inteiro 		  +
										 ";unidade_moeda="   + unidade_moeda   		  +
										 ";indice_moeda="    + indice_moeda    		  +
										 ";estuf="           + estuf    	   		  +
										 ";muncod="          + muncod    	   		  +
										 ";gtmid="           + gtmid    	   		  +
										 ";tpmid="           + tpmid    	   		  +
										 ";tidid1="          + tidid1    	   		  +
										 ";tidid2="          + tidid2    	   		  +
										 ";regcod="          + regcod    	   		  +
										 ";finac_qtde="      + finac_qtde      		  +
										 ";finac_valor="     + finac_valor     		  +
										 ";detalhe="         + detalhe    	   		  +
										 ";valorDetalhe="    + valorDetalhe    		  +
										 ";periodicidade="   + periodicidade
				
			}
		}
		
		if(tipografico != "linha" && tipografico != "barra"){
			var myAjax = new Ajax.Request(
						'painel.php?modulo=principal/painel_controle&acao=A',
						{
							method: 'post',
							parameters: "exibeLegendaGrafico=true&indid=" + indid + "&tdiid=" + fatia + "&valorTdiid=" + idFatia + "&dpeid_inicio=" + dpeid_inicio + "&dpeid_fim=" + dpeid_fim + "&estuf=" + estuf + "&muncod=" + muncod  + "&detalhe=" + reg + "&valorDetalhe=" + valorDetalhe,
							asynchronous: false,
							onComplete: function(resp){
								document.getElementById('legenda_grafico_' + indid).innerHTML = resp.responseText;
							},
							onLoading: function(){
							}
						});
		}else{
			document.getElementById('legenda_grafico_' + indid).innerHTML = "";
		}
		//alert(geraGrafico);
		document.getElementById('tr_' + acaid + '_' + indid + '_grafico_grafico').style.display = "";
		document.getElementById('tr_' + acaid + '_' + indid + '_grafico_dados').style.display = "none";
		document.getElementById('tr_' + acaid + '_' + indid + '_grafico').style.display = "";
		document.getElementById('exibeGrafico_' + indid ).style.display = "none";
		swfobject.embedSWF("/includes/open_flash_chart/open-flash-chart.swf", "grafico_indicador_" + indid, "620", "260", "9.0.0", "expressInstall.swf", {"data-file":"" + geraGrafico,"loading":"Carregando gráfico..."} );
		
		document.getElementById('selectGrafico_' + indid ).value = tipografico;
		
		divCarregado();
		
	}
}

function checkFinanceiro(obj,indid){
	
	if(document.getElementById('finac_qtde_' + indid).checked == false && document.getElementById('finac_valor_' + indid).checked == false && document.getElementById('finac_ambos_' + indid).checked == false){
		alert('É necessário selecionar pelo menos uma linha de exibição!');
		obj.checked = true;
		return false;
	}else{
		return true;
	}
}

function exibedadosIndicador(indid,acaid){
	divCarregando();
	document.getElementById('tr_' + acaid + '_' + indid + '_grafico_grafico').style.display = "none";
	document.getElementById('tr_' + acaid + '_' + indid + '_grafico_dados').style.display = "";
	document.getElementById('tr_' + acaid + '_' + indid + '_grafico').style.display = "";
	
	var myAjax = new Ajax.Request(
		'painel.php?modulo=principal/painel_controle&acao=A',
		{
			method: 'post',
			parameters: "exibeDadosIndicador=true&indid=" + indid,
			asynchronous: false,
			onComplete: function(resp){
				document.getElementById('exibe_dados_indicador_' + indid).innerHTML = resp.responseText;
			},
			onLoading: function(){
			}
		});
		
	divCarregado();
	
}

function teste(tt) {
	alert('dasdasdasdasd');
	alert(tt);
	//abrebox(tt);
	document.getElementById('box'+tt).style.display='block';
}


function exibeTabelaIndicador(indid,acaid,filtros,detalhe,periodo){
	divCarregando();
	document.getElementById('tr_' + acaid + '_' + indid + '_grafico_grafico').style.display = "none";
	document.getElementById('tr_' + acaid + '_' + indid + '_grafico_dados').style.display = "";
	document.getElementById('tr_' + acaid + '_' + indid + '_grafico').style.display = "";
	
	if(document.getElementById(detalhe)){
		var det = document.getElementById(detalhe).value;
	}else{
		var det = '';
	}
	
	if(document.getElementById(periodo)){
		var perid = document.getElementById(periodo).value;
	}else{
		var perid = '';
	}
	
	var myAjax = new Ajax.Request(
		'painel.php?modulo=principal/painel_controle&acao=A',
		{
			method: 'post',
			parameters: "exibeTabelaIndicador=true&indicadorid=" + indid + "&periodo=" + perid + "&detalhe=" + det + "&acaid=" + acaid + "&filtros=1&" + filtros,
			asynchronous: false,
			onComplete: function(resp){
				document.getElementById('exibe_dados_indicador_' + indid).innerHTML = resp.responseText;
			},
			onLoading: function(){
			}
		});
		
	divCarregado();
	
}

