<?php

	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	
	// carrega caminho para arquivo
	$diretorio = APPRAIZ . 'arquivos/anexos/';
	$arquivo = basename( $_REQUEST['arquivo'] );
	$caminho = $diretorio . $arquivo;
	$id = $_REQUEST['id'];
	
	// verifica se arquivo existe
	if ( $arquivo == '' || !file_exists( $caminho ) )
	{
		header( 'Content-Type: text/plain' );
		print 'Arquivo inexistente';
		exit();
	}
	
	// TODO realizar verificação de se o usuário pode visualizar o arquivo solicitado
	// testar direito de acesso
	// regras:
	// se ostensivo, então abero a todos
	// se reservado, todos os usuários cadastrados em documento_acesso podem ver o documento
	// se confidencial só o criador do documento pode ver
	// submete arquivo ao usuário
	$abrearquivo=0;
     $sql = "select docsigilo,usucpf from documento where docid=$id"	;
     $pode_ter_acesso = $db->pegaLinha($sql);
     if ($pode_ter_acesso['docsigilo']=='O' ) $abrearquivo = 1;
     else
     if ($pode_ter_acesso['docsigilo']=='C' and $pode_ter_acesso['usucpf']==$_SESSION['usucpf'])
     $abrearquivo = 1;
     else if ($pode_ter_acesso['docsigilo']=='R' ) 
     {
     	// verifica a tabela de documento_acesso
     	 $sql = "select * from documento_acesso where docid=$id and usucpf='".$_SESSION['usucpf']."' and doastatus='A'"	;
     	 $lista_acesso = $db->pegaLinha($sql);
     	 if (is_array($lista_acesso)) $abrearquivo=1;
     }
      if ($abrearquivo)
      {
  		header( 'Content-Type: ' . mime_content_type( $caminho ) );
		$fp = fopen( $caminho, 'r' );
		while( !feof( $fp ) )
		{
			print fgets( $fp, 1024 );
		}
		fclose( $fp );
      }
      else 
      {
      ?>
      <script>
          alert ('Este documento não pode ser aberto por você!');
          close();
      </script>
      <?
      }
      
	