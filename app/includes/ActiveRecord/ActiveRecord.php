<?php
//
// $Id$
//



require_once APPRAIZ . "adodb/adodb.inc.php";



/**
 * @class ActiveRecord
 * @version $Revision$
 */
abstract class ActiveRecord {
    /**
     * 
     */
    const DRIVER              = "postgres";
    /**
     * Servidor de banco de dados
     * @var string
     * @access private
     */
    const HOSTNAME            = "";

    /**
     * Nome do banco de dados
     * @var string
     * @access private
     */
    const DATABASE            = "";

    /**
     * Usuário do banco de dados
     * @var string
     * @access private
     */
    const USERNAME            = "";

    /**
     * Senha do usuário do banco de dados
     * @var string
     * @access private
     */
    const PASSWORD            = "";

    /**
     * Link de conexão com o banco de dados
     * @var resource
     */
    static private $_db       = null;

    /**
     * @var string
     */
    protected $sequence       = null;

    /**
     * @var string
     */
    protected $tabela         = null;

    /**
     * @var array
     */
    protected $campos         = array();

    /**
     * Array contendo as chaves primárias relativas à tabela.
     *
     * @var array
     */
    protected $chavePrimaria  = array();

    /**
     * Instância da classe cls_banco
     * @var cls_banco
     */
    protected $db             = null;

    /**
     * 
     */
    protected $new            = true;


    //------------------------------------------------------------------ public
    /**
     * Construtor
     *
     * Executa a conexão com o banco de dados
     */
    final public function __construct($primary_key = null)
    {
        if (self::$_db === null)
            self::_connect();

        if ($primary_key != null && $primary_key != 'null' && trim((string) $primary_key) != '')
            $this->carregar($primary_key);

        $this->new = $this->chavePrimaria[1] !== null;
    }


    /**
     * 
     */
    final public function __destruct()
    {
        //self::$_db->Disconnect();
    }


    /**
     * 
     */
    final public function __get($campo)
    {
        if (array_key_exists($campo, $this->campos)) {
            return stripslashes($this->campos[$campo]);
        } elseif (in_array($campo, $this->chavePrimaria)) {
            return $this->chavePrimaria[1];
        } else {
            return null;
        }
    }


    /**
     * 
     */
    final public function __set($campo, $valor)
    {
        if (array_key_exists($campo, $this->campos)) {
            $this->campos[$campo] = $valor;
        } elseif (in_array($campo, $this->chavePrimaria)) {
            $this->chavePrimaria[1] = $valor;
        }/* else {
            throw new Exception("A propriedade `$campo' não foi definida "
                               ."para a entidade " . get_class($this) . ".");
        }*/
    }


    /**
     * 
     */
    static public function import($class)
    {
        if (!class_exists($class)) {
            if (file_exists(APPRAIZ . "includes/ActiveRecord/classes/" . $class . ".php"))
                require_once APPRAIZ . "includes/ActiveRecord/classes/" . $class . ".php";
            else
                throw new Exception("Não foi possível carregar a classe " . $class);
        }
    }


    /**
     * 
     */
    abstract public function carregar($pk = null);


    /**
     * 
     */
    abstract public function setPrimaryKey($valor);


    /**
     * 
     */
    abstract public function getPrimaryKey();


    /**
     * 
     */
    final public function BeginTransaction()
    {
        return self::$_db->BeginTrans();
    }


    /**
     * 
     */
    final public function Commit()
    {
        return self::$_db->CommitTrans();
    }


    /**
     * 
     */
    final public function Rollback()
    {
        return self::$_db->RollbackTrans();
    }


    /**
     * 
     */
    public function salvar()
    {
        if ($this->chavePrimaria[1] === null || $this->chavePrimaria[1] == 'null') {
            return $this->_inserir();
        } else {
            return $this->_alterar();
        }
    }


    /**
     * 
     */
    public function save()
    {
        return $this->salvar();
    }


    /**
     * 
     */
    public function excluir($primary_key = null)
    {
        if ($primary_key === null)
            $primary_key = $this->chavePrimaria[1];

        if ($primary_key === null)
            throw new Exception("Erro ao excluir o registro. Chave primária não informada ou inexistente.");

        $sql = sprintf("DELETE FROM %s WHERE %s = ?",
                       $this->tabela,
                       $this->chavePrimaria[0]);

        return $this->Execute($sql, array($primary_key));
    }


    /**
     * 
     */
    public function carregarColecao($whereClause = null, $limit = 0, $offset = 0)
    {
        $sql = "SELECT * FROM "  . $this->tabela . ($whereClause !== null ? " WHERE " . $whereClause : '');

        $res = $this->Execute($sql);
        $arr = array();
        $cls = get_class($this);

        while (!$res->EOF) {
            $arr[] = new $cls($res->fields[0]);
            $res->MoveNext();
        }

        return $arr;
    }


    /**
     * 
     */
    public function getFieldNames()
    {
        return array_keys($this->campos);
    }


    /**
     * 
     */
    final public function getSelectSql(array $campos = array(), $whereClause = null)
    {
        if (sizeof($campos) == 0) {
            $campos = array_keys($this->campos);
        }

        return "SELECT " .  implode(", ", $campos) . " FROM " . $this->tabela . ($whereClause !== null ? " WHERE " . $whereClause : '');
    }


    /**
     * 
     */
    final public function getArrayCampos()
    {
        return array_keys($this->campos);
    }


