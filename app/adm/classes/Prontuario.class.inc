<?php

class Prontuario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "paciente.tb_prontuario";

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
        'descricao' => null,
        'tipo_prontuario_id' => null,
        'pessoa_paciente_id' => null,
        'agenda_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'receituario_especial' => null,
        'excluido' => null,
        'arqid' => null
    );

    /**
     * @param $params
     * @return bool|int
     * @throws Exception
     */
    public function salvarProntuario($params)
    {
        $tipoProntuarioId = null;
        if (isset($params['tipo_prontuario_id']) && !empty($params['tipo_prontuario_id'])) {
            $tipoProntuarioId = $params['tipo_prontuario_id'];
        } else if (isset($params['tipo']) && !empty($params['tipo'])) {
            $mTipo = new TipoProntuario();
            $tipo = $mTipo->getPorSigla($params['tipo']);
            $tipoProntuarioId = $tipo['id'];
        }

        $pacienteId = null;
        if (isset($params['pessoa_paciente_id']) && !empty($params['pessoa_paciente_id'])) {
            $pacienteId = $params['pessoa_paciente_id'];
        } else if (isset($params['agenda_id']) && !empty($params['agenda_id'])) {
            $mAgenda = new EstabelecimentoProfissionalAgenda($params['agenda_id']);
            $pacienteId = $mAgenda->pessoa_paciente_id;
        }

        $dados = array(
            'descricao'             => $params['descricao'],
            'receituario_especial'  => $params['receituario_especial'],
            'pessoa_paciente_id'    => $pacienteId,
            'tipo_prontuario_id'    => $tipoProntuarioId,
            'agenda_id'             => $params['agenda_id'],
        );

        if (!empty($params['modelo_documento_id'])) {
            $mModeloDocumento = new ModeloDocumento();
            $mModeloDocumento->adicionarUsoDoModelo($params['modelo_documento_id']);
        }

        $prontuarioId = $this->manter($params, $dados);

        $mAnexo = new AnexoProntuario();
        $mAnexo->salvarAnexos($params['arquivo_id'], $prontuarioId);

        $mProntuarioCid = new ProntuarioCid();
        $arCids = !empty($params['cids']) ? explode(',', $params['cids']) : array();
        $mProntuarioCid->salvarProntuarioCid($prontuarioId, $arCids);

        return $prontuarioId;
    }

    public function listarPorPessoaId($pessoaPacienteId, $limit = null, $offset = null, $arWhere = array())
    {
        $sqlLimit = $limit ? "LIMIT {$limit}" : '';
        $sqlOffset = $offset ? "OFFSET {$offset}" : '';

        $arWhere[] = "pron.excluido IS FALSE";
        $arWhere[] = "pron.pessoa_paciente_id = {$pessoaPacienteId}";

        $sql = "SELECT
                    pron.id,
                    pron.descricao,
                    tpo.descricao AS tipo_prontuario,
                    tpo.sigla AS tipo_sigla,
                    Coalesce((SELECT CASE
                       WHEN pes_prof.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                         THEN tp.pron_trat_masc || ' '
                       WHEN pes_prof.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                         THEN tp.pron_trat_fem || ' '
                       ELSE ''
                       END AS pronome
                    FROM profissionalsaude.tb_profissional prof
                      INNER JOIN profissionalsaude.tb_tipo_profissional tp
                        ON prof.tipo_profissional_id = tp.id
                    WHERE prof.pessoafisica_id = pes_prof.id AND prof.excluido IS FALSE
                    ORDER BY ordem_pronome
                    LIMIT 1),'') || COALESCE(pes_prof.nome_completo, usu_prof.usunome) AS nome_profissional,
                    COALESCE(pes_prof.sexo, usu_prof.ususexo) AS profissional_sexo,
                    TO_CHAR(COALESCE(pron.atualizado_em, pron.criado_em), 'DD/MM/YYYY HH24:MI:SS') AS criado_em_formatado,
                    TO_CHAR(epa.inicio, 'DD/MM/YYYY') AS data_agenda,
                    TO_CHAR(epa.inicio, 'HH24:MI') AS inicio_agenda,
                    TO_CHAR(epa.fim, 'HH24:MI') AS fim_agenda,
                    proc.descricao AS procedimento,
                    tco.descricao AS tipo_consulta,
                    pron.agenda_id,
                    pes_prof.foto_arquivo_id AS foto_profissional,
                    epa.id AS agenda_id,
                    (SELECT prof.id FROM profissionalsaude.tb_profissional prof WHERE prof.pessoafisica_id = pes_prof.id AND prof.excluido IS FALSE LIMIT 1) AS profissional_id,
                    TO_CHAR(epa.inicio, 'DD') AS dia_agenda,
                    TO_CHAR(epa.inicio, 'MM') AS mes_agenda,
                    TO_CHAR(epa.inicio, 'YYYY') AS ano_agenda,
                    vw_cid.cids,
                    pron.receituario_especial,
                    pron.arqid
                FROM
                    paciente.tb_prontuario pron
                INNER JOIN
                    paciente.tb_tipo_prontuario tpo ON pron.tipo_prontuario_id = tpo.id
                INNER JOIN
                    corporativo.tb_pessoafisica pes_pac ON pron.pessoa_paciente_id = pes_pac.id AND pes_pac.excluido IS FALSE
                INNER JOIN
                    seguranca.usuario usu_prof ON COALESCE(pron.atualizado_por, pron.criado_por) = usu_prof.id
                LEFT JOIN
                    corporativo.tb_pessoafisica pes_prof ON regexp_replace(pes_prof.cpf, '[\.-]+', '', 'g') = usu_prof.usucpf AND pes_prof.excluido IS FALSE
                LEFT JOIN 
                    estabelecimentosaude.tb_estabelecimento_profissional_agenda epa ON pron.agenda_id = epa.id
                LEFT JOIN 
                    estabelecimentosaude.tb_valor_procedimento vlr ON epa.valor_procedimento_id = vlr.id
                LEFT JOIN 
                    estabelecimentosaude.tb_procedimento proc ON proc.id = vlr.procedimento_id
                LEFT JOIN
                    estabelecimentosaude.tb_tipo_consulta tco ON epa.tipo_consulta_id = tco.id
                LEFT JOIN
                    (
                        SELECT
                          pc.prontuario_id,
                          string_agg(vw.codigo_formatado,', ') AS cids
                        FROM 
                          paciente.tb_prontuario_cid pc
                          JOIN
                          cid.vw_cid10 vw ON vw.codigo = pc.codigo_cid
                        WHERE
                          pc.excluido IS FALSE
                        GROUP BY
                          1
                    ) vw_cid ON vw_cid.prontuario_id = pron.id
                WHERE
                    " . implode(' AND ', $arWhere) . "
                ORDER BY
                    pron.atualizado_em DESC, pron.criado_em DESC
                {$sqlLimit}
                {$sqlOffset}";

        return $this->carregarArray($sql);
    }

    public function getTodosDados($prontuarioId)
    {
        $sql = "SELECT 
                    pron.id,
                    pron.descricao,
                    pron.tipo_prontuario_id,
                    pron.pessoa_paciente_id,
                    pron.agenda_id,
                    pron.criado_em,
                    pron.criado_por,
                    pron.atualizado_em,
                    pron.atualizado_por,
                    pron.excluido,
                    tpron.sigla,
                    eep.estabelecimento_id,
                    pron.receituario_especial
                FROM
                    paciente.tb_prontuario pron
                LEFT JOIN
                    estabelecimentosaude.tb_estabelecimento_profissional_agenda age ON pron.agenda_id = age.id
                LEFT JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON age.estabelecimento_especialidade_profissional_id = eep.id
                INNER JOIN
                    paciente.tb_tipo_prontuario tpron ON pron.tipo_prontuario_id = tpron.id
                WHERE
                    pron.id = {$prontuarioId}";

        return $this->pegaLinha($sql);
    }

    public function excluirProntuario($prontuarioId)
    {
        return $this->excluirLogicamente($prontuarioId);
    }

    public function salvarDadosAssinatura($prontuarioId, $arqId){
        $sql = "update paciente.tb_prontuario set arqid = " . $arqId . " where id = " .$prontuarioId .
            " RETURNING arqid ";
        return $this->pegaUm($sql);
    }
}