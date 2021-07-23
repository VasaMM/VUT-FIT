<?php
/* * * * * * * * * *
 * AUTOR: xmarti76
 * DKA - projekt do IPP
 * 15. 03. 207
 * * * * * * * * * */

function read_args($argv, $options) {
	if (count($argv) > 1) {
		unset($argv[0]);	// Ignoruji nazev souboru
		foreach ($argv as $parametr) {
			if ($parametr[0] != '-') error("Bad parametr '$parametr'!", 1);

			switch ($parametr) {
				case '--help':
					if (count($argv) != 1) error("Only '--help' parametr is allowed!", 1);

					echo "\nThis is very simple help for DKA script. For more informations, read doc.pdf\n\n".
					     "You can use these parameters:\n".
					     "\t--input=<name>              for set input file. If you don't set it, it'll be used stdin\n".
					     "\t--output=<name>             for set output file. If you don't set it, it'll be used stdout\n".
					     "\t-e, --no-epsilon-rules      Remove e-rules, you can't use with determinization!\n".
					     "\t-d, --determinization       Do determinization, you can't use with removing e-rules!\n".
					     "\t-i, --case-insensitive      Script will work case insensitive\n".
					     "\t--analyze-string=<string>   Analyze if <string> is string of finite automaton\n".
					     "\nThis parameters aren't implemented:\n".
					     "\t-w, -white-char\n".
					     "\t-r, -rules-only\n".
					     "\t-wsfa\n\n\n".
					     "AUTHOR: xmarti76\t15. 03. 2017\tVUT FIT\n\n";
					exit(0);

				case (preg_match('/^--input=.*/', $parametr) ? true : false):
					if (!$options->input) {
						$options->init_input(substr($parametr, 8));
						if (!$options->input) error("Opening the file '" . substr($parametr, 8) . "' was unsuccesful!", 2);
					}
					else 
						error("You can use only one input!", 1);
					break;

				case (preg_match('/^--output=.*/', $parametr) ? true : false):
					if (!$options->output) {
						$options->init_output(substr($parametr, 9));
						if (!$options->output) error("Opening the file '" . substr($parametr, 9) . "' was unsuccesful!", 3);
					}
					else
						error("You can use only one output!", 1);
					break;

				case '-e':
				case '--no-epsilon-rules':
					if ($options->no_epsilon_rules) { error("You can use only once '--no-epsiolon-rules' or '-e'!", 1); }
					if ($options->determinization) error("You can't use '--no-epsiolon-rules' with '--determinization'!", 1);
					$options->no_epsilon_rules = true;
					break;

				case '-d':
				case '--determinization':
					if ($options->determinization) { error("You can use only once '--determinization' or '-d'!", 1); }
					if ($options->no_epsilon_rules) error("You can't use '--determinization' with '--no-epsiolon-rules'!", 1);
					$options->determinization = true;
					break;

				case '-i':
				case '--case-insensitive':
					if ($options->case_insensitive) { error("You can use only once '--case-insensitive' or '-i'!", 1); }
					$options->case_insensitive = true;
					break;

				case '-w':
				case '--white-char':
					if ($options->white_char) { error("You can use only once '--white-char' or '-w'!", 1); }
					$options->white_char = true;
					break;

				case '-r':
				case '--rules-only':
					if ($options->rules_only) { error("You can use only once '--rules-only' or '-r'!", 1); }
					$options->rules_only = true;
					break;

				case (preg_match('/^--analyze-string=".*"/', $parametr) ? true : false):
					if ($options->no_epsilon_rules || $options->determinization) { error("You can't use determinization or no_epsilon_rules with analyze-string!", 1); }
					if (strlen($options->analyze_string)) { error("You can use only once '--analyze-string'!", 1); }
					$options->analyze_string = substr($parametr, 18, strlen($parametr) - 19);
					break;

				case '-wsfa':
					if ($options->wsfa) { error("You can use only once '--wsfa'!", 1); }
					$options->wsfa = true;
					break;

				default:
					error("Some error in read args!", 1);
			}
		}
	}
	
	if (!$options->input)  $options->init_input();
	if (!$options->output) $options->init_output();
}

?>
