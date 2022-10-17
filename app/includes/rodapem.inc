<?php

if(isset($cabecalhoMetronic) && $cabecalhoMetronic === true) : ?>

    </div>
    <!-- END CONTENT BODY -->
    </div>
    <!-- END CONTENT -->
    </div>
    <!-- END CONTAINER -->
    <!-- BEGIN FOOTER -->
    <div class="page-footer">
        <div class="page-footer-inner">
            <?= @date('Y') ?> &copy; Smart Pregão
            - <a style="color: #FFF;" href="javascript:abrirChangelog()"><?= VERSAO ?></a>
            - Tx.: <?= number_format( ( getmicrotime() - $Tinicio ), 4, ',', '.' ); ?>s / <?=number_format(memory_get_usage()/(1024*1024),2,',','.');?>
        </div>
        <div class="scroll-to-top">
            <i class="icon-arrow-up"></i>
        </div>
    </div>

    <script type="text/javascript">
        function abrirChangelog() {
            var modal = $('#changelog-modal');
            modal.find('.modal-body').html('<div style="height: 250px;"></div>')

            App.blockUI({
                message: 'Carregando...',
                target: '#changelog-modal .modal-body'
            });

            modal.find('.modal-body').load(window.location.href, {
                act: 'abrirChangelog'
            }, function () {
                App.unblockUI('#changelog-modal .modal-body');

                modal.find('.modal-body .portlet-body').slimScroll({
                    height: '250px'
                });
            });

            modal.modal({
                backdrop: 'static'
            }).modal('show');
        }
    </script>

    <div class="modal fade" id="changelog-modal" tabindex="-1" role="basic" aria-hidden="true">
        <div class="modal-dialog modal-lg">
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal" aria-hidden="true"></button>
                    <h2 class="modal-title">Histórico de alterações</h2>
                </div>
                <div class="modal-body">
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn dark btn-outline" data-dismiss="modal">Fechar</button>
                </div>
            </div>
            <!-- /.modal-content -->
        </div>
        <!-- /.modal-dialog -->
    </div>
    <!-- END FOOTER -->
    </body>

    </html>

<?php endif;