    /**
     * 
     */
    public function toJson()
    {
        $parts = array();

        if ($this->chavePrimaria[0] !== null)
            $json = '{' . $this->chavePrimaria[0] . ':\'' . $this->chavePrimaria[1] . '\',';
        else
            $json = '{';

        foreach ($this->campos as $campo => $valor) {
            $parts[] = $campo . ':\'' . addslashes($valor) . '\'';
        }

        return $json . implode(",", $parts) . "}";
    }


    //--------------------------------------------------------------- protected
    /**
     * @return integer
     */
    final protected function insertId()
    {
        if ($this->sequence !== null) {
            $rs = $this->Execute("SELECT last_value FROM {$this->sequence}");
            return $rs->fields["last_value"];
        }

        return null;
    }


    /**
     * @param string $sql
     * @param array $params
     * @return resource
     */
    final protected function Execute($sql, array $params = array(), $auditoria = true)
    {
        if (count($params)) {
            $rs = self::$_db->Execute($sql, $params);
        } else {
            $rs = self::$_db->Execute($sql);
        }

        if ($rs === false) {
            throw new Exception(self::$_db->ErrorMsg());
        }

        if ($auditoria)
            ActiveRecord::_salvarAuditoria($sql, $params);

        return $rs;
    }


    /**
     * 
     */
    static final protected function execSQL($sql, array $params = array(), $auditoria = true)
    {
        self::_connect();

        if (count($params)) {
            $rs = self::$_db->Execute($sql, $params);
        } else {
            $rs = self::$_db->Execute($sql);
        }

        if ($rs === false) {
            throw new Exception(self::$_db->ErrorMsg(),
                                self::$_db->ErrorNo());
        }

        if ($auditoria)
            ActiveRecord::_salvarAuditoria($sql, $params);

        return $rs;
    }


    //----------------------------------------------------------------- private
    /**
     * 
     */
    private function _inserir()
    {
        $campos  = array();
        $valores = array();
        $binds   = array();

        foreach ($this->campos as $campo => $valor) {
            if ($valor !== null) {
                if ($valor === 'null') {
                    //$valores[] = "null";
                    $binds[]   = "null";
                } else {
                    $binds[]   = "?";
                    $valores[] = trim($valor);
                }

                $campos[]  = $campo;
            }
        }

        $sql = sprintf("INSERT INTO %s (%s) VALUES (%s)",
                       $this->tabela,
                       implode(", ", $campos),
                       implode(", ", $binds));

        $ret                    = $this->Execute($sql, $valores);
        $this->chavePrimaria[1] = $this->insertId();

        return $ret;
    }


    /**
     * 
     */
    private function _alterar()
    {
        if ($this->chavePrimaria[0] === null) {
            throw new Exception("Não existe chave primária definida para "
                               ."a entidade " . get_class($this) . ".");
        }

        $campos  = array();
        $valores = array();

        foreach ($this->campos as $campo => $valor) {
            if ($valor !== null) {
                if ($valor === 'null') {
                    $campos[]  = $campo . " = null";
                } else {
                    $campos[]  = $campo . " = ?";
                    $valores[] = trim($valor);
                }
            }
        }

        $valores[] = $this->chavePrimaria[1];

        $sql = sprintf("UPDATE %s SET %s WHERE %s = ?",
                       $this->tabela,
                       implode(", ", $campos),
                       $this->chavePrimaria[0]);

        return $this->Execute($sql, $valores);
    }


    /**
     * Ajeitar isso pra usar as funções pg_* (ou PDO?)
     * ADO mesmo :)
     *
     */
    static private function _connect()
    {
        if (!defined('_ADODB_LAYER')) {
            require_once "../adodb/adodb.inc.php";
        }

        //require_once "../global/config.inc";

        if (self::$_db === null) {
            self::$_db = &ADONewConnection(self::DRIVER);
            self::$_db->Connect($GLOBALS['servidor_bd'],
                                $GLOBALS['usuario_db'],
                                $GLOBALS['senha_bd'],
                                $GLOBALS['nome_bd'],
                                $GLOBALS['porta_bd']);
        }

        pg_set_client_encoding('LATIN5');
    }


    static private function _salvarAuditoria($sql, $params)
    {
        preg_match('/(SELECT.*FROM|INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+([A-Za-z0-1.]+).*/smiu', $sql, $matches);

        $audtipo = substr($matches[1], 0, 1);

        if ($audtipo == 'I' || $audtipo == 'D' || $audtipo == 'U') {
            $audtabela = $matches[2];

            $_sql= 'insert into seguranca.auditoria (usucpf,
                                                     mnuid,
                                                     audsql,
                                                     audtabela,
                                                     audtipo,
                                                     audip,
                                                     sisid,
                                                     auddata) values (?, ?, ?, ?, ?, ?, ?, now())';

            $sql = str_replace(array("  ", "\r\n", "\n", "\t"), " ", $sql);
            $sql = vsprintf(str_replace("?", "'%s'", $sql), $params);

            while (strpos($sql, "  "))
                $sql = str_replace("  ", " ", $sql);

            self::$_db->Execute($_sql, array((string)  $_SESSION['usucpforigem'],
                                             (string)  $_SESSION['mnuid'],
                                             (string)  $sql,
                                             (string)  $audtabela,
                                             (string)  $audtipo,
                                             (string)  $_SERVER['REMOTE_ADDR'],
                                             (integer) $id));
        }
    }
}