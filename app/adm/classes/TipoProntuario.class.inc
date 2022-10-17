<?php

class TipoProntuario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "paciente.tb_tipo_prontuario";

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
        'descricao' => null,
        'sigla' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    const EVOLUCAO = 'EVT';
    const PEDIDO_EXAME_LABORATORIAL = 'PEL';
    const PEDIDO_EXAME_IMAGEM = 'PEI';
    const PRESCRICAO = 'PRE';
    const DIAGNOSTICO = 'DIA';

    public function getPorSigla($sigla)
    {
        return $this->recuperarLinha('*', array("sigla = '{$sigla}'", "excluido IS false"), false);
    }

    public static function getEstilo()
    {
        $mTipoProntuario = new TipoProntuario();
        $tipos = $mTipoProntuario->recuperarTodos('*', array('excluido IS FALSE'), '1');

        $estilo = array(
            self::EVOLUCAO => array(
                'classe_cor' => 'green-jungle',
                'classe_icone' => 'fa fa-line-chart',
                'cor_hexadecimal' => '26C281'
            ),
            self::PEDIDO_EXAME_LABORATORIAL => array(
                'classe_cor' => 'blue-steel',
                'classe_icone' => 'fa fa-flask',
                'cor_hexadecimal' => '4B77BE'
            ),
            self::PEDIDO_EXAME_IMAGEM => array(
                'classe_cor' => 'blue-dark',
                'classe_icone' => 'fa fa-picture-o',
                'cor_hexadecimal' => '5e738b'
            ),
            self::PRESCRICAO => array(
                'classe_cor' => 'yellow',
                'classe_icone' => 'fa fa-pencil-square-o',
                'cor_hexadecimal' => 'C49F47'
            ),
            self::DIAGNOSTICO => array(
                'classe_cor' => 'purple',
                'classe_icone' => 'fa fa-stethoscope',
                'cor_hexadecimal' => '8E44AD'
            ),
        );

        $tiposRetorno = array();
        foreach ($tipos as $tipo){
            $tipo['estilo'] = isset($estilo[$tipo['sigla']]) ? $estilo[$tipo['sigla']] : array(
                'classe_cor' => '',
                'classe_icone' => '',
                'cor_hexadecimal' => ''
            );

            $tiposRetorno[$tipo['sigla']] = $tipo;
        }

        return $tiposRetorno;
    }

    public function getTiposSemFormatacao()
    {
        // return $this->recuperarColuna('id', array("excluido IS false", "sigla IN ('" . self::PRESCRICAO . "')"));
        return array();
    }
}