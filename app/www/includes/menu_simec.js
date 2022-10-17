
isNS4 = (document.layers) ? true : false;
NS4 = (document.layers);
IE4 = (document.all);
ver4 = (NS4 || IE4);isMac = (navigator.appVersion.indexOf("Mac") != -1);
isMenu = (NS4 || (IE4 && !isMac));


var platform = navigator.appVersion;
isUNIX = (platform.indexOf("X11") != -1) ||  (platform.indexOf("Linux") != -1) ||  (platform.indexOf("SunOS") != -1) ||  (platform.indexOf("IRIX") != -1) ||   (platform.indexOf("HP-UX") != -1);
if (!ver4) event = null;browser = (((navigator.appName == "Netscape") && (parseInt(navigator.appVersion) >= 3 )) || ((navigator.appName == "Microsoft Internet Explorer") && (parseInt(navigator.appVersion) >= 4 )));



if (NS4) 
{
	document.write('<layer name="searchBox2" left="20px" top="59px" width="117px" height="20px" visibility="hidden" id="searchBox2" z-index="0"><table cellpadding="0" cellspacing="0" border="0"><tr><td><a href="#" onMouseOver="javascript:restoreSearch();"><img src="stylesheet/ns4search.gif" vspace="0" border="0" alt=""><\/a><\/td><\/tr><\/table><\/layer>');
}

menunum=0;
menus=new Array();
_d=document;

function addmenu()
{
	menunum++;menus[menunum]=menu;
}

