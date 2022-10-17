<?php
$p = getParam();

$paramAba = setParam(array('id' => $p['id'], 'pessoa_id' => $p['pessoa_id'], 'profissional_id' => $p['profissional_id']));
$_REQUEST['p'] = setParam(array(
'id' => $p['pessoa_id'],
'estabelecimento_id' => $p['id'],
'anexo_id' => $p['anexo_id']
), false);


UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);

include_once APPRAIZ . '/adm/modulos/principal/profissional/documento.inc';