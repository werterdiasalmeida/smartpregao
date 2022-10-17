<?php
$cfgDisque = array();
$cfgDisque['login'] = '03700155689';
$cfgDisque['senha'] = '12345';
$cfgDisque['url'] = "http://disque100.call.inf.br/sql.php";


define("INDICADOR_GRUPO_VIOLACAO", 695);
define("INDICADOR_VIOLACAO_TIPO_VIOLACAO", 827);


function getData($sql, $parametros=array())
{
	global $cfgDisque;

	if ( count($parametros) > 0 )
	{
		foreach ( $parametros as $key=>$val ) 
		{
			$sql = str_replace( $key, $val, $sql );
		}
	}

	$data = array ("type"     => 'json', 
				   "cmdsql"   => $sql,
				   "user" 	  => $cfgDisque['login'],
				   "password" => $cfgDisque['senha']);
	$data = htmlspecialchars_decode(http_build_query($data));
	$context_options = array ('http' => array (
											'method' => 'POST',
											'header'=> "Content-type: application/x-www-form-urlencoded\r\n" . 
											"Content-Length: " . strlen($data) . "\r\n",
											'content' => $data
										)
							);

	$context = stream_context_create($context_options);
	$url = $cfgDisque['url'];
	$return = file_get_contents ($url, false, $context);
	return $return;
}

function getCSVGrupoViolacao( $dados )
{
	$csv = array();;
	$csv[] = "id do indicador;id da periodicidade;quantidade;id detalhe (Grupo de Violação);uf";
	foreach( $dados as $linha )
	{
		$csv[] = sprintf( "%s;%s;%s;%s;%s", $linha['indicador'], $linha['periodicidade'], $linha['qtd'], $linha['detalhe'], $linha['estuf'] ); 
	}
	return $csv;
}

function getCSVViolacaoPorTipoViolacao( $dados )
{
	$csv = array();;
	$csv[] = "id do indicador;id da periodicidade;quantidade;id detalhe (Tipo de violação);código do município";
	foreach( $dados as $linha )
	{
		$csv[] = sprintf( "%s;%s;%s;%s;%s", $linha['indicador'], $linha['periodicidade'], $linha['qtd'], $linha['detalhe'], $linha['muncod'] ); 
	}
	return $csv;
}


$i = 0;
$cfgIndicador = array();
$cfgIndicador[$i]['id'] = 695;
$cfgIndicador[$i]['sql'] = "Select 
		case
			when estuf is null then 'ZZ'
			when estuf is not null then estuf
		end as estuf
		, %PERIODICIDADE% as periodicidade
		, 2281 as detalhe
		, '%IDINDICADOR%' as indicador
--		, 'idoso' as nmdetalhe
		, count(*) as qtd
	From
		(
		Select 
			B.id_denuncia, min(id_demandante) as id_demandante
		From 
			disque100.tb_demandante as A Right Join disque100.tb_denuncia as B
			on A.id_atendimento = B.id_atendimento
		where
			id_grupo_violacao = 1
			and A.id_atendimento in (select id_atendimento from disque100.tb_atendimento where dt_atendimento between '%DATA_INICIAL%' and %DATA_FINAL%)
		Group By
			B.id_denuncia
		Order By
			B.id_denuncia
		) as A Left Join disque100.tb_demandante as B On A.id_demandante = B.id_demandante
	Group By
		estuf
union 
Select 
		case
			when estuf is null then 'ZZ'
			when estuf is not null then estuf
		end as estuf
		, %PERIODICIDADE% as periodicidade
		, 2282 as detalhe
		, '%IDINDICADOR%' as indicador
