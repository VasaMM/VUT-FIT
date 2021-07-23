<?php
	function search_user( $user ) {
		if ( isset( $user[ 'role' ] ) ) {
			switch ( $user[ 'role' ] ) {
				case 'student':  $role = 'AND role = \'0\'';  break;
				case 'teacher':  $role = 'AND role = \'1\'';  break;
				case 'admin':    $role = 'AND role = \'2\'';  break;
				default:		 $role = '';
			}
		}

		$login   = '';
		$name    = '';
		$surname = '';
		if ( isset( $user[ 'login'   ] ) ) $login   = &$user[ 'login'   ];
		if ( isset( $user[ 'name'    ] ) ) $name    = &$user[ 'name'    ];
		if ( isset( $user[ 'surname' ] ) ) $surname = &$user[ 'surname' ];

		include "./sql/sql.php";

		return $sql->query( "
			SELECT role, login, name, surname
			FROM IIS_users
			LEFT JOIN IIS_persons ON person  = IIS_persons.id
			WHERE
				( login   LIKE '%$login%'   OR login   IS NULL )
			AND ( name    LIKE '%$name%'    OR name    IS NULL )
			AND ( surname LIKE '%$surname%' OR surname IS NULL )
			$role
		");
	}

	function role_to_string ( $role ) {
		switch ( $role ) {
			case 0: return 'student';
			case 1: return 'teacher';
			case 2: return 'admin';
		}
	}

	function role_to_int ( $role ) {
		switch ( $role ) {
			case 'student': return 0;
			case 'teacher': return 1;
			case 'admin':   return 2;
		}
	}

	function gender_to_string ( $gender ) {
		switch ( $gender ) {
			case 0: return 'female';
			case 1: return 'male';
		}
	}

	function gender_to_int ( $gender ) {
		switch ( $gender ) {
			case 'female': return 0;
			case 'male':   return 1;
		}
	}
?>
