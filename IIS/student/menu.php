		<ul>
			<li <?php if( $page == "actual" ) echo "class=\"active\""; ?> ><a href="actual" class="onlychild">Aktuality</a></li>
			<li <?php if( $page == "subjects" ) echo "class=\"active\""; ?> >
				<a href="subjects">Předměty</a>
				<ul>
					<li><a href="./exams">Zkoušky</a></li>
					<li><a href="./terms">Termíny</a></li>
				</ul>
			</li>
			<li <?php if( $page == "logout" ) echo "class=\"active\""; ?> ><a href="logout" class="onlychild">Odhlásit se</a></li>
		</ul>