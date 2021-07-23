<?php
	if ( ! isset( $_GET[ 'exam' ] ) ) {
		$exam = '';
		$where = '';
	}
	else {
		$exam = "WHERE exam = '" . $_GET[ 'exam' ] . "'";
		$where = "AND id = '" . $_GET[ 'exam' ] . "'";
	}

	include ( './sql/sql.php' );
	
	// Zkontroluji existenci a prava k danemu terminu
	$valid = $sql->query( "
		SELECT subject, halftime
		FROM IIS_exams
		JOIN IIS_user_subject
			ON subject = id_subject
		WHERE id_user = '" . $_SESSION[ 'login' ] . "'
		$where
	" );
echo $sql->error;
	if ( $valid->num_rows == 0 ) {
		echo "Nemáte právo přistupovat k tomuto předmětu.";
		return;;
	}

?>

	<br><table>
		<tr>


<?php
	$valid = $valid->fetch_assoc();

	$subject = $valid[ 'subject' ];
	$type = '';
	if ( $valid[ 'halftime' ] ) { $type = 'půl'; }
	if ( isset( $_GET[ 'exam' ] ) ) {
		echo "<h1>Termíny k " . $type . "semestrální zkoušce z předmětu <a href=\"./subjects&id=$subject\">$subject</a></h1><br>";
	}
	else {
		echo "<h1>Termíny</h1>";
		echo "<th>Předmět</th>";
	}
?>

			<th>Zkouška</th>
			<th>Přihlašování</th>
			<th>Přihlášeno</th>
			<th>Místnost</th>
			<th>Body</th>
			<th>Přihlásit</th>
			<th></th>
		</tr>

<?php
	$terms = $sql->query( "
		SELECT subject, exam_start, exam_end, login_start, login_end, place, max_students, COUNT( ut.id_user ), t.id, min_points
		FROM IIS_terms AS t
		JOIN IIS_exams AS e
			ON t.exam = e.id
		LEFT JOIN IIS_user_term AS ut
			ON ut.id_term = t.id
		$exam
		GROUP BY t.id
	" );

	while ( $term = $terms->fetch_assoc() ) {
		$id = $term[ 'id' ];
		$status = $sql->query( "SELECT id_user FROM IIS_user_term WHERE id_term = '$id' AND id_user='" . $_SESSION[ 'login' ] . "'" );
		
		$logged = '';
		if ( $status->num_rows > 0 ) { $logged = 'Přihlášen'; }

		$red = "";
		if ( $term[ 'COUNT( ut.id_user )' ] == $term[ 'max_students' ] ) { $red = 'class="red"'; }

		$link = "";
		if ( date( 'Y-m-d H:i:s' ) > $term[ 'login_start' ] && date( 'Y-m-d H:i:s' ) < $term[ 'login_end' ] && ( $red == '' || $logged != '' ) ) { $link = "<a title=\"Zapsat\" class=\"fa fa-sign-in\" data-id=\"$id\"></a>"; }

		$total = '-';
		$min = '';
		$title = '';
		$mouse = '';
		if ( date( 'Y-m-d H:i:s' ) > $term[ 'exam_end' ] ) {
			$evaluation = $sql->query( "SELECT total, points, evaluator, evaluated, comment FROM IIS_evaluations WHERE login = '" . $_SESSION[ 'login' ] . "' AND term_id = '$id'" );
			if ( $evaluation->num_rows > 0 ) {
				$evaluation = $evaluation->fetch_assoc();
				$total = $evaluation[ 'total' ];
				if ( $total < $term[ 'min_points' ] ) { $min = 'class="red"'; }

				$title = "Body: " . substr( $evaluation[ 'points' ], 1 ) . "\nHondotil: " . $evaluation[ 'evaluator' ] . " v " . $evaluation[ 'evaluated' ] . "\nKomentář: " . $evaluation[ 'comment' ];

				$mouse = 'style="cursor: help;"';
			}
		}

		echo "\t<tr>\n";
		if ( ! isset( $_GET[ 'exam' ] ) )  echo "\t\t<td><a href=\"./subjects&id=" . $term[ 'subject' ] . "\">" . $term[ 'subject' ] . "</a></td>\n";

		echo "
			\t\t<td>" . $term[ 'exam_start' ] . " - <br>" . $term[ 'exam_end' ] . "</td>\n
			\t\t<td>" . $term[ 'login_start' ] . " - <br>" . $term[ 'login_end' ] . "</td>\n
			\t\t<td $red>" . $term[ 'COUNT( ut.id_user )' ] . "/" . $term[ 'max_students' ] . "</td>\n
			\t\t<td>" . $term[ 'place' ] . "</td>\n
			\t\t<td $min title=\"$title\" $mouse>$total</td>\n
			\t\t<td>$link</td>\n
			\t\t<td class=\"logged\">$logged</td>\n
			\t</tr>\n
		";
	}
?>

</table>
