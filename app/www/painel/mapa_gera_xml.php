<?php
	// abre conexão com o servidor de banco de dados
	include_once "config.inc";
	//include "verificasistema.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";
	$db = new cls_banco();
	$conteudo = "";
	$filtro="";
	// Busca
	if ($_REQUEST["texto_busca"]){
		$busca="AND UPPER(o.obrdesc || ' ' ||ent.entnome) LIKE UPPER('%".$_REQUEST["texto_busca"]."%')";
	}
	
	// Filtros
	if ($_REQUEST["orgid"]){
		$filtro.=" AND o.orgid IN (".$_REQUEST["orgid"].") ";
	}
	if ($_REQUEST["stoid"]){
		$filtro.=" AND o.stoid IN (".$_REQUEST["stoid"].") ";
	}
	if ($_REQUEST["cloid"]){
		$filtro.=" AND o.cloid IN (".$_REQUEST["cloid"].") ";
	}
	if ($_REQUEST["estuf"]){
		$estuf=str_replace("\'","'",$_REQUEST["estuf"])	;
		$filtro.=" AND e.estuf IN (".$estuf.") ";
	}
	if ($_REQUEST["entid"]){
		$filtro.=" AND o.entidunidade IN (".implode("','",$_REQUEST["entid"]).") ";
	}
	
	if( $_REQUEST["requisicao"] == "supervisao"){
		$join = "INNER JOIN obras.repositorio ore ON ore.obrid = o.obrid";
		$filtro .= " AND repstatus = 'A' ";
	}
	
	$sql = "SELECT
            -- ID do registro
            o.obrid as idobra,

            -- Descrição do registro
            TRIM (o.obrdesc) as obrdesc,

            --############### LATITUDE ###################### --
			CASE WHEN (SPLIT_PART(e.medlatitude, '.', 1) <>'' AND SPLIT_PART(e.medlatitude, '.', 2) <>'' AND split_part(e.medlatitude, '.', 3) <>'') THEN
               CASE WHEN split_part(e.medlatitude, '.', 4) <>'N' THEN
                   (((split_part(e.medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(e.medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(e.medlatitude, '.', 1)::int)))*(-1)
                ELSE
                   ((SPLIT_PART(e.medlatitude, '.', 3)::double precision / 3600) +(SPLIT_PART(e.medlatitude, '.', 2)::double precision / 60) + (SPLIT_PART(e.medlatitude, '.', 1)::int))
               END
            ELSE
            -- Valores do IBGE convertidos em  decimal
            CASE WHEN (length (mun.munmedlat)=8) THEN 
                CASE WHEN length(REPLACE('0' || mun.munmedlat,'S','')) = 8 THEN
                    ((SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'S',''),1,2)::double precision))*(-1)
                ELSE
                    (SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || mun.munmedlat,'N',''),1,2)::double precision)
                END
            ELSE
                CASE WHEN length(REPLACE(mun.munmedlat,'S','')) = 8 THEN
                   ((SUBSTR(REPLACE(mun.munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'S',''),1,2)::double precision))*(-1)
                ELSE
                  0--((SUBSTR(REPLACE(mun.munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(mun.munmedlat,'N',''),1,2)::double precision))
                END
            END  
            END as latitude, 
            --############### FIM LATITUDE ###################### --
            
            --############### LONGITUDE ###################### --
            CASE WHEN (SPLIT_PART(e.medlongitude, '.', 1) <>'' AND SPLIT_PART(e.medlongitude, '.', 2) <>'' AND split_part(e.medlongitude, '.', 3) <>'') THEN
               ((split_part(e.medlongitude, '.', 3)::double precision / 3600) +(SPLIT_PART(e.medlongitude, '.', 2)::double precision / 60) + (SPLIT_PART(e.medlongitude, '.', 1)::int))*(-1)
            ELSE
                -- Valores do IBGE convertidos em  decimal
               (SUBSTR(REPLACE(mun.munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(mun.munmedlog,'W',''),3,2)::double precision/60)) *(-1)
            END as longitude, 
            --############### FIM LONGITUDE ###################### --

            --Origem do registro (caso venha do IBGE, somar 100 ao tipo
            CASE WHEN e.medlatitude <> '' AND e.medlatitude is not null THEN
                o.orgid
            ELSE 
                o.orgid+100
            END as tipo

            FROM obras.obrainfraestrutura o	{$join}
                INNER JOIN entidade.endereco e on e.endid=o.endid and e.endstatus='A' 
                INNER JOIN entidade.entidade ent on ent.entid=o.entidunidade  
                INNER JOIN territorios.municipio mun on e.muncod=mun.muncod
			WHERE o.obsstatus='A' $busca  $filtro ORDER BY mun.munmedlog";
		$dados = $db->carregar($sql);
	
	$conteudo .= "<markers> ";
	for($i=0;$i < count($dados);$i++){
		 $idobra = $dados[$i]["idobra"];
		 $orgid=$dados[$i]["tipo"];
		 $latitude = $dados[$i]["latitude"];
		 $longitude= $dados[$i]["longitude"];
		 $obrdesc=$dados[$i]["obrdesc"];
		 $obrdesc=str_replace('"',"",$obrdesc)	;
		
		 $conteudo .= "<marker ";
		 $conteudo .= "idobra='$idobra' ";
		 $conteudo .= "obrdesc=\"". $obrdesc ."\" ";
		 $conteudo .= "orgid='$orgid' ";
		 $conteudo .= "lat='$latitude' ";
		 $conteudo .= "lng='$longitude' ";
		 $conteudo .= "/> ";
	}
	$conteudo .= "</markers> ";
	print $conteudo;
?>			