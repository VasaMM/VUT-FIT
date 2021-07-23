<?php
/* * * * * * * * * *
 * AUTOR: xmarti76
 * DKA - projekt do IPP
 * 15. 03. 207
 * * * * * * * * * */

//FIXME EOF check

// Funcke na tisk ERROR
function error ($string = "Some error!", $return_value = 100) {
	error_log("ERROR: $string\n");
	exit($return_value);
}

// Nactu pristi ','
function read_comma (options $options) {
	if ($options->get_char() != ',') {
		error("You must use the ',' to divide it!", 40);
	}
}

// Precte vsechny stavy ze vstupu a ulozi je do fa
function read_states (options $options, finite_automaton $fa) {
	// Ctu vstupni stavy
	if ($options->get_char() != '{')		// Vzdy ohraniceny pomoci { }
		error("Set of states must star by '{'!", 40);
	
	// Nacitam jednotliva ID az po konec mnoziny
	$id = '';
	while (($tmp_char = $options->get_char()) != '}') {
		if(($tmp_char >= 'A' && $tmp_char <= 'Z') || ($tmp_char >= 'a' && $tmp_char <= 'z')) {
			$id .= $tmp_char;
		}
		// Cislo a podtrzitko nesmi byt na zacatku
		elseif (($tmp_char == '_'  || ($tmp_char >= '0' && $tmp_char <= '9')) && strlen($id)) {
			$id .= $tmp_char;
		}
		// Nacetl jsem cele ID-> ulozim ho a ctu dalsi
		elseif ($tmp_char == ',') {
			$fa->add_state(new state($id));
			$id = '';
		}
		else {
			error("State '$id' is invaid!", 40);
		}
	}
	// Ulozim posledni ID
	$fa->add_state(new state($id));

	read_comma($options);
}

// Precte abecedu ze vstupu a ulozi ji do fa
function read_alphabet (options $options, finite_automaton $fa) {
	if ($options->get_char() != '{')		// Vzdy ohraniceny pomoci { }
		error("Set of states must star by '{'!", 40);
	
	while (true) {
		// Znak je vzdy ohranicen uvozovkama
		$tmp_char = $options->get_char();
		if ($tmp_char != '\'')
			error("Invalid input alphabet!", 40);

		$tmp_char = $options->get_char(true);

		// NOTE Pridat kontrolu na nepovolene znaky?
		// Vyjimka pro ''''
		if ($tmp_char == '\'') {
			$tmp_char = $options->get_char(true);
			if ($tmp_char == '\'') {
				$tmp_char = $options->get_char(true);
				if ($tmp_char == '\'') {
					$fa->add_symbol($tmp_char);

					$tmp_char = $options->get_char(true);
					if ($tmp_char == '}')
						break;
					elseif ($tmp_char == ',')
						continue;
				}
					
				error("Invalid input alphabet!", 40);

			}
			elseif ($tmp_char == '}') {
				$fa->add_symbol('');
				break;
			}
			elseif ($tmp_char == ',') {
				$fa->add_symbol('');
				continue;
			}
			else {
			}
		}
		$fa->add_symbol($tmp_char);
		
		$tmp_char = $options->get_char(true);
		if ($tmp_char != '\'')
			error("Invalid input alphabet!", 40);

		$tmp_char = $options->get_char();
		if ($tmp_char == '}')
			break;
		elseif ($tmp_char != ',')
			error("Invalid input alphabet!", 40);
	}

	read_comma($options);
}

