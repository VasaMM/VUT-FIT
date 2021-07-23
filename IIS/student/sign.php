<span id="show">

<?php
	if ( ! isset( $_POST[ 'id' ] ) ) {
		echo "Nebylo zadáno id termínu, musíte ho nejdřív <a href=\"./terms\" title=\"Vybrat termín\">vybrat</a>.";
		return;
	}
	$id = $_POST[ 'id' ];

	include "./sql/sql.php";

	// Zkontroluji, zda se v tomto 
	$check = $sql->query( "
		SELECT t.exam
		FROM IIS_exams AS e
		LEFT JOIN IIS_terms AS t
			ON t.exam = e.id
		LEFT JOIN IIS_user_subject AS us
			ON us.id_subject = e.subject
		WHERE t.id = '$id'
		AND us.id_user = '" . $_SESSION[ 'login' ] . "'
	" );
	
	if ( $check->num_rows == 0 ) {
		echo "Nemáte právo se přihlásit na tento termín!";
		return;
	}
	$exam = $check->fetch_assoc();
	$exam = $exam[ 'exam' ];


	$signed = $sql->query( "SELECT id_term FROM IIS_user_term WHERE id_user = '" . $_SESSION[ 'login' ] . "' AND id_term = '$id'");

	if ( $signed->num_rows == 0 ) {
		// Prihlasuji se 

		// Musim zkontrolovat, zda neni user prihlaseny na jiny termin
		$terms = $sql->query( "
			SELECT id_user
			FROM IIS_user_term
			JOIN IIS_terms ON id_term = id
			WHERE id_user = '" . $_SESSION[ 'login' ] . "'
			AND exam = '$exam'
			AND exam_start > '" . date( 'Y-m-d H:i:s' ) . "'
		" );

		if ( $terms->num_rows != 0 ) {
			echo "Nelze se přihlásit na 2 termíny zároveň.";		
			return;
		}

		// U semestralni zkousky max. tri terminy
		$terms = $sql->query( "
			SELECT halftime
			FROM IIS_user_term
			JOIN IIS_terms AS t ON id_term = t.id
			JOIN IIS_exams AS e ON exam = e.id
			WHERE id_user = '" . $_SESSION[ 'login' ] . "'
			AND exam = '$exam'
		" );

		if ( $terms->num_rows > 3 ) {
			echo "Nelze se přihlásit na více jak 3 termíny.";		
			return;
		}


		$sign = array(
			'id_user' => $_SESSION[ 'login' ],
			'id_term' => $_POST[ 'id' ],
		);

		if ( $sql->add_row( 'IIS_user_term', $sign ) ) {
			echo "Přihlášen";
		}
		else {
			echo $sql->error;
		}
	}
	else {
		// Odhlasuji se
		if ( $sql->query( "DELETE FROM IIS_user_term WHERE id_term = '$id'" ) ) {
			echo "Odhlášen";
		}
		else {
			echo $sql->error;		
		}
	}


?>

</span>