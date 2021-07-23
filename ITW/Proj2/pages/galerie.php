<?php
	if($gallery != "") {
		echo "\t<h1 class=\"gallery\">" . $gallery_names["$gallery"] . "</h1>\n\n";
	
		echo "\t<div id=\"big-img\">\n";
		echo "\t\t<span id=\"go-left\"></span>\n";
		echo "\t\t\t<img src=\"./gallery/" . $gallery . "/" . "$gallery_pic[0]" . "\" width=\"300px\">";
		echo "\t\t<span id=\"go-right\"></span>\n";
		echo "\t</div>\n";
		echo "\t<div id=\"img-list\">";

		for($i = 0 ; $i < $pic_count ; $i++) {
			echo "\t\t<div class=\"thumbnail\">\n";
			echo "\t\t\t<img src=\"./gallery/" . $gallery . "/" . $gallery_pic[$i] . "\" id=\"" . $i . "\" height=\"100px\">\n";
			echo "\t\t</div>\n";
		}
		echo "\t</div>";
	}
	else {
		echo "\t<h1>Galerie</h1>\n\n";
		echo "\t\t<br><p>Zde by měli být odkazy na galerie...</p>\n";
	}
?>



