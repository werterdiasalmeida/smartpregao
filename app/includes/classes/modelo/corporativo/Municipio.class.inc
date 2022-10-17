<?php
	
class Municipio extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_municipio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "id" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'id' => null,
									  	'uf_id' => null,
									  	'microregiao_id' => null,
									  	'mesoregiao_id' => null,
									  	'municipio' => null,
									  	'processo' => null,
									  	'cod_ibge_completo' => null,
									  	'latitude' => null,
									  	'longitude' => null,
									  	'hemisferio' => null,
									  	'altitude' => null,
									  	'area' => null,
									  	'cep_menor' => null,
									  	'cep_maior' => null,
									  	'raio' => null,
									  	'meridiano' => null,
									  	'codigo_siafi' => null,
									  	'populacao' => null,
									  	'populacao_homem' => null,
									  	'populacao_mulher' => null,
									  	'populacao_urbana' => null,
									  	'populacao_rural' => null,
									  );

    public function getCombo($jsonEncode = false){

        $sql = "SELECT
                    mun.id AS codigo, 
                    mun.municipio AS descricao,
                    mun.municipio || '/' || uf.uf_sigla AS descricaoCompleta,
                    uf.uf_nome_estado AS estado,
                    uf.uf_sigla AS sigla
                FROM
                    {$this->stNomeTabela} mun
                JOIN
                    corporativo.tb_unidade_federacao uf ON uf.id = mun.uf_id
                ORDER BY
                    uf.uf_sigla,mun.municipio";

        $retorno = $this->carregar($sql);
        $retorno = $retorno ? $retorno : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode(addslashesArray($retorno))) : $retorno;
    }

}