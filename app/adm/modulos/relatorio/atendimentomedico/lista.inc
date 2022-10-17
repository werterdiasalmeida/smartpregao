<?php
require_once 'dados_relatorio.inc';

if (isset($_REQUEST['ajax']) && $_REQUEST['titulo']) {
    $titulo = html_entity_decode(htmlentities($_REQUEST['titulo'], null, 'utf-8'));
    switch ($_REQUEST['ajax']) {
        case 'carregaRelatorio':
            $result = $mRelatorio->carregarParametrosBanco($titulo);
            echo json_encode(utf8ArrayEncode($result));
        break;
    }
die();
}

// exibe consulta
if (isset($_REQUEST['act'])) {
    $_REQUEST['agrupador'][] = 'codigo_agenda';
    switch ($_REQUEST['act']) {
        case 'relatorio':
            include "os_resultado.inc";
            exit;
            break;
        case 'relatorioxls':
            include "osxls_resultado.inc";
            exit;
            break;
        case 'grafico':
            include "grafico.inc";
            exit;
            break;
        case 'salvar':
            $mRelatorio->gravarParametrosBanco($_REQUEST['titulo']);
            $db->sucessom($_REQUEST['modulo']);
            break;
        case 'excluir':
            $mRelatorio->excluirParametroBanco($_REQUEST['titulo']);
            $db->sucessom($_REQUEST['modulo']);
            break;
    }

}
$mEstabelecimento = new Estabelecimento();
$mEstabelecimentoEspecialidadeProfissional = new EstabelecimentoEspecialidadeProfissional();
include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('RELATÓRIO DE ATENDIMENTOS');

?>

<script language="JavaScript" src="../includes/funcoes.js"></script>

