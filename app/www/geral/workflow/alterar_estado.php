<?php
// inicializa sistema
require_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . 'includes/composer/vendor/autoload.php';
spl_autoload_register('namespaceAutoloader');
switch ( $_SESSION['sisdiretorio'] ){
    case 'adm':
        include_once APPRAIZ . "www/adm/_constantes.php";
        include_once APPRAIZ . "www/adm/_componentes.php";
        include_once APPRAIZ . "www/adm/_funcoes.php";

        spl_autoload_register(function ($class_name) {
            $arCaminho = array(
                APPRAIZ . "adm/classes/",
                APPRAIZ . "includes/classes/modelo/public/",
                APPRAIZ . "includes/classes/modelo/app/",
                APPRAIZ . "includes/classes/modelo/territorios/",
                APPRAIZ . "includes/classes/modelo/entidade/",
                APPRAIZ . "includes/classes/modelo/corporativo/",
                APPRAIZ . "includes/classes/modelo/seguranca/",
                APPRAIZ . "includes/classes/controller/",
                APPRAIZ . "includes/classes/",
                APPRAIZ . "includes/classes/exception/",
                APPRAIZ . "includes/classes/view/",
                APPRAIZ . "includes/classes/html/",
            );

            foreach($arCaminho as $caminho){
                $arquivo = $caminho . $class_name . '.class.inc';
                if ( file_exists( $arquivo ) ){
                    require_once( $arquivo );
                    break;
                }
            }
        });
        break;
}

if ( !$db )
{
    $db = new cls_banco();
}

if(!$_REQUEST['docid'] || !$_REQUEST['esdid'] || !$_REQUEST['aedid']) {
    echo "<script>
			alert('Informações não foram passadas corretamente. Refaça o procedimento.');
			window.opener.location='?modulo=inicio&acao=C';
			window.close();
		  </script>";
    exit;
}

$docid = (integer) $_REQUEST['docid'];
$esdid = (integer) $_REQUEST['esdid'];
$aedid = (integer) $_REQUEST['aedid'];
$cmddsc = trim( $_REQUEST['cmddsc'] );
$verificacao = (string) $_REQUEST['verificacao'];

// verifica se precisa de comentário e se comentário está preenchido
if ( wf_acaoNecessitaComentario2( $aedid ) && !$cmddsc )
{
    include "alterar_estado_comentario.php";
    exit();
}

// trata dado para verificacao externa
$dadosVerificacao = unserialize( stripcslashes( $verificacao ) );
if ( !is_array( $dadosVerificacao ) )
{
    $dadosVerificacao = array();
}

// realiza alteracao de estado
if ( wf_alterarEstado( $docid, $aedid, $cmddsc, $dadosVerificacao ) )
{
    //var_dump($a);
    //die();
    $mensagem = "Estado alterado com sucesso!";
}
else
{
    $mensagem = wf_pegarMensagem();
    $mensagem = $mensagem ? $mensagem : "Não foi possível alterar estado do documento.";
}

?>
<script type="text/javascript">
    window.opener.wf_atualizarTela( '<?php echo $mensagem ?>', self );
</script>