<?php

//VERIFICA SE O USUARIO POSSUI PERMISS?O AO M?DULO DEMANDAS
$sql = "SELECT count(usucpf) as total FROM seguranca.usuario_sistema WHERE sisid=44 and suscod='A' and usucpf='".$_SESSION['usucpf']."'";
$total = $db->pegaUm($sql);

if($total==0){
	// vincula o usu?rio com o m?dulo
	$sql = sprintf("INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '%s', %d, %d )",
				   $_SESSION['usucpf'],44,235);
	$db->executar( $sql );
	
	// inclui o perfil
	$sql = sprintf("INSERT INTO seguranca.perfilusuario ( usucpf, pflcod ) VALUES ( '%s', %d )",
				   $_SESSION['usucpf'],235);
	$db->executar( $sql );

	$db->commit();
}
//FIM VERIFICA

/*
 * Fun??o que faz o insert da demanda 
 */
function salvaDemanda(){
	global $db;
	
	
	$horario = "C";
	$dmdqtde = 1;
	$atendimentoRemoto = 'f';
	$dmdsalaatendimento = 120;
	$unaid = 15;
	$laaid = 14;	
	$reproducao = "";
	
	$tipid = 2;
	
	switch ($_SESSION['sisid']) {
	    case 13: //M?dulo PAR - Plano de Metas
	        $sisid = 28;
	        break;
	    case 57: //M?dulo Emendas
	        $sisid = 293;
	        break;
	}	
	
	$sidid = $sisid; 

	/*	
	--------------------- CODIGOS DA TABELA seguranca.sistema ----------------------------
	1;"Sistema de Monitoramento e Avalia??o";"''";"PPA-Monitoramento e Avalia??o";"monitora"
	2;"Sistema de Programa??o Or?ament?ria";"''";"Programa??o Or?ament?ria";"elabrev"
	3;"Sistema de Informa??es Or?ament?rias e Financeiras";"http://";"SIOF";"siof"
	4;"Sistema de Seguran?a";"http://";"SIS";"seguranca"
	5;"Sistema de Elabora??o e Revis?o";"''";"PPA-Elabora??o e Revis?o";"elabrev"
	6;"Sistema de Projetos Especiais";"";"Projetos Especiais";"monitora"
	7;"Sistema  Or?ament?rio e Financeiro";"http://";"Or?ament?rio e Financeiro";"financeiro"
	10;"Sistema de Monitoramento do Plano de Desenvolvimento da Educa??o";"http://";"PDE";"pde"
	11;"Sistema de Gerenciamento de Projetos";"''";"Ger?ncia de Projetos";"pde"
	12;"Sistema REUNI - Reestrutura??o e Expans?o das Universidades"";"''";"REUNI";"reuni"
	13;"Sistema PAR - Plano de Metas Compromisso Todos pela Educa??o";"''";"PAR - Plano de Metas";"cte"
	14;"Sistema Brasil Profissionalizado";"''";"Brasil Profissionalizado";"brasilpro"
	15;"Sistema de Monitoramento de Obras";"''";"Monitoramento de Obras";"obras"
	17;"Sistema de Informa??es Gerenciais";"''";"SIG - Informa??es para Presid?ncia";"sig"
	21;"Sistema Administrativo (Compras / Eventos / Contratos)";"''";"Administrativo";"evento"
	23;"PAR - Plano de Metas 2010";"''";"PAR 2010";"par"
	27;"Sistema REHUF";"''";"REHUF";"rehuf"
	29;"Sistema de Consultoria Jur?dica";"''";"CONJUR";"conjur"
	32;"Sistema PAR Ind?gena";"http://";"PAR Ind?gena";"parindigena"
	34;"Sistema Escola (Pde Escola / Mais Educa??o / Escola Acess?vel / Escola Aberta)";"''";"Escola";"pdeescola"
	44;"Sistema de Demandas";"''";"Demandas";"demandas"
	48;"Sistema Painel de Controle";"http://";"Painel";"painel"
	56;"Sistema Rede Federal";"''";"Rede Federal";"academico"
	57;"Sistema de Emendas";"http://";"Emendas";"emenda"
	58;"Sistema de Gest?o de Tarefas";"http://";"Gest?o de Tarefas";"tarefa"
	62;"Sistema Assessoria Internacional";"''";"Assessoria Internacional";"assint"
	64;"Sistema de Gest?o de Pessoas";"http://";"Gest?o de Pessoas";"gestaopessoas"
	65;"Sistema do Programa Sa?de na Escola";"http://";"PSE";"pse"
	66;"Sistema de Institui??es de Educa??o Superior";"''";"IES";"ies"
	67;"Sistema Ensino M?dio Inovador";"''";"Ensino M?dio Inovador";"emi"

	--------------------- CODIGOS DA TABELA demandas.sistemadetalhe ----------------------------
	26;"SIMEC SIS";"SIMEC - M?dulo de Seguran?a"
	27;"SIMEC OR? FINAN";"SIMEC - M?dulo Or?ament?rio e Financeiro"
	28;"SIMEC PA PLANOM";"SIMEC - M?dulo PAR - Plano de Metas Compromisso Todos pela Educa??o"
	29;"PPA-ELABORA??O E REVIS?O";"SIMEC - M?dulo de Elabora??o e Revis?o do PPA"
	30;"REUNI";"REUNI - Reestrutura??o e Expans?o das Universidades"
	31;"GER?NCIA DE PROJETOS";"SIMEC - M?dulo de Gerenciamento de Projetos"
	32;"SIMEC PROJ ESP";"SIMEC - M?dulo de Projetos Especiais"
	33;"SIMEC CONJUR";"SIMEC - M?dulo de Consultoria Jur?dica"
	34;"SIMEC PDE";"SIMEC - M?dulo de Monitoramento do Plano de Desenvolvimento da Educa??o"
	35;"SIMEC PROGR OR?";"SIMEC - M?dulo de Programa??o Or?ament?ria"
	36;"SIMEC AI";"SIMEC - M?dulo Assessoria Internacional"
	37;"SIMEC DEMANDAS";"SIMEC - M?dulo de Demandas"
	38;"SIMEC PAR IND?G";"SIMEC - M?dulo PAR Ind?gena"
	39;"SIMEC SIG";"SIMEC - M?dulo de Informa??es Gerenciais"
	40;"BRASIL PROFISSIONALIZADO";"SIMEC - M?dulo Brasil Profissionalizado"
	41;"SIMEC REHUF";"SIMEC - M?dulo Reestrutura??o de Hospitais Universit?rios"
	42;"SIMEC OBRAS";"SIMEC - M?dulo de Monitoramento de Obras"
	43;"SIMEC PDEESCOLA";"SIMEC - M?dulo Escola - PDE-Escola"
	205;"PSE";"Programa Sa?de na Escola"
	207;"SIMEC";"Sistema de Monitoramento Estrat?gico"
	232;"SIMEC - PAINEL";"Painel de Controle"
	235;"SIMEC EVENTOS";"SIMEC - M?dulo Administrativo - Eventos"
	236;"Rede Federal";"Rede Federal"
	238;"SIMEC - CTE";"SIMEC - M?dulo Cadastro de Tecnologias Educacionais"
	239;"Plano de A??es ";"SIMEC - M?dulo Plano de A??es Articuladas"
	240;"SIMEC - LSE";"SIMEC - M?dulo Levantamento da Situa??o Escolar"
	243;"Biblioteca Proj";"SIMEC - M?dulo Biblioteca de Projetos"
	282;"SIMEC COMPRAS";"SIMEC - M?dulo Administrativo - Compras "
	283;"SIMEC-MAIS EDUC";"SIMEC - M?dulo Escola - Mais Educa??o"
	290;"SIMEC SIGOEI";"SIMEC - M?dulo de Gest?o da OEI"
	293;"SIMEC EME";"SIMEC - M?dulo Emendas"
	294;"SIMEC TAR";"SIMEC - M?dulo Gest?o de Tarefas"
	295;"RH";"M?dulo Mapeamento da For?a de Trabalho"
	296;"SIMEC RH GDPGPE";"SIMEC - M?dulo RH - Avalia??o da GDPGPE"
	297;"SIMEC RH CAMS";"SIMEC - M?dulo RH - Monitoramento de Atendimento na CAMS"
	313;"SIMEC - EMI";"SIMEC - M?dulo Ensino M?dio Inovador"
	315;"SIMEC RH FT";"SIMEC - M?dulo RH - For?a de Trabalho"
	350;"SIMEC IES";"SIMEC - M?dulo de IES"
	351;"SIMEC SIDOC-WEB";"SIMEC - M?dulo de Gest?o de Documentos - SIDOC-WEB"
	361;"SIMEC CONTRATOS";"SIMEC - M?dulo Administrativo - Contratos"
	362;"SIMEC ESCACESSI";"SIMEC - M?dulo Escola - Escola Acess?vel"
	363;"SIMEC ESCABERTA";"SIMEC - M?dulo Escola - Escola Aberta"
	365;"SIMEC SEED";"SIMEC - M?dulo de Gerenciamento de Processos da SEED"
	371;"ENEM";"Exame Nacional do Ensino M?dio"
	*/
	
	
	$sql = sprintf("INSERT INTO demandas.demanda
					(
						usucpfdemandante,
						usucpfinclusao,
						tipid,
						sidid,
						dmdtitulo,
						dmddsc,
						dmdreproducao,
						dmdstatus,
						laaid,
						dmdsalaatendimento,
						unaid,
						dmdqtde,
						dmdhorarioatendimento,
						dmdatendremoto,
						dmddatainclusao		
					)VALUES(
						'%s',
						'%s',
						%s,
						%s,
						'%s',
						'%s',
						'%s',
						'A',
						 %s,
						'%s',
						%d,
						%d,
						'%s',
						'%s',
						'%s'
					) RETURNING dmdid ",
					$_SESSION['usucpf'], 
					$_SESSION['usucpf'],
					$tipid,
					$sidid,
					$_POST['assunto'],
					$_POST['necessidade'],
					$reproducao,
					$laaid,
					$dmdsalaatendimento,
					$unaid,
					$dmdqtde,
					$horario,
					$atendimentoRemoto,
					date('Y-m-d H:i:s')
					);
					
	$dmdid = $db->pegaUm($sql);
	$db->commit();
	return $dmdid;
}


