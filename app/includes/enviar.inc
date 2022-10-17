<?php
$subacoesSql = '';
$subacoesRecebidas 	= array();
$tamanhoCheck 		= count($_REQUEST['checkbox']);

if($tamanhoCheck){
	for($cont = 0; $cont < $tamanhoCheck; $cont++){
		$subacoesRecebidas[] = $_REQUEST['checkbox'][$cont];
	}
	$subacoesSql = implode(',',$subacoesRecebidas);
	$subacoesSql = 'AND s.sbaid in('.$subacoesSql.')';
	$parid1 			= $_REQUEST['parecer1'];
	$parid2 			= $_REQUEST['parecer2'];
	//if($parid1 && $parid2){
	//	$parid = "in(".$parid1.", ".$parid2.")"; 
	//}else{
		if($parid1){
			$parid = "in(".$parid1.")";
		}else if($parid2){
			$parid = "in(".$parid2.")";
		}
		
	//}
}

// Textos da Justificativa do PTA

$justificativaPadrao = 'O Plano de Metas Compromisso Todos pela Educação (Compromisso), estabelecido pelo Decreto n° 6.094, de 24 de abril de 2007, é parte integrante do Plano de Desenvolvimento da Educação (PDE) e prevê a conjugação de esforços da União e demais entes federados, atuando em regime de colaboração, das famílias e da comunidade, em proveito da melhoria da qualidade da educação básica.

Por meio de adesão voluntária ao Compromisso, Estados, Distrito Federal e Municípios assumem a responsabilidade de promover a melhoria da qualidade da educação básica em suas esferas de competência, cumprindo metas de evolução do Índice de Desenvolvimento da Educação Básica (IDEB), observando-se as diretrizes relacionadas no referido Decreto.

As adesões ao Compromisso norteiam o apoio suplementar e voluntário da União às redes públicas de educação básica dos entes federados e ocorrem mediante a elaboração de um Plano de Ações Articuladas (PAR).

O PAR é o conjunto articulado de ações que visa o cumprimento das metas do Compromisso e a observância das suas diretrizes, a partir de um diagnóstico feito em parceria com técnicos do MEC.

É fundamental mencionar que a realização do diagnóstico, bem como a elaboração do PAR foram feitas de forma participativa, tendo sido a coleta de informações e o seu detalhamento obtidos a partir da discussão conjunta entre os membros da equipe técnica local criada para este fim.

De forma detalhada, o trabalho foi realizado conforme a seguinte sistemática:

Foram consideradas quatro dimensões para análise e geração do diagnóstico e do PAR: Gestão Educacional; Formação de Professores e dos Profissionais de serviço e apoio escolar; Práticas Pedagógicas e Avaliação; e Infra-estrutura e Recursos Pedagógicos.

Para cada uma das dimensões existem áreas e indicadores específicos (52 no total), sendo que para os indicadores existem 4 níveis de descrição dos critérios de pontuação. Cabe considerar que apenas o indicador é objeto de pontuação, logo a maior atenção do processo fica voltada para a sua interpretação.

É importante lembrar que os 4 níveis de descrição dos critérios de pontuação estabelecem, de uma forma detalhada, qual a situação de cada indicador. O juízo de valor sobre o nível de atendimento de cada critério foi construído a partir das fontes disponíveis: i) informações de caráter qualitativo (resultado da discussão da equipe local); e ii) documentação (levantamento de evidências concretas).

A elaboração do PAR iniciou-se após a conclusão do diagnóstico e com base nas informações geradas para cada indicador, o Estado determinou as ações e subações mais pertinentes para encaminhamento.

A execução dessas ações e subações foi definida como podendo ser de responsabilidade direta deste Estado, além de assistência técnica, com e sem complementação financeira do MEC, descentralizações, transferências legais e transferências voluntárias do Ministério.

O PAR foi apresentado à Comissão Técnica do Compromisso Todos pela Educação, formada por representantes das Secretarias-fim do Ministério, do FNDE, da CAPES e do INEP e, após uma série de ajustes acordados, foram aprovadas as ações:

- Gestão Educacional;

- Formação de Professores e de Profissionais de Serviço e Apoio Escolar;

- Infra-estrutura e Recursos Pedagógicos;

- Práticas Pedagógicas.';

$justificativaCaminhoEscola = 'A Organização das Nações Unidas para a Educação, a Ciência e a Cultura (UNESCO) reconhece a educação como um fator determinante no desenvolvimento socioeconômico de qualquer nação. A complexa realidade do Brasil, por sua dimensão continental e sua diversidade cultural, dificulta a elaboração e execução de políticas públicas condizentes com realidade de cada local sem a efetiva colaboração de todos os entes federados.

A Lei de Diretrizes e Bases da Educação - Lei nº 9.394, de 1996 -, define e regulariza o sistema de educação brasileiro. Em seu artigo 9º, inciso III, determina a União "prestar assistência técnica e financeira aos Estados, ao Distrito Federal e aos Municípios para o desenvolvimento de seus sistemas de ensino e o atendimento prioritário à escolaridade obrigatória, exercendo sua função redistributiva e supletiva".

Nesse contexto, consoante disposto no Decreto nº 6.768, de 2009, que disciplina o Programa Caminho da Escola, a União, por intermédio do Ministério da Educação, apoiará os entes federativos na aquisição de veículos para transporte dos estudantes, preferencialmente, da zona rural. 

O Programa Caminho da Escola compreende a aquisição, por meio de adesão à ata de pregão eletrônico, para registro de preços - disciplinado pelo FNDE -, de veículos padronizados para o transporte escolar. O serviço, apontado, dentre outros objetivos, como fundamental para o acesso e a permanência dos estudantes moradores da zona rural nas escolas públicas da educação básica, reduzindo a evasão escolar, em observância às metas do Plano Nacional de Educação, serve para deslocar o aluno de sua residência ou local específico previamente acordado até a instituição de ensino. Em função do número de alunos desta municipalidade que residem na zona rural e que utilizam o transporte escolar entendemos como fundamental a assistência financeira da união para aquisição de veículos escolares, como forma de contribuir para o acesso e permanência dos alunos do campo nas escolas e reduzir os índices de evasão escolar. 
';

$_REQUEST['prsjustificativapta'] = $_REQUEST['prsjustificativapta'] == 'caminhoEscola' ? $justificativaCaminhoEscola : $justificativaPadrao; 

/*******************************
* 
*	Descrição: Arquivo responsável pelo envio do convênio entre o SIMEC e o FNDE
* 
*******************************/
	session_start();

/*******************************
*	Constantes & Variaveis
*******************************/
	/******* Constantes para conexão com o Banco de Dados  *******/
	/*
	switch($_SESSION['baselogin']) {
		case 'simec':	
			define( 'HOST', '172.20.193.15' );// Base produção FNDE
			break;
			
		case 'simec2008':
		case 'simec_espelho_producao':	
		case 'simec_desenvolvimento':
			//define( 'HOST', '172.20.65.115' );// Base desenv FNDE
			define( 'HOST', '172.20.65.117' );// Base Homologação FNDE
			break; 
	}
*/
	
	 
	define( 'HOST', '172.20.193.29' ); // Base produção FNDE
	//define( 'HOST', '172.20.193.15' );// Base produção FNDE
	//define( 'HOST', '172.20.65.117' );// Base Homologação FNDE
	//define( 'HOST', '172.20.65.115' );// Base desenv FNDE
	define( 'PORT', '8201' );
	define( 'TIMEOUT', 300 );

	/******* Variaveis Globais *******/
	$_SESSION['validaErro'] = array();
	$erroConexao 			= false;
	$idsSubacao 			= array();
	$idsSubacao  			= $subacoesRecebidas;
	
	if($_SESSION['sisid'] == 13 ){ // Se PAR
		$condicaoQuery 		= " s.frmid in(11,3,2) ";
		$local				= "/cte/cte.php";
		$localA				= "cte";
		$tipoProjeto 		= 1; // Codigo do projeto
		$descricaoProjeto 	= "Plano de Metas";// Descrição do projeto
		
	}else if($_SESSION['sisid'] == 14){ // Se Brasil PRO
		$condicaoQuery 		= " s.frmid in(16,17) ";
		$local				= "/brasilpro/brasilpro.php";
		$localA				= "brasilpro";
		$tipoProjeto 		= 2; // Codigo do projeto
		$descricaoProjeto 	= "Brasil Profissionalizado";// Descrição do projeto
	}else{
		echo "<script> 
			alert( 'Você está em um módulo que não tem acesso a está pagina. ' ); 
		  </script>";
		exit;
	}
	
