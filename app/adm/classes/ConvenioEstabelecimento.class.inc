<?php

class ConvenioEstabelecimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_convenio_estabelecimento";

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
        'nome' => null,
        'cnpj' => null,
        'estabelecimento_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
        'ordem' => null,
        'codigo_operadora' => null
    );

    /**
     * @param $params
     * @param $estabelecimentoId
     * @throws Exception
     */
    public function salvarConvenios($params, $estabelecimentoId)
    {
        $convenios = is_array($params['convenio']) ? $params['convenio'] : array($params['convenio']);
        $this->excluirPorEstabelecimento($estabelecimentoId);

        foreach($convenios as $convenio){
            if(empty($convenio['convenio'])){
                continue;
            }

            $convenio['estabelecimento_id'] = $estabelecimentoId;

            $this->salvarConvenio($convenio);
            $this->setDadosNull();
        }
    }

    public function salvarConvenio($params)
    {
        list($params, $camposNulos) = $this->limparDadosVazios($params);

        $dados = array(
            'id' => $params['id'],
            'nome' => $params['nome'],
            'cnpj' => $params['cnpj'],
            'email' => $params['email'],
            'telefone' => $params['telefone'],
            'codigo_operadora' => $params['codigo_operadora'],
            'excluido' => 'f',
            'estabelecimento_id' => $params['estabelecimento_id'],
        );

        if(empty($params['id'])){
            $dados['ordem'] = $this->getMaxConveniosPorEstabelecimento($params['estabelecimento_id']) + 1;
        }

        $dados = $this->getDadosCarimbo($params, $dados);
        $id = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);


        $mContato = new ContatoConvenio();
        $mContato->excluirPorConvenio($id);
        if($params['contatos']){
            if(!is_array($params['contatos'])){
                $params['contatos'] = str_replace('\"', '"', $params['contatos']);
                $params['contatos'] = utf8_encode($params['contatos']);
                $params['contatos'] = utf8ArrayDecode(json_decode($params['contatos'], true));
            }

            foreach ($params['contatos'] as $contato){
                if(empty($contato['contato'])){
                    continue;
                }

                $contato['flg_principal'] = 'f';
                if($contato['tipo_contato_sigla'] === 'principal'){
                    $contato['tipo_contato_sigla'] = TipoContato::TIPO_TELEFONE_FIXO;
                    $contato['flg_principal'] = 't';
                }

                $contato['convenio_id'] = $id;
                $mContato->salvarContatos($contato);
                $mContato->setDadosNull();
            }
        }

        return $id;
    }

    /**
     * @param $estabelecimentoId
     * @throws Exception
     */
    public function excluirPorEstabelecimento($estabelecimentoId)
    {
        $sql = "UPDATE estabelecimentosaude.tb_convenio_estabelecimento SET excluido = TRUE WHERE estabelecimento_id = {$estabelecimentoId}";
        $this->executar($sql);
    }

    public function listarPorEstabelecimento($estabelecimentoId, $jsonEncode = false)
    {
        $rs = $this->recuperarTodos('id, nome', array("estabelecimento_id = {$estabelecimentoId}", "excluido IS FALSE"));
        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    public function getComboPorEstabelecimento($estabelecimentoId)
    {
        $rs = $this->listarPorEstabelecimento($estabelecimentoId);
        return array_map(function($convenio){
            return array(
                'codigo' => $convenio['id'],
                'descricao' => $convenio['nome']
            );
        }, $rs);
    }

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "con.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();

        $sql = "SELECT
                  con.codigo,
                  con.nome,
                  con.cnpj,
                  con.codigo_operadora,
                  contato_tel_principal.contato AS telefone_principal,
                  contato_tel.contato AS telefone_secundario,
                  contato_email.contato AS email,
                  con.ordem,
                  omin.ordem_min AS ordem_min,
                  omax.ordem_max AS ordem_max,
                  con.estabelecimento_id,
                  con.id
                FROM
                  estabelecimentosaude.tb_convenio_estabelecimento con
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento est ON con.estabelecimento_id = est.id
                LEFT JOIN 
                  estabelecimentosaude.tb_contato_convenio contato_tel_principal ON con.id = contato_tel_principal.convenio_id
                                                                 AND contato_tel_principal.flg_principal IS TRUE
                                                                 AND contato_tel_principal.tipo_contato_id = (SELECT id FROM corporativo.tb_tipo_contato  WHERE  sigla = '" . TipoContato::TIPO_TELEFONE_FIXO . "')
                LEFT JOIN 
                  estabelecimentosaude.tb_contato_convenio contato_email ON con.id = contato_email.convenio_id
                                                                        AND contato_email.tipo_contato_id = (SELECT id FROM corporativo.tb_tipo_contato WHERE sigla = '" . TipoContato::TIPO_EMAIL . "')
                LEFT JOIN 
                  estabelecimentosaude.tb_contato_convenio contato_tel ON con.id = contato_tel.convenio_id
                                                                      AND contato_tel.flg_principal IS FALSE
                                                                      AND contato_tel.tipo_contato_id = (SELECT id FROM corporativo.tb_tipo_contato WHERE sigla = '" . TipoContato::TIPO_TELEFONE_FIXO . "')
                LEFT JOIN
                    (SELECT 
                        MIN(cons.ordem) AS ordem_min, 
                        cons.estabelecimento_id 
                    FROM 
                        estabelecimentosaude.tb_convenio_estabelecimento cons 
                    WHERE 
                        cons.excluido IS FALSE 
                    GROUP BY 
                        cons.estabelecimento_id) omin ON omin.estabelecimento_id = con.estabelecimento_id
                LEFT JOIN
                    (SELECT 
                        MAX(cons.ordem) AS ordem_max, 
                        cons.estabelecimento_id 
                    FROM 
                        estabelecimentosaude.tb_convenio_estabelecimento cons 
                    WHERE 
                        cons.excluido IS FALSE 
                    GROUP BY 
                        cons.estabelecimento_id) omax ON omax.estabelecimento_id = con.estabelecimento_id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY con.ordem";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 20);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function subir($convenioId)
    {
        $dadosSubir = $this->recuperarLinha('*', array("id = {$convenioId}"), false);
        $minConvenios = $this->getMinConveniosPorEstabelecimento($dadosSubir['estabelecimento_id']);

        if($dadosSubir['ordem'] === $minConvenios){
            return;
        }

        $dadosDescer = $this->pegaLinha("SELECT 
                                                  * 
                                              FROM 
                                                  estabelecimentosaude.tb_convenio_estabelecimento
                                              WHERE
                                                  ordem < {$dadosSubir['ordem']}
                                                  AND
                                                  excluido IS FALSE
                                                  AND
                                                  estabelecimento_id = {$dadosSubir['estabelecimento_id']}
                                              ORDER BY
                                                  ordem DESC");

        $this->manter(array(
            'id' => $dadosDescer['id'],
            'ordem' => $dadosSubir['ordem']
        ), array());
        $this->setDadosNull();

        $this->manter(array(
            'id' => $dadosSubir['id'],
            'ordem' => $dadosDescer['ordem']
        ), array());
        $this->setDadosNull();
    }

    public function descer($convenioId)
    {
        $dadosDescer = $this->recuperarLinha('*', array("id = {$convenioId}"), false);
        $maxConvenios = $this->getMaxConveniosPorEstabelecimento($dadosDescer['estabelecimento_id']);

        if($dadosDescer['ordem'] === $maxConvenios){
            return;
        }

        $dadosSubir = $this->pegaLinha("SELECT 
                                                 * 
                                             FROM 
                                                 estabelecimentosaude.tb_convenio_estabelecimento
                                             WHERE
                                                 ordem > {$dadosDescer['ordem']}
                                                 AND
                                                 excluido IS FALSE
                                                 AND
                                                 estabelecimento_id = {$dadosDescer['estabelecimento_id']}
                                             ORDER BY
                                                 ordem ASC");

        $this->manter(array(
            'id' => $dadosSubir['id'],
            'ordem' => $dadosDescer['ordem']
        ), array());
        $this->setDadosNull();

        $this->manter(array(
            'id' => $dadosDescer['id'],
            'ordem' => $dadosSubir['ordem']
        ), array());
        $this->setDadosNull();
    }

    public function getTotalConveniosPorEstabelecimento($estabelecimentoId)
    {
        return $this->recuperarUm('count(*)', array(
            "excluido IS FALSE",
            "estabelecimento_id = {$estabelecimentoId}"
        ));
    }

    public function getMaxConveniosPorEstabelecimento($estabelecimentoId)
    {
        return $this->recuperarUm('COALESCE(MAX(ordem), 0)', array(
            "excluido IS FALSE",
            "estabelecimento_id = {$estabelecimentoId}"
        ));
    }

    public function getMinConveniosPorEstabelecimento($estabelecimentoId)
    {
        return $this->recuperarUm('COALESCE(MIN(ordem), 0)', array(
            "excluido IS FALSE",
            "estabelecimento_id = {$estabelecimentoId}"
        ));
    }

    public function copiarDados($origem,$destino, $desabilita = false)
    {
        $mContato = new ContatoConvenio();

        if($desabilita){
            $this->excluirPorEstabelecimento($destino);
        }

        $sql = "SELECT
                 ce.*
               FROM
                 estabelecimentosaude.tb_convenio_estabelecimento ce
               WHERE
                 ce.excluido IS FALSE AND
                 ce.estabelecimento_id = '{$origem}';";

        $dados = $this->carregar($sql);

        $arReferenciaChaves = array();
        if (is_array($dados) && count($dados) > 0) {
            foreach ($dados as $index => $item) {
                $arReferenciaChaves[$item['id']] = $this->manter(array(
                    'estabelecimento_id' => $destino,
                    'nome' => $item['nome'],
                    'criado_em' => 'now()',
                    'criado_por' => $_SESSION['usuario_id'],
                    'excluido' => 'false',
                    'cnpj' => $item['cnpj'],
                    'codigo_operadora' => $item['codigo_operadora'],
                    'ordem' => $item['ordem']
                ));
                $this->setDadosNull();
                $mContato->copiarDados($item['id'],$arReferenciaChaves[$item['id']]);
            }
        }

        return $arReferenciaChaves;
    }

    public static function isVisualizaConvenio($estabelecimentoId)
    {
        return UsuarioResponsabilidadeEstabelecimento::isPossuiPerfilEstabelecimento($estabelecimentoId, array(
            ADM_PERFIL_LOCAL,
            ADM_PERFIL_GERAL,
            ADM_PERFIL_RECEPCIONISTA
        ));
    }

    public function getByEstabelecimento($estabelecimentoId)
    {
        return $this->recuperarTodos('id AS codigo, nome AS descricao', ["excluido IS FALSE", "estabelecimento_id = {$estabelecimentoId}"], "ordem ASC");
    }


    public function getConveniosComGuiaByEstabelecimento($estabelecimentoId, $convenioEstabelecimentoIdRetornar = null)
    {
        $whereConvenioRetornar = !empty($convenioEstabelecimentoIdRetornar) ? " OR id = {$convenioEstabelecimentoIdRetornar}" : '';

        return $this->recuperarTodos('id AS codigo, nome AS descricao', [
            "excluido IS FALSE",
            "estabelecimento_id = {$estabelecimentoId}",
            "(
                id IN (
                    SELECT 
                        convenio_estabelecimento_id 
                    FROM 
                        estabelecimentosaude.tb_guia gui
                    WHERE
                        gui.excluido IS FALSE
                        AND
                        gui.id NOT IN (
                            SELECT 
                                lgu.guia_id 
                            FROM
                                estabelecimentosaude.tb_lote_guia lgu
                            INNER JOIN
                                estabelecimentosaude.tb_lote lot ON lgu.lote_id = lot.id AND lot.excluido IS FALSE
                            WHERE
                                lgu.excluido IS FALSE
                        )
                )
                {$whereConvenioRetornar}
            )"
        ], "ordem ASC");
    }
}