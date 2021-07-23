		<ul>
			<li <?php if( $page == "info" ) echo "class=\"active\""; ?> >
				<a href="info">Info</a>
				<ul>
					<li><a href="./all-tables">Všechny tabulky</a></li>
					<li><a href="./run-sql">Spustit SQL</a></li>
				</ul>
			</li>
			<li <?php if( $page == "users" ) echo "class=\"active\""; ?> >
				<a href="users">Uživatelé</a>
				<ul>
					<li><a href="./users">Najít uživatele</a></li>
					<li><a href="./add-user">Vytvořit uživatele</a></li>
				</ul>
			</li>
			<li <?php if( $page == "user-subject" ) echo "class=\"active\""; ?> >
				<a href="user-subject">Předměty</a>
				<ul>
					<li><a href="./add-subject">Vytvořit předmět</a></li>
					<li><a href="./user-subject">Spravovat uživatele v předmětech</a></li>
				</ul>
			</li>
			<li <?php if( $page == "create" ) echo "class=\"active\""; ?> >
				<a href="create">Vytvořit</a>
				<ul>
					<li><a href="./create-menu">Vytvořit menu</a></li>
					<li><a href="./create-tables">Vytvořit tabulky</a></li>
				</ul>
			</li>
			<li <?php if( $page == "logout" ) echo "class=\"active\""; ?> ><a href="logout" class="onlychild">Odhlásit se</a></li>
		</ul>