<?php

class TagPaciente extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "paciente.tb_tag_paciente";

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
        'pessoa_paciente_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function getTagsPorPaciente($pessoaPacienteId)
    {
        return $this->recuperarTodos('*', array(
            "excluido IS FALSE",
            "pessoa_paciente_id = {$pessoaPacienteId}"
        ));
    }

    public function getStrTagsPorPaciente($pessoaPacienteId, $espaco = '')
    {
        return implode(',' . $espaco, array_map(function($tag){
            return $tag['descricao'];
        }, $this->getTagsPorPaciente($pessoaPacienteId)));
    }

    public function getBadgeTagsPorPaciente($pessoaPacienteId, $espaco = '')
    {
        $strRetorno = '<span class="label label-default">';

        $strRetorno .= implode('</span>  <span class="label label-default">' . $espaco, array_map(function($tag){
            return $tag['descricao'];
        }, $this->getTagsPorPaciente($pessoaPacienteId)));
        $strRetorno .= '</span>';

        return $strRetorno;
    }

    /**
     * @param $descricaoTag
     * @param $pessoaPacienteId
     * @return bool|int
     */
    public function adicionarTagPaciente($descricaoTag, $pessoaPacienteId)
    {
        $descricaoTag = stripslashes($descricaoTag);
        return $this->manter(array(), array(
            'descricao' => $descricaoTag,
            'pessoa_paciente_id' => $pessoaPacienteId,
        ));
    }

    /**
     * @param $descricaoTag
     * @param $pessoaPacienteId
     * @return bool|int
     */
    public function excluirTagPaciente($descricaoTag, $pessoaPacienteId)
    {
        $dadosTag = $this->recuperarLinha('*', array(
            "descricao = '{$descricaoTag}'",
            "pessoa_paciente_id = {$pessoaPacienteId}",
            "excluido IS FALSE"),
            false
        );

        return $this->excluirLogicamente($dadosTag['id']);
    }
}