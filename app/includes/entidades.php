<?php
//
// $Id$
//

include_once APPRAIZ . "adodb/adodb.inc.php";
include_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
include_once APPRAIZ . "includes/ActiveRecord/Decorator.php";
include_once APPRAIZ . "seguranca/modulos/sistema/geral/endereco.inc";



define('PESSOA_JURIDICA',      1);
define('PESSOA_FISICA',        2);

define('ENT_PREFEITURA',       1); //$db->pegaUm("SELECT funid FROM entidade.funcao WHERE UPPER(fundsc) = 'PREFEITURA'"));
define('ENT_PREFEITO',         2); //$db->pegaUm("SELECT funid FROM entidade.funcao WHERE UPPER(fundsc) = 'PREFEITO'"));
define('ENT_ESCOLA',           3); //$db->pegaUm("SELECT funid FROM entidade.funcao WHERE UPPER(fundsc) = 'ESCOLA'"));
define('ENT_UNIVERSIDADE',    12); //$db->pegaUm("SELECT funid FROM entidade.funcao WHERE UPPER(fundsc) = 'UNIVERSIDADE'"));

define('ENT_PESSOA_JURIDICA',  1);
define('ENT_PESSOA_FISICA',    2);

define('ENT_FORM_CADASTRO',    1);
define('ENT_FORM_BUSCA',       2);
define('ENT_FORM_LEITURA',     4);



/**
 * Efetua a buscar de entidades no banco de dados, de acordo com a função
 * frmBuscarEntidade
 *
 * @see frmBuscarEntidade
 * @return array Array com as entidades encontradas, agrupadas pela funcao
 *          relacionada
 *      array (
 *        FUNID_1 => array (
 *          0 => ENTID_X
 *        ),
 *        FUNID_2 => array (
 *          0 => ENTID_Y
 *        )
 *      )
 */
function buscarEntidade()
{
    $sql = '
    SELECT
        entid
    FROM
        cte.entidade';
}



/**
 * 
 */
function frmBuscarEntidade($tiposBusca = array(), $tiposEntidade = array()) {}

/**
 * Processa a entidade e salva os dados
 * @param integer $entid ID da entidade a ser salva
 * @param array $dados Array de dados a serem salvos na entidade
 * @return Entidade A entidade salva
 * @throw Exception Em caso de erros
 */
function salvarEntidade(array $dados, $entid = null){}

function tabEscola($entid=null){
	global $db;
	
	if (!$entid){
		return false;
	}
	
	$css = '<style type="text/css">
				@charset "iso-8859-1";
				/* CSS Document */
				body{
				margin:0px;
				padding:0px;
				margin-left:10px;
				width:740px;
				}
				th, td{
				border: 1px solid #fff;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				}
				h1{
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:14px;
				color:#6E8D62;
				text-transform: uppercase;
				}
				img{
				border:none;
				}
				caption {
				display:none;
				}
				.upper {
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:8px;
				color:#000000;
				text-transform: uppercase;
				}
				#indicadores_titulo{
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:10px;
				width:721px;
				font-weight:bold;
				margin-top:5px;
				text-align:left;
				}
				#indicadores_titulo img{
				margin-right:0px;
				}
				#texto_indicadores{
				margin-top:10px;
				width:745px;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:11px;
				text-align:justify;
				}
				.back_button a{
				font-size: 12px;
				font-weight: normal;
				color: #2B729D;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				text-decoration:none;
				}
				.back_button avisited{
				font-size: 12px;
				font-weight: normal;
				color: #2B729D;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				text-decoration:none;
				}
				.back_button a:hover{
				font-size: 12px;
				font-weight: normal;
				color: #2B729D;
				font-family:Verdana, Arial, Helvetica, sans-serif;
				text-decoration:underline;
				}
				#print{
				width:80px;
				text-align:right;
				height:20px;
				margin-top:5px;
				margin-bottom:5px;
				float:right;
				}
				#voltar{
					width:200px;
					text-align:right;
					height:20px;
					margin-right:20px;
					margin-top:50px;
					margin-bottom:5px;
					float:right;
					text-decoration:none;
				}
				
				#voltar a{
				text-decoration:none;
				}
				
				#voltar a:link{
				text-decoration:none;
				}
				
				#voltar a:hover{
				text-decoration:none;
				}
				
				
				table{
					width:721px;
					font-size:10px;
					float:left;
				}
				.th1{
				font-size:14px;
				color:#FFFFFF;
				text-align: left;
				height:30px;
				padding-left:10px;
				font-weight:bold;
				}
				
				/* Tabela 1*/
				#tb1{
				border: 3px double #aba476;
				background-color: #ecebd5;
				margin-bottom:14px;
				}
				#tb1 .th1{
				background-color:#aba476;
				}
				#tb1 .th2{
				height:30px;
				font-size:11px;
				text-align: center;
				background-color:#d4ce6e;
				}
				#tb1 .th3{
				font-weight:bold;
				text-align:center;
				height:30px;
				background-color:#DFDCB5;
				font-size:11px;
				}
				#tb1 .th4{
				font-weight:bold;
				text-align:center;
				background-color:#EFEAA4;
				font-size:11px;
				}
				#tb1 .th5{
				text-align:center;
				height:30px;
				}
				#tb1 .th5_blue{
				text-align:center;
				height:30px;
				color:#000099;
				font-weight: bold;
				}
				#tb1 .th5_red{
				text-align:center;
				height:30px;
				color:#CC3300;
				font-weight: bold;
				}
				#tb1 .th5_green{
				text-align:center;
				height:30px;
				color:#003300;
				font-weight: bold;
				}
				.th6{
				font-family:Verdana, Arial, Helvetica, sans-serif;
				font-size:9px;
				color:#000000;
				text-transform: uppercase;
				}
				#tb1 .th6{
				text-align:left;
				height:30px;
				font-weight:bold;
				padding-left:10px;
				background-color:#aba476;
				color:#FFFFFF;
				}
				#tb1 .th7{
				text-align:left;
				height:30px;
				font-weight:bold;
				background-color:#aba476;
				color:#FFFFFF;
				}
				#tb1 .th3_red{
				font-weight:bold;
				text-align:center;
				background-color:#edebbd;
				height:30px;
				font-size:10px;
				color: #CC3300;
				}
				#tb1 .th3_green{
				font-weight:bold;
				text-align:center;
				background-color:#D1DFC3;
				height:30px;
				font-size:10px;
				color: #003300;
				}
				/* Fim da Tabela 1*/
				
				/* Tabela 2*/
				#tb2{
				border: 3px double #6E8D62;
				background-color: #e9f0e3;
				margin-bottom:14px;
				}
				#tb2 .th1{
				background-color:#6E8D62;
				}
				#tb2 .th2{
				height:30px;
				font-size:11px;
				text-align: center;
				background-color:#B3BFA7;
				}
				#tb2 .th3{
				font-weight:bold;
				text-align:center;
				background-color:#D1DFC3;
				height:30px;
				font-size:11px;
				}
				#tb2 .th3_red{
				font-weight:bold;
				text-align:center;
				background-color:#D1DFC3;
				height:30px;
				font-size:10px;
				color: #CC3300;
				}
				#tb2 .th3_green{
				font-weight:bold;
				text-align:center;
				background-color:#D1DFC3;
				height:30px;
				font-size:10px;
				color: #003300;
				}
				#tb2 .th4{
				font-weight:bold;
				text-align:center;
				background-color:#B3CC99;
				height:30px;
				font-size:11px;
				}
				#tb2 .th5{
				text-align:center;
				height:30px;
				}
				#tb2 .th52{
				height:30px;
				}
				#tb2 .th5_blue{
				text-align:center;
				height:30px;
				color:#000099;
				font-weight: bold;
				}
				#tb2 .th5_red{
				text-align:center;
				height:30px;
				color:#CC3300;
				font-weight: bold;
				}
				#tb2 .th5_green{
				text-align:center;
				height:30px;
				color:#003300;
				font-weight: bold;
				}
				#tb2 .th6{
				text-align:left;
				height:30px;
				font-weight:bold;
				padding-left:10px;
				background-color:#6E8D62;
				color:#FFFFFF;
				}
				/* Fim da Tabela 2*/
				
			</style>';
	
	$sql = "SELECT
				--edt.*,
				--e.entid,
				edt.entcodent,
				entnome,
				entemail,
				-- entnumdddcelular,
				-- entnumcelular,
				entnumfax,
			--	entnumramalfax,
				entnumdddcomercial,
				entnumcomercial as telefone,
			--	entnumramalcomercial,
			--	entnumdddresidencial,
			--	entnumresidencial,
				endlog,
				endcom,
				endbai,
				endnum,
				to_char(endcep::float,'99999-999') AS endcep,
				mundescricao,
				CASE
				  WHEN e.tpcid = 1 THEN 'Estadual'
				  ELSE 'Municipal'
				END AS rede,
				tl.tpldesc AS zona		
			FROM
				entidade.entidade e
			  --INNER JOIN entidade.entidadeendereco ee ON ee.entid = e.entid
				INNER JOIN entidade.endereco ed ON ed.entid = e.entid
				INNER JOIN territorios.municipio m ON m.muncod = ed.muncod
				INNER JOIN entidade.entidadedetalhe edt ON edt.entid = e.entid 
				INNER JOIN entidade.tipolocalizacao tl ON tl.tplid = e.tplid		
			WHERE
				e.entid = {$entid}
				".($_REQUEST['entcodent'] ? " AND e.entcodent = '".$_REQUEST['entcodent']."'" : '');
	
	$col = $db->pegaLinha($sql);
	
	if (!$col){
		die("<script>
				alert('Não há registro!');
				window.close();
			 </script>");
	}
	
	$html = '<table cellspacing="0" cellpadding="1" border="1" summary="Número de escolas em áreas específicas - Rede Estadual 2007 - Zona Rural" id="tb2">
				<caption>
				Escolas - Zona Rural - Rede Municipal - 2007</caption>
				<thead>
					<tr>
				    <th id="h1" class="th1" colspan="22">'.$col['entnome'].'</th>
				  </tr>
				  </thead>
				   <tbody>
				   <tr>
				      <th id="h3" class="th2" colspan="2">Código da Escola
				      </th><td headers="h1 h3" class="th7" colspan="8">'.$col['entcodent'].'</td>
				      <th id="h3" class="th2" colspan="2">Município
				      </th><td headers="h1 h3" class="th7" colspan="10">'.strtoupper($col['mundescricao']).'</td>
				    </tr>
				    <tr>
				      <th id="h3" class="th2">Endereço
				      </th><td headers="h1 h3" class="th7" colspan="5">'.strtoupper($col['endlog']).'</td>
				      <th id="h3" class="th2" colspan="1">Complem.
				      </th><td headers="h1 h3" class="th7" colspan="2">'.($col['endcom'] ? strtoupper($col['endcom']) : '&nbsp;').'</td>
				      <th id="h3" class="th2" colspan="1">N°</th><td headers="h1 h3" class="th7">'.($col['endnum'] ? $col['endnum'] : '&nbsp;').'</td>
				      
				      <th headers="h1 h3" id="h32" class="th2">Bairro</th>
				      <td colspan="10" headers="h1 h3 h32" class="th7">'.strtoupper($col['endbai']).'</td>
				    </tr>
				    <tr>
				      <th id="h4" class="th2">Rede
				      </th><td colspan="5" headers="h1 h4" class="th7">'.strtoupper($col['rede']).'</td>
				      <th colspan="1" id="h41" class="th2">Zona</th>
				      <td headers="h1 h4 h41" class="th7" colspan="3">'.strtoupper($col['zona']).'</td>
				      <th id="h42" class="th2">CEP</th>
				      <td colspan="11" headers="h1 h4 h42" class="th7">'.$col['endcep'].'</td>
				    </tr>
				    <tr>
				      <th id="h6" class="th2" rowspan="2">Contato        
				      </th><th id="h61" class="th3">DDD</th>
				      <th colspan="2" id="h62" class="th3">Telefone</th>
				      <th colspan="3" id="h63" class="th3"><p>Telefone<br/>
				      Público <br/>
				      1</p></th>
				      <th colspan="3" id="h64" class="th3"><p>Telefone<br/>Público 2</p></th>
				      <th id="h65" class="th3">Fax</th>
				      <th colspan="11" id="h66" class="th3">E-mail</th>
				    </tr>   
				    <tr>
				      <td headers="h6 h61" class="th5">'.$col['entnumdddcomercial'].'</td>
				      <td colspan="2" headers="h6 h62" class="th5">'.($col['telefone'] ? $col['telefone'] : '-').'</td>
				      <td colspan="3" headers="h6 h63" class="th5">-</td>
				      <td colspan="3" headers="h6 h64" class="th5">-</td>
				      <td headers="h6 h65" class="th5">'.($col['entnumfax'] ? $col['entnumfax'] : '-').'</td>
				      <td colspan="11" headers="h6 h66" class="th5"> <a href="mailto:'.$col['entemail'].'">'.$col['entemail'].'</a></td>
				    </tr>
				<!--    
				    <tr>
				    <th id="h1" class="th4" colspan="22">Matrículas por Nível</th>
				  	</tr>
				    <tr>
				      <th id="h7" class="th3" colspan="3">Creche</th>
				      <th id="h8" class="th3" colspan="3"> Pré-escola </th>
				      <th colspan="3" id="h9" class="th3">Anos Iniciais <br/>
				      Ensino Fundamental</th>
				      <th colspan="2" id="h10" class="th3">Anos Finais<br/>
				      Ensino Fundamental</th>
				      <th id="h11" class="th3" colspan="3">Ensino Médio</th>
				      <th id="h12" class="th3" colspan="3">EJA Ensino Fundamental</th>
				      <th id="h13" class="th3" colspan="5">EJA Ensino Médio</th>         
				      </tr>
				    <tr>
				      <td class="th5" colspan="3"> 0</td>   
				      <td class="th5" colspan="3"> 30</td>
				      <td class="th5" colspan="3"> 82</td>
				      <td class="th5" colspan="2"> 0</td>
				      <td class="th5" colspan="3"> 0</td>
				      <td class="th5" colspan="3"> 18</td>
				    <td class="th5" colspan="5"> 0</td>
				    </tr>
				    <tr>
				    <th height="38" id="h1" class="th4" colspan="22">IDEB</th>
				  </tr>
				 
				    <tr>
				      <td height="24" id="h7" class="th4" colspan="9">Anos Iniciais do Ensino Fundamental</td>
				      <td id="h7" class="th4" colspan="13">Anos Finais do Ensino Fundamental</td>
				     </tr>
				    <tr>
				      <td id="h14" class="th4" colspan="5">IDEB Observado</td>
				      <td id="h14" class="th4" colspan="4">Metas</td>
				      <td id="h7" class="th4" colspan="7">IDEB Observado</td>
				      <td id="h7" class="th4" colspan="6">Metas</td>
				    </tr>
				    <tr>
				      <td id="h14" class="th4" colspan="2">2005</td>
				      <td id="h14" class="th4" colspan="3">2007</td>
				      <td id="h15" class="th4" colspan="2">2007</td>
				      <td id="h15" class="th4" colspan="2">2021</td>
				      <td id="h7" class="th4" colspan="3">2005</td>
				      <td id="h7" class="th4" colspan="4">2007</td>
				      <td id="h7" class="th4" colspan="3">2007</td>
				      <td id="h7" class="th4" colspan="3">2021</td>
				     </tr>
				    <tr>
				      <td id="h14" class="th5" colspan="2">  - </td>
				      <td id="h14" class="th5" colspan="3">  - </td>
				      <td id="h23" class="th5" colspan="2">  - </td>
				      <td id="h15" class="th5" colspan="2">  - </td>
				      <td id="h7" class="th5" colspan="3">  - </td>
				      <td id="h7" class="th5" colspan="4">  - </td>
				      <td id="h7" class="th5" colspan="3">  - </td>
				      <td id="h7" class="th5" colspan="3">  - </td>
				     </tr>
				-->     
				<!--          <tr>
				      <td colspan="2" class="th5" id="h14">-</td>
				      <td colspan="3" class="th5" id="h14">-</td>
				      <td colspan="2" class="th5" id="h23">-</td>
				      <td colspan="2" class="th5" id="h15">-</td>
				      <td colspan="3" class="th5" id="h7">-</td>
				      <td colspan="4" class="th5" id="h7">-</td>
				      <td colspan="3" class="th5" id="h7">-</td>
				      <td colspan="3" class="th5" id="h7">-</td>
				     </tr>
				-->  </tbody>
				</table>';
	
	return $css.$html;
}




