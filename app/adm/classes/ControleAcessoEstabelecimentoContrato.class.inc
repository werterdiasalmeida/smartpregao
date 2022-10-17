<?php

class ControleAcessoEstabelecimentoContrato
{
    public function isPossuiAcessoTela()
    {
        $possuiAcesso = true;

        $params = getParam();

        $mEstabelecimento = new Estabelecimento();

        $estabelecimento = null;
        if (isset($params['id'])) {
            $estabelecimento = $mEstabelecimento->carregarPorId($params['id'])->getDados();
        }

        if ($estabelecimento) {
            $mEstabelecimentoContrato = new EstabelecimentoContrato();
            $possuiAcesso = $mEstabelecimentoContrato->isContratoEmVigenciaAtivoPorEstabelecimento($estabelecimento['id']);
        }

        return $possuiAcesso;
    }
}