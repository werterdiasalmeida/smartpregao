<?php
$mEstabelecimento = new Estabelecimento();
$enderecos = $mEstabelecimento->listarEnderecosMapa(true);

include APPRAIZ . "includes/cabecalhom.inc";
monta_titulom('Mapa de Clientes e Editais');
?>

<div id="mapa" class="gmaps" style="height: 600px;"></div>

<style type="text/css">
    .lbl-mapa {
        font-weight: bold;
        width: 100px;
        text-align: right;
        margin-right: 5px;
    }
</style>
<script src="//maps.google.com/maps/api/js?key=AIzaSyD5k0hd7cZ5TdHpYxESl7Lu3xRYvshzVHA" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/gmaps/gmaps.min.js" type="text/javascript"></script>
<script src="/includes/metronic/global/plugins/gmaps/markerclusterer.js" type="text/javascript"></script>
<script type="text/javascript">
    var MapaEstabelecimentos = function (){
        var map = null;
        var infoWindow = null;
        var config = {
            lat : -15.790669,
            lng : -47.892967,
            zoom : 5
        };

        return {
            map : map,
            init : function (){
                map = new GMaps({
                    div: '#mapa',
                    lat : config.lat,
                    lng : config.lng,
                    zoom : config.zoom,
                    minZoom: 4
                });
            },
            adicionarPontoMapa : function (marker) {
                return map.addMarker({
                    lat : marker.lat,
                    lng : marker.lng,
                    title: marker.title,
                    info : marker,
                    click: function(e) {
                        if(infoWindow){
                            infoWindow.close();
                        }

                        this.infoWindow.open(this.map, this);
                        infoWindow = this.infoWindow;
                    },
                    infoWindow: {
                        content: '<div class="text-center"><h5>' + marker.razao_social + '</h5></div>' +
                                 '<div><label class="lbl-mapa">Nome fantasia: </label>' + marker.nome_fantasia + '</div>' +
                                 '<div><label class="lbl-mapa">CNPJ: </label>' + marker.cnpj + '</div>' +
                                 '<div><label class="lbl-mapa">Tipo: </label>' + marker.tipo + '</div>' +
                                 '<div class="text-right"><a href="?modulo=principal/estabelecimento/formulario&acao=C&p=' + marker.p + '">Acessar estabelecimento</a></div>'
                    },
                    mouseover: function() {
                        if(infoWindow){
                            infoWindow.close();
                        }

                        this.infoWindow.open(this.map, this);
                        infoWindow = this.infoWindow;
                    },
                    mouseout: function() {
                        // this.infoWindow.close();
                    }
                });
            },
            carregarMapa : function (enderecos) {
                var mapaEstabelecimento = this;
                var markers = [];
                enderecos.forEach(function(endereco){
                    markers.push(mapaEstabelecimento.adicionarPontoMapa(endereco));
                });

                /*new MarkerClusterer(map.map, markers, {
                    imagePath: '/includes/metronic/global/plugins/gmaps/images/m'
                });*/
            },
        }
    };

    $(function(){
        var mapaEstabelecimentos = new MapaEstabelecimentos();
        mapaEstabelecimentos.init();
        mapaEstabelecimentos.carregarMapa(<?= $enderecos ?>);
    });
</script>