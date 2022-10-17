<?php
/*!@
echo("<pre>");
print_r($_REQUEST);
echo("</pre>");
*/
if (array_key_exists('btnGravar', $_REQUEST ) ) {    
    $undidLab          = $_REQUEST['undid_lab'];
    $labDesc           = $_REQUEST['labdesc'];
    $labQtdPosto       = $_REQUEST['labqtdpostotrab'];        
    $labQtdAreaMin     = $_REQUEST['labqtdareaminima'];       
    $labNum            = $_REQUEST['labnumero'];
    $ldiid             = $_REQUEST['ldiid'];

    if ( !$_REQUEST['labid'] ) {    
             //INSERT
             $sql = "INSERT INTO 
                            cte.laboratorio ( 
                                             undid, 
                                             labdesc, 
                                             labqtdpostotrab, 
                                             labqtdareaminima, 
                                             labnumero, 
                                             ldiid
                                             )
                           VALUES (
                                   $undidLab,
                                   '$labDesc',
                                   " . str_replace(',', '.', str_replace('.','', $_REQUEST['labqtdpostotrab'])). ",
                                   " . str_replace(',', '.', str_replace('.','', $_REQUEST['labqtdareaminima'])). ",
                                   $labNum,
                                   $ldiid
                                   ) returning labid";
             // dbg($sql);                      
              $_REQUEST['labid'] = $db->pegaUm( $sql );
               echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';
                    
         } else {
             $sql = " UPDATE cte.laboratorio SET
                           undid            = $undidLab , 
                           labdesc          = '$labDesc', 
                           labqtdpostotrab  = " . str_replace(',', '.', str_replace('.','', $_REQUEST['labqtdpostotrab'])). ",
                           labqtdareaminima = " . str_replace(',', '.', str_replace('.','', $_REQUEST['labqtdareaminima'])). ",
                           labnumero        = $labNum, 
                           ldiid            = $ldiid
                      WHERE
                           labid            =  {$_REQUEST['labid']} "; 

              $db->executar( $sql );    
             
              echo '<script type="text/javascript">window.opener.location.reload(); window.close();</script>';
         }
         
         $db->commit();
}
//FIM DO INSERT PARA O LABORATORIO.

if ($_REQUEST['labid']) {
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                labid,
                undid,
                labdesc,
                labqtdpostotrab,
                labqtdareaminima,
                labnumero,
                ldiid
            FROM 
                cte.laboratorio 
            WHERE
                labid = {$_REQUEST['labid']}";               
    
    $rsDadosLab = $db->carregar( $sql );
}
    ?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
    </style>
  </head>
  <body>

<form onSubmit="return validaForm(this);"action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadItensComposicao" >
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Cadastro de Itens de Laboratório</div>
          <div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
            <table id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
            <tr><!-- carregar uma combo com as unidades e seus ids -->
                <td class="SubTituloDireita">Unidade: </td>
                <td align="left">
                                
                <?php 
