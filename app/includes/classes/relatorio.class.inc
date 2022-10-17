<?php

define( "TIPO_TOTALIZADOR_SUB_ITEM", 2 );

/*
 * Classe   MontaRelatorio
 * @author  Felipe Tarchiani Cerávolo Chiavicatti
 * @since   13/02/2009
 * @exemple pdeescola/modulos/relatorio/resultRel.php 
 * 			pdeescola/modulos/relatorio/resultFinanceiro.php
 * @link    http://simec-local/pdeescola/pdeescola.php?modulo=relatorio/formRel&acao=A
 * 			http://simec-local/pdeescola/pdeescola.php?modulo=relatorio/formFinanceiro&acao=A 
 */
class montaRelatorio{

    public $db;
    private $agrupador;
    private $ctrlAgp;
    private $arrAgp;
    private $js;
    private $coluna;
    private $erro;
    private $relatorio;
    private $dados;
    private $profundidade;
    private $totalizador;
    private $agpContador;
    private $brasao;
    private $totalizadorGeral;
    private $totalizadorTextoGeral;
    private $totalizadorLinha;
    private $monstrarTolizadorNivel;
    private $ordenacao;
    private $orderType;
    private $espandir;
    private $xls;
    private $linexls=1;
    private static $indRel;

    /**
     * Função __construct
     * Método construtor do PHP. Carrega alguns atributos da classe.
     *
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @return void
     */
    function __construct(){
        self::$indRel++;
        //global $db;
        //$this->db = $db;
        if (class_exists('Banco')){
            $this->db = new Banco;
        }else{
            $this->db = (object) '';
        }

        $this->totalizador = array();
        $this->setTotNivel(false);
        $this->setBrasao(false);
        $this->setMonstrarTolizadorNivel(false);
        $this->setTolizadorLinha(true);
        $this->setTotalizador(true);
        $this->setTextoTotalizador();
        $this->setEspandir(false);
        $this->ordenacao = $_POST['ordenacao'] ? $_POST['ordenacao'] : '';
        $this->orderType = $_POST['orderType'] ? $_POST['orderType'] : '';
    }

    /**
     * Função  setAgrupador
     * Método  usado para setar alguns atributos da classe (agrupador|dados).
     *
     * @access   public
     * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since    13/02/2009
     * @param    array $dado - Deve conter os agrupadores do relatório.
     * @param    array $dado1 - Deve conter os dados do relatório (no formato de dados do banco).
     * @tutorial $dado = array(2) {
    ["agrupador"]=>
    array(3) {
    [0]=>
    array(2) {
    ["campo"]=>
    string(6) "estado"
    ["label"]=>
    string(6) "Estado"
    }
    [1]=>
    array(2) {
    ["campo"]=>
    string(9) "municipio"
    ["label"]=>
    string(9) "Município"
    }
    [2]=>
    array(2) {
    ["campo"]=>
    string(4) "nome"
    ["label"]=>
    string(6) "Escola"
    }
    }
    ["agrupadoColuna"]=>
    array(3) {
    [0]=>
    string(5) "quant"
    [1]=>
    string(6) "classe"
    [2]=>
    string(5) "valor"
    }
    };
    $dado1 = array(2) {
    [0]=>
    array(6) {
    ["codigo"]=>
    string(10) "11025620  "
    ["nome"]=>
    string(172) "EEEF TANCREDO DE ALMEIDA NEVES"
    ["estado"]=>
    string(2) "RO"
    ["municipio"]=>
    string(21) "Alta Floresta D'Oeste"
    ["valor"]=>
    string(8) "20000.00"
    ["quant"]=>
    string(1) "1"
    }
    [1]=>
    array(6) {
    ["codigo"]=>
    string(10) "11025638  "
    ["nome"]=>
    string(165) "EEEFM PE EZEQUIEL RAMIN"
    ["estado"]=>
    string(2) "RO"
    ["municipio"]=>
    string(21) "Alta Floresta D'Oeste"
    ["valor"]=>
    string(8) "21500.00"
    ["quant"]=>
    string(1) "1"
    }
    }
     * @return void
     */
    function setAgrupador($dado = null, $dado1 = null){
        $this->agrupador = $dado;
        $this->dados	 = $dado1;
        $this->setArrAgp($dado['agrupador']);
    }

    /**
     * Função  setArrAgp
     * Método usado para setar alguns atributos da classe (arrAgp).
     *
     * @access private
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  27/07/2009
     * @param  array $dado - Deve conter TODOS os agrupadores do relatório.
     * @return void
     */
    private function setArrAgp( array $dado){
        $this->arrAgp = array();
        foreach($dado as $dado):
            array_push($this->arrAgp, $dado['campo']);
        endforeach;
    }

    /**
     * Função  setCtrlAgp
     * Método usado para setar alguns atributos da classe (ctrlAgp).
     *
     * @access private
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @param  array $dado - Deve conter os agrupadores do relatório.
     * @return void
     */
    private function setCtrlAgp($dado){
        $this->ctrlAgp = array();
        foreach($dado as $dado):
            array_push($this->ctrlAgp, $dado['campo']);
        endforeach;
    }

