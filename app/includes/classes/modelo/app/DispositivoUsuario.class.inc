<?php
class DispositivoUsuario extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "app.dispositivousuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("diuid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
		'diuid' => null,
		'diuidentificador' => null,
		'usucpf' => null,
		'diudtcadastro' => null,
		'diustatus' => null
	);

	public function vincular($usuario, $identificadorDispositivo){
		$arDados = array(
			'diuidentificador' => $identificadorDispositivo,
			'usucpf' => $usuario,
			'diudtcadastro' => 'now()',
			'diustatus' => 'A'
		);

		$this->popularDadosObjeto($arDados)->salvar();
	}

	public function getDispositivoByUsuario($usucpf){
		$sql = "SELECT
					*
				FROM
					app.vw_dispositivo_usuario
				WHERE
					usucpf = '{$usucpf}'";

		$arRs = $this->carregar($sql);
		return $arRs ? $arRs : array();
	}

	public function getDispositivosByAplicativo($appid, $getSql = false){
		$sql = "SELECT
					diuid AS codigo,
					usunome || ' (' || displataforma || ' ' || disversaoplataforma || ')' AS descricao
				FROM
					app.vw_dispositivo_usuario
				WHERE
					appid = {$appid}
				ORDER BY
					2";

		if($getSql){
			return $sql;
		}else{
			$arRs = $this->carregar($sql);
			return $arRs ? $arRs : array();
		}
	}
}

