<?php

class FilaParticipante extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_fila_participante";

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
        'fila_id' => null,
        'tipo_documento_id' => null,
        'documento' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getPorFila($filaId)
    {
        return $this->recuperarTodos('id, tipo_documento_id, documento', array("fila_id = {$filaId}", "excluido IS FALSE"));
    }

    public function salvarParticipante($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $dados = array(
            'id' => $params['id'],
            'fila_id' => $params['fila_id'],
            'tipo_documento_id' => $params['tipo_documento_id'],
            'documento' => $params['documento'],
            'excluido' => 'false',
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    /**
     * @param $filaId
     * @throws Exception
     */
    public function excluirParticipantePorFila($filaId)
    {
        $sql = "UPDATE estabelecimentosaude.tb_fila_participante SET excluido = TRUE WHERE fila_id = {$filaId}";
        $this->executar($sql);
    }
}