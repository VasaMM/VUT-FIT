<?php
	header("Content-Type: text/html; charset=UTF-8");

	include("define-menu.php");

	if(isset($_GET["page"]))
		$page = $_GET["page"];
	else
		$page = "o-nas";

	if(!array_key_exists($page, $pages)) {
		$page = "o-nas";
	}
	if(array_key_exists("id", $pages["$page"])) {
		$page = $pages["$page"]["id"];
	}
	if(array_key_exists("url", $pages["$page"])) {
		$url = $pages["$page"]["url"];
	}
	else {
		$url = "pages/" . $page . ".php";
	}

	$title = $pages[$page]["name"];

?>

<!DOCTYPE html>
<head>
	<!--<title>Zámečnictví - Marek Martinka</title>-->
	<title><?php echo $title ?> - Zámečnictví Marek Martinka</title>
	<meta charset="utf-8">
	<link rel="stylesheet" type="text/css" href="styles/top-menu.css">
	<link rel="stylesheet" type="text/css" href="styles/style.css">
	<link rel="stylesheet" media="print" href="styles/printer.css">

	<script type="text/javascript" src="./scripts/jquery.min.js"></script>
	<script type="text/javascript" src="./scripts/hide.js"></script>
	<script type="text/javascript" src="./scripts/top-menu.js"></script>

<?php 
	if($page == "galerie") {
		include("prepare-gallery.php");
		echo "\t<script type=\"text/javascript\" src=\"./scripts/gallery.js\"></script>";
	}
?>


</head>

<body>

<?php
	echo "<!-- Hlavicka -->\n";
		include("header.php");
	echo "<div class=\"body-content\">\n";
	echo "<div class=\"inner\" id=\"content\">\n";

	echo "<!-- Levy sloupec -->\n";
		include("left-column.php");

	echo "<!-- Hlavni obsah stranky -->\n";
	echo "\n\n<article id=\"main\">\n";
		
		if( $page != "galerie")
			echo "\t<h1>" . $pages["$page"]["name"] . "</h1>\n";
		include($url);
	echo "\n\n</article>\n";

	echo "</div>\n";
	echo "</div>\n";
	echo "<!-- Paticka -->\n";
		include("footer.php");
?>

</body>
</html>

