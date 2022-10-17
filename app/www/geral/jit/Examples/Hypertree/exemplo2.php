<?
include_once "config.inc";

$email_sistema = 'cristiano.cabral@mec.gov.br; adonias.malosso@mec.gov.br';

// configuração de conexão com o servidor de banco de dados

//$nome_bd     = $_SESSION['sisbaselogin'] ? $_SESSION['sisbaselogin'] : $nome_bd;

/**
 * Solução paleativa para o problema de navegação entre sistemas. Esta
 * rotina tenta adivinhar qual módulo o usuário teve a intenção de acessar.
 * A decisão é tomada a partir da url solicitada pelo usuário no qual ele
 * indica o diretório e a ação pretendida.
 */
preg_match( '/\/([a-zA-Z]*)\//', $_SERVER['REQUEST_URI'], $sisdiretorio );
$sisdiretorio = $sisdiretorio[1];

preg_match( '/\/([a-zA-Z]*)\.php/', $_SERVER['REQUEST_URI'], $sisarquivo );
$sisarquivo = $sisarquivo[1];

define( 'SISRAIZ', APPRAIZ . $_SESSION['sisdiretorio'] . '/' );
    

include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
$_SESSION['usucpf'] = '00000000191';

if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


define("MAX_NIVEL", 2);
$id=0;

function viz($muncod, $nivel=1, $muncodpai = null) {
	global $db, $id;

	if($muncodpai) {
		$cl = "AND mun.muncod!='".$muncodpai."'";
	}
	
	$sql = "SELECT mun.muncod, mun.mundescricao FROM territorios.municipiosvizinhos mv 
			INNER JOIN territorios.municipio mun ON mun.muncod = mv.muncodvizinho 
			WHERE mv.muncod='".$muncod."' ".$cl;
	
	$vizinhos = $db->carregar($sql);
	
	for($i=0;$i<=$nivel;$i++) {
		$espacamento .= chr(9);
	}
	
	
	if($vizinhos[0]) {
		foreach($vizinhos as $vv) {
			$json_child[] = chr(10).$espacamento.'{"id": "'.($id++).'_'.$vv['muncod'].'", "name": "'.$vv['mundescricao'].'", "data": {"band": "Jerome Dillon","relation": "member of band"}, "children": ['.(($nivel<MAX_NIVEL)?viz($vv['muncod'],($nivel+1), $muncod):"").']}';

		}
		return implode(",",$json_child);
	}
}

function carregarJsonMunicipio($dados) {
	global $db, $id;
	if($dados['idinicio']) $id=$dados['idinicio']; 
	
	$sql = "SELECT * FROM territorios.municipio WHERE muncod='".$dados['muncod']."'";
	$centro = $db->pegaLinha($sql);
	$json = 'var json = {"id":     "'.($id++).'_'.$centro['muncod'].'","name":   "'.$centro['mundescricao'].'", "children": ['.viz($centro['muncod']).'], "data":   ""};';
	echo $json;
}


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Hypertree - Tree Animation</title>

<!-- CSS Files -->
<link type="text/css" href="../css/base.css" rel="stylesheet" />
<link type="text/css" href="../css/Hypertree.css" rel="stylesheet" />

<!--[if IE]><script language="javascript" type="text/javascript" src="../../Extras/excanvas.js"></script><![endif]-->

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<!-- JIT Library File -->
<script language="javascript" type="text/javascript" src="../../jit.js"></script>

<!-- Example File -->
<script language="javascript" type="text/javascript">
var labelType, useGradients, nativeTextSupport, animate;

var carregar = false;

(function() {
  var ua = navigator.userAgent,
      iStuff = ua.match(/iPhone/i) || ua.match(/iPad/i),
      typeOfCanvas = typeof HTMLCanvasElement,
      nativeCanvasSupport = (typeOfCanvas == 'object' || typeOfCanvas == 'function'),
      textSupport = nativeCanvasSupport 
        && (typeof document.createElement('canvas').getContext('2d').fillText == 'function');
  //I'm setting this based on the fact that ExCanvas provides text support for IE
  //and that as of today iPhone/iPad current text support is lame
  labelType = (!nativeCanvasSupport || (textSupport && !iStuff))? 'Native' : 'HTML';
  nativeTextSupport = labelType == 'Native';
  useGradients = nativeCanvasSupport;
  animate = !(iStuff || !nativeCanvasSupport);
})();

