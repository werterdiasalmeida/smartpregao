<?php

class ConsorcioMunicipio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_consorcio_municipio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("id");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id' => null,
        'consorcio_id' => null,
        'municipio_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function salvarConsorcioMunicipio($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $dados = array(
            'id' => $this->recuperarUm('id', array("consorcio_id = {$params['consorcio_id']}", "municipio_id = {$params['municipio_id']}")),
            'consorcio_id' => $params['consorcio_id'],
            'municipio_id' => $params['municipio_id'],
            'excluido' => 'false',
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function excluirPorConsorcio($consorcioId)
    {
        $sql = "UPDATE estabelecimentosaude.tb_consorcio_municipio SET excluido = 't' WHERE consorcio_id = {$consorcioId}";
        $this->executar($sql);
    }

    public function getPorConsorcio($consorcioId, $jsonEncode = false)
    {
        $sql = "SELECT
                    municipio_id
                FROM
                    estabelecimentosaude.tb_consorcio_municipio
                WHERE
                    consorcio_id = {$consorcioId}
                    AND
                    excluido IS FALSE";

        $rs = $this->carregarColuna($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode($rs) : $rs;
    }

    public function getMunicipiosDisponiveis($consorcioId = null, $recuperarIndisponiveisComFlag = false, $jsonEncode = false)
    {
        $arWhere = array(
            '(com.excluido IS FALSE OR com.id IS NULL)',
            '(cm.excluido IS FALSE OR cm.id IS NULL)'
        );

        $colunaDisponivel = ", CASE WHEN com.id IS NULL THEN true ELSE false END AS disponivel";
        if(!$recuperarIndisponiveisComFlag){
            $colunaDisponivel = '';
            $arWhere[] = "com.id IS NULL";
        }

        $whereConsorcio = '';
        if($consorcioId){
            $whereConsorcio = "AND cm.consorcio_id != {$consorcioId}";
        }

        $sql = "SELECT DISTINCT 
                    mun.id AS codigo, 
                    mun.municipio AS descricao,
                    mun.municipio || '/' || uf.uf_sigla AS descricaoCompleta,
                    uf.uf_nome_estado AS estado,
                    uf.uf_sigla AS sigla
                    {$colunaDisponivel}
                FROM
                    corporativo.tb_municipio mun
                INNER JOIN
                    corporativo.tb_unidade_federacao uf ON uf.id = mun.uf_id
                LEFT JOIN
                    estabelecimentosaude.tb_consorcio_municipio cm ON mun.id = cm.municipio_id
                                                                  AND cm.excluido IS FALSE
                                                                 {$whereConsorcio}
                LEFT JOIN
                    estabelecimentosaude.tb_consorcio com ON com.id = cm.consorcio_id
                                                    AND com.excluido IS FALSE
                WHERE
                    " . implode(' AND ', $arWhere) . "
                ORDER BY
                    uf.uf_sigla, mun.municipio";

        $retorno = $this->carregar($sql);
        $retorno = $retorno ? $retorno : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode(addslashesArray($retorno))) : $retorno;
    }

    public function getIdMunicipiosIndisponiveis($consorcioId = null, $jsonEncode = false)
    {
        $muncipios = $this->getMunicipiosDisponiveis($consorcioId, true, false);
        $muncipios = array_filter(array_map(function($municipio){
            return $municipio['disponivel'] === 'f' ? $municipio['codigo'] : null;
        }, $muncipios));

        return $jsonEncode ? json_encode(utf8ArrayEncode(addslashesArray($muncipios))) : $muncipios;
    }
}