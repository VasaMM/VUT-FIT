<?php
	session_start();
	session_destroy();
	header( 'Location: http://www.stud.fit.vutbr.cz/~xmarti76/IIS/index.php' );
?>