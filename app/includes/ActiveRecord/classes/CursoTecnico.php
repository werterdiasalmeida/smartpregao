<?php
//
// $Id$
//

require_once "base/CursoTecnicoBase.php";

class CursoTecnico extends CursoTecnicoBase {
    public $areaCurso;


    public function carregarAreaCurso()
    {
        $this->areaCurso = new AreaCurso($this->areid);
    }
}





