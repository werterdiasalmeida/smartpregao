    <div class="tab-pane fade  " id="kt_tab_tramitacao_anexos" role="tabpanel">
        <div class="row">
            <div class="col-md-6">
                <h5>Tramita��o e Hist�rico</h5>
                <?php
                $_REQUEST['docid'] = $docid;
                $ocultar['historico'] = false;
                wf_desenhaBarraNavegacaoHorizontalM($docid, array('id' => $editalCliente), $ocultar, null, null, null);
                ?>
            </div>
            <div class="col-md-6">
                <h5>Anexos</h5>
                <?php
                ?>
            </div>
        </div>
    </div>