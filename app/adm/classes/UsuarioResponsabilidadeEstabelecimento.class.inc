<?php

class UsuarioResponsabilidadeEstabelecimento extends UsuarioResponsabilidade
{
    private static $responsabilidadeEstabelecimento = [];
    private static $responsabilidadeEstabelecimentoUrl = [];
    private static $possuiPerfilEstabelecimento = [];

    public function listarEstabelecimentos($usucpf, $pflcod, $jsonEncode = false)
    {
        $usucpf = str_replace(array('.', '-'), '', $usucpf);

        $sql = "SELECT
                    pj.nome_fantasia,
                    pj.cnpj,
                    est.id,
                    CASE WHEN ur.rpuid IS NOT NULL THEN TRUE ELSE FALSE END AS selecionado
                FROM
                    estabelecimentosaude.tb_estabelecimento est
                LEFT JOIN 
                    corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                LEFT JOIN
                    seguranca.usuarioresponsabilidade ur ON ur.estabelecimento_id = est.id
                                                        and ur.pflcod = {$pflcod}
                                                        AND ur.rpustatus = 'A'
                                                        AND ur.usucpf = '{$usucpf}'
                WHERE
                    est.excluido IS FALSE
                ORDER BY
                    1, 2";

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();
        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    /**
     * @param $params
     * @throws Exception
     */
    public function salvarResponsabilidadeEstabelecimento($params)
    {
        $usucpf = $params['usucpf'];
        $pflcod = $params['pflcod'];

        $this->excluirPorUsuarioPerfil($usucpf, $pflcod);

        if (isset($params['estabelecimentos']) && is_array($params['estabelecimentos'])) {
            foreach ($params['estabelecimentos'] as $estabelecimento) {
                $this->atribuirResponsabilidadeEstabelecimento($usucpf, $pflcod, $estabelecimento);
                $this->setDadosNull();
            }
        }
    }

    public function atribuirResponsabilidadeEstabelecimento($usucpf, $pflcod, $estabelecimentoId)
    {
        $responsabilidadeEstabelecimento = array(
            'rpuid' => $this->recuperarUm('rpuid', array("usucpf = '{$usucpf}'", "pflcod = {$pflcod}", "estabelecimento_id = {$estabelecimentoId}")),
            'pflcod' => $pflcod,
            'usucpf' => $usucpf,
            'rpustatus' => 'A',
            'rpudata_inc' => 'now()',
            'estabelecimento_id' => $estabelecimentoId,
        );

        return $this->popularDadosObjeto($responsabilidadeEstabelecimento)->salvar();
    }

    public static function gerarWhereResponsabilidadeEstabelecimento($aliasTabela = 'est', $coluna = 'id', $perfisVerificarResp = array())
    {
        global $db;
        $where = array();
        $aliasTabela = !empty($aliasTabela) ? "{$aliasTabela}." : '';
        if (count($perfisVerificarResp) == 0) {
            $perfisVerificarResp = array(
                ADM_PERFIL_LOCAL,
                ADM_PERFIL_GERAL,
                ADM_PERFIL_REPRESENTANTE,
                ADM_PERFIL_RECEPCIONISTA
            );
        }

        if (in_array(ADM_PERFIL_LOCAL, $perfisVerificarResp) && possuiPerfil(array(ADM_PERFIL_LOCAL)) && !$db->testa_superuser()) {
            $where[] = "{$aliasTabela}{$coluna} IN (SELECT estabelecimento_id FROM seguranca.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}'
                                                                                                                       AND rpustatus = 'A'
                                                                                                                       AND pflcod IN (" . ADM_PERFIL_LOCAL . "))";
        }

        if (in_array(ADM_PERFIL_GERAL, $perfisVerificarResp) && possuiPerfil(array(ADM_PERFIL_GERAL)) && !$db->testa_superuser()) {
            $where[] = "{$aliasTabela}{$coluna} IN (SELECT estabelecimento_id FROM seguranca.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}'
                                                                                                                       AND rpustatus = 'A'
                                                                                                                       AND pflcod IN (" . ADM_PERFIL_GERAL . "))";
        }

