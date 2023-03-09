<?php

class EstabelecimentoCategoria extends Modelo
{
  /**
   * Nome da tabela especificada
   * @var string
   * @access protected
   */
  protected $stNomeTabela = "estabelecimento.tb_estabelecimento_categoria";

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
    'estabelecimento_id' => null,
    'categoria' => null,
  );

  public function salvarCategorias($params)
  {
   $sql = "DELETE FROM estabelecimento.tb_estabelecimento_categoria WHERE estabelecimento_id = {$params['id']}";
   $this->executar($sql);
    foreach ($params['categorias'] as $dado) {
      $sql = "INSERT INTO  estabelecimento.tb_estabelecimento_categoria (estabelecimento_id, categoria) VALUES ({$params['id']}, '{$dado}')";
      $gravou  = $this->executar($sql);
    }
    return $gravou;
  }

  public function getDadosCategorias($where)
  {
    
    $sql = "
    SELECT
       categoria
    FROM
        estabelecimento.tb_estabelecimento_categoria
    WHERE
    {$where}";
    $retorno = $this->carregar($sql);
    return $retorno;
  }
}
