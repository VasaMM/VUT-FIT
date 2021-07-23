<?php
	if ( ! isset( $_GET[ 'id' ] ) ) {
		echo "Nebylo zadáno id zkoušky, musíte ji nejdřív <a href=\"./exams\" title=\"Vybrat zkoušku\">vybrat</a>.";
		return;
	}
	$id = $_GET[ 'id' ];

	include "./sql/sql.php";

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

	$addTerm = array(
		'action' => '',
		'method' => 'post',
		'submit' => 'Přidat termín',
		'fields' => array (
			'IIS_terms' => array (
				'name'   => 'Termín',
				'inputs' => array (
					'exam_start' => array (
						'name'     => 'Začátek zkoušky',
						'type'     => 'datetime-local',
						'required' => true,
					),
					'exam_end' => array (
						'name'     => 'Konec zkoušky',
						'type'     => 'datetime-local',
						'required' => true,
					),
					'login_start' => array (
						'name'     => 'Začátek přihlašování',
						'type'     => 'datetime-local',
						'required' => true,
					),
					'login_end' => array (
						'name'     => 'Konec přihlašování',
						'type'     => 'datetime-local',
						'required' => true,
					),
					'place' => array (
						'name'     => 'Místo konání',
						'type'     => 'text',
						'required' => true,
					),
					'max_students' => array (
						'name'     => 'Maximální počet studentů',
						'type'     => 'number',
						'required' => true,
					),
				),
			),
		),
	);

	include "./form.php";
	$addTerm = new form( $addTerm );
	$addTerm->make();

	$output = $addTerm->output( 'IIS_terms' );
	$output[ 'exam' ] = $id;

	if ( $output[ 'exam_start' ] > $output[ 'exam_end' ] ) {
		$addTerm->setValid( false );
		echo "<div class=\"warning\">Zkouška musí končit až poté, co začne!</div>";
	}

	if ( $output[ 'login_start' ] > $output[ 'login_end' ] ) {
		$addTerm->setValid( false );
		echo "<div class=\"warning\">Přihlašování musí končit až poté, co začne!</div>";
	}

	if ( $addTerm->isOK() ) {

		if ( $sql->add_row( 'IIS_terms', $output ) ) {
			echo "Termín byl úspěšně přidán<br><br>Přidat další termín:";
			$addTerm->printForm( true );
		}
		else {
			echo $sql->error;
		}

	}
	else {
		$addTerm->printForm();
	}
?>

