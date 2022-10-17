<?php
	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	$db = new cls_banco();
	
	$sqlEstado = "
                    select
                        estuf as codigo,
                        estdescricao as descricao
                    from territorios.estado
                    order by
                        estdescricao
                ";
	$estados = $db->carregar( $sqlEstado );

	$PstEstados			= $_REQUEST["estados"];
	if(isset($PstEstados)){
		$sqlListaMunicipios="select muncod as codigo, estuf as estados, mundescricao as nome from territorios.municipio
							where estuf = '".$PstEstados."' order by mundescricao";
		$municipios = $db->carregar( $sqlListaMunicipios );
	}
	
	
?>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<form id="formulario" name="formulario" method="post" action="">
  <table width="100%" border="0">
    <tr>
      <td>Estados:</td>
      <td>
      <select name="estados" id="estados">
      <?
      	foreach($estados as $estados){
			$ID	  = $estados['codigo'];
			$Nome = $estados['descricao'];
      ?>
        <option  value="<?=$ID?>"><?=$Nome?></option>
      <?
		}
      ?>
      </select>
      <input id="button" type="submit" name="button" value="Filtrar" />
      </td>
    </tr>
      <? 
  if(isset($PstEstados)){
  	$maximo = count($municipios);	
  	foreach($municipios as $municipios){
  		$Cor = "#e2e6e7 ";
		$Divisao=$Cont%2;
		if($Divisao == 0){
			$Cor = "#fbfbfb ";
		}
		$Cont = $Cont+1;
  		?>
    <tr>
      <td style="background-color:<?=$Cor?>;">
      <input name="checkbox<?= $municipios['codigo']?>" 
      type="checkbox"
      title="<?= $municipios['nome']." - ".$municipios['estados']?>" 
      id="checkbox<?= $municipios['codigo']?>" 
      value="<?= $municipios['codigo']?>"
      onclick="obterMarcados('checkbox<?= $municipios['codigo']?>', '<?= $municipios['codigo']?>','<?= $municipios['nome']." - ".$municipios['estados']?>');" />
      </td>
      <td  style="background-color:<?=$Cor?>;"><?=$municipios['nome']?> - <?=$municipios['estados']?> </td>

    </tr>
      <?
  	}
 ?>
  <tr>
  <td>
    <input id="Selecionatodos" type="button" name="Selecionatodos" value="Todos" onclick="selecionaTodos();"  />
  </td>
  </tr>
  <?
 }
  ?>

  </table>
  <div>

  
  </div>
</form>
<script language="javascript">
var k = 0;
var t = opener.document.formulario.municipiosUsuario;
var a = document.formulario.elements;
for(k; k< a.length; k++){
	var elementoatual = a[k];
	switch(elementoatual.type){
	case "checkbox":
		for(i=0;i<t.length;i++){
		var item = t.options[i];
			if(item.value == elementoatual.value){
				elementoatual.checked = true;
			}
		}	
	break;
	default:
		continue;
	break;
	}
}

function obterMarcados(Nome,Valor,Estado) {     
	checkBox = document.getElementById(Nome); 
	if ( checkBox.checked ) { 
		if((opener.document.formulario.municipiosUsuario.options.length == 1) && (opener.document.formulario.municipiosUsuario.options[0].value == "")){
			opener.document.formulario.municipiosUsuario.options[0] = null;
		}
	  	var d=opener.document.formulario.municipiosUsuario.options.length++;
		opener.document.formulario.municipiosUsuario.options[d].text = Estado;
		opener.document.formulario.municipiosUsuario.options[d].value = Valor;
		opener.document.formulario.municipiosUsuario.options[d].setAttribute("selected","selected");
	}else{
		var listaOpcoes = opener.document.formulario.municipiosUsuario.options;
		for(x = 0 ; x< listaOpcoes.length; x++){
			if(listaOpcoes[x].value == Valor ){
				opener.document.formulario.municipiosUsuario.options[x] = null;
			}
			if(listaOpcoes.length == 0){
				var textocombogeral = "Duplo clique para selecionar da lista"; 
				var d=opener.document.formulario.municipiosUsuario.options.length++;
				opener.document.formulario.municipiosUsuario.options[d].text = textocombogeral;
				opener.document.formulario.municipiosUsuario.options[d].value = "";
				opener.document.formulario.municipiosUsuario.options[d].setAttribute("","");
			} 
		}
	}   
} 

function selecionaTodos(){
	 for (i=0;i<document.formulario.elements.length;i++){ 
      if(document.formulario.elements[i].type == "checkbox" && !document.formulario.elements[i].checked){
         document.formulario.elements[i].checked=true;
		 Nome = document.formulario.elements[i].name;
		 Valor = document.formulario.elements[i].value;
		 Estado = document.formulario.elements[i].title;
		 obterMarcados( Nome, Valor,Estado);
		}
	}
}
</script>