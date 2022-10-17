<?php
require_once(APPRAIZ . 'includes/tcpdf/tcpdf.php');

$arrParams = array_merge($_POST, getParam());
$mAgenda = new EstabelecimentoProfissionalAgenda();
$arrDados = $mAgenda->getTodosDados($arrParams['agenda_id']);
extract($arrDados);
$mEstabelecimento = new Estabelecimento();
$arrDadosEstabelecimento = $mEstabelecimento->getTodosDados($arrDados['estabelecimento_id']);

$mEnderecoPessoaJuridica = new EnderecoPessoaJuridica();
$enderecosPJ = $mEnderecoPessoaJuridica->listarPorPessoaJuridica($arrDadosEstabelecimento['pessoajuridica_id'], false, 1);
if($enderecosPJ[0])
{
    $enderecosPJ = $enderecosPJ[0];
}

$mContatoPessoaJuridica = new ContatoPessoaJuridica();
$contatosPJ = $mContatoPessoaJuridica->listarEspecificosPorPessoaJuridica($arrDadosEstabelecimento['pessoajuridica_id']);

$mObservacaoAgenda = new ObservacaoAgenda();
$arrObservacaoAgenda = $mObservacaoAgenda->listarPorAgenda($arrParams['agenda_id']);

$mObservacaoPessoaFisica = new ObservacaoPessoaFisica();
$arrObservacaoPessoaFisica = $mObservacaoPessoaFisica->listarPorPessoa($arrDados['pessoa_paciente_id']);

$observacao = null;
if($arrObservacaoAgenda)
{
    foreach($arrObservacaoAgenda as $obs)
    {
        $observacao .= '<br/>- ' . $obs['observacao'];
    }
}

if($arrObservacaoPessoaFisica)
{
    foreach($arrObservacaoPessoaFisica as $obs)
    {
        $observacao .= '<br/>- ' . $obs['observacao'];
    }
}

//dbg($arrDadosEstabelecimento);
//dbg($arrObservacaoAgenda);
//dbg($arrObservacaoPessoaFisica);
//dbg($arrDados,1);
//dbg($enderecosPJ);
//dbg($contatosPJ,1);

// Extend the TCPDF class to create custom Header and Footer
class ProtocoloExamePDF extends TCPDF
{

    public $clinica;
    public $endereco = null;
    public $estabelecimento = [];
    public $cabecalhoRodape = 'S';
    public $logo = APPRAIZ . 'www/imagens/logo.png';

    public function SetClinica($clinica)
    {
        $this->clinica = $clinica;
    }

    public function GetClinica()
    {
        return $this->clinica;
    }

    public function SetLogo($logo)
    {
        $this->logo = $logo;
    }

    public function GetLogo()
    {
        return $this->logo;
    }

    public function SetCabecalhoRodape($cabecalhoRodape)
    {
        $this->cabecalhoRodape = $cabecalhoRodape;
    }

    public function GetCabecalhoRodape()
    {
        return $this->cabecalhoRodape;
    }

    public function SetEndereco($endereco)
    {
        $this->endereco = $endereco;
    }

    public function GetEndereco()
    {
        return $this->endereco;
    }

    public function SetContato($contato)
    {
        $this->contato = $contato;
    }

    public function GetContato()
    {
        return $this->contato;
    }

    public function SetEstabelecimento($estabelecimento)
    {
        $this->estabelecimento = $estabelecimento;
    }

    public function GetEstabelecimento()
    {
        return $this->estabelecimento;
    }

    //Page header
    public function Header()
    {
        if($this->GetCabecalhoRodape() == 'S')
        {
            $image_file = $this->GetLogo();
            $this->Image($image_file, 8, 6, 25, '', '', '', 'T', false, 300, '', false, false, 0, false, false, false);
            $this->SetFont('dejavusans', 'B', 11, '', true);
            $this->Ln(11);
            $this->Cell(0, 0, $this->GetClinica(), 0, false, 'R', 0, '', 0, false, 'M', 'M');
            $this->Ln(7);
            $this->writeHTML('<div style="width:100%; border-bottom: 2px solid #9a9a9a;" ></div>', true, false, true, false, '');
        }
        
    }

    // Page footer
    public function Footer()
    {   
        $estabelecimento = $this->estabelecimento;
        if ($this->GetCabecalhoRodape() == 'N') {
            return '<!DOCTYPE html>';
        }
        $logo = APPRAIZ . 'www/imagens/logo2.png';

        $strEndereco = '';
        if ($this->GetEndereco()) {
            $strEndereco = '<b>Endereço:</b> ' . $this->GetEndereco();
        }

        $strContato = '';
        if ($this->GetContato()) {
            $strContato = "<b>Telefone(s):</b> " . $this->GetContato();
        }

        $html = <<<HTML
            <div style="padding-bottom: 10px; ">
                <table style="width: 100%; font-size: 14px; background-color: #19769F; color:#ffffff; padding: 3px">
                <tr>
                    <td style="width: 80%;">
                        <b>{$estabelecimento['nome_fantasia']} - {$estabelecimento['cnpj']}</b>
                    </td>
                    <td style="width: 20%;" rowspan="3">
                        <span style="padding: 2px" ></span><br>
                        <img src="{$logo}" height="300px" />
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

    $this->SetFont('dejavusans', '', 11, '', true);
    $this->SetY(-25);
    $this->writeHTML($html, true, false, true, false, '');

    }
}

