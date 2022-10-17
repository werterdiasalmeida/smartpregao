<?php
//
// $Id
//


function salvarEndereco()
{
    if (array_key_exists('endid', $_REQUEST) && $_REQUEST['endid'] != '') {
        atualizarEntidadeEndereco($_REQUEST);
    } else {
        inserirEntidadeEndereco($_REQUEST);
    }
}



/**
 * 
 */
function listarEnderecoEntidade($entid)
{
    $sql = "select
                '<img src=\"../imagens/alterar.gif\" alt=\"Alterar Endereço\" onclick=\"alterarEndereco(' || en.endid || ', this)\" />&nbsp;&nbsp;&nbsp;<img src=\"../imagens/excluir.gif\" alt=\"Excluir Endereço\" onclick=\"excluirEndereco(' || en.endid || ')\" />' as endid,
                te.tpedsc,
                en.endcep,
                en.endlog,
                en.endnum,
                en.endcom,
                en.endbai,
                en.estuf,
                en.muncod
            from
                entidade.endereco as en
            left join
                entidade.tipoendereco as te
            on
                en.tpeid = te.tpeid
            where
                entid = " . pg_escape_string($entid);

    $cabecalho = array('Ações',
                       'Tipo de Endereço',
                       'CEP',
                       'Logradouro',
                       'Número',
                       'Complemento',
                       'Bairro',
                       'UF',
                       'Município');

    global $db;
    $db->monta_lista($sql, $cabecalho, 50, 20, '', '', '');
}


/**
 * 
 */
function atualizarEntidadeEndereco(array $endereco)
{
    $sql = "UPDATE entidade.endereco SET
                entid     =  " .pg_escape_string($endereco['entid']    ). ",
                tpeid     =  " .pg_escape_string($endereco['tpeid']    ). "null,
                endcep    = '" .str_replace     (array("-", "."), '', pg_escape_string($endereco['endcep'])). "',
                endlog    = '" .pg_escape_string($endereco['endlog']   ). "',
                endcom    = '" .pg_escape_string($endereco['endcom']   ). "',
                endbai    = '" .pg_escape_string($endereco['endbai']   ). "',
                muncod    = '" .pg_escape_string($endereco['muncod']   ). "',
                estuf     = '" .pg_escape_string($endereco['estuf']    ). "',
                endnum    = '" .pg_escape_string($endereco['endnum']   ). "'
            WHERE
                endid     =  " .pg_escape_string($endereco['endid']) . " AND
                entid     =  " .pg_escape_string($endereco['entid']);

    global $db;
    //print_r($endereco);

    //echo $sql;
    //die();

    $db->executar($sql);

    if ($db->commit()) {
        die('Registro atualizado com sucesso!');
    } else {
        die('Não foi possível executar a atualização do registro!');
    }
}


/**
 * 
 */
function inserirEntidadeEndereco(array $endereco)
{
    $sql = "INSERT INTO entidade.endereco (
                entid ,
                tpeid ,
                endcep,
                endlog,
                endcom,
                endbai,
                muncod,
                estuf ,
                endnum,
                endstatus
            ) VALUES (
                 " .pg_escape_string($endereco['entid']     ). ",
                 " .pg_escape_string($endereco['tpeid']     ). ",
                '" .str_replace     (array("-", "."), '', pg_escape_string($endereco['endcep'])). "',
                '" .pg_escape_string($endereco['endlog']    ). "',
                '" .pg_escape_string($endereco['endcom']    ). "',
                '" .pg_escape_string($endereco['endbai']    ). "',
                '" .pg_escape_string($endereco['muncod']    ). "',
                '" .pg_escape_string($endereco['estuf']     ). "',
                '" .pg_escape_string($endereco['endnum']    ). "',
                '" .pg_escape_string($endereco['endstatus'] ). "'
            )";

    global $db;
    $db->executar($sql);

    if ($db->commit()) {
        die('Registro inserido com sucesso!');
    } else {
        die('Não foi possível executar a inserção do registro!');
    }
}


/**
 * 
 */
function excluirEntidadeEndereco($endid)
{
    $sql = "UPDATE entidade.endereco SET endstatus = 'I' WHERE endid = " . $endid;

    global $db;
    $db->executar($sql);

    if ($db->commit()) {
        die('Registro excluído com sucesso!');
    } else {
        die('Não foi possível executar a exclusão do registro!');
    }
}


/**
 * 
 */
