<?php

class PainelSenhaRegistro extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_painel_senha_registro";

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
        'criado_em'             => null,
        'criado_por'            => null,
        'excluido'              => null,
        'painel_senha_id'       => null,
        'numero'                => null,
        'guiche'                => null,
        'qtd_chamadas'          => null,
    );

    /**
     * @param $painelSenhaId
     * @param int $quantidade
     * @return array
     */
    public function getUltimosByPainel($painelSenhaId, $quantidade = 5)
    {
        $arWhere = array(
            "psr.painel_senha_id = {$painelSenhaId}",
            "psr.excluido IS FALSE"
        );

        $sql = "SELECT 
                    psr.id,
                    psr.numero,
                    psr.guiche,
                    psr.qtd_chamadas,
                    TO_CHAR(psr.criado_em, 'HH24:mi:ss') AS hora
                FROM 
                    {$this->stNomeTabela} psr
                WHERE 
                    " . implode(" AND ", $arWhere) . " 
                ORDER BY
                    psr.criado_em DESC
                LIMIT 
                    {$quantidade};";

        return $this->carregarArray($sql);
    }

    /**
     * @param $estabelecimentoId
     * @param false $jsonEncode
     * @return array
     */
    public function getListByEstabelecimento($estabelecimentoId, $jsonEncode = false, $tipo = 'R')
    {
        $arWhere = array(
            "ps.estabelecimento_id = {$estabelecimentoId}",
            "ps.tipo = '{$tipo}'",
            "data_validade > '" . (new DateTime())->format('Y-m-d H:i:s') . "'",
            "ps.excluido IS FALSE",
            "psr.excluido IS FALSE",
        );

        $sql = "SELECT 
                    psr.id AS painel_senha_registro_id,
                    ps.id AS painel_senha_id,
                    u.usunome AS responsavel,
                    psr.numero,
                    psr.guiche,
                    TO_CHAR(psr.criado_em,'DD/MM/YYYY HH24:MI') AS data,
                    psr.qtd_chamadas
                FROM 
                    {$this->stNomeTabela} psr
                INNER JOIN
                    seguranca.usuario u ON psr.criado_por = u.id
                INNER JOIN
                    estabelecimentosaude.tb_painel_senha ps ON ps.id = psr.painel_senha_id
                WHERE 
                    " . implode(" AND ", $arWhere) . " 
                ORDER BY
                    psr.criado_em DESC;";

        $rs = $this->carregarArray($sql);
        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    /**
     * @param bool $dados
     * @return bool|int
     */
    public function chamarSenha($dados)
    {
        $ultimosChamados = $this->getUltimosByPainel($dados['painel_senha_id']);
        if (is_array($ultimosChamados) && count($ultimosChamados) > 0) {
            foreach ($ultimosChamados AS $ultimoChamado) {
                if ($ultimoChamado['numero'] == $dados['numero'] && $ultimoChamado['guiche'] == $dados['guiche']) {
                    $sql = "UPDATE {$this->stNomeTabela} SET qtd_chamadas = (qtd_chamadas + 1) WHERE id = " . $ultimoChamado['id'];
                    $this->executar($sql);
                    return $ultimoChamado['id'];
                }
            }
        }

        $arDados = [
            'guiche'            => $dados['guiche'],
            'numero'            => $dados['numero'],
            'painel_senha_id'   => $dados['painel_senha_id'],
            'excluido'          => 'false'
        ];

        return $this->manter($arDados);
    }
}