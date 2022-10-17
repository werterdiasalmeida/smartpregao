<?php
abstract class TField{
	protected $prop;
	protected $db;
//	protected $name;
//	protected $size;
//	protected $value;
//	protected $habil;
//	protected $tag;

	function __construct($name){
		// define algumas características iniciais
        self::setHabil(true);
        self::setObrig(false);
        self::setName($name);
        self::setSize(250);
        
        $this->db = new cls_banco();
	}

	function __set($prop, $value){
		
	    // verifica se existe método set_<propriedade>
        if ( method_exists($this, 'set' . ucfirst($prop)) )
        {
        	$this->{'set' . ucfirst($prop)}( $value ); 
            // executa o método set_<propriedade>
//			call_user_func(array($this, 'set' . ucfirst($prop)), $value);
        }/*else{
            // atribui o valor da propriedade
			$this->prop[$prop] = $value;
        }*/
		
	}
	
	function __get($prop){
	        // verifica se existe método get_<propriedade>
        if ( method_exists($this, 'get' . ucfirst($prop) ))
        {
            // executa o método get_<propriedade>
            return call_user_func(array($this, 'get' . ucfirst($prop) ));
        }/*else{
            // retorna o valor da propriedade
            return $this->prop[$prop];
        }*/
	}	
	
    /**
     * método setName()
     * Define o nome do widget
     * @param  $name    = nome do widget
     */
    public function setName($name)
    {
        $this->prop['name'] = $name;
    }

    /**
     * método getName()
     * Retorna o nome do widget
     */
    public function getName()
    {
        return $this->prop['name'];
    }
    
    /**
     * método setValue()
     * Define o valor de um campo
     * @param  $value   = valor do campo
     */
    public function setValue($value)
    {
        $this->prop['value'] = $value;
    }

    /**
     * método getValue()
     * Retorna o valor de um campo
     */
    public function getValue()
    {
        return $this->prop['value'];
    }
    
    /**
     * método setEditable()
     * Define se o campo poderá ser editado
     * @param  $habil = TRUE ou FALSE
     */
    public function setHabil($habil)
    {
    	if ( is_bool($habil) )
    		$habil = $habil ? 'S' : 'N';
    	else
    		$habil = $habil;
    		
        $this->prop['habil'] = $habil;
    }

    /**
     * método getEditable()
     * Retorna o valor da propriedade habil
     */
    public function getHabil()
    {
        return $this->prop['habil'];
    }
    
//    /**
//     * método setProperty()
//     * Define uma propriedade para o campo
//     * @param  $name  = nome propriedade
//     * @param  $valor = valor da propriedade
//     */
//    public function setProperty($name, $value)
//    {
//        // $this->tag é um objeto agregado
//        // seré executado o seu método __set()
//        // lidando internamente com a propriedade
//        $this->tag->$name = $value;
//    }
    
    /**
     * método setSize()
     * Define a largura do widget
     * @param  $size    = tamanho em pixels
     */
    public function setSize($size)
    {
        $this->prop['size'] = $size;
    }	
    
    /**
     * método getSize()
     * retorna a largura da propriedade size
     */
    public function getSize()
    {
		return $this->prop['size'];
    }	
    
    /**
     * método setObrigatorio()
     * Define a obrigatoriedade do campo
     * @param  $obrig    = TRUE ou FALSE
     */
    public function setObrig($obrig)
    {
    	$obrig = $obrig ? 'S' : 'N';
        $this->prop['obrig'] = $obrig;
    }	
    
    /**
     * método getObrigatorio()
     * retorna a obrigatoriedade do campo
     */
    public function getObrig()
    {
        return $this->prop['obrig'];
    }	

    /**
     * método setLabel()
     * Define a label do campo
     * @param  $label = string do Label
     */
    public function setLabel($label)
    {
        $this->prop['label'] = $label;
    }	
    
    /**
     * método getLabel()
     * retorna o label do campo
     */
    public function getLabel()
    {
        return $this->prop['label'];
    }	
    
    /**
     * método setAcao()
     * Define a acao do campo
     * @param  $acao = (string) função da ação JAVASCRIPT
     */
    public function setAcao($acao)
    {
        $this->prop['acao'] = htmlspecialchars($acao);
    }	
    
    /**
     * método getAcao()
     * retorna o acao do campo
     */
    public function getAcao()
    {
        return $this->prop['acao'];
    }	
    
    /**
     * método setId()
     * Define o id do campo
     * @param  $id = (string) ID do campo
     */
    public function setId($id)
    {
        $this->prop['id'] = $id;
    }	
    
    /**
     * método getId()
     * retorna o ID do campo
     */
    public function getId()
    {
    	$this->prop['id'] = empty($this->prop['id']) ? str_replace(array("[","]"), array("_"), $this->prop['name']) : $this->prop['id'];
        return $this->prop['id'];
    }	
	
}
?>