    /**
     * Função  setColuna
     * Método usado para setar atributo da classe (coluna).
     *
     * @access   public
     * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since    13/02/2009
     * @param    array $dado - Deve conter as colunas do relatório.
     * @tutorial $dado = array(2) {
    [0]=>
    array(5) {
    ["campo"]=>
    string(5) "quant"	==> Campo com o mesmo nome que vem da query (sql)
    ["label"]=>
    string(21) "Quantidade de Escolas"	==> Label indentificador do campo, será usado no título da coluna.
    ["title"]=>
    string(21) "Quantidade informada por ..."	==> Title utilizado quando posicionado o mouse em cima da coluna.
    ["blockAgp"]=>
    string(4) "nome"	==>> Pode ser passado um array.
    ["mostraNivel"] =>
    "nome"	==>> Pode ser passado um array. Com esse atributo setado com o nome do agrupador, o valor é apresentado
    ao lado do agrupador escolhigo, e não necessariamente no ultimo nivel.
    ["type"]=>
    string(7) "numeric"	==> Pode ser passado numeric|string se não for passado, assume o valor que for identificado.
    ["html"]=>
    string(101)	==> A coluna que eu queira que apareça mesclado ao HTML, fica assim: {campo} Ex.: <a href=\"#\">{quant}</a>
    ["php"]=>	==> Contém um array com parametros.
    array(6) {
    ["blockAgp"]=>    ==> impede que a expressão php seja usada no agrupador definido.
    array(1) {
    [0]=>
    string(5) "estuf"	==> array|string
    }
    ["html"]=>
    string(9) "<span>{quant} | {marcado}</span>" ==> "A coluna que se queira que apareça mesclado ao HTML (TODOS os campos que foram agrupados ao agrupador, serão disponibilizados),
    fica assim: {campo}
    Ex.: <a href=\"#\">{quant}</a>"
    ["expressao"]=>
    string(12) "{ate500} > 0"  ==> Expressão de retorno booleano
    ["var"]=>
    string(7) "marcado"	==> Variável onde será setado o valor, se true ou false.
    ["true"]=>
    string(1) "X"	==> Valor de true.
    ["false"]=>
    string(1) "-"	==> Valor de false.
    }
    }
    [1]=>
    array(3) {
    ["campo"]=>
    string(5) "valor"
    ["label"]=>
    string(9) "Valor Paf"
    ["type"]=>
    string(6) "string"
    }
    }
     * @return void
     */
    function setColuna($dado = null){
        $this->coluna = $dado;
    }

    function setColunaTop($dado = null){
        $this->coltop = $dado;
    }

    /**
     * Função  setMonstrarTolizadorNivel
     * Método usado para setar atributo da classe (monstrarTolizadorNivel).
     *
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @param  boolean $dado - Se "true" será feita a contagem de linhas e mostrado nos agrupadores (padrão:false).
     * @return void
     */
    function setMonstrarTolizadorNivel($dado){
        $this->monstrarTolizadorNivel = $dado;
    }

    /**
     * Função  setTotNivel
     * Método usado para setar atributo da classe (totNivel).
     *
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @param  boolean $dado - Se true será feito a somatória nos níveis do agrupador (padrão:false).
     * @return void
     */
    function setTotNivel($dado){
        $this->totNivel = $dado;
    }

    /**
     * Função  setBrasao
     * Método usado para setar atributo da classe (brasao).
     *
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @param  boolean $dado - Se "true" será incluído no relatório o brasão (padrão:false).
     * @return void
     */
    function setBrasao($dado){
        $this->brasao = $dado;
    }

    /**
     * Função  setOverFlowTable
     * Método usado para controlar o conteúdo da table permitindo barra de rolagem na vertical apenas no conteúdo.
     * O cabeçalho e o rodape estaram fixos
     *
     * @access public
     * @author ALEXANDRE DOURADO
     * @since  15/03/2010
     * @param  array $dado - array('height' => 'yyy')
     * @return void
     */
    function setOverFlowTableHeight($dado) {
        $this->overFlowTableHeight = $dado;
    }



    /**
     * Função  setEspandir
     * Método usado para setar atributo da classe (espandir).
     *
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  10/08/2009
     * @param  boolean $dado - Se "true" o relatório virá com os agrupadores espandidos, ou seja, abertos (padrão:false).
     * @return void
     */
    function setEspandir($dado){
        $this->espandir = (boolean) $dado;
    }

    /**
     * Função  setErro
     * Método usado capturar erros previstos e setar no atributo da classe (relatorio).
     *
     * @access private
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @param  array $dado - Mensagem de erro.
     * @return void
     */
    private function setErro($dado){
        $this->relatorio = $dado;
    }

    /**
     * Função  setAgpContador
     * Método usado para setar atributo da classe (apgContador).
     *
     * @param  array $dado - Deve conter os agrupadores do relatório.
     * @access private
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @return void
     */
    private function setAgpContador($dado){
        $dado = end($dado);
        $this->agpContador = $dado['campo'];
    }

    /**
     * Função  setTolizadorLinha
     * Método usado para setar atributo da classe (TolizadorLinha).
     *
     * @param  boolean $dado - Se "true" mostra no final um totalizador das linhas.(padrão:true).
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @return void
     */
    public function setTolizadorLinha($dado){
        $this->totalizadorLinha = $dado;
    }

