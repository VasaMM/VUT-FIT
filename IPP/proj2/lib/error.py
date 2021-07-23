# # # # # # # # # # # # # # # # # # # # 
# AUTOR: Vaclav Martinka (xmarti76)   #
# FIT VUTBR                           #
# 13. 04. 2017                        #
# # # # # # # # # # # # # # # # # # # # 

import sys

def error ( text = "Some error...", return_code = 1 ):
	"""Print error and exit porgram"""
	print( "ERROR: " + text, file=sys.stderr )
	exit( return_code )

def warning ( text = "Some warning..." ):
	"""Print error and exit porgram"""
	print( "WARNING: " + text, file=sys.stderr )
