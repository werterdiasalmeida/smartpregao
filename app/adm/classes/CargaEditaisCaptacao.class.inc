<?php

class CargaEditaisCaptacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cargas.tb_carga_editais_captacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("id_siga");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id_siga' => null,
        'nome' => null,
        'nomeportal' => null,
        'nomemodalidade' => null,
        'uasg' => null,
        'numero' => null,
        'codigo' => null,
        'titulo' => null,
        'descricao' => null,
        'uf' => null,
        'orgao' => null,
        'aberturaformatada' => null,
        'encerramentoformatado' => null,
        'datapublicacaoformatada' => null,
        'situacao' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {

        $arWhere[] = "id_siga IS NOT NULL";
        $arWhere[] = "codigo IS NOT NULL";
        $arWhere[] = "codigo <> ''";
        $sql = "SELECT
                   id_siga,
                   codigo,
                   nomeportal,
                   aberturaformatada,
                   orgao,
                   uasg,
                   numero,
                   titulo,
                   descricao,
                   datapublicacaoformatada,
                   situacao
                FROM
                    cargas.tb_carga_editais_captacao car
                WHERE
                  " . implode(' AND ', $arWhere);
        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY 1 DESC";
            }
        }
        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 20);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function getListEditaisCaptacao($arWhere = array())
    {
        $arWhere[] = "id_siga IS NOT NULL";
        $arWhere[] = "codigo IS NOT NULL";
        $arWhere[] = "codigo <> ''";
        $sql = "SELECT
                   id_siga,
                   codigo
                FROM
                    cargas.tb_carga_editais_captacao car
                WHERE
                  " . implode(' AND ', $arWhere);
        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public function getListEditaisDownloadCompleto($arWhere = array())
    {
        $arWhere[] = "id_siga IS NOT NULL";
        $arWhere[] = "codigo IS NOT NULL";
        $arWhere[] = "codigo <> ''";
        $sql = "SELECT
                    car.orgao as orgao,
                    car.titulo as edital,
                    car.descricao as objeto,
                    car.uf as uf,
                    car.aberturaformatada,
                    emp.cnpj,
                    emp.nome_fantasia,
                    emp.razao_social,
                    emp.email_1,
                    emp.telefone_1,
                    emp.municipio_uf,
                    emp.cnae
                FROM
                    cargas.tb_carga_editais_captacao car
                    JOIN cargas.tb_carga_estabelecimento_pregao est ON est.prgcod::integer = car.codigo::integer
                    JOIN cargas.tb_carga_estabelecimento emp ON emp.cnpj = est.cnpj
                WHERE
                  " . implode(' AND ', $arWhere);
                  //ver($sql);
        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }


    public function salvarEditalCaptacao($params)
    {
        $dados['id_siga'] = $params['id_siga'];
        $dados['nomeportal'] = $params['nomeportal'];
        $dados['nomemodalidade'] = $params['nomemodalidade'];
        $dados['uasg'] = $params['uasg'];
        $dados['numero'] = $params['numero'];
        $dados['codigo'] = $params['codigo'];
        $dados['titulo'] = $params['titulo'];
        $dados['descricao'] = $params['descricao'];
        $dados['uf'] = $params['uf'];
        $dados['orgao'] = $params['orgao'];
        $dados['aberturaformatada'] = $params['aberturaformatada'];
        $dados['encerramentoformatado'] = $params['encerramentoformatado'];
        $dados['datapublicacaoformatada'] = $params['datapublicacaoformatada'];
        $campos = implode("','", $dados);
        $sql = "INSERT INTO cargas.tb_carga_editais_captacao VALUES ('{$campos}')";
        $retorno = $this->carregar($sql);

        return $retorno;
    }

    public function excluirEditalCaptacao($id)
    {
        return $this->excluir($id);
    }
}