// create new PDF document
$pdf = new ProtocoloExamePDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

// set dados da clinica
$pdf->SetClinica($arrDadosEstabelecimento['nome_fantasia']);
$pdf->SetEstabelecimento($arrDadosEstabelecimento);

if($arrDadosEstabelecimento['logo_arquivo_id'])
{   
    $file = new FilesSimec();
    $arquivo = $file->getCaminhodArquivo($arrDadosEstabelecimento['logo_arquivo_id']);
    if($arquivo)
    {
        $pdf->SetLogo($arquivo);
    }
}

if($enderecosPJ)
{   
    $endrereco = $enderecosPJ['logradouro'] . ', ' . $enderecosPJ['bairro'] . ' - ' . $enderecosPJ['estado_cidade'] . ' - CEP: ' . $enderecosPJ['cep'];;
    $pdf->SetEndereco(utf8_encode($endrereco));
}

if($contatosPJ)
{   
    $contato = $contatosPJ['contato_telefone'];
    $pdf->SetContato(utf8_encode($contato));
}

// set document information
$pdf->SetCreator(PDF_CREATOR);
$pdf->SetAuthor('Club Vida');
$pdf->SetTitle('Protocolo');
$pdf->SetSubject('Protocolo');
$pdf->SetKeywords('clubvida, exame, protocolo');

// set default header data
$pdf->SetHeaderData(PDF_HEADER_LOGO, PDF_HEADER_LOGO_WIDTH, PDF_HEADER_TITLE, PDF_HEADER_STRING);

$pdf->SetCabecalhoRodape($arrDadosEstabelecimento['flg_cabecalho_rodape']);

// set header and footer fonts
$pdf->setHeaderFont(array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
$pdf->setFooterFont(array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

// set default monospaced font
$pdf->SetDefaultMonospacedFont(PDF_FONT_MONOSPACED);

// set margins
$pdf->SetMargins(5, PDF_MARGIN_TOP, 5);
$pdf->SetHeaderMargin(PDF_MARGIN_HEADER);
$pdf->SetFooterMargin(PDF_MARGIN_FOOTER);

// set auto page breaks
$pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

// set image scale factor
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO);

// set some language-dependent strings (optional)
if (@file_exists(dirname(__FILE__) . '/lang/eng.php')) {
    require_once(dirname(__FILE__) . '/lang/eng.php');
    $pdf->setLanguageArray($l);
}

// ---------------------------------------------------------

// set font
$pdf->SetFont('helvetica', '', 10);

// add a page
$pdf->AddPage();

$paciente = utf8_encode($paciente);
if($convenio)
{
    $convenio = utf8_encode($convenio) . ($numero_convenio_plano ? ' (' . $numero_convenio_plano . ')' : '');
}else{
    $convenio = 'Não';
}
$profissional = utf8_encode($profissional);
$procedimento = utf8_encode($procedimento);
$tipo_consulta = trim(ucwords(utf8_encode($tipo_consulta)));
$is_consulta = false;
if(in_array($tipo_consulta,['Consulta','Consulta Remota','Primeira Consulta'])){
    $is_consulta = true;
}
$sala = utf8_encode($sala);
$arrDadosEstabelecimento['nome_fantasia'] = utf8_encode($arrDadosEstabelecimento['nome_fantasia']);
$data_hora_atend = formata_timestamp($inicio);
$duracao = '';

$especialidade = utf8_encode($especialidade);

if($inicio && $fim)
{
    $to_time = strtotime($inicio);
    $from_time = strtotime($fim);
    $duracao = round(abs($to_time - $from_time) / 60,2). " minutos";
    if($duracao > 60)
    {
        $horas = floor($duracao / 60);
        $minutos = ($duracao % 60);
        if($minutos > 0)
        {
            $duracao = $horas . 'h';
        }else{
            $duracao = $horas . 'h' . $minutos . 'min';
        }
        
    }
}

$observacao = $observacao != null ? utf8_encode($observacao) : 'Nenhuma';

$server_url = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] === 'on' ? "https" : "http") . "://$_SERVER[HTTP_HOST]";

