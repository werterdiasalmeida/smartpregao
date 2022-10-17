<?php
class TFile extends TField{

	function __construct($name, $possuiDescricao){
		parent::__construct($name);

		if($possuiDescricao == true){
    		$this->prop['descricao'] = new TTextArea('dsc_' . $name);
		}

		parent::setSize(50);
	}

	function setExtensao($extensao){
		$this->prop['extensao'] = $extensao;
	}

	function getExtensao(){
		return $this->prop['extensao'];
	}

	function getDescricao(){
		return $this->prop['descricao'];
	}

	private function _configurarDescricao(){
	    $valor = $this->getValue();

	    $this->getDescricao()->setHabil( $this->getHabil() );
	    $this->getDescricao()->setValue($valor['descricao']);


	}

	function show(){

	    $habil     = $this->getHabil() == 'N' ? 'disabled' : '';
	    $editarArq = $this->getHabil() == 'N' ? '' : "<img src='/imagens/anexo.gif' style='cursor: pointer; ' title='Enviar outro arquivo' onclick='quest.obPerg.enviarOutroArquivo(\"{$this->name}\");' />";
	    $valor     = $this->getValue();
	    $valor     = $valor['arquivo'];

	    $out = "";

	    if($valor){
	        $out .= "<span id='spnArquivo{$this->name}'>
            	         {$editarArq}
            	         <a href='javascript: quest.obPerg.downloadArquivo({$valor});' style='cursor:pointer; text-decoration: none;' title='Download do Arquivo'  >
                	         <img src='/imagens/salvar.png' style='border:0;' />
                	         <label style='font-weight: normal; vertical-align: top; cursor:pointer;' onclick='quest.obPerg.downloadArquivo({$valor});'>Arquivo</label>
            	         </a>
            	     </span>";
	    }

	    $out .= "<span id='spnArquivoNovo{$this->name}' ".($valor ? "style='display: none;'" : '').">";
		$out .= "    <input type='file' name='{$this->name}' id='{$this->name}' {$habil} onchange='quest.obPerg.validarExtensao(\"{$this->getExtensao()}\", this);' />";
		$out .= $this->obrig == 'S' ? obrigatorio() : '';
        $out .= "</span>";

		$out .= "<input type='hidden' name='perid' id='perid' />";

		echo $out;

		if($this->getDescricao()){
		    $this->_configurarDescricao();

		    echo "<br /><br />";
		    echo "<label style='font-weight: bold;'>Descrição: </label>";
		    $this->getDescricao()->show();
		}
	}
}
