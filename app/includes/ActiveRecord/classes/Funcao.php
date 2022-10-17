<?php
//
// $Id$
//

require_once "base/FuncaoBase.php";

class Funcao extends FuncaoBase {
    /**
     * 
     */
    public function toJson()
    {
        $arr = array();

        foreach ($this->carregarColecao() as $funcao) {
            $arr[] = '{funid:\''    . addslashes($funcao->getPrimaryKey()) . '\','
                   . 'fundsc:\''    . addslashes($funcao->fundsc         ) . '\','
                   . 'funtipo:\''   . addslashes($funcao->funtipo        ) . '\','
                   . 'funstatus:\'' . addslashes($funcao->funstatus      ) . '\'}';
        }

        return "[" . implode(",", $arr) . "]";
    }
}





