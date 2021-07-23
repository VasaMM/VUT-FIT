<?php
	$id = '';
	if ( isset( $_GET[ 'id' ] ) ) {
		$id = $_GET[ 'id' ];
	}

	
	if ( $id == '' ) {
		echo "Nejednoznačné ID termínu, musíte ho nejdřív <a href=\"./terms\" title=\"Vyhledat termín\">vyhledat</a>.";
		return;
	}
	else {
		// Kontrola na práva
		include ( './sql/sql.php' );
		
		$valid = $sql->query( "
			SELECT id_user
			FROM IIS_terms AS t
			JOIN IIS_exams AS e ON exam = e.id
			JOIN IIS_user_subject AS us ON e.subject = us.id_subject
			WHERE id_user = '" . $_SESSION[ 'login' ] . "'
			AND t.id = '$id'
		" );
		
		if ( $valid->num_rows == 0 ) {
			echo "Nemáte právo přistupovat k danému předmětu.";
			return;
		}

		$exam = $sql->query( "SELECT halftime, exam_start, exam_end, max_points, min_points, questions, subject FROM IIS_terms AS t JOIN IIS_exams AS e ON exam = e.id WHERE t.id = '$id'" );
		
		if ( $exam->num_rows == 0 ) {
			echo "Nejednoznačné ID termínu, musíte ho nejdřív <a href=\"./terms\" title=\"Vyhledat termín\">vyhledat</a>.";
			return;
		}
		$exam = $exam->fetch_assoc();

		// Vypsat název předmětu a info o zkoušce
		$halftime = 'semestrílní';
		if ( $exam[ 'halftime' ] ) $halftime = 'půlsemestrální';
		echo "<h1>Hodnocení $halftime zkoušky z předmětu " . $exam[ 'subject' ] . "</h1><br>";
		echo "<strong>Termín:</strong> " . $exam[ 'exam_start' ] . " - " . $exam[ 'exam_end' ] . "<br>";
		echo "<strong>Minimálně bodů:</strong> <span class=\"min-points\">" . $exam[ 'min_points' ] . "</span><br>";
		echo "<strong>Maximálně bodů:</strong> <span class=\"max-points\">" . $exam[ 'max_points' ] . "</span><br>";

		echo "
			<form method=\"post\" class=\"evaluations\">
			<table>
				<tr>
				<th>Login</th>
		";

		for ( $i = 1 ; $i <= $exam[ 'questions' ] ; $i++ ) {
			echo "<th>$i</th>";
		}

		echo "
			<th>Celkem</th>
			<th>Komentář</th>
			</tr>
		";

		$users = $sql->query( "SELECT id_user FROM IIS_user_term WHERE id_term = '$id'" );
		if( $users->num_rows > 0 ) {
			$error = false;

			$points = '';
			$comment = '';
			while ( $user = $users->fetch_assoc() ) {
				$login = $user[ 'id_user' ];
				
				
				echo "
					<tr>
					<td>$login</td>
				";
				
				$evaluation = $sql->query( "SELECT points, comment FROM IIS_evaluations WHERE login = '$login' AND term_id = $id" );
				$tmp = '';
				if ( $evaluation->num_rows > 0 ) {
					$evaluation = $evaluation->fetch_assoc();

					$comment = $evaluation[ 'comment' ];

					$tmp = $evaluation[ 'points' ];
				}

				$sum = 0;
				for ( $i = 1 ; $i <= $exam[ 'questions' ] ; $i++ ) {
					$val = '';

					if ( ! empty( $tmp ) ) {
						$tmp = substr( $tmp, strpos( $tmp, ',' ) + 1 );

						$pos = strpos( $tmp, ',');
						if ( $pos ) {
							$val = substr( $tmp, 0, $pos );
						}
						else {
							$val = $tmp;
						}
					}

				

					if ( isset( $_POST[ $login . '-' . $i ] ) ) {
						$val = $_POST[ $login . '-' . $i ];
					}

					if ( ! empty( $val ) ) {
						$points .= ',' . $val;
						$sum += $val;
					}
					else {
						$points .= ',0';
					}

					echo '<td><input type="number" size=1 name="' . $login . '-' . $i . '" data-points value="' . $val . '"></td>';
				}
				if ( $sum < 0 ) $sum = 0;

				$red = '';
				if ( $sum > $exam[ 'max_points' ] ) {
					$red = "warning";
					$error = true;
				}

				$blue = '';
				if ( $sum < $exam[ 'min_points' ] ) {
					$blue = 'style="color: blue;"';
				}

				if ( isset( $_POST[ $login . '-comment' ] ) ) $comment = $_POST[ $login . '-comment' ];

				echo '
					<td><input type="text" disabled size=1 class="total-points ' . $red . '" ' . $blue . ' data-points value="' . $sum . '"></td>
					<td><input type="text" size=60 name="' . $login . '-comment" data-points value="' . $comment . '"></td>
					</tr>
				';

			}

			// Odeslal jsem formular
			// Ulozim data do DB
			if ( ! empty( $_POST ) ) {
				$exist = $sql->query( "SELECT * FROM IIS_evaluations WHERE login = '$login' AND term_id = $id" );
				if ( $exist->num_rows > 0 ) {
					// Aktualizuji stavajici hodnoty
					$sql->query( "
						UPDATE IIS_evaluations
						SET comment = '$comment', points = '$points', total = '$sum', evaluator = '" . $_SESSION[ 'login' ] . "', evaluated = '" . date( 'Y-m-d H:i:s' ) . "'
						WHERE login = '$login' AND term_id = $id
					" );
				}
				else {
					// Vlozim nove
					$sql->query( "
						INSERT INTO IIS_evaluations ( login, term_id, comment, points, total, evaluator, evaluated )
						VALUES ( '$login', '$id', '$comment', '$points', '$sum', '" . $_SESSION[ 'login' ] . "', '" . date( 'Y-m-d H:i:s' ) . "' )
					" );
				}
			}
		}
echo $sql->error;
		{ $users = $users->fetch_assoc(); }
	}

	echo '
		</table>
		<input type="submit" value="Odeslat">
		</form>
	';

	if ( ! $error ) {
		// Ulozim / aktualizuji DB
		
	}
?>