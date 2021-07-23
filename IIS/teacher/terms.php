<?php
	$exam = '';
	if ( isset( $_GET[ 'exam' ] ) ) {
		$exam = 'WHERE exam = "' . $_GET[ 'exam' ] . '"';
	}

	include ( './sql/sql.php' );
	// Nactu vsechny predmety pro dany login
	$ids = $sql->query( "SELECT id_subject FROM IIS_user_subject WHERE id_user = '" . $_SESSION[ 'login' ] . "'" );
	if ( $ids->num_rows > 0 ) {
		$subjects = ' AND (';
		if ( empty( $exam ) ) {
			$subjects = 'WHERE ';
		}

		while ( $id = $ids->fetch_assoc() ) {
			$subjects .=  'subject = \'' . $id[ 'id_subject' ] . '\' OR ';
		}

		$subjects = substr( $subjects, 0, -4 );
		
		if ( ! empty( $exam ) ) {
			$subjects .= ')';
		}
	}

	$subjects = '';

	// $terms = $sql->query( "SELECT subject, exam_start, exam_end, place, t.id FROM IIS_terms AS t JOIN IIS_exams AS e ON t.exam = e.id $exam $subjects" );
	$terms = $sql->query( "
		SELECT subject, exam_start, exam_end, login_start, login_end, place, max_students, COUNT( ut.id_user ), t.id
		FROM IIS_terms AS t
		JOIN IIS_exams AS e
			ON t.exam = e.id
		LEFT JOIN IIS_user_term AS ut
			ON ut.id_term = t.id
		$exam $subjects
		GROUP BY t.id
	" );
	
	if ( $terms->num_rows > 0 ) {?>

		<table>
			<tr>
				<?php if ( ! isset( $_GET[ 'exam' ] ) ) echo "<th>Předmět</th>"; ?>
				<th>Zkouška</th>
				<th>Přihlašování</th>
				<th>Přihlášeno</th>
				<th>Místnost</th>
				<th>Upravit</th>
			</tr>

	<?php
		while ( $term = $terms->fetch_assoc() ) {
			$id = $term[ 'id' ];
			echo "\t<tr>\n";
			if ( ! isset( $_GET[ 'exam' ] ) ) echo "\t\t<td><a href=\"./subjects&id=" . $term[ 'subject' ] . "\">" . $term[ 'subject' ] . "</a></td>\n";

			$evaluation = '';
			if ( $term[ 'exam_end' ] < date( 'Y-m-d H:i:s' ) ) $evaluation = '<a title="Přidat hodnocení" class="fa fa-pencil-square-o" style="padding-left: 1em;" href="./evaluations&id=' . $id . '"></a>';
			echo "
				\t\t<td>" . $term[ 'exam_start' ] . " - " . $term[ 'exam_end' ] . "</td>\n
				\t\t<td>" . $term[ 'login_start' ] . " - " . $term[ 'login_end' ] . "</td>\n
				\t\t<td>" . $term[ 'COUNT( ut.id_user )' ] . "/" . $term[ 'max_students' ] . "</td>\n
				\t\t<td>" . $term[ 'place' ] . "</td>\n
				\t\t<td class=\"edit\"><a title=\"Upravit termín\" class=\"fa fa-pencil\" style=\"padding-right: 1em;\" data-id=\"$id\"></a><a title=\"Smazat termín\" class=\"fa fa-times\" data-id=\"$id\"></a>$evaluation</td>\n
				\t</tr>\n
			";
		} ?>
		
	</table>

	<?php }
?>


<iframe class="term" id="edit-term" src width="100%" height="0"></iframe>
