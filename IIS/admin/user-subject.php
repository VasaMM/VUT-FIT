<?php

	if ( ! isset( $_GET[ 'id' ] ) ) {
		echo "<h1>Spravovat uživatele v předmětech</h1><br>";

		$ask = array(
			'action' => '',
			'method' => 'post',
			'id'     => 'search-subject',
			'submit' => 'Najít',
			'fields' => array (
				'subject' => array (
					'name'   => 'Najít předmět',
					'inputs' => array (
						'id' => array (
							'name' => 'ID',
							'type' => 'text',
						),
						'name' => array (
							'name' => 'Jméno',
							'type' => 'text',
						),
					),
				),
			),
		);

		include "./form.php";

		$subject = new form( $ask );
		$subject->make();

		$subject->printForm( false );

		$output = $subject->output( 'subject' );


		$id   = '';
		$name = '';
		if ( isset( $output[ 'id'   ] ) ) $id   = &$output[ 'id'   ];
		if ( isset( $output[ 'name' ] ) ) $name = &$output[ 'name'    ];

		include "./sql/sql.php";

		$subjects = $sql->query( "
			SELECT id, name
			FROM IIS_subjects
			WHERE id   LIKE '%$id%'
			AND   name LIKE '%$name%'
		");
?>
	
	<br>
	<table>
	<tr>
	<th>ID</th>
	<th>Jméno</th>
	<th></th>
	</tr>

<?php
		while ( $subject = $subjects->fetch_assoc() ) {

			echo "
				\t<tr>
				\t\t<td>" . $subject[ 'id' ] . "</td>\n
				\t\t<td>" . $subject[ 'name' ] . "</td>\n
				\t\t<td><a href=\"./user-subject&id=" . $subject[ 'id' ] . "\" class=\"fa fa-address-book-o\"></a></td>\n
				\t</tr>";
		}
		
		echo '</table>';
	}
	else {
		$id = $_GET[ 'id' ];
		include "./sql/sql.php";

		// Zkontroluji existenci
		$valid = $sql->query( "SELECT id FROM IIS_subjects WHERE id = '$id'" );

		if ( $valid->num_rows == 0 ) {
			echo "Zadaný předmět neexistuje. Musíte ho nejdrřív <a href=\"./user-subject\" title=\"Vyhledat předmět\">vyhledat</a>.";
			return;
		}

		echo "<h1>Spravovat uživatele v předmětu $id</h1><br>";

		$users = $sql->query( "
			SELECT login, id_subject FROM IIS_users
			LEFT JOIN IIS_user_subject ON login = id_user
			WHERE role != '2'
			ORDER BY role DESC, login ASC
		" );

		$processed = array();

		while ( $user = $users->fetch_assoc() ) {

			// Uzivatele jsem jeste nezpracoval
			if ( ! isset( $processed[ $user[ 'login' ] ] ) ) {
				$processed[ $user[ 'login' ] ] = false;
			}
			
			// Uzivatel uz ma predmet zapsany
			if ( $user[ 'id_subject' ] == $id ) {
				$processed[ $user[ 'login' ] ] = true;
			}
		}


		// Odeslal jsem formular
		// Musim ulozit zmeny do DB
		if ( ! empty( $_POST ) ) {
			foreach ( $processed as $login => $signed ) {
				// User je zapsany v DB
				if ( $signed ) {
					// Neni oznaceny => musim ho smazat
					if ( ! isset( $_POST[ $login ] ) ) {
						$sql->query( "DELETE FROM IIS_user_subject WHERE id_user = '$login' AND id_subject = '$id'" );
						$processed[ $login ] = false;
					}
					// Je oznaceny => nemusim delat nic
					continue;
				}
				// User neni zapsany v DB
				else {
					// Je oznaceny, musim ho zapsat
					if ( isset( $_POST[ $login ] ) ) {
						$sql->add_row( 'IIS_user_subject', array( 'id_subject' => $id, 'id_user' => $login ) );
						$processed[ $login ] = true;
					}
					// Neni ani oznaceny, nemusim delat nic
					continue;
				}
			}
		}
		
		function print_users( $users, $condition ) {
			$checked = '';
			$data = 'data-group="other"';
			if ( $condition ) {
				$checked = 'checked';
				$data = 'data-group="assigned"';
			}
			
			echo "
				<table>\n
				\t<tr>
					<th>Login</th>
					<th><input type=\"checkbox\" class=\"check-all\" $data></th>
				</tr>\n
			";
			
			foreach ( $users as $login => $cond ) {

				if ( $condition == $cond ) {
					echo "
						\t<tr>
							<td>$login</td>
							<td><input type=\"checkbox\" name=\"$login\" value=\"$login\" $checked $data></td>
						</tr>\n
					";
				}
			}
			
			echo "</table>";
		}

		echo "
			<br>
			<form method=\"post\">
			<h3>Přiřazení uživatelé:</h3><br>
		";

		print_users( $processed, true );

		echo "<br><hr><h3>Ostatní uživatelé:</h3><br>";

		print_users( $processed, false );

		echo "
			<input type=\"submit\" value=\"Uložit\">
			</form>
		";
	}
?>
