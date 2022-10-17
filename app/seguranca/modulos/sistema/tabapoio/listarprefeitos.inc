<?php
//
// $Id$
//

//echo "<pre>\n" , print_r(get_included_files(), true);
//die();

if (array_key_exists('desvincularPrefeito', $_REQUEST)) {
    include_once APPRAIZ . 'includes/ActiveRecord/classes/Entidade.php';

    try {
        $entidade = new Entidade($_REQUEST['entid']);

        $entidade->BeginTransaction();

        $entidade->entidassociado = 'null';

        $entidade->save();
        $entidade->Commit();
        die('OK');
    } catch (Exception $e) {
        $entidade->Rollback();
        die('ERRO');
    }
}

include_once APPRAIZ . 'includes/cabecalho.inc';


$db->cria_aba($abacod_tela, $url, $parametros);
$titulo_modulo = 'Buscar Prefeitos';
monta_titulo($titulo_modulo, '');




?>

  <script type="text/javascript" src="../includes/prototype.js"></script>
  <script type="text/javascript">
    //<![CDATA[
    var frmValido = false;
    function validarFrmBusca()
    {
        var inputs = $('frmBusca').getElements();

        for (var i = 0, length = inputs.length; i < length; i++) {
            if (inputs[i].type != 'text' && inputs[i].tagName.toUpperCase() != 'SELECT')
                continue;

            if (trim(inputs[i].value) != '' && inputs[i].value != 'null')
                frmValido = true
        }

        if (!frmValido)
            alert('Por favor, preencha pelo menos um dos campos de busca.');

        return frmValido;
    }

    function editarPrefeito(entid)
    {
        return windowOpen('seguranca.php?modulo=sistema/tabapoio/prefeito&acao=A&entid=' + entid,
                          'edicao',
                          'height=750,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function editarPrefeitura(entid)
    {
        return windowOpen('seguranca.php?modulo=sistema/tabapoio/cadastrarprefeitura&acao=A&entid=' + entid,
                          'edicao',
                          'height=750,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function desvincularPrefeito(callee, entid, entnome)
    {
        if (!confirm('Deseja realmente desvincular este prefeito da prefeitura atual?\n  - Atualmente vinculado a ' + entnome))
            return false;

        new Ajax.Request(window.location.href,
        {
            parameters: 'desvincularPrefeito=1&entid=' + entid,
            method: 'post',
            onComplete: function(res)
            {
                callee.parentNode.parentNode.parentNode.style.display = 'none';
            },
            onFailure: function()
            {
                alert('Não foi possível excluir o registro selecionado.');
            }
        });
    }

    function carregarMunicipios(estuf)
    {
        return new Ajax.Request('/geral/dne.php?opt=municipio&regcod=' + estuf,
                                {
                                    method: 'post',
                                    onComplete: function (res)
                                    {
                                        while ($('muncod').options[1]) {
                                            $('muncod').options[1] = null;
                                        }

                                        eval(res.responseText);

                                        for (var i = 1; i < listaMunicipios[estuf].length; i++) {
                                            $('muncod').options[i] = new Option(listaMunicipios[estuf][i][1],
                                                                                listaMunicipios[estuf][i][0],
                                                                                false,
                                                                                false);
                                        }
                                    }
                                });
    }
    // ]]>
  </script>
<div align="center">

<form method="post" name="formulario" id="frmBusca" onsubmit="return validarFrmBusca();">
  <center>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align="right" class="SubTituloDireita">CPF:</td>
        <td><?php echo campo_texto('entnumcpfcnpj', 'N', 'S', 'CPF', '20 ', '14', '###.###.###/##', '', 'left', '', 0, 'id="entnumcpfcnpj" onblur="MouseBlur(this);"'); ?></td>
      </tr>

      <tr>
        <td align="right" class="SubTituloDireita">Nome (ou parte do nome do prefeito):</td>
        <td><?php echo campo_texto('entnome', 'N', 'S', 'CPF', '50', '150', '', '', 'left', '', 0, 'id="entnome" onblur="MouseBlur(this);"'); ?></td>
      </tr>

      <tr>
        <td align="right" class="SubTituloDireita">UF:</td>
        <td><?php echo $db->monta_combo('estuf',
                                        'select estuf as codigo, estdescricao as descricao from territorios.estado order by estuf asc',
                                        'S',
                                        'UF',
                                        'carregarMunicipios',
                                        '',
                                        '',
                                        '',
                                        '',
                                        'estuf'); ?>
        </td>
      </tr>

      <tr>
        <td align="right" class="SubTituloDireita">Município:</td>
        <td><select name="muncod" class="CampoEstilo" id="muncod"><option value="null">Selecione a UF</option></select></td>
      </tr>

      <tr>
        <td colspan="2" align="center">
          <input type="hidden" name="entbusca" value="1" />
          <input type="submit" value="Buscar" />
          <input type="button" value="Inclcuir" onclick="return editarPrefeito('');" />
        </td>
      </tr>

    </table>
  </center>
</form>

<center>
<?php

if (array_key_exists('entbusca', $_REQUEST) || $_REQUEST['entbusca'] == '1') {
    $sql = '
    SELECT
        \'<center style="margin:0;padding:0">
          <img src="/imagens/alterar.gif" title="Clique aqui para vizualizar/editar os dados do prefeito." onclick="editarPrefeito(\'||ent.entid||\')" />
          <img src="/imagens/excluir.gif" title="Clique aqui para desvincular o prefeito da prefeitura atual." onclick="desvincularPrefeito(this, \'||ent.entid||\', \\\'\' || pre.entnome || \'\\\')" /></center>\' as entid,
        ent.entnumcpfcnpj as entnumcpfcnpj,
        ent.entnome as prefeito,
        \'<a title="Clique para editar os dados da prefeitura" href="javascript:void(0);" onclick="editarPrefeitura(\' || pre.entid || \');">\' || pre.entnome || \'</a>\' as prefeitura,
        mun.mundescricao as municipio,
        entd.estuf as estuf
    FROM
        entidade.entidade ent
    INNER JOIN 
    	entidade.funcaoentidade fe1 ON fe1.entid = ent.entid
    INNER JOIN 
	entidade.funentassoc fea ON fea.fueid = fe1.fueid
    INNER JOIN
        entidade.entidade pre on fea.entid = pre.entid
    INNER JOIN 
    	entidade.funcaoentidade fe2 ON fe2.entid = pre.entid
    INNER JOIN
        entidade.endereco entd on entd.entid = pre.entid
    INNER JOIN
        territorios.municipio mun on entd.muncod = mun.muncod
    WHERE
       fe2.funid = 1 AND 
       fe1.funid = 2';

    if (trim($_REQUEST['entnumcpfcnpj']) != '') {
        $sql .= ' AND ent.entnumcpfcnpj = \'' . str_replace(array('.','-','/'), '', $_REQUEST['entnumcpfcnpj']) .'\'';
    }

    if (trim($_REQUEST['entnome']) != '') {
        $sql .= ' AND ent.entnome ILIKE \'%' . str_replace(" ", "%", $_REQUEST['entnome']) . '%\'';
    }

    if (trim($_REQUEST['estuf']) != '') {
        $sql .= ' AND entd.estuf = \'' . $_REQUEST['estuf'] . '\'';
    }

    if (trim($_REQUEST['muncod']) != '' && $_REQUEST['muncod'] != 'null') {
        $sql .= ' AND mun.muncod = \'' . $_REQUEST['muncod'] . '\'';
    }

    $db->monta_lista($sql, array('Ação', 'CPF', 'Nome', 'Prefeitura', 'Município', 'UF'), 10, 50, 'N', '', '');
}

echo '</center></div>';

