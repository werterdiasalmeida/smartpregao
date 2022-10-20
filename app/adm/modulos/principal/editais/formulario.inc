<?php
$mEdital = new Edital();

if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'salvar':
            $params = array_merge($_POST, getParam());
            $mEdital->salvarEdital($params);
            $mEdital->commit();

            $db->sucessom('principal/editais/lista');
            break;
        case 'carregarMunicipios':
            montaComboMunicipio($_POST['uf_sigla'], $_POST['municipio_id'], 'S');
            die;
    }
}

$params = getParam();
$participantes = array();
if (isset($params['id']) && !empty($params['id'])) {
    $mEdital->carregarPorId($params['id']);
    $dados = $mEdital->getDados();
    extract($dados);

    if ($municipio_id) {
        $mMunicipio = new Municipio($municipio_id);
        $mUf = new UnidadeFederacao($mMunicipio->uf_id);
        $uf_sigla = $mUf->uf_sigla;
    }
}

include APPRAIZ . "includes/cabecalhom.inc";
?>
<br />
<style type="text/css">
    .documento .form-control.bs-select {
        width: 20% !important;
    }

    .documento .form-control {
        width: 80% !important;
    }

    .documento select.form-control.bs-select {
        display: none;
    }
</style>
<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="icon-list font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Informações do Edital
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="edital-form" method="POST" enctype="multipart/form-data">
            <div class="form-body">
                <div class="row">
                    <div class="row">
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Portal do Pregoeiro</label>
                                <?php
                                $portais = array();
                                $portais[0]['codigo'] = 'ComprasNet';
                                $portais[0]['descricao'] = 'ComprasNet';
                                $portais[1]['codigo'] = 'BB';
                                $portais[1]['descricao'] = 'Banco do Brasil';
                                $portais[2]['codigo'] = 'Outros';
                                $portais[2]['descricao'] = 'Outros';
                                $db->monta_combom("portal", $portais, 'S', 'Selecione...', '', '', '', '', 'S');                                 ?>
                            </div>
                        </div>
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Orgão</label>
                                <?= campo_textom('orgao', 'S', 'S', '', 100, 100, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Edital</label>
                                <?= campo_textom('edital', 'S', 'S', '', 100, 100, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-3">
                            <div class="form-group">
                                <label>Data limite</label>
                                <?= campo_datam('data_limite', 'S', 'S', '', 'S'); ?>
                            </div>
                        </div>
                        <div class="col-md-3">
                            <div class="form-group">
                                <label>Horário do pregão</label>
                                <?= campo_textom('hora_limite', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>

                        <div class="col-md-3">
                            <div class="form-group">
                                <label>UF</label>
                                <?php
                                $mUnidadeFederacao = new UnidadeFederacao();
                                $estados = $mUnidadeFederacao->recuperarTodos('uf_sigla AS codigo, uf_nome_estado AS descricao', array(), 2);
                                $db->monta_combom("uf_sigla", $estados, 'S', 'Selecione...', '', '', '', '', 'S');
                                ?>
                            </div>
                        </div>

                        <div class="col-md-3">
                            <div class="form-group">
                                <label>Município</label>
                                <div id="cidades">
                                    <?php
                                    montaComboMunicipio($uf_sigla, $municipio_id, 'S');
                                    ?>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Objeto</label>
                                <?= campo_textaream('objeto', 'S', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Comentários</label>
                                <?= campo_textaream('comentarios', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Habilitação Técnica</label>
                                <?= campo_textaream('habilitacao_tecnica', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-6">
                            <div class="form-group">
                                <label>Serviço a ser Prestado</label>
                                <?= campo_textaream('resumo_servico', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Valor Estimado (R$)</label>
                                <?= campo_textom('valor_estimado', 'N', 'S', '', 255, 255, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Temperatura</label>
                                <?= campo_textom('temperatura', 'N', 'S', '', 255, 255, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Google Drive</label>
                                <?= campo_textom('link_g_drive', 'N', 'S', '', 255, 255, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
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

<script type="text/javascript">
    $(function() {
        $(".edital-form").validateForm({
            rules: {}
        });

        $('select[name=uf_sigla]').change(function() {
            carregarMunicipio();
        });

        $('.btn-voltar').click(function() {
            window.location = '?modulo=principal/editais/lista&acao=C';
        });

    });

    function carregarMunicipio(municipio_id) {
        var uf_sigla = $('select[name=uf_sigla]').val();

        App.blockUI({
            message: 'Carregando...',
            target: '#cidades'
        });

        $('div#cidades').load(window.location.href, {
            act: 'carregarMunicipios',
            uf_sigla: uf_sigla,
            municipio_id: municipio_id
        }, function() {
            App.unblockUI('#cidades');
        });
    }
</script>