/*
 * Executa a fun??o que salva a demanda
 * Retorna mensagem de sucesso e redireciona a p?gina
 */
if($_POST['assunto'] && $_POST['necessidade']) {

 	 $dmdid 	= salvaDemanda();
 	 
 	 

 	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], '', $dmdid)) {
		
 	 	 //envia email
	 	 $remetente = array('nome'=>'M?dulo Demandas', 'email'=>'demandas@mec.gov.br');
	 	 $destinatario = "marilucia.queiroz@mec.gov.br";
	 	 $emailCopia = "cristiano.cabral@mec.gov.br";
		 $assunto = "Demanda N? {$dmdid} Cadastrada pelo m?dulo ".$_SESSION['sisabrev'];
		 $conteudo = "Demanda N? {$dmdid} Cadastrada pelo m?dulo ".$_SESSION['sisabrev'].".<br>Acesse o sistema demandas para acompanh?-la.";
		 enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
 	 	
 		?>
 		<script>
		 		alert("Sua Demanda de n? <?=$dmdid?> foi enviada com sucesso. \nAguarde que iremos atend?-lo o mais breve poss?vel!");
				location.href="<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/cadDemandaGeral&acao=A";
		</script>
		<?
		 	  
	 	die;
	 	
	 } else {
	 	die("<script>
	 			alert(\"Problemas no envio do arquivo.\");
	 			history.go(-1);
	 		</script>");	
	 }
}


######### Fun??es de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$dmdid){
	global $db;

	if (!$arquivo || !$dmdid)
		return false;
		
	// obt?m o arquivo
	#$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		redirecionar( 'demandas', $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	
	$sisid = 44; // m?dulo demandas
	//Insere o registro do arquivo na tabela public.arquivo
	$sql = "INSERT INTO public.arquivo 	
			(
				arqnome,
				arqextensao,
				arqdescricao,
				arqtipo,
				arqtamanho,
				arqdata,
				arqhora,
				usucpf,
				sisid
			)VALUES(
				'".current(explode(".", $arquivo["name"]))."',
				'".end(explode(".", $arquivo["name"]))."',
				'".$dados["arqdescricao"]."',
				'".$arquivo["type"]."',
				'".$arquivo["size"]."',
				'".date('Y/m/d')."',
				'".date('H:i:s')."',
				'".$_SESSION["usucpf"]."',
				". $sisid ."
			) RETURNING arqid;";
	$arqid = $db->pegaUm($sql);
	
	//Insere o registro na tabela demandas.anexos
	$sql = "INSERT INTO demandas.anexos 
			(
				dmdid,
				arqid,
				anxdtinclusao,
				anxstatus
			)VALUES(
			    ". $dmdid .",
				". $arqid .",
				now(),
				'A'
			);";
	$db->executar($sql);
	
	if(!is_dir('../../arquivos/demandas/'.floor($arqid/1000))) {
		mkdir(APPRAIZ.'/arquivos/demandas/'.floor($arqid/1000), 0777);
	}
	$caminho = APPRAIZ . 'arquivos/demandas/'. floor($arqid/1000) .'/'. $arqid;
	switch($arquivo["type"]) {
		case 'image/jpeg':
			ini_set("memory_limit", "128M");
			list($width, $height) = getimagesize($arquivo['tmp_name']);
			$original_x = $width;
			$original_y = $height;
			// se a largura for maior que altura
			if($original_x > $original_y) {
  	 			$porcentagem = (100 * 640) / $original_x;      
			}else {
   				$porcentagem = (100 * 480) / $original_y;  
			}
			$tamanho_x = $original_x * ($porcentagem / 100);
			$tamanho_y = $original_y * ($porcentagem / 100);
			$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
			$image   = imagecreatefromjpeg($arquivo['tmp_name']);
			imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
			imagejpeg($image_p, $caminho, 100);
			//Clean-up memory
			ImageDestroy($image_p);
			//Clean-up memory
			ImageDestroy($image);
			break;
		default:
			if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
				$db->rollback();
				return false;
			}
	}
	
	
	$db->commit();
	return true;

}

