<?
/*
  Sistema Simec
  Setor responsável: SPO-MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier
  Programador: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
  Módulo:testa_acesso.inc
  Finalidade: permitir o acesso às páginas do sistema
*/

//dbg($_SESSION,1);
//Verifica a sessão
if( !isset($_SESSION['usucpf']) || !isset($_SESSION['sisid']) ) {
    session_unset();
    $_SESSION['MSG_AVISO'] = 'Sua sessão expirou. Efetue login novamente.';
    header('Location: /login.php');
    exit();
}

$url = str_replace('/'.$_SESSION['sisdiretorio'].'/','',$_SERVER['REQUEST_URI']);

if (! strpos($url, 'favorito')) {  $_SESSION['favurl']=$url; }
$posicao = strpos($url, '&acao=');
$url = substr($url,0,$posicao+7);

// buscar na tabela de menu um registro cujo link seja igual à url
$sql = 'select mnu.mnuid,mnu.mnudsc, mnu.abacod, mnu.mnutransacao, pfu.usucpf, pfu.pflcod, pfl.pfldsc ';
$sql = $sql . ' from seguranca.perfilusuario pfu, seguranca.perfil pfl, seguranca.menu mnu, seguranca.perfilmenu pfm ';
$sql = $sql . " where pfu.pflcod = pfl.pflcod and trim(mnu.mnulink) ='".$url."' and mnu.mnuid = pfm.mnuid and pfm.pflcod = pfl.pflcod and pfu.usucpf= '".$_SESSION['usucpf']."' and pfl.sisid=".$_SESSION['sisid'];
//dbg( $sql, 1, '00677864132' );
$saida = $db->recuperar($sql);
//dbg( $saida, 0, '00677864132' );
if(! is_array($saida))
{
    // dbg($sql,'1','37423428787');
    //dbg($sql,'1','80690122691');
    // então busca em delegação de competência se é possível navegar
    $sql = "select usucpf_delegante as usu_superior from seguranca.delegacao_competencia where sisid=".$_SESSION['sisid']." and usucpf_delegado='".$_SESSION['usucpf']."' and dcpstatus='A' and dcpdata_ini <='".date('Y-m-d')."' and dcpdata_fim >='".date('Y-m-d')."'";
//	dbg( $sql );
    $RS = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS);
    $ok=0;
    for ($i=0;$i<=$nlinhas;$i++)
    {
        $res =  $db->carrega_registro($RS,$i);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        $sql = 'select mnu.mnuid,mnu.mnudsc, mnu.abacod, mnu.mnutransacao, pfu.usucpf, pfu.pflcod, pfl.pfldsc ';
        $sql = $sql . ' from seguranca.perfilusuario pfu, seguranca.perfil pfl, seguranca.menu mnu, seguranca.perfilmenu pfm ';
        $sql = $sql . " where pfu.pflcod = pfl.pflcod and trim(mnu.mnulink) ='".$url."' and mnu.mnuid = pfm.mnuid and pfm.pflcod = pfl.pflcod and pfu.usucpf= '".$usu_superior."'";
//      dbg( $sql, 1, '00677864132' );
//		dbg( (boolean) $ok, 0, '00677864132' );
        $res = $db->recuperar($sql);
        if (is_array($res))
        {
            $ok=1;
            break;
        }
    }
//    dbg((boolean)$ok,1);
//    dbg( (boolean) $ok, 0, '00677864132' );
    if (! $ok)
    {
//        if($_SESSION['sisid'] == 72 && $url === 'adm.php?modulo=inicio&acao=C'){
//            $mResponsabilidade = new UsuarioResponsabilidade();
//
//            if($mResponsabilidade->getUrlTelaInicial() != $url){
//                header("Location: {$mResponsabilidade->getUrlTelaInicial()}");
//                die;
//            }
//        }


//		dbg( $sql, 0, '00677864132' );
//		dbg( (boolean) $ok, 1, '00677864132' );
        ?><script>
        location.href = "../acesso_negado.php";
    </script>
        <?
        exit();
    }
}
//dbg( $ok, 1, '00677864132' );
//utilizado nos titulos de páginas
$abacod_tela = $saida['abacod'];
$titulo_modulo = $saida['mnutransacao'];
if (! strpos($url, 'favorito')) $_SESSION['mnudsc']=$saida['mnudsc'].'...';

//utilizado pela migalha de pao no cabeçalho e pela função de auditoria
$modulo_atual=$saida['mnuid'];
$_SESSION['mnuid']=$saida['mnuid'];
