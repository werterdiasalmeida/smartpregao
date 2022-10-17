<?php

class EstabelecimentoConfiguracao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_estabelecimento_configuracao";

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
        'id'                                => null,
        'estabelecimento_id'                => null,
        'configuracao_disparo_mensagens'    => null,
        'criado_em'                         => null,
        'criado_por'                        => null,
        'atualizado_em'                     => null,
        'atualizado_por'                    => null,
        'excluido'                          => null,
        'inicial_lote'                      => null
    );


    /**
     * Retorna um registro de configuração por estabelecimento
     * @param $estabelecimento_id
     * @return array
     */
    public function getByEstabelecimento($estabelecimento_id){
        $sql = "SELECT
                    *
                FROM
                    {$this->stNomeTabela}
                WHERE
                    estabelecimento_id = {$estabelecimento_id} AND
                    excluido IS FALSE";

        $dados = $this->pegaLinha($sql);
        return is_array($dados) ? $dados : [];
    }


    /**
     * Retorna um registro de configuração por estabelecimento
     * @param $estabelecimento_id
     * @return array
     */
    public function getConfigDisparoMensagensByEstabelecimento($estabelecimento_id){
        $sql = "SELECT
                    configuracao_disparo_mensagens
                FROM
                    {$this->stNomeTabela}
                WHERE
                    estabelecimento_id = {$estabelecimento_id} AND
                    excluido IS FALSE";
        $configuracaoDisparo = $this->pegaUm($sql);

        $configDisparoMensagens = [];
        $dadosConfiguracao = !empty($configuracaoDisparo) ? json_decode($configuracaoDisparo,true) : [];
        foreach ($dadosConfiguracao AS $confMensagem){
            $configDisparoMensagens[$confMensagem['tipo_mensagem_id']] = $confMensagem['habilitado'];
        }

        return $configDisparoMensagens;
    }

    /**
     * Retorna se um estabelecimento pode enviar uma mensagem de acordo com o tipo
     * @param $estabelecimentoId
     * @param $tipoMensagemId
     * @return mixed
     */
    public function isEstabelecimentoPodeEnviarMensagemPorTipo($estabelecimentoId, $tipoMensagemId) {
        $configuracao = $this->getConfigDisparoMensagensByEstabelecimento($estabelecimentoId);

        return $configuracao[$tipoMensagemId];
    }
}