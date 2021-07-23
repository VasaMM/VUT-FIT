<?php

	$ask = array(
		'action' => '',
		'method' => 'post',
		'id'     => 'add-user',
		'class'  => 'random',
		'submit' => 'Vytvořit',
		'fields' => array (
			'IIS_users' => array (
				'name'   => 'Uživatel',
				'inputs' => array (
					'login' => array (
						'name'     => 'Login',
						'type'     => 'text',
						'disabled' => true,
					),
					'password' => array (
						'name'     => 'Heslo',
						'type'     => 'password',
						'default'  => '',
						'required' => true,
					),
					'role' => array (
						'name'    => 'Role',
						'type'    => 'select',
						'default' => 'student',
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
						'required' => true,
					),
					'surname' => array (
						'name'     => 'Příjmení',
						'type'     => 'text',
						'required' => true,
					),
					'email' => array (
						'name'     => 'Email',
						'type'     => 'email',
						'required' => true,
					),
					'birthday' => array (
						'name'     => 'Datum narození',
						'type'     => 'date',
						'required' => true,
					),
					'phone' => array (
						'name' => 'Telefon',
						'type' => 'phone',
					),
					'gender' => array (
						'name'     => 'Pohlaví',
						'type'     => 'radio',
						'default'  => 'male',
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
						'required' => true,
					),
					'street' => array (
						'name'     => 'Ulice',
						'type'     => 'text',
					),
					'house_number' => array (
						'name'     => 'Číslo popisné',
						'type'     => 'number',
					),
					'psc' => array (
						'name'     => 'PSČ',
						'type'     => 'number',
						'required' => true,
					),
				),
			),
		),
	);

	include "./form.php";

	$addUser = new form( $ask );
		$addUser->make();
		$addUser->printForm();

	$output = $addUser->output();

	include "./sql/username.php";

	if ( isset( $output[ 'IIS_persons' ][ 'name' ] ) && isset( $output[ 'IIS_persons' ][ 'surname' ] ) && isset( $output[ 'IIS_users' ][ 'role' ] ) ) {
		$output[ 'IIS_users' ][ 'login' ] = username( $output[ 'IIS_persons' ][ 'name' ], $output[ 'IIS_persons' ][ 'surname' ], $output[ 'IIS_users' ][ 'role' ] );
	}

	if ( isset( $output[ 'IIS_users' ][ 'role' ] ) ) {
		switch ( $output[ 'IIS_users' ][ 'role' ] ) {
			case 'student':  $output[ 'IIS_users' ][ 'role' ] = 0;  break;
			case 'teacher':  $output[ 'IIS_users' ][ 'role' ] = 1;  break;
			case 'admin':    $output[ 'IIS_users' ][ 'role' ] = 2;  break;
			
			default:
				$output[ 'IIS_users' ][ 'role' ] = 0;
		}
	}

	if ( $addUser->isOK() ) {
		$output[ 'IIS_users' ][ 'password' ] = hash( 'sha512' , $output[ 'IIS_users' ][ 'password' ]  );
		
		include "./sql/sql.php";

		// Postupne naplnim tabulky daty
		if ( $sql->add_row( 'IIS_addresses', $output[ 'IIS_addresses' ] ) ) {
			// echo "Add row to IIS_addresses was <strong>successfully</strong><br>"; 
			
			$output[ 'IIS_persons' ][ 'address' ] = $sql->last_id;
		
			if ( $sql->add_row( 'IIS_persons', $output[ 'IIS_persons' ] ) ) {
				// echo "Add row to IIS_persons was <strong>successfully</strong><br>"; 
				
				$output[ 'IIS_users' ][ 'person' ] = $sql->last_id;

				if ( $sql->add_row( 'IIS_users', $output[ 'IIS_users' ] ) ) {
					// echo "Add row to IIS_users was <strong>successfully</strong><br>"; 
					echo "Uživatel byl vytvořen<br>"; 
				}
				else {
					echo "<span style=\"color: red; font-weight: bold;\">Error in IIS_users: " . $sql->error . "</span><br>";
				}
			}
			else {
				echo "<span style=\"color: red; font-weight: bold;\">Error in IIS_persons: " . $sql->error . "</span><br>";
			}
		}
		else {
			echo "<span style=\"color: red; font-weight: bold;\">Error in IIS_addresses: " . $sql->error . "</span><br>";
		}
		
		$sql->close();
	}

?>
