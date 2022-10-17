<?php

class FormularioQuestionario{
	private $dado;
	private $child;
	
	function __construct($name = 'formulario'){
		$this->dado['name'] 	= $name;
		$this->dado['action']	= '?' . $_SERVER['QUERY_STRING'];
		$this->dado['method']	= 'post';
		$this->dado['id']		= $name;
		$this->dado['enctype']	= 'multipart/form-data';
	}
	
	function __set($prop, $value){
		
	    // verifica se existe método set_<propriedade>
        if ( method_exists($this, 'set' . ucfirst($prop)) )
        {
            // executa o método set_<propriedade>
			call_user_func(array($this, 'set' . ucfirst($prop)), $value);
        }else{
            // atribui o valor da propriedade
			$this->dado[$prop] = $value;
        }
		
	}
	
	function __get($prop){
	        // verifica se existe método get_<propriedade>
        if ( method_exists($this, 'get' . ucfirst($prop) ))
        {
            // executa o método get_<propriedade>
            return call_user_func(array($this, 'get' . ucfirst($prop) ));
        }else{
            // retorna o valor da propriedade
            return $this->dado[$prop];
        }
	}
	
	
	function show(){
		$tag = new TElement("form");
		// Adiciona propriedades
		foreach($this->dado as $prop => $val){
			$tag->{$prop} = $val;
		}
		
		$this->child = $this->child ? $this->child : array('');
		while ( current($this->child) ){
			$tag->add( current($this->child) );
			next( $this->child );
		}
		// exibe o formulário
        $tag->show();		
	}
	
	
    /**
     * método add()
     * Adiciona um objeto no formulário
     * @param  $object = objeto a ser adicionado
     */
    public function add($object)
    {
        $this->child[] = $object;
    }	
    
}
?>