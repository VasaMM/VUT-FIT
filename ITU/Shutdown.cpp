#include "MainWindow.h"

void ITU::Shutdown::run () {
	switch ( action ) {
		case SHUTDOWN: shutdown();
		case RESTART:  restart();
		case SLEEP:    sleep();
		case LOGOUT:   logout();
	}
}


System::String^ ITU::Shutdown::getName () {
	switch ( action ) {
		case RESTART:  return "Restartovat";
		case SLEEP:    return "Uspat";
		case LOGOUT:   return "Odhlasit";
		default:
		case SHUTDOWN: return "Vypnout";
	}
}


System::Drawing::Image^ ITU::Shutdown::getImage ( System::ComponentModel::ComponentResourceManager^ images ) {
	switch ( action ) {
		case RESTART:  return ( cli::safe_cast< System::Drawing::Image^ >( images->GetObject( L"restart.Image48"  ) ) );
		case SLEEP:    return ( cli::safe_cast< System::Drawing::Image^ >( images->GetObject( L"sleep.Image48"    ) ) );
		case LOGOUT:   return ( cli::safe_cast< System::Drawing::Image^ >( images->GetObject( L"logout.Image48"   ) ) );
		default:
		case SHUTDOWN: return ( cli::safe_cast< System::Drawing::Image^ >( images->GetObject( L"shutdown.Image48" ) ) );
	}
}