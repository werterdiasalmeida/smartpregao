<?php
/**
 * classe TRadioButton
 * Classe para construчуo de rсdio
 */
class TRadioButton extends TField
{
//    private $checked;
	private $tag;
	
	function __construct($nome){
		parent::__construct($nome);
		
		$this->tag = new TElement("input");
	}
    
	function setChecked($check){
		$this->prop['checked'] = $check;
	}
	
	function getChecked(){
		return $this->prop['checked'];
	}	
	
    /**
     * mщtodo show()
     * exibe o widget na tela
     */
    public function show()
    {
        // atribui as propriedades da TAG
        $this->tag->name  = $this->name;
        $this->tag->value = $this->value;
        $this->tag->type  = 'radio';
        
        // se o campo nуo щ editсvel
        if ( parent::getHabil() == 'N' )
        {
            // desabilita a TAG input
            $this->tag->disabled = "disabled";
        }
        
        if ( $this->checked ){
        	$this->tag->checked = "checked";
        }

        if ( $this->acao ){
        	$this->tag->onclick = $this->acao;
        }        
                
        // exibe a tag
        $this->tag->show();
    }
}
?>