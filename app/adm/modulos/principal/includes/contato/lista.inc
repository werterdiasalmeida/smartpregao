<div class="row">
    <div class="col-md-12">
        <a class="btn green btn-incluir-contato">
            <i class="fa fa-plus-circle"></i>
            Incluir contato
        </a>
        <br />
        <br />
        <table id="tabela-contatos" data-toggle="table" class="table table-striped" data-unique-id="id">
            <thead>
            <tr>
                <th data-field="tipo_contato">Tipo de contato</th>
                <th data-field="contato">Contato</th>
                <th data-formatter="formatContatoPrincipal" data-field="flg_principal">Principal</th>
                <th data-formatter="formatAcaoContato" data-align="center">Ação</th>
            </tr>
            </thead>
            <tbody>
            </tbody>
        </table>
    </div>
</div>
<input type="hidden" name="contatos" value="<?= $contatos ?>"/>

<script type="text/javascript">

    $(function () {
        <?php if($contatos) : ?>
        $('#tabela-contatos').bootstrapTable('load', <?= $contatos ?>);
        <?php endif; ?>

        $('.btn-incluir-contato').click(function(){
            $('#modal-contato').modal({
                backdrop: 'static',
            }).modal('show');
        });
    });

    function formatAcaoContato(value, row, index) {
        var btnEditar = '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarContato(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>';

        var btnExcluir = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirContato(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';

        return btnEditar + btnExcluir;
    }

    function formatContatoPrincipal(value, row, index) {
        var retorno = 'Sim';
        if(row.flg_principal == 'f'){
            retorno = 'Não';
        }
        return retorno;
    }

    function excluirContato(index){
        removerItemTabela(index, $('#tabela-contatos'));
    }
</script>