function dumpmenus()
{
	mt="<script language=javascript>";
	for(a=1;a<menus.length;a++)
		{
			mt+=" menu"+a+"=menus["+a+"];"}mt+="<\/script>";
			_d.write(mt)
		}
	timegap=1000;
	followspeed=3;
	followrate=5;
	suboffset_top=0;
	suboffset_left=0;
	if(navigator.appVersion.indexOf("MSIE 6.0")>0)
		{	
			effect = "gradientwipe(size=1.00, wipestyle=1, motion=Forward, duration=0.3);Fade(duration=0.2);Alpha(style=0,opacity=95);"
		}
		else
		{
			effect = "Alpha(style=0,opacity=90);"
		}
		
		style2=[				// style1 is an array of properties. You can have as many property arrays as you need. This means that menus can have their own style.
				"000000",					// Mouse Off Font Color
				"9CBACE",				// Mouse Off Background Color
				"ffffff",				// Mouse On Font Color
				"85ABE5",				// Mouse On Background Color
				"528294",				// Menu Border Color 
				11,						// Font Size in pixels
				"normal",				// Font Style (italic or normal)
				"normal",					// Font Weight (bold or normal)
				"Tahoma",		// Font Name
				3,						// Menu Item Padding
				"imagens/arrow_b.gif",			// Sub Menu Image (Leave this blank if not needed)
				"000000",						// 3D Border & Separator bar
				"000000",				// 3D High Color
				"000000",				// 3D Low Color
				"000000",				// Current Page Item Font Color (leave this blank to disable)
				"000000",					// Current Page Item Background Color (leave this blank to disable)
				"imagens/arrow_b.gif",			// Top Bar image (Leave this blank to disable)
				"ffffff",				// Menu Header Font Color (Leave blank if headers are not needed)
				"000099",				// Menu Header Background Color (Leave blank if headers are not needed)
				]
		//style2=["ffffff","1c4267","FFFFFF","1b5e92","333333",10,"normal","normal","Verdana, Tahoma, Arial, Helvetica, sans-serif",3,"imagens/arrow.gif",,"66ffff","000099","2a2a2a","e0e0e0","imagens/arrow_b.gif","ffffff","000099",];
		
		style1=["333333","CEDFE7","FFFFFF","85ABE5","333333",10,"normal","normal","Verdana",4,"imagens/arrow.gif",0,"ffffff","000000","2a2a2a","e0e0e0","http://www.bdv.com.br/classificar/arrow_b.gif","ffffff","000099",];
		
		style3=["ffffff","000000","FFFFFF","000000","3c3c3c",10,"normal","normal","Verdana, Tahoma, Arial, Helvetica, sans-serif",3,"http://www.bdv.com.br/classificar/arrow.gif",,"66ffff","000099","2a2a2a","e0e0e0","http://www.bdv.com.br/classificar/arrow_b.gif","ffffff","000099",];
		
		addmenu(menu=["mainmenu",	// Menu Name - This is needed in order for the menu to be called
						50,		// Menu Top - The Top position of the menu in pixels
						10,			// Menu Left - The Left position of the menu in pixels
						80,			// Menu Width - Menus width in pixels
						0,			// Menu Border Width 
						,			// Screen Position - here you can use "center;left;right;middle;top;bottom" or a combination of "center:middle"
						style2,		// Properties Array - this is set higher up, as above
						1,			// Always Visible - allows the menu item to be visible at all time (1=on/0=off)
						"left",			// Alignment - sets the menu elements text alignment, values valid here are: left, right or center
						,			// Filter - Text variable for setting transitional effects on menu activation - see above for more info
						1,			// Follow Scrolling - Tells the menu item to follow the user down the screen (visible at all times) (1=on/0=off)
						1,			// Horizontal Menu - Tells the menu to become horizontal instead of top to bottom style (1=on/0=off)
						1,			// Keep Alive - Keeps the menu visible until the user moves over another menu or clicks elsewhere on the page (1=on/0=off)
						"left",	// Position of TOP sub image left:center:right
						1,			// Set the Overall Width of Horizontal Menu to 100% and height to the specified amount (Leave blank to disable)
						,			// Right To Left - Used in Hebrew for example. (1=on/0=off)
						0,			// Open the Menus OnClick - leave blank for OnMouseover (1=on/0=off)
						,			// ID of the div you want to hide on MouseOver (useful for hiding form elements)
						,			// Reserved for future use
						,			// Reserved for future use
						,			// Reserved for future use
						
						1,
						"Inicial",
						"http://www.bdv.com.br/classificar/admin/inicial.asp",
						"# ",
						"Inicial",
						
						1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Monitoração&nbsp;&nbsp;&nbsp;",
						"show-menu=Monitoração",
						"# ",
						,
						
						1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Cadastro",
						"show-menu=Cadastro",
						"# ",
						,
						
						1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Financeiro",
						"show-menu=Financeiro",
						"# ",
						,
						
						1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Sistema",
						"show-menu=Sistema",
						"# ",
						,
						
						
						1]);
		
		
		
		
		addmenu(menu=["Inicial",23,-1,25,1,"",style1,,,effect,,,,,,,,,,,,," ","/",,,1]);
		addmenu(menu=["Monitoração",,,171,1,,style1,,,effect,,,0,,,,,,,,,,"Monitor de Clientes","http://www.bdv.com.br/classificar/admin/monitoracao/monitor_cliente.asp",,"Monitor de Clientes",0,"Monitor de Anuncios","http://www.bdv.com.br/classificar/admin/monitoracao/monitor_anuncio.asp",,"Monitor de Anuncios",0,"Tráfego","http://www.bdv.com.br/classificar/admin/monitoracao/trafego.asp",,"Tráfego",0,"Estatisticas Site","http://www.bdv.com.br/classificar/admin/monitoracao/busca_estatistica_site.asp",,"Estatisticas Site",0,"Recomenda site","http://www.bdv.com.br/classificar/admin/monitoracao/recomenda_site.asp",,"Recomenda site",0,"LOG Anunciante","http://www.bdv.com.br/classificar/admin/monitoracao/busca_log_anunciante.asp",,"LOG Anunciante",0,"Monitor de Opiniões","http://www.bdv.com.br/classificar/admin/monitoracao/monitor_opiniao.asp",,"Monitor de Opiniões",0,"Aviso de Emails p\/ Clientes","http://www.bdv.com.br/classificar/admin/monitoracao/busca_aviso_emails.asp",,"Aviso de Emails p\/ Clientes",0,"LOG busca","http://www.bdv.com.br/classificar/admin/monitoracao/busca_log_busca.asp",,"LOG busca",0,"Envia Emails p\/ Clientes","http://www.bdv.com.br/classificar/admin/monitoracao/envia_email_cliente.asp",,"Envia Emails p\/ Clientes",0,"Monitor de Sites","http://www.bdv.com.br/classificar/admin/monitoracao/monitor_site.asp",,"Monitor de Sites",0,"Monitor de Vendedores","show-menu=504","javascript:void(0);","Monitor de Vendedores",0,"Monitor de Contatos\/Recomendas","show-menu=510","javascript:void(0);","Monitor de Contatos\/Recomendas",0,"Monitor de Banners","http://www.bdv.com.br/classificar/admin/monitoracao/monitor_banners.asp",,"Monitor de Banners",0,"Relatório Mensal p\/clientes","http://www.bdv.com.br/classificar/admin/monitoracao/consulta_relatorio_mensal_cli.asp",,"Relatório Mensal p\/clientes",0]);
		addmenu(menu=["Cadastro",,,171,1,,style1,,,effect,,,0,,,,,,,,,,"Banners","show-menu=60","javascript:void(0);","Banners",0,"Opcionais","show-menu=70","javascript:void(0);","Opcionais",0,"Notícias","show-menu=80","javascript:void(0);","Notícias",0,"Serviços\/Igreja","show-menu=85","javascript:void(0);","Serviços\/Igreja",0,"Tipo Email","show-menu=400","javascript:void(0);","Tipo Email",0,"Aviso de Emails","show-menu=410","javascript:void(0);","Aviso de Emails",0,"Tipo Veiculos & Marcas\/Modelos","show-menu=420","javascript:void(0);","Tipo Veiculos & Marcas\/Modelos",0,"Pacotes Promocionais","show-menu=425","javascript:void(0);","Pacotes Promocionais",0,"Fórum","show-menu=435","javascript:void(0);","Fórum",0,"Contrato de Pacote p\/ Clientes","show-menu=450","javascript:void(0);","Contrato de Pacote p\/ Clientes",0,"Financiamento p\/ Clientes","show-menu=455","javascript:void(0);","Financiamento p\/ Clientes",0]);
		addmenu(menu=["Financeiro",,,171,1,,style1,,,effect,,,0,,,,,,,,,,"Boletos","show-menu=91","javascript:void(0);","Boletos",0]);
		addmenu(menu=["Sistema",,,171,1,,style1,,,effect,,,0,,,,,,,,,,"Usuários","http://www.bdv.com.br/classificar/admin/usuarios/cadastro.asp",,"Usuários",0,"Menu","show-menu=110","javascript:void(0);","Menu",0,"Perfil","show-menu=130","javascript:void(0);","Perfil",0,"FTP","http://www.bdv.com.br/classificar/admin/web_ex/webexplorer.asp",,"FTP",0]);
		
		addmenu(menu=["12","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["13","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["14","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["15","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["17","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["18","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["19","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["500","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["501","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["502","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["503","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["504","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Monitora","http://www.bdv.com.br/classificar/admin/monitoracao/monitor_vendedor.asp",,"Monitora",1	,"Vincula ao cliente","http://www.bdv.com.br/classificar/admin/monitoracao/vincula_vendedor_cliente.asp",,"Vincula ao cliente",1	,"Desvincula ao cliente","http://www.bdv.com.br/classificar/admin/monitoracao/desvincula_vendedor_cliente.asp",,"Desvincula ao cliente",1]);	
		addmenu(menu=["510","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Contatos","http://www.bdv.com.br/classificar/admin/monitoracao/busca_contato_anuncio.asp",,"Contatos",1	,"Qtd. Contatos\/Recomedas","http://www.bdv.com.br/classificar/admin/monitoracao/busca_contato_recomeda_anuncio.asp",,"Qtd. Contatos\/Recomedas",1]);	
		addmenu(menu=["515","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["517","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["60","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra Banner","http://www.bdv.com.br/classificar/admin/banners/cadastra_banner.asp",,"Cadastra Banner",1	,"Consulta Banners","http://www.bdv.com.br/classificar/admin/banners/consulta_banner.asp",,"Consulta Banners",1]);	
		addmenu(menu=["70","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra\/Altera Opcionais","http://www.bdv.com.br/classificar/admin/cadastro_opcionais/cadastro.asp",,"Cadastra\/Altera Opcionais",1	,"Vincula Opcionais\/Tipo Veículo","http://www.bdv.com.br/classificar/admin/cadastro_opcionais/vincula_opcionais.asp",,"Vincula Opcionais\/Tipo Veículo",1	,"Consulta Opcionais\/Tipo Veículo","http://www.bdv.com.br/classificar/admin/cadastro_opcionais/vincula_opcionais.asp",,"Consulta Opcionais\/Tipo Veículo",1]);	
		addmenu(menu=["80","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra Notícias","http://www.bdv.com.br/classificar/admin/noticias/cadastro1.asp",,"Cadastra Notícias",1	,"Consulta\/ Edita\/ Publica Notícias","http://www.bdv.com.br/classificar/admin/noticias/consulta.asp",,"Consulta\/ Edita\/ Publica Notícias",1]);
		addmenu(menu=["85","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra serviços","http://www.bdv.com.br/classificar/admin/servicos/cadastro.asp",,"Cadastra serviços",1	,"Consulta\/Edita","http://www.bdv.com.br/classificar/admin/servicos/consulta.asp",,"Consulta\/Edita",1]);
		addmenu(menu=["400","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra Tipo Email","http://www.bdv.com.br/classificar/admin/tipo_email/cadastro.asp",,"Cadastra Tipo Email",1	,"Consulta\/Edita","http://www.bdv.com.br/classificar/admin/tipo_email/consulta.asp",,"Consulta\/Edita",1]);
		addmenu(menu=["410","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra Aviso Emails","http://www.bdv.com.br/classificar/admin/aviso_email/cadastro.asp",,"Cadastra Aviso Emails",1	,"Consulta\/Edita","http://www.bdv.com.br/classificar/admin/aviso_email/consulta.asp",,"Consulta\/Edita",1]);
		addmenu(menu=["420","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra Tipo","http://www.bdv.com.br/classificar/admin/tipo_veiculo/cadastro.asp",,"Cadastra Tipo",1	,"Consulta\/Edita Tipo","http://www.bdv.com.br/classificar/admin/tipo_veiculo/consulta.asp",,"Consulta\/Edita Tipo",1	,"Marcas\/Modelos","http://www.bdv.com.br/classificar/admin/tipo_veiculo/consulta_marca_modelo.asp",,"Marcas\/Modelos",1]);
		addmenu(menu=["425","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra","http://www.bdv.com.br/classificar/admin/pacotes/cadastro.asp",,"Cadastra",1	,"Consulta\/Edita","http://www.bdv.com.br/classificar/admin/pacotes/consulta.asp",,"Consulta\/Edita",1	,"Pacote Exclusivo p\/ Clientes","http://www.bdv.com.br/classificar/admin/pacotes/pacote_exclusivo.asp",,"Pacote Exclusivo p\/ Clientes",1]);
		addmenu(menu=["435","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra Indice","http://www.bdv.com.br/classificar/admin/forum/cadastra_indice.asp",,"Cadastra Indice",1	,"Consulta\/Edita Indice","http://www.bdv.com.br/classificar/admin/forum/consulta_indice.asp",,"Consulta\/Edita Indice",1	,"Cadastra Fórum","http://www.bdv.com.br/classificar/admin/forum/cadastra_forum.asp",,"Cadastra Fórum",1	,"Consulta\/Edita Forum","http://www.bdv.com.br/classificar/admin/forum/consulta_forum.asp",,"Consulta\/Edita Forum",1	,"Consulta\/Edita Tópicos","http://www.bdv.com.br/classificar/admin/forum/consulta_topicos.asp",,"Consulta\/Edita Tópicos",1]);	
		addmenu(menu=["450","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra","http://www.bdv.com.br/classificar/admin/contrato_pacote/cadastro.asp",,"Cadastra",1	,"Consulta","http://www.bdv.com.br/classificar/admin/contrato_pacote/consult.asp",,"Consulta",1]);	
		addmenu(menu=["455","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra\/Edita","http://www.bdv.com.br/classificar/admin/financiamento/cadastro.asp",,"Cadastra\/Edita",1]);	
		addmenu(menu=["91","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Lista\/Baixa  Boletos VEICULOS","http://www.bdv.com.br/classificar/admin/financeiro/lista_baixa_boleto.asp",,"Lista\/Baixa  Boletos VEICULOS",1	,"Paga Boleto de Veículos por Código","http://www.bdv.com.br/classificar/admin/financeiro/paga_boleto_codigo_veic.asp",,"Paga Boleto de Veículos por Código",1	,"Lista\/Baixa  Boletos BANNERS","http://www.bdv.com.br/classificar/admin/financeiro/lista_baixa_boleto_banner.asp",,"Lista\/Baixa  Boletos BANNERS",1]);	
		addmenu(menu=["105","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);	
		addmenu(menu=["110","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastra Menu","http://www.bdv.com.br/classificar/admin/menu/cadastro.asp",,"Cadastra Menu",1	,"Altera Menu","http://www.bdv.com.br/classificar/admin/menu/edita.asp",,"Altera Menu",1]);	
		addmenu(menu=["130","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,	,"Cadastro\/Altera Perfil","http://www.bdv.com.br/classificar/admin/perfil/cadastro.asp",,"Cadastro\/Altera Perfil",1	,"Gera Script Menu para todos os Perfis","http://www.bdv.com.br/classificar/admin/menu/edita_script_menu.asp",,"Gera Script Menu para todos os Perfis",1]);	
		addmenu(menu=["136","offset=3",,150,1,"",style1,,,effect,,,,,,,,,,,,]);
		
		dumpmenus();
		



