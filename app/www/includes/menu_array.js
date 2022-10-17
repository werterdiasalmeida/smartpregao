menunum=0;menus=new Array();_d=document;function
addmenu(){menunum++;menus[menunum]=menu;}function dumpmenus(){mt="<script language=javascript>";for(a=1;a<menus.length;a++){mt+=" menu"+a+"=menus["+a+"];"}mt+="<\/script>";_d.write(mt)}


////////////////////////////////////
// Editable properties START here //
////////////////////////////////////


timegap=100					// The time delay for menus to remain visible
followspeed=5				// Follow Scrolling speed
followrate=40				// Follow Scrolling Rate
suboffset_top=3;			// Sub menu offset Top position 
suboffset_left=5;			// Sub menu offset Left position


style1=[					// Menu Properties Array
"cc0000",					// Off Font Color
"cccccc",					// Off Back Color
"ffffff",					// On Font Color
"cc0000",					// On Back Color
"666666",					// Border Color
10,							// Font Size
"normal",					// Font Style
"bold",					// Font Weight
"Verdana",					// Font
3,							// Padding
"http://www.lucent.com/javascript/arrow.gif",	// SubMenu Image
0,						// 3D Border & Separator
"ffffff",					// 3D High Color
"000000"					// 3D Low Color
]

style2=[					// blank
"ffffff",					// Off Font Color
"ffffff",					// Off Back Color
"ffffff",					// On Font Color
"ffffff",					// On Back Color
"ffffff",					// Border Color
1,							// Font Size
"normal",					// Font Style
"bold",					// Font Weight
"Verdana",					// Font
0,							// Padding
"http://www.lucent.com/javascript/arrow.gif",			// Sub Menu Image
0,						// 3D Border & Separator
"ffffff",					// 3D High Color
"ffffff"					// 3D Low Color
]

addmenu(menu=["solutions",23,47,200,1,"",style1,,,,,,,,,,,'select1',,,,
,"for Service Providers","http://www.lucent.com/sp/","/","Service Providers",1
,"for Enterprises","http://www.lucent.com/enterprise/","/","Enterprises",1
,"for Government","http://www.lucent.com/gov/","/","Government",1
,"for Cable Operators","http://www.lucent.com/cable/","/","Cable Operators",1
,"Accelerate&#153; Next-Generation Communications Solutions","http://www.lucent.com/solutions/convergence.html","/","Accelerate&#153; Next-Generation Communications Solutions",1
,"Document library","http://www.lucent.com/knowledge/resourcelib/0,,,00.html","/","Document Library",1
,"Catalog index","http://www.lucent.com/products/product_index/0,,IXID+A-LOCL+1,00.html","/","Resource Library",1
,"How to buy","http://www.lucent.com/products/howtobuy.html",,"How to Buy",1
])


addmenu(menu=["support",23,167,150,1,"",style1,,,,,,,,,,,,,,,                   
,"Training","https://training.lucent.com/","/","Training",1
,"Lucent Service Offerings","http://www.lucent.com/solutions/lws.html","/","Lucent Service Offerings",1
])                                                                              

addmenu(menu=["bl",23,265,150,1,"",style1,,,,,,,,,,,,,,,
,"About Bell Labs","show-menu=blabout","http://www.bell-labs.com/about","About Bell Labs",1
,"Research Areas","show-menu=blresearch","http://www.bell-labs.com/research","Research Areas",1
,"Employment","http://www.bell-labs.com/employment",,"Employment",1
,"Software Downloads","http://www.bell-labs.com/blsoftware.html",,"Software Downloads",1
,"FAQs","http://www.bell-labs.com/faq.html","/","FAQ",1
])

addmenu(menu=["about",23,325,150,1,"",style1,,,,,,,,,,,,,,,
,"News &amp; Events","show-menu=news","http://www.lucent.com/news_events","News and Events",1
,"Investor Relations","show-menu=investor","http://www.lucent.com/investor/","Investor Relations",1
,"Industry Analyst Relations","http://www.lucent.com/industryanalyst/","http://www.lucent.com/industryanalyst/","Industry Analyst Relations",1
,"Company Information","show-menu=corpinfo","http://www.lucent.com/corpinfo/","Company Information",1
,"Social Responsibility","show-menu=social","http://www.lucent.com/social/","Social Responsibility",1
,"Work@Lucent","show-menu=work","http://www.lucent.com/work/work.html","Work@Lucent",1
,"Alumni Center","http://www.lucent.com/inside/alumni",,"Alumni Center",1
,"Solutions Partners","http://www.lucent.com/map/solutions/",,"Solutions Partners",1
,"Market Advantage Programs","show-menu=mktadv","http://www.lucent.com/map/","Market Advantage Programs",1
])

addmenu(menu=["blank",23,0,15,1,"",style2,,,,,,,,,,,,,,,
," ","/",,,1
])


