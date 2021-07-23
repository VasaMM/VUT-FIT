<?php
	if ( ! isset( $_GET[ 'id' ] ) ) {
		echo "Nebylo zadáno id předmětu, musíte ho nejdřív <a href=\"./subjects\" title=\"Vybrat předmět\">vybrat</a>.";
		return;
	}
	$id = $_GET[ 'id' ];

	include( './sql/sql.php' );

	// Zkontroluji, zda ma uzivatel prava
	$valid = $sql->query( "
		SELECT id_user
		FROM IIS_user_subject
		WHERE id_user='" . $_SESSION[ 'login' ] . "'
		AND id_subject = '$id'
	" );
	if ( $valid->num_rows == 0 ) {
		echo "Nemáte právo upravovat tento předmět!";
		return;
	}

	$addExam = array(
		'action' => 'add-exam.php&id=' . $_GET[ 'id' ],
		'method' => 'post',
		'submit' => 'Vytvořit',
		'fields' => array (
			'IIS_exams' => array (
				'name'   => 'Zkouška',
				'inputs' => array (
					'type' => array (
						'name'     => 'Typ',
						'type'     => 'radio',
						'default'  => 'halftime',
						'required' => true,
						'values'   => array (
							'halftime' => 'Půlsemestrální',
							'fulltime' => 'Semestrální',
						),
					),
					'max_points' => array (
						'name'     => 'Maximální počet bodů',
						'type'     => 'number',
						'required' => true,
					),
					'min_points' => array (
						'name'     => 'Minimální počet bodů',
						'type'     => 'number',
						'required' => true,
					),
					'questions' => array (
						'name'     => 'Počet otázek',
						'type'     => 'number',
						'required' => true,
					),
				),
			),
		),
	);


	include "./form.php";
	$addExam = new form( $addExam );
	$addExam->make();

	if ( $addExam->isOK() ) {
		$output = $addExam->output( 'IIS_exams' );
		switch ( $output[ 'type' ] ) {
			case 'halftime':  $output[ 'halftime' ] = '1';  break;
			case 'fulltime':  $output[ 'halftime' ] = '0';  break;
			default: $addExam->setValid( false );
		}

		if ( $output[ 'min_points' ] > $output[ 'max_points' ] ) {
			echo "<div class=\"warning\">Minimální počet bodů je větší než maximální!</div>";
			$addExam->setValid( false );
		}

		if (
			$output[ 'min_points' ] < 0 &&
			$output[ 'max_points' ] < 1 &&
			$output[ 'questions'  ] < 1
		) {
			echo "<div class=\"warning\">Zadali jste moc malé číslo!</div>";
			$addExam->setValid( false );
		}



		if ( $addExam->isOK() ) { 
			unset( $output[ 'type' ] );
			$output[ 'subject' ] = $id;
			
			if ( $sql->add_row( 'IIS_exams', $output ) ) {

				echo "<br>Zkouška do předmětu <a href=\"./subjects&id=$id\">$id</a> byla vytvořena, přidejte termíny:<br><br>\n
				      <iframe class=\"exam\" src=\"http://www.stud.fit.vutbr.cz/~xmarti76/IIS/teacher/add-term.php&id=" . $sql->last_id . "\" width=\"100%\" height=\"500\" )></iframe>";
			}
			else {
				echo $sql->error;
			}
		}
	}
	else {
		$addExam->printForm();
	}
?>

