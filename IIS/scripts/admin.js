$( document ).ready( function() {
	// Create menu
	$( 'button#create-menu' ).click( function() {
		$.ajax( {
			url: "http://www.stud.fit.vutbr.cz/~xmarti76/IIS/make-menu.php",
			context: document.body
		}).done( function( data ) {
			$( '#result' ).text( data );
			location.reload();
		});
	});


	// Create tables
	$( 'button#create-tables' ).click( function() {
		$.ajax( {
			url: "http://www.stud.fit.vutbr.cz/~xmarti76/IIS/sql/create-tables.php",
			context: document.body
		}).done( function( data ) {
			$( '#result' ).html( data );
		});
	});

	// Run SQL
	function run () {
		$.ajax( {
			type: 'POST',
			dataType: 'html',
			data: {
				SQL: $( 'input#sql' ).val()
			},
			url: 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/sql/sql.php',
			context: document.body,
			
			success: function (data) {
				$( '#result' ).html( data );
			}
		
		});
	}

	$( '#submit' ).click( function() {
		run();
	});
	$( document ).keypress( function(e) {
		if ( e.which == 13 ) {
			run();
		}
	});

	// Delete user
	var last_clicked;
	$( '.users a.fa-times' ).click( function() {
		$( this ).removeClass( 'fa-times' ).css( { fontWeight : 'bold', color : 'red' } ).text( 'SMAZAT' ).click( function() {
			// Uz jsem klikl na jiny - musim ho vratit do puvodniho stavu
			if ( last_clicked != null ) {
				last_clicked.addClass( 'fa-times' ).css( { fontWeight : 'normal', color : '' } ).text( '' );
			}
			last_clicked = $( this );

			var row = $( this ).parent().parent();
			
			$.ajax( {
				type: 'POST',
				dataType: 'html',
				data: {
					login: $( this ).data( 'login' ),
				},
				url: 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/admin/delete-user.php',
				context: document.body,
				
				success: function (data) {
					$( '#delete-result' ).html( $( data ).find( '#show' ) );
					row.remove();
				},

				error: function (xhr, status, error) {
					var err = eval("(" + xhr.responseText + ")");
  					alert(err.Message);
				}
			});
			// delete_it( this );
		});
	});
})