/******************************* 
*	Verifica se documento está finalizado
*******************************/
	$envia_docid 	 = cte_pegarDocid( $_SESSION['inuid']);
	$envia_documento = wf_pegarEstadoAtual( $envia_docid );
	$envia_esdid 	 = (integer) $envia_documento['esdid'];

#############################################################################################################################
/*******************************  
*	Envio Convenio para o FNDE + validações dos dados.
*******************************/
$projetos  = geraprojeto();

if( $_POST['visualiza'] == 'true' ){
	include APPRAIZ . "cte/modulos/principal/visualizarEnvioProjetoSape.inc";
	die;
}

if( $_POST['visualizaBrasilPro'] == 'true' ){
	include APPRAIZ . "brasilpro/modulos/principal/visualizarEnvioProjetoSape.inc";
	die;
}

if( count( $_SESSION['validaErro'] )){
	$_SESSION['validaErroAtualizado'] = true;
	if($_SESSION['sisid'] == 14){
		echo "<script> 
			location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A&exibirerros=true';
		  </script>";
		  exit;
	}
	echo "<script> 
			//location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A&exibirerros=true';
			window.opener.location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A&exibirerros=true'; 
			window.close();
		  </script>";
	exit;
}



$converter = new assocArray2xml;
$xmlMeio   = $converter->array2xml($projetos);

if ( $xmlMeio == "" ){ //Verifica se a subação a ser enviada no XML
	if($_SESSION['sisid'] == 14){
		echo "<script> 
			alert( 'Não há subação a ser enviada!' ); 
			location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
		  </script>";
		exit();	
	}
	echo "<script> 
			alert( 'Não há subação a ser enviada!' ); 
			//location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
			window.close();
		  </script>";
	exit();
}

$xml  = "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?><pta><entidade>";
$xml .= $xmlMeio;
$xml .= "</entidade></pta>";

//TESTE para visualizacao do xml
//ob_get_clean();
//header( "Content-Type: text/xml" );
//echo $xml;
//exit();

