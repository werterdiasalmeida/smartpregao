<?php
$mTabela = new TabelaProcedimentoConvenio();
$mTabelaPreco = new TabelaPreco();

$p = getParam();
$pUrlParam = setParam(array('id' => $p['id']));

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar' :
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            unset($params['id']);
            $existe = !empty($params['tabela_preco_id']) ? true : false;

            $tabelaPrecoId = $mTabelaPreco->salvarTabelaPreco($params);
            if(!$existe){
                $mTabelaPreco->atualizaTabelaPrecoInfoVigente($tabelaPrecoId);
            }

            $mTabelaPreco->commit();
            $pUrlParam = setParam(array('id' => $p['id'],'tabela_preco_id' => $tabelaPrecoId));
            $db->sucessom('principal/estabelecimento/tabelaprecos/formulario', $pUrlParam);
            break;
        case 'salvar-individual' :
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            unset($params['id']);

            //salvando o registro de tabela preco
            $tabelaPrecoId = $mTabelaPreco->salvarTabelaPreco($params);
            $params['tabela_preco_id'] = $tabelaPrecoId;

            //Salvando o registro individual
            $mTabela->salvarTabelaIndividual($params, true, false);

            $mTabela->commit();
            $db->sucessomAjax("",array('tabela_preco_id' => $tabelaPrecoId));
            break;
        case 'salvar-individual-profissional' :
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            unset($params['id']);

            //salvando o registro de tabela preco
            $tabelaPrecoId = $mTabelaPreco->salvarTabelaPreco($params);
            $params['tabela_preco_id'] = $tabelaPrecoId;

            //Salvando o registro individual
            $mTabela->salvarTabelaIndividual($params, false,true);

            $mTabela->commit();
            $db->sucessomAjax('',array('tabela_preco_id' => $tabelaPrecoId));
            break;
        case 'valida-range-datas' :
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            unset($params['id']);

            $params['inicio'] = formata_data_sql($params['inicio']);
            $params['fim'] = formata_data_sql($params['fim']);

            //Verifica se existe registros entre as datas informadas
            $qtd = $mTabelaPreco->getQtdRegistroEntreDatas($params['estabelecimento_id'], $params['inicio'],$params['fim'],$params['tabela_preco_id']);
            $db->sucessomAjax('',array('quantidade_registros' => $qtd));
            break;
        case 'excluir-valor-procedimento' :
            $params = array_merge($_POST, getParam());
            $params['estabelecimento_id'] = $params['id'];
            unset($params['id']);

            $mValorProcedimento = new ValorProcedimento();

            $mValorCodigo = new ValorProcedimentoCodigo();
            $mTabelaProcConvenio = new TabelaProcedimentoConvenio();

            $tabelaPrecoId = $mTabelaProcConvenio->getIdByTabelaPrecoConvenio($p['tabela_preco_id'], $params['convenio_id']);

            $tabelaValorProcedimentoId = $mValorProcedimento->getIdByProcedimentoTabela($params['procedimento_id'],$tabelaPrecoId);
            $codigos = $mValorCodigo->getByProcedimentoValor($params['procedimento_id'],$tabelaValorProcedimentoId);

            if(is_array($codigos) && !empty($codigos)){
                foreach ($codigos as $codigo) {
                    if($codigo['id']){
                        $mValorCodigo->setValorNull($codigo['id']);
                    }
                }

            }

            $mValorProcedimento->excluirPorConvenioProcedimento($params['convenio_id'],$params['procedimento_id'],$params['tabela_preco_id']);

            $mValorProcedimento->commit();
            $mValorCodigo->commit();

            $db->sucessomAjax('',array('tabela_preco_id' => $params['tabela_preco_id']));
            break;
        case 'carregar-codigos-procedimento' :
            include_once 'tabelaValoresProcedimento.inc';
            die;
            break;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();

UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(array('id' => $p['id'])), null, array(), true, true);

$valoresTabela = array();
$dataHabilitada = 'S';
$estabelecimentoId = $p['id'];

if(isset($p['tabela_preco_id']) && $p['tabela_preco_id'] != ''){
    $mConvenio = new ConvenioEstabelecimento();
    $convenios = $mConvenio->getList(array("con.estabelecimento_id = {$p['id']}"), false);
    $jsonConvenios = json_encode(utf8ArrayEncode(array_map(function($convenio){
        return array(
            'codigo' => $convenio['id'],
            'descricao' => $convenio['nome'],
            'ordem' => $convenio['ordem'],
        );
    }, $convenios)));

    $mProcedimento = new Procedimento();
    $procedimentos = $mProcedimento->getList(array("(pro.estabelecimento_id = {$p['id']} OR pro.estabelecimento_id IS NULL)"), false);
    $jsonProcedimentos = json_encode(utf8ArrayEncode(array_map(function($procedimento){
        return array(
            'codigo' => $procedimento['id'],
            'descricao' => $procedimento['descricao'],
        );
    }, $procedimentos)));

    $mTabelaPreco->carregarPorId($p['tabela_preco_id']);
    $dadosTabelaPreco = $mTabelaPreco->getDados();

    $inicio = $dadosTabelaPreco['inicio_vigencia'];
    $fim = $dadosTabelaPreco['fim_vigencia'];
    $percentualGlobal = $dadosTabelaPreco['percentual_global'];
    $dataHabilitada = 'N';

    $valoresTabela = $mTabela->getValoresTabelaAgrupadosByTabelaPreco($dadosTabelaPreco['id']);
    $profissionaisValores = $mTabela->getValoresTabelaProcedimentosProfissionaisByTabelaPreco($estabelecimentoId,$dadosTabelaPreco['id']);

    $valorProfissionaisPorProcedimento = $mTabela->getValoresProcedimentosProfissionaisAgrupados($profissionaisValores);
    $jsonValorProfissionaisPorProcedimento = json_encode(utf8ArrayEncode($valorProfissionaisPorProcedimento));
}

