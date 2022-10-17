<?php
class Mensagem extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "mensageria.tb_mensagem";

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
		'categoria_mensagem_id' => null,
		'mensagem'              => null,
		'codigo'                => null,
		'destinatario'          => null,
		'enviado_em'            => null,
		'criado_em'             => null,
		'criado_por'            => null,
		'atualizado_em'         => null,
		'atualizado_por'        => null,
		'excluido'              => null
	);


    public function enviarMensagem($mensagem, $destinatario, $tipoMensagem, $categoriaMensagem)
    {
         $retorno = [];
         switch ($tipoMensagem){
             case TipoMensagem::TIPO_MENSAGEM_EMAIL:
                 $retorno = $this->enviarEmail($mensagem, $destinatario, $categoriaMensagem);
                break;
             case TipoMensagem::TIPO_MENSAGEM_NOTIFICACAO_CELULAR:
                 $retorno = $this->enviarPush($mensagem, $destinatario, $categoriaMensagem);
                 break;
             case TipoMensagem::TIPO_MENSAGEM_SMS:
                 $retorno = $this->enviarSMS($mensagem, $destinatario, $categoriaMensagem);
                 break;
             default:
                 throw new Exception("Tipo de mensagem não implementado!");
                 break;
         }

         return $retorno;
    }

    private function enviarEmail($mensagem, $destinatario, $categoriaMensagem)
    {
        global $email_from;

        $mCategoriaMensagem = new CategoriaMensagem();
        $mCategoriaMensagem->carregarPorId($categoriaMensagem);
        $dadosCategoria = $mCategoriaMensagem->getDados();

        $remetente = array();
        $remetente['email'] = $email_from;
        $remetente['nome'] = $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'];
        enviar_email($remetente, $destinatario, 'ClubVida - ' . $dadosCategoria['descricao'], $mensagem);

        $arDados = [
            'tipo_mensagem_id'      => TipoMensagem::TIPO_MENSAGEM_EMAIL,
            'categoria_mensagem_id' => $categoriaMensagem,
            'mensagem'              => $mensagem,
            'destinatario'          => $destinatario,
            'excluido'              => 'false'
        ];
        $this->manter($arDados);
        return $this->getDados();
    }

    private function enviarPush($mensagem, $destinatario, $categoriaMensagem)
    {
        $mNotificacao = new NotificacaoApp();
        $mNotificacao->enviarNotificacao("ClubVida", $mensagem, [$destinatario]);

        $arDados = [
            'tipo_mensagem_id'      => TipoMensagem::TIPO_MENSAGEM_NOTIFICACAO_CELULAR,
            'categoria_mensagem_id' => $categoriaMensagem,
            'mensagem'              => $mensagem,
            'destinatario'          => $destinatario,
            'excluido'              => 'false'
        ];
        $this->manter($arDados);
        return $this->getDados();
    }

    /**
     * Envia uma mensagem de sms
     * @param $mensagem
     * @param $destinatario
     * @return array|null
     * @throws Exception
     */
    private function enviarSMS($mensagem, $destinatario, $categoriaMensagem)
    {
        $mSMS = new SimecSMS();
        $retorno = null;
        try{
            $destinatario = preg_replace("/[^0-9,]/","",$destinatario);
            $retorno = $mSMS->enviarSMS($mensagem, $destinatario, $categoriaMensagem, true);
        }catch(Exception $e){
            throw $e;
        }

        return $retorno;
    }
}

