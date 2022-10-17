<?php 
$diretorio = APPRAIZ."www/includes/layout";
if(is_dir($diretorio)){
	
	if ($handle = opendir($diretorio)) {
	
	   while (false !== ($file = readdir($handle))) {
	
	      if ($file != "." && $file != ".." && $file != ".svn") {
	         if (is_dir("$diretorio/$file")) {
	            echo "<option ".($theme == $file ? "selected='selected'" : "") ." value=\"$file\" >".ucwords(str_replace("_"," ",$file))."</option>";
	         }
	      }
	   }
	   closedir($handle);
	}
	
}else{
	echo "<option value=\"verde\" >Verde</option>";
}

?>