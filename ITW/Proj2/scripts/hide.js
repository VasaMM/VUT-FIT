$(document).ready(function() {
	//Nactu LocalStorage
	if (localStorage.getItem("hide_stat") === null) {
		var hide_stat = "true";
	}
	else {
		var hide_stat = localStorage.getItem("hide_stat");
	}
	
	//Na zacatku skryju
	if (hide_stat == "true") {
		$('#site-map').hide();
	}
	else {
		$('#site-map').show();
	}
	
	//Spustim funkce komtrolujici stav
	//hide_show('stat');
	hide_show('#site-map')
});

function hide_show(what) {
	//Ukazu tlacitko
	if( $(what).is(':visible') ) {
		$(what + '-but').text('Skrýt');
	}
	else {
		$(what + '-but').text('Zobrazit');
	}
	$(what + '-but').show();
	
	//Pri kliknuti zmenit stav
	$(what + '-but').click(function() {
		if( $(what).is(':visible') ) {
			$(what).slideToggle('normal');
			$(what + '-but').text('Zobrazit');
			localStorage.setItem("hide_stat", "true");
		}
		else {
			$(what).slideToggle('normal');
			$(what + '-but').text('Skrýt');
			localStorage.setItem("hide_stat", "false");
		}
	});
};