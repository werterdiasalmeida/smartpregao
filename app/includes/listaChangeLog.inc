<?php
const TIPO_ITEM_VERSAO_MELHORIA = 'M';
const TIPO_ITEM_VERSAO_CORRECAO = 'C';
const TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE = 'N';

$arrVersoes = array(
    array(
        'numVersao' => '1.0.21',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Filtro de profissionais e opção de relatório expandido."
            )
        )
    ),
    array(
        'numVersao' => '1.0.20',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada funcionalidade para salvar relatórios."
            )
        )
    ),
    array(
        'numVersao' => '1.0.19',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Correção de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.18',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada a tela de relatório de atendimentos."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Permitir editar os dados da agenda após a consulta estar finalizada."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Na marcação de agenda, exibir os procedimentos com valores 0,00."
            ),
        )
    ),
    array(
        'numVersao' => '1.0.17',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada a tela de modelo de paciente do médico."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Envio de emails de agendamento e confirmação de consulta para o paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Avaliação da consulta por parte do paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Impressão de extrato na aba \"Meu extrato\"."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Agora é permitido alterar os dados da consulta antes da confirmação da presença."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Correção de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.16',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada a tela de estorno de consulta."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Melhorias na tela de Prontuário."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Melhorias na tela de Agenda."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Correção de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.15',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Exibição de dados da agenda na tela de 'Visão Geral'."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Alteração dos gráficos do dashboard para ordenar os valores exibidos."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Correção de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.14',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Alteração na visão geral do dashboard na ordem das colunas e adicionada uma coluna nova que exibirá o total de pacientes agendados."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Alteração dos gráficos do dashboard para exibir os pacientes atendidos por profissional."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Correção de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.13',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Adicionado o componente do 'memed' na prescrição de medicamentos."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada a aba de \"Diagnósticos\" no prontuário do paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Adicionada a possibilidade de vincular um ou mais códigos CID ao item de prontuário."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Melhorias de usabilidade na tela de prontuário do paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Adicionado o registro do profissional e informação dos CIDs na geração do PDF."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Correção de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.12',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Adicionada as variáveis de \"TURNO\" e \"DATA ATUAL POR EXTENSO\" na tela de Modelo de Documento."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Adicionada a possibilidade de criar um tipo de consulta que não gere valor para o médico."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criação da aba \"Minha Agenda\" na tela inicial do profisiional de saúde."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Melhorias na tela de prontuário do paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Correção de erros."
            )
        )
    ),
);

foreach ($arrVersoes as &$versao) {
    foreach ($versao['listAlteracoes'] as &$itemAlteracao) {
        switch ($itemAlteracao['tipo']) {
            case TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE:
                $itemAlteracao['icone'] = 'fa-plus';
                $itemAlteracao['class_label'] = 'label-success';
                break;
            case TIPO_ITEM_VERSAO_MELHORIA:
                $itemAlteracao['icone'] = 'fa-caret-up';
                $itemAlteracao['class_label'] = 'label-warning';
                break;
            case TIPO_ITEM_VERSAO_CORRECAO:
                $itemAlteracao['icone'] = 'fa-bug';
                $itemAlteracao['class_label'] = 'label-danger';
                break;
        }
    }
}

?>

<div class="portlet-body">
    <?php foreach ($arrVersoes as $versaoNum): ?>
        <h3 style="margin-top: 0;">
            Versão #<?= $versaoNum['numVersao'] ?>
        </h3>

        <ul class="feeds" style="margin-bottom: 25px;">
            <?php foreach ($versaoNum['listAlteracoes'] as $itemVersao) : ?>
                <li>
                    <div class="col1">
                        <div class="cont">
                            <div class="cont-col1">
                                <div class="label label-sm <?= $itemVersao['class_label'] ?>"
                                     style="padding: 5px 4px 6px 5px !important;">
                                    <i class="fa <?= $itemVersao['icone'] ?>"></i>
                                </div>
                            </div>
                            <div class="cont-col2">
                                <div class="desc">
                                    <?= $itemVersao['descricao'] ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </li>
            <?php endforeach; ?>
        </ul>
    <? endforeach; ?>
</div>