/*
                echo("<pre>");
                print_r($rsDadosLab);
                echo("</pre>");
*/
                
                //query abaixo vai carregar as unidades
                if (trim($_REQUEST['labid']) == '') {
                    
                    $somenteLeituraUnidade = "S";
                    
                    $sql = "
                            SELECT DISTINCT
                                und.undid AS codigo, und.unddsc AS descricao
                            FROM 
                                cte.unidademedida und
                            WHERE
                                und.undid
                            NOT IN (SELECT undid FROM cte.laboratorio) ORDER BY descricao";
                } else {
                    
                    $somenteLeituraUnidade = "N";
                    
                    $sql = "
                            SELECT  
                                undid AS codigo, unddsc AS descricao
                            FROM 
                                cte.unidademedida
                            WHERE
                                undid = " . $rsDadosLab[0]["undid"];
                }
                   
                    $undid_lab = $rsDadosLab[0]["undid"];
					$db->monta_combo('undid_lab', $sql, $somenteLeituraUnidade, "Selecione...", '', '', '', '532', 'N');
                ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> Descriçao: </td>
                <td align="left">
                   <?php $labdesc = $rsDadosLab[0]["labdesc"]; ?>
                  <?= campo_texto('labdesc', 'N', $somenteLeitura, '', 100, 1000, '', '', 'left', '',  0, 'id="ilbdesc" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> Quant. Posto Trabalho: </td>
                <td align="left">
                 <?php $labqtdpostotrab = $rsDadosLab[0]["labqtdpostotrab"]; 
                       $labqtdpostotrab = number_format($labqtdpostotrab,2,",",".");
                 ?>
                  <?= campo_texto('labqtdpostotrab', 'N', $somenteLeitura, '', 10, 18, '####,##', '', 'left', '',  0, 'id="labqtdpostotrab" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
            <td class ="SubTituloDireita"> Quant. Áera Mínima: </td>
                <td align="left">
                <?php $labqtdareaminima = $rsDadosLab[0]["labqtdareaminima"]; 
                      $labqtdareaminima = number_format($labqtdareaminima,2,",",".");
                ?>
                  <?= campo_texto('labqtdareaminima', 'N', $somenteLeitura, '', 10, 16, '####,##', '', 'left', '',  0, 'id="labqtdareaminima" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> Número: </td>
                <td align="left">  
                  <?php $labnumero = $rsDadosLab[0]["labnumero"]; ?>
                  <?= campo_texto('labnumero', 'N', $somenteLeitura, '', 10, 10, '######', '', 'left', '',  0, 'id="labnumero" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita"> Disciplina Atendida: </td>
                <td align="left">  
                 <?php
                    $sql = "SELECT ldiid AS codigo, ldidsc AS descricao FROM cte.labdisciplina order by descricao";
                    $ldiid = $rsDadosLab[0]["ldiid"];
					$db->monta_combo("ldiid", $sql, 'S', "Selecione...", '', '', '', '175', 'N');
                ?> 
                </td>
            </tr>
           
<?php
if (array_key_exists('excluir', $_REQUEST)) 
{
    $sql = "DELETE FROM cte.itenslaboratorio WHERE ilbid = {$_REQUEST['excluir']}";
   // $db->executar('DELETE FROM cte.itenslaboratorio WHERE ilbid = ' . (integer) $_REQUEST['ilbid'][$k]);
 
    $deleteItemLab = $db->executar( $sql );
    $db->commit();
     
   // die('null');
}



if (array_key_exists('btnGravar', $_REQUEST) && is_array($_REQUEST['ilbid'])) 
{
    if (  $_REQUEST['ilbqtditens'] ) 
    {
        foreach ($_REQUEST['ilbid'] as $k => $ilbid) 
        {
            if (trim($_REQUEST['ilbdesc'][$k]) == '' )
                continue;

            if ($ilbid == '' &&  $_REQUEST['ilbqtditens'][$k] != '' &&  $_REQUEST['ilbvlrunitario'][$k] != '' && $_REQUEST['ilbdesc'][$k] != '' ) 
            {
  
                    $sql = "INSERT INTO cte.itenslaboratorio (
                                labid,
                                ilbdesc,
                                unddid,
                                ilbqtditens,
                                ilbvlrunitario
                            ) VALUES ('{$_REQUEST['labid']}',       
                                      '{$_REQUEST['ilbdesc'][$k]}', 
                                      {$_REQUEST['unddid'][$k]}, 
                                      " . str_replace(',', '.', str_replace('.','', $_REQUEST['ilbqtditens'][$k])). ",
                                      " . str_replace(',', '.', str_replace('.','', $_REQUEST['ilbvlrunitario'][$k])) . ")";
                                      
                                    
            
            }
            else
            {
                if (  $_REQUEST['ilbqtditens'][$k] != '' &&  $_REQUEST['ilbvlrunitario'][$k] != '' && $_REQUEST['ilbdesc'][$k] != '')
                {
                   
                        $sql = "UPDATE cte.itenslaboratorio SET
                                    ilbdesc         = '{$_REQUEST['ilbdesc'][$k]}',
                                    unddid          = {$_REQUEST['unddid'][$k]},
                                    ilbqtditens     = " . str_replace(',', '.', str_replace('.','', $_REQUEST['ilbqtditens'][$k])). ",
                                    ilbvlrunitario     = " . str_replace(',', '.', str_replace('.','', $_REQUEST['ilbvlrunitario'][$k])). "
                                WHERE
                                    ilbid     =  '{$_REQUEST['ilbid'][$k]}' "; 
                  
                }
            }
           
            $db->executar($sql);
        }

        $db->commit();
        $dadosSalvos = true;
        
        // <script type="text/javascript">carregarItensLaboratorio();</script>
       
        
       }
}

?><?php if ($_REQUEST['labid'])
        { ?>
            <div id="loader-container">
              <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
            </div>
          
            <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
              <tr style="background-color: #cccccc;">
                <td>
                  <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Itens</div>
                  <div style="margin: 0; padding: 0; height: 250px; width: 100%; background-color: #eee; border: none;" class="div_rolagem">
                    <table id="itensLaboratorioSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
                      <colgroup>
                        <col width="10%" />
                        <col width="25%" />
                        <col width="25%" />
                        <col width="20%" />
                        <col width="20%" />
                      </colgroup>
        
                      <tr style="text-align: center">
                        <td style="padding: 2px; font-weight: bold;">&nbsp;</td>
                        <td style="padding: 2px; font-weight: bold;">Descriçao do Item</td>
                        <td style="padding: 2px; font-weight: bold;">Unidade de Medida</td>
                        <td style="padding: 2px; font-weight: bold;">Quantidade</td>
                        <td style="padding: 2px; font-weight: bold;">Valor Unitario</td>
                      </tr>
                    </table>
                  </div>
              <div style="padding: 5px 0px 0px 5px">
            <span style="cursor:pointer" id="linkInserirItem" onclick="return adicionarItemLaboratorio('','','','','');"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir Item de Laboratório</span>
          </div>
        <?php } ?>
          <br />
          
        </td>
      </tr> 
      <?php if ( $_REQUEST['labid'] ) {
          ?>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita">
          <span style="display: block;font-weight:bold;font-size:110%;margin-bottom:5px;color:red">Atenção! Itens sem identifição serão descartados</span>
          <input type="submit" name="btnGravar" value="Gravar" />
        </td>
      </tr>
      <?php } else {?>
      <tr style="background-color: #cccccc;">
        <td class="SubTituloDireita" colspan="2">
        <span style="text-align: center;display: block;font-weight:bold;font-size:100%;margin-bottom:5px;color:blue">Para inserir ítens neste laboratório, salve este registro, após, clique no ícone de alteraçao na página seguinte</span>

          <input type="submit" name="btnGravar" value="Gravar"/>
        </td>
      </tr>
      <?php } ?>
    </table>
    </form>
  </body>

  <script type="text/javascript">
  <!--
  
  //Funçao que abre a popup para listar os laboratorios cadastrados.
  function mostrarLaboratorios()
    {

        return windowOpen('/seguranca/seguranca.php?modulo=sistema/tabapoio/popuplistarlaboratorio&acao=A',
                          'mostrarLaboratorios',
                          'height=440,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
 
    }
  
  
  
    this._closeWindows = false;
    var _modificado    = false;

    /**
     * 
     */
    function removerItemLaboratorio(linha, ilbid)
    {
        var linha = $(linha);
        linha.style.backgroundColor = 'rgb(255,255,210)';
        linha.style.backgroundColor = '#DAE0D2';

        if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
            linha.style.backgroundColor = '#f0f0f0';
        } else {
            $('loader-container').show();
            try {
                if (ilbid != '') {
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: 'excluir=' + ilbid,
                                                   onComplete: function(r)
                                                   {
                                                       //if (r.responseText == 'null')
                                                           linha.parentNode.removeChild(linha);
                                                       //else
                                                       //    alert('Não foi possível excluir o item.');
                                                           linha.style.backgroundColor = '#f0f0f0';
                                                   }
                                               });
                } else {
                    linha.parentNode.removeChild(linha);
                }
            } catch (e) {
            }

            $('loader-container').hide();
        }

        return false;
    }

    
    /**
     * 
     */
    function adicionarItemLaboratorio(ilbid, ilbdesc, unddid, ilbqtditens,ilbvlrunitario)
    {
        var input_ilbdesc;
        var input_unddid;
        var input_ilbqtditens;
        var input_ilbvlrunitario;
        var input_ilbid;

        /*
        var btnExcluirItemLaboratorio = new Element('img',
            {
                src:'/imagens/excluir.gif',
                onClick: function()
                {
                    alert(this);
                    //removerItemLaboratorio(this.parentNode.parentNode.getAttribute("id"), ilbid);
                }
            });
        */

        var btnExcluirItemLaboratorio = document.createElement('img');
        btnExcluirItemLaboratorio.src     = '/imagens/excluir.gif';
        btnExcluirItemLaboratorio.onclick = function()
        {
            removerItemLaboratorio(this.parentNode.parentNode.getAttribute("id"), ilbid);
        }

        //Descriçao do Item
        input_ilbdesc    = document.createElement('input');
        input_ilbdesc.setAttribute('type', 'text');
        input_ilbdesc.setAttribute('class', 'normal');
        input_ilbdesc.setAttribute('name', 'ilbdesc[]');
        input_ilbdesc.setAttribute('maxlength', '100');
        input_ilbdesc.setAttribute('size', '30');
        input_ilbdesc.setAttribute('value', ilbdesc);

        //hidden, id do laboratorio
        input_ilbid     = document.createElement('input');
        input_ilbid.setAttribute('type', 'hidden');
        input_ilbid.setAttribute('name', 'ilbid[]');
        input_ilbid.setAttribute('value', ilbid);

        //combo das unidades
        input_unddid   = document.createElement('select');
        input_unddid.setAttribute('class', 'CampoEstilo');
        input_unddid.setAttribute('name', 'unddid[]');

        <?php
    $res = (array) $db->carregar('select unddid, undddsc from cte.unidademedidadetalhamento order by undddsc');

    while (list(, $item) = each($res)) {
        $item = array_map('addslashes', $item);
        echo 'selected=unddid==' , $item["unddid"]
            , ';input_unddid.options[input_unddid.options.length]=new Option(\'' , $item["undddsc"] , '\',\'' , $item["unddid"] , '\',selected,selected);';
    }

?>

       //Quantidade
        input_ilbqtditens    = document.createElement('input');
        input_ilbqtditens.setAttribute('type', 'text');
        input_ilbqtditens.setAttribute('class', 'normal');
        input_ilbqtditens.setAttribute('name', 'ilbqtditens[]');
        input_ilbqtditens.setAttribute('maxlength', '100');
        input_ilbqtditens.setAttribute('size', '20');
        input_ilbqtditens.setAttribute('value', mascaraglobal('#.###.###,#', ilbqtditens));
        input_ilbqtditens.setAttribute('onKeyPress', 'this.value=mascaraglobal("#.###.###,#", this.value)');;
        
        //Valor unitario
        input_ilbvlrunitario    = document.createElement('input');
        input_ilbvlrunitario.setAttribute('type', 'text');
        input_ilbvlrunitario.setAttribute('class', 'normal');
        input_ilbvlrunitario.setAttribute('name', 'ilbvlrunitario[]');
        input_ilbvlrunitario.setAttribute('maxlength', '100');
        input_ilbvlrunitario.setAttribute('size', '20');
        input_ilbvlrunitario.setAttribute('value', mascaraglobal('#.###.###,##',ilbvlrunitario));
        input_ilbvlrunitario.setAttribute('onKeyPress', 'this.value=mascaraglobal("#.###.###,##", this.value)');;
        
        

        var tabelaItensLaboratorioSubAcao = $('itensLaboratorioSubAcao');
        var novaLinha                    = tabelaItensLaboratorioSubAcao.insertRow(tabelaItensLaboratorioSubAcao.rows.length);
        novaLinha.id                     = 'linhaItensLaboratorioSubAcao' + tabelaItensLaboratorioSubAcao.rows.length + 1;
        novaLinha.style.textAlign        = 'center'
        novaLinha.style.backgroundColor  = '#f0f0f0';

        /**
         * Botao excluir
         */
        var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula0.appendChild(btnExcluirItemLaboratorio);

        /**
         * Descricao
         */
        var novaCelula2 = novaLinha.insertCell(novaLinha.cells.length);
        input_ilbdesc.setAttribute('id', novaLinha.cells.length);
        novaCelula2.appendChild(input_ilbdesc);
        novaCelula2.appendChild(input_ilbid);

        /**
         * Unidade de medida
         */
        var novaCelula3 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula3.appendChild(input_unddid);
        
         /**
         * Quantidade
         */
        var novaCelula4 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula4.appendChild(input_ilbqtditens);

        /**
         * Valor Unitario
         */
        var novaCelula5 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula5.appendChild(input_ilbvlrunitario);

         if (ilbdesc == null)
            input_ilbdesc.select();

        return false;
    }


    function carregarItensLaboratorio(){<?php
        
    if( $_REQUEST['labid'] )
    {    
   
        $itens = $db->carregar('SELECT
                                    cs.ilbid,
                                    cs.labid,
                                    cs.unddid,
                                    cs.ilbdesc,
                                    cs.ilbqtditens,
                                    cs.ilbvlrunitario,
                                    cs.ilbvlrtotal,
                                    um.unddid
                                FROM
                                    cte.itenslaboratorio cs
                                INNER JOIN
                                    cte.unidademedidadetalhamento um ON cs.unddid = um.unddid
                                WHERE
                                    cs.labid  = ' . $_REQUEST['labid'] . '
                                ORDER BY
                                    cs.ilbdesc ASC');
    }

    if (!is_array($itens)) {
        echo 'return false;';
    } else {
        while (list(, $item) = each($itens)) {
            $item = array_map('addslashes', $item);
            echo "adicionarItemLaboratorio(" , $item['ilbid'] , ",'" , $item['ilbdesc'] , "'," , $item['unddid'] , "," , $item['ilbqtditens'], "," , str_replace(array('.',','), '', $item['ilbvlrunitario']),");";
        }
    }




?>}/*!@ !function carregarItensLaboratorio() */
<?php if ( $_REQUEST['labid'] ) 
{ ?>
    carregarItensLaboratorio();
    $('loader-container').hide();
<?php } ?>    
function fecharJanela()
{
    if (_modificado) {
        if (!confirm('Existem dados que não foram salvos.\nDeseja fechar a janela sem salvá-los?'))
            return false;
    }

    window.close();
}

function validaForm()
{
  
   var total = cadItensComposicao.elements.length;
    i = 0;
     while (i <= 5)
     {
         if(cadItensComposicao.elements[i].value == "")
         {
           alert("É necessário o preenchimento de todos os campos");
           cadItensComposicao.elements[i].focus();
           return false;
           break;
         }
        i++;
     }
   
 }
        
            
 
  </script>
</html>

