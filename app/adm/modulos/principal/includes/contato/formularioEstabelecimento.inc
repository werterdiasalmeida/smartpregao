<div class="row">
    <div class="col-md-6">
        <div class="form-group">
            <label>Telefone</label>
            <div class="input-group">
                <span class="input-group-addon">
                    <i class="fa fa-phone-square"></i>
                </span>
                <?= campo_textom("contatos[".TipoContato::TIPO_TELEFONE_FIXO."][contato]", 'S', 'S', '', 19, 19, '', '', '', '', 0, null, null, $contato_telefone, null, null, 'contato_telefone'); ?>
                <input type="hidden" name="contatos[<?= TipoContato::TIPO_TELEFONE_FIXO ?>][tipo_contato_sigla]" value="<?= TipoContato::TIPO_TELEFONE_FIXO ?>" />
            </div>
        </div>
    </div>
    <div class="col-md-6">
        <div class="form-group">
            <label>Email</label>
            <div class="input-group">
                <span class="input-group-addon">
                    <i class="fa fa-envelope"></i>
                </span>
                <?= campo_textom("contatos[".TipoContato::TIPO_EMAIL."][contato]", 'S', 'S', '', 70, 70, '', '', '', '', 0, null, null, $contato_email, null, null, 'contato_email'); ?>
                <input type="hidden" name="contatos[<?= TipoContato::TIPO_EMAIL ?>][tipo_contato_sigla]" value="<?= TipoContato::TIPO_EMAIL ?>" />
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(function(){
        $("#contato_celular, #contato_telefone").inputmask({
            mask: ["(99) 9999-9999", "(99) 99999-9999"],
        });
    });
</script>