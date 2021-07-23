$( document ).ready( function() {
	var login    = $( 'input#login' );
	var password = $( 'input#password' );
	var name     = $( 'input#name' );
	var surname  = $( 'input#surname' );
	var role     = $( 'select#role' );

	switch ( role.val() ) {
		case 'student':
		case 'teacher':
		case 'admin':
			break;
		default:
			role.val( 'student' );
	}

	// Pri nacteni a jakekoli zmene aktualizuji login
	update_login( login, name.val(), surname.val(), role.val() );
	login.parent().removeClass( 'empty' );

	$( document ).keyup( function() {
		update_login( login, name.val(), surname.val(), role.val() );
	});

	role.change( function() {
		update_login( login, name.val(), surname.val(), role.val() );
	});

	if ( password.val().length === 0 ) {
		password.parent().removeClass( 'empty' );
		password.prop( 'type', 'text' );
		password.val( generate_password() );

		password.click( function() {
			password.prop( 'type', 'password' );
			password.val( '' );
		});
	}
});

function update_login( login, name, surname, role ) {
	$.ajax( {
		url: "http://www.stud.fit.vutbr.cz/~xmarti76/IIS/sql/username.php?name=" + name + "&surname=" + surname + "&role=" + role + "",
		context: document.body
	}).done( function( data ) {
		login.val( data );
	});
}

function generate_password() {
	return "password";
}