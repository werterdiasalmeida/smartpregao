function executarAjax(params,destino,tipo)
{
	destino.html( 'Carregando...' );
	$.ajax({
		   type: "POST",
		   url: window.location,
		   data: params,
		   success: function(msg){
		   		if(tipo == "val"){
		   			destino.val( msg );
		   		}else{
		   			destino.html( msg );
		   		}
		   }
		 });
}

function filtrarAcao(secid)
{
	var params = "requisicaoAjax=filtrarAcao&secid=" + secid;
	var destino = $('#td_combo_acaid');
	executarAjax(params,destino);
}

function filtrarIndicador(acaid)
{
	var params = "requisicaoAjax=filtrarIndicador&acaid=" + acaid + "&secid=" + $('[name=secid]').val();
	var destino = $('#td_combo_indid');
	executarAjax(params,destino);
}

function filtrarPeriodicidade(indid)
{
	var params = "requisicaoAjax=filtrarPeriodicidade&indid=" + indid;
	var destino = $('#td_combo_perid');
	executarAjax(params,destino);
}

function listarTodasMetas()
{
	window.location.href = "painel.php?modulo=principal/listarMetasIndicador&acao=A";
}

function pesquisarMetas()
{
	$('#formulario_metas').submit();
}
function salvarMetas()
{
	var erro = 0;
	if(!$('[name=indid]').val()){
		alert('Favor selecionar o indicador');
		erro = 1;
		$('[name=indid]').focus();
		return false;
	}
	if(!$('[name=perid]').val()){
		alert('Favor selecionar a periodicidade');
		erro = 1;
		$('[name=perid]').focus();
		return false;
	}
	if(!$('[name=metdesc]').val()){
		alert('Favor informar a meta');
		erro = 1;
		$('[name=metdesc]').focus();
		return false;
	}
	if(erro == 0){
		$('#formulario_metas').submit();
	}
}

function editarMeta(metid)
{
	$('[name=requisicao]').val("");
	$('[name=metid]').val(metid);
	$('#formulario_metas').attr("action","painel.php?modulo=principal/cadastrarMetasIndicador&acao=A");
	$('#formulario_metas').submit();
}

function excluirMeta(metid)
{
	if(confirm("Deseja realmente excluir a meta?")){
		$('[name=requisicao]').val("excluirMeta");
		$('[name=metid]').val(metid);
		$('#formulario_metas').submit();
	}
}

function addEstrategia()
{
	executarAjax("requisicaoAjax=getTxtEstrategia",$("#td_input"));
	$("#identificador").val("");
	$("#td_descricao").html("Estrat�gia");
	$("#requisicao").val("salvarEstrategia");
	$("[name=campo_descricao]").val("");
	$("#div_estrategia").attr("style","display:block");
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
	$("[name=campo_descricao]").focus();
}

function fecharDiv(div)
{
	$("#" + div).attr("style","display:none");
}

function salvarEstrategiaAcao()
{
	var metid 		  = $("#metid").val();
	var estid 		  = $("#estid").val();
	var identificador = $("#identificador").val();
	var descricao     = $("[name=campo_descricao]").val();
	var acao    	  = $("[name=acaid]").val();
	var requisicao 	  = $("#requisicao").val();
	
	if(requisicao == "salvarEstrategia"){
		if(!descricao){
			alert('Favor preencher a descri��o da Estrat�gia!');
			$("[name=campo_descricao]").focus();
			return false;
		}else{
			if(identificador){
				var params = "requisicaoAjax=" + requisicao + "&metid=" + metid + "&descricao=" + descricao + "&id=" + identificador;
			}else{
				var params = "requisicaoAjax=" + requisicao + "&metid=" + metid + "&descricao=" + descricao 
			}
		}
	}else{
		if(!acao){
			alert('Favor selecionar a A��o!');
			$("[name=acaid]").focus();
			return false;
		}else{
			if(identificador){
			var params = "requisicaoAjax=" + requisicao + "&estid=" + estid + "&acaid=" + acao + "&id=" + identificador;
			}else{
				var params = "requisicaoAjax=" + requisicao + "&estid=" + estid + "&acaid=" + acao 
			}
		}
	}
	
	var destino = $("#lista_estrategia");
		
	$.ajax({
	   type: "POST",
	   url: window.location,
	   data: params,
	   success: function(msg){
	   		if( msg == "true"){
	   			executarAjax("requisicaoAjax=listarEstrategias",destino);
	   			fecharDiv("div_estrategia");
	   			alert('Opera��o realizada com sucesso!');
	   		}else{
	   			alert('N�o foi poss�vel realizar a opera��o!');
	   		}
	   }
	 });
}

