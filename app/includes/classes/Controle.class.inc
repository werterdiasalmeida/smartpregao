<?php

abstract class Controle
{

	/**
	 * Objeto html
	 * @access protected
	 * @var string
	 * @name $html
	 */
	protected $visao;
	
	/**
	 * 
	 * Enter description here ...
	 * @var unknown_type
	 */
	protected $db; 
	
	/**
	 *
	 * Construtor seta o objeto de registro
	 *
	 * @param object $registry
	 */
	public function __construct()
	{
		global $db;
			
		if($db){
			$this->db = $db;
		} else {
			$this->db = new cls_banco();
		}
		$this->visao = new Visao();
		$this->visao->db = $this->db;
	}

}