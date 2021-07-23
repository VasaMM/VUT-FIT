<?php
	if ( ! isset( $_GET[ 'id' ] ) ) {
		echo "Nebylo zadáno id zkoušky, musíte ji nejdřív <a href=\"./exams\" title=\"Vybrat zkoušku\">vybrat</a>.";
		return;
	}
	$id = $_GET[ 'id' ];

	include( './sql/sql.php' );
	$exam =  $sql->query( "SELECT * FROM IIS_exams WHERE id = '$id'" );


	if ( $exam->num_rows != 1 ) {
		echo "Bylo zadáno nejednoznačné ID zkoušky, musíte ji nejdřív <a href=\"./exams\" title=\"Vybrat zkoušku\">vybrat</a>.";
		return;
	}

	
	$exam = $exam->fetch_assoc();
	$subject = $exam[ 'subject' ];
	
	// Zkontroluji, zda ma uzivatel prava
	$valid = $sql->query( "SELECT id_user FROM IIS_user_subject WHERE id_user='" . $_SESSION[ 'login' ] . "' AND id_subject = '$subject'" );
	if ( $valid->num_rows == 0 ) {
		echo "Nemáte právo upravovat tento předmět!";
		return;
	}



	echo "<h1>Upravit zkoušku pro předmět <a href=\"./subjects&id=$subject\">$subject</a></h1><br>";

	$halftime = 'fulltime';
	if ( $exam[ 'halftime' ] ) { $halftime = 'halftime'; }

	$editExam = array(
		'action' => '',
		'method' => 'post',
		'submit' => 'Uložit',
		'fields' => array (
			'IIS_exams' => array (
				'name'   => 'Zkouška',
				'inputs' => array (
					'type' => array (
						'name'     => 'Typ',
						'type'     => 'radio',
						'default'  => $halftime,
						'required' => true,
						'values'   => array (
							'halftime' => 'Půlsemestrální',
							'fulltime' => 'Semestrální',
						),
					),
					'max_points' => array (
						'name'     => 'Maximální počet bodů',
						'type'     => 'number',
						'default'  => $exam[ 'max_points' ],
						'required' => true,
					),
					'min_points' => array (
						'name'     => 'Minimální počet bodů',
						'type'     => 'number',
						'default'  => $exam[ 'min_points' ],
						'required' => true,
					),
					'questions' => array (
						'name'     => 'Počet otázek',
						'type'     => 'number',
						'default'  => $exam[ 'questions' ],
						'required' => true,
					),
				),
			),
		),
	);


	include "./form.php";
	$editExam = new form( $editExam );
	$editExam->make();
	$editExam->printForm( false );
	$output = $editExam->output( 'IIS_exams' );


	if ( ! empty( $output ) ) {
		switch ( $output[ 'type' ] ) {
			case 'halftime':  $output[ 'halftime' ] = '1';  break;
			case 'fulltime':  $output[ 'halftime' ] = '0';  break;
			default: $editExam->setValid( false );
		}
		unset( $output[ 'type' ] );

		if ( $output[ 'min_points' ] > $output[ 'max_points' ] ) {
			echo "<div class=\"warning\">Minimální počet bodů je větší než maximální!</div>";
			$editExam->setValid( false );
		}

		if (
			$output[ 'min_points' ] < 0 &&
			$output[ 'max_points' ] < 1 &&
			$output[ 'questions'  ] < 1
		) {
			echo "<div class=\"warning\">Zadali jste moc malé číslo!</div>";
			$editExam->setValid( false );
		}

		
		if ( $editExam->isOK() ) {		
			include ( './sql/sql.php' );

			$query = 'UPDATE IIS_exams SET ';
			foreach ( $output as $key => $value) {
				$query .= "$key = '$value', ";
			}
			$query = substr( $query, 0, -2 );
			$query .= " WHERE id = '" . $exam[ 'id' ] . "'";
			

			if ( $sql->query( $query ) ) {

				echo "Uloženo<br>\n";

			}
			else {
				echo $sql->error;
			}
		}
	}

	$_GET[ 'exam' ] = $exam[ 'id' ];

	echo "<h3>Termíny:</h3>";
	include ( "./teacher/terms.php" );

	echo "<div id=\"delete-result\" class=\"warning\"></div>";
	echo "<button id=\"add-term-btn\" data-id=\"" . $_GET[ 'id' ] . "\">Přidat termín</button>"
	
	?>
	
	<iframe class="exam" id="add-term" src width="100%" height="300"></iframe>

