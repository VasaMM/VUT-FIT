<?php
	include "./sql/sql.php";

	$subjects = $sql->query( "
		SELECT id, name
		FROM IIS_subjects
		JOIN IIS_user_subject ON id_subject = id
		WHERE id_user = '" . $_SESSION[ 'login' ] . "'
	" );
	

	if ( $subjects->num_rows > 0 ) {
		if ( ! isset( $_GET[ 'id' ] ) ) {
			echo "<h2>Předměty</h2>";
			while ( $subject = $subjects->fetch_assoc() ) {

				echo "\t<a href=\"./subjects&id=" . $subject[ 'id' ] . "\" title=\"Přejít na " . $subject[ 'name' ] . "\" class=\"subject-container\">\n";
				foreach ( $subject as $key => $value ) {
					echo "\t\t<span class=\"$key\">$value</span>\n";
				}
				echo "</a>\n";
			}
			
		}
		else {
			$subject = $sql->query( "
				SELECT id, name, about
				FROM IIS_subjects
				JOIN IIS_user_subject ON id_subject = id
				WHERE id_user = '" . $_SESSION[ 'login' ] . "'
				AND id = '" . $_GET[ 'id' ] . "'
			" );

			if ( $subject->num_rows === 1 ) {
				$subject = $subject->fetch_assoc();
				$about = str_replace( "\n", "<br>", $subject[ 'about' ] );
				
				echo "<h2>" . $_GET[ 'id' ] . "</h2>";
				echo "<div class=\"info\">$about</div>";

				include( 'exams.php' ); ?>

			<?php }
			else {
				echo "K danému předmětu nemáte právo přistupovat.";
			}
			
		}
	}
	else {
		echo "Žádné předměty!";
	}



?>
