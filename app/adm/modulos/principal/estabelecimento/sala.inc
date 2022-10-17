<?php
$p = getParam();
UsuarioResponsabilidadeEstabelecimento::isPossuiResponsabilidadeEstabelecimentoFuncionalidade($p['id']);
$mSala = new SalaEstabelecimento();

if ($_REQUEST && isset($_REQUEST['act'])) {
    switch ($_REQUEST['act']) {
        case 'salvar' :
            $params = array_merge($_POST, getParam());
            $params['sala'] = $params['sala'] ? $params['sala'] : array();

            $salas = array();
            foreach($params['sala'] as $idSala => $sala){
                $idSala = (strpos($idSala, 'id_') === false ? null : str_replace('id_', '', $idSala));

                $salas[] = array(
                    'id' => $idSala,
                    'sala' => $sala
                );
            }

            $mSala->salvarSalas(array('sala' => $salas), $params['id']);
            $mSala->commit();

            $db->sucessom($_REQUEST['modulo'], setParam(array('id' => $p['id'])));
            break;
    }
}

$mSala = new SalaEstabelecimento();
$salas = $mSala->listarPorEstabelecimento($p['id'], true);

include APPRAIZ . "includes/cabecalhom.inc";
echo Estabelecimento::gerarCabecalho();
UsuarioResponsabilidadeEstabelecimento::montaTituloAba($abacod_tela, $url, setParam(getParam()), null, array(), true);
?>

<div class="portlet light bordered">
    <div class="portlet-title">
        <div class="caption font-blue-sharp">
            <i class="fa fa-building font-blue-sharp"></i>
            <span class="caption-subject bold uppercase">
                Salas do estabelecimento
            </span>
        </div>
    </div>
    <div class="portlet-body form">
        <form role="form" name="formulario" class="sala-form" method="POST" enctype="multipart/form-data">
            <div class="form-body">
                <div class="row">
                    <div class="col-md-12">
                        <a class="btn green" onclick="incluirSala();">
                            <i class="fa fa-plus-circle"></i>
                            Incluir sala
                        </a>
                        <br />
                        <br />
                        <table id="tabela-salas" data-toggle="table" class="table table-striped" data-unique-id="id">
                            <thead>
                            <tr>
                                <th data-field="sala" data-formatter="formatCampoSala">Sala</th>
                                <th data-formatter="formatAcaoSala" data-align="center" data-width="10%">Ação</th>
                            </tr>
                            </thead>
                            <tbody>

                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
            <div class="form-actions">
                <button type="submit" class="btn blue">
                    <i class="fa fa-save"></i> Salvar
                </button>
            </div>

            <input type="hidden" name="act" value="salvar"/>
        </form>
    </div>

<script type="text/javascript">
    $(function () {
        $(".sala-form").validateForm();

        <?php if($salas) : ?>
        $('#tabela-salas').bootstrapTable('load', <?= $salas ?>);
        <?php endif; ?>
    });

    function incluirSala(){
        var tabelaSalas = $('#tabela-salas');
        adicionarItemTabela({sala : '', id : ''}, tabelaSalas);
        tabelaSalas.find('[name^=sala]:last').focus();
    }

    function formatAcaoSala(value, row, index) {
        return '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirSala(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';
    }

    function excluirSala(index){
        removerItemTabela(index, $('#tabela-salas'));
    }

    function formatCampoSala(value, row, index){
        var indice = row.id ? 'id_' + row.id : '';
        return '<input type="text" name="sala[' + indice + ']" '
            + ' value="' + value + '" class="normal form-control sala" placeholder="Descreva o número ou alguma referência da sala" onchange="atualizarCampoSala(' + index + ', this);" />';
    }

    function atualizarCampoSala(index, elem){
        var table = $('#tabela-salas');
        var dados = table.bootstrapTable('getData');
        var linha = dados[index];
        linha.sala = $(elem).val();

        adicionarItemTabela(linha, table, index)
    }
</script>