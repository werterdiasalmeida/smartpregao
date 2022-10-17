<?php

class Listam{

	const TYPENUMERIC 		= 'numeric'; 		// 2.345,34 | 0
	const TYPENUMERICNATIVO = 'numericnativo';  // 2345.34
	const TYPEMONEY 		= 'money'; 			// 2.345,34 | 0,00
	const TYPESTRING 		= 'string'; 		// texto comum
	const TYPEDATE	 		= 'date'; 			// 12-10-2010
	const TYPETIME	 		= 'time'; 			// 12:59

	const ORDERASC			= 'asc';
	const ORDERDESC			= 'desc';

	private $div;
	private $table;
	private $row;
	private $cell;
	private $rowspan;
	private $mapRow;
	private $arCabecalho;
	private $exibeCabecalho;
	private $arCorpo;
	private $arCorpoTotal;
	private $acao;
	private $classTr;
	private $arHTML;
	private $config;
	private $totalColuna;
	private static $numlista;
	private $trRodape;
//	private $order;
//	private $itemOrder;
	/**
	 * Atributos usados somente quando usar paginação ajax
	 */
	private $paginacao;

//	public function __construct(Array $arParam = null, $arDefinicaoPaginacao = array()){
	public function __construct(Array $arParam = null){
		self::$numlista++;
		self::open( $arParam );
		$this->rowspanTotal = 1;

		$this->ordemAtivo = true;
		// Define se a listagem conterá um totalizador de registros
		$this->config['totalRegistro'] = $arParam['totalRegistro'] ? $arParam['totalRegistro'] : false;
		// Define se a listagem conterá um totalizador das linhas
		$this->config['totalLinha'] = $arParam['totalLinha'] ? $arParam['totalLinha'] : false;
		// Define se a listagem conterá uma subLinha para cada linha
		$this->config['subLinha'] = $arParam['subLinha'] ? $arParam['subLinha'] : false;
		//Será utilizado para o cálculo do total de colunas
		$this->acaoAltTotalColuna = 0;
	}

//	public function setPaginacao($numPorPag, $numIndice, $numTotal = false){
//		$this->paginacao['numPag'] = $numPorPag;
//		$this->paginacao['numInd'] = $numIndice;
//		$this->paginacao['numTot'] = $numTotal;
//
//		return $this;
//	}

	public function setCabecalho(Array $arCabecalho = null){
		$this->arCabecalho = $arCabecalho;
		return $this;
	}

	public function setAcao( $acao, $localInicio = true ){
		$this->acao['localinicio'] = $localInicio;
		$this->acao['acao']		   = $acao;

		return $this;
	}

	public function setCorpo($arDados = null, Array $arConfig = null){
		if ($arDados['pagina']){
			$this->arCorpo['dados']  		= $arDados['dados'];
			$this->arCorpo['total']  		= $arDados['total'];
			$this->arCorpo['pagina'] 		= $arDados['pagina'];
			$this->arCorpo['totalPagina'] 	= $arDados['totalPagina'];
			$this->arCorpo['identificador'] = $arDados['identificador'];
			$this->arCorpo['ordem'] 		= $arDados['ordem'];
			$this->arCorpo['numRegistroInicialPagina'] = $arDados['numRegistroInicialPagina'];
			$this->arCorpo['numRegistroFinalPagina'] = $arDados['numRegistroFinalPagina'];
			self::javaScript();
		}else{
			$this->arCorpo['dados']  = $arDados;
		}
		$this->arCorpo['config'] = $arConfig;

		return $this;
	}

//	public function setItemOrdem(Array $itens){
//		$this->itemOrder = $itens;
//		return $this;
//	}

	public function setOrdemAtivo($ativo){
		$this->ordemAtivo = $ativo ? true : false;
	}

	public function setNotCabecalho(){
		$this->exibeCabecalho = false;
		return $this;
	}
//	public function show($arConfigPaginacao = array()){
	public function show(){
//		self::carregaDadosParam();
//		self::montaCabecalhoPaginacao();
		if ( is_array($this->arCorpo['dados']) && $this->arCorpo['dados'][0] ){
			self::montaCabecalho();
		}
//		$this->arConfigPaginacao = $arConfigPaginacao;
		self::montaCorpo();
//		self::montaCabecalhoPaginacao();
		$this->div->show();

		$css = <<<CSS
            <style type='text/css'>
                table thead th{
                    text-align: center !important;
                }
            </style>
CSS;
        echo $css;
	}

