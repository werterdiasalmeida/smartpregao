<?php
//
// $Id$
//



require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * 
 */
class ArquivoBase extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'public.arquivo';

    /**
     * Sequence pertencente a tabela entidade.funcao
     * @access protected
     */
    protected $sequence      = 'public.arquivo_arqid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('arqid', null);

    protected $campos        = array('arqnome'      => null,
                                     'arqdescricao' => null,
                                     'arqextensao'  => null,
                                     'arqtipo'      => null,
                                     'arqtamanho'   => null,
                                     'arqdata'      => null,
                                     'arqhora'      => null,
                                     'arqstatus'    => null,
                                     'usucpf'       => null,
                                     'sisid'        => null);


    /**
     * Carrega os dados recuperados do banco de dados no objeto.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    public function carregar($id = null)
    {
        if ($id == null)
            return new Arquivo();

        $sql = "SELECT
                    arqid,
                    arqnome,
                    arqdescricao,
                    arqextensao,
                    arqtipo,
                    arqtamanho,
                    arqdata,
                    arqhora,
                    arqstatus,
                    usucpf,
                    sisid
                FROM
                    public.arquivo
                WHERE
                    arqid = ?";

        $rs                     = $this->Execute($sql, array($id));

        $this->arqnome          = $rs->fields['arqnome'];
        $this->arqdescricao     = $rs->fields['arqdescricao'];
        $this->arqextensao      = $rs->fields['arqextensao'];
        $this->arqtipo          = $rs->fields['arqtipo'];
        $this->arqtamanho       = $rs->fields['arqtamanho'];
        $this->arqdata          = $rs->fields['arqdata'];
        $this->arqhora          = $rs->fields['arqhora'];
        $this->arqstatus        = $rs->fields['arqstatus'];
        $this->usucpf           = $rs->fields['usucpf'];
        $this->sisid            = $rs->fields['sisid'];

        $this->chavePrimaria[1] = $rs->fields['arqid'];

        return clone $this;
    }


    /**
     * 
     */
    public function setPrimaryKey($valor)
    {
        return $this->chavePrimaria[1] = $valor;
    }


    /**
     * 
     */
    public function getPrimaryKey()
    {
        return $this->chavePrimaria[1];
    }
}





