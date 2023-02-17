    <div class="tab-pane fade " id="kt_tab_contratacao" role="tabpanel">
        <div class="row">
            <div class="col-md-6">
                <?php
                require('dados_edital.inc');
                ?>
            </div>
            <div class="col-md-6">
                <div class="card">
                    <div class="card-header">
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#contatacao_documentacao">
                            <b>
                                <?php
                                $mEditalClienteAcompanhamento = new EditalClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Documentação para Contratação' AND editalcliente_id = {$dadosEditalCliente['id']}";
                                $rs = $mEditalClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-file-earmark-check accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-file-earmark-check"></i>';
                                }
                                ?>
                                Documentação para Contratação </b>
                        </div>
                    </div>
                    <div id="contatacao_documentacao" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Documentação para Contratação')">
                                <i class="bi bi-plus-square" title="Novo Documento"></i>
                                Nova Documentação para Contratação
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
                            $arConfig[1]['html'] = <<<HTML
                                <a onclick="baixarAnexo({arqid})">{arquivo}</a>
                            HTML;
                            $mEditalClienteAcompanhamento = new Listam(array("style" => "width: 100%;"));
                            $mEditalClienteAcompanhamento->setCabecalho(array(
                                'Data',
                                'Arquivo',
                                'Informação',
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
                        <div class="card-title collapsed" data-toggle="collapse" data-target="#equipe_base">
                            <b> <i class="bi bi-person-check"></i> Equipe Base Inicial</b>
                        </div>
                    </div>
                    <div id="equipe_base" class="collapse">
                        <div class="card-body">
                            Em breve.
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>