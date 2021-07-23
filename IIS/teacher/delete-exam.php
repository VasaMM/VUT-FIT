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
		FROM IIS_exams AS e
		JOIN IIS_user_subject AS us
			ON e.subject = us.id_subject
		WHERE id_user='" . $_SESSION[ 'login' ] . "'
		AND e.id = '$id'
	" );
	if ( $valid->num_rows == 0 ) {
		echo "Nemáte právo upravovat tento předmět!";
		return;
	}

	if ( $sql->query( "DELETE FROM IIS_terms WHERE exam = '$id'" ) ) {
		if ( $sql->query( "DELETE FROM IIS_exams WHERE id = '$id'"  ) ) {
			return;
		}
		else {
			echo "Nepodařilo se smazat zkoušku.";		
		}
	}
	else {
		echo "Nepodařili se smazat všechny termíny zkoušky.";		
	}

	echo $sql->error;
?>

</span>