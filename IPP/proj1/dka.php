<?php
/* * * * * * * * * *
 * AUTOR: xmarti76
 * DKA - projekt do IPP
 * 15. 03. 207
 * * * * * * * * * */

	
	/* * * * * *
	NOTE zname chyby:

		# Test05: spatne razeni ' ' a '#'

	* * */


	// Include source codes
	include('classes.php');
	include('functions.php');
	include('read_args.php');

	// Make objekt of options
	$options = new options();
	// Read args
	read_args($argv, $options);

	// Make finite automaton
	$finite_automaton = new finite_automaton();

	// Nactu prvni (
	if ($options->get_char() != '(')		// getchar != get_char !!!
		error("Finite automaton must star by '('!", 40);

	// Prectu veschny stavy
	read_states($options, $finite_automaton);
	// Nactu vstupni abecedu
	read_alphabet($options, $finite_automaton);
	// Nactu pravidla
	read_rules($options, $finite_automaton);
	// Nactu startovni stav
	read_start_state($options, $finite_automaton);
	// Nactu koncove stavy
	read_final_states($options, $finite_automaton);

	// Nactu koncovou )
	if ($options->get_char() != ')')		// getchar != get_char !!!
		error("Finite automaton must end by ')'!", 40);

	// Detekce "smeti"
	$tmp_char = '';
	if (($tmp_char = $options->get_char()) != '' || !$options->is_eof())
		error("Some other chars after end of finite automaton ($tmp_char)!", 40);

	// Provedu odstraneni e-prechodu
	if ($options->no_epsilon_rules) {
		remove_e($finite_automaton);
	}

	// Provedu determinizaci
	if ($options->determinization) {
		remove_e($finite_automaton);
		determinization($finite_automaton);
	}

	if (strlen($options->analyze_string)) {
		determinization($finite_automaton);
		$finite_automaton = analyze_string($finite_automaton, $options->analyze_string);
	}

	fwrite($options->output, $finite_automaton);
?>