	public function imprimir($gerarImpressaoAoAbrir = true){
		if ( is_array($this->arCorpo['dados']) && $this->arCorpo['dados'][0] ){
			self::montaCabecalhoImpressao();
		}
		self::montaCorpoImpressao();

		echo monta_cabecalho_relatorio(100);
		echo "<br />";

		$this->div->show();
		echo '<div id="divBtnImprimir" style="text-align: center;">
			  	<br />
				<button onclick="imprimirLista();">Imprimir</button>
			  </div>
			  <style type="text/css">
				  @media print {
					body { -webkit-print-color-adjust: exact; }
				    thead { display: table-header-group; }
				    tfooter { display: table-footer-group; }
				    #divBtnImprimir { display: none; }
				  }

				  table.listagem{
				    border-collapse: collapse;
					border: 1px solid #00000;
	              }

				  table.listagem tr td, table.listagem tr th{
			  		font: 12px Arial,verdana;
					border: 1px solid #000000;
				  }
			  </style>
			  <script type="text/javascript">
				  function imprimirLista(){
					window.print();
				  }

				  '.($gerarImpressaoAoAbrir ? 'imprimirLista()' : '').';
			  </script>';
	}

//	private function carregaDadosParam(){
////		dbg(self::$numlista, Http::get('numlista'), self::$numlista == Http::get('numlista'));
//		if ( self::$numlista == $_POST['numlista'] ){
//			if ( $this->paginacao != false ){
//				$this->paginacao['numlista'] = $_POST['numlista'];
//				$this->paginacao['pagAtual'] = $_POST['pag'] > 0 ? $_POST['pag'] : 1;
//			}
//			$order = $_POST['order'];
//			if ( is_numeric($order) ){
//				$this->order = $order;
//			}
//
//		}elseif ($this->paginacao != false){
//			$this->paginacao['numlista'] = self::$numlista;
//			$this->paginacao['pagAtual'] = 1;
//		}
//	}

//	private function montaCabecalhoPaginacao(){
//		if ( $this->paginacao != false ){
//			$totRegistro = count($this->arCorpo['dados']);
//			$numPag		 = $this->paginacao['numPag'];
//			$numInd		 = $this->paginacao['numInd'];
//			$numTot		 = $this->paginacao['numTot'] ? $this->paginacao['numTot'] : $totRegistro;
//
//			$numIndTot = ceil( $numTot/$numPag );
//			$numInd	   = $numIndTot > $numInd ? $numInd : $numIndTot;
//			if ( $numIndTot > 1 || $this->itemOrder ){
//				$url = $_SERVER['REQUEST_URI'];
//				$url .= ((strrpos($url, "/") + 1) == strlen($url)) ? "" : "/";
//
//				foreach ($_POST as $k => $val){
//					if ($val)
//						$url  .= $k . "/" . $val . "/";
//				}
//
//				$pos	   = strpos($url, "numlista") !== false  ? strpos($url, "numlista") : strlen($url);
//				$urlNativa = substr($url, 0, $pos);
//				$url 	   = $urlNativa . "numlista/" . self::$numlista;
//
//				$pagAtual					 = $this->paginacao['pagAtual'];
//				$pagAtual					 = $pagAtual < $numIndTot ? $pagAtual : $numIndTot;
//				$this->paginacao['pagAtual'] = $pagAtual;
//
//				$indMeio = floor($numInd/2) - 1;
//				$iniLoop = (($pagAtual - $indMeio) <= 0 ? 1 : ($pagAtual - $indMeio));
//				$iniLoop = (($iniLoop + $numInd) <= $numIndTot ) ? $iniLoop : ($numIndTot - $numInd + 1);
//				$fimLoop = $numIndTot > $numInd ? $numInd : $numIndTot;
//				$fimLoop += $iniLoop - 1;
//				for($i=$iniLoop; $i <= $fimLoop; $i++){
//					$dir = $url . "/pag/" . $i;
//					$txt = $pagAtual == $i ? "<u>{$i}</u>" : $i;
//					$txtInd[] = "<span onclick=\"window.location='{$dir}'\" style='cursor:pointer;'>" . $txt . "</span>";
//				}
//
//				$dirPri = $url . "/pag/1";
//				$dirUlt = $url . "/pag/" . $numIndTot;
//				$txtInd = "<span onclick=\"window.location='{$dirPri}'\" style='cursor:pointer;'>primeira</span> "
//						   . implode(" | ",$txtInd) .
//						  " <span onclick=\"window.location='{$dirUlt}'\" style='cursor:pointer;'>última</span>";
//				$colspan	   = count($this->arCorpo['dados'][0]) + 1;
//				$cell 	 	   = $this->table->addRow()->addCell( ($fimLoop > 1 ? self::montaOrdenacao($urlNativa) . $txtInd : self::montaOrdenacao($urlNativa)));
//				$cell->nowrap  = 'nowrap';
//				$cell->align   = 'right';
//				$cell->colspan = $colspan;
//			}
//		}elseif (is_array($this->itemOrder)){
//				$url = $_SERVER['REQUEST_URI'];
//				$url .= ((strrpos($url, "/") + 1) == strlen($url)) ? "" : "/";
//
//				foreach ($_POST as $k => $val){
//					if ($val)
//						$url  .= $k . "/" . $val . "/";
//				}
//
//				$pos	   = strpos($url, "numlista") !== false  ? strpos($url, "numlista") : strlen($url);
//				$urlNativa = substr($url, 0, $pos);
//
//				$colspan	   = count($this->arCorpo['dados'][0]) + 1;
//				$cell 	 	   = $this->table->addRow()->addCell( self::montaOrdenacao($urlNativa) );
//				$cell->nowrap  = 'nowrap';
//				$cell->align   = 'right';
//				$cell->colspan = $colspan;
//		}
//	}

//	private function montaOrdenacao($url){
//		if (is_array($this->itemOrder)){
//			$pos   = strpos($url, "order") !== false  ? strpos($url, "order") : strlen($url);
//			$url   = substr($url, 0, $pos) . "{ordemlista}numlista/" . self::$numlista;
//
//			$txt = '<b style="font-size:10px;">Ordenar por: </b>';
//			$i = 0;
//			foreach ($this->itemOrder as $item){
//				$redir = str_replace("{ordemlista}", "order/$i/", $url);
//				$arItem[] = "<a href='{$redir}' class='{$item['class']}'>{$item['nome']}</a>";
//				$i++;
//			}
//			$txt .= implode(" | ", $arItem);
//
//			return '<span style="float:left;">'.$txt.'</span>';
//		}
//	}

	private function open(Array $arParam = null){
		$this->paginacao = false;

		$this->table = new TTable();
		$this->table->style		  = $arParam['style'];
		$this->table->class		  = "table table-striped table-bordered table-hover";

		if ( is_array( $arParam ) ){
			foreach ( $arParam as $att => $val ){
				$this->table->$att = $val;
			}
		}
		$div = new TElement('div');
		$div->name = 'listagem_' . self::$numlista;
        $div->style = 'padding-top: 20px; overflow-x:auto;';
		$div->add( $this->table );
		$this->div = $div;
	}

