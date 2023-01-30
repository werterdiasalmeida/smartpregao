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
                            <a onclick="exibirModalBatimentoAtestados()">
                                <i class="bi bi-check-square" title="Batimento de Atestados"></i>
                                Batimento dos Atestados
                            </a>
                            <?php
                            $mEditalBatimentoHabilitacao = new EditalBatimentoHabilitacao();
                            unset($arConfig, $acoes);
                            $arWhere = array();
                            $arWhere[] = " tipo = 'Habilitação Técnica' AND ane.estabelecimento_id = {$dadosEditalCliente['estabelecimento_id']}";
                            $rs = $mEditalBatimentoHabilitacao->getListCliente($arWhere, false, null);
                            if (count($rs)) {
                                $mEditalBatimentoHabilitacao = new Listam(array("style" => "width: 100%;"));
                                $mEditalBatimentoHabilitacao->setCabecalho(array(
                                    'Item do Edital',
                                    'Atestado(s)'
                                ));
                                $mEditalBatimentoHabilitacao->setCorpo($rs, $arConfig);
                                $mEditalBatimentoHabilitacao->setAcao($acao);
                                $mEditalBatimentoHabilitacao->show();
                            } else {
                                echo '<br /><br />';
                            }
                            echo nl2br($dados['habilitacao_tecnica']) ?>
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
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Documentação de Habilitação' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-journals accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-journals"></i>';
                                }
                                ?>
                                Documentação de Habilitação
                            </b>
                        </div>
                    </div>
                    <div id="habilitaca_documentacao" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Documentação de Habilitação')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Novo Documento de Habilitação
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
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#pedido_vistoria">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Pedido de vistoria' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-search accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-search"></i>';
                                }
                                ?>
                                Pedido de vistoria</b>
                        </div>
                    </div>
                    <div id="pedido_vistoria" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Pedido de vistoria')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Novo Pedido de vistoria
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
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#pedido_esclarecimento">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Pedido de esclarecimentos' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-info-circle accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-info-circle"></i>';
                                }
                                ?>
                                Pedido de esclarecimentos</b>
                        </div>
                    </div>
                    <div id="pedido_esclarecimento" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Pedido de esclarecimentos')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Novo Pedido de esclarecimentos
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
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#pedido_impugnacao">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Pedido de impugnação' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-exclamation-diamond accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-exclamation-diamond"></i>';
                                }
                                ?>
                                Pedido de impugnação</b>
                        </div>
                    </div>
                    <div id="pedido_impugnacao" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Pedido de impugnação')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Novo Pedido de impugnação
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