<?php

	class Mensagem{

		/**
		 * Indica a quantidade de itens que devem ser exibidos por página.
		 */
		const QUANTIDADE = 30;
		private $banco;
		
		public function Mensagem()
		{
			global $db;
			$this->banco = $db;
		}
		
		private function listar( $sql_total, $sql_dados, $pagina )
		{
			
			
			/*dbg( $sql_total );
			dbg( $sql_dados );*/
			
			$retorno = new StdClass();
			$retorno->dados = $this->banco->carregar( $sql_dados );
			$retorno->total = $this->banco->pegaUm( $sql_total );
			
			return $retorno;
		}

		public function listar_enviadas( $usucpf )
		{
			return null;
		}

		public function listar_excluidas( $usucpf )
		{
			return null;
		}

		public function listar_recebidas( $usucpf, $pagina = 0 ){
			
			$sql_total = sprintf(
				"SELECT count(msgid) as total 
				FROM public.mensagem m
				INNER JOIN public.mensagemusuario mu USING ( msgid )
				LEFT JOIN seguranca.usuario u ON m.usucpf = u.usucpf
				LEFT JOIN seguranca.sistema s ON m.sisid = s.sisid
				WHERE mu.usucpf = '%s'
				AND mu.msuexcluida = 'f'",
				$usucpf
			);
			
			$sql_dados = sprintf(
				"SELECT m.*, mu.msulida, CASE WHEN m.usucpf IS NULL THEN s.sisdsc ELSE u.usunome END AS remetente
				FROM public.mensagem m
				INNER JOIN public.mensagemusuario mu USING ( msgid )
				LEFT JOIN seguranca.usuario u ON m.usucpf = u.usucpf
				LEFT JOIN seguranca.sistema s ON m.sisid = s.sisid
				WHERE mu.usucpf = '%s'
				AND mu.msuexcluida = 'f'
				ORDER BY m.msgid DESC
				LIMIT %d OFFSET %d				
				",
				$usucpf,
				self::QUANTIDADE,
				self::QUANTIDADE * $pagina
			);
			
			return $this->listar( $sql_total, $sql_dados, $pagina );
		}
		
		public function excluir( $msgid )
		{
			$sql = sprintf(
				"UPDATE public.mensagemusuario
				 SET msuexcluida='t'
				 WHERE msgid=%d
				",
				$msgid
			);
			if( !$this->banco->executar( $sql ) )
			{
				return false;
			}
			
			return true;
		}
		
		public function carregar_mensagem( $msgid )
		{
			$sql = sprintf(
				"SELECT m.*, mu.msulida, CASE WHEN m.usucpf IS NULL THEN s.sisdsc ELSE u.usunome END AS remetente
				FROM public.mensagem m
				INNER JOIN public.mensagemusuario mu USING ( msgid )
				LEFT JOIN seguranca.usuario u ON m.usucpf = u.usucpf
				LEFT JOIN seguranca.sistema s ON m.sisid = s.sisid
				WHERE mu.msgid = %d			
				",
				$msgid
			);
			
			return $this->banco->pegaLinha( $sql );			
		}
		
		public function marcar_lida( $msgid, $status )
		{
			$sql = sprintf(
				"UPDATE public.mensagemusuario
				 SET msulida = '%s'
				 WHERE msgid = %d
				",
				$status,
				$msgid
			);
			return $this->banco->executar( $sql );			
		}
		
	}

?>