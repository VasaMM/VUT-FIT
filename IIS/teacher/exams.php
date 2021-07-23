<?php
	$subject = '';
	if ( isset( $_GET[ 'id' ] ) ) {
		$subject = $_GET[ 'id' ];
	}

	include ( './sql/sql.php' );

	// Nactu vsechny predmety pro dany login
	$ids = $sql->query( "SELECT id_subject FROM IIS_user_subject WHERE id_user = '" . $_SESSION[ 'login' ] . "'" );
	if ( $ids->num_rows > 0 ) {
		$subjects = 'WHERE ';
		$find = false;
		while ( $id = $ids->fetch_assoc() ) {
			$subjects .=  'subject = \'' . $id[ 'id_subject' ] . '\' OR ';
			if ( $id[ 'id_subject' ] == $subject ) { $find = true; }
		}

		if ( $find ) {
			$subjects = "WHERE subject = '$subject'";
		}
		else if ( isset( $_GET[ 'id' ] ) ) {
			echo "K danému předmětu nemáte právo přistupovat.";
			return;
		}
		else {
			$subjects = substr( $subjects, 0, -4 );
		}
	}


	$exams = $sql->query( "SELECT e.id, halftime, max_points, min_points, questions, subject, COUNT(t.exam) FROM IIS_exams AS e LEFT JOIN IIS_terms AS t ON e.id = t.exam $subjects GROUP BY e.id" );	

	if ( $exams->num_rows > 0 ) { ?>
		
		<table>
			<tr>
				<?php if ( ! isset( $_GET[ 'id' ] ) )  echo "\t\t\t<th>Předmět</th>\n"; ?>
				<th>Typ</th>
				<th>Min. bodů</th>
				<th>Max. bodů</th>
				<th>Počet otázek</th>
				<th>Počet termínů</th>
				<th></th>
			</tr>


		<?php while ( $exam = $exams->fetch_assoc() ) {
			echo "\t<tr>\n";
			if ( ! isset( $_GET[ 'id' ] ) )  echo "\t\t<td><a href=\"./subjects&id=" . $exam[ 'subject' ] . "\">" . $exam[ 'subject' ] . "</a></td>\n";

			
			$halftime = 'Semestrální';
			if ( $exam[ 'halftime' ] === '1' ) { $halftime = 'Půlsemestrální'; }
			
			$red = "";
			if ( $exam[ 'COUNT(t.exam)' ] === '0' ) { $red = 'class="red"'; }
			
			echo "
				\t\t<td>$halftime</td>\n
				\t\t<td>" . $exam[ 'max_points'  ] . "</td>\n
				\t\t<td>" . $exam[ 'min_points'  ] . "</td>\n
				\t\t<td>" . $exam[ 'questions'   ] . "</td>\n
				\t\t<td $red>" . $exam[ 'COUNT(t.exam)' ] . "</td>\n
				\t\t<td class=\"edit\"><a href=\"./edit-exam&id=" . $exam[ 'id' ] . "\" title=\"Upravit termín\" class=\"fa fa-pencil\" style=\"padding-right: 1em;\"></a><a title=\"Smazat termín\" class=\"fa fa-times\" data-id=\"" . $exam[ 'id' ] . "\"></a></td>\n
				\t</tr>\n";
		} ?>
	</table>

<?php }
	else {
		echo "Žádné zkoušky.";
	}?>

<div id="delete-result" class="warning" ></div>