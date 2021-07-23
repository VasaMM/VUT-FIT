# # # # # # # # # # # # # # # # # # # # 
# AUTOR: Vaclav Martinka (xmarti76)   #
# FIT VUTBR                           #
# 13. 04. 2017                        #
# # # # # # # # # # # # # # # # # # # # 

from lib.format import *
from lib.functions import *
from lib.error import *

# Nactu si argumenty do options
options = Options()
read_args( options )
options.open_files()

# Parsuji formatovaci soubor
if options.format_file is None:
	# Nemam zadany formatovaci soubor -> konec
	output = options.input_file.read()
	# Escape
	output = re.sub( "&", "&amp;", output )
	output = re.sub( "<", "&lt;", output )
	output = re.sub( ">", "&gt;", output )
	options.output_file.write( output )
	
	exit(0)

format_file = parse_format_file( options )


# Nekde byla chyba -> konec
if format_file is False:
	error( "Bad format file!", 4 )

if escape_dangerous_chars ( format_file ) is False:
	error( "Bad format file!", 4 )

if replace_spec_regex( format_file ) is False:
	error( "Bad format file!", 4 )

if replace_negation( format_file ) is False:
	error( "Bad format file!", 4 )


output = formating_text ( options, format_file, options.input_file.read() )
if output is False:
	error( "Bad format file!", 4 )
elif output is not None:
	if options.make_br is True:
		output =  re.sub( r"\n", "<br />\n", output )
	
	options.output_file.write( output )