    /**
     * Função  setTotalizador
     * Método usado para setar atributo da classe (totalizadorGeral).
     *
     * @param  boolean $dado - Se "true" mostra no final o total de registros.(padrão:true).
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @return void
     */
    public function setTotalizador($dado){
        $this->totalizadorGeral = $dado;
    }

    /**
     * Função  setTextoTotalizador
     * Método usado para setar atributo da classe (totalizadorTextoGeral), que define a msg de totais de registros (ultimo nível).
     *
     * @param  String $dado Texto que será mostrado na última linha, no total de registros. (padrão:"Total de registros:")
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  18/08/2010
     * @return void
     */
    public function setTextoTotalizador($dado = "Total de registros:"){
        $this->totalizadorTextoGeral = $dado;
    }



    /**
     * Função  setRelatorio
     * Método usado para setar atributo da classe (relatorio).
     * Este faz a montagem do relatório, gerenciando os métodos necessários para isso.
     *
     * @access private
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @return void
     */
    private function setRelatorio(){
        try {
            if ( empty($this->dados) && get_class($this->db) == "Banco" ){
                if ( !empty($this->ordenacao) && !empty($this->orderType) ){
                    $this->setOrder( $this->ordenacao, $this->orderType );
                }
                dbg($this->db->recuperaSql(1));
                $this->dados = $this->db->carregar();
            }

            if ( !is_array($this->dados) || !$this->dados[0]){
                $this->relatorio = '<table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">'.
                    '<TR style="background:#FFF; color:red;">'.
                    '<TD colspan="10" align="center">'.
                    'Não foram encontrados registros.'.
                    '</TD>'.
                    '</TR>'.
                    '</table>';
            }else{
                ?>
                <script type="text/javascript">
                    <?include($_SERVER['DOCUMENT_ROOT'] . "/includes/relatorio.class.js"); ?>
                </script>
                <?
                $rel .= '<script type="text/javascript">';
                $rel .= '	var d' . self::$indRel . ' = new Conteudo();';
                $rel .= '	d' . self::$indRel . '.conteudoCarregando();';
                $rel .= '</script>';

                $rel .= $this->montaCabecalho();
                if($this->overFlowTableHeight) $rel .= "<tbody style='overflow-y:auto;overflow-x:hidden;height:".$this->overFlowTableHeight.";'>";
                $rel .= $this->montaCorpo();
                if($this->overFlowTableHeight) $rel .= "</tbody>";
                $rel .= $this->montaRodape();

                $rel .= '<script type="text/javascript">' . $this->js . '</script>';
                $rel .= '<script type="text/javascript">';
                $rel .= '	d' . self::$indRel . '.conteudoCarregado();';
                $rel .= '</script>';
                $this->relatorio = $rel;
            }
        } catch (Exception $e) {
            $this->setErro("<b>" . $e->getMessage() . "\n");
        }
    }

    /**
     * Função  getRelatorioXls
     * Método usado para carregar o relatório no formato XLS (Excel), semelhante ao setRelatorio
     * porém utiliza métodos especificos para criar um arquivo XLS
     *
     * @access public
     * @author ALEXANDRE DOURADO
     * @since  16/03/2009
     */
    public function getRelatorioXls(){
        $this->xls = new GeraExcel();
        try {
            if ( !is_array($this->dados) || !$this->dados[0]){
                $this->xls->MontaConteudoString(0, 0, "Não foram encontrados registros.");
            }else{
                $this->montaCabecalhoXls();
                $this->montaCorpoXls();
                $this->montaRodapeXls();
            }
            $this->xls->GeraArquivo();
            exit;
        } catch (Exception $e) {
            $this->setErro("<b>" . $e->getMessage() . "\n");
        }
    }

    /**
     * Função  getRelatorio
     * Método usado para chamar o método setRelatorio().
     *
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @return HTML (relatório montado)
     */
    function getRelatorio(){
        $this->setRelatorio();
        return $this->relatorio;
    }

    /**
     * Função  getAgrupar
     * Método usado para retornar array de dados agrupados.
     *
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since  13/02/2009
     * @return HTML|ARRAY (caso não exista dados|array de dados agrupados)
     */
    function getAgrupar(){
        if ( !is_array($this->dados) || !$this->dados[0]){
            return  '<table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">'.
                '<TR style="background:#FFF; color:red;">'.
                '<TD colspan="10" align="center">'.
                'Não foram encontrados registros.'.
                '</TD>'.
                '</TR>'.
                '</table>';
        } else {
            return $this->agrupar();
        }
    }

    private function setOrder($ordenacao, $orderType){
        $this->db->setOrder( $ordenacao, 1 );
        $this->db->setTypeOrder( $orderType );
    }

    private function pegaColuna($colunaBusca){
        $arrColuna = (array) $this->coluna;
        while( $coluna = current($arrColuna) ){
            if ($colunaBusca == $coluna['campo']){
                return $coluna;
            }
            next($arrColuna);
        }
    }

