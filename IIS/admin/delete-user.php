<span id="show">

<?php
	if ( ! isset( $_POST[ 'login' ] ) ) {
		echo "Nelze smazat, nebyl zadan login.";
		return;
	}
	$login = $_POST[ 'login' ];

	include( './sql/sql.php' );

	$delete_address      = true;
	$delete_person       = true;
	$delete_user_subject = true;
	$delete_user         = true;

	$result = $sql->query( "SELECT person FROM IIS_users WHERE login = '$login'" );
	if ( $result->num_rows != 1 ) {
		echo "Nejednoznačné zadání.";
		return;
	}
	$result = $result->fetch_assoc();
	$person = $result[ 'person' ];

	// Nema nastavenou zadnou person -> nemam co mazat
	if ( empty( $person ) ) {
		$delete_address = false;
		$delete_person  = false;
	}

	$result = $sql->query( "SELECT address FROM IIS_persons WHERE id = '$person'" );
	if ( $result->num_rows > 1 ) {
		echo "Uživatel má více adres.";
		return;
	}
	// Nema nastavenou zadnou adresu -> nemam co mazat
	else if ( $result->num_rows == 0 ) {
		$delete_address = false;
	}
	else {
		$result  = $result->fetch_assoc();
		$address = $result[ 'address' ];
	}


	if ( $delete_address && ! $sql->query( "DELETE FROM IIS_addresses WHERE id = '$address'" ) ) {
		echo "Nepodařilo se smazat adresu.";
	}
	if ( $delete_person && ! $sql->query( "DELETE FROM IIS_persons      WHERE id      = '$person'"  ) ) {
		echo "Nepodařilo se smazat osobní údaje.";
	}
	if ( ! $sql->query( "DELETE FROM IIS_user_subject WHERE id_user = '$login'"   ) ) {
		echo "Nepodařilo se smazat seznam předmětů.";
	}
	if ( ! $sql->query( "DELETE FROM IIS_users        WHERE login   = '$login'"   ) ) {
		echo "Nepodařil se smazat profil.";
	}
	else {
		echo "Uživatel smazán.";
	}
	
?>

</span>