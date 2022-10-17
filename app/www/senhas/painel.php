<style type="text/css">
    body {
        font-size: 100%;
        background-color: #292929;
        color: #e7e7e7;
        padding-top: 10px;
    }

    .alerta-numero {
        height: 100%;
        display: none;
        background-color: #292929;
        position: absolute;
        top: 0;
        height: 100%;
        width: 100%;
        z-index: 30;
    }

    .alerta-numero .div-titulo-atual{
        text-align: center;
        font-weight: bolder;
        color: #bcbc09;
    }

    .alerta-numero .div-senha-atual, .alerta-numero .div-guiche-atual{
        text-align: center;
        font-weight: bolder;
    }

    .box{
        height: 100%;
        border: 1px solid white;
        padding: 10px;
        -webkit-border-radius: 15px;
        -moz-border-radius: 15px;
        border-radius: 15px !important;
        background-color:rgba(0, 0, 0, 0.1);
    }

    .painel {
        position: absolute;
        top: 0;
        height: 100%;
        width: 100%;
        z-index: 20;
    }

    .painel-esquerda {
        height: 80% !important;
    }

    .painel-direita {
        height: 80% !important;
    }

    .painel-rodape {
        height: 20% !important;
        padding-bottom: 10px;
    }

    .registro-atual {
        height: 100% !important;
        padding: 15px 0px 0px 15px;
    }

    .div-titulo-atual {
        text-align: center;
        font-weight: bolder;
        color: #bcbc09;
    }

    .registro-atual .div-senha-atual, .registro-atual .div-guiche-atual{
        text-align: center;
        font-weight: bolder;

    }

    .historico{
        height: 21%;
        width: 100%;
    }

    .historico-registros {
        height: 80% !important;
        padding: 15px;
        font-weight: bolder;
    }

    .relogio {
        height: 20%;
        text-align: center;
        margin: auto;
        font-weight: bolder;
    }

    .rss {
        height: 100% !important;
        width: 100%;
        padding: 15px;
    }

    .rss .span-texto-rss {
        margin-right: 1000px
    }

    .span-sair {
        position: absolute;
        bottom: 5px;
        right: 5px;
        font-weight: bolder;
        cursor: pointer;
    }
</style>

<audio id="audio-chamada">
    <source src="../audio/chamada.wav" type="audio/mpeg">
</audio>

<div class="container-fluid alerta-numero">
    <div style="width: 100%; height: 100%;">
        <div style="height: 50%">
            <div class="fit-text" style=" height: 30%">
                <div class="div-titulo-atual"><?= ($dadosPainel['tipo'] == 'A' ? 'Nome' : 'Senha') ?></div>
            </div>
            <div class="fit-text" style=" height: 70%">
                <div class="div-senha-atual" >--</div>
            </div>
        </div>
        <div style="height: 50%">
            <div class="fit-text" style=" height: 30%">
                <div class="div-titulo-atual"><?= ($dadosPainel['tipo'] == 'A' ? 'Sala' : 'Guichê') ?></div>
            </div>
            <div class="fit-text" style=" height: 70%">
                <div class="div-guiche-atual" >--</div>
            </div>
        </div>
    </div>
</div>

