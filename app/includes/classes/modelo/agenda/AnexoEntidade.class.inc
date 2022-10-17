<?php
	
class AnexoEntidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.anexoentidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "aenid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'aenid' 		=> null, 
									  	'taeid' 		=> null, 
									  	'entid' 		=> null,
 									  	'arqid' 		=> null,
    									'aendsc'		=> null
									  );
									  
	public function listarPorEntidade($entid, $coluna = null)
	{
		$coluna = $coluna ? $coluna : "*";
		$sql = "SELECT
					{$coluna}	
				FROM
					agenda.anexoentidade ae
				JOIN public.arquivo a ON a.arqid = ae.arqid
										 AND a.arqstatus = 'A'
				JOIN seguranca.usuario u ON u.usucpf = a.usucpf
				WHERE
					ae.entid = {$entid}";
		
		return $this->carregar($sql);
	}						
}
