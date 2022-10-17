<?php

include_once APPRAIZ . '/includes/VerImagemException.php';
include_once APPRAIZ . '/includes/SimpleImage.php';
include_once APPRAIZ . '/includes/classes_simec.inc';
include_once APPRAIZ . '/includes/classes/Modelo.class.inc';
include_once APPRAIZ . '/includes/classes/modelo/public/Arquivo.class.inc';

/**
 * Classe responsável por renderizar as imagens do SIMEC
 *
 * Class VerImagem
 */
class VerImagem
{
    const ERRO_FORMATO_NAO_SUPORTADO = 1;
    const PASTA_THUMBNAIL = 'thumbnail';

    /**
     * @var integer $arqid
     */
    protected $arqid;

    /**
     * @var string $caminho
     */
    protected $caminho;

    /**
     * @var SimpleImage $simpleImage
     */
    protected $simpleImage;

    /**
     * @var string $largura
     */
    protected $largura;

    /**
     * @var string $altura
     */
    protected $altura;

    /**
     * @var bool $redimensionado
     */
    protected $redimensionado = false;

    /**
     * @var bool $semImagem
     */
    protected $semImagem = false;

    /**
     * @var bool $marcaDAgua
     */
    protected $marcaDAgua = false;

    public function __construct($arqid = null)
    {
        $this->arqid = $arqid;
        $this->simpleImage = new SimpleImage();

        $this->verificaArquivo();
        $this->verificaFormato();
    }

    public function __destruct()
    {
        unset($this->simpleImage);
    }

    protected function verificaFormato()
    {
//        $type = mime_content_type($this->caminho);
//        if(!preg_match('/^image\/(' . implode('|', ['gif', 'jpeg', 'png']) . ')$/', $type, $match)){
//            throw new VerImagemException('Formato não suportado: ' . $match[1], self::ERRO_FORMATO_NAO_SUPORTADO);
//        }
    }

    protected function verificaArquivo()
    {
        $caminho = APPRAIZ.'arquivos/' . (($_REQUEST["_sisarquivo"]) ? $_REQUEST["_sisarquivo"] : $_SESSION["sisarquivo"]) . '/' . floor($this->arqid / 1000) . '/' . $this->arqid;
        if (!is_file($caminho)) {
            if ($_SESSION['sisarquivo'] == 'obras2' || $_REQUEST['_sisarquivo'] == 'obras2') {
                $caminho = APPRAIZ.'arquivos/obras/' . floor($this->arqid / 1000) . '/' . $this->arqid;
            }
        }
        if (!is_file($caminho)) {
            $caminho = APPRAIZ . 'www/imagens/no-picture.png';
            $this->semImagem = true;
        }
        $this->caminho = $caminho;
    }

    public function setAltura($altura)
    {
        $this->altura = $altura;
    }

    public function setLargura($largura)
    {
        $this->largura = $largura;
    }

    public function redimenciona()
    {
        if (!empty($this->largura) || !empty($this->altura)) {
            $this->redimensionado = true;
        }
    }

    public function redimencionarOuSelecionar()
    {
        if ($this->semImagem) {
            $this->simpleImage->fromFile($this->caminho);
            if ($this->largura && $this->altura) {
                // $this->simpleImage->thumbnail($this->largura, $this->altura);
                $this->simpleImage->resize($this->largura, $this->altura);
            } else {
                $this->simpleImage->resize($this->largura, $this->altura);
            }
        } elseif ($this->redimensionado) {
            $this->verificarDiretorioThumbnail();
            $caminhoThumbnail = $this->getCaminhoThumbnail();
            if (!file_exists($caminhoThumbnail)) {
                $this->criarThumbnail($caminhoThumbnail);
            } else {
                try{
                    $this->simpleImage->fromFile($caminhoThumbnail);
                }catch (Exception $e){
                    ver($e->getMessage(), $e->getTraceAsString(), d);
                }
            }
        } else {
            $this->simpleImage->fromFile($this->caminho);
        }
    }

    public function aplicaMarcaDAgua($caminho)
    {
        if (!$this->semImagem) {
            $this->simpleImage->overlay($caminho, 'bottom right', 0.1, -30, -30);
//            $this->simpleImage->overlay($caminho, 'bottom left', 0.1, 30, -30);
//            $this->simpleImage->overlay($caminho, 'top right', 0.1, -30, 30);
//            $this->simpleImage->overlay($caminho, 'top left', 0.1, 30, 30);
//            $this->simpleImage->overlay($caminho, 'center', 0.1);
        }
    }

    public function exibir()
    {
        $this->simpleImage->toScreen();
    }

    public function getCaminhoThumbnail()
    {
        $diretorio = dirname($this->caminho);
        $diretorioThumbnail = $diretorio . '/' . self::PASTA_THUMBNAIL;

        if($this->altura && $this->largura){
            $nomeThumbnail = $this->altura . '_' . $this->largura . '_' . $this->arqid;
        }else if($this->altura) {
            $nomeThumbnail = $this->altura . '_0_' . $this->arqid;
        }else{
            $nomeThumbnail = '0_' . $this->largura . '_' . $this->arqid;
        }

        return $diretorioThumbnail . '/' . $nomeThumbnail;
    }

    public function verificarDiretorioThumbnail()
    {
        $diretorio = dirname($this->caminho);
        $diretorioThumbnail = $diretorio . '/' . self::PASTA_THUMBNAIL;
        if (!is_dir($diretorioThumbnail)) {
            mkdir($diretorioThumbnail, 0777);
        }
    }

    public function criarThumbnail($caminhoThumbnail)
    {
        $this->simpleImage->fromFile($this->caminho);

        if ($this->largura && $this->altura) {
            $this->simpleImage->thumbnail($this->largura, $this->altura);
            // $this->simpleImage->resize($this->largura, $this->altura);
        }else {
            $this->simpleImage->resize($this->largura, $this->altura);
        }
        $this->simpleImage->toFile($caminhoThumbnail);
    }

    /**
     * @return SimpleImage
     */
    public function getSimpleImage()
    {
        return $this->simpleImage;
    }

    /**
     * @param SimpleImage $simpleImage
     */
    public function setSimpleImage($simpleImage)
    {
        $this->simpleImage = $simpleImage;
    }

}