$erroConexao 	= false;
abrirConexao($sk,$erroConexao);
if($erroConexao){// Verifica se a Conexão se mantem aberta
	die($erroConexao);
}
$codigo = $mensagem = false;
$respostaCompleta = lerStatus($sk,$codigo,$mensagem);
if($codigo !== 104){ // 1º Retorno do FNDE para Resposta de Conexão Aberta quando envia o XML.
	fclose($sk);
	unset($_SESSION['sqlSubacao']);
	unset($_SESSION['validaErroAtualizado']);
	unset($_SESSION['validaErro'] );
	if($_SESSION['sisid'] == 14){
		echo "<script> 
			alert( 'Erro cód(".$codigo.")".$mensagem."')";
    		"location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
			window.close();
    	  </script>";
		$db->rollback();
		exit();
	}
	
	echo "<script> 
			alert( 'Erro cód(".$codigo.")".$mensagem."')";
			"window.opener.location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A'; ";
    		"//location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
			window.close();
    	  </script>";
	$db->rollback();
	exit();
}else{
	enviarXML($sk,$xml);
  	$codigo = $mensagem = false;
  	lerStatus($sk,$codigo,$mensagem);
  	if($codigo !== 0){ // Retorno do FNDE se o XML foi validado e gravado em sua base com Sucesso.
  		fclose($sk);
  		if($codigo == 24){
  			verificaPendenciasConvenio();
	    	echo "<script>
		    		alert( 'Erro cód(".$codigo.")".$mensagem."');
	    			var pendencias = window.open('http://".$_SERVER['SERVER_NAME']."/".$localA."/errosPendenciaConvenio.php','Erros','width=500,height=600,scrollbars=1');
	    			location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
		    		pendencias.focus();
		    	  </script>";
	    	$db->rollback();
	    	exit();
    	}else{
    		unset($_SESSION['sqlSubacao']);
			unset($_SESSION['validaErroAtualizado']);
			unset($_SESSION['validaErro'] );
			if($_SESSION['sisid'] == 14){
				echo "<script>
		    		alert( 'Erro cód(".$codigo.")".$mensagem."');
    				location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
		    	  </script>";
	    		$db->rollback();
	    		exit();
			}
			
    		echo "<script>
		    		alert( 'Erro cód(".$codigo.")".$mensagem."');
    				//location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
    				window.opener.location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A'; 
    				window.close();
		    	  </script>";
	    	$db->rollback();
	    	exit();
    	}
  	}else{
  		
		$convenio 		= registraSubacoes($idsSubacao); // Cadastra as subações enviadas ao FNDE no banco de dados		
		$RetornoXML 	= $sk;   
		$retornoFNDE 	= leXML($RetornoXML);
		gravaRetornoFNDE($retornoFNDE, $convenio); // Cadastra os processos gerados pelo FNDE.	
		
		$projeto = gravaProjeto($idsSubacao);
		updateProjeto($retornoFNDE, $projeto);
		updateStatusSubacao($idsSubacao);
		
		// GRAVA XML NA PASTA CTE
		$estuf 	= cte_pegarEstuf( $_SESSION['inuid'] );
		$muncod = cte_pegarMuncod( $_SESSION['inuid'] );
		if ( $estuf )
		{
			$descricao = cte_pegarEstdescricao( $estuf );
		}
		else if ( $muncod )
		{
			$descricao = cte_pegarMundescricao( $muncod );
		}	
		
		
		include_once APPRAIZ."includes/classes/fileSimec.class.inc";
		
		$campos		= array("cnvid"	=>$convenio, "prsid"=>$projeto);	
		$file 		= new FilesSimec("convenioxml", $campos ,"cte");
		$arquivo 	= $file->criaArquivo(APPRAIZ."arquivos/".$_SESSION['inuid'].".xml",$xml);
		if($arquivo){
			$file->setMover(APPRAIZ."arquivos/".$_SESSION['inuid'].".xml", "text/xml");
		}
		//FIM - GRAVA XML NA PASTA CTE
		$db->commit();
		unset($_SESSION['sqlSubacao']);
		unset($_SESSION['validaErroAtualizado']);
		unset($_SESSION['validaErro'] );
		if($_SESSION['sisid'] == 14){
			echo "<script>
	    		alert( 'Envio com sucesso');
	    		location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
	    	  </script>";
			exit();
		}
		echo "<script>
	    		alert( 'Envio com sucesso');
	    		//location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A';
	    		window.opener.location.href = '$local?modulo=principal/estrutura_avaliacao&acao=A'; 
	    		window.close();
	    	  </script>";
		exit();
  	}
}
fclose($sk);
#############################################################################################################################
/*******************************
* 
*	Funções
* 		- recuperaPlanointernoAcaoSubacao();
* 		- recuperaitensdecomposicao();
* 		- recuperaEscolas();
*  		- recuperaBeneficiarios();
* 		- recuperavaloresgerais();
* 		- carregaValorUnitarioSubSeEscola();
* 		- registraSubacoes();
* 		- geraprojeto();
* 		- processaLinha();
* 		- abrirconexao();
* 		- lerStatus();
* 		- enviar_xml();
* 		- pegar_valor();
* 		- array2xml();
* 		- arraytransform();
* 		- xmlentities();
*       - leXML();
* 		- Campos_e_tags();
* 		- verificaPendenciasConvenio();
* 		- gravaRetornoFNDE();
* 
*******************************/
	function gravaProjeto($idsSubacao){
		global $db;
		$idInstrumento 	= $_SESSION['inuid'];
		$idsSubacao 	= array_unique( $idsSubacao );
		
		$sql = "INSERT INTO cte.projetosape (prsdata, prsano, inuid, usucpf, prsjustificativapta) 
				VALUES ('now()', date_part('year', current_date),".$idInstrumento.", '".$_SESSION['usucpf']."', '" . $_REQUEST['prsjustificativapta'] . "') RETURNING prsid";

		$idProjeto 	= pg_fetch_result($db->executar( $sql ),0); 
	  	$db->commit();
		
	  	foreach ( $idsSubacao as $idSubacao ){
	  		$sqlRegistraSubacoes = "INSERT INTO cte.projetosapesubacao (prsid, sbaid, pssdata, pssano) 
			  							VALUES ( ".$idProjeto.", ".$idSubacao.",now(), date_part('year', current_date))";				
			$db->executar( $sqlRegistraSubacoes );
	  	}
	  	$db->commit();
	  	return $idProjeto;
	}
	
	function updateProjeto($retornoFNDE, $idProjeto){
		global $db;
		$contprocesso 	= 0;
		$contpi 		= 0;
		for ($i=1; $i <= count($retornoFNDE[0]["projetos"]); $i++) {
			$numeroProcesso	= $retornoFNDE[0]["projetos"][$i]["processo"][$contprocesso];
			$sql = "SELECT c.prsnumeroprocesso FROM cte.projetosape c
					WHERE c.inuid = ".$_SESSION['inuid']." and c.prsnumeroprocesso = ".$numeroProcesso;
			$existeProcesso = $db->pegaUm( $sql );
			if($numeroProcesso != $existeProcesso){
				$prstipo = 'original';
			}else{
				$prstipo = 'aditivo';
			}
			for ($a=0; $a < count($retornoFNDE[0]["projetos"][$i]["pi"]); $a++) {
				$contpi++;
				$sql = "update cte.projetosape set prsnumeroprocesso = '".$numeroProcesso."', prstipo = '".$prstipo."' WHERE prsid = ".$idProjeto;
				if($db->executar( $sql )){
					$db->commit();
				}else{
					$db->rollback();
				}
			}
			$contprocesso++;
		}
		return true;
	}

	/*******************************
	* 	FUNCTION: recuperaPlanointernoAcaoSubacao();
	* 	26/06/2008
	*   CRIADA POR: Thiago Tasca
	*   DESCRIÇÃO:
	*	Recupera os as subações que irão gerar convênio
	* 
	*******************************/
	function recuperaPlanointernoAcaoSubacao(){
		global $db, $subacoesSql;
		
		//if($_SESSION['sisid'] == 13 ){ // Se PAR
			$plano = " and trim(spt.sptplanointerno) <> '' ";
			$selectNumPlano = " spt.sptplanointerno				AS pi, ";
			$fromSis	= " ";
			$orderBy = "spt.sptplanointerno,";
		/*	
		}else if($_SESSION['sisid'] == 14 ){
			$plano = '';
			$selectNumPlano = " pr.prgplanointerno AS pi, ";
			$fromSis	= " INNER JOIN cte.programa 	  	  	pr  ON pr.prgid  	= s.prgid  and trim(pr.prgplanointerno) <> ''
                    		LEFT  JOIN cte.convenioretorno    	cvr ON (cvr.inuid 	= iu.inuid and  spt.sptplanointerno = cvr.prgplanointerno )";
			$orderBy = "";
		}
		*/
		if($_SESSION['sqlSubacao'] && $_SESSION['validaErro']){ // Se tiver erro roda o script anterior.
			$arrSub = $db->record_set( $_SESSION['sqlSubacao'] );
			//unset($_SESSION['sqlSubacao']);
			//dbg($_SESSION['sqlSubacao']);
			return  $arrSub;
		}
		
		$municipio = cte_pegarMuncod( $_SESSION["inuid"] );
		if($municipio != NULL){ 		//*** Municipio ***//
			$select  = "m.muncod		AS codigoibge,
					    m.mundescricao	AS nomeentidade,
					    m.mundescricao 	AS nomeMunicipio,";
			$from    = "INNER JOIN territorios.municipio  m  ON m.muncod = iu.muncod";
			$where   = "m.muncod = '".$municipio."' AND"; 
			
		}else{ 							//*** Estado ***//
			$estado	= cte_pegarMuncodEstatual($_SESSION["inuid"]);
			$itrid 	= cte_pegarItrid($_SESSION["inuid"]);  
			
			$select = "m.estdescricao	AS nomeentidade,";
			$from   = "INNER JOIN territorios.estado m ON m.estuf = iu.estuf";
			$where  = "m.muncodcapital 	= '".$estado."' 
					   AND iu.inuid 	= '".$_SESSION["inuid"]."' 
					   AND iu.itrid 	= '".$itrid."' AND";
		}
		
			$sql= "
				SELECT distinct	".$select."
						s.sbaid							AS idsubacao,
						a.aciid							AS acao,
						-- a.acidtinicial 					AS cronogramaexecucaoinicial,
						case length(cast(spt.sptinicio as character(2)))
							when 1 then spt.sptano||'-'||'0'||spt.sptinicio||'-'||'01  00:00:00'
							when 2 then spt.sptano||'-'||spt.sptinicio||'-'||'01  00:00:00' end AS cronogramaexecucaoinicial,
							
						case length(cast(spt.sptinicio as character(2)))
							when 1 then spt.sptano||'-'||'0'||spt.sptinicio||'-'||'01  00:00:00'
							when 2 then 
							case when spt.sptanoterminocurso = null or spt.sptanoterminocurso = 0 
								then spt.sptano else spt.sptanoterminocurso end
							||'-'||spt.sptinicio||'-'||'01  00:00:00' end AS cronogramaexecucaofinal,
							
							
						-- a.acidtfinal 					AS cronogramaexecucaofinal,
						to_char(a.acidtinicial, 'YYYY')	AS anoinicioexecucao,
						to_char(a.acidtfinal, 'YYYY')	AS anofimexecucao,
						s.sbaid							AS codigodasubacao,
						s.sbadsc						AS descricaosubacao,
						s.sbaporescola					AS cronogramaporescola,
						s.sbacategoria					AS categoriadespesa,
						m.estuf 						AS uf,
						pp.partexto						AS parecer,
						--'teste de parecer'			AS parecer,
						pp.usucpf						AS cpfparecista, 
						--'22973613191'					AS cpfparecista, 
						su.usunome      				AS nomeparecista,
						--'Renilda'						AS nomeparecista,
						spt.sptparecer					AS parecersubacao,
						".$selectNumPlano."
						d.dimcod 						AS codigoDimensao,
						d.dimdsc 						AS descricaoDimensao,
						u.unddsc 						AS descricaounidademedida,
						u.undid 						AS codigounidademedida,
						spt.sptuntdsc                   AS comentarioitens,
						d.dimcod || '&10-- ' || 
						d.dimdsc || '&10-- ' || 
						ad.ardcod || '&10-- ' || 
						ad.arddsc || '&10-- ' || 
						i.indcod || '&10-- ' || 
						i.inddsc || '&10-- ' || 
						a.acidsc || '&10-- ' ||
						s.sbadsc    || '&10-- ' || 
						s.sbastgmpl || '&10-- ' || 
						u.unddsc    || '&10-- ' || 
						coalesce(cast(spt.sptunt as varchar),'0') 	 AS detalhamento,
						sptnumprocesso  AS numeroprocesso,
						--cvr.cvrnumprocesso 				AS numeroprocesso,
						spt.sptano AS anodoitens
				FROM cte.dimensao d
					INNER JOIN cte.areadimensao 	  	ad  ON ad.dimid 	= d.dimid
					INNER JOIN cte.indicador 	  	  	i   ON i.ardid  	= ad.ardid
					INNER JOIN cte.criterio		  	  	c   ON c.indid  	= i.indid
					INNER JOIN cte.pontuacao 	  	  	p   ON p.crtid  	= c.crtid and p.indid = i.indid and p.ptostatus = 'A'
					INNER JOIN cte.instrumentounidade 	iu  ON iu.inuid 	= p.inuid and iu.inuid 	= '".$_SESSION["inuid"]."'
					".$from."
					INNER JOIN cte.acaoindicador 	  	a   ON a.ptoid   	= p.ptoid
					INNER JOIN cte.subacaoindicador   	s   ON s.aciid   	= a.aciid
					INNER JOIN cte.subacaoparecertecnico spt ON spt.sbaid 	= s.sbaid and sptano = date_part('year', current_date) ".$plano."
					INNER JOIN cte.unidademedida 	  	u   ON u.undid   	= s.undid
					".$fromSis." --or ( cvr.inuid = iu.inuid and cvr.cnvid is null )
					LEFT JOIN cte.parecerpar 	  		pp  ON pp.inuid 	= iu.inuid
					LEFT JOIN seguranca.usuario 	  su  ON su.usucpf 		= pp.usucpf
					
				WHERE
					".$where."
					".$GLOBALS['condicaoQuery']." 	-- assistencia financeira.
					AND spt.ssuid = 3   			-- aprovada pela comissão.
					--AND pp.tppid = 1	 			-- parecer financeiro dado
					AND pp.parid ".$GLOBALS['parid']."
					--AND parhistorico in(SELECT max(parhistorico) FROM cte.parecerpar p WHERE date_part('year',pardata)= date_part('year', current_date) AND p.inuid = iu.inuid ) -- ultimo parecer 
					--AND date_part('year', pp.pardata)= date_part('year', current_date) -- parecer do ano corrente	
					".$subacoesSql."
				ORDER BY 
					".$orderBy."
					d.dimcod";
	//dbg($sql,1);
		$_SESSION['sqlSubacao'] = $sql;
		return  $db->record_set( $sql );
	}

	/*******************************
	* 	FUNCTION: recuperaitensdecomposicao($subacao, $escola);
	* 	18/08/2008
	*   DESENVOLVIDO POR: Thiago Tasca
	*   DESCRIÇÃO:
	*	Recupera os itens de composição da subação.
	* 	@PARAM  $subacao - numeric
	* 			$escola  - boolean	
	* 
	*******************************/
	function recuperaitensdecomposicao($subacao, $escola, $ano){
		global $db;
		if($escola == 't'){ // SQL quando a subação for por escola
			$select = "	somaquantidadeitens 	AS quantidade,
						sum(qtd.qfaqtd)				AS quantidadeglobal,
					  ";
			
			$inner = "	INNER JOIN
							--cte.qtdfisicoano qtd ON sba.sbaid = qtd.sbaid AND qtd.qfaano = '$ano'
							cte.qtdfisicoano qtd ON sba.sbaid = qtd.sbaid AND qtd.qfaano = date_part('year', current_date)
						LEFT JOIN 
							(	SELECT cosid, SUM(ecsqtd) as somaquantidadeitens 
								FROM cte.escolacomposicaosubacao 
								GROUP BY cosid) ecs ON cos.cosid = ecs.cosid";
			$groupby = " GROUP BY 
						cos.cosord,
						cos.cosdsc, 		
						cos.cosvlruni,
						ud.unddid,		
						ud.undddsc,
						ecs.somaquantidadeitens,
						cos.cosid	";
			
		}else{ // SQL quando a subação for global
			$select = "	cos.cosqtd 		AS quantidade,
					    spt.sptunt		AS quantidadeglobal,
					  ";
			
			$inner = "	INNER JOIN
							--cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = '$ano'
							cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = date_part('year', current_date)
							";
							
		} 
		
		$sql="		SELECT 
						".$select."
						cos.cosid 		AS codigo,
						cos.cosord   	AS ordem,
						cos.cosdsc 		AS descricaoitem,
						date_part('year', current_date) as ano,
						cos.cosvlruni 	AS valorunitario,
						ud.unddid		AS codigounidademedida,
						ud.undddsc		AS descricaounidademedida
					FROM
						cte.subacaoindicador sba
					INNER JOIN
						--cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = '$ano'
						cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = date_part('year', current_date)
					".$inner."
					INNER JOIN 
						cte.unidademedidadetalhamento ud ON ud.unddid = cos.unddid
					WHERE
					    sba.sbaid =".$subacao. 
					$groupby;
					//if($dados['idsubacao'] == "3514"){
	//if( $subacao  == "2253152"){
		//dbg($sql,1);
	//}
		return  $db->carregar( $sql );
	}
	
	/*******************************
	* 	FUNCTION: recuperaEscolas($codigoSubacao);
	* 	16/07/2008
	*   DESENVOLVIDO POR: Thiago Tasca
	*   DESCRIÇÃO:
	*	Recupera as escolas da subação quando o cronograma for por escola.
	* 	@PARAM  $codigoSubacao - numeric
	* 
	*******************************/
	function recuperaEscolas($codigoSubacao){
		global $db;
		$sql = "SELECT  e.entnome as nomeEscola , 
					    CASE WHEN substring(e.entcodent, 1, 2) = 'EN' OR length(trim(e.entcodent)) < 8
						THEN '' 
						ELSE e.entcodent END AS codigoInep, --Se o codigo da escola começar com EN (fora da base do inep coloca em branco.)
					    e.entescolanova as escolanova, 
					    coalesce(sum( q.qfaqtd ),0) as quantidadeAlunos
				FROM  cte.qtdfisicoano q
				INNER JOIN entidade.entidade e ON e.entid = q.entid
				INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
				where
					q.sbaid = ".$codigoSubacao." AND fe.funid = 3 and qfaano::text =  to_char(now(), 'YYYY')
				GROUP BY 
					e.entcodent, 
					e.entnome,
					e.entescolanova";
//dbg($sql,1);
		return  $db->carregar( $sql );
	}

	/*******************************
	* 	FUNCTION: recuperaBeneficiarios($codigoSubacao);
	* 	16/07/2008
	*   DESENVOLVIDO POR: Thiago Tasca
	*   DESCRIÇÃO:
	*	Recupera os beneficiarios da subação.
	* 	@PARAM  $codigoSubacao - numeric
	* 
	*******************************/
	function recuperaBeneficiarios($codigoSubacao){
		global $db;
		$sql = "SELECT b.benidfnde  AS codigobeneficiario,
					   b.bendsc     AS descricaobeneficiario,
				       sb.vlrurbano AS quantidadezonaurbana ,
				       sb.vlrrural  AS quantidadezonarural 
				FROM cte.subacaobeneficiario sb
				INNER JOIN cte.beneficiario b ON b.benid = sb.benid
				WHERE    sb.sbaid = ".$codigoSubacao." and sb.sabano = date_part('year', current_date)
				ORDER BY sb.benid";
		//dbg($sql,1);
		return  $db->carregar( $sql );
	}
	
	/*******************************
	* 	FUNCTION: recuperavaloresgerais($subacao, $escola);
	* 	15/08/2008
	*   DESENVOLVIDO POR: Thiago Tasca
	*   DESCRIÇÃO:
	*	Recupera o cronograma do ano, valor concedente e o proponente de cada subação.
	*   	- Calculo para o valor concedente: Soma de todos valores do item * a quantidade * 0.99% -- Valor que o MEC irá pagar.
	* 		- Calculo para o valor proponente: Soma de todos valores do item * a quantidade * 0.01% -- Valor que o estado ou município irá pagar.
	* 	@PARAM  $codigoSubacao  - numeric
	* 			$escola			- boolean
	* 
	*******************************/
	function recuperavaloresgerais($subacao, $escola, $ano){
		global $db;
		
		if($escola == 't'){ // SQL quando a subação for por escola
			$select = "	sum(cos.cosvlruni * ecs.ecsqtd) 		AS cronograma,
						(sum( cos.cosvlruni * ecs.ecsqtd)*0.99) AS valorconcedente,
						(sum(cos.cosvlruni * ecs.ecsqtd)*0.01) 	AS valorproponente,";
			$inner = "INNER JOIN cte.escolacomposicaosubacao ecs ON cos.cosid = ecs.cosid ";
			
		}else{ // SQL quando a subação for global
			$select = "	sum(cos.cosqtd * cos.cosvlruni ) 			AS cronograma,
						(sum(cos.cosqtd * cos.cosvlruni ) * 0.99) 	AS valorconcedente,
						(sum(cos.cosqtd * cos.cosvlruni ) * 0.01) 	AS valorproponente,";
			$inner = "	--INNER JOIN cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = '".$ano."'
						INNER JOIN cte.subacaoparecertecnico spt ON sba.sbaid = spt.sbaid AND sptano = date_part('year', current_date)
					";
		}
		
		$sql=	"select ".$select."	
					'$ano' as ano
					from 
						cte.subacaoindicador sba
					INNER JOIN
						--cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = '".$ano."'
						cte.composicaosubacao cos ON sba.sbaid = cos.sbaid AND cosano = date_part('year', current_date)
					".$inner."
					where sba.sbaid = ".$subacao."
					group by sba.sbaid";
						//2308341
		//if( $subacao  == "2253152"){
	//	dbg($sql,1);
		//}
		return  $db->carregar( $sql );
	}
	
	
	/*******************************
	* 	FUNCTION: registraSubacoes($idsSubacao);
	* 	17/06/2008
	*   DESENVOLVIDO POR: Thiago Tasca
	*   DESCRIÇÃO:
	*	registra envio do convênio e suas subações nas tabelas: cte.convenio e cte.subacaoconvenio.
	* 	@PARAM  $idsSubacao  - Array
	* 
	*******************************/
	function registraSubacoes($idsSubacao){
		global $db;
	  	$idInstrumento 			= $_SESSION['inuid'];
	  	$idsSubacao 			= array_unique( $idsSubacao );
	  	$sqlBaseRegistraEnvio 	= "INSERT INTO cte.convenio ( inuid, usucpf  ) 
	  							   VALUES (".$idInstrumento." , '".$_SESSION['usucpf']."') returning cnvid";
	  	$idConvenio 			= pg_fetch_result($db->executar( $sqlBaseRegistraEnvio ),0);
	  	$db->commit();
	  	$sql = "SELECT sac.sbaid FROM cte.convenio c
				INNER JOIN cte.subacaoconvenio sac ON sac.cnvid = c.cnvid
				WHERE c.inuid = ".$_SESSION['inuid'];
	  	$subacoesconvenio = $db->carregar($sql);
	  	foreach ( $idsSubacao as $idSubacao ){
	  		if ( $idSubacao ){
	  			if(is_array($subacoesconvenio)){
		  			foreach( $subacoesconvenio as $subacoesconvenio ){
			  			if($subacoesconvenio == $idSubacao ){
			  				$sbctipo = "A";
			  			}else{
			  				$sbctipo = "N";
			  			}
		  			}
	  			}else{
	  				$sbctipo = "N";
	  			}
	  			$sqlBaseRegistraSubacoes = "INSERT INTO cte.subacaoconvenio (cnvid, sbaid, sbctipo, sbcano) VALUES ( $idConvenio, $idSubacao, '".$sbctipo."', date_part('year', current_date) )";
	  			$db->executar( $sqlBaseRegistraSubacoes );
	  		}
	  	}
	  	return $idConvenio;
	}	
	
	/*******************************
	* 	FUNCTION: geraprojeto();
	*   Modificada POR: Thiago Tasca
	* 	17/06/2008
	*   DESCRIÇÃO:
	*	Responsavel por montar o Array com o conteudo que irá montar o XML chamando a função processalinha.
	*   Guarda no array 'idsSubacao' as subações que foram enviadas.
	* 
	*******************************/
	function geraprojeto(){
		global $db;
	    $recurso 	= recuperaPlanointernoAcaoSubacao();
	  	$total		= $db->conta_linhas($recurso) + 1;
	  	$projetos 	= array();
	
	  	for ( $linha = 0; $linha < $total; $linha++ ){
	  		$dados = $db->carrega_registro( $recurso, $linha );
	  		
	  		processaLinha($dados,$projetos);
		    array_push( $GLOBALS['idsSubacao'], (integer) $dados["codigodasubacao"] );
	  	}
	
	  	if(is_array($projetos["projetos"])){
		  	foreach( $projetos["projetos"] as $indice1 => $arProjeto ){
		  		if( $arProjeto == "projeto" ) continue;
		  		foreach(  $arProjeto["acoes"] as $indice2 => $arAcao ){
		  			if( $arAcao == "acao" ) continue;
		  			
		  			if( isset( $arAcao["cronograma"] ) && isset( $arAcao["valorconcedente"] ) && isset( $arAcao["valorproponente"] ) ){
		  				
			  			foreach($arAcao["cronograma"] as $indice3 => $arCronograma) {
			  				if( $arCronograma == "periodo" ) continue;
			  				
			  				if( isset( $projetos["projetos"][$indice1]["acoes"][$indice2]["cronograma"][$indice3]["valor"] ) ){
				  				$valorTotal = (string) number_format(round($arCronograma["valor"],3),2,".","");
				  				$valorTotalAntesPonto = substr($valorTotal,0, strpos($valorTotal, '.'));
				  				$valorTotalDepoisPonto = substr($valorTotal, strpos($valorTotal, '.')+1);
				  				$valorConcedente = number_format(round($arAcao["valorconcedente"],2),2,".","");
				  				$valorProponente = number_format(round($arAcao["valorproponente"],2),2,".","");
				  				$valorTotalCP = (string) $valorConcedente + $valorProponente;
				  				$valorTotalCPAntesPonto = substr($valorTotalCP,0, strpos($valorTotalCP, '.'));
				  				$valorTotalCPDepoisPonto = substr($valorTotalCP, strpos($valorTotalCP, '.')+1);
				  				
				  				$valorTotalCPDepoisPonto = (int) $valorTotalCPDepoisPonto;
				  				$valorTotalDepoisPonto = (int) $valorTotalDepoisPonto;
				  				$subtracao = $valorTotalCPDepoisPonto > $valorTotalDepoisPonto ? $valorTotalCPDepoisPonto - $valorTotalDepoisPonto : $valorTotalDepoisPonto - $valorTotalCPDepoisPonto;
				  				
				  				if( ($valorTotalCPDepoisPonto != $valorTotalDepoisPonto) && ($valorTotalAntesPonto == $valorTotalCPAntesPonto) && ($subtracao < 2) ){
				  						$projetos["projetos"][$indice1]["acoes"][$indice2]["cronograma"][$indice3]["valor"] = $valorTotalCP;
				  				}else{
				  					$projetos["projetos"][$indice1]["acoes"][$indice2]["cronograma"][$indice3]["valor"] = $valorTotal;
				  				}
			  				}
			  			}
			  			if( isset( $projetos["projetos"][$indice1]["acoes"][$indice2]["valorconcedente"] ) && $projetos["projetos"][$indice1]["acoes"][$indice2]["valorproponente"] ){
				  			$projetos["projetos"][$indice1]["acoes"][$indice2]["valorconcedente"] = number_format(round($arAcao["valorconcedente"],2),2,".","");
				  			$projetos["projetos"][$indice1]["acoes"][$indice2]["valorproponente"] = number_format(round($arAcao["valorproponente"],2),2,".","");
			  			}
		  			}
		  		}
		  	}
	  	}
	  	return $projetos;
	}
	
	/*******************************
	* 	processaLinha();
	*   Modificada POR: Thiago Tasca
	*	Le cada linha do Array e Monta um array 
	*   com os campos solicitados pelo FNDE (Array com as Tags do XML)
	* 
	*******************************/
	function processaLinha($dados,&$projetos){
		
	/************** Regras de Negócio ************/
		if(($dados['categoriadespesa'] == '') || ($dados['categoriadespesa'] == 0) ){ // Se não existir categoria da subação vai como default 4
			$dados['categoriadespesa'] = 4; 
		}
		
		if($dados['descricaosubacao']){
			$dados['descricaosubacao'] = substr($dados['descricaosubacao'],0,240);
		}
		
	/************** Validações de campos em branco ************/
		
		$erroSubacao = $dados['codigodasubacao'];
		if(!$dados['parecersubacao']){
			$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
			$_SESSION['validaErro'][$erroSubacao]['parecer']= 'O parecer técnico na subação não está preenchido';
		}
		if(!$dados['parecer']){
			$_SESSION['validaErro'][$erroSubacao]['parecerGeral']= 'O parecer Técnico não foi preenchido';
		}
		if(!$dados['pi']){
			$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
			$_SESSION['validaErro'][$erroSubacao]['PI']= "O programa selecionado na subação não tem plano interno - PI vinculado. Entrar em contato com o Gestor.";
		}
		
	//********************* Valores unitarios da Subação + chamada para recuperar itens de composição. ***************************//
		$dados['valorunitariosubacao'] = 0;
		$itens = recuperaitensdecomposicao($dados['codigodasubacao'], $dados['cronogramaporescola'], $dados['anodoitens']);
		if($itens != false) {
					foreach($itens as $item){
						$totalQuantidades += $item['quantidade'];
						$total = $item['valorunitario'] * $item['quantidade'];
						$dados['valorunitariosubacao'] += $total;
					}	
			if($item['quantidadeglobal'] == 0){
				$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
				$_SESSION['validaErro'][$erroSubacao]['quantidadesubacao']= 'Obrigatório preencher a quantidade das escolas. ';
				
			}
			if($item['quantidadeglobal'] != 0){
				$dados['valorunitariosubacao'] = $dados['valorunitariosubacao'] / $item['quantidadeglobal'] ;	
			}
			$dados['quantidadesubacao'] = $item['quantidadeglobal'];
		}		
		
		

		$valoresGerais =  recuperavaloresgerais($dados['codigodasubacao'], $dados['cronogramaporescola'], $dados['anodoitens']);
		
		$projetos['codigoibge']		= $dados['codigoibge'];
		//$projetos['nomeentidade']	= $dados['nomeentidade'];
		//$projetos['nomemunicipio']	= $dados['nomemunicipio'];
		$projetos['uf']				= $dados['uf'];	 
		$projetos['parecer']		= $dados['parecer'];
		$projetos['cpfparecista'] 	= $dados['cpfparecista'];
		$projetos['nomeparecista'] 	= $dados['nomeparecista'];
		
		/************** Carrega os projetos (PIS) ************/
		if($dados['numeroprocesso'] == ''){
			$projeto = 'novo';
		}else{
			$projeto = $dados['numeroprocesso'];
		}
		
		
		$pi 													= $projeto;//$dados['pi'];
		$projetos['projetos']['NOME_FILHO'] 					= 'projeto';
		$projetos['projetos'][$pi]['tipoprojeto'];
		$projetos['projetos'][$pi]['tipoprojeto']['codigo']		= $GLOBALS['tipoProjeto'];
		$projetos['projetos'][$pi]['tipoprojeto']['descricao']	= $GLOBALS['descricaoProjeto'];
		//$projetos['projetos'][$pi]['pi'] 						= $dados['pi'];
		$projetos['projetos'][$pi]['anoinicioexecucao'] 		= $dados['anoinicioexecucao'];
		$projetos['projetos'][$pi]['anofimexecucao'] 			= $dados['anofimexecucao'];
		$projetos['projetos'][$pi]['numeroprocesso'] 			= $dados['numeroprocesso'];
		
		/************** Carrega as ações ************/
		$codigodimensao 																	= $dados['codigodimensao'];
		$projetos['projetos'][$pi]['acoes']['NOME_FILHO'] 									= 'acao';
	    $projetos['projetos'][$pi]['acoes'][$codigodimensao]['codigodimensao']				= $dados['codigodimensao'];
	    $projetos['projetos'][$pi]['acoes'][$codigodimensao]['descricaodimensao'] 			= $dados['descricaodimensao'];
	    
	    //************** Carrega os Cronogramas das ações **************//
	    	//$valoresGerais[0]['valorconcedente'] = str_replace(",", ".", $valoresGerais[0]['valorconcedente']);
	    	//$valoresGerais[0]['valorproponente'] = str_replace(",", ".", $valoresGerais[0]['valorproponente']); 
	    	$teste1 = $teste1 + $valoresGerais[0]['valorconcedente']; 
	 		$teste2 = $teste2 + $valoresGerais[0]['valorproponente']; 
	    	//$teste1 = number_format($teste1, 2, '.', '');
			//$teste2 = number_format($teste2, 2, '.', ''); 
			$teste = $teste1 + $teste2;
	    
	    
	    	$projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronograma']['NOME_FILHO'] = 'periodo';
		    if(is_array($valoresGerais[0])){
		    	foreach($valoresGerais[0] as $ano=>$valor){
					if($ano == "cronograma"){
		    			if($valor != 0){
		    				$anodositens = $valoresGerais[0]['ano'];
			    			$projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronograma'][$anodositens]['ano'] = $valoresGerais[0]['ano'];
			    			//$projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronograma'][$anodositens]['valor'] = $projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronograma'][$anodositens]['valor'] + $valor;
		    			$projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronograma'][$anodositens]['valor'] = $projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronograma'][$anodositens]['valor'] + $teste;
		    			}
					}
		    	}
		    }
		   // $projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronograma'][$anodositens]['valor'] = number_format($projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronograma'][$anodositens]['valor'], 2, '.', ''); 
					
	    $projetos['projetos'][$pi]['acoes'][$codigodimensao]['detalhamento'] .= $dados['detalhamento'];
	   
	    //************** Carrega Valor Concedente e Proponente **************//
		//$projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorconcedente'] = str_replace(",", ".", $projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorconcedente']) + $valoresGerais[0]['valorconcedente']; 
	    //$projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorproponente'] = str_replace(",", ".", $projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorproponente']) + $valoresGerais[0]['valorproponente']; 
	    //$projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorconcedente'] = number_format($projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorconcedente'], 2, '.', ''); 
		//$projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorproponente'] = number_format($projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorproponente'], 2, '.', ''); 
	    $projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorconcedente'] = $projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorconcedente'] + $valoresGerais[0]['valorconcedente']; 
	 	$projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorproponente'] = $projetos['projetos'][$pi]['acoes'][$codigodimensao]['valorproponente'] + $valoresGerais[0]['valorproponente']; 
	   
	    //************** Cronograma de execução inicial e final **************//
	    $projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronogramaexecucaoinicial'] 	= $dados['cronogramaexecucaoinicial'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['cronogramaexecucaofinal'] 	= $dados['cronogramaexecucaofinal'];
		
		//************** Carrega Escolas (Agrupado todas as Escolas das subações para a ação)**************
		if($dados['cronogramaporescola'] == 't'){
			$escolas = recuperaEscolas($dados['codigodasubacao']);
			$projetos['projetos'][$pi]['acoes'][$codigodimensao]['escolas']['NOME_FILHO'] = 'escola';
			if($escolas !=false){
				foreach($escolas as $dadosEscolas){				
						$codigoinep = $dadosEscolas['codigoinep'];
						$projetos['projetos'][$pi]['acoes'][$codigodimensao]['escolas'][$codigoinep]['codigoinep'] 			= $dadosEscolas['codigoinep'];
						$projetos['projetos'][$pi]['acoes'][$codigodimensao]['escolas'][$codigoinep]['nomeescola'] 			= $dadosEscolas['nomeescola'];
						$projetos['projetos'][$pi]['acoes'][$codigodimensao]['escolas'][$codigoinep]['quantidadealunos'] 	= $dadosEscolas['quantidadealunos'];
				}
			}else{
						$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
						$_SESSION['validaErro'][$erroSubacao]['escola'] = "Subação não possui <i>escolas</i>.";
			}
		}
		
		//************** Carrega Beneficiarios (Agrupado todos dos Beneficiarios das subações para a ação)**************
		$beneficiarios = recuperaBeneficiarios($dados['codigodasubacao']);
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['beneficiarios']['NOME_FILHO'] = 'beneficiario';
		if($beneficiarios != false){
			foreach($beneficiarios as $dadosBeneficiarios){	
				if($dadosBeneficiarios['quantidadezonarural'] == '' && $dadosBeneficiarios['quantidadezonaurbana'] == '' ){
					$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
					$_SESSION['validaErro'][$erroSubacao]['beneficiarios'][] = $dadosBeneficiarios['descricaobeneficiario'].": Os dados não foram preenchidos";
				}
				
				$codigobeneficiario = $dadosBeneficiarios['codigobeneficiario'];
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['beneficiarios'][$codigobeneficiario]['codigobeneficiario'] 	= $dadosBeneficiarios['codigobeneficiario'];
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['beneficiarios'][$codigobeneficiario]['descricaobeneficiario'] = $dadosBeneficiarios['descricaobeneficiario'];
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['beneficiarios'][$codigobeneficiario]['quantidadezonarural'] 	= $dadosBeneficiarios['quantidadezonarural'];
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['beneficiarios'][$codigobeneficiario]['quantidadezonaurbana'] 	= $dadosBeneficiarios['quantidadezonaurbana'];  
			
			}
		}else{
			$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];	
			$_SESSION['validaErro'][$erroSubacao]['beneficiarios'] = "Subação não possui <i>beneficiário</i>.";
			}
		
		//************** Carrega Especificações (As Subações)**************
		if($dados['descricaosubacao'] == ''){
			$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
			$_SESSION['validaErro'][$erroSubacao]['descricaosubacao'] = "Subação não possui <i>descrição</i>.";
		}

		if($dados['quantidadesubacao'] == ''){
			$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
			$_SESSION['validaErro'][$erroSubacao]['quantidadesubacao'] = "Obrigatório preencher a quantidade do cronograma.";
		}
		
		
		$codigosubacao = $dados['codigodasubacao'];
		$dados['valorunitariosubacao'] = number_format($dados['valorunitariosubacao'], 2, ',', ''); //str_replace(".", ",", $dados['valorunitariosubacao']);
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes']['NOME_FILHO'] = 'especificacao';
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['pi'] 						= $dados['pi'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['codigosubacao'] 			= $dados['codigodasubacao'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['descricaosubacao'] 			= $dados['descricaosubacao'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['comentarioitens'] 			= $dados['comentarioitens'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['codigounidademedida'] 		= $dados['codigounidademedida'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['descricaounidademedida'] 	= $dados['descricaounidademedida'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['quantidade'] 				= $dados['quantidadesubacao'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['valorunitariosubacao'] 		= $dados['valorunitariosubacao'];
		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['categoriadespesa'] 			= $dados['categoriadespesa'];	
		
		//************** Carrega Especificações.Itens (As Subações)**************	
		//$itens = recuperaitensdecomposicao($dados['codigodasubacao'], $dados['cronogramaporescola']);

		$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['itens']['NOME_FILHO'] = 'item';
		if($itens != false){
			foreach($itens as $item){
				$codigoItem = $item['codigo'].$item['descricaoitem'];
				$descricaoitem = $item['descricaoitem'];
				if(($item['valorunitario'] == '') ||($item['valorunitario'] == 0)  ){
					$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
					$_SESSION['validaErro'][$erroSubacao]['itemcomposicaovalor'][$descricaoitem] = $item['descricaoitem'];
				}
				if($item['descricaoitem'] == '' ){
					$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
					$_SESSION['validaErro'][$erroSubacao]['descricaoitemcomposicao'] = ": Existem itens de composição sem identificação. (Verifique em  Editar / Inserir Itens de Composição   )";
				}
				
				if(($item['quantidade'] == '') ||($item['quantidade'] == 0)  ){
					$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
					$_SESSION['validaErro'][$erroSubacao]['itemcomposicao'][$descricaoitem] = $item['descricaoitem'];	
				}
				
				$item['quantidade'] = $item['quantidade'];//str_replace(".", "", $item['quantidade']);
				$item['valorunitario'] = number_format($item['valorunitario'], 2, '.', ''); //str_replace(".", ",", $item['valorunitario']);   
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['itens'][$codigoItem]['descricaoitem'] 			= $item['descricaoitem'];
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['itens'][$codigoItem]['codigounidademedida'] 	= $item['codigounidademedida'];
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['itens'][$codigoItem]['descricaounidademedida'] 	= $item['descricaounidademedida'];
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['itens'][$codigoItem]['quantidade'] 				= $item['quantidade']; 
				$projetos['projetos'][$pi]['acoes'][$codigodimensao]['especificacoes'][$codigosubacao]['itens'][$codigoItem]['valorunitario'] 			= $item['valorunitario'];  
				$cont++;
				}
			}else{
				$_SESSION['validaErro'][$erroSubacao]['nome'] = $dados['descricaosubacao'];
				$_SESSION['validaErro'][$erroSubacao]['itemcomposicao'] = "Subação não possui <i>itens de composição</i>.";
			}
		}
	
	/*******************************
	* 	abrir_conexao();
	*	Abre a conexão via SOCKET 
	*******************************/
	function abrirConexao(&$sk,&$erro) {
  		$sk=fsockopen(HOST,PORT,$errnum,$errstr,TIMEOUT) ;
  		if (!is_resource($sk)) {$erroConexao = "Erro de Conexao:".$errnum." ".$errstr;}
	}
	
	/*******************************
	* 	lerStatus();
	*	Lê o estado do envio do Documento.
	*******************************/
	function lerStatus($sk,&$codigo,&$mensagem) {
  		$lido = "";
  		$lido.= fgets ($sk, 1024);
  		$codigo = (int)pegarValor($lido,'codigo');
  		$mensagem = pegarValor($lido,'msg');
	}
	
	/*******************************
	* 	enviar_xml();
	*	escreve o conteúdo da string
	* 	para ser enviada. 
	*******************************/
	function enviarXML($sk,$xml) {
  		fputs($sk, $xml);
	}
	
	/*******************************
	* 	pegar_valor();
	*	Pega o valor que contem em cada tag do XML.
	* 	Retorna os Valores
	*******************************/
	function pegarValor($xml,$tag) {
	  $inicio = (strpos  ( $xml, "<$tag>" ) + strlen($tag) + 2) ;
	  $tamanho = strpos  ( $xml, "</$tag>" ) - $inicio;
	  return  substr($xml, $inicio , $tamanho);
	}
	
	/*******************************
	* 	CLASSE GERADORA DO XML
	*		Nome: assoc_array2xml
	* 		Variaveis publicas: 
	* 			- $text, $arrays, $keys, $node_flag, $depth, $xml_parser
	* 
	*******************************/
	class assocArray2xml {
		var $text;
		var $arrays, $keys, $node_flag, $depth, $xml_parser;
		
		/*******************************
		* 	array2xml();
		* 
		*******************************/
		function array2xml($array) {
			$this->arrayTransform($array);
			return $this->text;
		}
		
		/*******************************
		* 	array_transform();
		*	Gera o XML
		* 	retorna o XML
		* 
		*******************************/
		function arrayTransform($array,$ident = 1,$nome_filho_recebido = false){
		
			foreach($array as $key => $value){
				if(!is_array($value)){
					if($key != 'NOME_FILHO') {
					  $this->text .= "<$key>".xmlentities($value)."</$key>";
					}
				} else {
					$nome_filho_a_passar = false;
					if(isset($value['NOME_FILHO'])) {
					  $nome_filho_a_passar = $value['NOME_FILHO'];
					}
					if($nome_filho_recebido) {
					  $key = $nome_filho_recebido;
					}
					$this->text.= "<$key>";
					$this->arrayTransform($value,$ident+1,$nome_filho_a_passar);
					$this->text.= "</$key>";
				}
			}
			$ident--;
			return $this->text;
		}
		
	}

	/*******************************
	* 	xmlentities();
	*	Insere o conteudo das Tags XML 
	*******************************/
	function xmlentities( $strText, $strQuoteStyle = ENT_QUOTES, $strCharSet = 'ISO' )
	{
		if ( $strCharSet == 'UTF8' )
		{
			$strText = utf8_decode( $strText );
		}
		static $trans;
		if ( !isset( $trans ) )
		{
			$trans = get_html_translation_table( HTML_ENTITIES, $strQuoteStyle );
			foreach ( $trans as $key => $value )
			{
				$trans[ $key ] = '&#' . ord( $key ) . ';';
			}
			$trans[ chr(38) ] = '&';
		}
		return preg_replace( "/&(?![A-Za-z]{0,4}\w{2,3};|#[0-9]{2,5};)/" , "&#38;" , strtr( $strText, $trans ) );
	}
	
	/*******************************
	* 
	* 	FUNCTION: leXML();
	*	17/06/2008
	* 	DESCRIÇÃO:
	* 	Faz a leitura do XML e retorna os 
	*   valores em um array.
	* 
	* 	@PARAM $sk - stream (XML)
	* 
	*******************************/
	function leXML($sk) {
		$XML = "";
		while (!feof($sk)) { // Monta o XML
	  		$XML.= fgets ($sk, 1024);
		}
		$Arquivo = $XML;
		$Analisador = xml_parser_create();//Cria um analisador Parser
		xml_parser_set_option($Analisador, XML_OPTION_CASE_FOLDING, 0);
		xml_parser_set_option($Analisador, XML_OPTION_SKIP_WHITE, 1);
		xml_parse_into_struct($Analisador, $Arquivo, $Valor, $Tags);
		xml_parser_free($Analisador);
		foreach ($Tags as $Chaves=>$Valores) {
			if ($Chaves == "msg") { // verifica apartir de qual TAG deseja ser analisada.
			    $Valores = $Valores;
			    for ($i=0; $i < count($Valores); $i+=2) {
			    	$ValoresTags = $Valores[$i] + 1;
			        $Nos = $Valores[$i + 1] - $ValoresTags;
			        $ArrayComCampos[] = Campos_e_tags(array_slice($Valor, $ValoresTags, $Nos));
			    }
			}else{
				continue;
			}
		}
		return $ArrayComCampos;
	}
	
/*******************************
	* 
	* 	FUNCTION: Campos_e_tags();
	*	17/06/2008
	* 	DESCRIÇÃO:
	* 	Faz a leitura dos campos do XML e coloca em Array.
	* 
	* 	@PARAM $MatrizValores - Array
	* 
	*******************************/
	function Campos_e_tags($MatrizValores) {	
		$contprocesso 	= 0;
		$contpi 	 	= 0;
		$contprojetos 	= 0;
		
		for ($i=0; $i < count($MatrizValores); $i++){
			if($MatrizValores[$i]["tag"] == "codigoibge"){
				$DadosFNDE["codigoibge"]  = $MatrizValores[$i]["value"]; // Adiciona no Array código do IBGE
			}
			if($MatrizValores[$i]["tag"] == "uf"){
				$DadosFNDE["uf"] =$MatrizValores[$i]["value"]; // Adiciona no Array o UF
			}
			if(($MatrizValores[$i]["tag"] == "projetos") && ($MatrizValores[$i]["type"] != "close")){
				$projetos = "projetos"; //Cria tag projetos no array
			}
			if(($MatrizValores[$i]["tag"] == "projeto") && ($MatrizValores[$i]["type"] != "close")){
				$contprojetos++; //cria subarray projetos (conta quantidade de projetos existente no XML e coloca no Array)
			}
			if(($MatrizValores[$i]["tag"] == "numeroprocesso") && ($MatrizValores[$i]["type"] != "close") ){
				$DadosFNDE[$projetos][$contprojetos]["processo"][$contprocesso] =$MatrizValores[$i]["value"];
				$contprocesso++; // Array de numero de processos
			}
			if(($MatrizValores[$i]["tag"] == "pi") && ($MatrizValores[$i]["type"] != "close")){
				$DadosFNDE[$projetos][$contprojetos]["pi"][$contpi] =$MatrizValores[$i]["value"];
				$contpi++; // Array de numero de PIs
			}
		}
			return($DadosFNDE);
	}
	
	/*******************************
	* 
	*	FUNCTION: verificaPendenciasConvenio();
	* 	28/10/2008
	* 	DESCRIÇÃO:
	* 	Se o FNDE retornou erro dizendo que não se pode conveniar, 
	*   a função verifica quais subações estão com pedencia de enpenho.
	* 
	* 	@PARAM $retornoFNDE - Array
	* 
	*******************************/
	function verificaPendenciasConvenio(){
		global $db;
		$sql = "SELECT  s.sbaid, 
							s.sbadsc, 
							b.cvrnumprocesso    
						FROM cte.dimensao d
						INNER JOIN cte.areadimensao ad ON ad.dimid = d.dimid
						INNER JOIN cte.indicador i ON i.ardid  = ad.ardid
						INNER JOIN cte.criterio c ON c.indid  = i.indid
						INNER JOIN cte.pontuacao p ON p.crtid  = c.crtid and p.indid = i.indid and p.ptostatus = 'A'
						INNER JOIN cte.instrumentounidade iu ON iu.inuid = p.inuid
						INNER JOIN cte.acaoindicador a ON a.ptoid   = p.ptoid
						INNER JOIN cte.subacaoindicador s ON s.aciid   = a.aciid
						LEFT JOIN cte.subacaoparecertecnico spt ON spt.sbaid = s.sbaid and sptano = date_part('year', current_date)
						left join 
							( select distinct sc.sbaid, cvrnumprocesso  
							from cte.instrumentounidade iu 
							left JOIN cte.convenio c ON c.inuid = iu.inuid
							left join  cte.subacaoconvenio sc ON sc.cnvid = c.cnvid
							left JOIN cte.convenioretorno cr ON cr.inuid = iu.inuid 
							) as b ON b.sbaid = s.sbaid
						WHERE iu.inuid =".$_SESSION["inuid"]." AND spt.ssuid = 3 AND ".$GLOBALS['condicaoQuery'];
    		$conteudo = $db->carregar( $sql );
    		$_SESSION['erro_empenho'] = $conteudo;
	}
	
	/*******************************
	* 
	*	FUNCTION: gravaRetornoFNDE();
	* 	17/06/2008
	* 	DESCRIÇÃO:
	* 	Grava o retorno dos PIS e os números dos processos 
	*   (criados pelo FNDE) que foram cadastrados na base do FNDE. 	
	* 
	* 	@PARAM $retornoFNDE - Array
	* 
	*******************************/
	function gravaRetornoFNDE($retornoFNDE, $convenio){
		global $db;
		$codigoMunicipio = $retornoFNDE[0]["codigoibge"];
		$uf 			 = $retornoFNDE[0]["uf"];
		if($codigoMunicipio != NULL){
			$sql = "select inuid from cte.instrumentounidade where muncod =".$codigoMunicipio;
		}else{
			$sql = "select inuid from cte.instrumentounidade where  estuf = '".$uf."' and muncod is null and itrid = 1";
		}
		$sql="select inuid from cte.convenio where cnvid= ".$convenio;
		$instrumentoUnidade = $db->pegaLinha( $sql );
		$instrumentoUnidade = $instrumentoUnidade['inuid'];
		//$sqlDel = "delete from cte.convenioretorno where inuid =".$instrumentoUnidade." and cnvid = ".$convenio;
		//$db->executar( $sqlDel );
		$contprocesso = 0;
		$contpi = 0;
		for ($i=1; $i <= count($retornoFNDE[0]["projetos"]); $i++) {
			$numeroProcesso	= $retornoFNDE[0]["projetos"][$i]["processo"][$contprocesso];
			$sql="select cvrnumprocesso from cte.convenioretorno where cvrnumprocesso= '".$numeroProcesso."'";
			$existeProcesso = $db->pegaUm( $sql );
			if($numeroProcesso != $existeProcesso){
				for ($a=0; $a < count($retornoFNDE[0]["projetos"][$i]["pi"]); $a++) {
					$planoInterno	= $retornoFNDE[0]["projetos"][$i]["pi"][$contpi];
					$contpi++;
		
					$sql = "INSERT INTO cte.convenioretorno (	cnvid,
														cvrnumprocesso, 
													 	cvrdata, 
													 	inuid, 
													 	prgplanointerno)
						VALUES (".$convenio.",
								".$numeroProcesso.",
								now(),
								".$instrumentoUnidade.",
								'".$planoInterno."'
						   	   )";
					
					if($db->executar( $sql )){
						$db->commit();
					}else{
						$db->rollback();
					}
				}
			}
			$contprocesso++;
		}
		
	}
	
	function updateStatusSubacao($sbaid){
		global $db;
		$sbaid 	= array_unique( $sbaid );
		foreach ( $sbaid as $idSubacao ){
			$sql = "update cte.subacaoparecertecnico s set ssuid = 7 where s.sbaid = ".$idSubacao." and s.sptano = date_part('year', current_date)";
			$db->executar($sql);
		}
		
		return true;
		
	}
	
?>