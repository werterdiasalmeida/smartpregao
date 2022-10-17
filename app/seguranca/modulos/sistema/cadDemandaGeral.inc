<?php

//VERIFICA SE O USUARIO POSSUI PERMISSÃO AO MÓDULO DEMANDAS
$sql = "SELECT count(usucpf) as total FROM seguranca.usuario_sistema WHERE sisid=44 and suscod='A' and usucpf='".$_SESSION['usucpf']."'";
$total = $db->pegaUm($sql);

if($total==0){
	// vincula o usuário com o módulo
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
 * Função que faz o insert da demanda 
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
	    case 13: //Módulo PAR - Plano de Metas
	        $sisid = 28;
	        break;
	    case 57: //Módulo Emendas
	        $sisid = 293;
	        break;
	}	
	
	$sidid = $sisid; 

	/*	
	--------------------- CODIGOS DA TABELA seguranca.sistema ----------------------------
	1;"Sistema de Monitoramento e Avaliação";"''";"PPA-Monitoramento e Avaliação";"monitora"
	2;"Sistema de Programação Orçamentária";"''";"Programação Orçamentária";"elabrev"
	3;"Sistema de Informações Orçamentárias e Financeiras";"http://";"SIOF";"siof"
	4;"Sistema de Segurança";"http://";"SIS";"seguranca"
	5;"Sistema de Elaboração e Revisão";"''";"PPA-Elaboração e Revisão";"elabrev"
	6;"Sistema de Projetos Especiais";"";"Projetos Especiais";"monitora"
	7;"Sistema  Orçamentário e Financeiro";"http://";"Orçamentário e Financeiro";"financeiro"
	10;"Sistema de Monitoramento do Plano de Desenvolvimento da Educação";"http://";"PDE";"pde"
	11;"Sistema de Gerenciamento de Projetos";"''";"Gerência de Projetos";"pde"
	12;"Sistema REUNI - Reestruturação e Expansão das Universidades"";"''";"REUNI";"reuni"
	13;"Sistema PAR - Plano de Metas Compromisso Todos pela Educação";"''";"PAR - Plano de Metas";"cte"
	14;"Sistema Brasil Profissionalizado";"''";"Brasil Profissionalizado";"brasilpro"
	15;"Sistema de Monitoramento de Obras";"''";"Monitoramento de Obras";"obras"
	17;"Sistema de Informações Gerenciais";"''";"SIG - Informações para Presidência";"sig"
	21;"Sistema Administrativo (Compras / Eventos / Contratos)";"''";"Administrativo";"evento"
	23;"PAR - Plano de Metas 2010";"''";"PAR 2010";"par"
	27;"Sistema REHUF";"''";"REHUF";"rehuf"
	29;"Sistema de Consultoria Jurídica";"''";"CONJUR";"conjur"
	32;"Sistema PAR Indígena";"http://";"PAR Indígena";"parindigena"
	34;"Sistema Escola (Pde Escola / Mais Educação / Escola Acessível / Escola Aberta)";"''";"Escola";"pdeescola"
	44;"Sistema de Demandas";"''";"Demandas";"demandas"
	48;"Sistema Painel de Controle";"http://";"Painel";"painel"
	56;"Sistema Rede Federal";"''";"Rede Federal";"academico"
	57;"Sistema de Emendas";"http://";"Emendas";"emenda"
	58;"Sistema de Gestão de Tarefas";"http://";"Gestão de Tarefas";"tarefa"
	62;"Sistema Assessoria Internacional";"''";"Assessoria Internacional";"assint"
	64;"Sistema de Gestão de Pessoas";"http://";"Gestão de Pessoas";"gestaopessoas"
	65;"Sistema do Programa Saúde na Escola";"http://";"PSE";"pse"
	66;"Sistema de Instituições de Educação Superior";"''";"IES";"ies"
	67;"Sistema Ensino Médio Inovador";"''";"Ensino Médio Inovador";"emi"

	--------------------- CODIGOS DA TABELA demandas.sistemadetalhe ----------------------------
	26;"SIMEC SIS";"SIMEC - Módulo de Segurança"
	27;"SIMEC ORç FINAN";"SIMEC - Módulo Orçamentário e Financeiro"
	28;"SIMEC PA PLANOM";"SIMEC - Módulo PAR - Plano de Metas Compromisso Todos pela Educação"
	29;"PPA-ELABORAçãO E REVISãO";"SIMEC - Módulo de Elaboração e Revisão do PPA"
	30;"REUNI";"REUNI - Reestruturação e Expansão das Universidades"
	31;"GERêNCIA DE PROJETOS";"SIMEC - Módulo de Gerenciamento de Projetos"
	32;"SIMEC PROJ ESP";"SIMEC - Módulo de Projetos Especiais"
	33;"SIMEC CONJUR";"SIMEC - Módulo de Consultoria Jurídica"
	34;"SIMEC PDE";"SIMEC - Módulo de Monitoramento do Plano de Desenvolvimento da Educação"
	35;"SIMEC PROGR ORç";"SIMEC - Módulo de Programação Orçamentária"
	36;"SIMEC AI";"SIMEC - Módulo Assessoria Internacional"
	37;"SIMEC DEMANDAS";"SIMEC - Módulo de Demandas"
	38;"SIMEC PAR INDíG";"SIMEC - Módulo PAR Indígena"
	39;"SIMEC SIG";"SIMEC - Módulo de Informações Gerenciais"
	40;"BRASIL PROFISSIONALIZADO";"SIMEC - Módulo Brasil Profissionalizado"
	41;"SIMEC REHUF";"SIMEC - Módulo Reestruturação de Hospitais Universitários"
	42;"SIMEC OBRAS";"SIMEC - Módulo de Monitoramento de Obras"
	43;"SIMEC PDEESCOLA";"SIMEC - Módulo Escola - PDE-Escola"
	205;"PSE";"Programa Saúde na Escola"
	207;"SIMEC";"Sistema de Monitoramento Estratégico"
	232;"SIMEC - PAINEL";"Painel de Controle"
	235;"SIMEC EVENTOS";"SIMEC - Módulo Administrativo - Eventos"
	236;"Rede Federal";"Rede Federal"
	238;"SIMEC - CTE";"SIMEC - Módulo Cadastro de Tecnologias Educacionais"
	239;"Plano de Ações ";"SIMEC - Módulo Plano de Ações Articuladas"
	240;"SIMEC - LSE";"SIMEC - Módulo Levantamento da Situação Escolar"
	243;"Biblioteca Proj";"SIMEC - Módulo Biblioteca de Projetos"
	282;"SIMEC COMPRAS";"SIMEC - Módulo Administrativo - Compras "
	283;"SIMEC-MAIS EDUC";"SIMEC - Módulo Escola - Mais Educação"
	290;"SIMEC SIGOEI";"SIMEC - Módulo de Gestão da OEI"
	293;"SIMEC EME";"SIMEC - Módulo Emendas"
	294;"SIMEC TAR";"SIMEC - Módulo Gestão de Tarefas"
	295;"RH";"Módulo Mapeamento da Força de Trabalho"
	296;"SIMEC RH GDPGPE";"SIMEC - Módulo RH - Avaliação da GDPGPE"
	297;"SIMEC RH CAMS";"SIMEC - Módulo RH - Monitoramento de Atendimento na CAMS"
	313;"SIMEC - EMI";"SIMEC - Módulo Ensino Médio Inovador"
	315;"SIMEC RH FT";"SIMEC - Módulo RH - Força de Trabalho"
	350;"SIMEC IES";"SIMEC - Módulo de IES"
	351;"SIMEC SIDOC-WEB";"SIMEC - Módulo de Gestão de Documentos - SIDOC-WEB"
	361;"SIMEC CONTRATOS";"SIMEC - Módulo Administrativo - Contratos"
	362;"SIMEC ESCACESSI";"SIMEC - Módulo Escola - Escola Acessível"
	363;"SIMEC ESCABERTA";"SIMEC - Módulo Escola - Escola Aberta"
	365;"SIMEC SEED";"SIMEC - Módulo de Gerenciamento de Processos da SEED"
	371;"ENEM";"Exame Nacional do Ensino Médio"
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
 * Executa a função que salva a demanda
 * Retorna mensagem de sucesso e redireciona a página
 */