/**
 * Mexe nisso nao! :P
 */
function formEntidade(Entidade $entidade     = null,
                               $formAction   = null,
                               $tipoEntidade = PESSOA_JURIDICA,
                               $formEndereco = true,
                               $formEscola   = false,
                               $cadPrefeito  = false,
                               $editavel     = true,
                               array $funids = array(),
                               $universidade = false,
                               $uniescola    = false)
{

	require APPRAIZ . "www/includes/webservice/pj.php";
	require APPRAIZ . "www/includes/webservice/cpf.php";	
    static $totalForms = 0;
    $urldefault = $_SERVER["REQUEST_URI"];
    
//    echo substr($formAction, 0, strpos($formAction, 'acao'));
    
    if (!($entidade instanceof Entidade)) {
        $entidade = new Entidade((integer) $entidade);
    }

    if ($entidade->entnumcpfcnpj != null) {
        $tipoPessoa = strlen($entidade->entnumcpfcnpj) == 11 ? PESSOA_FISICA  :
                                                               PESSOA_JURIDICA;
    }

    $formEscola = $formEscola && $tipoEntidade == PESSOA_JURIDICA;

    global $db,
           $entid,
           $funid,
           $njuid,
           $entidassociado,
           $entnumcpfcnpj,
           $entnome,
           $entemail,
           $entnuninsest,
           $entobs,
           $entstatus,
           $entnumrg,
           $entorgaoexpedidor,
           $entsexo,
           $entdatanasc,
           $entdatainiass,
           $entdatafimass,
           $entnumdddresidencial,
           $entnumresidencial,
           $entnumdddcelular,
           $entnumcelular,
           $entnumdddcomercial,
           $entnumramalcomercial,
           $entnumcomercial,
           $entnumdddfax,
           $entnumramalfax,
           $entnumfax,
           $entcodent,
           $tpctgid,
           $tpcid,
           $tplid,
           $entunicod,
           $tpsid;

    $superUser                  = $db->testa_superuser();

    $entid                      = $entidade->getPrimaryKey();
    $funid                      = $entidade->funid;
    $njuid                      = $entidade->njuid;
    $entidassociado             = $entidade->entidassociado;
    $entnumcpfcnpj              = $entidade->entnumcpfcnpj;
    $entnome                    = $entidade->entnome;
    $entemail                   = $entidade->entemail;
    $entnuninsest               = $entidade->entnuninsest;
    $entobs                     = $entidade->entobs;
    $entstatus                  = $entidade->entstatus;
    $entnumrg                   = $entidade->entnumrg;
    $entorgaoexpedidor          = $entidade->entorgaoexpedidor;
    $entsexo                    = $entidade->entsexo;
    $entcodent                  = $entidade->entcodent;

    //$entdatanasc                = date("d/m/Y", strtotime($entidade->entdatanasc));
    //$entdatainiass              = date("d/m/Y", strtotime($entidade->entdatainiass));
    //$entdatafimass              = date("d/m/Y", strtotime($entidade->entdatafimass));

    $entdatanasc                = $entidade->entdatanasc;
    $entdatainiass              = $entidade->entdatainiass;
    $entdatafimass              = $entidade->entdatafimass;

    $entnumdddresidencial       = $entidade->entnumdddresidencial;
    $entnumresidencial          = $entidade->entnumresidencial;
    $entnumdddcelular			= $entidade->entnumdddcelular;
    $entnumcelular				= $entidade->entnumcelular;
    $entnumdddcomercial         = $entidade->entnumdddcomercial;
    $entnumramalcomercial       = $entidade->entnumramalcomercial;
    $entnumcomercial            = $entidade->entnumcomercial;
    $entnumdddfax               = $entidade->entnumdddfax;
    $entnumramalfax             = $entidade->entnumramalfax;
    $entnumfax                  = $entidade->entnumfax;
    $entunicod                  = $entidade->entunicod;


    /*!@
     * PESSOA_JURIDICA
     *  name="entnumcpfcnpj" id="entnumcpfcnpj"
     *  name="entnuninsest"  id="entnuninsest"
     *  name="entnome"       id="entnome"
     *  name="entemail"      id="entemail"
     *  name="njuid"         id="njuid"
     *  name="funid"         id="funid"
     *  name="entunicod"     id="entunicod"
     *
     * PESSOA_FISICA
     *  name="entnumcpfcnpj"     id="entnumcpfcnpj"
     *  name="entnome"           id="entnome"
     *  name="entemail"          id="entemail"
     *  name="entnumrg"          id="entnumrg"
     *  name="entorgaoexpedidor" id="entorgaoexpedidor"
     *  name="entsexo"           id="entsexo"
     *  name="entdatanasc"       id="entdatanasc"
     *
     *  @prefeito - $entidade->funid == 2
     *    name="entdatainiass" id="entdatainiass"
     *    name="entdatafimass" id="entdatafimass"
     *  !prefeito
     *
     *  name="entnumdddresidencial" id="entnumdddresidencial"
     *  name="entnumresidencial"    id="entnumresidencial"
     //                                                                     */

    $input_entid                = campo_texto('entid',                'S', 'S', 'entid',                '   ', '   ', '',                   '', 'left', '', 0, 'id="entid"');
    $input_funid                = campo_texto('funid',                'S', 'S', 'Função',               '   ', '   ', '',                   '', 'left', '', 0, 'id="funid"');
    $input_entidassociado       = campo_texto('entidassociado',       'S', 'S', 'entidassociado',       '   ', '   ', '',                   '', 'left', '', 0, 'id="entidassociado"');
    $input_entnome              = campo_texto('entnome',              'S', 'S', 'Nome',                 '60 ', '255', '',                   '', 'left', '', 0, 'id="entnome" onblur="MouseBlur(this);"');
    $input_entemail             = campo_texto('entemail',             'N', 'S', 'E-Mail',               '60 ', '100', '',                   '', 'left', '', 0, 'id="entemail" onblur="MouseBlur(this);"');
    $input_entnumdddfax         = campo_texto('entnumdddfax',         'N', 'S', 'DDD Fax',              '3  ', '2  ', '##',                 '', 'left', '', 0, 'id="entnumdddfax" onblur="MouseBlur(this);"');
    $input_entnumfax            = campo_texto('entnumfax',            'N', 'S', 'Num Fax',              '12 ', '9  ', '####-####',          '', 'left', '', 0, 'id="entnumfax" onblur="MouseBlur(this);"');
    $input_entnumramalfax       = campo_texto('entnumramalfax',       'N', 'S', 'Ramal',                '6  ', '4  ', '####',               '', 'left', '', 0, 'id="entnumramalfax" onblur="MouseBlur(this);"');

    $input_entobs               = campo_textarea('entobs',            'N', 'S', 'Observações',          '45 ', '5  ', 500);
    $input_entstatus            = campo_radio('entstatus', array('Ativo'   => array('valor' => 'A',
                                                                                    'id'    => 'entstatus_a'),
                                                                 'Inativo' => array('valor' => 'I',
                                                                                    'id'    => 'entstatus_i')), 'h', true);

    $form = ''
            . '<script type="text/javascript" src="/includes/prototype.js"></script>'
            . '<script type="text/javascript" src="/includes/entidades.js"></script>'
            . '<form id="frmEntidade" method="post" action="' . $formAction . '" onsubmit="return Entidade.submeterFrmEntidade(this);">'
            . '  <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">'
            . '    <tbody id="tableEntidade">'
            . '      <tr>'
            . '        <td style="font-weight: bold" colspan="2">Dados cadastrais</td>'
            . '      </tr>';

    if ($tipoEntidade == PESSOA_JURIDICA) {
    	
        $input_entnumdddcomercial   = campo_texto('entnumdddcomercial',   'S', 'S', 'DDD Com',              '3  ', '2  ', '##',                 '', 'left', '', 0, 'id="entnumdddcomercial" onblur="MouseBlur(this);"');
        $input_entnumcomercial      = campo_texto('entnumcomercial',      'S', 'S', 'Num Com',              '12 ', '9  ', '####-####',          '', 'left', '', 0, 'id="entnumcomercial" onblur="MouseBlur(this);"');
        $input_entnumramalcomercial = campo_texto('entnumramalcomercial', 'N', 'S', 'Ramal',                '6  ', '4  ', '####',               '', 'left', '', 0, 'id="entnumramalcomercial" onblur="MouseBlur(this);"');
        $input_entnuninsest         = campo_texto('entnuninsest',         'N', 'S', 'Inscrição Estadual',   '22 ', '14 ', '',                   '', 'left', '', 0, 'id="entnuninsest" onblur="MouseBlur(this);"');
        $input_njuid                = $db->monta_combo('njuid', 'SELECT njuid as codigo, njudsc as descricao FROM entidade.naturezajuridica ORDER BY descricao', 'S', 'Selecione', '', '', 'Natureza Jurídica', '', 'N', 'njuid', true);

        if ($formEscola && !$universidade && !$uniescola) {
        	
            global $entcodent_radio;
            $input_entnumcpfcnpj        = campo_texto('entnumcpfcnpj',        'S', 'S', 'CNPJ',                 '22 ', '18 ', '##.###.###/####-##', '', 'left', '', 0, 'id="entnumcpfcnpj" onblur="MouseBlur(this);"');
            $input_entcodent            = campo_texto('entcodent',            'N', 'S', 'Código da escola',     '22 ', '9  ', '',                   '', 'left', '', 0, 'id="entcodent" onblur="MouseBlur(this);"');

            $entidadeDetalhe            = new EntidadeDetalhe();
            $entidadeDetalhe->carregar($entidade->entcodent);

            $entcodent_radio        = (string) $entidade->entcodent == '' ? 1 : 0;
            $input_entcodent        = campo_texto('entcodent', 'N', 'S', 'Código da escola', '22', '9', '', '', 'left', '', 0, 'id="entcodent" onblur="MouseBlur(this);"');
            $input_entcodent_radios = campo_radio('entcodent_radio', array('Código da escola (INEP)'    => array('valor'    => '0',
                                                                                                                 'callback' => 'Element.hide(\'tr_entnumcpfcnpj_container\'); Element.show(\'tr_entcodent_container\'); $(\'entcodent\').activate();',
                                                                                                                 'id'       => 'entcodent_1'),
                                                                           'CNPJ'                       => array('valor'    => '1',
                                                                                                                 'callback' => 'Element.show(\'tr_entnumcpfcnpj_container\'); Element.hide(\'tr_entcodent_container\'); $(\'entnumcpfcnpj\').activate();',
                                                                                                                 'id'       => 'entcodent_0')), 'h', true);

            $form  .= ''
                    . '      <tr class="entcodent_container_radios" id="tr_entcodent_container_radios">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Tipo de busca:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entcodent_radios
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entcodent_container" id="tr_entcodent_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Código da escola (INEP):</label></td>'
                    . '        <td>'
                    . '          ' . $input_entcodent
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entnumcpfcnpj_container" id="tr_entnumcpfcnpj_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CNPJ:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entnumcpfcnpj
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entnuninsest_container" id="tr_entnuninsest_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Inscrição Estadual:</label></td>'
                    . '        <td>';

        } elseif ( $formEscola && $uniescola ){
        	
        	global $entcodent_radio;
            
        	$input_entnumcpfcnpj        = campo_texto('entnumcpfcnpj',        'S', 'S', 'CNPJ',                 '22 ', '18 ', '##.###.###/####-##', '', 'left', '', 0, 'id="entnumcpfcnpj" onblur="MouseBlur(this);"');
            $input_entcodent            = campo_texto('entcodent',            'N', 'S', 'Código da escola',     '22 ', '9  ', '',                   '', 'left', '', 0, 'id="entcodent" onblur="MouseBlur(this);"');
			$input_entunicod            = campo_texto('entunicod', 			  'N', 'S', 'Código da unidade',	'22 ', '5  ', '#####', 				'', 'left', '', 0, 'id="entunicod" onblur="MouseBlur(this);"');
			
            $entidadeDetalhe            = new EntidadeDetalhe();
            $entidadeDetalhe->carregar($entidade->entcodent);
        	
            $entcodent_radio        = (string) $entidade->entcodent == '' ? 1 : 0;
            $input_entcodent        = campo_texto('entcodent', 'N', 'S', 'Código da escola', '22', '9', '', '', 'left', '', 0, 'id="entcodent" onblur="MouseBlur(this);"');
            $input_entcodent_radios = campo_radio('entcodent_radio', array('Código da escola (INEP)'    => array('valor'    => '0',
                                                                                                                 'callback' => 'Element.hide(\'tr_entnumcpfcnpj_container\'); Element.show(\'tr_entcodent_container\'); Element.hide(\'tr_entunicod_container\'); $(\'entcodent\').activate();',
                                                                                                                 'id'       => 'entcodent_1'),
                                                                           'CNPJ'                       => array('valor'    => '1',
                                                                                                                 'callback' => 'Element.show(\'tr_entnumcpfcnpj_container\'); Element.hide(\'tr_entcodent_container\'); Element.hide(\'tr_entunicod_container\'); $(\'entnumcpfcnpj\').activate();',
                                                                                                                 'id'       => 'entcodent_0'),
            															   'Código da Unidade'          => array('valor'    => '2',
                                                                                                                 'callback' => 'Element.show(\'tr_entunicod_container\'); Element.hide(\'tr_entnumcpfcnpj_container\'); Element.hide(\'tr_entcodent_container\'); $(\'entunicod\').activate(); $(\'entunicod\').removeAttribute(\'disabled\'); $(\'entunicod\').focus();',
                                                                                                                 'id'       => 'entcodent_2')), 'h', true);
        	
        	$form  .= ''
                    . '      <tr class="entcodent_container_radios" id="tr_entcodent_container_radios">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Tipo de busca:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entcodent_radios
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entcodent_container" id="tr_entcodent_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Código da escola (INEP):</label></td>'
                    . '        <td>'
                    . '          ' . $input_entcodent
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entnumcpfcnpj_container" id="tr_entnumcpfcnpj_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CNPJ:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entnumcpfcnpj
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entunicod_container" id="tr_entunicod_container" style="display:none;">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Código da Unidade:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entunicod
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entnuninsest_container" id="tr_entnuninsest_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Inscrição Estadual:</label></td>'
                    . '        <td>';
        	
        } elseif ($universidade) {
            $input_entunicod            = campo_texto('entunicod', 'N', 'S', 'Código da unidade', '22 ', '5', '#####', '', 'left', '', 0, 'id="entunicod" onblur="MouseBlur(this);"');

            $form  .= ''
                    . '      <tr class="entnumcpfcnpj_container" id="tr_entnumcpfcnpj_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Código da Unidade Orçamentária:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entunicod
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entnuninsest_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CNPJ:</label></td>'
                    . '        <td>';
        } else {
            $input_entnumcpfcnpj = campo_texto('entnumcpfcnpj',        'S', 'S', 'CNPJ',                 '22 ', '18 ', '##.###.###/####-##', '', 'left', '', 0, 'id="entnumcpfcnpj" onblur="MouseBlur(this);"');
            $form  .= ''
                    . '      <tr class="entnumcpfcnpj_container" id="tr_entnumcpfcnpj_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CNPJ:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entnumcpfcnpj
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entnuninsest_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Inscrição Estadual:</label></td>'
                    . '        <td>';
        }

        if ($formEscola && !$universidade) {
            $form  .= ''
                    . '  <script type="text/javascript">'
                    . '    Element.hide($(\'tr_entcodent_container\'));'
                    . '    Element.show($(\'tr_entnumcpfcnpj_container\'));'
                    . '    Element.show($(\'entcodent_1\'));'
                    . '  </script>';
        }

        $form  .= ''
                . '          ' . $input_entnuninsest
                . '        </td>'
                . '      </tr>';

        if ($formEscola && !$universidade) {
            $form .= ''
                   . '      <tr class="entescolanova_container" id="tr_entescolanova_container">'
                   . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label for="entescolanova">Nova escola (não possui código INEP):</label></td>'
                   . '        <td>'
                   . '          <input onchange="return Entidade.cadastrarEscolaNova(this);" type="checkbox" id="entescolanova" name="entescolanova"' . ($entidade->entescolanova == 't' ? ' checked="checked"' : '') . ' />'
                   . '        </td>'
                   . '      </tr>';
        }

        $form  .= ''
                . '      <tr class="entnome_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Nome:</label></td>'
                . '        <td>'
                . '          ' . $input_entnome
                . '        </td>'
                . '      </tr>'
                . '      <tr class="entemail_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>E-Mail:</label></td>'
                . '        <td>'
                . '          ' . $input_entemail
                . '        </td>'
                . '      </tr>'
                . '      <tr class="njuid_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Natureza jurídica:</label></td>'
                . '        <td>'
                . '          ' . $input_njuid
                . '        </td>'
                . '      </tr>';

        if (($count = sizeof($funids)) > 0) {
            $form  .= ''
                    . '      <tr>'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Tipo de entidade:</label></td>'
                    . '        <td>'
                    . '          <select class="CampoEstilo" name="funid" id="funid">';
            if ($count > 1)
                $form  .= '<option value="">Selecione</option>';

            while (list($funid, $fundsc) = each($funids)) {
                $form  .= '<option value="' . $funid . '" ' . ($funid == $entidade->funid ? ' selected="selected">' : '>') . $fundsc . '</option>';
            }

            $form  .= ''
                    . '          </select>'
                    . '        </td>'
                    . '      </tr>';
        } else {
            $form  .= '  <input type="hidden" name="funid" id="funid" value="' . $entidade->funid . '" />';
        }

    } else {
        $input_entnumcpfcnpj        = campo_texto('entnumcpfcnpj',        'S', 'S', 'CPF',                  '20 ', '14 ', '###.###.###-##',     '', 'left', '', 0, 'id="entnumcpfcnpj" onblur="MouseBlur(this);"');
        $input_entnumrg             = campo_texto('entnumrg',             'N', 'S', 'RG',                   '15 ', '15 ', '',                   '', 'left', '', 0, 'id="entnumrg" onblur="MouseBlur(this);"');
        $input_entorgaoexpedidor    = campo_texto('entorgaoexpedidor',    'N', 'S', 'Órgão Expeditor',      '15 ', '10 ', '',                   '', 'left', '', 0, 'id="entorgaoexpedidor" onblur="MouseBlur(this);"');
        $input_entdatanasc          = campo_texto('entdatanasc',          'N', 'S', 'Data de Nascimento',   '12 ', '10 ', '##/##/####',         '', 'left', '', 0, 'id="entdatanasc" onblur="MouseBlur(this);"');
        $input_entdatainiass        = campo_texto('entdatainiass',        'N', 'S', 'Início do Mandato',    '12 ', '10 ', '##/##/####',         '', 'left', '', 0, 'id="entdatainiass" onblur="MouseBlur(this);"');
        $input_entdatafimass        = campo_texto('entdatafimass',        'N', 'S', 'Fim do Mandato',       '12 ', '10 ', '##/##/####',         '', 'left', '', 0, 'id="entdatafimass" onblur="MouseBlur(this);"');
        $input_entnumdddresidencial = campo_texto('entnumdddresidencial', 'N', 'S', 'DDD Res',              '3  ', '2  ', '##',                 '', 'left', '', 0, 'id="entnumdddresidencial" onblur="MouseBlur(this);"');
        $input_entnumresidencial    = campo_texto('entnumresidencial',    'N', 'S', 'Num Res',              '12 ', '9  ', '####-####',          '', 'left', '', 0, 'id="entnumresidencial" onblur="MouseBlur(this);"');
        $input_entnumdddcelular 	= campo_texto('entnumdddcelular', 	  'N', 'S', 'DDD Cel',              '3  ', '2  ', '##',                 '', 'left', '', 0, 'id="entnumdddcelular" onblur="MouseBlur(this);"');
        $input_entnumcelular	    = campo_texto('entnumcelular',    	  'N', 'S', 'Num Cel',              '12 ', '9  ', '####-####',          '', 'left', '', 0, 'id="entnumcelular" onblur="MouseBlur(this);"');
        $input_entnumdddcomercial   = campo_texto('entnumdddcomercial',   'S', 'S', 'DDD Com',              '3  ', '2  ', '##',                 '', 'left', '', 0, 'id="entnumdddcomercial" onblur="MouseBlur(this);"');
        $input_entnumcomercial      = campo_texto('entnumcomercial',      'S', 'S', 'Num Com',              '12 ', '9  ', '####-####',          '', 'left', '', 0, 'id="entnumcomercial" onblur="MouseBlur(this);"');
        $input_entnumramalcomercial = campo_texto('entnumramalcomercial', 'N', 'S', 'Ramal',                '6  ', '4  ', '####',               '', 'left', '', 0, 'id="entnumramalcomercial" onblur="MouseBlur(this);"');

        $input_entsexo              = '<select name="entsexo" class="CampoEstilo" id="entsexo">'
                                    . '  <option value="" >Selecione</option>'
                                    . '  <option value="M">Masculino</option>'
                                    . '  <option value="F">Feminino</option>'
                                    . '</select>';

        $form  .= ''
                . '      <tr class="entnumcpfcnpj_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CPF:</label></td>'
                . '        <td>'
                . '          ' . $input_entnumcpfcnpj
                . '        </td>'
                . '      </tr>'
                . '      <tr class="entnome_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Nome:</label></td>'
                . '        <td>'
                . '          ' . $input_entnome
                . '        </td>'
                . '      </tr>'
                . '      <tr class="entemail_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>E-Mail</label></td>'
                . '        <td>'
                . '          ' . $input_entemail
                . '        </td>'
                . '      </tr>'
                . '      <tr class="entnumrg_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>RG:</label></td>'
                . '        <td>'
                . '          ' . $input_entnumrg
                . '        </td>'
                . '      </tr>'
                . '      <tr class="entorgaoexpedidor_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Orgão Expeditor:</label></td>'
                . '        <td>'
                . '          ' . $input_entorgaoexpedidor
                . '        </td>'
                . '      </tr>'
                . '      <tr class="entsexo_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sexo:</label></td>'
                . '        <td>'
                . '          ' . $input_entsexo
                . '          <script type="text/javascript">'
                . '            $(\'entsexo\').value = \'' . $entidade->entsexo . '\';'
                . '            $(\'entsexo\').select();'
                . '          </script>'
                . '        </td>'
                . '      </tr>'
                . '      <tr class="entdatanasc_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Data de nascimento:</label></td>'
                . '        <td>'
                . '          ' . $input_entdatanasc
                . '        </td>'
                . '      </tr>';

        if ($cadPrefeito || $entidade->funid == 2) {
            $form  .= ''
                    . '      <tr class="entdatainiass_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Data início mandato:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entdatainiass
                    . '        </td>'
                    . '      </tr>'
                    . '      <tr class="entdatafimass_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Data fim mandato:</label></td>'
                    . '        <td>'
                    . '          ' . $input_entdatafimass
                    . '        </td>'
                    . '      </tr>';
        }

        $form  .= ''
                . '      <tr class="entnumdddresidencial_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>(DDD) Telefone residencial:</label></td>'
                . '        <td>'
                . '          <span style="display: block; float: left; width: 45px";>' . $input_entnumdddresidencial . '</span><span style="display: block; float: left; width: 90px";>' . $input_entnumresidencial . '</span>'
                . '        </td>'
                . '      </tr>'
                . '      <tr class="entnumdddcelular_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>(DDD) Telefone celular:</label></td>'
                . '        <td>'
                . '          <span style="display: block; float: left; width: 45px";>' . $input_entnumdddcelular . '</span><span style="display: block; float: left; width: 90px";>' . $input_entnumcelular . '</span>'
                . '        </td>'
                . '      </tr>';
    }

    $form  .= ''
            . '      <tr class="entnumdddcomercial_container">'
            . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>(DDD) Telefone comercial</label></td>'
            . '        <td>'
            . '          <span style="display: block; float: left; width: 45px";>' . $input_entnumdddcomercial . '</span><span style="display: block; float: left; width: 90px";>' . $input_entnumcomercial . '</span> Ramal: ' . $input_entnumramalcomercial
            . '        </td>'
            . '      </tr>'
            . '      <tr class="entnumdddfax_container">'
            . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>(DDD) Fax:</label></td>'
            . '        <td>'
            . '          <span style="display: block; float: left; width: 45px";>' . $input_entnumdddfax . '</span><span style="display: block; float: left; width: 90px";>' . $input_entnumfax . '</span> Ramal: ' . $input_entnumramalfax
            . '        </td>'
            . '      </tr>'
            . '      <tr class="entobs_container">'
            . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Observações</label></td>'
            . '        <td>'
            . '          ' . $input_entobs
            . '        </td>'
            . '      </tr>';

    if ($formEscola && !$universidade) {
        /*!@
         * tipoclassificacao
         */
        $tpcid         = $entidade->tpcid;
        $input_tpcid   = $db->monta_combo('tpcid', 'SELECT tpcid as codigo, tpcdesc as descricao FROM entidade.tipoclassificacao WHERE tpcstatus = \'A\' OR tpcstatus IS NULL ORDER BY descricao', 'S', 'Selecione', '', '', 'Classificação', '', 'N', 'tpcid', true);

        $form  .= ''
                . '      <tr class="tpcid_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Classificação:</label></td>'
                . '        <td>'
                . '          ' . $input_tpcid
                . '        </td>'
                . '      </tr>';

        /*!@
         * tipocategoriaescolaprivada
         */
        $tpctgid       = $entidade->tpctgid;
        $input_tpctgid = $db->monta_combo('tpctgid', 'SELECT tpctgid as codigo, tpctgdesc as descricao FROM entidade.tipocategoriaescolaprivada WHERE tpctgstatus = \'A\' ORDER BY descricao', 'S', 'Selecione', '', '', 'Categoria Escola Privada', '', 'N', 'tpctgid', true);

        $form  .= ''
                . '      <tr class="tpctgid_container" id="tpctgid_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Categoria da escola privada:</label></td>'
                . '        <td>'
                . '          ' . $input_tpctgid
                . '        </td>'
                . '      </tr>';

        $tplid       = $entidade->tplid;
        $tpsid       = $entidade->tpsid;
        $input_tplid = $db->monta_combo('tplid', 'SELECT tplid as codigo, tpldesc as descricao FROM entidade.tipolocalizacao WHERE tplstatus = \'A\' ORDER BY descricao', 'S', 'Selecione', '', '', 'Localização', '', 'N', 'tplid', true);
        $input_tpsid = $db->monta_combo('tpsid', 'SELECT tpsid as codigo, tpsdesc as descricao FROM entidade.tiposituacao WHERE tpsstatus = \'A\' ORDER BY descricao', 'S', 'Selecione', '', '', 'Situação', '', 'N', 'tpsid', true);

        $form  .= ''
                . '      <tr class="tplid_container">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Localização:</label></td>'
                . '        <td>'
                . '          ' . $input_tplid
                . '        </td>'
                . '      </tr>';

        if ($entidade->entcodent != null) {
            $form  .= ''
                    . '      <tr class="tpsid_container">'
                    . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Situação:</label></td>'
                    . '        <td>'
                    . '          ' . $input_tpsid
                    . '        </td>'
                    . '      </tr>';
        }
    }

    if ($superUser) {
        $form  .= ''
                . '      <tr>'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Status:</label></td>'
                . '        <td>'
                . '          ' . $input_entstatus
                . '        </td>'
                . '      </tr>';

        if ($entstatus == '') {
            $form .= '<script type="text/javascript">$("entstatus_a").checked=true;</script>';
        }
    }

    /*!@
    if ($formEscola) {
        $form  .= ''
                . '      <tr>'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">Detalhes da escola</td>'
                . '        <td style="font-weight: bold;"><label style="display:block;padding:4px;" onclick="Element.toggle(\'entidade_detalhe_container\')">Clique aqui para editar os valores</label>'
                . '          <table id="entidade_detalhe_container" style="display:none">'
                . '            <tr>';

        $cont = 0;
        foreach ($entidadeDetalhe->getArrayCampos() as $campo) {
            $ignorados = array('entid',
                               'entcodent',
                               'entdnum_enem_media_total_2006',
                               'entdnum_medio_abandono_2005',
                               'id_lingua_indigena',
                               'id_mod_ens_regular',
                               'id_esp_medio_profissional');

            $numericos = array('entdnum_alunos_ed_comp_escola',
                               'entdnum_alunos_ed_comp_outra',
                               'entdnum_funcionarios',
                               'entdnum_salas_existentes',
                               'entdnum_salas_utilizadas',
                               'entdnum_comp_administrativos',
                               'num_comp_alunos',
                               'num_alunos_atend_escola',
                               'num_alunos_atend_outra_escola');

            if (in_array($campo, $ignorados) || in_array($campo, $numericos)) {
                continue;
            } else {
                $form  .= ''
                        . '              <td>'
                        . '                <input type="checkbox" ' . ($entidadeDetalhe->$campo != '' ? 'checked="checked"' : '') . ' name="entidadedetalhe[' . $campo . ']" id="entidadedetalhe_' . $campo . '" />'
                        . '                <label for="entidadedetalhe_' . $campo . '">' . str_replace(array('entd', 'ent', '_'), ' ', $campo) . '</label></td>';

                if ($cont >= 2) {
                    $cont = 0;
                $form  .= ''
                        . '            </tr>'
                        . '            <tr>';
                } else {
                    $cont++;
                }
            }
        }

        $form  .= '</tr>';

        foreach ($entidadeDetalhe->getArrayCampos() as $campo) {
            $numericos = array('entdnum_alunos_ed_comp_escola',
                               'entdnum_alunos_ed_comp_outra',
                               'entdnum_funcionarios',
                               'entdnum_salas_existentes',
                               'entdnum_salas_utilizadas',
                               'entdnum_comp_administrativos',
                               'num_comp_alunos',
                               'num_alunos_atend_escola',
                               'num_alunos_atend_outra_escola');

            if (in_array($campo, $numericos)) {
                $form  .= ''
                        . '<tr>'
                        . '  <td td align="right" class="SubTituloDireita"><label for="entidadedetalhe_' . $campo . '">' . str_replace(array('entd', '_'), ' ', $campo) . '</label></td>'
                        . '  <td colspan="2">'
                        . '    <input onkeyup="this.value=mascaraglobal(\'#######\',this.value);" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" onmouseover="MouseOver(this);" class="normal" type="text" name="entidadedetalhe[' . $campo . ']" id="entidadedetalhe_' . $campo . '" />'
                        . '  </td>'
                        . '</tr>';
            }
        }

        $form  .= ''
                //. '            </tr>'
                . '          </table>'
                . '        </td>'
                . '      </tr>';

    }
    //                                                                      */
    // Formulario para cadastro de Endereços
    if ($formEndereco) {
        $entidade->carregarEnderecos();

        if (!($entidade->enderecos[0] instanceof Endereco)) {
            $entidade->enderecos[0] = new Endereco();
        }
        $dadoslatitude = explode(".",$entidade->enderecos[0]->medlatitude);
        $dadoslongitude = explode(".",$entidade->enderecos[0]->medlongitude);

        $form  .= ''
                . '      <tr>'
                . '        <td style="font-weight: bold" colspan="2">Endereço</td>'
                . '      </tr>'
                . '      <tr>'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>'
                . '        <td>'
                . '          <input type="text" name="endereco[endcep]" onkeyup="this.value=mascaraglobal(\'##.###-###\', this.value);" onkeydown="return Entidade.__getEnderecoPeloCEPKeyDown(event)" onblur="Entidade.__getEnderecoPeloCEP(this);" class="CampoEstilo" id="endcep" value="' . $entidade->enderecos[0]->endcep . '" size="13" maxlength="10" /><img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0">'
                . '        </td>'
                . '      </tr>'
                . '      <tr id="escolha_logradouro_id" style="display:none">'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Selecione o Logradouro:</label></td>'
                . '        <td>'
                . '          <input readonly="readonly" type="text" name="endlog" class="CampoEstilo" id="endlog" value="' . $entidade->enderecos[0]->endlog . '" size="48" />'
                . '        </td>'
                . '      </tr>'
                . '      <tr>'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Logradouro:</label></td>'
                . '        <td>'
                . '          <input type="text" name="endereco[endlog]" class="CampoEstilo" id="endlogradouro" value="' . $entidade->enderecos[0]->endlog . '" size="48" />'
                . '        </td>'
                . '      </tr>'
                . '      <tr>'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>'
                . '        <td>'
                . '          <input type="text" name="endereco[endnum]" class="CampoEstilo" id="endnum" value="' . $entidade->enderecos[0]->endnum . '" size="5" maxlength="8" />'
                . '        </td>'
                . '      </tr>'
                . '      <tr>'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>'
                . '        <td>'
                . '          <input type="text" name="endereco[endcom]" class="CampoEstilo" id="endcom" value="' . $entidade->enderecos[0]->endcom . '" size="48" maxlength="100" />'
                . '        </td>'
                . '      </tr>'
                . '      <tr>'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>'
                . '        <td>'
                . '          <input type="text" name="endereco[endbai]" class="CampoEstilo" id="endbai" value="' . $entidade->enderecos[0]->endbai . '" />'
                . '        </td>'
                . '      </tr>'
                . '      <tr>'
                . '        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>'
                . '        <td>'
                . '          <input readonly="readonly" type="text" name="mundescricao" class="CampoEstilo" id="mundescricao" value="' . $entidade->enderecos[0]->getMunDescricao() . '" />'
                . '          <input type="hidden" name="endereco[muncod]" id="muncod" class="CampoEstilo" value="' . $entidade->enderecos[0]->muncod . '" />'

                //. '          <select readonly="readonly" name="endereco[muncod]" class="CampoEstilo" id="muncod" value="' . $entidade->enderecos[0]->muncod    . '">'
                //. '            <option value="null">Selecione</option>'
                //. '          </select>'

                . '          <input readonly="readonly" type="text" name="endereco[estuf]" class="CampoEstilo" id="estuf" value="' . $entidade->enderecos[0]->estuf . '" style="width: 5ex; padding-left: 2px" />'
                . '        </td>'
                . '      </tr>'
				. '<tr>
				   	<td class="SubTituloDireita">Latitude</td>
					<td><input type="text" name="endereco[graulatitude]" size="3" maxlength="2" value="'.$dadoslatitude[0].'" onKeyUp= "this.value=mascaraglobal(\'##\',this.value);"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="graulatitude" style="text-align : left; width:5ex;" title=\'\' /> ° <input type="text" name="endereco[minlatitude]" size="3" maxlength="2" value="'.$dadoslatitude[1].'" onKeyUp= "this.value=mascaraglobal(\'##\',this.value);"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="minlatitude"  style="text-align : left; width:5ex;" title=\'\' /> \' <input type="text" name="endereco[seglatitude]" size="3" maxlength="2" value="'.$dadoslatitude[2].'" onKeyUp= "this.value=mascaraglobal(\'##\',this.value);"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="seglatitude"  style="text-align : left; width:5ex;" title=\'\' /> \'\' <select name="endereco[pololatitude]" id="pololatitude" class="CampoEstilo" style="width: 40px;"><option value="N" '.((trim($dadoslatitude[3])=='N')?'selected':'').'>N</option><option value="S" '.((trim($dadoslatitude[3])=='S')?'selected':'').'>S</option></select></td>
				   </tr>
				   <tr>
					<td class="SubTituloDireita">Longitude</td>
					<td><input type="text" name="endereco[graulongitude]" size="3" maxlength="2" value="'.$dadoslongitude[0].'" onKeyUp= "this.value=mascaraglobal(\'##\',this.value);"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="graulongitude" style="text-align : left; width:5ex;" title=\'\' /> ° <input type="text" name="endereco[minlongitude]" size="3" maxlength="2" value="'.$dadoslongitude[1].'" onKeyUp= "this.value=mascaraglobal(\'##\',this.value);"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="minlongitude" style="text-align : left; width:5ex;" title=\'\' /> \' <input type="text" name="endereco[seglongitude]" size="3" maxlength="2" value="'.$dadoslongitude[2].'" onKeyUp= "this.value=mascaraglobal(\'##\',this.value);"  class="normal"  onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="seglongitude" style="text-align : left; width:5ex;" title=\'\' /> \'\' &nbsp W</td>
				   </tr>
				   <tr>
					<td class="SubTituloDireita"></td>
					<td><a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a> <input style="display:none;" type="text" name="endereco[endzoom]" id="endzoom" value="'.$entidade->enderecos[0]->endzoom.'"></td>
				  </tr>';
    }

    $form  .= ''
    		. ' <tr id="trUtil" style="display: none">
					<td class="SubTituloDireita"><div id="labelUtil"></div></td>
					<td><div id="campoUtil"></div></td>
				</tr>';
    
    $form  .= '    </tbody>'
            . '    <tr bgcolor="#C0C0C0">'
            . '    <td colspan=2>'
            . '  <input type="hidden" name="endid" id="endid" value="' . $entidade->enderecos[0]->endid . '" />'
            . '  <input type="hidden" name="entid" id="entid" value="' . $entidade->entid . '" />'
            . '  <input type="hidden" name="entidassociado" id="entidassociado" value="null" />'
            . '  <input type="hidden" name="funid" id="funid" value="' . $entidade->funid . '" />'
            . '  <div id="frmEntidadeIncluir" style="padding-top: 10px; text-align: center;">';

    if ($editavel) {
        $form  .= ''
                . '    <input id="editavel" type="hidden" name="editavel" value="true" />'
                . '    <input id="submitEntidade" type="submit" value="Salvar" />'
                . '    <input id="resetEntidade" type="reset" value="Cancelar" />';
    } else {
        $form  .= ''
                . '    <input id="editavel" type="hidden" name="editavel" value="" />';
    }

    $form  .= ''
            . '    <input id="closeWindow" type="button" onclick="window.close();" value="Fechar" />'
            . '  </span>'
            . '   </td>'
            . '   </tr>'
            . '  </table>'
            . '</form>';


    if ($tipoPessoa == PESSOA_JURIDICA) {
	    $form  .= '<script type="text/javascript">'
	            . 'if($("entnumcpfcnpj"))$("entnumcpfcnpj").value=mascaraglobal("##.###.###/####-##",$F("entnumcpfcnpj"));'
	            . '$("endcep").value=mascaraglobal("##.###-###",$F("endcep"));';
    } else {
    	$form  .= '<script src="/includes/funcoes.js"></script>';
	    $form  .= '<script type="text/javascript">'
	           . 'if($("entnumcpfcnpj"))$("entnumcpfcnpj").value=mascaraglobal("###.###.###-##",$F("entnumcpfcnpj"));';
	    if($formEndereco) {        
	            $form .= '$("endcep").value=mascaraglobal("##.###-###",$F("endcep"));';
	    }
    }

    if ($entidade->funcao != 4) {
        $form  .= 'if($("tpctgid_container"))Element.hide("tpctgid_container");';
    }

    if (!$editavel) {
        $form  .= ''
                . 'Form.disable("frmEntidade");'
                . 'if($("entnumcpfcnpj"))$("entnumcpfcnpj").removeAttribute("disabled");'
                . '$("closeWindow").removeAttribute("disabled");';
    }
    
    $form .= "if ($('entnumcpfcnpj')){\n";
    
    if ($tipoEntidade == PESSOA_JURIDICA){
    	$form .= "\n $('entnumcpfcnpj').onblur  = function (e)
					  {         
					            if ( this.value == '' || trim(this.value) == trim(this.defaultValue) || (this.value.length != 18))
					                return false;
					
					            var req = new Ajax.Request('".$urldefault."', {
					                                       method: 'post',
					                                       parameters: '&opt=buscarCnpj&entnumcpfcnpj=' + this.value,
					                                       onComplete: function (res)
					                                       {
					                                           if (res.responseText != 0) {
					                                               if (confirm('O CNPJ informado já se encontra cadastrado.".'\n'."' 
					                                               			   +'Deseja carregar o registro?'))
					                                               {
					                                                   window.location.href = '".$urldefault."&busca=entnumcpfcnpj&entid=' + res.responseText;
					                                               } else {
					                                                   $('entnumcpfcnpj').value = '';
					                                                   $('entnumcpfcnpj').activate();
					                                               }
					                                           }else{
					                                        	   var comp = new dCNPJ();
					                                        	   comp.buscarDados($('entnumcpfcnpj').value);
					                                        	   
																   if (comp.dados.no_igrejarial_rf != ''){	
					                                        	   		$('entnome').value = comp.dados.no_igrejarial_rf;
					                                        	   		$('entnome').readOnly = true;	
																   }else if ($('entnome')){
					                                        	   		$('entnome').readOnly = false;
					                                        	   }
																	   
																   for (i=0; i < $('njuid').options.length; i++) {
																	   if ($('njuid').options[i].value == comp.dados.co_natureza_juridica_rf){
																	   		$('njuid').options[i].selected = 'true';	   
																	   } 	   
																   }
					
																   if ($('endcep') && comp.dados.nu_cep != '') {
																		$('endcep').value = comp.dados.nu_cep;
																		$('endcep').focus();
																		$('entemail').focus();
																   }     	 					
					                                           }    
					                                       }
					            });
					        }";
    	
    	if ($_GET['entid']):
	 		$form .= "\n 
	 		 		   var comp = new dCNPJ();
					   comp.buscarDados($('entnumcpfcnpj').value);

					   if (comp.dados.no_igrejarial_rf != ''){
				   	   		$('entnome').value 	  = comp.dados.no_igrejarial_rf;
				   	   		$('entnome').readOnly = true;
				   	   }
			
					   if (comp.dados.nu_cep != '' && $('endcep').value == ''){
							$('endcep').value = comp.dados.nu_cep;
							$('endcep').focus();
							$('entemail').focus();	
					   } ";	
		endif;
    }else{
    	$form .= "\n    	
    					$('entnumcpfcnpj').onblur = function (e)
				        {
				            if (this.value == this.defaultValue || this.value == '')
				                return false;
							
				            if (!validar_cpf(this.value)){
								alert('CPF inválido!');
								var elemento = $('frmEntidade').elements;
								
								for (var i in $('frmEntidade').elements){
									if ($('frmEntidade').elements[i].type == 'text' || $('frmEntidade').elements[i].tagName == 'TEXTAREA'){
										$('frmEntidade').elements[i].value = '';
										$('frmEntidade').elements[i].readOnly = false;
									}else if ($('frmEntidade').elements[i].tagName == 'SELECT'){
										$('frmEntidade').elements[i].options[0].selected = true;
									}
								}
								$('entnumcpfcnpj').focus();
								$('entnumcpfcnpj').select();
								return false;		
				            }
				                
				            var req = new Ajax.Request('".$urldefault."', {
				                                       method: 'post',
				                                       parameters: '&opt=buscarCnpj&entnumcpfcnpj=' + this.value,
				                                       onComplete: function (res)
				                                       {
				                                       	   
				                                           if (res.responseText != 0) {
//				                                               if (confirm('O CPF informado já se encontra cadastrado.".'\n'."' 
//				                                                          +'Deseja carregar o registro?'))
//				                                               {
				                                                   window.location.href = '".$urldefault."&busca=entnumcpfcnpj&entid=' + res.responseText;
//				                                               } else {
//				                                                   $('entnumcpfcnpj').value = '';
//				                                                   $('entnumcpfcnpj').activate();
//				                                               }
				                                           }else{
				                                               var data = '';
				                                        	   var comp = new dCPF();
				                                        	   comp.buscarDados($('entnumcpfcnpj').value);
				                                        	   
				                                        	   if (comp.dados.no_pessoa_rf != ''){
				                                        	   		$('entnome').value = comp.dados.no_pessoa_rf;
				                                        	   		$('entnome').readOnly = true;
				                                        	   }else if ($('entnome')){
				                                        	   		$('entnome').readOnly = false;
				                                        	   }
				                                        	   
				                                        	   $('entnumrg').value    		= comp.dados.nu_rg;
				                                        	   $('entorgaoexpedidor').value = comp.dados.ds_orgao_expedidor_rg;
				
				                                        	   for (i=0; i < $('entsexo').options.length; i++){
				                                        	   		if ($('entsexo').options[i].value == comp.dados.sg_sexo_rf){
				                                        	   			$('entsexo').options[i].selected = true;			
				                                        	   		}	
				                                        	   }
				
															   if ($('entdatanasc') && comp.dados.dt_nascimento_rf != ''){	
				                                        	   		data = comp.dados.dt_nascimento_rf.substr(6,2)+'/'+comp.dados.dt_nascimento_rf.substr(4,2)+'/'+comp.dados.dt_nascimento_rf.substr(0,4);
				                                        	   		$('entdatanasc').value = data;
															   }	
				
															   if ($('entnumdddresidencial')&& comp.dados.ds_contato_pessoa != ''){
																    var tel = comp.dados.ds_contato_pessoa.split('-');
															   		$('entnumdddresidencial').value = tel[0];
															   		$('entnumresidencial').value	= tel[1]; 		
															   }
				
															   if ($('endcep') && comp.dados.nu_cep != ''){
																	$('endcep').value = comp.dados.nu_cep;
																	$('endcep').focus();
																	$('entemail').focus();
																	
															   }	   
				                                           }    
				                                       }
				            });
				        }";
    	
    	if ($_GET['entid']):
	 		$form .= "\n
	 				   var comp = new dCPF();
					   comp.buscarDados($('entnumcpfcnpj').value);
					   
			    	   if ($('entnome') && comp.dados.no_pessoa_rf != ''){
				   	   		$('entnome').value 	  = comp.dados.no_pessoa_rf;
				   	   		$('entnome').readOnly = true;
				   	   }
			
					   if (comp.dados.nu_cep != '' && $('endcep').value == ''){
							$('endcep').value = comp.dados.nu_cep;
							$('endcep').focus();
							$('entemail').focus();	
					   }";	
		endif;			
    }
    $form .= "}";
    
	if ($formEscola){
    	$form .= "\n
    				if ($('entcodent')){
	    				$('entcodent').onblur  = function (e)
					        {
					            if ($('entid').value != '' || this.value == '')
					                return false;
					
					            var req = new Ajax.Request('".$urldefault."', {
					                                       method: 'post',
					                                       parameters: '&opt=buscarEscola&entcodent=' + this.value,
					                                       onComplete: function (res)
					                                       {
					                                           if (res.responseText != 0) {
//					                                               if (confirm('O código informado já se encontra cadastrado.".'\n'."' 
//					                                                          +'Deseja carregar o registro?'))
//					                                               {
					                                                   window.location.href = '".$urldefault."&busca=entcodent&entid=' + res.responseText;
//					                                               } else {
//					                                                   $('entcodent').value = '';
//					                                                   $('entcodent').select();
//					                                               }
					                                           }
					                                       }
					            });
					        }
					}";
    }
    
    $form .= '</script>';

    $totalForms++;
    return $form;
}

