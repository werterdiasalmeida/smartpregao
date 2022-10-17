<?php
include APPRAIZ . 'includes/Agrupador.php';
//se for deletar poe aki.

 if ($_REQUEST['excluir'])
     {    
       // echo("<script>alert('vai');</script>");
        //verificando a integridade para poder excluir o laboratorio.
         $sql = "SELECT 
                    uni.undid 
                FROM 
                    cte.unidademedida AS uni
                LEFT JOIN 
                    cte.subacaoindicador AS sub ON sub.undid = uni.undid
                LEFT JOIN 
                    cte.laboratorio AS lab ON lab.undid = sub.undid
                LEFT JOIN 
                    cte.indicadorunidademedida AS ind ON ind.undid = lab.undid
                WHERE
                    uni.undid = {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros[0]);
         
         //dbg($num);
         //die();
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 0 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao desta unidade de medida, pois a mesma possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else
         {             
             $sql = "DELETE FROM cte.unidademedida WHERE undid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: seguranca.php?modulo=sistema/tabapoio/cadastrarunidademedida&acao=A');
          }
          if ($var == 1)
          {              
              echo '<script type="text/javascript">window.location.href = "seguranca.php?modulo=sistema/tabapoio/cadastrarunidademedida&acao=A";</script>';
          }
           
     }


     
     
     if (array_key_exists('btnGravar', $_REQUEST))
{
    if($_REQUEST['unddsc']!='' && $_REQUEST['undtipo']!='')
    {    //faz o insert
        
        $unddsc = strtoupper($_REQUEST['unddsc']); 
        //verifica se ja existe uma unidade com a mesma descriçao e tipo
        $sql = "SELECT * FROM 
                    cte.unidademedida 
                WHERE 
                    UPPER(unddsc) = '$unddsc' 
                AND 
                    undtipo       = '{$_REQUEST['undtipo']}'";
                    
        $arrReg = $db->carregar($sql);
        

        $numReg = count($arrReg);

        
        //se o numero de registros encontrados for maior do que zero
        if( $arrReg && $numReg > 0 )
        {
            echo("<script type=\"text/javascript\">alert('Esta Unidade já está cadastrada!');</script>");
                    
        }
        else
        {                 
            $sql = "INSERT INTO 
                        cte.unidademedida (undid, unddsc, undtipo) 
                    VALUES ( 
                            nextval('cte.unidademedida_undid_seq'),
                            '{$_REQUEST['unddsc']}', 
                            '{$_REQUEST['undtipo']}' 
                            )";
            
             //dbg($sql);
             //die();
                            
            $db->executar($sql);
            $db->commit();
            
        }
    }
}
     


include APPRAIZ . 'includes/cabecalho.inc';

?>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Cadastro de Unidades de Medida</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadUnidadesMedida">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
             <tr>
                <td class ="SubTituloDireita" align"right"> Descriçao: </td>
                <td>                   
                  <?= campo_texto('unddsc', 'N', $somenteLeitura, '', 70, 600, '', '', 'left', '',  0, 'id="unddsc" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align"right"> Tipo: </td>
                <td>
					<input id="undtipo" type="radio" name="undtipo" value="M" checked="checked" />
					<label for="Municipal">Municipal</label>
					
					<input id="undtipo" type="radio" name="undtipo" value="E"/>
					<label for="Estadual">Estadual</label>
                </td>
            
                
            </tr>
            <tr>
             <td class="SubTituloDireita"></td>
             <td colspan ="2">
              
              <input type="submit" name="btnBuscar" value="Buscar" />
              <input type="submit" name="btnGravar" value="Gravar" />
             </td>
          </tr>
          </form>
          <tr>
            <td class="SubTituloDireita" colspan = "2">
                <?php
                
                if (array_key_exists('btnBuscar', $_REQUEST))
                {
                    $sql = "SELECT
                                    '<img onclick=\"return excluirUnidadeMedida('|| undid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                     unddsc,
                                     undtipo
                                FROM 
                                     cte.unidademedida";
                                     
                     if( strtoupper($_REQUEST['unddsc']))
                     {     
                     $sql.=" WHERE 
                                    UPPER(unddsc)  ILIKE '%" .strtoupper($_REQUEST['unddsc']). "%' ";
                     }
                     if( $_REQUEST['undtipo'] )
                     { 
                     $sql.=" AND 
                                    undtipo       = '{$_REQUEST['undtipo']}' ";
                     } 
                     if( strtoupper($_REQUEST['unddsc']))
                     {     
                     $sql.=" ORDER BY
                                    unddsc ";
                     }               
                                    
                     $buscar = 1;
                }
                else
                {
                     $sql = "SELECT
                                '<img onclick=\"return excluirUnidadeMedida('|| undid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                 unddsc,
                                 undtipo
                            FROM 
                                 cte.unidademedida";  
                }
                $cabecalho = array( "Excluir", "Descriçao", "Tipo" );
                
                
                
        
                $db->monta_lista($sql, $cabecalho, 15, 6, 'N', '', '' );
                
                ?>
            </td>
           </tr>
        
       </table>
 
 <script type="text/javascript">

/* Funçao para validar os dados do formulario requisitante.
 * author: PedroDantas
 * date: 2008-08-25
 */
function validaForm()
{  
    var total = cadUnidadesMedida.elements.length;
    i = 0;
    while (i <= total)
     {
         if(cadUnidadesMedida.elements[i].value == "")
         {
           alert("É necessário o preenchimento de todos os campos");
           
           cadUnidadesMedida.elements[i].focus();
           return false;
           break;
         }
        i++;
     }   
 }
 
 /* Funçao para excluir uma unidade de medida
 * author: PedroDantas
 * date: 2008-08-25
 * params: undid (id da unidade selecionada)
 */
 function excluirUnidadeMedida(undid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'seguranca.php?modulo=sistema/tabapoio/cadastrarunidademedida&acao=A&excluir=' + undid;
                
             }
             
         }
 
 </script>

