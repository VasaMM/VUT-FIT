$( document ).ready( function() {
	// Pro heslo pridam oko
	$( 'input[type="password"]' ).each( function() {
		var input = $( this );
		input.parent().append( '<i class="fa fa-eye" aria-hidden="true"></i>' )
		
		// Pri kliknuti na oko zobrazim heslo
		input.siblings( 'i.fa-eye' ).mousedown( function() {
			input.addClass( 'show' );
			input.prop( 'type', 'text' );
		})
	});

	// Pri odkliknuti zase skryji
	$( document ).mouseup( function() {
		$( 'input.show' ).each( function() {
			$( this ).prop( 'type', 'password' );
		});
	});

	check_inputs();
	
	$( document ).keyup( function() {
		check_inputs();
	});

	$( document ).click( function() {
		check_inputs();
	});

	// Pridam moznost generovat nahodne hodnoty
	$( 'form.random' ).each( function() {
		var form = $( this );
		
		var random_button = $( '<i class="fa fa-random" aria-hidden="true" style="cursor: pointer;"></i>' );
		$( this ).prepend( random_button );
		
		random_button.click( function() {
			random_generator( form );
		});
	});


	// O(d)znacit vse
	$( 'input[type="checkbox"].check-all' ).click( function() {
		$( '[data-group="' + $(this).data('group') + '"]').prop( 'checked', $( this ).prop( 'checked' ) );
	});
});

function check_inputs () {
	$( 'form' ).each( function() {
		var empty = false;
		
		$( this ).find( '.required' ).each( function() {
			if ( $( this ).find( 'input' ).val().length > 0 ) {
				$( this ).removeClass( 'empty' );
			}
			else {
				$( this ).addClass( 'empty' );
				empty = true;
			}
		});

		if ( empty ) {
			$( this ).find( 'input[type="submit"]' ).prop( 'disabled', 'disabled' );
		}
		else {
			$( this ).find( 'input[type="submit"]' ).prop( 'disabled', '' );
		}
	});
}

function random_generator ( form ) {
	$( form ).find( 'input' ).each( function() {
		if ( ! $( this ).prop( 'disabled' ) ) {
			switch ( $( this ).prop( 'type' ) ) {
				case 'text':
					$( this ).val( random_name() );
					break;

				case 'number':
					$( this ).val( random_number() );
					break;

				case 'email':
					$( this ).val( random_name() + '@' + random_name() + '.mail' );
					break;

				case 'date':
					$( this ).val( random_date() );
					break;

				case 'phone':
					$( this ).val( '+420' + random_date() +  random_date() +  random_date() );
					break;
			}
		}
	});
}

function random_name () {
	var names = new Array( "Jiří", "Jan", "Petr", "Josef", "Pavel", "Jaroslav", "Martin", "Miroslav", "Tomáš", "František", "Zdeněk", "Václav", "Karel", "Milan", "Michal", "Vladimír", "Lukáš", "David", "Ladislav", "Jakub", "Stanislav", "Roman", "Ondřej", "Antonín", "Radek", "Marek", "Daniel", "Miloslav", "Vojtěch", "Jaromír", "Filip", "Ivan", "Aleš", "Libor", "Oldřich", "Rudolf", "Vlastimil", "Jindřich", "Miloš", "Adam", "Lubomír", "Patrik", "Bohumil", "Luboš", "Robert", "Matěj", "Dominik", "Radim", "Richard", "Ivo", "Počet", "Rostislav", "Dušan", "Luděk", "Vladislav", "Bohuslav", "Alois", "Vit", "Vít", "Štěpán", "Kamil", "Ján", "Jozef", "Zbyněk", "Štefan", "Viktor", "Emil", "Michael", "Eduard", "Vítězslav", "Ludvík", "René", "Marcel", "Peter", "Dalibor", "Radomír", "Otakar", "Bedřich", "Šimon", "Břetislav", "Vilém", "Vratislav", "Matyáš", "Radovan", "Leoš", "Marian", "Igor", "Přemysl", "Bohumir", "Bohumír", "Alexandr", "Kryštof", "Otto", "Arnošt", "Svatopluk", "Denis", "Adolf", "Hynek", "Erik", "Bronislav", "Alexander", "Marie", "Jana", "Eva", "Anna", "Hana", "Věra", "Lenka", "Alena", "Kateřina", "Petra", "Lucie", "Jaroslava", "Ludmila", "Helena", "Jitka", "Martina", "Zdeňka", "Veronika", "Jarmila", "Michaela", "Ivana", "Jiřina", "Monika", "Tereza", "Božena", "Zuzana", "Vlasta", "Markéta", "Marcela", "Dagmar", "Dana", "Libuše", "Marta", "Irena", "Miroslava", "Barbora", "Pavla", "Eliška", "Růžena", "Olga", "Kristýna", "Andrea", "Iveta", "Šárka", "Pavlína", "Blanka", "Milada", "Zdenka", "Klára", "Renata", "Počet", "Nikola", "Gabriela", "Adéla", "Radka", "Simona", "Milena", "Miloslava", "Iva", "Daniela", "Miluše", "Denisa", "Karolína", "Romana", "Aneta", "Ilona", "Stanislava", "Květoslava", "Emilie", "Anežka", "Naděžda", "Soňa", "Vladimíra", "Kamila", "Drahomíra", "Danuše", "Jindřiška", "Natálie", "Františka", "Renáta", "Mária", "Alžběta", "Vendula", "Štěpánka", "Bohumila", "Ladislava", "Magdalena", "Dominika", "Blažena", "Žaneta", "Květa", "Sabina", "Julie", "Antonie", "Alice", "Kristina", "Karla", "Hedvika", "Květuše", "Alexandra", "Silvie" );
	return names[ Math.floor( Math.random() * names.length ) ];
}

function random_number () {
	return Math.floor( Math.random() * 999 );
}


function random_date () {
	var month = Math.floor( Math.random() * 11 ) + 1;
	switch ( month ) {
		case  1:
		case  3:
		case  5:
		case  7:
		case  8:
		case 10:
		case 12:
			var day = Math.floor( Math.random() * 30 ) + 1;
			break;

		case  2:
			var day = Math.floor( Math.random() * 27 ) + 1;
			break;

		case  4:
		case  6:
		case  9:
		case 11:
			var day = Math.floor( Math.random() * 29 ) + 1;
			break;

		default:
			var day = '01';
	}

	if ( month < 10 ) { month = '0' + month; }
	if ( day   < 10 ) { day   = '0' + day; }

	return ( Math.floor( Math.random() * 80 ) + 1950 ) + '-' + month + '-' + day;
}
