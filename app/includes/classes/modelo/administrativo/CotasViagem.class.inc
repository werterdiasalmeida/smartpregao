<?php

class CotasViagem extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "administrativo.cotasviagem";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "covid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'covid' 		   => null,
									  	'entid' 		   => null,
									  	'covano' 		   => null,
									  	'covquantidade'    => null,
									  	'covvalor'         => null,
									  	'covstatus'        => null
									  );

   public function listarByWhere(array $where = array()){
       $where[] = "cov.covstatus = 'A'";

       $where = implode(' AND ', $where);

       $sql = "SELECT
                    ent.entnome,
                    cov.covano,
                    cov.covquantidade,
                    cov.covvalor,
                    cov.covid
               FROM
                    {$this->stNomeTabela} cov
               INNER JOIN
                    agenda.entidade ent ON ent.entid = cov.entid
               WHERE
                    {$where}";
        return $this->carregar($sql);
   }

}
