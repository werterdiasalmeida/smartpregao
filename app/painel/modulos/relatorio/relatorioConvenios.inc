<?php
if ($_REQUEST['tbs'] == 't' && $_REQUEST['codconv'])
{
	header("Content-Type: text/html; charset=ISO-8859-1");
	print "Convênio: " . $_REQUEST['codconv']; 
	$sql = "select substring(dfidatasaldo::varchar,6,2) || '/' || substring(dfidatasaldo::varchar,0,5), SUM(dfisaldoconta) as dfisaldoconta, (SUM(dfisaldofundo) + SUM(dfisaldopoupanca) + SUM(dfisaldordbcdb)) AS totalaplicacao, (SUM(dfisaldoconta) + SUM(dfisaldofundo) + SUM(dfisaldopoupanca) + SUM(dfisaldordbcdb)) AS totalconta
				from painel.dadosfinanceirosconvenios dfi
					inner join painel.dadosconvenios dco on dco.dcoprocesso = dfi.dfiprocesso
					where dco.dcoconvenio = '" . $_REQUEST['codconv'] . "'
					group by dfidatasaldo
					order by dfidatasaldo";
	$cabecalho = array('Mês Ref.','Saldo Conta','Saldo Aplicações','Total Conta');
	$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
	exit();
}

if ($_REQUEST['tbs'] == 'r' && $_REQUEST['codconv'])
{
	header("Content-Type: text/html; charset=ISO-8859-1");
	print "Convênio: " . $_REQUEST['codconv']; 
	$sql = "select to_char(drcdatapagamento, 'DD/MM/YYYY') as datapagamento, drcordembancaria, drcvalorpago, 'Banco: ' || drcbanco || '<br>Ag: ' || drcagencia || '<br>CC: ' || drcconta as drcdadosbancarios 
				from painel.dadosrepassesconvenios drc
					inner join painel.dadosconvenios dco on dco.dcoprocesso = drc.drcprocesso
					where dco.dcoconvenio = '" . $_REQUEST['codconv'] . "'
					order by drcdatapagamento";
	$cabecalho = array('Data do Repasse','OB','Valor Repassado','Dados Bancários');
	$db->monta_lista_simples($sql,$cabecalho,200,20,'','','');
	exit();
}
if ( $_POST['pesquisa'] || $_GET['pesquisa'] ){
	ini_set("memory_limit","250M");
	set_time_limit(0);
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if($_GET['boPainel']){
		$_POST['agrupador'] 		= array(0 => 'dcoconvenio');
		$_POST['muncod'] 			= array(0 => $_GET['muncod']);
		$_POST['muncod_campo_flag'] = true;
		$_POST['estuf'] 			= array(0 => $_GET['estuf']);
		$_POST['estuf_campo_flag'] = true;
		$_POST['desid'] 			= array(0 => $_GET['desid']);
		
		$sql = "select distinct dfimesanosaldo, dfidatasaldo from painel.dadosfinanceirosconvenios order by dfidatasaldo desc limit 1";
		$_POST['dfimesanosaldo'] = $db->pegaUm($sql);
		
	}
	
	$sql   = monta_sql();
	$dados = $db->carregar($sql);
	$agrup = monta_agp();
	$col   = monta_coluna();
	
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados);
	$r->setColuna($col);
	$r->setBrasao($true ? true : false);
	$r->setOverFlowTableHeight(500);
	$r->setTotNivel(true);
	$r->setEspandir(true);
	?>
	<html>
		<head>
			<script type="text/javascript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
			<script type="text/javascript" src="../includes/remedial.js"></script>
			<script type="text/javascript" src="../includes/superTitle.js"></script>
		</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
	<?php 
	echo $r->getRelatorio();	
	?>
	</body>
	</html>
	<?php 
	die;	
}

