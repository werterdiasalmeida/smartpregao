<?php
$urlConfirmacaoAtendimento = "{$baseUrl}/confirmacao_agendamento_atendimento.php?p={$hash_agenda_id}";

$dataAtual = new DateTime();
$inicioLdJson = new DateTime($inicio);
$fimLdJson = new DateTime($fim);

$dataAtualLdJson = $dataAtual->format(DateTime::ISO8601);
$inicioLdJson = $inicioLdJson->format(DateTime::ISO8601);
$fimLdJson = $fimLdJson->format(DateTime::ISO8601);
$imageLdJson = $baseUrl . montaUrlImgFoto($foto_arquivo_id_profissional, $sexo_profissional, '100', '100');
?>
<?php if (isset($endereco_estabelecimento)): ?>
<script type="application/ld+json">
{
   "@context":"http://schema.org",
   "@type":"EventReservation",
   "reservationNumber":"<?= $hash_agenda_id ?>",
   "reservationStatus":"http://schema.org/Confirmed",
   "underName":{
      "@type":"Person",
      "name":"<?= $paciente; ?>"
   },
   "modifiedTime": "<?= $dataAtualLdJson ?>",
   "modifyReservationUrl": "<?= $urlConfirmacaoAtendimento ?>",
   "reservationFor":{
      "@type":"Event",
      "name":"<?= $tipo_consulta . ' - ' . $profissional ?>",
      "startDate": "<?= $inicioLdJson ?>",
      "endDate": "<?= $fimLdJson ?>",
      "performer":{
         "@type":"Person",
         "name":"<?= $profissional ?>",
         "image":"<?= $imageLdJson ?>"
      },
      "location":{
         "@type":"Place",
         "name":"<?= $nome_fantasia ?>",
         "address":{
            "@type":"PostalAddress",
            "streetAddress":"<?= $endereco_estabelecimento['descricao'] ?>",
            "addressLocality":"<?= $endereco_estabelecimento['bairro'] ?>",
            "postalCode":"<?= $endereco_estabelecimento['cep'] ?>",
            "addressCountry":"<?= $endereco_estabelecimento['uf_sigla'] ?>",
            "addressRegion": "BR"
         }
      }
   }
}
</script>
<?php endif; ?>
<div style="text-align: center;">
    <h2 class="font-blue" style="font-weight: bold; color: #1BBC9B;">Atendimento Confirmado!</h2>
</div>
<div style="font-size: 14px; color: #666;">
    <p style="margin: 15px 0 10px 0">
        Olá, <b><?= $paciente; ?></b>.
    </p>
    <p>
        Seu atendimento foi confirmado. Estes são os dados do atendimento:
    </p>
    <p style="margin: 15px 0 15px 0">
        Estabelecimento: <b><?= $nome_fantasia ?></b> <br>
        <?php if (isset($url_atendimento_remoto)): ?>
            Atendimento virtual: <a href="<?= $url_atendimento_remoto ?>" target="_blank">
                <b><?= $url_atendimento_remoto ?></b>
            </a> <br>
        <?php elseif (isset($endereco_estabelecimento)): ?>
            Endereço: <b><?= $endereco_estabelecimento['descricao'] . ". " . $endereco_estabelecimento['bairro']
                . ' - ' . $endereco_estabelecimento['estado_cidade'] ?></b> <br>
        <?php endif; ?>
        Profissional: <b><?= $profissional ?></b> <br>
        Tipo de atendimento: <b><?= $tipo_consulta ?></b> <br>
        Especialidade: <b><?= $especialidade ?></b> <br>
        Convênio: <b><?= $convenio ?></b> <br>
        Data: <b><?= $data_inicio ?></b> <br>
        Horário: De <b><?= $hora_inicio ?></b> até <b><?= $hora_fim ?></b> <br>
    </p>
</div>