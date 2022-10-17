<?php
require_once( APPRAIZ . 'includes/fpdf/fpdf.inc');
class PDF_PTA_Table extends PDF_Table
{
	var $widths;
	var $aligns;
	var $entreLinha = 5;
	
	function setEntreLinha($h)
	{
	    //Set the array of column widths
	    $this->entreLinha = $h;
	}
	
	function SetWidths($w)
	{
	    //Set the array of column widths
	    $this->widths=$w;
	}
	
	function SetAligns($a)
	{
	    //Set the array of column alignments
	    $this->aligns=$a;
	}
	
	function Row($data)
	{
	    //Calculate the height of the row
	    $nb=0;
	    for($i=0;$i<count($data);$i++)
	        $nb=max($nb,$this->NbLines($this->widths[$i],$data[$i]));
	        
	    $height = $this->unit == 'cm' ? ($this->entreLinha/10) : $this->entreLinha;
	    $h= $height*$nb;
	    //Issue a page break first if needed
	    $this->CheckPageBreak($h);
	    //Draw the cells of the row
	    for($i=0;$i<count($data);$i++)
	    {
	        $w=$this->widths[$i];
	        $a=isset($this->aligns[$i]) ? $this->aligns[$i] : 'L';
	        //Save the current position
	        $x=$this->GetX();
	        $y=$this->GetY();
	        //Draw the border
	        $this->Rect($x,$y,$w,$h);
	        //Print the text
	        $this->MultiCell($w,$height,$data[$i],0,$a);
	        //Put the position to the right of the cell
	        $this->SetXY($x+$w,$y);
	    }
	    //Go to the next line
	    $this->Ln($h);
	}
	
	function CheckPageBreak($h)
	{
	    //If the height h would cause an overflow, add a new page immediately
	    if($this->GetY()+$h>$this->PageBreakTrigger)
	        $this->AddPage($this->CurOrientation);
	}
	
	function NbLines($w,$txt)
	{
	    //Computes the number of lines a MultiCell of width w will take
	    $cw=&$this->CurrentFont['cw'];
	    if($w==0)
	        $w=$this->w-$this->rMargin-$this->x;
	    $wmax=($w-2*$this->cMargin)*1000/$this->FontSize;
	    $s=str_replace("\r",'',$txt);
	    $nb=strlen($s);
	    if($nb>0 and $s[$nb-1]=="\n")
	        $nb--;
	    $sep=-1;
	    $i=0;
	    $j=0;
	    $l=0;
	    $nl=1;
	    while($i<$nb)
	    {
	        $c=$s[$i];
	        if($c=="\n")
	        {
	            $i++;
	            $sep=-1;
	            $j=$i;
	            $l=0;
	            $nl++;
	            continue;
	        }
	        if($c==' ')
	            $sep=$i;
	        $l+=$cw[$c];
	        if($l>$wmax)
	        {
	            if($sep==-1)
	            {
	                if($i==$j)
	                    $i++;
	            }
	            else
	                $i=$sep+1;
	            $sep=-1;
	            $j=$i;
	            $l=0;
	            $nl++;
	        }
	        else
	            $i++;
	    }
	    return $nl;
	}

	public function montarRowComTitulo( $arTitulos, $arDados, $arWidth, $arFormatacao ){
		
		$arDadosFormatados = array();
		foreach( $arDados as $dado ){
			$arDadosFormatados[] = "\n$dado";
		}
		
		$yAntes = $this->getY();
		
		$this->SetFont($arFormatacao[0],$arFormatacao[1],$arFormatacao[2]);
		$this->SetWidths( $arWidth );
		$this->Row( $arDadosFormatados );
		
		$yDepois = $this->getY();
		
		$this->SetFont($arFormatacao[0],$arFormatacao[3],$arFormatacao[2]);
		$this->setY( $yAntes );
		
		foreach( $arTitulos as $key => $stTitulo ){
			$this->cell( $arWidth[$key], 0.5, $stTitulo, 0, 0 );
		}
		
		$this->setY( $yDepois );
	}
	
