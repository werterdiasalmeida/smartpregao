<?php
class NotificacaoUsuario extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "app.notificacaousuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("nouid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'nouid' => null,
		'noaid' => null,
		'diuid' => null,
		'noustatus' => null
	);

	public function getDispositivosByNotificacao($noaid){
		$sql = "SELECT
					dis.disid,
					dis.distoken,
					dis.displataforma,
					dis.disversaoplataforma,
					usu.usunome,
					usu.usucpf
				FROM
					{$this->stNomeTabela} nou
				INNER JOIN
					app.dispositivousuario diu ON diu.diuid = nou.diuid
				INNER JOIN
					app.dispositivo dis ON dis.disidentificador = diu.diuidentificador
								       AND dis.disdtcadastro = (( SELECT max(disdtcadastro) AS max FROM app.dispositivo WHERE disidentificador = dis.disidentificador))
				INNER JOIN
					seguranca.usuario usu ON usu.usucpf = diu.usucpf
				WHERE
					nou.noaid = {$noaid}";

		$arRs = $this->carregar($sql);
		return $arRs ? $arRs : array();
	}
}

