    <div class="tab-pane fade " id="kt_tab_pos_pregao" role="tabpanel">
        <div class="row">
            <div class="col-md-6">
                <?php
                require('dados_edital.inc');
                ?>
            </div>
            <div class="col-md-6">
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#div_comentarios3">
                            <b>
                                <?php
                                if (str_word_count($dados['comentarios'])) {
                                    echo '<i class="bi bi-megaphone-fill accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-megaphone"></i>';
                                }
                                ?>
                                Coment�rios</b>
                        </div>
                    </div>
                    <div id="div_comentarios3" class="collapse">
                        <div class="card-body">
                            <?php
                            echo nl2br($dados['comentarios']) ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#div_habilitacao_tecnica3">
                            <b>
                                <?php
                                if (str_word_count($dados['habilitacao_tecnica'])) {
                                    echo '<i class="bi bi-building-check accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-building-check"></i>';
                                }
                                ?>
                                Habilita��o T�cnica</b>
                        </div>
                    </div>
                    <div id="div_habilitacao_tecnica3" class="collapse">
                        <div class="card-body">
                            <?php echo nl2br($dados['habilitacao_tecnica']) ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#div_atalhos3">
                            <b> <i class="bi bi-diagram-2-fill accordion_com_conteudo"></i> Atalhos Externos</b>
                        </div>
                    </div>
                    <div id="div_atalhos3" class="collapse">
                        <div class="card-body">
                            <?php
                            require('atalhos.inc');
                            ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#monitoramento">
                            <b>
                                <?php

                                $where = " editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteMonitoramento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-wifi accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-building-check"></i>';
                                }
                                ?>
                                Monitoramento </b>
                        </div>
                    </div>
                    <div id="monitoramento" class="collapse">
                        <div class="card-body">
                            <br />
                            <a onclick="exibirModalNovoMonitoramento()">
                                <i class="bi bi-plus-square" title="Novo Monitoramento"></i>
                                Novo Monitoramento
                            </a>

                            <?php
                            $acao = <<<ACAO
                           <div style="min-width: 20px;">
                               <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirMonitoramento({id});" data-toggle="tooltip"  title="Excluir">
                                   <i class="icon-trash"></i>
                               </a>
                           </div>
ACAO;
                            $arConfig = array();

                            $arConfig[0]['html'] = <<<HTML
                           <spam class="badge badge-primary"><b>{falta}</b></spam>
HTML;

                            $mEditalClienteMonitoramento = new Listam(array("style" => "width: 100%;"));
                            $mEditalClienteMonitoramento->setCabecalho(array(
                                'Faltam',
                                'Data/Hora',
                                'Informa��o',
                                'A��o'
                            ));
                            $mEditalClienteMonitoramento->setCorpo($rs, $arConfig);
                            $mEditalClienteMonitoramento->setAcao($acao);
                            $mEditalClienteMonitoramento->show();
                            ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#avaliacao_contorrentes">
                            <b> <i class="bi bi-binoculars"></i> Avalia��o dos concorrentes</b>
                        </div>
                    </div>
                    <div id="avaliacao_contorrentes" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#pedido_recurso">
                            <b> <i class="bi bi-exclamation-octagon"></i> Pedido de recurso</b>
                        </div>
                    </div>
                    <div id="pedido_recurso" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#poc">
                            <b> <i class="bi bi-projector"></i> Prova de Conceito (POC)</b>
                        </div>
                    </div>
                    <div id="poc" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>