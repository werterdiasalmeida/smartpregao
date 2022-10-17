<?php
	
class Evento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "agenda.evento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "evtid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'evtid'	 	   => null, 
    									'evtnome'	   => null,
    									'evtlocal'	   => null,
    									'evtdescricao' => null,
    									'evtinicio'    => null,
    									'evtfim' 	   => null,
    									'entidpj' 	   => null,
    								 	'entid' 	   => null
									  );
									  
	public function getDados(){
		$arAtr = $this->arAtributos;
		
		$inicioTimeStamp = explode(" ", $arAtr['evtinicio']);
		$arAtr['evtinicio_data'] = $inicioTimeStamp[0];
		$arAtr['evtinicio_hora'] = substr($inicioTimeStamp[1], 0, 5);
		
		
		$fimTimeStamp = explode(" ", $arAtr['evtfim']);
		$arAtr['evtfim_data'] = $fimTimeStamp[0];
		$arAtr['evtfim_hora'] = substr($fimTimeStamp[1], 0, 5);
		
		return $arAtr;
	}

}