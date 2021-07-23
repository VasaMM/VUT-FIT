/* * * * * * * * * * * * * * * * * * *
 * AUTOR: Vaclav Martinka (xmarti76)
 * POP3 klient
 * 17. 11. 2017 FIT VUT
 * * * * * * * * * * * * * * * * * * */

#include <fstream>
#include <iostream>
#include <string>
#include <regex>


#include "client.hpp"
#include "functions.hpp"
#include "options.hpp"
#include "error.hpp"

using namespace std;

string get_messages ( Options &, Client &, string &list, int messages_on_server );

int main ( int argc, char *argv[] ) {
	say( "SPOUSTIM SE" );

	Options options;

	// Nactu argumenty
	say( "Parsuji argumenty" );
	read_args( options, argc, argv );

	// Prectu si soubor pro autentizaci
	say( "Parsuji autentizacni soubor ", "" );
	if ( ! options.parse_user_file() ) {
		exit_error ( "Authentication file '" + options.authentication() + "' not exist!");
	}
	if ( options.name().empty() || options.password().empty() ) {
		exit_error ( "Authentication file '" + options.authentication() + "' isn't valid!");
	}

	// Inicializuji klienta
	Client client( &options );
	client.connect_server();

	// Prihlasim se
	if ( ! client.log_in() ) {
		client.disconnect_server();
		exit_error( "Couldn't log in to server. Maybe bad username or password?" );
	}

	// Zjistim pocet zprav na serveru
	string answer;
	int messages_on_server = client.statistics( answer );
	if ( messages_on_server < 0 ) {
		exit_error( "Server didn't return the number of messages." );
	}
	// Na serveru nejsou zadne zpravy
	else if ( messages_on_server == 0 ) {
		out ( string( options.only_new() ? "No new" : "No" ) + string( " messages on server." ) );
		return 0;
	}

	// Ziskam seznam zprav a jejich velikost
	string list;
	if ( ! client.list( list ) ) { exit_error( "Server didn't return the list of messages." ); }
	list.erase( list.end() - 3, list.end() );	// Odstranim konec ( .\n )

	// Ve smycce stahnu zpravy a ulozim je do souboru
	// Na zaver vypisu pocet stazenych zprav
	out ( get_messages( options, client, list, messages_on_server ) );

	// Odhlasim se
	client.disconnect_server();

	return 0;
}


