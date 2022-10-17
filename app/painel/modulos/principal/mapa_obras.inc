<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
<?php

//Funções
include_once (APPRAIZ.'www/painel/_funcoes_painel_controle.php');
include_once (APPRAIZ.'www/obras/_funcoes.php');

function gerarCor() {
	$p = sprintf("%02s",dechex(mt_rand(0,255)));
	$s = sprintf("%02s",dechex(mt_rand(0,255)));
	$t = sprintf("%02s",dechex(mt_rand(0,255)));
	return $p.$s.$t;
}

if($_REQUEST['verfiltrounidades']) {
	//////////// Unidades //////////////
		if($_REQUEST['orgid']) {
			$filtro = "WHERE obi.orgid IN('".implode("','", $_REQUEST['orgid'])."')";
		}
		
		$sql = " SELECT	DISTINCT entid AS codigo, entnome AS descricao, obi.orgid FROM obras.obrainfraestrutura obi 
				 INNER JOIN entidade.entidade ent ON obi.entidunidade = ent.entid 
				 ".$filtro." 
				 ORDER BY obi.orgid, entnome";
		combo_popup( 'entid', $sql, 'Selecione as Unidades', '400x400', 0, array(), '', 'S', false, false, 5, 155, '', '' );
	exit;

}

if($_REQUEST['verresumo']) {
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if(!$_REQUEST['agrupador'][0])
	$_REQUEST['agrupador'][] = "situacao";
	
	if(!$_REQUEST['orgid'][0]) {
		$_REQUEST['orgid'] = array();
		if($_REQUEST['possuifiltro']) {
			$_REQUEST['orgid'] = array("1","2","3","4");
		}
	}
	
	$sql       = obras_monta_sql_relatio();
	$agrupador = obras_monta_agp_relatorio();
	$coluna    = obras_monta_coluna_relatorio();
	
	// Transformando o label em imagens LABEL
	$imagens = array("superior" => "<img src='../imagens/icone_capacete_1.png' border='0'>","tecnico" => "<img src='../imagens/icone_capacete_2.png' border='0'>","basico" => "<img src='../imagens/icone_capacete_3.png' border='0'>","administrativa" => "<img src='../imagens/icone_capacete_4.png' border='0'>","total" => "Total");
	if($coluna) {
		foreach($coluna as $key => $col) {
			$coluna[$key]['label'] = $imagens[$col['campo']];
		}
	}
	if($_POST['orgid'][0]) {	
	$dados = $db->carregar( $sql );
	echo '<table width="100%" border="0" cellpadding="0" cellspacing="0">
			<tr><td class="SubTituloCentro"><input type="radio" name="agrupadorp" value="situacao" onclick="document.getElementById(\'agrupadorresumo\').value=this.value;carregar_resumo();" '.(($_POST['agrupador'][0]=='situacao'||$_POST['agrupador'][0]=='')?"checked":"").'> Situação</td><td class="SubTituloCentro"><input type="radio" name="agrupadorp" value="classificacao" onclick="document.getElementById(\'agrupadorresumo\').value=this.value;carregar_resumo();" '.(($_POST['agrupador'][0]=='classificacao')?"checked":"").'> Classificação</td><td class="SubTituloCentro"><input type="radio" name="agrupadorp" value="uf" onclick="document.getElementById(\'agrupadorresumo\').value=this.value;carregar_resumo();" '.(($_POST['agrupador'][0]=='uf')?"checked":"").'> UF</td></tr>
		  </table>';
	
	}
	
	$rel = new montaRelatorio();	
	$rel->setTolizadorLinha(false);
	$rel->setTotalizador(false);	
	$rel->setAgrupador($agrupador, $dados); 
	$rel->setColuna($coluna);
	$rel->setTotNivel(true);
	echo $rel->getRelatorio();
	exit;
}


if($_REQUEST['verlistaobras']) {
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if(!$_REQUEST['agrupador'][0])
	$_REQUEST['agrupador'] = array("unidade","nomedaobra2");
	
	if(!$_REQUEST['orgid'][0]) {
		$_REQUEST['orgid'] = array();
		if($_REQUEST['possuifiltro']) {
			$_REQUEST['orgid'] = array("1","2","3","4");
		}
	}
		
	$sql       = obras_monta_sql_relatio();
	$agrupador = obras_monta_agp_relatorio();
	if($_POST['orgid'][0]) {
	$dados = $db->carregar( $sql );
	}

	$rel = new montaRelatorio();
	$rel->setTolizadorLinha(false);
	$rel->setTotalizador(false);	
	$rel->setAgrupador($agrupador, $dados); 
	$rel->setTotNivel(true);
	echo $rel->getRelatorio();
	exit;
}