function monta_sql(){
	
	$where = array();
	
	extract($_POST);
	
	// Programa
	if( $dprcodprograma ){
		array_push($where, " dc.dcoprocesso in (select dprprocesso from painel.dadosprograma where dprcodprograma = '".$dprcodprograma."')");
	}
	// Estado
	if( $estuf[0] && $estuf_campo_flag ){
		array_push($where, " m.estuf in ('" . implode( "','", $estuf ) . "') ");
	}
	// Município
	if( $muncod[0] && $muncod_campo_flag ){
		array_push($where, " dc.muncod in ('" . implode( "','", $muncod ) . "') ");
	}
	// Processo
	if( $processo ){
		array_push($where, " dcoprocesso =  '".$processo."'");
	}
	// Convênio
	if( $convenio ){
		array_push($where, " dcoconvenio like '%".$convenio."%'");
	}
	// Ano Convênio
	if( $anoconvenioini && $anoconveniofim ){
		for($i=$anoconvenioini;$i<=$anoconveniofim;$i++) $anof[] = $i;
		array_push($where, " dcoano IN('".implode("','",$anof)."')");
	}
	//Esfera
	if($_GET['boPainel']){
		if( $desid[0] ){
			array_push($where, " dc.desid in (" . implode( "','", $desid ) . ") ");
		}else{
			array_push($where, " dc.desid = 0 ");
		}
	}else{
		if( $desid <> '' ){
			array_push($where, " dc.desid = ".$desid."");
		}
	}
	
	
	$sql = "SELECT DISTINCT
					dcorazaosocial, 
					dcoprocesso || ' ' AS dcoprocesso, 
					dcoconvenio || ' ' AS dcoconvenio,
					dcoano, 
					dc.muncod, 
					e.estdescricao || ' - ' || e.estuf as estdescricao,
					m.mundescricao || ' / ' || e.estuf AS mundescricao, 
					dcovalorconveniado,
			        dcovalorempenhado, 
			        dcovalorpago,
			        dcovalorconcedente,
					dcovalorcontrapartida,
			        (sum(dfisaldofundo) + sum(dfisaldopoupanca) + sum(dfisaldordbcdb)) AS totalaplicacao,
					(sum(dfisaldoconta) + sum(dfisaldofundo) + sum(dfisaldopoupanca) + sum(dfisaldordbcdb)) AS totalconta,
					to_char(dcodatapagamento, 'DD/MM/YYYY') as dcodatapagamento, 
					--'Banco: ' || dfibanco || '<br>Ag: ' || dfiagencia || '<br>CC: ' || dficonta as dfibanco,
					painel.contasconvenio(dcoprocesso, '$dfimesanosaldo') as dfibanco,
					sum(dfisaldoconta) AS dfisaldoconta,
					sum(dfisaldofundo) AS dfisaldofundo, 
					sum(dfisaldopoupanca) AS dfisaldopoupanca, 
					sum(dfisaldordbcdb) AS dfisaldordbcdb, 
			        dfimesanosaldo,
			        painel.programasconvenio(dcoprocesso) as dcoprograma,
			        to_char(dcodatainicio, 'DD/MM/YYYY') as dcodatainicio,
			        to_char(dcodatafim, 'DD/MM/YYYY') as dcodatafim,
			        --case dcovalorprojeto when 0 then dcovalorconveniado else dcovalorprojeto end as dcovalorprojeto,
			        desesfera
			FROM painel.dadosconvenios dc
				LEFT JOIN painel.dadosfinanceirosconvenios df on dc.dcoprocesso = df.dfiprocesso AND dfimesanosaldo =  '$dfimesanosaldo'	
				INNER JOIN territorios.municipio m on dc.muncod = m.muncod
				INNER JOIN territorios.estado e on m.estuf = e.estuf
				INNER JOIN painel.dadosesfera des on dc.desid = des.desid
			--WHERE to_char(dcodatafim, 'YYYY-MM-DD') >= '".date("Y-m-d")."'
			WHERE 1=1
				" . ( is_array($where) && count($where) ? ' AND' . implode(' AND ', $where) : '' ) ."
			GROUP BY dcorazaosocial, dcoprocesso, dcoconvenio, dcoano, dc.muncod, e.estdescricao, e.estuf, m.mundescricao, dcovalorconveniado, dcovalorempenhado, dcovalorpago, dcovalorconcedente,
			dcovalorcontrapartida, dcodatapagamento, --dfibanco, dfiagencia, dficonta, 
			dfimesanosaldo, dcodatainicio, dcodatafim, desesfera
			ORDER BY dcoano, " . ( is_array($agrupador) && count($agrupador) ? implode(', ', $agrupador) : '' ) ."
			--limit 50";
	//ver($sql);
	return $sql;
}

