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
		// define algumas caracter?sticas iniciais
        self::setHabil(true);
        self::setObrig(false);
        self::setName($name);
        self::setSize(250);
        
        $this->db = new cls_banco();
	}

	function __set($prop, $value){
		
	    // verifica se existe m?todo set_<propriedade>
        if ( method_exists($this, 'set' . ucfirst($prop)) )
        {
        	$this->{'set' . ucfirst($prop)}( $value ); 
            // executa o m?todo set_<propriedade>
//			call_user_func(array($this, 'set' . ucfirst($prop)), $value);
        }/*else{
            // atribui o valor da propriedade
			$this->prop[$prop] = $value;
        }*/
		
	}
	
	function __get($prop){
	        // verifica se existe m?todo get_<propriedade>
        if ( method_exists($this, 'get' . ucfirst($prop) ))
        {
            // executa o m?todo get_<propriedade>
            return call_user_func(array($this, 'get' . ucfirst($prop) ));
        }/*else{
            // retorna o valor da propriedade
            return $this->prop[$prop];
        }*/
	}	
	
    /**
     * m?todo setName()
     * Define o nome do widget
     * @param  $name    = nome do widget
     */
    public function setName($name)
    {
        $this->prop['name'] = $name;
    }

    /**
     * m?todo getName()
     * Retorna o nome do widget
     */
    public function getName()
    {
        return $this->prop['name'];
    }
    
    /**
     * m?todo setValue()
     * Define o valor de um campo
     * @param  $value   = valor do campo
     */
    public function setValue($value)
    {
        $this->prop['value'] = $value;
    }

    /**
     * m?todo getValue()
     * Retorna o valor de um campo
     */
    public function getValue()
    {
        return $this->prop['value'];
    }
    
    /**
     * m?todo setEditable()
     * Define se o campo poder? ser editado
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
     * m?todo getEditable()
     * Retorna o valor da propriedade habil
     */
    public function getHabil()
    {
        return $this->prop['habil'];
    }
    
//    /**
//     * m?todo setProperty()
//     * Define uma propriedade para o campo
//     * @param  $name  = nome propriedade
//     * @param  $valor = valor da propriedade
//     */
//    public function setProperty($name, $value)
//    {
//        // $this->tag ? um objeto agregado
//        // ser? executado o seu m?todo __set()
//        // lidando internamente com a propriedade
//        $this->tag->$name = $value;
//    }
    
    /**
     * m?todo setSize()
     * Define a largura do widget
     * @param  $size    = tamanho em pixels
     */
    public function setSize($size)
    {
        $this->prop['size'] = $size;
    }	
    
    /**
     * m?todo getSize()
     * retorna a largura da propriedade size
     */
    public function getSize()
    {
		return $this->prop['size'];
    }	
    
    /**
     * m?todo setObrigatorio()
     * Define a obrigatoriedade do campo
     * @param  $obrig    = TRUE ou FALSE
     */
    public function setObrig($obrig)
    {
    	$obrig = $obrig ? 'S' : 'N';
        $this->prop['obrig'] = $obrig;
    }	
    
    /**
     * m?todo getObrigatorio()
     * retorna a obrigatoriedade do campo
     */
    public function getObrig()
    {
        return $this->prop['obrig'];
    }	

    /**
     * m?todo setLabel()
     * Define a label do campo
     * @param  $label = string do Label
     */
    public function setLabel($label)
    {
        $this->prop['label'] = $label;
    }	
    
    /**
     * m?todo getLabel()
     * retorna o label do campo
     */
    public function getLabel()
    {
        return $this->prop['label'];
    }	
    
    /**
     * m?todo setAcao()
     * Define a acao do campo
     * @param  $acao = (string) fun??o da a??o JAVASCRIPT
     */
    public function setAcao($acao)
    {
        $this->prop['acao'] = htmlspecialchars($acao);
    }	
    
    /**
     * m?todo getAcao()
     * retorna o acao do campo
     */
    public function getAcao()
    {
        return $this->prop['acao'];
    }	
    
    /**
     * m?todo setId()
     * Define o id do campo
     * @param  $id = (string) ID do campo
     */
    public function setId($id)
    {
        $this->prop['id'] = $id;
    }	
    
    /**
     * m?todo getId()
     * retorna o ID do campo
     */
    public function getId()
    {
    	$this->prop['id'] = empty($this->prop['id']) ? str_replace(array("[","]"), array("_"), $this->prop['name']) : $this->prop['id'];
        return $this->prop['id'];
    }	
	
}
?>