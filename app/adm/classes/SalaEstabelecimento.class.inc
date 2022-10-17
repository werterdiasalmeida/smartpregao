<?php

class SalaEstabelecimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_sala_estabelecimento";

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
        'sala' => null,
        'estabelecimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    /**
     * @param $params
     * @param $estabelecimentoId
     * @throws Exception
     */
    public function salvarSalas($params, $estabelecimentoId)
    {
        $salas = is_array($params['sala']) ? $params['sala'] : array($params['sala']);
        $this->excluirPorEstabelecimento($estabelecimentoId);

        foreach($salas as $sala){
            if(empty($sala['sala'])){
                continue;
            }

            $sala['estabelecimento_id'] = $estabelecimentoId;

            $this->salvarSala($sala);
            $this->setDadosNull();
        }
    }

    public function salvarSala($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $dados = array(
            'id' => $params['id'],
            'sala' => $params['sala'],
            'excluido' => 'f',
            'estabelecimento_id' => $params['estabelecimento_id'],
        );

        $dados = $this->getDadosCarimbo($params, $dados);
        return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
    }

    /**
     * @param $estabelecimentoId
     * @throws Exception
     */
    public function excluirPorEstabelecimento($estabelecimentoId)
    {
        $sql = "UPDATE estabelecimentosaude.tb_sala_estabelecimento SET excluido = TRUE WHERE estabelecimento_id = {$estabelecimentoId}";
        $this->executar($sql);
    }

    public function listarPorEstabelecimento($estabelecimentoId, $jsonEncode = false)
    {
        $rs = $this->recuperarTodos('id, sala', array("estabelecimento_id = {$estabelecimentoId}", "excluido IS FALSE"));
        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    public function getComboPorEstabelecimento($estabelecimentoId)
    {
        $rs = $this->listarPorEstabelecimento($estabelecimentoId);
        return array_map(function($sala){
            return array(
                'codigo' => $sala['id'],
                'descricao' => $sala['sala']
            );
        }, $rs);
    }
}