function monta_agp(){

	$agrupador = (array) $_POST['agrupador'];
	
	$agp = array(
				 "agrupador" 	  => array(),
				 "agrupadoColuna" => array(
											"dcoprocesso",
											"dcorazaosocial",
											"mundescricao",
											"dcoprograma",
											"dcoconvenio",
											"dcoano",
											"dcovalorconveniado",
											"dcovalorempenhado",
											"dcovalorpago",
											"dcovalorconcedente",
  											"dcovalorcontrapartida",
											"totalaplicacao",
											"totalconta",
											"dcodatapagamento",
											"dfibanco",
											"dfisaldoconta",
									        "dfimesanosaldo",
											"dcodatainicio",
											"dcodatafim",
											//"dcovalorprojeto",
											"desesfera"
										  ) 	
				);
	
	foreach( $agrupador as $val ):
		switch ($val) {
		    case 'dcorazaosocial':
				array_push($agp['agrupador'], array(
													"campo" => "dcorazaosocial",
											  		"label" => "Entidade"
												   )										
						  );				
		    continue;
		    break;
		    case 'estdescricao':
				array_push($agp['agrupador'], array(
													"campo" => "estdescricao",
											  		"label" => "Estado"
												   )										
						  );				
		    continue;
		    break;
		    case 'mundescricao':
				array_push($agp['agrupador'], array(
													"campo" => "mundescricao",
											  		"label" => "Município"
												   )										
						  );				
		    continue;
		    break;
		    case 'dcoprograma':
				array_push($agp['agrupador'], array(
													"campo" => "dcoprograma",
											  		"label" => "Programa"
												   )										
						  );				
		    continue;
		    break;
		    case 'dcoconvenio':
				array_push($agp['agrupador'], array(
													"campo" => "dcoconvenio",
											  		"label" => "Convênio"
												   )										
						  );				
		    continue;
		    break;		
		    case 'dfibanco':
				array_push($agp['agrupador'], array(
													"campo" => "dfibanco",
											  		"label" => "Dados Bancários"
												   )										
						  );				
		    continue;
		    break;		    		    
		}       
	endforeach;
	
	return $agp;
}

