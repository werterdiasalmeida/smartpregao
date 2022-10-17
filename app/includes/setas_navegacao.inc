<table>
<tr>
<td>
<img src="../imagens/primeiro.gif" title="Ir para o primeiro" width="11" height="16" onClick="VerRegistro(0)">
<img src="../imagens/anterior.gif" title="Ir para o anterior" width="11" height="16" onClick="VerRegistro(-1)">
<? 
if ($nlinhas > 10) print '<img src="../imagens/anterior10.gif" title="Ir para 10 atr&aacute;s" width="16" height="16" onClick="VerRegistro(-10)">&nbsp;';
if ($nlinhas > 50) print '<img src="../imagens/anterior50.gif" title="Ir para 50 atr&aacute;s" width="22" height="16" onClick="VerRegistro(-50)">&nbsp;';
if ($nlinhas > 100) print '<img src="../imagens/anterior100.gif" title="Ir para 100 atr&aacute;s" width="27" height="16" onClick="VerRegistro(-100)">&nbsp;';
if ($nlinhas > 500) print '<img src="../imagens/anterior500.gif" title="Ir para 500 atr&aacute;s" width="32" height="16" onClick="VerRegistro(-500)">&nbsp;';
if ($nlinhas > 500) print '<img src="../imagens/proximo500.gif" title="Ir para 500 adiante" width="32" height="16" onClick="VerRegistro(+500)">&nbsp;';
if ($nlinhas > 100) print '<img src="../imagens/proximo100.gif" title="Ir para 100 adiante" width="27" height="16" onClick="VerRegistro(+100)">&nbsp;';
if ($nlinhas > 50) print '<img src="../imagens/proximo50.gif" title="Ir para 50 adiante" width="22" height="16" onClick="VerRegistro(+50)">&nbsp;';
if ($nlinhas > 10) print '<img src="../imagens/proximo10.gif" title="Ir para 10 adiante" width="16" height="16" onClick="VerRegistro(+10)">&nbsp;';
?>
<img src="../imagens/proximo.gif" title="Ir para o pr&oacute;ximo" width="11" height="16" onClick="VerRegistro(+1)">
<img src="../imagens/ultimo.gif" title="Ir para o &uacute;ltimo" width="11" height="16" onClick="VerRegistro(-2)"></td>
</tr>
</table>