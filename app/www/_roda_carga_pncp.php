<?php

include_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/SimecHashids.class.inc";

$db = new cls_banco();
$_SESSION['superuser'] = true;

function __autoload($class_name)
{
    $arCaminho = array(
        APPRAIZ . "adm/classes/",
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/mensageria/",
        APPRAIZ . "includes/classes/modelo/financeiro/",
        APPRAIZ . "includes/classes/",
    );

    foreach ($arCaminho as $caminho) {
        $arquivo = $caminho . $class_name . '.class.inc';
        if (file_exists($arquivo)) {
            require_once($arquivo);
            break;
        }
    }
}

include_once 'adm/_funcoes.php';

$mCargaEditaisCaptacao = new CargaEditaisCaptacao();
$mCargaEditais = new CargaEditais();
$mEdital = new Edital();

$sql = "select DISTINCT tipo_carga from cargas.tb_carga_palavra_chave ORDER BY tipo_carga";
$tipo = $mCargaEditaisCaptacao->carregar($sql);
foreach ($tipo as $dado_tipo) {
    $tipo_carga = $dado_tipo['tipo_carga'];
    $sql = "select palavra from cargas.tb_carga_palavra_chave where tipo_carga = '{$tipo_carga}'";
    $palavras = $mCargaEditaisCaptacao->carregar($sql);
    foreach ($palavras as $dado) {
        $resultadoCarga = $mCargaEditaisCaptacao->baixarPNCP($dado['palavra']);
        echo $resultadoCarga . ' para ' . $dado['palavra'] . '<br/>';
    }
    echo "<br>----------------------------------------<br>";

    // Marca os registros carregados, pelo objeto
    foreach ($palavras as $dado) {
        $sql = "update cargas.tb_carga_editais_captacao 
        set situacao = '{$tipo_carga}' 
        where descricao ilike '%{$dado['palavra']}%';";
        $mCargaEditaisCaptacao->carregar($sql);
        $mCargaEditaisCaptacao->commit();
    }

    $sql = "SELECT COUNT(0)
                FROM cargas.tb_carga_editais_captacao 
                WHERE situacao ILIKE '%{$tipo_carga}%' 
                AND DATE(data_atualizacao) = CURRENT_DATE";
    $num = $mCargaEditaisCaptacao->pegaUm($sql);
    echo "<br>Foram processados <b>{$num}</b> registros para <b>{$tipo_carga}</b> pelo Objeto.";

    //Carrega para a base de Editais 
    $sql = "SELECT *
                FROM cargas.tb_carga_editais_captacao 
                WHERE situacao ILIKE '%{$tipo_carga}%' 
                AND DATE(data_atualizacao) = CURRENT_DATE";
    $editaisCarga = $mCargaEditaisCaptacao->carregar($sql);

    foreach ($editaisCarga as $dado) {

        $salvar['edital'] =  $dado['nomemodalidade'] . ' ' . $dado['numero'] . ' UASG: ' . $dado['uasg'];
        $salvar['portal'] = 'ComprasNet';
        $salvar['orgao'] = $dado['orgao'];
        $dataHora = explode(' às ', $dado['encerramentoformatado']);
        $salvar['data_limite'] = $dataHora[0];
        $salvar['hora_limite'] = substr($dataHora[1], 0, 5);
        $salvar['data_limite'] = $salvar['data_limite'];
        $salvar['objeto'] = str_replace('"', '', $dado['descricao']);
        $salvar['objeto'] = str_replace('?', ' ', $salvar['objeto']);
        $salvar['objeto'] = str_replace('...', ' ', $salvar['objeto']);
        $salvar['objeto'] = str_replace('*', '', $salvar['objeto']);
        $salvar['valor_estimado'] = 'R$ -';
        $salvar['id_externo'] = $dado['id_siga'];

        if (!$salvar['id_origem']) $salvar['id_origem'] = 'NULL';
        $salvar['categoria'] = $tipo_carga;
        $existe = $mEdital->pegaLinha(" SELECT id FROM edital.edital  WHERE id_externo::TEXT = '{$salvar['id_externo']}'");
      //ver($salvar, $existe,d);
        if (!$existe) {
            $id = $mCargaEditais->salvarEditalCarga($salvar);
            $mCargaEditais->commit();
        }
    }
    echo "<br><br>-------------- Registros Carregados -------------------<br>";

}