function ent_exibir( $entidade )
{
	
	/*if ( ! $entidade instanceof Entidade )
	{
		$entidade = new Entidade( (integer) $entidade );
	}

	echo formEntidade($entidade, null, PESSOA_JURIDICA, true, false, false, false);
	//echo '<script type="text/javascript">$(\'frmEntidade\').disable();</script>';
	return true;
	*/

	global $db;
	
	// captura instancia da entidade
	if ( ! $entidade instanceof Entidade )
	{
		$entidade = new Entidade( (integer) $entidade );
	}
	// carrega endereços
	if ( count( $entidade->enderecos ) == 0 )
	{
		$entidade->carregarEnderecos();
	}
	
	echo Decorator::decorate(
		$entidade,
		array(
			array( 'entid', 'ID', null ),
			array( 'entnome', 'Nome', null ),
			array( 'entnumcpfcnpj', 'E-Mail', 'formatar_cpf_cnpj' ),
			array( 'entcodent', 'Código do INEP', null )
		)
	);

    Decorator::resetConf();

    $permiteAlteracao = true;

    
    if ($permiteAlteracao) {
        foreach ($entidade->enderecos as $endereco)
        {
            $dados          = array();
            $dados['endid'] = $endereco->getPrimaryKey();
			
		    //dbg($endereco);
            
            
            foreach ($endereco->getArrayCampos() as $campo) {
                $dados[$campo] = $endereco->$campo;
            }

            echo formEntidadeEndereco($dados);
        }
    } 
    else {
        foreach ( $entidade->enderecos as $endereco )
        {
            echo Decorator::decorate(
                $endereco,
                array(
                    array( 'endid', 'ID', null ),
                    array( 'endlog', 'Logradouro', null ),
                    array( 'endcom', 'Complemento', null )
                )
            );
        }
    }
}














































