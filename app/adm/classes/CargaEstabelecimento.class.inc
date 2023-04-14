<?php

use Respect\Validation\Rules\TrueVal;

class CargaEstabelecimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cargas.tb_carga_estabelecimento";

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
        'cnpj' => null,
        'razao_social' => null,
        'nome_fantasia' => null,
        'cnae' => null,
        'data_abertura' => null,
        'municipio_uf' => null,
        'email_1' => null,
        'email_2' => null,
        'telefone_1' => null,
        'telefone_2' => null,
        'situacao_carga' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "cnpj IS NOT NULL";
        $sql = "SELECT
            id,
            cnpj,
            razao_social,
            nome_fantasia,
            cnae,
            municipio_uf,
            email_1,
            telefone_1,
            situacao_carga,
            array_to_string(array((SELECT prgcod FROM cargas.tb_carga_estabelecimento_pregao WHERE cnpj = car.cnpj)), ', ')as pregoes
        FROM
        cargas.tb_carga_estabelecimento car
         
                WHERE
                  " . implode(' AND ', $arWhere);
        $sql .= " GROUP  BY 1,2,3,4,5";
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
    public function getListCNPJ($arWhere = array())
    {
        $arWhere[] = "cnpj IS NOT NULL";
        $sql = "SELECT
                    id,
                    cnpj
                FROM
                    cargas.tb_carga_estabelecimento car
                WHERE
                  " . implode(' AND ', $arWhere);
        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }
    public function getListCNPJDadosBasicos($arWhere = array())
    {
        $arWhere[] = "cnpj IS NOT NULL";
        $sql = "SELECT
                    id,
                    cnpj
                FROM
                    cargas.tb_carga_estabelecimento car
                WHERE
                  " . implode(' AND ', $arWhere);
        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public function salvarBasico($params)
    {
        $sql = "INSERT INTO cargas.tb_carga_estabelecimento 
            (cnpj, 
            razao_social,
            situacao_carga
            ) 
            VALUES (
                '{$params['cnpj']}',
                '{$params['razao_social']}',
                '{$params['situacao_carga']}'
                )";
        $this->executar($sql);
        return $this->commit();
    }
    public function salvarCNPJPregao($params)
    {
        $sql = "INSERT INTO cargas.tb_carga_estabelecimento_pregao 
            (cnpj, 
            prgcod
            ) 
            VALUES (
                '{$params['cnpj']}',
                '{$params['prgcod']}'
                )";
        $this->executar($sql);
        $sql = "UPDATE cargas.tb_carga_editais_captacao SET situacao = 'Clientes Baixados' WHERE codigo = '{$params['prgcod']}'";
        $this->executar($sql);

        return $this->commit();
    }


    public function excluirEstabelecimento($id)
    {
        return $this->excluir($id);
    }

    public function consutlaAPICNPJ($cnpj)
    {
        $cnpj = str_replace('/', '', $cnpj);
        $cnpj = str_replace('.', '', $cnpj);
        $cnpj = str_replace('-', '', $cnpj);
        $url = "https://apibus.fabricawebsis.com.br/services-bar/api-atualizar-dados-empresa-cnpj/index.php";
        $apikey = '7d75a34f365a17df69592f96567552016dd023630bad8a5820d91db9559d7845b723233984efeecc731ae2fed43398ce';
        $ch = curl_init();
        
        curl_setopt($ch, CURLOPT_URL, $url);
        // Configura
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, array(
            'api_key' => $apikey,
            'cnpj' => $cnpj,
        ));
        // Define a opção CURLOPT_FOLLOWLOCATION para permitir o redirecionamento
        curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
        // Envio
        $resp = curl_exec($ch);
        curl_close($ch);
        $resp = json_decode($resp,true);

        $estabelecimento = array_map("utf8_decode", $resp['company']);
        $atividade = array_map("utf8_decode", $resp['mainActivity']);
        $cidade = array_map("utf8_decode", $resp['address']);
        $estado = array_map("utf8_decode", $resp['address']);

        $saida['nome_fantasia'] = $estabelecimento['name'];
        $saida['atividade_principal'] = $atividade['id'] . ' - ' . $atividade['text'];
        $saida['data_abertura'] = $resp['founded'];
        $saida['municipio_uf'] = $cidade['city'] . ' (' . $estado['state'] . ')';
        $saida['email_1'] = $resp['emails'][0]['address'];
        $saida['telefone_1'] = '(' . $resp['phones'][0]['area'] . ') ' . $resp['phones'][0]['number'];
        
        return $saida;
    }
    public function salvarDadosAPICNPJ($cnpj)
    {
        $retorno = '';
        // Baixa os dados complementares da API
        $dadosComplementares = $this->consutlaAPICNPJ($cnpj);

//        ver($dadosComplementares,d);

        $rsDadosBaixados['nome_fantasia'] = str_replace("'", ' ', $dadosComplementares['nome_fantasia']);
        $rsDadosBaixados['cnae'] = str_replace("'", ' ', $dadosComplementares['atividade_principal']);
        $rsDadosBaixados['data_abertura'] = $dadosComplementares['data_abertura'];
        $rsDadosBaixados['municipio_uf'] = str_replace("'", ' ', $dadosComplementares['municipio_uf']);
        $rsDadosBaixados['email_1'] = $dadosComplementares['email_1'];
        $rsDadosBaixados['telefone_1'] = $dadosComplementares['telefone_1'];
        if ($rsDadosBaixados['nome_fantasia'] == '') {
            $rsDadosBaixados['nome_fantasia'] = 'Serviço indisponível';
        } else {
            $sql = "UPDATE cargas.tb_carga_estabelecimento SET 
                nome_fantasia = '{$rsDadosBaixados['nome_fantasia']}',
                cnae = '{$rsDadosBaixados['cnae']}',
                data_abertura = '{$rsDadosBaixados['data_abertura']}',
                municipio_uf = '{$rsDadosBaixados['municipio_uf']}',
                email_1 = '{$rsDadosBaixados['email_1']}',
                telefone_1 = '{$rsDadosBaixados['telefone_1']}',
                situacao_carga = 'Atualizado API'
                WHERE cnpj = '{$cnpj}'";
            $this->executar($sql);
        }
        $retorno = 'Atualizado API: ' .  $cnpj . ' ( ' . $rsDadosBaixados['nome_fantasia']  . ') <br/>';
        return $retorno;
    }

    public function retornaDadosDashboard()
    {
        $saida['editais_cadastrados'] = $this->pegaUm(" SELECT count(0) FROM edital.edital");
        $saida['editais_captacao'] = $this->pegaUm(" SELECT count(0) FROM cargas.tb_carga_editais_captacao car");
        $saida['clientes_captacao'] = $this->pegaUm(" SELECT count(0) FROM cargas.tb_carga_estabelecimento");

        $saida['editais_cadastrados_update'] = $this->pegaUm(" SELECT MAX(data_atualizacao) FROM edital.edital");
        $saida['editais_captacao_update'] = $this->pegaUm(" SELECT MAX(data_atualizacao) FROM cargas.tb_carga_editais_captacao car");
        $saida['clientes_autalizados_api'] = $this->pegaUm(" SELECT COUNT(0) FROM cargas.tb_carga_estabelecimento WHERE situacao_carga = 'Atualizado API'");

        return $saida;
    }

    public function baixarGravarEmpresasPregao($params)
    {

        $prgcod = $params['prgcod'];
        $url = "http://comprasnet.gov.br/livre/pregao/anexosPropostaHabilitacao.asp?prgCod={$prgcod}";
        $curl = curl_init();
        curl_setopt_array($curl, array(
            CURLOPT_URL => $url,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_MAXREDIRS => 10,
            CURLOPT_TIMEOUT => 0,
            CURLOPT_FOLLOWLOCATION => true,
            CURLOPT_CUSTOMREQUEST => 'GET',
        ));
        $response = curl_exec($curl);
        curl_close($curl);
        $return = explode('<tr class="tex3" bgcolor="#FFFFFF">', $response);
        $resultadoCarga = '';
        if (is_array($return)) {
            foreach ($return as $dados) {
                $rsDadosBaixados = array();
                $dado  = explode('<td>', $dados);
                $texto = $dado[1];
                $saida = explode(' - ', $texto);
                $saida[0] = str_replace(' ', '', $saida[0]);
                if ($saida[1]) {
                    $rsDadosBaixados['cnpj'] = $saida[0];
                    $texto2 = explode('   ', $saida[1]);
                    $rsDadosBaixados['razao_social'] = $texto2[0];
                    $rsDadosBaixados['situacao_carga'] = 'dados-basicos';
                    $rsDadosBaixados['prgcod'] = $prgcod;
                    //Grava no Banco
                    $existe = $this->pegaLinha(" SELECT id FROM cargas.tb_carga_estabelecimento  WHERE cnpj = '{$rsDadosBaixados['cnpj']}'");
                    if (!$existe) {
                        $this->salvarBasico($rsDadosBaixados);
                        $this->salvarCNPJPregao($rsDadosBaixados);
                        $resultadoCarga .= 'Carregado: ' .  $rsDadosBaixados['cnpj'] . ' ( ' . $rsDadosBaixados['razao_social']  . ') <br/>';
                    } else {
                        // Cadastrar apenas a empresa a este novo pregão
                        $existe = $this->pegaLinha(" SELECT id FROM cargas.tb_carga_estabelecimento_pregao  WHERE cnpj = '{$rsDadosBaixados['cnpj']}' AND prgcod = $prgcod");
                        if (!$existe) {
                            $this->salvarCNPJPregao($rsDadosBaixados);
                        } else {
                            $resultadoCarga .= 'Vinculado ao pregão: ' .  $rsDadosBaixados['cnpj'] . ' ( ' . $rsDadosBaixados['razao_social']  . ') <br/>';
                        }
                        $resultadoCarga .= 'Já esta cadastrado neste pregão: ' .  $rsDadosBaixados['cnpj'] . ' ( ' . $rsDadosBaixados['razao_social']  . ') <br/>';
                    }
                    unset($rsDadosBaixados);
                }
            }
            return $resultadoCarga;
        } else {
            return 'Sem dados para Baixar neste Pregão: <b>' .  $prgcod . '</b>';
        }
    }
}
