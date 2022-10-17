<?php
//
// $Id$
//



include_once APPRAIZ . "includes/entidades.php";

$entidade = new Entidade();

if (array_key_exists('buscarPrefeitura', $_REQUEST) &&
    array_key_exists('entnumcpfcnpj', $_REQUEST)) {
    $entidade = Entidade::carregarEntidadePorCnpjCpf($_REQUEST['entnumcpfcnpj']);

    echo 'var entidade = ' , $entidade->toJson() , ';';
    die();
}

if (array_key_exists('buscarPrefeito', $_REQUEST) &&
    array_key_exists('entnumcpfcnpj', $_REQUEST)) {
    $entidade = Entidade::carregarEntidadePorCnpjCpf($_REQUEST['entnumcpfcnpj']);

    echo 'var entidade = ' , $entidade->toJson() , ';';
    die();
}

if (array_key_exists('entnumcpfcnpj', $_REQUEST)) {
    $entidade = new Entidade();

    if (trim($_REQUEST['entid']) != '')
        $entidade->carregar($_REQUEST['entid']);

    foreach ($_REQUEST as $campo => $valor) {
        if (!is_array($campo))
            $entidade->$campo = $valor == '' ? 'null' : $valor;

        $entidade->funid = 1;
    }

    try {
        $entidade->BeginTransaction();

        $entidade->entnumcpfcnpj = str_replace(array('.', '-', '/'), '', $entidade->entnumcpfcnpj);
        $entidade->save();

        if ($_REQUEST['prefid'] != '') {
            $prefeito = new Entidade($_REQUEST['prefid']);
            $prefeito->entidassociado = $entidade->getPrimaryKey();
            $prefeito->funid          = 2;
            $prefeito->save();
        }

        $entidade->carregarEnderecos();
        $endereco = $entidade->enderecos[0];

        foreach ($_REQUEST['endereco'] as $campo => $valor)
            $endereco->$campo = $valor;

        $endereco->endcep = str_replace(array('.', '-'), '', $endereco->endcep);

        $endereco->save();

        include_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeEndereco.php";
        EntidadeEndereco::adicionar($entidade, $endereco);

        $entidade->Commit();
    } catch (Exception $e) {
        echo $e;
    }

    echo '<script type="text/javascript">window.close();</script>';
}


?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
      this._closeWindows = false;
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
    <div>
      <h3 class="TituloTela" style="color:#000000; text-align: center"><?php echo $titulo_modulo; ?></h3>

<?php


if ($_REQUEST['entid'] != '')
    $entidade->carregar((integer) $_REQUEST['entid']);

//echo formEntidade($entidade, $_SERVER['REQUEST_URI'], PESSOA_JURIDICA, true, false, false, true, array(2 => 'Prefeitura'));
echo formEntidade($entidade, $_SERVER['REQUEST_URI'], PESSOA_JURIDICA, true, false, false, true, array(2 => 'Prefeitura',
                                                                                                       7 => 'Secretaria Municipal de Educação'));

$prefeito = Entidade::carregarPrefeito($entidade);

?>

    </div>
    <script type="text/javascript">
      //<![CDATA[
        Entidade.setCamposObrigatorios(['endcep']);

        var newTr  = null;
        var inline = null;

        newTr  = $('tableEntidade').insertRow($('tableEntidade').numRows);
        inline = '<td style="font-weight: bold" colspan="2" id="">Prefeito</td>';

        Element.insert(newTr,           {bottom: inline});
        Element.insert('tableEntidade', {bottom: newTr });


        newTr  = $('tableEntidade').insertRow($('tableEntidade').numRows);
<?php

if ($prefeito->entnome != null) {
    echo '        inline = \'<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"></td><td>'
        .'<span id="entnomepref">' . addslashes($prefeito->entnome) . '</span>'
        //.'<a href="javascript:void(0);" onclick="window.opener.editarPrefeito(' . $prefeito->getPrimaryKey() . ');return false;"> (Alterar)</a>'
        .'<input type="hidden" name="prefid" id="prefid" value="' . $prefeito->getPrimaryKey() . '" /></td>\';';
} else {
    echo '        inline = \'<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">CPF:</td><td>'
        .'<span style="display:block"id="entnomepref">Não há prefeito vinculado a esta prefeitura. Para vincular, informe o CPF.</span>'
        .'<input type="text" class="normal" id="entnumcpfcnpjpref" value="" onkeyup="this.value=mascaraglobal(\\\'###.###.###-##\\\',this.value);" />'
        .'<input type="button" id="botaoBuscarPrefeito" onclick="carregarPrefeito()" value="Buscar" />'
        .'<input type="hidden" name="prefid" id="prefid" value="" /></td>\';';
}
?>

        Element.insert(newTr,           {bottom: inline});
        Element.insert('tableEntidade', {bottom: newTr });

        buscarPrefeitura = function(e)
        {
            if (trim(this.defaultValue) != '')
                return false;

            return new Ajax.Request(window.location.href,
            {
                parameters: 'buscarPrefeitura=1&entnumcpfcnpj=' + this.value,
                method: 'post',
                onComplete: function(res)
                {
                    eval(res.responseText);
                    window.location.href = '/seguranca/seguranca.php?modulo=sistema/tabapoio/cadastrarprefeitura&acao=A&entid=' + entidade.entid;
                }
            });
        }

        carregarPrefeitura = function(e)
        {
            if (trim(this.defaultValue) != '')
                this.setAttribute('readOnly', 'readOnly');
        }

        function carregarPrefeito(e)
        {
            var cpf = $F('entnumcpfcnpjpref');
            if (trim(cpf) == '')
                return false;
            else if (!validar_cpf(cpf)) {
                alert('CPF inválido.');
                return false;
            } else {
                return new Ajax.Request(window.location.href,
                {
                    parameters: 'buscarPrefeito=1&entnumcpfcnpj=' + cpf,
                    method: 'post',
                    onComplete: function(response)
                    {
                        eval(response.responseText);
                        if (entidade.entnome == '') {
                            alert('CPF não encontrado.');
                            return false;
                        }
    
                        if (confirm('Confirmar o cadastro do prefeito ' + entidade.entnome.toUpperCase() +
                                    ' (CPF: ' + mascaraglobal('###.###.###-##', entidade.entnumcpfcnpj) + ') para a <?php echo $entidade->entnome; ?>?'))
                        {
                            $('entnumcpfcnpjpref').hide();
                            $('entnomepref').update(entidade.entnome).show();
                            $('prefid').value = entidade.entid;
                        } else {
                            $('entnumcpfcnpjpref').value = '';
                        }
                    }
                });
            }
        }

        $('entnumcpfcnpj').observe('focus', carregarPrefeitura);
        $('entnumcpfcnpj').observe('blur', buscarPrefeitura);

        //if ($('entnumcpfcnpjpref'))
        //    $('entnumcpfcnpjpref').observe('blur', carregarPrefeito);
<?php

if ($db->testa_superuser()) {
    echo '
        Form.disable("frmEntidade");';

    echo '
        $("entemail",
          "entnumdddcomercial",
          "entnumcomercial",
          "entnumramalcomercial",
          "entnumdddfax",
          "entnumfax",
          "entnumramalfax",
          "endcep",
          "endlog",
          "endlogradouro",
          "endbai",
          "endnum",
          "endcom",
          "submitEntidade",
          "resetEntidade",
          "closeWindow",
          "botaoBuscarPrefeito",
          "entnumcpfcnpjpref").each(function(el) { if (!el) return false; else el.removeAttribute("disabled"); });';
}

?>

        //]]>
    </script>