	private function montaCabecalho(Array $arCabecalho = null){
		$arCabecalho = !is_null( $arCabecalho ) ? $arCabecalho : $this->arCabecalho;

		if ( !is_null($arCabecalho) ){
			if( is_null($this->cell) ){
				self::atribuiNivelCabecalho( $arCabecalho, 1 );
			}
            $this->totalColuna = 1;

			foreach( $arCabecalho as $indice => $cabecalho ){
				if ( is_array($cabecalho) ){
					$txtCabecalho = $indice;
				}else{
					$txtCabecalho = $cabecalho;
				}
				$txtCabecalho 								  = is_array($cabecalho) ? $indice : $cabecalho;
                $this->row[$this->cell[$txtCabecalho]['row']] = (($this->row[$this->cell[$txtCabecalho]['row']] instanceof TElement)
                    ?
                    $this->row[$this->cell[$txtCabecalho]['row']]
                    :
                    $this->table->addHeadRow(true));
				$row										  = $this->row[$this->cell[$txtCabecalho]['row']];
				$row->class   = 'head';

				$this->cell[$txtCabecalho]['obj'] = $row->addHead( $txtCabecalho );
				$this->cell[$txtCabecalho]['obj']->align  = 'center';
				$this->cell[$txtCabecalho]['obj']->valign = 'middle';
//				$this->cell[$txtCabecalho]['obj']->class  = 'title text-center';
				// $this->cell[$txtCabecalho]['obj']->style  = 'border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255); font-weight: bold;';

				if ( is_int($this->cell[$txtCabecalho]['colspan']) ){
					$this->cell[$txtCabecalho]['obj']->colspan = $this->cell[$txtCabecalho]['colspan'];
				}

				if ( !is_array( $cabecalho ) ){
					// Para a ordenação funcionar a Query tem que ter vindo da classe Paginacao
                    if ($this->ordemAtivo && $this->arCorpo['identificador'] && !($this->acao && $this->totalColuna >= count($arCabecalho))){
						$numOrdem = (is_null($this->acao)) ? $this->totalColuna + 1 : $this->totalColuna;

						if ( $this->arCorpo['ordem'][0] == $numOrdem){
							$order = $numOrdem .' '. ($this->arCorpo['ordem'][1] == self::ORDERASC ? self::ORDERDESC : self::ORDERASC);
						}else{
							$order = $numOrdem .' '. self::ORDERASC;
						}

						$this->cell[$txtCabecalho]['obj']->onclick = "gerenciaOrdem('{$order}', {$this->arCorpo['identificador']});";
						$this->cell[$txtCabecalho]['obj']->title   = "Clique para ordenar";
						$this->cell[$txtCabecalho]['obj']->style   .= "cursor: pointer;";
					}
					// Soma 1 ao total do cabecalho, pois quando entra aqui é o último nível do cabeçalho
					$this->totalColuna++;
					$rowspan = $this->cell[$txtCabecalho]['row'] != 1 ? ($this->rowspanTotal - ($this->cell[$txtCabecalho]['row'] - 1)) : $this->rowspanTotal;
					$this->cell[$txtCabecalho]['obj']->rowspan = $rowspan;
//					$this->cell[$txtCabecalho]['obj']->add( '(' . $this->rowspanTotal . ' - ' . $this->cell[$txtCabecalho]['row'] . ')' );
				}else{
					self::montaCabecalho( $cabecalho );
				}
			}
		}
	}

	private function montaCabecalhoImpressao(Array $arCabecalho = null, $addHeadRow = true){
		$arCabecalho = !is_null( $arCabecalho ) ? $arCabecalho : $this->arCabecalho;

		if ( !is_null($arCabecalho) ){
			if( is_null($this->cell) ){
				self::atribuiNivelCabecalho( $arCabecalho, 1 );
			}

			foreach( $arCabecalho as $indice => $cabecalho ){
				if ( is_array($cabecalho) ){
					$txtCabecalho = $indice;
				}else{
					$txtCabecalho = $cabecalho;
				}
				$txtCabecalho 								  = is_array($cabecalho) ? $indice : $cabecalho;
				$this->row[$this->cell[$txtCabecalho]['row']] = (($this->row[$this->cell[$txtCabecalho]['row']] instanceof TElement)
						?
						$this->row[$this->cell[$txtCabecalho]['row']]
						:
						$this->table->addHeadRow($addHeadRow));
				$row										  = $this->row[$this->cell[$txtCabecalho]['row']];
				$row->style = 'background-color: #e9e9e9;';

				$this->cell[$txtCabecalho]['obj'] = $row->addCellHead( $txtCabecalho );
				$this->cell[$txtCabecalho]['obj']->align  = 'center';
				$this->cell[$txtCabecalho]['obj']->valign = 'middle';
				$this->cell[$txtCabecalho]['obj']->class  = 'title';
				$this->cell[$txtCabecalho]['obj']->style  = 'font-weight: bold;';

				if ( is_int($this->cell[$txtCabecalho]['colspan']) ){
					$this->cell[$txtCabecalho]['obj']->colspan = $this->cell[$txtCabecalho]['colspan'];
				}

				if ( !is_array( $cabecalho ) ){
					// Para a ordenação funcionar a Query tem que ter vindo da classe Paginacao
					if ($this->ordemAtivo && $this->arCorpo['identificador'] && !($this->acao && is_null($this->totalColuna))){
						$numOrdem = (is_null($this->acao)) ? $this->totalColuna + 1 : $this->totalColuna;
						if ( $this->arCorpo['ordem'][0] == $numOrdem){
							$order = $numOrdem .' '. ($this->arCorpo['ordem'][1] == self::ORDERASC ? self::ORDERDESC : self::ORDERASC);
						}else{
							$order = $numOrdem .' '. self::ORDERASC;
						}

						$this->cell[$txtCabecalho]['obj']->onclick = "gerenciaOrdem('{$order}', {$this->arCorpo['identificador']});";
						$this->cell[$txtCabecalho]['obj']->title   = "Clique para ordenar";
						$this->cell[$txtCabecalho]['obj']->style   .= "cursor: pointer;";
					}
					// Soma 1 ao total do cabecalho, pois quando entra aqui é o último nível do cabeçalho
					$this->totalColuna++;
					$rowspan = $this->cell[$txtCabecalho]['row'] != 1 ? ($this->rowspanTotal - ($this->cell[$txtCabecalho]['row'] - 1)) : $this->rowspanTotal;
					$this->cell[$txtCabecalho]['obj']->rowspan = $rowspan;
					//					$this->cell[$txtCabecalho]['obj']->add( '(' . $this->rowspanTotal . ' - ' . $this->cell[$txtCabecalho]['row'] . ')' );
				}else{
					self::montaCabecalhoImpressao( $cabecalho, false );
				}
			}
		}
	}

//	private function montaCabecalho(Array $arCabecalho = null){
//		$arCabecalho = !is_null( $arCabecalho ) ? $arCabecalho : $this->arCabecalho;
//
//		if ( !is_null($arCabecalho) ){
//			if( is_null($this->cell) ){
//				self::atribuiNivelCabecalho( $arCabecalho, 1 );
//			}
//
//
//			foreach( $arCabecalho as $indice => $cabecalho ){
//				if ($this->exibeCabecalho !== false){
//					if ( is_array($cabecalho) ){
//						$txtCabecalho = $indice;
//					}else{
//						$txtCabecalho = $cabecalho;
//					}
//					$txtCabecalho 								  = is_array($cabecalho) ? $indice : $cabecalho;
//					$this->row[$this->cell[$txtCabecalho]['row']] = (($this->row[$this->cell[$txtCabecalho]['row']] instanceof TElement)
//																	?
//																		$this->row[$this->cell[$txtCabecalho]['row']]
//																	:
//																		$this->table->addRow());
//					$row										  = $this->row[$this->cell[$txtCabecalho]['row']];
//					$row->bgcolor = '#EFEFEF';
//
//					if ( $this->order[$indice] ){
//						$order = ($this->order[$indice]['ordenacao'] == self::ORDERASC ? self::ORDERDESC : self::ORDERASC) . "_" . $indice;
//					}else{
//						$order =  self::ORDERASC . "_" . $indice;
//					}
//
//
//					$this->cell[$txtCabecalho]['obj'] = $row->addCell( $txtCabecalho );
//					if ($this->ordemAtivo){
//						$url   = $_SERVER['REQUEST_URI'];
//						$url  .= ((strrpos($url, "/") + 1) == strlen($url)) ? "" : "/";
//
//						foreach ($_POST as $k => $val){
//							if ($val)
//								$url  .= $k . "/" . $val . "/";
//						}
//
//						$pos   = strpos($url, "numlista") !== false  ? strpos($url, "numlista") : strlen($url);
//						$url   = substr($url, 0, $pos);
//						$pos   = strpos($url, "order") !== false  ? strpos($url, "order") : strlen($url);
//						$url   = substr($url, 0, $pos) . "{ordemlista}numlista/" . self::$numlista;
//						$redir = str_replace("{ordemlista}", "order/$order/", $url);
//
//						$this->cell[$txtCabecalho]['obj']->onclick  = "location.href='$redir'";
//						$this->cell[$txtCabecalho]['obj']->title  	= 'Clique para ordenar';
//					}
//					$this->cell[$txtCabecalho]['obj']->align  	= 'center';
//					$this->cell[$txtCabecalho]['obj']->valign 	= 'middle';
//	//				$this->cell[$txtCabecalho]['obj']->class  	= 'title';
//					$this->cell[$txtCabecalho]['obj']->style  	= 'border-right: 1px solid rgb(192, 192, 192);
//																   border-bottom: 1px solid rgb(192, 192, 192);
//																   border-left: 1px solid rgb(255, 255, 255);
//																   font-weight: bold;
//																   cursor: pointer;';
//
//					if ( is_int($this->cell[$txtCabecalho]['colspan']) ){
//						$this->cell[$txtCabecalho]['obj']->colspan = $this->cell[$txtCabecalho]['colspan'];
//					}
//				}
//
//				if ( !is_array( $cabecalho ) ){
//					// Soma 1 ao total do cabecalho, pois quando entra aqui é o último nível do cabeçalho
//					$this->totalColuna++;
//					if ($this->exibeCabecalho !== false){
//						$rowspan = $this->cell[$txtCabecalho]['row'] != 1 ? ($this->rowspanTotal - ($this->cell[$txtCabecalho]['row'] - 1)) : $this->rowspanTotal;
//						$this->cell[$txtCabecalho]['obj']->rowspan = $rowspan;
//	//					$this->cell[$txtCabecalho]['obj']->add( '(' . $this->rowspanTotal . ' - ' . $this->cell[$txtCabecalho]['row'] . ')' );
//					}
//				}else{
//					self::montaCabecalho( $cabecalho );
//				}
//			}
//		}
//	}

//	private function ordenarDados( Array $arDado){
//		if (is_numeric($this->order)){
//			$linha = 0;
//			foreach($arDado as $dado){
////				for ($i=0; $i < count($dado); $i++){
//				$i = 0;
//				foreach ($dado as $colName => $colVal){
//					if ($colName == $this->itemOrder[$this->order]['coluna']){
//						$tipoOrder 			  = $this->itemOrder[$this->order]['order'];
//						$colOrder[$linha][$i] = self::mascaraType($colVal, $this->itemOrder[$this->order]['mascara']);
//						break;
//					}
//					$i++;
//				}
//				$linha++;
//			}
//			if ( $tipoOrder == self::ORDERASC ){
//				array_multisort($colOrder, SORT_ASC, $arDado);
//			}else{
//				array_multisort($colOrder, SORT_DESC, $arDado);
//			}
//		}
//		return $arDado;
////		if ( $this->order ){
////			$linha = 0;
////			foreach($arDado as $dado){
//////				for ($i=0; $i < count($dado); $i++){
////				$i = 0;
////				foreach ($dado as $colVal){
////					if ( is_array($this->order[$i]) ){
////						$tipoOrder 			  = $this->order[$i]['ordenacao'];
////						$colOrder[$linha][$i] = $colVal;
////						break;
////					}
////					$i++;
////				}
////				$linha++;
////			}
////			if ( $tipoOrder == self::ORDERASC ){
////				array_multisort($colOrder, SORT_ASC, $arDado);
////			}else{
////				array_multisort($colOrder, SORT_DESC, $arDado);
////			}
////		}
////		return $arDado;
//	}

