<div class="row">
    <div class="col-md-4">
        <div class="form-group">
            <label>Telefone</label>
            <div class="input-group">
                <span class="input-group-addon">
                    <i class="fa fa-phone-square"></i>
                </span>
                <?= campo_textom("telefone_principal", 'N', 'S', '', 19, 19, '', '', '', '', 0, null, null, $telefone_principal, null, null, 'telefone_principal'); ?>
            </div>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label>Email</label>
            <div class="input-group">
                <span class="input-group-addon">
                    <i class="fa fa-envelope"></i>
                </span>
                <?= campo_textom("email_principal", 'N', 'S', '', 70, 70, '', '', '', '', 0, null, null, $email_principal, null, null, 'email_principal'); ?>
            </div>
        </div>
    </div>
</div>
<script type="text/javascript">
    $(function(){
        $("#telefone_principal").inputmask({
            mask: ["(99) 9999-9999"],
        });
    });
</script>