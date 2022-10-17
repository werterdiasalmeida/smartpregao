<?php

class TrechoViagem extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "administrativo.trechoviagem";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "trvid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'trvid' 		   	=> null,
									  	'rqvid'   	     	=> null,
									  	'muncodorigem'      => null,
									  	'muncoddestino'     => null,
									  	'paiidorigem'       => null,
									  	'paiiddestino'      => null,
									  	'trvdatahora'       => null,
									  	'trvstatus'         => null
									  );

    /**
     *
     * Faz exclusão física pelo rqvid
     * @param $rqvid
     */
	public function deleteByRequisicaoViagem($rqvid){
	    $sql = "DELETE FROM {$this->stNomeTabela} WHERE rqvid = {$rqvid}";
	    $this->executar($sql);
	}

	public function getByRequisicaoViagem($rqvid){
        $sql = "SELECT
                    trv.trvid,
                    trv.muncodorigem,
                    muno.estuf AS estuforigem,
                    trv.muncoddestino,
                    mund.estuf AS estufdestino,
                    trv.paiidorigem,
                    trv.paiiddestino,
                    trv.trvdatahora,
                    to_char(trv.trvdatahora,'DD/MM/YYYY') AS data,
                    to_char(trv.trvdatahora,'HH24:MI')    AS hora,
                    CASE
                        WHEN
                            NOT trv.muncodorigem IS NULL AND NOT trv.muncoddestino IS NULL
                        THEN
                            'N'

                        WHEN
                            NOT trv.paiidorigem IS NULL AND NOT trv.paiiddestino IS NULL
                        THEN
                            'I'
                    END AS tipo
                FROM
                    {$this->stNomeTabela} trv
                LEFT JOIN
                    territorios.municipio muno ON trv.muncodorigem = muno.muncod
                LEFT JOIN
                    territorios.municipio mund ON trv.muncoddestino = mund.muncod
                WHERE
                    rqvid = {$rqvid}";
        $rs = $this->carregar($sql);

        return $rs;
	}


    public function getByRequisicaoViagemDetalhado($rqvid){
        $sql = "SELECT
                    CASE
                        WHEN
                            NOT trv.muncodorigem IS NULL AND NOT trv.muncoddestino IS NULL
                        THEN
                            'Nacional'

                        WHEN
                            NOT trv.paiidorigem IS NULL AND NOT trv.paiiddestino IS NULL
                        THEN
                            'Internacional'
                    END AS tipo,


                    CASE
                        WHEN
                            NOT trv.muncodorigem IS NULL AND NOT trv.muncoddestino IS NULL
                        THEN
                             muno.mundescricao ||' - ' ||muno.estuf

                        WHEN
                            NOT trv.paiidorigem IS NULL AND NOT trv.paiiddestino IS NULL
                        THEN
                            paio.paidescricao
                    END AS origem,


                    CASE
                        WHEN
                            NOT trv.muncodorigem IS NULL AND NOT trv.muncoddestino IS NULL
                        THEN
                             mund.mundescricao ||' - ' || mund.estuf

                        WHEN
                            NOT trv.paiidorigem IS NULL AND NOT trv.paiiddestino IS NULL
                        THEN
                            paid.paidescricao
                    END AS destino,

                    to_char(trv.trvdatahora,'DD/MM/YYYY') AS data,
                    to_char(trv.trvdatahora,'HH24:MI')    AS hora
                FROM
                    {$this->stNomeTabela} trv
                LEFT JOIN
                    territorios.municipio muno ON trv.muncodorigem = muno.muncod
                LEFT JOIN
                    territorios.municipio mund ON trv.muncoddestino = mund.muncod
                LEFT JOIN
                    territorios.pais paio ON trv.paiidorigem = paio.paiid
                LEFT JOIN
                    territorios.pais paid ON trv.paiiddestino = paid.paiid
                WHERE
                    rqvid = {$rqvid}";
        $rs = $this->carregar($sql);

        return $rs;
    }
}
