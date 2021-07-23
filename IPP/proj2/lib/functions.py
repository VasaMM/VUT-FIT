# # # # # # # # # # # # # # # # # # # # 
# AUTOR: Vaclav Martinka (xmarti76)   #
# FIT VUTBR                           #
# 13. 04. 2017                        #
# # # # # # # # # # # # # # # # # # # # 

# knihovny
from lib.format import *
from lib.error import *
from lib.help import print_help

import sys         # argumenty
import re          # regex
import operator    # sorted


def my_open ( name, mode = 'r' ):
	"""new open"""
	if name is not None:
		try:
			return open( name , mode )
		except IOError:
			return None


class Options:
	format_file = None
	input_file  = None
	output_file = None
	make_br     = False
	escape      = False
	nooverlap   = False

	def open_files ( self ):
		self.format_file = my_open( self.format_file )
			
		if self.input_file is None:
			self.input_file = sys.stdin
		else:
			self.input_file  = my_open( self.input_file )
		
		if self.output_file is None:
			self.output_file = sys.stdout
		else:
			self.output_file = my_open( self.output_file, 'w' )

		if self.input_file is None:
			error( "Opening of input file was unsuccessful!", 2 )
		if self.output_file is None:
			error( "Creating of output file was unsuccessful!", 3 )


def read_args ( options ):
	"""This function read args and save it into options class"""

	if type( options ) is not Options:
		error( "Options is type " + str( type( options ) ) + ", bud excepted options" )

	if len(sys.argv) == 1:
		error("You must use some arguments! Use --help for help.")

	for arg in sys.argv:
		if arg == sys.argv[0]:
			continue

		elif arg == "--help":
			if len(sys.argv) != 2:
				error( "You can't combinate --help with some other argument!" )
			print_help()
			exit(0)

		elif arg == "--br":
			options.make_br = True
		elif arg == "--escape":
			options.escape = True
		elif arg == "--nooverlap":
			options.nooverlap = True

		elif re.match( "^--format=.+" , arg ) is not None:
			if options.format_file is not None:
				error( "You can't use more than one format file!" )
			options.format_file = arg[9:]	#substring

		elif re.match( "^--input=.+" , arg ) is not None:
			if options.input_file is not None:
				error( "You can't use more than one input file!" )
			options.input_file = arg[8:]	#substring

		elif re.match( "^--output=.+" , arg ) is not None:
			if options.output_file is not None:
				error( "You can't use more than one output file!" )
			options.output_file = arg[9:]	#substring
			
		else:
			error( "Bad argument '" + arg + "'! Use --help for help.")




def parse_format_file ( options ):
	"""Parse format file by defied rules."""

	if type( options ) is not Options:
		error( "Options is type " + str( type( options ) ) + ", bud excepted options", 100 )

	output = Format()
	for line in options.format_file:
		# Preskocim prazdny radek
		if line == "\n":
			continue
		# Nactu radek
		pattern = re.compile(r"^([^\t]+)(\t+)([^\n]+)$")
		matches = pattern.match( line )

		# Kontrola na spravnost vstupu
		if matches is None:
			return False
		
		# Ulozim si vstup do zaznamu
		if output.add_format( output.FormatRecord( matches.group(1), matches.group(3) ) ) != True:
			return False
	
	return output


def _replace_one_spec_regex ( format ):
	if re.search( r"%[^sadlLwWtn.|!*+()%]", format ):
		warning( "You can't escape " +  str( re.search( r"%[^.|!*+()%]", format ) ) )
		return False

	format = re.sub( r"!%a", r"",                   format )
	format = re.sub( r"!%([.|!*+()%])", r"[^\\\1]", format )

	format = re.sub( r"%s", r"[ \\t\\n\\r\\f\\v]",   format )
	format = re.sub( r"%a", r"(.)",                  format )
	format = re.sub( r"%d", r"[0-9]",                format )
	format = re.sub( r"%l", r"[a-z]",                format )
	format = re.sub( r"%L", r"[A-Z]",                format )
	format = re.sub( r"%w", r"[a-zA-Z]",             format )
	format = re.sub( r"%W", r"[a-zA-Z0-9]",          format )
	format = re.sub( r"%t", r"[\\t]",                format )
	format = re.sub( r"%n", r"[\\n]",                format )
	format = re.sub( r"%([.|!*+()%])", r"(\\\1)",    format )
	return format