	function Header(){

		$nrAnexo = $_SESSION['nrAnexoCabecalho'] = $_SESSION['nrAnexo'];
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		
		$stTitulo    = 'PLANO DE AÇÕES ARTICULADAS - PAR';
		switch( $nrAnexo ){
			case( 1 ):
				$stSubtitulo = 'DESCRIÇÃO DO PROJETO';
				break;
			case( 2 ):
				$stSubtitulo = 'DETALHAMENTO DA AÇÃO';
				break;
			case( 3 ):
				$stSubtitulo = 'METAS FÍSICAS E CRONOGRAMA DE EXECUÇÃO DA AÇÃO';
				break;
			case( 4 ):
				$stSubtitulo = 'CRONOGRAMA DE DESEMBOLSO DO PROJETO';
				break;
			case( 5 ):
				$stSubtitulo = 'DETALHAMENTO DOS ITENS DA ESPECIFICAÇÃO DA AÇÃO';
				break;
		}
		
		
		$nrTamanhoPequeno 	= 3;
		$nrTamanhoMedio 	= 5;
		$nrTamanhoGrande 	= $this->CurOrientation == 'P' ? 11 : 19.5;
		$nrTamanhoBox 		= $this->CurOrientation == 'P' ? 19 : 27.5;
		
		$this->SetFillColor( 200, 200, 200 );
		$this->SetFont('Arial','B',16);
		$this->cell( $nrTamanhoMedio, 1.5, 'MEC/FNDE', 1, 0, 'C', 1 );
		$this->cell( $nrTamanhoGrande, 1.5, 'PLANO DE TRABALHO', 1, 0, 'C', 1 );
		$this->cell( $nrTamanhoPequeno, 1.5, 'ANEXO', 1, 1, 'C', 1 );
		
		$this->ln(-0.8);
		$this->SetFont('Arial','B',7);
		$this->cell( $nrTamanhoMedio, 1, '', 0, 0, 'C' );
		$this->cell( $nrTamanhoGrande, 1, $stSubtitulo, 0, 0, 'C' );
		$this->SetFont('Arial','B',15);
		$this->cell( $nrTamanhoPequeno, 1, $nrAnexo, 0, 1, 'C' );

		$this->SetFont('Arial','B',7);
		if( $nrAnexo != 5 ){
			$this->cell( $nrTamanhoBox, 0.4, $stTitulo, 0, 1, 'C' );		
		}
		else{
			$this->SetFont('Arial','',6);
			$stTitulo    = 'Utilize este formulário somente para a especificação, discriminada no Anexo 4, com a unidade KIT, MÓDULO, EXEMPLAR e ESCOLA.';
			$this->cell( $nrTamanhoBox, 0.2, $stTitulo, 0, 1, 'C' );		
			$this->cell( $nrTamanhoBox, 0.2, 'Obs: Utilize quantos formulários forem necessários para complementação deste anexo.', 0, 1, 'C' );		
		}
		
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		
		$arFormatacao = array( 'Arial', '', 7, 'B' ); 
		
		switch( $nrAnexo ){
			case( 1 ):
			case( 4 ):

				// Primeira Linha
				$arWidth = array( 3, 8, 8 );
				
				$arTitulo[] = '1 - EXERCÍCIO';
				$arTitulo[] = '2 - NÍVEL DE ENSINO';
				$arTitulo[] = '3 - ABRANGÊNCIA DO PROJETO';
				
				$arDados[] = $arDadosCabecalho['nrExercicio'];
				$arDados[] = 'Educação Básica';
				$arDados[] = 'PAR';
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Segunda Linha
				$arWidth = array( 3, 16 );
				
				$arTitulo[] = '4 - CNPJ';
				$arTitulo[] = '5 - NOME DO ÓRGÃO OU ENTIDADE';

				$arDados[] = $arDadosCabecalho['cnpjOrgao'];
				$arDados[] = $arDadosCabecalho['nomeOrgao'];
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Terceira Linha
				$arWidth = array( 17, 2 );
				
				$arTitulo[] = $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? '6 - MUNICÍPIO' : '6 - ESTADO';
				$arTitulo[] = '7 - UF';
				
				$arDados[] = $arDadosCabecalho['mundescricao'];
				$arDados[] = $arDadosCabecalho['estuf'];
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Quarta Linha
				$arWidth = array( 19 );
				
				$arTitulo[] = '9 - AÇÃO A SER EXECUTADA';
				
				$arDados[] = implode( "\n", $arDadosCabecalho['dimcod'] );
					
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );				
				
				break;
			case( 6 ):

				// Primeira Linha
				$arWidth = array( 3, 8, 8 );
				
				$arTitulo[] = '1 - EXERCÍCIO';
				$arTitulo[] = '2 - NÍVEL DE ENSINO';
				$arTitulo[] = '3 - ABRANGÊNCIA DO PROJETO';
				
				$arDados[] = $arDadosCabecalho['nrExercicio'];
				$arDados[] = 'Educação Básica';
				$arDados[] = 'PAR';
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Segunda Linha
				$arWidth = array( 3, 16 );
				
				$arTitulo[] = '4 - CNPJ';
				$arTitulo[] = '5 - NOME DO ÓRGÃO OU ENTIDADE';

				$arDados[] = $arDadosCabecalho['cnpjOrgao'];
				$arDados[] = $arDadosCabecalho['nomeOrgao'];
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Terceira Linha
				$arWidth = array( 17, 2 );
				
				$arTitulo[] = $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? '6 - MUNICÍPIO' : '6 - ESTADO';
				$arTitulo[] = '7 - UF';
				
				$arDados[] = $arDadosCabecalho['mundescricao'];
				$arDados[] = $arDadosCabecalho['estuf'];
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );			
				break;
				
