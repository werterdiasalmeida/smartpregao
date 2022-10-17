<?php

class PerfilUsuario extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.perfilusuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('pflcod', 'usucpf');

    protected $tabelaAssociativa = true;

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pflcod' => null,
        'usucpf' => null,
    );

    public function excluirPerfisUsuarioPorSistema($usucpf, $sisid)
    {
        $sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '{$usucpf}' 
                    AND pflcod IN (SELECT pflcod FROM seguranca.perfil WHERE sisid = {$sisid})";
        $this->executar($sql);
    }

    public function inserir(){

        $arCampos  = array();
        $arValores = array();
        $arSimbolos = array();

        foreach( $this->arAtributos as $campo => $valor ){

            if( $campo == $this->arChavePrimaria[0] && !$this->tabelaAssociativa ) continue;
            if( $valor !== null ){
                $arCampos[]  = $campo;
                $arValores[] = trim( pg_escape_string( $valor ) );
            }
        }

        if( count( $arValores ) ){
            $sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' )
					 returning {$this->arChavePrimaria[0]}";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm( $sql );
        }
    } // Fim _inserir()
}