<?php

	include "./search-user-function.php";

	$ask = array(
		'action' => '',
		'method' => 'post',
		'id'     => 'search-user',
		'submit' => 'Najít',
		'fields' => array (
			'user' => array (
				'name'   => 'Najít uživatele',
				'inputs' => array (
					'login' => array (
						'name' => 'Login',
						'type' => 'text',
					),
					'name' => array (
						'name' => 'Jméno',
						'type' => 'text',
					),
					'surname' => array (
						'name' => 'Příjmení',
						'type' => 'text',
					),
					'role' => array (
						'name'    => 'Role',
						'type'    => 'select',
						'default' => 'none',
						'values'  => array (
							'none'    => '',
							'student' => 'Student',
							'teacher' => 'Vyučující',
							'admin'   => 'Admin',
						),
					),
				),
			),
		),
	);

	include "./form.php";

	$user = new form( $ask );
	$user->make();

	$user->printForm( false );

	$output = $user->output( 'user' );
	
	// Formular byl uz jednou zavolam, muzu zobrazit vysledky
	if ( ! empty( $output ) ) {
		$users = search_user( $output );

		echo "<br><table>\n";
		echo "\t<tr>";
		echo "\t\t<th>Role</th>\n";
		echo "\t\t<th>Login</th>\n";
		echo "\t\t<th>Jméno</th>\n";
		echo "\t\t<th>Příjmení</th>\n";
		echo "\t\t<th></th>\n";
		echo "\t</tr>";

		while ( $user = $users->fetch_assoc() ) {

			echo "\t<tr>";
			foreach ( $user as $key => $value ) {
				if ( $key == 'role' ) {
					switch ( $value ) {
						case 0:  echo "\t\t<td>Student</td>\n";  break;
						case 1:  echo "\t\t<td>Učitel</td>\n";   break;
						case 2:  echo "\t\t<td>Admin</td>\n";    break;
					}
				}
				else {
					echo "\t\t<td>$value</td>\n";
				}
			}
			echo "\t\t<td><a href=\"./edit-user&login=" . $user[ 'login' ] . "\" title=\"Upravit uživatele " . $user[ 'name' ] . " " . $user[ 'surname' ] . "\" class=\"fa fa-pencil\" style=\"padding-right: 1em;\"></a>
				<a title=\"Smazat uživatele " . $user[ 'name' ] . " " . $user[ 'surname' ] . "\" class=\"delete fa fa-times\" data-login=\"" . $user[ 'login' ] . "\"></a></td>\n
				\t</tr>";
		}
		
		echo '</table>';
		echo '<div id="delete-result"></div>';
	}

?>
