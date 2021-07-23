<?php
	setlocale( LC_ALL, 'czech' );
	
	if ( session_id() == '' ) {
		session_start();
	}

	// Otestuji, zda nekdo nechce nabourat stranku
	if ( ! isset( $_SESSION[ 'login' ] ) || $_SESSION[ 'login' ] == '' || $_SESSION[ 'role' ] != 'admin' ) {
		header( 'Location: http://www.stud.fit.vutbr.cz/~xmarti76/IIS/' );
		return;
	}

	// Pokud byli predany strance GET argumenty - nejspis ji vola javascript
	if ( isset( $_GET[ 'name' ] ) && isset( $_GET[ 'surname' ] ) && isset( $_GET[ 'role' ] ) ) {	
		echo username( $_GET[ 'name' ], $_GET[ 'surname' ], $_GET[ 'role' ] );
	}

	// Samotna funkce
	function username( $name, $surname, $role ) {
		include "/home/users/xm/xmarti76/WWW/IIS/sql/sql.php";
		
		// Odstranim diakritiku
		$name    = iconv( 'utf-8', 'ascii//TRANSLIT', $name );
		$name    = str_replace( '\'', '', $name );
		$name    = str_replace( '"', '', $name );
		
		$surname = iconv( 'utf-8', 'ascii//TRANSLIT', $surname );
		$surname = str_replace( '\'', '', $surname );
		$surname = str_replace( '"', '', $surname );
		
		// Prijmeni je moc kratke => musim pripojit jmeno
		if ( strlen( $surname ) < 5 ) {
			$surname .= $name;
		}
		
		// Stale moc kratke => pripojim xxx
		if ( strlen( $surname ) < 5 ) {
			$surname .= 'xxxxx';
		}

		switch ( $role ) {
			case 'admin':    $role = 'a';  break;
			case 'teacher':  $role = 't';  break;
			case 'student':
			default:         $role = 's';  break;
			
		}

		// Zkratim a na zacatek pridam pismenko
		$surname = substr( $surname, 0, 5 );
		$surname = $role . strtolower( $surname );
		// Ziskam vsechny uzivatele s podobnym jmenem
		$users = $sql->query( "SELECT login FROM IIS_users WHERE login LIKE '$surname%' ORDER BY login DESC " );

		if   ( $users->num_rows < 9 ) {  $zero = '0';  }
		else                          {  $zero = '' ;  }

		$sql->close();
		return $surname . $zero . $users->num_rows;
	}

?>