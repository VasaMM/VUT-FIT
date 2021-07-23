<?php
	include "./sql/sql.php";

	$tables = $sql->query( 'SHOW TABLES' );

	while ( $table = $tables->fetch_assoc() ) {
		foreach ( $table as $table_name ) {
			echo '<br><h3>' . $table_name . '</h3>';

			$rows = $sql->query( 'SELECT * FROM ' . $table_name );
			
			echo '<table id="all-tables">';
			
			$first = true;
			while ( $row = $rows->fetch_assoc() ) {
				if ( $first ) {
					$first = false;
					echo '<tr>';
					foreach ( $row as $key => $value ) {
						echo '<th>';
						echo $key;
						echo '</th>';
					}
					echo '</tr>';
				}

				echo '<tr>';
				foreach ( $row as $key => $value ) {
					echo '<td>';
					echo $value;
					echo '</td>';
				}
				echo '</tr>';
			}
			echo '</table>';
		}
	}


?>