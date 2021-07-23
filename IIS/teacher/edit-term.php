<?php
	header( 'Content-Type: text/html; charset=utf-8' );

	if ( ! isset( $_GET[ 'id' ] ) ) {
		echo "Nebylo zadáno id termínu, musíte ho nejdřív <a href=\"./terms\" title=\"Vybrat termín\">vybrat</a>.";
		return;
	}
	$id = $_GET[ 'id' ];

	include( './sql/sql.php' );

	$term =  $sql->query( "SELECT * FROM IIS_terms WHERE id = '$id'" );

	if ( $term->num_rows != 1 ) {
		echo "Bylo zadáno nejednoznačné ID termínu, musíte ho nejdřív <a href=\"./terms\" title=\"Vybrat termín\">vybrat</a>.";
		return;
	}

	// Zkontroluji, zda ma uzivatel prava
	$valid = $sql->query( "
		SELECT us.id_user
		FROM IIS_terms AS t
		JOIN IIS_exams AS e
			ON t.exam = e.id
		JOIN IIS_user_subject AS us
			ON e.subject = us.id_subject
		WHERE us.id_user='" . $_SESSION[ 'login' ] . "'
		AND t.id = '$id'
	" );
	if ( $valid->num_rows == 0 ) {
		echo "Nemáte právo upravovat tento předmět!";
		return;
	}
	
	function SQL2inputDateTime ( $input ) { return str_replace( " ", "T", $input ); }

	$term = $term->fetch_assoc();

	$editTerm = array(
		'action' => '',
		'method' => 'post',
		'submit' => 'Uložit',
		'fields' => array (
			'IIS_terms' => array (
				'name'   => 'Termín',
				'inputs' => array (
					'exam_start' => array (
						'name'     => 'Začátek zkoušky',
						'type'     => 'datetime-local',
						'default'  => SQL2inputDateTime( $term[ 'exam_start' ] ),
						'required' => true,
					),
					'exam_end' => array (
						'name'     => 'Konec zkoušky',
						'type'     => 'datetime-local',
						'default'  => SQL2inputDateTime( $term[ 'exam_end' ] ),
						'required' => true,
					),
					'login_start' => array (
						'name'     => 'Začátek přihlašování',
						'type'     => 'datetime-local',
						'default'  => SQL2inputDateTime( $term[ 'login_start' ] ),
						'required' => true,
					),
					'login_end' => array (
						'name'     => 'Konec přihlašování',
						'type'     => 'datetime-local',
						'default'  => SQL2inputDateTime( $term[ 'login_end' ] ),
						'required' => true,
					),
					'place' => array (
						'name'     => 'Místo konání',
						'type'     => 'text',
						'default'  => $term[ 'place' ],
						'required' => true,
					),
					'max_students' => array (
						'name'     => 'Maximální počet studentů',
						'type'     => 'number',
						'default'  => $term[ 'max_students' ],
						'required' => true,
					),
				),
			),
		),
	);

	include "./form.php";
	$editTerm = new form( $editTerm );
	$editTerm->make();
	$editTerm->printForm( false );

	$output = $editTerm->output( 'IIS_terms' );

	if ( ! empty( $output ) ) {
		if ( $editTerm->isOK() ) {		
			include ( './sql/sql.php' );

			$query = 'UPDATE IIS_terms SET ';
			foreach ( $output as $key => $value) {
				$query .= "$key = '$value', ";
			}
			$query = substr( $query, 0, -2 );
			$query .= " WHERE id = '$id'";
			

			if ( $sql->query( $query ) ) {
				echo "Uloženo<br>\n";

			}
			else {
				echo $sql->error;
			}
		}
	}
?>