    protected function agrupar(){

        if (!is_array($this->agrupador['agrupador']) || !$this->agrupador['agrupador'])
            throw new Exception('<B>Método:</B> agrupar<br><B>O atributo (agrupador), não foi setado corretamente.</b>');

        if (!is_array($this->dados) || !$this->dados)
            throw new Exception("<b>Classe: </b>" . get_class() ."<BR>" . '<B>Método:</B> agrupar<br><B>O atributo (dados), não foi setado corretamente.</b>');

        if (!$this->ctrlAgp && !is_array($this->ctrlAgp)){
            $this->setCtrlAgp($this->agrupador['agrupador']);
        }

        $agrupadores   = $this->ctrlAgp;
        $agrupaColuna  = is_array($this->agrupador['agrupadoColuna']) ? $this->agrupador['agrupadoColuna'] : array();
        $lista		   = $this->dados;
        $existeProximo = count( $agrupadores ) > 0;

        if ( $existeProximo == false ){
            return array();
        }
        $campo = array_shift( $agrupadores );
        $novo  = array();

        foreach ( $lista as $item ):

            $chave = $item[$campo];
            if ( array_key_exists( $chave, $novo ) == false ){
                foreach ($agrupaColuna as $agrupa):
                    $novo[$chave][$agrupa] = NULL;
                endforeach;
                $novo[$chave]['totalAgrupador'] = 0;
                $novo[$chave]['agrupador']  	= $campo;
                $novo[$chave]['sub_itens']  	= array();
            }
            $item[$agrupa] = trim($item[$agrupa]);
            foreach ($agrupaColuna as $agrupa){
                $arrColuna = (array) self::pegaColuna($agrupa);
                if ( (is_numeric($item[$agrupa]) || ($novo[$chave][$agrupa] && is_numeric($novo[$chave][$agrupa]))) &&  strtoupper( $arrColuna['type'] ) != strtoupper( 'string' ) ){
                    $novo[$chave][$agrupa] += $item[$agrupa];
                }else{
                    $novo[$chave][$agrupa] = $item[$agrupa];
                }

            }

            $novo[$chave]['totalAgrupador']++;

            if ( $existeProximo )
                array_push( $novo[$chave]["sub_itens"], $item );

        endforeach;

        if ( $existeProximo ):
            foreach ( $novo as $chave => $dados ){
                $this->dados     			  		   = $novo[$chave]["sub_itens"];
                $this->ctrlAgp 						   = $agrupadores ? $agrupadores : array();
                $novo[$chave]["sub_itens"] 	  		   = $this->agrupar();
            }
        endif;
        return $novo;
    }

    private function labelAgrupador(){
        $agp = $this->agrupador['agrupador'];

        $html = "<div style=\"font-weight:bold;\">";
        foreach($agp as $agp):
            $nivel++;

            if ( get_class($this->db) == "Banco" ){
                if ( $agp['campo'] == $this->ordenacao ){
                    $img      = '<img height="13" align="middle" width="11" src="../imagens/seta_ordem' . $this->orderType . '.gif"/>&nbsp;';
                    $newOrdem = ($this->orderType == 'ASC') ? 'DESC' : 'ASC';
                }else{
                    $img = '';
                    $newOrdem = 'ASC';
                }
                $title  = ' title="Clique para ordernar pelo agrupador" ';
                $evento = ' onclick="setOrdem(\'' . $agp['campo'] . '\', \'' . $newOrdem . '\');"  onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#CCCCCC\';" ';
            }else{
                $title  = '';
                $evento = '';
                $img 	= '';

            }

            $html .= $nivel == 1 ?
                "<div {$title}{$evento}>{$img}{$agp['label']}</div>"
                :
                "<div style=\"padding-left:".(20*($nivel-1))."px;\">
										  		<img src=\"../imagens/seta_filho.gif\" align=\"absmiddle\"{$title}{$evento}/> {$img}{$agp['label']} 
								  </div>";
        endforeach;
        $html .= "</div>";

        return $html;
    }

    /**
     * Função  labelAgrupadorXls
     * Método usado para imprimir os agrupadores na celula (0,0) do arquivo XLS, semelhante ao labelAgrupador
     * porém não imprime código HTML
     * @access private
     * @author ALEXANDRE DOURADO
     * @since  16/03/2009
     */
    private function labelAgrupadorXls(){
        $agp = $this->agrupador['agrupador'];

        foreach($agp as $agp):
            $nivel++;
            $code[] = $agp['label'];
        endforeach;

        return implode(" | ", $code);
    }


    private function montaBrasao(){
        return '<center>'.monta_cabecalho_relatorio( '100' ).'</center>';
    }

