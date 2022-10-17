<?php
	
class MunTipoMunicipio extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.muntipomunicipio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tpmid' => null, 
									  	'muncod' => null, 
									  	'estuf' => null, 
									  );
									  
	public function recuperarUFs(){

		$sql = "select estuf as codigo, estdescricao as descricao 
				from territorios.estado 
				order by estuf asc";
		
		return $this->carregar( $sql );
	}
	
	public function recuperarTiposMunicipio(){

		$sql = "select
					'<input type=\"checkbox\" name=\"tpmid[]\" id=\"tpmid_'|| tm.tpmid ||'\" value=\"'|| tm.tpmid ||'\" />' as checkbox,
					tpmdsc 
				from territorios.tipomunicipio tm
				where tpmstatus = 'A'";

		return $this->carregar( $sql );
	}
	
	public function excluirTudoPorMuncod( $muncod ){

		$sql = "delete from territorios.muntipomunicipio where muncod = '$muncod' ";

		return $this->carregar( $sql );
	}
	
	public function inserir(){
		
		$sql = "insert into territorios.muntipomunicipio values ( '$this->tpmid', '$this->muncod', '$this->estuf' )";
		
		return $this->carregar( $sql );
	}
	
}