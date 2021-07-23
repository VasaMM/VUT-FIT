<?php
	$gallery_names = array(
		'anteny'			=> 'Anténní stožáry a výložníky',
		'brany'				=> 'Brány',
		'ploty'				=> 'Ploty a branky',
		'zakazkova-vyroba'	=> 'Zakázková výroba',
	);

/**
'id'	=> 	array(
				'group'	=>	'id skupiny',
				'type'	=>	'top / subtop / onlychild / hide',
				'name'	=>	'Jméno',
				'id'	=>	'id odkazu' -> volitelny
				'url'	=>	adresa souboru -> volitelny
			),
*/

	$pages = array(
		'o-nas'					=> 	array(
										'group'	=>	'o-nas',
										'top'	=>	true,
										'type'	=>	'top',
										'name'	=>	'O nás',
									),
		'kdo-jsme'				=>	array(
										'group'	=>	'o-nas',
										'top'	=>	false,
										'type'	=>	'subtop',
										'name'	=>	'Kdo jsme',
										'id'	=>	'o-nas',
									),
		'co-delame'				=>	array(
										'group'	=>	'o-nas',
										'top'	=>	false,
										'type'	=>	'subtop',
										'name'	=>	'Co děláme',
									),
		'sluzby'				=>	array(
										'group'	=>	'sluzby',
										'top'	=>	true,
										'type'	=>	'onlychild',
										'name'	=>	'Služby',
									),
		'galerie'				=>	array(
										'group'	=>	'galerie',
										'top'	=>	true,
										'type'	=>	'top',
										'name'	=>	'Galerie',
									),
		'kontakt'				=>	array(
										'group'	=>	'kontakt',
										'top'	=>	true,
										'type'	=>	'top',
										'name'	=>	'Kontakt',
									),
		'kde-nas-najit'			=>	array(
										'group'	=>	'kontakt',
										'top'	=>	false,
										'type'	=>	'subtop',
										'name'	=>	'Kde nás najít',
										'id'	=>	'kontakt',
									),
		'napiste-nam'			=>	array(
										'group'	=>	'kontakt',
										'top'	=>	false,
										'type'	=>	'subtop',
										'name'	=>	'Napište nám',
									),
		'site-map'				=>	array(
										'group'	=>	'hidden',
										'top'	=>	false,
										'type'	=>	'hide',
										'name'	=>	'Mapa stránek',
										'url'	=>	'menu.php'
									),
	);
?>