var Log = {
  elem: false,
  write: function(text){
    if (!this.elem) 
      this.elem = document.getElementById('log');
    this.elem.innerHTML = text;
    this.elem.style.left = (500 - this.elem.offsetWidth / 2) + 'px';
  }
};

function carregarJsonMunicipio(muncod,id) {
	var jsm;
	
	$.ajax({
   		type: "POST",
   		url: "http://<? echo $_SERVER['HTTP_HOST']; ?>/geral/jit/Examples/Hypertree/exemplo1.php",
   		data: "requisicao=carregarJsonMunicipio&muncod="+muncod+"&idinicio="+id,
   		async: false,
   		success: function(msg){
   			jsm = msg;
   		}
 		});
 	
 	return jsm;
}

function init(){
	/* Carregando a variavel "json" utilizada */
    var json_ = carregarJsonMunicipio('5300108','0');
    eval(json_);

    var infovis = document.getElementById('infovis');
    var w = infovis.offsetWidth - 50, h = infovis.offsetHeight - 50;
    
    function recarregarArvore(no) {
		var arr = no.split('_');
		var json_ = carregarJsonMunicipio(arr[1],arr[0]);
		//alert(json_);
		eval(json_);
		//load JSON data.
		ht.loadJSON(json);
 
	    ht.op.morph(json, {
	            type: 'fade:con',
		 	  	duration: 1000,  
		   	  	fps: 35,  
	            hideLabels: true,
	            onComplete: function(){ Log.write("morph complete!"); }
	   });

	   
    };
    
    
    //init Hypertree
    var ht = new $jit.Hypertree({
      //id of the visualization container
      injectInto: 'infovis',
      //canvas width and height
      width: w,
      height: h,
      //Change node and edge styles such as
      //color, width and dimensions.
      Node: {
          dim: 8,
          color: "#f00"
      },
      Edge: {
          lineWidth: 1,
          color: "#088"
      },
      onBeforeCompute: function(node){
          Log.write("centering");
      },
      //Attach event handlers and add text to the
      //labels. This method is only triggered on label
      //creation
      onCreateLabel: function(domElement, node){
          domElement.innerHTML = node.name;
          $jit.util.addEvent(domElement, 'click', function () {
          	  ht.onClick(node.id);
          });
      },
      //Change node styles when labels are placed
      //or moved.
      onPlaceLabel: function(domElement, node){
          var style = domElement.style;
          style.display = '';
          style.cursor = 'pointer';
          if (node._depth <= 1) {
              style.fontSize = "0.8em";
              style.color = "#ddd";

          } else if(node._depth == 2){
              style.fontSize = "0.7em";
              style.color = "#555";

          } else {
              style.display = 'none';
          }

          var left = parseInt(style.left);
          var w = domElement.offsetWidth;
          style.left = (left - w / 2) + 'px';
      },
      onAfterCompute: function(){
          Log.write("done");
          //Build the right column relations list.
          //This is done by collecting the information (stored in the data property) 
          //for all the nodes adjacent to the centered node.
          var node = ht.graph.getClosestNodeToOrigin("current");
      	  if(carregar) {
      	  	recarregarArvore(node.id);
			carregar = false;
      	  } else {
      	  	carregar = true;
      	  }
          var html = "<h4>" + node.name + "</h4><b>Connections:</b>";
          html += "<ul>";
          node.eachAdjacency(function(adj){
              var child = adj.nodeTo;
              if (child.data) {
                  var rel = (child.data.band == node.name) ? child.data.relation : node.data.relation;
                  html += "<li>" + child.name + " " + "<div class=\"relation\">(relation: " + rel + ")</div></li>";
              }
          });
          html += "</ul>";
          $jit.id('inner-details').innerHTML = html;
      }
    });
    
    //load JSON data.
    ht.loadJSON(json);
    //compute positions and plot.
    ht.refresh();
    //end
    ht.controller.onAfterCompute();
}


</script>
</head>

<body onload="init();">
<div id="container">

<div id="left-container">



        <div class="text">
        <h4>
Tree Animation    
        </h4> 

            A static JSON Tree structure is used as input for this animation.<br /><br />
            Clicking on a node should move the tree and center that node.<br /><br />
            The centered node's children are displayed in a relations list in the right column.
            
        </div>

        <div id="id-list"></div>


<div style="text-align:center;"><a href="example1.js">See the Example Code</a></div>            
</div>

<div id="center-container">
    <div id="infovis"></div>    
</div>

<div id="right-container">

<div id="inner-details"></div>

</div>

<div id="log"></div>
</div>
</body>
</html>
