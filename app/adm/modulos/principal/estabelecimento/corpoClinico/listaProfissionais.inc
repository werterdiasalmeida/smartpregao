<?php
$mProfissional = new Profissional();
$arWhere = array();

if(!empty($_REQUEST['pesquisa'])){
    $pesquisa = trim(addslashes($_REQUEST['pesquisa']));
    $arWhere['pesquisa'] = "(
        pes.nome_completo ILIKE '%{$pesquisa}%'
        OR
        p.registro_profissional || '/' || p.uf_registro ILIKE '%{$pesquisa}%' 
        OR
        tp.descricao ILIKE '%{$pesquisa}%'
    ) ";
}

if(!empty($_REQUEST['nome'])){
    $nome = trim(addslashes($_REQUEST['nome']));
    $arWhere['nome'] = "pes.nome_completo ILIKE '%{$nome}%' ";
}

if(!empty($_REQUEST['registro'])){
    $registro = trim(addslashes($_REQUEST['registro']));
    $arWhere['registro_profissional'] = "p.registro_profissional || '/' || p.uf_registro ILIKE '%{$registro}%' ";
}

if(!empty($_REQUEST['tipo_profissional_id'])){
    $tipo_profissional_id = trim(addslashes($_REQUEST['tipo_profissional_id']));
    $arWhere['tipo_profissional_id'] = "p.tipo_profissional_id = '{$tipo_profissional_id}'" ;
}

$profissionais = $mProfissional->getListPorTipo($arWhere, false, null, true, false);
?>
<!--data-click-to-select="true"-->
<table id="tabela-profissionais" data-toggle="table" class="table table-striped"  data-unique-id="id"
       data-single-select="true" data-pagination="true" data-height="300">
    <thead>
    <tr>
        <th data-field="selecionado" data-radio="true"></th>
        <th data-field="nome" data-width="40%">Nome</th>
        <th data-field="tipo" data-width="30%">Tipo</th>
        <th data-field="registro" data-width="30%">Registro</th>
    </tr>
    </thead>
    <tbody>
    </tbody>
</table>

<script type="text/javascript">
    $(function () {
        var data = <?= $profissionais ?>;
        var carregarEspecialidades = false;
        if(data.length === 1){
            data[0].selecionado = true;
            carregarEspecialidades = true;
        }

        $('#tabela-profissionais').bootstrapTable({
            data: data
        }).on('check.bs.table', function (e, row) {
            listarEspecialidades(row)
        });

        $( "#profissionais" ).closest('.portlet.light.bordered').animate({backgroundColor: "#FFFFCC" }, 1000).animate({backgroundColor: "#f1f4f9" }, 1000);

        if(carregarEspecialidades){
            listarEspecialidades(data[0]);
        }
    });

    function listarEspecialidades(row){
        App.blockUI({
            message: 'Carregando...',
            target: '#especialidades'
        });

        $('#especialidades').load(window.location.href, {
            act : 'pesquisarEspecialidades',
            profissional_id : row.id,
        }, function(){
            App.unblockUI('#especialidades');
        });
    }
</script>
