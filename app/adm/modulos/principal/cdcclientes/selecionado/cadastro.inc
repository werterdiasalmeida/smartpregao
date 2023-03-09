<form role="form" name="formulario" id="form-cadastro" method="POST" enctype="multipart/form-data">
    <input type="hidden" name="act" value="salvarCadastro" />

    <div class="tab-pane fade active " id="kt_tab_cadastro" role="tabpanel">
        <div class="row">
            <div class="col-md-6">
                <?php
                require_once('dados_cdc.inc');
                ?>
            </div>
            <div class="col-md-6">
                <?php
                $_REQUEST['docid'] = $docid;
                $ocultar['historico'] = false;
                wf_desenhaBarraNavegacaoHorizontalM($docid, array('id' => $editalCliente), $ocultar, null, null, null);
                ?>
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
                            <?= campo_textaream('comentarios', 'N', 'S', '', 5, 5, '', '', '', '', 0); ?>
                            <button class="btn blue" type="submit" data-toggle="tooltip" title="Salvar">
                                <i class="bi bi-save"></i>
                            </button>
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
                                $mCDCClienteAcompanhamento = new CDCClienteAcompanhamento();
                                $where = " tipo_acompanhamento = 'Acompanhamento' AND cdc_id = {$dados['id']}";
                                $rs = $mCDCClienteAcompanhamento->getList($where);
                                if (count($rs['dados'])) {
                                    echo '<i class="bi bi-journals accordion_com_conteudo"></i>';
                                } else {
                                    echo '<i class="bi bi-journals"></i>';
                                }
                                ?>
                                Acompanhamento
                            </b>
                        </div>
                    </div>
                    <div id="habilitaca_documentacao" class="collapse">
                        <div class="card-body">
                            <a onclick="exibirModalNovoAcompanhamento('Acompanhamento')">
                                <i class="bi bi-plus-square" title="Novo Acompanhamento"></i>
                                Novo Acompanhamento
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
                            $mCDCClienteAcompanhamento = new Listam(array("style" => "width: 100%;"));
                            $mCDCClienteAcompanhamento->setCabecalho(array(
                                'Data',
                                'Informação',
                                'Arquivo',
                                'Ação'
                            ));
                            $mCDCClienteAcompanhamento->setCorpo($rs, $arConfig);
                            $mCDCClienteAcompanhamento->setAcao($acao);
                            $mCDCClienteAcompanhamento->show();
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    </div>
</form>