	private function montaCorpo(){
		$arDados  = $this->arCorpo['dados'];
		$arConfig = $this->arCorpo['config'];
		$table 	  = $this->table;

		if ( !is_array($arDados) || !$arDados ){
			$row  = $table->addRow();
			$cell = $row->addCell('Não foram encontrados registros.');
			$cell->style = 'color:red;';
			return;
		}

		// Define o total de colunas a serem montadas na listagem
		$this->totalColuna = ( !is_null($this->totalColuna) ? $this->totalColuna : count( $arDados[0] ) );
		// Se tiver acao a coluna extra "Ação" deve ser contabilizada.
//		if ( $this->acao['acao'] )
//		$this->totalColuna++;

		$contLinha = 0;
		$totalColuna = $this->totalColuna - 1;
		// Caso possua "Ação" e "Cabeçalho" diminui 1 do total de colunas.
		$totalColuna += ($this->acao['acao'] && $this->arCabecalho) ? -1 : 0;

//		$arDados = self::ordenarDados( $arDados );
//		if ($this->paginacao != false){
//			$pagAtual  = $this->paginacao['pagAtual'];
//			$numPorPag = $this->paginacao['numPag'];
//
//			$regIni	 = ($pagAtual-1) * $numPorPag;
//			$regFim	 = $pagAtual * $numPorPag;
//			$arDados = array_slice($arDados, $regIni, $regFim);
//		}

		foreach( $arDados as $linha ):
			if ( is_numeric( $this->paginacao['numPag'] ) && $this->paginacao['numPag'] == $contLinha ){
				break;
			}
			$color 		  	  = is_int($contLinha/2) ? '' : '#f7f7f7';
			$row 	  	  	  = $table->addRow();
			// $row->style 	  = "font-size: 10px;";
//			if($this->classTr){
//				$row->class = $this->classTr;
//			}

//			$i = 0;
//			foreach( $linha as $indColuna => $coluna ):
			for($i=0; $i < $totalColuna; $i++):
				if ( current($linha) ){
					$indColuna = key( $linha );
					$coluna    = current( $linha );
				}else{
					$coluna    = 'N/A';
				}
				next( $linha );
				// define tipo de dado
				$type = strtolower($arConfig[$i]['type']);
				if ( empty($type) && is_numeric($coluna) ){
					$type = self::TYPENUMERIC;
				}elseif ( empty($type) ){
					$type = self::TYPESTRING;
				}
				$coluna = trim($coluna);
				// caso tenha sido definido o totalizador
				if ( $this->config['totalLinha'] ){
					// totaliza todos os campos numéricos
					self::calculaTotalLinha($indColuna, $coluna, $type);
				}
				//Este controle garante que os campos do corpo da lista não ultrapassem a quantidade de campos do cabeçalho
				if ($i >= $totalColuna)	continue;
//				if ($i >= ($this->totalColuna + $this->acaoAltTotalColuna))	continue;

				// Faz o tratamento do valor pelo tipo
				$coluna = self::trataType($coluna, $type);
				// Caso tenha html configurado faz a substituição
				if ( $arConfig[$i]['html'] ){
					$coluna = self::carregaHTML($arConfig[$i], $linha);
				}
				// Cria a célula e seta seu valor
				$cell = $row->addCell( $coluna );
				// configura a célula pelo "type"
				if ( ($type == self::TYPENUMERIC || $type == self::TYPENUMERICNATIVO || $type == self::TYPEMONEY) ){
					$cell->align = 'right';
				}elseif ( $type == self::TYPEDATE ){
					$cell->align = 'center';
				}elseif( $type == self::TYPETIME ){
					$cell->align = 'right';
				}else{
					$cell->align = 'left';
				}
				// Atribuição do alinhamento, vindo do config.
				if ( $arConfig[$i]['align'] ){
					$cell->align = $arConfig[$i]['align'];
				}

				// Atribuição do style, vindo do config.
				$style 		  = $cell->style;
				$style 		 .= $arConfig[$i]['style'];
				$cell->style = $style;

//				$i++;
//			endforeach;
			endfor;

            if ( $this->acao['acao'] && $this->acao['localinicio'] == true ){
                $coluna = self::carregaHTML($this->acao['acao'], $linha);
                $tdAcao = $row->addCell( $coluna );
                $tdAcao->style = 'text-align: center;';
//				$this->acaoAltTotalColuna = 1;
            }

			if ( $this->acao['acao'] && $this->acao['localinicio'] == false ){
				$coluna = self::carregaHTML($this->acao['acao'], $linha);
				$row->addCell( $coluna );
//				$this->acaoAltTotalColuna = 1;
			}

			self::montaSubLinha($linha);
			$contLinha++;
		endforeach;
		self::montaRodape();
		if($this->arCorpo['pagina']){
			self::montaPaginacao();
		}
	}

