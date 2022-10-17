<?php

class Profissional extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "profissionalsaude.tb_profissional";

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
        'tipo_profissional_id' => null,
        'registro_profissional' => null,
        'uf_registro' => null,
        'data_registro' => null,
        'curriculo' => null,
        'pessoafisica_id' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null
    );

    public function listarPorPessoaFisica($pessoaFisicaId, $jsonEncode = false, $estabelecimentoId = null)
    {

        $joinEstabelecimento = '';
        if($estabelecimentoId){
            $joinEstabelecimento = "INNER JOIN
                                        profissionalsaude.tb_especialidade_profissional ep ON ep.profissional_id = p.id 
                                                                                            AND ep.excluido IS FALSE
                                    INNER JOIN
                                        estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON eep.especialidade_profissional_id = ep.id 
                                                                                                              AND eep.excluido IS FALSE
                                                                                                              AND eep.estabelecimento_id = {$estabelecimentoId}";
        }

        $sql = "SELECT DISTINCT
                    p.id, 
                    p.registro_profissional, 
                    tp.descricao AS tipo_profissional, 
                    p.tipo_profissional_id,
                    uf.uf_sigla, 
                    uf.uf_nome_estado AS uf_descricao,
                    p.data_registro,
                    p.curriculo
                FROM
                    profissionalsaude.tb_profissional p
                {$joinEstabelecimento}    
                LEFT JOIN
                    corporativo.tb_unidade_federacao uf ON p.uf_registro = uf.uf_sigla
                LEFT JOIN
                    profissionalsaude.tb_tipo_profissional tp ON tp.id = p.tipo_profissional_id
                WHERE
                    p.pessoafisica_id = {$pessoaFisicaId}
                    AND
                    p.excluido IS FALSE";

        $profissionais = $this->carregar($sql);
        $profissionais = $profissionais ? $profissionais : array();


        $profissionais = array_map(function($profissional) use($estabelecimentoId) {
            $mEspecialidadeProfissional = new EspecialidadeProfissional();
            $especialidades = $mEspecialidadeProfissional->listarPorProfissional($profissional['id'], false, $estabelecimentoId);

            $arIdsEspecialidades = array();
            $arEspecialidades = array();
            if(is_array($especialidades) && count($especialidades) > 0){
                foreach ($especialidades AS $especialidade){
                    $arIdsEspecialidades[] = array(
                        'especialidade' => $especialidade['especialidade_id'],
                        'rqe' => $especialidade['rqe'],
                        'duracao_consulta' => $especialidade['duracao_consulta']
                    );
                    $arEspecialidades[] = $especialidade['especialidade'];
                }
            }

            $profissional['jsonEspecialidades'] = $arIdsEspecialidades;
            $profissional['especialidades'] = $arEspecialidades;
            $profissional['data_registro'] = formata_data($profissional['data_registro']);

            return $profissional;
        }, $profissionais);

        return $jsonEncode ? json_encode(utf8ArrayEncode($profissionais)) : $profissionais;
    }

    public function salvarProfissional($params)
    {
        $camposNulos = array();
        foreach ($params as $campo => $valor){
            if($valor === ''){
                $params[$campo] = null;
                $camposNulos[] = $campo;
            }
        }

        //Limpando os profissionais da pessoa física caso não seja uma alteração feita a partir da tela de um estabelecimento
        if(!$params['estabelecimento_id']){
            $this->excluirPorPessoaFisica($params['pessoafisica_id']);
        }

        //Limpando o vinculo entre estabelecimento e especialidade profissional (será recadastrado mais pra frente)
        $mEstabelecimentoProfissional = new EstabelecimentoEspecialidadeProfissional();
        if($params['estabelecimento_id']){
            $mEstabelecimentoProfissional->excluirPorPessoaFisicaEstabelecimento($params['pessoafisica_id'], $params['estabelecimento_id']);
        }


        if($params['infoprofissionais']){
            $params['infoprofissionais'] = str_replace('\"', '"', $params['infoprofissionais']);
            $params['infoprofissionais'] = utf8_encode($params['infoprofissionais']);
            $params['infoprofissionais'] = utf8ArrayDecode(json_decode($params['infoprofissionais'], true));

            $mEstabelecimento = new Estabelecimento();
            $estabelecimentos = array();
            if($params['estabelecimento_id']){
                $whereEstabelecimento = array("est.id = {$params['estabelecimento_id']}");
                $estabelecimentos = $mEstabelecimento->getList($whereEstabelecimento, false);
            }

            foreach ($params['infoprofissionais'] as $infoprofissional){

                $infoprofissional['pessoafisica_id'] = $params['pessoafisica_id'];
                $infoprofissional['uf_registro'] = $infoprofissional['uf_sigla'];
                $infoprofissional['excluido'] = 'f';

                $infoprofissional['id'] = $this->recuperarUm('id', array(
                    "tipo_profissional_id = {$infoprofissional['tipo_profissional_id']}",
                    "pessoafisica_id = {$infoprofissional['pessoafisica_id']}",
                ));

                if(empty($infoprofissional['id'])){
                    $dadosCarimbo = array(
                        'criado_em' => 'now()',
                        'criado_por' => $_SESSION['usuario_id'],
                    );
                }else{
                    $dadosCarimbo = array(
                        'atualizado_em' => 'now()',
                        'atualizado_por' => $_SESSION['usuario_id'],
                    );
                }

                if($infoprofissional['data_registro']){
                    $infoprofissional['data_registro'] = formata_data_sql($infoprofissional['data_registro']);
                }else{
                    $infoprofissional['data_registro'] = null;
                    $camposNulos[] = 'data_registro';
                }

                $infoprofissional = array_merge($infoprofissional, $dadosCarimbo);
                $infoprofissional['curriculo'] = $params['curriculo'];
                
                $id = $this->popularDadosObjeto($infoprofissional)->salvar(true, true, $camposNulos);
                $this->setDadosNull();

                //Limpando todas as especialidades profissional caso não seja uma alteração feita a partir da tela de um estabelecimento
                $mEspecialidadeProfissional = new EspecialidadeProfissional();
                if(!$params['estabelecimento_id']){
                    $mEspecialidadeProfissional->excluirPorProfissional($id);
                }

                //Gravando  a relação com a especialidade
                if(!empty($infoprofissional['especialidades'])){
                    $arEspecialidades = $infoprofissional['especialidades'];
                    if(is_array($arEspecialidades) && count($arEspecialidades) > 0){
                        foreach ($arEspecialidades AS $especialidade){
                            $arDado = array(
                                'id' => $mEspecialidadeProfissional->recuperarUm('id', array("especialidade_id = {$especialidade['especialidade']}", "profissional_id = {$id}")),
                                'especialidade_id' => $especialidade['especialidade'],
                                'rqe' => $especialidade['rqe'],
                                'profissional_id' => $id,
                                'criado_em' => 'now()',
                                'criado_por' => $_SESSION['usuario_id'],
                                'excluido' => 'f'
                            );

                            $idEspecialidadeProfissional = $mEspecialidadeProfissional->salvarEspecialidadeProfissional($arDado);
                            $mEspecialidadeProfissional->setDadosNull();

                            // Grava a especialidade nas responsabilidades do usuário
                            foreach ($estabelecimentos as $estabelecimento){
                                $mEstabelecimentoProfissional->salvarEspecialidade(array(
                                    'estabelecimento_id' => $estabelecimento['id'],
                                    'especialidade_profissional_id' => $idEspecialidadeProfissional,
                                    'duracao_consulta' => $especialidade['duracao_consulta'] ? $especialidade['duracao_consulta'] : 5,
                                ));

                                $mEstabelecimentoProfissional->setDadosNull();
                            }
                        }
                    }
                }
            }

            $usucpf = str_replace(array('-', '.'), '', $params['cpf']);
            $email = isset($params['contatos']['EM']['contato']) ? $params['contatos']['EM']['contato'] : null;

            $mUsuario = new Usuario();
            $dadosUsarios = $mUsuario->recuperarTodos('*', array("(usucpf = '{$usucpf}')"));

            if(count($dadosUsarios) == 0){
                $userParams = array(
                    'usucpf' => $params['cpf'],
                    'usudatanascimento' => $params['data_nascimento'],
                    'usunome' => $params['nome_completo'],
                    'ususexo' => $params['sexo'],
                    'senha_padrao' => true,
                    'perfil' => array(ADM_PERFIL_PROFISSIONAL_SAUDE)
                );

                $mUsuario->salvarUsuario($userParams);
            }elseif(count($dadosUsarios) == 1){
                $mUsuario->atribuirPerfil($dadosUsarios[0]['usucpf'], ADM_PERFIL_PROFISSIONAL_SAUDE);
            }
        }
    }

    public function getList($arWhere = array(), $paginacao = true, $order = null, $perfisVerificarResp = array())
    {
        $arWhere[] = "pes.excluido IS FALSE";
        $arWhere[] = self::getWhereResponsabilidadeEstabelecimento('pes', 'id', $perfisVerificarResp);

        $sql = "SELECT DISTINCT
                  Coalesce((SELECT CASE
                       WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                         THEN tp.pron_trat_masc || ' '
                       WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                         THEN tp.pron_trat_fem || ' '
                       ELSE ''
                       END AS pronome
                    FROM profissionalsaude.tb_profissional prof
                      INNER JOIN profissionalsaude.tb_tipo_profissional tp
                        ON prof.tipo_profissional_id = tp.id
                    WHERE prof.pessoafisica_id = pes.id AND prof.excluido IS FALSE
                    ORDER BY ordem_pronome
                    LIMIT 1),'') || pes.nome_completo AS nome_completo,
                  pes.cpf,
                  CASE 
                    WHEN pes.sexo = 'M' THEN 'Masculino'
                    WHEN pes.sexo = 'F' THEN 'Feminino'
                  END AS sexo,
                  (SELECT 
                      array_to_string(array_agg(DISTINCT tp.descricao), ', ')
                   FROM 
                      profissionalsaude.tb_tipo_profissional tp
                   INNER JOIN
                      profissionalsaude.tb_profissional pro ON pro.tipo_profissional_id = tp.id 
                                                           AND pro.excluido IS FALSE
                   WHERE 
                      pro.pessoafisica_id = pes.id) as tipo,
                  pes.id
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY nome_completo";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function getListPorTipo($arWhere = array(), $paginacao = true, $order = null, $jsonEncode = false, $filtrarResponsabilidade = true)
    {
        $arWhere[] = "pes.excluido IS FALSE";
        if($filtrarResponsabilidade){
            $arWhere[] = self::getWhereResponsabilidadeEstabelecimento();
        }

        $sql = "SELECT
                  CASE 
                  WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                  THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                  WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                  THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                  ELSE pes.nome_completo
                  END AS nome,
                  p.registro_profissional || '/' || p.uf_registro AS registro,
                  tp.descricao AS tipo,
                  p.id
                FROM
                  corporativo.tb_pessoafisica pes
                INNER JOIN
                  profissionalsaude.tb_profissional p ON p.pessoafisica_id = pes.id AND p.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_tipo_profissional tp ON p.tipo_profissional_id = tp.id 
                                                           AND p.excluido IS FALSE                
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY nome_completo, tp.descricao";
            }
        }

        if ($paginacao) {
            $retorno = Paginacao::getPaginacao($this, $sql, 15);
        } else {
            $retorno = $this->carregar($sql);
            $retorno = $retorno ? $retorno : array();
        }

        return $jsonEncode ? json_encode(utf8ArrayEncode($retorno)) : $retorno;
    }

    public function excluirPorPessoaFisica($pessoaFisicaId)
    {
        $sql = "UPDATE profissionalsaude.tb_profissional SET excluido = TRUE WHERE pessoafisica_id = {$pessoaFisicaId}";
        $this->executar($sql);
    }

    public function excluirProfissional($id)
    {
        $arDados = array(
            'id' => $id,
            'status' => 'I',
        );

        return $this->popularDadosObjeto($arDados)->salvar();
    }

    public static function getWhereResponsabilidadeEstabelecimento($aliasTabela = 'pes', $coluna = 'id', $perfisVerificarResp = array())
    {
        $aliasTabela = $aliasTabela ? "{$aliasTabela}." : '';
        $whereProfissional = "({$aliasTabela}{$coluna} IN (SELECT
                                                                 rpro.pessoafisica_id
                                                           FROM
                                                               profissionalsaude.tb_profissional rpro
                                                           INNER JOIN
                                                               profissionalsaude.tb_especialidade_profissional resp ON rpro.id = resp.profissional_id
                                                           INNER JOIN
                                                               estabelecimentosaude.tb_estabelecimento_especialidade_profissional reep ON resp.id = reep.especialidade_profissional_id
                                                           INNER JOIN
                                                               estabelecimentosaude.tb_estabelecimento rest ON reep.estabelecimento_id = rest.id            
                                                           WHERE
                                                               " . UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento('rest', 'id', $perfisVerificarResp) . "))";

        return $whereProfissional;
    }

    public function getTodosDados($id)
    {
        $sql = "SELECT
                    pro.curriculo,
                    pro.id,
                    CASE 
                      WHEN pes.sexo = 'M' AND tp.pron_trat_masc IS NOT NULL
                      THEN tp.pron_trat_masc || ' ' || pes.nome_completo
                      WHEN pes.sexo = 'F' AND tp.pron_trat_fem IS NOT NULL
                      THEN tp.pron_trat_fem || ' ' || pes.nome_completo
                      ELSE pes.nome_completo
                      END AS nome_completo,
                    pes.sexo,
                    pro.registro_profissional,
                    pro.uf_registro,
                    tp.descricao AS tipo_profissional,
                    tp.sigla_tipo_registro,
                    pes.foto_arquivo_id
                FROM
                    profissionalsaude.tb_profissional pro
                INNER JOIN
                    corporativo.tb_pessoafisica pes ON pro.pessoafisica_id = pes.id
                INNER JOIN
                    profissionalsaude.tb_tipo_profissional tp ON pro.tipo_profissional_id = tp.id
                WHERE
                    pro.id = {$id}";

        return $this->pegaLinha($sql);
    }

    public function listaEstabelecimentosPorCpfProfissional($cpfProfissional)
    {
        $whereCpdProfissional = "AND pes.cpf = '{$cpfProfissional}'";

        if($this->testa_superuser()){
            $whereCpdProfissional = "";
        }

        $sql = "SELECT DISTINCT 
                    est.id AS codigo,
                    pj.nome_fantasia AS descricao,
                    pj.logo_arquivo_id AS estabelecimento_logo
                FROM
                    profissionalsaude.tb_profissional pro
                INNER JOIN
                    corporativo.tb_pessoafisica pes ON pro.pessoafisica_id = pes.id
                INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON pro.id = ep.profissional_id
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON ep.id = eep.especialidade_profissional_id
                INNER JOIN
                    estabelecimentosaude.tb_estabelecimento est ON eep.estabelecimento_id = est.id
                INNER JOIN
                    corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                WHERE
                    ep.excluido IS FALSE
                    AND
                    eep.excluido IS FALSE
                    AND
                    est.excluido IS FALSE
                    {$whereCpdProfissional}
                ORDER BY
                  2";

        return $this->carregarArray($sql);
    }

    public function isProfissional($cpf)
    {
        $cpf = formatar_cpf(str_replace(array('.', '-'), '', $cpf));
        $sql = "SELECT 
                  COUNT(*)
                FROM 
                  profissionalsaude.tb_profissional prof
                INNER JOIN
                  corporativo.tb_pessoafisica pes ON prof.pessoafisica_id = pes.id
                WHERE
                  prof.excluido IS FALSE
                  AND
                  pes.excluido IS FALSE
                  AND
                  pes.cpf = '{$cpf}'
                  AND " . self::getWhereResponsabilidadeEstabelecimento();

        return $this->pegaUm($sql) > 0;
    }

    public function getPrefixoProfissional($profissionalId)
    {
        $profissional = $this->getTodosDados($profissionalId);
        return $profissional['nome_completo'];
    }

    public function gerarRelatorioProfissionaisApp($jsonEncode = false)
    {
        $sql = "SELECT
                    COUNT(DISTINCT usu.usucpf) AS qtd,
                    CASE  WHEN dis.appid IS NOT NULL THEN 
                        'Acessou o app'
                        ELSE
                        'Não acessou o app'
                    END AS acessou
                FROM
                    seguranca.usuario usu
                INNER JOIN
                    seguranca.perfilusuario pu ON usu.usucpf = pu.usucpf
                LEFT JOIN
                    app.dispositivousuario du ON usu.usucpf = du.usucpf
                LEFT JOIN
                    app.dispositivo dis ON dis.disidentificador = du.diuidentificador
                WHERE
                    pu.pflcod = 440
                    AND
                    (dis.appid = 2 OR dis.appid IS NULL)
                GROUP BY
                    2";

        $dados = $this->carregarArray($sql);
        $dadosGrafico = [];
        foreach ($dados as $linha){
            $dadosGrafico[] = [
                'name' => $linha['acessou'],
                'tipo' => $linha['acessou'] == 'Acessou o app' ? 'acessou' : 'nao_acessou',
                'y' => intval($linha['qtd'])
            ];
        }

        return $jsonEncode ? json_encode(utf8ArrayEncode($dadosGrafico)) : $dadosGrafico;
    }

    public function listarProfissionaisAcessoApp($tipo)
    {
        $where = 'false';
        switch ($tipo) {
            case 'acessou' :
                $where = 'dis.appid IS NOT NULL';
                break;
            case 'nao_acessou' :
                $where = 'dis.appid IS NULL';
                break;
        }

        $sql = "SELECT DISTINCT 
                    usu.usucpf AS cpf,
                    usu.usunome AS nome,
                    usu.usuemail AS email
                FROM
                    seguranca.usuario usu
                INNER JOIN
                    seguranca.perfilusuario pu ON usu.usucpf = pu.usucpf
                LEFT JOIN
                    app.dispositivousuario du ON usu.usucpf = du.usucpf
                LEFT JOIN
                    app.dispositivo dis ON dis.disidentificador = du.diuidentificador
                WHERE
                    pu.pflcod = 440
                    AND
                    (dis.appid = 2 OR dis.appid IS NULL)
                    AND
                    {$where}
                ORDER BY
                    2";

        return $this->carregarArray($sql);
    }
}