function monta_coluna(){

	$coluna = array(
				array(
					  "campo" => "dcoprocesso",
			   		  "label" => "Processo",
			   		  "type"  => "string"
				),
				array(
					  "campo" => "dcorazaosocial",
			   		  "label" => "Entidade",
			   		  "type"  => "string",
			   		  "html"  => "<div align=left>{dcorazaosocial}</div>"
				),
				array(
					  "campo" => "mundescricao",
			   		  "label" => "Município",
			   		  "type"  => "string",
			   		  "html"  => "<div align=left>{mundescricao}</div>"
				),
				array(
					  "campo" => "dcoprograma",
			   		  "label" => "Programa",
			   		  "type"  => "string",
			   		  "html"  => "<div align=left>{dcoprograma}</div>"
				),
				//array(
				//	  "campo" => "dcovalorprojeto",
			   	//	  "label" => "Valor Projeto<br>R$"
				//),
				array(
					  "campo" => "dcovalorconveniado",
			   		  "label" => "Valor Conveniado<br>R$"
				),
				array(
					  "campo" => "dcovalorcontrapartida",
			   		  "label" => "Valor Contrapartida<br>R$"
				),
				array(
					  "campo" => "dcovalorconcedente",
			   		  "label" => "Valor Concedente<br>R$",
					  "html"  => "<b>{dcovalorconcedente}</b>"
				),
				array(
					  "campo" => "dcovalorempenhado",
			   		  "label" => "Valor Empenhado<br>R$"
				),
				array(
					  "campo" => "dcovalorpago",
			   		  "label" => "Valor Repassado<br>R$",
					  "html"  => "<b>{dcovalorpago}</b>"
				),
				array(
					  "campo" => "dcodatapagamento",
			   		  "label" => "Data do Último Repasse",
			   		  "html" => "<span	onclick=\"SuperTitleAjax( 'painel.php?modulo=relatorio/relatorioConvenios&acao=R&tbs=r&codconv={dcoconvenio}', this );\" onmouseout=\"SuperTitleOff( this );\">{dcodatapagamento}</span>"
				),
				array(
					  "campo" => "desesfera",
			   		  "label" => "Esfera"
				),
				array(
					  "campo" => "dcodatainicio",
			   		  "label" => "Data Início de Vigência"
				),
				array(
					  "campo" => "dcodatafim",
			   		  "label" => "Data Fim de Vigência"
				),
				array(
					  "campo" => "dfisaldoconta",
			   		  "label" => "Saldo da Conta Corrente<br>(A)"
				),
				array(
					  "campo" => "totalaplicacao",
			   		  "label" => "Saldo de Aplicações<br>(B)",
				),
				array(
					  "campo" => "totalconta",
			   		  "label" => "Total Conta<br>(A+B)",
			   		  "html"  => "<b>{totalconta}</b>"
				),
				array(
					  "campo" => "dfimesanosaldo",
			   		  "label" => "Data do Saldo",
					  "html" => "<span	onclick=\"SuperTitleAjax( 'painel.php?modulo=relatorio/relatorioConvenios&acao=R&tbs=t&codconv={dcoconvenio}', this );\" onmouseout=\"SuperTitleOff( this );\">{dfimesanosaldo}</span>"
				)				
			  );
return $coluna;				  
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$titulo_modulo = "Relatório de Convênios";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
	function onOffCampo( campo ){
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		
		if ( div_on.style.display == 'none' ){
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}else{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
	function geraPopRelatorio(){
		
		var form = $('formulario');
		var agrupador  = document.getElementById( 'agrupador' );

		if ( !agrupador.options.length ){
			alert( 'Favor selecionar ao menos um item para agrupar o resultado.' );
			return false;
		}
		
		if(document.getElementById('anoconvenioini').value != '' || document.getElementById('anoconveniofim').value != '') {
			if(document.getElementById('anoconvenioini').value == '') {
				alert('Ano convênio inicial deve ser preenchido');
				return false;
			}
			if(document.getElementById('anoconveniofim').value == '') {
				alert('Ano convênio final deve ser preenchido');
				return false;
			}

		}

		if($('estuf_campo_flag').value == "1"){
			selectAllOptions( form.estuf );
		}
		if($('muncod_campo_flag').value == "1"){
			selectAllOptions( form.muncod );
		}
		
		selectAllOptions( form.agrupador );
		form.target = 'resultadoRelatorioPainel';

		var janela = window.open( '', 'resultadoRelatorioPainel', 'fullscreen=1,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		
		janela.focus();
		form.submit();
	}
	
</script>

<form name="formulario" id="formulario" action="" method="post">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<input type="hidden" name="pesquisa" value="1"/>
	<tr>
		<td class="SubTituloDireita">Agrupadores</td>
		<td>
			<?php
				// Início dos agrupadores
				$agrupador = new Agrupador('formulario','');
				
				// Dados padrão de origem
				$origem = array(
					array(
						'codigo'    => 'dcoprograma',
						'descricao' => 'Programa'
					),
					array(
						'codigo'    => 'dcorazaosocial',
						'descricao' => 'Entidade'
					),
					array(
						'codigo'    => 'estdescricao',
						'descricao' => 'Estado'
					),
					array(
						'codigo'    => 'mundescricao',
						'descricao' => 'Município'
					),
					array(
						'codigo'    => 'dcoconvenio',
						'descricao' => 'Convênio'
					),
					array(
						'codigo'    => 'dfibanco',
						'descricao' => 'Dados Bancários'
					),
				);
				
				// exibe agrupador
				$agrupador->setOrigem( 'naoAgrupador', null, $origem );
				$agrupador->setDestino( 'agrupador', null);
				$agrupador->exibir();
				
			?>
		</td>
	</tr>
	<?php
		$sql = "select distinct dprcodprograma as codigo, 
					dprprograma as descricao 
				from painel.dadosprograma
				order by descricao";
	?>
	<tr>
    	<td align='right' class="SubTituloDireita">Programa:</td>
    	<td><?=$db->monta_combo('dcoprograma',$sql,'S','Selecione o Programa',"",'','','','N','','',$dprcodprograma);?></td>
	</tr>
	<?php
		$sql = "select desid as codigo, 
					desesfera as descricao 
				from painel.dadosesfera
				order by descricao";
	?>
	<tr>
    	<td align='right' class="SubTituloDireita">Esfera:</td>
    	<td><?=$db->monta_combo('desid',$sql,'S','Selecione a Esfera',"",'','','','N','','',$desid);?></td>
	</tr>
	<?php 
		$sql = "select estuf as codigo, 
					   estdescricao as descricao
				from territorios.estado
					   order by	estdescricao";
		mostrarComboPopup( 'Estado', 'estuf',  $sql, $stSqlCarregados, 'Selecione o(s) Estados(s)' );

		$sql = "select muncod as codigo, mundescricao as descricao from territorios.municipio order by descricao";		
		mostrarComboPopup( 'Município', 'muncod',  $sql, $stSqlCarregados, 'Selecione o(s) Município(s)' );

		$sql = "select distinct dfimesanosaldo as codigo, 
					dfimesanosaldo as descricao,
					dfidatasaldo 
				from painel.dadosfinanceirosconvenios
				order by dfidatasaldo desc";
	?>
	<tr>
    	<td align='right' class="SubTituloDireita">Data do Saldo:</td>
    	<td><?=$db->monta_combo('dfimesanosaldo',$sql,'S','',"",'','','','N','','',$dfimesanosaldo);?></td>
	</tr> 
	<tr>
		<td class="SubTituloDireita" valign="top">Convênio</td>
		<td><?php echo campo_texto( 'convenio', 'N', $habilitado, 'Convênio', 60, 50, '', '','','','','id="convenio"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Ano convênio</td>
		<td>de <?php echo campo_texto( 'anoconvenioini', 'N', $habilitado, 'Ano convênio', 5, 4, '', '','','','','id="anoconvenioini"'); ?> até <?php echo campo_texto( 'anoconveniofim', 'N', $habilitado, 'Ano convênio', 5, 4, '', '','','','','id="anoconveniofim"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Processo</td>
		<td><?php echo campo_texto( 'processo', 'N', $habilitado, 'Processo', 60, 50, '', '','','','','id="processo"'); ?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Pesquisar" onclick="geraPopRelatorio();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>
