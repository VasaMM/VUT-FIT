#pragma once

#include <stdlib.h>

namespace ITU {

class Shutdown {
public:
	// Mozne akce a promenna pro jeji ulozeni
	enum actionType { SHUTDOWN, SLEEP, RESTART, LOGOUT };
	actionType action = SHUTDOWN;

	void run  ();
	void stop () { system( "shutdown /a" ); }
	System::String^ getName ();
	System::Drawing::Image^ getImage ( System::ComponentModel::ComponentResourceManager^ images );	//FIXME

private:
	// Funkce pro jednotlive akce
	void shutdown  () { system( "shutdown /s /f /t 0" ); }
	void restart   () { system( "shutdown /r /f /t 0" ); }
	void logout    () { system( "shutdown /l /t 0" ); }
	void sleep     () {;}
	// void hybernate () { system( "shutdown /h /t 0" ); }
};

#ifndef SHUTDOWNMEMBER
#define SHUTDOWNMEMBER
#endif // !SHUTDOWNMEMBER


}