<div class="container-fluid painel">
    <div class="row">
        <div class="col-xs-6 painel-esquerda">
            <div class="row">
                <div class="col-xs-12 registro-atual">
                    <div class="box">
                        <div style="width: 100%; height: 100%;">
                            <div style="height: 50%; width: 100%">
                                <div class="fit-text" style=" height: 30%">
                                    <div class="div-titulo-atual"><?= ($dadosPainel['tipo'] == 'A' ? 'Nome' : 'Senha') ?></div>
                                </div>
                                <div class="fit-text" style=" height: 70%">
                                    <div class="div-senha-atual" >--</div>
                                </div>
                            </div>
                            <div style="height: 50%">
                                <div class="fit-text" style=" height: 30%">
                                    <div class="div-titulo-atual"><?= ($dadosPainel['tipo'] == 'A' ? 'Sala' : 'Guichê') ?></div>
                                </div>
                                <div class="fit-text" style=" height: 70%">
                                    <div class="div-guiche-atual" >--</div>
                                </div>
                            </div>
                        </div>

                    </div>
                </div>
            </div>
        </div>
        <div class="col-xs-6 painel-direita">
            <div class="row">
                <div class="col-xs-12 historico-registros">
                    <div class="box">
                        <div style="text-align: center;">
                            <div class="titulo-historico" style="height: 16%; width: 100%;">
                                <div style="width: 60%; height: 100%; float: left; padding: 10px;">
                                    <div class="fit-text" style="height: 100%">
                                        <div class="div-titulo-atual"><?= ($dadosPainel['tipo'] == 'A' ? 'Nome' : 'Senha') ?></div>
                                    </div>
                                </div>
                                <div style="width: 20%; height: 100%; float: left; padding: 10px;">
                                    <div class="fit-text" style="height: 100%">
                                        <div class="div-titulo-atual"><?= ($dadosPainel['tipo'] == 'A' ? 'Sala' : 'Guichê') ?></div>
                                    </div>
                                </div>
                                <div style="width: 20%; height: 100%; float: left; padding: 10px;">
                                    <div class="fit-text" style="height: 100%">
                                        <div class="div-titulo-atual">Hora</div>
                                    </div>
                                </div>
                            </div>

                            <? for($i = 1; $i < 5; $i++): ?>
                                <div class="historico" style="width: 100%">
                                    <div style="width: 60%; height: 100%; float: left; padding: 10px;">
                                        <div class="fit-text" style="height: 100%;">
                                            <div class="historico-senha">---</div>
                                        </div>
                                    </div>
                                    <div style="width: 20%; height: 100%; float: left; padding: 10px;">
                                        <div class="fit-text" style="height: 100%;">
                                            <div class="historico-guiche">---</div>
                                        </div>
                                    </div>
                                    <div style="width: 20%; height: 100%; float: left; padding: 10px;">
                                        <div class="fit-text" style="height: 100%;">
                                            <div class="historico-hora">---</div>
                                        </div>
                                    </div>
                                </div>
                            <? endfor; ?>
                        </div>
                    </div>
                </div>
                <div class="col-xs-12 relogio">
                    <div class="box fit-text">
                        <div></div>
                    </div>
                </div>
            </div>
        </div>
        <div class="col-xs-12 painel-rodape">
            <div class="row">
                <div class="col-xs-12 rss">
                    <div class="box fit-text" style="padding: 0px !important;">
                        <div>
                            <marquee class="html-marquee" direction="left" behavior="scroll" scrollamount="20" >
                                <span class="span-texto-rss">Em atendimento:  <span id="em-atendimento">0</span></span>
                                <span class="span-texto-rss">Aguardando: <span id="aguardando">0</span></span>
                                <span>Atendidos: <span id="atendidos">0</span></span>
                            </marquee>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <spam class="span-sair" title="Sair do painel"><span class="icon-logout"></span> Sair</spam>
    </div>
    <form id="form-logoff" method="POST" style="display: none;">
        <input type="hidden" name="logoff" value="1">
        <input type="hidden" name="msg" value="">
    </form>
</div>

