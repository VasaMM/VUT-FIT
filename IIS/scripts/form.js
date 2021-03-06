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
	var names = new Array( "Ji????", "Jan", "Petr", "Josef", "Pavel", "Jaroslav", "Martin", "Miroslav", "Tom????", "Franti??ek", "Zden??k", "V??clav", "Karel", "Milan", "Michal", "Vladim??r", "Luk????", "David", "Ladislav", "Jakub", "Stanislav", "Roman", "Ond??ej", "Anton??n", "Radek", "Marek", "Daniel", "Miloslav", "Vojt??ch", "Jarom??r", "Filip", "Ivan", "Ale??", "Libor", "Old??ich", "Rudolf", "Vlastimil", "Jind??ich", "Milo??", "Adam", "Lubom??r", "Patrik", "Bohumil", "Lubo??", "Robert", "Mat??j", "Dominik", "Radim", "Richard", "Ivo", "Po??et", "Rostislav", "Du??an", "Lud??k", "Vladislav", "Bohuslav", "Alois", "Vit", "V??t", "??t??p??n", "Kamil", "J??n", "Jozef", "Zbyn??k", "??tefan", "Viktor", "Emil", "Michael", "Eduard", "V??t??zslav", "Ludv??k", "Ren??", "Marcel", "Peter", "Dalibor", "Radom??r", "Otakar", "Bed??ich", "??imon", "B??etislav", "Vil??m", "Vratislav", "Maty????", "Radovan", "Leo??", "Marian", "Igor", "P??emysl", "Bohumir", "Bohum??r", "Alexandr", "Kry??tof", "Otto", "Arno??t", "Svatopluk", "Denis", "Adolf", "Hynek", "Erik", "Bronislav", "Alexander", "Marie", "Jana", "Eva", "Anna", "Hana", "V??ra", "Lenka", "Alena", "Kate??ina", "Petra", "Lucie", "Jaroslava", "Ludmila", "Helena", "Jitka", "Martina", "Zde??ka", "Veronika", "Jarmila", "Michaela", "Ivana", "Ji??ina", "Monika", "Tereza", "Bo??ena", "Zuzana", "Vlasta", "Mark??ta", "Marcela", "Dagmar", "Dana", "Libu??e", "Marta", "Irena", "Miroslava", "Barbora", "Pavla", "Eli??ka", "R????ena", "Olga", "Krist??na", "Andrea", "Iveta", "????rka", "Pavl??na", "Blanka", "Milada", "Zdenka", "Kl??ra", "Renata", "Po??et", "Nikola", "Gabriela", "Ad??la", "Radka", "Simona", "Milena", "Miloslava", "Iva", "Daniela", "Milu??e", "Denisa", "Karol??na", "Romana", "Aneta", "Ilona", "Stanislava", "Kv??toslava", "Emilie", "Ane??ka", "Nad????da", "So??a", "Vladim??ra", "Kamila", "Drahom??ra", "Danu??e", "Jind??i??ka", "Nat??lie", "Franti??ka", "Ren??ta", "M??ria", "Al??b??ta", "Vendula", "??t??p??nka", "Bohumila", "Ladislava", "Magdalena", "Dominika", "Bla??ena", "??aneta", "Kv??ta", "Sabina", "Julie", "Antonie", "Alice", "Kristina", "Karla", "Hedvika", "Kv??tu??e", "Alexandra", "Silvie" );
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
