<?php
class TipoMensagem extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "mensageria.tb_tipo_mensagem";

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
		'id'        => null,
		'descricao' => null,
		'ilimitado' => null,
		'codigo' => null,
		'icone' => null
	);


    const TIPO_MENSAGEM_EMAIL = 1;
    const TIPO_MENSAGEM_NOTIFICACAO_CELULAR = 2;
    const TIPO_MENSAGEM_SMS = 3;

    public function getInformacoesTipoMensagemByEstabelecimento($estabelecimento_id)
    {
        $sql = "SELECT
                    tp.id,
                    tp.descricao,
                    tp.codigo,
                    tp.icone,
                    COALESCE(total.qtd_total,0) AS qtd_total,
                    COALESCE(total.qtd_disponivel,0) AS qtd_disponivel,
                    tp.ilimitado
                FROM
                    {$this->stNomeTabela} tp
                LEFT JOIN
                    (SELECT
                        SUM(quantidade_total) AS qtd_total,
                        SUM(quantidade_disponivel) AS qtd_disponivel,
                        epm.estabelecimento_id,
                        ppm.tipo_mensagem_id
                     FROM
                        estabelecimentosaude.tb_estabelecimento_pacote_mensagem epm
                     INNER JOIN
                        estabelecimentosaude.tb_plano_pacote_mensagem ppm ON ppm.id = epm.plano_pacote_id
                     INNER JOIN
                        financeiro.tb_fatura fat ON epm.fatura_id = fat.id
                     WHERE
                        epm.quantidade_disponivel != 0 AND
                        fat.situacao = '" . Fatura::SITUACAO_FATURA_PAGA . "' AND
                        epm.excluido IS FALSE
                     GROUP BY
                        3,4
                      ) as total ON total.tipo_mensagem_id = tp.id AND total.estabelecimento_id = {$estabelecimento_id}
                WHERE
                    true
                ORDER BY
                    tp.ilimitado DESC";

        $dados = $this->carregar($sql);
        return $dados ? $dados : array();
    }

    public function alterarConfiguracaoTipoByEstabelecimento($estabelecimento, $tipoMensagem, $ativo = true)
    {
        $mConfiguracao = new EstabelecimentoConfiguracao();
        $dadosConfiguracao = $mConfiguracao->getByEstabelecimento($estabelecimento);

        $jsonSalvar = [];
        $configuracaoDisparo = !empty($dadosConfiguracao['configuracao_disparo_mensagens']) ? json_decode($dadosConfiguracao['configuracao_disparo_mensagens'],true) : [];
        foreach ($configuracaoDisparo AS $chave => $confMensagem){
            $jsonSalvar[$chave]['tipo_mensagem_id'] = $confMensagem['tipo_mensagem_id'];
            $jsonSalvar[$chave]['habilitado'] = ($tipoMensagem == $confMensagem['tipo_mensagem_id']) ? $ativo : $confMensagem['habilitado'];
        }
        $dadosConfiguracao['configuracao_disparo_mensagens'] = json_encode($jsonSalvar);
        return $mConfiguracao->manter($dadosConfiguracao);
    }
}

