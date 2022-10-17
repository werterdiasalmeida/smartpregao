<?php

class AnexoProntuario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "paciente.tb_anexo_prontuario";

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
        'arquivo_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    /**
     * @param $arquivosId
     * @param $prontuarioId
     * @throws Exception
     */
    public function salvarAnexos($arquivosId, $prontuarioId)
    {
        $arquivosId = is_array($arquivosId) ? $arquivosId : array();
        $this->desabilitarPorProntuario($prontuarioId);

        foreach($arquivosId as $arquivoId){
            $dados = array(
                'id' => $this->recuperarUm('id', array("prontuario_id = {$prontuarioId}", "arquivo_id = {$arquivoId}")),
                'prontuario_id' => $prontuarioId,
                'arquivo_id' => $arquivoId,
                'excluido' => 'false'
            );

            $this->manter($dados, $dados);
            $this->setDadosNull();
        }
    }

    /**
     * @param $prontuarioId
     * @throws Exception
     */
    private function desabilitarPorProntuario($prontuarioId)
    {
        $this->executar("UPDATE paciente.tb_anexo_prontuario SET excluido = TRUE WHERE prontuario_id = {$prontuarioId}");
    }

    public function listarAgrupadosProntuarioPorPessoa($pessoaPacienteId, $prontuariosId = array())
    {
        $sqlProntuariosId = '';
        if(count($prontuariosId) > 0){
            $sqlProntuariosId = " AND pro.id IN (" . implode(', ', $prontuariosId) . ")";
        }

        $sql = "SELECT
                    anp.arquivo_id,
                    anp.prontuario_id,
                    arq.arqtamanho,
                    arq.arqextensao,
                    arq.arqtipo,
                    arq.arqnome
                FROM
                    paciente.tb_anexo_prontuario anp
                INNER JOIN
                    public.arquivo arq ON anp.arquivo_id = arq.arqid
                INNER JOIN
                    paciente.tb_prontuario pro ON anp.prontuario_id = pro.id
                WHERE
                    anp.excluido IS FALSE
                    AND
                    pro.pessoa_paciente_id = {$pessoaPacienteId}
                    {$sqlProntuariosId}";

        $rs = array();
        foreach($this->carregarArray($sql) as $anexo){
            if(!isset($rs[$anexo['prontuario_id']])){
                $rs[$anexo['prontuario_id']] = array();
            }

            $rs[$anexo['prontuario_id']][] = $anexo;
        }

        return $rs;
    }
}