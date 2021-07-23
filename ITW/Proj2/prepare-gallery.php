<?php
	$gallery_error = false;

	//Otevru slozku gallery	
	$dir = opendir("./gallery");
	if(!$dir) {		///!!!
		$gallery_error = true;
		echo "Nepodarilo se nacist galerii!";
	}


	$gal_count = count($gallery_names);

	if(isset($_GET["gallery"])) {
		$gallery = $_GET["gallery"];

		if(array_key_exists("$gallery", $gallery_names)) {
			//Otevru slozku
			$dir = opendir("./gallery/" . $gallery);
			
			//Nactu seznam jpg obrazku
			//Vše mimo jpg ignoruji!
			for($i = 0 ; $tmp = readdir($dir) ;) {
				if(pathinfo($tmp, PATHINFO_EXTENSION) != "jpg") {
					continue;
				}
				$gallery_pic[$i] = $tmp;
				$i++;
			}
			
			
			//Pridam obrazky do JS pole
			$pic_count = count($gallery_pic);
			
			echo "\n\n\t<script>\n";
			echo "\t\tvar img_max = ";
			echo $pic_count - 1 . ";\n";
			echo "\t\tvar pictures = new Array(";
			for($i = 0 ; $i < $pic_count ; $i++) {
				echo "\"./gallery/" . $gallery . "/" . $gallery_pic[$i];
				if($i + 1 != $pic_count) {
					echo "\",";
				}
			}
			echo "\");\n";
			
			echo "\t</script>\n";
		}
	}
	
	else {
			$gallery = "";
	}
?>