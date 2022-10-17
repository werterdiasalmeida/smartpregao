<?php
if ( $_SESSION['superuser'] ) {
    $sql = "select distinct
                                                u.usucpf as codigo,
                                                u.usunome as descricao,
                                                ( SELECT 
                                                    array_to_string(array_agg(pfl.pfldsc), ',') AS pfldsc
                                                  FROM 
                                                    seguranca.perfil pfl 
                                                  INNER JOIN seguranca.perfilusuario pu ON pfl.pflcod = pu.pflcod
                                                                                       AND pu.usucpf = u.usucpf) AS perfis
                                                from seguranca.usuario u
                                                inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
                                                inner join perfil p on p.pflcod = pf.pflcod and	p.sisid = " . $_SESSION['sisid'] . "
                                                inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and	us.sisid = p.sisid
                                                where us.suscod = 'A' and p.pflnivel >= (
                                                select min(pflnivel) from seguranca.perfil
                                                inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
                                                where perfilusuario.usucpf = '" . $_SESSION['usucpforigem'] . "'
                                                and perfil.sisid = " . $_SESSION['sisid'] . " )
                                                AND u.usucpf != '" . $_SESSION['usucpforigem'] . "'
                                                AND u.usucpf != '" . $_SESSION['usucpf'] . "'
                                                order by 2 ";
} else {
    $sql = "select distinct u.usucpf as codigo, u.usunome as descricao from seguranca.usuario u inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf inner join perfil p on p.pflcod = pf.pflcod and p.sisid =".$_SESSION['sisid']." inner join seguranca.usuario_sistema us on us.usucpf=u.usucpf and us.sisid=p.sisid where us.suscod='A' and  u.usucpf not in (select pu.usucpf from seguranca.perfilusuario pu, seguranca.perfil p where pu.pflcod=p.pflcod and p.pflsuperuser = 't' and p.sisid=".$_SESSION['sisid'].") order by 2";
    $_SESSION['usuuma'] = 1;
}
$dadosusuario = $db->pegaLinha($sql);
$usucpf_simu = $_SESSION['usucpf'];
?>
    <table id="tabela-simulacao-usuario" data-toggle="table" data-pagination="true" data-page-size="10" data-search="true" class="table table-striped">
        <thead>
        <tr>
            <th data-sortable="true" data-width="58%">Usuário</th>
            <th data-sortable="true" data-width="15%" data-align="center">CPF</th>
            <th data-sortable="true" data-width="25%">Perfis</th>
            <th data-sortable="true" data-width="2%" data-align="center">Simular</th>
        </tr>
        </thead>
        <tbody>
        <?php foreach($db->carregar($sql) as $linha) :
            ?>
            <tr>
                <td><?= $linha['descricao']; ?></td>
                <td><?= formatar_cpf($linha['codigo']); ?></td>
                <td>
                    <?php
                    $perfisSimulacaoUsuario = '<span class="label label-sm label-success">'
                        . implode('</span>&nbsp;<span class="label label-sm label-success">', explode(',', $linha['perfis']))  .
                        '</span>';

                    echo str_replace('label-success">Super Usuário', 'label-danger">Super Usuário', $perfisSimulacaoUsuario);
                    ?>
                </td>
                <td>
                    <a class="btn btn-circle btn-icon-only btn-default green" onclick="setpfl('<?= $linha['codigo']; ?>')" data-toggle="tooltip" title="" data-original-title="Simular usuário">
                        <i class="icon-user"></i>
                    </a>
                </td>
            </tr>
        <?php endforeach; ?>
        </tbody>
    </table>
<?php if($_SESSION['usucpforigem'] != $_SESSION['usucpf']) : ?>
    <div style="text-align: center;">
        <button type="button" class="btn blue btn-outline" onclick="setpfl('<?= $_SESSION['usucpforigem']; ?>')">Cancelar simulação</button>
    </div>
<?php endif; ?>
<script type="application/javascript">
    $(function(){
        $('#tabela-simulacao-usuario').bootstrapTable();
    });
</script>
