<?php
	function echo_array ( $array ) {
		echo "<pre>";
		print_r( $array );
		echo "</pre>";
	}

	header( 'Content-Type: text/html; charset=utf-8' );
	session_start();

	// Zjistim, zda jsem prihlasen
	if ( ! isset( $_SESSION[ 'login' ] ) || $_SESSION[ 'login' ] == '' ) {
		header( 'Location: http://www.stud.fit.vutbr.cz/~xmarti76/IIS/' );
		return;		
	}

	// Otestuji, zda nekdo nechce nabourat stranku
	if ( isset( $_GET[ 'role' ] ) && $_GET[ 'role' ] != $_SESSION[ 'role' ] ) {
		$url = './no-access.php';
		$header = 'Přístup zamítnut!';
		$title = $header;
		$page = null;
	}
	else {
		$role = $_SESSION[ 'role' ];
		include( 'define-menu.php' );

		if ( isset( $_GET[ 'page' ] ) ) {
			$page = $_GET[ 'page' ];
		}
		else {
			$page = $pages[ $role ][ 'default' ] ;
		}
		
		// Odstranim .php
		if ( substr( $page, -4 ) == '.php' ) {
			$page = substr( $page, 0, -4 );
		}
		
		$type = '';
		if ( isset( $pages[ $role ][ $page ][ 'type' ] ) ) {
			$type = $pages[ $role ][ $page ][ 'type' ];
		}

		if ( ! array_key_exists( $page, $pages[ $role ] ) ) {
			$page = $pages[ $role ][ 'default' ] ;
		}

		if ( array_key_exists( 'id', $pages[ $role ][ $page ] ) ) {
			$page = $pages[ $role ][ $page ][ 'id' ];
		}

		if ( array_key_exists( 'url', $pages[ $role ][ $page ] ) ) {
			$url = $pages[ $role ][ $page ][ 'url' ];
		}
		else {
			$url = './' . $role . '/' . $page . '.php';
		}
		$header = $pages[ $role ][ $page ][ 'name' ];
		
		$title = $header;
		if ( isset( $pages ) && $type == 'inline' ) $title = '';
		if ( isset( $pages[ $role ][ $page ][ 'title' ] ) && $pages[ $role ][ $page ][ 'title' ] === false ) $title = '';
	}


	echo "<!-- Hlavicka -->\n";
	include( 'header.php' );
	echo "<div class=\"body-content\">\n";
	echo "<div class=\"inner\" id=\"content\">\n";
	echo "<!-- Hlavni obsah stranky -->\n";
	echo "\n\n<article id=\"main\">\n";
	echo "\t<h1>$title</h1>\n";
	include( $url );
	echo "\n\n</article>\n";

	echo "</div>\n";
	echo "</div>\n";
	echo "<!-- Paticka -->\n";
	if ( $type != 'inline' ) include( 'footer.php' );
?>

</body>
</html>

