$( document ).ready( function() {
	// Smazat termin / zkousku
	var last_clicked;
	$( '.edit a.fa-times' ).click( function() {
		$( this ).removeClass( 'fa-times' ).css( { fontWeight : 'bold', color : 'red' } ).text( 'SMAZAT' ).click( function() {
			// Uz jsem klikl na jiny - musim ho vratit do puvodniho stavu
			if ( last_clicked != null ) {
				last_clicked.addClass( 'fa-times' ).css( { fontWeight : 'normal', color : '' } ).text( '' );
			}
			last_clicked = $( this );

			var row = $( this ).parent().parent();
			var var_url;

			if ( $( 'body' ).hasClass( 'subjects' ) || $( 'body' ).hasClass( 'exams' ) ) {
				var_url = 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/teacher/delete-exam.php';
			}
			else if ( $( 'body' ).hasClass( 'edit-exam' ) || $( 'body' ).hasClass( 'terms' ) ) {
				var_url = 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/teacher/delete-term.php';
			}
			else { return; }
			$.ajax( {
				type: 'POST',
				dataType: 'html',
				data: {
					id: $( this ).data( 'id' ),
				},
				url: var_url,
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



	// Pridat zkousku
	$( 'button#add-exam' ).click( function() {
		if ( $( this ).data( 'hide' ) ) {
			$( '#add-exam-result' ).html( '' );	
			$( this ).text( "Přidat zkoušku" ).data( 'hide', false );
		}
		else {	
			$( this ).text( "Skrýt" ).data( 'hide', true );
			$.ajax( {
				url: "http://www.stud.fit.vutbr.cz/~xmarti76/IIS/teacher/add-exam.php&id=" + $( this ).data( 'id' ),
				context: document.body
			}).done( function( data ) {
				$( '#add-exam-result' ).html( $( data ).find( 'form' ) );
			});
		}
	});


	// Ulozit 'about'
	$( 'button#save-about' ).click( function() {
		$.ajax( {
			type: 'POST',
			dataType: 'html',
			data: {
				id: $( this ).data( 'id' ),
				about: $( 'textarea#about' ).val()
			},
			url: 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/teacher/save-about.php',
			context: document.body,
			
			success: function (data) {
				$( '#about-result' ).html( $( data ).find( '#show' ) );
			}
		
		});
	});


	// Upravit termin
	$( '.edit a.fa-pencil' ).click( function() {
		$( 'iframe#edit-term' ).height( 300 ).attr( 'src', 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/teacher/edit-term.php&id=' + $( this ).data( 'id' ) );
		// $( 'iframe#edit-term' ).attr( 'src', 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/teacher/edit-term.php&id=' + $( this ).data( 'id' ) );

		// Po ulozeni znovu nactu stranku
		$( 'iframe#edit-term' ).load( function() {
			var content = $( 'iframe#edit-term' ).contents();
		});
	});

	// Pridat termin
	$( 'button#add-term-btn' ).click( function() {
		$( this ).remove();
		$( 'iframe#add-term' ).attr( 'src', 'http://www.stud.fit.vutbr.cz/~xmarti76/IIS/teacher/add-term.php&id=' + $( this ).data( 'id' ) );

		// Po ulozeni znovu nactu stranku
		$( 'iframe#add-term' ).load( function() {
			var content = $( 'iframe#add-term' ).contents();

			content.find( '#submit' ).click( function() {
				window.location.reload();
			});
		});
	});


	// Hodnoceni - soucet bodu	
	$( 'form.evaluations' ).keyup( function() {
		sum_points();
	});

	$( 'form.evaluations' ).click( function() {
		sum_points();
	});

})

function sum_points () {
	max_points = parseInt( $( 'span.max-points' ).text() );
	min_points = parseInt( $( 'span.min-points' ).text() );

	$( 'tr' ).each( function() {
		$sum = 0;
		$inputs = $( 'input[data-points]').each( function() {
			if ( $( this ).hasClass( 'total-points' ) ) {
				if ( $sum < 0 ) $sum = 0;
				$( this ).val( $sum );

				$( this ).removeClass( 'warning' );
				$( this ).css( 'color', '' );

				if ( $sum > max_points ) {
					$( this ).addClass( 'warning' );
				}

				if ( $sum < min_points ) {
					$( this ).css( 'color', 'blue' );
				}
			}
			else {
				$val = $( this ).val();
				if ( $val.length == 0 ) $val = 0;
				$sum += parseInt( $val );
			}
		});
	});
}