<?php

class LoteGuia extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_lote_guia";

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
        'lote_id' => null,
        'guia_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    public function excluirPorLote(int $loteId)
    {
        $sql = "UPDATE 
                    estabelecimentosaude.tb_lote_guia 
               SET 
                    excluido = TRUE,
                    atualizado_em = now(),
                    atualizado_por = '{$_SESSION['usuario_id']}'
               WHERE 
                    lote_id = {$loteId}";

        $this->executar($sql);
    }

    public function salvarLoteGuia($loteId, $guiaId)
    {
        $dados = [
            'id' => $this->recuperarUm('id', ["lote_id = {$loteId}", "guia_id = {$guiaId}"]),
            'lote_id' => $loteId,
            'guia_id' => $guiaId,
            'excluido' => 'false'
        ];

        return $this->manter($dados);
    }

    public function getIdGuiasByLote($loteId)
    {
        return $this->recuperarColuna('guia_id', ["excluido IS FALSE", "lote_id = {$loteId}"]);
    }

    public function getGuiasByLote($loteId)
    {
        $sql = "SELECT 
                    * 
                FROM 
                    estabelecimentosaude.tb_guia gui
                INNER JOIN
                    estabelecimentosaude.tb_lote_guia log ON gui.id = log.guia_id
                WHERE
                    log.lote_id = {$loteId}
                    AND
                    log.excluido IS FALSE";
        $rs = array_map(function ($dadosGuia) {
            $dadosGuia['guia_json'] = $dadosGuia['guia'];
            $dadosGuia['guia'] = utf8ArrayDecode(json_decode(utf8_encode($dadosGuia['guia_json']), true));

            return $dadosGuia;
        }, $this->carregarArray($sql));

        return $rs;
    }
}