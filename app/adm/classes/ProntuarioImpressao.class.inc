<?php
require_once(APPRAIZ . 'includes/tcpdf/tcpdf.php');

abstract class ProntuarioImpressao
{
    protected $descricao = '';
    protected $agenda = [];
    protected $estabelecimento = [];
    protected $endereco = [];
    protected $contato = [];
    protected $paciente = [];
    protected $tipo = [];
    protected $profissional = [];
    protected $cids = [];
    protected $receituarioEspecial = false;

    protected $arqid;

    protected $pdf;

    protected $assinarDigitalmente;

    public function __construct($descricao, $estabelecimento, $paciente, $tipo)
    {
        $this->descricao = $descricao;
        $this->estabelecimento = $estabelecimento;
        $this->paciente = $paciente;
        $this->tipo = $tipo;
    }

    public function setProfissional($profissional)
    {
        $this->profissional = $profissional;
    }

    public function setEndereco($endereco)
    {
        $this->endereco = $endereco;
    }

    public function setContato($contato)
    {
        $this->contato = $contato;
    }

    public function setCids($cids)
    {
        $this->cids = $cids;
    }

    public function setReceituarioEspecial($receituarioEspecial)
    {
        $this->receituarioEspecial = $receituarioEspecial;
    }

    public function setAgenda($agenda)
    {
        $this->agenda = $agenda;
    }

    protected abstract function setPdf();

    protected abstract function setBody();

    public abstract function generate();

    public function getArqid()
    {
        return $this->arqid;
    }

    public function setArqid($arqid)
    {
        $this->arqid = $arqid;
    }

    public function getAssinarDigitalmente()
    {
        return $this->assinarDigitalmente;
    }

    public function setAssinarDigitalmente($assinarDigitalmente)
    {
        $this->assinarDigitalmente = $assinarDigitalmente;
    }




}