// Precte praidla ze vstupu a ulozi je do fa
function read_rules (options $options, finite_automaton $fa) {
	if ($options->get_char() != '{')		// Vzdy ohraniceny pomoci { }
		error("Set of rules must star by '{'!", 40);

	while (($tmp_char = $options->get_char()) != '}') {
		$found_epsilon = false;
		$found_arrow   = false;			

		// Nactu vychozi stav
		$start_state = '';
		while (1) {
			if(($tmp_char >= 'A' && $tmp_char <= 'Z') || ($tmp_char >= 'a' && $tmp_char <= 'z') || ($tmp_char >= '0' && $tmp_char <= '9') || $tmp_char == '_')
				$start_state .= $tmp_char;
			else
				break;
			$tmp_char = $options->get_char();
		}
		// Otestuju platnost ID
		$start_state = $fa->is_state($start_state);

		// Otestuju ' pro pristi znak
		if ($tmp_char != '\'' || !$start_state)
			error("Invalid set of rules!", 40);

		// Nactu a otestuju pismeno abecedy
		$input_char = $options->get_char(true);
		// Vyjimka por ''''
		if ($input_char == '\'') {
			$found_epsilon = true;
			$input_char = '';

			$tmp_char = $options->get_char(true);
			if ($tmp_char == '\'') {
				$tmp_char = $options->get_char(true);
				if ($tmp_char == '\'') {
					$input_char = '\'';
				}
			}
			elseif ($tmp_char == '-') {
				if ($options->get_char() != '>')
					error("Invalid set of rules!", 40);
				$found_arrow = true;			
			}
		}
		// Nejedna se o validni vstupni znak
		if (!$found_epsilon && !$fa->is_input_char($input_char))
			error("Invalid set of rules!", 41);
		// Nactu ' ->
		if (!$found_epsilon && $options->get_char(true) != '\'')
			error("Invalid set of rules!", 40);			
		if (!$found_arrow && $options->get_char() != '-')
			error("Invalid set of rules!", 40);			
		if (!$found_arrow && $options->get_char() != '>')
			error("Invalid set of rules!", 40);			
		
		// Nactu cilovy stav
		$final_state = '';
		$tmp_char = $options->get_char();
		while (1) {
			if(($tmp_char >= 'A' && $tmp_char <= 'Z') || ($tmp_char >= 'a' && $tmp_char <= 'z') || ($tmp_char >= '0' && $tmp_char <= '9') || $tmp_char == '_')
				$final_state .= $tmp_char;
			else
				break;
			$tmp_char = $options->get_char();
		}

		// Otestuju platnost ID
		$final_state = $fa->is_state($final_state);
		if (!$final_state)
			error("Invalid set of rules!", 41);
		// Otestuju , pro pristi znak
		if ($tmp_char != ',') {
			if ($tmp_char == '}') {
				$fa->add_rule(new rule($start_state, $final_state, $input_char));
				break;
			}
			else {
				error("Invalid set of rules!", 40);
			}
		}

		// Pridam pravidlo
		$fa->add_rule(new rule($start_state, $final_state, $input_char));
	}

	read_comma($options);
}

// Precte pocatecni stav
function read_start_state (options $options, finite_automaton $fa) {
	$id = '';
	while (($tmp_char = $options->get_char()) != ',') {
		if ($options->is_eof()) {
			error("invalid start states!", 40);
		}
		$id .= $tmp_char;
	}

	if (($tmp_state = $fa->is_state($id))) {
		$fa->set_start_state($tmp_state);
	}
	else
		error("Start state isn't in set of states!", 41);
}

// Precte koncove stavy
function read_final_states (options $options, finite_automaton $fa) {
	if ($options->get_char() != '{')		// Vzdy ohraniceny pomoci { }
		error("Set of final states must star by '{'!", 40);
	
	$id = '';
	while (($tmp_char = $options->get_char()) != '}') {
		if ($tmp_char == ',') {		// Nacetl jsem id -> porovnam a zaznacim, pokud neexistuje, koncim
			if (($tmp_state = $fa->is_state($id))) {
				$tmp_state->set_final();
				$id = '';
			}
			else
				error("Invalid set of final states - state '$id' not exist!", 41);
		}
		elseif ($options->is_eof()) {
			error("Invalid set of final states!", 40);
		}
		else {
			$id .= $tmp_char;
		}
	}
	
	if ($id != '') {	
		$not_found = true;
		foreach ($fa->get_states() as $state) {
			if ($state == $id) {
				$state->set_final();

				$not_found = false;
				break;
			}
		}
		if ($not_found)
			error("invalid set of final states - state '$id' not exist!", 41);
	}
}



/* * * * * * * * * * *
 * * DKA algoritmy * *
 * * * * * * * * * * */
