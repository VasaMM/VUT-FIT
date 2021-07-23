		<ul>
			<li <?php if( $page == "actual" ) echo "class=\"active\""; ?> ><a href="actual" class="onlychild">Aktuality</a></li>
			<li <?php if( $page == "subjects" ) echo "class=\"active\""; ?> >
				<a href="subjects">Předměty</a>
				<ul>
					<li><a href="./subjects">Přehled všech předmětů</a></li>
					<li><a href="./exams">Přehled všech zkoušek</a></li>
					<li><a href="./terms">Přehled všech termínů</a></li>
				</ul>
			</li>
			<li <?php if( $page == "logout" ) echo "class=\"active\""; ?> ><a href="logout" class="onlychild">Odhlásit se</a></li>
		</ul>