function editarEstrategia(estid)
{
	executarAjax("requisicaoAjax=getTxtEstrategia&estid=" + estid,$("#td_input"));
	$("#td_descricao").html("Estrat�gia");
	$("#requisicao").val("salvarEstrategia");
	$("#identificador").val(estid);
	$("#div_estrategia").attr("style","display:block");
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
	$("[name=campo_descricao]").focus();
	
}

function excluirEstrategia(estid)
{
	if(confirm("Deseja realmente excluir esta estrat�gia e todas as a��es vinculadas?")){
		var params = "requisicaoAjax=excluirEstrategia&estid=" + estid;
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: params,
		   success: function(msg){
		   		if( msg == "true"){
		   			var destino = $("#lista_estrategia");
		   			executarAjax("requisicaoAjax=listarEstrategias",destino);
		   			alert('Opera��o realizada com sucesso!');
		   		}else{
		   			alert('N�o foi poss�vel realizar a opera��o!');
		   		}
		   }
		 });
	}
}

function addAcao(estid)
{
	$("#identificador").val("");
	$("[name=campo_descricao]").val("");
	$("[name=estid]").val(estid);
	$("#td_descricao").html("A��o");
	executarAjax("requisicaoAjax=getComboAcao",$("#td_input"));
	$("#requisicao").val("salvarAcao");
	$("#div_estrategia").attr("style","display:block");
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
	$("[name=acaid]").focus();
}

function carregarAcaoPorEstrategia(estid)
{
	var tr  	= $("#tr_estid_" + estid);
	var td  	= $("#td_estid_" + estid);
	var img 	= $("#img_estid_" + estid);
	var destino = $("#td_estid_" + estid);
	
	if(tr.attr("class") == "hidden"){
		img.attr("src","../imagens/menos.gif");
		tr.attr("class","visible");
	}else{
		img.attr("src","../imagens/mais.gif");
		tr.attr("class","hidden");
	}
	executarAjax("requisicaoAjax=getAcaoPorEstrategia&estid=" + estid,destino);
	
}

function editarAcao(aceid,estid,acaid)
{
	$("#identificador").val(aceid);
	$("[name=estid]").val(estid);
	$("#td_descricao").html("A��o");
	executarAjax("requisicaoAjax=getComboAcao&acaid=" + acaid,$("#td_input"));
	$("#requisicao").val("salvarAcao");
	$("#div_estrategia").attr("style","display:block");
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
	$("[name=acaid]").focus();
}

function excluirAcao(aceid)
{
	if(confirm("Deseja realmente excluir esta a��o estrat�gica?")){
		var params = "requisicaoAjax=excluirAcao&aceid=" + aceid;
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: params,
		   success: function(msg){
		   		if( msg == "true"){
		   			var destino = $("#lista_estrategia");
		   			executarAjax("requisicaoAjax=listarEstrategias",destino);
		   			alert('Opera��o realizada com sucesso!');
		   		}else{
		   			alert('N�o foi poss�vel realizar a opera��o!');
		   		}
		   }
		 });
	}
}

function salvarValorMeta()
{
	var dmiid 	 = $("[name=dmiid]").val();
	var dpeid 	 = $("[name=dpeid]").val();
	var dmivalor = $("[name=dmivalor]").val();
	var dmiqtde  = $("[name=dmiqtde]").val();
	var erro = 0;
	
	if(!dpeid){
		alert('Favor informar o Per�odo');
		$("[name=dpeid]").focus();
		erro = 1;
		return false;
	}
	
	if(!dmiqtde){
		alert('Favor informar a Quantidade');
		$("[name=dmiqtde]").focus();
		erro = 1;
		return false;
	}
	/*
	if( dmivalor == "" ){
		alert('Favor informar o Valor');
		$("[name=dmivalor]").focus();
		erro = 1;
		return false;
	}
	*/
	if(erro == 0){
		 $("[name=formulario_valor_metas]").submit();
	}
	
}

function excluirValorMeta(dmiid)
{
	if(confirm("Deseja realmente excluir?")){
		var params = "requisicaoAjax=excluirValorMeta&dmiid=" + dmiid;
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: params,
		   success: function(msg){
		   		if( msg == "true"){
		   			var destino = $("#lista_valor_meta");
		   			executarAjax("requisicaoAjax=listarValorMetas",destino);
		   			alert('Opera��o realizada com sucesso!');
		   		}else{
		   			alert('N�o foi poss�vel realizar a opera��o!');
		   		}
		   }
		 });
	}
}