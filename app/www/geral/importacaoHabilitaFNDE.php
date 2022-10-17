<?php
// carrega as funções gerais
include_once 'config.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once 'ImportacaoHabilitaFNDE.class.php';

//CONSTANTES REFERENTE A IMPORTAÇÃO DE ARQUIVOS DO FNDE
define( "HEADERFNDE", 1 );
define( "DETALHEFNDE", 2 );
define( "TRAILLERFNDE", 3 );

//CONSTANTES REFERENTE AO TIPO DE ARQUIVOS PARA IMPORTAÇÃO
/*define( "K_TIPO_ENTIDADE", 'CadastroEntidade' );
define( "K_TIPO_DOCUMENTO", 'DocumentosHabilitacao' );
define( "K_TIPO_DILIGENCIA", 'Diligencia' );
define( "DIRETORIO_ORIGEM_HABILITA", 'C:/ArquivosEmendas' );
define( "DIRETORIO_DESTINO_HABILITA", 'C:/ArquivosEmendas/processado' );
define( "DIRETORIO_PROCESSANDO_HABILITA", 'C:/ArquivosEmendas/processando' );
define( "DIRETORIO_LOG_HABILITA", 'C:/ArquivosEmendas/log' );*/

define( "K_TIPO_ENTIDADE", 'CadastroEntidade' );
define( "K_TIPO_DOCUMENTO", 'DocumentosHabilitacao' );
define( "K_TIPO_DILIGENCIA", 'Diligencia' );
define( "DIRETORIO_ORIGEM_HABILITA", '../../arquivos/emenda/habilita' );
define( "DIRETORIO_DESTINO_HABILITA", '../../arquivos/emenda/habilita/processado' );
define( "DIRETORIO_PROCESSANDO_HABILITA", '../../arquivos/emenda/habilita/processando' );
define( "DIRETORIO_LOG_HABILITA", '../../arquivos/emenda/habilita/log' );


// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

set_time_limit(0);
// pega o endereço do diretório atual
//getcwd();
//K_TIPO_ENTIDADE, K_TIPO_DOCUMENTO, K_TIPO_DILIGENCIA
$arTipos = array( K_TIPO_ENTIDADE, K_TIPO_DOCUMENTO, K_TIPO_DILIGENCIA );

//importacaoHabilitaFNDE::start($diretorio, $arTipos);
$importaFNDE = new importacaoHabilitaFNDE(DIRETORIO_ORIGEM_HABILITA, $arTipos);
?>