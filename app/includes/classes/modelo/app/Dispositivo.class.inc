<?php
class Dispositivo extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "app.dispositivo";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("disid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'disid'		             => null,
		'distoken'	             => null,
		'displataforma'	         => null,
		'disversaoplataforma'    => null,
		'disstatus'	             => null,
		'disdtcadastro'	         => null,
		'disidentificador'	     => null,
		'disipcadastro'	         => null,
		'appid'	                 => null
	);

    public function gerarRelatorioDispositivosRegistrados($jsonEncode = false) {
        $sql = "SELECT
                  lower(dis.displataforma) AS plataforma,
                  dis.disdtcadastro::date AS data,
                  TO_CHAR(dis.disdtcadastro::date, 'DD/MM/YYYY') AS data_formatada,
                  app.appnome AS nome,
                  COUNT(dis.disid) AS qtd
                FROM
                  app.dispositivo dis
                INNER JOIN
                  app.aplicativo app ON dis.appid = app.appid
                GROUP BY
                  1, 2, 3, 4
                ORDER BY
                  data ASC";

        $dados = $this->carregarArray($sql);

        $categorias = array_values(array_unique(array_map(function($linha) {
            return $linha['data_formatada'];
        }, $dados)));

        $dadosGrafico = [];
        foreach ($dados as $linha) {
            $descricao = "{$linha['nome']} - {$linha['plataforma']}";
            $key = sha1($descricao);
            if(!isset($dadosGrafico[$key])){
                $dadosGrafico[$key] = [
                    'name' => $descricao,
                    'data' => []
                ];

                foreach ($categorias as $data) {
                    $dadosGrafico[$key]['data'][$data] = 0;
                }
            }

            $dadosGrafico[$key]['data'][$linha['data_formatada']] = intval($linha['qtd']);
        }

        $dadosGrafico = array_values(array_map(function($linha) {
            $linha['data'] = array_values($linha['data']);
            return $linha;
        }, $dadosGrafico));

        return [
            'categorias' => $jsonEncode ? json_encode($categorias) : $categorias,
            'dados' => $jsonEncode ? json_encode($dadosGrafico) : $dadosGrafico
        ];
    }

    public function gerarRelatorioDispositivosPlataforma($jsonEncode = false) {
        $sql = "SELECT
                  lower(dis.displataforma) || ' ' || dis.disversaoplataforma AS plataforma,
                  app.appnome AS nome,
                  COUNT(dis.disid) AS qtd
                FROM
                  app.dispositivo dis
                INNER JOIN
                  app.aplicativo app ON dis.appid = app.appid
                WHERE
                  dis.disstatus = 'A'
                GROUP BY
                  1, 2
                ORDER BY
                  1";

        $dados = $this->carregarArray($sql);

        $categorias = array_values(array_unique(array_map(function($linha) {
            return $linha['nome'];
        }, $dados)));

        $dadosGrafico = [];
        foreach ($dados as $linha) {
            $descricao = $linha['plataforma'];
            $key = sha1($descricao);
            if(!isset($dadosGrafico[$key])){
                $dadosGrafico[$key] = [
                    'name' => $descricao,
                    'data' => []
                ];

                foreach ($categorias as $plataforma) {
                    $dadosGrafico[$key]['data'][$plataforma] = 0;
                }
            }

            $dadosGrafico[$key]['data'][$linha['nome']] = intval($linha['qtd']);
        }

        $dadosGrafico = array_values(array_map(function($linha) {
            $linha['data'] = array_values($linha['data']);
            return $linha;
        }, $dadosGrafico));

        return [
            'categorias' => $jsonEncode ? json_encode($categorias) : $categorias,
            'dados' => $jsonEncode ? json_encode($dadosGrafico) : $dadosGrafico
        ];
    }
}

