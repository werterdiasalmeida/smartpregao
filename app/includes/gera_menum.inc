<?php
if ( ! $_SESSION['menu'][$_SESSION['usucpf']][$_SESSION['sisid']] || $db->testa_superuser()) {
	$lista_cpf="'".$_SESSION['usucpf']."'";
	$sql = "select usucpf_delegante as usu_superior from seguranca.delegacao_competencia where sisid=".$_SESSION['sisid']." and usucpf_delegado='".$_SESSION['usucpf']."' and dcpstatus='A' and dcpdata_ini <='".date('Y-m-d')."' and dcpdata_fim >='".date('Y-m-d')."'";
	    
	$rs = @$db->carregar( $sql );

	if (  $rs && count($rs) > 0 )
		{
		 $i=0;
		 foreach ( $rs as $linha )
			{
			 foreach($linha as $k=>$v) ${$k}=$v;
	       	 $lista_cpf .=",'".$usu_superior."'";
	  		 $i++;
			}
		}

    $whereIgreja = "";
//    if($_SESSION['sisid'] == 72 && Igreja::isIgrejaRepresentacaoDefinida() && !$db->testa_superuser()){
//        $mResponsabilidade = new UsuarioResponsabilidade();
//        $whereIgreja = $mResponsabilidade->getWhereMenuResponsabilidade();
//    }

	//Menu Principal (Superior Horizontal)
	 $sql= "select distinct mnu.mnucod,mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink , 
	 		mnu.mnutipo , mnu.mnustile , mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod,
	 		mnu.mnucod
	 		from seguranca.menu mnu, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu 
	 		where mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod 
	 		and pfu.usucpf in (".$lista_cpf.") and mnu.mnutipo=1 and mnu.mnushow='t' and mnu.mnustatus='A' 
	 		and mnu.sisid=".$_SESSION['sisid']."  
	 		{$whereIgreja}
	 		order by mnu.mnucod, mnu.mnuid , mnu.mnuidpai, mnu.mnucod, mnu.mnudsc";
	$arMenusPrimeiroNivel = @$db->carregar( $sql );

	//Sub-Menus Verticais
	$sql2= "select distinct  mnu.mnuid , mnu.mnuidpai , mnu.mnudsc , mnu.mnustatus , mnu.mnulink , mnu.mnutipo , mnu.mnustile , 
			mnu.mnuhtml , mnu.mnusnsubmenu , mnu.mnutransacao , mnu.mnushow , mnu.abacod, mnu2.mnudsc as mnudscpai,
			mnu.mnucod
			from seguranca.menu mnu, seguranca.menu mnu2, seguranca.perfilmenu pmn, seguranca.perfil pfl, seguranca.perfilusuario pfu 
			where mnu2.mnuid=mnu.mnuidpai and mnu.sisid=mnu2.sisid and mnu.mnuid=pmn.mnuid and pmn.pflcod=pfl.pflcod and pfl.pflcod=pfu.pflcod 
			and pfu.usucpf in (".$lista_cpf.") and mnu.mnuidpai is not null and mnu.mnushow= 't' and mnu.mnustatus='A' 
			and mnu.sisid=".$_SESSION['sisid']." 
			{$whereIgreja}
			order by mnu.mnutipo, mnu.mnuidpai, mnu.mnucod, mnu.mnuid,mnu.mnudsc";
	$arSubMenus = @$db->carregar( $sql2 );

	$_SESSION['menu'][$_SESSION['usucpf']][$_SESSION['sisid']] = array(
		'primeiro_nivel' => $arMenusPrimeiroNivel,
		'sub' => $arSubMenus
	);
}

unset($arMenusPrimeiroNivel, $arSubMenus, $sql, $sql2, $rs, $lista_cpf);
?>

<div class="page-sidebar navbar-collapse collapse">
	<!-- BEGIN SIDEBAR MENU -->
    <!-- page-header-fixed -->
	<ul class="page-sidebar-menu page-sidebar-menu-closed" data-keep-expanded="false" data-auto-scroll="true"
		data-slide-speed="200">
		<li class="sidebar-toggler-wrapper hide">
			<!-- BEGIN SIDEBAR TOGGLER BUTTON -->
			<div class="sidebar-toggler">
				<span></span>
			</div>
			<!-- END SIDEBAR TOGGLER BUTTON -->
		</li>
		<?php foreach($_SESSION['menu'][$_SESSION['usucpf']][$_SESSION['sisid']]['primeiro_nivel'] as $k => $primeiroNivel) :
			$agrupador = $primeiroNivel['mnusnsubmenu'] == 't' ;
			$link = $agrupador ?  'javascript:;' : $primeiroNivel['mnulink'];
			$atual = $link == str_replace('/'.$_SESSION['sisdiretorio'].'/','',$_SERVER['REQUEST_URI']);

			$icone = array();
			if($primeiroNivel['mnuhtml']){
				preg_match( '/<i class="[A-Za-z0-9 _-]+"><\/i>/', $primeiroNivel['mnuhtml'], $icone );
			}
			?>
			<li class="nav-item <?= ($atual ? 'active' : '') ?>">
				<a href="<?= $link; ?>" class="nav-link <?= ($agrupador ? 'nav-toggle' : '') ?>">
					<?= current($icone); ?>
					<span class="title"><?= $primeiroNivel['mnudsc'] ?></span>
                    <span class="selected"></span>
					<?= ($agrupador ? '<span class="arrow"></span>' : '') ?>
				</a>
				<?php
				if($agrupador) :
					desenhaSubNivelMenu($primeiroNivel, $_SESSION['menu'][$_SESSION['usucpf']][$_SESSION['sisid']]['sub']);
				endif;
				?>
			</li>
		<?php endforeach; ?>
	</ul>
	<!-- END SIDEBAR MENU -->
</div>
<script type="text/javascript">
	if(window.jQuery){
		$(function(){
			$('li.nav-item').has('li.nav-item.active:first').find('a.nav-link.nav-toggle:first').trigger('click');
			$('li.nav-item.open').addClass('active');
		});
	}
</script>

<?php
function desenhaSubNivelMenu ($primeiroNivel, $subNivelSession) {
	$subNiveis = array();
	foreach($subNivelSession as $sub){
		if($sub['mnuidpai'] == $primeiroNivel['mnuid']){
			$subNiveis[] = $sub;
		}
	}

	if(is_array($subNiveis) && count($subNiveis) > 0):
		?>
		<ul class="sub-menu">
			<?php foreach($subNiveis as $kSub => $sub) :
				$agrupador = $sub['mnusnsubmenu'] == 't' ;
				$link = $agrupador ?  'javascript:;' : $sub['mnulink'];
				$atual = $link == str_replace('/'.$_SESSION['sisdiretorio'].'/','',$_SERVER['REQUEST_URI']);

				$icone = array();
				if($sub['mnuhtml']){
					preg_match( '/<i class="[A-Za-z0-9_-]+"><\/i>/', $sub['mnuhtml'], $icone );
				}
				?>
				<li class="nav-item <?= ($atual ? 'active' : '') ?>">
					<a href="<?= $link; ?>" class="nav-link <?= ($agrupador ? 'nav-toggle' : '') ?>">
						<?= current($icone); ?>
						<span class="title"><?= $sub['mnudsc'] ?></span>
						<?= ($agrupador ? '<span class="arrow"></span>' : '') ?>
					</a>

					<?php
					if($agrupador) :
						desenhaSubNivelMenu($sub, $subNivelSession);
					endif;
					?>
				</li>
			<?php endforeach; ?>
		</ul>
		<?php
	endif;
}