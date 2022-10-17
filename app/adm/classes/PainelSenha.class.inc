<?php

class PainelSenha extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_painel_senha";

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
        'id'                    => null,
        'data_validade'         => null,
        'codigo'                => null,
        'criado_em'             => null,
        'criado_por'            => null,
        'excluido'              => null,
        'estabelecimento_id'    => null,
        'tipo'                  => null
    );

    /**
     * @param string $codigo
     * @return false
     */
    public function getDadosByCodigo($codigo = "")
    {
        if (empty($codigo)) {
            return false;
        }

        $arWhere = array(
            "excluido IS FALSE",
            "data_validade > '" . (new DateTime())->format('Y-m-d H:i:s') . "'",
            "codigo = '{$codigo}'"
        );

        $sql   = "SELECT * FROM {$this->stNomeTabela} WHERE " . implode(" AND ", $arWhere) . " LIMIT 1;";
        $dados = $this->pegaLinha($sql);

        if (!$dados || empty($dados)) {
            return false;
        }

        $painelSenhaRegistro    = new PainelSenhaRegistro();
        $dados['registros']     = $painelSenhaRegistro->getUltimosByPainel($dados['id'], 5);

        $estabelecimentoProfissionalAgenda  = new EstabelecimentoProfissionalAgenda();
        $dados['atendimento']               = $estabelecimentoProfissionalAgenda->getDadosPainelFilaAtendimento($dados['estabelecimento_id'], date('Y-m-d'))[0];

        return $dados;
    }

    /**
     * @param $estabelecimentoId
     * @param string $tipo
     * @return array|false
     */
    public function getByEstabelecimento($estabelecimentoId, $tipo = 'R')
    {
        $arWhere = array(
            "excluido IS FALSE",
            "data_validade > '" . (new DateTime())->format('Y-m-d H:i:s') . "'",
            "estabelecimento_id = {$estabelecimentoId}",
            "tipo = '{$tipo}'"
        );

        $sql   = "SELECT * FROM {$this->stNomeTabela} WHERE " . implode(" AND ", $arWhere) . " LIMIT 1;";
        $dados = $this->pegaLinha($sql);
        return $dados ?: [];
    }

    /**
     * @param $estabelecimentoId
     * @return array|false
     */
    public function excluirLogicamenteByEstabelecimento($estabelecimentoId)
    {
        $arWhere = array(
            "excluido IS FALSE",
            "estabelecimento_id = {$estabelecimentoId}",
        );

        $sql   = "UPDATE {$this->stNomeTabela} SET excluido = TRUE WHERE " . implode(" AND ", $arWhere) . ";";
        return $this->executar($sql);
    }

    /**
     * Cria os paineis de senha do atendimento e da recepção para um estabelecimento
     *
     * @param bool $dados
     * @return bool|int
     */
    public function criarPainelSenha($dados)
    {
        $mHashSimec = new SimecHashids();

        $arDados = [
            'data_validade'       => (DateTime::createFromFormat('d/m/Y H:i', $dados['validade']))->format('Y-m-d H:i:s'),
            'estabelecimento_id'  => $dados['id'],
            'codigo'              => $dados['id'],
            'excluido'            => 'false'
        ];

        // Criando o painel da recepção
        $id = $this->manter(array_merge($arDados, ['tipo' => 'R']));

        // Atualizando o hash
        $painelRecepcao = $this->manter(['id' => $id, 'codigo' => $mHashSimec->encode($id, 6)]);

        // Limpando o objeto
        $this->setDadosNull();

        // Criando o painel do atendimento
        $id = $this->manter(array_merge($arDados, ['tipo' => 'A']));

        // Atualizando o hash
        $painelAtendimento = $this->manter(['id' => $id, 'codigo' => $mHashSimec->encode($id, 6)]);

        return $painelRecepcao;
    }
}