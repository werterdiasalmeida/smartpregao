<?php

class Estabelecimento extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "estabelecimentosaude.tb_estabelecimento";

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
        'pessoajuridica_id' => null,
        'telefone_principal' => null,
        'email_principal' => null,
        'criado_em' => null,
        'criado_por' => null,
        'atualizado_em' => null,
        'atualizado_por' => null,
        'excluido' => null,
    );

    /**
     * @param $params
     * @param array $arquivos
     * @return bool|int
     * @throws CnpjIndisponivel
     * @throws Exception
     */
    public function salvarEstabelecimento($params, $arquivos = array())
    {
        $camposNulos = array();
        foreach ($params as $campo => $valor) {
            if ($valor === '') {
                $params[$campo] = null;
                $camposNulos[] = $campo;
            }
        }

        if ($params['id']) {
            $this->carregarPorId($params['id']);
        }

        $mPessoaJuridica = new PessoaJuridica();
        $paramsPj = $params;
        $paramsPj['id'] = $this->pessoajuridica_id;
        $pessoaJuridicaId = $mPessoaJuridica->salvarPessoaJuridica($paramsPj, $arquivos);

        $dados = array(
            'id' => $params['id'],
            'tipo_estabelecimento_id' => $params['tipo_estabelecimento_id'],
            'telefone_principal' => $params['cnes'],
            'email_principal' => $params['registro_ans'],
            'pessoajuridica_id' => $pessoaJuridicaId,
        );

        if (empty($params['id'])) {
            $dadosCarimbo = array(
                'criado_em' => 'now()',
                'criado_por' => $_SESSION['usuario_id'],
            );
        } else {
            $dadosCarimbo = array(
                'atualizado_em' => 'now()',
                'atualizado_por' => $_SESSION['usuario_id'],
            );
        }

        $dados = array_merge($dados, $dadosCarimbo);


        $id = $this->popularDadosObjeto($dados)->salvar(true, true, $camposNulos);

        if (empty($params['id'])) {
            $mResponsabilidade = new UsuarioResponsabilidadeEstabelecimento();

            if (possuiPerfil(array(ADM_PERFIL_GERAL))) {
                $mResponsabilidade->atribuirResponsabilidadeEstabelecimento($_SESSION['usucpf'], ADM_PERFIL_GERAL, $id);
            }

            if (possuiPerfil(array(ADM_PERFIL_LOCAL))) {
                $mResponsabilidade->atribuirResponsabilidadeEstabelecimento($_SESSION['usucpf'], ADM_PERFIL_LOCAL, $id);
            }

            if (possuiPerfil(array(ADM_PERFIL_REPRESENTANTE))) {
                $mResponsabilidade->atribuirResponsabilidadeEstabelecimento($_SESSION['usucpf'], ADM_PERFIL_REPRESENTANTE, $id);
            }

            if (possuiPerfil(array(ADM_PERFIL_RECEPCIONISTA))) {
                $mResponsabilidade->atribuirResponsabilidadeEstabelecimento($_SESSION['usucpf'], ADM_PERFIL_RECEPCIONISTA, $id);
            }
        }

        return $id;
    }

    public function getList($arWhere = array(), $paginacao = true, $order = null)
    {
        $arWhere[] = "est.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();

        $sql = "SELECT
                  pj.nome_fantasia,
                  pj.razao_social,
                  pj.cnpj,
                  est.pessoajuridica_id,
                  est.id
                FROM
                  estabelecimentosaude.tb_estabelecimento est
                INNER JOIN
                  corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                                                  AND pj.excluido IS FALSE
                WHERE
                  " . implode(' AND ', $arWhere);

        if (!isset($_REQUEST['ordem'])) {
            if (!is_null($order)) {
                $sql .= " ORDER BY {$order}";
            } else {
                $sql .= " ORDER BY pj.nome_fantasia, pj.razao_social DESC";
            }
        }

        if ($paginacao) {
            $dado = Paginacao::getPaginacao($this, $sql, 10);
            return $dado;
        } else {
            $retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
    }

    public function getQtdAgendaByEstabelecimento($id)
    {
        $sql = "SELECT 
                     COUNT(1) 
                  FROM 
                     estabelecimentosaude.tb_estabelecimento_profissional_agenda epa
                  INNER JOIN
                     estabelecimentosaude.tb_estabelecimento_especialidade_profissional eep ON epa.estabelecimento_especialidade_profissional_id = eep.id
                                                                                       AND eep.excluido IS FALSE
                  INNER JOIN
                    profissionalsaude.tb_especialidade_profissional ep ON eep.especialidade_profissional_id = ep.id
                                                                      AND ep.excluido IS FALSE
                  WHERE 
                    eep.estabelecimento_id = {$id}";

        return $this->pegaUm($sql);
    }

    public function excluirEstabelecimento($id)
    {
        return $this->excluirLogicamente($id);
    }

    public function getTodosDados($id)
    {
        $sql = "SELECT
                  pj.nome_fantasia,
                  pj.razao_social,
                  pj.cnpj,
                  pj.classificacao,
                  pj.logo_arquivo_id,
                  pj.flg_cabecalho_rodape,
                  est.telefone_principal,
                  est.email_principal,
                  est.cnes,
                  est.registro_ans,
                  est.pessoajuridica_id,
                  est.id
                FROM
                  estabelecimentosaude.tb_estabelecimento est
                INNER JOIN
                  corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                                                  AND pj.excluido IS FALSE
                WHERE
                  est.id = {$id}";

        return $this->pegaLinha($sql);
    }

    public static function gerarCabecalho($id = null, $verificarResponsabilidade = true, $redirecionarResponsabilidade = true)
    {
        return false;
        $p = getParam();
        $id = $id ? $id : ($p['estabelecimento_id'] ? $p['estabelecimento_id'] : $p['id']);
        $pParam = setParam(array('id' => $id));

        if ($verificarResponsabilidade) {
            UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($id, $redirecionarResponsabilidade);
        }

        $mEstabelecimento = new Estabelecimento();
        $dados = $mEstabelecimento->getTodosDados($id);
        $classificacao = $dados['classificacao'] == PessoaJuridica::TIPO_PUBLICA ? 'Pública' : 'Privada';

        if (count($p) === 1) {
            $estabelecimentos = array_map(function ($estabelecimento) {
                return array(
                    'codigo' => setParam(array('id' => $estabelecimento['id'])),
                    'descricao' => "{$estabelecimento['nome_fantasia']}"
                );
            }, $mEstabelecimento->getList(array(), false, null));

            $pValue = setParam(array('id' => $p['id']));
            $comboEstabelecimentos = $mEstabelecimento->monta_combom('estabelecimento_id', $estabelecimentos, 'S', '', "window.location = '?modulo={$_REQUEST['modulo']}&acao={$_REQUEST['acao']}' + this.value;", '', '', '', '', '', true, $pValue, null, true);
        } else {
            $comboEstabelecimentos = "<strong>{$dados['nome_fantasia']}</strong>";
        }

        $html = <<<HTML
            <div class="page-bar portlet light bordered" style="margin-top: 0; margin-bottom: 10px; padding: 5px 20px 0 20px; border-right: 0 !important; border-left: 0 !important;">
                <div class="portlet-body">
                    <div class="row">
                        <div class="col-sm-4 col-md-5">
                            <div class="form-group">
                                <div class="row">
                                    <div class="col-sm-5 col-md-3 col-lg-3">
                                        Cliente:
                                    </div>
                                    <div class="col-sm-7 col-md-9  col-lg-9">
                                        {$comboEstabelecimentos}            
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="col-sm-4 col-md-3">
                            <div class="form-group">
                                CNPJ:
                                <strong>{$dados['cnpj']}</strong>
                            </div>
                        </div>
                        <div class="col-sm-2 col-md-2">
                            <div class="form-group">
                                Situação:
                                <strong>Ativa <spam class="badge badge-primary"><b>Plano Free</b></spam></strong>
                            </div>
                        </div>
                        <div class="col-sm-2 col-md-2">
                            <div class="form-group">
                                <a class="btn btn-circle btn-icon-only btn-default purple-plum " href="?modulo=principal/editaisclientes/inicio&acao=C{$pParam}" data-toggle="tooltip" title="Painel" target="_top">
                                    <i class="icon-arrow-right"></i>
                                </a>
                                <a class="btn btn-circle btn-icon-only btn-default red" href="?modulo=principal/cdcclientes/inicio&acao=C{$pParam}" data-toggle="tooltip" title="Painel Cotações" target="_top">
                                    <i class="icon-arrow-up"></i>
                                </a>
                                <a class="btn btn-circle btn-icon-only btn-default yellow " href="?modulo=principal/editaisclientes/monitoramento&acao=C{$pParam}" data-toggle="tooltip" title="Monitoramento" target="_top">
                                    <i class="icon-eye"></i>
                                </a>
                                <a class="btn btn-circle btn-icon-only btn-default red" href="?modulo=principal/estabelecimento/agenda&acao=C{$pParam}" data-toggle="tooltip" title="Agenda">
                                    <i class="icon-calendar"></i>
                                </a>
                                <a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/estabelecimento/formulario&acao=C{$pParam}" data-toggle="tooltip" title="Configurações">
                                    <i class="fa fa-edit"></i>
                                </a>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            
            <div class="modal fade in" id="modal-chamar-senha" tabindex="-1" role="modal-chamar-senha" aria-hidden="true">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <form id="form-chamar-senha" method="post">
                            <div id="form-chamar-senha-content"></div>
                        </form>
                    </div>
                </div>
            </div>
            <script type="text/javascript">
            
                function chamarSenha(param) {
                    var content = $('#form-chamar-senha-content');
                    content.html('<div style="height: 150px; width: 100%;"></div>');
        
                    App.blockUI({
                        message: 'Carregando...',
                        target: '#form-chamar-senha-content'
                    });
        
                    content.load("?modulo=principal/estabelecimento/agenda&acao=C" + param, {
                        act: 'chamarSenha'
                    }, function () {
                        App.unblockUI('#form-chamar-senha-content');
                    });
        
                    $('#modal-chamar-senha').modal({
                        backdrop: 'static',
                    }).modal('show');
                }
            </script>
HTML;

        return $html;
    }

    public function getComboPorResponsabilidade($arWhere = array())
    {
        $rs = $this->getList($arWhere, false);
        return array_map(function ($row) {
            return array(
                'codigo' => $row['id'],
                'descricao' => $row['nome_fantasia']
            );
        }, $rs);
    }

    public function listarEnderecos($arWhere = array(), $jsonEncode = false)
    {
        $arWhere[] = "est.excluido IS FALSE";
        $arWhere[] = UsuarioResponsabilidadeEstabelecimento::gerarWhereResponsabilidadeEstabelecimento();

        $sql = "SELECT DISTINCT 
                  pj.nome_fantasia,
                  pj.razao_social,
                  pj.cnpj,
                  tes.descricao AS tipo,
                  est.id AS estabelecimento_id,
                  ST_AsGeoJSON(epj.geo_localizacao) AS geo_localizacao
                FROM
                  estabelecimentosaude.tb_estabelecimento est
                INNER JOIN
                  corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                                                  AND pj.excluido IS FALSE
                INNER JOIN
                  corporativo.tb_endereco_pessoajuridica epj ON epj.pessoajuridica_id = pj.id
                LEFT JOIN
                    corporativo.tb_unidade_federacao uf ON epj.uf_id = uf.id
                LEFT JOIN
                    corporativo.tb_municipio mun ON epj.municipio_id = mun.id
                INNER JOIN
                  estabelecimentosaude.tb_tipo_estabelecimento tes ON est.tipo_estabelecimento_id = tes.id
                WHERE
                  " . implode(' AND ', $arWhere);

        $enderecos = array_map(function ($endereco) {
            if ($endereco['geo_localizacao']) {
                $coordenadas = json_decode($endereco['geo_localizacao'], true);
                $endereco['geo_localizacao'] = array(
                    'lat' => floatval($coordenadas['coordinates'][0]),
                    'lng' => floatval($coordenadas['coordinates'][1])
                );
            }
            return $endereco;
        }, $this->carregarArray($sql));
        return $jsonEncode ? json_encode(utf8ArrayEncode($enderecos)) : $enderecos;
    }

    public function listarEnderecosMapa($jsonEncode = false)
    {
        $enderecos = array_map(function ($endereco) {
            return array(
                'lat' => $endereco['geo_localizacao']['lat'],
                'lng' => $endereco['geo_localizacao']['lng'],
                'title' => "{$endereco['nome_fantasia']} ({$endereco['cnpj']})",
                'razao_social' => $endereco['razao_social'],
                'nome_fantasia' => $endereco['nome_fantasia'],
                'cnpj' => $endereco['cnpj'],
                'tipo' => $endereco['tipo'],
                'p' => setParam(array('id' => $endereco['estabelecimento_id']), false)
            );
        }, $this->listarEnderecos(array("epj.geo_localizacao IS NOT NULL")));

        return $jsonEncode ? json_encode(utf8ArrayEncode($enderecos)) : $enderecos;
    }

    public function getByCnpj($cnpj)
    {
        $arWhere[] = "est.excluido IS FALSE";
        $arWhere[] = "pj.cnpj = '{$cnpj}'";

        $sql = "SELECT
                  pj.nome_fantasia,
                  pj.razao_social,
                  pj.cnpj,
                  est.pessoajuridica_id,
                  est.id
                FROM
                  estabelecimentosaude.tb_estabelecimento est
                INNER JOIN
                  corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                                                  AND pj.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_tipo_estabelecimento tes ON est.tipo_estabelecimento_id = tes.id
                WHERE
                  " . implode(' AND ', $arWhere);

        $retorno = $this->pegaLinha($sql);

        if ($retorno) {
            $mPessoaJuridica = new PessoaJuridica();
            $retorno = $mPessoaJuridica->getDadosEnderecoEContatoPrincipal($retorno);
        }

        return $retorno ? $retorno : array();
    }

    public function listarComLimit($arWhere = array(), $limit = 10)
    {
        $arWhere[] = "est.excluido IS FALSE";

        $sql = "SELECT
                  pj.nome_fantasia,
                  pj.razao_social,
                  pj.cnpj,
                  tes.descricao AS tipo_estabelecimento_descricao,
                  est.pessoajuridica_id,
                  est.id
                FROM
                  estabelecimentosaude.tb_estabelecimento est
                INNER JOIN
                  corporativo.tb_pessoajuridica pj ON est.pessoajuridica_id = pj.id
                                                  AND pj.excluido IS FALSE
                INNER JOIN
                  estabelecimentosaude.tb_tipo_estabelecimento tes ON est.tipo_estabelecimento_id = tes.id
                WHERE
                  " . implode(' AND ', $arWhere) . "
                LIMIT 
                   {$limit} 
              ";

        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

    public static function gerarCabecalhoDoMedico()
    {
        global $db;

        $estabelecimento_id = $_SESSION['estabelecimento_selecionado'];
        $usucpfFormatado = formatar_cpf($_SESSION['usucpf']);


        $html = <<<HTML
<div class="page-bar portlet light bordered"
     style="margin-top: 0; margin-bottom: 10px; padding: 5px 20px 0 20px; border-right: 0 !important; border-left: 0 !important;"
 >
    <div class="portlet-body">
        <div class="row align-items-center">
            <div class="col-sm-12">
                <div class="form-group">
                    <div class="row">
                        <div class="col-sm-2">
                            Estabelecimento:
                        </div>
                        <div class="col-sm-10">
                            <form method="post" name="form-estabelecimento">
                                {$strCombo}
                            </form>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function () {
        $('[name=estabelecimento_id]').change(function () {
            $('[name=form-estabelecimento]').submit();
        });
    });

</script>
HTML;
        return $html;
    }

    public static function setEstabelecimentoDoMedicoNaSession()
    {
        global $db;
        $usucpfFormatado = formatar_cpf($_SESSION['usucpf']);
        $mProfissional = new Profissional();
        $estabelecimentos = $mProfissional->listaEstabelecimentosPorCpfProfissional($usucpfFormatado);

        if ($_REQUEST['estabelecimento_id']) {
            $estabelecimento_id = $_REQUEST['estabelecimento_id'];
        } else if (isset($_SESSION['estabelecimento_selecionado']) && !empty($_SESSION['estabelecimento_selecionado'])) {
            $estabelecimento_id = $_SESSION['estabelecimento_selecionado'];
        } else if (count($estabelecimentos) == 1) {
            $estabelecimento_id = $estabelecimentos[0]['codigo'];
        } else {
            include APPRAIZ . "adm/modulos/principal/medico/listaEstabelecimentos.inc";
            die;
        }

        $_SESSION['estabelecimento_selecionado'] = $estabelecimento_id;
    }


    public function retornaAtalhosCliente($id)
    {

        if (UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($id, false, 'adm.php?modulo=principal/cdcclientes/inicio&acao=C')) {
            $acaoPainelCDCEmpresa = '<a class="btn btn-circle btn-icon-only btn-default blue" href="?modulo=principal/cdcclientes/inicio&acao=C&p=' . setParam(array('id' => $id), false) . '" data-toggle="tooltip" title="Painel de Cotações">
        <i class="bi bi-cash-coin"></i>
                </a>';
        }


        if (UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($id, false, 'adm.php?modulo=principal/editaisclientes/inicio&acao=C')) {
            $acaoPainelEmpresa = '<a class="btn btn-circle btn-icon-only btn-default purple" href="?modulo=principal/editaisclientes/inicio&acao=C&p=' . setParam(array('id' => $id), false) . '" data-toggle="tooltip" title="Painel de Pregões">
            <i class="bi bi-hammer"></i>
      </a>';
        }

        if (UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($id, false, 'adm.php?modulo=principal/contratos/inicio&acao=C')) {
            $acaoContratos = '<a class="btn btn-circle btn-icon-only btn-default green" href="?modulo=principal/contratos/inicio&acao=C&p=' . setParam(array('id' => $id), false) . '" data-toggle="tooltip" title="Painel de Contratos">
            <i class="bi bi-journals"></i>
                    </a>';
        }

        if (UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($id, false, 'adm.php?modulo=principal/editaisclientes/monitoramento&acao=C')) {
            $acaoMonitoramento = '<a class="btn btn-circle btn-icon-only btn-default yellow" href="?modulo=principal/editaisclientes/monitoramento&acao=C&p=' . setParam(array('id' => $id), false) . '" data-toggle="tooltip" title="Central de Alertas">
            <i class="bi bi-exclamation-octagon"></i>
                    </a>';
        }
        if (UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($id, false, 'adm.php?modulo=principal/estabelecimento/financeiro/extrato&acao=C')) {
            $acaoAgenda = '<a class="btn btn-circle btn-icon-only btn-default red" href="?modulo=principal/estabelecimento/agenda&acao=C&p=' . setParam(array('id' => $id), false) . '" data-toggle="tooltip" title="Agenda do estabelecimento">
            <i class="bi bi-calendar"></i>
                    </a>';
        }
        if (UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($id, false, 'adm.php?modulo=principal/estabelecimento/formulario&acao=C')) {
            $acaoEdicao = '<a class="btn btn-circle btn-icon-only btn-default purple-plum" href="?modulo=principal/estabelecimento/formulario&acao=C&p=' . setParam(array('id' => $id), false) . '" data-toggle="tooltip" title="Ajustes da empresa">
            <i class="bi bi-gear"></i>
                    </a>';
        }
        $plano = "<i class=\"badge badge-light\" title=\"Plano atual da empresa\">Plano Free</i>";
        $atalhos = " <div style=\"min-width: 200px;\">
            
            {$acaoPainelEmpresa} 
            {$acaoPainelCDCEmpresa}
            {$acaoContratos}
            {$acaoMonitoramento}
            {$acaoAgenda}
            {$acaoEdicao}
            {$plano}
            </div>";
        
        $saida = "<div style=\"float:right; padding-bottom:10px;\">{$atalhos}</div>";
        return $saida;
    }
}
