<?php
class EntidadeEntidade extends EntidadeMatriz
{
	/*
	 * PESSOA - Início
	 */
	/**
	 * Função  pessoaTela
	 * Método usado para carregar/exibir a tela de pessoa
	 *
	 * @access public
	 * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since  17/06/2010
	 * @param  string $dado (default|reload|popup)
	 * @return void
	 */
	public function pessoaTela($op='default')
	{
		if ($this->mantemSessao){
			$_SESSION['agenda']['pessoa']['entid'] = $this->entidade['entid'];
		}
		$this->op = $op;
//		$this->entidade->entcpfcnpj = formatar_cpf_cnpj($this->entidade->entcpfcnpj);
//		$this->entidade->entcep 	= formata_cep($this->entidade->entcep);
		$this->entidade['entemail_confirmacao'] = $this->entidade['entemail'];
		$this->entidade['entemail_confirmacao_profissional'] = $this->entidade['entemailprofissional'];
		parent::extract($this->entidade);
		parent::renderView("pessoa");
	}

	/**
	 * Função  salvarPessoaDefault
	 * Método usado salvar a pessoa e dar REFRESH na página atual.
	 *
	 * @access public
	 * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since  17/06/2010
	 * @return void
	 */
	public function salvarPessoaDefault($url)
	{
		$_REQUEST['tpeid'] = ENTIDADE_PESSOA;
		$entid 			   = self::salvar();
//		if ($this->mantemSessao){
			$_SESSION['agenda']['pessoa']['entid'] = $entid;
//		}
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location='{$url}';
			 </script>");
	}

	/**
	 * Função  salvarPessoaPopup
	 * Método usado salvar a pessoa e dar REFRESH na página atual.
	 *
	 * @access public
	 * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since  17/06/2010
	 * @return void
	 */
	public function salvarPessoaPopup()
	{
		$_REQUEST['tpeid'] = ENTIDADE_PESSOA;
		$entid 			   = self::salvar();
//		if ($this->mantemSessao){
//			$_SESSION['agenda']['pessoa']['entid'] = $entid;
//		}

		die("<script type=\"text/javascript\" src=\"../../includes/JQuery/jquery-1.4.2.js\"></script>
			 <script type=\"text/javascript\" src=\"../../includes/agendaentidades.js\"></script>
			 <script>
			 	if($.isFunction(window.opener.atualizaLista)){
			 		alert('Operação realizada com sucesso!');
					window.opener.atualizaLista('pessoa');
					window.location = window.location;
				}else{
					if(window.opener.$('#_arvore')[0]){
						window.opener.$('#_arvore').jstree('refresh');
					}
					var Entidade = new Entidade();
					Entidade.popup.atualizaInformacaoPai({msg : '(" . formatar_cpf($_REQUEST['entcpfcnpj']) . ') ' . htmlspecialchars($_REQUEST['entnome'], ENT_QUOTES) . "', entid : {$entid}, enttelfone : '{$_REQUEST['enttelfone']}', entemail : '{$_REQUEST['entemail']}', entramal : '{$_REQUEST['entramal']}'});
					window.close();
				}
			 </script>");
	}
	/*
	 * PESSOA - Fim
	 */

	/*
	 * ÓRGÃO - INÍCIO
	 */
	/**
	 * Função  orgaoTela
	 * Método usado para carregar/exibir a tela de orgão
	 *
	 * @access public
	 * @author
	 * @since  20/06/2010
	 * @param  string $dado (default|reload|popup)
	 * @return void
	 */
	public function orgaoTela($op='default')
	{
		$entid = $this->entidade['entid'];
		if ($this->mantemSessao){
			$_SESSION['agenda']['orgao']['entid'] = $entid;
		}
		$this->op = $op;

		if ($this->entidade['entidpai']){
			// Buscar entidade PAI
			$entidade = new Entidade();
			$dados = (array) $entidade->recuperarLinha("entid AS entidpai, '(' || COALESCE(TRIM(TO_CHAR(entcpfcnpj::float,'00\".\"000\".\"000\"/\"0000\"-\"00')), 'N/A') || ') ' || entnome AS entidadenomepai",
													  array("entid = {$this->entidade['entidpai']}", "entstatus='A'"),
													  false);
			parent::extract($dados);
		}
		// Buscar TIPO
		$dados = new TipoEntidade();
		$this->itens['tipo'] = $dados->recuperarTodos('tpeid AS codigo, tpedsc AS descricao', array("tpeid != '".ENTIDADE_PESSOA."'" ), 'tpedsc' );

		$this->entidade['entemail_confirmacao'] = $this->entidade['entemail'];
		parent::extract($this->entidade);

		parent::renderView("orgao");
	}

