<?php

class GuiaImpressao
{
    protected $guia = [];
    protected $estabelecimento = [];
    protected $tipoGuia = null;

    protected $pdf;

    public function __construct($guia, $estabelecimento = [], $tipoGuia)
    {
        $this->guia = $guia;
        $this->estabelecimento = $estabelecimento;
        $this->tipoGuia = $tipoGuia;
    }

    protected function setPdf()
    {
        $header = $this->getHeader();
        $footer = $this->getFooter();

        require_once(APPRAIZ . 'includes/composer/vendor/autoload.php');
        $this->pdf = new \mikehaertl\wkhtmlto\Pdf(array(
            'no-outline',
            'margin-top' => 8,
            'margin-bottom' => 8,
            'margin-right' => 5,
            'margin-left' => 5,
            'disable-smart-shrinking',
            'header-html' => $header,
            'footer-html' => $footer,
            'orientation' => 'landscape',
            'footer-center' => '[page]/[toPage]',
            'footer-font-size' => 6,
        ));
    }

    protected function getLogo()
    {
        $logo = APPRAIZ . 'www/imagens/logo_assinatura_email.png';
        $estabelecimento = $this->estabelecimento;

        if ($estabelecimento && !empty($estabelecimento['logo_arquivo_id'])) {
            $fileSimec = new FilesSimec();
            $logo = $fileSimec->getCaminhodArquivo($estabelecimento['logo_arquivo_id']);
        }

        return $logo;
    }

    protected function setBody()
    {
        $this->setPdf();

        switch ($this->tipoGuia) {
            case TipoGuia::TIPO_GUIA_EXAME :
                $htmlGuia = $this->guiaExame();
                break;
            default :
                $htmlGuia = $this->guiaConsulta();
                break;
        }

        $html = <<<HTML
            <!DOCTYPE html>
            <style type="text/css">
                div.corpo {
                    border: 1px solid #000000;
                    padding: 5px;
                    font-size: 8px;
                }
                
                .tabela, .tabela td {
                    border-collapse: collapse;
                    border-spacing: 0;
                    width: 100%;
                    padding: 0;
                }
                
                .info {
                    border: 1px solid #000;
                    padding: 2px;
                    margin-bottom:3px;
                }
                
                table.tabela > tbody > tr > td:not(:last-child) > div.info {
                    margin-right: 3px;
                }
                
                .info label{
                    display: block;
                    font-size: 7px;
                    font-weight: bold;
                }
                
                .info > span {
                    margin-left: 8px;
                }
                
                .titulo {
                    background-color: #C0C0C0;
                    font-weight: bold;
                    padding: 2px;
                    font-size: 8px;
                    margin-bottom: 3px;
                }
                
                .observacao {
                    min-height: 20px;
                    background-color: #C0C0C0;
                }
                
                .assinatura {
                    height: 18px;
                }
                
                .cabecalho {
                    width: 100%; 
                    font-size: 11px;
                }
                
                .tipo-guia {
                    font-size: 13px; 
                    vertical-align: middle; 
                    text-align: center; 
                    text-transform: uppercase; 
                    font-weight: bold;
                }
                
                .numero-guia {
                    font-size: 15px; 
                    font-weight: bold;
                }
                
                .logo {
                    height: 30px;
                }
            </style>
            
            <div class="corpo">
                {$htmlGuia}
            </div>
HTML;

        $this->pdf->addPage($html);
    }

