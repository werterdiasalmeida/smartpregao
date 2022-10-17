<?php

class TipoGuia extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_tipo_guia";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("id");

    const TIPO_GUIA_CONSULTA = 'consulta';
    const TIPO_GUIA_EXAME = 'exame';

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'id' => null,
        'descricao' => null,
        'tipo' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getIdByTipo($tipo)
    {
        return $this->recuperarUm('id', ["tipo = '{$tipo}'", "excluido IS FALSE"]);
    }

    public function getTipoById($id)
    {
        if(empty($id)) {
            return null;
        }

        return $this->recuperarUm('tipo', ["id = '{$id}'", "excluido IS FALSE"]);
    }

    public function getCombo()
    {
        return $this->recuperarTodos('id AS codigo, descricao', ["excluido IS FALSE"], 2);
    }
}