$percentualGlobal = formata_valor(($percentualGlobal ?: PERCENTUAL_GLOBAL_REPASSE_MEDICO));
?>
<style type="text/css">
    .procedimento, .profissional {
        min-width: 400px;
        height: 35px;
    }

    .convenio {
        min-width: 115px;
        text-align: center;
    }

    table.tabela-precos {
        margin-bottom: 0;
    }

    table.tabela-precos-profissional th {
        text-align: center;
    }

    .DTFC_Cloned{
        margin-top: 1px;
        border-right: 1px solid #e7ecf1;
    }

    .nao-atende, .nao-atende:hover {
        background-color: #E08283 !important;
    }

    .label-nao-atende {
        cursor: pointer;
        color: #ffffff;
        font-weight: bolder;
    }

    .convenio{
        cursor: pointer;
    }

    .convenio:hover {
        cursor: pointer;
        background-color: #d5e6fb;
    }

    .profssional-valor-container {
        text-align: center;
        min-width: 115px;
    }

</style>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-table font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações da tabela de preços
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="tabela-precos-form" method="POST">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Início da vigência</label>
                            <div class="input-group">
                                <?= campo_datam('inicio', 'S', $dataHabilitada, '', 'S'); ?>
                                <?php if($dataHabilitada === 'S') : ?>
                                    <span class="input-group-addon">
                                        <i class="fa fa-calendar"></i>
                                    </span>
                                <?php endif; ?>
                            </div>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Fim da vigência</label>
                            <div class="input-group">
                                <?= campo_datam('fim', 'S', 'S', '', 'S'); ?>
                                <span class="input-group-addon">
                                    <i class="fa fa-calendar"></i>
                                </span>
                            </div>
                        </div>
                    </div>
                    <div class="col-md-4">
                        <div class="form-group">
                            <label>Percentual global</label>
                            <div class="input-group">
                                <?= campo_textom('percentualGlobal', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                <span class="input-group-addon" style="color: #c5cdda !important;">
                                    <strong>%</strong>
                                </span>
                            </div>
                        </div>
                    </div>
                </div>
                <?php if(isset($p['tabela_preco_id']) && $p['tabela_preco_id'] != ''): ?>
                    <div class="row">
                        <div class="col-md-12">
                            <div class="row">
                                <div class="col-md-12">
                                    <fieldset>
                                        <legend>Pesquisa</legend>
                                            <div class="row">
                                                <div class="col-md-6">
                                                    <div class="form-group">
                                                        <label class="control-label">Procedimentos</label>
                                                        <select id="procedimentos" name="procedimentos" multiple style="display: none;"></select>
                                                    </div>
                                                </div>
                                                <div class="col-md-6">
                                                    <div class="form-group">
                                                        <label class="control-label">Convênios/Planos</label>
                                                        <select id="convenios" name="convenios" multiple style="display: none;"></select>
                                                    </div>
                                                </div>
                                            </div>
                                    </fieldset>
                                </div>
                            </div>

                            <table class="tabela-precos table table-stripped" style="width: 100%;">
                                <thead>
                                    <tr>
                                        <th class="procedimento set-height">Procedimento</th>
                                        <th></th>
                                        <?php foreach($convenios as $convenio) : ?>
                                            <th class="convenio convenios convenio_<?= $convenio['id'] ?>"><?= $convenio['nome'] ?></th>
                                        <?php endforeach; ?>
                                    </tr>
                                </thead>
                                <tbody>
                                    <?php foreach($procedimentos as $procedimento) : ?>
                                        <tr class="procedimentos procedimento_<?= $procedimento['id'] ?>">
                                            <td class="procedimento">
                                                <span class="descricao"><?= $procedimento['descricao'] ?></span>
                                            </td>
                                            <td>
                                                <?php if(isset($valorProfissionaisPorProcedimento[$procedimento['id']]) && count($valorProfissionaisPorProcedimento[$procedimento['id']]) > 0) : ?>
                                                    <a class="btn btn-circle btn-icon-only btn-default green" href="javascript: exibirModalValoresProfissional(<?= $procedimento['id'] ?>, '<?= addslashes($procedimento['descricao']) ?>');" data-toggle="tooltip" title="Valores por profissional" data-placement="right">
                                                        <i class="fa fa-plus-circle"></i>
                                                    </a>
                                                <?php endif; ?>
                                            </td>
                                            <?php
                                            foreach($convenios as $convenio) :
                                                $valor = isset($valoresTabela[$convenio['id']][$procedimento['id']])
                                                    ? formata_valor($valoresTabela[$convenio['id']][$procedimento['id']]['valor'])
                                                    : null;
                                                ?>
                                                <td class="convenio convenios convenio_<?= $convenio['id'] ?>" onclick="exibeModalAlteracaoValores('<?= $convenio['id'] ?>', '<?= $procedimento['id'] ?>', '<?= $valor ?>')">
                                                    <div class="convenio_<?= $convenio['id'] ?>_procedimento_<?= $procedimento['id'] ?>" data-delay='{ "show": 500 }' data-toggle="tooltip" title="Clique aqui para editar" data-placement="right" >
                                                        <span class="valor-formatado">
                                                            <?= ($valor ? $valor : '-'); ?>
                                                        </span>
                                                        <input type="hidden" id="valor" value="<?= ($valoresTabela[$convenio['id']][$procedimento['id']]['valor']) ?>">
                                                        <input type="hidden" id="porcentagemProfissional" value="<?= ($valoresTabela[$convenio['id']][$procedimento['id']]['percentual_profissional']) ?>">
                                                        <input type="hidden" id="valorProfissional" value="<?= ($valoresTabela[$convenio['id']][$procedimento['id']]['valor_profissional']) ?>">
                                                    </div>
                                                </td>
                                            <?php endforeach; ?>
                                        </tr>
                                    <?php endforeach; ?>
                                </tbody>
                            </table>
                        </div>
                    </div>
                <?php endif; ?>
            </div>
            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
                <button type="button" class="btn btn-voltar">
                    <i class="fa fa-arrow-left"></i> Voltar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar"/>
        </form>
    </div>
</div>

<?php if(isset($p['tabela_preco_id']) && $p['tabela_preco_id'] != ''): ?>
    <div class="modal fade in" id="modal-profissional" tabindex="-1" role="modal-profissional" aria-hidden="true">
        <div class="modal-dialog" style="width: 90%;">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Valores do procedimento <strong class="procedimento"></strong></h4>
                </div>
                <div class="modal-body">
                    <div class="tabela-precos-profissional-container">

                    </div>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn" data-dismiss="modal">Fechar</button>
                </div>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>


    <div class="modal fade in" id="modal-alteracao-valores" tabindex="-1" role="modal-alteracao-valores" aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Atualizar os valores <strong class="procedimento"></strong></h4>
                </div>
                <form role="form" name="formulario-valores" class="valores-procedimento-form" method="POST">
                    <input type="hidden" id="procedimento">
                    <input type="hidden" id="convenio">
                    <div class="modal-body">
                        <div class="row">
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Valor do procedimento</label>
                                    <div class="input-group">
                                        <?= campo_textom('valorProcedimento', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                        <span class="input-group-addon">
                                            <i class="fa fa-money"></i>
                                        </span>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label>Valores de repasse padrão?</label>
                                    <?= campo_radiom('repassePadrao', array("Sim" => array("valor" => 'S', 'default' => true, 'callback' => 'gerenciaCamposRepasse'), "Não" => array("valor" => 'N', 'callback' => 'gerenciaCamposRepasse')), 'N'); ?>
                                </div>
                            </div>
                            <div class="campos-repasse">
                                <div class="col-md-6">
                                    <div class="form-group">
                                        <label>Porcentagem do profissional</label>
                                        <div class="input-group">
                                            <?= campo_textom('percentualProfissional', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                            <span class="input-group-addon" style="color: #c5cdda !important;">
                                                <strong>%</strong>
                                            </span>
                                        </div>
                                    </div>
                                </div>
                                <div class="col-md-6">
                                    <div class="form-group">
                                        <label>Valor do profissional</label>
                                        <div class="input-group">
                                            <?= campo_textom('valorProfissional', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                            <span class="input-group-addon">
                                                <i class="fa fa-money"></i>
                                            </span>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-12 codigos-procedimento valores-procedimento">

                            </div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn red btn-excluir-valores"> <i class="fa fa-save"></i> Excluir valores </button>
                        <button type="submit" class="btn blue"> <i class="fa fa-save"></i> Salvar </button>
                        <button type="button" class="btn" data-dismiss="modal">Cancelar</button>
                    </div>
                </form>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>

    <div class="modal fade in" id="modal-alteracao-valores-profissional" tabindex="-1" role="modal-alteracao-valores-profissional" aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Atualizar os valores por profissional<strong class="procedimento"></strong></h4>
                </div>
                <form role="form" name="formulario-valores-profissional" class="valores-procedimento-profissional-form" method="POST">
                    <input type="hidden" id="profissional">
                    <input type="hidden" id="procedimento">
                    <input type="hidden" id="convenio">
                    <div class="modal-body">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="bold">
                                        Procedimento
                                    </label>
                                    <div class="input-group label-procedimento">

                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label class="bold">
                                        Profissional
                                    </label>
                                    <div class="input-group label-profissional">
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6">
                                <div class="form-group">
                                    <label class="bold">
                                        Convênio/Plano
                                    </label>
                                    <div class="input-group label-convenio">
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label>O profissional atende esse procedimento para esse convênio/plano?</label>
                                    <?= campo_radiom('executaProcedimento', array("Sim" => array("valor" => 'S', 'default' => true, 'callback' => 'gerenciaCamposExecutaProcedimento'), "Não" => array("valor" => 'N', 'callback' => 'gerenciaCamposExecutaProcedimento')), 'S'); ?>
                                </div>
                            </div>
                            <div class="col-md-6 valores-procedimento">
                                <div class="form-group">
                                    <label>Valor do procedimento</label>
                                    <div class="input-group">
                                        <?= campo_textom('valorProcedimento', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                        <span class="input-group-addon">
                                            <i class="fa fa-money"></i>
                                        </span>
                                    </div>
                                </div>
                            </div>
                            <div class="col-md-6 valores-procedimento">
                                <div class="form-group">
                                    <label>Valores de repasse padrão?</label>
                                    <?= campo_radiom('repassePadrao', array("Sim" => array("valor" => 'S', 'default' => true, 'callback' => 'gerenciaCamposRepasseProfissional'), "Não" => array("valor" => 'N', 'callback' => 'gerenciaCamposRepasseProfissional')), 'N'); ?>
                                </div>
                            </div>
                            <div class="campos-repasse">
                                <div class="col-md-6 valores-procedimento">
                                    <div class="form-group">
                                        <label>Porcentagem do profissional</label>
                                        <div class="input-group">
                                            <?= campo_textom('percentualProfissional', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                            <span class="input-group-addon" style="color: #c5cdda !important;">
                                                <strong>%</strong>
                                            </span>
                                        </div>
                                    </div>
                                </div>
                                <div class="col-md-6 valores-procedimento">
                                    <div class="form-group">
                                        <label>Valor do profissional</label>
                                        <div class="input-group">
                                            <?= campo_textom('valorProfissional', 'N', 'S', '', 90, 90, '', '', '', '', 0); ?>
                                            <span class="input-group-addon">
                                                <i class="fa fa-money"></i>
                                            </span>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-12 codigos-procedimento valores-procedimento">

                            </div>
                        </div>
                    </div>
                    <div class="modal-footer">
                        <button type="submit" class="btn blue">
                            <i class="fa fa-save"></i> Salvar
                        </button>
                        <button type="button" class="btn" data-dismiss="modal">Cancelar</button>
                    </div>
                </form>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>
<?php endif; ?>

<script type="text/javascript" src="/includes/metronic/global/plugins/datatables/datatables.all.min.js"></script>
<script type="text/javascript">
    <?php if(isset($p['tabela_preco_id']) && $p['tabela_preco_id'] != ''): ?>
        var tabelaPrecos = null;
        var tabelaPrecosProfissionais = null;
        var profissionaisObj = {
            valorProfissionaisPorProcedimento : <?= $jsonValorProfissionaisPorProcedimento ?>,
            getValorPorProcedimento : function (procedimentoId){
                return this.valorProfissionaisPorProcedimento[procedimentoId] ? this.valorProfissionaisPorProcedimento[procedimentoId] : [];
            },
            setValorPorProcedimento : function (procedimentoId, profissionais){
                this.valorProfissionaisPorProcedimento[procedimentoId] = profissionais;
            }
        };
    <?php endif; ?>

    $(function () {

        $.validator.methods.min = function( value, element ) {
            var valor = desformataValor(value);
            return valor > 0;
        }

        $(".tabela-precos-form").validateForm({
            rules: {
                fim: {
                   dateGreaterThan: "[name=inicio]"
                }
            },
            submitHandler: function (e) {
                $(e).find('input[type=submit], button[type=submit]').prop('disabled', true).html('Enviando...');
                var handle = this;
                ajaxValidaRangeDatas(function(){
                    //sucesso
                    $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                    handle.submitForm(e);
                }, function(){
                    //erro
                    $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                });
            }
        });

        $(".valores-procedimento-form").validateForm({
            submitHandler: function (e) {
                $(e).find('input[type=submit], button[type=submit]').prop('disabled', true).html('Enviando...');

                //sucesso
                App.blockUI({
                    message: 'Carregando...',
                    target: '.form-body'
                });

                ajaxValidaRangeDatas(function(){
                    //sucesso
                    var convenio = $('#modal-alteracao-valores #convenio').val();
                    var percentualGlobal = $('[name=percentualGlobal]').val();
                    var procedimento = $('#modal-alteracao-valores #procedimento').val();

                    var valor = $('#modal-alteracao-valores #valorProcedimento').val();
                    var porcentagemProfissional = null;
                    var valorProfissional = null;
                    if($('#modal-alteracao-valores [name=repassePadrao]:checked').val() == 'N'){
                        porcentagemProfissional = desformataValor($('#modal-alteracao-valores #percentualProfissional').val());
                        valorProfissional = desformataValor($('#modal-alteracao-valores #valorProfissional').val());
                    }

                    $('#modal-alteracao-valores').modal('toggle');

                    let valorCodigo = {};
                    $('.valor-procedimento-codigo [name^=valor]:visible').each(function () {
                        valorCodigo[$(this).data('codigo')] = $(this).val();
                    });

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType : 'json',
                        data: {
                            act : 'salvar-individual',
                            convenio: convenio,
                            procedimento: procedimento,
                            valor: valor,
                            valorProfissional: valorProfissional,
                            percentualProfissional: porcentagemProfissional,
                            percentualGlobal: percentualGlobal,
                            inicio: $('[name=inicio]').val(),
                            fim:  $('[name=fim]').val(),
                            valor_codigo : valorCodigo
                        },
                        success: function (retorno) {
                            App.unblockUI('.form-body');
                            var divDados = $('.convenio_' + convenio + '_procedimento_' + procedimento);

                            if(valor === '' || valor === null){
                                valor = null;
                                porcentagemProfissional = null;
                                valorProfissional = null;
                            }

                            divDados.find('#valor').val(valor ? desformataValor(valor) : '');
                            divDados.find('#porcentagemProfissional').val(porcentagemProfissional);
                            divDados.find('#valorProfissional').val(valorProfissional);
                            divDados.find('.valor-formatado').html(valor ? valor : '-');

                            $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                            exibirSucesso(retorno.msg ? retorno.msg : 'Operação realizada com sucesso!');
                        },
                        error: function (result) {
                            App.unblockUI('.form-body');
                            $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                            exibirAlert('Não foi possível realizar a operação');
                        }
                    });
                }, function(){
                    $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                    //erro
                });
            }
        });

        $(".valores-procedimento-profissional-form").validateForm({
            submitHandler: function (e) {

                ajaxValidaRangeDatas(function(){
                    $(e).find('input[type=submit], button[type=submit]').prop('disabled', true).html('Enviando...');

                    //sucesso
                    App.blockUI({
                        message: 'Carregando...',
                        target: '.form-body'
                    });

                    var convenio = $('#modal-alteracao-valores-profissional #convenio').val();
                    var procedimento = $('#modal-alteracao-valores-profissional #procedimento').val();
                    var profissional = $('#modal-alteracao-valores-profissional #profissional').val();
                    var executaProcedimento = $('#modal-alteracao-valores-profissional [name=executaProcedimento]:checked').val();
                    var divDados = $('.convenio_' + convenio + '_profissional_' + profissional);

                    var porcentagemProfissional = null;
                    var valorProfissional = null;
                    var valor = null;

                    if(executaProcedimento == 'S'){
                        valor = $('#modal-alteracao-valores-profissional #valorProcedimento').val();
                        if($('#modal-alteracao-valores-profissional [name=repassePadrao]:checked').val() == 'N'){
                            porcentagemProfissional = desformataValor($('#modal-alteracao-valores-profissional #percentualProfissional').val());
                            valorProfissional = desformataValor($('#modal-alteracao-valores-profissional #valorProfissional').val());
                        }
                    }

                    let valorCodigo = {};
                    $('.valor-procedimento-codigo [name^=valor]:visible').each(function () {
                        valorCodigo[$(this).data('codigo')] = $(this).val();
                    });

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType : 'json',
                        data: {
                            act : 'salvar-individual-profissional',
                            convenio: convenio,
                            procedimento: procedimento,
                            profissional: profissional,
                            valor: valor,
                            valorProfissional: valorProfissional,
                            executaProcedimento: executaProcedimento,
                            percentualProfissional: porcentagemProfissional,
                            percentualGlobal: $('[name=percentualGlobal]').val(),
                            inicio: $('[name=inicio]').val(),
                            fim:  $('[name=fim]').val(),
                            valor_codigo : valorCodigo
                        },
                        success: function (retorno) {
                            App.unblockUI('.form-body');

                            if(valor === '' || valor === null){
                                valor = null;
                                porcentagemProfissional = null;
                                valorProfissional = null;
                                // executaProcedimento = 'S';
                            }

                            divDados.find('#valor').val(valor ? desformataValor(valor) : '');
                            divDados.find('#porcentagemProfissional').val(porcentagemProfissional);
                            divDados.find('#valorProfissional').val(valorProfissional);
                            divDados.find('.valor-formatado').html(valor ? valor : '-');

                            if(executaProcedimento == 'N'){
                                divDados.find('.label-nao-atende').show();
                                divDados.find('.valor-formatado').hide();
                                divDados.addClass('nao-atende');
                            }else{
                                divDados.find('.label-nao-atende').hide();
                                divDados.find('.valor-formatado').show();
                                divDados.removeClass('nao-atende');
                            }
                            $('#modal-alteracao-valores-profissional').modal('toggle');

                            var objetoProfissionais = profissionaisObj.getValorPorProcedimento(procedimento);
                            if(objetoProfissionais){
                                if(objetoProfissionais[profissional]){
                                    objetoProfissionais[profissional]['valor'] = objetoProfissionais[profissional]['valor'] ? objetoProfissionais[profissional]['valor'] : {};
                                    objetoProfissionais[profissional]['atende'] = objetoProfissionais[profissional]['atende'] ? objetoProfissionais[profissional]['atende'] : {};
                                    objetoProfissionais[profissional]['percentual_profissional'] = objetoProfissionais[profissional]['percentual_profissional'] ? objetoProfissionais[profissional]['percentual_profissional'] : {};
                                    objetoProfissionais[profissional]['valor_profissional'] = objetoProfissionais[profissional]['valor_profissional'] ? objetoProfissionais[profissional]['valor_profissional'] : {};

                                    objetoProfissionais[profissional]['valor'][convenio] = valor;
                                    objetoProfissionais[profissional]['atende'][convenio] = executaProcedimento == 'S' ? true : false;
                                    objetoProfissionais[profissional]['percentual_profissional'][convenio] = porcentagemProfissional;
                                    objetoProfissionais[profissional]['valor_profissional'][convenio] = valorProfissional;
                                    profissionaisObj.setValorPorProcedimento(procedimento,objetoProfissionais);
                                }
                            }
                            $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                            exibirSucesso(retorno.msg ? retorno.msg : 'Operação realizada com sucesso!');
                        },
                        error: function (result) {
                            App.unblockUI('.form-body');
                            exibirAlert('Não foi possível realizar a operação');
                            $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                        }
                    });
                }, function(){
                    //erro
                    $(e).find('input[type=submit], button[type=submit]').prop('disabled', false).html('<i class="fa fa-save"></i> Salvar');
                });
            }
        });

        $('.btn-voltar').click(function () {
            window.location = '?modulo=principal/estabelecimento/tabelaprecos/lista&acao=C<?= $pUrlParam ?>';
        });

        $('.btn-excluir-valores').click(function () {
            excluirValorProcedimento();
        });

        setMoneyMask($("input[type=text][name^=valor]"));
        setPercentageMask($("input[type=text][name^=percentual]"));

        <?php if(isset($p['tabela_preco_id']) && $p['tabela_preco_id'] != ''): ?>
            tabelaPrecos = $('.tabela-precos').DataTable( {
                scrollY:        "400px",
                scrollX:        true,
                scrollCollapse: true,
                paging:         false,
                ordering:       false,
                searching:      false,
                info:           false,
                fixedColumns:   {
                    leftColumns: 2
                }
            });

            $('#procedimentos').selectize({
                persist: false,
                maxItems: null,
                placeholder: 'Todos',
                plugins: ['remove_button'],
                valueField: 'codigo',
                labelField: 'descricao',
                searchField: ['descricao'],
                sortField: [
                    {field: 'descricao', direction: 'asc'}
                ],
                options: <?= $jsonProcedimentos?>,
                items: {},
                onChange: function () {
                    filtrarProcedimentosTabela(tabelaPrecos);
                }
            });

            $('#convenios').selectize({
                persist: false,
                maxItems: null,
                placeholder: 'Todos',
                plugins: ['remove_button'],
                valueField: 'codigo',
                labelField: 'descricao',
                searchField: ['descricao'],
                options: <?= $jsonConvenios?>,
                items: {},
                onChange: function () {
                    filtrarConveniosTabela(tabelaPrecos);
                }
            });

            $('#modal-alteracao-valores input').keyup(function(){
                calculaValoresProcedimentoPlano(this);
            });

            $('#modal-alteracao-valores-profissional input').keyup(function(){
                calculaValoresProcedimentoPlanoProfissional(this);
            });

            $('#modal-alteracao-valores-profissional').on('show.bs.modal', function (e) {
                $('#modal-profissional').modal('hide');
            }).on('hide.bs.modal', function (e) {
                $('#modal-profissional').modal('show');
            });
        <?php endif; ?>
    });


    function excluirValorProcedimento(){

        var convenio = $('#modal-alteracao-valores #convenio').val();
        var procedimento = $('#modal-alteracao-valores #procedimento').val();

        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType : 'json',
            data: {
                act : 'excluir-valor-procedimento',
                convenio_id: convenio,
                procedimento_id: procedimento,
            },
            success: function (retorno) {
                App.unblockUI('.form-body');
                var divDados = $('.convenio_' + convenio + '_procedimento_' + procedimento);

                if(valor === '' || valor === null){
                    valor = null;
                    porcentagemProfissional = null;
                    valorProfissional = null;
                }

                divDados.find('#valor').val('');
                divDados.find('#porcentagemProfissional').val(porcentagemProfissional);
                divDados.find('#valorProfissional').val(valorProfissional);
                divDados.find('.valor-formatado').html('-');

                exibirAviso('<strong>Valor do procedimento excluído com sucesso!</strong>');
                var modal = $('#modal-alteracao-valores');
                modal.modal('hide');
            },
            error: function (result) {
                exibirAviso('<strong>Não foi possível excluir o valor do procedimento</strong>');
            }
        });
    }

    function ajaxValidaRangeDatas(callbackSucesso,callbackErro){
        $.ajax({
            type: "POST",
            url: window.location.href,
            dataType : 'json',
            data: {
                act : 'valida-range-datas',
                inicio: $('[name=inicio]').val(),
                fim:  $('[name=fim]').val()
            },
            success: function (retorno) {
                if(retorno.params.quantidade_registros && retorno.params.quantidade_registros > 0){
                    exibirAviso('<strong>Datas de início e(ou) fim da vigência em conflito com alguma tabela de preço já cadastrada!</strong>');
                    callbackErro();
                }else{
                    callbackSucesso()
                }
            },
            error: function (result) {
                App.unblockUI('.form-body');
                exibirAlert('Não foi possível realizar a validação das datas de início e fim da vigência');
                callbackErro();
            }
        });
    }

    <?php if(isset($p['tabela_preco_id']) && $p['tabela_preco_id'] != ''): ?>

        function calculaValoresProcedimentoPlanoProfissional(elemento){
            var nomeCampo = $(elemento).prop('name');
            var valorProcedimento = desformataValor($('#modal-alteracao-valores-profissional #valorProcedimento').val());
            var percentualProfissional = desformataValor($('#modal-alteracao-valores-profissional #percentualProfissional').val());
            var percentualGlobal = desformataValor($('#percentualGlobal').val());
            var valorProfissional = desformataValor($('#modal-alteracao-valores-profissional #valorProfissional').val());

            var procedimentoId = $('#modal-alteracao-valores-profissional #procedimento').val();
            var convenioId = $('#modal-alteracao-valores-profissional #convenio').val();
            var divContainerConvenio = $('.convenio_' + convenioId + '_procedimento_' + procedimentoId);
            percentualGlobal =  new Number(divContainerConvenio.find('#porcentagemProfissional').val() || percentualGlobal);

            if($('#modal-alteracao-valores-profissional [name=repassePadrao]:checked').val() == 'S'){
                $('#modal-alteracao-valores-profissional #percentualProfissional').val(formataValor(percentualGlobal));
                valorProfissional = (valorProcedimento * percentualGlobal / 100);
                $('#modal-alteracao-valores-profissional #valorProfissional').val(formataValor(valorProfissional));
            }else{
                if(nomeCampo == 'percentualProfissional'){
                    valorProfissional = (valorProcedimento * percentualProfissional / 100);
                    $('#modal-alteracao-valores-profissional #valorProfissional').val(formataValor(valorProfissional));
                }else if(nomeCampo == 'valorProfissional'){
                    if(valorProfissional > valorProcedimento){
                        valorProfissional = valorProcedimento;
                        $('#modal-alteracao-valores-profissional #valorProfissional').val(formataValor(valorProfissional));
                    }
                    percentualProfissional = valorProfissional / valorProcedimento * 100;
                    $('#modal-alteracao-valores-profissional #percentualProfissional').val(formataValor(percentualProfissional));
                }else{
                    if(elemento){
                        valorProfissional = (valorProcedimento * percentualProfissional / 100);
                    }
                    $('#modal-alteracao-valores-profissional #valorProfissional').val(formataValor(valorProfissional));
                }
            }
        }

        function calculaValoresProcedimentoPlano(elemento){
            var nomeCampo = $(elemento).prop('name');
            var valorProcedimento = desformataValor($('#modal-alteracao-valores #valorProcedimento').val());
            var percentualProfissional = desformataValor($('#modal-alteracao-valores #percentualProfissional').val());
            var percentualGlobal = desformataValor($('#percentualGlobal').val());
            var valorProfissional = desformataValor($('#modal-alteracao-valores #valorProfissional').val());

            if($('#modal-alteracao-valores [name=repassePadrao]:checked').val() == 'S'){
                $('#modal-alteracao-valores #percentualProfissional').val($('#percentualGlobal').val());
                valorProfissional = (valorProcedimento * percentualGlobal / 100);
                $('#modal-alteracao-valores #valorProfissional').val(formataValor(valorProfissional));
            }else{
                if(nomeCampo == 'percentualProfissional'){
                    valorProfissional = (valorProcedimento * percentualProfissional / 100);
                    $('#modal-alteracao-valores #valorProfissional').val(formataValor(valorProfissional));
                }else if(nomeCampo == 'valorProfissional'){
                    if(valorProfissional > valorProcedimento){
                        valorProfissional = valorProcedimento;
                        $('#modal-alteracao-valores #valorProfissional').val(formataValor(valorProfissional));
                    }
                    percentualProfissional = valorProfissional / valorProcedimento * 100;
                    $('#modal-alteracao-valores #percentualProfissional').val(formataValor(percentualProfissional));
                }else{
                    if(elemento){
                        valorProfissional = (valorProcedimento * percentualProfissional / 100);
                    }
                    $('#modal-alteracao-valores #valorProfissional').val(formataValor(valorProfissional));
                }
            }
        }

        function gerenciaCamposExecutaProcedimento(elemento){
            var objeto = $(elemento);

            if(objeto.val() == 'N'){
                $('.valores-procedimento').hide();

                $('#modal-alteracao-valores-profissional #valorProcedimento').val('');
                $('#modal-alteracao-valores-profissional #percentualProfissional').val('');
                $('#modal-alteracao-valores-profissional #valorProfissional').val('');
                $('#modal-alteracao-valores-profissional #repassePadrao').val('');
            }else{
                $('.valores-procedimento').show();
            }
        }

        function gerenciaCamposRepasseProfissional(elemento){
            var objeto = $(elemento);

            if(objeto.val() == 'N'){
                $('#modal-alteracao-valores-profissional .campos-repasse input').prop('disabled','');
            }else{
                $('#modal-alteracao-valores-profissional .campos-repasse input').prop('disabled','disabled');
            }

            calculaValoresProcedimentoPlanoProfissional();
        }

        function gerenciaCamposRepasse(elemento){
            var objeto = $(elemento);

            if(objeto.val() == 'N'){
                $('#modal-alteracao-valores .campos-repasse input').prop('disabled','');
            }else{
                $('#modal-alteracao-valores .campos-repasse input').prop('disabled','disabled');
            }

            calculaValoresProcedimentoPlano();
        }

        function exibeModalAlteracaoValores(convenio,procedimento,valor){

            if(valor == ''){
                $('.btn-excluir-valores').hide();
            }else{
                $('.btn-excluir-valores').show();
            }

            if(!validaValoresTabelaPreco()){
                return false;
            }

            let modalAlteracaoValores = $('#modal-alteracao-valores');
            let codigosProcedimentoContainer = modalAlteracaoValores.find('.codigos-procedimento');

            codigosProcedimentoContainer.html('');

            var divDados = $('.convenio_' + convenio + '_procedimento_' + procedimento);
            var valor = divDados.find('#valor').val();
            var porcentagemProfissional = divDados.find('#porcentagemProfissional').val();
            var valorProfissional = divDados.find('#valorProfissional').val();

            modalAlteracaoValores.find('#valorProcedimento').val(formataValor(valor));
            modalAlteracaoValores.find('#percentualProfissional').val(formataValor(porcentagemProfissional));
            modalAlteracaoValores.find('#valorProfissional').val(formataValor(valorProfissional));
            modalAlteracaoValores.find('#procedimento').val(procedimento);
            modalAlteracaoValores.find('#convenio').val(convenio);

            if(porcentagemProfissional != ''){
                modalAlteracaoValores.find('[name=repassePadrao][value="N"]').prop('checked','checked');
            }else{
                modalAlteracaoValores.find('[name=repassePadrao][value="S"]').prop('checked','checked')
            }

            gerenciaCamposRepasse(modalAlteracaoValores.find('[name=repassePadrao]:checked'));
            modalAlteracaoValores.modal({backdrop: 'static'}).modal('show');

            App.blockUI({
                target : modalAlteracaoValores,
                message : 'Carregando...'
            });

            codigosProcedimentoContainer.load(window.location.href, {
                act : 'carregar-codigos-procedimento',
                procedimento : procedimento,
                convenio : convenio
            }, function () {
                App.unblockUI(modalAlteracaoValores);
            });
        }

        function exibeModalAlteracaoValoresProfissional(convenioId,profissionalId,procedimentoId){
            if(!validaValoresTabelaPreco()){
                return false;
            }

            var divContainer = $('.convenio_' + convenioId + '_profissional_' + profissionalId);
            var convenio = ($('.convenio_' + convenioId + ' span.descricao').html()).trim();
            var procedimento = ($('.procedimento_' + procedimentoId + ' span.descricao').html()).trim();
            var profissional = ($('span.profissional_' + profissionalId).html()).trim();

            if(divContainer.hasClass('nao-atende')){
                $('[name=executaProcedimento][value="N"]').prop('checked','checked');
            }else{
                $('[name=executaProcedimento][value="S"]').prop('checked','checked')
            }

            var valor = divContainer.find('#valor').val();
            var porcentagemProfissional = divContainer.find('#porcentagemProfissional').val();
            var valorProfissional = divContainer.find('#valorProfissional').val();

            let modalAlteracaoValores = $('#modal-alteracao-valores-profissional');
            let codigosProcedimentoContainer = modalAlteracaoValores.find('.codigos-procedimento');

            codigosProcedimentoContainer.html('');

            modalAlteracaoValores.find('#valorProcedimento').val(formataValor(valor));
            modalAlteracaoValores.find('#percentualProfissional').val(formataValor(porcentagemProfissional));
            modalAlteracaoValores.find('#valorProfissional').val(formataValor(valorProfissional));
            modalAlteracaoValores.find('#procedimento').val(procedimentoId);
            modalAlteracaoValores.find('#convenio').val(convenioId);
            modalAlteracaoValores.find('#profissional').val(profissionalId);

            if(porcentagemProfissional != ''){
                modalAlteracaoValores.find('[name=repassePadrao][value="N"]').prop('checked','checked');
            }else{
                modalAlteracaoValores.find('[name=repassePadrao][value="S"]').prop('checked','checked')
            }

            gerenciaCamposExecutaProcedimento( $('[name=executaProcedimento]:checked'));
            gerenciaCamposRepasseProfissional(modalAlteracaoValores.find('[name=repassePadrao]:checked'));

            modalAlteracaoValores.find('.label-profissional').html(profissional);
            modalAlteracaoValores.find('.label-procedimento').html(procedimento);
            modalAlteracaoValores.find('.label-convenio').html(convenio);
            $(modalAlteracaoValores).modal('show');

            App.blockUI({
                target : modalAlteracaoValores,
                message : 'Carregando...'
            });

            codigosProcedimentoContainer.load(window.location.href, {
                act : 'carregar-codigos-procedimento',
                procedimento : procedimentoId,
                convenio : convenioId,
                profissional : profissionalId
            }, function () {
                App.unblockUI(modalAlteracaoValores);
            });
        }

        function exibirModalValoresProfissional(id, procedimento){
            if(!validaValoresTabelaPreco()){
                return false;
            }

            var modal = $('#modal-profissional');
            modal.find('.procedimento').html(procedimento);
            modal.modal({
                backdrop: 'static',
            }).modal('show');

            modal.find('.tabela-precos-profissional-container').html(
                '<table class="tabela-precos-profissional table table-stripped" style="width: 100%; margin-bottom: 0px !important;"></table>'
                + '<input type="hidden" name="procedimento_selecionado" id="procedimento_selecionado" value="' + id + '" />'
            );

            setTimeout(function(){
                var valoresProcedimento = profissionaisObj.getValorPorProcedimento(id);
                var data = $.map(valoresProcedimento, function(value) {
                    <?php foreach($convenios as $convenio) : ?>
                    if(!value.convenio_<?= $convenio['id'] ?>) {
                        value.convenio_<?= $convenio['id'] ?> = null;
                    }
                    <?php endforeach; ?>

                    return value;
                });

                tabelaPrecosProfissionais = $('.tabela-precos-profissional').DataTable({
                    scrollY:        "400px",
                    scrollX:        true,
                    scrollCollapse: true,
                    paging:         false,
                    ordering:       false,
                    searching:      false,
                    info:           false,
                    fixedColumns:   {
                        leftColumns: 1
                    },
                    columns: [
                        { title: "Profissional",
                            data : "profissional",
                            className : "profissional",
                            render : function(data, type, row, meta){
                                return "<span class='profissional_" + row.profissional_id + "'>" + data + "</span>";
                            }},
                        <?php foreach($convenios as $convenio) : ?>
                        { title: "<span class='descricao'><?= $convenio['nome'] ?></span>",
                            data : "valor",
                            className : "convenios convenio convenio_<?= $convenio['id'] ?>",
                            render : function(data, type, row, meta){
                                var valor                   = row.valor ? (row.valor[<?= $convenio['id'] ?>] ? row.valor[<?= $convenio['id'] ?>] : '') : '';
                                var porcentagemProfissional = row.percentual_profissional ? (row.percentual_profissional[<?= $convenio['id'] ?>] ? row.percentual_profissional[<?= $convenio['id'] ?>] : '') : '';
                                var valorProfissional       = row.valor_profissional ? (row.valor_profissional[<?= $convenio['id'] ?>] ? row.valor_profissional[<?= $convenio['id'] ?>] : '') : '';
                                var atende                  = row.atende ? row.atende[<?= $convenio['id'] ?>] : null;
                                return '<div class="profssional-valor-container convenio_<?= $convenio['id'] ?>_profissional_' + row.profissional_id + (atende === false ? ' nao-atende ' : ' ') + '"  '
                                    + 'onclick="exibeModalAlteracaoValoresProfissional(\'<?= $convenio['id'] ?>\',\'' + row.profissional_id + '\',\'' + id + '\')" data-toggle="tooltip" title="Clique aqui para editar" data-placement="right">'
                                    + '<span class="valor-formatado" style="' + (atende === false ? 'display:none;' : '') + '">'
                                    + (valor ? formataValor(valor) : '-')
                                    + '</span>'
                                    + '<input type="hidden" id="valor" value="' + valor + '">'
                                    + '<input type="hidden" id="porcentagemProfissional" value="' + porcentagemProfissional + '">'
                                    + '<input type="hidden" id="valorProfissional" value="' + valorProfissional + '">'
                                    + '<label class="label-nao-atende" style="' + (atende !== false  ? 'display:none;' : '') + '">'
                                    + 'Não atende'
                                    + '</label>'
                                    + '</div>';
                            }},
                        <?php endforeach; ?>
                    ],
                    data : data
                });

                setMoneyMask($("input[type=text].profissional-valor-convenio"));

                $('input[type=checkbox].profissional-atende-convenio').on('change', function(){
                    if($(this).is(':checked')){
                        $(this).closest('td').addClass('nao-atende')
                            .find('.profissional-valor-convenio').prop('disabled', true).val('');
                    }else{
                        $(this).closest('td').removeClass('nao-atende')
                            .find('.profissional-valor-convenio').prop('disabled', false);
                    }
                }).change();

                filtrarConveniosTabela(tabelaPrecosProfissionais);
            }, 150);

        }

    function filtrarConveniosTabela(tabela){
        $('.convenios').hide();
        var convenios = $('#convenios').val();

        if(convenios){
            $(convenios).each(function(index, codigo){
                $('.convenio_' + codigo).show();
            });
        }else{
            $('.convenios').show();
        }

        tabela.draw();
    }

    function filtrarProcedimentosTabela(tabela){
        $('.procedimentos').hide();
        var procedimentos = $('#procedimentos').val();

        if(procedimentos){
            $(procedimentos).each(function(index, codigo){
                $('.procedimento_' + codigo).show();
            });
        }else{
            $('.procedimentos').show();
        }

        if(tabela){
            tabela.draw();
        }
    }

    function validaValoresTabelaPreco(){
        var inicio = $("[name=inicio]").val();
        var fim = $("[name=fim]").val();
        var percentualGlobal = $("[name=percentualGlobal]").val();

        if(inicio == '' || fim == '' || percentualGlobal == '' ){
            exibirAviso('<strong>Os valores de vigência e o percentual global são obrigatórios!</strong>');
            return false;
        }

        return true;
    }
    <?php endif; ?>

    function setMoneyMask(elem){
        $(elem).inputmask('decimal', {
            integerDigits: 9,
            autoGroup: true,
            digits: 2,
            groupSeparator: '.',
            radixPoint: ',',
            rightAlign: true,
            digitsOptional: false,
            placeholder: '',
            allowMinus: false,
            allowPlus: false
        });
    }

    function setPercentageMask(elem){
        $(elem).inputmask('decimal', {
            integerDigits: 3,
            autoGroup: true,
            digits: 2,
            groupSeparator: '.',
            radixPoint: ',',
            rightAlign: true,
            digitsOptional: false,
            placeholder: '',
            allowMinus: false,
            allowPlus: false
        });

        elem.keyup(function(){
            var valor = new Number(($(this).val()).split(',').join('.'));
            if(valor > 100){
                $(this).val('100,00');
            }
        }).keyup();
    }
</script>
