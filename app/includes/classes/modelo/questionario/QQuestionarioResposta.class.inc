<?php
	
class QQuestionarioResposta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.questionarioresposta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qrpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'qrpid' => null, 
									  	'queid' => null, 
									  	'qrptitulo' => null, 
									  	'qrpdata' => null, 
									  );
									  
	public function carregaUm(Array $where = null, $coluna = 'qrpid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if ($k == "qrpid"){
				$codicao[] = "qrpid = " . $item;
			}elseif ($k == "queid"){
				$codicao[] = "queid = " . $item;
			}
//			if (get_class($item) == 'Sistema'){
//				$codicao[] = "sisid = " . $item->sisid;
//			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		
		return parent::pegaUm($sql);
	}							

	public function pegaQuestionario( $qrpid ){
		
		$sql = "SELECT
					queid
				FROM
					{$this->stNomeTabela}
				WHERE
					qrpid = ".$qrpid;
					
		return parent::pegaUm($sql);
		
	}
}