addmenu(menu=["enabled","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"DSL Services for Enterprises","http://www.lucent.com/solutions/dsl.html",,"DSL",1		
,"Enhanced Frame Relay and ATM Services for Enterprises","http://www.lucent.com/solutions/fr.html",,"frame relay",1		
,"Ethernet over SONET (EoS) Services for Enterprises","http://www.lucent.com/solutions/ethernet.html",,"ethernet",1		
,"IP Centrex Services for Enterprises","http://www.lucent.com/solutions/ip_centrex.html",,"IP Centrex",1	
,"Managed Contact Center for Enterprises","http://www.lucent.com/solutions/contact.html",,"contact",1		
,"Managed Wavelength Services for Enterprises","http://www.lucent.com/solutions/wavelength.html",,"wavelength",1		
])
addmenu(menu=["news","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"Press Releases","http://www.lucent.com/press",,"",1		
,"Features","http://www.lucent.com/news_events/archive.html",,"",1		
,"Events &amp; Speakers","http://www.lucent.com/events/archive/0,2284,inDocTypeId+7-inPageNumber+1-inByLocation+0,00.html",,"",1
,"Webcasts","http://www.lucent.com/news_events/webcast",,"",1		
,"Photo Gallery","http://www.lucent.com/news_events/gallery/executives.html",,"",1		
,"Press Contacts","http://www.lucent.com/intl/prc.html",,"",1		
,"Corrections and Clarifications","http://www.lucent.com/news_events/correct_clarify.html",,"",1		
])
addmenu(menu=["investor","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"Company Information","http://www.lucent.com/corpinfo",,"",1		
,"Financial Information","http://www.lucent.com/investor/financialreports.html",,"",1		
,"SEC Filings","http://www.lucent.com/investor/secfiling.html",,"",1		
,"Corporate Governance","http://www.lucent.com/investor/governance.html",,"",1		
,"Executive Presentations","http://www.lucent.com/investor/presentation.html",,"",1		
,"Events Calendar","http://www.lucent.com/investor/calendar.html",,"",1		
,"Stock Charts","http://www.edgar-online.com/brand/lu/chart.asp",,"",1		
,"Tax Basis Worksheet","http://www.lucent.com/investor/taxinfo.html",,"",1		
,"FAQs","http://www.lucent.com/investor/faq.html",,"",1		
])
addmenu(menu=["corpinfo","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"Network Solutions Group","http://www.lucent.com/corpinfo/nsg.html",,"",1		
,"Lucent Worldwide Services","http://www.lucent.com/corpinfo/ws.html",,"",1		
,"Bell Labs","http://www.lucent.com/corpinfo/bl.html",,"",1		
,"Awards and Achievements","http://www.lucent.com/corpinfo/awardachieve.html",,"",1		
,"History","http://www.lucent.com/corpinfo/history.html",,"",1		
,"Our Leaders","http://www.lucent.com/corpinfo/leaders.html",,"",1		
])
addmenu(menu=["social","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"Business Ethics and Compliance","http://www.lucent.com/social/bec.html",,"",1		
,"Corporate Governance","http://www.lucent.com/social/governance.html",,"",1		
,"Diversity","http://www.lucent.com/social/diversity.html",,"",1		
,"EH&S","http://www.lucent.com/social/ehs.html",,"",1		
,"Philanthropy","http://www.lucent.com/social/phil.html",,"",1		
,"Valuing People","http://www.lucent.com/social/valuing.html",,"",1		
])
addmenu(menu=["work","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"Search Jobs","http://64.157.137.11/cgi-bin/parse-file?LOCATION=&TEMPLATE=/htdocs/job-search-interest-page.html",,"",1		
,"My Profile","http://64.157.137.11/cgi-bin/parse-file?TEMPLATE=/htdocs/create.html",,"",1		
,"Life@Lucent","http://www.lucent.com/work/lucentlife.html",,"",1		
,"College Recruiting","http://www.lucent.com/work/collegerecruitment.html",,"",1		
])

addmenu(menu=["mktadv","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"for Customers","http://www.lucent.com/map/customers",,"",1		
,"for Sales Business Partners","http://www.lucent.com/map/salesbp",,"",1		
,"for Application Partners","http://www.lucent.com/map/applications/",,"",1		
])

addmenu(menu=["blabout","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"History","show-menu=blhistory","http://www.bell-labs.com/","",1	
,"Awards","http://www.bell-labs.com/about/awards.html",,"",1	
,"People","http://www.bell-labs.com/about/people.html",,"",1		
,"News &amp; Features","http://www.bell-labs.com/news",,"",1		
])

addmenu(menu=["blresearch","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"Physical Sciences","http://www.bell-labs.com/research/phys.html",,"",1		
,"Computer Science &amp; Software","http://www.bell-labs.com/research/cs.html",,"",1
,"Mathematical Science","http://www.bell-labs.com/research/math.html",,"",1		
,"Network Applications","http://www.bell-labs.com/research/netapps.html",,"",1		
,"Optical Networking","http://www.bell-labs.com/research/optnet.html",,"",1
,"Wireless Networking","http://www.bell-labs.com/research/wireless.html",,"",1						
])

addmenu(menu=["blhistory","offset=3",,150,1,"",style1,,,,,,,,,,,,,,,
,"Timeline","http://www.bell-labs.com/about/history/timeline.html",,"",1		
,"Foundational Research","http://www.bell-labs.com/about/history/foundational.html",,"",1		
,"Bell Labs Presidents","http://www.bell-labs.com/about/history/presidents",,"",1		
])



//////////////////////////////////
// Editable properties END here //
//////////////////////////////////
dumpmenus() // This must be the last line in this file
