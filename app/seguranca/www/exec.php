<?php
function my_exec($cmd, $input=''){
	$proc = proc_open($cmd, array(0=>array('pipe', 'r'), 1=>array('pipe', 'w'), 2=>array('pipe', 'w')), $pipes);
	fwrite($pipes[0], $input);fclose($pipes[0]);
	$stdout=stream_get_contents($pipes[1]);fclose($pipes[1]);
	$stderr=stream_get_contents($pipes[2]);fclose($pipes[2]);
	$rtn=proc_close($proc);
	return array('stdout'=>$stdout,
                       'stderr'=>$stderr,
                       'return'=>$rtn
                      );
         }
echo "<pre>";
//var_export(my_exec('set PGPASSWORD=root', 'C:\log.txt'));
//var_export(my_exec('pg_dump -s -n emi -h mec-32-746 -U root simec', 'C:\log.txt')); 
var_export(my_exec('ping mec-32-746', 'C:\log.txt'));