    protected function guiaConsulta ()
    {
        $guia = $this->guia;

        $html = <<<HTML
            <table class="cabecalho">
                <tbody>
                    <tr>
                        <td style="text-align: left;  width: 10%;">
                            <img src="{$this->getLogo()}" class="logo"/>
                        </td>
                        <td class="tipo-guia">
                            Guia de consulta
                        </td>
                        <td style="text-align: right;  width: 18%;">
                            <label style="font-size: 10px;">2 - Nº Guia no Prestador</label>
                            <span class="numero-guia">{$guia['numero_guia_prestador']}</span>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 30%;">
                            <div class="info">
                                <label>1 - Registro ANS</label>
                                <span>{$guia['registro_ans']}</span>
                            </div>
                        </td>
                        <td>
                            <div class="info">
                                <label>3 - Número da Guia Atribuído pela Operadora</label>
                                <span>{$guia['numero_guia_operadora']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <div class="titulo">
                Dados do Beneficiário
            </div>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 50%">
                            <div class="info">
                                <label>4 - Número da Carteira</label>
                                <span>{$guia['numero_carteira']}</span>
                            </div>
                        </td>
                        <td style="width: 30%">
                            <div class="info">
                                <label>5 - Validade da Carteira</label>
                                <span>{$guia['validade_carteira']}</span>
                            </div>
                        </td>
                        <td style="width: 20%">
                            <div class="info">
                                <label>6 - Atendimento a RN</label>
                                <span>{$guia['atendimento_rn']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 60%">
                            <div class="info">
                                <label>7 - Nome</label>
                                <span>{$guia['nome_beneficiario']}</span>
                            </div>
                        </td>
                        <td style="width: 30%">
                            <div class="info">
                                <label>8 - Cartão Nacional de Saúde</label>
                                <span>{$guia['cartao_nacional_saude']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <div class="titulo">
                Dados do Contratado
            </div>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 20%">
                            <div class="info">
                                <label>9 - Código na Operadora</label>
                                <span>{$guia['codigo_operadora']}</span>
                            </div>
                        </td>
                        <td style="width: 60%">
                            <div class="info">
                                <label>10 - Nome do Contratado</label>
                                <span>{$guia['nome_contratado']}</span>
                            </div>
                        </td>
                        <td style="width: 20%">
                            <div class="info">
                                <label>11 - Código CNES</label>
                                <span>{$guia['cnes']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 43%">
                            <div class="info">
                                <label>12 - Nome d Profissional Executante</label>
                                <span>{$guia['nome_profissional']}</span>
                            </div>
                        </td>
                        <td style="width: 17%">
                            <div class="info">
                                <label>13 - Conselho Profissional</label>
                                <span>{$guia['conselho_profissional']}</span>
                            </div>
                        </td>
                        <td style="width: 20%">
                            <div class="info">
                                <label>14 - Número do Conselho</label>
                                <span>{$guia['numero_conselho_profissional']}</span>
                            </div>
                        </td>
                        <td style="width: 10%">
                            <div class="info">
                                <label>15 - UF</label>
                                <span>{$guia['uf_conselho_profissional']}</span>
                            </div>
                        </td>
                        <td style="width: 10%">
                            <div class="info">
                                <label>16 - Código CBO</label>
                                <span>{$guia['cbos']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <div class="titulo">
                Dados do Atendimento / Procedimento Realizado
            </div>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 30%">
                            <div class="info">
                                <label>17 - Indicação de Acidente</label>
                                <span>{$guia['indicacao_acidente']}</span>
                            </div>
                        </td>
                        <td style="width: 40%">
                            <div class="info">
                                <label>18 - Data do Atendimento</label>
                                <span>{$guia['data_atendimento']}</span>
                            </div>
                        </td>
                        <td style="width: 30%">
                            <div class="info">
                                <label>19 - Tipo de Consulta</label>
                                <span>{$guia['tipo_consulta']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 20%">
                            <div class="info">
                                <label>20 - Tabela</label>
                                <span>{$guia['codigo_tabela']}</span>
                            </div>
                        </td>
                        <td style="width: 40%">
                            <div class="info">
                                <label>21 - Código do Procedimento</label>
                                <span>{$guia['codigo_procedimento']}</span>
                            </div>
                        </td>
                        <td style="width: 40%">
                            <div class="info">
                                <label>22 - Valor do Procedimento</label>
                                <span>{$guia['valor_procedimento']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 100%">
                            <div class="info observacao">
                                <label>23 - Observação / Justificativa</label>
                                <span>{$guia['observacao']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 50%">
                            <div class="info assinatura">
                                <label>24 - Assinatura do Profissional Executante</label>
                            </div>
                        </td>
                        <td style="width: 50%">
                            <div class="info assinatura">
                                <label>25 - Assinatura do Beneficiário ou Responsável</label>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
HTML;

        return $html;
    }

    protected function guiaExame ()
    {
        $guia = $this->guia;

        $tabelaProcedimentosSolicitados = function () use ($guia) {
            $html = '';
            foreach($guia['solicitado'] as $k => $solicitado) {
                $seq = $k + 1;
                $html .= "<tr>
                            <td style='width: 10%;'>
                                {$seq} - {$solicitado['codigo_tabela']}
                            </td>
                            <td style='width: 20%;'>
                                {$solicitado['codigo_procedimento']}
                            </td>
                            <td style='width: 50%;'>
                                {$solicitado['descricao_procedimento']}
                            </td>
                            <td style='width: 10%;'>
                                {$solicitado['qtd_solicitada']}
                            </td>
                            <td style='width: 10%;'>
                                {$solicitado['qtd_autorizada']}
                            </td>
                        </tr>";
            }

            return $html;
        };

        $tabelaProcedimentosExecutados = function () use ($guia) {
            $html = '';
            foreach($guia['executado'] as $k => $executado) {
                $seq = $k + 1;
                $html .= "<tr>
                            <td style='width: 8%;'>
                                {$seq} - {$executado['data_execucao']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['hora_inicial_execucao']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['hora_final_execucao']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['codigo_tabela_execucao']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['codigo_procedimento_execucao']}
                            </td>
                            <td style='width: 22%;'>
                                {$executado['descricao_execucao']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['qtd_execucao']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['via']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['tec']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['fator_red_acresc']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['valor_unitario']}
                            </td>
                            <td style='width: 5%;'>
                                {$executado['valor_total']}
                            </td>
                          </tr>";
            }

            return $html;
        };

        $tabelaProfissionais = function () use ($guia) {
            $html = '';
            foreach($guia['profissional_executante'] as $k => $profissionalExecutante) {
                $seq = $k + 1;
                $html .= "<tr>
                            <td style='width: 8%;'>
                                {$seq}
                            </td>
                            <td style='width: 8%;'>
                                {$profissionalExecutante['grau_part']}
                            </td>
                            <td style='width: 15%;'>
                                {$profissionalExecutante['codigo_operadora']}
                            </td>
                            <td style='width: 36%;'>
                                {$profissionalExecutante['nome_profissional']}
                            </td>
                            <td style='width: 10%;'>
                                {$profissionalExecutante['conselho_profissional']}
                            </td>
                            <td style='width: 10%;'>
                                {$profissionalExecutante['numero_conselho_profissional']}
                            </td>
                            <td style='width: 5%;'>
                                {$profissionalExecutante['uf_conselho_profissional']}
                            </td>
                            <td style='width: 10%;'>
                                {$profissionalExecutante['cbos']}
                            </td>
                          </tr>";
            }

            return $html;
        };

        $tabelaDatas = function () use ($guia) {
            $html = '<tr>';
            foreach($guia['data_realizacao_procedimentos'] as $k => $dataRealizacao) {
                if($k > 0 && $k % 5 === 0) {
                    $html .= '</tr><tr>';
                }

                $seq = $k + 1;
                $html .= " <td style='width: 3%;'>
                                {$seq} - {$dataRealizacao['data_realizacao_procedimentos']}
                           </td>
                           <td style='width: 10%;'>
                                <div class='assinatura' style='border-bottom: 1px solid #000; margin-right: 5px;'>
                                </div>
                           </td>";
            }
            $html .= '</tr>';

            return $html;
        };

        $html = <<<HTML
            <table class="cabecalho">
                <tbody>
                    <tr>
                        <td style="text-align: left;  width: 10%;">
                            <img src="{$this->getLogo()}" class="logo"/>
                        </td>
                        <td class="tipo-guia">
                            Guia de serviço profissional / Serviço auxiliar de<br/>diagnóstico e terapia - SP/SADT
                        </td>
                        <td style="text-align: right;  width: 18%;">
                            <label style="font-size: 10px;">2 - Nº Guia no Prestador</label>
                            <span class="numero-guia">{$guia['numero_guia_prestador']}</span>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 30%;">
                            <div class="info">
                                <label>1 - Registro ANS</label>
                                <span>{$guia['registro_ans']}</span>
                            </div>
                        </td>
                        <td>
                            <div class="info">
                                <label>3 - Número da Guia Principal</label>
                                <span>{$guia['numero_guia_principal']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 20%;">
                            <div class="info">
                                <label>4 - Data da Autorização</label>
                                <span>{$guia['data_autorizacao']}</span>
                            </div>
                        </td>
                        <td style="width: 30%;">
                            <div class="info">
                                <label>5 - Senha</label>
                                <span>{$guia['senha']}</span>
                            </div>
                        </td>
                        <td style="width: 20%;">
                            <div class="info">
                                <label>6 - Data de Validade da Senha</label>
                                <span>{$guia['data_validade_senha']}</span>
                            </div>
                        </td>
                        <td>
                            <div class="info">
                                <label>7 - Número da Guia Atribuído pela Operadora</label>
                                <span>{$guia['numero_guia_operadora']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <div class="titulo">
                Dados do Beneficiário
            </div>

            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 14%">
                            <div class="info">
                                <label>8 - Número da Carteira</label>
                                <span>{$guia['numero_carteira']}</span>
                            </div>
                        </td>
                        <td style="width: 15%">
                            <div class="info">
                                <label>9 - Validade da Carteira</label>
                                <span>{$guia['validade_carteira']}</span>
                            </div>
                        </td>
                        <td style="width: 39%">
                            <div class="info">
                                <label>10 - Nome</label>
                                <span>{$guia['nome_beneficiario']}</span>
                            </div>
                        </td>
                        <td style="width: 18%">
                            <div class="info">
                                <label>11 - Cartão Nacional de Saúde</label>
                                <span>{$guia['cartao_nacional_saude']}</span>
                            </div>
                        </td>
                        <td style="width: 14%">
                            <div class="info">
                                <label>12 - Atendimento a RN</label>
                                <span>{$guia['atendimento_rn']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>

            <div class="titulo">
                Dados do Solicitante
            </div>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 30%">
                            <div class="info">
                                <label>13 - Código na Operadora</label>
                                <span>{$guia['codigo_operadora']}</span>
                            </div>
                        </td>
                        <td>
                            <div class="info">
                                <label>14 - Nome do Contratado</label>
                                <span>{$guia['nome_contratado']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 35%">
                            <div class="info">
                                <label>15 - Nome do Profissional Solicitante</label>
                                <span>{$guia['nome_profissional']}</span>
                            </div>
                        </td>
                        <td style="width: 9%">
                            <div class="info">
                                <label>16 - Conselho</label>
                                <span>{$guia['conselho_profissional']}</span>
                            </div>
                        </td>
                        <td style="width: 15%">
                            <div class="info">
                                <label>17 - Número no Conselho</label>
                                <span>{$guia['numero_conselho_profissional']}</span>
                            </div>
                        </td>
                        <td style="width: 6%">
                            <div class="info">
                                <label>18 - UF</label>
                                <span>{$guia['uf_conselho_profissional']}</span>
                            </div>
                        </td>
                        <td style="width: 10%">
                            <div class="info">
                                <label>19 - Código CBO</label>
                                <span>{$guia['cbos']}</span>
                            </div>
                        </td>
                        <td style="width: 25%">
                            <div class="info assinatura">
                                <label>20 - Assinatura do Profissional Solicitante</label>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>

            <div class="titulo">
                Dados da Solicitação / Procedimentos ou Itens Assistenciais Solicitados
            </div>

             <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 18%">
                            <div class="info">
                                <label>21 - Caráter do Atendimento</label>
                                <span>{$guia['carater_atendimento']}</span>
                            </div>
                        </td>
                        <td style="width: 15%">
                            <div class="info">
                                <label>22 - Data da Solicitação</label>
                                <span>{$guia['data_solicitacao']}</span>
                            </div>
                        </td>
                        <td>
                            <div class="info">
                                <label>23 - Indicação Clínica</label>
                                <span>{$guia['indicacao_clinica']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
                
            <div class="info">
                <table class="tabela">
                    <tbody>
                        <tr>
                            <td style="width: 10%;">
                                <label>24 - Tabela</label>
                            </td>
                            <td style="width: 20%;">
                                <label>25 - Código do Procedimento ou Item Assistencial</label>
                            </td>
                            <td style="width: 50%;">
                                <label>26 - Descrição</label>
                            </td>
                            <td style="width: 10%;">
                                <label>27 - Qtde. Solic.</label>
                            </td>
                            <td style="width: 10%;">
                                <label>28 - Qtde. Aut</label>
                            </td>
                        </tr>
                        {$tabelaProcedimentosSolicitados()}
                    </tbody>
                </table>
            </div>
            
            <div class="titulo">
                Dados do Contratado Executante
            </div>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 20%">
                            <div class="info">
                                <label>29 - Código na Operadora</label>
                                <span>{$guia['codigo_operadora_executante']}</span>
                            </div>
                        </td>
                        <td style="width: 50%">
                            <div class="info">
                                <label>30 - Nome do Contratado</label>
                                <span>{$guia['nome_contratado_executante']}</span>
                            </div>
                        </td>
                        <td style="width: 20%">
                            <div class="info">
                                <label>31 - Código CNES</label>
                                <span>{$guia['cnes']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <div class="titulo">
                Dados do Atendimento
            </div>

            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 20%">
                            <div class="info">
                                <label>32 - Tipo de Atendimento</label>
                                <span>{$guia['tipo_atendimento']}</span>
                            </div>
                        </td>
                        <td style="width: 20%">
                            <div class="info">
                                <label>33 - Indicação de Acidente</label>
                                <span>{$guia['indicacao_acidente']}</span>
                            </div>
                        </td>
                        <td style="width: 20%">
                            <div class="info">
                                <label>34 - Tipo de Consulta</label>
                                <span>{$guia['tipo_consulta']}</span>
                            </div>
                        </td>
                        <td style="width: 20%">
                            <div class="info">
                                <label>35 - Motivo de encerramento</label>
                                <span>{$guia['motivo_encerramento']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>

            <div class="titulo">
                Dados da Execução / Procedimentos e Exames Realizados
            </div>

            <div class="info">
                <table class="tabela">
                    <tbody>
                        <tr>
                            <td style="width: 8%;">
                                <label>36 - Data</label>
                            </td>
                            <td style="width: 5%;">
                                <label>37 - Hora Inicial</label>
                            </td>
                            <td style="width: 5%;">
                                <label>38 - Hora Final</label>
                            </td>
                            <td style="width: 5%;">
                                <label>39 - Tabela</label>
                            </td>
                            <td style="width: 5%;">
                                <label>40 - Código do Procedimento</label>
                            </td>
                            <td style="width: 22%;">
                                <label>41 - Descrição</label>
                            </td>
                            <td style="width: 5%;">
                                <label>42 - Qtde.</label>
                            </td>
                            <td style="width: 5%;">
                                <label>43 - Via</label>
                            </td>
                            <td style="width: 5%;">
                                <label>44 - Tec.</label>
                            </td>
                            <td style="width: 5%;">
                                <label>45 - Fator Red.Acresc.</label>
                            </td>
                            <td style="width: 5%;">
                                <label>46 - Valor Unitário (R$)</label>
                            </td>
                            <td style="width: 5%;">
                                <label>47 - Valor Total (R$)</label>
                            </td>
                        </tr>
                        {$tabelaProcedimentosExecutados()}
                    </tbody>
                </table>
            </div>

            <div class="titulo">
                Identificação do(s) Profissional(is) Executante(s)
            </div>
            
            <div class="info">
                <table class="tabela">
                    <tbody>
                        <tr>
                            <td style="width: 8%;">
                                <label>48 - Seq. Ref</label>
                            </td>
                            <td style="width: 8%;">
                                <label>49 - Grau Part.</label>
                            </td>
                            <td style="width: 15%;">
                                <label>50 - Código na Operadora/CPF</label>
                            </td>
                            <td style="width: 36%;">
                                <label>51 - Nome do Profissional</label>
                            </td>
                            <td style="width: 10%;">
                                <label>52 - Conselho Profissonal</label>
                            </td>
                            <td style="width: 10%;">
                                <label>53 - Número do Conselho</label>
                            </td>
                            <td style="width: 5%;">
                                <label>54 - UF</label>
                            </td>
                            <td style="width: 10%;">
                                <label>55 - Código CBO</label>
                            </td>
                        </tr>
                        
                        {$tabelaProfissionais()}
                        
                    </tbody>
                </table>
            </div>
            
            <div class="info">
                <table class="tabela" style="width: 50%;">
                    <tbody>
                        <tr>
                            <td style="width: 10%; padding: 0;">
                                <label>56 - Data de Realização de Procedimentos em Série</label>
                            </td>
                            <td style="width: 10%; padding: 0;">
                                <label>57 - Assinatura do Beneficiário ou Responsável</label>
                            </td>
                        </tr>
                    </tbody>
                </table>
                <table class="tabela">
                    <tbody>
                        {$tabelaDatas()}
                    </tbody>
                </table>
            </div>

            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 100%">
                            <div class="info observacao">
                                <label>58 - Observação / Justificativa</label>
                                <span>{$guia['observacao']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>

            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 14%;">
                            <div class="info valor">
                                <label>59 - Total de Procedimentos (R$)</label>
                                <span>{$guia['total_procedimentos']}</span>
                            </div>
                        </td>
                        <td style="width: 14%;">
                            <div class="info valor">
                                <label>60 - Total de Taxas e Aluguéis (R$)</label>
                                <span>{$guia['total_taxas_alugueis']}</span>
                            </div>
                        </td>
                        <td style="width: 14%;">
                            <div class="info valor">
                                <label>61 - Total de Materiais (R$)</label>
                                <span>{$guia['total_materiais']}</span>
                            </div>
                        </td>
                        <td style="width: 14%;">
                            <div class="info valor">
                                <label>62 - Total de OPME (R$)</label>
                                <span>{$guia['total_opme']}</span>
                            </div>
                        </td>
                        <td style="width: 14%;">
                            <div class="info valor">
                                <label>63 - Total de Medicamentos (R$)</label>
                                <span>{$guia['total_medicamentos']}</span>
                            </div>
                        </td>
                        <td style="width: 14%;">
                            <div class="info valor">
                                <label>64 - Total de Gases Medicinais (R$)</label>
                                <span>{$guia['total_gases_medicinais']}</span>
                            </div>
                        </td>
                        <td style="width: 14%;">
                            <div class="info valor">
                                <label>65 - Total Geral (R$)</label>
                                <span>{$guia['total_geral']}</span>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
            
            <table class="tabela">
                <tbody>
                    <tr>
                        <td style="width: 33%;">
                            <div class="info assinatura">
                                <label>66 - Assinatura do Repsonsável pela Autorização</label>
                            </div>
                        </td>
                        <td style="width: 33%;">
                            <div class="info assinatura">
                                <label>67 - Assinatura do Beneficiário ou Responsável</label>
                            </div>
                        </td>
                        <td style="width: 33%;">
                            <div class="info assinatura">
                                <label>68 - Assinatura do Contratado</label>
                            </div>
                        </td>
                    </tr>
                </tbody>
            </table>
HTML;

        return $html;
    }

    public function generate()
    {
        $this->setBody();
        $numeroGuia = str_replace('/', '_', $this->guia['numero_guia_prestador']);
        $this->pdf->send("guia_{$numeroGuia}.pdf", true);
    }

    private function getHeader()
    {
        return '';
    }

    public function getFooter()
    {
        return '';
    }
}

