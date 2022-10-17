<?php

class Paginacao
{
    private $db;
    private $numRegistro;
    private $param;
    public static $identificador;

    private function __construct($db, $numRegistro)
    {
        self::$identificador++;
        self::preparaParam();
        $this->db = $db;
        $this->numRegistro = isset($_POST['limit']) ? $_POST['limit'] : $numRegistro;
    }

    public static function getPaginacao(cls_banco $db, $sql, $numRegistro = 30)
    {
        $pag = new Paginacao($db, $numRegistro);
        return $pag->paginar($sql);
    }

    private function paginar($sql)
    {
        // Pega TOTAL de registros do banco
        $sqlCount = self::preparaQueryCount($sql);
        $totalRegistro = $this->db->pegaUm($sqlCount);
        // Pega offset da paginação (de onde deve começar a lista)
        $offset = self::offsetPaginacao($totalRegistro);
        $numRegistro = $this->numRegistro;
        if ($this->param['ordem']) {
            $sql .= " ORDER BY " . $this->param['ordem'];
        }
        $sql .= " LIMIT {$numRegistro} OFFSET {$offset}";
        $dados = $this->db->carregar($sql);
        $dados = $dados ? $dados : array();
        $totalPagina = ceil($totalRegistro / $this->numRegistro);

        return array("total" => $totalRegistro,
            "totalPagina" => $totalPagina,
            "ordem" => explode(" ", $this->param['ordem']),
            "numRegistroInicialPagina" => $offset + 1,
            "numRegistroFinalPagina" => $offset + sizeof($dados),
            "pagina" => $this->param['pag'],
            "identificador" => self::$identificador,
            "dados" => ($dados ? $dados : array()));
    }

    private function preparaParam()
    {
        $this->param['ordem'] = $_POST['ordem'][self::$identificador] ? $_POST['ordem'][self::$identificador] : '';
        $this->param['pag'] = $_POST['pag'][self::$identificador] > 0 ? $_POST['pag'][self::$identificador] : 1;
        $this->param['offset'] = isset($_POST['offset']) && !empty($_POST['offset']) ? $_POST['offset'] : null;
    }

    private function preparaQueryCount($sql)
    {
        $sql = "SELECT 
					COUNT(f.*)
				FROM ({$sql}) AS f;";
        return $sql;
    }

    private function offsetPaginacao($totalRegistro)
    {
        $offSet = $this->param['offset'];

        if (is_null($offSet)) {
            $offSet = 0;
            $pag = ($this->param['pag'] - 1);
            if ($pag >= 0) {
                $pages = ceil($totalRegistro / $this->numRegistro);
                $pag = $pag > $pages ? $pages : $pag;
                $offSet = $pag * $this->numRegistro;
            }
        }

        return $offSet;
    }
}
?>