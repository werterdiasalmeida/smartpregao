<?php
/**
 * classe TRadioGroup
 * Classe para exibi??o de um grupo de Radio Buttons
 */
class TRadioGroup extends TField
{
    private $layout = 'vertical';
    private $itens;
    
    /**
     * m?todo setLayout()
     * Define a dire??o das op??es (vertical ou horizontal)
     */
    public function setLayout($dir)
    {
        $this->layout = $dir;
    }
    
    /**
     * method addItens($itens)
     * adiciona itens (bot?es de r?dio)
     * @param  $itens = array idexado contendo os itens
     */
    public function setItens($itens)
    {
        $this->itens[] = $itens;
    }
    
    /**
     * m?todo show()
     * exibe o widget na tela
     */
    public function show()
    {
        if ($this->itens)
        {
            // percorre cadauma das op??es do r?dio
            foreach ($this->itens as $item/*$index => $label*/)
            {
            	$index 	 = $item['codigo'];
            	$label 	 = $item['descricao'];	
                $onclick = $item['onClick'];	
            	
                $button = new TRadioButton($this->name);
                $button->setValue($index);
                $button->setHabil( $this->getHabil() );
                
//                $button->setAcao( $this->getAcao() );
                
                if ( (is_array($this->value) &&  @in_array($index, $this->value)) || ($this->value == $index) )
                {
                    $button->checked = "checked";
                }
                
                if ( $onclick ){
	                $button->setAcao( $onclick );
                }
                
                $button->show();
                $obj = new TLabel($label);
                $obj->show();
                
                if( $item['divSubPergunta'] instanceof TElement ){
                	$item['divSubPergunta']->show();
                }
                
                if ($this->layout == 'vertical')
                {
                    // exibe uma tag de quebra de linha
                    $br = new TElement('br');
                    $br->show();
                }
                
                echo "\n";
            }
        }
    }
}
?>
