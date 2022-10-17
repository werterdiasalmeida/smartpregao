<?php

class UsuarioSistema extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.usuario_sistema";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('usucpf', 'sisid');

    protected $tabelaAssociativa = true;

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'sisid' => null,
        'susstatus' => null,
        'pflcod' => null,
        'susdataultacesso' => null,
        'suscod' => null,
    );

    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array())
    {
        if( $boAntesSalvar ){
            if( !$this->antesSalvar() ){ return false; }
        }

        $usuarioSistema = $this->recuperarLinha('*', array("usucpf = '{$this->usucpf}'", "sisid = {$this->sisid}") , false);

        if(is_array($usuarioSistema)){
            $resultado = $this->alterar($arCamposNulo);
        }else{
            $resultado = $this->inserir();
        }


        if( $resultado && $boDepoisSalvar){
            $this->depoisSalvar();
        }

        return $resultado;
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
    }

    public function alterar($arCamposNulo = array()){
        $campos = "";
        $valorCampoChave = array();
        foreach( $this->arAtributos as $campo => $valor ){
            if( $valor !== null ){
                if( $campo == $this->arChavePrimaria[0] ){
                    $valorCampoChave[0] = $valor;
                    continue;
                }

                if( $campo == $this->arChavePrimaria[1] ){
                    $valorCampoChave[1] = $valor;
                    continue;
                }

                $valor = pg_escape_string( $valor );

                $campos .= $campo." = '".$valor."', ";
            }
            else{
                if(in_array($campo, $arCamposNulo)) {
                    $campos .= $campo." = null, ";
                }
            }
        }

        $campos = substr( $campos, 0, -2 );

        $sql = "UPDATE $this->stNomeTabela SET $campos WHERE {$this->arChavePrimaria[0]} = '{$valorCampoChave[0]}' AND {$this->arChavePrimaria[1]} = '{$valorCampoChave[1]}' RETURNING {$this->arChavePrimaria[0]}";

        return $this->pegaUm( $sql );
    } // Fim _alterar()

    public function excluirUsuarioSistemaPorSistema($usucpf, $sisid)
    {
        $sql = "UPDATE seguranca.usuario_sistema SET susstatus = 'I' WHERE usucpf = '{$usucpf}' 
                    AND sisid = {$sisid}";
        $this->executar($sql);
    }
}