    private function montaCabecalho(){
        $coluna = (array) $this->coluna;
        $coltop = (array) $this->coltop;

        if ( get_class($this->db) == "Banco" ){
            $cabecalho .= "<form name='formOrdenacao' action='' method='POST'>
								<input name='ordenacao' id='ordenacao'>
								<input name='orderType' id='orderType'>
						   </form>
						   <script>
						   	function setOrdem(campo, order){
								d = document;
								
								d.getElementById('ordenacao').value = campo;
								d.getElementById('orderType').value = order;
								d.formOrdenacao.submit();
							}	
						   </script>";
        }

        $cabecalho .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";

        if ($this->brasao){
            $cabecalho .= "<tr>" .
                "<td colspan=\"100\">" .
                $this->montaBrasao() .
                "</td>" .
                "</tr>";
        }

        if($this->overFlowTableHeight) $cabecalho .= "<thead>";

        $cabecalho .= "	<TR style=\"background:#D9D9D9;\">";

        /*
         * O Coltop refere-se ao cabeçalho com um nível acima agrupado. Ele deve ser passado no arquivo principal indicando o
         * seu label (nome que irá ser apresentado na tabela) e o seu colspan da seguinte forma:
         *
         * $coltop = array( 0 => array( "label" => "label1", "colspan" => "2" ),
                            1 => array( "label" => "label2", "colspan" => "2" ),
                            2 => array( "label" => "label3", "colspan" => "2" ),
                            3 => array( "label" => "label4", "colspan" => "2" ),
                );
                $r->setColunaTop($coltop);

        * Dessa forma o componente irá montar esse cabeçalho acima do preparado no arquivo principal.
        */

        if( $coltop ){
            $cabecalho .= "<TD rowspan='2' >
							 ".$this->labelAgrupador()."
						   </TD>";
            foreach ( $coltop as $top ){
                $cabecalho .= "<TD align=\"center\" valign=\"top\" colspan=\"".$top['colspan']."\" style=\"font-weight:bold;\">{$top['label']}</TD>";
            }
            $cabecalho .= "</TR><TR style=\"background:#D9D9D9;\">";
        } else {
            $cabecalho .= "<TD>
							 ".$this->labelAgrupador()."
						   </TD>";
        }
        foreach ($coluna as $col):

            if ( get_class($this->db) == "Banco" ){
                if ( $col['campo'] == $this->ordenacao ){
                    $img      = '<img height="13" align="middle" width="11" src="../imagens/seta_ordem' . $this->orderType . '.gif"/>&nbsp;';
                    $newOrdem = ($this->orderType == 'ASC') ? 'DESC' : 'ASC';
                }else{
                    $img = '';
                    $newOrdem = 'ASC';
                }
                $title  = ' title="Clique para ordernar pelo campo" ';
                $evento = ' onclick="setOrdem(\'' . $col['campo'] . '\', \'' . $newOrdem . '\');"  onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#CCCCCC\';" ';
            }else{
                $title  = '';
                $evento = '';
                $img 	= '';

            }

            $cabecalho .= "<TD align=\"center\" valign=\"top\" style=\"font-weight:bold;\"{$title}{$evento}>{$img}{$col['label']}</TD>";
        endforeach;

        if($this->overFlowTableHeight) $cabecalho .= "<td width=5>&nbsp;</td>";

        $cabecalho .= "</TR>";
        if($this->overFlowTableHeight) $cabecalho .= "</thead>";


        return $cabecalho;
    }

    /**
     * Função  montaCabecalhoXls
     * Método usado para criar o cabeçalho na linha 0 do arquivo XLS, semelhante ao montaCabecalho
     * porém não imprime código HTML
     * @access private
     * @author ALEXANDRE DOURADO
     * @since  16/03/2009
     */
    private function montaCabecalhoXls(){
        $coluna = (array) $this->coluna;
        $this->xls->MontaConteudoString(0, 0, $this->labelAgrupadorXls());
        $coln=1;
        foreach ($coluna as $col):
            $this->xls->MontaConteudoString(0, $coln, $col['label']);
            $coln++;
        endforeach;
    }

