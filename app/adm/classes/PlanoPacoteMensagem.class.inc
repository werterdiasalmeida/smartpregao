<?php

class PlanoPacoteMensagem extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_plano_pacote_mensagem";

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
        'tipo_mensagem_id'      => null,
        'valor'                 => null,
        'quantidade_mensagens'  => null,
        'descricao'             => null,
        'criado_em'             => null,
        'criado_por'            => null,
        'atualizado_em'         => null,
        'atualizado_por'        => null,
        'excluido'              => null
    );

    /**
     * Retorna os registros para serem utilizados em combobox por tipo
     * @param $tipo
     * @return array
     */
    public function getComboPacotesMensagemByTipo($tipo)
    {
        $sql = "SELECT
                    ppm.id AS codigo,
                    ppm.descricao AS descricao
                FROM
                    {$this->stNomeTabela} ppm
                INNER JOIN
                    mensageria.tb_tipo_mensagem tm ON tm.id = ppm.tipo_mensagem_id
                WHERE
                    ppm.excluido IS FALSE AND
                    tm.codigo = '{$tipo}' AND
                    quantidade_mensagens IS NOT NULL
                ORDER BY
                    ppm.valor ASC";

        $dados = $this->carregar($sql);
        return $dados ? $dados : array();
    }
}