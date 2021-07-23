/***************************
 * Author: Václav Martinka
 * About: Projekt do predmetu WAP, FIT VUTBR 2019
 * License: CC BY-SA 2.0
 **************************/

// Konstanty pro nastaveni chovani
const INIT_WINDOW_WIDTH   = 200;
const INIT_WINDOW_HEIGHT = 100;

const MINIMAL_WINDOW_WIDTH  = 120;
const MINIMAL_WINDOW_HEIGHT = 90;



// Pojmenovani vsech okraju okna
var borders = [ 'left-top', 'top', 'right-top', 'left', 'right', 'left-bottom', 'bottom', 'right-bottom' ];



// Registruje mys i dotyk zaroven
// FIXME dotykova obrazovka
function listen_up( obj, action ) {
	obj.addEventListener( 'mouseup',  action, true );
	// obj.addEventListener( 'touchend', action, true );
}

function listen_down( obj, action ) {
	obj.addEventListener( 'mousedown',  action, true );
	// obj.addEventListener( 'touchstart', action, true );
}

function listen_move( obj, action ) {
	obj.addEventListener( 'mousemove', action, true );
	// obj.addEventListener( 'touchmove', action, true );
}



// Vypise log do konzole
function log ( string ) {
	console.log( string );
}



// Vytvori objekt okna
function create_window_obj( w, desktop ) {
	let obj = {
		// HTML Object
		html: w,


		// Desktop HTML
		desktop: desktop,


		// Presune okno na vrch
		top_me: () => { if (w.parentNode) { w.parentNode.appendChild( w ) } },

		
		// Zavre okno
		close_me: () => w.style.display = 'none',


		// Minimalizace
		minimize: () => {
			w.removeAttribute( 'data-maximize' );
			w.style.bottom = '';
			w.style.right  = '';

			obj.reset_dimension();
			obj.reset_position();
		},	


		// Maximalizace
		maximize: () => {
			w.setAttribute( 'data-maximize', true );
			w.style.top    = '0';
			w.style.right  = '0';
			w.style.bottom = '0';
			w.style.left   = '0';
			
			w.style.width  = '';
			w.style.height = '';
		},	
		is_maximize: () => { return w.getAttribute( 'data-maximize' ) === 'true' },


		// Rozmery
		get width() {
			let width = w.getAttribute( 'width' );
			return width ? parseInt( width ) : INIT_WINDOW_WIDTH;
		},
		
		set width( value ) {
			value = parseInt( value );
			value = value > w.parentNode.clientWidth ? w.parentNode.clientWidth : value;
			if ( value >= MINIMAL_WINDOW_WIDTH ) {
				w.style.width = value + 'px';
				w.setAttribute( 'width', value + 'px' );
			}
		},

		set_width: ( value ) => {
			if ( value < MINIMAL_WINDOW_WIDTH ) return false;
			else return obj.width = value;
		},


		get height() {
			let height = w.getAttribute( 'height' );
			return height ? parseInt( height ) : INIT_WINDOW_HEIGHT;
		},
		
		set height( value ) {
			value = parseInt( value );
			value = value > w.parentNode.clientHeight ? w.parentNode.clientHeight : value;
			if ( value >= MINIMAL_WINDOW_HEIGHT ) {
				w.style.height = value + 'px';
				w.setAttribute( 'height', value + 'px' );
			}
		},

		set_height: ( value ) => {
			if ( value < MINIMAL_WINDOW_HEIGHT ) return false;
			else return obj.height = value;
		},

		reset_dimension: () => {
			obj.width  = obj.width;
			obj.height = obj.height;

			if ( obj.is_maximize() ) obj.maximize();
		},
		
		
		// Pozice
		get left() {
			let left = w.getAttribute( 'data-left' );
			return left ? parseInt( left ) : 0;
		},

		set left( value ) {
			value = parseInt( value );
			value = value < 0 ? 0 : value;
			w.style.left = value + 'px';
			w.setAttribute( 'data-left', value + 'px' );
		},

		get top() {
			let top = w.getAttribute( 'data-top' );
			return top ? parseInt( top ) : 0;
		},

		set top( value ) {
			value = parseInt( value );
			value = value < 0 ? 0 : value;
			w.style.top = value + 'px';
			w.setAttribute( 'data-top', value + 'px' );
		},

		reset_position: () => {
			obj.left = obj.left + obj.width  > w.parentNode.clientWidth  ? w.parentNode.clientWidth  - obj.width  : obj.left;
			obj.top  = obj.top  + obj.height > w.parentNode.clientHeight ? w.parentNode.clientHeight - obj.height : obj.top;
			
			if ( obj.is_maximize() ) {
				obj.maximize();
			}
		},
	};

	return obj;
}