    private function montaCorpo($lista=null, $profundidade=0){
        static $associaAgp = array();

        $lista 		  = $lista ? $lista : $this->agrupar();
        $coluna 	  = (array) $this->coluna;
        $agrupaColuna = $this->agrupador['agrupadoColuna'] ? $this->agrupador['agrupadoColuna'] : array();

        if ($profundidade == 0){
            $this->setAgpContador($this->agrupador['agrupador']);
        }
        $endAgp	      = $this->agpContador;

        if ( count( $lista ) == 0 ){
            return;
        }
//		$contCor = 0;
        foreach ( $lista as $chave => $dados ):
            $contId++;
            $a = 0;
            $arrDataValue = array();

            foreach ($agrupaColuna as $agrupa):
                foreach($coluna as $col){
                    if ($agrupa == $col['campo']){
                        $type = $col['type'];
                        continue;
                    }
                }
                $arrDataValue[$agrupa] = (string) $dados[$agrupa];
                $indHTML[$a] = '{' . $agrupa . '}';
                $valHTML[$a] = ( ((is_numeric($dados[$agrupa])  || $type == 'numeric') && $type != 'string') ? ($type == 'numeric' ? number_format($dados[$agrupa],0,',','.') : number_format($dados[$agrupa],2,',','.')) : $dados[$agrupa]);
                $valExpressaoHTML[$a] = $dados[$agrupa];
                $a++;
            endforeach;
            unset($col);

            $agrupador 		   = (string) $dados["agrupador"];

            if ($agrupador == $endAgp){
                $this->totalizador['total']++;
            }
            elseif( $this->monstrarTolizadorNivel ){


                $subitens = is_array( $dados["sub_itens"] ) ? count( $dados["sub_itens"] ) : 0;
                $total = ( $this->monstrarTolizadorNivel === 2 ) ? $subitens : $dados["totalAgrupador"];
                $totalPorAgrupador = "&nbsp;&nbsp;&nbsp;($total)";
            }

            $bgColor = array(
                "#DFDFDF",
                "#E5E5E5",
                "#E9E9E9",
                "#EEEEEE",
                "#F1F1F1",
                "#F7F7F7",
                "#FAFAFA",
            );


            $bg = $bgColor[$profundidade];

            $associaAgp[$profundidade]['id'] 	= $agrupador . '_' . $contId;
            $associaAgp[$profundidade]['nivel'] = $contId;
            $contP 					   = 0;
            $id	   					   = '';
            $idPai					   = array();
            $idPaiTx				   = array();

            while ($contP < $profundidade+1){
                $id .= $associaAgp[$contP]['id'] . '_' . self::$indRel;
                if ($contP < $profundidade || $profundidade == 0){
                    $idPai[]   = '{\'id\':\'' . $id . '\', \'nivel\':\'' . $associaAgp[$contP]['nivel'] . '\'}';
                    $idPaiTx[] = $id;
                }
                $contP++;
            }

            $idPai = ' var idPai = new Array(); ' .
                'idPai = [' . implode(",", $idPai) . ']; ';
            $idPaiTx = implode(":", $idPaiTx);

            if ($this->espandir){
                $espandido = 'S';
                $imgEsp    = 'menos.gif';
                $title	   = 'Clique para minimizar';
                $style	   = '';
            }/*elseif ($profundidade == 0){
				$espandido = 'S';
				$imgEsp    = 'mais.gif';
				$title	   = 'Clique para expandir';
				$style	   = '';
			}*/else{
                $espandido = 'N';
                $imgEsp    = 'mais.gif';
                $title	   = 'Clique para expandir';
                $style	   =  $profundidade == 0 ? '' : 'display: none;';
            }


            $this->js .= $idPai . "d" . self::$indRel . ".carregaElemento(idPai, '$id', '$espandido', $profundidade);";
            $icone = $agrupador != $endAgp ? '<img id="' . $id . '_img" src="/imagens/' . $imgEsp . '" title="' . $title . '" style="cursor: pointer;" onclick="d' . self::$indRel . '.conteudoCarregando(\'' . $id . '_img\'); window.setTimeout(&#34;d' . self::$indRel . '.controle(&#39;' . $id . '&#39;, &#39;' . $idPaiTx . '&#39;, &#39;' . $id . '_img&#39;)&#34;, 2); ">' : '';

            $html .= '<tr id="' . $id . '" style="' . $style . '" bgcolor="' . $bg . '" onmouseout="this.bgColor=\'' . $bg . '\';" onmouseover="this.bgColor=\'#ffffcc\';">';
            $html .= '<td style="padding-left:' . ($profundidade * 20) . 'px; font-size:10px;">'.
                (($profundidade > 0)
                    ?
                    '<img src="../imagens/seta_filho.gif" align="absmiddle"/>'
                    :
                    '&nbsp;').
                $icone . '&nbsp;<b>' . $chave . '</b>'.
                '<B style="color:rgb(0, 102, 204);">'.$totalPorAgrupador.'</B>'.
                '</td>';

            $i = 0;


            foreach ($coluna as $col){
                $colunaValue = $arrDataValue[$col['campo']];
                $cl    = (is_numeric($colunaValue) && $col['type'] != 'string') ? 'rgb(0, 102, 204);' : '#000000;';
                $align = ( (is_numeric($colunaValue) && $col['type'] != 'string') ) ? 'right' : 'center';
                if ($profundidade == 0 && is_numeric($colunaValue)):
                    $this->totalizador[$i] += $colunaValue;

                endif;

                if ( ( in_array($agrupador, (array) $col['blockAgp']) ) ||
                    ( !$this->totNivel && $agrupador != $endAgp ) ||
                    ( $this->totNivel && $agrupador != $endAgp && !is_numeric($colunaValue) ) ||
                    ( $agrupador != $endAgp && $col['type'] == 'string' ) &&
                    (  empty($col['mostraNivel']) || !in_array($agrupador, (array) $col['mostraNivel']) ) ) {

                    $colunaValue = '-';
                }else{
                    if ( !empty($col['php']) && !in_array($agrupador, (array) $col['php']['blockAgp']) ){
                        // Caso tenha um html, no array da expressão php, substituirá o padrão, caso exista.
                        $col['html'] = !empty($col['php']['html']) ? $col['php']['html'] : $col['html'];

                        $col['php']['expressao'] = '$boolean = ' . str_replace($indHTML, $valExpressaoHTML, $col['php']['expressao']) . ';';
//						$col['php']['expressao'] = '$boolean = ' . str_replace($indHTML, $valHTML, $col['php']['expressao']) . ';';

                        if(!strstr($col['php']['expressao'],"&&  > 0);")){
                            eval($col['php']['expressao']);
                        }

                        $indHTML['php'] = "{" . $col['php']['var'] . "}";

                        $col['php']['true']  = str_replace($indHTML, $valExpressaoHTML, $col['php']['true']);
                        $col['php']['false'] = str_replace($indHTML, $valExpressaoHTML, $col['php']['false']);
                        if ($col['php']['type'] == 'numeric'){
                            if ( $boolean )
                                $returnTrue  = eval('return (' . $col['php']['true'] . ');');
                            else
                                $returnFalse = eval('return (' . $col['php']['false'] . ');');
                        }

//						$valHTML['php'] = isset($return) ? $return : $col['php']['true'];

                        if ( $boolean ){
                            $valHTML['php'] = isset($returnTrue) ? $returnTrue : $col['php']['true'];
                        }else{
                            $valHTML['php'] = isset($returnFalse) ? $returnFalse : $col['php']['false'];
                        }
                        unset($boolean, $returnFalse, $returnTrue);
                    }

                    if (!$col['html']){
                        $colunaValue = (is_numeric($colunaValue) || $col['type'] == 'numeric') && $col['type'] != 'string' ? ($col['type'] == 'numeric' ? number_format($colunaValue,0,',','.') : number_format($colunaValue,2,',','.')) : $colunaValue;
                    }else{
                        $colunaValue = str_replace($indHTML, $valHTML, $col['html']);
                    }
                }
                $tit = $col['title'] ? $col['title'] : $col['label'];
                $html .= '<td align="'.$align.'" style="color:'.$cl.';" title="'.$tit.'" >' . $colunaValue . '</td>';
                $i++;
            }

            if($this->overFlowTableHeight) $html .= "<td>&nbsp;</td>";

            $html .= '</tr>';

            $html .= $this->montaCorpo( $dados["sub_itens"], $profundidade + 1 );
//			$contCor++;
        endforeach;

        return $html;
    }

