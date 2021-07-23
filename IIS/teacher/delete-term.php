<span id="show">
	
<?php
	if ( ! isset( $_POST[ 'id' ] ) ) {
		echo "Nelze smazat, nebylo zadano id.";
		return;
	}
	$id = $_POST[ 'id' ];

	include( './sql/sql.php' );

	// Zkontroluji, zda ma uzivatel prava
	$valid = $sql->query( "
		SELECT us.id_user
		FROM IIS_terms AS t
		JOIN IIS_exams AS e
			ON t.exam = e.id
		JOIN IIS_user_subject AS us
			ON e.subject = us.id_subject
		WHERE us.id_user='" . $_SESSION[ 'login' ] . "'
		AND t.id = '$id'
	" );
	if ( $valid->num_rows == 0 ) {
		echo "Nemáte právo upravovat tento předmět!";
		return;
	}

	if ( ! $sql->query( "DELETE FROM IIS_terms WHERE id = '$id'" ) ) {
		echo "Nepodařilo se smazat termín zkoušky.";		
	}

	echo $sql->error;
?>

</span>