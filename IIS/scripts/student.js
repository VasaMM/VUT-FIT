$( document ).ready( function() {
	// Prihlasit / odhlasit se
	
	$( 'table a.fa-sign-in' ).click( function() {
		var result = $( this ).parent().parent().children( '.logged' );
		$.ajax( {
			type: 'POST',
			dataType: 'html',
			data: {
				id: $( this ).data( 'id' ),
			},
			url: 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/student/sign.php',
			context: document.body,
			
			success: function ( data ) {
				// alert(data);
				result.html( $( data ).find( '#show' ) )
				// $( '#delete-result' ).html( $( data ).find( '#show' ) );
				// row.remove();
			},

			error: function ( xhr, status, error ) {
				var err = eval("(" + xhr.responseText + ")");
				alert(err.Message);
			}
		});
	});
});