if($_POST['assunto'] && $_POST['necessidade']) {

 	 $dmdid 	= salvaDemanda();
 	 
 	 

 	 if (!$_FILES['anexo']['size'] || EnviarArquivo($_FILES['anexo'], '', $dmdid)) {
		
 	 	 //envia email
	 	 $remetente = array('nome'=>'Módulo Demandas', 'email'=>'demandas@mec.gov.br');
	 	 $destinatario = "marilucia.queiroz@mec.gov.br";
	 	 $emailCopia = "cristiano.cabral@mec.gov.br";
		 $assunto = "Demanda Nº {$dmdid} Cadastrada pelo módulo ".$_SESSION['sisabrev'];
		 $conteudo = "Demanda Nº {$dmdid} Cadastrada pelo módulo ".$_SESSION['sisabrev'].".<br>Acesse o sistema demandas para acompanhá-la.";
		 enviar_email( $remetente, $destinatario, $assunto, $conteudo, $emailCopia );
 	 	
 		?>
 		<script>
		 		alert("Sua Demanda de nº <?=$dmdid?> foi enviada com sucesso. \nAguarde que iremos atendê-lo o mais breve possível!");
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


######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null,$dmdid){
	global $db;

	if (!$arquivo || !$dmdid)
		return false;
		
	// obtém o arquivo
	#$arquivo = $_FILES['arquivo'];
	if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
		redirecionar( 'demandas', $_REQUEST['acao'], $parametros );
	}
	// BUG DO IE
	// O type do arquivo vem como image/pjpeg
	if($arquivo["type"] == 'image/pjpeg') {
		$arquivo["type"] = 'image/jpeg';
	}
	
	$sisid = 44; // módulo demandas
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

######## Fim funções de UPLOAD ########


	
include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

monta_titulo( 'Cadastro de Demanda', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.' );
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
			  ELSE '<span style=\'color:blue;\' title=\'Não Atribuído\'>Em Processamento</span>'
			END AS situacao,				
			to_char(d.dmddatainclusao, 'DD/MM/YYYY HH24:MI:SS') AS dmddatainclusao,
			to_char(d.dmddatafimprevatendimento, 'DD/MM/YYYY HH24:MI:SS') AS dmddataconclusao,
			CASE
			  WHEN u.usucpf IS NOT NULL THEN usunome
			  ELSE '<span style=\'color:red;\' title=\'Não Atribuído\'>Não Informado</span>'
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
	
	$cabecalho = array( "Código" , "Título da Demanda", "Situação", "Data de Inclusão", "Data Prevista de Término","Técnico Responsável");
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '');
	
	$flag_duplicado = $db->PegaUm( $sql );
	
	
?>


<script type="text/javascript">
d = document;


/*
* Function destinada a validação do formulário
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
		alert('O campo necessidade, é obrigatório!');
		return false;
	}

	<?if($flag_duplicado){?>
	if(confirm('Existe(m) Demanda(s) em Aberto. \nVerifique na lista abaixo se esta demanda já existe. \nDeseja cadastrar mesmo assim?'))
	{
		return true;
	}else{
		return false;
	}
	<?}?>


}
</script>
</body>
