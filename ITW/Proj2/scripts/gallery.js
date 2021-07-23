$(document).ready(function() {
	img_number = 0;
	left_btn = "url(\"pictures/left-arr.png\")";
	right_btn = "url(\"pictures/right-arr.png\")";
	
	
	$('#go-right').css('background-image', right_btn);
	
	$('#go-left').click(function() {
		go_left();
	});
	
	$('#go-right').click(function() {
		go_right();
	});
	
	
	$('#img-list img').click(function() {
		pick($(this).attr("id"));
	});


//TODO Esc, sipky...
	is_big = false;
	$('#big-img img').click(function() {
		zoom();
	});
});

function go_left() {
	if(img_number != 0) {
		img_number--;
		$('#big-img img').attr("src", pictures[img_number]);
	}
	if(img_number == 0) {
		$('#go-left').css('background-image', 'none');
	}

	if($('#go-right').css('background-image') == "none") {
		$('#go-right').css('background-image', right_btn);
	}
};

function go_right() {
	if(img_number != img_max) {
		img_number++;
		$('#big-img img').attr("src", pictures[img_number]);
	}
	if(img_number == img_max) {
		$('#go-right').css('background-image', 'none');
	}
	
	if($('#go-left').css('background-image') == "none") {
		$('#go-left').css('background-image', left_btn);
	}
};

function pick(pic_id) {
	$('#big-img img').attr("src", pictures[pic_id]);
	img_number = pic_id;
	if(img_number == img_max) {
		$('#go-right').css('background-image', 'none');
		$('#go-left').css('background-image', left_btn);
	}
	else if(img_number == 0) {
		$('#go-left').css('background-image', 'none');
		$('#go-right').css('background-image', right_btn);
	}
	else {
		$('#go-left').css('background-image', left_btn);
		$('#go-right').css('background-image', right_btn);		
	}
};

function zoom() {

	if (!is_big) {
		$('#big-img').css('position', 'fixed');
		$('#big-img').css('top', '0');
		$('#big-img').css('left', '0');
		$('#big-img').css('width', '100%');
		$('#big-img').css('height', '100%');
		$('#big-img').css('max-height', '100%');
		$('#big-img').css('background-color', 'rgba(0,0,0,.9)');

		$('#big-img img').css('height', 'calc(100% - 6em)');
		$('#big-img img').css('padding', '3em 0');
		$('#big-img img').css('max-width', 'calc(100% - 2em - 50px)');
		$('#big-img img').css('box-shadow', 'none');
		$('#img-list').css('padding-top', '370px');

		$('#go-right').css('right', '2em');
		$('#go-right').css('left', 'initial');
		$('#go-left').css('left', '2em');
		$('#go-left').css('right', 'initial');
		is_big = true;
	}
	else {		//Zmensim
		$('#big-img').css('position', '');
		$('#big-img').css('top', '');
		$('#big-img').css('left', '');
		$('#big-img').css('width', '');
		$('#big-img').css('height', '');
		$('#big-img').css('max-height', '');
		$('#big-img').css('background-color', '');

		$('#big-img img').css('height', '');
		$('#big-img img').css('padding', '');
		$('#big-img img').css('max-width', '');
		$('#big-img img').css('box-shadow', 'grey 7px 6px 13px');
		$('#img-list').css('padding-top', '');

		$('#go-right').css('left', '2em');
		$('#go-right').css('right', 'initial');
		$('#go-left').css('right', '2em');
		$('#go-left').css('left', 'initial');
		is_big = false;
	}
};