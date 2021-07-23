<?php
/**
'id'	=> 	array(
				'group'	=>	'id skupiny',
				'type'	=>	'top / subtop / onlychild / inline / hide',
				'name'	=>	'Jméno',
				'id'	=>	'id odkazu' -> volitelny
				'url'	=>	adresa souboru -> volitelny
			),
*/

	$pages = array(
		'student' => array(
			'default' => 'actual',
			'actual'   => array(
					'group'	=>	'actual',
					'type'	=>	'onlychild',
					'name'	=>	'Aktuality',
			),
			'subjects' => array(
					'group'	=>	'subjects',
					'type'	=>	'top',
					'name'	=>	'Předměty',
					'title' =>  false,
			),
			'exams' => array(
					'group'	=>	'subjects',
					'type'	=>	'subtop',
					'name'	=>	'Zkoušky',
			),
			'terms' => array(
					'group'	=>	'subjects',
					'type'	=>	'subtop',
					'name'	=>	'Termíny',
					'title' =>  false,
			),
			'sign' => array(
					'group'	=>	'hide',
					'type'	=>	'inline',
					'name'	=>	'Přihlásit se',
			),
			'logout' => array(
					'group'	=>	'logout',
					'type'	=>	'onlychild',
					'name'	=>	'Odhlásit se',
					'url'	=>	'./logout.php',
			),
		),

		'admin' => array(
			'default' => 'info',
			'info' => array(
					'group'	=>	'tables',
					'type'	=>	'top',
					'name'	=>	'Info',
			),
			'all-tables' => array(
					'group'	=>	'tables',
					'type'	=>	'subtop',
					'name'	=>	'Všechny tabulky',
			),
			'run-sql' => array(
					'group'	=>	'tables',
					'type'	=>	'subtop',
					'name'	=>	'Spustit SQL',
			),
			'user' => array(
					'group'	=>	'users',
					'type'	=>	'top',
					'id'	=>	'users',
					'name'	=>	'Uživatelé',
			),
			'users' => array(
					'group'	=>	'users',
					'type'	=>	'subtop',
					'name'	=>	'Najít uživatele',
			),
			'add-user' => array(
					'group'	=>	'users',
					'type'	=>	'subtop',
					'name'	=>	'Vytvořit uživatele',
			),
			'edit-user' => array(
					'group'	=>	'hide',
					'type'	=>	'hide',
					'name'	=>	'Upravit uživatele',
			),
			'delete-user' => array(
					'group'	=>	'hide',
					'type'	=>	'inline',
					'name'	=>	'Smazat uživatele',
			),
			'subject' => array(
					'group'	=>	'subjects',
					'type'	=>	'top',
					'id'	=>	'user-subject',
					'name'	=>	'Předměty',
			),
			'add-subject' => array(
					'group'	=>	'subjects',
					'type'	=>	'subtop',
					'name'	=>	'Vytvořit předmět',
			),
			'user-subject' => array(
					'group'	=>	'subjects',
					'type'	=>	'subtop',
					'name'	=>	'Spravovat uživatele v předmětech',
					'title' =>  false,
			),
			'create' => array(
					'group'	=>	'create',
					'type'	=>	'top',
					'name'	=>	'Vytvořit',
			),
			'create-menu' => array(
					'group'	=>	'create',
					'type'	=>	'subtop',
					'name'	=>	'Vytvořit menu',
			),
			'create-tables' => array(
					'group'	=>	'create',
					'type'	=>	'subtop',
					'name'	=>	'Vytvořit tabulky',
			),
			'logout' => array(
					'group'	=>	'logout',
					'type'	=>	'onlychild',
					'name'	=>	'Odhlásit se',
					'url'	=>	'./logout.php',
			),
		),

		'teacher' => array(
			'default' => 'actual',
			'actual' => array(
					'group'	=>	'actual',
					'type'	=>	'onlychild',
					'name'	=>	'Aktuality',
			),
			'subject' => array(
					'group'	=>	'subjects',
					'type'	=>	'top',
					'name'	=>	'Předměty',
					'id'	=>	'subjects',
					'title' =>  false,
			),
			'subjects' => array(
					'group'	=>	'subjects',
					'type'	=>	'subtop',
					'name'	=>	'Přehled všech předmětů',
					'title' =>  false,
			),
			'exams' => array(
					'group'	=>	'subjects',
					'type'	=>	'subtop',
					'name'	=>	'Přehled všech zkoušek',
			),
			'terms' => array(
					'group'	=>	'subjects',
					'type'	=>	'subtop',
					'name'	=>	'Přehled všech termínů',
			),
			'evaluations' => array(
					'group'	=>	'subjects',
					'type'	=>	'hide',
					'name'	=>	'Hodnocení studentů',
					'title' =>  false,
			),
			'add-exam' => array(
					'group'	=>	'hide',
					'type'	=>	'hide',
					'name'	=>	'Vytvořit zkoušku',
			),
			'delete-exam' => array(
					'group'	=>	'hide',
					'type'	=>	'inline',
					'name'	=>	'Smazat zkoušku',
			),
			'edit-exam' => array(
					'group'	=>	'hide',
					'type'	=>	'hide',
					'name'	=>	'Upravit zkoušku',
					'title' =>  false,
			),
			'add-term' => array(
					'group'	=>	'hide',
					'type'	=>	'inline',
					'name'	=>	'Vytvořit termín',
			),
			'edit-term' => array(
					'group'	=>	'hide',
					'type'	=>	'inline',
					'name'	=>	'Upravit termín',
			),
			'delete-term' => array(
					'group'	=>	'hide',
					'type'	=>	'inline',
					'name'	=>	'Smazat termín',
			),
			'save-about' => array(
					'group'	=>	'hide',
					'type'	=>	'inline',
					'name'	=>	'Uložit popis předmětu',
			),
			'logout' => array(
					'group'	=>	'logout',
					'type'	=>	'onlychild',
					'name'	=>	'Odhlásit se',
					'url'	=>	'./logout.php',
			),
		),
	);
?>