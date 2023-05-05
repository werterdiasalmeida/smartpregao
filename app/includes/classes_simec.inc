<?
/*
 Sistema Sistema Simec
 Setor responsável: SPO/MEC
 Desenvolvedor: Desenvolvedores Simec
 Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
 Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
 Módulo: classes_simec.inc
 Finalidade: reunião de todas as classes do sistema
 Data de criação: 24/06/2005
 */

///conexão dbsimec apoio


$GLOBALS["connapoio"] = "host=$servidor_bd_apoio port=$porta_bd_apoio dbname=$nome_bd_apoio user=$usuario_db_apoio password=$senha_bd_apoio";

/*
$servidor_bd_apoio = '192.168.222.20';
$porta_bd_apoio = '6544';
$nome_bd_apoio = 'dbsimec';
$usuario_db_apoio = 'seguranca';
$senha_bd_apoio = '123seg';
$GLOBALS["connapoio"] = "host=$servidor_bd_apoio port=$porta_bd_apoio dbname=$nome_bd_apoio user=$usuario_db_apoio password=$senha_bd_apoio";
*/

class cls_classes_simec {
    var $mess_erro;
    var $ultimo_erro;
    function __construct() {
        $this->mess_erro[101] = "Não foi possível estabelecer uma conexão com o servidor LDAP.";
        $this->mess_erro[102] = "Não foi possível autenticar no servidor LDAP.";
        $this->mess_erro[103] = "A pesquisa não retornou resultado.";
        $this->mess_erro[104] = "Não foi possível estabelecer uma conexão com o cls_banco de Dados.";
        $this->mess_erro[105] = "Falha na seleção da base de dados.";
        $this->mess_erro[106] = "Falha na execução da operação. Foi enviada uma mensagem para o Administrador.";
        $this->mess_erro[107] = "Usuário ou senha inválida!";
        $this->mess_erro[108] = "Ocorreu um erro durante a execução do Bind.";
        $this->mess_erro[200] = "Registro cancelado.";
    }
}
class cls_banco
{
    var $link;
    var $linkapoio;
    var $resultado;
    var $num_rows;
    var $ultimo_SQL;
    var $sql;
    var $oci_id;
    var $tentativas;
    function __construct()
    {
        if(empty($this->link)){
            while((!$this->link) and ($this->tentativas < 5))
            {
                //                	dbg( "host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."" );
                $this->link = pg_connect("host=".$GLOBALS["servidor_bd"]." port=".$GLOBALS["porta_bd"]." dbname=".$GLOBALS['nome_bd']."  user=".$GLOBALS["usuario_db"] ." password=".$GLOBALS["senha_bd"] ."");

                if($GLOBALS["servidor_bd"] == '192.168.222.20')
                    pg_query($this->link, "SET search_path TO seguranca,monitora,elabrev,public");
                else
                    pg_query($this->link, "SET search_path TO public, seguranca");

                // pg_query($this->link, "SET search_path TO public");

                pg_set_client_encoding($this->link,'LATIN5');
                // pg_set_client_encoding($this->link,'UTF-8');
                $this->tentativas++;
            }

            if((!$this->link) and ($this->tentativas == 5))
                cls_classes_simec::erro(104,2);
            else
                $this->tentativas = 0;
        }

        return 1;

    }


    public function __destruct()
    {
        if (isset($_SESSION['transacao']))
        {
            pg_query($this->link, 'rollback; ');
            unset($_SESSION['transacao']);
        }
//		if (is_resource($this->link)) {@pg_close( $this->link );}
    }




    ///////////////////Grficos do sistema
    function grafico_validacao_prg($prgid=0,$refcod=0)
    {
        $sql= 'select * from validacaostat where refcod='.$refcod.' and prgid='.$prgid;
        $rs=$this->recuperar($sql);
        if (!is_array($rs))
            return '<img src="../imagens/valida3.gif" align="absmiddle" width="15" height="15">';
        else
            return monta_grafico_validacao($rs['faltap'],$rs['total_cor1'],$rs['total_cor2'],$rs['total_cor3'],$rs['acatotal']);

    }

    ////////////////////Fim Grficos


    function eof($sql)
    {
        $RS = $this->carregar($sql);
        $nlinhas = $RS ? count($RS) : 0;
        if ($nlinhas==0) return true; else return false;
    }

    /**
     * Função que altera uma string evitando a presença de caracteres próprios do sql
     *
     * @param unknown_type $string
     * @param unknown_type $p
     * @return string
     */
    function antiInjection($string,$p="")
    {
        // remove palavras que contenham sintaxe sql
        $string = str_replace('"',"\'",$string);
        $string = str_replace("'","\'",$string);

        $string = ($string);
        if($p !=2){
            $string = str_replace( "|" , "" , $string);
        }
        $string = preg_replace(sql_regcase("/(from|select|insert|delete|where|drop table|show tables|#|--|\\\\)/"),"",$string);
        $string = trim($string);//limpa espaços vazio
        //	$string = strip_tags($string);//tira tags html e php
        $string = addslashes($string);//Adiciona barras invertidas a uma string
        if($p!=1){
            if(strlen(trim($string))==0) $string ='';
        }

        return $string;

    }

    /**
     *
     */
    function executar( $SQL, $auditoria = true )
    {
        /*if (!isset($_SESSION) || !array_key_exists('usucpforigem', $_SESSION) || $_SESSION['usucpforigem'] == '') {
			return false;
		}*/
        $SQL = trim($SQL);
        //detecta operacao e tabela (Insert, Update ou Delete)
        preg_match('/(CREATE\s+TABLE|ALTER\s+TABLE|DROP\s+TABLE|SELECT.*|INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+([A-Za-z0-1._]+).*/smui', utf8_encode($SQL), $matches);
        $audtipoCompleto = strtoupper($matches[1]);
        $audtipo         = substr($audtipoCompleto, 0, 1);
        $audtabela       = strtoupper($matches[2]);

        if ($audtipoCompleto == 'ALTER TABLE' || $audtipoCompleto == 'DROP TABLE')
            throw new Exception('A execução do comando "' .$audtipoCompleto. '" não é permitido pelo sistema');

        $_SESSION['sql'] = $SQL;

        // Inicia a transação quando nao estiver iniciada e obrigatoriamente quando
        // a operação for diferente de SELECT
        if (!isset($_SESSION['transacao']) && $audtipo != 'S') {
            $this->resultado = pg_query($this->link, 'begin transaction; ');
            $_SESSION['transacao'] = '1';
        }

        $this->resultado = pg_query($this->link, $SQL);

        if ($this->resultado == null){
            throw new Exception( $SQL . pg_errormessage( $this->link ) );
        }

        if ($auditoria && $audtipo != 'S')
        {
            //Se no tiver vindo de algum módulo
            if ($_SESSION['mnuid']=='') $_SESSION['mnuid']=1;
            //Se no tiver sessão validada
            if ($_SESSION['usucpf']=='') $_SESSION['usucpf']='00000000191';
            //Insere dados na tabela auditoria
            if (!$_SESSION['sisid'])
                $id = 4;
            else
                $id = $_SESSION['sisid'];

            //$SQL = stripslashes($SQL);
            //$SQL = str_replace("'","''",$SQL);
            $SQL = pg_escape_string($SQL);
            $sql_audit = "insert into seguranca.auditoria (usucpf, mnuid, audsql, auddata, audtabela, audtipo, audip, sisid) values ('".$_SESSION['usucpforigem']."', ".$_SESSION['mnuid'].", '".$SQL."', '".date('Y-m-d H:i:s')."', '".$audtabela."', '".$audtipo."', '".$_SERVER["REMOTE_ADDR"]."',$id)";
            if (!is_resource($this->linkapoio))
            {
                $this->linkapoio = pg_connect( $GLOBALS["connapoio"] );
                pg_set_client_encoding($this->linkapoio,'LATIN5');
                pg_query($this->linkapoio, 'begin transaction; ');
            }
            pg_query($this->linkapoio, $sql_audit);
        }

        return $this->resultado;
    }

    /**
     * function executar
     * @description : Retorna um objeto como resultado da execução do sql.
     * @param string sql (obrigatório)
     * @param boolean class (opcional)
     * @return : object
     * @author Gilberto Arruda Cerqueira Xavier
     * @since 24/06/2005
     */
    function pegaUmObjeto( $sql, $class = null ){
        if (!$sql or $sql == '')
            return (object) '';

        $res = $class ? pg_fetch_object( $this->executar($sql), null, $class) : pg_fetch_object( $this->executar($sql) );
        return $res;

    }

    function carregarObjeto( $sql, $class = null ){
        if (!$sql or $sql == '')
            return (object) '';

        $res = $this->executar( $sql );
        while ( $linhaObj = ($class ? pg_fetch_object( $res, null, $class ) : pg_fetch_object( $res )) ){
//        	$linhaObj = ($class ? pg_fetch_object( $res, null, $class ) : pg_fetch_object( $res ));
            $result[] = $linhaObj;
        }

        return $result;

    }





    function apagatudo($texto,$tabela)
    {
        return false;
    }


    function recuperar($SQL, $var = null) {
        if (! $SQL or $SQL=='')
            return null;
        else {
            $res = pg_fetch_all($this->executar($SQL));
            if ( $var != null )
            {
                global ${$var};
                ${$var} = $res[0];
            }
            return $res[0];
        }
    }


    function busca_explicacao($campo,$tabela)
    {
        $sql="select p2.atttypmod-4 as tmax, p4.typname as tipo from pg_attribute p2 left join pg_type p4 on p2.atttypid=p4.oid left join pg_description p1 on p2.attnum=p1.objsubid left join pg_class p3 on p3.relfilenode=p2.attrelid and p3.relname='$tabela' where p2.attname='$campo' limit 1";

        $RS = $this->record_set($sql);
        $res = @$this->carrega_registro($RS,0);
        if ($res['explicacao']=='')
        {

            $sql="select distinct p1.description as explicacao from pg_description p1 inner join pg_attribute p2 on p2.attrelid=p1.objoid and trim(p2.attname) = '$campo' and p2.attnum=p1.objsubid inner join pg_class p3 on p3.relname='$tabela' and p3.reltype::integer=p2.attrelid::integer +1 ";


            $RS = $this->record_set($sql);
            $nlinhas = $this->conta_linhas($RS);

            if ($nlinhas>=0)
            {
                $res2 = @$this->carrega_registro($RS,0);
                $res['explicacao'] = $res2['explicacao'];

            } else   $res['explicacao'] = 'Campo sem descrição detalhada';
        }

        return $res;

    }

    function carregar($SQL, $var = null)
    {
        $res = pg_fetch_all($this->executar($SQL));

        if ( $var != null )
        {
            global ${$var};
            ${$var} = $res;
        }

        return $res;
    }

    function carregarArray($SQL, $var = null){
        $rs = $this->carregar($SQL, $var);
        return $rs ? $rs : array();
    }

    function carregarColuna($SQL, $coluna = '',$var = null)
    {
        $res = pg_fetch_all($this->executar($SQL));

        if ( $var != null )
        {
            global ${$var};
            ${$var} = $res;
        }

        if (!$res) return array();

        $result = array();

        foreach ($res as  $row ){
            if(!$coluna){
                array_push($result,reset($row));
            }
            else{
                array_push($result,$row[$coluna]);
            }
        }
        return $result;
    }



    function verifica_momento() {
        // esta função verifica o momento
        $sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento1 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
        if ($this->pegaUm($sql)) return 1;
        else {
            $sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento2 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
            if ($this->pegaUm($sql)) return 2;
            else return 0;
        }
    }

