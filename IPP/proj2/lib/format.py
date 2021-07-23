# # # # # # # # # # # # # # # # # # # # 
# AUTOR: Vaclav Martinka (xmarti76)   #
# FIT VUTBR                           #
# 13. 04. 2017                        #
# # # # # # # # # # # # # # # # # # # # 


import re	# regex
from enum import Enum

from lib.error import *


class Format:
	# Podtrida pro jednotlive zaznamy
	class FormatRecord:
		# Minitrida s konkretnim html tagem
		class HtmlTag:
			class HtmlTagType( Enum ):
				BOLD      = ( "<b>", "</b>" )
				ITALIC    = ( "<i>", "</i>" )
				UNDERLINE = ( "<u>", "</u>" )
				TELETYPE  = ( "<tt>", "</tt>" )
				SIZE      = ( "<font size=@>", "</font>" )
				COLOR     = ( "<font color=#@>", "</font>" )
			_type  = None
			_value = None
			def __init__ ( self, type_, value_ = None ):
				if type( type_ ) is not self.HtmlTagType:
					error( "Type_ is type " + str( type( type_ ) ) + ", bud excepted HtmlTagType", 100 )
				self._type = type_
				if value_ is not None:
					self._value = value_
			def tag ( self, pos ):		# pos = <0,1>
				if pos == 0:
					if self._value is not None:
						return re.sub( "@", self._value, self._type.value[0] )
					else:
						return self._type.value[0]
				elif pos == 1:
					return self._type.value[1]
				else:
					error( "Tag() acept only 0 and 1!", 100 )
				
		_regex_data  =  None
		_format_data = ()

		def __init__ ( self, r_data = None, f_data = None ):
			if r_data is not None:
				if self.regex( r_data ) != True:
					self.regex( False )
			if f_data is not None:
				if self.formating( f_data ) != True:
					self.regex( False )


		def regex ( self, data = None ):
			if data is None:
				return self._regex_data

			# TODO pridat kontrolu a vracet True/False
			self._regex_data = data
			return True

		def formating ( self, data = None ):
			if data is None:
				return self._format_data
			# Kontrolni regex
			pattern = r"(bold|italic|underline|teletype|size:[1-7]|color:[a-fA-F0-9]{6})"
			if re.match( r"(" + pattern + ",[ \t]*)*" + pattern + "+$", data ) is None:
				return False
			else:
				start  = ""
				end    = ""
				formating = list()

				for rule in re.findall( pattern, data ):
					if rule == "bold":
						start += "<b>"
						end    = "</b>" + end
						formating.append( self.HtmlTag( self.HtmlTag.HtmlTagType.BOLD ) )
					elif rule == "italic":
						start += "<i>"
						end    = "</i>" + end
						formating.append( self.HtmlTag( self.HtmlTag.HtmlTagType.ITALIC ) )
					elif rule == "underline":
						start += "<u>"
						end    = "</u>" + end
						formating.append( self.HtmlTag( self.HtmlTag.HtmlTagType.UNDERLINE ) )
					elif rule == "teletype":
						start += "<tt>"
						end    = "</tt>" + end
						formating.append( self.HtmlTag( self.HtmlTag.HtmlTagType.TELETYPE ) )
					elif rule[0 : 5] == "size:":
						start += "<font size=" + rule[ 5 : ] + ">"
						end    = "</font>" + end
						formating.append( self.HtmlTag( self.HtmlTag.HtmlTagType.SIZE, rule[ 5 : ] ) )
					elif rule[0 : 6] == "color:":
						start += "<font color=#" + rule[ 6 : ] + ">"
						end    = "</font>" + end					
						formating.append( self.HtmlTag( self.HtmlTag.HtmlTagType.COLOR, rule[ 6 : ] ) )
					else:
						return False


				if len(self._format_data) == 0:
					self._format_data = formating 
				else:
					self._format_data += formating 
				
				return True


	# Pole formatu
	_formats = []

	def add_format ( self, new_format ):
		if new_format.regex() is False or new_format.formating() is False:
			return False
		
		self._formats.append( new_format )
		return True
	
	def get_formats ( self ):
		return self._formats
