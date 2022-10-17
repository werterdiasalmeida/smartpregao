<?php

class ProntuarioCid extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "paciente.tb_prontuario_cid";

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
        'prontuario_id' => null,
        'codigo_cid' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    /**
     * @param $prontuarioId
     * @param array $arCids
     * @throws Exception
     */
    public function salvarProntuarioCid($prontuarioId, $arCids = array())
    {
        $this->desabilitaPorProntuario($prontuarioId);

        if(is_array($arCids) && count($arCids) > 0){
            foreach ($arCids AS $codigo){
                $dados = array(
                    'id' => $this->recuperarUm('id', array("prontuario_id = {$prontuarioId}", "codigo_cid = '{$codigo}'")),
                    'prontuario_id' => $prontuarioId,
                    'codigo_cid' => $codigo,
                    'excluido' => 'false'
                );

                $this->manter($dados);
                $this->setDadosNull();
            }
        }
    }

    /**
     * @param $prontuarioId
     * @return resource
     * @throws Exception
     */
    public function desabilitaPorProntuario($prontuarioId)
    {
        $sql = "UPDATE 
                    {$this->stNomeTabela}
                SET
                    excluido = true,
                    atualizado_em = now(),
                    atualizado_por = {$_SESSION['usuario_id']}
                WHERE
                    prontuario_id = {$prontuarioId} AND 
                    excluido IS FALSE;";

        return $this->executar($sql);
    }

    /**
     * @param $prontuarioId
     * @return resource
     * @throws Exception
     */
    public function getCidsPorProntuario($prontuarioId)
    {
        $sql = "SELECT
                  vw.codigo,
                  vw.codigo_formatado,
                  vw.descricao,
                  vw.descricao_abreviada
                FROM
                  paciente.tb_prontuario_cid pc
                  JOIN
                  cid.vw_cid10 vw ON vw.codigo = pc.codigo_cid
                WHERE
                  pc.prontuario_id = {$prontuarioId} AND
                pc.excluido IS FALSE
                ORDER BY
                1 ASC";

        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : [];
    }



    /**
     * @param $prontuarioId
     * @return resource
     * @throws Exception
     */
    public function getCodigosCidsPorProntuario($prontuarioId)
    {
        $sql = "SELECT
                  vw.codigo_formatado
                FROM
                  paciente.tb_prontuario_cid pc
                  JOIN
                  cid.vw_cid10 vw ON vw.codigo = pc.codigo_cid
                WHERE
                  pc.prontuario_id = {$prontuarioId} AND
                pc.excluido IS FALSE
                ORDER BY
                1 ASC";

        $retorno = $this->carregarColuna($sql);
        return $retorno ? $retorno : [];
    }
}