<!DOCTYPE html>
<head>
	<title>VIS<?php if ( isset( $header ) ) echo ' ' . $header; ?></title>
	<meta charset="utf-8">
	<link rel="stylesheet/less" type="text/css" href="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/styles/font-awesome.min.css">
	<link rel="stylesheet/less" type="text/css" href="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/styles/top-menu.less">

	<script type="text/javascript" src="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/scripts/jquery.min.js"></script>
	<script type="text/javascript" src="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/scripts/top-menu.js"></script>
	<script type="text/javascript" src="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/scripts/form.js"></script>

	<link rel="stylesheet/less" type="text/css" href="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/styles/style.less">
	
	<?php
		if ( $page == 'login-page' ) {
			echo '<script type="text/javascript" src="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/scripts/login.js"></script>';
		}
		else if ( $page == 'add-user' ) {
			echo '<script type="text/javascript" src="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/scripts/add-user.js"></script>';
		}

		if ( isset( $role ) ) {
			switch ( $role ) {
				case 'admin':
					echo '<link rel="stylesheet/less" type="text/css" href="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/styles/admin.less">';
					echo '<script type="text/javascript" src="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/scripts/admin.js"></script>';
					break;
				case 'teacher':
					echo '<link rel="stylesheet/less" type="text/css" href="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/styles/teacher.less">';
					echo '<script type="text/javascript" src="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/scripts/teacher.js"></script>';
					break;
				case 'student':
					echo '<link rel="stylesheet/less" type="text/css" href="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/styles/student.less">';
					echo '<script type="text/javascript" src="http://www.stud.fit.vutbr.cz/~xmarti76/IIS/scripts/student.js"></script>';
					break;
			}
		}

		if ( ! isset( $type ) ) {
			$type = '';
		}
	?>
	
	<script src="//cdnjs.cloudflare.com/ajax/libs/less.js/2.7.2/less.min.js"></script>


</head>

<body class="<?php if ( isset( $page ) ) echo $page; ?> <?php if ( $type == 'inline' ) echo 'inline'; ?>" >
	<?php  if ( $type != 'inline' ) { ?>
	<div class="top">
		<div class="inner">
			<header>
				<a class="logo" href="http://www.stud.fit.vutbr.cz/~xmarti76/IIS">
					<h1>VIS</h1>
				</a>
			</header>
			
			<?php
			if ( ! isset( $no_menu ) ) {
			?>
				<!-- https://www.iconfinder.com/icons/106200/menu_icon#size=128 -->
				<img src="pictures/menu.png" id="menu-btn" alt="menu" width="0" height="0">
				<nav id="main-menu">
					<?php include( $_SESSION[ 'role' ] . '/menu.php' ); ?>
				</nav>
			
			<?php } ?>
		</div>
	</div>	
	<?php } ?>