    /**
     * Função  montaCorpoXls
     * Método usado para criar toda estrutura do arquivo XLS, semelhante ao montaCorpo
     * porém não imprime código HTML
     * @access private
     * @author ALEXANDRE DOURADO
     * @since  16/03/2009
     */
    private function montaCorpoXls($lista=null, $profundidade=0){
        $lista 		  = $lista ? $lista : $this->agrupar();
        $coluna 	  = (array) $this->coluna;
        $agrupaColuna = $this->agrupador['agrupadoColuna'] ? $this->agrupador['agrupadoColuna'] : array();

        if ($profundidade == 0){
            $this->setAgpContador($this->agrupador['agrupador']);
        }
        $endAgp	      = $this->agpContador;

        if ( count( $lista ) == 0 ){
            return;
        }
        foreach ( $lista as $chave => $dados ):
            $a=0;
            $arrDataValue = array();

            foreach ($agrupaColuna as $agrupa):
                $arrDataValue[$agrupa]   = (string) $dados[$agrupa];
                $indHTML[$a] = '{'.$agrupa.'}';
                $valHTML[$a] = (is_numeric($dados[$agrupa]) ? number_format($dados[$agrupa],2,',','.') : $dados[$agrupa]);
                $valHTML[$a] = (is_numeric($dados[$agrupa]) ? number_format($dados[$agrupa],2,',','.') : $dados[$agrupa]);
                $a++;
            endforeach;
            $agrupador 		   = (string) $dados["agrupador"];

            if ($agrupador == $endAgp){
                $this->totalizador['total']++;
            }elseif ($this->monstrarTolizadorNivel){
                $totalPorAgrupador = $dados["totalAgrupador"];
            }

            $this->xls->MontaConteudoString($this->linexls, 0, $chave);
            $i = 0;
            foreach ($coluna as $col){
                $colunaValue = $arrDataValue[$col['campo']];

                if ($profundidade == 0 && is_numeric($colunaValue)):
                    $this->totalizador[$i] += $colunaValue;

                endif;

                if ( ( in_array($agrupador, (array) $col['blockAgp']) ) ||
                    ( !$this->totNivel && $agrupador != $endAgp ) ||
                    ( $this->totNivel && $agrupador != $endAgp && !is_numeric($colunaValue) ) ||
                    ( $agrupador != $endAgp && $col['type'] == 'string' ) ) {

                    $colunaValue = '-';
                }else{
                    $colunaValue = is_numeric($colunaValue) && $col['type'] != 'numeric' && $col['type'] != 'string' ? number_format($colunaValue,2,',','.') : $colunaValue;
                    $colunaValue = !$col['html'] ? $colunaValue : str_replace($indHTML, $valHTML, $col['html']);
                }
                $this->xls->MontaConteudoString($this->linexls, $i+1, $colunaValue);
                $i++;
            }
            $this->linexls++;

            $this->montaCorpoXls( $dados["sub_itens"], $profundidade + 1 );
        endforeach;

    }