function remove_e (finite_automaton &$fa) {
	// Vytvorim novy automat
	$new_fa = new finite_automaton();
	foreach ($fa->get_states() as $state)     { $new_fa->add_state($state); }
	foreach ($fa->get_alphabet() as $symbol)  { $new_fa->add_symbol($symbol); }
	$new_fa->set_start_state($fa->get_start_state());

	// Vytvorim e-uzavery
	foreach ($fa->get_states() as $state) {
		$e_closure = array($state);

		foreach ($fa->get_rules() as $rule) {
			if ($rule->start_state == $state && empty($rule->input_char) && !array_search($rule->final_state, $e_closure)) {
				if ($rule->final_state->is_final())
					$state->set_final();
				array_push($e_closure, $rule->final_state);
			}
		}

		foreach ($e_closure as $closure) {
			foreach ($fa->get_rules() as $rule) {
				if ($rule->start_state == $closure) {
					if (!empty($rule->input_char))
						$new_fa->add_rule(new rule($state, $rule->final_state, $rule->input_char));
				}
			}
		}
	}
	$fa = $new_fa;
}

function determinization (finite_automaton &$fa) {
	// Vytvorim novy automat
	$new_fa = new finite_automaton();
	$new_fa->add_state($fa->get_start_state());
	foreach ($fa->get_alphabet() as $symbol)  { $new_fa->add_symbol($symbol); }
	$new_fa->set_start_state($fa->get_start_state());

	// Postupne vytvorim novy fa
	$workspace = array ($new_fa->get_start_state());
	while ( count($workspace) ) {
		$state = array_shift($workspace);
		
		foreach ($fa->get_alphabet() as $symbol) {
			$is_final = false;
			$tmp_state = array ();
			foreach ($state->get_id() as $id) {

				foreach ($fa->get_rules() as $rule) {
					if ($rule->input_char == $symbol && $rule->start_state == $id && !in_array($rule->final_state->__toString(), $tmp_state)) {
						array_push($tmp_state, $rule->final_state->__toString());

						if (!$is_final)
							$is_final = $fa->is_final_state($rule->final_state);
					}
				}
			}

			// Ignoruji prazdny stav
			if(!empty($tmp_state)) {
				natsort($tmp_state);
				$is_state = $new_fa->is_state(implode("_",$tmp_state)); 
				
				// Pokud stav neexistuje -> vytvorim ho
				if (!$is_state) {
					$new_state = new state($tmp_state, $is_final);
					$new_fa->add_state($new_state);
					array_push($workspace, $new_state);
				}
				else
					$new_state = $is_state;
	
				// Vytvorim nove pravidlo
				$new_fa->add_rule(new rule($state, $new_state, $symbol));
			}	
		}	
	}

	$fa = $new_fa;
}

function analyze_string (finite_automaton &$fa, $test_string) {
	// Kontrolu zacinam na vstupnim stavu
	$actual = $fa->get_start_state();

	// Postupne ctu znaky z $test_string a hledam odpovidajici pravidla a stavy
	// Pokud nenajdu reseni -> konec
	for ( $i = 0 ; $i < strlen($test_string) ; $i++ ) {
		// Otestuji, zda je symbol ve vstupni abecede
		if (!$fa->is_input_char($test_string[$i])) { return "0"; }

		// Najdu pravidlo, ktere me z aktualniho stavu ($actual) pomoci symbolu $test_string[$i] presune dal
		$found = false;
		foreach ($fa->get_rules() as $rule) {
			echo "Testuji: " . $rule . "\n";
			// Toto pravidlo nelze pouzit -> zkusim dalsi
			if (($rule->start_state != $actual) || ($rule->input_char != $test_string[$i])) {
				continue;
			}
			else {
				// Mohu se posunout dal
				$actual = $rule->final_state;
				$found = true;
				break;
			}
		}

		// Nenasel jsem zadne vhodne pravidlo -> konec
		if (!$found)
			return "0";
	}

	// Provedu test na koncovy stav
	if (!$actual->is_final())
		return "0";
	
	return "1";
}
?>