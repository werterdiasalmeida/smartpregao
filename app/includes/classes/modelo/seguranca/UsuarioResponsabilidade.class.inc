<?php

class UsuarioResponsabilidade extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.usuarioresponsabilidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("rpuid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'rpuid' => null,
        'pflcod' => null,
        'usucpf' => null,
        'rpustatus' => null,
        'rpudata_inc' => null,
        'estabelecimento_id' => null,
    );

    public function listarPerfisResponsabilidade($usucpf, $sisid)
    {
        $usucpf = str_replace(array('.', '-'), '', $usucpf);

        $sql = "SELECT
                    per.pflcod,
                    per.pfldsc,
                    array_to_string(array_agg(pj.nome_fantasia), ',') AS estabelecimentos,
                    ur.usucpf,
                    per.sisid
                FROM
                    seguranca.perfil per
                INNER JOIN
                    seguranca.tprperfil tpr ON per.pflcod = tpr.pflcod
                INNER JOIN
                    seguranca.perfilusuario pu ON per.pflcod = pu.pflcod
                                              AND pu.usucpf = '{$usucpf}'
                LEFT JOIN
                    seguranca.usuarioresponsabilidade ur ON per.pflcod = ur.pflcod
                                                        AND ur.rpustatus = 'A'
                                                        AND ur.usucpf = pu.usucpf
                LEFT JOIN
                    estabelecimento.tb_estabelecimento est ON ur.estabelecimento_id = est.id
                                                               AND est.excluido = FALSE
                LEFT JOIN 
                    corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                WHERE
                    per.pflstatus = 'A'
                    AND
                    per.sisid = '{$sisid}'
                GROUP BY
                    per.pflcod,
                    per.pfldescricao,
                    ur.usucpf,
                    per.sisid";

        $rs = $this->carregar($sql);
        return $rs ? $rs : array();
    }

    /**
     * @param $usucpf
     * @param $pflcod
     * @throws Exception
     */
    public function excluirPorUsuarioPerfil($usucpf, $pflcod)
    {
        $this->executar("UPDATE seguranca.usuarioresponsabilidade 
                                 SET rpustatus = 'I' 
                               WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod}");
    }
}