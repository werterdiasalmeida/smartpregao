<?php
/**
 * classe TEntry
 * classe para constru��o de caixas de texto
 */
class TEntry extends TField
{
    /**
     * m�todo show()
     * exibe o widget na tela
     */
    public function show()
    {
        // atribui as propriedades da TAG
//        $this->tag->name = $this->name;     // nome da TAG
//        $this->tag->value = $this->value;   // valor da TAG
        $this->tag->type 		 = 'text';          // tipo de input
        $this->tag->style 		.= "width:{$this->size}"; // tamanho em pixels
        $this->tag->onblur 		.= '; MouseBlur(this);';		
        $this->tag->onfocus 	.= '; MouseClick(this); this.select();';		
        $this->tag->onmouseover .= '; MouseOver(this);';		
        $this->tag->onmouseout 	.= '; MouseOut(this);';		
        
        // se o campo n�o � edit�vel
        if (!parent::getEditable())
        {
            $this->tag->readonly = "1";
            $this->tag->class .= ' disabled'; // classe CSS
        }else{
	        $this->tag->class .= ' normal';		  // classe CSS
        }
        
        if($this->mask) $this->tag->maxlength = ""; 
        
        // exibe a tag
        $this->tag->show();
        
        if($this->mask){
	        $script = new TElement('script');
			$script->setType('text/javascript')
				   ->add("$('#{$this->tag->id}').mask('{$this->mask}',{placeholder:' '});")
				   ->show();
        }
    }
    
    /* 
      JQuery � Masked Input
      
	   Os par�metros que o plugin aceita para a m�scara s�o:
	
	     a � Representa um car�cter alpha num�rico (A-Z,a-z)
	     9 � Representa um car�cter num�rico (0-9)
    	 * � Representa um car�cter alpha numeric character (A-Z,a-z,0-9)
     */
    public function setMascara($mask){
//    	$this->tag->onkeyup .= ";this.value=mascaraglobal('$mask',this.value);";
        $this->mask = $mask;
    	return $this;
    }
}
?>