/**
 * Função q cria o formulário de cadastro de entidade
 *
 * 
 */
function cadastrarUnidade()
{

$titulo_modulo='Cadastro de Unidades';
monta_titulo($titulo_modulo,'');
$acao = $_REQUEST['acao'];
$evento = $_REQUEST['evento'];
$status = $_REQUEST['status'];
$entid = $_REQUEST['entid'];

if($_REQUEST['cpf'] != ''){
    $entnumcpfcnpj = pega_numero($_REQUEST['cpf']);
}else if($_REQUEST['cnpj'] != ''){
    $entnumcpfcnpj = pega_numero($_REQUEST['cnpj']);
}else $entnumcpfcnpj = pega_numero($_SESSION['entnumcpfcnpj']);



if($status == 'I'){
    $status = 'Inativo';
}else{
    $status = 'Ativo';
}

$pessoa_fisica = true;
if($_REQUEST['tipoPessoa'] != ''){

    if($_REQUEST['tipoPessoa'] != "fisica"){
        $pessoa_fisica = false;
    }else{
        $pessoa_fisica = true;
    }
}

if($acao != ""){

    switch ( $acao ) {
        //rotina que insere uma nova unidade
        case 'C':
            if($evento == "valido"){				
                if(cpfcnpj_cadastrado($entnumcpfcnpj)){
                    
                    echo("<script> alert('CPF ou CNPJ já cadastrado.');</script>");
                    
                }else{					
                    inserir_unidade($pessoa_fisica);	
                    $_REQUEST['evento'] = "";
                    $db->sucesso("sistema/cadastro/cadunidade", '');
                }	
            }

            break;

            //rotina que altera uma unidade
        case 'U':
            //atualizar dados no banco
            if($evento == "valido"){
                
                atualizar_unidade($entid);
                $_REQUEST['evento'] = "";
                $db->sucesso("sistema/cadastro/cadunidade", '');	
                
            //exibir dados na tela
            }else{

                $entnumcpfcnpj = $_REQUEST['entnumcpfcnpj'];
                //$entnumcpfcnpj = $_SESSION['entnumcpfcnpj'];
                //pagar em produção
                
                $entnumcpfcnpj = '92640524100';
                $sql_doc="SELECT
                          entid,
                          entnome,
                          entemail,
                          entnuninsest,
                          entobs,
                          entnumrg,
                          entorgaoexpedidor,
                          entsexo,
                          entdatanasc,
                          entdatainiass,
                          entdatafimass,
                          entnumdddresidencial,
                          entnumresidencial,
                          entnumdddcomercial,
                          entnumramalcomercial,
                          entnumcomercial,
                          entnumdddfax,
                          entnumramalfax,
                          entnumfax
                        FROM
                        entidade.entidade
                        WHERE 
                        entnumcpfcnpj = ".$entnumcpfcnpj."					
                ";
                global $db;
                $resultado = $db->pegaLinha($sql_doc);
                $entid = $resultado['entid'];
                $entnome = $resultado['entnome'];
                $entemail = $resultado['entemail'];
                $entnuninsest = $resultado['entnuninsest'];
                $entobs = $resultado['entobs'];
                if($resultado['entstatus'] == 'A' || $resultado['entstatus'] == ''){
                    $entstatus = "Ativo";
                }else{
                    $entstatus = "Inativo";
                }
                if(strlen($entnumcpfcnpj)  > 11){
                    $cnpj = formatar_cnpj($entnumcpfcnpj);
                    $pessoa_fisica = false;
                }else{ 
                    $cpf = formatar_cpf($entnumcpfcnpj);
                    $pessoa_fisica = true;
                }
                    
                 $entnumrg = $resultado['entnumrg'];
                 $entorgaoexpedidor = $resultado['entorgaoexpedidor'];
                 $entsexo = $resultado['entsexo'];
                 $entdatanasc = formata_data($resultado['entdatanasc']);
                 $entdatainiass = formata_data($resultado['entdatainiass']);
                 $entdatafimass = formata_data($resultado['entdatafimass']);
                 $entnumdddresidencial = $resultado['entnumdddresidencial'];
                 $entnumresidencial = $resultado['entnumresidencial'];
                 $entnumdddcomercial = $resultado['entnumdddcomercial'];
                 $entnumramalcomercial = $resultado['entnumramalcomercial'];
                 $entnumcomercial = $resultado['entnumcomercial'];
                 $entnumdddfax = $resultado['entnumdddfax'];
                 $entnumramalfax = $resultado['entnumramalfax'];
                 $entnumfax = $resultado['entnumfax'];
            }
                
                
            break;

            //rotina que exclui uma unidade
        case 'D':
            $entnumcpfcnpj = $_REQUEST['entnumcpfcnpj'];
            if(isset($_REQUEST['entnumcpfcnpj'])){				
                
                excluir_entidade($entid);		
            }

            break;

        default:
            break;
    }
}
$form = "
<form method='POST' name='formulario'>

     <input type='hidden'name='tipoPessoa' id='tipoPessoa' value=''>
     <input type='hidden' name='evento' id='evento' value='.$_REQUEST\['envento' \].'>
     <input type='hidden' name='entid' id='entid' value='.$entid .'>

<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3'
    align='center'>

    </tr>
";

    
}