if($is_consulta){
    $data = explode(" ",$data_hora_atend)[0];
    $hora = explode(" ",$data_hora_atend)[1];
    $html = <<<HTML
    <style>
        table {
            border: none;
            width:100%;
        }
    </style>
    <br/><br/>
    <table class="table" border="0" >
     <tr>
      <td width="100%" >
      <br/>
      <div style="text-align:center;width:100%">
      <span style="font-size:16px;font-weight:bold">Comprovante de Agendamento de {$tipo_consulta}</span><br/>
        <br/>
      </div> 
      <br/><br/>
        <table width="92%" border="0" style="border:none">
        <tr>
            <td colspan="2" ><strong>Nome:</strong> {$paciente}</td>
        </tr>
        <tr>
            <td width="50%" ><br/><br/><strong>Data:</strong> {$data}</td>
            <td><br/><br/><strong>Horário:</strong> {$hora}</td>
        </tr>
        <tr>
            <td><br/><br/><strong>Procedimento:</strong> {$procedimento}</td>
            <td><br/><br/><strong>Médico:</strong> {$profissional}</td>
        </tr>
        <tr>
            <td colspan="2" style="text-align:center" ><br/><br/><strong>Orientações:</strong></td>
        </tr>
        <tr>
            <td colspan="2" style="text-align:justify" ><br/><br/>- É indispensável a apresentação deste protocolo e documento de identificação com foto do paciente para a entrega e seu resultado.</td>
        </tr>
        <tr>
            <td colspan="2" style="text-align:justify" >- A entrega do resultado a terceiros só será realizada mediante apresentação do termo de autorizaão de entrega de exames a terceiros (entregue no dia da realização do exame).</td>
        </tr>
        <tr>
            <td colspan="2" style="text-align:justify" >- Este protocolo, bem como os dados de código e senha de acesso a resultados online, é de inteira responsabilidade do seu portador.</td>
        </tr>
        </table>
        <br/><br/>
      </td>
    </tr>
    </table>
HTML;
}else{
    $html = <<<HTML
    <style>
        table {
            border: none;
            width:100%;
        }
    </style>
    <br/><br/>
    <table class="table" border="0" >
    <tr>
    <td width="70%" >
    <br/>
    <div style="text-align:center;width:100%">
    <span style="font-size:16px;font-weight:bold">Protocolo de {$tipo_consulta}</span><br/>
        {$procedimento}<br/>
    </div> 
    <br/><br/>
        <table width="92%" border="0" style="border:none">
        <tr>
            <td colspan="2" ><strong>Paciente:</strong> {$paciente}</td>
        </tr>
        <tr>
            <td colspan="2" ><strong>Convênio:</strong> {$convenio}</td>
        </tr>
        <tr>
            <td colspan="2" ><strong>Médico:</strong> {$profissional}</td>
        </tr>
        <tr>
            <td width="60%"><strong>Procedimento:</strong> {$procedimento}</td>
            <td><strong>Especialidade:</strong> {$especialidade}</td>
        </tr>
        <tr>
            <td><strong>Tipo:</strong> {$tipo_consulta}</td>
            <td><strong>Sala:</strong> {$sala}</td>
        </tr>
        <tr>
            <td><strong>Data e Hora Atend.:</strong> {$data_hora_atend}</td>
            <td><strong>Duração:</strong> {$duracao}</td>
        </tr>
        <tr>
            <td colspan="2" style="text-align:justify" ><strong>Observação:</strong> {$observacao}</td>
        </tr>
        <tr>
            <td colspan="2" style="text-align:justify" ><br/><br/>- É indispensável a apresentação deste protocolo e documento de identificação com foto do paciente para a entrega e seu resultado.</td>
        </tr>
        <tr>
            <td colspan="2" style="text-align:justify" >- A entrega do resultado a terceiros só será realizada mediante apresentação do termo de autorizaão de entrega de exames a terceiros (entregue no dia da realização do exame).</td>
        </tr>
        <tr>
            <td colspan="2" style="text-align:justify" >- Este protocolo, bem como os dados de código e senha de acesso a resultados online, é de inteira responsabilidade do seu portador.</td>
        </tr>
        </table>
        <br/><br/>
    </td>
    <td align="justify" width="30%" ><br/><br/>
        Resultado pela Internet<br/>
        Entrar no endereço<br/>
        <a href="{$server_url}">{$server_url}</a><br/>
        Informar o código e a senha<br/>
        <strong>Código</strong><br/>
        <strong>Senha</strong><br/>
        <br/><br/>
        <br/><br/>
        <br/><br/>
        <br/><br/>
        <br/><br/>
        <br/>
        <span style="font-size:10px">Escaneie o QRCode acima com o celular</span>
    </td>
    </tr>
    </table>
HTML;
}

// output the HTML content
$pdf->writeHTML($html, true, false, true, false, '');

if($is_consulta === false)
{
    $pdf->write2DBarcode($server_url, 'QRCODE,Q', 148, 64, 45, 45, $style, 'N');
}

// ---------------------------------------------------------

//Close and output PDF document
$pdf->Output('protocolo_' . date('d_m_Y') . '.pdf', 'I');