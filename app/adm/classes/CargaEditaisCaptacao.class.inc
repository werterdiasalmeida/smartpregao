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
    public function utf8_decode_recursive($data)
    {
        if (is_array($data)) {
            return array_map('utf8_decode_recursive', $data);
        } elseif (is_string($data)) {
            return utf8_decode($data);
        }
        return $data;
    }
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
        $dados['titulo'] = str_replace(array("'", '"'), ' ', $params['titulo']);
        $dados['descricao'] = str_replace(array("'", '"'), ' ', $params['descricao']);
        $dados['uf'] = $params['uf'];
        $dados['orgao'] = str_replace(array("'", '"'), ' ', $params['orgao']);;
        $dados['aberturaformatada'] = $params['aberturaformatada'];
        $dados['encerramentoformatado'] = $params['encerramentoformatado'];
        $dados['datapublicacaoformatada'] = $params['datapublicacaoformatada'];
        $campos = implode("','", $dados);
        $sql = "INSERT INTO cargas.tb_carga_editais_captacao VALUES ('{$campos}') ON CONFLICT (id_siga) DO NOTHING RETURNING id_siga;";
        $this->commit();
        $retorno = $this->carregar($sql);

        return $retorno;
    }

    public function excluirEditalCaptacao($id)
    {
        return $this->excluir($id);
    }

    public function baixarPNCP($pesquisa)
    {

        $pesquisa = str_replace(" ", "%20", $pesquisa);
        $url = 'https://pncp.gov.br/api/search/?q=' . $pesquisa . '&tipos_documento=edital&ordenacao=-data&pagina=1&tam_pagina=10000&status=recebendo_proposta';

        $curl = curl_init();
        curl_setopt_array($curl, array(
            CURLOPT_URL => $url,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_MAXREDIRS => 10,
            CURLOPT_TIMEOUT => 0,
            CURLOPT_FOLLOWLOCATION => true,
            CURLOPT_CUSTOMREQUEST => 'GET',
            CURLOPT_HTTPHEADER => array(
                'Content-Type: application/json;',
                'Accept: application/json'
            ),
        ));

        $response = curl_exec($curl);
        curl_close($curl);

        $data = json_decode($response, true);
        if ($data) {
            // Verifique se a resposta foi decodificada corretamente
            if (json_last_error() === JSON_ERROR_NONE) {
                // Converte todos os campos de string para ISO-8859-1
                $data = $this->convert_to_iso($data);
            } else {
                echo "Erro ao decodificar JSON: " . json_last_error_msg();
                ver($data, d);
            }
        }
        $reg = 0;
        foreach ($data['items'] as $dadoPncp) {

            $dado['id_siga'] = $dadoPncp['id'];
            $dado['nomeportal'] = 'PNCP';
            $dado['nomemodalidade'] = $dadoPncp['modalidade_licitacao_nome'];
            $dado['uasg'] = $dadoPncp['unidade_codigo'];
            $numero = explode('nº', $dadoPncp['title']);
            $dado['numero'] = trim($numero[1]);
            $dado['codigo'] = $dadoPncp['numero_controle_pncp'];
            $dado['titulo'] = $dadoPncp['title'];

            $dado['uf'] = $dadoPncp['uf'];
            $dado['orgao'] = $dadoPncp['orgao_nome'];

            $dado['aberturaformatada'] = str_replace('T', ' às ', $dadoPncp['data_inicio_vigencia']);
            $dado['encerramentoformatado'] = str_replace('T', ' às ', $dadoPncp['data_fim_vigencia']);
            $dado['datapublicacaoformatada'] = $dadoPncp['data_atualizacao_pncp'];
            $comp = explode('compras', $dadoPncp['item_url']);
            $link = 'https://pncp.gov.br/app/editais' . $comp[1];
            $link = " <a href='{$link}' target='_blank'>[Link PNCP]</a>";
            $dado['descricao'] = $dadoPncp['description'];
            $dado['descricao'] = trim($dado['descricao']) . "{$link}";

            $gravou = $this->salvarEditalCaptacao($dado);

            if ($gravou) {
                $reg++;
            }
        }
        $resultadoCarga = "Foram baixados <b>{$reg}</b> registros ";

        return $resultadoCarga;
    }

    // Função para converter recursivamente strings para ISO-8859-1
    public function convert_to_iso($data)
    {
        if (is_array($data)) {
            foreach ($data as $key => $value) {
                $data[$key] = $this->convert_to_iso($value);
            }
        } elseif (is_string($data)) {
            return mb_convert_encoding($data, 'ISO-8859-1', 'UTF-8');
        }
        return $data;
    }

    public function donwloadXslProcessados($tipo_carga)
    {

        //Carrega para a base de Editais 
        $sql = "SELECT *
            FROM cargas.tb_carga_editais_captacao 
            WHERE situacao ILIKE '%{$tipo_carga}%' 
            --AND DATE(data_atualizacao) = CURRENT_DATE
            ";
        $dados = (array) $this->carregar($sql);
        header("Content-type: application/octet-stream");
        header("Content-Disposition: attachment; filename=carga.xls");
        header("Pragma: no-cache");
        header("Expires: 0");
 
        echo "<table border=\"1\"><tr>
        <td>Portal</td>
        <td>Modalidade</td>
        <td>Código</td>
        <td>Título</td>
        <td>Data</td>
        <td>Órgão</td>
        <td>UF</td>
        <td>Descrição</td>
        </tr>";
        
        foreach ($dados as $linha) {
        
            $linha['encerramentoformatado'] = explode('às', $linha['encerramentoformatado']);
            $linha['encerramentoformatado'] = formata_data($linha['encerramentoformatado'][0]);
            
            echo "<tr>
            <td style=\"white-space: nowrap\">{$linha['nomeportal']}</td>
            <td style=\"white-space: nowrap\">{$linha['nomemodalidade']}</td>
            <td style=\"white-space: nowrap\">{$linha['codigo']}</td>
            <td style=\"white-space: nowrap\">{$linha['titulo']}</td>
            <td style=\"white-space: nowrap\">{$linha['encerramentoformatado']}</td>
            <td style=\"white-space: nowrap\">{$linha['orgao']}</td>
            <td style=\"white-space: nowrap\">{$linha['uf']}</td>
            <td style=\"white-space: nowrap\">{$linha['descricao']}</td>

            </tr>";
        }
        
        
        echo '</table>';
        die();
        
    }
}