######## Fim fun??es de UPLOAD ########


	
include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Cadastro de Demanda', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigat?rio.' );
?>
<body>
<form id="formDemanda" action="" method="post" enctype="multipart/form-data" onsubmit="return validaForm();">

<table id="tblFormDemanda" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' class="SubTituloDireita" style="width:39.5%;">Informe o <b>assunto</b> para a demanda:</td>
		<td><?= campo_texto( 'assunto', 'S', 'S', '', 80, 250, '', ''); ?></td>
	</tr>	
	<tr>
		<td align='right' class="SubTituloDireita"><b>Descreva</b> sua necessidade:</td>
		<td><?= campo_textarea('necessidade', 'S', 'S', '', 80, 5, 4000); ?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Anexe um arquivo:<br><b>(Opcional)</b></td>
		<td>
		 <input name="anexo" type="file" style="text-align: left; width: 83ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" onmouseover="MouseOver(this);" class="normal" size="81">
		</td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td width="15%">&nbsp;</td>
		<td>
		<input type="submit" class="botao" name="btalterar" value="Cadastrar">
		</td>
	</tr>
	<tr>
		<td>
			<br>
			<b>Demandas em Aberto</b>
			<br>
			<br>
		</td>
	</tr>	
</table>
</form>


<?php
	$sql = "SELECT
			d.dmdid AS codigo,
			d.dmdtitulo,
			CASE
			  WHEN d.docid IS NOT NULL THEN esddsc 
			  ELSE '<span style=\'color:blue;\' title=\'N?o Atribu?do\'>Em Processamento</span>'
			END AS situacao,				
			to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI:SS') AS dmddatainclusao,
			to_char(d.dmddatafimprevatendimento, 'DD/MM/YYYY HH24:MI:SS') AS dmddataconclusao,
			CASE
			  WHEN u.usucpf IS NOT NULL THEN usunome
			  ELSE '<span style=\'color:red;\' title=\'N?o Atribu?do\'>N?o Informado</span>'
			END AS responsavel
		FROM 
			demandas.demanda AS d
			LEFT JOIN workflow.documento doc ON doc.docid 	  = d.docid
			LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
			LEFT JOIN seguranca.usuario u ON u.usucpf 		  = d.usucpfexecutor
		WHERE 
			d.dmdstatus = 'A' 
			AND d.usucpfdemandante   = '".$_SESSION['usucpf']."'
			AND (doc.esdid in (91,107,92,108) OR doc.esdid is null) 
		ORDER BY 
			 d.dmddatainclusao DESC";
	
	$cabecalho = array( "C?digo" , "T?tulo da Demanda", "Situa??o", "Data de Inclus?o", "Data Prevista de T?rmino","T?cnico Respons?vel");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	
	$flag_duplicado = $db->PegaUm( $sql );
	
	
?>


<script type="text/javascript">
d = document;


/*
* Function destinada a valida??o do formul?rio
*/
function validaForm(){
	
	if (d.getElementsByName('assunto')[0].value == ''){
		d.getElementsByName('assunto')[0].focus();
		d.getElementsByName('assunto')[0].select();
		alert('O campo assunto, deve ser preenchido!');
		return false;
	}
	if (d.getElementsByName('necessidade')[0].value == ''){
		d.getElementsByName('necessidade')[0].focus();
		d.getElementsByName('necessidade')[0].select();
		alert('O campo necessidade, ? obrigat?rio!');
		return false;
	}

	<?if($flag_duplicado){?>
	if(confirm('Existe(m) Demanda(s) em Aberto. \nVerifique na lista abaixo se esta demanda j? existe. \nDeseja cadastrar mesmo assim?'))
	{
		return true;
	}else{
		return false;
	}
	<?}?>


}
</script>
</body>
