<div class="row">
    <div class="col-md-4">
        <div class="form-group">
            <label>1 - Registro ANS</label>
            <?= campo_textom('registro_ans', 'S', $habilitado, '', '', '6', '999999', ''); ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label>2 - Nº Guia no Prestador</label>
            <?= campo_textom('numero_guia_prestador', 'S', 'N', '', '', '20', '', ''); ?>
        </div>
    </div>
    <div class="col-md-4">
        <div class="form-group">
            <label>3 - Nº Guia Principal</label>
            <?= campo_textom('numero_guia_principal', 'N', $habilitado, '', '', '20', '', ''); ?>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>4 - Data da Autorização</label>
            <?= campo_datam('data_autorizacao', 'N', $habilitado, '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>5 - Senha</label>
            <?= campo_textom('senha', 'N', $habilitado, '', '', '20', '', ''); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>6 - Data de Validade da Senha</label>
            <?= campo_datam('data_validade_senha', 'N', $habilitado, '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>7 - Nº Guia atribuído pela operadora</label>
            <?= campo_textom('numero_guia_operadora', 'N', $habilitado, '', '', '20', '', ''); ?>
        </div>
    </div>
</div>

<h4 class="form-section">Dados do Beneficiário</h4>

<div class="row">
    <div class="col-md-4">
        <div class="form-group">
            <label>8 - Número da Carteira</label>
            <?= campo_textom('numero_carteira', 'S', $habilitado, '', '', '20', '', ''); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>9 - Validade da Carteira</label>
            <?= campo_datam('validade_carteira', 'N', $habilitado, '', 'S'); ?>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-6">
        <div class="form-group">
            <label>10 - Nome</label>
            <?= campo_textom('nome_beneficiario', 'S', $habilitado, '', '', '70', '', ''); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>11 - Cartão Nacional de Saúde</label>
            <?= campo_textom('cartao_nacional_saude', 'N', $habilitado, '', '', '15', '', ''); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>12 - Atendimento a RN</label>
            <?= campo_radiom('atendimento_rn', $mGuia->getDominioRecemNascidoRadio(), 'S', false, $habilitado); ?>
        </div>
    </div>
</div>

<h4 class="form-section">Dados do Solicitante</h4>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>13 - Código na Operadora</label>
            <?= campo_textom('codigo_operadora', 'S', $habilitado, '', '', '14', '', ''); ?>
        </div>
    </div>
    <div class="col-md-9">
        <div class="form-group">
            <label>14 - Nome do Contratado</label>
            <?= campo_textom('nome_contratado', 'S', $habilitado, '', '', '70', '', ''); ?>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-4">
        <div class="form-group">
            <label>15 - Nome do Profissional Solicitante</label>
            <?= campo_textom('nome_profissional', 'S', $habilitado, '', '', '70', '', ''); ?>
        </div>
    </div>
    <div class="col-md-2">
        <div class="form-group">
            <label>16 - Conselho</label>
            <?= $db->monta_combom('conselho_profissional', $mGuia->getDominioConselho(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-2">
        <div class="form-group">
            <label>17 - Nº Conselho</label>
            <?= campo_textom('numero_conselho_profissional', 'S', $habilitado, '', '', '15', '', ''); ?>
        </div>
    </div>
    <div class="col-md-2">
        <div class="form-group">
            <label>18 - UF</label>
            <?= $db->monta_combom('uf_conselho_profissional', $mGuia->getDominioUf(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-2">
        <div class="form-group">
            <label>19 - Código CBO</label>
            <?= $db->monta_combom('cbos', $mGuia->getDominioCBO(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
</div>

<h4 class="form-section">Dados da Solicitação</h4>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>21 - Caráter do Atendimento</label>
            <?= $db->monta_combom('carater_atendimento', $mGuia->getDominioCaraterAtendimento(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>22 - Data da Solicitação</label>
            <?= campo_datam('data_solicitacao', 'S', $habilitado, '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-6">
        <div class="form-group">
            <label>23 - Indicação Clínica</label>
            <?= campo_textom('indicacao_clinica', 'N', $habilitado, '', '', '500', '', ''); ?>
        </div>
    </div>
</div>


<h4 class="form-section">Procedimentos ou Itens Assistenciais Solicitados</h4>

<table class="procedimentos-solicitados-adicionar table table-hover table-bordered" style="margin-bottom: 0;">
    <thead>
        <tr>
            <th class="text-center" style="width: 10%;">24 - Tabela</th>
            <th class="text-center" style="width: 20%;">25 - Cód. Procedimento</th>
            <th class="text-center" style="width: 40%;">26 - Descrição</th>
            <th class="text-center" style="width: 10%;">27 - Qtde. Solic.</th>
            <th class="text-center" style="width: 10%;">28 - Qtde. Aut.</th>
            <?php if($habilitado === 'S') : ?>
                <th class="text-center" style="width: 10%;">Ação</th>
            <?php endif; ?>
        </tr>
    </thead>
    <tbody>
        <?php if($habilitado === 'S') : ?>
            <tr>
                <td class="text-center">
                    <?= $db->monta_combom('solicitado_codigo_tabela', $mGuia->getDominioTipoTabela(), $habilitado, '', '', '', '', '', 'N', '', false, null, null, false, false); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('solicitado_codigo_procedimento', 'N', $habilitado, '', '', '150', '', ''); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('solicitado_descricao_procedimento', 'N', $habilitado, '', '', '500', '', '', 'left', '', 0, '', '', null, '', null, null, null, '', true); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('solicitado_qtd_solicitada', 'N', $habilitado, '', '', '3', '999', ''); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('solicitado_qtd_autorizada', 'N', $habilitado, '', '', '3', '999', ''); ?>
                </td>
                <td class="text-center">
                    <input type="hidden" name="solicitado_index" />
                    <button type="button" class="btn btn-circle btn-icon-only btn-default green add-procedimento-solicitado">
                        <i class="fa fa-plus"></i>
                    </button>
                </td>
            </tr>
        <?php endif; ?>
    </tbody>
</table>
<input type="hidden" name="solicitado" />
<table class="procedimentos-solicitados">
    <thead class="hidden">
    <tr>
        <th data-align="center" data-width="10%" data-field="codigo_tabela">24 - Tabela</th>
        <th data-align="center" data-width="20%" data-field="codigo_procedimento">25 - Cód. Procedimento</th>
        <th data-align="left" data-width="40%" data-field="descricao_procedimento">26 - Descrição</th>
        <th data-align="right" data-width="10%" data-field="qtd_solicitada">27 - Qtde. Solic.</th>
        <th data-align="right" data-width="10%" data-field="qtd_autorizada">28 - Qtde. Aut.</th>
        <?php if($habilitado === 'S') : ?>
            <th data-align="center" data-width="10%" data-formatter="acaoProcSolicitados">Ação</th>
        <?php endif; ?>
    </tr>
    </thead>
</table>

<h4 class="form-section">Dados do Contratado Executante</h4>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>29 - Código na Operadora</label>
            <?= campo_textom('codigo_operadora_executante', 'S', $habilitado, '', '', '14', '', ''); ?>
        </div>
    </div>
    <div class="col-md-6">
        <div class="form-group">
            <label>30 - Nome do Contratado</label>
            <?= campo_textom('nome_contratado_executante', 'S', $habilitado, '', '', '70', '', ''); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>31 - Código CNES</label>
            <?= campo_textom('cnes', 'S', $habilitado, '', '', '7', '', ''); ?>
        </div>
    </div>
</div>

<h4 class="form-section">Dados do Atendimento</h4>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>32 - Tipo de Atendimento</label>
            <?= $db->monta_combom('tipo_atendimento', $mGuia->getDominioTipoAtendimento(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>33 - Indicação de Acidente</label>
            <?= $db->monta_combom('indicacao_acidente', $mGuia->getDominioIndicacaoAcidente(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>34 - Tipo de Consulta</label>
            <?= $db->monta_combom('tipo_consulta', $mGuia->getDominioTipoConsulta(), $habilitado, '', '', '', '', '', 'S'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>35 - Motivo de Encerramento</label>
            <?= $db->monta_combom('motivo_encerramento', $mGuia->getDominioMotivoEncerramento(), $habilitado, '', '', '', '', '', 'N'); ?>
        </div>
    </div>
</div>

<h4 class="form-section">Dados da Execução / Procedimentos e Exames Realizados</h4>

<table class="datas-realizacao-adicionar table table-hover table-bordered" style="margin-bottom: 0;">
    <thead>
    <tr>
        <th class="text-center" style="width: 8%;">36 - Data</th>
        <th class="text-center" style="width: 6%;">37 - Hora Ini.</th>
        <th class="text-center" style="width: 6%;">38 - Hora Fin.</th>
        <th class="text-center" style="width: 5%;">39 - Tabela</th>
        <th class="text-center" style="width: 9%;">40 - Cód. Procedimento</th>
        <th class="text-center" style="width: 24%;">41 - Descrição</th>
        <th class="text-center" style="width: 5%;">42 - Qtde.</th>
        <th class="text-center" style="width: 5%;">43 - Via</th>
        <th class="text-center" style="width: 5%;">44 - Tec.</th>
        <th class="text-center" style="width: 5%;">45 - Fator Red./Acresc.</th>
        <th class="text-center" style="width: 7%;">46 - Vlr. Unitário</th>
        <th class="text-center" style="width: 7%;">47 - Vlr. Total</th>
        <?php if($habilitado === 'S') : ?>
            <th class="text-center" style="width: 8%;">Ação</th>
        <?php endif; ?>
    </tr>
    </thead>
    <tbody>
        <?php if($habilitado === 'S') : ?>
            <tr>
                <td class="text-center">
                    <?= campo_datam('executado_data_execucao', 'N', $habilitado, '', 'S'); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('executado_hora_inicial_execucao', 'N', $habilitado, '', '', '5', '', '', 'left', '', 0, '', '', null, '', null, null, null, 'hora'); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('executado_hora_final_execucao', 'N', $habilitado, '', '', '5', '', '', 'left', '', 0, '', '', null, '', null, null, null, 'hora'); ?>
                </td>
                <td class="text-center">
                    <?= $db->monta_combom('executado_codigo_tabela_execucao', $mGuia->getDominioTipoTabela(), $habilitado, '', '', '', '', '', 'N', '', false, null, null, false, false); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('executado_codigo_procedimento_execucao', 'N', $habilitado, '', '', '150', '', ''); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('executado_descricao_execucao', 'N', $habilitado, '', '', '500', '', '', 'left', '', 0, '', '', null, '', null, null, null, '', true); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('executado_qtd_execucao', 'N', $habilitado, '', '', '3', '999', '', 'left', '', 0, '', '', null, '', null, null, null, ''); ?>
                </td>
                <td class="text-center">
                    <?= $db->monta_combom('executado_via', $mGuia->getDominioViaAcesso(), $habilitado, '', '', '', '', '', 'N', '', false, null, null, false, false); ?>
                </td>
                <td class="text-center">
                    <?= $db->monta_combom('executado_tec', $mGuia->getDominioTecnicaUtilizada(), $habilitado, '', '', '', '', '', 'N', '', false, null, null, false, false); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('executado_fator_red_acresc', 'N', $habilitado, '', '', '4', '', '', 'right', '', 0, '', '', '1,00', '', null, null, null, 'float', true); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('executado_valor_unitario', 'N', $habilitado, '', '', '20', '', '', 'left', '', 0, '', '', null, '', null, null, null, 'monetario'); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('executado_valor_total', 'N', 'N', '', '', '20', '', '', 'right', '', 0, '', '', null, '', null, null, null, 'monetario', true); ?>
                </td>
                <td class="text-center">
                    <input type="hidden" name="executado_index" />
                    <button type="button" class="btn btn-circle btn-icon-only btn-default green add-procedimento-executado">
                        <i class="fa fa-plus"></i>
                    </button>
                </td>
            </tr>
        <?php endif; ?>
    </tbody>
</table>
<input type="hidden" name="executado" />
<table class="procedimentos-executados">
    <thead class="hidden">
    <tr>
        <th data-width="8%" data-align="center" data-field="data_execucao">36 - Data</th>
        <th data-width="6%" data-align="center" data-field="hora_inicial_execucao">37 - Hora Ini.</th>
        <th data-width="6%" data-align="center" data-field="hora_final_execucao">38 - Hora Fin.</th>
        <th data-width="5%" data-align="center" data-field="codigo_tabela_execucao">39 - Tabela</th>
        <th data-width="9%" data-align="center" data-field="codigo_procedimento_execucao">40 - Cód. Procedimento</th>
        <th data-width="24%" data-field="descricao_execucao">41 - Descrição</th>
        <th data-width="5%" data-align="right" data-field="qtd_execucao">42 - Qtde.</th>
        <th data-width="5%" data-align="center" data-field="via">43 - Via</th>
        <th data-width="5%" data-align="center" data-field="tec">44 - Tec.</th>
        <th data-width="5%" data-align="right" data-field="fator_red_acresc">45 - Fator Red./Acresc.</th>
        <th data-width="7%" data-align="right" data-field="valor_unitario">46 - Vlr. Unitário</th>
        <th data-width="7%" data-align="right" data-field="valor_total">47 - Vlr. Total</th>
        <?php if($habilitado === 'S') : ?>
            <th data-width="8%" data-align="center" data-formatter="acaoProcExecutados">Ação</th>
        <?php endif; ?>
    </tr>
    </thead>
</table>

<h4 class="form-section">Identificação do(s) Profissionais Executante(s)</h4>

<table class="procedimentos-solicitados-adicionar table table-hover table-bordered" style="margin-bottom: 0;">
    <thead>
        <tr>
            <th class="text-center" style="width: 5%;">48 - Seq. Ref</th>
            <th class="text-center" style="width: 10%;">49 - Grau Part.</th>
            <th class="text-center" style="width: 10%;">50 - Cód. Operadora / CPF</th>
            <th class="text-center" style="width: 25%;">51 - Nome do Profissional</th>
            <th class="text-center" style="width: 10%;">52 - Conselho</th>
            <th class="text-center" style="width: 10%;">53 - Número</th>
            <th class="text-center" style="width: 10%;">54 - UF</th>
            <th class="text-center" style="width: 10%;">55 - CBO</th>
            <?php if($habilitado === 'S') : ?>
                <th class="text-center" style="width: 10%;">Ação</th>
            <?php endif; ?>
        </tr>
    </thead>
    <tbody>
        <?php if($habilitado === 'S') : ?>
            <tr>
                <td class="text-center">
                    -
                </td>
                <td class="text-center">
                    <?= $db->monta_combom('profissional_executante_grau_part', $mGuia->getDominioGrauParticipacao(), $habilitado, '', '', '', '', '', 'N', '', false, null, null, false, false); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('profissional_executante_codigo_operadora', 'N', $habilitado, '', '', '14', '', ''); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('profissional_executante_nome_profissional', 'N', $habilitado, '', '', '70', '', ''); ?>
                </td>
                <td class="text-center">
                    <?= $db->monta_combom('profissional_executante_conselho_profissional', $mGuia->getDominioConselho(), $habilitado, '', '', '', '', '', 'N', '', false, null, null, false, false); ?>
                </td>
                <td class="text-center">
                    <?= campo_textom('profissional_executante_numero_conselho_profissional', 'N', $habilitado, '', '', '15', '', ''); ?>
                </td>
                <td class="text-center">
                    <?= $db->monta_combom('profissional_executante_uf_conselho_profissional', $mGuia->getDominioUf(), $habilitado, '', '', '', '', '', 'N', '', false, null, null, false, false); ?>
                </td>
                <td class="text-center">
                    <?= $db->monta_combom('profissional_executante_cbos', $mGuia->getDominioCBO(), $habilitado, '', '', '', '', '', 'N', '', false, null, null, false, false); ?>
                </td>
                <td class="text-center">
                    <input type="hidden" name="profissional_executante_index" />
                    <button type="button" class="btn btn-circle btn-icon-only btn-default green add-profissional-executante">
                        <i class="fa fa-plus"></i>
                    </button>
                </td>
            </tr>
        <?php endif; ?>
    </tbody>
</table>
<input type="hidden" name="profissional_executante" />
<table class="profissionais-executantes">
    <thead class="hidden">
        <tr>
            <th data-align="center" data-width="5%" data-formatter="campoSeqRefProfissionaisExecutantes">48 - Seq. Ref</th>
            <th data-align="center" data-width="10%" data-field="grau_part">49 - Grau Part.</th>
            <th data-width="10%" data-field="codigo_operadora">50 - Cód. Operadora / CPF</th>
            <th data-field="nome_profissional" data-width="25%">51 - Nome do Profissional</th>
            <th data-align="center" data-width="10%" data-field="conselho_profissional">52 - Conselho</th>
            <th data-field="numero_conselho_profissional" data-width="10%">53 - Número</th>
            <th data-align="center" data-width="10%" data-field="uf_conselho_profissional" data-formatter="campoUfProfissionaisExecutantes">54 - UF</th>
            <th data-align="center" data-width="10%" data-field="cbos">55 - CBO</th>
            <?php if($habilitado === 'S') : ?>
                <th data-align="center" data-width="10%" data-formatter="acaoProfissionaisExecutantes">Ação</th>
            <?php endif; ?>
        </tr>
    </thead>
</table>

<div class="row margin-bottom-10">
    <div class="col-md-6">
        <table class="datas-realizacao-adicionar table table-hover table-bordered margin-top-10" style="margin-bottom: 0;">
            <thead>
            <tr>
                <th class="text-center" style="width: 80%;">56 - Data de Realizacação de Procedimentos em Série</th>
                <?php if($habilitado === 'S') : ?>
                    <th class="text-center" style="width: 20%;">Ação</th>
                <?php endif; ?>
            </tr>
            </thead>
            <tbody>
            <?php if($habilitado === 'S') : ?>
                <tr>
                    <td class="text-center">
                        <?= campo_datam('realizacao_procedimentos_data', 'N', $habilitado, '', 'S'); ?>
                    </td>
                    <td class="text-center">
                        <input type="hidden" name="realizacao_procedimentos_index" />
                        <button type="button" class="btn btn-circle btn-icon-only btn-default green add-data-realizacao">
                            <i class="fa fa-plus"></i>
                        </button>
                    </td>
                </tr>
            <?php endif; ?>
            </tbody>
        </table>
        <input type="hidden" name="data_realizacao_procedimentos" />
        <table class="datas-realizacao">
            <thead class="hidden">
            <tr>
                <th data-align="center" data-width="80%" data-field="data_realizacao_procedimentos">56 - Data de Realizacação de Procedimentos em Série</th>
                <?php if($habilitado === 'S') : ?>
                    <th data-align="center" data-width="20%" data-formatter="acaoDatasRealizacao">Ação</th>
                <?php endif; ?>
            </tr>
            </thead>
        </table>
    </div>

    <div class="col-md-6">
        <div class="form-group margin-top-10">
            <label>58 - Observação / Justificativa</label>
            <?= campo_textaream('observacao', 'N', $habilitado, '', 5, 5, 1000); ?>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>59 - Total de Procedimentos</label>
            <?= campo_textom('total_procedimentos', 'N', 'N', '', '', '10', '', '', 'right', '', 0, '', '', null, '', null, null, null, 'monetario', ($habilitado === 'S')); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>60 - Total de Taxas e Aluguéis</label>
            <?= campo_textom('total_taxas_alugueis', 'N', $habilitado, '', '', '10', '', '', 'left', '', 0, '', '', null, '', null, null, null, 'monetario'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>61 - Total de Materiais</label>
            <?= campo_textom('total_materiais', 'N', $habilitado, '', '', '10', '', '', 'left', '', 0, '', '', null, '', null, null, null, 'monetario'); ?>
        </div>
    </div>

    <div class="col-md-3">
        <div class="form-group">
            <label>62 - Total de OPME</label>
            <?= campo_textom('total_opme', 'N', $habilitado, '', '', '10', '', '', 'left', '', 0, '', '', null, '', null, null, null, 'monetario'); ?>
        </div>
    </div>
</div>

<div class="row">
    <div class="col-md-3">
        <div class="form-group">
            <label>63 - Total de Medicamentos</label>
            <?= campo_textom('total_medicamentos', 'N', $habilitado, '', '', '10', '', '', 'left', '', 0, '', '', null, '', null, null, null, 'monetario'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>64 - Total de Gases Medicinais</label>
            <?= campo_textom('total_gases_medicinais', 'N', $habilitado, '', '', '10', '', '', 'left', '', 0, '', '', null, '', null, null, null, 'monetario'); ?>
        </div>
    </div>
    <div class="col-md-3">
        <div class="form-group">
            <label>65 - Total Geral</label>
            <?= campo_textom('total_geral', 'S', 'N', '', '', '10', '', '', 'right', '', 0, '', '', null, '', null, null, null, 'monetario', ($habilitado === 'S')); ?>
        </div>
    </div>
    <?php if($exibirValorProfissional) : ?>
        <div class="col-md-3">
            <div class="form-group">
                <label class="label-vlr-profissional">
                    Valor do profissional
                </label>
                <?= campo_textom('valor_profissional', 'S', $habilitado, '', '', '10', '', '', 'right', '', 0, '', '', null, '', null, null, null, 'monetario', ($habilitado === 'S')); ?>
            </div>
        </div>
    <?php endif; ?>
</div>

<style type="text/css">
    .bs-select-d {
        width: 62px;
    }

    .bs-select-1v {
        width: 54px;
    }

    .bs-select-7v {
        width: 98px;
    }
</style>

<script type="text/javascript">
    $(function () {
        let guiaForm = $('.guia-form');

        adicionarLabelObrigatorio(guiaForm.find(':required'));
        adicionarDatePicker(guiaForm);

        guiaForm.find('.monetario').inputmask('decimal', {
            integerDigits: 6,
            autoGroup: true,
            digits: 2,
            groupSeparator: '.',
            radixPoint: ',',
            rightAlign: true,
            digitsOptional: false,
            placeholder: '0,00',
            allowMinus: false,
            allowPlus: false
        });

        setTussAutocomplete($('[name=solicitado_codigo_procedimento]'), [], function (){
            let selectize = $(this)[0].selectize;
            let selectedItem = selectize.options[selectize.getValue()];
            $('[name=solicitado_descricao_procedimento]').val(selectedItem ? selectedItem.descricao : '');
        });
        guiaForm.find('button.add-procedimento-solicitado').click(adicionarProcedimentoSolicitado);
        guiaForm.find('table.procedimentos-solicitados').bootstrapTable({
            height: 200,
            data: <?= json_encode(utf8ArrayEncode($solicitado ?: [])) ?>,
        });

        setTussAutocomplete($('[name=executado_codigo_procedimento_execucao]'), [], function (){
            let selectize = $(this)[0].selectize;
            let selectedItem = selectize.options[selectize.getValue()];
            $('[name=executado_descricao_execucao]').val(selectedItem ? selectedItem.descricao : '');
        });
        guiaForm.find('button.add-procedimento-executado').click(adicionarProcedimentoExecutado);
        guiaForm.find('table.procedimentos-executados').bootstrapTable({
            height: 200,
            data: <?= json_encode(utf8ArrayEncode($executado ?: [])) ?>
        });

        guiaForm.find('button.add-profissional-executante').click(adicionarProfissionalExecutante);
        guiaForm.find('table.profissionais-executantes').bootstrapTable({
            height: 200,
            data: <?= json_encode(utf8ArrayEncode($profissional_executante ?: [])) ?>
        });

        guiaForm.find('button.add-data-realizacao').click(adicionarDataRealizacao);
        guiaForm.find('table.datas-realizacao').bootstrapTable({
            height: 200,
            data: <?= json_encode(utf8ArrayEncode($data_realizacao_procedimentos ?: [])) ?>
        });

        guiaForm.find('.bs-select').selectpicker('refresh');
        guiaForm.validate().destroy();

        <?php if($habilitado === 'S') : ?>
            guiaForm.validateForm({
                rules: {},
                submitHandler: function (e) {
                    App.blockUI({
                        message: 'Carregando...',
                        target: guiaForm
                    });

                    guiaForm.find('input[type=hidden][name=solicitado]').val(
                        JSON.stringify(guiaForm.find('table.procedimentos-solicitados').bootstrapTable('getData'))
                    );

                    guiaForm.find('input[type=hidden][name=data_realizacao_procedimentos]').val(
                        JSON.stringify(guiaForm.find('table.datas-realizacao').bootstrapTable('getData'))
                    );

                    guiaForm.find('input[type=hidden][name=executado]').val(
                        JSON.stringify(guiaForm.find('table.procedimentos-executados').bootstrapTable('getData'))
                    );

                    guiaForm.find('input[type=hidden][name=profissional_executante]').val(
                        JSON.stringify(guiaForm.find('table.profissionais-executantes').bootstrapTable('getData'))
                    );

                    $.ajax({
                        type: "POST",
                        url: window.location.href,
                        dataType: 'json',
                        data: guiaForm.serialize(),
                        success: function (retorno) {
                            exibirMsg(retorno.msg);
                            carregarGuia(<?= $_REQUEST['agenda_id'] ?>);
                        },
                        error: function (result) {
                            App.unblockUI(guiaForm);
                            exibirAlert('Não foi possível salvar a guia');
                        }
                    });
                }
            });
        <?php endif; ?>

        $('.hora').inputmask("99:99", {placeholder: ''}).timepicker({
            autoclose: true,
            minuteStep: 1,
            showSeconds: false,
            showMeridian: false,
            defaultTime: null,
            format: 'HH:mm'
        }).on('keydown', function (e) {
            if (e.keyCode === 9) {
                $(this).timepicker('hideWidget');
            }
        }).on('show.timepicker', function (e) {
            if (e.time.value === '') {
                $(this).timepicker('setTime', '08:00');
            }
        });

        $('.monetario').inputmask('decimal', {
            integerDigits: 6,
            autoGroup: true,
            digits: 2,
            groupSeparator: '.',
            radixPoint: ',',
            rightAlign: true,
            digitsOptional: false,
            placeholder: '0,00',
            allowMinus: false,
            allowPlus: false
        });

        $('.float').inputmask('decimal', {
            integerDigits: 1,
            autoGroup: true,
            digits: 2,
            groupSeparator: '.',
            radixPoint: ',',
            rightAlign: true,
            digitsOptional: false,
            placeholder: '0,00',
            allowMinus: false,
            allowPlus: false
        });

        $('#total_taxas_alugueis, #total_materiais, #total_opme, #total_medicamentos, #total_gases_medicinais').change(calcularTotalGuiaExame);
        $('#executado_qtd_execucao, #executado_valor_unitario').change(calcularValorTotalProcedimento);


        $('.label-vlr-profissional').tooltip({
            html : true,
            title : 'Total de Procedimentos x <?= formata_valor($dadosAgenda['percentual_profissional'], 0) ?>% = Valor do profissional'
        });
    });

    function highlightTr(tabela, index) {
        if(index === '') {
            index = tabela.bootstrapTable('getData').length - 1;
        }

        tabela.find("tr[data-index="+index+"]").effect("highlight", {color: '#ffffcc'}, 1500);
        tabela.closest('.fixed-table-body').animate({
            scrollTop: (index * 55)
        }, 500);
    }

    function adicionarProcedimentoSolicitado() {
        let campoCodigoTabela = $('[name=solicitado_codigo_tabela]');
        let campoCodigoProcedimento = $('[name=solicitado_codigo_procedimento]');
        let campoDescricaoProcedimento = $('[name=solicitado_descricao_procedimento]');
        let campoQtdSolicitada = $('[name=solicitado_qtd_solicitada]');
        let campoQtdAutorizada = $('[name=solicitado_qtd_autorizada]');
        let campoIndex = $('[name=solicitado_index]');

        let index = campoIndex.val();

        let tabela = $('table.procedimentos-solicitados');
        adicionarItemTabela({
            codigo_tabela: campoCodigoTabela.val(),
            codigo_procedimento: campoCodigoProcedimento.val(),
            descricao_procedimento: campoDescricaoProcedimento.val(),
            qtd_solicitada: campoQtdSolicitada.val(),
            qtd_autorizada: campoQtdAutorizada.val(),
        }, tabela, index);

        let procedimentoSelectize = campoCodigoProcedimento[0].selectize;

        procedimentoSelectize.setValue('');
        campoCodigoTabela.val('');
        campoDescricaoProcedimento.val('');
        campoQtdSolicitada.val('');
        campoQtdAutorizada.val('');
        campoIndex.val('');

        highlightTr(tabela, index);
    }

    function alterarProcedimentoSolicitado (index) {
        let row = getItemTabela(index, $('table.procedimentos-solicitados'));

        let campoCodigoTabela = $('[name=solicitado_codigo_tabela]');
        let campoCodigoProcedimento = $('[name=solicitado_codigo_procedimento]');
        let campoDescricaoProcedimento = $('[name=solicitado_descricao_procedimento]');
        let campoQtdSolicitada = $('[name=solicitado_qtd_solicitada]');
        let campoQtdAutorizada = $('[name=solicitado_qtd_autorizada]');
        let campoIndex = $('[name=solicitado_index]');

        let procedimentoSelectize = campoCodigoProcedimento[0].selectize;

        procedimentoSelectize.addOption({codigo : row.codigo_procedimento, descricao : row.descricao_procedimento});
        procedimentoSelectize.setValue(row.codigo_procedimento);
        campoCodigoTabela.val(row.codigo_tabela);
        campoDescricaoProcedimento.val(row.descricao_procedimento);
        campoQtdSolicitada.val(row.qtd_solicitada);
        campoQtdAutorizada.val(row.qtd_autorizada);
        campoIndex.val(index);
    }

    function excluirProcedimentoSolicitado(index) {
        removerItemTabela(index, $('table.procedimentos-solicitados'));
    }

    function acaoProcSolicitados(value, row, index) {
        return '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarProcedimentoSolicitado(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>'
            +'<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirProcedimentoSolicitado(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';
    }

    function adicionarProcedimentoExecutado() {
        let tabela = $('table.procedimentos-executados');

        let campoData = $('[name=executado_data_execucao]');
        let campoHoraInicial = $('[name=executado_hora_inicial_execucao]');
        let campoHoraFinal = $('[name=executado_hora_final_execucao]');
        let campoCodTabela = $('[name=executado_codigo_tabela_execucao]');
        let campoCodigoProc = $('[name=executado_codigo_procedimento_execucao]');
        let campoDescricao = $('[name=executado_descricao_execucao]');
        let campoQtd = $('[name=executado_qtd_execucao]');
        let campoVia = $('[name=executado_via]');
        let campoTec = $('[name=executado_tec]');
        let campoRedAcresc = $('[name=executado_fator_red_acresc]');
        let campoValorUnit = $('[name=executado_valor_unitario]');
        let campoValorTotal = $('[name=executado_valor_total]');
        let campoIndex = $('[name=executado_index]');

        let index = campoIndex.val();

        adicionarItemTabela({
            data_execucao: campoData.val(),
            hora_inicial_execucao: campoHoraInicial.val(),
            hora_final_execucao: campoHoraFinal.val(),
            codigo_tabela_execucao: campoCodTabela.val(),
            codigo_procedimento_execucao: campoCodigoProc.val(),
            descricao_execucao: campoDescricao.val(),
            qtd_execucao: campoQtd.val(),
            via: campoVia.val(),
            tec: campoTec.val(),
            fator_red_acresc: '1,00', //campoRedAcresc.val(),
            valor_unitario: campoValorUnit.val(),
            valor_total: campoValorTotal.val()
        }, tabela, index);

        let procedimentoSelectize = campoCodigoProc[0].selectize;

        campoData.val('');
        campoHoraInicial.val('');
        campoHoraFinal.val('');
        campoCodTabela.val('');
        procedimentoSelectize.setValue('');
        campoDescricao.val('');
        campoQtd.val('');
        campoVia.val('');
        campoTec.val('');
        campoRedAcresc.val('1,00');
        campoValorUnit.val('');
        campoValorTotal.val('');
        campoIndex.val('');

        highlightTr(tabela, index);
        calcularTotalGuiaExame();
    }

    function alterarProcedimentoExecutado(index) {
        let row = getItemTabela(index, $('table.procedimentos-executados'));

        let campoData = $('[name=executado_data_execucao]');
        let campoHoraInicial = $('[name=executado_hora_inicial_execucao]');
        let campoHoraFinal = $('[name=executado_hora_final_execucao]');
        let campoCodTabela = $('[name=executado_codigo_tabela_execucao]');
        let campoCodigoProc = $('[name=executado_codigo_procedimento_execucao]');
        let campoDescricao = $('[name=executado_descricao_execucao]');
        let campoQtd = $('[name=executado_qtd_execucao]');
        let campoVia = $('[name=executado_via]');
        let campoTec = $('[name=executado_tec]');
        let campoRedAcresc = $('[name=executado_fator_red_acresc]');
        let campoValorUnit = $('[name=executado_valor_unitario]');
        let campoValorTotal = $('[name=executado_valor_total]');
        let campoIndex = $('[name=executado_index]');

        let procedimentoSelectize = campoCodigoProc[0].selectize;

        if(row.data_execucao) {
            campoData.datepicker("update", toDateObj(row.data_execucao));
        }
        campoHoraInicial.val(row.hora_inicial_execucao);
        campoHoraFinal.val(row.hora_final_execucao);
        campoCodTabela.val(row.codigo_tabela_execucao);
        procedimentoSelectize.addOption({codigo : row.codigo_procedimento_execucao, descricao : row.descricao_execucao});
        procedimentoSelectize.setValue(row.codigo_procedimento_execucao);
        campoDescricao.val(row.descricao_execucao);
        campoQtd.val(row.qtd_execucao);
        campoVia.val(row.via);
        campoTec.val(row.tec);
        campoRedAcresc.val(row.fator_red_acresc);
        campoValorUnit.val(row.valor_unitario);
        campoValorTotal.val(row.valor_total);
        campoIndex.val(index);
    }

    function excluirProcedimentoExecutado(index) {
        removerItemTabela(index, $('table.procedimentos-executados'));
        calcularTotalGuiaExame();
    }

    function acaoProcExecutados(value, row, index) {
        return '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarProcedimentoExecutado(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>'
            + '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirProcedimentoExecutado(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';
    }

    function adicionarDataRealizacao() {
        let campoData = $('[name=realizacao_procedimentos_data]');
        let campoIndex = $('[name=realizacao_procedimentos_index]');

        let index = campoIndex.val();

        let tabela = $('table.datas-realizacao');
        adicionarItemTabela({
            data_realizacao_procedimentos: campoData.val(),
        }, tabela, index);

        campoData.val('');
        campoIndex.val('');

        highlightTr(tabela, index);
    }

    function alterarDataRealizacao(index) {
        let row = getItemTabela(index, $('table.datas-realizacao'));

        let campoData = $('[name=realizacao_procedimentos_data]');
        let campoIndex = $('[name=realizacao_procedimentos_index]');

        if(row.data_realizacao_procedimentos) {
            campoData.datepicker("update", toDateObj(row.data_realizacao_procedimentos));
        }

        campoIndex.val(index);
    }

    function excluirDataRealizacao(index) {
        removerItemTabela(index, $('table.datas-realizacao'));
    }

    function acaoDatasRealizacao(value, row, index) {
        return '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarDataRealizacao(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>'
            + '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirDataRealizacao(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';
    }

    function adicionarProfissionalExecutante() {
        let tabela = $('table.profissionais-executantes');

        let campoGrauPart = $('[name=profissional_executante_grau_part]');
        let campoCodigoOperadora = $('[name=profissional_executante_codigo_operadora]');
        let campoNomeProfissional = $('[name=profissional_executante_nome_profissional]');
        let campoConselhoProfissional = $('[name=profissional_executante_conselho_profissional]');
        let campoNumeroConselho = $('[name=profissional_executante_numero_conselho_profissional]');
        let campoUfConselho = $('[name=profissional_executante_uf_conselho_profissional]');
        let campoCbos = $('[name=profissional_executante_cbos]');
        let campoIndex = $('[name=profissional_executante_index]');

        let index = campoIndex.val();

        adicionarItemTabela({
            grau_part: campoGrauPart.val(),
            codigo_operadora: campoCodigoOperadora.val(),
            nome_profissional: campoNomeProfissional.val(),
            conselho_profissional: campoConselhoProfissional.val(),
            numero_conselho_profissional: campoNumeroConselho.val(),
            uf_conselho_profissional: campoUfConselho.val(),
            cbos: campoCbos.val(),
        }, tabela, index);

        campoGrauPart.val('');
        campoCodigoOperadora.val('');
        campoNomeProfissional.val('');
        campoConselhoProfissional.val('');
        campoNumeroConselho.val('');
        campoUfConselho.val('');
        campoCbos.val('');
        campoIndex.val('');

        highlightTr(tabela, index);
    }

    function alterarProfissionalExecutante(index) {
        let row = getItemTabela(index, $('table.profissionais-executantes'));

        let campoGrauPart = $('[name=profissional_executante_grau_part]');
        let campoCodigoOperadora = $('[name=profissional_executante_codigo_operadora]');
        let campoNomeProfissional = $('[name=profissional_executante_nome_profissional]');
        let campoConselhoProfissional = $('[name=profissional_executante_conselho_profissional]');
        let campoNumeroConselho = $('[name=profissional_executante_numero_conselho_profissional]');
        let campoUfConselho = $('[name=profissional_executante_uf_conselho_profissional]');
        let campoCbos = $('[name=profissional_executante_cbos]');
        let campoIndex = $('[name=profissional_executante_index]');

        campoGrauPart.val(row.grau_part);
        campoCodigoOperadora.val(row.codigo_operadora);
        campoNomeProfissional.val(row.nome_profissional);
        campoConselhoProfissional.val(row.conselho_profissional);
        campoNumeroConselho.val(row.numero_conselho_profissional);
        campoUfConselho.val(row.uf_conselho_profissional);
        campoCbos.val(row.cbos);
        campoIndex.val(index);
    }

    function excluirProfissionalExecutante(index) {
        removerItemTabela(index, $('table.profissionais-executantes'));
    }

    function acaoProfissionaisExecutantes(value, row, index) {
        return '<a class="btn btn-circle btn-icon-only btn-default blue" href="javascript:alterarProfissionalExecutante(' + index + ')" title="Alterar" style="margin: 2px;">'
            + '<i class="icon-pencil"></i>'
            + '</a>'
            + '<a class="btn btn-circle btn-icon-only btn-default red" href="javascript:excluirProfissionalExecutante(' + index + ')" title="Excluir" style="margin: 2px;">'
            + '<i class="icon-trash"></i>'
            + '</a>';
    }

    function campoSeqRefProfissionaisExecutantes (value, row, index) {
        return index + 1;
    }

    function campoUfProfissionaisExecutantes(value, row, index) {
        let ufs = <?= json_encode(utf8ArrayEncode($mGuia->getDominioUf())) ?>;
        ufs.forEach(function(uf){
            if(uf.codigo === value) {
                value = uf.descricao;
                return true;
            }
        });
        return value;
    }

    function setTussAutocomplete (elem, option, onChange) {
        onChange = onChange ? onChange : function () {};
        $(elem).selectize({
            persist: false,
            maxItems: 1,
            valueField: 'codigo',
            labelField: 'descricao',
            searchField: ['descricao', 'codigo'],
            options: option,
            render: {
                item: function(item, escape) {
                    return '<div>' +
                        item.codigo +
                        '</div>';
                },
                option: function(item, escape) {
                    return '<div>' +
                        '<strong>' + escape(item.codigo) + '</strong> - ' +
                        escape(item.descricao) +
                        '</div>';
                }
            },
            load: function(query, callback) {
                if (!query.length) {
                    return callback();
                }

                $.ajax({
                    url: window.location.href,
                    type: 'POST',
                    dataType : 'json',
                    data : {
                        descricao : query,
                        act : 'buscarTuss'
                    },
                    error: function() {
                        callback();
                    },
                    success: function(response) {
                        callback(response);
                    }
                });
            }
        }).on('change', onChange);
    }

    function calcularTotalGuiaExame() {
        let campoTotalProcedimentos = $('[name=total_procedimentos]');
        let totalTaxasAlugueis = $('[name=total_taxas_alugueis]').val();
        let totalMateriais = $('[name=total_materiais]').val();
        let totalOpme = $('[name=total_opme]').val();
        let totalMedicamentos = $('[name=total_medicamentos]').val();
        let totalGasesMedicinais = $('[name=total_gases_medicinais]').val();
        let campoTotalGeral = $('[name=total_geral]');

        let totalProcedimentos = 0;
        $('.procedimentos-executados').bootstrapTable('getData').forEach(function(procedimento){
            totalProcedimentos += valueToFloat(procedimento.valor_total);
        });

        let totalGeral = totalProcedimentos
            + valueToFloat(totalTaxasAlugueis)
            + valueToFloat(totalMateriais)
            + valueToFloat(totalOpme)
            + valueToFloat(totalMedicamentos)
            + valueToFloat(totalGasesMedicinais);

        campoTotalProcedimentos.val(mascaraglobal('[###.]###,##', totalProcedimentos.toFixed(2)));
        campoTotalGeral.val(mascaraglobal('[###.]###,##', totalGeral.toFixed(2)));

        calcularValorProfissional();
    }

    function calcularValorTotalProcedimento() {
        let quantidade = $('#executado_qtd_execucao').val();
        let valorUnitario = $('#executado_valor_unitario').val();
        let valorTotal = valueToFloat(quantidade) * valueToFloat(valorUnitario);

        $('#executado_valor_total').val(mascaraglobal('[###.]###,##', valorTotal.toFixed(2)));
    }

    function calcularValorProfissional() {
        let percentual = parseFloat('<?= $dadosAgenda['percentual_profissional'] ?>');
        let totalProcedimentos = valueToFloat($('[name=total_procedimentos]').val());
        $('[name=valor_profissional]').val(mascaraglobal('[###.]###,##', (totalProcedimentos * (percentual / 100)).toFixed(2)));
    }
</script>
