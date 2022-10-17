<?php

class Consorcio extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_consorcio";

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
        'descricao' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );


    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "c.excluido IS FALSE";

        $sql = "SELECT
                  c.descricao,
                  pj.razao_social,
                  pj.nome_fantasia,
                  pj.cnpj,
                  c.pessoajuridica_id,
                  (SELECT COUNT(*) FROM estabelecimentosaude.tb_consorcio_municipio cm WHERE cm.consorcio_id = c.id AND cm.excluido IS FALSE) AS qtd_municipio,
                  c.id
                FROM
                  estabelecimentosaude.tb_consorcio c
                INNER JOIN
                   corporativo.tb_pessoajuridica pj ON pj.id = c.pessoajuridica_id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY c.descricao ASC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function excluir($id = null, $retorno = null)
    {
        $this->excluirLogicamente($id);
    }

    public function salvarConsorcio($params, $arquivos = array()){
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        if($params['id']){
            $this->carregarPorId($params['id']);
        }

        $mPessoaJuridica = new PessoaJuridica();
        $paramsPj = $params;
        $paramsPj['id'] = $this->pessoajuridica_id;
        $pessoaJuridicaId = $mPessoaJuridica->salvarPessoaJuridica($paramsPj, $arquivos);

        $dados = array(
            'id' => $params['id'],
            'descricao' => $params['descricao'],
            'pessoajuridica_id' => $pessoaJuridicaId,
            'geo_localizacao' => null,
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        $id = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);

        $mConsorcioMunicipio = new ConsorcioMunicipio();
        $mConsorcioMunicipio->excluirPorConsorcio($id);

        $municipiosIndisponiveis = $mConsorcioMunicipio->getIdMunicipiosIndisponiveis($id);
        $params['municipios'] = is_array($params['municipios']) ? $params['municipios'] : array();

        foreach ($params['municipios'] as $municipioId){
            if(in_array($municipioId, $municipiosIndisponiveis)){
                throw new MunicipioConsorcioIndisponivel();
            }

            $mConsorcioMunicipio->salvarConsorcioMunicipio(array(
                'municipio_id' => $municipioId,
                'consorcio_id' => $id,
            ));

            $mConsorcioMunicipio->setDadosNull();
        }

        return $id;
    }

    public function getTodosDados($id)
    {
        $sql = "SELECT
                  pj.nome_fantasia,
                  pj.razao_social,
                  pj.cnpj,
                  pj.classificacao,
                  pj.logo_arquivo_id,
                  con.descricao,
                  con.pessoajuridica_id,
                  con.id
                FROM
                  estabelecimentosaude.tb_consorcio con
                INNER JOIN
                  corporativo.tb_pessoajuridica pj ON con.pessoajuridica_id = pj.id
                                                  AND pj.excluido IS FALSE
                WHERE
                  con.id = {$id}";

        return $this->pegaLinha($sql);
    }
}