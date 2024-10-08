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
                'descricao' => "Filtro de profissionais e op��o de relat�rio expandido."
            )
        )
    ),
    array(
        'numVersao' => '1.0.20',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada funcionalidade para salvar relat�rios."
            )
        )
    ),
    array(
        'numVersao' => '1.0.19',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Corre��o de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.18',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada a tela de relat�rio de atendimentos."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Permitir editar os dados da agenda ap�s a consulta estar finalizada."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Na marca��o de agenda, exibir os procedimentos com valores 0,00."
            ),
        )
    ),
    array(
        'numVersao' => '1.0.17',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada a tela de modelo de paciente do m�dico."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Envio de emails de agendamento e confirma��o de consulta para o paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Avalia��o da consulta por parte do paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Impress�o de extrato na aba \"Meu extrato\"."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Agora � permitido alterar os dados da consulta antes da confirma��o da presen�a."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Corre��o de erros."
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
                'descricao' => "Melhorias na tela de Prontu�rio."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Melhorias na tela de Agenda."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Corre��o de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.15',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Exibi��o de dados da agenda na tela de 'Vis�o Geral'."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Altera��o dos gr�ficos do dashboard para ordenar os valores exibidos."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Corre��o de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.14',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Altera��o na vis�o geral do dashboard na ordem das colunas e adicionada uma coluna nova que exibir� o total de pacientes agendados."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Altera��o dos gr�ficos do dashboard para exibir os pacientes atendidos por profissional."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Corre��o de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.13',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Adicionado o componente do 'memed' na prescri��o de medicamentos."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Criada a aba de \"Diagn�sticos\" no prontu�rio do paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Adicionada a possibilidade de vincular um ou mais c�digos CID ao item de prontu�rio."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Melhorias de usabilidade na tela de prontu�rio do paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Adicionado o registro do profissional e informa��o dos CIDs na gera��o do PDF."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Corre��o de erros."
            )
        )
    ),
    array(
        'numVersao' => '1.0.12',
        'listAlteracoes' => array(
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Adicionada as vari�veis de \"TURNO\" e \"DATA ATUAL POR EXTENSO\" na tela de Modelo de Documento."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Adicionada a possibilidade de criar um tipo de consulta que n�o gere valor para o m�dico."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_NOVA_FUNCIONALIDADE,
                'descricao' => "Cria��o da aba \"Minha Agenda\" na tela inicial do profisiional de sa�de."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_MELHORIA,
                'descricao' => "Melhorias na tela de prontu�rio do paciente."
            ),
            array(
                'tipo' => TIPO_ITEM_VERSAO_CORRECAO,
                'descricao' => "Corre��o de erros."
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
            Vers�o #<?= $versaoNum['numVersao'] ?>
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