string get_messages ( Options & options, Client &client, string &list, int messages_on_server ) {
	unsigned int i, index_size = 0;
	string answer, number, previous_lastID, lastID = "", file_time, file_name;
	smatch match;

	// Zjistim, zda stahuji jen nove zpravy
	if( options.only_new() ) {
		// Pokusim se nacist ID posledni zpravy ze souboru
		ifstream lastID_file( options.out_dir() + "last.ini" );
		if ( lastID_file.is_open() ) {
			getline( lastID_file, previous_lastID );
			say( "ID posledni zpravy: " + previous_lastID );
		}
		// Pokud neexistuje nevadi, stahnu vsechny
		else {
			print_error( "Unable to find the last download of the message (the file does not exist)." );
			print_error( "All messages will be downloaded." );
		}
	}

	// Abych mohl stahovat opravdu jen nove, musim postupovat od konce
	for ( i = messages_on_server ; i > 0 ; --i ) {
		// Posunu se na predchozi radek v seznamu
		size_t endline = list.find_last_of( "\n" );

		if ( endline != string::npos ) {
			list.erase( endline - 1 );	// Odstranim konec
			regex msg_size ( "(\\d+) (\\d+)$" );
	
			// Regexem vyhledam index a velikost zpravy
			if ( regex_search ( list, match, msg_size ) ) {
				// Zkontroluji, zda stahuji spravnou zpravu
				number = match[ 1 ];
				if ( str_to_uint( number, index_size ) ) {
					if ( index_size != i ) {
						continue;
						exit_error( "Server returned bad message (indices are different)." );
					}
				}
				else {
					continue;
					exit_error( "Indice of message isn't number." );
				}

				// Prectu zpravu
				number = match[ 2 ];
				if ( str_to_uint( number, index_size ) ) {
					if ( ! client.read_message( answer, i, index_size ) ) {
						continue;
						exit_error( "Couldn't read message." );
					}
				}
				else {
					continue;
					exit_error( "Size of message isn't number." );
				}
			}
			else {
				continue;
				exit_error( "Bad regex." );
			}
		}
		else {
			continue;
			exit_error( "Missing EOF" );
		}

		// Zjistim ID zpravy
		regex id ( "Message-I[dD]: (.+)" );
			
		if ( regex_search ( answer, match, id ) ) {
			// Zajima me pouze prvni (respektive posledni)
			if( lastID.empty() ) { lastID = match[ 1 ]; }

			// Zjistim, zda stahuji jen nove zpravy a nemam uz nahodou zkoncit
			if ( options.only_new() && previous_lastID == match[ 1 ] ) { break; }
		}


		// Z hlavicky ziskam datum, cas a odesilatele a vytvorim nazev souboru
		// yyyy-MMM-dd_hh-mm-ss_from.imf
		// Nactu datum a cas odeslani
		regex date ( "Date: ([A-Z][a-z]{2}, )?(\\d?\\d) ([A-Z][a-z]{2}) (\\d{4}) (\\d?\\d):(\\d?\\d):(\\d?\\d) (\\+|-)\\d{4}.*" );
		if ( regex_search ( answer, match, date ) ) {
			file_time = match[ 4 ];
			file_time += "-";
			file_time += MMM2mm( match[ 3 ] );
			file_time += "-";
			if( match[ 2 ].length() == 1 ) { file_time += "0"; }
			file_time += match[ 2 ];
			file_time += "_";
			if( match[ 5 ].length() == 1 ) { file_time += "0"; }
			file_time += match[ 4 ];
			file_time += "-";
			if( match[ 6 ].length() == 1 ) { file_time += "0"; }
			file_time += match[ 5 ];
			file_time += "-";
			if( match[ 7 ].length() == 1 ) { file_time += "0"; }
			file_time += match[ 6 ];
		}
		// Pokud jsem cas nenasel, pouziju posledni
		else if ( file_time.empty() ) {
			file_time = "0000-000-00_00-00-00";
		}
		
		// Lepsi by byl subject, ale je to moc komplikovane...
		// regex from ( "From:.*<(.+)>" );
		regex from ( "From: (.* )?<?([a-zA-Z0-9_.\\+\\-]+@[a-zA-Z0-9\\-]+\\.[a-zA-Z0-9\\-\\.]+)>?" );
		if ( regex_search ( answer, match, from ) ) {
			file_name = file_time + "_";
			file_name += match[ 2 ];

			size_t pos = file_name.find_first_of( '<' );
			if ( pos != string::npos ) { file_name.erase( pos, 2 ); }
			pos = file_name.find_first_of( '>' );
			if ( pos != string::npos ) { file_name.erase( pos, 2 ); }
		}
		// Nepodarilo se najit odesilatele
		else { file_name = file_time + "_No-sender"; }

		// Otestuji, zda zprava jiz neexistuje
		if ( is_file( options.out_dir() + file_name + ".imf" ) ) {
			int exist = 0;
			while ( true ) {
				// Najdu takovy nazev, ktery jeste neexistuje
				if ( ! is_file( options.out_dir() + file_name + "_" + to_string( ++exist ) + ".imf" ) ) {
					break;
				}

			}
			file_name += "_";
			file_name += to_string( exist );
		}
		
		// Vytvorim soubor
		ofstream output_file( options.out_dir() + file_name + ".imf" );
		if ( output_file.is_open() ) {
			output_file << answer;
			output_file.close();
		}
		else { print_error( "Couldn't create file '" + file_name + ".imf'." ); }

		// Vse se povedlo, muzu zpravu smazat
		if ( options.delete_it() ) {
			if ( ! client.delete_message( answer, i ) ) { print_error( "Couldn't delete message." ); }
		}

		draw_progress( messages_on_server - i + 1, messages_on_server );
	}

	// Ulozim si posledni stazenou zpravu do souboru
	ofstream output_file( options.out_dir() + "last.ini" );
	if ( output_file.is_open() ) {
		output_file << lastID;
		output_file.close();
	}
	else { print_error( "Couldn't create file 'last.ini'." ); }

	return (
		options.only_new() ? to_string( messages_on_server - i ) + " new" : to_string( messages_on_server ) ) +
		( messages_on_server == 1 ? " message" : " messages" ) + " have been downloaded" + 
		( options.delete_it() ? " and deleted." : "." );
}
