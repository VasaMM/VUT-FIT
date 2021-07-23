<?php
	$file = fopen("menu.php", "w+");

	include("define-menu.php");

	fwrite($file, "\t\t<ul>\n");

	$tmp_id    = "";
	$tmp_group = "";
	$tmp_type  = "";
	$tmp_name  = "";

	$prev_id    = "";
	$prev_group = "";
	$prev_type  = "";

	
	foreach($pages as $tmp_id=>$tmp_page) {
		if(array_key_exists ('group',$tmp_page))
			$tmp_group = $tmp_page["group"];
		else
			$tmp_group = $prev_group;
		
		if(array_key_exists ('type',$tmp_page))
			$tmp_type  = $tmp_page["type"];
		else
			$tmp_type = "hide";
		
		if(array_key_exists ('name',$tmp_page))
			$tmp_name = $tmp_page["name"];
		else
			$tmp_name = "Unknown!";
		
		if(array_key_exists ('id',$tmp_page))
			$tmp_id = $tmp_page["id"];


		switch ($tmp_type) {
			case 'top':
				if($prev_type == "subtop") {
					fwrite($file, "\t\t\t\t</ul>\n");
					fwrite($file, "\t\t\t</li>\n");
				}
				elseif($prev_type == "top") {
					fwrite($file, "\t\t\t</li>\n");
				}

				fwrite($file, "\t\t\t");
				fwrite($file, "<li");
				fwrite($file, " <?php if(\$page == \"$tmp_id\") echo \"class=\\\"active\\\"\";?>");
				fwrite($file, ">");
				fwrite($file, "<a href=\"index.php?page=$tmp_id\">");
				fwrite($file, "$tmp_name");
				fwrite($file, "</a>\n");
				fwrite($file, "\t\t\t\t");
				fwrite($file, "<ul>\n");

				$prev_type = $tmp_type;
				if($tmp_id == "galerie") {
					foreach($gallery_names as $tmp_id => $tmp_gallery_name) {
						fwrite($file, "\t\t\t\t\t<li>");
						fwrite($file, "<a href=\"index.php?page=galerie&gallery=$tmp_id\">");
						fwrite($file, "$tmp_gallery_name");
						fwrite($file, "</a></li>\n");
					}
					$prev_type = 'subtop';
				}

				$prev_id = $tmp_id;
				$prev_group = $tmp_group;
				continue;
			
			case 'subtop':
				if($tmp_group != $prev_group) {
					$prev_id = $tmp_id;
					$prev_group = $tmp_group;
					$prev_type = $tmp_type;
					continue;
				}

				fwrite($file, "\t\t\t\t\t<li><a href=\"index.php?page=$tmp_id\">$tmp_name</a></li>\n");
				$prev_id = $tmp_id;
				$prev_group = $tmp_group;
				$prev_type = $tmp_type;
				continue;
			
			case 'onlychild':
				if($prev_type == "subtop") {
					fwrite($file, "\t\t\t\t</ul>\n");
					fwrite($file, "\t\t\t</li>\n");
				}
				elseif($prev_type == "top") {
					fwrite($file, "\t\t\t</li>\n");
				}

				fwrite($file, "\t\t\t");
				fwrite($file, "<li");
				fwrite($file, " <?php if(\$page == \"$tmp_id\") echo \"class=\\\"active\\\"\";?>");
				fwrite($file, ">");
				fwrite($file, "<a href=\"index.php?page=$tmp_id\" class=\"onlychild\">");
				fwrite($file, "$tmp_name");
				fwrite($file, "</a></li>\n");

				$prev_id = $tmp_id;
				$prev_group = $tmp_group;
				$prev_type = $tmp_type;
				continue;
			
			case 'hide':
			
			default:
				/*$prev_id = $tmp_id;
				$prev_group = $tmp_group;
				$prev_type = $tmp_type;*/
				continue;
		}
	}

	if($prev_type == "subtop") {
		fwrite($file, "\t\t\t\t</ul>\n");
		fwrite($file, "\t\t\t</li>\n");
	}
	elseif($prev_type == "top") {
		fwrite($file, "\t\t\t</li>\n");
	}

	fwrite($file, "\t\t</ul>");

	fclose($file);

	echo "DONE!\n";
?>
