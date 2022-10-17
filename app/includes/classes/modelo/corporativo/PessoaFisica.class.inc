<?php

class PessoaFisica extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "corporativo.tb_pessoafisica";

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
        'nome_completo' => null,
        'nome_social' => null,
        'cpf' => null,
        'nome_mae' => null,
        'nome_pai' => null,
        'data_nascimento' => null,
        'foto_arquivo_id' => null,
        'sexo' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'chave_pessoa' => null,
        'excluido' => null,
        'cartao_nacional_saude' => null,
    );

    /**
     * @param $params
     * @param array $arquivos
     * @param bool $salvarEndereco
     * @param bool $salvarContato
     * @return bool|int
     * @throws CpfIndisponivel
     */
    public function salvarPessoaFisica($params, $arquivos = array(), $salvarEndereco = true, $salvarContato = true, $salvarConvenioPlano = true)
    {
        $camposNulos = array();
        foreach ($params as $campo => $valor){
            if($valor === ''){
                $params[$campo] = null;
                $camposNulos[] = $campo;
            }
        }

        if($params['cpf'] && !$this->isCpfDisponivel($params['cpf'], $params['id'])) {
            throw new CpfIndisponivel();
        }

        if($params['id']){
            $this->carregarPorId($params['id']);
        }

        $arquivoId = $params['foto_arquivo_id'];
        if (isset($arquivos['foto'])
            && isset($arquivos['foto']['name']) && !empty($arquivos['foto']['name'])
            && isset($arquivos['foto']['type']) && !empty($arquivos['foto']['type'])
            && isset($arquivos['foto']['size']) && !empty($arquivos['foto']['size'])) {
            $file = new FilesSimec();
            $arquivoId = $file->setUploadArquivoEspecifico("Foto da Pessoa {$params['nome_completo']}", 'foto');
        }

        $dados = array(
            'id' => $params['id'],
            'nome_completo' => $params['nome_completo'],
            'nome_social' => $params['nome_social'],
            'foto_arquivo_id' => $arquivoId,
            'cpf' => $params['cpf'],
            'nome_mae' => $params['nome_mae'],
            'data_nascimento' => formata_data_sql($params['data_nascimento']),
            'sexo' => $params['sexo'],
            'cartao_nacional_saude' => $params['cartao_nacional_saude'],
            'chave_pessoa' => $this->gerarChavePessoa($params)
        );

        if(empty($params['id'])){
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

        $dados = array_merge($dados, $dadosCarimbo);
        $id = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);

        if($salvarEndereco){
            $mEnderecoPf = new EnderecoPessoaFisica();
            $mEnderecoPf->excluirPorPessoaFisica($id);
            if($params['enderecos']){
                $params['enderecos'] = str_replace(['\"', "\'"], ['"',"'"], $params['enderecos']);
                $params['enderecos'] = utf8_encode($params['enderecos']);
                $params['enderecos'] = utf8ArrayDecode(json_decode($params['enderecos'], true));
                foreach ($params['enderecos'] as $endereco){
                    $endereco['pessoafisica_id'] = $id;
                    $mEnderecoPf->salvarEndereco($endereco);
                    $mEnderecoPf->setDadosNull();
                }
            }
        }

        if($salvarContato){
            $mContatoPf = new ContatoPessoaFisica();
            $mContatoPf->excluirPorPessoaFisica($id);
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

                    $contato['pessoafisica_id'] = $id;
                    $mContatoPf->salvarContatos($contato);
                    $mContatoPf->setDadosNull();
                }
            }
        }

        if($salvarConvenioPlano){
            $mConvenioPlano = new ConvenioEstabelecimentoPaciente();
            $mConvenioPlano->excluirPorPessoaFisica($id);
            if($params['convenios_planos']){
                if(!is_array($params['convenios_planos'])){
                    $params['convenios_planos'] = str_replace('\"', '"', $params['convenios_planos']);
                    $params['convenios_planos'] = utf8_encode($params['convenios_planos']);
                    $params['convenios_planos'] = utf8ArrayDecode(json_decode($params['convenios_planos'], true));
                }

                foreach ($params['convenios_planos'] as $convenioPlano){
                    $mConvenioPlano->salvarConvenioPaciente($id, $convenioPlano['convenio_estabelecimento_id'], $convenioPlano['numero'], $convenioPlano['validade']);
                    $mConvenioPlano->setDadosNull();
                }
            }
        }

        return $id;
    }

    /**
     * @param $params
     * @param array $arquivos
     * @return array|bool
     */
    public function alteraFoto($params, $arquivos = array())
    {
        if($params['id']){
            $this->carregarPorId($params['id']);

            $arquivoId = $params['foto_arquivo_id'];
            if (isset($arquivos['foto'])
                && isset($arquivos['foto']['name']) && !empty($arquivos['foto']['name'])
                && isset($arquivos['foto']['type']) && !empty($arquivos['foto']['type'])
                && isset($arquivos['foto']['size']) && !empty($arquivos['foto']['size'])) {
                $file = new FilesSimec();
                $arquivoId = $file->setUploadArquivoEspecifico("Foto da Pessoa {$params['nome_completo']}", 'foto');
            }

            $dados = array(
                'id' => $params['id'],
                'foto_arquivo_id' => $arquivoId,
                'atualizado_em' => 'now()',
                'atualizado_por' => $_SESSION['usuario_id'],
            );
            $id = $this->popularDadosObjeto($dados)->salvar(true, true);
        }
        return $id ? $dados : false;
    }

    public function getList($arWhere = array(), $paginacao = true, $order = null, $limit = null, $filtrarResponsabilidade = true)
    {
        $arWhere[] = "pes.excluido IS FALSE";
        if($filtrarResponsabilidade){
            $arWhere[] = self::getWhereResponsabilidadeEstabelecimento('pes', 'id', true, false);
        }

        $sql = "SELECT DISTINCT 
                  pes.nome_completo,
                  pes.cpf,
                  CASE 
                    WHEN pes.sexo = 'M' THEN 'Masculino'
                    WHEN pes.sexo = 'F' THEN 'Feminino'
                  END AS sexo,
                  pes.data_nascimento,
                  pes.cartao_nacional_saude,
                  pes.nome_mae,
                  contato_tel.contato AS telefone,
                  contato_telfix.contato AS telefone_fixo,
                  contato_email.contato AS email,
                  TO_CHAR(pes.data_nascimento, 'DD/MM/YYYY') AS data_nascimento_formatado,
                  pes.sexo AS sexo_sigla,
                  pes.nome_social,
                  pes.id
                FROM
                  corporativo.tb_pessoafisica pes
                LEFT JOIN 
                  corporativo.tb_contato_pessoafisica contato_tel ON pes.id = contato_tel.pessoafisica_id
                                                                 AND contato_tel.tipo_contato_id = (SELECT id FROM corporativo.tb_tipo_contato WHERE sigla = '" . TipoContato::TIPO_CELULAR . "')
                LEFT JOIN 
                  corporativo.tb_contato_pessoafisica contato_email ON pes.id = contato_email.pessoafisica_id
                                                                   AND contato_email.tipo_contato_id = (SELECT id FROM corporativo.tb_tipo_contato WHERE sigla = '" . TipoContato::TIPO_EMAIL . "')
                LEFT JOIN 
                  corporativo.tb_contato_pessoafisica contato_telfix ON pes.id = contato_telfix.pessoafisica_id
                                                                    AND contato_telfix.tipo_contato_id = (SELECT id FROM corporativo.tb_tipo_contato WHERE sigla = '" . TipoContato::TIPO_TELEFONE_FIXO . "')
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if(!is_null($order)){
                $sql .= " ORDER BY {$order}";
            }else{
                $sql .= " ORDER BY pes.nome_completo, pes.nome_social DESC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 15);
            return $dado;
        } else {
            if($limit && is_numeric($limit)){
                $sql .= " LIMIT {$limit}";
            }

            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function excluirPessoaFisica($id)
    {
        $this->excluirLogicamente($id);
    }

    public function isCpfDisponivel($cpf, $id = null)
    {
        $where = array(
            "cpf = '{$cpf}'",
            "excluido IS FALSE",
            // self::getWhereResponsabilidadeEstabelecimento('')
        );
        if($id){
            $where[] = "id != {$id}";
        }

        return intval($this->recuperarUm('count(*)', $where)) === 0;
    }

    public function gerarChavePessoa($params)
    {
        $tratarString = function ($str){
            return strtoupper(removeAcentos(preg_replace('/\s+/', '', $str)));
        };

        return md5_encrypt_senha(json_encode(array(
            'nome_completo' => $tratarString($params['nome_completo']),
            'data_nascimento' => formata_data_sql($params['data_nascimento']),
            'sexo' => $params['sexo'],
            'nome_mae' => $tratarString($params['nome_mae']),
        )), '');
    }

    public static function getWhereResponsabilidadeEstabelecimento($aliasTabela = 'pes', $coluna = 'id', $verificarPaciente = true, $verificarProfissional = true)
    {
        $whereProfissional = $verificarProfissional ? Profissional::getWhereResponsabilidadeEstabelecimento($aliasTabela, $coluna) : 'false' ;

        $aliasTabela = $aliasTabela ? "{$aliasTabela}." : '';
        $wherePaciente = $verificarPaciente ? "({$aliasTabela}{$coluna} IN (SELECT
                                                             repa.pessoa_paciente_id
                                                       FROM
                                                             estabelecimentosaude.tb_estabelecimento_profissional_agenda repa
                                                       INNER JOIN
                                                             estabelecimentosaude.tb_estabelecimento_especialidade_profissional reep ON repa.estabelecimento_especialidade_profissional_id = reep.id
                                                       INNER JOIN
                                                             estabelecimentosaude.tb_estabelecimento rest ON reep.estabelecimento_id = rest.id
                                                       WHERE
                                                             " . UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento('rest') . "))" : 'false';

        return "(({$wherePaciente}) OR ({$whereProfissional}))";
    }

    public function getTodosDados($pessoaPacienteId)
    {
        $sql = "SELECT 
                    * 
                FROM 
                    corporativo.tb_pessoafisica pes
                WHERE
                    pes.id = {$pessoaPacienteId}";

        return $this->pegaLinha($sql);
    }

    public function getDivInfoPaciente($pessoaPacienteId, $exibeTags = true)
    {
        $mTag = new TagPaciente();
        $dadosPaciente = $this->getTodosDados($pessoaPacienteId);
        $img = montaImgFoto($dadosPaciente['foto_arquivo_id'], $dadosPaciente['sexo'], '180px', '', '200', '200', 'img-circle bg-white', true);
        $idade = calcularIdade($dadosPaciente['data_nascimento']);
        $dataNascimento = formata_data($dadosPaciente['data_nascimento']);
        $sexo = (($dadosPaciente['sexo']) == 'F' ? 'Feminino' : 'Masculino');

        $htmlTags = "";
        if($exibeTags){
            $tags = $mTag->getStrTagsPorPaciente($dadosPaciente['id'], ' ');
            $htmlTags = <<<HTMLTAG
                <div class="text-center" style="padding: 10px;">
                    {$tags}
                </div>
HTMLTAG;

        }

        $html = <<<HTML
                <div class="row">
                    <div class="col-sm-12">
                        <div class="mt-widget-1">
                            <div class="mt-img">
                                {$img}
                            </div>
                            <div class="mt-body">
                                <h3 class="mt-username">{$dadosPaciente['nome_completo']}</h3>
                                <p class="mt-user-title">
                                    <strong>Idade: </strong>
                                    {$idade}
                                    <br/>
                                    <strong>Nascimento: </strong>
                                    {$dataNascimento}
                                    <br/>
                                    <strong>Sexo: </strong>
                                    {$sexo}
                                </p>
                                {$htmlTags}
                            </div>
                        </div>
                    </div>
                </div>
HTML;

        return $html;
    }

    public function getCombo($arrWhere = array(), $jsonEncode = true)
    {
        $arrWhere[] = "pes.excluido IS FALSE";

        $sql = "SELECT DISTINCT 
                  pes.nome_completo AS descricao,
                  pes.id AS codigo,
                  pes.foto_arquivo_id,
                  pes.sexo
                FROM
                  corporativo.tb_pessoafisica pes
                WHERE
                  " . implode(' AND ', $arrWhere);

        $rs = $this->carregar($sql);
        $rs = $rs ? $rs : array();

        return $jsonEncode ? json_encode(utf8ArrayEncode($rs)) : $rs;
    }

    public function getListByCampoDeBusca($busca, $retonrarJson = true) {
        $busca = trim(addslashes(utf8_decode($busca)));
        $pessoas = array();
        if($busca){
            $arWhere = array(
                "pes.nome_completo ILIKE '%{$busca}%'",
                "replace(replace(pes.cpf, '.', ''), '-', '') ILIKE replace(replace('%{$busca}%', '.', ''), '-', '')",
                "replace(replace(pes.cartao_nacional_saude, '.', ''), '-', '') ILIKE replace(replace('%{$busca}%', '.', ''), '-', '')",
                "replace(pes.data_nascimento::text, '-', '') ILIKE replace('%" . formata_data_sql($busca) . "%', '-', '')",
            );

            $pessoas = $this->getList(array('(' . implode(' OR ', $arWhere) . ')'), false, null, 10, false);
        }

        return $retonrarJson ? json_encode(utf8ArrayEncode(array('data' => $pessoas))): $pessoas;
    }

    public function getWherePessoaAtendidaPeloProfissionalAndEstabelecimento($cpfProfissional, $estabelecimentoId, $campoIdPessoa = "pes.id") {
        $where = "{$campoIdPessoa} IN
            (SELECT DISTINCT
                epa.pessoa_paciente_id
            FROM
                  estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                INNER JOIN
                  estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                        AND eep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                                                                    AND ep.excluido IS FALSE
                INNER JOIN
                  profissionalsaude.tb_profissional pro ON ep.profissional_id = pro.id
                INNER JOIN
                  corporativo.tb_pessoafisica p ON pro.pessoafisica_id = p.id
                INNER JOIN
                  workflow.documento doc ON epa.documento_id = doc.docid AND doc.esdid = " . EstabelecimentoProfissionalAgenda::WF_ESTADO_REALIZADA . "
            WHERE 
              p.cpf = '{$cpfProfissional}' AND eep.estabelecimento_id = {$estabelecimentoId} AND epa.excluido = FALSE)";

        return $where;
    }
}