    // $db->ehdecisor($_SESSION['acaid'],'A',$momento);
    function ehcoorduma($cpf)
    {
        // esta função verifica se o usuário possui perfil de coordenador da uma
        $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='$cpf'";
        if ($this->pegaum($sql)) return 1; else return 0;

    }
    function ehdecisor($cod,$tab,$mom)
    {
        // esta função verifica, pelo cpf do usuário logado e pelo momento se ele é decisor externo, interno ou não é decisor
        if ($this->testa_superuser()) return 1;
        if ($mom==1)
        {
            if ($tab=='A')
            {
                // ação no momento 1
                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql))
                {
                    // então o usuário possui perfil de coordenador de planejamento de unidade ou ação
                    // testo a unidade
                    $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    if ($this->pegaUm($sql)) return 1;
                    else
                    {
                        // testo a responsabilidade em ação
                        $sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        if ($this->pegaUm($sql)) return 1;
                        else return 0;
                    }
                }

            }
            if ($tab=='P')
            {
                // programa no momento 1
                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql))
                {
                    // então o usuário possui perfil de coordenador de planejamento de unidade ou ação
                    // testo a unidade
                    $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    if ($this->pegaUm($sql)) return 1;
                    else
                    {
                        // testo a responsabilidade em programa
                        $sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        if ($this->pegaUm($sql)) return 1;
                        else return 0;
                    }
                }

            }
        }
        if ($mom==2)
        {
            if ($tab=='A')
            {
                // ação no momento 2
                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql)) return 1; // coordenador da UMA

                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql))
                {
                    // então o usuário possui perfil de uma de unidade ou ação ou coord uma
                    // testo a unidade
                    $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    if ($this->pegaUm($sql)) return 1;
                    else
                    {
                        // testo a responsabilidade em ação
                        $sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        if ($this->pegaUm($sql)) return 1;
                        else return 0;
                    }
                }

            }
            if ($tab=='P')
            {
                // programa no momento 2
                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql)) return 1; // coordenador da UMA

                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql))
                {
                    // então o usuário possui perfil de uma de unidade ou ação ou coord uma
                    // testo a unidade
                    $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    if ($this->pegaUm($sql)) return 1;
                    else
                    {
                        // testo a responsabilidade em ação
                        $sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        if ($this->pegaUm($sql)) return 1;
                        else return 0;
                    }
                }

            }
        }
    }

    function ehdigitador($cod,$tab,$mom)
    {

        // esta função verifica, pelo cpf do usuário logado e pelo momento se ele é digitador externo, interno ou não é digitador
        if ($mom==1)
        {
            if ($tab=='A')
            {
                // ação no momento 1
                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql))
                {
                    // então o usuário possui perfil de equipe de planejamento de unidade ou ação
                    // testo a unidade
                    $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    if ($this->pegaUm($sql)) return 1;
                    else
                    {
                        // testo a responsabilidade em ação
                        $sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        if ($this->pegaUm($sql)) return 1;
                        else return 0;
                    }
                }

            }
            if ($tab=='P')
            {
                // programa no momento 1
                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql))
                {
                    // então o usuário possui perfil de equipe de planejamento de unidade ou ação
                    // testo a unidade
                    $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    if ($this->pegaUm($sql)) return 1;
                    else
                    {
                        // testo a responsabilidade em programa
                        $sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        if ($this->pegaUm($sql)) return 1;
                        else return 0;
                    }
                }

            }
        }
        if ($mom==2)
        {
            if ($tab=='A')
            {
                // ação no momento 2
                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36,62) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql)) return 1; // é coordenador da UMA ou super usuário

                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql))
                {
                    // então o usuário possui perfil de UMA unidade ou programa
                    // testo a unidade
                    $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    if ($this->pegaUm($sql)) return 1;
                    else
                    {
                        // testo a responsabilidade em ações no programa específico
                        $sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid in (select prgid from elabrev.ppaacao_proposta where acaid=$cod) and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        if ($this->pegaUm($sql)) return 1;
                        else return 0;
                    }
                }

            }
            if ($tab=='P')
            {
                // programa no momento 2
                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36,62) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql)) return 1; // é coordenador da UMA ou super usuário

                $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
                if ($this->pegaum($sql))
                {
                    // então o usuário possui perfil de equipe de perfil de UMA unidade ou programa
                    // testo a unidade
                    $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod inner join elabrev.ppaacao_proposta ap on ap.acaid = ua.acaid and ap.prgid=$cod where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    if ($this->pegaUm($sql)) return 1;
                    else
                    {
                        // testo a responsabilidade em programa
                        $sql = "select distinct ur.prgid from elabrev.usuarioresponsabilidade ur where ur.prgid=$cod and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        if ($this->pegaUm($sql)) return 1;
                        else return 0;
                    }
                }

            }
        }
    }

    /**
     * @param string $texto
     * @return string
     */
    function escape( $mixValorCampo  ){


        if( ( gettype( $mixValorCampo ) == 'integer' ) || ( gettype( $mixValorCampo ) == 'float') )
        {
            return $mixValorCampo;
        }
        if( is_string( $mixValorCampo ) )
        {
            return "'" . pg_escape_string( $this->link, $mixValorCampo ) . "'" ;
        }
        if( is_null( $mixValorCampo ) )
        {
            return 'NULL';
        }
        return $mixValorCampo;
    }

    function verifica_momento2() {
        // esta função verifica, pelo cpf do usuário logado e pela data se ele tem direito de agir no sistema e se está dentro do seu momento.
        //1. verificando se o usuário é coordenador de planejamento ou equipe de apoio, ou superusuário ou UMA ou coordenador da UMA
        $retorna=0;
        $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (33,68) and pu.usucpf='".$_SESSION['usucpf']."'";
        $resp = $this->pegaUm($sql);
        if ($resp > 0 ) {$ok=1;$respeqpplan = $resp;}
        $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,67) and pu.usucpf='".$_SESSION['usucpf']."'";
        $resp = $this->pegaUm($sql);
        if ($resp > 0 ) {$ok=1;$respcoordplan = $resp;}
        $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (63,66) and pu.usucpf='".$_SESSION['usucpf']."'";
        $resp = $this->pegaUm($sql);
        if ($resp > 0 ) {$ok=1;$respuma = $resp;}
        $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (36) and pu.usucpf='".$_SESSION['usucpf']."'";
        $resp = $this->pegaUm($sql);
        if ($resp > 0 ) {$ok=1;$respcoorduma = $resp;}
        $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (62) and pu.usucpf='".$_SESSION['usucpf']."'";
        $resp = $this->pegaUm($sql);
        if ($resp > 0 ) {$ok=1;$respsupusu = $resp;	}

        if ($ok > 0)
        {
            // então pode editar
            // verifica se a data está dentro do período autorizado
            // $sql="select refdata_inicio as inicio, refdata_limite_momento1 as momento1,refdata_limite_momento2 as momento2,refdata_limite_momento3 as momento3 from elabrev.referencia where refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
            //$momentos=$this->pegaLinha($sql,0);
            // se for coordenador de planejamento ou equipe de planejamento - perfil 24 ou 33 e se estiver dentro do momento 1, então ok.
            if ($respcoordplan or $respeqpplan or $respsupusu)
            {
                $sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento1 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
                $ref=$this->pegaUm($sql) ;
            }
            // se for uma ou coordenador da uma e se estiver dentro do momento 2, então ok.
            if ($respcoorduma or $respuma or $respsupusu)
            {
                $sql="select refcod from elabrev.referencia where refdata_inicio<=current_date and refdata_limite_momento2 >=current_date and refstatus='A' and refano_ref='".$_SESSION['exercicio']."'";
                $ref=$this->pegaUm($sql) ;
            }
            if ($ref) $retorna=1;

        }
        return $retorna;
    }

    function ehcriador($cod,$busca=0)
    {
        if (! $busca or $busca=='A')
            $sql = "select acaid from elabrev.ppaacao_proposta where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
        else if ($busca == 'P')
            $sql = "select prgid from elabrev.ppaprograma_proposta where usucpf='".$_SESSION['usucpf']."' and prgid=$cod and prsano='".$_SESSION['exercicio']."'";
        else if ($busca == 'EA')
            $sql = "select acaid from elabrev.proposta_exclusao_acao where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
        else if ($busca == 'MA')
            $sql = "select acaid from elabrev.proposta_migracao_acao where usucpf='".$_SESSION['usucpf']."' and acaid=$cod and prsano='".$_SESSION['exercicio']."'";
        else if ($busca == 'EP')
            $sql = "select prgid from elabrev.proposta_exclusao_programa where usucpf='".$_SESSION['usucpf']."' and prgid=$cod and prsano='".$_SESSION['exercicio']."'";
        else if ($busca == 'CP')
            $sql = "select eraid from elabrev.elaboracaorevisao where usucpf='".$_SESSION['usucpf']."' and eraid=$cod and prsano='".$_SESSION['exercicio']."'";

        if ($this->pegaUm($sql)) return true;
        else return false;
    }

    function verifica_resp_elabrev($cod=0,$tab=0)
    {
        unset($_SESSION['uniorc']);
        //$cod= ação ou programa
        //$tab = tabela de ação ou tabela de programa
        // esta função verifica, pelo cpf do usuário logado e pela id da ação e do programa se ele pode editar o registro.
        //1. verificando se o usuário é coordenador de planejamento ou equipe de apoio, ou superusuário ou UMA ou coordenador da UMA
        $retorna=0;
        $sql = "select pu.pflcod from seguranca.perfilusuario pu where pflcod in (24,33,36,62,63,66,67,68) and pu.usucpf='".$_SESSION['usucpf']."'";
        $resp = @$this->carregar( $sql );
        if (  $resp && count($resp) > 0 )
        {
            foreach ( $resp as $linha )
            {
                foreach($linha as $k=>$v) ${$k}=$v;
                if ($pflcod == 24 or $pflcod==33)
                {
                    // então pode editar
                    //verifica se a ação pertence à unidade
                    if ($_SESSION['acaid'])
                    {
                        $sql = "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur
inner join elabrev.unidade_acao ua on ua.unicod=ur.unicod
inner join elabrev.ppaacao_proposta  ap on ap.acaid = ua.acaid and ua.acaid=".$_SESSION['acaid']." where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        $ret1=$this->pegaUm($sql);
                    }
                    //$retorna=$resp;
                    // verifica, se for perfil 24 ou 33 quais as unidades que ele pode operar
                    $sql= "select distinct ur.unicod from elabrev.usuarioresponsabilidade ur where ur.usucpf='".$_SESSION['usucpf']."' and ur.unicod is not null and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                    $rs = $this->carregar($sql);
                    if (  $rs && count($rs) > 0 )
                    {
                        foreach ( $rs as $ln )
                        {
                            foreach($ln as $k=>$v) ${$k}=$v;
                            $_SESSION['uniorc'][] = $unicod;
                        }
                    }
                }
                if ($pflcod == 67 or $pflcod==68)
                {
                    // então pode editar
                    //verifica se está responsável pela ação
                    if ($_SESSION['acaid'])
                    {
                        $sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=".$_SESSION['acaid']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        $ret2=$this->pegaUm($sql);
                    }
                }
                if ($pflcod == 67 or $pflcod==68)
                {
                    // então pode editar
                    // verifica se está responsável pela ação
                    if ($_SESSION['acaid'])
                    {
                        $sql = "select distinct ur.acaid from elabrev.usuarioresponsabilidade ur where ur.acaid=".$_SESSION['acaid']." and ur.usucpf='".$_SESSION['usucpf']."' and ur.rpustatus='A' and ur.prsano='".$_SESSION['exercicio']."' ";
                        $ret3=$this->pegaUm($sql);
                    }
                }
                if ($pflcod == 62 or $pflcod==34 or $pflcod==36)
                {
                    unset($_SESSION['uniorc']);
                    $ret4 = 1;
                }
            }
        }
        if ($ret1) $retorna=$ret1;
        if ($ret2) $retorna=$ret2;
        if ($ret3) $retorna=$ret3;
        if ($ret4) $retorna=$ret4;

        return $retorna;
    }


    function verifica_geral()
    {
        // esta funação verifica vários itens
        // verificando o plano de trabalho
        // fase 1 - verifica se o usuário tem responsabilidades em ação
        $sql="select ur.acaid from monitora.usuarioresponsabilidade ur inner join seguranca.usuario u on u.usucpf=ur.usucpf inner join monitora.acao a on a.acaid=ur.acaid and a.prgano='".$_SESSION['exercicio']."' where ur.acaid is not null and ur.usucpf='".$_SESSION['usucpf']."'";

        $RS = $this->record_set($sql);
        $nlinhas = $this->conta_linhas($RS);
        if ($nlinhas >= 0) {
            // então o usuário possui responsabilidade
            for ($i=0;$i<=$nlinhas;$i++)
            {
                $res =  $this->carrega_registro($RS,$i);
                if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
                // para cada ação, verifico se existe plano de trabalho
                $sql = "select p.ptoavisoantecedencia  as antec from monitora.plantrabacao pa inner join monitora.planotrabalho p on p.ptoid=pa.ptoid and p.ptotipo in ('F','E') and to_char(p.ptodata_fim,'YYYY') >='".date('Y')."' "." where pa.acaid=$acaid";
                $res =  $this->pegaUm($sql);
                if ($res) $antec=$res;
                if (! $antec) $antec=7;
                $sql = "select pa.* , p.ptodata_ini,p.ptodata_fim, p.ptodsc,a.acacod, a.acadsc from monitora.plantrabacao pa inner join monitora.planotrabalho p on p.ptoid=pa.ptoid and p.ptotipo in ('F','E') and ptodata_ini <= current_date- interval '$antec days' and to_char(p.ptodata_fim,'YYYY') >='".date('Y')."' "." inner join acao a on a.acaid=pa.acaid where pa.acaid=$acaid";

                $RS2 = $this->record_set($sql);
                $nlinhas2 = $this->conta_linhas($RS2);
                // agora tenho os planos de trabalho da ação testada.
                // para cada ptoid vou verificar se já foi lançado algum evento.
                $ok=1;
                if ($nlinhas2 >= 0) {
                    for ($ii=0;$ii<=$nlinhas2;$ii++)
                    {
                        $res =  $this->carrega_registro($RS2,$ii);
                        $ok=0;
                        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
                        $sql = "select e.expid from monitora.execucaopto e where ptoid = $ptoid and acaid = $acaid";
                        $RS3 = $this->record_set($sql);
                        $nlinhas3 = $this->conta_linhas($RS3);
                        if ($nlinhas3 >= 0) {$ok=1;break;}
                        else
                        {
                            // então não fez nada que devia
                            $_SESSION['texto'][] ="Você cadastrou atividades no Plano de Trabalho - $ptodsc da ação $acacod-$acadsc e até o momento não monitorou a atividade";
                            $popup=1;
                        }
                    }
                }

            }
            if ($popup)
            {
                ?>
                <script>
                    e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/msg_geral&acao=A";
                    window.open(e, "Associação_de_Responsáveis","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=600,height=400'");
                </script>
                <?
            }
        }
    }

    /**
     * Monta combobox html retornando ou imprimindo o componente
     *
     * @param string $var Nome da variável que possui o valor selecionado
     * @param string|array $sql Sql ou array com as opções do combobox
     * @param string $habil 'S'|'N' Define se será habilitado para o usuário
     * @param string $titulo Titulo
     * @param string $acao Nome da funcao a ser executada, ele complementa sozinho o ( this.value )
     * @param string $opc
     * @param string $txtdica
     * @param integer $size Tamanho do campo
     * @param char $obrig 'S'|'N' Define se é campo obrigatório
     * @param string $id Nome do id
     * @param bool $return true|false Retorna(true) o valor ou imprime(false)
     * @param string $value
     * @return string
     */
    function monta_combo($var,$sql,$habil,$titulo='',$acao,$opc,$txtdica='',$size='',$obrig='', $id = '', $return = false, $value = null, $title= null)
    {
        // este método monta um combobox onde: var = nome do combo; sql é a query,
        //titulo é o título da primeira linha do combo
        global ${$var};
        $value = $value ? $value : ${$var};

        if (is_array($sql))
            $rescombo = $sql;
        else
            $rescombo = $this->carregar( $sql );

        $select = "";
        if ( $txtdica )
            $select = "<span onmouseover=\"return escape('". $txtdica . "');\">";

        // define atributos do select
        if ( $habil == 'S' )
            $select .= "<select  name='" . strtolower( $var ) . "' ";
        else
            $select .= "<select name='" . strtolower( $var ) . "_disable' ";

        $class = "CampoEstilo";

        if ( $obrig == 'S' )
            $class .= " obrigatorio";

        $select .= " class='$class' ";

        if($title){
            $select .= "title='".  $title  ."'";
        }

        if ( $size <> '' )
            $select .= " style='width:".$size."px;'";
        else $select .= " style='width: auto'";
        if ( $habil == 'N' )
            $select = $select. ' disabled="disabled" ';
        if ( isset( $tamanho ) && $tamanho > 1 )
            $select .= ' size="' . $tamanho . '" ';
        if ( $acao )
            $select .= ' onchange="' . $acao . '(this.value)"';
        if ( $id )
            $select .= ' id="' . htmlentities( $id ).  '"';
        $select .= ">";

        // cria os options
        if ( $titulo )
            $select .= '<option value="">'.$titulo.'</option>';

        if($rescombo){
            for ( $i = 0; $i < count( $rescombo ); $i++ )
            {
                if ( $value == $rescombo[$i]['codigo'] )
                    $sel = 'selected="selected"';
                else
                    $sel='';
                $select .= "<option value='" . $rescombo[$i]['codigo'] . "' " . $sel . ">" .
                    $rescombo[$i]['descricao'] .
                    "</option>\n";
            }
        }
        if ( $value == 'x')
            $sel = 'selected="selected"';
        else
            $sel='';
        if ( $opc )
            $select .= "<option " . $sel . " value='x'>" . $opc . "</option>\n";

        // finaliza impressão do select
        $select .= '</select>';
        if ( $txtdica )
            $select .= "</span>";
        if ( $habil == 'N' )
            $select .= "<input type='hidden' name ='" . strtolower( $var ) . "' value='" . ${$var} . "' />";
        if ( $obrig == 'S' )
            $select .= obrigatorio();

        if ($return)
            return $select;
        else
            echo $select;
    }


    /**
     * Monta combobox_multipla html imprimindo o componente
     *
     * @param string  $var Nome da variável que possui o valor selecionado
     * @param string|array $sql Sql ou array com as opções do combobox
     * @param string  $habil 'S'|'N' Define se será habilitado para o usuário
     * @param string  $titulo Titulo
     * @param string  $acao Nome da funcao JAVASCRIPT a ser executada, ele complementa sozinho o ( this.value )
     * @param string  $opc
     * @param string  $txtdica
     * @param integer $tamanho Quantidade de opções que serão exibidas
     * @param integer $size Tamanho do campo
     * @return void
     */
    function monta_combo_multiplo($var,$sql,$habil,$titulo='',$acao,$opc,$txtdica='', $tamanho=4, $size='',$arrSelected = array(), $id = null,$return = false) {
        // este mtodo monta um combobox onde: var = nome do combo; sql  a query,
        //titulo  o ttulo da primeira linha do combo
        global ${$var};

        if(!is_array(${$var}) || @count(${$var})<1) {
            ${$var} = array();
        }

        if (is_array($sql))
            $rescombo = $sql;
        else
            $rescombo = $this->carregar( $sql );

        $select = "";
        if ($txtdica) $select = "<span onmouseover=\"return escape('$txtdica');\">";
        $select .= "<select name='".strtolower($var)."[]' class='CampoEstilo' size='" . $tamanho . "' multiple='multiple'";
        if ( $size <> '' )
            $select .= " style='width:".$size."px;'";
        else
            $select .= " style='width: auto'";

        if($id){
            $select .= " id='$id'";
        }

        if ($habil=='N')      $select = $select. ' disabled="disabled" ';

        if ($acao) $select = $select. ' onChange="'.$acao.'(this.value)"';
        $select = $select .">";


        if($titulo) $select .= '<option ' . ((!isset(${$var}) || (@count(${$var})==1 && !(bool)${$var}[0]) ) ? 'selected' : ''). ' value="">'.$titulo.'</option>';
        for ($i=0;$i<count($rescombo);$i++) {
            if (array_search($rescombo[$i]['codigo'], ${$var}) !== false) $sel = '  SELECTED'; else $sel='';
            $select .= "<option ".(is_array($arrSelected) && in_array($rescombo[$i]['codigo'],$arrSelected) ? " selected='selected' " : "").$sel." value='".$rescombo[$i]['codigo']."'>".$rescombo[$i]['descricao']."</option>";
        }
        if (array_search('x', ${$var}) !== false) $sel = '  SELECTED'; else $sel='';
        if ($opc and $opc<>'') $select .="<option ".$sel." value='x'>".$opc."</option>";
        $select .= '</select>';
        if ($txtdica) $select .= "</span>";
        if ($habil=='N') $select .= "<input type='hidden' name ='".strtolower($var)."' value='".${$var}."' />";

        if($return){
            return $select;
        }else{
            echo $select;
        }
    }

    /*
	 * Monta checkboxes a partir de um select as colunas codigo e descricao
	 *
	 * @param $nome String nome do campo
	 * @param $query String a consulta sql que vai ser executada
	 * @param $marcados Array traz os itens que devem ser selecinados
	 * @param $separador String elemento a ser usado entre dois checkboxes por padrão
	 * @return void
	 */
    function monta_checkbox($nome, $query, $marcados, $separador='  ') {
        if(!is_array($marcados) || @count($marcados)<1) {
            $marcados = array();
        }

        $saida = "";
        $rescombo=$this->carregar($query);
        if(is_array($rescombo) && @count($rescombo)>0) {
            for ($i=0, $j=count($rescombo)-1;$i<count($rescombo);$i++) {
                $checked = array_search($rescombo[$i]['codigo'], $marcados) !== false ? 'checked="checked"' : '';
                echo $rescombo[$i]['indice']."<input type=\"checkbox\" name=\"" . $nome . "\" value=\"" . $rescombo[$i]['codigo'] . "\" " . $checked . " /> " . $rescombo[$i]['descricao'];
                if($i!=$j) echo $separador;
            }
        }
    }

    function cria_aba($abacod_tela,$url,$parametros, Array $arMnuid = array())
    {
        $where = "";
        if($_SESSION['sisid']){
            $where = " and menu.sisid = {$_SESSION['sisid']} ";
        }

        if( $arMnuid ){
            $filtro = "and menu.mnuid not in (".implode(',', $arMnuid).")";
        }

        //Função cria aba que monta as abas visualmente
        if (trim($abacod_tela)<>'')
        {
            $sql = "select menu.mnuid, menu.mnudsc, menu.mnulink, menu.mnutransacao
                    from menu, aba_menu
                where menu.mnuid=aba_menu.mnuid
                and aba_menu.abacod=".$abacod_tela."
                $where
                and menu.mnuid in(
                                select distinct m2.mnuid from perfilmenu m2, perfilusuario p
                                    where m2.pflcod=p.pflcod and p.usucpf='".$_SESSION['usucpf']."'
                                )
				$filtro
				order by menu.mnucod";
            $RS = $this->carregar($sql);

            if(is_array($RS)) {
                print '<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" class="notprint"><tr><td><table cellpadding="0" cellspacing="0" align="left" class="tabela_abas"><tr>';
                $nlinhas = count($RS)-1;

                for ($j=0; $j<=$nlinhas;$j++) {
                    foreach($RS[$j] as $k=>$v){
                        ${$k}=$v;
                    }

                    $parametro = is_array( $parametros ) ? $parametros[$j] : $parametros;

                    $classe = "";
                    if ($url==$mnulink) {
                        $classe = "aba_selecionada";
                    } else {
                        $classe = "aba_normal";
                    }


                    if ($mnulink<>$url){
                        print '<td height="20" align="center" valign="middle" class="'.$classe.' padding-left: 10px; padding-right: 10px;" onclick="window.location = \''.$mnulink.$parametro.'\'">';
                        print $mnudsc.'</td>';
                    } else {
                        print '<td height="20" align="center" valign="middle" class="'.$classe.' padding-left: 10px; padding-right: 10px;">' . $mnudsc.'</td>';
                    }

                    print "<td class='aba_espaco'></td>";
                }

                print '</tr></table></td></tr></table>';
            }
        }
    }

    function cria_abam($abacod_tela, $url, $parametros, Array $arMnuid = array(), $numerarAbas = false, $adicionarAbaAtual = false)
    {
        $where = "";
        if($_SESSION['sisid']){
            $where = " and menu.sisid = {$_SESSION['sisid']} ";
        }

        $unionAbaAtual = '';
        if($adicionarAbaAtual){
            $unionAbaAtual = "UNION ALL
                          (SELECT 
                            menu.mnuid, 
                            menu.mnucod, 
                            menu.mnudsc, 
                            menu.mnulink, 
                            menu.mnutransacao,
                            true AS agregado
                          FROM 
                            seguranca.menu 
                          WHERE
                            menu.mnulink = '{$url}')";
        }

        $filtro = '';
        if( $arMnuid ){
            $filtro = "and menu.mnuid not in (".implode(',', $arMnuid).")";
        }

        //Função cria aba que monta as abas visualmente
        if (trim($abacod_tela)<>'')
        {

            $sql = "SELECT 
                  * 
                FROM (
                    (SELECT 
                      menu.mnuid, 
                      menu.mnucod, 
                      menu.mnudsc, 
                      menu.mnulink, 
                      menu.mnutransacao,
                      false AS agregado
                    FROM 
                      seguranca.menu
                    INNER JOIN
                      seguranca.aba_menu ON (menu.mnuid = aba_menu.mnuid)
                    WHERE 
                      aba_menu.abacod=".$abacod_tela."
                      {$where}
                      AND 
                      menu.mnuid IN(SELECT DISTINCT 
                                        m2.mnuid 
                                    FROM 
                                        seguranca.perfilmenu m2, 
                                        seguranca.perfilusuario p
                                    WHERE 
                                        m2.pflcod=p.pflcod 
                                        AND 
                                        p.usucpf='{$_SESSION['usucpf']}')
                    {$filtro})
                {$unionAbaAtual}
                ) aba
                ORDER BY 
                        mnucod, mnuid";

            $RS = $this->carregar($sql);

            if(is_array($RS)) {
                print '<ul class="nav nav-tabs" style="margin-bottom: -1px;">';
                $nlinhas = count($RS)-1;
                $qtdAgregados = 0;
                for ($j=0; $j<=$nlinhas;$j++) {
                    foreach($RS[$j] as $k=>$v){
                        ${$k}=$v;
                    }

                    if($agregado == 'f'){
                        $numAba = $numerarAbas ? ($j-$qtdAgregados+1) . '. ' : '';
                    }else{
                        $numAba = $numerarAbas ? ($j) . '.1. ' : '';
                        $qtdAgregados++;
                    }

                    $parametro = is_array( $parametros ) ? $parametros[$j] : $parametros;

                    $classe = "";
                    if ($url==$mnulink) {
                        $classe = "active";
                    }


                    if ($mnulink<>$url){
                        print <<<ABA
                    <li class="{$classe}">
                        <a href="{$mnulink}{$parametro}">{$numAba}{$mnudsc} </a>
                    </li>
ABA;
                    } else {
                        print <<<ABA
                    <li class="{$classe}">
                        <a>{$numAba}{$mnudsc}</a>
                    </li>
ABA;
                    }
                }

                print '</ul>';
            }
        }
    }

    /*
 * Monta um arquivo Excel a partir de uma Query
 *
 * @param $sql String Query a ser executada
 * @param $arquivo String prefixo do arquivo a ser gerado
 * @param $cabecalho Array Opcional, nome dos campos no cabecalho do arquivo
 * @param $formatocoluna Array pode ser n (Numero) ou s (String)
 */
    function sql_to_excel($sql,$arquivo,$cabecalho="",$formatocoluna="") {
        // este método transforma uma query em excel
        global $nomeDoArquivoXls;
        $nomeDoArquivoXls = "excel_".date("His")."_".$arquivo;

        $xls = new GeraExcel();
        $RS= !is_array( $sql ) ? $this->carregar($sql) : $sql;
        $nlinhas = $RS ? count($RS) : 0;
        if (! $RS) $nl = 0; else $nl=$nlinhas;
        if ($nlinhas>0)
        {
            //Monta Cabeçalho
            if(is_array($cabecalho))
            {
                for ($i=0;$i<count($cabecalho);$i++)
                {
                    $xls->MontaConteudoString(0, $i, $cabecalho[$i]);
                }
            }
            else
            {
                $col=0;
                $lin=0;
                foreach($RS[0] as $k=>$v)
                {
                    $xls->MontaConteudoString($lin, $col, $k);
                    $col++;
                }
            }
            //Monta Listagem
            for ($i=0;$i<$nlinhas;$i++)
            {
                $lin = $i+1;
                $col = 0;
                foreach($RS[$i] as $k=>$v)
                {
                    if ($formatocoluna[$col]=='n')
                        $xls->MontaConteudoNumero($lin, $col, $v);
                    else
                        $xls->MontaConteudoString($lin, $col, $v);
                    $col++;
                }
            }
            $xls->GeraArquivo();
        }
    }


    function monta_lista_tabulado($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S', $totalregistro=false , $arrHeighTds = false , $heightTBody = false, $boImprimiTotal = false ) {
        if(!(bool)$largura) $largura = '95%';
        // este método monta uma listagem na tela baseado na sql passada
        //Registro Atual (instanciado na chamada)
        if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

        if (is_array($sql))
            $RS = $sql;
        else
            $RS = $this->carregar($sql);

        $nlinhas = $RS ? count($RS) : 0;
        $totalRegistro = $nlinhas;
        if (! $RS) $nl = 0; else $nl=$nlinhas;
        if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;

//			$header .= '<table>';

        //Monta Cabeçalho
        if(is_array($cabecalho))
        {
//				$header.= '<tr>';
//				$header.= chr(13).chr(10);
            for ($i=0;$i<count($cabecalho);$i++)
            {
                $header.= $cabecalho[$i].(($i===$totalregistro)?' (Total:'.count($RS).')':''). chr(9);
            }
        }

//			$header.= "</table>";

        if ($nlinhas>0)
        {

//				$body.= '<table>';

            //Monta Listagem
            $totais = array();
            $tipovl = array();
            for ($i=($numero-1);$i<$reg_fim;$i++)
            {
                $c = 0;
                $body .= chr(13);
                foreach($RS[$i] as $k=>$v) {
                    $body.= $v;
                    $body .= chr(9);
                    $c = $c + 1;
                }
//					$body .= chr(13);
            }

//		        $body.= '</table>';

        }

        echo $header;
        echo $body;

    }

    function monta_lista_simples($sql,$cabecalho="",$perpage,$pages,$soma='N',$largura='95%', $valormonetario='S', $totalregistro=false , $arrHeighTds = false , $heightTBody = false, $boImprimiTotal = false ) {

        if(!(bool)$largura) $largura = '95%';
        // este método monta uma listagem na tela baseado na sql passada
        //Registro Atual (instanciado na chamada)
        if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);

        if (is_array($sql))
            $RS = $sql;
        else
            $RS = $this->carregar($sql);

        $nlinhas = $RS ? count($RS) : 0;
        $totalRegistro = $nlinhas;
        if (! $RS) $nl = 0; else $nl=$nlinhas;
        if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;

        if( is_array( $arrHeighTds ) && is_int($heightTBody) ){

            $header = "<div>";
            $header .= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';

            //Monta Cabeçalho
            if(is_array($cabecalho))
            {
                $header.= '<thead><tr>';
                for ($i=0;$i<count($cabecalho);$i++)
                {
                    $header.= '<td align="center" width='. $arrHeighTds[$i] .'% valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].(($i===$totalregistro)?' (Total:'.count($RS).')':'').'</label>';
                }
                $header.= '<td><div style="width:10px" ></div></td>';
                $header.= '</tr> </thead>';
            }

            $header.= "</table>";
            $header.= "</div>";


            if ($nlinhas>0)
            {

                $body = "<div style=\"margin: 0px 0px 0px 0px; padding: 0px 0px 0px 0px ; overflow-y:auto;height:$heightTBody\" >";

                $body.= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';

                //Monta Listagem
                $totais = array();
                $tipovl = array();
                for ($i=($numero-1);$i<$reg_fim;$i++)
                {
                    $c = 0;
                    if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
                    $body.= '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
                    foreach($RS[$i] as $k=>$v) {
                        // Clausula que faz soma numeros monetários
                        if (!is_numeric($v)) {
                            if (is_numeric(str_replace(array(".", ","),array("", "."),$v))) {
                                $v = str_replace(array(".", ","),array("", "."),$v);
                                $valormonetario="S";
                            }
                        }
                        // fim - Clausula que faz soma numeros monetários
                        if (is_numeric($v))
                        {
                            //cria o array totalizador
                            if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
                            //Mostra o resultado
                            if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
                            if ($v<0) $body.= '<td width='. $arrHeighTds[$c] .'% align="right" valign="top" style="color:#cc0000;" title="'.$cabecalho[$c].'">('.$v.')'; else $body.= '<td width='. $arrHeighTds[$c] .'% align="right" valign="top" style="color:#999999;" title="'.$cabecalho[$c].'">'.$v;
                            $body.= ('<br>'.$totais[$c]);
                        }
                        elseif( strpos(trim($v),"<tr") > 0){
                            $body.= $v;
                        }else{
                            $body.= '<td width='. $arrHeighTds[$c] .'% valign="top" title="'.$cabecalho[$c].'">'.$v;
                        }
                        $body.= '</td>';
                        $c = $c + 1;
                    }
                    $body.= '</tr>';
                }

                $body.= '</tbody>';
                $body.= '</table>';


                $body.= "</div>";


            }
            else
            {

                $body = "<div>";

                $body.= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';

                $body.= "<tr>";

                $body.= '<td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>';

                $body.= "</tr>";

                $body.= "<div>";


            }

            //inicio totais
            $totaisN = "<div>";
            $totaisN.= '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
            $somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
            if ($soma=='S' || $somarCampos){
                //totaliza (imprime totais dos campos numericos)
                $totaisN.= '<tr bgcolor=#DEDEDE >';
                for ($i=0;$i<$c;$i++)
                {

                    $totaisN.= '<td width='. $arrHeighTds[$i] .'% align="right" title="'.$cabecalho[$i].'">';
                    if ($i==0) $totaisN .= 'Totais:   ';
                    if(($somarCampos && $soma[$i]) || $soma=='S') {
                        if (is_numeric($totais['0'.$i]))
                            if($valormonetario == 'S'){
                                $totaisN.= "<b>".number_format($totais['0'.$i], 2, ',', '.')."</b>";
                            }else{
                                $totaisN.= "<b>".number_format( $totais['0'.$i], 0, ',', '.')."</b>";
                            }
                        else
                            $totaisN.= $totais['0'.$i];
                    }
                    $totaisN.= '</td>';

                }
                $totaisN.= '<td><div style="width:10px" ></div></td>';
                $totaisN.= '</tr>';
            }
            $totaisN.= "</table>";
            $totaisN.= "</div>";
            //fim totais


            echo $header;
            echo $body;
            echo $totaisN;
            if( $boImprimiTotal && $totalRegistro != 0 )
                print '<div>
					<table width="'.$largura.'" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
						<tr bgcolor="#ffffff">
							<td><b>Total de Registros: ' . $totalRegistro . '</b></td>
						<tr>
					</table>
					</div>';


        }else{

            print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
            if ($nlinhas>0)
            {

                //Monta Cabeçalho
                if(is_array($cabecalho))
                {
                    print '<thead><tr>';
                    for ($i=0;$i<count($cabecalho);$i++)
                    {
                        print '<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cabecalho[$i].(($i===$totalregistro)?' (Total:'.count($RS).')':'').'</label>';
                    }
                    print '</tr> </thead>';
                }

                echo '<tbody>';

                //Monta Listagem
                $totais = array();
                $tipovl = array();

                for ($i=($numero-1);$i<$reg_fim;$i++)
                {
                    $c = 0;
                    if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
                    print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
                    foreach($RS[$i] as $k=>$v) {
                        // Clausula que faz soma numeros monetários
                        if (!is_numeric($v)) {
                            if (is_numeric(str_replace(array(".", ","),array("", "."),$v))) {
                                $v = str_replace(array(".", ","),array("", "."),$v);
                                $valormonetario="S";
                            }
                        }
                        // fim - Clausula que faz soma numeros monetários
                        if (is_numeric($v))
                        {
                            //cria o array totalizador
                            if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
                            //Mostra o resultado
                            if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
                            if ($v<0) print '<td align="right" valign="top" style="color:#cc0000;" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="right" valign="top" style="color:#999999;" title="'.$cabecalho[$c].'">'.$v;
                            print ('<br>'.$totais[$c]);
                        }
                        elseif( strpos(trim($v),"<tr") > 0){
                            print $v;
                        }else print '<td valign="top" title="'.$cabecalho[$c].'">'.$v;
                        print '</td>';
                        $c = $c + 1;
                    }
                    print '</tr>';
                }

                print '</tbody>';

                $somarCampos = $soma!='S' && is_array($soma) && (@count($soma)>0);
                if ($soma=='S' || $somarCampos){
                    //totaliza (imprime totais dos campos numericos)
                    print '<tfoot><tr>';
                    for ($i=0;$i<$c;$i++)
                    {
                        print '<td align="right" title="'.$cabecalho[$i].'">';
                        if ($i==0) print 'Totais:   ';
                        if(($somarCampos && $soma[$i]) || $soma=='S') {
                            if (is_numeric($totais['0'.$i]))
                                if($valormonetario == 'S'){
                                    print number_format($totais['0'.$i], 2, ',', '.');
                                }else{
                                    print number_format( $totais['0'.$i], 0, ',', '.');
                                }
                            else
                                print $totais['0'.$i];
                        }
                        print '</td>';
                    }
                    print '</tr></tfoot>';
                    //fim totais
                }

            }
            else {
                print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
            }
            print '</table>';

            if( $boImprimiTotal && $totalRegistro != 0 )
                print '<div>
				<table width="'.$largura.'" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<tr bgcolor="#ffffff">
						<td><b>Total de Registros: ' . $totalRegistro . '</b></td>
					<tr>
				</table>
				</div>';
        }
    }


    /**
     * Monta uma lista com sumário a partir de um array multidimensional
     *
     */
    function monta_lista_agrupado($arrRsAgrupado, $cabecalho="",$soma='',$largura='95%') {
        if(!(bool)$largura) $largura = '95%';

        $somarCampos = $soma!='' && is_array($soma) && (@count($soma)>0);
        $nlinhas = $arrRsAgrupado ? count($arrRsAgrupado) : 0;

        print '<table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">';
        if ($nlinhas>0)
        {
            // Monta Cabeçalho
            if(is_array($cabecalho))
            {
                print '<thead><tr>';
                foreach($cabecalho as $cab) {
                    print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$cab."</td>";
                }
                print '</tr></thead>';
            }

            // Monta Listagem
            $totais = array();
            foreach($arrRsAgrupado as $grupo=>$dados) {
                foreach ($dados as $pos=>$linha) {
                    $marcado = $pos % 2 ? '' : '#F7F7F7';
                    echo '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';
                    $i=0;
                    foreach($linha as $col=>$val) {
                        if(!isset($totais[$grupo][$col]))
                            $totais[$grupo][$col] = 0;

                        if (!is_numeric($val)) {
                            print '<td title="'.$cabecalho[$i].'">'.$val.'</td>';
                        }
                        else {
                            $totais[$grupo][$col] += $val;
                            $valTela = $somarCampos && $soma[$i] ? number_format($val, 2, ',', '.') : $val;

                            if ($val<0) {
                                echo '<td align="right" style="color:#cc0000;" title="'.$cabecalho[$i].'">(' . $valTela . ')</td>';
                            }
                            else {
                                echo '<td align="right" style="color:#999999;" title="'.$cabecalho[$i].'">'. $valTela .'</td>';
                            }
                        }
                        $i++;
                    }
                    echo "</tr>\n";
                }
                //
                // Subtotal
                if ($somarCampos) {
                    print '<thead><tr>';
                    $i=0;
                    foreach ($totais[$grupo] as $col=>$total) {
                        print '<td align="right" title="'.$cabecalho[$i].'">';
                        if ($i==0) print 'Subtotal:   ';
                        if ($somarCampos && $soma[$i]) {
                            if (is_numeric($total))
                                print number_format($total, 2, ',', '.');
                            else
                                print $total;
                        }
                        else {
                            print " ";
                        }
                        print '</td>';
                        $i++;
                    }
                    print '</tr></thead>';
                }
            }
            //
            // Total geral
            if ($somarCampos) {
                $totalGeral = array();
                foreach ($totais as $grupo=>$colunas) {
                    foreach ($colunas as $col=>$total) {
                        $totalGeral[$col] += $total;
                    }
                }

                $i=0;
                print '<tr><td colspan="' . count($cabecalho) . '"> </td></tr>';
                print '<thead><tr>';

                foreach($totalGeral as $col=>$total) {
                    print '<td align="right" title="'.$cabecalho[$i].'">';
                    if ($i==0) print '<b>Total geral:</b>   ';
                    if ($somarCampos && $soma[$i]) {
                        if (is_numeric($total))
                            print number_format($total, 2, ',', '.');
                        else
                            print $total;
                    }
                    else {
                        print " ";
                    }
                    print '</td>';
                    $i++;
                }
                print '</tr></thead>';
            }
        }
        else {
            print '<tr><td align="center" style="color:#cc0000;">não foram encontrados registros.</td></tr>';
        }
        print '</table>';
    }

    function busca_pai($cod,$i)
    {
        // busca, num plano de trabalho, se o registro está subordinado
        $sql = "select ptoid_pai from planotrabalho where ptostatus='A' and ptoid=".$cod;
        //print $sql;
        //exit();
        $RS = $this->record_set($sql);
        $res = $this->carrega_registro($RS,0);
        if ($res['ptoid_pai']<>'')
            return 2;
        else
        {
            // verifico se tem filho alm de no ter pai
            $sql = "select ptoid from planotrabalho where ptostatus='A' and ptoid_pai=".$cod;
            $RS = $this->record_set($sql);
            $nlinhas = $this->conta_linhas($RS);
            if ($nlinhas < 0)
                return 1;
            else return 0;
        }

    }

    function monta_lista($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth="",$celAlign="") {
        // este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
        //$sql = Texto - sql que vai gerar a lista
        //$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
        //Parmetros de paginao
        //$perpage = Numrico - Registros por pgina
        //$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
        // $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
        // $ordem = alinhamento dos títulos (left, rigth, center)
        // $valormonetario = Define se o valor a ser tratado é monetário (com casas decimais) ou numérico (sem casas decimais)
        //$nomeformulario = Caso seja necessário um formulário para acessar objetos dentro da tabela, informar o nome do formulário, que o componente irá criar um <form name="nomedoformularioinformado">...</form>
        //Registro Atual (instanciado na chamada)
        //$sql = str_replace(";", "", $sql);

        /**************************************************************************************************
        -> NOVOS PARAMETROS (programador: Felipe Carvalho / data: 24/07/2009)
        $celWidth => array com o width(tamanho) de cada célula. Pode-se usar qualquer unidade (px, %, pt, etc)
        $celAlign => array com o alinhamento de cada célula.

        NOS DOIS PARAMETROS O TAMANHO DO ARRAY DEVE SER O MESMO DO
        NUMERO DE ELEMENTOS QUE IRAO COMPOR A LISTA.

        PARAMETROS NAO OBRIGATORIOS!
        SE NAO FOREM INFORMADOS USA-SE O PADRAO DO COMPONENTE.

        EXEMPLO:
        $cabecalho 		= array( "Comando", "Ação");
        $tamanho		= array( '10%', '90%' );
        $alinhamento	= array( 'center', 'left' );
        $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
         **************************************************************************************************/

        if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);
        //Controla o Order by
        if (!is_array($sql) && $_REQUEST['ordemlista']<>'')
        {
            if ($_REQUEST['ordemlistadir'] <> 'DESC') {$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';} else {$ordemlistadir = 'DESC'; $ordemlistadir2 = 'ASC';}
            $subsql = substr($sql,0,strpos(trim(strtoupper($sql)),'ORDER '));
            $sql = (!$subsql ? $sql : $subsql).' order by '.$_REQUEST['ordemlista'].' '.$ordemlistadir;
        }

        if (is_array($sql)){
            $RS = $sql;
        }else{
            $sql = trim($sql);
            $char = substr($sql, -1);
            if ($char == ";") $sql = substr($sql, 0, -1);

            $sqlCount = "select
						count(1)
					 from (" . $sql . ") rs";

            $totalRegistro = $this->pegaUm($sqlCount);

            $sql = $sql . " LIMIT {$perpage} offset ".($numero-1);

            $RS = $this->carregar($sql);
        }
        $nlinhas = count($RS);

        if (! $RS) $nl = 0; else $nl=$nlinhas;
//	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
        $reg_fim = $nlinhas;
        if ($nl>0)
        {
            $total_reg = $totalRegistro;

            //monta o formulario da lista mantendo os parametros atuais da pgina
            print '<form name="formlista" method="post"><input type="Hidden" name="numero" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';
            foreach($_POST as $k=>$v){if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero') print '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';}
            print '</form>';

            if($nomeformulario != "")
                print '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'" enctype="multipart/form-data" action="" method="post">';

            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            //Monta Cabealho
            if ( $cabecalho === null ) {

            }else if(is_array($cabecalho))
            {
                print '<thead><tr class="head">';
                for ($i=0;$i<count($cabecalho);$i++)
                {
                    if ($_REQUEST['ordemlista'] == ($i+1)) {
                        $ordemlistadirnova = $ordemlistadir2;
                        $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                    } else {
                        $ordemlistadirnova = 'ASC';
                        $imgordem = '';
                    }
                    print '<th align="' . $alinha . '" valign="top" class="title" style="" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$cabecalho[$i].'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label></th>';
                }
                print '</tr></thead>';
            }
            else
            {
                print '<thead><tr class="head">'; $i=0;
                foreach($RS[0] as $k=>$v)
                {
                    if ($_REQUEST['ordemlista'] == ($i+1)) {
                        $ordemlistadirnova = $ordemlistadir2;
                        $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                    } else {
                        $ordemlistadirnova = 'ASC';
                        $imgordem = '';}
                    print '<th valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$k.'">'.$imgordem.'<strong>'.$k.'</strong></label></th>';
                    $i=$i+1;}
                print '</tr> </thead>';
            }
            //Monta Listagem
            $totais = array();
            $tipovl = array();
//		for ($i=($numero-1);$i<$reg_fim;$i++)

            for ($i=0; $i < $reg_fim; $i++)
            {
                $c = 0;
                if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
                print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';

                // contador -> numero de celulas
                $numCel = 0;
                foreach($RS[$i] as $k=>$v) {
                    // Setando o alinhamento da célula usando o array $celAlign.
                    // Se não for passado o parâmetro, usa o padrão do componente.
                    if(is_array($celAlign)) {
                        $alignNumeric = $alignNotNumeric = $celAlign[$numCel];
                    } else {
                        $alignNumeric 		= 'right';
                        $alignNotNumeric	= 'left';
                    }
                    // Setando o tamanho da célula usando o array $celWidth.
                    // Se não for passado o parâmetro, usa o padrão do componente.
                    $width = (is_array($celWidth)) ? 'width="'.$celWidth[$numCel].'"' : '';

                    if (is_numeric($v))
                    {
                        //cria o array totalizador
                        if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
                        //Mostra o resultado
                        if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
                        if ($v<0) print '<td align="'.$alignNumeric.'" '.$width.' style="color:#cc0000;" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="'.$alignNumeric.'" '.$width.' style="color:#0066cc;" title="'.$cabecalho[$c].'">'.$v;
                        print ('<br>'.$totais[$c]);
                    }
                    else {
                        print '<td align="'.$alignNotNumeric.'" '.$width.' title="'.$cabecalho[$c].'">'.$v;
                    }
                    print '</td>';
                    $c = $c + 1;
                    $numCel++;
                }
                print '</tr>';
            }

            if ($soma=='S'){
                //totaliza (imprime totais dos campos numericos)
                print '<thead><tr>';
                for ($i=0;$i<$c;$i++)
                {
                    print '<td align="right" title="'.$cabecalho[$i].'">';

                    if ($i==0) print 'Totais:   ';
                    if (is_numeric($totais['0'.$i])) {

                        if($valormonetario == 'S'){
                            print number_format($totais['0'.$i], 2, ',', '.');
                        }else{
                            print $totais['0'.$i];
                        }
                    }
                    else print $totais['0'.$i];
                    print '</td>';
                }
                print '</tr>';
                //fim totais
            }

            print '</table>';

            if($nomeformulario != "")
                print '</form>';

            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';

            include APPRAIZ."includes/paginacao.inc";
            print '</td></tr></table>';
            print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
        }
        else
        {
            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
            print '</table>';
        }
    }

    // SEM BUG ORBER BY
    // Para que funcione, coloque um GROUP BY em sua SQL
    function monta_lista_ordenaGROUPBY($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth="",$celAlign="") {
        // este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
        //$sql = Texto - sql que vai gerar a lista
        //$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
        //Parmetros de paginao
        //$perpage = Numrico - Registros por pgina
        //$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
        // $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
        // $ordem = alinhamento dos títulos (left, rigth, center)
        // $valormonetario = Define se o valor a ser tratado é monetário (com casas decimais) ou numérico (sem casas decimais)
        //$nomeformulario = Caso seja necessário um formulário para acessar objetos dentro da tabela, informar o nome do formulário, que o componente irá criar um <form name="nomedoformularioinformado">...</form>
        //Registro Atual (instanciado na chamada)
        //$sql = str_replace(";", "", $sql);

        /**************************************************************************************************
        -> NOVOS PARAMETROS (programador: Felipe Carvalho / data: 24/07/2009)
        $celWidth => array com o width(tamanho) de cada célula. Pode-se usar qualquer unidade (px, %, pt, etc)
        $celAlign => array com o alinhamento de cada célula.

        NOS DOIS PARAMETROS O TAMANHO DO ARRAY DEVE SER O MESMO DO
        NUMERO DE ELEMENTOS QUE IRAO COMPOR A LISTA.

        PARAMETROS NAO OBRIGATORIOS!
        SE NAO FOREM INFORMADOS USA-SE O PADRAO DO COMPONENTE.

        EXEMPLO:
        $cabecalho 		= array( "Comando", "Ação");
        $tamanho		= array( '10%', '90%' );
        $alinhamento	= array( 'center', 'left' );
        $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
         **************************************************************************************************/

        if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);
        //Controla o Order by
        if (!is_array($sql) && $_REQUEST['ordemlista']<>'')
        {
            if ($_REQUEST['ordemlistadir'] <> 'DESC') {$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';} else {$ordemlistadir = 'DESC'; $ordemlistadir2 = 'ASC';}
            $qtd = strlen($sql);
            $achou = 0;
            for($x=1;$achou!=1;$x++){
                $pos     = strpos(substr($sql,($qtd-(10*$x)),$qtd-($qtd-(10*$x))),'ORDER ');
                $posFrom = strpos(substr($sql,($qtd-(10*$x)),$qtd-($qtd-(10*$x))),'GROUP BY ');
                if($pos){
                    $pos = ($qtd-(10*$x)) + $pos;
                    $achou = 1;
                }
                if($posFrom){
                    $achou = 1;
                }
            }
            if($pos){
                $subsql = substr($sql,0,$pos);
            }else{
                $subsql = false;
            }
            $sql = (!$subsql ? $sql : $subsql).' order by '.$_REQUEST['ordemlista'].' '.$ordemlistadir;
        }

        if (is_array($sql)){
            $RS = $sql;
        }else{
            $sql = trim($sql);
            $char = substr($sql, -1);
            if ($char == ";") $sql = substr($sql, 0, -1);

            $sqlCount = "select
							count(1)
						 from (" . $sql . ") rs";

            $totalRegistro = $this->pegaUm($sqlCount);

            $sql = $sql . " LIMIT {$perpage} offset ".($numero-1);

            $RS = $this->carregar($sql);
        }
        $nlinhas = count($RS);

        if (! $RS) $nl = 0; else $nl=$nlinhas;
        //	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
        $reg_fim = $nlinhas;
        if ($nl>0)
        {
            $total_reg = $totalRegistro;

            //monta o formulario da lista mantendo os parametros atuais da pgina
            print '<form name="formlista" method="post"><input type="Hidden" name="numero" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';
            foreach($_POST as $k=>$v){if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero') print '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';}
            print '</form>';

            if($nomeformulario != "")
                print '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'" action="" enctype="multipart/form-data" method="post">';

            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            //Monta Cabealho
            if ( $cabecalho === null ) {

            }else if(is_array($cabecalho))
            {
                print '<thead><tr>';
                for ($i=0;$i<count($cabecalho);$i++)
                {
                    if ($_REQUEST['ordemlista'] == ($i+1)) {
                        $ordemlistadirnova = $ordemlistadir2;
                        $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                    } else {
                        $ordemlistadirnova = 'ASC';
                        $imgordem = '';
                    }
                    print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$cabecalho[$i].'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
                }
                print '</tr> </thead>';
            }
            else
            {
                print '<thead><tr>'; $i=0;
                foreach($RS[0] as $k=>$v)
                {
                    if ($_REQUEST['ordemlista'] == ($i+1)) {
                        $ordemlistadirnova = $ordemlistadir2;
                        $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                    } else {
                        $ordemlistadirnova = 'ASC';
                        $imgordem = '';}
                    print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$k.'">'.$imgordem.'<strong>'.$k.'</strong></label>';
                    $i=$i+1;}
                print '</tr> </thead>';
            }
            //Monta Listagem
            $totais = array();
            $tipovl = array();
            //		for ($i=($numero-1);$i<$reg_fim;$i++)

            for ($i=0; $i < $reg_fim; $i++)
            {
                $c = 0;
                if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
                print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';

                // contador -> numero de celulas
                $numCel = 0;
                foreach($RS[$i] as $k=>$v) {
                    // Setando o alinhamento da célula usando o array $celAlign.
                    // Se não for passado o parâmetro, usa o padrão do componente.
                    if(is_array($celAlign)) {
                        $alignNumeric = $alignNotNumeric = $celAlign[$numCel];
                    } else {
                        $alignNumeric 		= 'right';
                        $alignNotNumeric	= 'left';
                    }
                    // Setando o tamanho da célula usando o array $celWidth.
                    // Se não for passado o parâmetro, usa o padrão do componente.
                    $width = (is_array($celWidth)) ? 'width="'.$celWidth[$numCel].'"' : '';

                    if (is_numeric($v))
                    {
                        //cria o array totalizador
                        if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
                        //Mostra o resultado
                        if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
                        if ($v<0) print '<td align="'.$alignNumeric.'" '.$width.' style="color:#cc0000;" title="'.$cabecalho[$c].'">('.$v.')'; else print '<td align="'.$alignNumeric.'" '.$width.' style="color:#0066cc;" title="'.$cabecalho[$c].'">'.$v;
                        print ('<br>'.$totais[$c]);
                    }
                    else {
                        print '<td align="'.$alignNotNumeric.'" '.$width.' title="'.$cabecalho[$c].'">'.$v;
                    }
                    print '</td>';
                    $c = $c + 1;
                    $numCel++;
                }
                print '</tr>';
            }

            if ($soma=='S'){
                //totaliza (imprime totais dos campos numericos)
                print '<thead><tr>';
                for ($i=0;$i<$c;$i++)
                {
                    print '<td align="right" title="'.$cabecalho[$i].'">';

                    if ($i==0) print 'Totais:   ';
                    if (is_numeric($totais['0'.$i])) {

                        if($valormonetario == 'S'){
                            print number_format($totais['0'.$i], 2, ',', '.');
                        }else{
                            print $totais['0'.$i];
                        }
                    }
                    else print $totais['0'.$i];
                    print '</td>';
                }
                print '</tr>';
                //fim totais
            }

            print '</table>';

            if($nomeformulario != "")
                print '</form>';

            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';

            include APPRAIZ."includes/paginacao.inc";
            print '</td></tr></table>';
            print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
        }
        else
        {
            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
            print '</table>';
        }
    }



    /*
 * função responsavel pela geração de lista agrupada
 *
 *
*/

    /*	 este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
 *	$sql = Texto - sql que vai gerar a lista
 *	$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
 *	Parmetros de paginao
 *	$perpage = Numrico - Registros por pgina
 *	$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
 *	 $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
 *	 $par2 = Reservado para o futuro
 *	$nomeformulario = Caso seja necessário um formulário para acessar objetos dentro da tabela, informar o nome do formulário, que o componente irá criar um <form name="nomedoformularioinformado">...</form>
 *	Registro Atual (instanciado na chamada)
 *	$sql = str_replace(";", "", $sql);
 *
 *	$nomeformulario = nome do formulario.
 *	$grupo = nome da coluna que realizará o agrupamento
 *	$ordem = neste parametro e necessário passar um array: $ordem = array("nomeDoEsquena.nomeDaTabela", "nomeDaColunaOrdem", "nomeDoId")
 *
 *  segue exemplo abaixo para utilização desse parametro
 *
 *	Obs.: para utilizar este componente e necessário utilizar alias na query,
 * 		 exemplo de query para um bom funcionamento:
 * 			 |------------|
 * 			 |  tblTeste  |
 * 			 |------------|
 * 			 | id		  |
 * 			 | nome		  |
 * 			 | sexo		  |
 * 			 | ordem	  |
 * 			 |------------|
 *
 * 			* select t.id, t.nome, t.sexo, t.id as ordem from teste t
 * 			   order by t.ordem
 *
 *    		* caso seja agrupado e so passar o nome da coluna que deseja realizar o agrupamento.
 */

    function monta_lista_grupo($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$par2,$nomeformulario="", $grupo="", $ordem=array() ) {
        if($_REQUEST['ordemId1'] && $_REQUEST['ordemId2'] && $_REQUEST['ordemTable'] && $_REQUEST['ordemAtributo'] && $_REQUEST['ordemId']){
            $this->mudaPosicao($_REQUEST['ordemId1'], $_REQUEST['ordemId2'], $_REQUEST['ordemTable'], $_REQUEST['ordemAtributo'], $_REQUEST['ordemId']);
        }

        if ($_REQUEST['numero']=='')
            $numero = 1;
        else
            $numero = intval($_REQUEST['numero']);
        //Controla o Order by
        if (!is_array($sql) && $_REQUEST['ordemlista']<>''){
            if ($_REQUEST['ordemlistadir'] <> 'DESC') {
                $ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';
            }else{
                $ordemlistadir = 'DESC'; $ordemlistadir2 = 'ASC';
            }
            $subsql = substr($sql,0,strpos(trim(strtoupper($sql)),'ORDER '));
            if($grupo){
                $sql = (!$subsql ? $sql : $subsql).' order by '.$grupo.', '.$_REQUEST['ordemlista'].' '.$ordemlistadir;
            }else{
                $sql = (!$subsql ? $sql : $subsql).' order by '.$_REQUEST['ordemlista'].' '.$ordemlistadir;
            }
        }

        if (is_array($sql)){
            $RS = $sql;
            $totalRegistro = count($sql);
        }else{
            $sql = trim($sql);
            $char = substr($sql, -1);
            if ($char == ";") $sql = substr($sql, 0, -1);

            $sqlCount = "select
						count(1)
					 from (" . $sql . ") rs";

            $totalRegistro = $this->pegaUm($sqlCount);

            $sql = $sql . " LIMIT {$perpage} offset ".($numero-1);

            $RS = $this->carregar($sql);
        }
        $nlinhas = count($RS);

        if (! $RS)
            $nl = 0;
        else
            $nl=$nlinhas;
//	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
        $reg_fim = $nlinhas;
        if ($nl>0)
        {
            $total_reg = $totalRegistro;

            //monta o formulario da lista mantendo os parametros atuais da pgina
            print '<form name="formlista" method="post">
				<input type="hidden" name="numero" value="'.$_REQUEST['numero'].'" />
				<input type="hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/>
				<input type="hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>

				<input type="hidden" name="ordemId1" value=""/>
				<input type="hidden" name="ordemId2" value=""/>
				<input type="hidden" name="ordemTable" value=""/>
				<input type="hidden" name="ordemAtributo" value=""/>
				<input type="hidden" name="ordemId" value=""/>';

            foreach($_POST as $k=>$v){
                if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero' and $k<>'ordemId1' and $k<>'ordemId2' and $k<>'ordemTable' and $k<>'ordemAtributo' and $k<>'ordemId' )
                    print '<input type="hidden" name="'.$k.'" value="'.$v.'"/>';
            }
            print '</form>';

            if($nomeformulario != "")
                print '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'">';

            print '<table width="95%" id="tabelalista" name="tabelalista" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            //Monta Cabealho
            if ( $cabecalho === null ) {

            }else if(is_array($cabecalho))
            {
                print '<thead><tr>';
                for ($i=0;$i<count($cabecalho);$i++)
                {
                    if ($_REQUEST['ordemlista'] == ($i+1)) {
                        $ordemlistadirnova = $ordemlistadir2;
                        $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                    } else {
                        $ordemlistadirnova = 'ASC';
                        $imgordem = '';
                    }
                    print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$cabecalho[$i].'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
                }
                print '</tr> </thead>';
            }
            else
            {
                print '<thead><tr>'; $i=0;
                foreach($RS[0] as $k=>$v)
                {
                    if ($_REQUEST['ordemlista'] == ($i+1)) {
                        $ordemlistadirnova = $ordemlistadir2;
                        $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                    } else {
                        $ordemlistadirnova = 'ASC';
                        $imgordem = '';}
                    print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$k.'">'.$imgordem.'<strong>'.$k.'</strong></label>';
                    $i=$i+1;}
                print '</tr> </thead>';
            }
            //Monta Listagem
            $totais = array();
            $tipovl = array();
//		for ($i=($numero-1);$i<$reg_fim;$i++)
            $w = 1;
            $contador = 0;
            $gruNomeAnt = "Nenhum Grupo";
            for ($i=0; $i < $reg_fim; $i++){
                $c = 0;
                if (fmod($i,2) == 0)
                    $marcado = '';
                else
                    $marcado='#F7F7F7';

                foreach($RS[$i] as $k=>$v) {

                    if($k != $grupo){
                        if($grupo){ //verifica se foi passado algum parametro para realizar o agrupamento
                            if( ($RS[$i][$grupo] != $gruNomeAnt) ){
                                $gruNomeAnt = $RS[$i][$grupo];
                                $w = $i;
                                $contador = 0;
                                if($gruNomeAnt){
                                    print "<tr bgcolor=''>";
                                    print "		<td class=\"title\" style=\"background: #CCCCCC;\" colspan=\"".count($cabecalho)."\"><strong>Grupo:</strong> <i>".$RS[$i][$grupo]."</i></td>";
                                    print "</tr>";
                                    print "<tr bgcolor='".$marcado."' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='".$marcado."'\">";

                                }else{
                                    print "<tr bgcolor=''>";
                                    print "		<td class=\"title\" style=\"background: #CCCCCC;\" colspan=\"".count($cabecalho)."\"><strong>Grupo:</strong> <i>Nenhum Grupo</i></td>";
                                    print "</tr>";
                                    print "<tr bgcolor='".$marcado."' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='".$marcado."'\">";
                                }
                            }
                        }

                        if( $i != $w  ){
                            print "<tr bgcolor='".$marcado."' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='".$marcado."'\">";
                            $w = $i;
                        }
                        if($grupo){ //verifica se foi passado algum parametro para realizar o agrupamento com ordenação
                            if($ordem[0]){
                                if($k == $ordem[1]){
                                    $contador++;
                                    $key = $i + 1;

                                    $grupoNome1 = $RS[$key][$grupo];

                                    print "<td style=\"text-align: center;\"><input type=\"hidden\" name=\"ordem_".$v."_ordem\" value=\"".$v."\" >";

                                    if( ($gruNomeAnt != $grupoNome1) || ($i == (count($RS) - 1) )){
                                        print "<img src=\"../imagens/seta_baixod.gif\" />";
                                    }else{
                                        print "<img onclick=\"mudaPosicao('tabelalista','baixo',this.parentNode.parentNode.rowIndex, '{$ordem[0]}', '{$ordem[1]}', '{$ordem[2]}');\" style=\"cursor: pointer;\" src=\"../imagens/seta_baixo.gif\" />";
                                    }

                                    if($contador == 1  ){
                                        print "<img src=\"../imagens/seta_cimad.gif\" />";
                                    }else {
                                        print "<img onclick=\"mudaPosicao('tabelalista','cima',this.parentNode.parentNode.rowIndex, '{$ordem[0]}', '{$ordem[1]}', '{$ordem[2]}');\" style=\"cursor: pointer;\" src=\"../imagens/seta_cima.gif\" />";
                                    }
                                    print "</td>";
                                    break;
                                }
                            }
                        }else{
                            if($ordem[0]){
                                if($k == $ordem[1]){
                                    $contador++;

                                    $key = $i + 1;

                                    print "<td style=\"text-align: center;\"><input type=\"hidden\" name=\"ordem_".$v."_ordem\" value=\"".$v."\" >";

                                    if( $i == (count($RS) - 1) ){
                                        print "<img src=\"../imagens/seta_baixod.gif\" />";
                                    }else{
                                        print "<img onclick=\"mudaPosicao('tabelalista','baixo',this.parentNode.parentNode.rowIndex, '{$ordem[0]}', '{$ordem[1]}', '{$ordem[2]}');\" style=\"cursor: pointer;\" src=\"../imagens/seta_baixo.gif\" />";
                                    }

                                    if($contador == 1  ){
                                        print "<img src=\"../imagens/seta_cimad.gif\" />";
                                    }else {
                                        print "<img onclick=\"mudaPosicao('tabelalista','cima',this.parentNode.parentNode.rowIndex, '{$ordem[0]}', '{$ordem[1]}', '{$ordem[2]}');\" style=\"cursor: pointer;\" src=\"../imagens/seta_cima.gif\" />";
                                    }
                                    print "</td>";
                                    break;
                                }
                            }
                        }
                        if (is_numeric($v)){
                            //cria o array totalizador

                            if (!$totais['0'.$c]) {
                                $coluna = array('0'.$c => $v);
                                $totais = array_merge($totais, $coluna);
                            }else
                                $totais['0'.$c] = $totais['0'.$c] + $v;
                            //Mostra o resultado
                            if (strpos($v,'.')) {
                                $v = number_format($v, 2, ',', '.');
                                if (!$tipovl['0'.$c]) {
                                    $coluna = array('0'.$c => 'vl');
                                    $tipovl = array_merge($totais, $coluna);
                                } else
                                    $tipovl['0'.$c] = 'vl';
                            }
                            if ($v<0)
                                print '<td align="right" style="color:#cc0000;" title="'.$cabecalho[$c].'">('.$v.')';
                            else
                                print '<td align="right" style="color:#0066cc;" title="'.$cabecalho[$c].'">'.$v;
                            print ('<br>'.$totais[$c]);
                        }
                        else{
                            print '<td title="'.$cabecalho[$c].'">'.$v;
                        }
                        print '</td>';
                        $c = $c + 1;
                    }
                }//if do grupo
                print '</tr>';
            }

            if ($soma=='S'){
                //totaliza (imprime totais dos campos numericos)
                if($ordem[0])
                    $c++;
                print '<thead><tr>';
                for ($i=0;$i<$c;$i++){

                    print '<td align="right" title="'.$cabecalho[$i].'">';

                    if ($i==0)
                        print 'Totais:   ';
                    if (is_numeric($totais['0'.$i]))
                        print number_format($totais['0'.$i], 2, ',', '.');
                    else
                        print $totais['0'.$i];
                    print '</td>';
                }
                print '</tr>';
                //fim totais
            }

            print '</table>';

            if($nomeformulario != "")
                print '</form>';

            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';

            include APPRAIZ."includes/paginacao.inc";
            print '</td></tr></table>';
            print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}

			function mudaPosicao(idTabela, tipo, rowIndex, table, atributoOrdem, id){

				var tabela = window.document.getElementById(idTabela);
				maxRows = tabela.rows.length - 1;

				if(tipo == "baixo"){
					var tr1 =  tabela.rows[rowIndex].innerHTML;
					var tr2 =  tabela.rows[rowIndex + 1].innerHTML;

					//Pegando id 1
					var ini1 = tr1.indexOf("ordem_");
					var fim1 = tr1.indexOf("_ordem");
					var id1 = tr1.substring(ini1,fim1);
					id1 = id1.replace(/ordem_/, "")

					//Pegando id 2
					var ini2 = tr2.indexOf("ordem_");
					var fim2 = tr2.indexOf("_ordem");
					var id2 = tr2.substring(ini2,fim2);
					id2 = id2.replace(/ordem_/, "");
				}
				//alert(rowIndex);
				if(tipo == "cima"){
					var tr1 =  tabela.rows[rowIndex].innerHTML;
					var tr2 =  tabela.rows[rowIndex - 1].innerHTML;

					//Pegando id 1
					var ini1 = tr1.indexOf("ordem_");
					var fim1 = tr1.indexOf("_ordem");
					var id1 = tr1.substring(ini1,fim1);
					id1 = id1.replace(/ordem_/, "")

					//Pegando id 2
					var ini2 = tr2.indexOf("ordem_");
					var fim2 = tr2.indexOf("_ordem");
					var id2 = tr2.substring(ini2,fim2);
					id2 = id2.replace(/ordem_/, "");
				}
				window.document.formlista.ordemId1.value = id1;
				window.document.formlista.ordemId2.value = id2;
				window.document.formlista.ordemTable.value = table;
				window.document.formlista.ordemAtributo.value = atributoOrdem;
				window.document.formlista.ordemId.value = id;
				window.document.formlista.ordemlista.value = "";

				window.document.formlista.submit();
			}
		</script>';
        }
        else
        {
            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
            print '</table>';
        }
    }


    function criarColunasDeOrdenacaoEmArray($arrayOriginal,$arrayDeTiposParaOrdenacao,$ordem) {
        $arrayInteger = array();
        $arrayDate = array();
        foreach( $arrayOriginal as $linhaOriginal ) {
            foreach( $linhaOriginal as $chaveOriginal => $valorOriginal ) {
                foreach( $arrayDeTiposParaOrdenacao as $linhaParaOrdenacao ) {
                    foreach( $linhaParaOrdenacao as $chaveParaOrdenacao => $tipoParaOrdenacao ) {
                        if( $chaveOriginal==$chaveParaOrdenacao && $chaveOriginal==$ordem ) {
                            if( $tipoParaOrdenacao == "integer" ) {
                                $chaveComplementar = $chaveOriginal . "_campoextra";
                                $arrayInteger[] = array( $chaveComplementar => ((int)$valorOriginal) );
                                $arrayOriginal = $this->merge($arrayInteger, $arrayOriginal);
                            }
                            if( $tipoParaOrdenacao == "date" && $valorOriginal != "" ) {
                                $dataPartes = explode("/", $valorOriginal);
                                $dataComoNumeroOrdenavel = preg_replace( "/[^\d]/", "", $dataPartes[2].$dataPartes[1].$dataPartes[0] );
                                $chaveComplementar = $chaveOriginal . "_campoextra";
                                $arrayDate[] = array( $chaveComplementar => ((int)$dataComoNumeroOrdenavel) );
                                $arrayOriginal = $this->merge($arrayDate, $arrayOriginal);
                            }
                        }
                    }
                }
            }
        }
        return $arrayOriginal;
    }

    function merge($array1, $array2) {
        if(sizeof($array1)>sizeof($array2)) {
            $size = sizeof($array1);
        }else{
            $a = $array1;
            $array1 = $array2;
            $array2 = $a;

            $size = sizeof($array1);
        }

        $keys2 = array_keys($array2);

        for($i = 0;$i<$size;$i++)
        {
            $array1[$keys2[$i]] = $array1[$keys2[$i]] + $array2[$keys2[$i]];
        }

        $array1 = array_filter($array1);
        return $array1;
    }


    function monta_lista_array($dados,$cabecalho="",$perpage,$pages,$soma,$alinha,$html=array(),$arrayDeTiposParaOrdenacao=array()) {
        // este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
        //$sql = Texto - sql que vai gerar a lista
        //$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
        //Parmetros de paginao
        //$perpage = Numrico - Registros por pgina
        //$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
        // $soma = Boleano - Mostra somatrio de campos numricos no fim da lista
        // $ordem = alinhamento dos títulos (left, rigth, center)
        //$html = passa-se um array com os respesctivos HTML's que seram substituidos pelos campos do resultado,
        //		  sendo que onde se quiser colocar o valor vindo do sql deve-se escrever {campo[1]},{campo[2]},... "começando de {campo[1]}"
        //		  EX.: <a href="javascript:void(0);" id="campo_popup_checkbox_{campo[0]}_1" onclick="{campo[3]}">{campo[1]} - {campo[2]}</a>
        //Registro Atual (instanciado na chamada)
        /**
        $arrayDeTiposParaOrdenacao = Vetor contendo campos e seus respectivos tipos especiais, permitindo a correta ordenação dos mesmos.
        Não há necessidade de passar todos os campos já presentes no vetor(array) de dados, mas somente aqueles que são diferentes do
        tipo textual (string).
        Para um dado array contendo todos os dados da lista:
        ...
        $dados_array[$posicaoNoLoop] = array("acao" 		=> $acao						  ,
        "num_processo" => $val['prcnumsidoc']		 ." " ,
        "interessado" 	=> $val['prcnomeinteressado']." " ,
        "prioridade" 	=> $val['prioridade']			  ,
        "data" 		=> $val['dataultimocadastro']	  ,
        "dttramite" 	=> $val['dttramite']			  ,
        "coord" 		=> $val['coodsc']				  ,
        "situacao" 	=> $val['esddsc']
        );
        }
        ...
        Passar também o parâmetro tal como (somente campos diferentes de 'string'):
        ...
        $arrayDeTiposParaOrdenacao = array();
        $arrayDeTiposParaOrdenacao[] = array( "data"   		 => "date"    );
        $arrayDeTiposParaOrdenacao[] = array( "num_processo" => "integer" );
        ...
         */

        if ($_REQUEST['numero']=='')
            $numero = 1;
        else
            $numero = intval($_REQUEST['numero']);
        //Controla o Order by
        if ($_REQUEST['ordemlista']<>'') {

            $dados = $this->criarColunasDeOrdenacaoEmArray($dados,$arrayDeTiposParaOrdenacao,$_REQUEST['ordemlista']);
            $campoParaOrdenacao = $_REQUEST['ordemlista'];
//			dbg($campoParaOrdenacao,1);

            foreach( $arrayDeTiposParaOrdenacao as $linhaParaOrdenacao ) {
                foreach( $linhaParaOrdenacao as $chaveParaOrdenacao => $tipoParaOrdenacao ) {
                    if( $chaveParaOrdenacao == $campoParaOrdenacao ) {
                        $campoParaOrdenacao .= "_campoextra";
                    }
                }
            }

            if ($_REQUEST['ordemlistadir'] <> 'DESC') {
                $ordemlistadir = 'ASC';
                $ordemlistadir2 = 'DESC';
            } else {
                $ordemlistadir = 'DESC';
                $ordemlistadir2 = 'ASC';
            }

            // 	Obter uma lista de colunas
            foreach ($dados as $key => $row) {
                $row = ereg_replace("[^a-zA-Z0-9_]", "", strtr($row[($campoParaOrdenacao)], "áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ", "aaaaeeiooouucAAAAEEIOOOUUC"));
                $crt[$key]  = $row;
            }

            switch($ordemlistadir) {
                case 'ASC':
                    array_multisort($crt, SORT_ASC , $dados);
                    break;
                case 'DESC':
                    array_multisort($crt, SORT_DESC, $dados);
                    break;
            }
        }

        $RS = $dados;

        $nlinhas = count($RS);
        if (! $RS)
            $nl = 0;
        else
            $nl=$nlinhas;
        if (($numero+$perpage)>$nlinhas)
            $reg_fim = $nlinhas;
        else
            $reg_fim = $numero+$perpage-1;
        if ($nl>0){

            $ordenador = array_keys($RS[0]);

            $total_reg = $nlinhas;
            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            //monta o formulario da lista mantendo os parametros atuais da pgina
            print '<form name="formlista" method="post">
						<input type="Hidden" name="numero" value="" />
						<input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/>
						<input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';

            foreach($_POST as $k=>$v){
                if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero')
                    if ( is_array($v) ){
                        while ($val = current($v)){
                            print '<input type="hidden" name="'.$k.'[]" value=\'' . $val . '\'/>';
                            next($v);
                        }
                    }else{
                        print '<input type="hidden" name="'.$k.'" value=\'' . $v . '\'/>';
                    }
            }

            print '</form>';
            //Monta Cabealho

            $campoOrdenacaoOriginal = $_REQUEST['ordemlista'];
            //$posicao_ordenador_original = $camposOrdenacaoSubstituicao['posicao_ordenador_original'];

            if ( $cabecalho === null ) {

            }else if(is_array($cabecalho)){
                print '<thead><tr>';
                for ($i=0;$i<count($cabecalho);$i++)
                {
                    if ($campoParaOrdenacao==($ordenador[$i]) || $campoOrdenacaoOriginal==($ordenador[$i]) ) {
                        $ordemlistadirnova = $ordemlistadir2;
                        $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                    } else {
                        $ordemlistadirnova = 'ASC';
                        $imgordem = '';
                    }
                    print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($ordenador[$i]).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$cabecalho[$i].'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
                }
                print '</tr> </thead>';
            }
            else
            {
                print '<thead><tr>'; $i=0;
                foreach($RS[0] as $k=>$v)
                {
                    if ($campoParaOrdenacao==($i+1) ) {
                        $ordemlistadirnova = $ordemlistadir2;
                        $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                    } else {
                        $ordemlistadirnova = 'ASC';
                        $imgordem = '';}
                    print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.$k.'">'.$imgordem.'<strong>'.$k.'</strong></label>';
                    $i=$i+1;}
                print '</tr> </thead>';
            }
            //Monta Listagem
            $totais = array();
            $tipovl = array();

            //Recebe padrão de substituição de HTML
            $search = array();
            for ($i=($numero-1);$i<$reg_fim;$i++)
            {
                $c = 0;
                if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
                print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';

                if ( count($RS[$i]) != count($search) ):
                    for ($z=1; $z <= count($RS[$i]); $z++){
                        $search[$z] = '{campo['.$z.']}';
                    }
                endif;
                $col = 0;
                foreach($RS[$i] as $k=>$v) {
                    if ( $col > count($cabecalho)-1 && count($cabecalho) ) break;

                    if (is_numeric($v)){
                        //cria o array totalizador
                        if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
                        //Mostra o resultado
                        if (strpos($v,'.')) {
                            $v = number_format($v, 2, ',', '.');
                            if (!$tipovl['0'.$c]) {
                                $coluna = array('0'.$c => 'vl');
                                $tipovl = array_merge($totais, $coluna);
                            }else{
                                $tipovl['0'.$c] = 'vl';
                            }
                        }
                        if ($v<0)
                            print '<td align="right" style="color:#cc0000;" title="'.$cabecalho[$c].'">('.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v).')';
                        else
                            print '<td align="right" style="color:#0066cc;" title="'.$cabecalho[$c].'">'.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v);

                        print ('<br>'.$totais[$c]);

                    }else{
                        print '<td title="'.$cabecalho[$c].'">'.( $html[$col] ? str_replace($search, $RS[$i], $html[$col]) : $v);
                    }
                    print '</td>';

                    $c = $c + 1;
                    $col++;
                }
                print '</tr>';
            }

            if ($soma=='S'){
                //totaliza (imprime totais dos campos numericos)
                print '<thead><tr>';
                for ($i=0;$i<$c;$i++)
                {
                    print '<td align="right" title="'.$cabecalho[$i].'">';

                    if ($i==0) print 'Totais:   ';
                    if (is_numeric($totais['0'.$i])) print number_format($totais['0'.$i], 2, ',', '.'); else print $totais['0'.$i];
                    print '</td>';
                }
                print '</tr>';
                //fim totais
            }

            print '</table>';
            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: '.$nl.'</b></td><td>';

            include APPRAIZ."includes/paginacao.inc";
            print '</td></tr></table>';
            print '<script language="JavaScript">';
            print 'function ordena(ordem, direcao) {';
            print 	'document.formlista.ordemlista.value=ordem;';
            print	'document.formlista.ordemlistadir.value=direcao;';
            print 	'document.formlista.submit()';
            print 	'} ';
            print 'function pagina(numero) {';
            print 	'document.formlista.numero.value=numero;';
            print	'document.formlista.submit();';
            print	'} ';
            print '</script>';
        }
        else
        {
            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
            print '</table>';
        }
    }


    function monta_radio($var,$sql,$habil,$op) {
        // este mtodo monta uma sequencia html radio onde:
        // var = Texto - Nome do campo Radio;
        // sql  = Texto - Consulta para gerar a lista;
        // op = 0, 1 ou 2, onde 0 - significa &nbsp&nbsp, 1 - significa <br>, 2 - significa <br><br>
        global ${$var};
        if ($habil <> 'N') $habil = '' ; else $habil='  disabled="disabled" ';
        $esp = '  ';
        if ($op == 1) $esp = '<br>' ;
        if ($op == 2) $esp = '<br><br>';
        $res=$this->carregar($sql);
        for ($i=0;$i<count($res);$i++) {
            if (${$var}== $res[$i]['codigo']) $sel = '  checked '; else $sel='';
            print "<input type='radio' value='".$res[$i]['codigo']."' name='".$var."'".$sel.$habil."/>".$res[$i]['descricao'].$esp;
        }
    }

    function record_set ($SQL)
    {
        $res = @$this->executar($SQL);
        return $res;
    }

    function carregaAgrupado($sql, $agrupador) {
        $rs = $this->record_set($sql);
        $retorno = array();
        while($linha = pg_fetch_assoc($rs)) {
            $retorno[$linha[$agrupador]][] = $linha;
        }
        return $retorno;
    }

    function conta_linhas ($recurso)
    {
        // este mtodo conta as linhas de um recurso e subtrai 1
        $linhas = pg_num_rows($recurso);
        if( $linhas >= 0 )
            return $linhas-1;
        else
            return -1;
        //           if (! is_array(pg_fetch_all($recurso))) return -1;
        //           else return count(pg_fetch_all($recurso)) -1;
    }

    function pegaLinha($SQL, $linha=0) {
        //Retorna um registro de uma query, a partir da coluna especificada
        if(($RS = $this->executar($SQL)) && (pg_num_rows($RS)>=1)) {
            return pg_fetch_assoc($RS, $linha);
        }
        else return false;
    }

    function pegaUm($SQL, $coluna=0) {
        //Retorna um registro de uma query, a partir da coluna especificada
        if(($RS = $this->executar($SQL)) && (pg_num_rows($RS)>=1)) {
            return pg_fetch_result($RS, 0, $coluna);
        }
        else return false;
    }

    /**
     * Retorna o ano do exercício ativo.
     */
    function pega_ano_atual()
    {
        static $ano = false;
        if ( !$ano )
        {
            $esquema = $_SESSION['sisdiretorio'];
            $tabela = "programacaoexercicio";
            $sql =
                " select count(*) " .
                " from pg_class pc " .
                " inner join pg_namespace ns on " .
                " ns.oid = pc.relnamespace and " .
                " ns.nspname='" . $esquema . "' " .
                " where " .
                " relname = '" . $tabela . "'";
            $existe = (boolean) $this->pegaUm( $sql );
            if ( $existe )
            {
                $sql =
                    " select prsano " .
                    " from " . $esquema . "." . $tabela .
                    " where prsexerccorrente = 't' " .
                    " order by prsano desc " .
                    " limit 1 ";
                $ano = (integer) $this->pegaUm( $sql );
            }
            if ( !$ano )
            {
                $ano = $this->pegaUm(
                    "select ano from public.anos where anosnatual = 't' order by ano desc limit 1"
                );
            }
        }
        return $ano;
    }

    /**
     * Captura o nome das colunas de uma tabela.
     *
     * @param string $table
     * @param string $schema
     * @return string[]
     */
    function pegarColunas( $table, $schema = 'public' )
    {
        $table = str_replace( "'", "\\'", $table );
        $schema = str_replace( "'", "\\'", $schema );
        $sql = "select column_name from information_schema.columns where table_schema = '" . $schema . "' and table_name = '" . $table . "'";
        $linhas = $this->carregar( $sql );
        if ( !$linhas )
        {
            return array();
        }
        $colunas = array();
        foreach ( $linhas as $linha )
        {
            array_push( $colunas, $linha['column_name'] );
        }
        return $colunas;
    }

    /**
     * Captura as tabelas de um schema, caso o schema não seja indicado
     * todas as tabelas são retornadas.
     *
     * @param string $schema
     * @return string[]
     */
    function pegarTabelas( $schema = null )
    {
        $where = '';
        if ( $schema != null )
        {
            $schema = str_replace( "'", "\\'", $schema );
            $where = " where table_schema = '" . $schema. "' ";
        }
        $sql = 'select distinct table_name from information_schema.columns ' . $where;
        $linhas = $this->carregar( $sql );
        if ( !$linhas )
        {
            return array();
        }
        $tabelas = array();
        foreach ( $linhas as $linha )
        {
            array_push( $tabelas, $linha['table_name'] );
        }
        return $tabelas;
    }

    function carrega_registro ( $recurso, $posicao = 0 )
    {
        // este metodo carrega em um array os campos de um registro
        $registro = array();
        if ( is_resource( $recurso ) && pg_num_rows( $recurso ) > 0 )
        {
            $registro = pg_fetch_array( $recurso, $posicao );
        }
        return $registro;
    }

    function carrega_tudo ($recurso)
    {
        // este mtodo carrega em um array os campos de um registro
        return pg_fetch_all($recurso);
    }

    /*  function sucesso( $modulo, $parametros='')
 {
 if(! $modulo) {
 $saida = "history.back();";
 }
 else {
 $saida = $_SESSION['sisdiretorio'].'/'.$_SESSION['sisdiretorio'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
 }

 header("Location: ../sucesso.php?saida=".urlencode($saida));
 exit();
 }
 */
    function sucesso($modulo, $parametros='', $msg = "Operação realizada com sucesso")
    {
        ?>
        <html>
        <head>
            <script>
                alert('<?= $msg; ?>');
            </script>
            <script>
                <?if(!$modulo) {?>
                history.back();
                <?}else{
                $saida = $_SESSION['sisarquivo'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
                ?>
                location.href="<?=$saida?>";
                <?}?>
            </script>
        </head>
        <body>

        </body>
        </html>
        <?
        exit();
    }

    function sucessom($modulo, $parametros = '', $msg = 'Operação realizada com sucesso')
    {
        addMsgInfo($msg);
        ?>
        <html>
        <head>
            <script>
                <?if(!$modulo) {?>
                history.back();
                <?}else{
                $saida = $_SESSION['sisarquivo'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
                ?>
                location.href="<?=$saida?>";
                <?}?>
            </script>
        </head>
        <body>
        </body>
        </html>
        <?
        exit();
    }

    function insucesso( $mensagem='', $parametros='', $modulo='', $acao = 'C' )
    {
        $modulo = $modulo ? $modulo : "inicio";
        $saida = $_SESSION['sisdiretorio'] . '/' . $_SESSION['sisdiretorio'] . ".php?modulo=" . $modulo . "&acao=" . $acao . $parametros;
        $url = "../insucesso.php?saida=" . urlencode( $saida )."&mensagem=" . $mensagem;
        ?>
        <html>
        <head>
            <script type="text/javascript">
                location.href = "<?= $url ?>";
            </script>
        </head>
        <body>

        </body>
        </html>
        <?
        exit();
    }

    function rollback() {
        pg_query($this->link, 'rollback; ');
        unset($_SESSION['transacao']);
    }

    function commit() {
        if ($_SESSION['usucpf']==$_SESSION['usucpforigem'] or $_SESSION['superuser'] ){
            pg_query($this->link, 'commit; ');
            if(is_resource($this->linkapoio)) pg_query($this->linkapoio, 'commit; ');
            unset($_SESSION['transacao']);
            return true;
        } else {
            pg_query($this->link, 'rollback; ');
            if(is_resource($this->linkapoio)) pg_query($this->linkapoio, 'rollback; ');
            unset($_SESSION['transacao']);
            return false;
        }
    }

    function close() {
        if ($_SESSION['transacao'])
        {
            pg_query($this->link, 'rollback; ');
            if(is_resource($this->linkapoio)) pg_query($this->linkapoio, 'rollback; ');
            unset($_SESSION['transacao']);
        }
//	if (is_resource($this->link)) {@pg_close( $this->link );}
//	if(is_resource($this->linkapoio)) {@pg_close( $this->linkapoio );}
    }
    /* Store login informations */
    Function StoreLogin($username,$password) {
        /* Generate a mask and store the result */
        $data = $username . chr(1) . $password;
        $key = $this->randomkey(32);
        /* Store encrypted username and password */
        $_SESSION[serverauth] = criptografia::rc4($key,$data,"en");
        /* Store the mask */
        SetCookie( "cookieauth",base64_encode($key),0,"/");
        return 1;
    }
    Function GetLogin(&$username,&$password) {
        if( strlen( $_SESSION[serverauth] ) > 0 and  strlen( $_COOKIE[cookieauth] ) > 0 ) {
            $key=base64_decode(stripslashes($_COOKIE[cookieauth]));
            if( strlen( $key ) > 0 ) {
                $edata=stripslashes($_SESSION[serverauth]);
                $data=criptografia::rc4($key,$edata,"de");
                if( strlen( $data ) > 0 ) {
                    $chr1pos = strpos( $data, chr( 1 ) );
                    if( $chr1pos > 0 ) {
                        $username = substr( $data, 0, $chr1pos );
                        $password = substr( $data, $chr1pos + 1 );
                    }
                }
            }
            return 1;
        } else
            return 0;
    }
    /* generate a random string (length $len) */
    Function randomstr( $len ) {
        $c = "";
        for( $i = 0; $i < $len; $i++ )
            $c .= chr( rand( 0, 255 ) );
        return $c;
    }
    /* A random key */
    Function randomkey( $len ) {
        srand( ( double )microtime() * 1000000 );
        return $this->randomstr( $len );
    }
    /* Retrieve login informations */

    function testa_coordenador($acao,$t)
    {
        // verifica se  coordenador de ao ou se  responsvel por subao
        if ($t == 'A')
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=1 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."' and rpustatus <> 'I' ";
        if ($t == 'S')
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=9 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."' and rpustatus <> 'I' ";

        $registro=$this->recuperar($sql);
        if (is_array($registro)) return true;
        else return false;
    }

    function testa_emenda()
    {
        $sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=21 and pu.usucpf ='".$_SESSION['usucpf']."'";
        if ($this->pegaUm($sql)) return true;
        else return  false;
    }

    function testa_altagestao()
    {
        $sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=11 and pu.usucpf ='".$_SESSION['usucpf']."'";
        if ($this->pegaUm($sql)) return true;
        else return  false;
    }
    function testa_altagestaopje($cod=0)
    {
        // verifica se é alta-gestão ou gerente do projeto
        $sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod in (58,12) and pu.usucpf ='".$_SESSION['usucpf']."' inner join monitora.usuarioresponsabilidade ur on ur.usucpf=pu.usucpf and ur.pjeid=".$_SESSION['pjeid'];
        if ($cod)
        {
            // então está testando um específico projeto
            $sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod in (58,12) and pu.usucpf ='".$_SESSION['usucpf']."' inner join monitora.usuarioresponsabilidade ur on ur.usucpf=pu.usucpf and ur.pjeid=$cod";

        }
        if ($this->pegaUm($sql)) return true;
        else return  false;
    }

    function testa_coordenador_plan($cod)
    {
        // verifica se  coordenador de planejamento ou super-usuario
        if ($this->testa_superuser())  return true;
        else {
            $sql= 'select usucpf from '.$_SESSION['sisdiretorio'].'.usuarioresponsabilidade where pflcod in (24,23) and acaid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";

            $registro=$this->recuperar($sql);
            if (is_array($registro)) return true;
            else return false;
        }
    }

    function testa_gerente($prg,$t)
    {
        // verifica se  gerente de programa
        $sql= 'select usucpf from '.$_SESSION['sisdiretorio'].'.usuarioresponsabilidade where pflcod=2 and prgid='.$prg." and usucpf = '".$_SESSION['usucpf']."'";
        $registro=$this->recuperar($sql);
        if (is_array($registro)) return true;
        else return false;
    }


    function testa_responsavel_projespec($cod=0)
    {
        // verifica se  é responsavel por projeto especial ou super usuário
        if ($cod)
        {
            //
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod in (12,47,56) and pjeid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";
        }
        else
            $sql= "select usucpf from seguranca.perfilusuario where pflcod in (62) and usucpf = '".$_SESSION['usucpf']."'";
        $resp = $this->pegaUm($sql);

        if ($resp) return true;
        else return false;
    }


    function testa_documentador_projespec($cod=0)
    {
        // verifica se  é responsavel por documentar o projeto especial ou super usuário
        $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod in (75,56) and pjeid='.$cod." and usucpf = '".$_SESSION['usucpf']."'";
        $resp = $this->pegaUm($sql);
        if ($resp) return true;
        else return false;
    }

    function testa_gerente_exe($acao,$t)
    {
        // verifica se  gerente-executivo de ao ou de programa
        if ($t == 'A')
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=3 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
        else
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=3 and prgid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
        $registro=$this->recuperar($sql);
        if (is_array($registro)) return true; else return false;
    }

    function testa_digitador($acao,$t)
    {
        // verifica se  digitador de ao, de programa ou de subao ou projeto especial
        if ($t == 'A')
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and acaid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
        if ($t == 'P')
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and prgid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
        if ($t == 'S')
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=8 and saoid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";
        if ($t == 'E')
            $sql= 'select usucpf from monitora.usuarioresponsabilidade where pflcod=51 and pjeid='.$acao." and usucpf = '".$_SESSION['usucpf']."'";

        $registro=$this->recuperar($sql);
        if (is_array($registro)) return true; else return false;
    }

    function testa_proprietario($cod)
    {
        // verifica se  digitador é o proprietário do registro
        $sql= "select ptosntemdono as temdono,usucpf   from monitora.planotrabalho where ptoid=$cod ";
        $res=$this->pegalinha($sql);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        if ($temdono == 'f') return true;
        else if ($temdono == 't' and $usucpf==$_SESSION['usucpf']) return true;
        else return false;
    }

    private static $testaSuperUser = null;
    function testa_superuser()
    { // testa se  super usurio
        if (!$_SESSION['usucpf'] || !$_SESSION['sisid']) {
            return false;
        }

        if (self::$testaSuperUser === null) {
            $sql = "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflsuperuser='t' and pu.usucpf ='" . $_SESSION['usucpf'] . "' and p.sisid=" . $_SESSION['sisid'];
            $registro = $this->recuperar($sql);

            self::$testaSuperUser = is_array($registro);
        }

        return self::$testaSuperUser;
    }

    function testa_coorduma()
    { // testa se  é coordenador da UMA
        $sql= "select pu.usucpf from seguranca.perfilusuario pu inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod=4 and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
        $registro=$this->recuperar($sql);
        if (is_array($registro)) return true;
        else return false;
    }

    function testa_uma()
    {
        // verifica se o usuário pode emular outro usuário
        $sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflsuporte='t' and usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
        $registro=$this->recuperar($sql);
        if (is_array($registro)) return true;
        else return false;
    }

    function testa_cgo()
    {
        // verifica se  perfil CGO  ou equipe cgo
        if ($_SESSION['sisid']==1)
            $sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (4,6,18) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
        else if ($_SESSION['sisid']==2)
            $sql= "select pu.usucpf from perfilusuario pu inner join perfil p on p.pflcod = pu.pflcod and p.pflcod in (50,52) and pu.usucpf ='".$_SESSION['usucpf']."' and p.sisid=".$_SESSION['sisid'];
        $registro=$this->recuperar($sql);
        if (is_array($registro)) return true;
        else return false;
    }

    function testa_responsabilidade($id,$pflcod, $campo)
    {
        // verifica se  gerente de programa
        $sql= "select usucpf from usuarioresponsabilidade where pflcod=$pflcod and $campo = '$id' and usucpf = '".$_SESSION['usucpf']."';";
        $registro=$this->recuperar($sql);
        if (is_array($registro)) return true;
        else return false;
    }

    function mostra_resp( $chave_valor, $chave_nome,$ano=true,$schema='monitora' )
    {
        // carrega os registros
        if ($ano){
            $sql = sprintf(
                "select distinct p.pflnivel, p.pfldsc, usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum, uni.unidsc
				from seguranca.perfil p
				inner join $schema.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
				inner join seguranca.usuario usu on usu.usucpf = ur.usucpf
				inner join unidade uni on uni.unicod = usu.unicod
				where p.pflstatus = 'A' and ur.%s = '%s' and ur.rpustatus = 'A' and p.pflresponsabilidade != 'H' and ur.prsano = '%s'
				order by p.pflnivel",
                $chave_nome,
                $chave_valor,
                $_SESSION['exercicio']
            );


        }else
            $sql = sprintf(
                "select distinct p.pflnivel, p.pfldsc, usu.usucpf, usu.usunome, usu.usufoneddd, usu.usufonenum, uni.unidsc
				from seguranca.perfil p
				inner join $schema.usuarioresponsabilidade ur on ur.pflcod = p.pflcod
				inner join seguranca.usuario usu on usu.usucpf = ur.usucpf
				inner join unidade uni on uni.unicod = usu.unicod
				where p.pflstatus = 'A' and ur.%s = '%s' and ur.rpustatus = 'A' and p.pflresponsabilidade != 'H'
				order by p.pflnivel",
                $chave_nome,
                $chave_valor
            );
        $responsaveis = $this->carregar( $sql );
        if ( !$responsaveis ) {
            return;
        }

        // exibe o primeiro registro
        echo "<script>
				function exibirEquipeApoio(){
					elemento = document.getElementById( 'responsaveis' );
					imagem = document.getElementById( 'botao_mais_menos' );
					if ( elemento.style.display == 'block' ) {
						elemento.style.display = 'none';
						imagem.src = '../imagens/mais.gif';
					} else {
						imagem.src = '../imagens/menos.gif';
						elemento.style.display = 'block';
					}
				}
			</script>";
        $responsavel = array_shift( $responsaveis );
        $htm = sprintf(
            "<tr><td width='250' align='right' class='SubTituloDireita'><a href='#' title='exibir equipe de apoio' onclick='exibirEquipeApoio();'><img id='botao_mais_menos' src='../imagens/mais.gif' border='0'/></a> %s:</td><td><img src='../imagens/email.gif' title='Enviar e-mail ao Gestor' border='0' onclick='envia_email(\"%s\");'> %s<br><font color=#888888>%s - Tel: (%s) %s</font></td></tr>",
            $responsavel['pfldsc'],
            $responsavel['usucpf'],
            $responsavel['usunome'],
            $responsavel['unidsc'],
            $responsavel['usufoneddd'],
            $responsavel['usufonenum']
        );
        echo $htm;

        // exibe os demais registros
        echo '<tr><td colspan="2" style="border: 0; padding: 0;"><div id="responsaveis" style="display: none"><table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%; border: 0">';
        foreach ( $responsaveis as $indice => $responsavel ) {
            // monta o html
            $htm = sprintf(
                "<tr><td width='250' align='right' class='SubTituloDireita' width='20%%'>%s:</td><td><img src='../imagens/email.gif' title='Enviar e-mail ao Gestor' border='0' onclick='envia_email(\"%s\");'> %s<br><font color=#888888>%s - Tel: (%s) %s</font></td></tr>",
                $responsavel['pfldsc'],
                $responsavel['usucpf'],
                $responsavel['usunome'],
                $responsavel['unidsc'],
                $responsavel['usufoneddd'],
                $responsavel['usufonenum']
            );
            echo $htm;
        }
        echo "</table></div></td></tr>";
    }


    function cabecalho_projeto($pjeid)
    {
        $sql = "select p.*,u.ungabrev from monitora.projetoespecial p inner join unidadegestora u using (ungcod) where pjeid=".$pjeid;
        $RS = $this->record_set($sql);
        $nlinhas = $this->conta_linhas($RS);
        $nl=$nlinhas;
        //if ($nlinhas >= 0) {
        $res =  $this->carrega_registro($RS,0);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        // a linha abaixo transforma em variÃ¡veis todos os campos do array

        ?>
        <tr>
            <td align='right' class="subtitulodireita" width="20%">Denominação:</td>
            <td><?=$ungabrev.'-'.$pjecod.'  '.$pjedsc?></td>
        </tr>
        <?
        @$this -> mostra_resp($pjeid, 'pjeid',false);
    }


    function cabecalho_acao($acaid)
    {
        global $prgcod,$acacod,$loccod,$unicod;
        $sql = "select * from acao where acaid=".$acaid;
        $RS = $this->record_set($sql);
        $nlinhas = $this->conta_linhas($RS);
        $nl=$nlinhas;
        //if ($nlinhas >= 0) {
        $res =  $this->carrega_registro($RS,0);
        // a linha abaixo transforma em variÃ¡veis todos os campos do array
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        $unidsc = $this->pegaUm( "select unidsc from public.unidade where unicod = '" . $unicod . "'" );
        ?>
        <tr>
            <td align='right' class="subtitulodireita" width="20%">Ação:</td>
            <td><?=$prgcod.'.'.$acacod.'.'.$unicod.'.'.$loccod.' - '.$acadsc?></td>
        </tr>
        </tr>
        <td align='right' class="subtitulodireita">Unidade:</td>
        <td><?=$unicod.' - '.$unidsc?></td>
        </tr>
        <?
        @$this -> mostra_resp($acaid, 'acaid');
    }

    function cabecalho_programa($prgid)
    {

        $sql = "select * from monitora.programa where prgid=".$prgid;
        $RS = $this->record_set($sql);
        $nlinhas = $this->conta_linhas($RS);
        $nl=$nlinhas;
        //if ($nlinhas >= 0) {
        $res =  $this->carrega_registro($RS,0);
        // a linha abaixo transforma em variÃ¡veis todos os campos do array
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        $orgdsc = $this->pegaUm( "select orgdsc from public.orgao where orgcod = '" . $orgcod . "' limit 1" );
        ?>
        <tr>
            <td align='right' class="subtitulodireita" width="20%">Programa:</td>
            <td><?=$prgcod . ' - ' . $prgdsc?></td>
        </tr>
        </tr>
        <td align='right' class="subtitulodireita">Órgão:</td>
        <td><?=$orgcod.' - '.$orgdsc?></td>
        </tr>
        <?
        @$this -> mostra_resp($prgid, 'prgid');
    }


    function relatsubacao($res,$acaoid)
    {

        unset($inicio,$fim,$soma);
        if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
        $nivel = $this->busca_pai($ptoid,0);
        if (! in_array($ptoid,$_SESSION['ptoid']))

        {
            if ($ptotipo=='S' and $nivel==0)
            {
                include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
                $sql = "select p.acaid,p.ptoid, ptoid_pai,ptotipo,p.ptocod, case when p.ptotipo='S' then 'Subao' when p.ptotipo='E' then 'Etapa' else 'Fase' end as tipo,p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado,to_char(p.ptodata_ini,'DD/MM/YYYY') as inicio, to_char(p.ptodata_fim,'DD/MM/YYYY') as fim from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptostatus='A' and p.ptoid_pai=".$ptoid."  and p.acaid=".$acaoid."  group by p.acaid,p.ptoid_pai,p.ptoid,p.ptotipo, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma,p.ptoordem, u.unmdsc,p.ptodata_ini,p.ptodata_fim order by p.ptoordem,p.ptotipo desc,p.ptoid_pai, p.ptocod";

                $RSp = $this->record_set($sql);
                $nlinhasp = $this->conta_linhas($RSp);
                for ($im=0; $im<=$nlinhasp;$im++)
                {
                    $res = $this->carrega_registro($RSp,$im);
                    $this->relatsubacao($res,$acaoid);
                }
            }
            else if ($nivel==1)
                include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
            else if ($nivel==2) {
                if (in_array($ptoid_pai,$_SESSION['ptoid']))
                    include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/relatorio/acao/dadosfisevo2.inc";
            }
        }
    }


// Returns true if $string is valid UTF-8 and false otherwise.
    function is_utf8($string) {

        // From http://w3.org/International/questions/qa-forms-utf-8.html
        return preg_match('%^(?:
         [\x09\x0A\x0D\x20-\x7E]            # ASCII
       | [\xC2-\xDF][\x80-\xBF]            # non-overlong 2-byte
       |  \xE0[\xA0-\xBF][\x80-\xBF]        # excluding overlongs
       | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}  # straight 3-byte
       |  \xED[\x80-\x9F][\x80-\xBF]        # excluding surrogates
       |  \xF0[\x90-\xBF][\x80-\xBF]{2}    # planes 1-3
       | [\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
       |  \xF4[\x80-\x8F][\x80-\xBF]{2}    # plane 16
   )*$%xs', $string);

    } // function is_utf8

    function converttoxml( &$dbconnection, $strsql, $filename, $arquivo )
    {
        ini_set( 'memory_limit', '128M' );
        $rs = pg_query($strsql);
        $nlinhas = !is_array( pg_fetch_all( $rs ) ) ? -1 : count( pg_fetch_all( $rs ) ) - 1;
        $dir = IS_PRODUCAO ? '/var/www/html/simec/arquivos/SIGPLAN/exportacao/' : '/var/www/html/simec/arquivos/SIGPLAN/exportacao/';
        $filename = basename( $filename, '.xml' ) . date( '-d-m-y' ) . '.xml';
        $path = $dir . $filename;
        $xml_file = fopen( $path, 'w' );
        if ( !$xml_file )
        {
            return 0;
        }
        $xml = '<?xml version="1.0" encoding="utf-8"?'.">\n";
        $xml .= '<ArrayOf' . $arquivo . ' xmlns="http://www.sigplan.gov.br/xml/">';
        //$xml.='<ArrayOf'.$arquivo.' xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://www.sigplan.gov.br/xml/">';
        $sucesso = fwrite( $xml_file, $xml );
        for ( $j = 0; $j <= $nlinhas; $j++ )
        {
            $xml = '';
            $res = pg_fetch_array( $rs, $j );
            if( is_array( $res ) )
            {
                $xml .= "\n  <" . $arquivo. ">\n";
                foreach( $res as $k => $v )
                {
                    ${$k} = $v;
                    if ( $v == 't' )
                    {
                        $v = '1';
                    }
                    elseif ( $v == 'f' )
                    {
                        $v = '0';
                    }
                    if ( !is_int( $k ) && $v != '' )
                    {
                        $v = utf8_encode( htmlspecialchars( $v ) );
                        $v = str_replace( "'", "&apos;", $v );
                        $xml .= "    <" . $k . ">" . $v . "</" . $k . ">\n";
                    }
                }
                $xml .= "  </" . $arquivo . ">";
                $sucesso = fwrite( $xml_file, $xml );
            }
        }
        $xml = "\n</ArrayOf" . $arquivo . ">";
        $sucesso = fwrite( $xml_file, $xml );
        fclose( $xml_file );
        return $sucesso > 0 ? 1 : 0;
    }




    function chama_etapafase($cod)
    {
        unset($referencias);
        $referencias = array();
        if ($coordaca or $digit){
            $sql = "select refcod from referencia where cast(current_date as date)>=refdata_inicio and cast(current_date as date)<=refdata_limite_avaliacao_aca";
            $RS = $db->record_set($sql);
            $nlinhas = $db->conta_linhas($RS);
            if ($nlinhas >= 0) {
                for ($i=0; $i<=$nlinhas;$i++){
                    $res = $db->carrega_registro($RS,$i);
                    foreach($res as $k=>$v) ${$k}=$v;
                    array_push($referencias, $v);
                }
            }
        }
        $sql = "select p.ptoid, p.ptocod, p.ptodsc, p.ptoprevistoexercicio as previsto, p.ptosnpercent, p.ptosnsoma, u.unmdsc, case when sum(e.exprealizado) is null then 0 else sum(e.exprealizado) end as totalrealizado from planotrabalho p inner join unidademedida u on p.unmcod=u.unmcod left join execucaopto e on p.ptoid=e.ptoid where p.ptoid = $cod and p.acaid=".$_SESSION['acaid']."  group by p.ptoid, p.ptocod, p.ptodsc, p.ptoprevistoexercicio, p.ptosnpercent, p.ptosnsoma, u.unmdsc";
        $RS = $this->record_set($sql);
        $nlinhas = $this->conta_linhas($RS);
        if ($nlinhas >= 0)
        {
            $res = $this->carrega_registro($RS,0);
            foreach($res as $k=>$v) ${$k}=$v;
            $porcentorealizado = $totalrealizado*100/$previsto;
            if ($porcentorealizado > 100) $mostraporcentorealizado = 100; else $mostraporcentorealizado = $porcentorealizado;
            $porcentoexecutado = 100 - $mostraporcentorealizado;?>
            <THEAD bgcolor="#f5f5f5">
            <tr style="background-color: #ececec;">
                <TD colspan="12" align="left" style="color: #000099;">Cd.: <strong><?=$ptocod?>
                        - <?=$ptodsc?></strong></TD>
                <TD colspan="2" align="right">Unid. Medida:<strong><?=$unmdsc?></strong><br>
                    0% <label
                            style="border: 1px solid #000000; font-size: 8px; border-top: 1px solid #c0c0c0; border-right: 1px solid #c0c0c0; border-left: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; background-color: #ffffff;"
                            title="Realizado <?=number_format($porcentorealizado, 0, '.', '')?>%"><span
                                style="background-color: #33cc00; font-size: 8px;"><?for ($k=0; $k<$mostraporcentorealizado/2;$k++) print " ";?></span><?for ($k=0; $k<$porcentoexecutado/2;$k++) print " ";?></label>
                    <?
                    if ($porcentorealizado < 100) print number_format($porcentorealizado, 0, '.', ''); else print '100'; ?>%</TD>
            </tr>
            </THEAD>
            <TBODY>
            <TR style="color: #808080;" bgcolor="#f6f6f6">
                <?
                $sql = "select refmes_ref, refano_ref from referencia where refdata_limite_avaliacao_aca is not null and refsnmonitoramento='t'   and refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
                $RS2 = $this->record_set($sql);
                $nlinhas2 = $this->conta_linhas($RS2);
                if ($nlinhas2 >= 0)
                {
                    for ($j=0; $j<=$nlinhas2;$j++)
                    {
                        $res2 = $this->carrega_registro($RS2,$j);
                        foreach($res2 as $k=>$v) ${$k}=$v;
                        ?>
                        <TD align="right"><?=$refmes_ref.'/'.$refano_ref?></TD>
                    <?  }
                }
                $colspan = 12 - $j;
                if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
                ?>
                <TD align="right">Realizado</TD>
                <TD align="right">Previsto</TD>
            </TR>
            <TR style="height: 30px;">
                <?
                $sql = "select r.refcod, case when e.exprealizado is null then -1 else e.exprealizado end as exprealizado,  e.expobs, e.tpscod, t.tpscod, t.tpsdsc from referencia r left join execucaopto e on r.refcod=e.refcod and e.ptoid=".$ptoid." left join tiposituacao t on e.tpscod=t.tpscod where r.refdata_limite_avaliacao_aca is not null and r.refsnmonitoramento='t' and r.refano_ref='".$_SESSION['exercicio']."'   order by refano_ref,refmes_ref";
                $RS2 = $this->record_set($sql);
                $nlinhas2 = $this->conta_linhas($RS2);
                $totalrealizado = 0;
                if ($nlinhas2 >= 0)
                {
                    for ($j=0; $j<=$nlinhas2;$j++)
                    {
                        $res2 = $this->carrega_registro($RS2,$j);
                        foreach($res2 as $k=>$v) ${$k}=$v;
                        if ($exprealizado == -1) $v_exprealizado=$exprealizado+1; else $v_exprealizado=$exprealizado;
                        if (in($refcod,$referencias) and ($coordaca or $digit)) $txtexprealizado = '<input type="Text" size="5" value="'.$v_exprealizado.'" class="CampoEstilo" style="text-align : right; color:#808080;"  readonly="readonly" onclick="edita(\''.$refcod.'\',\''.$ptoid.'\');" onmouseover="MouseOver(this);" onmouseout="MouseOut(this);"/>'; else $txtexprealizado = $v_exprealizado;?>
                        <TD align="right" style="color: #3366ff;"><?=$txtexprealizado?></TD>
                        <?	if ($ptosnsoma=='r') { if($exprealizado>-1) $totalrealizado = $exprealizado;}
                    else
                    {
                        if ($exprealizado == -1) $exprealizado=$exprealizado+1;
                        $totalrealizado = $totalrealizado + $exprealizado;
                    }
                    }
                }
                $colspan = 12 - $j;
                if ($colspan>0) print "<TD colspan='".$colspan."'></TD>";
                ?>
                <TD align="right" style="color: #cc0000;"><?=number_format($totalrealizado, 0, '.', '');?></TD>
                <TD align="right" style="color: #0000CC;"><?=$previsto?></TD>
            </TR>
            </TBODY>
            <tr>
                <td colspan="14"
                    style="height: 1px; background-color: #e5e5e5; padding: 0px"></td>
            </tr>
            <tr>
                <td colspan="14"
                    style="height: 2px; background-color: #000000; padding: 0px"></td>
            </tr>
            <?
        }
    }


    function chama_subacao($cod)
    {
        print ' uma subao'.$cod;
    }


    /**
     * Verifica se o usuário possui algum perfil que permite manipular
     * todas as ações. Os perfis que permitem essa manipulação são os
     * que não possuem registros em elabrev.tprperfil.
     *
     * @return boolean
     */
    function usuarioPossuiPermissaoTodasUnidades( $esquema = 'elabrev' )
    {
        $cpf = $_SESSION['usucpf'];
        $sisid = $_SESSION['sisid'];
        $sql =
            "select count(*)
			from seguranca.perfilusuario
				inner join seguranca.perfil using ( pflcod )
				left join " . $esquema . ".tprperfil using ( pflcod )
			where
				usucpf = '" . $cpf . "' and
				tprcod is null and
				sisid = " . $sisid . " ";
        return (boolean) $this->pegaUm( $sql );
    }

    /**
     * Verifica se o usuário possui algum perfil que permite manipular
     * alguma unidade. Os perfis que permitem essa manipulação são os
     * que possuem registros em elabrev.tprperfil com o tprcod = 9.
     *
     * @return boolean
     */
    function usuarioPossuiPermissaoAlgumaUnidade( $esquema = 'elabrev' )
    {
        $cpf = $_SESSION['usucpf'];
        $sql = "select count(*) from seguranca.perfilusuario left join " . $esquema . ".tprperfil using ( pflcod ) where usucpf = '" . $cpf . "'";
        return (boolean) $this->pegaUm( $sql );
    }

    /**
     * Verifica se usuário pode manipular uma determinada unidade.
     *
     * @param string $unicod
     * @return boolean
     */
    function usuarioPossuiPermissaoUnidade( $unicod, $esquema = 'elabrev' )
    {
        if ( $this->usuarioPossuiPermissaoTodasUnidades( $esquema ) )
        {
            return true;
        }
        if ( !$this->usuarioPossuiPermissaoAlgumaUnidade( $esquema ) )
        {
            return false;
        }
        $unicod = str_replace( "'", "\\'", $unicod );
        $cpf = $_SESSION['usucpf'];
        $sql = "select count(*) from " . $esquema . ".usuarioresponsabilidade where usucpf = '" . $cpf . "' and rpustatus = 'A' and unicod = '" . $unicod . "'";
        return (boolean) $this->pegaUm( $sql );
    }

    /**
     * Monta join a ser utilizado por algum outro query com as unidades do
     * usuário. não é possível adivinhar qual o nome da tabela que conterá
     * o unicod para fazer a restrição, portanto A RESTRICAO DO UNICOD DEVE
     * SER ADICIONADO AO FINAL DO RETORNO.
     *
     * Exemplo:
     * $join = $db->usuarioJoinUnidadesPermitidas();
     * $join .= " and minhatabela.unicod = unijoin.unicod ";
     *
     * O nome das tabelas são unijoin (unidade) e usujoin
     * (usuarioresponsabilidade). Este segundo só aparece caso o usuário não
     * possui um perfil que permita trabalhar com todas as unidades
     *
     * @return string
     */
    function usuarioJoinUnidadesPermitidas( $esquema = 'elabrev' )
    {
        $join = "";
        $podeTodas = $this->usuarioPossuiPermissaoTodasUnidades( $esquema );
        if ( !$podeTodas )
        {
            $join .=
                " inner join " . $esquema . ".usuarioresponsabilidade usujoin on " .
                " usujoin.usucpf = '" . $_SESSION['usucpf'] . "' and " .
                " usujoin.rpustatus = 'A' and " .
                " usujoin.prsano = '" . $_SESSION['exercicio'] . "' and " .
                " usujoin.pflcod in ( " .
                " select pu.pflcod " .
                " from seguranca.perfilusuario pu" .
                " inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.sisid = " .$_SESSION['sisid'].
                " where pu.usucpf = '" . $_SESSION['usucpf'] . "'" .
                " ) ";
        }
        $join .=
            " inner join unidade unijoin on " .
            " unijoin.unicod != '26100' and " .
            " unijoin.unicod != '26000' and " .
            " unijoin.orgcod = '26000' ";
        if ( !$podeTodas )
        {
            $join .= " and unijoin.unicod = usujoin.unicod ";
        }
        return $join;
    }

    /**
     * Captura os códigos da unidades que o usuário pode manipular.
     *
     * @return string[]
     */
    function usuarioUnidadesPermitidas( $esquema = 'elabrev' )
    {
        if ( $this->usuarioPossuiPermissaoTodasUnidades( $esquema ) )
        {
            $sql = "
				select unicod
				from unidade
				where
					unistatus = 'A' and
					(
						( orgcod = '26000' and unicod != '26100' )
						or
						( unicod = '74902' )
					)
			";
        }
        else
        {
            $sql =
                " select " .
                " distinct unicod " .
                " from " . $esquema . ".usuarioresponsabilidade " .
                " inner join unidade using ( unicod ) " .
                " inner join seguranca.perfilusuario using ( pflcod, usucpf ) " .
                " where " .
                " usucpf = '" . $_SESSION['usucpf'] . "' and " .
                " rpustatus = 'A' and " .
                " unistatus = 'A' and " .
                " orgcod = '26000' and " .
                " unicod != '26100' ";
        }
        if ( !$this->usuarioPossuiPermissaoAlgumaUnidade( $esquema ) )
        {
            return array();
        }
        $dados =  $this->carregar( $sql );
        $retorno = array();
        if ( $dados )
        {
            foreach ( $dados as $dado )
            {
                array_push( $retorno, $dado['unicod'] );
            }
        }
        return $retorno;
    }

    public function alterar_status_usuario( $cpf, $status, $justificativa, $sisid = null ){
        // $sql_status é a query que altera o status em algum sistema/geral
        // $sql_cascata é a query que altear o status em alguma sistema/geral devido a alteração inicialmente definida
        // $sql_historico é a query que gera histórico para a alteração que está sendo efetuada
        // $sql_historico_cascata é a query que gera histórico para as alterações que estão sendo efetuadas em cascata
        $usucpfadm = $cpf != $_SESSION['usucpf'] ? "'" . $_SESSION['usucpf'] . "'" : 'null';
        $sql_cascata = '';
        $sql_historico_cascata = array();
        if ( $sisid ) {
            $sql_status = sprintf(
                "UPDATE seguranca.usuario_sistema SET suscod = '%s' WHERE sisid = %d AND usucpf = '%s'",
                $status,
                $sisid,
                $cpf
            );
            $sql_historico = sprintf(
                "INSERT INTO seguranca.historicousuario ( htudsc, usucpf, sisid, suscod, usucpfadm ) VALUES ( '%s', '%s', %d, '%1s', %s )",
                $justificativa,
                $cpf,
                $sisid,
                $status,
                $usucpfadm
            );
            if ( $status == 'A' ) // verifica se está alterando para ativo
            {
                $sql_status_geral = sprintf(
                    "SELECT suscod FROM seguranca.usuario WHERE usucpf = '%s'",
                    $cpf
                );
                $status_geral = $this->pegaUm( $sql_status_geral );
                if ( $status_geral != 'A' ) // verifica se status geral não é ativo
                {
                    // altera em cascata no geral
                    $sql_cascata = sprintf(
                        "UPDATE seguranca.usuario SET suscod = '%s' WHERE usucpf = '%s'",
                        $status,
                        $cpf
                    );
                    $sql_historico_cascata = array( sprintf(
                        "INSERT INTO seguranca.historicousuario ( htudsc, usucpf, suscod, usucpfadm ) VALUES ( '%s', '%s', '%1s', %s )",
                        $justificativa,
                        $cpf,
                        $status,
                        $usucpfadm
                    ) );
                }
            }
        } else {
            $sql_status = sprintf(
                "UPDATE seguranca.usuario SET suscod = '%s' WHERE usucpf = '%s'",
                $status,
                $cpf
            );
            $sql_historico = sprintf(
                "INSERT INTO seguranca.historicousuario ( htudsc, usucpf, suscod, usucpfadm ) VALUES ( '%s', '%s', '%1s', %s )",
                $justificativa,
                $cpf,
                $status,
                $usucpfadm
            );
            if ( $status != 'A' )
            {
                // altera em cascata para todos os sistemas
                $sql_cascata = sprintf(
                    "UPDATE seguranca.usuario_sistema SET suscod = '%s' WHERE usucpf = '%s'",
                    $status,
                    $cpf
                );
                // captura os sistemas do usuário
                $sql_sistemas_usuario = sprintf(
                    "SELECT DISTINCT sisid, suscod FROM seguranca.usuario_sistema WHERE usucpf = '%s'",
                    $cpf
                );
                $sistemas = $this->carregar( $sql_sistemas_usuario );
                foreach ( $sistemas as $sistema )
                {
                    if ( $sistema['suscod'] == $status ) // verifica se precisa gerar historico
                    {
                        continue;
                    }
                    array_push(
                        $sql_historico_cascata,
                        sprintf(
                            "INSERT INTO seguranca.historicousuario ( htudsc, usucpf, sisid, suscod, usucpfadm ) VALUES ( '%s', '%s', %d, '%1s', %s )",
                            $justificativa,
                            $cpf,
                            $sistema['sisid'],
                            $status,
                            $usucpfadm
                        )
                    );
                }
            }
        }
        $this->executar( $sql_status );
        if ( $sql_cascata != '' )
        {
            $this->executar( $sql_cascata );
        }
        if ( count( $sql_historico_cascata ) > 0 )
        {
            foreach ( $sql_historico_cascata as $sql )
            {
                $this->executar( $sql );
            }
        }

        $this->executar( $sql_historico );

    }

    public function gerar_senha(){
        $dicionario = array(
            'a','b','c','d','e','f','g','h','i','j','l','m','n','o','p','q','r','s','t','u','v','x','z'
        );
        $j = rand( 4, 6 );
        $senha = '';
        for( $i=0; $i < $j ;$i++ ) {
            $senha .= $dicionario[array_rand( $dicionario )];
        }
        $dicionarionumeros = array('1','2','3','4','5','6','7','8','9','0');
        $j = rand( 2, 3 );
        for( $i=0; $i < $j ;$i++ ) {
            $senha .= $dicionarionumeros[array_rand( $dicionarionumeros )];
        }

        return $senha;
    }

    function monta_combom($var, $sql, $habil, $titulo = '', $acao, $opc, $txtdica = '', $size = '', $obrig = '', $id = '', $return = false, $value = null, $title = null, $pesquisa = false, $bsSelect = true)
    {
        // este método monta um combobox onde: var = nome do combo; sql é a query,
        //titulo é o título da primeira linha do combo
        global ${$var};
        $value = $value ? $value : ${$var};

        if (is_array($sql))
            $rescombo = $sql;
        else
            $rescombo = $this->carregar($sql);

        $select = "";
        if($habil === 'S'){
            if ($txtdica)
                $select = "<span onmouseover=\"return escape('" . $txtdica . "');\">";

            // define atributos do select
            if ($habil == 'S')
                $select .= "<select  name='" . strtolower($var) . "'";
            else
                $select .= "<select name='" . strtolower($var) . "_disable' ";

            $class = "CampoEstilo form-control " . ($bsSelect ? 'bs-select' : '');

            if ($obrig == 'S')
                $select .= " required ";

            $select .= " class='select2 $class' ";

            if($pesquisa){
                $select .= " data-live-search='true' ";
            }

            if ($title) {
                $select .= "title='" . $title . "'";
            }

            if ($size <> '')
                $select .= " style='width:" . $size . "px;'";
            else $select .= " style=''";
            if ($habil == 'N')
                $select = $select . ' disabled="disabled" ';
            if (isset($tamanho) && $tamanho > 1)
                $select .= ' size="' . $tamanho . '" ';
            if ($acao)
                $select .= ' onchange="' . $acao . '(this.value)"';
            if ($id)
                $select .= ' id="' . htmlentities($id) . '"';
            $select .= ">";

            // cria os options
            if ($titulo)
                $select .= '<option value="">' . $titulo . '</option>';

            if ($rescombo) {
                for ($i = 0; $i < count($rescombo); $i++) {
                    if ($value == $rescombo[$i]['codigo'])
                        $sel = 'selected';
                    else
                        $sel = '';
                    $select .= "<option value='" . $rescombo[$i]['codigo'] . "' " . $sel . ">" .
                        $rescombo[$i]['descricao'] .
                        "</option>\n";
                }
            }
            if ($value == 'x')
                $sel = 'selected="selected"';
            else
                $sel = '';
            if ($opc)
                $select .= "<option " . $sel . " value='x'>" . $opc . "</option>\n";

            // finaliza impressão do select
            $select .= '</select>';
            if ($txtdica)
                $select .= "</span>";
            if ($habil == 'N')
                $select .= "<input type='hidden' name ='" . strtolower($var) . "' value='" . ${$var} . "' />";
        }else{
            $select = "<div><strong>-</strong></div>";
            if($value){
                $valorSelecionado = array_shift(array_filter($rescombo, function($opcao) use ($value){
                    return $opcao['codigo'] == $value;
                }));

                if(isset($valorSelecionado['descricao']) && !empty($valorSelecionado['descricao'])){
                    $select = "<div><strong>{$valorSelecionado['descricao']}</strong></div>";
                }
            }

            $select .= "<input type='hidden' name ='" . strtolower($var) . "' value='{$value}' " . ($id ? ' id="' . htmlentities($id) . '"' : '') .  " />";
        }

        if ($return)
            return $select;
        else
            echo $select;
    }

    function monta_checkboxm($nome, $query, $habilitado = 'S', $obrig = '', $marcados = array(), $separador = '  ')
    {
        global ${$nome};

        if ((!is_array($marcados) || count($marcados) == 0) && ${$nome}) {
            $marcados = !is_array(${$nome}) ? array(${$nome}) : ${$nome};
        }

        $saida = "";
        if (is_array($query)) {
            $rescombo = $query;
        } else {
            $rescombo = $this->carregar($query);
        }

        $disabled = $habilitado == 'S' ? '' : 'disabled';
        $comp = $habilitado == 'S' ? '' : '_disabled';
        $obrigatorio = $obrig == 'S' ? 'required' : '';

        if (is_array($rescombo) && @count($rescombo) > 0) {
            for ($i = 0, $j = count($rescombo) - 1; $i < count($rescombo); $i++) {

                $checked = in_array($rescombo[$i]['codigo'], $marcados) !== false ? 'checked="checked"' : '';
                echo $rescombo[$i]['indice'] . "<label class='mt-checkbox'><input type=\"checkbox\" " . $obrigatorio . " " . $disabled . " name=\"" . $nome . $comp . "\" id=\"" . $nome . "_" . $rescombo[$i]['codigo'] . "\" value=\"" . $rescombo[$i]['codigo'] . "\" " . $checked . " /> " . $rescombo[$i]['descricao'] . "<span></span></label>";
                if ($i != $j) echo $separador;

                if ($checked && $disabled) {
                    echo "<input type='hidden' name='{$nome}' value='{$rescombo[$i]['codigo']}' />";
                }

            }
        }
    }

    function monta_radiom($nome, $query, $habilitado = 'S', $obrig = '', $valor = null, $separador = '&nbsp;&nbsp;&nbsp;&nbsp;')
    {
        global ${$nome};
        $valor = $valor ? $valor : ${$nome};

        if (is_array($query)) {
            $rescombo = $query;
        } else {
            $rescombo = $this->carregar($query);
        }

        if($habilitado === 'S'){
            $disabled = $habilitado == 'S' ? '' : 'disabled';
            $comp = $habilitado == 'S' ? '' : '_disabled';
            $obrigatorio = $obrig == 'S' ? 'required' : '';

            if (is_array($rescombo) && @count($rescombo) > 0) {
                for ($i = 0, $j = count($rescombo) - 1; $i < count($rescombo); $i++) {

                    $checked = $rescombo[$i]['codigo'] == $valor ? 'checked="checked"' : '';
                    echo $rescombo[$i]['indice'] . "<label class=\"not-obrigatorio mt-radio\" style=\"cursor:pointer\"><input style='vertical-align: text-top; cursor: pointer;' type=\"radio\" " . $obrigatorio . " " . $disabled . " name=\"" . $nome . $comp . "\" id=\"" . $nome . "_" . $rescombo[$i]['codigo'] . "\" value=\"" . $rescombo[$i]['codigo'] . "\" " . $checked . " />" . $rescombo[$i]['descricao'] . "<span></span></label>";
                    if ($i != $j) echo $separador;

                    if ($checked && $disabled) {
                        echo "<input type='hidden' name='{$nome}' value='{$rescombo[$i]['codigo']}' />";
                    }
                }
            }
        }else{
            $valorSelecionado = array_shift(array_filter($rescombo, function($opcao) use ($valor){
                return $opcao['codigo'] == $valor;
            }));
            $texto = "<div><strong>-</strong></div>";
            $codigo = '';
            if(isset($valorSelecionado['descricao']) && !empty($valorSelecionado['descricao'])){
                $codigo = $valorSelecionado['codigo'];
                $texto = "<div><strong>{$valorSelecionado['descricao']}</strong></div>";
            }

            $texto .= "<input type='hidden' name ='" . strtolower($nome) . "' value='{$valor}' " . " id='{$nome}_{$codigo}' />";

            echo $texto;
        }
    }

    function insucessom($modulo, $parametros = '', $msg = 'Não foi possível executar a operação')
    {
        addMsgDanger($msg);
        ?>
        <html>
        <head>
            <script>
                <?if(!$modulo) {?>
                history.back();
                <?}else{
                $saida = $_SESSION['sisarquivo'] . ".php?modulo=" . $modulo . "&acao=" . $_REQUEST['acao'] . $parametros;
                ?>
                location.href = "<?=$saida?>";
                <?}?>
            </script>
        </head>
        <body>
        </body>
        </html>
        <?
        exit();
    }

    function sucessomAjax($modulo = '', $parametros = '', $msg = 'Operação realizada com sucesso')
    {
        die(json_encode(utf8ArrayEncode(array(
            'codigo' => 1,
            'msg' => $msg,
            'redirect' => $modulo,
            'params' => $parametros
        ))));
    }

    function insucessomAjax($modulo = '', $parametros = '', $msg = 'Não foi possível realizar a operação')
    {
        die(json_encode(utf8ArrayEncode(array(
            'codigo' => 0,
            'msg' => $msg,
            'redirect' => $modulo,
            'params' => $parametros
        ))));
    }

    function responseAjax($response)
    {
        die(json_encode(utf8ArrayEncode($response)));
    }
}

//class cls_banco_sipia extends cls_banco
//{
//    function cls_banco_sipia()
//	{
//
//		$cls_classes_simec = new cls_classes_simec();
//		$cls_classes_simec->cls_classes_simec();
//
//		if(empty($this->link)){
//			while((!$this->link) and ($this->tentativas < 5))
//			{
//				$this->link = pg_connect("host=".$GLOBALS["servidor_bd_sipia"]." port=".$GLOBALS["porta_bd_sipia"]." dbname=".$GLOBALS['nome_bd_sipia']."  user=".$GLOBALS["usuario_db_sipia"] ." password=".$GLOBALS["senha_bd_sipia"] ."");
//
//                pg_query($this->link, "SET search_path TO public, seguranca");
//
//				pg_set_client_encoding($this->link,'LATIN5');
//				$this->tentativas++;
//			}
//
//			if((!$this->link) and ($this->tentativas == 5))
//				cls_classes_simec::erro(104,2);
//			else
//				$this->tentativas = 0;
//		}
//		return 1;
//	}
//}

class criptografia {
    function rc4($pwd, $data, $case) {
        if ($case == 'de') {
            $data = urldecode($data);
        }
        $key[] = "";
        $box[] = "";
        $temp_swap = "";
        $pwd_length = 0;
        $pwd_length = strlen($pwd);
        for ($i = 0; $i <= 255; $i++) {
            $key[$i] = ord(substr($pwd, ($i % $pwd_length), 1));
            $box[$i] = $i;
        }
        $x = 0;
        for ($i = 0; $i <= 255; $i++) {
            $x = ($x + $box[$i] + $key[$i]) % 256;
            $temp_swap = $box[$i];
            $box[$i] = $box[$x];
            $box[$x] = $temp_swap;
        }
        $temp = "";
        $k = "";
        $cipherby = "";
        $cipher = "";
        $a = 0;
        $j = 0;
        for ($i = 0; $i < strlen($data); $i++) {
            $a = ($a + 1) % 256;
            $j = ($j + $box[$a]) % 256;
            $temp = $box[$a];
            $box[$a] = $box[$j];
            $box[$j] = $temp;
            $k = $box[(($box[$a] + $box[$j]) % 256)];
            $cipherby = ord(substr($data, $i, 1)) ^ $k;
            $cipher .= chr($cipherby);
        }
        if ($case == 'de') {
            $cipher = urldecode(urlencode($cipher));
        } else {
            $cipher = urlencode($cipher);
        }
        return $cipher;
    }
    function hash_senha($senha) {
        return "{MD5}".base64_encode(pack("H*",md5($senha)));
    }
}

/**
 * Agrupadora de testes e iterador. Permite dizer se uma bateria de testes foi
 * realizada com sucesso, alm de permitir percorrer os testes realizados
 * @author Adonias Malosso <adonias@mesote.com.br>
 * @version 1.0
 */
class Testes implements Iterator {
    private $_testes;

    public function __construct() {
        $this->_testes = array();
    }

    public function addTeste(Teste $teste) {
        $this->_testes[] = $teste;
    }

    public function delTeste($pos) {
        if(isset($this->_testes[$pos]))
            unset($this->_testes[$pos]);
    }

    public function rewind() {
        reset($this->_testes);
    }

    public function current() {
        $var = current($this->_testes);
        return $var;
    }

    public function key() {
        $var = key($this->_testes);
        return $var;
    }

    public function next() {
        $var = next($this->_testes);
        return $var;
    }

    public function valid() {
        $var = $this->current() !== false;
        return $var;
    }

    public function resultado() {
        foreach ($this->_testes as $teste)
            if(!$teste->status)
                return false;

        return true;
    }
}


class  GeraExcel{

    // define parametros(init)
    function  __construct(){
        global $nomeDoArquivoXls;
        $this->armazena_dados   = ""; // Armazena dados para imprimir(temporario)
        $this->nomeDoArquivoXls = $nomeDoArquivoXls; // Nome do arquivo excel
        $this->ExcelStart();
    }// fim constructor


    // Monta cabecario do arquivo(tipo xls)
    function ExcelStart(){

        //inicio do cabecario do arquivo
        $this->armazena_dados = pack( "vvvvvv", 0x809, 0x08, 0x00,0x10, 0x0, 0x0 );
    }

    // Fim do arquivo excel
    function FechaArquivo(){
        $this->armazena_dados .= pack( "vv", 0x0A, 0x00);
    }




    // monta conteudo
    //function MontaConteudoString( $excel_linha, $excel_coluna, $value){
    function MontaConteudoString( $excel_linha, $excel_coluna, $value){
        $tamanho = strlen($value);
        $this->armazena_dados .= pack( "v*", 0x0204, 8 + $tamanho, $excel_linha, $excel_coluna, 0x00, $tamanho );
        $this->armazena_dados .= $value;
    }//Fim, monta Col/Lin

    function MontaConteudoNumero($row, $col, $num)
    {
        $record    = 0x0203;
        $length    = 0x000E;
        $xf	= 0x0F;

        $header    = pack("vv",  $record, $length);
        $data      = pack("vvv", $row, $col, $xf);
        $xl_double = pack("d",   $num);
        $this->armazena_dados .= ($header.$data.$xl_double);
    }

    // Gera arquivo(xls)
    function GeraArquivo(){

        //Fecha arquivo(xls)
        $this->FechaArquivo();

        header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
        header ( "Pragma: no-cache" );
        header ( "Content-type: application/xls; name=$this->nomeDoArquivoXls".".xls");
        header ( "Content-Disposition: attachment; filename=$this->nomeDoArquivoXls".".xls");
        header ( "Content-Description: MID Gera excel" );
        print  ( $this->armazena_dados);


    }// fecha funcao
    # Fim da classe que gera excel
}


/**
 * Abstrai o conceito de teste atravs de nome, informao, status e msg de erro.
 *
 * @author Adonias Malosso <adonias@mesotec.com.br>
 * @example $t = new Teste("Permisso de escrita no diretrio", "/var/log", is_writeable("/var/log");
 * @version 1.0
 */
class Teste {
    const IMGOK = "<img src='../imagens/valida1.gif' border='0' title='OK'/>";
    const IMGERRO = "<img src='../imagens/valida3.gif' border='0' title='Erro'/>";
    public $nome;
    public $info;
    public $status;
    public $msg;

    public function __construct($pnome, $pinfo, $pstatus, $pmsg="") {
        $this->nome = $pnome;
        $this->info = $pinfo;
        $this->status = $pstatus;
        $this->msg = $pmsg;
    }

    public function getImagem() {
        switch ($this->status) {
            case 0:
                return self::IMGERRO;
                break;
            case 1:
                return self::IMGOK;
                break;
        }
    }
}


