<?php
class NotificacaoAppEnviada extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "app.notificacaoappenviada";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("naeid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'naeid'			=> null,
		'noaid'		    => null,
		'naedtenviado'	=> null,
		'usucpf'		=> null
	);

	public function getLista(array $arWhere = array(), $paginacao = false, $limit = null){
		$arWhere[] = 'true';

		$sql = "SELECT
			    	usu.usunome,
			    	TO_CHAR(nae.naedtenviado, 'DD/MM/YYYY HH24:MI:SS') AS naedtenviado_formatado,
			    	COALESCE(COUNT(DISTINCT dis.disidentificador), 0) AS qtd_enviado,
			    	COALESCE(COUNT(DISTINCT din.dindtrecebida), 0) AS qtd_recebido,
			    	COALESCE(COUNT(DISTINCT din.dindtlida), 0) AS qtd_lida
			    FROM
			    	app.notificacaoappenviada nae
			    INNER JOIN
			    	seguranca.usuario usu ON usu.usucpf = nae.usucpf
			    LEFT JOIN
			    	app.dispositivonotificacaoapp din ON din.naeid = nae.naeid
			    LEFT JOIN
			    	app.dispositivo dis ON dis.disid = din.disid
			    WHERE
			    	" . implode(' AND ', $arWhere) . "
			    GROUP BY
			    	nae.naedtenviado, usu.usunome";

		if(!$_POST["ordem"]){
			$sql .= " ORDER BY nae.naedtenviado DESC";
		}

		if($limit){
			$sql .= " LIMIT {$limit}";
		}

		if($paginacao){
			$dado = Paginacao::getPaginacao($this, $sql, 50);
			return $dado;
		}else{
			$retorno = $this->carregar($sql);
			return $retorno ? $retorno : array();
		}
	}
}

