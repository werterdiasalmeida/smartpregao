<div class="row">
    <div class="col-md-12">
        <a class="btn green btn-incluir-endereco">
            <i class="fa fa-plus-circle"></i>
            Incluir endereço
        </a>
        <br />
        <br />
        <table id="tabela-enderecos" data-toggle="table" class="table table-striped" data-unique-id="id">
            <thead>
            <tr>
                <th data-field="cep">CEP</th>
                <th data-field="logradouro">Logradouro</th>
                <th data-field="complemento">Número/Complemento</th>
                <th data-field="bairro">Bairro</th>
                <th data-field="estado_cidade">Cidade/Estado</th>
                <th data-formatter="formatAcaoEndereco" data-align="center">Ação</th>
            </tr>
            </thead>
            <tbody>

            </tbody>
        </table>
    </div>
</div>
<input type="hidden" name="enderecos" value="<?= $enderecos ?>"/>

<script type="text/javascript">

    $(function () {
        <?php if($enderecos) : ?>
        $('#tabela-enderecos').bootstrapTable('load', <?= $enderecos ?>);
        <?php endif; ?>

        $('.btn-incluir-endereco').click(function(){
            $('#modal-endereco').modal({
                backdrop: 'static',
            }).modal('show');
        });
    });

    function formatAcaoEndereco(value, row, index) {
        var btnEditar = '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarEndereco(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>';

        var btnExcluir = '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirEndereco(' + index + ')" title="Excluir" style="margin: 2px;">'
            + ' <i class="bi bi-x-square"></i>'
            + '</a>';

        return btnEditar + btnExcluir;
    }

    function excluirEndereco(index){
        removerItemTabela(index, $('#tabela-enderecos'));
    }
</script>