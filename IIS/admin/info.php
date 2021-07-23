<?php
	include "./sql/sql.php";

	$tables = $sql->query( 'SHOW TABLES' );

	while ( $table = $tables->fetch_assoc() ) {
		foreach ( $table as $table_name ) {
			echo '<br><h3>' . $table_name . '</h3>';

			$count = $sql->query( "SELECT COUNT(*) FROM $table_name" );
			
			$count = $count->fetch_assoc();

			echo '<div style="margin-left: 1em; margin-bottom: 1em;">Počet záznamů: ' . $count[ 'COUNT(*)' ] . '</div>';			
		}
	}

?>