def replace_spec_regex ( formats ):
	"""Replace specil regex from IFJ to Pyrhon"""
	if type( formats ) is not Format:
		error( "Formats is type " + str( type( formats ) ) + ", bud excepted format", 100 )

	for format in formats.get_formats():
		# Nahradim %x
		format_regex = _replace_one_spec_regex( format.regex() )
		if format_regex is False:
			return False

		# NQS rozsireni
		format_regex = re.sub( r"([^\\]|^)!!", r"\1", format_regex )
		format_regex = re.sub( r"([^\\]|^)\+{2,}", r"\1+", format_regex )
		format_regex = re.sub( r"([^\\]|^)\*{2,}", r"\1*", format_regex )
		format_regex = re.sub( r"([^\\]|^)[\+\*]{2,}", r"\1*", format_regex )
		
		# Kontrola pro nasobne spec. znaky
		if ( re.search( r"\.{2,}|\|{2,}", format_regex ) ):
			warning( "Some multiple spec. char! " + str( re.search( r"\.{2,}|\|{2,}", format_regex ) ) )
			return False

		# Kontrola pro osamocene spec. znaky
		if ( re.search( r"(^[\.\|])|([\.\|]$)", format_regex ) ):
			warning( "Some alone spec. char! " + str( re.search( r"(^[\.\|])|([\.\|]$)", format_regex ) ) )
			return False

		# Kontrola pro prefix spec. znaky
		if ( re.search( r"!$", format_regex ) ):
			warning( "The '!' need some char to negation! " + str( re.search( r"!$", format_regex ) ) )
			return False

		# Kontrola pro postfix spec. znaky
		if ( re.search( r"^[\*\+]", format_regex ) ):
			warning( "The '*' or '+' need some previous char! " + str( re.search( r"^[\*\+]", format_regex ) ) )
			return False
		
		# Kontrola na neprazdne zavorky zavorek 
		if ( re.search( r"([^\\]|^)\(\)", format_regex ) ):
			warning( "The brackets cann't be empty! " + str ( re.search( r"([^\\]|^)\(\)", format_regex ) ) )
			return False
		
		# Kontrola na neplatne kombinace 
		if ( re.search( r"(\.\|)|(\.\*)|(\.\+)|(\|\.)|(\!\|)|(\!\.)|(\!\*)|(\!\+)", format_regex ) ):
			warning( "This " + str( re.search( r"(\.\|)|(\.\*)|(\.\+)|(\|\.)|(\!\|)|(\!\.)|(\!\*)|(\!\+)", format_regex ) ) + " is invalid combination!" )
			return False

		# Nahradim A.B za AB
		format.regex( re.sub( r"([^\(])\.([^\)])", r"\1\2", format_regex ) )



def replace_negation ( formats ):
	"""Replace negation"""
	if type( formats ) is not Format:
		error( "Formats is type " + str( type( formats ) ) + ", bud excepted Format", 100 )

	for format in formats.get_formats():
		# Vice jak jedna negace neni povolena
		if ( re.search( "!{2,}", format.regex() ) ):
			return False
		
		format.regex( re.sub( r"([^\\]|^)(!)\[(.+)\]", r"\1[^\3]", format.regex() ) )
		format.regex( re.sub( r"([^\\]|^)(!)(.)",      r"\1[^\3]", format.regex() ) )


def escape_dangerous_chars (  formats ):
	"""Escape dangerous chars"""
	if type( formats ) is not Format:
		error( "Formats is type " + str( type( formats ) ) + ", bud excepted format", 100 )

	for format in formats.get_formats():
		format.regex( re.sub( r"([\^\$\?\{\}\[\]\\])", r"\\\1", format.regex() ) )



def formating_text ( options, formats, text ):
	if type( options ) is not Options:
		error( "Options is type " + str( type( options ) ) + ", bud excepted options", 100 )
	if type( formats ) is not Format:
		error( "Formats is type " + str( type( formats ) ) + ", bud excepted format", 100 )
	if type( text ) is not str:
		error( "Text is type " + str( type( text ) ) + ", bud excepted string", 100 )

	formating = list()
	tmp = None
	

	for format in formats.get_formats():
		positions = list()
		
		# Najdu vsechny retezce, ktere odpovidaji regexu a ulozim si jejich pozici
		try:
			tmp = re.finditer( format.regex(), text, re.S )
		except:
			return False

		for pos in tmp:
			if pos.start() != pos.end():
				positions.append( ( pos.start(), pos.end() ) )

		# Ulozim si k aktualnimu formatu vsechny pozice, kde se ma aplikovat
		formating.append( ( format.formating(), positions ) )

	# Prevedu vstupni text na seznam ve tvaru ( 'tag pred znakem', 'znak', 'tag za znakem' )
	tmp = list( text )
	output = list()
	for char in tmp:
		output.append( ("", char, "") )

	# Aplikuji formaty
	for format in formating:
		for pos in format[1]:
			for tag in format[0]:
				output[ pos[0]   ] = ( output[ pos[0]   ][0] + tag.tag(0), output[ pos[0]   ][1],              output[ pos[0]   ][2] )
				output[ pos[1]-1 ] = ( output[ pos[1]-1 ][0],              output[ pos[1]-1 ][1], tag.tag(1) + output[ pos[1]-1 ][2] )

	# Vytvorim string z listu a vratim ho
	tmp = output
	output = ""
	for char in tmp:
		new_char = char[1]
		# Escape
		if options.escape:
			if new_char == '&':
				new_char = "&amp;"
			elif new_char == '<':
				new_char = "&lt;"
			elif new_char == '>':
				new_char = "&gt;"

		output += char[0] + new_char + char[2]

	return output