	private function montaCorpoImpressao(){
		$arDados  = $this->arCorpo['dados'];
		$arConfig = $this->arCorpo['config'];
		$table 	  = $this->table;

		if ( !is_array($arDados) || !$arDados ){
			$row  = $table->addRow();
			$cell = $row->addCell('Não foram encontrados registros.');
			$cell->style = 'color:red;';
			return;
		}

		// Define o total de colunas a serem montadas na listagem
		$this->totalColuna = ( !is_null($this->totalColuna) ? $this->totalColuna : count( $arDados[0] ) );

		$contLinha = 0;
		$totalColuna = $this->totalColuna;
		// Caso possua "Ação" e "Cabeçalho" diminui 1 do total de colunas.
		$totalColuna += ($this->acao['acao'] && $this->arCabecalho) ? -1 : 0;

		foreach( $arDados as $linha ):
			if ( is_numeric( $this->paginacao['numPag'] ) && $this->paginacao['numPag'] == $contLinha ){
				break;
			}
			$color 		  	  = is_int($contLinha/2) ? '' : '#f7f7f7';
			$row 	  	  	  = $table->addRow();
			$row->style 	  = "font-size: 10px; background-color: {$color};";

			if ( $this->acao['acao'] && $this->acao['localinicio'] == true ){
				$coluna = self::carregaHTML($this->acao['acao'], $linha);
				$row->addCell( $coluna );
			}

			for($i=0; $i < $totalColuna; $i++):
				if ( current($linha) ){
					$indColuna = key( $linha );
					$coluna    = current( $linha );
				}else{
					$coluna    = 'N/A';
				}
				next( $linha );
				// define tipo de dado
				$type = strtolower($arConfig[$i]['type']);
				if ( empty($type) && is_numeric($coluna) ){
					$type = self::TYPENUMERIC;
				}elseif ( empty($type) ){
					$type = self::TYPESTRING;
				}
				$coluna = trim($coluna);
				// caso tenha sido definido o totalizador
				if ( $this->config['totalLinha'] ){
					// totaliza todos os campos numéricos
					self::calculaTotalLinha($indColuna, $coluna, $type);
				}
				//Este controle garante que os campos do corpo da lista não ultrapassem a quantidade de campos do cabeçalho
				if ($i >= $totalColuna)	continue;
				//				if ($i >= ($this->totalColuna + $this->acaoAltTotalColuna))	continue;

				// Faz o tratamento do valor pelo tipo
				$coluna = self::trataType($coluna, $type);
				// Caso tenha html configurado faz a substituição
				if ( $arConfig[$i]['html'] ){
					$coluna = self::carregaHTML($arConfig[$i], $linha);
				}
				// Cria a célula e seta seu valor
				$cell = $row->addCell( $coluna );
				// configura a célula pelo "type"
				if ( ($type == self::TYPENUMERIC || $type == self::TYPENUMERICNATIVO || $type == self::TYPEMONEY) ){
					$cell->align = 'right';
					$cell->style = 'color:#0066CC;';
				}elseif ( $type == self::TYPEDATE ){
					$cell->align = 'center';
				}elseif( $type == self::TYPETIME ){
					$cell->align = 'right';
				}else{
					$cell->align = 'left';
				}
				// Atribuição do alinhamento, vindo do config.
				if ( $arConfig[$i]['align'] ){
					$cell->align = $arConfig[$i]['align'];
				}

				// Atribuição do style, vindo do config.
				$style 		  = $cell->style;
				$style 		 .= $arConfig[$i]['style'];
				$cell->style = $style;

				//				$i++;
				//			endforeach;
			endfor;
			if ( $this->acao['acao'] && $this->acao['localinicio'] == false ){
				$coluna = self::carregaHTML($this->acao['acao'], $linha);
				$row->addCell( $coluna );
				//				$this->acaoAltTotalColuna = 1;
			}

			self::montaSubLinha($linha);
			$contLinha++;
		endforeach;
		self::montaRodapeImpressao();
		if($this->arCorpo['pagina']){
			self::montaPaginacao();
		}
	}