if($_REQUEST['vergrafico']) {
	
	ini_set("memory_limit", "1024M");
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if(!$_REQUEST['agrupador'][0])
	$_REQUEST['agrupador'][] = "situacao";
	
	if(!$_REQUEST['orgid'][0]) {
		$_REQUEST['orgid'] = array();
		if($_REQUEST['possuifiltro']) {
			$_REQUEST['orgid'] = array("1","2","3","4");
		}
	}
	
	$sql       = obras_monta_sql_relatio();
	$agrupador = obras_monta_agp_relatorio();
	$coluna    = obras_monta_coluna_relatorio();
	array_pop($coluna);
	
	if($_POST['orgid'][0]) {	
		$dados = $db->carregar( $sql );
		echo '<table width="100%" border="0" cellpadding="0" cellspacing="0">
				<tr><td class="SubTituloCentro"><input type="radio" name="agrupadorp2" value="situacao" onclick="document.getElementById(\'agrupadorgrafico\').value=this.value;carregar_grafico();" '.(($_POST['agrupador'][0]=='situacao')?"checked":"").'> Situação</td><td class="SubTituloCentro"><input type="radio" name="agrupadorp2" value="classificacao" onclick="document.getElementById(\'agrupadorgrafico\').value=this.value;carregar_grafico();" '.(($_POST['agrupador'][0]=='classificacao')?"checked":"").'> Classificação</td><td class="SubTituloCentro"><input type="radio" name="agrupadorp2" value="uf" onclick="document.getElementById(\'agrupadorgrafico\').value=this.value;carregar_grafico();" '.(($_POST['agrupador'][0]=='uf')?"checked":"").'> UF</td></tr>
				<tr><td id=graf colspan=\"3\"></td></tr>
			  </table>';
	
	}
	
	$rel = new montaRelatorio();	
	$rel->setTolizadorLinha(false);
	$rel->setTotalizador(false);	
	$rel->setAgrupador($agrupador, $dados);
	$dadosagrupados = $rel->getAgrupar();
	
	if(is_array($dadosagrupados)) {
	
		if(!is_dir(APPRAIZ."www/graficos")) {
			mkdir(APPRAIZ."www/graficos", 0777);
		}
		if(!is_dir(APPRAIZ."www/graficos/obras")) {
			mkdir(APPRAIZ."www/graficos/obras", 0777);
			mkdir(APPRAIZ."www/graficos/obras/xml", 0777);
		}
		if(!is_dir(APPRAIZ."www/graficos/obras/xml")) {
			mkdir(APPRAIZ."www/graficos/obras/xml", 0777);
		}
		$caminho_fisico = APPRAIZ."www/graficos/obras/xml/";
		$caminho_logico = "../graficos/obras/xml/";
		
		$arquivo_xml = "visualizamapa_".date("Ymdhis").".xml";
		
		$conteudo_xml =  "<graph xAxisName=\"Tipo de Ensino\" yAxisName=\"".$agrupador['agrupador'][0]['label']."\" caption=\"\" subCaption=\"\" decimalPrecision=\"0\" rotateNames=\"1\" numDivLines=\"3\" numberPrefix=\"\" showValues=\"0\" formatNumberScale=\"0\">";
		if($coluna) {
			$conteudo_xml_categorias .= "<categories>";
			foreach($coluna as $col) {
				$conteudo_xml_categorias .= "<category name=\"".$col['label']."\" />";
				foreach($dadosagrupados as $agr => $subdados) {
					$conteudo_xml_datasets[$agr][] .= $subdados[$col['campo']];
				}
			}
			$conteudo_xml_categorias .= "</categories>";
			$cor=0;
			foreach($conteudo_xml_datasets as $lin => $valores) {
				$conteudo_xml_dataset .= "<dataset seriesName=\"".$lin."\" color=\"".gerarCor()."\" showValues=\"0\">";
				foreach($valores as $valor) {
					$conteudo_xml_dataset .= "<set value=\"".$valor."\" />";
				}
				$conteudo_xml_dataset .= "</dataset>";			
			}
		}
		$conteudo_xml .= $conteudo_xml_categorias.$conteudo_xml_dataset."</graph>";
		$xml = fopen($caminho_fisico.$arquivo_xml, 'w');
		fwrite($xml, $conteudo_xml);
		fclose($xml);
	
		$script = "<script type=\"text/javascript\">
			   			var chart = new FusionCharts(\"../includes/FusionCharts/FusionChartsFree/Charts/FCF_StackedColumn2D.swf\", \"ChartId\", \"300\", \"300\");
			   			chart.setDataURL(\"".$caminho_logico.$arquivo_xml."\");		   
			   			chart.render(\"graf\");
				   </script>";
	   
	   echo $script;
   
   } else {
   	   echo $dadosagrupados;
   }
   exit;
}



	$_POST["tipo_ensino"]=Array();
	
	include APPRAIZ . 'includes/cabecalho.inc';
	echo "<br>";
	
	if( $_SESSION["sisid"] == 48 ){
	
		if(!$cabecalho_painel){
		       exibeAbaUsuario($_SESSION['usucpf'],$_SERVER['REQUEST_URI']);
		       monta_titulo( 'Mapa de Obras', '');
		}
	}else{
		
		$titulo_modulo = "Mapa de Obras";
		monta_titulo( $titulo_modulo, '' );
		
	}
	
	function curPageURL() {
		 $pageURL = 'http';
		 if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
		 $pageURL .= "://";
		 if ($_SERVER["SERVER_PORT"] != "80") {
		  $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
		 } else {
		  $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
		 }
		 return $pageURL;
		}
		$local= explode("/",curPageURL());
	    ?>
		<?if ($local[2]=="simec.mec.gov.br" ){ ?>
	    	<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxQhVwj8ALbvbyVgNcB-R-H_S2MIRxTIdhrqjcwTK3xxl_Nu_YMC5SdLWg" type="text/javascript"></script>
	  	<? } ?>
	  	<?if ($local[2]=="simec-d.mec.gov.br"){ ?>
	  		<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRYtD8tuHxswJ_J7IRZlgTxP-EUtxT_Cz5IMSBe6d3M1dq-XAJNIvMcpg" type="text/javascript"></script> 
		<? } ?>
		<?if ($local[2]=="simec" ){ ?>
			 <script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTNzTBk8zukZFuO3BxF29LAEN1D1xSIcGWxF7HCjMwks0HURg6MTfdk1A" type="text/javascript"></script>
	    <? } ?>
	  	<?if ($local[2]=="simec-d"){ ?>
	  		 <script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTFm3qU4CVFuo3gZaqihEzC-0jfaRTY9Fe8UfzYeoYDxtThvI3nGbbZEw" type="text/javascript"></script> 
		<? } ?>
		<?if ($local[2]=="simec-local"){ ?>
			<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxRzjpIxsx3o6RYEdxEmCzeJMTc4zBSMifny_dJtMKLfrwCcYh5B01Pq_g" type="text/javascript"></script> 	
	    <? } ?>
	    <?if ($local[2]=="painel.mec.gov.br"){ ?>
			<script src="http://maps.google.com/maps?file=api&amp;v=2&amp;key=ABQIAAAAwN0kvNsueYw8CBs704pusxTPkFYZwQy2nvpGvFj08HQmPOt9ZBT2EJmQsTms0WQqU_5GvEj7bMZd7g" type="text/javascript"></script> 	
	    <? } ?>
	    	
	    	
	    	
    <script type="text/javascript">
    	document.write('<script type="text/javascript" src="../includes/extlargemapcontrol'+(document.location.search.indexOf('packed')>-1?'_packed':'')+'.js"><'+'/script>');
  	</script>
  	<script type="text/javascript" src="../includes/prototype.js"></script>
	<script type="text/javascript" src="../includes/FusionCharts/FusionCharts.js"></script>
    <script type="text/javascript">
    var map;
    function ajaxatualizar(params,iddestinatario) {
    	var myAjax = new Ajax.Request(
    		window.location.href,
    		{
    			method: 'post',
    			parameters: params,
    			asynchronous: false,
    			onComplete: function(resp) {
    				if(iddestinatario) {
    					extrairScript(resp.responseText);
    					document.getElementById(iddestinatario).innerHTML = resp.responseText;
    				} 
    			},
    			onLoading: function(){
    				document.getElementById(iddestinatario).innerHTML = 'Carregando...';
    			}
    		});
    }
    function carregar_grafico() {
        // varrendo Tipo Ensino
        var org = document.getElementsByName('orgid[]');
        var orgparam='';
		for(i=0;i<org.length;i++) {
			if(org[i].checked) {
				orgparam = orgparam+'orgid[]='+org[i].value+'&';	
			}
		}
        // varrendo Situação
        var sit = document.getElementsByName('stoid[]');
        var sitparam='';
        var sitflag=false;
		for(i=0;i<sit.length;i++) {
			if(sit[i].checked) {
				sitflag = true;
				sitparam = sitparam+'stoid[]='+sit[i].value+'&';	
			}
		}
		if(sitflag) {
			sitparam = sitparam+'stoid_campo_flag=1&';
		}
        // varrendo Situação

        // varrendo Classificação
        var clo = document.getElementsByName('cloid[]');
        var cloparam='';
        var cloflag=false;
		for(i=0;i<clo.length;i++) {
			if(clo[i].checked) {
				cloflag = true;
				cloparam = cloparam+'cloid[]='+clo[i].value+'&';	
			}
		}
		if(cloflag) {
			cloparam = cloparam+'cloid_campo_flag=1&';
		}
        // varrendo Classificação

        // varrendo UF
        var uf = document.getElementById('estuf');
        var ufparam='';
        if(uf.options[0].value) {
			for(i=0;i<uf.options.length;i++) {
				ufparam = ufparam+'uf[]='+uf.options[i].value+'&';				
			}
			ufparam = ufparam+'uf_campo_flag=1&';
        }
        // varrendo UF

        // varrendo Entidade
        var ent = document.getElementById('entid');
        var entparam='';
        if(ent.options[0].value) {
			for(i=0;i<ent.options.length;i++) {
				entparam = entparam+'unidade[]='+ent.options[i].value+'&';				
			}
			entparam = entparam+'unidade_campo_flag=1&';
        }
        // varrendo Entidade
        

		var agp='';
		if(document.getElementById('agrupadorgrafico').value) {
			agp='agrupador[]='+document.getElementById('agrupadorgrafico').value+'&';
		}
        
		var possuifiltro = '';
		if(document.getElementById('clickcarregar').value=='1') {
			possuifiltro='possuifiltro=true&';
		}
		
		ajaxatualizar(entparam+orgparam+sitparam+cloparam+ufparam+possuifiltro+agp+'vergrafico=true','grafico');
        var conteudo=document.getElementById("grafico");
    
        var scripts = conteudo.getElementsByTagName("script");
        for(i = 0; i < scripts.length; i++)
        {
            s = scripts[i].innerHTML;
            eval(s);
        }
    }

    function verificarCarregar() {
    	
        var org = document.getElementsByName('orgid[]');
        
        var problema=true;
		for(i=0;i<org.length;i++) {
			if(org[i].checked) {
				problema=false;
			}
		}
		if(problema) {
			alert('Selecione o tipo de ensino');
			return false;
		} else {
	    	document.getElementById('clickcarregar').value='1';
	    	document.getElementById('carregar').disabled=true;
	    	document.getElementById('carregando').innerHTML="Carregando...";
	    	document.getElementById('listagem').innerHTML="Carregando...";
		    document.getElementById('resumo').innerHTML="Carregando...";
		    //document.getElementById('grafico').innerHTML="Carregando...";
	    	carregar();
	    	carregar_resumo();
	    	carregar_listaobras();
	    	//carregar_grafico();
	    	document.getElementById('carregando').innerHTML="";
	    	document.getElementById('carregar').disabled=false;
		}
    }
    
    function carregar_listaobras() {
        // varrendo Tipo Ensino
        var org = document.getElementsByName('orgid[]');
        var orgparam='';
		for(i=0;i<org.length;i++) {
			if(org[i].checked) {
				orgparam = orgparam+'orgid[]='+org[i].value+'&';	
			}
		}
        // varrendo Situação
        var sit = document.getElementsByName('stoid[]');
        var sitparam='';
        var sitflag=false;
		for(i=0;i<sit.length;i++) {
			if(sit[i].checked) {
				sitflag = true;
				sitparam = sitparam+'stoid[]='+sit[i].value+'&';	
			}
		}
		if(sitflag) {
			sitparam = sitparam+'stoid_campo_flag=1&';
		}
        // varrendo Situação

        // varrendo Classificação
        var clo = document.getElementsByName('cloid[]');
        var cloparam='';
        var cloflag=false;
		for(i=0;i<clo.length;i++) {
			if(clo[i].checked) {
				cloflag = true;
				cloparam = cloparam+'cloid[]='+clo[i].value+'&';	
			}
		}
		if(cloflag) {
			cloparam = cloparam+'cloid_campo_flag=1&';
		}
        // varrendo Classificação

        // varrendo UF
        var uf = document.getElementById('estuf');
        var ufparam='';
        if(uf.options[0].value) {
			for(i=0;i<uf.options.length;i++) {
				ufparam = ufparam+'uf[]='+uf.options[i].value+'&';				
			}
			ufparam = ufparam+'uf_campo_flag=1&';
        }
        // varrendo UF

        // varrendo Entidade
        var ent = document.getElementById('entid');
        var entparam='';
        if(ent.options[0].value) {
			for(i=0;i<ent.options.length;i++) {
				entparam = entparam+'unidade[]='+ent.options[i].value+'&';				
			}
			entparam = entparam+'unidade_campo_flag=1&';
        }
        // varrendo Entidade

        
		var possuifiltro = '';
		if(document.getElementById('clickcarregar').value=='1') {
			possuifiltro='possuifiltro=true&';
		}

		ajaxatualizar(entparam+orgparam+sitparam+cloparam+ufparam+possuifiltro+'verlistaobras=true','listagem');
    }
    
    function carregar_resumo() {
        // varrendo Tipo Ensino
        var org = document.getElementsByName('orgid[]');
        var orgparam='';
		for(i=0;i<org.length;i++) {
			if(org[i].checked) {
				orgparam = orgparam+'orgid[]='+org[i].value+'&';	
			}
		}
        // varrendo Situação
        var sit = document.getElementsByName('stoid[]');
        var sitparam='';
        var sitflag=false;
		for(i=0;i<sit.length;i++) {
			if(sit[i].checked) {
				sitflag = true;
				sitparam = sitparam+'stoid[]='+sit[i].value+'&';	
			}
		}
		if(sitflag) {
			sitparam = sitparam+'stoid_campo_flag=1&';
		}
        // varrendo Situação

        // varrendo Classificação
        var clo = document.getElementsByName('cloid[]');
        var cloparam='';
        var cloflag=false;
		for(i=0;i<clo.length;i++) {
			if(clo[i].checked) {
				cloflag = true;
				cloparam = cloparam+'cloid[]='+clo[i].value+'&';	
			}
		}
		if(cloflag) {
			cloparam = cloparam+'cloid_campo_flag=1&';
		}
        // varrendo Classificação

        // varrendo UF
        var uf = document.getElementById('estuf');
        var ufparam='';
        if(uf.options[0].value) {
			for(i=0;i<uf.options.length;i++) {
				ufparam = ufparam+'uf[]='+uf.options[i].value+'&';				
			}
			ufparam = ufparam+'uf_campo_flag=1&';
        }
        // varrendo UF
        
        // varrendo Entidade
        var ent = document.getElementById('entid');
        var entparam='';
        if(ent.options[0].value) {
			for(i=0;i<ent.options.length;i++) {
				entparam = entparam+'unidade[]='+ent.options[i].value+'&';				
			}
			entparam = entparam+'unidade_campo_flag=1&';
        }
        // varrendo Entidade

        
		var agp='';
		if(document.getElementById('agrupadorresumo').value) {
			agp='agrupador[]='+document.getElementById('agrupadorresumo').value+'&';
		}
		var possuifiltro = '';
		if(document.getElementById('clickcarregar').value=='1') {
			possuifiltro='possuifiltro=true&';
		}
		ajaxatualizar(entparam+orgparam+sitparam+cloparam+ufparam+agp+possuifiltro+'verresumo=true','resumo');
    }
    
    function carregar_filtrounidades() {
    	document.getElementById('unidades').innerHTML = "Carregando...";
        // 
        var org = document.getElementsByName('orgid[]');
        var orgparam='';
		for(i=0;i<org.length;i++) {
			if(org[i].checked) {
				orgparam = orgparam+'orgid[]='+org[i].value+'&';	
			}
		}
		ajaxatualizar(orgparam+'verfiltrounidades=true','unidades');
    }
    
	var marcadores = new Array();
	var markerGroups = new Array();
	function initialize() {
      if (GBrowserIsCompatible()) {
       var opts = {
		 googleBarOptions : {
		   style : 'new'
		 }
		}
		map = new GMap2(document.getElementById("mapa"), opts);
		// Controles      	
		map.addControl(new GLargeMapControl3D());
        map.addControl(new GOverviewMapControl());
        map.enableScrollWheelZoom();
        map.addMapType(G_PHYSICAL_MAP);

       	// Preenchendo o valor inicial do mapa
		var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;		
		map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));

		// Mostrar Lat e Lng MouseMove
		GEvent.addListener(map,"mousemove",function(latlng) {
	       	document.getElementById('lat').value=dec2grau(latlng.lat());
	        document.getElementById('lng').value=dec2grau(latlng.lng());
        });

		} 
	}
	
	// Criando o ícones do mapa
	var customIcons = [];

	var marca = new GIcon();

	
	marca.iconSize = new GSize(9, 14);
	marca.iconAnchor = new GPoint(9, 15);
	marca.infoWindowAnchor = new GPoint(9, 2);
	marca.image = '../imagens/icone_capacete_1.png';
	customIcons[1] = marca;

	var marca = new GIcon();
	marca.iconSize = new GSize(9, 14);
	marca.iconAnchor = new GPoint(9, 15);
	marca.infoWindowAnchor = new GPoint(9, 2);
	marca.image = '../imagens/icone_capacete_2.png';
	customIcons[2] = marca;

	var marca = new GIcon();
	marca.iconSize = new GSize(9, 14);
	marca.iconAnchor = new GPoint(9, 15);
	marca.infoWindowAnchor = new GPoint(9, 2);
	marca.image = '../imagens/icone_capacete_3.png';
	customIcons[3] = marca;

	var marca = new GIcon();
	marca.iconSize = new GSize(9, 14);
	marca.iconAnchor = new GPoint(9, 15);
	marca.infoWindowAnchor = new GPoint(9, 2);
	marca.image = '../imagens/icone_capacete_4.png';
	customIcons[4] = marca;

	var marca = new GIcon();
	marca.iconSize = new GSize(9, 14);
	marca.iconAnchor = new GPoint(9, 15);
	marca.infoWindowAnchor = new GPoint(9, 2);
	marca.image = '../imagens/icone_capacete_101.png';
	customIcons[101] = marca;

	var marca = new GIcon();
	marca.iconSize = new GSize(9, 14);
	marca.iconAnchor = new GPoint(9, 15);
	marca.infoWindowAnchor = new GPoint(9, 2);
	marca.image = '../imagens/icone_capacete_102.png';
	customIcons[102] = marca;

	var marca = new GIcon();
	marca.iconSize = new GSize(9, 14);
	marca.iconAnchor = new GPoint(9, 15);
	marca.infoWindowAnchor = new GPoint(9, 2);
	marca.image = '../imagens/icone_capacete_103.png';
	customIcons[103] = marca;

	var marca = new GIcon();
	marca.iconSize = new GSize(9, 14);
	marca.iconAnchor = new GPoint(9, 15);
	marca.infoWindowAnchor = new GPoint(9, 2);
	marca.image = '../imagens/icone_capacete_104.png';
	customIcons[104] = marca;

	markerGroups = {1:[], 2:[], 3:[], 4:[], 101:[], 102:[], 103:[], 104:[]};
 
	function carregar(texto_busca){
		// Limpar todos os marcadores
		map.clearOverlays();

		// Verifica se é busca ou filtro
		if (texto_busca !='' && texto_busca !=undefined){
			xml_filtro="mapa_gera_xml.php?texto_busca="+texto_busca;
		} else {
			// Lendo os Filtros
			var orgid= "0";
			for(i = 0 ; i < document.getElementsByName("orgid[]").length ; i++){
				if(document.getElementsByName("orgid[]")[i].checked)
					orgid += ","+document.getElementsByName("orgid[]")[i].value;
			}
			var stoid= "0";
			for(i = 0 ; i < document.getElementsByName("stoid[]").length ; i++){
				if(document.getElementsByName("stoid[]")[i].checked)
					stoid += ","+document.getElementsByName("stoid[]")[i].value;
			}
			var cloid= "0";
			for(i = 0 ; i < document.getElementsByName("cloid[]").length ; i++){
				if(document.getElementsByName("cloid[]")[i].checked)
					cloid += ","+document.getElementsByName("cloid[]")[i].value;
			}
	        // varrendo Entidade
	        var ent = document.getElementById('entid');
	        var entparam='';
	        if(ent.options[0].value) {
				for(i=0;i<ent.options.length;i++) {
					entparam = entparam+'entid[]='+ent.options[i].value+'&';				
				}
	        }
	        // varrendo Entidade
			
			selectAllOptions(document.getElementById('estuf'));
			estuf="''";
			var selObj = document.getElementById('estuf');
			var i;
			for (i=0; i<selObj.options.length; i++) {
				if (selObj.options[i].selected) {
			     	estuf+=",'"+selObj.options[i].value+"'";
			  	}
			}
			if (estuf!="'',''")
				xml_filtro="mapa_gera_xml.php?"+entparam+"orgid="+orgid+"&stoid="+stoid+"&cloid="+cloid+"&estuf="+estuf;
			else
				xml_filtro="mapa_gera_xml.php?"+entparam+"orgid="+orgid+"&stoid="+stoid+"&cloid="+cloid;
		
			
		}
		
		// window.open(xml_filtro);
		
		// Criando os Marcadores com o resultado
		GDownloadUrl(xml_filtro, function(data) {
			var xml = GXml.parse(data);
			var markers = xml.documentElement.getElementsByTagName("marker");
			
			if(markers.length > 1) {
			var lat_ant=0;
			var lng_ant=0;
			for (var i = 0; i < markers.length; i++) {
				var obrdesc = markers[i].getAttribute("obrdesc");
				var obrid = markers[i].getAttribute("idobra");
				type = parseInt(markers[i].getAttribute("orgid"));
	
				// Verifica pontos em um mesmo lugar e move o seguinte para a direita
				if(lat_ant==markers[i].getAttribute("lat") && lng_ant==markers[i].getAttribute("lng"))
					var point = new GLatLng(markers[i].getAttribute("lat"),	markers[i].getAttribute("lng"));
				else
					var point = new GLatLng(markers[i].getAttribute("lat"),	parseFloat(markers[i].getAttribute("lng"))+0.0005);				
	
				lat_ant=markers[i].getAttribute("lat");
				lng_ant=markers[i].getAttribute("lng");
				
				// Cria o marcador na tela
				var marker=createMarker(point,obrdesc,type,obrid);
				markerGroups[type].push(marker);
				marcadores[obrid]=marker;
				map.addOverlay(marker);
	          }
			} else {
				alert('Não existem obras');
			}
	        });
     }

	
	function abrebalao(marcador){
		marcadores[marcador].openInfoWindowHtml('<iframe src=/obras/obras.php?modulo=principal/mapa_balao&acao=A&obrid='+marcador+' width=350 frameborder=0 ></iframe>');
	}
	function createMarker(point,obrdesc,type,obrid) {
		var icone = customIcons[type];
		var marker = new GMarker(point,{title: obrdesc, icon: icone, draggable:false });
		GEvent.addListener(marker, 'click', function() {
			html='<iframe src=/obras/obras.php?modulo=principal/mapa_balao&acao=A&requisicao=supervisao&obrid='+obrid+' width=350 frameborder=0 ></iframe>';
	        marker.openInfoWindowHtml(html);
	    	});
	 return marker;
	}
	function toggleGroup(type) {
	      for (var i = 0; i < markerGroups[type].length; i++) {
	        var marker = markerGroups[type][i];
	        if (marker.isHidden()) {
	          marker.show();
	        } else {
	          marker.hide();
	        }
	      } 
	      for (var i = 0; i < markerGroups[type+100].length; i++) {
		        var marker = markerGroups[type+100][i];
		        if (marker.isHidden()) {
		          marker.show();
		        } else {
		          marker.hide();
		        }
		  }
	    }
	
 	// Converter em Graus
    function dec2grau(valor){
		ddLat=valor+"";
		if (ddLat.substr(0,1) == "-") {
			ddLatVal = ddLat.substr(1,ddLat.length-1);
		} else {
			ddLatVal = ddLat;
		}
		// Graus 
		ddLatVals = ddLatVal.split(".");
		dmsLatDeg = ddLatVals[0];
		// * 60 = mins
		ddLatRemainder  = ("0." + ddLatVals[1]) * 60;
		dmsLatMinVals   = ddLatRemainder.toString().split(".");
		dmsLatMin = dmsLatMinVals[0];
		// * 60 novamente = secs
		ddLatMinRemainder = ("0." + dmsLatMinVals[1]) * 60;
		dmsLatSec  = Math.round(ddLatMinRemainder);
		
		if (ddLat.substr(0,1) == "-") {
			valor="-"+dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";
		} else {
			valor=dmsLatDeg+unescape('%B0')+" "+dmsLatMin+"' "+dmsLatSec+"''";
		}

     	return valor;
     }

	function buscar(){
		var valor=document.getElementById('texto_busca').value;
		carregar(valor);
	}
	
	// Esconder e mostrar os paineis
	function mostrar_painel(painel){
		switch(painel) {
			case 'barra_direita':
			status=document.getElementById('lateral_d').style.display;
			if (status=="none"){
				document.getElementById('lateral_d').style.display="block";
				document.getElementById('lateral_e').style.display="none";
				document.getElementById('mapa').style.width="600px";
				map.checkResize();
			}
			else {
				document.getElementById('lateral_d').style.display="none";
				document.getElementById('lateral_e').style.display="block";
				document.getElementById('mapa').style.width="100%";
				map.checkResize();
			}
			break;
			case 'situacao':
			case 'classificacao':
			case 'uf':
			case 'resumo':
			case 'unidades':
			case 'listagem':
			case 'grafico':
				if(document.getElementById(painel).style.display == "none") {
					document.getElementById("img_"+painel).src="../imagens/menos.gif";
				} else {
					document.getElementById("img_"+painel).src="../imagens/mais.gif";
				}
			break;
		}
			
		
		status=document.getElementById(painel).style.display;
		if (status=="none") {
			document.getElementById(painel).style.display="block";
		} else {
			document.getElementById(painel).style.display="none";
		}
	}
	function visao(tipo){
		if (tipo=='mapa')
			map.setMapType(G_NORMAL_MAP);
		if (tipo=='satelite')
			map.setMapType(G_SATELLITE_MAP);
		if (tipo=='hibrido')
			map.setMapType(G_HYBRID_MAP);
		if (tipo=='terreno')
			map.setMapType(G_PHYSICAL_MAP);		
	}
function mapa_original (){
	// Preenchendo o valor inicial do mapa
	var zoom = 4;	var lat_i = -14.689881; var lng_i = -52.373047;		
	map.setCenter(new GLatLng(lat_i,lng_i), parseInt(zoom));
}
    </script>
    <form name="formulario" id="pesquisar" method="POST" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
		<tr>
			<td valign="top" bgcolor=#ffffff width="170px">
				<table cellSpacing="0" cellPadding="3" align="left" class="tabela" width="260px">
				<tr > <td class="SubTituloEsquerda">Busca Textual</td></tr>
				<tr > <td><input type="text" id=texto_busca name=texto_busca size=16> 
				<input type=button value="Ok" onClick="javascript: buscar();"></td></tr>
				<tr > <td class="SubTituloEsquerda">Tipo Ensino</td></tr>
				<tr valign="middle">
				<td>
	    		<?php
					if( ($db->testa_superuser()) || ( possuiPerfil( PERFIL_CONSULTAGERAL) || 
									   				  possuiPerfil( PERFIL_GESTORMEC ) ) ){
						$orgaos = $db->carregar("SELECT 
													orgid, orgdesc 
												 FROM 
													obras.orgao");
						$count = count($orgaos);
						for($i = 0; $i < $count; $i++){
							echo '<input onClick="javascript: toggleGroup(' . $orgaos[$i]['orgid'] . ');carregar_filtrounidades();" type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['orgid'] . '"/>' . '<img  src=/imagens/icone_capacete_'.$orgaos[$i]['orgid'].'.png> '  . $orgaos[$i]["orgdesc"] . '<br>';
						}
					}else{
						$orgaos = obras_pegarOrgaoPermitido();
						echo '<input type="checkbox" id="orgid" name="orgid[]" value="'.$orgaos[0]["id"].'" readonly="readonly"/>' . $orgaos[0]["descricao"] . '&nbsp;';
					}
				?></td>
				</tr>
				<tr> <td class="SubTituloEsquerda"><img style="cursor: pointer" src="../imagens/mais.gif" id="img_situacao" onclick="javascript: mostrar_painel('situacao');" border=0> Situação</td></tr>
				<tr> <td >
				
				<div id=situacao style="display: none">
				<?php  
				$sql="SELECT stoid as codigo, stodesc as descricao FROM obras.situacaoobra WHERE stostatus='A' ORDER BY stoordem";
				$db->monta_checkbox("stoid[]", $sql, $stoid, "<br>"); ?>
				</div>
				
				</td></tr>
				<tr> <td class="SubTituloEsquerda"><img style="cursor: pointer" src="../imagens/mais.gif" id="img_classificacao" onclick="javascript: mostrar_painel('classificacao');" border=0> Classificação</td></tr>
				<tr> <td >
				<div id=classificacao style="display: none">
				<?php  
				$sql="SELECT cloid as codigo, clodsc as descricao FROM obras.classificacaoobra ORDER BY clodsc";
				$db->monta_checkbox("cloid[]", $sql, $cloid, "<br>"); ?>
				</div>
				</td></tr>
				<tr> <td class="SubTituloEsquerda"><img style="cursor: pointer" src="../imagens/mais.gif" id="img_uf" onclick="javascript: mostrar_painel('uf');" border=0> UF</td></tr>
				<tr> <td >
				<div id=uf style="display: none">
				<?php
				//////////// UF //////////////
				$sql = " SELECT	estuf AS codigo,
										estdescricao AS descricao
									FROM 
										territorios.estado
									ORDER BY
										estdescricao ";
	
				combo_popup( 'estuf', $sql, 'Selecione as Unidades Federativas', '400x400', 0, array(), '', 'S', false, false, 5, 155, '', '' );
				?>
				</div>
				</td></tr>
				<tr> <td class="SubTituloEsquerda"><img style="cursor: pointer" src="../imagens/mais.gif" id="img_unidades" onclick="javascript: mostrar_painel('unidades');" border=0> Unidades</td></tr>
				<tr> <td >
				<div id=unidades style="display: none">
				<?
				$sql = "SELECT	DISTINCT entid AS codigo, entnome AS descricao, obi.orgid FROM obras.obrainfraestrutura obi 
						 INNER JOIN entidade.entidade ent ON obi.entidunidade = ent.entid 
						 ORDER BY obi.orgid, entnome";
				combo_popup( 'entid', $sql, 'Selecione as Unidades', '500x500', 0, array(), '', 'S', false, false, 5, 155, '', '' );
				
				?>
				</div>
				</td></tr>

			<tr> <td class="SubTituloDireita">
			<input type="button" value="Carregar" id="carregar" onClick="javascript: verificarCarregar();">
			<input type="hidden" name="clickcarregar" id="clickcarregar" value="0">
			<br>
			<div id="carregando"></div>
			</td></tr>
			</table>
		</td>
		<td style="vertical-align: top;">
		    <table bgcolor=#c3c3c3 width=100%>
		    <tr><td align=left><input type="button" value="Mapa Brasil" onClick="javascript: mapa_original();"></td><td align=center></td><td align=center></td><td align="right"><input type="button" value="Mapa" onClick="javascript: visao('mapa');"><input type="button" value="Satélite" onClick="javascript: visao('satelite');"><input type="button" value="Híbrido" onClick="javascript: visao('hibrido');"><input type="button" value="Terreno" onClick="javascript: visao('terreno');"></td></tr>
		    </table>
		    <!-- Mapa -->
		    	<div id="mapa" style="width: 100%; height: 520px; position: relative"></div>
		    <!-- Mapa -->
		    
		    <table bgcolor=#c3c3c3 width=100%>
		    <tr>
		    <td>
		    Latitude: <input type=text id=lat name=lat value="0" STYLE="border:none ; background-color:#c3c3c3" size=8 /> 
		    Longitude: <input type=text id=lng name=lng value="0" STYLE="border:none; background-color:#c3c3c3" size=8 />
		    </td>
		    </tr>
		    </table>
		</td>
		<td valign="top">
		<a href="javascript: mostrar_painel('barra_direita');" title="Mais opções"><b><div id="lateral_e"><<</div><div id="lateral_d" style="display: none;">>></div></b></a>
		<!-- barra movel da direita -->
			<div id=barra_direita style="display: none;">
		<!-- barra movel da direita -->
		<table cellSpacing="0" cellPadding="3" align="center" width="300px">
		<tr>
		<td class="SubTituloEsquerda" width="300px"><img style="cursor: pointer" src="../imagens/mais.gif" id="img_listagem" onclick="javascript: mostrar_painel('listagem');"> Lista de Obras</td>
		</tr>
		<tr>
		<td>
		<div id=listagem style="display: none; height:300px; width: 300px; overflow: auto;">
		<script>carregar_listaobras();</script>
		</div>
		</td>
		</tr>
		<tr>
		<td class="SubTituloEsquerda" width="300px"><img style="cursor: pointer" src="../imagens/mais.gif" id="img_resumo" onclick="javascript: mostrar_painel('resumo');" > Resumo</td>
		</tr>
		<tr>
		<td>
		<input type="hidden" name="agrupadorresumo" id="agrupadorresumo">
		<div id=resumo style="display: none; height:250px; width: 300px; overflow: auto;">
		<script>carregar_resumo();</script>
		</div>
		</td>
		</tr>
		<!-- 
		<tr>
		<td class="SubTituloEsquerda" width="300px"><img style="cursor: pointer" src="../imagens/mais.gif" id="img_grafico"  onclick="javascript: mostrar_painel('grafico');"> Gráficos</td>
		</tr>
		<tr>
		<td>
		<input type="hidden" name="agrupadorgrafico" id="agrupadorgrafico">
		<div id=grafico  style="display: none; height:320px; width: 300px; overflow: auto;">
		<script>carregar_grafico();</script>
		</div>
		</td>
		</tr>
		 -->
		</table>
		</div>
		</td>
		</tr>
	</table>
    </form>
<script type="text/javascript">initialize(); </script>