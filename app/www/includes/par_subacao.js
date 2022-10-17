function _totalizar(x) {
	total = x;
}

function carregarBeneficiarios()
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&reqt='+ reqt +'&req=carregarBeneficiarios&sabano=' + anoExercicio,
                                asynchronous: false,
                                onComplete: function(res)
                                {
                                    $('beneficiariosSubAcao' + anoExercicio).innerHTML = res.responseText;
                                }
                            });
}

function carregarItensComposicao()
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&reqt='+ reqt +'&req=carregarItensComposicao&cosano=' + anoExercicio + '&sbaporescola='+porEscola,
                                asynchronous: false,
                                onComplete: function(res)
                                {
                                    $('itensComposicaoSubAcao' + anoExercicio).innerHTML = res.responseText;
                                }
                            });
}
	    
function carregarDadosParecer() // Parametro utilizado apenas na função travaAbaAno(cosano)
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&reqt='+ reqt +'&req=carregarDadosParecer&sbtano=' + anoExercicio,
                                asynchronous: false,
                                onComplete: function(res)
                                {
                                    var sptparecer              = $('sptparecer_'         + anoExercicio);
                                    var sptunt                  = $('sptunt_'             + anoExercicio);
                                    var sptuntdsc               = $('sptuntdsc_'          + anoExercicio);
                                    var sptinicio               = $('sptinicio_'          + anoExercicio);
                                    var sptfim                  = $('sptfim_'             + anoExercicio);
                                    var sptanoterminocurso      = $('sptanoterminocurso_' + anoExercicio);
                                    var ssuid                   = $('ssuid_'              + anoExercicio);

                                    var ssuidValue              = getElementText('ssuid'             , res);
                                    var sptinicioValue          = getElementText('sptinicio'         , res);
                                    var sptfimValue             = getElementText('sptfim'            , res);
                                    var sptanoterminocursoValue = getElementText('sptanoterminocurso', res);

                                    sptanoterminocurso.value = sptanoterminocursoValue;

                                    if (sptunt)
                                        sptunt.value = getElementText('sptunt',    res);

                                    for (var i = 0; i < sptinicio.options.length; i++) {
                                        if (sptinicioValue == sptinicio.options[i].value)
                                            sptinicio.selectedIndex = sptinicio.options[i].index;
                                    }

                                    for (var i = 0; i < sptfim.options.length; i++) {
                                        if (sptfimValue == sptfim.options[i].value)
                                            sptfim.selectedIndex = sptfim.options[i].index;
                                    }

                                    if (ssuid) {
                                        for (var i = 0; i < ssuid.options.length; i++) {
                                            if (ssuidValue == ssuid.options[i].value)
                                                ssuid.selectedIndex = ssuid.options[i].index;
                                        }
                                    }

                                    if (sptparecer)
                                        sptparecer.value = getElementText('sptparecer', res);

                                    if (sptuntdsc)
                                        sptuntdsc.value  = getElementText('sptuntdsc',  res);

                                    $('loader-container').hide();
                                }
                            });
}

function carregarTotalizadores()
{
    return new Ajax.Request(window.location.href,
                            {
                                method: 'post',
                                parameters: '&reqt='+ reqt +'&req=carregarTotalizadores&sbaporescola='+porEscola,
                                onComplete: function(res)
                                {
                                    $('totalizadoresSubacao').innerHTML = res.responseText;
                                    $('loader-container').hide();
                                }
                            });
}
	    
function carregarParecerPadrao(){
	return new Ajax.Request(window.location.href,
    						{
                            	method: 'post',
								parameters: '&parecerPadrao=true&cosano=' + anoExercicio,
								onComplete: function(res) {
									if( $( 'parecerPadrao_'+anoExercicio ).checked ){
										$( 'sptparecer_' + anoExercicio ).value = res.responseText;
										$( 'sptparecer_' + anoExercicio ).disabled = true;
									}
									else{
										$( 'sptparecer_' + anoExercicio ).disabled = false;
										$( 'sptparecer_' + anoExercicio ).value = "";
									}
								}
							});
}

