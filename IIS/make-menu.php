<?php
	include( "define-menu.php" );

	foreach ( $pages as $name => $value ) {
		$file = fopen( $name . "/menu.php", "w+" );

		fwrite( $file, "\t\t<ul>\n" );

		$prev_id    = '';
		$prev_group = '';
		$prev_type  = '';
		
		foreach( $value as $id => $page ) {

			if ( $id == 'default' ) { continue; }

			if( array_key_exists ( 'group',$page ) )
				$group = $page[ 'group' ];
			else
				$group = $prev_group;
			
			if( array_key_exists ( 'type', $page ) )
				$tmp_type  = $page[ 'type' ];
			else
				$tmp_type = 'hide';
			
			if( array_key_exists ( 'name', $page ) )
				$name = $page[ 'name' ];
			else
				$name = 'Unknown!';
			
			if( array_key_exists ( 'id', $page ) )
				$id = $page[ 'id' ];


			switch ( $tmp_type ) {
				case 'top':
				case 'onlychild':
					if( $prev_type == "subtop" ) {
						fwrite( $file, "\t\t\t\t</ul>\n\t\t\t</li>\n" );
					}
					elseif( $prev_type == "top" ) {
						fwrite( $file, "\t\t\t</li>\n" );
					}

					fwrite( $file, "\t\t\t" . '<li <?php if( $page == "'. $id . '" ) echo "class=\"active\""; ?> >' );

					if ( $tmp_type == 'top' ) {
						fwrite( $file, "\n\t\t\t\t" . '<a href="' . $id . '">' . $name . "</a>\n\t\t\t\t<ul>\n" );
					}
					else {
						fwrite( $file, '<a href="' . $id . '" class="onlychild">' . $name . "</a></li>\n" );
					}
					
					$prev_id = $id;
					$prev_group = $group;
					$prev_type = $tmp_type;
					continue;
				
				case 'subtop':
					if ( $group == $prev_group ) {
						fwrite( $file, "\t\t\t\t\t<li><a href=\"./$id\">$name</a></li>\n" );
					}

					$prev_id = $id;
					$prev_group = $group;
					$prev_type = $tmp_type;
					continue;
				
				case 'hide':
				case 'inline':

				default:
					continue;
			}
		}

		if ( $prev_type == "subtop" ) {
			fwrite( $file, "\t\t\t\t</ul>\n\t\t\t</li>\n" );
		}
		else if ( $prev_type == "top" ) {
			fwrite( $file, "\t\t\t</li>\n" );
		}

		fwrite( $file, "\t\t</ul>" );

		fclose( $file );
	}


	echo 'DONE!';
?>
