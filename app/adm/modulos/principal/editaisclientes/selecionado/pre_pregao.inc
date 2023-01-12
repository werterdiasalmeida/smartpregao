    <div class="tab-pane fade active " id="kt_tab_pre_pregao" role="tabpanel">
        <div class="row">
            <div class="col-md-6">
                <?php
                require_once('dados_edital.inc');
                ?>
            </div>
            <div class="col-md-6">
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#div_comentarios">
                            <b>
                                <?php
                                if (str_word_count($dados['comentarios'])) {
                                    echo '<i class="bi bi-megaphone-fill accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-megaphone"></i>';
                                }
                                ?>
                                Comentários</b>
                        </div>
                    </div>
                    <div id="div_comentarios" class="collapse">
                        <div class="card-body">
                            <?php
                            echo nl2br($dados['comentarios']) ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#div_habilitacao_tecnica">
                            <b>
                                <?php
                                if (str_word_count($dados['habilitacao_tecnica'])) {
                                    echo '<i class="bi bi-building-check accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-building-check"></i>';
                                }
                                ?>
                                Habilitação Técnica</b>
                        </div>
                    </div>
                    <div id="div_habilitacao_tecnica" class="collapse">
                        <div class="card-body">
                            <?php echo nl2br($dados['habilitacao_tecnica']) ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#div_atalhos">
                            <b> <i class="bi bi-diagram-2-fill accordion_com_conteudo"></i> Atalhos Externos</b>
                        </div>
                    </div>
                    <div id="div_atalhos" class="collapse">
                        <div class="card-body">
                            <?php
                            require('atalhos.inc');
                            ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#habilitaca_documentacao">
                            <b> <i class="bi bi-journals"></i> Habilitação / Documentação</b>
                        </div>
                    </div>
                    <div id="habilitaca_documentacao" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#pedido_vistoria">
                            <b> <i class="bi bi-search"></i> Pedido de vistoria</b>
                        </div>
                    </div>
                    <div id="pedido_vistoria" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#pedido_esclarecimento">
                            <b> <i class="bi bi-info-circle"></i> Pedido de esclarecimentos</b>
                        </div>
                    </div>
                    <div id="pedido_esclarecimento" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#pedido_impugnacao">
                            <b> <i class="bi bi-exclamation-diamond"></i> Pedido de impugnação</b>
                        </div>
                    </div>
                    <div id="pedido_impugnacao" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>