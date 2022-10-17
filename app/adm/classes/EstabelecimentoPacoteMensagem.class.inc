<?php

class EstabelecimentoPacoteMensagem extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_estabelecimento_pacote_mensagem";

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
        'estabelecimento_id'    => null,
        'plano_pacote_id'       => null,
        'fatura_id'             => null,
        'quantidade_total'      => null,
        'quantidade_disponivel' => null,
        'criado_em'             => null,
        'criado_por'            => null,
        'atualizado_em'         => null,
        'atualizado_por'        => null,
        'excluido'              => null
    );

    /**
     * Recupera os pacotes que possuem pagamento pendente desse estabelecimento
     * @param $estabelecimento_id
     * @return array
     */
    public function getPacotesPagamentoPendente($estabelecimentoId, $tipo = '')
    {
        $strTipo = "";
        if($tipo){
            $strTipo = " AND tm.codigo = '{$tipo}' ";
        }

        $sql = "SELECT
                    epm.id AS estabelecimento_pacote_mensagem_id,
                    epm.plano_pacote_id,
                    epm.estabelecimento_id,
                    epm.criado_em AS data_contratacao,
                    (fat.situacao IS NOT NULL AND fat.situacao = '" . Fatura::SITUACAO_FATURA_PAGA . "') AS pago,
                    (fat.situacao IS NOT NULL AND fat.situacao = '" . Fatura::SITUACAO_FATURA_VENCIDA . "') AS vencida,
                    fat.id AS fatura_id,
                    fat.data_vencimento,
                    fat.secure_id_iugu,
                    ppm.valor,
                    tm.ilimitado,
                    ppm.quantidade_mensagens
                FROM
                    {$this->stNomeTabela} epm
                INNER JOIN
                    estabelecimentosaude.tb_plano_pacote_mensagem ppm ON ppm.id = epm.plano_pacote_id
                INNER JOIN
                    mensageria.tb_tipo_mensagem tm ON tm.id = ppm.tipo_mensagem_id
                LEFT JOIN
                    financeiro.tb_fatura fat ON epm.fatura_id = fat.id AND fat.excluido IS FALSE                                
                WHERE
                    epm.excluido IS FALSE AND
                    (fat.situacao IS NULL OR fat.situacao != '" . Fatura::SITUACAO_FATURA_PAGA . "') AND
                    epm.estabelecimento_id = {$estabelecimentoId}
                    {$strTipo}
                ORDER BY
                    epm.criado_em ASC";


        $dados = $this->carregar($sql);
        return $dados ? $dados : array();
    }

    /**
     * Recupera os pacotes que possuem pagamento pendente desse estabelecimento
     * @param $estabelecimento_id
     * @return array
     */
    public function getPacoteDisponivelByEstabelecimento($estabelecimentoId, $tipoMensagem)
    {
        $sql = "SELECT
                    epm.*,
                    tm.ilimitado
                FROM
                    {$this->stNomeTabela} epm
                INNER JOIN
                    estabelecimentosaude.tb_plano_pacote_mensagem ppm ON ppm.id = epm.plano_pacote_id
                INNER JOIN
                    mensageria.tb_tipo_mensagem tm ON tm.id = ppm.tipo_mensagem_id
                LEFT JOIN
                    financeiro.tb_fatura fat ON epm.fatura_id = fat.id AND fat.excluido IS FALSE            
                WHERE
                    epm.excluido IS FALSE AND
                    fat.situacao = '" . Fatura::SITUACAO_FATURA_PAGA . "' AND
                    epm.estabelecimento_id = {$estabelecimentoId} AND
                    tm.id = {$tipoMensagem} AND
                    epm.quantidade_disponivel != 0
                ORDER BY
                    epm.criado_em ASC";

        $dados = $this->pegaLinha($sql);
        return $dados ? $dados : array();
    }

    /**
     * Recupera os pacotes que possuem pagamento pendente desse estabelecimento
     * @param $estabelecimento_id
     * @return array
     */
    public function getDadosCompletosById($id)
    {
        $sql = "SELECT
                    epm.id AS estabelecimento_pacote_mensagem_id,
                    epm.plano_pacote_id,
                    epm.estabelecimento_id,
                    epm.criado_em AS data_contratacao,
                    (fat.situacao IS NULL OR fat.situacao != '" . Fatura::SITUACAO_FATURA_PAGA . "') AS pacote_pago,
                    ppm.valor AS plano_valor,
                    tm.ilimitado AS plano_ilimitado,
                    tm.descricao AS descricao_tipo_mensagem,
                    ppm.quantidade_mensagens,
                    pj.id as pessoa_juridica_id,
                    pj.razao_social,
                    pj.cnpj
                FROM
                    {$this->stNomeTabela} epm
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento est ON est.id = epm.estabelecimento_id
                INNER JOIN
                    corporativo.tb_pessoajuridica pj ON pj.id = est.pessoajuridica_id
                INNER JOIN
                    estabelecimentosaude.tb_plano_pacote_mensagem ppm ON ppm.id = epm.plano_pacote_id
                INNER JOIN
                    mensageria.tb_tipo_mensagem tm ON tm.id = ppm.tipo_mensagem_id
                LEFT JOIN
                    financeiro.tb_fatura fat ON epm.fatura_id = fat.id
                WHERE
                    epm.id = {$id}";

        $dados = $this->pegaLinha($sql);
        return $dados ? $dados : array();
    }

    /**
     * Salva todos os dados necessários para o sistema gerar um pacote para o estabelecimento
     * @param $params
     * @return bool|int
     */
    public function salvarDados($params)
    {
        //Recuperando as informações do pacote
        $mPlanoPacote = new PlanoPacoteMensagem($params['plano_pacote_mensagem_id']);
        $dadosPlanoPacote = $mPlanoPacote->getDados();

        $arDados = [
            'plano_pacote_id'       => $params['plano_pacote_mensagem_id'],
            'quantidade_total'      => $dadosPlanoPacote['quantidade_mensagens'],
            'quantidade_disponivel' => $dadosPlanoPacote['quantidade_mensagens'],
            'estabelecimento_id'    => $params['id'],
            'excluido'              => 'false'
        ];
        return $this->manter($arDados);
    }

    /**
     * Desabilita o registro do pagamento e do estabelecimento pacote mensagem
     * @param $id
     * @return bool|int
     */
    public function inativar($id)
    {
        $this->carregarPorId($id);
        $dados = $this->getDados();

        if($dados['fatura_id']){
            $mFatura = new Fatura();
            $mFatura->excluirLogicamente($dados['fatura_id']);
        }

        //Desativando o registro
        $id = $this->excluirLogicamente($id);

        return $id;
    }

    /**
     * Consome 1 mensagem disponível do pacote
     * @param $id
     * @return bool|string
     */
    public function consumirDisponivel($id)
    {
        $sql = "UPDATE
                    {$this->stNomeTabela}
                SET
                    quantidade_disponivel = (quantidade_disponivel -1)
                WHERE
                    id = {$id} AND 
                    quantidade_disponivel > 0
                RETURNING
                    quantidade_disponivel;";

        return $this->pegaUm($sql);
    }


    public function getListByEstabelecimentoAndTipo($estabelecimentoId, $tipoMensagem, $jsonEncode = false)
    {
        $sql = "SELECT
                    epm.id AS estabelecimento_pacote_mensagem_id,
                    epm.estabelecimento_id,
                    epm.plano_pacote_id,
                    epm.fatura_id,
                    epm.quantidade_total,
                    epm.quantidade_disponivel,
                    epm.criado_em AS data_cadastro,
                    tm.ilimitado,
                    fat.situacao AS fatura_situacao,
                    fat.secure_id_iugu,
                    ppm.valor AS plano_valor,
                    tm.id AS tipo_mensagem_id
                FROM
                    {$this->stNomeTabela} epm
                INNER JOIN
                    estabelecimentosaude.tb_plano_pacote_mensagem ppm ON ppm.id = epm.plano_pacote_id
                INNER JOIN
                    mensageria.tb_tipo_mensagem tm ON tm.id = ppm.tipo_mensagem_id
                LEFT JOIN
                    financeiro.tb_fatura fat ON epm.fatura_id = fat.id AND fat.excluido IS FALSE            
                WHERE
                    epm.excluido IS FALSE AND
                    epm.estabelecimento_id = {$estabelecimentoId} AND
                    tm.id = {$tipoMensagem} AND
                    epm.quantidade_disponivel != 0
                ORDER BY
                    epm.criado_em DESC";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : array();
        return $jsonEncode ? json_encode(utf8ArrayEncode($dados)) : $dados;
    }
}