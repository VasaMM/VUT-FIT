<?php

	$ask = array(
		'action' => '',
		'method' => 'post',
		'id'     => 'add-subject',
		'submit' => 'Vytvořit',
		'fields' => array (
			'IIS_subjects' => array (
				'name'   => 'Předmět',
				'inputs' => array (
					'id' => array (
						'name'     => 'ID',
						'type'     => 'text',
						'required' => true,
					),
					'name' => array (
						'name'     => 'Jméno',
						'type'     => 'text',
						'required' => true,
					),
				),
			),
		),
	);

	include "./form.php";

	$addSubject = new form( $ask );
	$addSubject->make();
	
	$output = $addSubject->output( 'IIS_subjects' );
	if ( isset( $output[ 'id' ] ) && strlen( $output[ 'id' ] ) != 3 ) {
		echo "<span style=\"color: red; font-weight: bold;\">ID předmětu musí mít délu 3 znaky!</span><br>";
		$addSubject->isOK();
		$addSubject->printForm( false );
		return;
	}

	$addSubject->printForm();




	if ( $addSubject->isOK() === true ) {
		include "./sql/sql.php";

		// Postupne naplnim tabulky daty
		if ( $sql->add_row( 'IIS_subjects', $output ) ) {
			echo "Předmět byl vytvořen.<br>"; 			
		}
		else {
			echo "<span style=\"color: red; font-weight: bold;\">Error in IIS_subjects: " . $sql->error . "</span><br>";
		}
		
		$sql->close();
	}

?>
