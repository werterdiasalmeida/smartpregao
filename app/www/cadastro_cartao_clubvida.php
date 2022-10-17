<?php

include_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/SimecHashids.class.inc";

$db = new cls_banco();
$_SESSION['superuser'] = true;

function __autoload($class_name)
{
    $arCaminho = array(
        APPRAIZ . "adm/classes/",
        APPRAIZ . "includes/classes/modelo/public/",
        APPRAIZ . "includes/classes/modelo/app/",
        APPRAIZ . "includes/classes/modelo/territorios/",
        APPRAIZ . "includes/classes/modelo/entidade/",
        APPRAIZ . "includes/classes/modelo/corporativo/",
        APPRAIZ . "includes/classes/modelo/seguranca/",
        APPRAIZ . "includes/classes/modelo/mensageria/",
        APPRAIZ . "includes/classes/",
        APPRAIZ . "clubvidacard/classes/",
    );

    foreach ($arCaminho as $caminho) {
        $arquivo = $caminho . $class_name . '.class.inc';
        if (file_exists($arquivo)) {
            require_once($arquivo);
            break;
        }
    }
}
require_once(APPRAIZ . 'includes/workflow_constantes.php');
require_once(APPRAIZ . 'includes/workflow.php');
$mEntidade = new CartaoAderencia();
$mMunicipio = new Municipio();
$mUnidadeFederacao = new UnidadeFederacao();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar':
            $params = array_merge($_POST, getParam());
            $mEntidade->salvarCartaoAderencia($params);
            $mEntidade->commit();

            echo "<script>alert('Solicitação de cadastro registrada com Sucesso!');history.go(-1);</script>";
            break;

        case 'carregarMunicipios':
            ob_clean();

            $ufId = $_POST['uf_id'];
            $municipio_id = $_POST['municipio_id'];
            $municipios = array();
            $titulo = 'Selecione um estado';

            if ($municipio_id) {
                $mMunicipio->carregarPorId($municipio_id);
                $ufId = $mMunicipio->uf_id;
            }

            if ($ufId) {
                $titulo = 'Município';
                $mMunicipio = new Municipio();
                $municipios = $mMunicipio->recuperarTodos(
                    'id AS codigo, municipio AS descricao',
                    array("uf_id = '{$ufId}'"),
                    2
                );
            }

            $db->monta_combom(
                "municipio_id",
                $municipios,
                'S',
                $titulo,
                '',
                '',
                '',
                '',
                'S',
                '',
                false,
                null,
                null,
                true
            );
            // echo "<script  type='text/javascript'>
            //             $(function(){
            //                 $('[name=uf_id]').val({$ufId}).selectpicker('refresh');
            //                 $('[name=municipio_id]').selectpicker('refresh');
            //             });
            //       </script>";
            die;
            break;
            //        case 'verificarCpf':
            //            $p = getParam();
            //            require APPRAIZ . "adm/modulos/principal/includes/pessoafisica/formulario.inc";
            //            break;
    }
}
$espHabilitado = 'N';
$params = getParam();

$dependentes = [];
$dados_pagamento = [];

if (isset($params['id']) && !empty($params['id'])) {
    $dados = $mEntidade->getDadosbyId($params['id']);
    extract($dados);


    $dependentes = utf8ArrayDecode(json_decode(utf8_encode($dependentes), true));
    $dados_pagamento = utf8ArrayDecode(json_decode(utf8_encode($dados_pagamento), true));
}





?>
<html>

<head>
    <meta charset="ISO-8859-1" />
    <title>ClubVida</title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <meta content="width=device-width, initial-scale=1" name="viewport" />
    <meta content="" name="description" />
    <meta content="" name="author" />
    <!-- BEGIN GLOBAL MANDATORY STYLES -->
    <link href="http://fonts.googleapis.com/css?family=Open+Sans:400,300,600,700&subset=all" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/simple-line-icons/simple-line-icons.min.css" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap/css/bootstrap.min.css" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/bootstrap-switch/css/bootstrap-switch.min.css" rel="stylesheet" type="text/css" />
    <!-- END GLOBAL MANDATORY STYLES -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->
    <link href="/includes/metronic/global/plugins/select2/css/select2.min.css" rel="stylesheet" type="text/css" />
    <link href="/includes/metronic/global/plugins/select2/css/select2-bootstrap.min.css" rel="stylesheet" type="text/css" />
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN THEME GLOBAL STYLES -->
    <link href="/includes/metronic/global/css/components.min.css" rel="stylesheet" id="style_components" type="text/css" />
    <link href="/includes/metronic/global/css/plugins.min.css" rel="stylesheet" type="text/css" />
    <!-- END THEME GLOBAL STYLES -->
    <!-- BEGIN PAGE LEVEL STYLES -->
    <link href="/includes/metronic/pages/css/login.min.css" rel="stylesheet" type="text/css" />
    <!-- END PAGE LEVEL STYLES -->
    <!-- BEGIN THEME LAYOUT STYLES -->
    <!-- END THEME LAYOUT STYLES -->
    <link rel="shortcut icon" href="/favicon.ico?v=<?= VERSAO ?>" />
    <script src="https://code.jquery.com/jquery-1.9.1.js"></script>