        if (in_array(ADM_PERFIL_REPRESENTANTE, $perfisVerificarResp) && possuiPerfil(array(ADM_PERFIL_REPRESENTANTE)) && !$db->testa_superuser()) {
            $where[] = "{$aliasTabela}{$coluna} IN (SELECT estabelecimento_id FROM seguranca.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}'
                                                                                                                       AND rpustatus = 'A'
                                                                                                                       AND pflcod IN (" . ADM_PERFIL_REPRESENTANTE . "))";
        }

        if (in_array(ADM_PERFIL_RECEPCIONISTA, $perfisVerificarResp) && possuiPerfil(array(ADM_PERFIL_RECEPCIONISTA)) && !$db->testa_superuser()) {
            $where[] = "{$aliasTabela}{$coluna} IN (SELECT estabelecimento_id FROM seguranca.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}'
                                                                                                                       AND rpustatus = 'A'
                                                                                                                       AND pflcod IN (" . ADM_PERFIL_RECEPCIONISTA . "))";
        }

        $where = count($where) == 0 ? array('true') : $where;
        return '(' . implode(' OR ', $where) . ')';
    }

    public static function possuiResponsabilidadeEstabelecimento($estabelecimentoId)
    {
        global $db;
        $possuiResponsabilidade = false;

        if ($estabelecimentoId) {
            if (!isset(self::$responsabilidadeEstabelecimento[$estabelecimentoId])) {
                $sql = "SELECT 
                    COUNT(1)
                FROM
                    estabelecimentosaude.tb_estabelecimento
                WHERE
                    id = {$estabelecimentoId}
                    AND 
                    " . self::gerarWhereResponsabilidadeEstabelecimento('');

                $result = $db->pegaUm($sql);
                $possuiResponsabilidade = $result > 0;

                self::$responsabilidadeEstabelecimento[$estabelecimentoId] = $possuiResponsabilidade;
            } else {
                $possuiResponsabilidade = self::$responsabilidadeEstabelecimento[$estabelecimentoId];
            }
        }

        return $possuiResponsabilidade;
    }

    public static function isPossuiResponsabilidadeEstabelecimento($estabelecimentoId, $redirect = true)
    {
        $possuiResponsabilidade = self::possuiResponsabilidadeEstabelecimento($estabelecimentoId);

        if (!$possuiResponsabilidade && $redirect) {
            $mensagem = "Você não pode acessar o estabelecimento informado.";
            addMsgDanger($mensagem);

            $html = <<<RETORNO
                <html>
                    <head>
                    <script type='text/javascript'>
                            history.back();
                    </script>
                    </head>
                    <body>
                    </body>
                </html>
RETORNO;
            die($html);
        }

        return $possuiResponsabilidade;
    }

    public static function isPossuiResponsabilidadeEstabelecimentoFuncionalidade($estabelecimentoId, $redirect = true, $url = null)
    {
        global $db;
        $possuiResponsabilidadeEstabelecimento = self::possuiResponsabilidadeEstabelecimento($estabelecimentoId);

        if (is_null($url)) {
            $url = str_replace('/' . $_SESSION['sisdiretorio'] . '/', '', $_SERVER['REQUEST_URI']);
            $posicao = strpos($url, '&acao=');
            $url = substr($url, 0, $posicao + 7);
        }

        if (!isset(self::$responsabilidadeEstabelecimentoUrl[$estabelecimentoId])) {
            self::$responsabilidadeEstabelecimentoUrl[$estabelecimentoId] = [];
        }

        if (!isset(self::$responsabilidadeEstabelecimentoUrl[$estabelecimentoId][$url])) {
            $sql = "SELECT 
                        COUNT(1)
                    FROM 
                        seguranca.usuarioresponsabilidade usr
                    INNER JOIN
                      seguranca.tprperfil tpr ON usr.pflcod = tpr.pflcod
                    INNER JOIN
                      seguranca.perfilmenu pfm ON pfm.pflcod = tpr.pflcod
                    INNER JOIN
                      seguranca.menu mnu ON mnu.mnuid = pfm.mnuid
                    INNER JOIN
                      seguranca.perfilusuario pfu ON pfm.pflcod = pfu.pflcod
                                                 AND usr.usucpf = pfu.usucpf
                    WHERE 
                        usr.rpustatus = 'A'
                        AND
                        trim(mnu.mnulink) = '{$url}'
                        AND
                        pfu.usucpf = '{$_SESSION["usucpf"]}'
                        AND 
                        mnu.sisid = {$_SESSION["sisid"]}
                        AND
                        usr.estabelecimento_id = {$estabelecimentoId}
                        AND (" . self::gerarWhereResponsabilidadeEstabelecimento('usr', 'estabelecimento_id') . ")";
            $possuiResponsabilidade = ($possuiResponsabilidadeEstabelecimento && $db->pegaUm($sql) > 0) || $db->testa_superuser();

            self::$responsabilidadeEstabelecimentoUrl[$estabelecimentoId][$url] = $possuiResponsabilidade;
        } else {
            return self::$responsabilidadeEstabelecimentoUrl[$estabelecimentoId][$url];
        }

        if (!$possuiResponsabilidade && $redirect) {
            $mensagem = "Você não pode acessar as informações solicitadas.";
            addMsgDanger($mensagem);
            ob_clean();

            header('Location: ' . $_SERVER['HTTP_REFERER']);
            die();
            /*$html = <<<RETORNO
                <html>
                    <head>
                    <script type='text/javascript'>
                            history.go(-1);
                    </script>
                    </head>
                    <body>
                    </body>
                </html>
RETORNO;
            die($html);*/
        }

        return $possuiResponsabilidade;
    }

    public static function isPossuiPerfilEstabelecimento($estabelecimentoId, $perfis)
    {
        global $db;

        if ($db->testa_superuser()) {
            return true;
        }

        if (is_array($perfis)) {
            $pflcods = array_map("intval", $perfis);
            $pflcods = array_unique($pflcods);
        } else {
            $pflcods = array((integer)$perfis);
        }

        if (count($pflcods) == 0) {
            return false;
        }

        $perfilIdentificador = implode('_', $perfis);
        if(!isset(self::$possuiPerfilEstabelecimento[$estabelecimentoId])){
            self::$possuiPerfilEstabelecimento[$estabelecimentoId] = [];
        }

        if(!isset(self::$possuiPerfilEstabelecimento[$estabelecimentoId][$perfilIdentificador])){
            $sql = "SELECT
                        COUNT(*)
                    FROM 
                        seguranca.perfilusuario pfu
                    INNER JOIN
                          seguranca.tprperfil tpr ON pfu.pflcod = tpr.pflcod
                    INNER JOIN
                          seguranca.usuarioresponsabilidade usr ON usr.pflcod = pfu.pflcod
                                                               AND usr.rpustatus = 'A'
                                                               AND usr.usucpf = pfu.usucpf
                                                               AND usr.estabelecimento_id = {$estabelecimentoId}
                    WHERE
                        pfu.usucpf = '{$_SESSION['usucpf']}' 
                        AND
                        pfu.pflcod IN ( " . implode(",", $pflcods) . " )";

            self::$possuiPerfilEstabelecimento[$estabelecimentoId][$perfilIdentificador] = $db->pegaUm($sql) > 0;
        }

        return self::$possuiPerfilEstabelecimento[$estabelecimentoId][$perfilIdentificador];
    }

    public static function getUsuariosPossuemResponsabilidadeEstabelecimento($estabelecimentoId, $retornarExcluidos = false)
    {
        global $db;
        $whereExcluidos = $retornarExcluidos ? "" : "usr.rpustatus = 'A' AND ";

        $sql = "SELECT
                    usr.usucpf
                FROM
                    seguranca.usuarioresponsabilidade usr
                WHERE
                    {$whereExcluidos}
                    estabelecimento_id = {$estabelecimentoId}";

        $rs = $db->carregarColuna($sql);
        return $rs ? $rs : array();
    }

    public static function getDadosUsuariosPossuemResponsabilidadeEstabelecimento($estabelecimentoId, $retornarExcluidos = false)
    {
        global $db;
        $whereExcluidos = $retornarExcluidos ? "" : "usr.rpustatus = 'A' AND ";

        $sql = "SELECT DISTINCT
                    usu.*
                FROM
                    seguranca.usuarioresponsabilidade usr
                JOIN
                    seguranca.usuario usu ON usu.usucpf = usr.usucpf
                WHERE
                    {$whereExcluidos}
                    estabelecimento_id = {$estabelecimentoId}";

        $rs = $db->carregar($sql);
        return $rs ? $rs : array();
    }

    public function getPerfisComResponsabilidadeEstabelecimento($usucpf, $estabelecimentoId)
    {
        $sql = "SELECT
                    per.pflcod
                FROM
                    seguranca.perfil per
                INNER JOIN
                    seguranca.perfilusuario pus ON per.pflcod = pus.pflcod
                                               AND pus.usucpf = '{$usucpf}'
                INNER JOIN
                    seguranca.usuarioresponsabilidade usr ON usr.pflcod = per.pflcod
                                                         AND usr.usucpf = pus.usucpf
                                                         AND usr.rpustatus = 'A'
                                                         AND usr.estabelecimento_id = {$estabelecimentoId}
                WHERE
                    per.pflstatus = 'A'
                    AND
                    per.sisid = {$_SESSION['sisid']}";
        $rs = $this->carregarColuna($sql);

        return $rs ? $rs : array();
    }

    public static function criaAbam($abacodTela, $url, $parametros, array $arMnuid = array(), $numerarAbas = false, $adicionarAbaAtual = false)
    {
        global $db;
        if (is_string($parametros)) {
            $parametros = str_replace('&p=', '', $parametros);
            $parametros = getParam($parametros);
        }

        $menusOcultar = array();
        if (!$db->testa_superuser()) {
            $estabelecimentoId = $parametros['estabelecimento_id'] ? $parametros['estabelecimento_id'] : $parametros['id'];

            $sql = "SELECT 
                      mnua.mnuid 
                    FROM 
                      seguranca.menu mnua 
                    INNER JOIN 
                      seguranca.aba_menu abma ON abma.mnuid = mnua.mnuid
                    WHERE 
                      abma.abacod = '{$abacodTela}'
                      AND 
                      mnua.sisid = '{$_SESSION["sisid"]}'
                      AND
                      mnua.mnuid NOT IN ( 
                        SELECT 
                            mnu.mnuid
                        FROM 
                            seguranca.usuarioresponsabilidade usr
                        INNER JOIN
                          seguranca.tprperfil tpr ON usr.pflcod = tpr.pflcod
                        INNER JOIN
                          seguranca.perfilmenu pfm ON pfm.pflcod = tpr.pflcod
                        INNER JOIN
                          seguranca.menu mnu ON mnu.mnuid = pfm.mnuid
                        INNER JOIN 
                          seguranca.aba_menu abm ON abm.mnuid = mnu.mnuid
                        INNER JOIN
                          seguranca.perfilusuario pfu ON pfm.pflcod = pfu.pflcod
                                                     AND usr.usucpf = pfu.usucpf
                        WHERE 
                            usr.rpustatus = 'A'
                            AND
                            pfu.usucpf = '{$_SESSION["usucpf"]}'
                            AND 
                            mnu.sisid = '{$_SESSION["sisid"]}'
                            AND
                            abm.abacod = '{$abacodTela}'
                            AND
                            usr.estabelecimento_id = '{$estabelecimentoId}'
                            AND 
                            (" . self::gerarWhereResponsabilidadeEstabelecimento('usr', 'estabelecimento_id') . "))";
            $menusOcultar = $db->carregarColuna($sql);
        }

        $db->cria_abam($abacodTela, $url, setParam($parametros), $menusOcultar, $numerarAbas, $adicionarAbaAtual);
    }

    public static function montaTituloAba($abacodTela = null, $url = null, $parametros = null, $tituloModulo = null, array $arMnuid = array(), $numerarAbas = false, $adicionarAbaAtual = false)
    {
        global $db;
        if (is_string($parametros)) {
            $parametros = str_replace('&p=', '', $parametros);
            $parametros = getParam($parametros);
        }

        $menusOcultar = array();
        if (!$db->testa_superuser()) {
            $estabelecimentoId = $parametros['estabelecimento_id'] ? $parametros['estabelecimento_id'] : $parametros['id'];

            $sql = "SELECT 
                      mnua.mnuid 
                    FROM 
                      seguranca.menu mnua 
                    INNER JOIN 
                      seguranca.aba_menu abma ON abma.mnuid = mnua.mnuid
                    WHERE 
                      abma.abacod = '{$abacodTela}'
                      AND 
                      mnua.sisid = '{$_SESSION["sisid"]}'
                      AND
                      mnua.mnuid NOT IN ( 
                        SELECT 
                            mnu.mnuid
                        FROM 
                            seguranca.usuarioresponsabilidade usr
                        INNER JOIN
                          seguranca.tprperfil tpr ON usr.pflcod = tpr.pflcod
                        INNER JOIN
                          seguranca.perfilmenu pfm ON pfm.pflcod = tpr.pflcod
                        INNER JOIN
                          seguranca.menu mnu ON mnu.mnuid = pfm.mnuid
                        INNER JOIN 
                          seguranca.aba_menu abm ON abm.mnuid = mnu.mnuid
                        INNER JOIN
                          seguranca.perfilusuario pfu ON pfm.pflcod = pfu.pflcod
                                                     AND usr.usucpf = pfu.usucpf
                        WHERE 
                            usr.rpustatus = 'A'
                            AND
                            pfu.usucpf = '{$_SESSION["usucpf"]}'
                            AND 
                            mnu.sisid = '{$_SESSION["sisid"]}'
                            AND
                            abm.abacod = '{$abacodTela}'
                            AND
                            usr.estabelecimento_id = '{$estabelecimentoId}'
                            AND 
                            (" . self::gerarWhereResponsabilidadeEstabelecimento('usr', 'estabelecimento_id') . "))";
            $menusOcultar = $db->carregarColuna($sql);
        }

        montaTituloAba($abacodTela, $url, setParam($parametros), $tituloModulo, $menusOcultar, $numerarAbas, $adicionarAbaAtual);
    }
}