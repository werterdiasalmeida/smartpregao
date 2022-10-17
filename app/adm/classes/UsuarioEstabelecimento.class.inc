<?php

/**
 * Class UsuarioEstabelecimento
 */
class UsuarioEstabelecimento extends Usuario
{
    public static function perfisEstabelecimento()
    {
        return array(
            ADM_PERFIL_GERAL,
            ADM_PERFIL_LOCAL,
            ADM_PERFIL_RECEPCIONISTA,
            ADM_PERFIL_CORRETOR_AGENDA
        );
    }

    /**
     * @param $params
     * @return mixed
     * @throws Exception
     */
    public function salvarUsuarioEstabelecimento($params)
    {
        $params['usucpf'] = str_replace(array('.', '-'), '', $params['usucpf']);
        // Busca CPF na base para ver se é uma alteração
        $usucpfBd = $this->recuperarUm('usucpf', array("usucpf = '{$params['usucpf']}'"));

        $perfisEstabelecimento = self::perfisEstabelecimento();

        $mUsuarioResponsabilidade = new UsuarioResponsabilidadeEstabelecimento();
        $perfisComOutrasResponsabilidades = $mUsuarioResponsabilidade->recuperarColuna('pflcod', array(
            "pflcod IN (" . implode(', ', $perfisEstabelecimento) .  ")",
            "estabelecimento_id != {$params['estabelecimento_id']}",
            "usucpf = '{$params['usucpf']}'",
            "rpustatus = 'A'"
        ));

        $perfisSelecionados = $params['perfil'] ? $params['perfil'] : array();

        $mPerfilUsuario = new PerfilUsuario();
        $perfis = $mPerfilUsuario->recuperarColuna('pflcod', array(
            "usucpf = '{$params['usucpf']}'",
            "pflcod IN (SELECT pflcod FROM seguranca.perfil WHERE sisid = {$_SESSION['sisid']})"
        ));
        $perfis = array_filter($perfis, function($perfil) use ($perfisEstabelecimento, $perfisComOutrasResponsabilidades){
            return !in_array($perfil, $perfisEstabelecimento) || in_array($perfil, $perfisComOutrasResponsabilidades);
        });

        $perfisResponsabilidade = array_filter($perfisSelecionados, function($perfil) use ($perfisEstabelecimento, $perfis){
            return in_array($perfil, $perfisEstabelecimento);
        });

        $params['perfil'] = array_unique(array_merge($perfis, $perfisResponsabilidade));

        // Só grava as informações do usuário na inclusão
        if(empty($usucpfBd)){
            $params['usucpf'] = $this->salvarUsuario($params);
        }else{
            $this->atribuirPerfis($params['perfil'], $usucpfBd);
        }

        foreach ($perfisEstabelecimento as $perfilResponsabilidade){
            $responsabilidades = $mUsuarioResponsabilidade->recuperarColuna('estabelecimento_id', array(
                "usucpf = '{$params['usucpf']}'",
                "rpustatus = 'A'",
                "pflcod = {$perfilResponsabilidade}",
                "estabelecimento_id != {$params['estabelecimento_id']}",
            ));

            if(in_array($perfilResponsabilidade, $perfisSelecionados)){
                $responsabilidades[] = $params['estabelecimento_id'];
            }

            $paramsResp = array(
                'estabelecimentos' => $responsabilidades,
                'pflcod' => $perfilResponsabilidade,
                'usucpf' => $params['usucpf']
            );

            $mUsuarioResponsabilidade->salvarResponsabilidadeEstabelecimento($paramsResp);
            $mUsuarioResponsabilidade->setDadosNull();
        }

        return $params['usucpf'];
    }
}