<?php
	
class EventoParticipante extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.eventoparticipante";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "evpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'evpid'	 => null, 
    									'evtid'  => null,
    									'entid'  => null,
    								 	'evpconfirmado' => null
									  );	
									  
									  
	public function getParticipantes($evtid){
		$sql = " 
				SELECT 
					ent.entid   AS codigo,
					ent.entnome AS descricao				
				FROM 
					{$this->stNomeTabela} evp
				INNER JOIN
			   	 	agenda.entidade ent ON evp.entid = ent.entid
			   	 WHERE
			   	 	evp.evtid = {$evtid}
			   ";
		return $this->carregar($sql);
	}
	
	public function excluirParticipantes($evtid){
		$sql = "
			DELETE FROM
				{$this->stNomeTabela}
			WHERE 
				evtid = {$evtid}";

		$this->executar($sql);
	}
}