<script type="text/javascript">
  var jsonRegistros = [];

  $(function () {
    iniciarRelogio();
    atualizarRss(<?= json_encode($dadosPainel['atendimento'] ?: []); ?>, true);
    atualizarTela(<?= json_encode(utf8ArrayEncode($dadosPainel['registros'] ?: [])); ?>, true);
    atualizarPainel();
    atualizarTextFit();

    $('.span-sair').click(function(){
      $('#form-logoff').submit();
    });
  });

  function iniciarRelogio() {
    setHora(new Date());

    function setHora(data) {
      function formataHora(num) {
        return num.toString().padStart(2, '0');
      }

      $('.relogio .box div').text(formataHora(data.getHours()) + ':' + formataHora(data.getMinutes()) + ':' + formataHora(data.getSeconds()));
    }

    setInterval(function () {
      setHora(new Date());
    }, 1000);
  }

  function atualizarPainel() {
    function atualizar() {
      $.ajax({
        type: "POST",
        url: window.location.href,
        dataType: 'json',
        data: {
          act: 'atualizar',
          codigo: $('[name=hdn-codigo]').val()
        },
        success: function (retorno) {
          if (retorno.params === false) {
              $('#form-logoff').find('[name=msg]').val('O painel de senhas expirou');
              $('#form-logoff').submit();
          }

          atualizarRss(retorno.params ? retorno.params.atendimento : []);
          atualizarTela(retorno.params ? retorno.params.registros : []);
        },
        error: function (result) {
          $('#form-logoff').find('[name=msg]').val('Erro ao recuperar as informações do painel');
          $('#form-logoff').submit();
        }
      });
    }

    setInterval(function () {
      atualizar();
    }, 5000);
  }

  function atualizarRss(dados) {
    $('.rss').find('span#em-atendimento').html(dados.qtd_em_atendimento ? dados.qtd_em_atendimento : 0);
    $('.rss').find('span#aguardando').html(dados.qtd_aguardando ? dados.qtd_aguardando : 0);
    $('.rss').find('span#atendidos').html(dados.qtd_atendido ? dados.qtd_atendido : 0);
    $('.rss').find('span#tempo-espera').html(dados.tempo_medio_espera ? dados.tempo_medio_espera : 0);
  }

  function atualizarTela(registros, primeiraVez) {
    let registrosNovos = [];

    if (registros.length > 0) {
      $.each(registros, function (i, v) {
        let found = false;
        if (jsonRegistros.length > 0) {
          $.each(jsonRegistros, function (j, k) {
            if (v.id == k.id && v.qtd_chamadas == k.qtd_chamadas) {
              found = true;
            }
          });
        }

        if (!found) {
          registrosNovos.push(v);
        }
      });
    }

    if (primeiraVez !== true) {
      alertaNovosRegistros(registrosNovos);
    }

    // Atualizando a global jsonRegistros com uma cópia do objeto registros
    jsonRegistros = [...registros];

    // Atualizando a tela
    let primeiro = registros.shift();
    $('.registro-atual .div-senha-atual').html(primeiro ? primeiro.numero : '---');
    $('.registro-atual .div-guiche-atual').html(primeiro ? primeiro.guiche : '---');

    for (var i = 0; i < 4; i++) {
      let registroAtual = registros.shift();
      $('.historico-registros .historico').eq(i).find('.historico-senha').html(registroAtual  ? registroAtual.numero : '---');
      $('.historico-registros .historico').eq(i).find('.historico-guiche').html(registroAtual ? registroAtual.guiche : '---');
      $('.historico-registros .historico').eq(i).find('.historico-hora').html(registroAtual ? registroAtual.hora   : '---');
    }

    atualizarTextFit();
  }

  function atualizarTextFit() {
    $('.painel .textFitted').removeClass('textFitAlignVert');
    $('.painel .textFitted').prop('style', '');

    textFit($('.painel .fit-text'), {
      alignVert: true, // if true, textFit will align vertically using css tables
      alignHoriz: true, // if true, textFit will set text-align: center
      multiLine: false, // if true, textFit will not set white-space: no-wrap
      detectMultiLine: false, // disable to turn off automatic multi-line sensing
      reProcess: true, // if true, textFit will re-process already-fit nodes. Set to 'false' for better performance
      alignVertWithFlexbox: true, // if true, textFit will use flexbox for vertical alignment
    });
  }

  async function alertaNovosRegistros (registros) {
    if(!registros || registros.length == 0) {
      return false;
    }

    registros = registros.reverse();
    for (var i = 0; i < registros.length; i++) {
      $('#audio-chamada').trigger("play");
      $('.alerta-numero .div-senha-atual').html(registros[i].numero);
      $('.alerta-numero .div-guiche-atual').html(registros[i].guiche);

      $(".alerta-numero").fadeIn(0);

      $('.alerta-numero .textFitted').removeClass('textFitAlignVert');
      $('.alerta-numero .textFitted').prop('style', '');

      textFit($('.alerta-numero .fit-text'), {
        alignVert: true, // if true, textFit will align vertically using css tables
        alignHoriz: true, // if true, textFit will set text-align: center
        multiLine: false, // if true, textFit will not set white-space: no-wrap
        detectMultiLine: false, // disable to turn off automatic multi-line sensing
        reProcess: true, // if true, textFit will re-process already-fit nodes. Set to 'false' for better performance
        alignVertWithFlexbox: true, // if true, textFit will use flexbox for vertical alignment
      });

      await timer(2500);
      if (i == (registros.length - 1)) {
        $(".alerta-numero").fadeOut();
      }
    }
  }

  // Returns a Promise that resolves after "ms" Milliseconds
  const timer = ms => new Promise(res => setTimeout(res, ms));

</script>