    private function montaRodape(){
        $totCol		 = count($this->coluna);
        $col		 = (array) $this->coluna;
        $totalizador = $this->totalizador;

        if($this->overFlowTableHeight) $html .= "<tfoot>";

        if ($this->totalizadorLinha){
            $html .= '<tr style="background:#DFDFDF;">'.
                '<td align="left">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>Totais:</b></td>';

            // Carrega os elementos da coluna para ser usado no HTML.
            $a = 0;
            foreach ($col as $colv):
                $type 				  = $colv['type'];
                $indHTML[$a] 		  = '{' . $colv['campo'] . '}';
                $valHTML[$a] 		  = ( ((is_numeric($totalizador[$a]) || $type == 'numeric') && $type != 'string') ? ($type == 'numeric' ? number_format($totalizador[$a],0,',','.') : number_format($totalizador[$a],2,',','.')) : $totalizador[$a]);
                $valExpressaoHTML[$a] = $totalizador[$a];
                $a++;
            endforeach;
            for ($i=0; $i<$totCol; $i++){

                if ( !empty($col[$i]['php'])){
                    // Caso tenha um html, no array da expressão php, substituirá o padrão, caso exista.
                    $col[$i]['html'] = !empty($col[$i]['php']['html']) ? $col[$i]['php']['html'] : $col[$i]['html'];

                    $col[$i]['php']['expressao'] = '$boolean = ' . str_replace($indHTML, $valExpressaoHTML, $col[$i]['php']['expressao']) . ';';
//					$col[$i]['php']['expressao'] = '$boolean = ' . str_replace($indHTML, $valHTML, $col[$i]['php']['expressao']) . ';';
                    eval($col[$i]['php']['expressao']);

                    $indHTML['php'] = "{" . $col[$i]['php']['var'] . "}";

                    $col[$i]['php']['true']  = str_replace($indHTML, $valExpressaoHTML, $col[$i]['php']['true']);
                    $col[$i]['php']['false'] = str_replace($indHTML, $valExpressaoHTML, $col[$i]['php']['false']);

                    if ($col[$i]['php']['type'] == 'numeric'){
                        if ( $boolean )
                            $returnTrue  = eval('return (' . $col[$i]['php']['true'] . ');');
                        else
                            $returnFalse = eval('return (' . $col[$i]['php']['false'] . ');');
                    }


                    if ( $boolean ){
                        $valHTML['php'] = isset($returnTrue) ? $returnTrue : $col[$i]['php']['true'];
                    }else{
                        $valHTML['php'] = isset($returnFalse) ? $returnFalse : $col[$i]['php']['false'];
                    }
                    unset($boolean, $returnFalse, $returnTrue);
                }

                if (is_numeric($totalizador[$i]) && $col[$i]['type'] != 'string'):
                    if (!$col[$i]['html']){
                        $totalizador[$i] = ($col[$i]['type'] == 'numeric' ? number_format($totalizador[$i],0,',','.') : number_format($totalizador[$i],2,',','.'));
                    }else{
                        $totalizador[$i] = str_replace($indHTML, $valHTML, $col[$i]['html']);
                    }
                else:
                    $totalizador[$i] = '-';
                endif;

                $html .= "<TD align='right'' style='color: rgb(0, 102, 204); font-weight:bold;'>".$totalizador[$i]."</TD>";

            }

            if($this->overFlowTableHeight) $html .= "<td>&nbsp;</td>";

            $html .= '</tr>';
        }

        if ($this->totalizadorGeral){

            $html .= '<TR style="background:#FFFFFF;">
						<TD colspan="100" align="right" style="font-weight:bold; font-size:9px; border-top:2px solid black; border-bottom:2px solid black;"><div style="float:left; font-size:11px;">' . $this->totalizadorTextoGeral . ' ' . $totalizador['total'].'</div></TD>
					  </tr>';
        }

        if($this->overFlowTableHeight) $html .= "</tfoot>";

        $html .= '</table>';

        return $html;
    }

    /**
     * Função  montaRodapeXls
     * Método usado para criar o rodape do arquivo XLS, semelhante ao montaRodape
     * porém não imprime código HTML
     * @access private
     * @author ALEXANDRE DOURADO
     * @since  16/03/2009
     */
    private function montaRodapeXls(){
        $totCol		 = count($this->coluna);
        $col		 = $this->coluna;
        $totalizador = $this->totalizador;
        if ($this->totalizadorLinha){
            $this->xls->MontaConteudoString($this->linexls, 0, "Totais:");
            for ($i=0; $i<$totCol; $i++){
                if (is_numeric($totalizador[$i]) && $col[$i]['type'] != 'numeric' && $col[$i]['type'] != 'string'):
                    $totalizador[$i] = number_format($totalizador[$i],2,',','.');
                elseif (!is_numeric($totalizador[$i]) || $col[$i]['type'] == 'string'):
                    $totalizador[$i] = '-';
                endif;
                $this->xls->MontaConteudoString($this->linexls, $i+1, $totalizador[$i]);
            }
            $this->linexls++;
        }
        if ($this->totalizadorGeral){
            $this->xls->MontaConteudoString($this->linexls, 0, "Total de registros: ");
            $this->xls->MontaConteudoString($this->linexls, 1, $totalizador['total']);
        }
    }

}

class montaRelatorioExcel extends montaRelatorio{
    private	$dadosAgrupado;
    private $lista;
    private $desagrupado;

    /**
     * Função __construct
     * Método construtor do PHP. Carrega alguns atributos da classe.
     * @access public
     * @author FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
     * @since 13/02/2009
     */
    function __construct(){
        $this->desagrupado = array();
    }

    public function setAgrupador($dado=null, $dado1=null){
        montaRelatorio::setAgrupador($dado, $dado1);
    }

    private function desagrupaAgrupado($lista=null){
        $lista = $lista ? $lista : montaRelatorio::agrupar();
        $novo  = array();

        if (!is_array($lista))
            return;

        foreach($lista as $k => $agp):
            $novo['Agrupadores'] = $k;
            foreach ($agp as $ch => $val){
                if ($ch == "sub_itens"){
                    $novaLista = $val;
                }elseif ($ch != "totalAgrupador" && $ch != "agrupador"){
                    $novo[$ch] = $val;
                }
            }
            array_push($this->desagrupado, $novo);
            $this->desagrupaAgrupado($novaLista);
        endforeach;

        //return $this->desagrupado;
    }

    public function getDesagrupado(){
        try{
            $this->desagrupaAgrupado();
            return $this->desagrupado;
        } catch (Exception $e) {
            die($e->getMessage() . "\n");
        }
    }
}
?>