<?php
set_time_limit(0);

class ClassImage{
	
    public $arquivo = "";
    private $erro = array ( "0" => "upload execultado com sucesso!",
                        "1" => "O arquivo é maior que o permitido pelo Servidor",
                        "2" => "O arquivo é maior que o permitido pelo formulario",
                        "3" => "O upload do arquivo foi feito parcialmente",    
                        "4" => "Não foi feito o upload do arquivo"
                       );	
    public function Verifica_Upload(){

        $this->arquivo = isset($_FILES['Filedata']) ? $_FILES['Filedata'] : FALSE;

        	if(!is_uploaded_file($this->arquivo['tmp_name'])) {
            		return false;
        	}    
        
		$get = getimagesize($this->arquivo['tmp_name']);
        
        	if($get["mime"] != "image/jpeg"){
			echo "Esse foto nao é uma imagem valida";
            		exit;
        	}
        return true;
    }
    
    public function reduz_imagem($img, $max_x, $max_y, $nome_foto, $extensao=false) {
		ini_set("memory_limit", "128M");
		list($width, $height) = getimagesize($img);
		$original_x = $width;
		$original_y = $height;
		
		if(!$original_x || !$original_y) {
			echo "<script>alert('Problemas na redução da imagem, entre em contato com a equipe técnica.');window.close();</script>";
			exit;
		}
			// se a largura for maior que altura
		if($original_x > $original_y) $porcentagem = (100 * $max_x) / $original_x;      
		else $porcentagem = (100 * $max_y) / $original_y;  
			
		$tamanho_x = $original_x * ($porcentagem / 100);
		$tamanho_y = $original_y * ($porcentagem / 100);
		
		switch($extensao) {
			case  'jpg':
			case 'jpeg':
				$image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
				$image   = imagecreatefromjpeg($img);
				imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
				return imagejpeg($image_p, $nome_foto, 100);
			break;
			case 'png':
			case 'gif':
				if ( !move_uploaded_file( $img, $nome_foto ) ) {
					echo "<script>alert(\"Problemas no envio do arquivo.\");</script>";
					return false;
				}
				return true;
			break;
			default:
				return false;
			
		}
	}		

    public function Envia_Arquivo(){
	
        if($this->Verifica_Upload()) {
           	$this->gera_fotos();
            return true;        
        } else {

           echo $this->erro[$this->arquivo['error']];
        }
    }
    public function gera_fotos(){
        
		$diretorio = "../../../arquivos/obras/imgs_tmp/";
        $nome_foto = date('YmdHis').md5($_FILES['Filedata']['name']).".jpg";          
        $this->reduz_imagem($this->arquivo['tmp_name'], 640, 480, $diretorio.$nome_foto);
		echo $this->erro[$this->arquivo['error']];
    }
    public function ResizeImage($filename,$w,$h){
    					
		// Get new sizes
		list($width, $height) = getimagesize($filename);
		$newwidth = $w;
		$newheight = $h;
		
		// Load
		$thumb = imagecreatetruecolor($newwidth, $newheight);
		$source = imagecreatefromjpeg($filename);
		
		// Resize
		imagecopyresized($thumb, $source, 0, 0, 0, 0, $newwidth, $newheight, $width, $height);
		
		// Output
		imagejpeg($thumb);
    	
    }    
}

?>