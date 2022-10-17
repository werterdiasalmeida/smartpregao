<?

 /*
   Sistema Sistema Simec
   Setor responsável: SPO/MEC
   Desenvolvedor: Desenvolvedores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo: menu_simec.inc
   Finalidade: montagem do menu do sistema
   Data de criação: 24/06/2005
   */
?>
<script>

  isNS4 = (document.layers) ? true : false;
  NS4 = (document.layers);
  IE4 = (document.all);
  ver4 = (NS4 || IE4);isMac = (navigator.appVersion.indexOf("Mac") != -1);  
  isMenu = (NS4 || (IE4 && !isMac));


  var platform = navigator.appVersion;
  isUNIX = (platform.indexOf("X11") != -1) || (platform.indexOf("Linux") != -1) || (platform.indexOf("SunOS") != -1) || (platform.indexOf("IRIX") != -1) || (platform.indexOf("HP-UX") != -1);
  if (!ver4) event = null;browser = (((navigator.appName == "Netscape") && (parseInt(navigator.appVersion) >= 3 )) || ((navigator.appName == "Microsoft Internet Explorer") && (parseInt(navigator.appVersion)  >= 4 )));
  


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
			effect = "gradientwipe(size=1.00, wipestyle=1, motion=Forward, duration=0.3);Fade(duration=0.2);Alpha(style=0,opacity=95);Shadow(color='#777777', Direction=135, Strength=5);"
		}
		else
		{
			effect = "Alpha(style=0,opacity=90);Shadow(color='#777777', Direction=135, Strength=5);"
		}
		

		
		/*style2=[	"ffffff",					// Mouse Off Font Color
				"2E4A67",				// Mouse Off Background Color
				"ffffff",				// Mouse On Font Color
				"85ABE5",				// Mouse On Background Color
				"528294",				// Menu Border Color 
				11,						// Font Size in pixels
				"normal",				// Font Style (italic or normal)
				"normal",					// Font Weight (bold or normal)
				"Tahoma",		// Font Name
				3,						// Menu Item Padding
				"../imagens/arrow_b.gif",			// Sub Menu Image (Leave this blank if not needed)
				"000000",						// 3D Border & Separator bar
				"000000",				// 3D High Color
				"000000",				// 3D Low Color
				"000000",				// Current Page Item Font Color (leave this blank to disable)
				"000000",					// Current Page Item Background Color (leave this blank to disable)
				"../imagens/arrow_b.gif",			// Top Bar image (Leave this blank to disable)
				"ffffff",				// Menu Header Font Color (Leave blank if headers are not needed)
				"000099",				// Menu Header Background Color (Leave blank if headers are not needed)
				]
		//style1= ["ffffff","1c4267","FFFFFF","1b5e92","333333",10,"normal","normal","Verdana, Tahoma, Arial, Helvetica, sans-serif",3,"../imagens/arrow.gif",,"66ffff","000099","2a2a2a","e0e0e0","imagens/arrow_b.gif","ffffff","000099",];
		
		//style1=[","Verdana",4,"../imagens/arrow.gif","ffffff","000000",	"2a2a2a","e0e0e0","","ffffff","000099",];
		

				// Menu Header Background Color (Leave blank if headers are not needed)
				//]
		style1=["333333","CEDFE7","FFFFFF","85ABE5","333333",10,"normal","normal","Verdana",4,"../imagens/arrow.gif",0,"ffffff","000000","2a2a2a","e0e0e0","imagens/arrow.gif","ffffff","000099",];
		
		//style1=["ffffff","000000","FFFFFF","000000","3c3c3c",10,"normal","normal","Verdana, Tahoma, Arial, Helvetica, sans-serif",3,"../imagens/arrow.gif",,"66ffff","000099","2a2a2a","e0e0e0","imagens/arrow.gif","ffffff","000099",];
		
          */
		  
		  style1=["2A2A2A","FFF2C1","FFFFFF","1b5e92","666666",10,"normal","bold","Verdana",4,"../imagens/arrow.gif",0,"ffffff","000000","2a2a2a","FFF2C1","imagens/arrow_b.gif","ffffff","000099",];
		  style2=["F0F0F0","3B5D82","FFFFFF","336699","336699",10,"normal","bold","Verdana, Tahoma, Arial, Helvetica, sans-serif",3,"../imagens/arrow_b.gif",,"66ffff","000099","1c4267","e0e0e0","imagens/arrow_b.gif","ffffff","000099",];   
  
		addmenu(menu=["mainmenu", // Menu Name - This is needed in order for the menu to be called
						97, // Menu Top - The Top position of the menu in pixels
						0, // Menu Left - The Left position of the menu in pixels
						, // Menu Width - Menus width in pixels
						0, // Menu Border Width 
						, // Screen Position - here you can use "center;left;right;middle;top;bottom" or a combination of "center:middle"
						style2, // Properties Array - this is set higher up, as above
						1, // Always Visible - allows the menu item to be visible at all time (1=on/0=off)
						"left", // Alignment - sets the menu elements text alignment, values valid here are: left, right or center
						, // Filter - Text variable for setting transitional effects on menu activation - see above for more info
						1, // Follow Scrolling - Tells the menu item to follow the user down the screen (visible at all times) (1=on/0=off)
						1, // Horizontal Menu - Tells the menu to become horizontal instead of top to bottom style (1=on/0=off)
						1, // Keep Alive - Keeps the menu visible until the user moves over another menu or clicks elsewhere on the page (1=on/0=off)
						, // Position of TOP sub image left:center:right
						1, // Set the Overall Width of Horizontal Menu to 100% and height to the specified amount (Leave blank to disable)
						, // Right To Left - Used in Hebrew for example. (1=on/0=off)
						0, // Open the Menus OnClick - leave blank for OnMouseover (1=on/0=off)
						, // ID of the div you want to hide on MouseOver (useful for hiding form elements)
						, // Reserved for future use
						, // Reserved for future use
						, // Reserved for future use
						1,
						"&nbsp;&nbsp;<img src=../imagens/principal.gif width=14 height=12 align=top border=0>&nbsp;&nbsp;Principal&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;","show-menu=principal"," ","Principal",1,
						"&nbsp;&nbsp;<img src=../imagens/report.gif width=10 height=11 align=top border=0>&nbsp;&nbsp;Relatórios&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;","show-menu=relatorio", " ", ,1,
						"&nbsp;&nbsp;<img src=../imagens/sistema.gif width=11 height=10 align=top border=0>&nbsp;&nbsp;Sistema&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;","show-menu=acompanhamento"," ", ,1,
						"<div nowrap width=\"40\" onclick=\"self.print();\" onMouseOver=\"this.style.color='#FF3333';this.style.cursor='hand';\" onMouseOut=\"this.style.color='#FFF000';\" class=\"f_tab_ajuda\">  <img src=../imagens/print.gif width=16 height=10 align=top alt=\"Imprime a Tela\">  <font color=\"#FFF000\"><b>Imprimir</b></font></div>","", " ", ,1,
						"&nbsp;&nbsp;<img src=../imagens/ajuda.gif width=12 height=12 align=top border=0>&nbsp;&nbsp;Ajuda&nbsp;&nbsp;"," ", " ", ,1,
						"&nbsp;&nbsp;<img src=../imagens/sair.gif width=12 height=12 align=top border=0>&nbsp;&nbsp;Sair&nbsp;&nbsp;","logout.php", " ", ,1]);
						
  addmenu(menu=["principal",,,150,1,"",style1,,,effect,0,0,,1,,0,,,,,1,,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Programas","show-menu=cadprograma","","Sistema",1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Usuários","show-menu=cadusuario","", ,1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Referências","show-menu=cadreferencia", "", ,1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Menus","show-menu=cadmenu", "", ,1,						
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Tabelas de Apoio","show-menu=cadtabapoio", "", ,1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Perfil","show-menu=cadperfil", "", ,1]);

			addmenu(menu=["cadreferencia",,,150,1,"",style1,,,effect,0,0,,1,,0,,,,,1,,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Administrar Referências","simec.php?modulo=principal/referencia/cadreferencia","#","",1]);

		
  
			addmenu(menu=["cadprograma",,,150,1,"",style1,,,effect,0,0,,1,,0,,,,,1,,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Listar Programas","simec.php?modulo=principal/programa/listaprograma","# ","",1]);
						
 addmenu(menu=["cadusuario",,,150,1,"",style1,,,effect,0,0,,1,,0,,,,,1,,
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Consultar","simec.php?modulo=principal/usuario/consusuario","# ","",1,
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Incluir","simec.php?modulo=principal/usuario/cadusuario&act='I'","# ","",1,
                        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Alterar","simec.php?modulo=principal/usuario/cadusuario&act='A'","# ","",1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Administrar Cadastro","simec.php?modulo=principal/usuario/cadusuario","# ","",1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Alterar Senha","simec.php?modulo=principal/usuario/altsenha","# ","",1]);
 
    addmenu(menu=["cadmenu",,,150,1,"",style1,,,effect,0,0,,1,,0,,,,,1,,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Administrar Menu","simec.php?modulo=principal/menu/menu","# ","",1]);
   addmenu(menu=["cadtabapoio",,,150,1,"",style1,,,effect,0,0,,1,,0,,,,,1,,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Orgao","simec.php?modulo=principal/tabapoio/listaorgao","# ","",1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Orgao SIORG","simec.php?modulo=principal/tabapoio/listaorgaosiorg","# ","",1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Unidade de Medida","simec.php?modulo=principal/tabapoio/listaunidmed","# ","",1,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Unidades","simec.php?modulo=principal/tabapoio/listaunidade","# ","",1]);						
							addmenu(menu=["cadacao",,,150,1,"",style1,,,effect,0,0,,1,,0,,,,,1,,
						"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Listar Ações","simec.php?modulo=principal/acao/listaacao","# ","",1]);							


	addmenu(menu=["cadperfil",,,150,1,"",style1,,,effect,0,0,,1,,0,,,,,1,, 
                "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Cadastro de Perfis","simec.php?modulo=principal/perfil/cadperfil", "", ,1,
				"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Associar Perfil ao Menu","simec.php?modulo=principal/perfil/associa_perfil","#","",1]);



						

    
  dumpmenus();

</script>
<script language="Javascript" src="includes/mmenu.js"></script>