class ProntuarioImpressaoTcpdf extends ProntuarioImpressao
{
    protected function setPdf()
    {
        $this->pdf = new ProntuarioPdf(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

        $this->pdf->setEstabelecimento($this->estabelecimento);
        $this->pdf->setContato($this->contato);
        $this->pdf->setEndereco($this->endereco);

        $this->pdf->setHeaderFont(array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
        $this->pdf->setFooterFont(array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

        $this->pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
        $this->pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
        $this->pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

        $this->pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

        $this->pdf->setImageScale(PDF_IMAGE_SCALE_RATIO);

        $this->pdf->setFontSubsetting(true);
        $this->pdf->SetFont('dejavusans', '', 12, '', true);
        $this->pdf->AddPage();
    }

    protected function setBody()
    {
        $this->setPdf();

        $descricao = isHtml($this->descricao) ? $this->descricao : nl2br(str_replace(' ', '&nbsp;', $this->descricao));
        $municipio = ($this->endereco ? "{$this->endereco['municipio']}, " : '');
        $data = date('j') . " de " . mes_extenso(date('n')) . " de " . date('Y') . ".";
        $profissionalStr = '';
        if ($this->profissional) {
            $registro = ($this->profissional['registro_profissional'] ? "{$this->profissional['registro_profissional']}/{$this->profissional['uf_registro']}" : '');

            $profissionalStr = <<<HTML
                <div style="text-align: center;">
                    ______________________________________________________
                    <br/>
                    {$this->profissional['nome_completo']}
                    <br/>
                    {$registro}
                </div>
HTML;
        }

        $html = <<<HTML
            <br/>
            <br/>
            <strong>Nome do paciente:</strong> {$this->paciente['nome_completo']}
            <br/>
        
            <h3 style="text-align: center;">
                {$this->tipo['descricao']}
            </h3>
            <br/>
            <span style="text-align: justify;">{$descricao}</span>
            <br/>
            <br/>
            <br/>
            {$municipio} {$data}
            <br/>
            <br/>
            {$profissionalStr}
HTML;

        $html = utf8_encode($html);
        $this->pdf->writeHTMLCell(0, 0, '', '', $html, 0, 1, 0, true, '', true);
    }

    public function generate()
    {
        $this->setBody();
        $this->pdf->Output("prontuario_" . date('YmdHis') . ".pdf", 'I');
    }
}

class ProntuarioPdf extends TCPDF
{
    private $estabelecimento;
    private $endereco;
    private $contato;
    private $cids;

    public function setEstabelecimento($estabelecimento)
    {
        $this->estabelecimento = $estabelecimento;
    }

    public function setEndereco($endereco)
    {
        $this->endereco = $endereco;
    }

    public function setContato($contato)
    {
        $this->contato = $contato;
    }

    public function setCids($cids)
    {
        $this->cids = $cids;
    }

    //Page header
    public function Header()
    {
        $estabelecimento = $this->estabelecimento;

        $html = <<<HTML
                <table>
                    <tr>
                        <td>
                        </td>
                        <td style="text-align: right;">
                            <img src="../imagens/logo_assinatura_email.png" style="height: 40px;"/>
                        </td>
                    </tr>
                </table>
HTML;

        if ($estabelecimento) {
            $possuiLogo = false;
            if (!empty($estabelecimento['logo_arquivo_id'])) {
                $mArquivo = new Arquivo($estabelecimento['logo_arquivo_id']);
                $dadosArquivo = $mArquivo->getDados();

                $fileSimec = new FilesSimec();
                $caminhoArquivo = $fileSimec->getCaminhodArquivo($estabelecimento['logo_arquivo_id']);
                $this->Image($caminhoArquivo, 10, 12, 20, '', $dadosArquivo['arqextensao'], '', 'T', false, 300, '', false, false, 0, false, false, false);
                $possuiLogo = true;
            }

            $tdAlinhamento = $possuiLogo ? "<td></td>" : "";
            $alinhamentoNome = $possuiLogo ? "text-align: center;" : "";
            $html = <<<HTML
                <table>
                    <tr>
                        {$tdAlinhamento}
                        <td style="font-size: 16px; {$alinhamentoNome}">
                            <br />
                            <br />
                            {$estabelecimento['nome_fantasia']}
                        </td>
                        <td style="text-align: right;">
                            <img src="../imagens/logo_assinatura_email.png" style="height: 40px;"/>
                        </td>
                    </tr>
                </table>
HTML;
        }

        $this->writeHTMLCell(0, 0, '', 10, utf8_encode($html), 0, 1, 0, true, '', true);
    }

    // Page footer
    public function Footer()
    {
        $endereco = $this->endereco;
        $contato = $this->contato;

        $strEndereco = '';
        $strContato = '';
        if ($endereco) {
            $strEndereco = implode(', ', array($endereco['logradouro'], $endereco['bairro']))
                . ' - '
                . $endereco['estado_cidade']
                . ' - CEP: '
                . $endereco['cep'];
        }

        if ($contato) {
            $strContato = "Tel: {$contato['contato']}";
        }

        $html = <<<HTML
            <table>
                <tr>
                    <td style="text-align: center;" colspan="2">
                        {$this->getAliasNumPage()}/{$this->getAliasNbPages()}
                    </td>
                </tr>
                <tr>
                    <td style="width: 80%;">
                        {$strEndereco}
                    </td>
                    <td style="text-align: right; width: 20%;">
                        {$strContato}
                    </td>
                </tr>
            </table>        
HTML;

        $this->writeHTMLCell(0, 0, '', '', utf8_encode($html), 0, 0, 0, true, '', true);
    }
}

class ProntuarioImpressaoWkHtml extends ProntuarioImpressao
{
    protected function setPdf()
    {
        $header = $this->getHeader();
        $footer = $this->getFooter();

        require_once(APPRAIZ . 'includes/composer/vendor/autoload.php');
        $this->pdf = new \mikehaertl\wkhtmlto\Pdf(array(
            'no-outline',
            'margin-left' => 3,
            'margin-right' => 3,
            'enable-local-file-access',
            'disable-smart-shrinking',
            'header-html' => $header,
            'footer-html' => $footer,
        ));
    }

    protected function setBody()
    {
        $this->setPdf();

        $descricao = isHtml($this->descricao) ? $this->descricao : ($this->descricao);

        $municipio = ($this->endereco ? "{$this->endereco['municipio']}, " : '');
        $data = getDataPorExtenso($this->agenda['data_inicio']) . ".";
        $profissionalStr = '';

        if ($this->profissional) {
            $registro = ($this->profissional['registro_profissional'] ? "{$this->profissional['sigla_tipo_registro']} - {$this->profissional['registro_profissional']} {$this->profissional['uf_registro']}" : '');

            $profissionalStr = <<<HTML
                <div style="padding-top: 125px;">
                    <table style="width: 100%; text-align: center;">
                        <tr>
                            <td>
                                <div style="text-align: center;">
                                    ______________________________________________________
                                    <br/>
                                    {$this->profissional['nome_completo']}
                                    <br/>
                                    {$registro}
                                </div>
                            </td>
                        </tr>
                    </table>
                </div>
                
HTML;

        }

        $cid = "";
        if (is_array($this->cids) && count($this->cids) > 0) {
            $arCodigosCid = [];
            foreach ($this->cids as $cids) {
                $arCodigosCid[] = $cids['codigo_formatado'];
            }
            $cid = "<br/><strong>CID:</strong> " . implode(', ', $arCodigosCid);
        }

        $html = <<<HTML
            <!DOCTYPE html>
            <div style="font-size: 16px;">
                <br/>
                <br/>
                <strong>Nome do paciente:</strong> {$this->paciente['nome_completo']}
                <br/>
                <br/>
                <br/>
                <h3 style="text-align: center;">
                    {$this->tipo['descricao']}
                </h3>
                <br/>
                <span style="text-align: justify; white-space: pre-line;">{$descricao}</span>
                <br/>
                {$cid}
                <br/>
                <br/>
                {$municipio}
                {$data}
                {$profissionalStr}
            </div>
HTML;

        $this->pdf->addPage($html);
    }

    public function generate()
    {
        $this->setBody();
        $this->pdf->send("prontuario_" . date('YmdHis') . ".pdf", true);
    }

    public function getBase64()
    {
        $this->setBody();
        return base64_encode($this->pdf->toString());
    }

    private function getHeader()
    {
        $estabelecimento = $this->estabelecimento;
        if ($estabelecimento['flg_cabecalho_rodape'] == 'N') {
            $html = <<<HTML
                <!DOCTYPE html>
                <table style="width: 100%; height:120px">
                    <tr>
                        <td>
                        </td>
                    </tr>
                </table>
HTML;
        return $html;
        }
        $logoClubvida = APPRAIZ . 'www/imagens/logo_assinatura_email.png';

        $htmlReceituarioEspecial = "";
        if ($this->receituarioEspecial) {
            $htmlReceituarioEspecial = <<<HTML
                    <tr>
                        <td style="text-align: center; font-weight: bolder;" colspan="2">
                            RECEITUÁRIO DE CONTROLE ESPECIAL
                        </td>
                    </tr>
HTML;

        }

        $html = <<<HTML
                <!DOCTYPE html>
                <table style="width: 100%; font-size: 14px;">
                    {$htmlReceituarioEspecial}
                    <tr>
                        <td style="text-align: center;" colspan="2">
                            <img src="{$logoClubvida}" style="height: 30px;"/>
                        </td>
                    </tr>
                </table>
HTML;

        if ($estabelecimento) {
            $fileSimec = new FilesSimec();
            $htmlLogo = '<div style="padding-top: 10px; font-weight: bolder;">' . $estabelecimento['nome_fantasia'] . '</div>';
            $logoEstabelecimento = $fileSimec->getCaminhodArquivo($estabelecimento['logo_arquivo_id']);
            if (!empty($estabelecimento['logo_arquivo_id']) && file_exists($logoEstabelecimento)) {
                $htmlLogo = '<img src="' . $fileSimec->getCaminhodArquivo($estabelecimento['logo_arquivo_id']) . '" style="height: 80px;"/>';
            }

            $html = <<<HTML
                <!DOCTYPE html>
                <table style="width: 100%; font-size: 14px; border-bottom: 2px solid #9a9a9a;">
                    {$htmlReceituarioEspecial}
                    <tr>
                        <td style="vertical-align: middle; text-align: left; padding-top: 15px; padding-left: 10px;">
                            {$htmlLogo}
                        </td>
                        <td style="vertical-align: middle; text-align: right; padding-top: 15px; font-weight: bolder; padding-right: 10px;">
                            {$estabelecimento['nome_fantasia']}
                        </td>
                    </tr>
                </table>
HTML;
        }

        return $html;
    }

    public function getFooter()
    {
        $estabelecimento = $this->estabelecimento;
                
        if ($estabelecimento['flg_cabecalho_rodape'] == 'N') {
            return '<!DOCTYPE html>';
        }
        $endereco = $this->endereco;
        $contato = $this->contato;
        $logo = APPRAIZ . 'www/imagens/logo2.png';

        $strEndereco = '';
        if ($endereco) {
            $strEndereco = '<b>Endereço: </b>'
                . implode(', ', array($endereco['logradouro'], $endereco['bairro']))
                . ' - '
                . $endereco['estado_cidade']
                . ' - CEP: '
                . $endereco['cep'];
        }

        $strContato = '';
        if ($contato) {
            $strContato = "<b>Telefone(s):</b> {$contato['contato']}";
        }

        $htmlReceituarioEspecial = "";
        $marginBottom = $this->getAssinarDigitalmente() ? '60px' : '10px';
        if ($this->receituarioEspecial) {
            $htmlReceituarioEspecial = <<<HTML
                <div style="margin-bottom: {$marginBottom}; page-break-inside: avoid;">
                    <table style="width: 100%; page-break-inside: avoid;">
                        <tr>
                            <td style="width: 48%;">
                                <div style="width: 100%; border: 1px solid; float: left; padding: 10px 5px 20px 5px;">
                                    <div style="text-align: center; font-weight: bolder;">IDENTIFICAÇÃO COMPRADOR</div>
                                    <div style="margin-top: 10px;">RG:</div>
                                    <div style="margin-top: 10px;">ENDEREÇO:</div>
                                    <div style="margin-top: 10px;">CIDADE:</div>
                                    <div style="margin-top: 10px;">TELEFONE:</div>
                                </div>
                            </td>
                            <td style="width: 4%;">
                            </td>
                            <td style="width: 48%;  vertical-align: bottom;">
                                <div style="width: 100%; border: 1px solid; float: right; padding: 10px 5px 20px 5px;">
                                    <div style="text-align: center; font-weight: bolder;">IDENTIFICAÇÃO FORNECEDOR</div>
                                    <div style="margin-top: 10px;">ASSINATURA:</div>
                                    <div style="margin-top: 10px;">DATA:</div>
                                </div>
                            </td>
                        </tr>
                    </table>
                </div>
HTML;
        }

        $html = <<<HTML
            <!DOCTYPE html>
            {$htmlReceituarioEspecial}
            <div style="padding-bottom: 10px;">
                <table style="width: 100%; font-size: 14px; background-color: #19769F; color:#ffffff; padding: 10px">
                <tr>
                    <td style="width: 85%">
                        <b>{$estabelecimento['nome_fantasia']} - {$estabelecimento['cnpj']}</b>
                    </td>
                    <td style="width: 15%" rowspan="2">
                        <img src="{$logo}" style="height: 30px;"/>
                    </td>
                </tr>
                <tr>
                    <td>
                        {$strContato}
                    </td>
                </tr>
                <tr>
                    <td style="font-size: 12px;" colspan="2">
                        {$strEndereco}
                    </td>
                </tr>
            </table>
            </div>
                    
HTML;

        return $html;
    }
}

