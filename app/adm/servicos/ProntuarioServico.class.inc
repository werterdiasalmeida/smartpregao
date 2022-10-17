<?php

class ProntuarioServico {


    public function getProntuarioImpressaoFromId($id, $assinarDigitalmente = false) {
        $mProntuario = new Prontuario($id);
        $prontuario = $mProntuario->getDados();

        $mPessoa = new PessoaFisica();
        $paciente = $mPessoa->getTodosDados($prontuario['pessoa_paciente_id']);

        $mTipo = new TipoProntuario($prontuario['tipo_prontuario_id']);
        $tipo = $mTipo->getDados();

        $mProntuarioCid = new ProntuarioCid();
        $cids = $mProntuarioCid->getCidsPorProntuario($id);

        if($prontuario['agenda_id']){
            $mAgenda = new EstabelecimentoProfissionalAgenda();
            $agenda = $mAgenda->getTodosDados($prontuario['agenda_id']);

            $mEstabelecimentoProfissional = new EstabelecimentoEspecialidadeProfissional($agenda['estabelecimento_especialidade_profissional_id']);
            $estabelecimentoProfissional = $mEstabelecimentoProfissional->getDados();

            $mEspecialidadeProfissional = new EspecialidadeProfissional($estabelecimentoProfissional['especialidade_profissional_id']);
            $especialidadeProfissional = $mEspecialidadeProfissional->getDados();

            $mProfissional = new Profissional();
            $profissional = $mProfissional->getTodosDados($especialidadeProfissional['profissional_id']);

            $mEstabelecimento = new Estabelecimento();
            $estabelecimento = $mEstabelecimento->getTodosDados($estabelecimentoProfissional['estabelecimento_id']);

            $mEndereco = new EnderecoPessoaJuridica();
            $enderecos = $mEndereco->listarPorPessoaJuridica($estabelecimento['pessoajuridica_id']);
            $endereco = array_shift($enderecos);

            $mContato = new ContatoPessoaJuridica();
            $contatos = $mContato->listarPorPessoaJuridica($estabelecimento['pessoajuridica_id']);
            $contato = array_shift($contatos);
        }
        include_once APPRAIZ . "adm/classes/ProntuarioImpressao.class.inc";
        $mProntuarioImpressao = new ProntuarioImpressaoWkHtml($prontuario['descricao'], $estabelecimento, $paciente, $tipo);
        $mProntuarioImpressao->setAgenda($agenda);
        $mProntuarioImpressao->setProfissional($profissional);
        $mProntuarioImpressao->setEndereco($endereco);
        $mProntuarioImpressao->setContato($contato);
        $mProntuarioImpressao->setCids($cids);
        $mProntuarioImpressao->setReceituarioEspecial($prontuario['receituario_especial'] == 't');
        $mProntuarioImpressao->setArqid($prontuario['arqid']);
        $mProntuarioImpressao->setAssinarDigitalmente($assinarDigitalmente);
        return $mProntuarioImpressao;
    }

    public function salvarProntuarioAssinado($pdfAssinadoBase64, $prontuarioId) {
        $fileSimec = new FilesSimec();
        $arqid = $fileSimec->salvarPdfBase64($pdfAssinadoBase64,'Prontuario');
        $mProntuario = new Prontuario($prontuarioId);
        $arqid = $mProntuario->salvarDadosAssinatura($prontuarioId, $arqid);
    }
}