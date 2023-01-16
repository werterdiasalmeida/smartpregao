<?php

class EstabelecimentoAgenda extends Modelo
{
  /**
   * Nome da tabela especificada
   * @var string
   * @access protected
   */
  protected $stNomeTabela = "estabelecimentosaude.tb_estabelecimento_agenda";

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
    'inicio' => null,
    'fim' => null,
    'compromisso' => null,
    'url' => null,
  );

  public function salvarAgenda($params)
  {
    list($params, $camposNulos) = $this->limparDadosVazios($params);
    $data = formata_data_sql($params['data_agenda']);
    $horaInicio = str_pad($params['hora_inicio'], 5, '0', STR_PAD_LEFT) . ':00';
    $horaFim = str_pad($params['hora_fim'], 5, '0', STR_PAD_LEFT) . ':00';

    $dados = array(
      'estabelecimento_id' => $params['estabelecimento_id'],
      'editalcliente_id' => $params['editalcliente_id'],
      'inicio' => "{$data} {$horaInicio}",
      'fim' => "{$data} {$horaFim}",
      'compromisso' => $params['compromisso'],
      'url' => $params['url'],
    );
    return $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);
  }

  public function getDadosCalendario($arWhere = array())
  {
    $saida['hoje'] = date('Y-m-d');
    $sql = "
    SELECT
       *
    FROM
        estabelecimentosaude.tb_estabelecimento_agenda
    WHERE
    {$arWhere}";
    $retorno = $this->carregar($sql);
    $saida['agenda'] = '[';
    foreach ($retorno as $dados) {
      $dados['inicio'] = substr($dados['inicio'], 0, 10) . 'T' . substr($dados['inicio'], 11, 8);
      $dados['fim'] = substr($dados['fim'], 0, 10) . 'T' . substr($dados['fim'], 11, 8);
      $apagar = '';
      $dados['compromisso'] = "{$dados['compromisso']} {$apagar}";
      if($dados['url']=='http://') $dados['url'] = '';
      $saida['agenda'] .= '{title: "' . $dados['compromisso'] . '",';
      $saida['agenda'] .= 'start: "' . $dados['inicio'] . '",';
      $saida['agenda'] .= 'end: "' . $dados['fim'] . '",';
      $saida['agenda'] .= 'url: "' . $dados['url'] . '"},';
    }
    $saida['agenda'] .= ']';
    return $saida;
  }
}
