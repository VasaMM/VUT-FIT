<span id="show">

<?php
	header( 'Content-Type: text/html; charset=utf-8' );
	if ( ! isset( $_POST[ 'id' ] ) ) {
		echo "Nebylo zadáno id předmětu, musíte ho nejdřív <a href=\"./subjects\" title=\"Vybrat předmět\">vybrat</a>.";
		return;
	}
	$id = $_POST[ 'id' ];

	if ( ! isset( $_POST[ 'about' ] ) ) {
		echo "Nebyl zadán popis předmětu, musíte ho nejdřív <a href=\"./subjects\" title=\"Napsat popis\">napsat</a>.";
		return;
	}
	$about = $_POST[ 'about' ];

	include ( "./sql/sql.php" );
	
	// Zkontroluji, zda ma uzivatel prava
	$valid = $sql->query( "SELECT id_user FROM IIS_user_subject WHERE id_user='" . $_SESSION[ 'login' ] . "' AND id_subject = '$id'" );
	if ( $valid->num_rows == 0 ) {
		echo "Nemáte právo upravovat tento předmět!";
		return;
	}

	if ( $sql->query( "UPDATE IIS_subjects SET about = '$about' WHERE id = '$id'" ) ) {
		echo "Uloženo";
	}
	else {
		echo $sql->error;
	}

?>

</span>