	private function montaSubLinha($linha){
		if ($this->config['subLinha'] !== false){
			$id    = self::carregaHTML($this->config['subLinha'], $linha);
			$table = $this->table;
			$row   = $table->addRow();
			$row->style = 'display:none;';
			$cell  = $row->addCell('');
			$cell->id = "subLinha_" . $id;
			$cell->colspan = ($this->totalColuna);
//			$cell->colspan = ($this->totalColuna + $this->acaoAltTotalColuna);
		}
	}

	private function montaRodape(){
		if ( $this->config['totalLinha'] ) 	  self::montaTotalLinha();
		if ( $this->config['totalRegistro'] ) self::montaTotalRegistro();
	}

	private function montaRodapeImpressao(){
		if ( $this->config['totalRegistro'] ) self::montaTotalRegistroImpressao();
	}

	private function montaTotalLinha(){
		$arColuna  = $this->arCorpo['dados'][0];
		$arConfig  = $this->arCorpo['config'];
		$table 	   = $this->table;
		$row  	   = $table->addRow();
		$row->bgcolor = '#E3E3E3';
		// Primeira célula
		$cell  	   = $row->addCell('Totais:');

		$totalColuna = $this->totalColuna;

		$i = 0;
        unset($arColuna[key($arColuna)]);
        if ( !$this->acao['acao'] ){
            $totalColuna--;
			$i++;
		}else{
			$totalColuna += ($this->acao['acao'] && $this->arCabecalho) ? -1 : 0;
//			$totalColuna--;
		}

		foreach ( $arColuna as $indCol => $valCol ):
			if ( $i >= ($totalColuna) ) break;

			// Atribui o valor total da coluna
			$valCol = trim($this->arCorpoTotal[$indCol]);

			// Pula a primeira coluna do array, pois ela será substituída pelo total
//			if ( $i == 0 ){
//				$i++;
//				continue;
//			}
			// define tipo de dado
			$type = strtolower($arConfig[$i]['type']);
			if ( empty($type) && is_numeric($valCol) ){
				$type = self::TYPENUMERIC;
			}elseif ( empty($type) ){
				$type = self::TYPESTRING;
			}
			// Somente campo do tipo numérico, ou seja, que permite somatória
			if ( (is_numeric( $valCol )
							&& ($type == self::TYPENUMERIC || $type == self::TYPENUMERICNATIVO || $type == self::TYPEMONEY)
							|| $type == self::TYPETIME)
					&& $arConfig[$i]['totalLinha'] !== false ){

				// Faz o tratamento do valor pelo tipo
				$valCol = self::trataType($valCol, $type);
				// Caso tenha html configurado faz a substituição
				if ( $arConfig[$i]['html'] ){
					$valCol = self::carregaHTML($arConfig[$i], $this->arCorpoTotal);
				}
			}else{
				$valCol = '-';
			}
			// Adiciona Célula
			$cell = $row->addCell( $valCol );
			$cell->nowrap = 'nowrap';

			if ( $valCol !== '-' ){
				$cell->align = 'right';

				if ( $type !== self::TYPETIME )
					$cell->style = 'color:#0066CC;';
				// Atribuição do alinhamento, vindo do config.
				if ( $arConfig[$i]['align'] ){
					$cell->align = $arConfig[$i]['align'];
				}
			}
			$i++;
		endforeach;
	}

	private function montaTotalRegistro(){
		$totalColuna = $this->totalColuna;
//		$qtdReg = count($this->arCorpo['dados']);
		$qtdReg = $this->arCorpo['total'] ? $this->arCorpo['total'] : count($this->arCorpo['dados']);
		$table  = $this->table;
		$row 	= $this->getTrRodape();
		$cell 	= $row->addCell("Total de Registros: $qtdReg");
		$cell->nowrap  = 'nowrap';
		// $cell->style   = 'border-top: 2px solid #000000; font-weight: bold;';
	}

	private function montaTotalRegistroImpressao(){
		$totalColuna = $this->totalColuna;
		// Caso possua "Ação" e NÃO possua "Cabeçalho" aumenta 1 do total de colunas.
		$totalColuna += ($this->acao['acao'] && empty($this->arCabecalho)) ? 1 : 0;

		$qtdCol = $totalColuna;
//		$qtdReg = count($this->arCorpo['dados']);
		$qtdReg = $this->arCorpo['total'] ? $this->arCorpo['total'] : count($this->arCorpo['dados']);
		$table  = $this->table;
		$row 	= $table->addFootRow();
		$cell 	= $row->addCell("Total de Registros: $qtdReg");
		$cell->colspan = $qtdCol;
		$cell->nowrap  = 'nowrap';
		$cell->style   = 'font-weight: bold;';
	}

