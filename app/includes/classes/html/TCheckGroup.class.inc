<?php
/**
 * classe TCheckGroup
 * Classe para exibi??o um grupo de CheckButtons
 */
class TCheckGroup extends TField
{
    private $layout = 'vertical';
    private $itens;
    private $onClick;

    /**
     * m?todo setLayout()
     * Define a dire??o das op??es (vertical ou horizontal)
     */
    public function setLayout($dir)
    {
        $this->layout = $dir;
    }
    
    /* m?todo itens($itens)
     * adiciona itens ao check group
     * @param  $itens = um vetor indexado de itens
     */
    public function setItens($item)
    {
        $this->itens[] = $item;
    }
    
    /* m?todo setOnClick($itens)
     * adiciona evento onclick aos itens
     * @param  $onClick = um vetor indexado de itens
     */
//    public function setOnClick($onClick)
//    {
//        $this->onClick[] = $onClick;
//    }
//    
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
            	$label   = $item['descricao'];	
            	$onclick = $item['onClick'];	
            	
                $button = new TCheckButton("{$this->name}[]");
                $button->setValue($index);
                $button->setHabil( $this->getHabil() );
//                $button->setAcao( $this->getAcao() );
                
                if ( is_array($this->value) &&  @in_array($index, $this->value))
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
                    echo "\n";
                }
            }
        }
    }
}
?>