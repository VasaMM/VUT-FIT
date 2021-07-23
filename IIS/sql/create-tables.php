<?php
	$DO_CONTROL = true;


	if ( $DO_CONTROL ) {
		if ( session_id() == '' ) {
			session_start();
		}

		// Otestuji, zda nekdo nechce nabourat stranku
		if ( ! isset( $_SESSION[ 'login' ] ) || $_SESSION[ 'login' ] == '' || $_SESSION[ 'role' ] != 'admin' ) {
			header( 'Location: http://www.stud.fit.vutbr.cz/~xmarti76/IIS/' );
			return;
		}
	}


	include "./sql.php";

	$pass = hash( 'sha512' , 'admin' );

	// Definuji sql prikazi k provedeni
	$sql_queries = array(
		"Drop all tables" => "
			DROP TABLE IF EXISTS
				IIS_evaluations,
				IIS_user_term,
				IIS_terms,
				IIS_exams,
				IIS_user_subject,
				IIS_subjects,
				IIS_users,
				IIS_persons,
				IIS_addresses;
			",

		"Create 'IIS_addresses' table" => "
			CREATE TABLE IIS_addresses (
				id           INT UNSIGNED AUTO_INCREMENT PRIMARY KEY, 
				city         VARCHAR( 255 ),
				street       VARCHAR( 255 ),
				house_number INT,
				psc          INT
				)
			",

		"Create 'IIS_persons' table" => "
			CREATE TABLE IIS_persons (
				id       INT UNSIGNED AUTO_INCREMENT PRIMARY KEY, 
				name     VARCHAR( 255 ),
				surname  VARCHAR( 255 ),
				birthday DATE          ,
				gender   BOOLEAN       ,  -- muz = true; zena = false
				email    VARCHAR( 255 ) CHECK ( email LIKE '%_@__%.__%' ),
				address  INT UNSIGNED UNIQUE REFERENCES IIS_addresses( id ),
				phone    VARCHAR( 255 )
				)
			",

		"Create 'IIS_users' table" => "
			CREATE TABLE IIS_users (
				login    VARCHAR(8) PRIMARY KEY,	-- FIXME generovat automaticky
				password VARCHAR(128),
				role     INT UNSIGNED CHECK ( role <= 2 ),   -- 0 = student, 1 = teacher, 2 = admin
				person   INT UNSIGNED UNIQUE REFERENCES IIS_persons( id )
				)
			",

		"Create 'IIS_subjects' table" => "
			CREATE TABLE IIS_subjects (
				id    VARCHAR(3) PRIMARY KEY, 
				name  VARCHAR(255),
				about VARCHAR(1023)
				)
			",

		"Create 'IIS_user_subject' table" => "
			CREATE TABLE IIS_user_subject (
				id_user    VARCHAR(8),
				id_subject VARCHAR(3),
				PRIMARY KEY ( id_user, id_subject )
				)
			",
		
		"Create 'IIS_exams' table" => "
			CREATE TABLE IIS_exams (
				id         INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
				subject    VARCHAR(3) REFERENCES IIS_subjects( id ), 
				halftime   BOOLEAN,
				max_points INT,
				min_points INT,
				questions  INT
				)
			",
		
		"Create 'IIS_terms' table" => "
			CREATE TABLE IIS_terms (
				id           INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
				exam         INT UNSIGNED REFERENCES IIS_exam( id ),
				exam_start   DATETIME,
				exam_end     DATETIME,
				login_start  DATETIME,
				login_end    DATETIME,
				place        VARCHAR(127),
				max_students INT
				)
			",

		"Create 'IIS_user_term' table" => "
			CREATE TABLE IIS_user_term (
				id_user VARCHAR(8),
				id_term INT UNSIGNED,
				PRIMARY KEY ( id_user, id_term )
				)
			",

		"Create 'IIS_evaluations' table" => "
			CREATE TABLE IIS_evaluations (
				login VARCHAR(8),
				term_id INT UNSIGNED,
				comment VARCHAR(1023),
				points VARCHAR(1023),
				total INT,
				evaluator VARCHAR(8),
				evaluated DATETIME,
				PRIMARY KEY ( login, term_id )
				)
			",
		



		// "Create adresa 1" => "
		// 	INSERT INTO IIS_addresses ( city, street, house_number, psc )
		// 		VALUES ( 'Brno', 'Novákova', 3, 12345 )
		// ",
		
		// "Create adresa 2" => "
		// 	INSERT INTO IIS_addresses ( city, street, house_number, psc )
		// 		VALUES ( 'Olomouc', 'Janů', 4, 45678 )
		// ",

		// "Create adresa 3" => "
		// 	INSERT INTO IIS_addresses ( city, street, house_number, psc )
		// 		VALUES ( 'Bratislava', 'První', 2, 98547 )
		// ",

		// "Create person Jan Novák" => "
		// 	INSERT INTO IIS_persons ( name, surname, email, birthday, gender, address, phone )
		// 		VALUES ( 'Jan', 'Novák', 'jan.novak@mail.cz', '1989-12-31', true, 1, '+420 605 605 605' )
		// 	",

		// "Create person Adam Milotný" => "
		// 	INSERT INTO IIS_persons ( name, surname, email, birthday, gender, address, phone )
		// 		VALUES ( 'Adam', 'Milotný', 'adamm@mail.cz', '1979-05-04', true, 2, '+420 458 458 458' )
		// 	",

		// "Create person Ivana Novotná" => "
		// 	INSERT INTO IIS_persons ( name, surname, email, birthday, gender, address, phone )
		// 		VALUES ( 'Ivana', 'Novotná', 'nova.ivana@mail.cz', '1992-06-15', false, 3, '+421 789 789 789' )
		// 	",

		"Create admin account" => "
				INSERT INTO IIS_users ( login, password, role )
					VALUES ( 'admin', '$pass', 2 );
			",

		// "Create teacher account" => "
		// 		INSERT INTO IIS_users ( login, password, role, person )
		// 			VALUES ( 'teacher', 'teacher', 1, 2 );
		// 	",

		// "Create student account" => "
		// 		INSERT INTO IIS_users ( login, password, role, person )
		// 			VALUES ( 'student', 'student', 0, 3 );
		// 	",

		// "Create IIS subject" => "
		// 		INSERT INTO IIS_subjects ( id, name, about )
		// 			VALUES ( 'IIS', 'Informační systémy', 'Nějaké ty kecy' );
		// 	",

		// "Create IDS subject" => "
		// 		INSERT INTO IIS_subjects ( id, name, about )
		// 			VALUES ( 'IDS', 'Databázové systémy', 'Nějaké ty kecy' );
		// 	",

		// "Create IMS subject" => "
		// 		INSERT INTO IIS_subjects ( id, name, about )
		// 			VALUES ( 'IMS', 'Matematika', 'Nějaké ty kecy' );
		// 	",

		// "Add IMS to teacher" => "
		// 		INSERT INTO IIS_user_subject ( id_user, id_subject )
		// 			VALUES ( 'teacher', 'IMS' );
		// 	",

		// "Add IDS to teacher" => "
		// 		INSERT INTO IIS_user_subject ( id_user, id_subject )
		// 			VALUES ( 'teacher', 'IDS' );
		// 	",

		// "Add IDS to student" => "
		// 		INSERT INTO IIS_user_subject ( id_user, id_subject )
		// 			VALUES ( 'student', 'IDS' );
		// 	",

		// "Add IIS to student" => "
		// 		INSERT INTO IIS_user_subject ( id_user, id_subject )
		// 			VALUES ( 'student', 'IIS' );
		// 	",

	);


	$output = $sql->queries( $sql_queries );

	// Provedu vsechny prikazy
	foreach( $output as $name => $result ) {
		if ( $result === TRUE ) {
			echo $name . " was <strong>successfully</strong><br>"; 
		}
		else {
			echo "<span style=\"color: red; font-weight: bold;\">Error in " . $name . ": " . $sql->error[ $name ] . "</span><br>";
		}
	}

	$sql->close();
?>