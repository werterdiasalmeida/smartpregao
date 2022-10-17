<?php

class EspecialidadeProfissional extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "profissionalsaude.tb_especialidade_profissional";

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
        'especialidade_id' => null,
        'profissional_id' => null,
        'rqe' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function excluirPorProfissional($profissionalId)
    {
        $sql = "UPDATE profissionalsaude.tb_especialidade_profissional SET excluido = TRUE WHERE profissional_id = {$profissionalId}";
        $this->executar($sql);
    }

    public function salvarEspecialidadeProfissional($dados, $camposNulos = array()){
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function listarPorProfissional($profissionalId, $jsonEncode = false, $estabelecimentoId = null)
    {
        $joinEstabelecimento = '';
        $colunasEstabelecimento = '';
        if($estabelecimentoId){
            $colunasEstabelecimento = ', eep.duracao_consulta';
            $joinEstabelecimento = "INNER JOIN
                                        estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id 
                                                                                                              AND eep.excluido IS FALSE
                                                                                                              AND eep.estabelecimento_id = {$estabelecimentoId}";
        }


        $sql = "SELECT
                    ep.id,
                    ep.especialidade_id,
                    ep.profissional_id,
                    ep.rqe,
                    e.descricao AS especialidade
                    {$colunasEstabelecimento}
                FROM
                    profissionalsaude.tb_especialidade_profissional ep
                JOIN
                    profissionalsaude.tb_especialidade e ON e.id = ep.especialidade_id AND e.excluido IS FALSE
                {$joinEstabelecimento}
                WHERE
                    profissional_id = {$profissionalId} AND
                    ep.excluido IS FALSE";

        $especialidades = $this->carregar($sql);
        $especialidades = $especialidades ? $especialidades : array();
        return $jsonEncode ? json_encode(utf8ArrayEncode($especialidades)) : $especialidades;
    }

    public function recuperarPorProfissional($profissionalId, $jsonEncode = false, $estabelecimentoId = null)
    {
        $joinEstabelecimento = '';
        $colunaSelecionadoEstabelecimento = '';
        if($estabelecimentoId){
            $colunaSelecionadoEstabelecimento = 'CASE WHEN eep.id IS NOT NULL THEN 1 ELSE 0 END AS selecionado, eep.duracao_consulta,';
            $joinEstabelecimento = "LEFT JOIN
                                        estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                                              AND eep.estabelecimento_id = {$estabelecimentoId}
                                                                                                              AND eep.excluido IS FALSE";
        }


        $sql = "SELECT
                    e.descricao AS especialidade,
                    {$colunaSelecionadoEstabelecimento}
                    ep.id,
                    e.id AS especialidade_id
                FROM
                    profissionalsaude.tb_especialidade e
                INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON e.id = ep.especialidade_id
                                                                      AND ep.excluido IS FALSE
                {$joinEstabelecimento}
                WHERE
                    ep.profissional_id = {$profissionalId}
                    AND
                    e.excluido IS FALSE
                ORDER BY
                    e.descricao";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();
        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }
}