--		, 'lgbt' as nmdetalhe
		, count(*) as qtd
	From
		(
		Select 
			B.id_denuncia, min(id_demandante) as id_demandante
		From 
			disque100.tb_demandante as A Right Join disque100.tb_denuncia as B
			on A.id_atendimento = B.id_atendimento
		where
			id_grupo_violacao = 2
			and A.id_atendimento in (select id_atendimento from disque100.tb_atendimento where dt_atendimento between '%DATA_INICIAL%' and %DATA_FINAL%)
		Group By
			B.id_denuncia
		Order By
			B.id_denuncia
		) as A Left Join disque100.tb_demandante as B On A.id_demandante = B.id_demandante
	Group By
		estuf
union 
Select 
		case
			when estuf is null then 'ZZ'
			when estuf is not null then estuf
		end as estuf
		, %PERIODICIDADE% as periodicidade
		, 2283 as detalhe
		, '%IDINDICADOR%' as indicador
--		, 'pess def' as nmdetalhe
		, count(*) as qtd
From
		(
		Select 
			B.id_denuncia, min(id_demandante) as id_demandante
		From 
			disque100.tb_demandante as A Right Join disque100.tb_denuncia as B
			on A.id_atendimento = B.id_atendimento
		where
			id_grupo_violacao = 3
			and A.id_atendimento in (select id_atendimento from disque100.tb_atendimento where dt_atendimento between '%DATA_INICIAL%' and %DATA_FINAL%)
		Group By
			B.id_denuncia
		Order By
			B.id_denuncia
		) as A Left Join disque100.tb_demandante as B On A.id_demandante = B.id_demandante
	Group By
		estuf
union
Select 
		case
			when estuf is null then 'ZZ'
			when estuf is not null then estuf
		end as estuf
		, %PERIODICIDADE% as periodicidade
		, 2285 as detalhe
		, '%IDINDICADOR%' as indicador
--		, 'crian adolesc' as nmdetalhe
		, count(*) as qtd
From
		(
		Select 
			B.id_denuncia, min(id_demandante) as id_demandante
		From 
			disque100.tb_demandante as A Right Join disque100.tb_denuncia as B
			on A.id_atendimento = B.id_atendimento
		where
			id_grupo_violacao = 4
			and A.id_atendimento in (select id_atendimento from disque100.tb_atendimento where dt_atendimento between '%DATA_INICIAL%' and %DATA_FINAL%)
		Group By
			B.id_denuncia
		Order By
			B.id_denuncia
		) as A Left Join disque100.tb_demandante as B On A.id_demandante = B.id_demandante
	Group By
		estuf
union
Select 
		case
			when estuf is null then 'ZZ'
			when estuf is not null then estuf
		end as estuf
		, %PERIODICIDADE% as periodicidade
		, 2284 as detalhe
		, '%IDINDICADOR%' as indicador
--		, 'pop rua' as nmdetalhe
		, count(*) as qtd
From
		(
		Select 
			B.id_denuncia, min(id_demandante) as id_demandante
		From 
			disque100.tb_demandante as A Right Join disque100.tb_denuncia as B
			on A.id_atendimento = B.id_atendimento
		where
			id_grupo_violacao = 5
			and A.id_atendimento in (select id_atendimento from disque100.tb_atendimento where dt_atendimento between '%DATA_INICIAL%' and %DATA_FINAL%)
		Group By
			B.id_denuncia
		Order By
			B.id_denuncia
		) as A Left Join disque100.tb_demandante as B On A.id_demandante = B.id_demandante
	Group By
		estuf
union
Select 
		case
			when estuf is null then 'ZZ'
			when estuf is not null then estuf
		end as estuf
		, %PERIODICIDADE% as periodicidade
		, 2286 as detalhe
		, '%IDINDICADOR%' as indicador
--		, 'outros' as nmdetalhe
		, count(*) as qtd
