<?php 
	session_start();

	if ( isset( $_POST[ 'login'    ] ) &&
	     isset( $_POST[ 'password' ] ) &&
	     ! empty( $_POST[ 'login'  ] ) &&
	     ! empty( $_POST[ 'password' ] )
	) {
		$login    = $_POST[ 'login' ];
		$password = $_POST[ 'password' ];
		
		$_SESSION[ 'login' ] = $login;
		unset( $_SESSION[ 'error' ] );
	}
	else {
		if ( isset( $_POST[ 'login' ] ) ) {
			$_SESSION[ 'login' ] = $_POST[ 'login' ];
		}
		$_SESSION[ 'error' ] = 'Špatný login nebo heslo!';
		header( 'Location: index' );
		return;
	}

	
	include "./sql/sql.php";

	// Nactu login a hash hesla z databaze a otestuji
	$get_user = "
		SELECT password, role
		FROM  IIS_users
		WHERE login = '" . $login . "'";

	$user = $sql->query( $get_user );
	$sql->close();

	switch ( $user->num_rows ) {
		case 1:
			$user = $user->fetch_assoc();
			if ( hash( 'sha512' , $password ) == $user[ 'password' ] ) {

				$sql->close();

				switch ( $user[ 'role' ] ) {
					case 0:
						$_SESSION[ 'role' ] = 'student';
						header( 'Location: student/' );
						return;
						
					case 1:
						$_SESSION[ 'role' ] = 'teacher';
						header( 'Location: teacher/' );
						return;

					case 2:
						$_SESSION[ 'role' ] = 'admin';
						header( 'Location: admin/' );
						return;
						
					default:
						$_SESSION[ 'error' ] = 'Neplatná uživatelská role!';
						header( 'Location: index' );
						return;
				}
			}

		case 0:
			$_SESSION[ 'error' ] = 'Špatný login nebo heslo!';
			break;

		default:
			$_SESSION[ 'error' ] = 'Nalezeno více loginů \'' . $login . '\'';
	}

	header( 'Location: index' );
?>