function extratoEscolas(qfaid)
{
    return windowOpen('/cte/cte.php?modulo=principal/extratoescolassubacao&acao=A&sbaid='+ subacao +'&qfaid=' + qfaid,
                      'extratoEscolas',
                      'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}
 
/************** ADITIVOS *********************/

function adicionarAditivo(ano){
			return new Ajax.Request(window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&reqt='+ reqt +'&req=novoAditivo',
	                                    onComplete: function(res)
	                                    {
	                                        subacaoPai = subacao;
	                                        subacaoAditivo = res.responseText;
	                                        window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid='+subacaoAditivo+'&sbaidpai='+subacao+'&anoconvenio='+ano;
	                                    }
	                                });	
		}
		
		function irAditivo(sbaidAditivo, ano){
			 window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&aditivo=1&sbaid='+sbaidAditivo+'&sbaidpai='+subacao+'&anoconvenio='+ano;
		}
		
		function voltarSubacaoPai(sbaidPai){
			 return new Ajax.Request(window.location.href,
	                                {
	                                    method: 'post',
	                                    parameters: '&reqt='+ reqt +'&req=voltarsubacaooriginal',
	                                    onComplete: function(res)
	                                    {
	                                    	subacaoOriginal = res.responseText;
	                                        window.location.href = '/cte/cte.php?modulo=principal/par_subacao&acao=A&sbaid='+subacaoOriginal;
	                                    }
	                                });
		}


		/*******************************
		* 
		*	FUNCTION: travaAnosAnteriores(cosano);
		* 	DATE: 03/12/2008
		* 	DESCRIÇÃO:
		* 	Se estiver em uma aba onde o ano e menor que o ano atual, e a forma de execução da subação
		* 	for assistencia financeira, transferencia voluntaria ou assistencia tecnica com complementação financeira,
		*   a aba dos anos anteriores são travadas.
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function travaAnosAnteriores(cosano){
			if(cosano < anoAtualTrava){
				bloquearDados(cosano);
			}else{
				bloquearDados(0);
			}
		
		}
		
		/*******************************
		* 
		*	FUNCTION: travaAbaAnoJaAnalisada(cosano);
		* 	DATE: 02/12/2008
		* 	DESCRIÇÃO:
		* 	Se estiver em Elaboração do PAR ou em  Validação do Município a aba (Ano)
		*   onde o parecer já foi dado trava.
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function travaAbaAnoJaAnalisada(cosano){
			if(	anoParecer2007 == cosano || // Elaboração
				anoParecer2008 == cosano || // Elaboração
				anoParecer2009 == cosano || // Elaboração
				anoParecer2010 == cosano || // Elaboração
				anoParecer2011 == cosano 
				)
			{ // Se o ano conveniado for igual ao ano da aba trava.
				bloquearDados(cosano);
			}else{
				bloquearDados(0);
			}
		}
		
		/*******************************
		* 	FUNÇÃO DE ADITIVO
		*	FUNCTION: travaAbaAnoSeAditivada(cosano);
		* 	DATE: 02/12/2008
		* 	DESCRIÇÃO:
		* 	Se a subação foi conveniada trava a aba que foi conveniada 
		*   onde o parecer já foi dado trava.
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function travaAbaAnoSeAditivada(cosano){
			if(	anoConvenio == cosano){
				$('divAditivo' + cosano).style.display="table-row"; // Mostra botão de Adicionar Aditivo.
				bloquearDados(cosano);
			}else{
				bloquearDados(0);
			}
		}
		
				/*******************************
		* 
		*	FUNCTION: bloquearDados(cosano);
		* 	12/11/2008
		* 	DESCRIÇÃO:
		* 	Trava a subação de acordo com o ano em que foi conveniada com o FNDE.
		*   Exemplo: Se a subação foi conveniada o ano de 2008 a aba de 2008 ficará desablilitada,
		*   será possivel apenas visualizar os dados. 	
		* 
		* 	@PARAM cosano - Ano referente a aba que se encontra.
		* 
		*******************************/
		function bloquearDados(cosano)
	    {   
			if(!novaSub){
				var dadosForms 	= $('frmParSubacao').elements;
				var tamanho 	= $('frmParSubacao').elements.length;
				if( cosano != 0 )
				{ // Se o ano conveniado for igual ao ano da aba trava.
					if(cosano == "2007"){
						ind = 0;
					}else if(cosano == "2008"){
						ind = 1;
					}
					else if(cosano == "2009"){
						ind = 2;
					}
					else if(cosano == "2010"){
						ind = 3;
					}
					else if(cosano == "2011"){
						ind = 4;
					}
					
					document.getElementsByName("adItensComposicao")[ind].style.display="none";
					document.getElementsByName("adBeneficiarios")[ind].style.display="none";
					if(document.getElementsByName("adEscolas")[ind]){
						document.getElementsByName("adEscolas")[ind].style.display="none";
					}
					if(document.getElementById('sptunt_'+cosano)){
						document.getElementById('sptunt_'+cosano).disabled="disabled";
					}
					document.getElementById('sptinicio_'+cosano).disabled="disabled";
					document.getElementById('sptfim_'+cosano).disabled="disabled";
					document.getElementById('sptanoterminocurso_'+cosano).disabled="disabled";
					document.getElementById('sptuntdsc_'+cosano).disabled="disabled";
					if($( 'sptparecer_' + cosano )){
						$( 'sptparecer_' + cosano ).disabled = true;
					}
					if($( 'ssuid_' + cosano )){
						$( 'ssuid_' + cosano ).disabled = true;
					}
					// Desabilita btns.
					
					var btnExcluirItensComposicao = document.getElementsByName('removeItens');
					for( cont=0; cont<btnExcluirItensComposicao.length; cont++ ){
						btnExcluirItensComposicao[cont].style.display="none";
					}
					
					var btnExcluirBenficiario = document.getElementsByName('removeBeneficiario');
					for( cont=0; cont<btnExcluirBenficiario.length; cont++ ){
						btnExcluirBenficiario[cont].style.display="none";
					}
					
					var btnExcluirEscolas = document.getElementsByName('removeEscolas');
					for( cont=0; cont<btnExcluirEscolas.length; cont++ ){
						btnExcluirEscolas[cont].style.display="none";
					}
					for( i=0; i<tamanho; i++ ){
						if(dadosForms[i].type == "button"){
							if( dadosForms[i].id != "btnAnterior" && dadosForms[i].id != "btnProximo" ){
								dadosForms[i].disabled="disabled";
							}
						}
					}
					// Fim desabilita Btns
				}else{ // Se não foi conveniada não bloqueia (OBS: libera os btns para as outras abas.)
					for( i=0; i<tamanho; i++ ){
						if(dadosForms[i].type == "button"){
							dadosForms[i].disabled="";
						}
					}
				}  
    		}
	    }