/**
 * Função q cria e executa o sql que exclui um aunidade (torna ela inativa)
 *
 * @param $entid 
 */
function excluir_entidade($entid){
        
    $sql_excluir = "UPDATE entidade.entidade
                SET entstatus= 'I' 
                WHERE entid ='".$entid."'  
                ";
    global $db;
    $db->executar($sql_excluir);
    $db->commit();
}


/**
 * Verifica se um dado cpf ou cnpj já está cadastrado no banco. Retorna treu caso afirmativo
 *
 * @param unknown_type $cpfcnpj
 * @return unknown
 */
function cpfcnpj_cadastrado($cpfcnpj){
    global $db;
    $sql="
            SELECT entnumcpfcnpj FROM entidade.entidade WHERE entnumcpfcnpj = ".$cpfcnpj."
        ";
    if ($db->pegaUm($sql)) return true;
    else return false;

}

/**
 * Função q cria e executa o sql de inserção de uma unidade
 *
 * @param boolen $pessoa_fisica  - indica se a pessoa é fisica ou não
 */
function inserir_unidade($pessoa_fisica){

    global $db;

    if($pessoa_fisica){
            
        $cpf = 	$_REQUEST['cpf'];
        $entnumcpfcnpj = pega_numero($cpf);
            
    }else{
            
        $cnpj = $_REQUEST['cnpj'];
        $entnumcpfcnpj = pega_numero($cnpj);
    }
    $entdatanasc = formata_data_sql( $_REQUEST['entdatanasc'] );
    $entdatanasc = $entdatanasc ? "'" . $entdatanasc . "'" : "null";
    $entdatainiass = formata_data_sql( $_REQUEST['entdatainiass'] );
    $entdatainiass = $entdatainiass ? "'" . $entdatainiass . "'" : "null";
    $entdatafimass = formata_data_sql( $_REQUEST['entdatafimass'] );
    $entdatafimass = $entdatafimass ? "'" . $entdatafimass . "'" : "null";

    if($_REQUEST['entdatanasc'] == 'Intivo'){
        $entstatus = "I";
    }else{
        $entstatus = "A";
    }

    $sql_inserir = "
            INSERT INTO entidade.entidade
             (entnumcpfcnpj,
              entnome,
              entemail,
              entnuninsest,
              entobs,
              entstatus,
              entnumrg,
              entorgaoexpedidor,
              entsexo,
              entdatanasc,
              entdatainiass,
              entdatafimass,
              entnumdddresidencial,
              entnumresidencial,
              entnumdddcomercial,
              entnumramalcomercial,
              entnumcomercial,
              entnumdddfax,
              entnumramalfax,
              entnumfax) 
            
            VALUES ('".$entnumcpfcnpj."', '".$_REQUEST['entnome']."', '".$_REQUEST['entemail']."', '".$_REQUEST['entnuninsest']."',
                    '".$_REQUEST['entobs']."', '$entstatus','".$_REQUEST['entnumrg']."', '".$_REQUEST['entorgaoexpedidor']."','".$_REQUEST['entsexo']."',
                    ".$entdatanasc.", ".$entdatainiass.", ".$entdatafimass.", '".$_REQUEST['entnumdddresidencial']."', '".$_REQUEST['entnumresidencial']."', '".$_REQUEST['entnumdddcomercial']."', 
                    '".$_REQUEST['entnumramalcomercial']."', '".$_REQUEST['entnumcomercial']."', '".$_REQUEST['entnumdddfax']."', '".$_REQUEST['entnumramalfax']."', '".$_REQUEST['entnumfax']."') 
     
            ";

    $db->executar($sql_inserir);
    $db->commit();

}
/**
 * Função q cria e executa o sql de inserção de uma unidade
 *
 * @param $entid
 * @param boolen $pessoa_fisica  - indica se a pessoa é fisica ou não
 */
