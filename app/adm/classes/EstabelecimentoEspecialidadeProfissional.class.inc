<?php

class EstabelecimentoEspecialidadeProfissional extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_estabelecimento_especialidade_profissional";

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
        'especialidade_profissional_id' => null,
        'duracao_consulta' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getList($arWhere = array(), $paginacao = true, $order = null, $retornarExcluidosComAgenda = false)
    {
        $arWhere[] = "pes.excluido IS FALSE";

        $sql = "SELECT DISTINCT
                  CASE 
                  WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                  THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                  WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                  THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                  ELSE pes.nome_completo
                  END AS nome_completo,
                  tp.descricao AS tipo_profissional,
                  p.registro_profissional || '/' || p.uf_registro AS registro,
                  array_to_string(array_agg(DISTINCT esp.descricao), ', ') AS especialidades,
                  pes.sexo,
                  pes.cpf,
                  p.curriculo,
                  pes.foto_arquivo_id,
                  pes.id AS pessoa_id,
                  p.id AS profissional_id,
                  tp.id AS tipo_profissional_id,
                  est.id AS estabelecimento_id,
                  eep.excluido AS estabelecimento_especialidade_profissional_excluido
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id 
                                                     AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id 
                                                           AND tp.excluido IS FALSE
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON p.id = ep.profissional_id
                                                                     AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id
                                                        AND esp.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                        " . ($retornarExcluidosComAgenda ? "AND (eep.excluido IS FALSE OR (eep.excluido IS TRUE AND eep.id IN (SELECT 
                                                                                                                                                                                                   estabelecimento_especialidade_profissional_id 
                                                                                                                                                                                               FROM 
                                                                                                                                                                                                   estabelecimentosaude.tb_estabelecimento_profissional_agenda 
                                                                                                                                                                                               WHERE 
                                                                                                                                                                                                    excluido IS FALSE)))"
                                                                                                                         : "AND eep.excluido IS FALSE") . "
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                                                             AND est.excluido IS FALSE
                WHERE
                  " . implode(' AND ', $arWhere) . "
                GROUP BY
                  pes.nome_completo,
                  p.registro_profissional,
                  p.uf_registro,
                  tp.descricao,
                  pes.id,
                  p.id,
                  tp.id,
                  est.id,
                  eep.excluido";

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY nome_completo ASC, tp.descricao";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 20);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function getListProfissionais($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "pes.excluido IS FALSE";

        $sql = "SELECT DISTINCT
                    CASE 
                      WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                      THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                      WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                      THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                      ELSE pes.nome_completo
                      END AS nome_completo,
                    tp.descricao AS tipo_profissional,
                    p.registro_profissional || '/' || p.uf_registro AS registro,
                    pes.sexo,
                    p.curriculo,
                    pes.foto_arquivo_id,
                    pes.id AS pessoa_id,
                    p.id AS profissional_id,
                    tp.id AS tipo_profissional_id,
                    est.id AS estabelecimento_id
                FROM
                    corporativo.tb_pessoafisica pes
                INNER JOIN
                    profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id 
                                                       AND p.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id 
                                                             AND tp.excluido IS FALSE
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON p.id = ep.profissional_id
                                                                     AND ep.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id
                                                          AND esp.excluido IS FALSE
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                          AND eep.excluido IS FALSE
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                                                               AND est.excluido IS FALSE
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY nome_completo ASC, tp.descricao";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 20);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function salvarEspecialidades($params)
    {
        $this->excluirPorProfissionalEstabelecimento($params['profissional_id'], $params['estabelecimento_id']);

        if(!is_array($params['especialidades'])){
            return;
        }

        foreach($params['especialidades'] as $especialidade){
            if(empty($especialidade) || !isset($especialidade['id'])
            || !isset($especialidade['duracao_consulta'])){
                continue;
            }

            $this->salvarEspecialidade(array(
                'estabelecimento_id' => $params['estabelecimento_id'],
                'especialidade_profissional_id' => $especialidade['id'],
                'duracao_consulta' => $especialidade['duracao_consulta'],
            ));
            $this->setDadosNull();
        }
    }

    public function salvarEspecialidade($params)
    {
        $camposNulos = array();
        foreach ($params as $campo => $valor){
            if($valor === ''){
                $params[$campo] = null;
                $camposNulos[] = $campo;
            }
        }

        $params['id'] = $params['id'] ? $params['id'] : $this->recuperarUm('id', array(
            "estabelecimento_id = {$params['estabelecimento_id']}",
            "especialidade_profissional_id = {$params['especialidade_profissional_id']}"
        ));

        $dados = array(
            'id' => $params['id'],
            'estabelecimento_id' => $params['estabelecimento_id'],
            'especialidade_profissional_id' => $params['especialidade_profissional_id'],
            'duracao_consulta' => $params['duracao_consulta'],
            'excluido' => 'f'
        );

        if(empty($params['id'])){
            $dadosCarimbo = array(
                'criado_em' => 'now()',
                'criado_por' => $_SESSION['usuario_id'],
            );
        }else{
            $dadosCarimbo = array(
                'atualizado_em' => 'now()',
                'atualizado_por' => $_SESSION['usuario_id'],
            );
        }

        $dados = array_merge($dados, $dadosCarimbo);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function excluirPorProfissionalEstabelecimento($profissionalId, $estabelecimentoId)
    {
        $sql = "UPDATE 
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional 
                SET 
                  excluido = TRUE,
                  atualizado_em = now(),
                  atualizado_por = {$_SESSION['usuario_id']}
                WHERE estabelecimento_id = {$estabelecimentoId} 
                  AND especialidade_profissional_id IN (
                    SELECT 
                      id 
                    FROM 
                      profissionalsaude.tb_especialidade_profissional 
                    WHERE 
                      profissional_id = {$profissionalId}
                  )";

        $this->executar($sql);
    }

    public function excluirPorPessoaFisicaEstabelecimento($pessoaFisicaId, $estabelecimentoId)
    {
        $sql = "UPDATE 
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional 
                SET 
                  excluido = TRUE,
                  atualizado_em = now(),
                  atualizado_por = {$_SESSION['usuario_id']}
                WHERE estabelecimento_id = {$estabelecimentoId} 
                  AND especialidade_profissional_id IN (
                    SELECT 
                      ep.id 
                    FROM 
                      profissionalsaude.tb_especialidade_profissional ep
                    JOIN
                      profissionalsaude.tb_profissional p ON p.id = ep.profissional_id
                    WHERE 
                      p.pessoafisica_id = {$pessoaFisicaId}
                  )";

        $this->executar($sql);
    }

    public function excluirPorEstabelecimento($estabelecimentoId)
    {
        $sql = "UPDATE 
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional 
                SET 
                  excluido = TRUE,
                  atualizado_em = now(),
                  atualizado_por = {$_SESSION['usuario_id']}
                WHERE estabelecimento_id = {$estabelecimentoId};";

        $this->executar($sql);
    }

    public function ativarPorProfissionalEstabelecimento($profissionalId, $estabelecimentoId)
    {
        $sql = "UPDATE 
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional 
                SET 
                  excluido = FALSE,
                  atualizado_em = now(),
                  atualizado_por = {$_SESSION['usuario_id']}
                WHERE estabelecimento_id = {$estabelecimentoId} 
                  AND especialidade_profissional_id IN (
                    SELECT 
                      id 
                    FROM 
                      profissionalsaude.tb_especialidade_profissional 
                    WHERE 
                      profissional_id = {$profissionalId}
                  )";

        $this->executar($sql);
    }

    public function getComboProfissionaisPorEstabelecimento($estabelecimentoId = null, $jsonEncode = false, $descricao = null)
    {
        if($estabelecimentoId){
            $whereEstabelecimento = " AND
                  est.id = {$estabelecimentoId}";
        }else{
            $whereEstabelecimento = " AND ".UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();
        }

        $descricao = $descricao ? $descricao : "CASE 
                  WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                  THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                  WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                  THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                  ELSE pes.nome_completo
                  END || ' (' || tp.descricao  || ')'";

        $sql = "SELECT DISTINCT
                  {$descricao} AS descricao,
                  p.id AS codigo,
                  pes.foto_arquivo_id
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id 
                                                     AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON p.id = ep.profissional_id
                                                                     AND ep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                                                             AND est.excluido IS FALSE
                WHERE
                  pes.excluido IS FALSE
                  {$whereEstabelecimento}
                ORDER BY
                  1";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }


    public function getIdsProfissionaisPorEstabelecimento($estabelecimentoId)
    {
        $sql = "SELECT DISTINCT 
                  p.id 
                FROM
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id
                                                                         AND ep.excluido IS FALSE
                INNER JOIN
                    profissionalsaude.tb_profissional p ON p.id = ep.profissional_id AND 
                                                          p.excluido IS FALSE
                WHERE
                  eep.excluido IS FALSE AND
                  eep.estabelecimento_id = {$estabelecimentoId}";

        $rs = $this->carregarColuna($sql);
        return $rs ? $rs : array();
    }

    public function getComboPessoasProfissionaisPorEstabelecimento($estabelecimentoId, $jsonEncode = false)
    {
        $sql = "SELECT DISTINCT 
                  CASE 
                  WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                  THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                  WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                  THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                  ELSE pes.nome_completo
                  END AS descricao,
                  pes.id AS codigo,
                  pes.foto_arquivo_id,
                  pes.sexo,
                  tp.ordem_pronome
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id 
                                                     AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id 
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON p.id = ep.profissional_id
                                                                     AND ep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                                                             AND est.excluido IS FALSE
                WHERE
                  pes.excluido IS FALSE
                  AND
                  est.id = {$estabelecimentoId}
                ORDER BY
                  ordem_pronome, 1";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    public function getComboPessoasProfissionaisByWhere($arWhere, $jsonEncode = false)
    {
        $arWhere[] = "pes.excluido IS FALSE";
        $arWhere[] = "es.excluido IS FALSE";

        $sql = "SELECT DISTINCT 
                  CASE 
                  WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                  THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                  WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                  THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                  ELSE pes.nome_completo
                  END AS descricao,
                  pes.id AS codigo,
                  pes.foto_arquivo_id,
                  pes.sexo,
                  tp.ordem_pronome
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id 
                                                     AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON p.id = ep.profissional_id
                                                                     AND ep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento es ON eep.estabelecimento_id = es.id
                                                             AND es.excluido IS FALSE
                WHERE
                  " . implode(' AND ', $arWhere) . "
                ORDER BY
                  ordem_pronome, 1";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    public function getEspecialidadesPorProfissionalEstabelecimento($profissionalId, $estabelecimentoId)
    {
        $sql = "SELECT DISTINCT
                  esp.descricao AS especialidade_descricao,
                  eep.id AS estabelecimento_especialidade_profissional_id,
                  tp.descricao AS tipo_profissional_descricao,
                  eep.duracao_consulta,
                  tp.id AS tipo_profissional_id
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id 
                                                     AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id 
                                                           AND tp.excluido IS FALSE
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON p.id = ep.profissional_id
                                                                     AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade esp ON esp.id = ep.especialidade_id
                                                        AND esp.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                                                             AND est.excluido IS FALSE
                WHERE
                  pes.excluido IS FALSE
                  AND
                  est.id = {$estabelecimentoId}
                  AND
                  p.id = {$profissionalId}";

        $rs = $this->carregar($sql);
        return $rs ? $rs : array();
    }

    public function getComboEspecialidadesPorProfissionalEstabelecimento($profissionalId, $estabelecimentoId)
    {
        $especialidades = $this->getEspecialidadesPorProfissionalEstabelecimento($profissionalId, $estabelecimentoId);
        $especialidades = array_map(function($especialidade){
            return array(
                'descricao' => $especialidade['especialidade_descricao'],
                'codigo' => $especialidade['estabelecimento_especialidade_profissional_id'],
            );
        }, $especialidades);


        return $especialidades;
    }

    public function getComboEspecialidadesPorEstabelecimento($estabelecimentoId, $jsonEncode = false)
    {
        $sql = "SELECT DISTINCT
                  e.descricao AS descricao,
                  e.id AS codigo
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id 
                                                     AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON p.id = ep.profissional_id
                                                                     AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade e ON ep.especialidade_id = e.id
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                                                             AND est.excluido IS FALSE
                WHERE
                  pes.excluido IS FALSE
                  AND
                  est.id = {$estabelecimentoId}
                ORDER BY 
                  1";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }



    public function getComboEspecialidadesByWhere($arWhere, $jsonEncode = false)
    {
        $arWhere[] = "pes.excluido IS FALSE";
        $arWhere[] = "es.excluido IS FALSE";

        $sql = "SELECT DISTINCT
                  e.descricao AS descricao,
                  e.id AS codigo
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id 
                                                     AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id
                INNER JOIN
                   profissionalsaude.tb_especialidade_profissional ep ON p.id = ep.profissional_id
                                                                     AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade e ON ep.especialidade_id = e.id
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento es ON eep.estabelecimento_id = es.id
                                                             AND es.excluido IS FALSE
                WHERE
                  " . implode(' AND ', $arWhere) . "
                ORDER BY 
                  1";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    public function getPorEspecialidadeProfissionalEstabelecimento($especialidadeProfissionalId, $estabelecimentoId)
    {
        return $this->recuperarLinha('*', array("estabelecimento_id = {$estabelecimentoId}", "especialidade_profissional_id = {$especialidadeProfissionalId}"), false);
    }


    public function copiarDados($origem,$destino, $desabilita = false)
    {
        if($desabilita){
            $this->excluirPorEstabelecimento($destino);
        }

        $sql = "SELECT
                 eep.*
               FROM
                 estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep
               INNER JOIN
                 profissionalsaude.tb_especialidade_profissional ep ON ep.id = eep.especialidade_profissional_id AND ep.excluido IS FALSE
               INNER JOIN
                 profissionalsaude.tb_profissional p ON p.id = ep.profissional_id AND p.excluido IS FALSE
               INNER JOIN
                 corporativo.tb_pessoafisica pf ON pf.id = p.pessoafisica_id AND pf.excluido IS FALSE
               WHERE
                 eep.excluido IS FALSE AND
                 estabelecimento_id = '{$origem}';";

        $dados = $this->carregar($sql);

        $arReferenciaChaves = array();
        if (is_array($dados) && count($dados) > 0) {
            foreach ($dados as $index => $item) {
                $id = $this->recuperarUm('id', array(
                    "estabelecimento_id = {$destino}",
                    "especialidade_profissional_id = {$item['especialidade_profissional_id']}"
                ));

                $arReferenciaChaves[$item['id']] = $this->manter(array(
                    'id' => $id,
                    'estabelecimento_id' => $destino,
                    'especialidade_profissional_id' => $item['especialidade_profissional_id'],
                    'criado_em' => 'now()',
                    'criado_por' => $_SESSION['usuario_id'],
                    'excluido' => 'false',
                    'duracao_consulta' => $item['duracao_consulta']
                ));
                $this->setDadosNull();
            }
        }

       return $arReferenciaChaves;
    }
}