			case( 2 ):

				// Primeira Linha
				$arWidth = array( 19 );
				
				$arTitulo[] = '1 - NOME DO ÓRGÃO OU ENTIDADE';
				$arDados[] = $arDadosCabecalho['nomeOrgao'];
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Segunda Linha
				$arWidth = array( 17, 2 );
				
				$arTitulo[] = $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? '2 - MUNICÍPIO' : '2 - ESTADO';
				$arTitulo[] = '3 - UF';
				
				$arDados[] = $arDadosCabecalho['mundescricao'];
				$arDados[] = $arDadosCabecalho['estuf'];
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );					
				
				break;
			case( 3 ):

				// Primeira Linha
				$arWidth = array( 3, 12.5, 10, 2 );
				
				$arTitulo[] = '1 - EXERCÍCIO';
				$arTitulo[] = '2 - NOME DO ÓRGÃO OU ENTIDADE';
				$arTitulo[] = $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? '3 - MUNICÍPIO' : '3 - ESTADO';
				$arTitulo[] = '4 - UF';
				
				$arDados[] = $arDadosCabecalho['nrExercicio'];
				$arDados[] = $arDadosCabecalho['nomeOrgao'];
				$arDados[] = $arDadosCabecalho['mundescricao'];
				$arDados[] = $arDadosCabecalho['estuf'];
				
				$this->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );				
				
				break;
			case( 5 ):
				$stSubtitulo = 'DETALHAMENTO DOS ITENS DA ESPECIFICAÇÃO DA AÇÃO';
				break;
			case( 6 ):
				$stSubtitulo = 'ESCOLAS BENEFICIADAS PELA AÇÃO';
				break;
		}		
		
		
		
		
	}
	
	function Footer(){

		$nrAnexo = $_SESSION['nrAnexoCabecalho'];
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		
		switch( $nrAnexo ){
			case( 1 ):
				$nrCampo = '11';
				break;
			case( 2 ):
			case( 4 ):
				$nrCampo = '7';
				break;
			case( 3 ):
				$nrCampo = '8';
				break;
			case( 5 ):
				$nrCampo = '10';
				break;
		}		
		
		$nrTamanhoBox = $this->CurOrientation == 'P' ? 19 : 27.5;
		$nrTamanhoIntervaloPequeno = $this->CurOrientation == 'P' ? 0.2 : 8.7;
		$nrTamanhoIntervaloGrande = $this->CurOrientation == 'P' ? 3 : 7.7;
		
		$this->SetFillColor( 200, 200, 200 );
		$this->SetY(-5);
		$this->SetFont('Arial','B',10);
		$this->cell( $nrTamanhoBox, 3, '', 													1, 1, 'C', 1 );
		
		$this->SetFillColor( 255, 255, 255 );
		$this->SetFont('Arial','',7);
		$this->cell( $nrTamanhoBox, 0.4, 'Formulário confeccionado obedecendo aos preceitos da IN/STN/MF nº 01, de 15.1.1997 e as suas alterações', 0, 0, 'C' );
		
		$this->ln(-3);
		$this->SetFont('Arial','B',7);
		$this->cell( 5, 0.5, $nrCampo . ' - AUTENTICAÇÃO DAS INFORMAÇÕES', 			0, 1, 'L' );
		$this->cell( 5, 0.5, '', 													0, 1, 'C' );
		$this->cell( $nrTamanhoIntervaloGrande, 0.5, '', 							0, 0, 'C' );
		$this->cell( 13, 0.5, '', 													1, 0, 'C', 1 );
		$this->cell( $nrTamanhoIntervaloGrande, 0.5, '', 							0, 1, 'C' );
		$this->cell( $nrTamanhoBox, 0.5, 'LOCALIDADE, UF E DATA', 					0, 1, 'C' );
		$this->cell( 0.2, 0.5, '', 													0, 0, 'C' );
		$this->cell( 9.2, 0.5, $arDadosCabecalho['dirigente'], 								1, 0, 'C', 1 );
		$this->cell( $nrTamanhoIntervaloPequeno, 0.5, '', 							0, 0, 'C' );
		$this->cell( 9.2, 0.5, '', 													1, 0, 'C', 1 );
		$this->cell( 0.2, 0.5, '', 													0, 1, 'C' );
		$this->cell( 9.5, 0.5, 'NOME DO DIRIGENTE OU REPRESENTANTE LEGAL', 			0, 0, 'C' );
		if( $this->CurOrientation == 'L' ){
			$this->cell( 8.5, 0.5, '', 												0, 0, 'C' );
		}
		$this->cell( 9.5, 0.5, 'ASSINATURA DO DIRIGENTE OU REPRESENTANTE LEGAL', 	0, 1, 'C' );
		
	}
	

}
?>