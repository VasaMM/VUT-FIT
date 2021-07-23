		<ul>
			<li <?php if($page == "o-nas") echo "class=\"active\"";?>><a href="index.php?page=o-nas">O nás</a>
				<ul>
					<li><a href="index.php?page=o-nas">Kdo jsme</a></li>
					<li><a href="index.php?page=co-delame">Co děláme</a></li>
				</ul>
			</li>
			<li <?php if($page == "sluzby") echo "class=\"active\"";?>><a href="index.php?page=sluzby" class="onlychild">Služby</a></li>
			<li <?php if($page == "galerie") echo "class=\"active\"";?>><a href="index.php?page=galerie">Galerie</a>
				<ul>
					<li><a href="index.php?page=galerie&gallery=anteny">Anténní stožáry a výložníky</a></li>
					<li><a href="index.php?page=galerie&gallery=brany">Brány</a></li>
					<li><a href="index.php?page=galerie&gallery=ploty">Ploty a branky</a></li>
					<li><a href="index.php?page=galerie&gallery=zakazkova-vyroba">Zakázková výroba</a></li>
				</ul>
			</li>
			<li <?php if($page == "kontakt") echo "class=\"active\"";?>><a href="index.php?page=kontakt">Kontakt</a>
				<ul>
					<li><a href="index.php?page=kontakt">Kde nás najít</a></li>
					<li><a href="index.php?page=napiste-nam">Napište nám</a></li>
				</ul>
			</li>
		</ul>