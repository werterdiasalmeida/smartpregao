<?php
include APPRAIZ . 'includes/Agrupador.php';

//verificando se há na listagemdo get, a posiçao excluir
     if ($_REQUEST['excluir'])
     {    
       // echo("<script>alert('vai');</script>");
        //verificando a integridade para poder excluir o laboratorio.
         $sql = "SELECT 
                   lab.labid
                FROM 
                    cte.laboratorio AS lab
                
                INNER JOIN 
                    cte.unidademedida AS uni ON lab.undid = uni.undid
                INNER JOIN 
                    cte.subacaoindicador AS sub ON sub.undid = uni.undid
                WHERE 
                lab.labid = {$_GET['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros[0]);
         
         //dbg($num);
         //die();
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 1 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao deste laboratório, pois o mesmo está vinculado com alguma Subaçao');\n</script>");
             $var = 1;   
         }
         else
         {             
             $sql = "DELETE FROM cte.itenslaboratorio WHERE labid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $sql = "DELETE FROM cte.laboratorio WHERE labid = {$_GET['excluir']}";
         
             $db->executar( $sql );
             $db->commit();
             
             header('Location: seguranca.php?modulo=sistema/tabapoio/listarlaboratorio&acao=A');
          }
          if ($var == 1){
              
              echo '<script type="text/javascript">window.location.href = "seguranca.php?modulo=sistema/tabapoio/listarlaboratorio&acao=A";</script>';
             
          }

            
     }


include APPRAIZ . 'includes/cabecalho.inc';
?>
    <script type="text/javascript" src="/includes/prototype.js"></script>
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


    <form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadItensComposicao" >
    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
      <tr style="background-color: #cccccc;">
        <td>
          <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Gerenciar Laboratórios</div>
          <div style="margin: 0; padding: 0;  width: 100%; background-color: #eee; border: none;">
            <table id="itensComposicaoSubAcao" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
            <tr><!-- carregar uma combo com as unidades e seus ids -->
              
                <td align="left">
                <?php 
                
                $sql = "SELECT
                            '<img onclick=\"return popupGerenciarLaboratorio('|| l.labid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirLaboratorio('|| l.labid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acoes,
                             l.labdesc,
                             l.labqtdpostotrab,
                             l.labqtdareaminima,
                             l.labnumero,
                             ld.ldidsc                       
                        FROM 
                             cte.laboratorio AS l
                        INNER JOIN cte.labdisciplina AS ld ON ld.ldiid = l.ldiid";  
                
                $cabecalho = array( "Editar / Excluir", "Descriçao", "Qtd.Ponto Trabalho", "Qtd.Area.Min", "Numero", "Disciplina" );

                $db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '' );
               ?>
            </tr>
            <tr>
			
			<td colspan = "6" align="center">
            
                <input TYPE="button" VALUE="Inserir Laboratório" NAME="insLab" onClick="adicionarLaboratorio();">
				<!-- <span style="cursor:pointer;display:block;float:center" onclick="return adicionarLaboratorio();"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir Laboratorio</span> -->
			</td>
		</tr>
          </div>
         </tr>
       
         </table>
         
         <script type="text/javascript">
         function popupGerenciarLaboratorio(labid)
         {
             return windowOpen('seguranca.php?modulo=sistema/tabapoio/popupcadaitenslaboratorio&acao=A&labid=' + labid,
                              'adicionarItensComposicao',
                              'height=540,width=900,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');

         }         
         
        
       
         function adicionarLaboratorio()
         {
       
             return windowOpen('/seguranca/seguranca.php?modulo=sistema/tabapoio/popupcadaitenslaboratorio&acao=A',
                          'adicionarLaboratorio',
                          'height=540,width=900,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
         
         }
         function excluirLaboratorio(labid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'seguranca.php?modulo=sistema/tabapoio/listarlaboratorio&acao=A&excluir=' + labid;
                
             }
             
         }
         </script>
         
         
