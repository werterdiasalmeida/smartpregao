<?php
include_once APPRAIZ . "includes/Snoopy.class.php";


// não há controle do locale do servidor...
$mes = array(
	"Janeiro",
	"Fevereiro",
	"Março",
	"Abril",
	"Maio",
	"Junho",
	"Julho",
	"Agosto",
	"Setembro",
	"Outubro",
	"Novembro",
	"Dezembro"
);

$parecer_municipio = $prefeitura["mundescricao"];
$parecer_muncod    = $prefeitura["muncod"];
$parecer_nome      = $prefeito["entnome"];
$parecer_data      = date( "d" ) . " de " . $mes[date( "m" )-1] . " de " . date( "Y" );

?>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title></title>
<style type="text/css">
	@page { size: 21.59cm 27.94cm; margin-top: 2.501cm; margin-bottom: 1.251cm; margin-left: 2cm; margin-right: 2cm }
	table { border-collapse:collapse; border-spacing:0; empty-cells:show }
	td, th { vertical-align:top; }
	h1, h2, h3, h4, h5, h6 { clear:both }
	h3, h4, h5, h6 { font-weight: normal; }
	ol, ul { padding:0; }
	* { margin:0; font-size:11pt; font-family:Arial; }
	*.fr1 { font-size:11pt; vertical-align:top; text-align:right; background-color:#ffffff; }
	*.fr2 { font-size:11pt; vertical-align:top; text-align:center; margin-left:0cm; margin-right:0cm; background-color:#000000; padding-left:0.28cm; padding-right:0.28cm; padding-top:0.153cm; padding-bottom:0.153cm; border-style:none; }
	*.Frame { font-size:11pt; vertical-align:top; text-align:center; }
	*.Graphics { font-size:11pt; vertical-align:top; text-align:center; }
	*.OLE { font-size:11pt; vertical-align:top; text-align:center; }
	*.Caption { font-family:Arial; font-size:11pt; margin-top:0.212cm; margin-bottom:0.212cm; font-style:italic; }
	*.Footer { font-family:Arial; font-size:11pt; }
	*.Framecontents { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.Heading { font-family:Arial; font-size:14pt; margin-top:0.423cm; margin-bottom:0.212cm; }
	*.Heading1 { font-family:Arial; font-size:11pt; line-height:150%; text-align:justify ! important; font-weight:bold; }
	*.Heading4 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.Heading9 { font-family:Arial; font-size:11pt; text-align:justify ! important; font-weight:bold; }
	*.Index { font-family:Arial; font-size:11pt; }
	*.Legenda { font-family:Arial; font-size:14pt; text-align:center ! important; font-weight:bold; }
	*.List { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.NormalWeb { font-family:Arial; font-size:11pt; margin-top:0.494cm; margin-bottom:0.494cm; }
	*.P1 { font-family:Arial; font-size:11pt; margin-left:0cm; margin-right:0.635cm; text-indent:0cm; }
	*.P10 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.P11 { font-family:Arial; font-size:11pt; font-weight:bold; }
	*.P12 { font-family:Arial; font-size:11pt; font-weight:bold; }
	*.P13 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P14 { font-family:Arial; font-size:11pt; text-align:right ! important; }
	*.P15 { font-family:Arial; font-size:11pt; }
	*.P16 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P17 { font-family:Arial; font-size:11pt; }
	*.P18 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P19 { font-family:Arial; font-size:11pt; }
	*.P2 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P20 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.P21 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P22 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P23 { font-family:Arial; font-size:11pt; text-align:right ! important; }
	*.P24 { font-family:Arial; font-size:11pt; }
	*.P25 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P26 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P27 { font-family:Arial; font-size:11pt; color:#008000; }
	*.P28 { font-family:Arial; font-size:11pt; text-align:center ! important; color:#008000; }
	*.P29 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P3 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P30 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P31 { font-family:Arial; font-size:11pt; margin-top:0.494cm; margin-bottom:0.494cm; text-align:justify ! important; }
	*.P32 { font-family:Arial; font-size:11pt; margin-top:0.494cm; margin-bottom:0.494cm; text-align:justify ! important; }
	*.P33 { font-family:Arial; font-size:11pt; margin-top:0.494cm; margin-bottom:0.494cm; text-align:justify ! important; }
	*.P34 { font-family:Arial; font-size:11pt; margin-left:-0.284cm; margin-right:0cm; text-align:center ! important; text-indent:0cm; font-weight:bold; }
	*.P35 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P36 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P37 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P38 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P39 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P4 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P40 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P41 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P42 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P43 { font-family:Arial; font-size:11pt; text-align:left ! important; }
	*.P44 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.P45 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.P46 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; }
	*.P47 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; }
	*.P48 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; }
	*.P49 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; }
	*.P5 { font-family:Arial; font-size:11pt; text-align:center ! important; }
	*.P50 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; }
	*.P51 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; font-weight:bold; }
	*.P52 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; font-weight:bold; }
	*.P53 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; font-weight:bold; }
	*.P54 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-top:0.494cm; margin-bottom:0.494cm; font-weight:bold; }
	*.P55 { font-family:Arial; font-size:11pt; text-align:right ! important; margin-top:0.494cm; margin-bottom:0.494cm; }
	*.P56 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-left:1.27cm; margin-right:0cm; text-indent:0cm; }
	*.P57 { font-family:Arial; font-size:11pt; text-align:justify ! important; margin-left:0.635cm; margin-right:0cm; margin-top:0.494cm; margin-bottom:0.494cm; text-indent:0cm; color:#008000; font-weight:bold; }
	*.P58 { font-family:Arial; font-size:11pt; text-align:justify ! important; font-weight:bold; }
	*.P59 { font-family:Arial; font-size:11pt; line-height:100%; text-align:justify ! important; font-weight:bold; margin-top:0.494cm; margin-bottom:0cm; }
	*.P6 { font-family:Arial; font-size:11pt; }
	*.P60 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.P61 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:normal; margin-left:-0.953cm; margin-right:0cm; text-indent:0cm; }
	*.P62 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.P63 { font-family:Arial; font-size:11pt; margin-left:0cm; margin-right:0.635cm; text-indent:0cm; }
	*.P64 { font-family:'Arial Unicode MS'; font-size:11pt; margin-top:0.494cm; margin-bottom:0.494cm; text-align:justify ! important; }
	*.P65 { font-family:Arial; font-size:11pt; margin-top:0.494cm; margin-bottom:0.494cm; text-align:justify ! important; color:#ff0000; }
	*.P7 { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.P8 { font-family:Arial; font-size:11pt; text-align:justify ! important; font-weight:bold; }
	*.P9 { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.Standard { font-family:Arial; font-size:11pt; }
	*.TableContents { font-family:Arial; font-size:11pt; }
	*.TableHeading { font-family:Arial; font-size:11pt; text-align:center ! important; font-weight:bold; }
	*.Textbody { font-family:Arial; font-size:11pt; text-align:justify ! important; }
	*.Table1 { width:15.254cm; float:none; }
	*.Table2 { width:16.016cm; margin-left:-0.132cm; }
	*.Table3 { width:17.604cm; margin-left:-0.132cm; }
	*.Table4 { width:18.919cm; margin-left:-1.057cm; }
	*.Table5 { width:17.766cm; margin-left:-0.199cm; }
	*.Table6 { width:18.274cm; float:none; }
	*.Table1A1 { vertical-align:middle; padding:0.026cm; border-left-width:0.018cm; border-left-style:solid; border-left-color:#000000; border-right-style:none; border-top-width:0.018cm; border-top-style:solid; border-top-color:#000000; border-bottom-width:0.018cm; border-bottom-style:solid; border-bottom-color:#000000; }
	*.Table1C111 { vertical-align:middle; padding:0.026cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table2A1 { vertical-align:top; background-color:#c0c0c0; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table2A211 { vertical-align:top; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-left-width:0.018cm; border-left-style:solid; border-left-color:#000000; border-right-style:none; border-top-width:0.018cm; border-top-style:solid; border-top-color:#000000; border-bottom-width:0.018cm; border-bottom-style:solid; border-bottom-color:#000000; }
	*.Table2A3 { vertical-align:bottom; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-left-width:0.018cm; border-left-style:solid; border-left-color:#000000; border-right-style:none; border-top-width:0.018cm; border-top-style:solid; border-top-color:#000000; border-bottom-width:0.018cm; border-bottom-style:solid; border-bottom-color:#000000; }
	*.Table2G211 { vertical-align:top; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table2H3 { vertical-align:bottom; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table3A1 { vertical-align:top; background-color:#e0e0e0; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table3A2 { vertical-align:top; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-left-width:0.018cm; border-left-style:solid; border-left-color:#000000; border-right-style:none; border-top-width:0.018cm; border-top-style:solid; border-top-color:#000000; border-bottom-width:0.018cm; border-bottom-style:solid; border-bottom-color:#000000; }
	*.Table3A5 { vertical-align:bottom; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-left-width:0.018cm; border-left-style:solid; border-left-color:#000000; border-right-style:none; border-top-width:0.018cm; border-top-style:solid; border-top-color:#000000; border-bottom-width:0.018cm; border-bottom-style:solid; border-bottom-color:#000000; }
	*.Table3L2 { vertical-align:top; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table3N5 { vertical-align:bottom; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table4A1 { vertical-align:top; background-color:#e0e0e0; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table4A2 { vertical-align:top; padding-left:0.123cm; padding-right:0.123cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table5A1 { vertical-align:top; background-color:#e0e0e0; padding-left:0.191cm; padding-right:0.191cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table5A2 { vertical-align:top; background-color:#e6e6e6; padding-left:0.191cm; padding-right:0.191cm; padding-top:0cm; padding-bottom:0cm; border-left-width:0.018cm; border-left-style:solid; border-left-color:#000000; border-right-style:none; border-top-width:0.018cm; border-top-style:solid; border-top-color:#000000; border-bottom-width:0.018cm; border-bottom-style:solid; border-bottom-color:#000000; }
	*.Table5A3 { vertical-align:top; padding-left:0.191cm; padding-right:0.191cm; padding-top:0cm; padding-bottom:0cm; border-left-width:0.018cm; border-left-style:solid; border-left-color:#000000; border-right-style:none; border-top-width:0.018cm; border-top-style:solid; border-top-color:#000000; border-bottom-width:0.018cm; border-bottom-style:solid; border-bottom-color:#000000; }
	*.Table5B211 { vertical-align:top; background-color:#e6e6e6; padding-left:0.191cm; padding-right:0.191cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table5F3 { vertical-align:top; padding-left:0.191cm; padding-right:0.191cm; padding-top:0cm; padding-bottom:0cm; border-width:0.018cm; border-style:solid; border-color:#000000; }
	*.Table6A1 { vertical-align:top; padding-left:0.191cm; padding-right:0.191cm; padding-top:0cm; padding-bottom:0cm; border-style:none; }
	*.Table1A { width:4.052cm; }
	*.Table1B { width:2.494cm; }
	*.Table1C { width:1.083cm; }
	*.Table1J { width:1.127cm; }
	*.Table2A { width:1.711cm; }
	*.Table2B { width:1.588cm; }
	*.Table2D { width:1.905cm; }
	*.Table2E { width:1.27cm; }
	*.Table2G { width:3.334cm; }
	*.Table2H { width:3.351cm; }
	*.Table3A { width:1.08cm; }
	*.Table3B { width:1.081cm; }
	*.Table3E { width:1.199cm; }
	*.Table3F { width:0.953cm; }
	*.Table3H { width:1.27cm; }
	*.Table3K { width:1.588cm; }
	*.Table3L { width:1.905cm; }
	*.Table3N { width:1.288cm; }
	*.Table4A { width:18.919cm; }
	*.Table5A { width:7.493cm; }
	*.Table5B { width:2.05cm; }
	*.Table5C { width:2.051cm; }
	*.Table5F { width:2.069cm; }
	*.Table6A { width:8.28cm; }
	*.Table6B { width:1.27cm; }
	*.Table6C { width:8.724cm; }
	*.Table11 { }
	*.Table12 { }
	*.Table1C11 { }
	*.Table21 { }
	*.Table22 { }
	*.Table23 { }
	*.Table2A22 { }
	*.Table31 { }
	*.Table41 { }
	*.Table51 { }
	*.Table52 { }
	*.Table5B21 { }
	*.Table61 { }
	*.Fonteparágpadrão { }
	*.PageNumber { }
	*.T1 { }
	*.T10 { font-family:Arial; }
	*.T11 { font-family:Arial; font-size:11pt; }
	*.T12 { font-family:Arial; font-size:11pt; }
	*.T13 { font-weight:bold; }
	*.T14 { font-family:Arial; }
	*.T15 { font-family:Arial; }
	*.T16 { font-family:Arial; }
	*.T17 { font-family:Arial; }
	*.T18 { font-family:Arial; }
	*.T19 { color:#ff0000; font-family:Arial; }
	*.T2 { font-weight:normal; }
	*.T20 { color:#ff0000; font-family:Arial; font-size:11pt; }
	*.T21 { vertical-align:sup; font-family:Arial; font-size:11pt; }
	*.T22 { font-family:Verdana; }
	*.T3 { font-family:Arial; font-weight:normal; }
	*.T4 { font-family:Arial; }
	*.T5 { font-family:Arial; }
	*.T6 { font-family:Arial; }
	*.T7 { font-family:Arial; }
	*.T8 { font-family:Arial; }
	*.T9 { font-family:Arial; font-style:italic; font-weight:bold; }
	*.WW8Num1z0 { font-family:Arial; }
	*.WW8Num1z1 { font-family:Arial; }
	*.WW8Num1z2 { font-family:Arial; }
	*.WW8Num2z0 { font-family:Arial; }
	*.WW8Num2z1 { font-family:Arial; }
	*.WW8Num2z2 { font-family:Arial; }
	
	#tabelaexterna table {
		border-collapse: collapse !important;
		border-spacing: 0pt !important;
		empty-cells: show !important;
		border: 1px solid black !important;
	}
	
	#tabelaexterna th, #tabelaexterna td {
		border: 1px solid black !important;
	}
		
	</style>
</head>
<body dir="ltr">
<p class="P62">
</p>
<p class="P4"> </p>
<p class="P61">Ministério da Educação </p>
<p class="P5">COMISSÃO TÉCNICA PERMANENTE </p>
<h1 class="P59"><a name="Parecer_n_C2_BA" />Parecer n. #PARECER#</h1>
<p class="P25"><span class="T7">Processo n.</span> <?= $prefeitura["munprocesso"] ?></span></p>
<p class="P25"><span class="T7">Interessado:</span><span class="T4">
Prefeitura Municipal de </span><span class="T14"><?= $parecer_municipio ?></span></p>
<p class="P25"><span class="T7">Assunto:</span><span class="T4"> Plano
de Ações Articuladas/Plano de Metas Compromisso Todos pela Educação</span></p>
<p class="P7"> </p>
<p class="P32"><span class="T4">O Governo Federal, por intermédio do
Decreto n. 6.094, de 24 de abril de 2007, publicado no D.O.U. em 25 de
abril de 2007, dispõe sobre a implementação do "Plano de Metas
Compromisso todos pela Educação", assim definido em seu Art. 1. </span><span
	class="T9">"....é a conjunção dos esforços da União, Estados, Distrito
Federal e Municípios, atuando em regime de colaboração, das famílias e
da comunidade, em proveito da melhoria da qualidade da educação básica."</span><span
	class="T4"> </span></p>
<p class="P50">Considerando os princípios básicos do Plano de
Desenvolvimento da Educação - PDE - educação sistêmica, ordenação territorial e
desenvolvimento, com foco nos propósitos de melhoria da qualidade da
educação e na redução de desigualdades relativas às oportunidades
educacionais e, ainda, em atendimento ao disposto no &#167; 1. do Art. 9. e orientado
a partir dos eixos citados no &#167; 3. do Art. 8. do Decreto citado anteriormente, o
Ministério da Educação construiu 52 indicadores, distribuídos em 04 (quatro) Dimensões
(Gestão Educacional; Formação de Professores e Profissionais de Serviço e Apoio Escolar;
Práticas Pedagógicas e Avaliação; e Infra-estrutura física e Recursos Pedagógicos) que
nortearam os municípios na realização do diagnóstico da educação básica do sistema
local. </p>
<p class="P31"><span class="T4">A partir desse diagnóstico, o </span><span
	class="T5">Município de</span><span class="T6"> </span><span
	class="T14"><?= $parecer_municipio ?> </span><span class="T4">elaborou o Plano
de Ações Articuladas - PAR, definido no Art. 9. do referido Decreto, como
sendo </span><span class="T9">"...o conjunto articulado de ações,
apoiado técnica ou financeiramente pelo Ministério da Educação, que visa
ao cumprimento das metas do Compromisso e à observância das suas
diretrizes."</span><span class="T4"> </span></p>
<p class="P31"><span class="T4">O
Fundo Nacional de Desenvolvimento da Educação - FNDE
recebeu do (a) Prefeito (a)
Municipal, Sr. (a) </span><span class="T18"><?= $parecer_nome ?></span><span
	class="T16">,</span><span class="T4"> o PAR contendo as ações que podem
contribuir para a
melhoria das condições de acesso e permanência dos alunos, e para o desenvolvimento
da rede pública da educação básica, na forma do disposto no inciso I, do
&#167; 1., do Art. 14, Capítulo VII, da Resolução CD/FNDE N. 029, de 20 de
junho de 2007, alterada pela Resolução CD/FNDE N. 047, de 20 de setembro
de 2007. Ainda de acordo com a mesma resolução, o FNDE, após análise
preliminar, encaminhou o PAR do município de </span><span class="T15"><?= $parecer_municipio ?></span><span
	class="T4"> à Comissão Técnica Permanente, instituída pela
Resolução/CD/FNDE n. 29, de 20 de junho de 2007, em especial no disposto
em seu Art. 6., para análise e deliberação, em conformidade com as
diretrizes estabelecidas no Plano de Metas Compromisso Todos pela
Educação e, ainda, com os critérios definidos na Resolução/CD/FNDE/n.
029, de 20 de junho de 2007.</span></p>
<p class="P51">1 - Da Análise </p>
<p class="P46"><span class="T4">O </span><span class="T5">Município de</span><span
	class="T6"> </span><span class="T14"><?= $parecer_municipio ?></span><span
	class="T17"> </span><span class="T4">aderiu ao Plano de Metas
Compromisso "Todos pela Educação", sendo beneficiado com as ações
suplementares de assistência técnica e financeira de que trata a
Resolução CD/FNDE N. 029, de 20 de junho de 2007, alterada pela
Resolução CD/FNDE N. 047, de 20 de setembro de 2007.</span></p>
<p class="P46"><span class="T4">A tabela a seguir apresenta o IDEB do </span><span
	class="T5">Município de</span><span class="T6"> </span><span
	class="T14"><?= $parecer_municipio ?> </span><span class="T4">no ano de 2005,
juntamente com a projeção até 2021, decorrentes do desenvolvimento de
ações contempladas no Plano de Ações Articuladas/PAR, que possibilitarão
o cumprimento das diretrizes estabelecidas no Termo de Adesão e o
alcance das metas previstas.</span></p>

<?php
//echo pegarPagina(
//	"http://10.220.5.239:8079",
//	"Os dados do IDEB estão insdisponíveis"
//);
?>

<?php if( !empty( $prefeitura["muncodcompleto"] ) ): ?>
	<div id="tabelaexterna">
		<?php
		$ideb_municipio = $prefeitura["muncodcompleto"] . "-" . urlencode( str_to_upper( $prefeitura["mundescricao"] ) );
		$endereco = "http://ideb.inep.gov.br/Site/ContentManager.php?Area=tabMun&mun={$ideb_municipio}";
		echo pegarPagina(
			$endereco,
			"Os dados do IDEB estão insdisponíveis"
		);
		?>
	</div>
<?php endif; ?>


<p class="P64"><span class="T10">De acordo com os dados fornecidos pelo
INEP, o </span><span class="T5">Município de</span><span class="T6"> </span><span
	class="T14"><?= $parecer_municipio ?></span><span class="T19">  </span><span
	class="T5">apresenta os seguintes indicadores demográficos e
educacionais: </span></p>

<div id="tabelaexterna">
<?php
echo pegarPagina(
	"http://portal.mec.gov.br/ide/layout_tabelas/tabConsulta.php?municipio={$parecer_muncod}",
	"Os indicadores demográficos e educacionais estão indisponíveis."
);
?>
</div>

<div id="tabelaexterna">
<?php
echo pegarPagina(
	"http://portal.mec.gov.br/ide/layout_tabelas/tabConsulta2.php?municipio={$parecer_muncod}",
	"Os indicadores demográficos e educacionais estão indisponíveis."
);
?>
</div>

<div id="tabelaexterna">
<?php
$texto = pegarPagina(
	"http://portal.mec.gov.br/ide/layout_tabelas/tabConsulta3.php?municipio={$parecer_muncod}",
	"Os indicadores demográficos e educacionais estão indisponíveis."
);
echo str_replace( "Tabela 5", "Tabela 3", $texto );
?>
</div>

<p class="P49">O instrumento para coleta de informações qualitativas,
componente do Diagnóstico do Contexto Educacional, propõe a ponderação
de indicadores educacionais, estruturados em 4 (quatro) grandes
dimensões, contemplando aspectos da realidade do sistema municipal. De
forma geral, o diagnóstico realizado pelo Município apresentou a
seguinte pontuação:  </p>
<div style="text-align:left">

	<?php
	$itrid = cte_pegarItrid( $inuid );
	$sql = sprintf(
		"select
			ins.itrid
			,d.dimid
			,d.dimcod || '. ' ||d.dimdsc as dimdsc
			,c.ctrpontuacao 
			,count ( c.ctrpontuacao ) as qtpontos
		from cte.instrumento ins
			inner join cte.instrumentounidade iu on iu.itrid = ins.itrid and iu.inuid = %d
			inner join cte.dimensao d on d.itrid = ins.itrid
			inner join cte.areadimensao ad on d.dimid = ad.dimid
			inner join cte.indicador i on i.ardid = ad.ardid
			inner join cte.criterio c on c.indid = i.indid
			left join cte.pontuacao pt on pt.crtid = c.crtid and pt.indid = i.indid and pt.inuid = %d
		where
			pt.ptostatus = 'A'
			and d.dimstatus = 'A'
			and ad.ardstatus = 'A'  
			and i.indstatus = 'A'
		group by
			ins.itrid,
			d.dimcod,
			d.dimdsc,
			c.ctrpontuacao,
			d.dimid,
			d.dimcod
		order by
			d.dimcod
		"
		,$inuid//$_SESSION['inuid']
		,$inuid//$_SESSION['inuid']
	);
	if( $resultado = $db->carregar($sql) )
	{
	if(!isset($icone)){$icone ="";}
		//percorrendo o resultado e criando um array por dimensao 
		foreach($resultado as $key => $val )
		{
			$cor = $icone ? '#959595' : '#133368';
			$relatorio[$val["dimid"]]["dimdsc"] = $val["dimdsc"];
			switch($val["ctrpontuacao"])
			{
				case "0":
					$relatorio[$val["dimid"]]["0"] = $val["qtpontos"];
					break;
				case "1":
					$relatorio[$val["dimid"]]["1"] = $val["qtpontos"];
					break;
				case "2":
					$relatorio[$val["dimid"]]["2"] = $val["qtpontos"];
					break;
				case "3":
					$relatorio[$val["dimid"]]["3"] = $val["qtpontos"];
					break;
				case "4":
					$relatorio[$val["dimid"]]["4"] = $val["qtpontos"];
					break;
			}
		}
	}
	$total["t0"] = 0;
	$total["t1"] = 0;
	$total["t2"] = 0;
	$total["t3"] = 0;
	$total["t4"] = 0;
	$cor = '#e7e7e7';
	?>
	<?php if( isset($relatorio)): ?>
		<table border="0" width="95%" cellspacing="0" cellpadding="4" align="center" bgcolor="#DCDCDC" class="listagem">
			<thead>
			<tr style="border-bottom:4px solid black;">
				<th rowspan="2">Dimensão</th>
				<th colspan="5">Pontuação</th>
			</tr>
			<tr>
				<th>4</th>
				<th>3</th>
				<th>2</th>
				<th>1</th>
				<th>n/a</th>
			</tr>
			</thead>
			<?php foreach($relatorio as $keyr => $valr ): 
					if(!isset($valr["0"])){$valr["0"] ="";}
					if(!isset($valr["1"])){$valr["1"] ="";}
					if(!isset($valr["2"])){$valr["2"] ="";}
					if(!isset($valr["3"])){$valr["3"] ="";}
					if(!isset($valr["4"])){$valr["4"] ="";}
			?>
				<tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';">
					<td><?php echo $valr["dimdsc"]; ?></td>
					<td align="right"><?php echo (int)$valr["4"]; ?>&nbsp;</td>
					<td align="right"><?php echo (int)$valr["3"]; ?>&nbsp;</td>
					<td align="right"><?php echo (int)$valr["2"]; ?>&nbsp;</td>
					<td align="right"><?php echo (int)$valr["1"]; ?>&nbsp;</td>
					<td align="right"><?php echo (int)$valr["0"]; ?>&nbsp;</td>
				</tr>
				<?php 
					$total["t0"] += (int)$valr["0"];
					$total["t1"] += (int)$valr["1"];
					$total["t2"] += (int)$valr["2"];
					$total["t3"] += (int)$valr["3"];
					$total["t4"] += (int)$valr["4"];
					if($cor == '#e7e7e7') $cor = '#ffffff'; else $cor = '#e7e7e7';
				?>
			<?php endforeach; ?>
				<tr>
					<td align="right"><b>Total:</b></td>
					<td align="right"><b><?php echo $total["t4"]; ?>&nbsp;</b></td>
					<td align="right"><b><?php echo $total["t3"]; ?>&nbsp;</b></td>
					<td align="right"><b><?php echo $total["t2"]; ?>&nbsp;</b></td>
					<td align="right"><b><?php echo $total["t1"]; ?>&nbsp;</b></td>
					<td align="right"><b><?php echo $total["t0"]; ?>&nbsp;</b></td>
				</tr>
				<tfoot>
				<tr >
						<td colspan="6" align="right">*n/a :  Não se Aplica.</td>
				</tr>
				<tfoot>
		</table>
	<?php else: ?>
		<table class="tabela" align="center" bgcolor="#fafafa"><tr><td align="center" style="color:red;">Nenhum Indicador Pontuado.</td></tr></table>
	<?php endif; ?>

<p class="P47"><span class="T4">A partir da pontuação gerada para cada
indicador presente nas 04 (quatro) Dimensões, o município de </span><span
	class="T16"><?= $parecer_municipio ?> </span><span class="T4">elaborou o PAR
detalhando suas ações. </span></p>
<!--
<p class="P50">O detalhamento das ações, contempladas no PAR do
MUNICÍPIO, está organizado de acordo com a forma de execução, estratégia
de implementação e cronograma, com metas quantitativas para execução de
2007 a 2011 e apresenta ações voltadas para:</p>
-->
<p class="P50">
O resultado do detalhamento das ações, contempladas no PAR do MUNICÍPIO,
está organizado de acordo com a forma de execução, estratégia de implementação
e cronograma, com metas quantitativas e qualitativas para execução de 2007 a
2011 e apresenta as seguintes ações:
</p>


<?php

$sql = sprintf(
	"select distinct ai.aciid, ai.acidsc, p.ptoid, i.indid
	from cte.pontuacao p
	inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
	inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	where p.inuid = %d and p.ptostatus = 'A'",
	$inuid
);
$_acoes = $db->carregaAgrupado( $sql, "indid" );

$sql = sprintf(
	"select distinct i.indid, i.indcod, i.inddsc, ad.ardid
	from cte.pontuacao p
	inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
	inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	where p.inuid = %d and p.ptostatus = 'A'
	order by ad.ardid, i.indcod",
	$inuid
);
$_indicadores = $db->carregaAgrupado( $sql, "ardid" );

$sql = sprintf(
	"select distinct ad.ardid, ad.ardcod, ad.arddsc, d.dimid
	from cte.pontuacao p
	inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
	inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	where p.inuid = %d and p.ptostatus = 'A'
	order by d.dimid, ad.ardcod",
	$inuid
);
$_areas = $db->carregaAgrupado( $sql, "dimid" );

$sql = sprintf(
	"select distinct d.dimid, d.dimcod, d.dimdsc
	from cte.pontuacao p
	inner join cte.acaoindicador ai on ai.ptoid = p.ptoid
	inner join cte.indicador i on i.indid = p.indid and i.indstatus = 'A'
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	where p.inuid = %d and p.ptostatus = 'A'
	order by d.dimcod",
	$inuid
);
$_dimensoes = $db->carregar( $sql );

?>
<table style="margin-left: 10px;">
	<?php foreach( $_dimensoes as $dimensao ): ?>
	<tr>
		<td style="padding: 0 0 0 0;">
			<h2 style="border-top: 1px solid #000000; border-bottom: 1px solid #000000;"><?= $dimensao["dimcod"] ?>. <?= $dimensao["dimdsc"] ?></h2>
		</td>
	</tr>
		<?php foreach( $_areas[$dimensao["dimid"]] as $area ): ?>
		<tr>
			<td style="padding: 0 0 0 30px;">
				<h3><?= $area["ardcod"] ?>. <?= $area["arddsc"] ?></h3>
			</td>
		</tr>
			<?php foreach( $_indicadores[$area["ardid"]] as $indicador ): ?>
			<tr>
				<td style="padding: 0 0 0 60px;">
					<!-- <h4><?= $indicador["indcod"] ?>. <?= $indicador["inddsc"] ?></h4> -->
					<h4>- <?= $indicador["inddsc"] ?></h4>
				</td>
			</tr>
				<?php $acoesUtilizadas = array(); ?>
				<?php foreach( $_acoes[$indicador["indid"]] as $acao ): ?>
					<?php
					if ( in_array( $acao["acidsc"], $acoesUtilizadas ) )
					{
						continue;
					}
					array_push( $acoesUtilizadas, $acao["acidsc"] );
					?>
					<tr>
						<td style="padding: 0 0 0 90px;">
							<h5>- <?= $acao["acidsc"] ?></h5>
						</td>
					</tr>
				<?php endforeach; ?>
			<?php endforeach; ?>
		<?php endforeach; ?>
	<?php endforeach; ?>
</table>
</div>

<p class="P47"><span class="T4">Dessa forma, o Plano de Ações
Articuladas/PAR do </span><span class="T5">Município de</span><span
	class="T6"> </span><span class="T8"><?= $parecer_municipio ?></span><span
	class="T4">, composto de diagnóstico do contexto educacional e das
metas a atingir para o desenvolvimento do IDEB, apresenta coerência com
as diretrizes do Plano de Metas Compromisso Todos pela Educação, Decreto
n. 6.094, de 24 de abril de 2007.</span></p>
<p class="P47"><span class="T4"> </span><span class="T6" style="font-weight: bold;">2 - Da
conclusão</span></p>
<p class="P33">De acordo com Art. 10. do Decreto 6.094 de 24 de abril de
2007,   o PAR será base para termo de convênio ou de cooperação técnica
firmado entre o Ministério da Educação e o ente apoiado.  </p>
<p class="P46"><span class="T4">Tendo procedido à análise do PAR, esta
Comissão Técnica Permanente </span><span class="T5">recomenda a
implementação para os exercícios 2007 a 2011 das ações nele constantes e</span><span
	class="T4"> encaminha o presente parecer e o respectivo Plano ao FNDE
para execução, a partir da análise dos custos estimados para realização
das ações contempladas no PAR, consoante a confirmação da
disponibilidade orçamentária.</span></p>
<p class="P49">Cabe destacar que a elaboração do Plano de Trabalho, na
forma do anexo III, da Resolução CD/FNDE N. 029, de 20 de junho de 2007,
alterada pela Resolução CD/FNDE N. 047, de 20 de setembro de 2007,
deverá indicar o conjunto de ações que o MUNICÍPIO pretende propor como
meta para o alcance dos objetivos operacionais necessários à melhoria
dos índices previstos para o primeiro ano, garantindo-se a
sustentabilidade para a execução das ações subseqüentes, que compõem o
PAR.</p>
<p class="P46"><span class="T4">Assim, a Comissão Técnica Permanente </span><span
	class="T5">aprova</span><span class="T4"> o Plano de Ações
Articuladas/PAR e recomenda a formalização de convênio e termo de
cooperação técnica, com a apresentação de plano de trabalho, na forma do
disposto na Resolução CD/FNDE N. 029, de 20 de junho de 2007, alterada
pela Resolução CD/FNDE N. 047, de 20 de setembro de 2007.</span></p>
<p class="P49">É este o parecer desta Comissão Técnica Permanente. </p>
<p class="P49">Encaminhe-se ao FNDE. </p>
<p class="P55"><span class="T4">Brasília,         </span><span
	class="T16"><?= $parecer_data ?></span><span class="T4"></span></p>
<p class="P38"> </p>
<div style="text-align:center">
<table border="0" cellspacing="0" cellpadding="0" class="Table6">
	<colgroup>
		<col width="362" />
		<col width="56" />
		<col width="381" />
	</colgroup>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P39">________________________________ </p>
		<p class="P44"> </p>
		<p class="P38">Representante da SEB/MEC </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P42">___________________________________ </p>
		<p class="P44"> </p>
		<p class="P41">Representante do FNDE </p>
		</td>
	</tr>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P40"> </p>
		<p class="P37"> </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P43"> </p>
		</td>
	</tr>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P39">________________________________ </p>
		<p class="P44"> </p>
		<p class="P38">Representante do INEP </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P39">___________________________________ </p>
		<p class="P44"> </p>
		<p class="P38">Representante da CAPES </p>
		</td>
	</tr>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P39"> </p>
		<p class="P38"> </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P39"> </p>
		</td>
	</tr>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P39">_______________________________ </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P39">__________________________________ </p>
		</td>
	</tr>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P45"> </p>
		<p class="P38">Representante da SECAD </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P45"> </p>
		<p class="P38">Representante da SEESP </p>
		</td>
	</tr>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P39"> </p>
		<p class="P38"> </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P43"> </p>
		</td>
	</tr>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P39">_______________________________ </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P43"> </p>
		</td>
	</tr>
	<tr class="Table61">
		<td style="text-align:left;width:8.28cm; " class="Table6A1">
		<p class="P45"> </p>
		<p class="P38">Representante da SEED </p>
		</td>
		<td style="text-align:left;width:1.27cm; " class="Table6A1">
		<p class="P40"> </p>
		</td>
		<td style="text-align:left;width:8.724cm; " class="Table6A1">
		<p class="P43"> </p>
		</td>
	</tr>
</table>
</div>
<p class="Textbody"> </p>
<p class="Standard"> </p>

</body>
</html>