	private function montaPaginacao()
	{
		$nrPaginaAtual = $this->arCorpo['pagina'];
		$nrPaginas 	   = $this->arCorpo['totalPagina'];
		$nrTotal       = $this->arCorpo['total'];
		$numRegistroInicial = $this->arCorpo['numRegistroInicialPagina'];
		$numRegistroFinal = $this->arCorpo['numRegistroFinalPagina'];

		/**
		 * BLOCO PAGINAÇÃO
		 */
		$anterior = $nrPaginaAtual - 1;
		$proximo  = $nrPaginaAtual + 1;

		# Numero total de páginas
//		$nrPaginas = ceil($nrTotalRegistro / $nrRegPorPagina);

//		# Pegamos a Página Inicial do Bloco da Paginação
//		$nrPagInicial = ($nrBlocoAtual*$nrBlocoPaginacaoMaximo)-($nrBlocoPaginacaoMaximo);
//		# Pegamos a Página Final do Bloco da Paginação
//		$nrPagFinal	  = ($nrPagInicial+$nrBlocoPaginacaoMaximo);
//		$nrPagFinal   = $nrPagFinal <= $nrPaginas ? $nrPagFinal : $nrPaginas;

		$imgProximo 			= "<img src=\"../imagens/controle_paginacao/proximo.gif\" align=\"top\" />";
		$imgProximo_01 			= "<img src=\"../imagens/controle_paginacao/proximo_01.gif\" align=\"top\" />";
		$imgAnterior 			= "<img src=\"../imagens/controle_paginacao/anterior.gif\" align=\"top\" />";
		$imgAnterior_01 		= "<img src=\"../imagens/controle_paginacao/anterior_01.gif\" align=\"top\" />";
		$imgBlocoAnterior 		= "<img src=\"../imagens/controle_paginacao/bloco_anterior.gif\" align=\"top\" />";
		$imgBlocoAnterior_01 	= "<img src=\"../imagens/controle_paginacao/bloco_anterior_01.gif\" align=\"top\" />";
		$imgBlocoProximo 		= "<img src=\"../imagens/controle_paginacao/bloco_proximo.gif\" align=\"top\" />";
		$imgBlocoProximo_01 	= "<img src=\"../imagens/controle_paginacao/bloco_proximo_01.gif\" align=\"top\" />";

		$htmlPaginacao = "Exibindo <b>{$numRegistroInicial}</b> até <b>{$numRegistroFinal}</b> de <b>{$nrTotal}</b> linhas.";

		if($nrTotal == 1) {
		    $htmlPaginacao = "Exibindo <b>{$nrTotal}</b> linha.";
        }

        $html = '<div class="pagination-count">' .  $htmlPaginacao . '</div><ul class="pagination">';

		# Verificamos se a quantidade de páginas é maior que 1
		if($nrPaginas > 1 ) {
			# Se tem página antes da atual, mostramos o botão de anterior
			if($anterior > 0) {
				$html .= '<li class="page-item"><a class="page-link" onclick="gerenciaPaginacao('.$anterior.','.$this->arCorpo['identificador'].')"><i class="fa fa-caret-left"></i></a></li>';
			} else { # mostra o botão anterior desabilitado
				$html .= '<li class="page-item disabled"><a class="page-link" href="javascript:;"><i class="fa fa-caret-left"></i></a></li>';
			}

			if($nrPaginaAtual < 5){
				$nrPagInicial = 0;
			} else {
				$nrPagInicial = $nrPaginaAtual - 6;
			}
			$nrPagFinal = $nrPaginaAtual + 5;
			$nrPagFinal = $nrPagFinal <= $nrPaginas ? $nrPagFinal : $nrPaginas;

			# Listando as paginas
			for($i=($nrPagInicial+1);$i <= ($nrPagFinal);$i++) {
				$sublinhado = "text-decoration: underline;";
				if($i == $nrPaginaAtual) {
					$marcar = "active";
					$sublinhado = "";
				}

				$pipe = '';
				if($i == $nrPaginas){
					$pipe = "";
				}

				$html .= '<li class="page-item '.$marcar.'"><a  class="page-link" onclick="gerenciaPaginacao('.$i.','.$this->arCorpo['identificador'].')">'.$i.'</a></li>'.$pipe.'';
//				$html .= ' <a style="cursor:pointer;'.$sublinhado.$marcar.'" onclick="listaPaginacao(this, '.$i.','.$nrRegPorPagina.','.$nrBlocoAtual.',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$i.'</a> '.$pipe.'';
				unset($marcar);
			}

			# verifica se existe página a frente da atual, se sim aparece o botão de próximo
			if($proximo <= $nrPaginas) {
				$html .= '<li class="page-item"><a  class="page-link" onclick="gerenciaPaginacao('.$proximo.','.$this->arCorpo['identificador'].')"><i class="fa fa-caret-right"></i></a></li>';
//				$html .= '<a style="cursor:pointer;" onclick="listaPaginacao(this, '.$proximo.','.$nrRegPorPagina.','.$nrBlocoAtual.',\''.$this->nmDiv.'\',\''.$nmControleMetodo.'\',\''.$nrBlocoPaginacaoMaximo.'\')">'.$imgProximo.'</a>';
			} else { # desabilita o botão proximo
				$html .= '<li class="page-item disabled"><a class="page-link" href="javascript:;"><i class="fa fa-caret-right"></i></a></li>';
			}
		}

        $html .= '</ul>';

		$row 	= $this->getTrRodape(array('class' => 'table-pagination', 'width' => null));
		$cell 	= $row->addCell($html);
		$cell->nowrap  = 'nowrap';
		$cell->style   = "text-align: right;";
		//$cell->style   = 'border-top: 2px solid #000000; font-weight: bold; text-align: right';
	}

	private function carregaHTML($config, Array $linha){
		if ( is_array($config) ){
			$html = $config['html'];
			$type = strtolower($config['type']);
		}else{
			$html = $config;
		}

		foreach($linha as $indCol => $valCol){
			$indHTML[] = '{' . $indCol . '}';
			$valHTML[] = self::trataType( $valCol, $type );
		}

		if($this->arCorpo['pagina']){
			$indHTML[] = '{pag_pagina}';
			$valHTML[] = trim($this->arCorpo['pagina']);
		}

		if($this->arCorpo['identificador']){
			$indHTML[] = '{pag_identificador}';
			$valHTML[] = trim($this->arCorpo['identificador']);
		}

		$html = str_replace($indHTML, $valHTML, $html);

		$iniPHP = stripos($html, "<php>");
		while ($iniPHP !== false && $iniPHP >= 0){
			$fimPHP    = stripos($html, "</php>");
			$searchPHP = substr($html,$iniPHP, ($fimPHP - $iniPHP)+6);
			$codigo    = substr($html,($iniPHP + 5), ($fimPHP - $iniPHP) - 5);
			$replace   = eval($codigo.";");

			$html = str_replace($searchPHP, $replace, $html);
			$iniPHP = stripos($html, "<php>");
		}

		return $html;
//		return str_replace($indHTML, $valHTML, $html);
	}

	private function trataType($valor, $type = null){
//		$valor = trim($valor);

		/*if ( $type == self::TYPENUMERIC && is_numeric(trim($valor)) ){
			$valor = number_format($valor, 2, ',', '.');
		}elseif ( $type == self::TYPEMONEY && is_numeric(trim($valor)) ){*/
		if ( $type == self::TYPENUMERIC && is_numeric($valor) ){
			if ( is_string($valor) && strpos($valor, '.') !== false ){
				$valor = (real) $valor;
			}
			$valor = ( $valor == 0 || is_float($valor) === false ) ? (integer) $valor : number_format($valor, 2, ',', '.');
		}elseif ( $type == self::TYPEMONEY && is_numeric($valor) ){
			$valor = number_format($valor, 2, ',', '.');
		}elseif ( $type == self::TYPETIME && ( strpos($valor, ':') !== false || is_numeric($valor) ) ){
			// Transforma o valor de TIME em SEGUNDO
			$seconds = self::transformaTimeSegundo( $valor );
			// Transforma o valor de SEGUNDO em TIME
			$valor	 = self::transformaSegundoTime( $seconds );
		}

		return $valor;
	}

