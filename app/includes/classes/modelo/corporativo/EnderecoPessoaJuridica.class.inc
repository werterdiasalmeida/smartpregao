<?php

class EnderecoPessoaJuridica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_endereco_pessoajuridica";

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
        'pessoajuridica_id' => null,
        'tipo_endereco' => null,
        'logradouro' => null,
        'bairro' => null,
        'complemento' => null,
        'cep' => null,
        'uf_id' => null,
        'municipio_id' => null,
        'geo_localizacao' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function excluirPorPessoaJuridica($pessoaJuridicaId)
    {
        $sql = "UPDATE corporativo.tb_endereco_pessoajuridica SET excluido = TRUE WHERE pessoajuridica_id = {$pessoaJuridicaId}";
        $this->executar($sql);
    }

    public function salvarEndereco($params)
    {
        $camposNulos = array();
        foreach ($params as $campo => $valor){
            if($valor === ''){
                $params[$campo] = null;
                $camposNulos[] = $campo;
            }
        }

        $ufId = $params['uf_id'];
        if($params['uf_sigla']){
            $mUnidadeFederacao = new UnidadeFederacao();
            $ufId = $mUnidadeFederacao->getIdBySigla($params['uf_sigla']);
        }

        $municipioId = $params['municipio_id'];
        if($params['muncod']){
            $municipioId = $params['muncod'];
        }

        $localizacao = null;
        if($params['geo_localizacao']){
            $coordenadas = $params['geo_localizacao'];
            if(is_string($params['geo_localizacao'])){
                $coordenadas = json_decode(str_replace('\"', '"', $params['geo_localizacao']), true);
            }
            $localizacao = "POINT({$coordenadas['lat']} {$coordenadas['lng']})";
        }

        $dados = array(
            'id' => $params['id'],
            'pessoajuridica_id' => $params['pessoajuridica_id'],
            'tipo_endereco' => $params['tipo_endereco'] ? $params['tipo_endereco'] : 'C',
            'logradouro' => $params['logradouro'],
            'bairro' => $params['bairro'],
            'complemento' => $params['complemento'],
            'cep' => str_replace(array('.', '-'), '', $params['cep']),
            'uf_id' => $ufId,
            'municipio_id' => $municipioId,
            'geo_localizacao' => $localizacao,
            'excluido' => 'f',
        );

        if(empty($params['id'])){
            $dadosCarimbo = array(
                'criado_em' => 'now()',
                'criado_por' => $_SESSION['usuario_id'],
            );
        }else{
            $dadosCarimbo = array(
                'atualizado_em' => 'now()',
                'atualizado_por' => $_SESSION['usuario_id'],
            );
        }

        $dados = array_merge($dados, $dadosCarimbo);
        $retorno = $this->popularDadosObjeto($dados)->salvar();
        return $retorno;
    }

    public function listarPorPessoaJuridica($pessoaJuridicaId, $jsonEncode = false, $limit = false)
    {
        $sql = "SELECT
                    epj.id, 
                    epj.logradouro, 
                    epj.bairro, 
                    epj.complemento, 
                    epj.cep AS cep,
                    ST_AsGeoJSON(epj.geo_localizacao) AS geo_localizacao,
                    mun.municipio || '/' || uf.uf_sigla AS estado_cidade,
                    uf.uf_sigla, 
                    mun.municipio,
                    mun.id AS municipio_id
                FROM
                    corporativo.tb_endereco_pessoajuridica epj
                LEFT JOIN
                    corporativo.tb_unidade_federacao uf ON epj.uf_id = uf.id
                LEFT JOIN
                    corporativo.tb_municipio mun ON epj.municipio_id = mun.id
                WHERE
                    epj.pessoajuridica_id = {$pessoaJuridicaId}
                    AND
                    epj.excluido IS FALSE";

        if($limit !== false && is_integer($limit)){
            $sql .= " LIMIT {$limit} ";
        }

        $enderecos = $this->carregar($sql);
        $enderecos = $enderecos ? $enderecos : array();

        $enderecos = array_map(function($endereco) {
            if($endereco['cep']){
                $endereco['cep'] = formata_cep($endereco['cep']);
            }

            if($endereco['geo_localizacao']){
                $coordenadas = json_decode($endereco['geo_localizacao'], true);
                $endereco['geo_localizacao'] = array(
                    'lat' => floatval($coordenadas['coordinates'][0]),
                    'lng' => floatval($coordenadas['coordinates'][1])
                );
            }
            return $endereco;
        }, $enderecos);
        return $jsonEncode ? json_encode(utf8ArrayEncode($enderecos)) : $enderecos;
    }

    public function getPrincipalPorPessoaJuridica($pessoaJuridicaId)
    {
        $sql = "SELECT
                    epj.id, 
                    epj.logradouro, 
                    epj.bairro, 
                    epj.complemento, 
                    epj.cep AS cep,
                    ST_AsGeoJSON(epj.geo_localizacao) AS geo_localizacao,
                    mun.municipio || '/' || uf.uf_sigla AS estado_cidade,
                    uf.uf_sigla, 
                    mun.municipio,
                    mun.id AS municipio_id
                FROM
                    corporativo.tb_endereco_pessoajuridica epj
                LEFT JOIN
                    corporativo.tb_unidade_federacao uf ON epj.uf_id = uf.id
                LEFT JOIN
                    corporativo.tb_municipio mun ON epj.municipio_id = mun.id
                WHERE
                    epj.pessoajuridica_id = {$pessoaJuridicaId}
                    AND
                    epj.excluido IS FALSE";

        $endereco = $this->pegaLinha($sql);
        return $endereco ? $endereco : array();
    }
}