<?php
/* * * * * * * * * *
 * AUTOR: xmarti76
 * DKA - projekt do IPP
 * 15. 03. 207
 * * * * * * * * * */

// Options
class options {
	public $input;
	public $output;
	public $no_epsilon_rules;
	public $determinization;
	public $case_insensitive;
	public $white_char;
	public $rules_only;
	public $analyze_string;
	public $wsfa;

	private $file_stack;

	public function init_input  ($in = 'php://stdin')    { $this->input  = fopen($in,  'r'); }
	public function init_output ($out = 'php://stdout')  { $this->output = fopen($out, 'w'); }

	public function is_eof()  { return (mb_strlen($this->file_stack) == 0) && feof($this->input); }

	// Vrati prvni platny znak
	// FIXME EOF
	public function get_char ($only_get = false) {
		// Naplni zasobnik, kdyz je potreba
		if (mb_strlen($this->file_stack) == 0)   {
			$this->file_stack = fgets($this->input);
			if ($this->is_eof()) { return ''; };
			// Opravim konce radku
			$this->file_stack = str_replace("\r\n", "\n", $this->file_stack);
			$this->file_stack = str_replace("\r",   "\n", $this->file_stack);
		}
		// Prevedu na mala pismena, je-li potreba
		if ($this->case_insensitive)
			$tmp_char = mb_strtolower(mb_substr($this->file_stack,0, 1));
		else
			$tmp_char = mb_substr($this->file_stack,0, 1);

		// Posunu se ve stringu
		$this->file_stack = mb_substr($this->file_stack, 1);

		// Nechci ignorovat bile znaky
		if ($only_get) { return $tmp_char; }

		if (ctype_space($tmp_char)) {					// Ignoruji bile znaky
			$tmp_char = $this->get_char();
		}
		elseif ($tmp_char == '#') {						// Komentar
			$this->file_stack = fgets($this->input);	//Muzu nacist dalsi radek
			$tmp_char = $this->get_char();
		}
		else {
			return $tmp_char;
		}

		return $tmp_char;
	}

	// Vypise veskere nastaveni
	public function __toString () {
		echo "Options:\n";
		foreach ($this as $key => $value) {
			echo "\t$key:\t$value\n";
		}
	}
}


// Konecny automat
class finite_automaton {
	private $set_of_states = array();
	private $alphabet = array();		// libovolny znak uzavreny v apostrofech a [(){}''->,.# \n\t], case-sensitive, '''' = ', '' = epsilon
	private $set_of_rules = array();
	private $start_state;


	// Metody pro vstupni abecedu
	public function get_alphabet () { return $this->alphabet; }

	public function add_symbol ($symbol) {
		if (!$this->is_symbol($symbol))
			array_push($this->alphabet, $symbol);
	}

	public function is_symbol ($symbol) {
		foreach ($this->get_alphabet() as $one_symbol) {
			if ($symbol == $one_symbol)
				return true;
		}
		return false;
	}

	// Metody pro stavy
	public function get_states       ()              { return $this->set_of_states; }
	public function get_start_state  ()              { return $this->start_state; }
	public function add_state        (state $state)  { array_push($this->set_of_states, $state); }
	public function set_start_state  (state $state)  { $this->start_state = $state; }

	public function is_state ($id) {
		foreach ($this->get_states() as $state) {
			switch (gettype($id)) {
				case 'object':
					if (get_class($id) == 'state' && $state->get_id() == $id)
						return $state;

				case 'string':
					if ($state == $id)
						return $state;					
			}
		}
		return false;
	}

	public function is_final_state ($id) {
		foreach ($this->get_states() as $state) {
			switch (gettype($id)) {
				case 'object':
					if (get_class($id) == 'state' && $state->get_id() == $id)
						return $state->is_final();

				case 'string':
					if ($state == $id)
						return $state->is_final();
			}
		}
		return false;	
	}
	public function is_input_char ($id) {
		foreach ($this->get_alphabet() as $input_char) {
			if ($input_char == $id)
				return true;
		}
		return false;
	}

	// Metody pro pravidla
	public function get_rules ()            { return $this->set_of_rules; }
	public function add_rule  (rule $rule)  { array_push($this->set_of_rules, $rule); }

	// Vypise cely koncovy automat (lze vyuzit napr. v echo)
	function __toString () {
		natsort($this->set_of_states);
		natsort($this->set_of_rules);
		natsort($this->alphabet);
		$i = $count = 0;

		$output = "(\n{";
		// Vypisu stavy
		$count = count($this->get_states());
		foreach ($this->get_states() as $state) {
			if ($i++)
				$output .= ", $state";
			else
				$output .= "$state";
		}
		$output .= "},\n{";
		// Vypisu vstupni abecedu
		$i = 0;
		$count = count($this->get_alphabet());
		foreach ($this->get_alphabet() as $value) {
			if ($value == '\'')
				$value = '\'\'';

			if ($i++)
				$output .= ", '$value'";
			else
				$output .= "'$value'";
		}
		$output .= "},\n{\n";
		// Vypisu pravidla
		// ktere maji jako vstupni symbol ' ' jsu spatne serazeny -> natsort() zrejme obsahuje chybu...
		$i = 0;
		$count = count($this->get_rules());
		foreach ($this->get_rules() as $rule) {
			if ($i++)
				$output .= ",\n" . $rule;
			else
				$output .= $rule;
		}
		// Vypisu startovni stav
		if ($count == 0)		// FIX 0 pravidel
			$output .= "},\n" . $this->get_start_state() . ",\n{";
		else
			$output .= "\n},\n" . $this->get_start_state() . ",\n{";
		// Vypisu koncove stavy
		$i = 0;
		$count = count($this->get_states());
		foreach ($this->get_states() as $state) {
			if ($state->is_final()) {
				if ($i++) 
					$output .= ", $state";
				else
					$output .= "$state";
			}
		}

		return $output . "}\n)\n";
	}
}

// Konecna mnozina pravidel
class rule {
	public $start_state;
	public $final_state;
	public $input_char;

	function __construct (state $start_state, state $final_state, $input_char) {
		$this->start_state = $start_state;
		$this->final_state = $final_state;
		$this->input_char  = $input_char;
	}

	function __toString () {
		$input_char = $this->input_char;
		if ($input_char == '\'')
			$input_char = '\'\'';
		return "$this->start_state '$input_char' -> $this->final_state";
	}
}

// Jeden stav
class state {
	private $id = array ();    // [a-Z][0-9][_], nezacina ani nekonci '_' a nezacina cislem
	private $is_final;

	function __construct ( $string, $final = false) {		// Ocekavam validni id
		$this->add_id($string);
		$this->is_final = $final;
	}

	public function set_final ( $final = true)  { $this->is_final = $final; }
	public function is_final  ()                { return $this->is_final; }

	public function get_id    ()  { return $this->id; }
	public function add_id    ($string) {
		switch (gettype($string)) {
			case 'string':
				array_push($this->id, $string);
				break;
			
			case 'array':
				foreach ($string as $one_string) {
					$this->add_id($one_string);
				}
				break;

			default:
				error("Bad type of state id: " . gettype($string), 101);
		}
		natsort($this->id);
	}

	function __toString () {
		return implode("_", $this->id);
	}
}


?>