<?php

class EstabelecimentoMensagem extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_estabelecimento_mensagem";

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
        'mensagem_id'           => null,
        'pacote_id'             => null,
        'estabelecimento_id'    => null,
        'criado_em'             => null,
        'criado_por'            => null,
        'atualizado_em'         => null,
        'atualizado_por'        => null,
        'excluido'              => null
    );

    /**
     * Envia uma mensagem de um tipo espec?fico para um estabelecimento
     *
     * @param $estabelecimento
     * @param $tipoMensagem
     * @param $categoriaMensagem
     * @param $mensagem
     * @param $destinatario
     * @throws Exception
     */
    public function enviarMensagem($estabelecimento, $tipoMensagem, $categoriaMensagem, $mensagem, $destinatario)
    {
        $mTipoMensagem = new TipoMensagem($tipoMensagem);
        $dadosTipoMensagem = $mTipoMensagem->getDados();

        //verificar se o estabelecimento est? configurado para enviar mensagens desse tipo
        $mConfiguracao = new EstabelecimentoConfiguracao();
        $disparoHabilitado = $mConfiguracao->isEstabelecimentoPodeEnviarMensagemPorTipo($estabelecimento, $tipoMensagem);
        if(!$disparoHabilitado){
            throw new Exception('O estabelecimento est? configurado para n?o enviar esse tipo de mensagem!');
        }

        //verificar se o estabelecimento possui saldo dispon?vel para enviar a mensagem (caso o pacote n?o seja ilimitado)
        $mPacoteMensagem = new EstabelecimentoPacoteMensagem();
        $pacoteDisponivel = $mPacoteMensagem->getPacoteDisponivelByEstabelecimento($estabelecimento,$tipoMensagem);
        if($dadosTipoMensagem['ilimitado'] == 'f' && (!$pacoteDisponivel || $pacoteDisponivel['quantidade_disponivel'] <= 0)){
            throw new Exception('O estabelecimento n?o possui mensagem desse tipo dispon?vel para envio!');
        }

        //enviar a mensagem (independente do tipo)
        $mMensagem = new Mensagem();
        $arMensagem = $mMensagem->enviarMensagem($mensagem, $destinatario, $tipoMensagem, $categoriaMensagem);

        //gravar o vinculo com o estabelecimento
        $arDados = [
            'mensagem_id'           => $arMensagem['id'],
            'pacote_id'             => $pacoteDisponivel['id'] ? $pacoteDisponivel['id'] : null,
            'estabelecimento_id'    => $estabelecimento,
            'excluido'              => 'false'
        ];
        $this->manter($arDados);

        //subtrair do total dispon?vel (caso o pacote n?o seja ilimitado)
        if($pacoteDisponivel['ilimitado'] == 'f'){
            $mPacoteMensagem->consumirDisponivel($pacoteDisponivel['id']);
        }

        //Se n?o for ilimitado e s? tinha uma mensagem ent?o desativa o envio de mensagem desse tipo
        if($dadosTipoMensagem['ilimitado'] == 'f' && (!$pacoteDisponivel || $pacoteDisponivel['quantidade_disponivel'] == 1)){
            $mTipoMensagem->alterarConfiguracaoTipoByEstabelecimento($estabelecimento,$tipoMensagem, false);
        }
        $this->commit();
    }

    /**
     * @param $mensagemOriginal
     * @param $idResposta
     * @return bool
     */
    public function processaResposta($mensagemOriginal,$idResposta)
    {
        return true;
    }
}