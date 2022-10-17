<?php

class ProfissionalEstabelecimentoProcedimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_profissional_estabelecimento_procedimento";

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
        'estabelecimento_id' => null,
        'profissional_id' => null,
        'procedimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function salvarProfissionalProcedimento(array $profissionaisId, $procedimentoId, $estabelecimentoId)
    {
        $this->excluirPorProcedimentoEstabelecimento($procedimentoId, $estabelecimentoId);

        foreach($profissionaisId as $profissionalId){
            $profissinalProcedimentoId = $this->recuperarUm('id', array(
                "estabelecimento_id = {$estabelecimentoId}",
                "profissional_id = {$profissionalId}",
                "procedimento_id = {$procedimentoId}"
            ));

            $this->manter(array(
                'id' => $profissinalProcedimentoId,
                'estabelecimento_id' => $estabelecimentoId,
                'profissional_id' => $profissionalId,
                'procedimento_id' => $procedimentoId,
                'excluido' => 'false'
            ));
            $this->setDadosNull();
        }
    }

    public function excluirPorProcedimentoEstabelecimento($procedimentoId, $estabelecimentoId)
    {
        $this->executar("UPDATE estabelecimentosaude.tb_profissional_estabelecimento_procedimento 
                                     SET excluido = TRUE WHERE procedimento_id = {$procedimentoId} 
                                                          AND estabelecimento_id = {$estabelecimentoId}");
    }

    public function getProfissionaisIdPorEstabelecimentoProcedimento($estabelecimentoId, $procedimentoId)
    {
        return $this->recuperarColuna('profissional_id', array("estabelecimento_id = {$estabelecimentoId}", "procedimento_id = {$procedimentoId}", "excluido IS FALSE"));
    }

    public function getQtdProfissionaisProcedimentoPorEstabelecimento($estabelecimentoId)
    {
        $sql = "SELECT
                    count(DISTINCT pep.profissional_id) AS qtd, 
                    pep.procedimento_id
                FROM
                    estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep
                INNER JOIN
                    profissionalsaude.tb_profissional pro ON pep.profissional_id = pro.id
                                                         AND pep.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON pro.id = ep.profissional_id
                                                                      AND ep.excluido IS FALSE
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional esp ON ep.id = esp.especialidade_profissional_id
                                                                                          AND esp.excluido IS FALSE
                                                                                          AND esp.estabelecimento_id = pep.estabelecimento_id
                WHERE
                   pep.excluido IS FALSE
                   AND
                   pep.estabelecimento_id = {$estabelecimentoId}
               GROUP BY
                   pep.procedimento_id";

        return $this->carregarArray($sql);
    }

    public function getQtdProfissionaisProcedimentoPorEstabelecimentoAgrupado($estabelecimentoId)
    {
        $qtdsProfissionais = $this->getQtdProfissionaisProcedimentoPorEstabelecimento($estabelecimentoId);
        $qtdsProfissionaisAgrupados = array();
        foreach($qtdsProfissionais as $qtdProfissional){
            $qtdsProfissionaisAgrupados[$qtdProfissional['procedimento_id']] = $qtdProfissional['qtd'];
        }

        return $qtdsProfissionaisAgrupados;
    }

    public function getProfissionaisProcedimentoPorEstabelecimento($estabelecimentoId)
    {
        $sql = "SELECT
                    CASE 
                      WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                      THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                      WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                      THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                      ELSE pes.nome_completo
                      END AS nome_completo,
                    tp.descricao AS tipo,
                    pep.profissional_id, 
                    pep.procedimento_id
                FROM
                    estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep
                INNER JOIN
                    profissionalsaude.tb_profissional pro ON pep.profissional_id = pro.id
                                                         AND pep.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON pro.id = ep.profissional_id
                                                                      AND ep.excluido IS FALSE
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional esp ON ep.id = esp.especialidade_profissional_id
                                                                                          AND esp.excluido IS FALSE
                                                                                          AND esp.estabelecimento_id = pep.estabelecimento_id
                INNER JOIN
                    corporativo.tb_pessoafisica pes ON pro.pessoafisica_id = pes.id
                INNER JOIN
                    profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                WHERE
                   pep.excluido IS FALSE
                   AND
                   pep.estabelecimento_id = {$estabelecimentoId}
               ORDER BY
                   nome_completo";

        return $this->carregarArray($sql);
    }

    public function getProfissionaisProcedimentoPorEstabelecimentoAgrupado($estabelecimentoId)
    {
        $profissionais = $this->getProfissionaisProcedimentoPorEstabelecimento($estabelecimentoId);

        $profissionaisAgrupados = array();
        foreach($profissionais as $profissional){
            $profissionaisAgrupados[$profissional['procedimento_id']] = $profissional;
        }

        return $profissionaisAgrupados;
    }

    public function getComboProcedimentosPorProfissionalEstabelecimento($estabelecimentoId, $profissionalId)
    {
        $sql = "SELECT
                    pep.id,
                    proc.descricao
                FROM
                    estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep
                INNER JOIN
                    profissionalsaude.tb_profissional pro ON pep.profissional_id = pro.id
                                                         AND pep.excluido IS FALSE
                INNER JOIN
                    corporativo.tb_pessoafisica pes ON pro.pessoafisica_id = pes.id
                INNER JOIN
                    estabelecimentosaude.tb_procedimento proc ON pep.procedimento_id = proc.id
                                                             AND proc.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                WHERE
                   pep.excluido IS FALSE
                   AND
                   pep.estabelecimento_id = {$estabelecimentoId}
                   AND
                   pep.profissional_id = {$profissionalId}
               ORDER BY
                   nome_completo";

        $rs = $this->carregarArray($sql);
        return array_map(function($row){
            return array(
                'codigo' => $row['id'],
                'descricao' => $row['descricao']
            );
        }, $rs);
    }

    public function copiarDados($origem,$destino,$estabelecimento_id)
    {
        $sql = "SELECT
                 pep.*
               FROM
                 estabelecimentosaude.tb_profissional_estabelecimento_procedimento pep
               WHERE
                 pep.excluido IS FALSE AND
                 pep.procedimento_id = '{$origem}';";
        $dados = $this->carregar($sql);

        //recuperar os profissionais do estebelecimento
        $mEstabelecimentoEspecialidadeProfissional = new EstabelecimentoEspecialidadeProfissional();
        $arIdsProfissionais = $mEstabelecimentoEspecialidadeProfissional->getIdsProfissionaisPorEstabelecimento($estabelecimento_id);

        $arReferenciaChaves = array();
        if (is_array($dados) && count($dados) > 0) {
            foreach ($dados as $index => $item) {
                if(in_array($item['profissional_id'],$arIdsProfissionais)){
                    $arReferenciaChaves[$item['id']] = $this->manter(array(
                        'procedimento_id' => $destino,
                        'estabelecimento_id' => $estabelecimento_id,
                        'profissional_id' => $item['profissional_id'],
                        'criado_em' => 'now()',
                        'criado_por' => $_SESSION['usuario_id'],
                        'excluido' => 'false'
                    ));
                    $this->setDadosNull();
                }
            }
        }

        return $arReferenciaChaves;
    }

    public function getIdByEstabelecimentoProcedimentoProfissional($estabelecimentoId, $procedimentoId, $profissionalId)
    {
        return $this->recuperarUm('id', array(
            "estabelecimento_id = {$estabelecimentoId}",
            "procedimento_id = {$procedimentoId}",
            "profissional_id = {$profissionalId}",
            "excluido IS FALSE"
        ));
    }
}