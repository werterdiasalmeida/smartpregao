<?php
if ($_POST && isset($_POST['act'])) {
    switch ($_POST['act']) {
        case 'carregarMunicipios':
            $uf_sigla = $_POST['uf_sigla'];
            $municipio_id = $_POST['municipio_id'];
            $munHabilitado = 'N';
            $municipios = array();

            if ($uf_sigla) {
                $munHabilitado = 'S';

                $mUnidadeFederacao = new UnidadeFederacao();
                $ufId = $mUnidadeFederacao->getIdBySigla($uf_sigla);

                $mMunicipio = new Municipio();
                $municipios = $mMunicipio->recuperarTodos('id AS codigo, municipio AS descricao', array("uf_id = '{$ufId}'"), 2);
            }

            $db->monta_combom("municipio_id", $municipios, $munHabilitado, 'Selecione...', '', '', '', '', 'S');
            echo "<script  type='text/javascript'>
                        $(function(){
                            $('[name=municipio_id]').selectpicker('refresh');
                        });
                  </script>";
            die;
            break;
    }
}
?>

<div class="modal fade in" id="modal-endereco" tabindex="-1" role="modal-endereco" aria-hidden="true">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <form role="form" name="endereco-form" class="endereco-form" method="POST" enctype="multipart/form-data">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h4 class="modal-title">Endereço</h4>
                </div>
                <div class="modal-body">
                    <div class="row">
                        <div class="col-md-2">
                            <div class="form-group">
                                <label>CEP</label>
                                <?= campo_textom('cep', 'S', 'S', '', 10, 10, '99.999-999', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-5">
                            <div class="form-group">
                                <label>Logradouro</label>
                                <?= campo_textom('logradouro', 'S', 'S', '', 200, 200, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-5">
                            <div class="form-group">
                                <label>Número/Complemento</label>
                                <?= campo_textom('complemento', 'S', 'S', '', 200, 200, '', '', '', '', 0); ?>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-3">
                            <div class="form-group">
                                <label>Bairro</label>
                                <?= campo_textom('bairro', 'S', 'S', '', 200, 200, '', '', '', '', 0); ?>
                            </div>
                        </div>
                        <div class="col-md-4">
                            <div class="form-group">
                                <label>Estado</label>
                                <?php
                                $mUnidadeFederacao = new UnidadeFederacao();
                                $estados = $mUnidadeFederacao->recuperarTodos('uf_sigla AS codigo, uf_nome_estado AS descricao', array(), 2);
                                $db->monta_combom("uf_sigla", $estados, 'S', 'Selecione...', '', '', '', '', 'S');
                                ?>
                            </div>
                        </div>
                        <div class="col-md-5">
                            <div class="form-group">
                                <label>Cidade</label>
                                <div id="cidades">
                                    <?php
                                    $munHabilitado = 'N';
                                    $municipios = array();
                                    $db->monta_combom("municipio_id", $municipios, $munHabilitado, 'Selecione...', '', '', '', '', 'S');
                                    ?>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-md-12">
                            <div class="form-group">
                                <label>Localização</label>
                                <div id="mapa" class="gmaps" style="height: 300px !important;"></div>
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
                <input type="hidden" id="geo_localizacao" name="geo_localizacao" />
            </form>
        </div>
    </div>
</div>

<script src="//maps.google.com/maps/api/js?key=AIzaSyD5k0hd7cZ5TdHpYxESl7Lu3xRYvshzVHA" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/gmaps/gmaps.min.js" type="text/javascript"></script>

<script type="text/javascript">
    var map;
    var defaultLatLng = {
        lat : -15.790669,
        lng : -47.892967,
        zoom : 4
    };

    $(function () {
        $('select[name=uf_sigla]').change(function () {
            carregarMunicipio();
        });

        $('input[type=text][name=cep]').change(function () {
            recuperarEnderecoPorCep();
        });

        $('#modal-endereco').on('shown.bs.modal', function() {
            inicializarMapa();
        });

        $('#modal-endereco').on('hidden.bs.modal', function() {
            limparDadosEndereco();
        });
    });

    function limparDadosEndereco() {
        var form = $('[name=endereco-form]');

        $(form).find('[name=cep]').val('');
        $(form).find('[name=logradouro]').val('');
        $(form).find('[name=complemento]').val('');
        $(form).find('[name=bairro]').val('');
        $(form).find('[name=uf_sigla]').val('').selectpicker('refresh');
        $(form).find('[name=municipio_id]').val('');
        $(form).find('[name=geo_localizacao]').val('');
        $(form).find('[name=index]').val('');
        $(form).find('[name=id]').val('');

        carregarMunicipio('');

        if(map){
            setDefaultCenterMap();
            map.removeMarkers();
        }
    }

    function recuperarEnderecoPorCep() {
        var campoCep = $('[name=cep]');
        var cep = campoCep[0];

        if (!cep || cep.value == '' || cep.value.replace(/[^0-9]/ig, '').length != 8) {
            if (cep.value != '') {
                addMsgErroCampo(campoCep, 'O CEP informado é inválido.');
            }

            limparDadosEndereco();
            return false;
        }

        App.blockUI({
            message: 'Carregando...',
            target: '#endereco'
        });

        $.ajax({
            type: "POST",
            url: "/geral/dne.php",
            data: {opt: 'dne', endcep: cep.value},
            success: function (retorno) {
                eval(retorno);

                if (DNE[0] && DNE.length >= 1) {
                    $('[name=logradouro]').val(DNE[0].logradouro);
                    $('[name=bairro]').val(DNE[0].bairro);
                    $('[name=uf_sigla]').val(DNE[0].estado).selectpicker('refresh');

                    carregarMunicipio(DNE[0].muncod);
                    buscarEnderecoMapa(DNE[0].cidade);
                } else {
                    limparDadosEndereco();
                }

                App.unblockUI('#endereco');
            },
            error: function (result) {
                App.unblockUI('#endereco');
            },
        });
    }

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
        }, function () {
            App.unblockUI('#cidades');
        });
    }

    function inicializarMapa() {
        if(map){
            return;
        }

        map = new GMaps({
            div: '#mapa',
            lat : defaultLatLng.lat,
            lng : defaultLatLng.lng,
            zoom : defaultLatLng.zoom,
            minZoom: 2,
            click: function (e) {
                adicionarPontoMapa({
                    lat: e.latLng.lat(),
                    lng: e.latLng.lng()
                });
            }
        });
    }

    function adicionarPontoMapa(latLng, centralizarPonto) {
        $('input[type=hidden][name=geo_localizacao]').val(JSON.stringify(latLng));

        map.removeMarkers();
        var marker = map.addMarker(latLng);

        if (centralizarPonto === true) {
            map.setZoom(17);
            map.setCenter(latLng.lat, latLng.lng);
        }

        return marker;
    }

    function buscarEnderecoMapa(cidade, incluiCep) {
        var endereco = [];
        incluiCep = typeof incluiCep === 'boolean' ? incluiCep : true;

        if (incluiCep && $('[name=cep]').val()) {
            endereco.push($('[name=cep]').val());
        }

        if ($('[name=bairro]').val()) {
            endereco.push($('[name=bairro]').val());
        }

        if (cidade) {
            endereco.push(cidade);
        }

        if ($('[name=uf_sigla]').val()) {
            endereco.push($('[name=uf_sigla]').val());
        }

        if (endereco.length > 0) {
            endereco.push('Brazil');

            $.ajax({
                type: "GET",
                url: "https://maps.googleapis.com/maps/api/geocode/json",
                dataType: 'JSON',
                data: {
                    address: endereco.join(','),
                    key: 'AIzaSyD5k0hd7cZ5TdHpYxESl7Lu3xRYvshzVHA'
                },
                success: function (retorno) {
                    $('.buscando-cep').hide();
                    if (retorno && retorno.results && retorno.results[0] && retorno.results[0].geometry) {
                        var location = retorno.results[0].geometry.location;
                        adicionarPontoMapa(location, true);
                    }else if(incluiCep){
                        buscarEnderecoMapa(cidade, false);
                    }
                },
                error: function (result) {
                    $('.buscando-cep').hide();
                }
            });
        }
    }

    function incluirEndereco(){
        var form = $('[name=endereco-form]');
        var endereco = {
            cep : $(form).find('[name=cep]').val(),
            logradouro : $(form).find('[name=logradouro]').val(),
            complemento : $(form).find('[name=complemento]').val(),
            bairro : $(form).find('[name=bairro]').val(),
            uf_sigla : $(form).find('[name=uf_sigla]').val(),
            municipio_id : $(form).find('[name=municipio_id]').val(),
            estado_cidade : $(form).find('[name=municipio_id]').find('option:selected').html()  + '/' + $(form).find('[name=uf_sigla]').val(),
            geo_localizacao : $(form).find('[name=geo_localizacao]').val(),
            id : $(form).find('[name=id]').val()
        };

        if(endereco.geo_localizacao){
            endereco.geo_localizacao = JSON.parse(endereco.geo_localizacao);
        }

        adicionarItemTabela(endereco, $('#tabela-enderecos'), $(form).find('#index').val());
        $('#modal-endereco').modal('hide');
    }

    function alterarEndereco(index){
        $('#modal-endereco').modal({
            backdrop: 'static',
        }).modal('show');

        var dados = $('#tabela-enderecos').bootstrapTable("getData");
        var endereco = dados[index];
        var form = $('[name=endereco-form]');

        $(form).find('[name=cep]').val(endereco.cep);
        $(form).find('[name=logradouro]').val(endereco.logradouro);
        $(form).find('[name=complemento]').val(endereco.complemento);
        $(form).find('[name=bairro]').val(endereco.bairro);
        $(form).find('[name=uf_sigla]').val(endereco.uf_sigla).selectpicker('refresh');
        $(form).find('[name=index]').val(index);
        $(form).find('[name=id]').val(endereco.id);

        carregarMunicipio(endereco.municipio_id);

        // var geo_localizacao = JSON.parse(endereco.geo_localizacao);
        setTimeout(function(){
            adicionarPontoMapa(endereco.geo_localizacao, true);
        }, 500)
    }

    function setDefaultCenterMap(){
        map.setCenter(-15.790669, -47.892967);
        map.setZoom(4);
    }
</script>