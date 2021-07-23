# # # # # # # # # # # # # # # # # # # # 
# AUTOR: Vaclav Martinka (xmarti76)   #
# FIT VUTBR                           #
# 13. 04. 2017                        #
# # # # # # # # # # # # # # # # # # # # 

def print_help ():
	print ( "\nThis is very simple help for SYN script. For more informations, read doc.pdf\n\n",
	        "You can use these parameters:\n",
	        "\t--format=<name>     for set format file. If you didn't set it or if file is unreadable, so the input file is copied to the output only.\n",
	        "\t--input=<name>      for set input file. If you didn't set it, it'll be used stdin\n",
	        "\t--output=<name>     for set output file. If you didn't set it, it'll be used stdout\n",
	        "\t--br                Make <br /> at the end of each row.\n",
	        "\t--escape            escape <> and & into html.\n",
	        "\nThis parameters aren't implemented:\n",
	        "\t--nooverlap\n\n\n",
	        "AUTHOR: xmarti76\t11. 04. 2017\tVUT FIT\n\n" )
