<?php

	if ( ! class_exists( 'SQL' ) ) {
		class SQL {
			public $error;
			public $last_id;
			private $sql;

			function __construct ( $server, $user, $password, $database ) {
				$this->sql = new mysqli( $server, $user, $password, $database );
				
				if ( $this->sql->connect_error ) {
					die( 'Nelze se připojit k databázi ' . mysql_error() );
				}

				if ( $this->sql->query( 'SET CHARACTER SET utf8' ) != TRUE ) {
					die( 'Nelze se přepnout na UTF-8 ' . mysql_error() );
				}
				// Connected successfully
			}

			public function query ( $query ) {
				// $query = mysqli_real_escape_string( $this->sql, $query );
				$output = $this->sql->query( $query );
				
				$this->error   = $this->sql->error;
				$this->last_id = $this->sql->insert_id;
				
				return $output;
			}

			public function queries ( $queries ) {
				$output = array();
				$error  = array();
				
				// Provedu vsechny prikazy
				foreach( $queries as $name => $query ) {
					$output[ $name ] = $this->sql->query( $query );
					$error [ $name ] = $this->sql->error;
				}

				$this->error = $error;
				return $output;
			}

			public function add_row ( $table, array $row ) {
				$keys   = '';
				$values = '';
				
				foreach ( $row as $key => $value ) {
					$keys   = $keys   . ',' . $key;
					$values = $values . ',\'' . $value . '\'';
				}

				$keys   = substr( $keys,   1 );
				$values = substr( $values, 1 );

				return $this->query( "INSERT INTO $table ( $keys ) VALUES ( $values )" );
			}

			public function add_rows ( array $rows ) {
				$output = array();
				$error  = array();
				
				// Provedu vsechny prikazy
				foreach( $rows as $name => $row ) {
					$output[ $name ] = $this->add_row( $name, $row );
					$error [ $name ] = $this->sql->error;
				}

				$this->error = $error;
				return $output;
			}

			public function close () {
				$this->sql->close();
			}
		}
	}

	$sql = new SQL( 'localhost', 'xmarti76', 'hokuj9ga', 'xmarti76' );


	// Run SQL
	if ( isset( $_POST[ 'SQL' ] ) ) {
		if ( session_id() == '' ) {
			session_start();
		}

		// Otestuji, zda nekdo nechce nabourat stranku
		if ( ! isset( $_SESSION[ 'login' ] ) || $_SESSION[ 'login' ] == '' || $_SESSION[ 'role' ] != 'admin' ) {
			header( 'Location: http://www.stud.fit.vutbr.cz/~xmarti76/IIS/' );
			return;
		}


		echo "<pre id='result'>\n";

		$query = str_replace( "\'","'", $_POST[ 'SQL' ] );
		$results = $sql->query( $query );

		if ( gettype( $results ) == 'object' ) {
			while ( $result = $results->fetch_assoc() ) {
				print_r($result);
				echo "\n";
			}
		}
		else {
			echo $results;
		}

		echo "</pre>\n";
		echo "Results: " . $results->num_rows . "<br>";
		echo "<br><br>Error:<br><div id='error'>\n$sql->error</div>\n";
		echo "<br><br>LAST ID:<br><div id='lastid'>\n$sql->last_id</div>\n";
	}
?>