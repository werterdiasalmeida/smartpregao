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
                                Comentários</b>
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
                                Habilitação Técnica</b>
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
                                Monitoramento do Pregão </b>
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
                            unset($arConfig, $acao);
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
                                'Informação',
                                'Ação'
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
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Avaliação dos concorrentes' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-binoculars accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-binoculars"></i>';
                                }
                                ?>
                                Avaliação dos concorrentes</b>
                        </div>
                    </div>
                    <div id="avaliacao_contorrentes" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Avaliação dos concorrentes')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Nova Avaliação dos concorrentes
                            </a>
                            <?php
                            unset($arConfig, $acoes);
                            $acao = <<<ACAO
                            <div style="min-width: 20px;">
                               <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirAcompanhamento({id});" data-toggle="tooltip"  title="Excluir">
                                   <i class="icon-trash"></i>
                               </a>
                            </div> 
                            ACAO;
                            $arConfig[2]['html'] = <<<HTML
                                <a onclick="baixarAnexo({arqid})">{arquivo}</a>
                            HTML;
                            $mEditalClienteAcompanhamento = new Listam(array("style" => "width: 100%;"));
                            $mEditalClienteAcompanhamento->setCabecalho(array(
                                'Data',
                                'Informação',
                                'Arquivo',
                                'Ação'
                            ));
                            $mEditalClienteAcompanhamento->setCorpo($rs, $arConfig);
                            $mEditalClienteAcompanhamento->setAcao($acao);
                            $mEditalClienteAcompanhamento->show();
                            ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#pedido_recurso">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Pedido de recurso' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-exclamation-octagon accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-exclamation-octagon"></i>';
                                }
                                ?>
                                Pedido de recurso</b>
                        </div>
                    </div>
                    <div id="pedido_recurso" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Pedido de recurso')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Novo Pedido de recurso
                            </a>
                            <?php
                            unset($arConfig, $acoes);
                            $acao = <<<ACAO
                            <div style="min-width: 20px;">
                               <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirAcompanhamento({id});" data-toggle="tooltip"  title="Excluir">
                                   <i class="icon-trash"></i>
                               </a>
                            </div> 
                            ACAO;
                            $arConfig[2]['html'] = <<<HTML
                                <a onclick="baixarAnexo({arqid})">{arquivo}</a>
                            HTML;
                            $mEditalClienteAcompanhamento = new Listam(array("style" => "width: 100%;"));
                            $mEditalClienteAcompanhamento->setCabecalho(array(
                                'Data',
                                'Informação',
                                'Arquivo',
                                'Ação'
                            ));
                            $mEditalClienteAcompanhamento->setCorpo($rs, $arConfig);
                            $mEditalClienteAcompanhamento->setAcao($acao);
                            $mEditalClienteAcompanhamento->show();
                            ?>
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#poc">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Prova de Conceito (POC)' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-projector accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-projector"></i>';
                                }
                                ?>
                                Prova de Conceito (POC)</b>
                        </div>
                    </div>
                    <div id="poc" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Prova de Conceito (POC)')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Nova Prova de Conceito (POC)
                            </a>
                            <?php
                            unset($arConfig, $acoes);
                            $acao = <<<ACAO
                            <div style="min-width: 20px;">
                               <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirAcompanhamento({id});" data-toggle="tooltip"  title="Excluir">
                                   <i class="icon-trash"></i>
                               </a>
                            </div> 
                            ACAO;
                            $arConfig[2]['html'] = <<<HTML
                                <a onclick="baixarAnexo({arqid})">{arquivo}</a>
                            HTML;
                            $mEditalClienteAcompanhamento = new Listam(array("style" => "width: 100%;"));
                            $mEditalClienteAcompanhamento->setCabecalho(array(
                                'Data',
                                'Informação',
                                'Arquivo',
                                'Ação'
                            ));
                            $mEditalClienteAcompanhamento->setCorpo($rs, $arConfig);
                            $mEditalClienteAcompanhamento->setAcao($acao);
                            $mEditalClienteAcompanhamento->show();
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>