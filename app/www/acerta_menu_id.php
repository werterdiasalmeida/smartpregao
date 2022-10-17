<?
include "config.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/classes_simec.inc";
$db = new cls_banco();
//;;;;; atenção para apontar para o banco certo!!!!!!!!!!
$sql="select mnuid,mnucod, mnucodpai from seguranca.menu order by mnucod";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
print $nlinhas;
$_SESSION['usucpf']='00000000191';
$_SESSION['usucpforigem']='00000000191';

$mnucod2='';
$j=1;
for ($i=0;$i<=$nlinhas;$i++)
{
    $res = $db->carrega_registro($RS,$i);
    // a linha abaixo transforma em variáveis todos os campos do array
    if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
    if (trim($mnucod2) != trim($mnucod))
{
    	print $i."diferente ----- $mnucod ---- ".$j++."<br>";
    	$mnucod2=$mnucod;
    	if ($mnucodpai)
    	{
    	$sql="select mnuid as mnuid2 from seguranca.menu where mnucod=$mnucodpai";
        $RS2 = $db->record_set($sql);
        $res = $db->carrega_registro($RS2,0);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
            	$sql="update seguranca.menu set mnuidpai=$mnuid2 where mnuid=$mnuid ";
            	print $sql;
        $saida=$db->executar($sql);
        
    	}   	
    }
}
$db->commit();
?>