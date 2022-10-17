<?php

include_once( APPRAIZ . "includes/cabecalho.inc" );
include_once( APPRAIZ . "includes/classes/Modelo.class.inc" );
include_once( APPRAIZ . "includes/classes/modelo/territorios/MunTipoMunicipio.class.php" );


$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo($titulo_modulo, '');

$obMunTipoMunicipio = new MunTipoMunicipio();
 
if( $_REQUEST["gravar"] ){
	
	try{
		
		$obMunTipoMunicipio->excluirTudoPorMuncod( $_REQUEST["muncod"] );
		
		if( is_array( $_REQUEST["tpmid"] ) ){
			
			foreach( $_REQUEST["tpmid"] as $indice => $arTpmid ){
				
				$arCampos = array( "tpmid", "muncod", "estuf" );
				$obMunTipoMunicipio->popularObjeto( $arCampos );
				$obMunTipoMunicipio->tpmid = $arTpmid;
				
				$obMunTipoMunicipio->inserir();
			}
		}
		$obMunTipoMunicipio->commit();
	}
	catch( Exception $erro ){
		$obMunTipoMunicipio->rollback();
    }
}

?>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

	var frmValido = false;
    function carregarMunicipios(estuf)
    {
        return new Ajax.Request('/geral/dne.php?opt=municipio&regcod=' + estuf,
                                {
                                    method: 'post',
                                    onComplete: function (res)
                                    {
                                        while ($('muncod').options[1]) {
                                            $('muncod').options[1] = null;
                                        }

                                        eval(res.responseText);

                                        for (var i = 1; i < listaMunicipios[estuf].length; i++) {
                                            $('muncod').options[i] = new Option(listaMunicipios[estuf][i][1],
                                                                                listaMunicipios[estuf][i][0],
                                                                                false,
                                                                                false);
                                        }
                                    }
                                });
    }
    
    function validarFrmBusca()
    {
        var inputs = $('frmBusca').getElements();

        for (var i = 0, length = inputs.length; i < length; i++) {
            if (inputs[i].type != 'text' && inputs[i].tagName.toUpperCase() != 'SELECT')
                continue;

            if (trim(inputs[i].value) != '' && inputs[i].value != 'null')
                frmValido = true
        }

        if (!frmValido)
            alert('Por favor, preencha pelo menos um dos campos de busca.');

        return frmValido;
    }

</script>

<div align="center">

	<form method="post" name="formulario" id="frmBusca" onsubmit="return validarFrmBusca();">
		<center>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td align="right" class="SubTituloDireita">UF:</td>
					<td><?php echo $db->monta_combo('estuf', $obMunTipoMunicipio->recuperarUFs(), 'S', 'UF', 'carregarMunicipios', '', '', '', '', 'estuf'); ?></td>
				</tr>
				<tr>
					<td align="right" class="SubTituloDireita">Município:</td>
					<td><select name="muncod" class="CampoEstilo" id="muncod"><option value="null">Selecione a UF</option></select></td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input type="hidden" name="entbusca" value="1" />
						<input type="submit" value="Buscar" />
					</td>
				</tr>
			</table>
		</center>	
	</form>
	
	<?php if( array_key_exists( 'entbusca', $_REQUEST ) || $_REQUEST['entbusca'] == '1' ){ ?>
		<form method="post" name="formClassificacao" id="frmClassificacao" >
			<center>
				<?php $arMunicipio = $db->pegaLinha( "select mundescricao, estuf from territorios.municipio where muncod = '{$_REQUEST["muncod"]}'" );
				
				echo "<h2>{$arMunicipio['mundescricao']} - {$arMunicipio['estuf']}</h2>";
				
				$arTiposMunicipio = $obMunTipoMunicipio->recuperarTiposMunicipio();
			    $db->monta_lista_simples( $arTiposMunicipio, array( '', 'Classificação' ), 50, 50, 'N', '95%', '' );
			
			    $coMunTipoMunicipio = $obMunTipoMunicipio->recuperarTodos( "*", array( "muncod = '{$_REQUEST["muncod"]}'" ) );
			    
				foreach( $coMunTipoMunicipio as $arMunTipoMunicipio ){
					echo '<script type="text/javascript">
							$("tpmid_' .trim( $arMunTipoMunicipio['tpmid']). '").setAttribute( "checked", "checked" );
						 </script>';
				}?>
				<input type="hidden" value="1" name="entbusca" />
				<input type="hidden" value="<?php echo $_REQUEST["estuf"]; ?>" name="estuf" />
				<input type="hidden" value="<?php echo $_REQUEST["muncod"]; ?>" name="muncod" />
				<input type="submit" value="Gravar" name="gravar" />
			</center>
		</form>
	<?php } ?>
</div>	