<script type="text/javascript">
    function exibeRelatorioXLS() {
        var formulario = document.formulario;
        // Tipo de acao
        formulario.act.value = 'relatorioxls';
        if(!isValidCampos()){
            return false;

        }
        formulario.target = '';
        formulario.submit();
    }

    function limparRelatorio() {
        var formulario = document.formulario;
        // Tipo de acao
        formulario.act.value = '';
        $('#titulo').selectize()[0].selectize.clear(true);
        $('#agrupador').selectize()[0].selectize.clear(true);
        $('#coluna').selectize()[0].selectize.clear(true);
        $('#ordenarcampo').selectize()[0].selectize.clear(true);
        $('#estabelecimento_id').selectize()[0].selectize.clear(true);
        $('#tipoordenar').val('ASC').change();
        $('#data_de').val('');
        $('#data_ate').val('');
    }

    function exibeRelatorio() {
        var formulario = document.formulario;
        // Tipo de acao
        formulario.act.value = 'relatorio';
        if(!isValidCampos()){
            return false;
        }

        formulario.target = 'resultadoGeral';

        var janela = window.open('', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        janela.focus();

        formulario.submit();

    }

    function exibeGrafico() {
        var formulario = document.formulario;
        // Tipo de acao
        formulario.act.value = 'grafico';
        if(!isValidCampos()){
            return false;
        }

        formulario.target = 'resultadoGeral';

        var janela = window.open('', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        janela.focus();

        formulario.submit();

    }

    function exibirModalExclusaoRelatorio(nome){
        var modal = $('#modal-excluir');
        modal.find('strong#nome').html(nome);
        modal.modal('show');
    }

    function excluirRelatorio(){
        var formulario = document.formulario;
        formulario.act.value ='excluir';
        formulario.target = '';
        formulario.submit();
    }

    function isValidCampos(){
        var agrupador = document.getElementById('agrupador');
        var coluna = document.getElementById('coluna');

        if (!agrupador.options.length) {
            alert('Favor selecionar ao menos um item para agrupar o resultado!');
            return false;
        }

        if (!coluna.options.length) {
            alert('Favor selecionar ao menos uma coluna!');
            return false;
        }

        return true;
    }

    function salvarRelatorio(){
        var formulario = document.formulario;
        // Tipo de acao
        formulario.act.value='salvar';
        formulario.target = '';
        if($('#titulo').val() == ""){
            alert('Favor criar um título para o relatório!');
            return false;
        }

        if(!isValidCampos()){
            return false;
        }

        formulario.submit();
    }

    var escape_html = function(str) {
        return (str + '')
            .replace(/&/g, '&amp;')
            .replace(/</g, '&lt;')
            .replace(/>/g, '&gt;')
            .replace(/"/g, '&quot;');
    };

    Selectize.define('delete_button_single', function(options) {
        options = $.extend({
            label     : '&nbsp<i class="fa fa-ban"></i>',
            title     : 'Excluir',
            className : 'delete',
            append    : true
        }, options);

        var self = this;
        var html = '<a style="color: red" href="javascript:" class="' + options.className + '" tabindex="-1" title="' + escape_html(options.title) + '">' + options.label + '</a>';

        /**
         * Appends an element as a child (with raw HTML).
         *
         * @param {string} html_container
         * @param {string} html_element
         * @return {string}
         */
        var append = function(html_container, html_element) {
            var pos = html_container.search(/(<\/[^>]+>\s*)$/);
            return html_container.substring(0, pos) + html_element + html_container.substring(pos);
        };

        this.setup = (function() {
            var original = self.setup;
            return function() {
                // override the item rendering method to add the button to each
                if (options.append) {
                    var render_item = self.settings.render.item;
                    self.settings.render.item = function(data) {
                        return append(render_item.apply(this, arguments), html);
                    };
                }

                original.apply(this, arguments);

                // add event listener
                this.$control.on('click', '.' + options.className, function(e) {
                    e.preventDefault();
                    if (self.isLocked) return;

                    self.close();
                    exibirModalExclusaoRelatorio(self.getValue());

                });

            };
        })();

    });
</script>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-user-follow font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Relatório de Atendimentos
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" id="filtro" class="profissional-form dont-check-changes" method="POST" enctype="multipart/form-data">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-6">

                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="control-label">Título do Relatório: </label>
                                    <select id="titulo" name="titulo"></select>
                                </div>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="control-label">Agrupadores: </label>
                                    <select id="agrupador" name="agrupador[]" multiple required></select>
                                    <?php
                                    if(is_array($_REQUEST['agrupador'])):
                                        foreach ($_REQUEST['agrupador'] as $agrupador): ?>
                                            <option value="<?= $agrupador ?>" <?= (in_array($agrupador, $arrAgrupadores) ? 'selected' : '') ?>>
                                                <?= $arrLabels[$agrupador] ?>
                                            </option>
                                    <?php endforeach;
                                    endif;
                                    ?>
                                </div>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="control-label">Colunas: </label>
                                    <select id="coluna" name="coluna[]" multiple required></select>
                                </div>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="control-label">Ordenar por: </label>
                                    <select id="ordenarcampo" name="ordenarcampo[]" multiple></select>
                                </div>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="control-label">Tipo de ordenação: </label>
                                    <select name="tipoordenar" id="tipoordenar" class="form-control bs-select" style="" tabindex="-98" aria-required="true">
                                        <option value='ASC'>Crescente</option>
                                        <option value='DESC'>Decrescente</option>
                                    </select>
                                </div>
                            </div>
                        </div>

                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="control-label">Expandir: </label>
                                    <?php
                                    $expandir = $expandir ?: '1';
                                    ?>
                                    <?= campo_radiom('expandir', array("Sim" => array("valor" => "1"), "Não" => array("valor" => "0")), 'N'); ?>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div class="col-md-6">
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="control-label">Clínica: </label>
                                    <select id="estabelecimento_id" name="estabelecimento_id[]" multiple></select>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label class="control-label">Data da consulta: </label>
                                    <div class="input-group">
                                            <span class="input-group-addon">
                                                            De
                                                    </span>
                                            <?= campo_datam('data_de', 'N', 'S', 'De:', 'N')?>

                                            <span class="input-group-addon">
                                                            Até
                                                        </span>
                                            <?= campo_datam('data_ate', 'N', 'S', 'Até:', 'N')?>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="row">
                            <div class="col-md-12">
                                <div class="form-group">
                                    <label>Profissional</label>
                                    <select id="profissional_id" name="profissional_id[]" multiple></select>
                                </div>
                            </div>
                        </div>
                    </div>

                    <input type="hidden" name="form" value="1"/>
                    <input type="hidden" name="act" value=""/>
                </div>
            </div>
            <div class="row form-actions">
                <button type="button" class="btn blue" onclick="exibeRelatorio();">
                    <i class="icon-magnifier"></i> Visualizar
                </button>
                <button type="button" class="btn green-jungle" onclick="exibeRelatorioXLS();">
                    <i class="fa fa-file-excel-o"></i> Visualizar XLS
                </button>
<!--                <button type="button" class="btn green-seagreen" onclick="exibeGrafico();">-->
<!--                    <i class="fa fa-bar-chart"></i> Gerar Gráfico-->
<!--                </button>-->
                <button type="button" class="btn default" onclick="limparRelatorio();">
                    <i class="fa fa-eraser"></i> Limpar
                </button>
                <button type="button" class="btn green" onclick="salvarRelatorio();">
                    <i class="fa fa-floppy-o"></i> Salvar
                </button>
            </div>
        </form>
    </div>
</div>

<div class="modal fade in" id="modal-excluir" tabindex="-1" role="modal-excluir" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                <h4 class="modal-title">Excluir relatório</h4>
            </div>
            <div class="modal-body">
                Você tem certeza que deseja excluir o relatório <strong id="nome"></strong>?
            </div>
            <div class="modal-footer">
                <button type="button" class="btn red" onclick="excluirRelatorio();">Sim</button>
                <button type="button" class="btn blue" data-dismiss="modal">Não</button>
            </div>
        </div>
    </div>
</div>

<script>

    function montaSelectize(campo, placeholder, objOptions){
        $('#' + campo).selectize({
            persist: false,
            maxItems: null,
            placeholder: placeholder,
            plugins: ['drag_drop','remove_button'],
            valueField: 'codigo',
            labelField: 'descricao',
            searchField: ['descricao'],
            sortField: [
                {field: 'descricao', direction: 'asc'}
            ],
            options: objOptions,
            render: {
                item: function (item, escape) {
                    var name = escape(item.descricao);
                    return '<div>' +
                        (name ? '<span class="name">' + name + '</span>' : '') +
                        '</div>';
                },
                option: function (item, escape) {
                    var name = escape(item.descricao);

                    return '<div>' +
                        '<span style="vertical-align: sub;">' + name + '</span>' +
                        '</div>';
                }
            },
        });
    }

    montaSelectize('coluna', 'Colunas', <?= $mRelatorio->arrayToSeletize($arrColunas)?>);
    montaSelectize('agrupador', 'Agrupadores', <?= $mRelatorio->arrayToSeletize($arrAgrupadores)?>);
    montaSelectize('ordenarcampo', 'Ordenação', <?= $mRelatorio->arrayToSeletize($arrColunas)?>);
    montaSelectize('estabelecimento_id', 'Todas', <?= json_encode(utf8ArrayEncode($mEstabelecimento->getComboPorResponsabilidade())); ?>);
    montaSelectize('profissional_id', 'Todas', <?= json_encode(utf8ArrayEncode($mEstabelecimentoEspecialidadeProfissional->getComboProfissionaisPorEstabelecimento())); ?>);

    $('#titulo').selectize({
        persist: false,
        placeholder: 'Título',
        valueField: 'codigo',
        plugins: ['delete_button_single'],
        labelField: 'descricao',
        searchField: ['descricao'],
        sortField: [
            {field: 'descricao', direction: 'asc'}
        ],
        options: <?= $mRelatorio->montaListaParametrosBanco()?>,
        create: {
            function(){
                return { value:123, text:input};
            }
        },
        onItemAdd : function(value, item){


            $.ajax({
                url: window.location.href,
                type: 'POST',
                data : {
                    ajax : 'carregaRelatorio',
                    titulo : value
                },
                dataType : 'json',
                success: function(obj) {
                    if(obj){
                        var campoAgrupador = $('#agrupador').selectize()[0].selectize;
                        var campoColuna = $('#coluna').selectize()[0].selectize;
                        var campoOrdem = $('#ordenarcampo').selectize()[0].selectize;
                        var estabelecimento = $('#estabelecimento_id').selectize()[0].selectize;
                        var profissionais = $('#profissional_id').selectize()[0].selectize;
                        var tipoordenar = $('#tipoordenar');

                        campoAgrupador.clear(true);
                        campoColuna.clear(true);
                        campoOrdem.clear(true);
                        estabelecimento.clear(true);

                        $.each(obj.agrupador, function(k, v){
                            campoAgrupador.addItem(v, true);
                        });

                        $.each(obj.coluna, function(k, v){
                            campoColuna.addItem(v, true);
                        });

                        $.each(obj.ordenarcampo, function(k, v){
                            campoOrdem.addItem(v, true);
                        });

                        if(obj.tipoordenar){
                            tipoordenar.val(obj.tipoordenar).change();
                        }else{
                            tipoordenar.val('ASC').change();
                        }

                        if(obj.estabelecimento_id){
                            $.each(obj.estabelecimento_id, function(k, v){
                                estabelecimento.addItem(v, true);
                            });
                        }

                        if(obj.profissional_id){
                            $.each(obj.profissional_id, function(k, v){
                                profissionais.addItem(v, true);
                            });
                        }

                        $('#data_de').val(obj.data_de);
                        $('#data_ate').val(obj.data_ate);

                        $('#radio_opcao_expandir_' + obj.expandir).prop('checked', true);
                    }

                }
            });
        }
    });

    $(function () {
        $('#filtro').validate();
    });


</script>