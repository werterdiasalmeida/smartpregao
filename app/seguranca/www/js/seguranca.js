function ajaxatualizar(params,iddestinatario, pai) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}else{
						document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}	
				} 
			},
			onLoading: function(){
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}else{
						document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}
				}	
			}
		});
}

function atualizarGridMonitoramento(valor) {
	if(document.getElementById('mes').value &&
	   document.getElementById('ano').value) {
	   
		document.getElementById('gridMonitoramento').innerHTML='<p align="center"><img src="/imagens/carregando.gif" align="absmiddle"> Carregando...</p>';
	   
		ajaxatualizar('requisicao=monitoramentoGRID&mes='+document.getElementById('mes').value+'&ano='+document.getElementById('ano').value, 'gridMonitoramento');
		
		document.getElementById('permesini').innerHTML = document.getElementById('mes').value;
		document.getElementById('permesfim').innerHTML = document.getElementById('mes').value;
		document.getElementById('peranoini').innerHTML = document.getElementById('ano').value;
		document.getElementById('peranofim').innerHTML = document.getElementById('ano').value;
		var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: 'requisicao=pegarDiasMes&mes='+document.getElementById('mes').value+'&ano='+document.getElementById('ano').value,
				asynchronous: false,
				onComplete: function(resp) {
					document.getElementById('diaini').value = '01';
					document.getElementById('diafim').value = resp.responseText;
				},
				onLoading: function(){}
			});

		
	} else {
		document.getElementById('gridMonitoramento').innerHTML="";	
	}
}


function atualizarGridMonitoramentoPeriodo(valor) {
	if(document.getElementById('mes').value &&
	   document.getElementById('ano').value &&
	   document.getElementById('diaini').value && 
	   document.getElementById('diafim').value) {
	   
		document.getElementById('gridMonitoramento').innerHTML='<p align="center"><img src="/imagens/carregando.gif" align="absmiddle"> Carregando...</p>';
	   
		ajaxatualizar('requisicao=monitoramentoGRID&mes='+document.getElementById('mes').value+'&ano='+document.getElementById('ano').value+'&diaini='+document.getElementById('diaini').value+'&diafim='+document.getElementById('diafim').value, 'gridMonitoramento');
		
	} else {
		document.getElementById('gridMonitoramento').innerHTML="";	
	}
}

function combinarGrafico(sisid,mes,ano) {
	if(document.getElementById('yesquerda').value == '') {
		alert('Selecione "Y(Esquerda)"');
		return false;
	}
	yesquerda = document.getElementById('yesquerda').value;
	if(document.getElementById('ydireita').value == '') {
		alert('Selecione "Y(Direita)"');
		return false;
	}
	ydireita  = document.getElementById('ydireita').value;
	document.getElementById('tr_grafico_mix').style.display = '';
	document.getElementById('grafico_mix').innerHTML = '<img src="chart-data2.php?dados='+sisid+';'+mes+';'+ano+';'+yesquerda+';'+ydireita+'">'; 

}