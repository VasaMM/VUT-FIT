$(document).ready(function() {
	$('#main-menu>li>a').hover(function() {
		//$('#main-menu li:hover > ul').slideToggle();
	});

	$('#menu-btn').click(function() {
		$('#main-menu').slideToggle('normal');
	});
});