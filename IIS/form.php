<?php

class Form {
	function __construct( array $definitation, $output = NULL, $escape_html = true ) {
		$this->definitation = $definitation;
		$this->escape_html = $escape_html;

		if ( ! empty( $output ) ) {
			$this->output = $output;
		}
	}
	
	private	$definitation;
	private $output;
	private $print = '';
	private $print_empty = '';
	private $escape_html;
	public  $valid = false;


	private function add_print( $print, $print_empty = "" ) {
		if ( $print_empty == "" ) {
			$this->print       .= $print;
			$this->print_empty .= $print;
		}
		else {
			$this->print       .= $print;
			$this->print_empty .= $print_empty;
		}
	}

	/*
	   PUBLIC
	           */
	public function isOK() {
		return $this->valid;
		if ( empty( $set ) ) {
		}
		echo "nasta";
		$this->valid = $set;
	}
	
	public function setValid( $valid ) {
		$this->valid = $valid;
	}

	public function output( $get = NULL ) {
		if ( empty( $get ) ) {
			return $this->output;
		}
		return $this->output[ $get ];
	}

	public function set_output( $set = NULL ) {
		$this->output = $set;
	}

	public function printForm( $print_empty = NULL ) {
		if ( $print_empty === false ) {
			echo $this->print;
		}
		else {
			if ( $this->valid ) {
				echo $this->print_empty;
			}
			else {
				echo $this->print;
			}
		}
	}

	public function make() {
		$this->valid  = true;
		$this->output = array();

		// Nastavim volitelne parametry
		$id = '';
		$class = '';
		if ( isset( $this->definitation[ 'id'    ] ) )  $id    = 'id="'    . $this->definitation[ 'id'    ] . '" ';
		if ( isset( $this->definitation[ 'class' ] ) )  $class = 'class="' . $this->definitation[ 'class' ] . '" ';

		$action = 'action="' . $this->definitation[ 'action' ] . '"';
		$method = 'method="' . $this->definitation[ 'method' ] . '"';
		
		$this->add_print( "<form $action $method $class $id >\n" );

		// Ulozim si pouzivanou metodu
		if ( $this->definitation[ 'method' ] == 'post' ) $method = &$_POST;
		if ( $this->definitation[ 'method' ] == 'get' )  $method = &$_GET;

		// V cyklu postupne projdu vsechny bloky formulare
		foreach ( $this->definitation[ 'fields' ] as $field_name => $field ) {
			$output = &$this->output[ $field_name ];
			$output = array();

			// Nastavim volitelne parametry
			$id = '';
			$class = '';
			if ( isset( $field[ 'id'    ] ) )  $id    = ' id="'    . $field[ 'id'    ] . '" ';
			if ( isset( $field[ 'class' ] ) )  $class = ' class="' . $field[ 'class' ] . '" ';

			$this->add_print( "\t<fieldset $id $class>\n" );
			$this->add_print( "\t\t<legend>" . $field[ 'name' ] . "</legend>\n" );

			// V cyklu postupne projdu vsechny prvky bloku
			foreach ( $field[ 'inputs' ] as $id => $options ) {
				// Otestuji zda je policko vyzadovano
				$required = '';
				if ( isset( $options[ 'required' ] ) && $options[ 'required' ] ) $required = 'required';
				
				// Otestuji, zda jsem danou hodnotu jiz nevyplnil
				if ( ! empty( $method[ $id ] ) ) {
					// Osetrim html znaky
					if ( $this->escape_html ) {
						$this->output[ $field_name ][ $id ] = str_replace( ">","&gt;", str_replace( "<","&lt;", $method[ $id ] ) );
					}
					else {
						$this->output[ $field_name ][ $id ] = $method[ $id ];
					}
				}
				else if ( ! empty( $required ) ) {
					$required = 'required empty';
					$this->valid = false;
				}

				// Nastavim volitelne parametry
				$class = '';
				$disabled = '';
				$size = 'size=10';
				if ( isset( $options[ 'disabled' ] ) && $options[ 'disabled' ] )  $disabled = 'disabled';
				if ( isset( $options[ 'size'     ] )                           )  $size     = 'size=' . $options[ 'size' ];
				if ( isset( $options[ 'class'    ] )                           )  $class    = 'class="' . $options[ 'class' ] . '"';

				$value = '';
				$default_value = '';
				if ( isset( $options[ 'default' ] ) ) {
					$default_value = $options[ 'default' ];
					$value = $default_value;
				}
				if ( isset( $this->output[ $field_name ][ $id ] ) ) $value = $this->output[ $field_name ][ $id ];

				$this->add_print( "\t\t<div class=\"container " . $required . "\">\n" );
				$this->add_print( "\t\t\t<label for=\"$id\">" . $options[ 'name' ] . "</label>\n" );
				
				// Vypisu samotny prvek
				switch ( $options[ 'type' ] ) {
					case 'text':
					case 'password':
					case 'number':
					case 'email':
					case 'date':
					case 'datetime-local':
					case 'phone':
						$this->add_print( "\t\t\t<input size=$size id=\"$id\" name=\"$id\" type=\"" . $options[ 'type' ] . "\" value=\"$value\" class=\"$class\" $disabled>\n",
						                  "\t\t\t<input size=$size id=\"$id\" name=\"$id\" type=\"" . $options[ 'type' ] . "\" value=\"$default_value\" class=\"$class\" $disabled>\n");
						break;

					case 'select':
						$this->add_print( "\t\t\t<select id=\"$id\" name=\"$id\">\n" );

						foreach ( $options[ 'values' ] as $value_ => $name ) {
							  if ( $value_ == $value ) $selected = 'selected';
							else                       $selected = '';

							  if ( $value_ == $default_value ) $default_selected = 'selected';
							else                               $default_selected = '';
							$this->add_print( "\t\t\t\t<option value=\"$value_\" $selected>$name</option>\n",
							                  "\t\t\t\t<option value=\"$value_\" $default_selected>$name</option>\n" );
						}
						$this->add_print( "\t\t\t</select>\n" );
						break;

					case 'radio':
						$this->add_print( "\t\t\t<div class=\"content\">\n" );
						
						foreach ( $options[ 'values' ] as $value_ => $name ) {
							  if ( $value_ == $value ) $selected = 'checked';
							else                       $selected = '';

							  if ( $value_ == $default_value ) $default_selected = 'checked';
							else                               $default_selected = '';
							
							$this->add_print( "\t\t\t\t<input type=\"radio\" name=\"$id\" value=\"$value_\" $selected>$name\n",
							                  "\t\t\t\t<input type=\"radio\" name=\"$id\" value=\"$value_\" $default_selected>$name\n" );
						}

						$this->add_print( "\t\t\t</div>\n" );
						break;
				}
				$this->add_print( "\t\t</div>\n" );
			}
			$this->add_print( "\t</fieldset>\n" );
		}

		$disabled = 'disabled';
		if ( $this->valid ) { $disabled = ''; }

		$this->add_print( "\t" . '<input id="submit" name="submit" type="submit" ' . $disabled . ' value="' . $this->definitation[ 'submit' ] . "\">\n" );
		$this->add_print( '</form>' );
	}

}


?>