	/**
	 * Função  salvarOrgaoDefault
	 * Método usado salvar o orgão e dar REFRESH na página atual.
	 *
	 * @access public
	 * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since  17/06/2010
	 * @return void
	 */
	public function salvarOrgaoDefault($url)
	{
		$entid 			   = self::salvar();
//		if ($this->mantemSessao){
			$_SESSION['agenda']['orgao']['entid'] = $entid;
//		}
		die("<script>
				alert('Operação realizada com sucesso!');
				window.location='{$url}';
			 </script>");
	}

	/**
	 * Função  salvarOrgaoPopup
	 * Método usado salvar o orgão e refresh.
	 *
	 * @access public
	 * @author Daniel dos Rios
	 * @since  28/07/2011
	 * @return void
	 */
	public function salvarOrgaoPopup()
	{
		$entid 			   = self::salvar();
//		if ($this->mantemSessao){
			$_SESSION['agenda']['orgao']['entid'] = $entid;
//		}
		die("<script type=\"text/javascript\" src=\"../../includes/JQuery/jquery-1.4.2.js\"></script>
			 <script>
			 	alert('Operação realizada com sucesso!');
				if($.isFunction(window.opener.atualizaLista)){
					window.opener.atualizaLista('orgao');
				}
				if(window.opener.$('#_arvore')[0]){
					window.opener.$('#_arvore').jstree('refresh');
				}
				window.location = window.location;
			 </script>");
	}

	/*
	 * ÓRGÃO - FIM
	 */

	/*
	 * GERAL (vale para todos os tipos PESSOA/ÓRGÃO) - Início
	 */
	/**
	 * Função  verificarCPFCNPJ
	 * Método usado para verificar se o cpf/cnpj já existe no banco.
	 *
	 * @access public
	 * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since  17/06/2010
	 * @return void
	 */
	public function verificarCPFCNPJ($cpfcnpj)
	{
			$entidade 	= new Entidade();
			$numCpfCnpj = removeCaractereEspecial($cpfcnpj);
			$cpfCnpj 	= $entidade->recuperarUm("entcpfcnpj", array("entcpfcnpj = '{$numCpfCnpj}'"));
//			$cpfCnpj = $entidade->pegaUm("SELECT entcpfcnpj FROM agenda.entidade WHERE entcpfcnpj = '{$numCpfCnpj}'");

			return $cpfCnpj;
	}
	/*
	 * GERAL (vale para todos os tipos PESSOA/ÓRGÃO) - fim
	 */


	private function salvar()
	{
//		if($pessoaFisica)
//		$_REQUEST['tpeid'] = ENTIDADE_PESSOA;

		$_REQUEST['entstatus'] = 'A';

		$arrCamposNulos = array();
		foreach ($_REQUEST as $index=>$value){
			if(!$value){
				$_REQUEST[$index] = null;
				$arrCamposNulos[] = $index;
			}
		}

//		if(is_array($_REQUEST['entidpai'])){
//			if($_REQUEST['entidpai'][count($_REQUEST['entidpai'])-1] != "")
//				$_REQUEST['entidpai'] = $_REQUEST['entidpai'][count($_REQUEST['entidpai'])-1];
//			else
//				$_REQUEST['entidpai'] = $_REQUEST['entidpai'][count($_REQUEST['entidpai'])-2];
//		}

		if($_REQUEST['entcpfcnpj'])
			$_REQUEST['entcpfcnpj'] = removeCaractereEspecial($_REQUEST['entcpfcnpj']);
		if($_REQUEST['entcep'])
			$_REQUEST['entcep']     = removeCaractereEspecial($_REQUEST['entcep']);

		if($_REQUEST['graulatitude'] && $_REQUEST['minlatitude'] && $_REQUEST['seglatitude']){
			$_REQUEST['medlatitude'] = $_REQUEST['graulatitude']  . "." . $_REQUEST['minlatitude']  . "." . $_REQUEST['seglatitude'] . "." . $_REQUEST['pololatitude'];
		}else{
			$_REQUEST['medlatitude'] = null;
			$arrCamposNulos[] = 'medlatitude';
		}

		if ( $_REQUEST['servidor'] == 'n' ){
			$_REQUEST['entsiape'] = NULL;
			$arrCamposNulos[] = "entsiape";
		}

		if($_REQUEST['graulongitude'] && $_REQUEST['minlongitude'] && $_REQUEST['seglongitude']){
			$_REQUEST['modlongitude'] = $_REQUEST['graulongitude'] . "." . $_REQUEST['minlongitude'] . "." . $_REQUEST['seglongitude'];
		}else{
			$_REQUEST['modlongitude'] = null;
			$arrCamposNulos[] = 'modlongitude';
		}

		$entidade = new Entidade($entid);

		$entid = $entidade->popularDadosObjeto()->salvar(true,true,$arrCamposNulos);
		$entidade->commit();

		return $entid;
	}
}