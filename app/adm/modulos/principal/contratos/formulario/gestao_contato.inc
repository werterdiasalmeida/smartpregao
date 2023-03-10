    <div class="tab-pane fade" id="kt_tab_gestao_contrato" role="tabpanel">
        <div class="row">
            <div class="col-md-6">
                <?php
                require('dados_contrato.inc');
                ?>
            </div>
            <div class="col-md-6">
                <?php
               
                $ocultar['historico'] = false;
                wf_desenhaBarraNavegacaoHorizontalM($dados['docid'], array('id' => $editalCliente), $ocultar, null, null, null);
                ?>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#contrato_documentacao">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Documentação do Contrato e Aditivos' AND editalcliente_id = {$dadosEditalCliente['id']}";
                               // $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-file-earmark-check accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-file-earmark-check"></i>';
                                }
                                ?>
                                Documentação do Contrato e Aditivos </b>
                        </div>
                    </div>
                    <div id="contrato_documentacao" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Documentação do Contrato e Aditivos')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Nova Documentação do Contrato
                            </a>
                            <?php
                            unset($arConfig, $acoes);
                            $acao = <<<ACAO
                            <div style="min-width: 20px;">
                               <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirAcompanhamento({id});" data-toggle="tooltip"  title="Excluir">
                               <i class="bi bi-x-square"></i>
                               </a>
                            </div> 
                            ACAO;
                            $arConfig[2]['html'] = <<<HTML
                                <a onclick="baixarAnexo({arqid})">{arquivo}</a>
                            HTML;
                            $mEditalClienteAcompanhamento = new Listam(array("style" => "width: 100%;"));
                            $mEditalClienteAcompanhamento->setCabecalho(array(
                                'Data/Validade',
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
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#acompanhamento_problemas">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Acompanhamento (problemas e soluções)' AND editalcliente_id = {$dadosEditalCliente['id']}";
                               // $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-sun accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-sun"></i>';
                                }
                                ?>
                                Acompanhamento (problemas e soluções)</b>
                        </div>
                    </div>
                    <div id="acompanhamento_problemas" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Acompanhamento (problemas e soluções)')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Novo Acompanhamento (problemas e soluções)
                            </a>
                            <?php
                            unset($arConfig, $acoes);
                            $acao = <<<ACAO
                            <div style="min-width: 20px;">
                               <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirAcompanhamento({id});" data-toggle="tooltip"  title="Excluir">
                               <i class="bi bi-x-square"></i>
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
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#equipe_centro_custo">
                            <b> <i class="bi bi-person-check"></i> Equipe e centro de custo</b>
                        </div>
                    </div>
                    <div id="equipe_centro_custo" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#evidencias_entregas">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Evidências e Entregas' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                //$rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-bookmark-check accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-bookmark-check"></i>';
                                }
                                ?>
                                Evidências e Entregas</b>
                        </div>
                    </div>
                    <div id="evidencias_entregas" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Evidências e Entregas')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Nova Evidências e/ou Entrega
                            </a>
                            <?php
                            unset($arConfig, $acoes);
                            $acao = <<<ACAO
                            <div style="min-width: 20px;">
                               <a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirAcompanhamento({id});" data-toggle="tooltip"  title="Excluir">
                               <i class="bi bi-x-square"></i>
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
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#notas_pagamentos">
                            <b> <i class="bi bi-cash"></i> Financeiro (Vencimentos / Faturas / Notas Fiscais / Pagamentos)</b>
                        </div>
                    </div>
                    <div id="notas_pagamentos" class="collapse">
                        <div class="card-body">
                            <?php
                            require('financeiro_contrato.inc');
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>