<?php

class RequisicaoViagem extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "administrativo.requisicaoviagem";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rqvid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'rqvid' 		   => null,
									  	'rqvmotivo'        => null,
									  	'tppid'            => null,
									  	'entid'            => null,
									  	'entiddemandante'  => null,
									  	'entidcoordenacao' => null,
									  	'entidsecretaria'  => null,
                                        'rqvscpd'          => null,
                                        'rqvvalor'         => null,
									  	'rqvjustificativa' => null,
									  	'rqvdatacadastro'  => null,
									  	'usucpf'           => null,
									  	'docid'            => null,
									  	'rqvstatus'        => null
									  );

	public function listarByWhere(array $where = array()){
        $where[] = " rqv.rqvstatus = 'A' ";

        $where = implode(" AND ", $where);

        $sql = "SELECT
                    rqv.rqvid,
                    ent4.entnome AS secretariademandante,
                    tpp.tppdsc,
                    ent1.entnome,
                    rqv.rqvmotivo,
                    (SELECT
                        to_char(MIN (trvdatahora), 'DD/MM/YYYY às HH24:MI')
                     FROM
                        administrativo.trechoviagem tr
                     WHERE
                        tr.rqvid = rqv.rqvid
                    )AS dataida,
                    (SELECT
                        to_char(MAX (trvdatahora), 'DD/MM/YYYY às HH24:MI')
                     FROM
                        administrativo.trechoviagem tr
                     WHERE
                        tr.rqvid = rqv.rqvid
                    ) AS datavolta,
                    rqv.rqvid,
                    (SELECT
                        COUNT(tr.rqvid)
                     FROM
                        administrativo.trechoviagem tr
                     WHERE
                        tr.rqvid = rqv.rqvid
                    ) AS qtdtrechos,
                    ARRAY_TO_STRING(ARRAY( SELECT
                                    CASE
                                        WHEN NOT trv.muncodorigem IS NULL AND NOT trv.muncoddestino IS NULL THEN
                                            muno.mundescricao||'-'||muno.estuf||'/'||mund.mundescricao||'-'||mund.estuf
                                        WHEN NOT trv.paiidorigem IS NULL AND NOT trv.paiiddestino IS NULL THEN
                                            paio.paidescricao||'/'||paid.paidescricao
                                    END AS descricaotrecho
                                FROM
                                   administrativo.trechoviagem trv
                                LEFT JOIN territorios.municipio AS muno on trv.muncodorigem  = muno.muncod
                                LEFT JOIN territorios.municipio AS mund on trv.muncoddestino = mund.muncod
                                LEFT JOIN territorios.pais      AS paio on trv.paiidorigem   = paio.paiid
                                LEFT JOIN territorios.pais      AS paid on trv.paiiddestino  = paid.paiid
                                WHERE
                                    trv.rqvid = rqv.rqvid
                                 ), ' - ') AS descricaotrecho,
                    wfe.esddsc,
                    rqv.rqvjustificativa,
                    ent2.entnome AS entcoordenacao,
                    rqv.entiddemandante
                FROM
                    administrativo.requisicaoviagem rqv
                LEFT JOIN
                    agenda.entidade ent1 ON ent1.entid = rqv.entid
                LEFT JOIN
                    agenda.entidade ent2 ON ent2.entid = rqv.entidcoordenacao
                LEFT JOIN
                    agenda.entidade ent3 ON ent3.entid = rqv.entidsecretaria
                LEFT JOIN
                    administrativo.tipoproposto tpp ON tpp.tppid = rqv.tppid
                LEFT JOIN
                	administrativo.trechoviagem trv ON trv.rqvid = rqv.rqvid
                INNER JOIN
                    agenda.entidade ent4 ON ent4.entid = rqv.entiddemandante
                JOIN
                	workflow.documento wfd ON wfd.docid = rqv.docid
                JOIN
                	workflow.estadodocumento wfe ON wfe.esdid = wfd.esdid

                WHERE
                    {$where}";
//		die($sql);
        return $this->carregar($sql);
    }


    public function getById($rqvid){

        $sql = "SELECT
                    rqv.rqvid,
                    rqv.rqvmotivo,
                    rqv.tppid,
                    ent1.entid,
                    rqv.entiddemandante,
                    ent2.entid   AS entidcoordenacao,
                    ent3.entid   AS entidsecretaria,
                    rqv.rqvjustificativa,
                    rqv.rqvscpd,
                    rqv.rqvvalor,
                    rqv.docid,
                    rqv.rqvstatus,
                    tpp.tppdsc,
                    ent1.entnome,
                    ent2.entnome AS entcoordenacao,
                    ent3.entnome AS entsecretaria,
                    ent4.entnome AS entdemandante,
                    doc.esdid
                FROM
                    administrativo.requisicaoviagem rqv
                LEFT JOIN
                    agenda.entidade ent1 ON ent1.entid = rqv.entid
                LEFT JOIN
                    agenda.entidade ent2 ON ent2.entid = rqv.entidcoordenacao
                LEFT JOIN
                    agenda.entidade ent3 ON ent3.entid = rqv.entidsecretaria
                INNER JOIN
                    agenda.entidade ent4 ON ent4.entid = rqv.entiddemandante
                LEFT JOIN
                    administrativo.tipoproposto tpp ON tpp.tppid = rqv.tppid
                LEFT JOIN
                    workflow.documento doc ON doc.docid = rqv.docid
                WHERE
                    rqv.rqvid = {$rqvid} AND rqv.rqvstatus = 'A'";

        return $this->pegaLinha($sql);
    }
}