From
		(
		Select 
			B.id_denuncia, min(id_demandante) as id_demandante
		From 
			disque100.tb_demandante as A Right Join disque100.tb_denuncia as B
			on A.id_atendimento = B.id_atendimento
		where
			id_grupo_violacao = 7
			and A.id_atendimento in (select id_atendimento from disque100.tb_atendimento where dt_atendimento between '%DATA_INICIAL%' and %DATA_FINAL%)
		Group By
			B.id_denuncia
		Order By
			B.id_denuncia
		) as A Left Join disque100.tb_demandante as B On A.id_demandante = B.id_demandante
	Group By
		estuf
order by 3,2,1";


// violação por tipo
$cfgIndicador[++$i]['id'] = 827;
$cfgIndicador[$i]['sql'] = "select 
	coalesce(muncod,'0000000') as muncod
	, %PERIODICIDADE% as periodicidade
	, case
			when id_tipo_violacao = 8 then 2847 -- ABUSO FINANCEIRO E ECONÔMICO/ VIOLÊNCIA PATRIMONIAL
			when id_tipo_violacao = 10 then 2848 -- DIREITO À MEMÓRIA E À VERDADE
			when id_tipo_violacao = 7 then 2849 -- DISCRIMINAÇÃO
			when id_tipo_violacao = 13 then 2850 -- EXPLORAÇÃO DO TRABALHO INFANTIL
			when id_tipo_violacao = 6 then 2851 -- NEGLIGÊNCIA
			when id_tipo_violacao = 12 then 2852 -- OUTRAS VIOLAÇÕES / OUTROS ASSUNTOS RELACIONADOS A DIREITOS HUMANOS
			when id_tipo_violacao = 5 then 2853 -- TORTURA E OUTROS TRATAMENTOS OU PENAS CRUÉIS, DESUMANOS OU DEGRADANTES
			when id_tipo_violacao = 11 then 2854 -- TRABALHO ESCRAVO
			when id_tipo_violacao = 9 then 2855 -- TRÁFICO DE PESSOAS
			when id_tipo_violacao = 1 then 2856 -- VIOLÊNCIA FÍSICA
			when id_tipo_violacao = 4 then 2857 -- VIOLÊNCIA INSTITUCIONAL
			when id_tipo_violacao = 2 then 2858 -- VIOLÊNCIA PSICOLÓGICA
			when id_tipo_violacao = 3 then 2859 -- VIOLÊNCIA SEXUAL
		end as detalhe
	, '%IDINDICADOR%' as indicador
	, count(*) as qtd 
from (
	select distinct
	a.id_denuncia, b.dt_atendimento, localdenuncias.muncod, localdenuncias.estuf, violacoesdenuncias.id_tipo_violacao, violacoesdenuncias.no_tipo_violacao
	from
	((disque100.tb_denuncia as a inner join disque100.tb_atendimento as b on a.id_atendimento = b.id_atendimento)
	inner join 
	(select a.id_denuncia, d.muncod, d.estuf from ((disque100.tb_cenario as a inner join disque100.tb_vitima as b on a.id_vitima = b.id_vitima) inner join disque100.tb_pessoa_fisica as c on b.id_pessoa_fisica = c.id_pessoa_fisica) inner join territorios.municipio as d on c.muncod = d.muncod and c.estuf = d.estuf) as localdenuncias
	on a.id_denuncia = localdenuncias.id_denuncia)
	inner join
	(select id_denuncia, c.id_tipo_violacao, no_violacao, no_tipo_violacao from ((select distinct id_denuncia, id_violacao from disque100.tb_cenario as aa left join disque100.tb_violacao_cenario as ab on aa.id_cenario = ab.id_cenario) as a left join disque100.tb_violacao as b on a.id_violacao = b.id_violacao) left join disque100.tb_tipo_violacao as c on b.id_tipo_violacao = c.id_tipo_violacao) as violacoesdenuncias
	on a.id_denuncia = violacoesdenuncias.id_denuncia
	where b.dt_atendimento between '%DATA_INICIAL%' and %DATA_FINAL%
) t
group by
	id_tipo_violacao, muncod
order by
	3, 2, 1";
?>