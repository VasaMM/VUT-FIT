$( document ).ready( function() {
	//Nactu LocalStorage
	var login = localStorage.getItem( 'last_login' );
	
	if ( login != null && $( 'input#login' ).val().length == 0 ) {
		$( 'input#login' ).val( login ).css( 'color', 'grey' );
		$( 'input#login' ).parent().removeClass( 'empty' );

		$( 'input#login' ).click( function() {
			$( this ).val( '' ).css( 'color', '' );
		});

		$( 'input#password' ).click( function() {
			$( 'input#login' ).css('color', 'black');
		});
	}

	$( 'input#submit' ).click( function() {
		localStorage.setItem( 'last_login', $( 'input#login' ).val() );
	});
});
