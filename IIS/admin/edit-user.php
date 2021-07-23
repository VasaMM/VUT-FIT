<?php
	if ( ! isset( $_GET[ 'login' ] ) ) {
		echo "Nebyl zadán login uživatele, musíte ho nejdřív <a href=\"./search_user\" title=\"Vyhledat uživatele\">vyhledat</a>";
		return;
	}
	
	include( './sql/sql.php' );

	$user =  $sql->query( "
		SELECT *
		FROM IIS_users
		JOIN IIS_persons   ON person           = IIS_persons.id
		JOIN IIS_addresses ON IIS_addresses.id = IIS_persons.address
		WHERE login = '" . $_GET[ 'login' ] . "'
	");

	if ( $user->num_rows != 1 ) {
		echo "Byl zadán nejednoznačný login uživatele, musíte ho nejdřív <a href=\"./search_user\" title=\"Vyhledat uživatele\">vyhledat</a>";
		return;
	}

	$user = $user->fetch_assoc();

	include( './search-user-function.php' );

	$editUser = array(
		'action' => '',
		'method' => 'post',
		'submit' => 'Uložit změny',
		'fields' => array (
			'IIS_users' => array (
				'name'   => 'Uživatel',
				'inputs' => array (
					'login' => array (
						'name'     => 'Login',
						'type'     => 'text',
						'default'  => $user[ 'login' ],
						'disabled' => true,
					),
					'password' => array (
						'name'     => 'Heslo',
						'type'     => 'password',
						'default'  => '**********',
					),
					'role' => array (
						'name'    => 'Role',
						'type'    => 'select',
						'default' => role_to_string( $user[ 'role' ] ),
						'values'  => array (
							'student' => 'Student',
							'teacher' => 'Vyučující',
							'admin'   => 'Admin',
						),
					),
				),
			),
			'IIS_persons' => array (
				'name'   => 'Podrobnosti',
				'inputs' => array (
					'name' => array (
						'name'     => 'Jméno',
						'type'     => 'text',
						'default'  => $user[ 'name' ],
						'required' => true,
					),
					'surname' => array (
						'name'     => 'Příjmení',
						'type'     => 'text',
						'default'  => $user[ 'surname' ],
						'required' => true,
					),
					'email' => array (
						'name'     => 'Email',
						'type'     => 'email',
						'default'  => $user[ 'email' ],
						'required' => true,
					),
					'birthday' => array (
						'name'     => 'Datum narození',
						'type'     => 'date',
						'default'  => $user[ 'birthday' ],
						'required' => true,
					),
					'phone' => array (
						'name' => 'Telefon',
						'type' => 'phone',
						'default'  => $user[ 'phone' ],
					),
					'gender' => array (
						'name'     => 'Pohlaví',
						'type'     => 'radio',
						'default'  => gender_to_string( $user[ 'gender' ] ),
						'required' => true,
						'values'   => array (
							'male'   => 'Muž',
							'female' => 'Žena',
						),
					),
				),
			),
			'IIS_addresses' => array (
				'name'   => 'Adresa',
				'inputs' => array (
					'city' => array (
						'name'     => 'Město',
						'type'     => 'text',
						'default'  => $user[ 'city' ],
						'required' => true,
					),
					'street' => array (
						'name'     => 'Ulice',
						'type'     => 'text',
						'default'  => $user[ 'street' ],
					),
					'house_number' => array (
						'name'     => 'Číslo popisné',
						'type'     => 'number',
						'default'  => $user[ 'house_number' ],
					),
					'psc' => array (
						'name'     => 'PSČ',
						'type'     => 'number',
						'default'  => $user[ 'psc' ],
						'required' => true,
					),
				),
			),
		),
	);


	include( './form.php' );

	$editUser = new form( $editUser );
	$editUser->make();
	$editUser->printForm( false );

	$output = $editUser->output();

	if (
		! empty( $output[ 'IIS_users'      ] ) &&
		! empty( $output[ 'IIS_persons'    ] ) &&
		! empty( $output[ 'IIS_addresses'  ] )
	) {

		$output[ 'IIS_users' ][ 'role' ] = role_to_int( $output[ 'IIS_users' ][ 'role' ] );

		if ( $output[ 'IIS_users' ][ 'password' ] === '**********' ) {
			unset( $output[ 'IIS_users' ][ 'password' ] );
		}

		if ( $editUser->isOK() ) {		
			include ( './sql/sql.php' );

			$query = 'UPDATE IIS_users SET ';
			foreach ( $output[ 'IIS_users' ] as $key => $value) {
				$query .= "$key = '$value', ";
			}
			$query = substr( $query, 0, -2 );
			$query .= " WHERE login = '" . $user[ 'login' ] . "'";
			

			if ( $sql->query( $query ) ) {
				$query = 'UPDATE IIS_persons SET ';
				foreach ( $output[ 'IIS_persons' ] as $key => $value) {
					$query .= "$key = '$value', ";
				}
				$query = substr( $query, 0, -2 );
				$query .= " WHERE id = '" . $user[ 'person' ] . "'";
				
				if ( $sql->query( $query ) ) {
					$query = 'UPDATE IIS_addresses SET ';
					foreach ( $output[ 'IIS_addresses' ] as $key => $value) {
						$query .= "$key = '$value', ";
					}
					$query = substr( $query, 0, -2 );
					$query .= " WHERE id = (SELECT address FROM IIS_persons WHERE id = '" . $user[ 'person' ] . "')";

					if ( $sql->query( $query ) ) {
						echo "Uloženo<br>";
					}
					else {
						echo $sql->error;
					}	
				}
				else {
					echo $sql->error;
				}
				
			}
			else {
				echo $sql->error;
			}
		}
	}

?>