</head>

<body>
    <div class="content">
        <div class="portlet light bordered">
            <div class="portlet-title">
                <div class="caption font-blue-sharp">
                    <i class="icon-briefcase font-blue-sharp"></i>
                    <span class="caption-subject bold uppercase">
                        Formulário de Adesão
                    </span>
                </div>

            </div>
            <div class="portlet-body form">
                <form role="form" name="formulario-adesao" class="formulario-adesao" method="POST">
                    <div class="form-body">

                        <div class="row">
                            <div class="col-md-12">

                                <div class="portlet light bg-inverse">
                                    <div class="portlet-title">
                                        <div class="caption">
                                            <span class="caption-subject bold uppercase"> Aderente</span>
                                        </div>
                                    </div>
                                    <div class="portlet-body">

                                        <div class="row">
                                            <div class="col-md-6">
                                                <div class="form-group">
                                                    <label>Nome completo de responsável</label>
                                                    <?= campo_textom('nome', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>Data de nascimento</label>
                                                    <?= campo_datam('dt_nascimento', 'S', 'S', '', 'S', '', ''); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>CPF</label>
                                                    <?= campo_textom('cpf', 'S', 'S', '', 19, 19, '999.999.999-99', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                        </div>


                                        <div class="row">
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>Identidade (RG)</label>
                                                    <?= campo_textom('rg', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-2">
                                                <div class="form-group">
                                                    <label>Orgão Emissor</label>
                                                    <?= campo_textom('orgao_emissor', 'S', 'S', '', 20, 20, '', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>CEP</label>
                                                    <?= campo_textom('cep', 'S', 'S', '', 90, 90, '99.999-999', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-1">
                                                <div class="form-group">
                                                    <label>UF</label>
                                                    <?php
                                                    $mUnidadeFederacao = new UnidadeFederacao();
                                                    $estados = $mUnidadeFederacao->recuperarTodos(
                                                        'id AS codigo, uf_sigla || \' - \' || uf_nome_estado AS descricao',
                                                        array(),
                                                        2
                                                    );
                                                    $db->monta_combom(
                                                        "uf_id",
                                                        $estados,
                                                        'S',
                                                        'Estado',
                                                        '',
                                                        '',
                                                        '',
                                                        '',
                                                        'S',
                                                        '',
                                                        false,
                                                        null,
                                                        null,
                                                        true
                                                    );
                                                    ?>
                                                </div>
                                            </div>
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>Município</label>
                                                    <div id="cidades">
                                                        <?php

                                                        $municipios = array();
                                                        $textoMunicipio = 'Selecione um estado';
                                                        if ($uf_id) {
                                                            $municipios = $mMunicipio->recuperarTodos(
                                                                'id AS codigo, municipio AS descricao',
                                                                array("uf_id = '{$uf_id}'"),
                                                                2
                                                            );
                                                            $textoMunicipio = "Município";
                                                        }
                                                        $db->monta_combom(
                                                            "municipio_id",
                                                            $municipios,
                                                            'S',
                                                            $textoMunicipio,
                                                            '',
                                                            '',
                                                            '',
                                                            '',
                                                            'S',
                                                            '',
                                                            false,
                                                            $municipio_id,
                                                            null,
                                                            true
                                                        );
                                                        ?>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>


                                        <div class="row">
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>Bairro</label>
                                                    <?= campo_textom('bairro', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-6">
                                                <div class="form-group">
                                                    <label>Logradouro: Rua / Avenida / Complemento</label>
                                                    <?= campo_textom('logradouro', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>Residência própria</label>
                                                    <br />
                                                    <?php
                                                    $opcoes = array(
                                                        array('codigo' => 't', 'descricao' => 'Sim'),
                                                        array('codigo' => 'f', 'descricao' => 'Não'),
                                                    );

                                                    echo $db->monta_radiom('residencia_propria', $opcoes, 'S', 'S', 'f');
                                                    ?>
                                                </div>
                                            </div>
                                        </div>


                                        <div class="row">
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>Telefone</label>
                                                    <?= campo_textom('telefone', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>Celular</label>
                                                    <?= campo_textom('celular', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                            <div class="col-md-6">
                                                <div class="form-group">
                                                    <label>E-mail</label>
                                                    <?= campo_textom('email', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                                </div>
                                            </div>
                                        </div>


                                    </div>
                                </div>
                                <div class="portlet light bg-inverse">
                                    <div class="portlet-title">
                                        <div class="caption">
                                            <span class="caption-subject bold uppercase"> Dependentes</span>
                                        </div>
                                        <div class="actions">
                                            <a class="btn green add-dependente">
                                                <i class="fa fa-plus-circle"></i> Adicionar </a>
                                        </div>
                                    </div>
                                    <div class="portlet-body">

                                        <div class="empty-dependente">
                                            Nenhum dependente selecionado.
                                        </div>
                                        <table class="table table-bordered table-hover tabela-dependentes <?= (count($dependentes) <= 0) ? 'hidden' : '' ?>">
                                            <thead>
                                                <tr>
                                                    <th>NOME COMPLETO</th>
                                                    <th>PARENTESCO</th>
                                                    <th>DATA NASC.</th>
                                                    <th>CPF</th>
                                                    <th style="width: 40px;">Ação</th>
                                                </tr>
                                            </thead>
                                            <tbody>
                                                <? if (count($dependentes) <= 0) : ?>
                                                    <tr>
                                                        <td><?= campo_textom('dependentes[nome][]', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?></td>
                                                        <td><?php
                                                            $opcoes = array(
                                                                array('codigo' => '1', 'descricao' => 'Cônjuge'),
                                                                array('codigo' => '2', 'descricao' => 'Filho'),
                                                            );
                                                            $db->monta_combom("dependentes[parentesco][]", $opcoes, 'N', 'Selecione...', '', '', '', '', 'S');
                                                            ?>
                                                        </td>
                                                        <td><?= campo_textom('dependentes[dt_nascimento][]', 'N', 'S', '', 19, 19, 'dd/mm/yyyy', '', '', '', 0); ?></td>
                                                        <td><?= campo_textom('dependentes[cpf][]', 'N', 'S', '', 19, 19, '999.999.999-99', '', '', '', 0); ?></td>
                                                        <td><a onclick="rmDependente(this)" class="btn btn-circle btn-icon-only btn-default red rm-dependente" title="Excluir"> <i class="icon-trash"></i></a></td>
                                                    </tr>
                                                <? else : ?>

                                                    <? foreach ($dependentes as $dep) : ?>
                                                        <tr>
                                                            <td><?= campo_textom('dependentes[nome][]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dep['nome']); ?></td>
                                                            <td><?php
                                                                $opcoes = array(
                                                                    array('codigo' => '1', 'descricao' => 'Cônjuge'),
                                                                    array('codigo' => '2', 'descricao' => 'Filho'),
                                                                );
                                                                $db->monta_combom("dependentes[parentesco][]", $opcoes, 'S', 'Selecione...', '', '', '', '', 'S', '', false, $dep['parentesco']);
                                                                ?>
                                                            </td>
                                                            <td><?= campo_textom('dependentes[dt_nascimento][]', 'S', 'S', '', 19, 19, 'dd/mm/yyyy', '', '', '', 0, '', '', formata_data($dep['dt_nascimento'])); ?></td>
                                                            <td><?= campo_textom('dependentes[cpf][]', 'S', 'S', '', 19, 19, '999.999.999-99', '', '', '', 0, '', '', $dep['cpf']); ?></td>
                                                            <td><a onclick="rmDependente(this)" class="btn btn-circle btn-icon-only btn-default red rm-dependente" title="Excluir"> <i class="icon-trash"></i></a></td>
                                                        </tr>
                                                    <? endforeach; ?>

                                                <? endif; ?>

                                            </tbody>
                                        </table>

                                    </div>
                                </div>
                                <div class="portlet light bg-inverse">
                                    <div class="portlet-title">
                                        <div class="caption">
                                            <span class="caption-subject bold uppercase"> Autorização Responsável Financeiro/Mensalidade</span>
                                        </div>
                                    </div>
                                    <div class="portlet-body">
                                        <div class="row">
                                            <div class="col-md-3">
                                                <div class="form-group">
                                                    <label>Opção</label>
                                                    <div class="form-select-content">
                                                        <?php

                                                        $opcoes = [
                                                            //                                                    ['codigo' => 'EE', 'descricao' => 'Conta de energia elétrica'],
                                                            ['codigo' => 'DE', 'descricao' => 'Débito conta corrente'],
                                                            ['codigo' => 'CC', 'descricao' => 'Cartão de crédito'],
                                                            ['codigo' => 'CA', 'descricao' => 'Carnê'],
                                                        ];

                                                        $db->monta_combom("opcao_pagamento", $opcoes, 'S', 'Selecione...', '', '', '', '', 'S');

                                                        ?>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>

                                        <div class="opcao-pagamento opcao-pagamento-EE hidden">
                                            <div class="row">
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Instituição de Cobrança</label>
                                                        <?= campo_textom('conta_energia[instituicao_cobranca]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['instituicao_cobranca']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Relação TODOS x Concessionária</label>
                                                        <br />
                                                        <?php
                                                        $opcoes = array(
                                                            array('codigo' => 'T', 'descricao' => 'Títular'),
                                                            array('codigo' => 'C', 'descricao' => 'Cônjuge'),
                                                            array('codigo' => 'I', 'descricao' => 'Inquilino'),
                                                        );

                                                        echo $db->monta_radiom('conta_energia[relacao_concessionaria]', $opcoes, 'S', 'S', $dados_pagamento['relacao_concessionaria']);
                                                        ?>

                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>UC</label>
                                                        <?= campo_textom('conta_energia[uc]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['uc']); ?>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="row">
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>CPF cadastrado na conta</label>
                                                        <?= campo_textom('conta_energia[cpf_cadastrado]', 'S', 'S', '', 19, 19, '999.999.999-99', '', '', '', 0, '', '', $dados_pagamento['cpf_cadastrado']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Títular da conta de energia elétrica</label>
                                                        <?= campo_textom('conta_energia[titular]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['titular']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>PN</label>
                                                        <?= campo_textom('conta_energia[pn]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['pn']); ?>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>

                                        <div class="opcao-pagamento opcao-pagamento-DE hidden">
                                            <div class="row">
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Instituição de Cobrança</label>
                                                        <?= campo_textom('conta_bancaria[instituicao_cobranca]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['instituicao_cobranca']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Agência</label>
                                                        <?= campo_textom('conta_bancaria[agencia]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['agencia']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Conta Corrente</label>
                                                        <?= campo_textom('conta_bancaria[conta_corrente]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['conta_corrente']); ?>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="row">
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Dig.</label>
                                                        <?= campo_textom('conta_bancaria[dig]', 'S', 'S', '', 19, 19, '', '', '', '', 0, '', '', $dados_pagamento['dig']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Op.</label>
                                                        <?= campo_textom('conta_bancaria[op]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['op']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-2">
                                                    <div class="form-group">
                                                        <label>Dia de Vencimento</label>
                                                        <?= campo_textom('conta_bancaria[dia_vencimento]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['dia_vencimento']); ?>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>

                                        <div class="opcao-pagamento opcao-pagamento-CC hidden">
                                            <div class="row">
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Instituição de Cobrança</label>
                                                        <?= campo_textom('cartao_credito[instituicao_cobranca]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['instituicao_cobranca']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Cartão</label>
                                                        <?= campo_textom('cartao_credito[cartao]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['cartao']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Nome conforme consta no cartão</label>
                                                        <?= campo_textom('cartao_credito[nome]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['nome']); ?>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="row">
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Número do Cartão de crédito.</label>
                                                        <?= campo_textom('cartao_credito[numero]', 'S', 'S', '', 19, 19, '9999 9999 9999 9999', '', '', '', 0, '', '', $dados_pagamento['numero']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-2">
                                                    <div class="form-group">
                                                        <label>Código de segurança</label>
                                                        <?= campo_textom('cartao_credito[cod_seguranca]', 'S', 'S', '', 3, 3, '', '', '', '', 0, '', '', $dados_pagamento['cod_seguranca']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-2">
                                                    <div class="form-group">
                                                        <label>Validade do cartão (mes/ano)</label>
                                                        <?= campo_textom('cartao_credito[vencimento]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['vencimento']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-2">
                                                    <div class="form-group">
                                                        <label>Dia de vencimento</label>
                                                        <?= campo_textom('cartao_credito[dia_vencimento]', 'S', 'S', '', 2, 2, '', '', '', '', 0, '', '', $dados_pagamento['dia_vencimento']); ?>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>

                                        <div class="opcao-pagamento opcao-pagamento-CA hidden">
                                            <div class="row">
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Instituição de Cobrança</label>
                                                        <?= campo_textom('carne[instituicao_cobranca]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['instituicao_cobranca']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>Responsável financeiro</label>
                                                        <?= campo_textom('carne[nome_responsavel]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['nome_responsavel']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>CPF/CNPJ</label>
                                                        <?= campo_textom('carne[cpf_cnpj]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['cpf_cnpj']); ?>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="row">
                                                <div class="col-md-4">
                                                    <div class="form-group">
                                                        <label>RG.</label>
                                                        <?= campo_textom('carne[rg]', 'S', 'S', '', 19, 19, '', '', '', '', 0, '', '', $dados_pagamento['rg']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-2">
                                                    <div class="form-group">
                                                        <label>Telefone fixo</label>
                                                        <?= campo_textom('carne[telefone]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['telefone']); ?>
                                                    </div>
                                                </div>
                                                <div class="col-md-2">
                                                    <div class="form-group">
                                                        <label>Celular</label>
                                                        <?= campo_textom('carne[celular]', 'S', 'S', '', 90, 90, '', '', '', '', 0, '', '', $dados_pagamento['celular']); ?>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>

                                    </div>
                                </div>

                            </div>
                        </div>

                        <? if ($params['id']) : ?>
                            <div class="row">
                                <div class="col-md-12">
                                    <div class="form-group">
                                        <label>Motivo da alteração</label>
                                        <?= campo_textaream('motivo', 'S', 'S', '', '', '3', 500); ?>
                                    </div>
                                </div>
                            </div>
                        <? endif; ?>

                    </div>
                    <div class="form-actions">
                        <button type="submit" class="btn blue">
                            <i class="fa fa-save"></i> Salvar
                        </button>
                        <button type="button" class="btn btn-voltar">
                            <i class="fa fa-arrow-left"></i> Voltar
                        </button>
                    </div>

                    <input type="hidden" name="act" value="salvar" />
                </form>
            </div>
        </div>
    </div>
    <!--[if lt IE 9]>
    <script src="/includes/metronic/global/plugins/respond.min.js"></script>
    <script src="/includes/metronic/global/plugins/excanvas.min.js"></script>
    <![endif]-->
    <!-- BEGIN CORE PLUGINS -->

    <script src="/includes/metronic/global/plugins/jquery.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap/js/bootstrap.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/js.cookie.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-hover-dropdown/bootstrap-hover-dropdown.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-slimscroll/jquery.slimscroll.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery.blockui.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/bootstrap-switch/js/bootstrap-switch.min.js" type="text/javascript"></script>
    <!-- END CORE PLUGINS -->
    <!-- BEGIN PAGE LEVEL PLUGINS -->


    <script src="/includes/metronic/global/plugins/jquery-validation/js/jquery.validate.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-validation/js/additional-methods.min.js" type="text/javascript"></script>

    <script src="/includes/metronic/global/plugins/select2/js/select2.full.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/plugins/jquery-inputmask/jquery.inputmask.bundle.min.js" type="text/javascript"></script>
    <script src="/includes/metronic/global/scripts/app.js" type="text/javascript"></script>

    <script src="/includes/funcoes.js" type="text/javascript"></script>
    <!-- END PAGE LEVEL PLUGINS -->
    <!-- BEGIN PAGE LEVEL SCRIPTS -->
    <script type="text/javascript">
        function rmDependente(elm) {
            tr = $(elm).parents('tr');

            if ($('.tabela-dependentes tbody tr').length == 1) {
                $(tr).find('input').val('').prop('checked', false);
                $('.tabela-dependentes').addClass('hidden');
                $('.empty-dependente').removeClass('hidden');
            } else {
                tr.remove();
            }

            if ($('.tabela-dependentes tbody tr').length < 6) {
                $('.add-dependente')
                    .removeAttr('disabled')
                    .attr('title', 'Adicionar dependente');
            }
        }

        function limparDadosEndereco() {
            $('[name=logradouro]').val();
            $('[name=bairro]').val();
            $('[name=uf_id]').val();
        }

        function recuperarEnderecoPorCep() {
            var campoCep = $('[name=cep]');
            var cep = campoCep[0];

            if (!cep || cep.value == '' || cep.value.replace(/[^0-9]/ig, '').length != 8) {
                if (cep.value != '') {
                    addMsgErroCampo(campoCep, 'O CEP informado é inválido.');
                }

                limparDadosEndereco();
                return false;
            }

            App.blockUI({
                message: 'Carregando...',
                target: '#endereco'
            });

            $.ajax({
                type: "POST",
                url: "/geral/dne.php",
                data: {
                    opt: 'dne',
                    endcep: cep.value
                },
                success: function(retorno) {
                    eval(retorno);

                    if (DNE[0] && DNE.length >= 1) {
                        $('[name=logradouro]').val(DNE[0].logradouro);
                        $('[name=bairro]').val(DNE[0].bairro);
                        $('[name=uf_id]').val(DNE[0].estado).selectpicker('refresh');

                        carregarMunicipio(DNE[0].muncod);
                    } else {
                        limparDadosEndereco();
                    }

                    App.unblockUI('.page-content .container');
                },
                error: function(result) {
                    App.unblockUI('.page-content .container');
                },
            });
        }

        function carregarMunicipio(municipio_id) {
            var uf_id = $('select[name=uf_id]').val();
            App.blockUI({
                message: 'Carregando...',
                target: '#cidades'
            });

            $('div#cidades').load(window.location.href, {
                act: 'carregarMunicipios',
                uf_id: uf_id,
                municipio_id: municipio_id
            }, function() {
                App.unblockUI('#cidades');
            });
        }
        jQuery(document).ready(function() {


            $('input[type=text][name=cep]').change(function() {
                recuperarEnderecoPorCep();
            });


            $('input[name="telefone"], input[name="celular"]').inputmask({
                mask: ["(99) 9999-9999", "(99) 99999-9999"],
            });

            $('input[name="rg"]').inputmask({
                mask: ["9999999"],
            });

            $('input[name="carne\[telefone\]\[\]"], input[name="carne\[celular\]\[\]"]').inputmask({
                mask: ["(99) 9999-9999", "(99) 99999-9999"],
            });
            $('input[name="carne\[cpf_cnpj\]\[\]"]').inputmask({
                mask: ["999.999.999-99", "99.999.999/9999-99"],
            });

            $('select[name=uf_id]').change(function() {
                carregarMunicipio();
            });

            $('input[name="cartao_credito\[vencimento\]"]').inputmask("m/y", {
                "placeholder": "MM/YYYY"
            });

            $('select[name=opcao_pagamento]').change(function() {
                $('.opcao-pagamento input').removeAttr('required');
                $('.opcao-pagamento').addClass('hidden');
                $('.opcao-pagamento-' + $(this).val()).removeClass('hidden');
                $('.opcao-pagamento-' + $(this).val() + ' input').prop('required', true);

                if ('<?= $opcao_pagamento ?>' != $(this).val()) {
                    $('.opcao-pagamento-' + $(this).val() + ' input').val('');
                }

            }).change();

            $('.btn-voltar').click(function() {
                history.go(-1);
            });

            $('.add-dependente').click(function() {
                if ($('.tabela-dependentes tbody tr').length >= 6) {
                    return;
                }

                if ($('.tabela-dependentes').hasClass('hidden')) {
                    $('.tabela-dependentes').removeClass('hidden');
                    $('.empty-dependente').addClass('hidden');
                    $('.tabela-dependentes').find('[name="dependentes\[dt_nascimento\]"]').inputmask("d/m/y");
                    $('.tabela-dependentes').find('[name="dependentes\[cpf\]"]').inputmask("999.999.999-99");
                } else {
                    tr = $('.tabela-dependentes tbody tr:eq(0)').clone();
                    $(tr).find('input').val('').prop('checked', false);;
                    $('.tabela-dependentes tbody').append(tr);

                    $('.tabela-dependentes').find('[name="dependentes\[dt_nascimento\]\[\]"]').inputmask("d/m/y");
                    $('.tabela-dependentes').find('[name="dependentes\[cpf\]\[\]"]').inputmask("999.999.999-99");

                    $(tr).find('td:eq(1)').html($(tr).find('select'));
                    $(tr).find('select').selectpicker();
                }


                if ($('.tabela-dependentes tbody tr').length == 6) {
                    $('.add-dependente')
                        .attr('disabled', 'disabled')
                        .attr('title', 'Quantidade máxima de dependentes alcançada.');
                }

            });


        });
    </script>
    <!-- END PAGE LEVEL SCRIPTS -->
    <!-- BEGIN THEME LAYOUT SCRIPTS -->
    <!-- END THEME LAYOUT SCRIPTS -->
</body>

</html>