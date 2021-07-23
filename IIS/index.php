<?php
	header("Content-Type: text/html; charset=UTF-8");
	session_start();

	// Zjistim, zda jsem se uz pokousel prihlasit
	$error;
	$login;
	if ( isset( $_SESSION[ 'error' ] ) ) {
		if ( isset( $_SESSION[ 'error' ] ) ) {
			$error = $_SESSION[ 'error' ];
		}
		if ( isset( $_SESSION[ 'login' ] ) ) {
			$login = $_SESSION[ 'login' ];
		}
	}
	// Posledni prihlaseni bylo uspesne - preskocim prihlasovaci stranku
	else if ( isset( $_SESSION[ 'login' ] ) ) {
		header( 'Location: ' . $_SESSION[ 'role' ] . '/' );
		return;
	}

	$title = 'Přihlášení';
	$page = 'login-page';
	$no_menu = true;

	echo "<!-- Hlavicka -->\n";
	include("header.php");
?>

		<div class="body-content">
			<div class="inner" id="content">

				<!-- Hlavni obsah stranky -->
				<article id="main">
					<h3>Vítejte ve <strong>V</strong>ysněném <strong>I</strong>nformačním <strong>S</strong>ystému</h3>

					<form  action="login" method="post">
						<fieldset>
							<legend>Přihlašovací údaje</legend>
							<div class="container required">
								<label for="login">Login</label>
								<input size=10 id="login" value="<?php if ( ! empty( $login ) ) echo $login ?>" name="login" type="text">
							</div>
							<div class="container required">
								<label for="password">Heslo</label>
								<input size=10 id="password" name="password" type="password">
							</div>

							<?php
								if ( ! empty( $error ) ) {
									echo '<div class="error">' . $error . '</div>';
								}
							?>
						</fieldset>

						<input id="submit" type="submit" value="Přihlásit">
					</form>
				</article>
			</div>
		</div>

	<!-- Paticka -->
	<?php include("footer.php"); ?>

</body>
</html>