	private function mascaraType($valor, $type = null){
		if ($type == self::TYPENUMERIC){
			$valor = str_replace(array(".", ","), array("", "."), $valor);
		}
		return $valor;
	}

	private function transformaTimeSegundo( $time ){
		$seconds = 0;
		if ( $time ){
			list( $g, $i, $s ) = explode( ':', $time );
			$seconds += $g * 3600;
			$seconds += $i * 60;
			$seconds += $s;
		}

		return $seconds;
	}

	private function transformaSegundoTime( $seconds ){
		if( $seconds ) {

			$hours = floor( $seconds / 3600 );
			$seconds -= $hours * 3600;
			$minutes = floor( $seconds / 60 );
			$seconds -= $minutes * 60;

			$time = sprintf("%02d", $hours) . ":" . sprintf("%02d", $minutes);
		} else {
			$time = "00:00";
		}

		return $time;
	}

	private function calculaTotalLinha($indice, $coluna, $type = null){

		if ( is_numeric($coluna) && $type != self::TYPETIME ){
			$this->arCorpoTotal[$indice] += $coluna;
		}elseif ( $type == self::TYPETIME && ( strpos($coluna, ':') !== false || is_numeric($coluna) ) ){
			$segHistorico = self::transformaTimeSegundo( $this->arCorpoTotal[$indice] );
			$segNovo	  = self::transformaTimeSegundo( $coluna );
			$this->arCorpoTotal[$indice] = self::transformaSegundoTime( $segHistorico + $segNovo );
		}
	}

	private function atribuiNivelCabecalho(Array $arCabecalho = null, $row){
		if ( !is_null($arCabecalho) && $this->exibeCabecalho !== false ){
			$this->rowspan 		= 1;
			$this->rowspanTotal = $this->rowspanTotal < $row ? $row : $this->rowspanTotal;

			foreach($arCabecalho as $indice => $cabecalho){
				$txtCabecalho = is_array($cabecalho) ? $indice : $cabecalho;
				$this->cell[$txtCabecalho]['row']     = empty($this->cell[$txtCabecalho]['row'])
						? $row
						: $this->cell[$txtCabecalho]['row'];
				$this->cell[$txtCabecalho]['rowspan'] = empty($this->cell[$txtCabecalho]['rowspan'])
						? $this->rowspan
						: $this->cell[$txtCabecalho]['rowspan'];

				if ( is_array($cabecalho) ){
//					$this->rowspanTotal++;
					$this->cell[$txtCabecalho]['colspan'] = self::calculaNivelCabecalho( $cabecalho );
					$this->cell[$txtCabecalho]['rowspan'] = $this->rowspan;


					self::atribuiNivelCabecalho( $cabecalho, ($row + 1) );
				}
			}
		}
	}

	private function calculaNivelCabecalho(Array $arCabecalho){
		$this->rowspan++;
		$colspan = count( $arCabecalho );
		foreach($arCabecalho as $indice => $cabecalho){
			$txtCabecalho = is_array($cabecalho) ? $indice : $cabecalho;
			if ( is_array($cabecalho) ){
				$colspanRetorno += self::calculaNivelCabecalho( $cabecalho, ($row+1) );
			}else{
				$colspanRetorno++;
			}
		}
		$colspan = ($colspan > $colspanRetorno ? $colspan : $colspanRetorno);

		return $colspan;
	}

	private function getTrRodape($arrOptions = array()){
		if(!$this->trRodape){
			$tabelaRodape = new TTable($arrOptions);
			$rowRodape    = $tabelaRodape->addRow();

			$cellContainer          = $this->table->addRow()->addCell($tabelaRodape);
			$cellContainer->colspan = $this->totalColuna;
			$cellContainer->style   = "padding: 0px;";

			$this->trRodape = $rowRodape;
		}

		return $this->trRodape;
	}

	private function javaScript()
	{
		$param = array();
		foreach ($_POST as $k => $v){
			$param[$k] = is_array($v) ? $v : utf8_encode($v);
		}
		?>
		<script>
			if (!param)
				var param = new Array();

			param[<?php echo $this->arCorpo['identificador']?>] = eval('(<?php echo json_encode($param)?>);');

			function gerenciaPaginacao(pagina, identificador)
			{
				var campo, name;
				var form = $(document.createElement("form"));
				$.each(param[identificador], function (k, v){
					if (typeof v == 'object'){
						$.each(v, function (y, x){
							name = k+'['+y+']';
							campo = $(document.createElement("input"));
							campo.attr('name', name);
							campo.val(x);
							form.append(campo);
						});


					}else{
						name = k;
						campo = $(document.createElement("input"));
						campo.attr('name', name);
						campo.val(v);
						form.append(campo);
					}
				})
				// Exclui campo de paginação
				$('[name*="pag"]', form).remove();
				// Adiciona campo de paginação
				name  = 'pag['+identificador+']';
				campo = $(document.createElement("input"));
				campo.attr('name', name);
				campo.val(pagina);
				form.append(campo);

				var div = $(document.createElement("div")).css('display', 'none')
						.append(form);
				$(document.body).append(div);
				form.attr({'action' : location.href, 'method' : 'post'})
						.submit();
			}

			function gerenciaOrdem(ordem, identificador)
			{
				var campo, name;
				var form = $(document.createElement("form"));
				$.each(param[identificador], function (k, v){
					if (typeof v == 'object'){
						$.each(v, function (y, x){
							name = k+'['+y+']';
							campo = $(document.createElement("input"));
							campo.attr('name', name);
							campo.val(x);
							form.append(campo);
						});


					}else{
						name = k;
						campo = $(document.createElement("input"));
						campo.attr('name', name);
						campo.val(v);
						form.append(campo);
					}
				})
				// Exclui campo de paginação
				$('[name*="pag"]', form).remove();
				$('[name*="ordem"]', form).remove();
				// Adiciona campo de paginação
				name  = 'ordem['+identificador+']';
				campo = $(document.createElement("input"));
				campo.attr('name', name);
				campo.val(ordem);
				form.append(campo);

				var div = $(document.createElement("div")).css('display', 'none')
						.append(form);
				$(document.body).append(div);
				form.attr({'action' : location.href, 'method' : 'post'})
						.submit();
			}

		</script>
		<?php
	}

	public function getTable(){
	    return $this->table;
    }
}