// Inicializuji okno
function init_window( w, desktop ) {
	// Vytvorim objekt
	let window_obj = create_window_obj( w, desktop );

	
	// Ulozim a pote vymazu puvodni obsah a title atribut
	let content = w.innerHTML;
	w.innerHTML = '';

	let title = w.getAttribute( 'title' );
	title = title === null ? '' : title;
	w.removeAttribute( 'title' );


	// Vytvorim ramecek
	for ( let i = 0 ; i < borders.length ; ++i ) {
		let tmp = document.createElement( 'SPAN' );
		    tmp.classList.add( 'border' );
		    tmp.classList.add( borders[ i ] );
		w.appendChild( tmp );
	}


	// Nactu ikonku
	let icon = '';
	if ( w.getAttribute( 'data-icon' ) ) {
		icon = '<img src="' + w.getAttribute( 'data-icon' ) + '" class="icon">';
	}


	// Vytvorim titulek
	let title_element = document.createElement( 'DIV' );
		title_element.classList.add( 'border' );
		title_element.classList.add( 'title' );
		title_element.innerHTML = icon + ' \
			<span class="move">' + title + '</span> \
			<i class="fa fa-times btn-close"></i> \
			<i class="fa fa-window-maximize btn-maximize"></i>';
			// <i class="fa fa-window-minimize btn-minimize"></i>'
	w.appendChild( title_element );


	// Nastavim rozmery a pozici
	window_obj.reset_dimension();
	window_obj.reset_position();
	

	// Vlozim puvodni obsah
	let wrap = document.createElement( 'DIV' );
		wrap.classList.add( 'wrap' );
		wrap.innerHTML = content;
	w.appendChild( wrap );


	// Ozivim tlacitka
	init_buttons( window_obj );


	// Zobrazim obsah
	w.style.display = 'block';


	// Ohlidam resize hlavniho okna
	window.addEventListener( 'resize', () => {
		window_obj.reset_dimension();
		window_obj.reset_position();
	});
}



// Pohyb okna
// Inspirace https://stackoverflow.com/questions/24050738/javascript-how-to-dynamically-move-div-by-clicking-and-dragging
function move_window( window_obj, title ) {
	let move_hold = false;
	// Relativni souradnice
	let x, y;

	


	listen_down( title, function( e ) {
		e.preventDefault();
		if ( window_obj.html.getAttribute( 'data-maximize' ) !== 'true' ) {

			move_hold = true;
			title.classList.add( 'grabbing' );
			title.style.cursor = 'grabbing';

			x = window_obj.left - e.clientX;
			y = window_obj.top  - e.clientY;
			
			window_obj.top_me();
		}
	});

	listen_up( document, function( e ) {
		e.preventDefault();
		title.classList.remove( 'grabbing' );
		title.style.cursor = '';
		move_hold = false;
	});

	listen_move( document, function( e ) {
		e.preventDefault();
		if ( move_hold ) {
			
			// Spocitam novou pozici
			let left = e.clientX + x;
			let top  = e.clientY + y;
			
			// Provedu kontrolu na okraje
			window_obj.left = left + window_obj.width  > window_obj.html.parentNode.clientWidth  ? window_obj.html.parentNode.clientWidth  - window_obj.width  : left;
			window_obj.top  = top  + window_obj.height > window_obj.html.parentNode.clientHeight ? window_obj.html.parentNode.clientHeight - window_obj.height : top;
		}
	});
}