function atualizar_unidade($entid){
    global $db;	
    $entdatanasc = formata_data_sql( $_REQUEST['entdatanasc'] );
    $entdatanasc = $entdatanasc ? "'" . $entdatanasc . "'" : "null";
    $entdatainiass = formata_data_sql( $_REQUEST['entdatainiass'] );
    $entdatainiass = $entdatainiass ? "'" . $entdatainiass . "'" : "null";
    $entdatafimass = formata_data_sql( $_REQUEST['entdatafimass'] );
    $entdatafimass = $entdatafimass ? "'" . $entdatafimass . "'" : "null";

    if($_REQUEST['entdatanasc'] == 'Intivo'){
        $entstatus = "I";
    }else{
        $entstatus = "A";
    }

    $sql_alterar = "UPDATE entidade.entidade
                        SET 
                          entnome = '".$_REQUEST['entnome']."',
                          entemail = '".$_REQUEST['entemail']."',
                          entnuninsest = '".$_REQUEST['entnuninsest']."',
                          entobs = '".$_REQUEST['entobs']."',
                          entstatus =  '$entstatus',
                          entnumrg = '".$_REQUEST['entnumrg']."',
                          entorgaoexpedidor ='".$_REQUEST['entorgaoexpedidor']."',
                          entsexo = '".$_REQUEST['entsexo']."',
                          entdatanasc = ".$entdatanasc.",
                          entdatainiass = ".$entdatainiass.",
                          entdatafimass = ".$entdatafimass.",
                          entnumdddresidencial = '".$_REQUEST['entnumdddresidencial']."',
                          entnumresidencial = '".$_REQUEST['entnumresidencial']."', 
                          entnumdddcomercial = '".$_REQUEST['entnumdddcomercial']."',
                          entnumramalcomercial = '".$_REQUEST['entnumramalcomercial']."',    
                          entnumcomercial = '".$_REQUEST['entnumcomercial']."',
                          entnumdddfax = '".$_REQUEST['entnumdddfax']."',
                          entnumramalfax = '".$_REQUEST['entnumramalfax']."',
                          entnumfax = '".$_REQUEST['entnumfax']."'
                        
                      WHERE entid ='".$entid."'  
                        ";
    
    $db->executar($sql_alterar);
    $db->commit();
}




function __autoload_entidades($class)
{
    static $classes = array();

    if (!in_array($class, $classes)) {
        $classes[$class] = APPRAIZ . 'includes/ActiveRecord/classes/' . $class . '.php';
    }

    include_once $classes[$class];
}


spl_autoload_register('__autoload_entidades');



