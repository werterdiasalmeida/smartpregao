<style type="text/css">

    body {
        padding-top: 40px;
        padding-bottom: 40px;
        background-color: #19769F!important;
    }

    .form-signin {
        max-width: 500px;
        padding: 15px;
        margin: 0 auto;
    }

    .form-signin .form-signin-heading, .form-signin .checkbox {
        margin-bottom: 10px;
    }

    .form-signin .checkbox {
        font-weight: 400;
    }

    .form-signin .form-control {
        position: relative;
        -webkit-box-sizing: border-box;
        -moz-box-sizing: border-box;
        box-sizing: border-box;
        height: auto;
        padding: 10px;
        font-size: 16px;
    }

    .form-signin .form-control:focus {
        z-index: 2;
    }

    .form-signin input[type="email"] {
        margin-bottom: -1px;
        border-bottom-right-radius: 0;
        border-bottom-left-radius: 0;
    }

    .form-signin input[type="password"] {
        margin-bottom: 10px;
        border-top-left-radius: 0;
        border-top-right-radius: 0;
    }

    .error.form-control {
        border-color: #e73d4a;
    }

    label.error {
        color: #e73d4a;
    }

    .logo {
        margin-top: 150px;
        margin-bottom: 50px;
        text-align: center;
    }

    .painel {
        background-color: #ffffff;
        padding: 35px;
    }

    .titulo {
        text-align: center;
        font-size: 28px;
        font-weight: 400!important;
    }
</style>

<div class="container">
    <form class="form-signin" method="POST">
        <input type="hidden" name="act" value="enviar-codigo">
        <div class="logo">
            <img src="/imagens/logo2.png?v=1.1.0" style="height: 65px;" alt="">
        </div>

        <div class="painel">
            <?php if(isset($_SESSION['erro_painel_senha']) && !empty($_SESSION['erro_painel_senha'])): ?>
                <div class="alert alert-danger alert-dismissible" role="alert">
                    <button type="button" class="close" data-dismiss="alert" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <?= $_SESSION['erro_painel_senha'] ?>
                </div>
            <?php
                unset($_SESSION['codigo_painel_senhas']);
                unset($_SESSION['erro_painel_senha']);
            endif;
            ?>

            <h3 class="form-title font-blue titulo">Informe o código para proseguir</h3>
            <label for="inputCodigo" style="margin-top: 15px"  class="sr-only">Código</label>
            <input type="text" name="codigo" id="inputCodigo" class="form-control" style="margin-top: 15px" placeholder="Código do painel" required="" autofocus="">
            <button class="btn blue uppercase btn btn-lg btn-block" style="margin-top: 15px" type="submit">Acessar</button>
        </div>
    </form>
</div>

<script type="text/javascript">
  $(function () {
    $("form").validate();
  });

</script>