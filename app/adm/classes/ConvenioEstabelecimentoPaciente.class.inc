<?php

class ConvenioEstabelecimentoPaciente extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_convenio_estabelecimento_paciente";

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
        'numero' => null,
        'validade' => null,
        'convenio_estabelecimento_id' => null,
        'pessoa_paciente_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function salvarConvenioPaciente($pessoaFisicaId, $convenioId, $numero, $validade = null)
    {
        $dados = $this->getByPessoaConvenio($pessoaFisicaId, $convenioId);

        $dados['convenio_estabelecimento_id'] = $convenioId;
        $dados['pessoa_paciente_id'] = $pessoaFisicaId;
        $dados['numero'] = $numero;
        $dados['validade'] = $validade ? formata_data_sql($validade) : null;
        $dados['excluido'] = 'f';

        list($params, $camposNulos) = $this->limparDadosVazios($dados);

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    public function getByPessoaConvenio($pessoaFisicaId, $convenioId)
    {
        return $this->recuperarLinha("id, numero, TO_CHAR(validade, 'DD/MM/YYYY') AS validade, convenio_estabelecimento_id, pessoa_paciente_id", [
            "convenio_estabelecimento_id = {$convenioId}",
            "pessoa_paciente_id = {$pessoaFisicaId}"
        ], false);
    }

    public function getConveniosByPessoa($pessoaFisicaId, $jsonEncode = false)
    {
        $sql = "SELECT
                    cep.id,
                    cep.numero,
                    TO_CHAR(cep.validade, 'DD/MM/YYYY') AS validade,
                    cep.convenio_estabelecimento_id,
                    cep.pessoa_paciente_id,
                    coe.nome AS convenio_plano,
                    pj.nome_fantasia AS estabelecimento,
                    est.id AS estabelecimento_id
                FROM
                    estabelecimentosaude.tb_convenio_estabelecimento_paciente cep
                INNER JOIN
                    estabelecimentosaude.tb_convenio_estabelecimento coe ON cep.convenio_estabelecimento_id = coe.id
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento est ON coe.estabelecimento_id = est.id
                INNER JOIN
                    corporativo.tb_pessoajuridica pj ON pj.id = est.pessoajuridica_id
                WHERE
                    cep.pessoa_paciente_id = {$pessoaFisicaId}
                    AND
                    cep.excluido IS FALSE
                    AND " . UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();

        $rs = $this->carregarArray($sql);

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    public function excluirPorPessoaFisica($id)
    {
        $sqlConvenioEstabelecimento = "SELECT id FROM estabelecimentosaude.tb_convenio_estabelecimento 
                                                 WHERE " . UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento('', 'estabelecimento_id');

        $this->executar("UPDATE estabelecimentosaude.tb_convenio_estabelecimento_paciente SET excluido = TRUE 
                                     WHERE convenio_estabelecimento_id IN ({$sqlConvenioEstabelecimento}) AND pessoa_paciente_id = {$id}");
    }
}