// Zmena velikosi okna
function resize_window( window_obj ) {
	let resize_hold = -1;
	let way;
	
	// Zaregistruji vsechny smery
	for ( let i = 0 ; i < borders.length ; ++i ) {
		listen_down( window_obj.html.querySelector( '.border.' + borders[ i ] ), function( e ) {
			if ( window_obj.html.getAttribute( 'data-maximize' ) !== 'true' ) {
				resize_hold = i;
			}
		});
	}


	listen_up( document, function() {
		resize_hold = -1;
	});


	listen_move( document, function( e ) {
		e.preventDefault();
		if ( resize_hold >= 0 ) {
			// Relativni souradnice
			let x = Math.round( e.clientX - window_obj.desktop.corner.pos().x );
			let y = Math.round( e.clientY - window_obj.desktop.corner.pos().y );

			// Normalizace na 0
			x = x < 0 ? 0 : x;
			y = y < 0 ? 0 : y;

			// Normalizace na okraje
			x = x > window_obj.html.parentNode.clientWidth  ? window_obj.html.parentNode.clientWidth  : x;
			y = y > window_obj.html.parentNode.clientHeight ? window_obj.html.parentNode.clientHeight : y;


			// Upravim patricne okraje
			if ( borders[ resize_hold ].search( 'right' ) >= 0 ) {
				window_obj.width = x - window_obj.left;
			}

			if ( borders[ resize_hold ].search( 'left' ) >= 0 ) {
				if ( window_obj.set_width( window_obj.width - ( x - window_obj.left ) ) ) {
					window_obj.left = x;
				}
			}
			
			if ( borders[ resize_hold ].search( 'bottom' ) >= 0 ) {
				window_obj.height = y - window_obj.top;
			}

			if ( borders[ resize_hold ].search( 'top' ) >= 0 ) {
				if ( window_obj.set_height( window_obj.height - ( y - window_obj.top ) ) ) {
					window_obj.top = y;
				}
			}

			window_obj.top_me();
		}
	});
}


// Inicializuji ovladani okna (pohyb, zmena velikosti, tlacitka)
function init_buttons( window_obj ) {
	let title = window_obj.html.getElementsByClassName( 'title' )[0];

	// let minimize = title.getElementsByClassName( 'btn-minimize' )[0];
	let maximize = title.getElementsByClassName( 'btn-maximize' )[0];
	let close    = title.getElementsByClassName( 'btn-close' )[0];

	// Zavreni okna
	close.onclick = function() { window_obj.close_me(); };

	// Zvyrazneni okna
	window_obj.html.onclick = function() { window_obj.top_me(); };


	// Maximalizace
	maximize.onclick = function(){
		if ( window_obj.is_maximize() ) {
			window_obj.minimize();
		}
		else {
			window_obj.maximize();
		}
	};


	// Posun okna
	move_window( window_obj, title.getElementsByClassName( 'move' )[0] );

	// Zmena velikosti okna
	resize_window( window_obj );
}


// Main funkce
function desktop() {
	'use strict';

	document.addEventListener( 'DOMContentLoaded', function() {
		let main = function () {
			// Nactu vsechna desktopy
			let desktops = document.getElementsByClassName( 'desktop' );

			// Postupne je inicializuji
			for ( let i = 0 ; i < desktops.length ; ++i ) {
				let desktop = desktops[ i ];

				// Vytvorim rohovy div
				let corner = document.createElement( 'DIV' );
				    corner.classList.add( '__desktop_corner__' );
				    corner.style.width = '0';
				    corner.style.height = '0';
				    corner.style.position = 'absolute';
				    corner.style.top  = '0px';
				    corner.style.left = '0px';

				// Vlozim na zacatek
				desktop.insertBefore( corner, desktop.firstChild );

				// Pridam podporu na vypocet souradnic
				desktop.corner = corner;
				desktop.corner.pos = corner.getBoundingClientRect;

				// Nactu a inicializuji jednotliva okna
				let windows = desktop.querySelectorAll( '.desktop > .window' );
				for ( let i = 0 ; i < windows.length ; ++i ) {
					init_window( windows[i], desktop );
				}
			}



		}

		// Zkontroluji pritomnost less
		if ( typeof less !== "undefined" ) {
			less.pageLoadFinished.then( function() {
				main();
			});
		}
		else {
			main();
		}
	}, false );
}
