<?php
//
// $Id
//



class Decorator {
    //-------------------------------------------------------------- properties
    /**
     * @var array
     */
    static protected $conf = array();

    /**
     * 
     */
    private $_plugins      = array();


    //------------------------------------------------------------------ public
    /**
     * 
     */
    final public function __construct()
    {
    }


    /**
     * @param ActiveRecord|array Instancia de um AR ou um array de AR's
     * @return void
     */
    static public function decorate($ar, array $conf = array())
    {
        Decorator::setConf($conf);

        if (sizeof(self::$conf) == 0) {
            throw new Exception('Você deve setar a configuração para a exibição dos dados (Decorator::setConf).');
        }

        if ($ar instanceof ActiveRecord) {
            return Decorator::block($ar);
        } elseif (is_array($ar)) {
            return Decorator::listing($ar);
        } else {
            throw new Exception('O parâmetro do método Decorator::decorate deve '
                               .'ser a instancia de um ActiveRecord ou um array.');
        }
    }


    /**
     * 
     */
    static public function setConf(array $conf)
    {
        self::$conf = array_merge_recursive(self::$conf, $conf);
    }


    /**
     * 
     */
    static public function resetConf()
    {
        self::$conf = array();
    }


    /**
     * 
     */
    public function __call($method, $params)
    {
        return $this->loadPlugin(ucfirst($method));
    }


    //--------------------------------------------------------------- protected
    /**
     * 
     */
    static protected function block(ActiveRecord $ar)
    {
        $table = "<table class=\"tabela\" bgcolor=\"#fafafa\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"table_" .$ar->getPrimaryKey(). "\">\n"
               . "  <colgroup>\n"
               . "    <col style=\"width: 25%\" />\n"
               . "    <col style=\"width: 75%\" />\n"
               . "  </colgroup>\n";

        foreach (self::$conf as $dados) {
            if ($dados[1] !== null) {
                $table  .= "  <tr>\n"
                        .  "    <td align=\"right\" class=\"SubTituloDireita\"><label for=\"" .$dados[0]. "\">" .$dados[1]. ":</label></td>\n"
                        .  "    <td>";

                if ($dados[2]) {
                    $table .= call_user_func($dados[2], (is_array($dados[0] ? $dados[0] : $ar->$dados[0])));
                } else {
                    $table .= $ar->$dados[0];
                }

                $table .= "</td>\n  </tr>\n";
            }
        }

        $table .= "</table>\n";

        return $table;
    }


    /**
     * 
     */
    static public function decorateTest(ActiveRecord $ar)
    {
        /**
         * $labels = array('label'          => null,
         *                 // Valor padrao da célula
         *                 'valor'          => null,
         *
         *                 // Campo do AR que contem o dado a ser retornado
         *                 'campo'          => null,
         *
         *                 // Callback a ser chamado para formatar o valor resultante
         *                 'callback'       => null,
         *
         *                 // Não funfa ;)
         *                 'paramsCallback' => array());
         //                                                                  */

        $table = "<table class=\"tabela\" bgcolor=\"#fafafa\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"table_" . $ar->getPrimaryKey() . "\">\n"
               . "  <colgroup>\n"
               . "    <col style=\"width: 25%\" />\n"
               . "    <col style=\"width: 75%\" />\n"
               . "  </colgroup>\n";

        foreach (self::$conf as $conf) {
            if (array_key_exists('label', $conf)) {
                $valor   = '';
                $table  .= "  <tr>\n"
                        .  "    <td align=\"right\" class=\"SubTituloDireita\"><label>" .$conf['label']. ":</label></td>\n"
                        .  "    <td>";

                if (array_key_exists('valor', $conf)) {
                    $valor = $conf['valor'];
                }

                if (array_key_exists('campo', $conf) && (($campo = $ar->$conf['campo']) !== null)) {
                    $valor = $campo;
                }

                if (array_key_exists('callback', $conf) && is_callable($conf['callback'])) {
                    $valor = call_user_func($conf['callback'], $valor);
                }

                $table .= $valor . "\n    </td>\n  </tr>\n";
            }
        }

        $table .= "</table>\n";

        return $table;
    }


    /**
     * 
     */
    static public function form(ActiveRecord $ar, array $form, array $conf = array())
    {
        /**
         * $labels = array('label'          => null,
         *                 // Valor padrao do input|select|textarea
         *                 'valor'          => null,
         *
         *                 // Valor padrao do input|select|textarea retornado do banco de dados
         *                 'campo'          => null,
         *
         *                 // tipo de input
         *                 'callback'       => null,
         *
         *                 // Não funfa ;)
         *                 'paramsCallback' => array());
         //                                                                  */

        $form  = "<form action=\"" . $form['action'] . "\" method=\"post\" onsubmit=\"return " . $form['onsubmit'] . "(this, new Array('" . implode("', '", $form['input']) . "'))\">\n";

                 Decorator::resetLabels();
                 Decorator::setConf($conf);
        $form .= Decorator::decorateTest($ar);

        /*
        $table = "<table class=\"tabela\" bgcolor=\"#fafafa\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" id=\"table_" . $ar->getPrimaryKey() . "\">\n"
               . "  <colgroup>\n"
               . "    <col style=\"width: 25%\" />\n"
               . "    <col style=\"width: 75%\" />\n"
               . "  </colgroup>\n";

        foreach (self::$conf as $conf) {
            if (array_key_exists('label', $conf)) {
                $valor   = '';
                $table  .= "  <tr>\n"
                        .  "    <td align=\"right\" class=\"SubTituloDireita\"><label for=\"" . $conf['campo'] . "\">" .$conf['label']. ":</label></td>\n"
                        .  "    <td>";

                if (array_key_exists('valor', $conf)) {
                    $valor = $conf['valor'];
                }

                if (array_key_exists('campo', $conf) && (($campo = $ar->$conf['campo']) !== null)) {
                    $valor = $campo;
                }

                if (array_key_exists('callback', $conf) && is_callable($conf['callback'])) {
                    $valor = call_user_func($conf['callback'], $valor);
                }

                $table .= $valor . "\n    </td>\n  </tr>\n";
            }
        }
        //                                                                  */

        $form .= "</form>\n";

        return $form;
    }


    //----------------------------------------------------------------- private
    /**
     * 
     */
    private function loadPlugin($plugin)
    {
        if (!array_key_exists($plugin, $this->_plugins)) {

            $filename = APPRAIZ . "includes/Decorator/" . $plugin . ".php";

            if (file_exists($filename)) {
                require_once $filename;
            }

            if (class_exists($plugin)) {
                $ref = new ReflectionClass($plugin);
                $this->_plugins[$plugin] = $ref->newInstance();
            }
        }

        return $this->_plugins[$plugin];
    }
}





