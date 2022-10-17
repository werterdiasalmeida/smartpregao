<?php


class RelatorioNovo
{

    protected $arrLabels;
    protected $arrColunas;
    protected $arrAgrupadores;
    protected $arrAgrupadoColunas;

    public function __construct($arrLabels = array(), $arrColunas = array(), $arrAgrupadores = array(), $arrAgrupadoColunas = array())
    {
        $this->arrLabels = $arrLabels;
        $this->arrColunas = $arrColunas;
        $this->arrAgrupadores = $arrAgrupadores;
        $this->arrAgrupadoColunas = $arrAgrupadoColunas;
    }

    public function monta_agp(array $agrupador = array())
    {
        $agrupador = $agrupador ? $agrupador : array();

        $agrupador = array_map(function ($k){
            return array(
                'campo' => $k,
                'label' => $this->arrLabels[$k]
            );
        }, $agrupador);

        $agp = array(
            "agrupador" => $agrupador,
            "agrupadoColuna" => $this->arrAgrupadoColunas
        );

        return $agp;
    }

    public function monta_coluna(array $coluna = array())
    {
        $coluna = array_map(function ($k){
            return array(
                'campo' => $k,
                'label' => $this->arrLabels[$k]
            );
        }, $coluna);

        return $coluna;
    }

    public function arrayToSeletize($arr){
        $arr = array_map(function($k){
            return array(
                'codigo' => $k,
                'descricao' => $this->arrLabels[$k]
            );
        }
        ,$arr);
        return json_encode(utf8ArrayEncode($arr));
    }

    function gravarParametrosBanco( $stTituloRelatorio ){
        global $db;
        $sql = "select prtid from public.parametros_tela 
                WHERE prtdsc = '{$stTituloRelatorio}' 
                AND mnuid = {$_SESSION['mnuid']}
                AND usucpf = '{$_SESSION['usucpf']}'";
        if ( $prtid = $db->pegaUm( $sql ) ) {
            $sql = sprintf( "UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
                $_REQUEST['titulo'],
                addslashes( addslashes( serialize( $_REQUEST ) ) ),
                $_SESSION['usucpf'],
                $_SESSION['mnuid'],
                $prtid );
        }
        else
        {
            $sql = sprintf( "INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
                $stTituloRelatorio,
                addslashes( addslashes( serialize( $_REQUEST ) ) ),
                'FALSE',
                $_SESSION['usucpf'],
                $_SESSION['mnuid'] );
        }
        $db->executar( $sql );
        return $db->commit();

    }

    public function carregarParametrosBanco( $stTituloRelatorio ){
        global $db;
        $sql = "select prtobj from public.parametros_tela 
                WHERE prtdsc = '{$stTituloRelatorio}' 
                AND mnuid = {$_SESSION['mnuid']}
                AND usucpf = '{$_SESSION['usucpf']}'";
        $itens = $db->pegaUm( $sql );
        return unserialize( stripslashes( stripslashes( $itens ) ) );
    }

    public function montaListaParametrosBanco( $mnuid = null, $usucpf = null ){
        global $db;
        // Se não passar valor por parametro recupera da asessão
        $mnuid  = ( empty( $mnuid ) )  ? $_SESSION['mnuid']  : $mnuid;
        $usucpf = ( empty( $usucpf ) ) ? $_SESSION['usucpf'] : $usucpf;

        $sql = "SELECT prtdsc as descricao, prtdsc as codigo FROM public.parametros_tela 
				WHERE mnuid = " . $mnuid . " AND usucpf = '" . $usucpf . "'";

        $result = $db->carregar($sql);
        return json_encode(utf8ArrayEncode($result));

    }

    public function excluirParametroBanco( $stTituloRelatorio ){
        global $db;
        $sql = "DELETE from public.parametros_tela WHERE prtdsc = '{$stTituloRelatorio}' 
                AND mnuid = {$_SESSION['mnuid']}
                AND usucpf = '{$_SESSION['usucpf']}'";
        $db->executar( $sql );
        return $db->commit();
    }

    public function parametroBancoTornarPublico( $prtid ){
        global $db;
        $sql = "UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = " . $prtid;
        $db->executar( $sql );
        $db->commit();
    }
}