function formEntidadeEndereco(array $dados = array(), $return = false)
{
    global $db;
    global $endcep, $endnum, $endcom;

    if (array_key_exists('endid', $_REQUEST)) {
        $sql   = "SELECT
                      endid ,
                      entid ,
                      tpeid ,
                      endcep,
                      endlog,
                      endcom,
                      endbai,
                      muncod,
                      estuf ,
                      endnum,
                      endstatus
                  FROM
                      entidade.endereco
                  WHERE
                      endid = " . $_REQUEST['endid'];

        $dados = $db->pegaLinha($sql);

    }

    $sql1         = "SELECT
                         tpeid as codigo,
                         tpedsc as descricao
                     FROM
                         entidade.tipoendereco
                     WHERE
                         tpestatus = 'A'
                     ORDER BY
                         tpedsc";

    $sql2         = "SELECT
                         regcod AS codigo,
                         regcod || ' - ' || descricaouf AS descricao
                     FROM
                         uf
                     WHERE
                         codigoibgeuf IS NOT NULL
                     ORDER BY 2";

    $dadosDefault = array('entid'     => null,
                          'tpeid'     => null,
                          'endcep'    => null,
                          'endlog'    => null,
                          'endcom'    => null,
                          'endbai'    => null,
                          'muncod'    => null,
                          'estuf'     => null,
                          'endnum'    => null,
                          'endstatus' => null);

    
    $dados        = array_merge($dadosDefault, $dados);

    //dbg($dados);
    
    $endcep       = addslashes($dados['endcep']);
    $endnum       = addslashes($dados['endnum']);
    $endcom       = addslashes($dados['endcom']);
    $endbai       = addslashes($dados['endbai']);
    $endlog       = addslashes($dados['endlog']);
    $muncod       = addslashes($dados['muncod']);
    $estuf     	  = addslashes($dados['estuf' ]);
    $endnum       = addslashes($dados['endnum']);

    $sql3 	        = " select muncod as codigo, mundescricao as descricao 
    					from territorios.municipio m where estuf = '$estuf'";
    
    $form         = '
    <form action="/geral/dne.php?opt=endereco" method="post" id="frmEndereco" onsubmit="return false;">
      <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <colgroup>
          <col width="25%" />
          <col width="75%" />
        </colgroup>

        <tr>
          <td align="right" class="SubTituloDireita"><label for="endcep_' .$dados['endid' ]. '">CEP:</label></td>
          <td>' . campo_texto('endcep', 'S', '', '', 12, 10, '##.###-###', '', 'left', '', 0, 'onblur="getEnderecoPeloCEP(\'' .$dados['endid' ]. '\');" id="endcep_' .$dados['endid' ]. '"', '', $endcep ) . '</td>
        </tr>

        <tr>
          <td align="right" class="SubTituloDireita"><label for="endlog_' .$dados['endid' ]. '">Logradouro:</label></td>
          <td>' . campo_texto('endlog', 'N', 'N', '', 50, 200, '', '', 'left', '', 0, 'id="endlog_' .$dados['endid' ]. '"', '', $endlog) . '</td>
        </tr>

        <tr>
          <td align="right" class="SubTituloDireita"><label for="endnum_' .$dados['endid' ]. '">Número:</label></td>
          <td>' . campo_texto('endnum', 'S', 'S', '', 12, 10, '', '', 'left', '', 0, 'id="endnum_' .$dados['endid' ]. '"', '', $endnum) . '</td>
        </tr>

        <tr>
          <td align="right" class="SubTituloDireita"><label for="endcom_' .$dados['endid' ]. '">Complemento:</label></td>
          <td>' . campo_texto('endcom', 'N', 'S', '', 50, 200, '', '', 'left', '', 0, 'id="endcom_' .$dados['endid' ]. '"', '', $endcom) . '</td>
        </tr>

        <tr>
          <td align="right" class="SubTituloDireita"><label for="endbai_' .$dados['endid' ]. '">Bairro:</label></td>
          <td>' . campo_texto('endbai', 'N', 'N', '', 50, 100, '', '', 'left', '', 0, 'id="endbai_' .$dados['endid' ]. '"', '', $endbai) . '</td>
        </tr>

        <tr>
          <td align="right" class="SubTituloDireita"><label for="estuf_' .$dados['endid' ]. '">UF:</label></td>

          <td>' . $db->monta_combo('estuf', $sql2, 'N', 'Selecione uma UF', 'carregarMunicipiosPorUf', '', '', '', 'N', 'estuf_' .$dados['endid' ], true, $estuf) . '</td>
        </tr>

        <tr>
          <td align="right" class="SubTituloDireita"><label for="muncod_' .$dados['endid' ]. '">Município:</label></td>
          <td>' . $db->monta_combo('muncod', $sql3, 'N', 'Selecione um município', '', '', '', '', 'N', 'muncod_' .$dados['endid' ], true, $muncod) . '</td>
        </tr>
      </table>
    </form>
    <script type="text/javascript">
    </script>';


    if ($return) {
        return $form;
    } else {
        echo $form;
    }
}


