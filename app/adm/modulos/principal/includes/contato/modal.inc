<div class="modal fade in" id="modal-contato" tabindex="-1" role="modal-contato" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <form role="form" name="contato-form" class="contato-form" method="POST">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Contato</h4>
                </div>
                <div class="modal-body">
                    <div class="row">
                        <div class="col-md-5">
                            <div class="form-group">
                                <label>Contato</label>
                                <?= campo_textom('contato', 'S', 'S', '', 90, 90, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-5">
                            <div class="form-group">
                                <label>Tipo</label>
                                <?php
                                $mTipoContato = new TipoContato();
                                $tiposContato = $mTipoContato->recuperarTodos('id AS codigo, descricao AS descricao', array(), 2);
                                $db->monta_combom("tipo_contato_id", $tiposContato, 'S', 'Selecione...', '', '', '', '', 'S');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-2">
                            <div class="form-group">
                                <label>Principal</label>
                                <br />
                                <?php
                                $opcoes = array(
                                    array('codigo' => 't', 'descricao' => 'Sim'),
                                    array('codigo' => 'f', 'descricao' => 'Não'),
                                );

                                echo $db->monta_radiom('flg_principal',$opcoes,'S','S', 'f');
                                ?>
                            </div>
                        </div>
                    </div>
                </div>
                <div class="modal-footer">
                    <button type="submit" class="btn blue">
                        <i class="fa fa-plus-circle"></i>
                        Incluir
                    </button>
                    <button type="button" class="btn" data-dismiss="modal">Cancelar</button>
                </div>

                <input type="hidden" id="index" name="index" />
                <input type="hidden" id="id" name="id" />
            </form>
        </div>
    </div>
</div>

<script type="text/javascript">

    $(function () {

    });

    function limparDadosContato() {
        var form = $('[name=contato-form]');
        $(form).find('[name=contato]').val('');
        $(form).find('[name=index]').val('');
        $(form).find('[name=tipo_contato_id]').val('').selectpicker('refresh');
        $(form).find('#flg_principal_f').prop('checked',true);
    }

    function incluirContato(){
        var form = $('[name=contato-form]');

        //Se for principal procura todos os outros que forem principais e altera para não
        if($(form).find('[name=flg_principal]:checked').val() == 't'){
            var dados = $('#tabela-contatos').bootstrapTable("getData");
            $.each(dados, function(index,item){
                if(item.flg_principal == 't') {
                    item.flg_principal = 'f';
                    $('#tabela-contatos').bootstrapTable('updateRow', {
                        index: index,
                        row: item
                    });
                }
            });
        }

        var contato = {
            contato : $(form).find('[name=contato]').val(),
            flg_principal : $(form).find('[name=flg_principal]:checked').val(),
            tipo_contato_id : $(form).find('[name=tipo_contato_id]').val(),
            tipo_contato : $(form).find('[name=tipo_contato_id]').find('option:selected').html(),
            id : $(form).find('[name=id]').val()
        };

        adicionarItemTabela(contato, $('#tabela-contatos'), $(form).find('#index').val());
        $('#modal-contato').modal('hide');
        limparDadosContato();
    }

    function alterarContato(index){
        $('#modal-contato').modal('show');

        var dados = $('#tabela-contatos').bootstrapTable("getData");
        var contato = dados[index];
        var form = $('[name=contato-form]');

        $(form).find('[name=contato]').val(contato.contato);
        $(form).find('[name=tipo_contato_id]').val(contato.tipo_contato_id).selectpicker('refresh');
        $(form).find('[name=index]').val(index);

        if(contato.flg_principal == 'f'){
            $(form).find('#flg_principal_f').prop('checked',true);
        }else{
            $(form).find('#flg_principal_t').prop('checked',true);
        }

        $(form).find('[name=id]').val(contato.id);
    }

</script>