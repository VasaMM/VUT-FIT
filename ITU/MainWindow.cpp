#include "SelectAction.h"
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;


Void ITU::MainWindow::halfSecondTimer_Tick( Object^ sender, EventArgs^ e ) {
	// Aktualizuji cas
	updateActualTime();

	// Blikam :
	if ( this->actualTime_splitter->ForeColor == SystemColors::WindowText ) {
		this->actualTime_splitter->ForeColor = SystemColors::GrayText;
	}
	else {
		this->actualTime_splitter->ForeColor = SystemColors::WindowText;
	}

	// Prekreslim progressbar
	if ( this->shutdownTimer->Enabled ) {
		// Spocitam minuty do konce
		int toEndSec = offHour - DateTime::Now.Hour;
		if ( toEndSec < 0 )	{	// K vypnuti dojde az zitra
			toEndSec += 24;
		}

		toEndSec *= 60;	// Prevedu na minuty
		toEndSec += offMinute - DateTime::Now.Minute;	// Prictu minuty
		toEndSec *= 60;	// Prevedu na secundy
		toEndSec -= DateTime::Now.Second;	// Prictu minuty

		auto percent = ( ( this->shutdownTimer->Interval - shutdownTime->Subtract( DateTime::Now ).TotalMilliseconds ) / this->shutdownTimer->Interval ) * 100;
		this->statusBar_progress->Value = Convert::ToInt32( percent );
	}
}


void ITU::MainWindow::alignOffBoxes() {
	this->offTime_hour->SelectAll();
	this->offTime_hour->SelectionAlignment = HorizontalAlignment::Right;
	this->offTime_hour->DeselectAll();
}


void ITU::MainWindow::updateActualTime () {
	this->statusBar_actualTime->Text = DateTime::Now.ToString( timeFormat );

	this->actualTime_hour->Text   = DateTime::Now.ToString( "HH" );
	this->actualTime_minute->Text = DateTime::Now.ToString( "mm" );
}


Void ITU::MainWindow::offTime_hourP_Click( Object^ sender, EventArgs^ e ) { offHourPlus(); }
Void ITU::MainWindow::offTime_hourM_Click( Object^ sender, EventArgs^ e ) { offHourMinus(); }

Void ITU::MainWindow::offTime_minuteP_Click( Object^ sender, EventArgs^ e ) { offMinutePlus(); }
Void ITU::MainWindow::offTime_minuteM_Click( Object^ sender, EventArgs^ e ) { offMinuteMinus(); }

Void ITU::MainWindow::numberTypingTimer_Tick ( Object^ sender, EventArgs^ e ) {
	this->numberTypingTimer->Enabled = false;
	numberTyping = false;
}


Void ITU::MainWindow::offTime_hour_KeyPress ( Object^ sender, KeyEventArgs^ e ) {
	// Osetrim sipku nahoru a dolu
	switch ( e->KeyCode ) {
		case Keys::Up:  
			offHourSet( ++offHour );
			break;

		case Keys::Down:
			offHourSet( --offHour );
			break;

		default:
			return;
	}

	e->Handled = true;
}

Void ITU::MainWindow::offTime_minute_KeyPress ( Object^ sender, KeyEventArgs^ e ) {
	// Osetrim sipku nahoru a dolu
	switch ( e->KeyCode ) {
		case Keys::Up:  
			offMinuteSet( ++offMinute, true );
			break;

		case Keys::Down:
			offMinuteSet( --offMinute, true );
			break;

		default:
			return;
	}

	e->Handled = true;
}

Void ITU::MainWindow::offTime_hour_KeyPress ( Object^ sender, KeyPressEventArgs^ e ) {
	int digit = e->KeyChar - 48;
	if ( digit < 0 || digit > 9 ) {
		// Ignoruji akci
		numberTypingTimer_Tick( nullptr, nullptr );
		offHourSet( offHour );
		return;
	}

	// Byla zadana prvni cislice
	if ( ! numberTyping ) {
		// Jedna se o 1 nebo 2 -> muze to byt dvouciferne cislo
		if ( digit < 3 ) {
			numberTyping = true;
			this->numberTypingTimer->Enabled = true;
		}
		// Cislo bylo uspesne zadano
		// Posunu se na minuty
		else {
			this->offTime_minute->Select();
		}

		offHour = digit;
	}
	// Byla zadana druha cislice
	else {
		offHour *= 10;
		offHour += digit;
		numberTypingTimer_Tick( nullptr, nullptr );

		// Posunu se na minuty
		this->offTime_minute->Select();
	}

	offHourSet( offHour );
}

Void ITU::MainWindow::offTime_minute_KeyPress ( Object^ sender, KeyPressEventArgs^ e ) {
	int digit = e->KeyChar - 48;
	if ( digit < 0 || digit > 9 ) {
		// Ignoruji akci
		numberTypingTimer_Tick( nullptr, nullptr );
		offMinuteSet( offMinute );
		return;
	}

	// Byla zadana prvni cislice
	if ( ! numberTyping ) {
		// Jedna se o 1 - 5 -> muze to byt dvouciferne cislo
		if ( digit < 6 ) {
			numberTyping = true;
			this->numberTypingTimer->Enabled = true;
		}
		// Cislo bylo uspesne zadano
		// Posunu se na vyber akce
		else {
			this->actionTypeButton->Select();
		}

		offMinute = digit;
	}
	// Byla zadana druha cislice
	else {
		offMinute *= 10;
		offMinute += digit;
		numberTypingTimer_Tick( nullptr, nullptr );

		// Posunu se na vyber akce
		this->actionTypeButton->Select();
	}

	offMinuteSet( offMinute );
}


void ITU::MainWindow::offHourSet ( int input ) {
	offHour = input;
	if ( offHour > 23 ) { offHour = 23; }
	if ( offHour < 0  ) { offHour = 0; }
	offHourUpdate();
}
void ITU::MainWindow::offMinuteSet ( int input, bool setHour ) {
	offMinute = input;

	if ( setHour ) {
		if ( offMinute > 59 ) {
			offMinute = 0;
			offHourPlus();
		}
		else if ( offMinute < 0  ) {
			offMinute = 59;
			offHourMinus();
		}
	}
	else {
		if ( offMinute > 59 ) {
			offMinute = 59;
		}
		else if ( offMinute < 0  ) {
			offMinute = 0;
		}
	}

	offMinuteUpdate();
}
void ITU::MainWindow::offMinuteSet ( int input ) {
	offMinute = input;

	if ( offMinute > 59 ) {
		offMinute = 59;
	}
	else if ( offMinute < 0  ) {
		offMinute = 0;
	}

	offMinuteUpdate();
}
void ITU::MainWindow::offHourPlus () {
	if ( ++offHour > 23 ) {
		offHour = 0;
	}
	offHourUpdate();
}
void ITU::MainWindow::offMinutePlus () {
	if ( ++offMinute > 59 ) {
		offMinute = 0;
		offHourPlus();
	}
	offMinuteUpdate();
}

void ITU::MainWindow::offHourMinus () {
	if ( --offHour < 0 ) {
		offHour = 23;
	}
	offHourUpdate();
}
void ITU::MainWindow::offMinuteMinus () {
	if ( --offMinute < 0 ) {
		offMinute = 59;
		offHourMinus();
	}
	offMinuteUpdate();
}

void ITU::MainWindow::offHourSet ( String^ input ) {
	offHourSet( Convert::ToInt32( input ) );
	offHourUpdate();
}
void ITU::MainWindow::offHourSet ( Decimal input ) {
	offHourSet( Convert::ToInt32( input ) );
	offHourUpdate();
}
void ITU::MainWindow::offMinuteSet ( String^ input ) {
	offMinuteSet( Convert::ToInt32( input ) );
	offMinuteUpdate();
}
void ITU::MainWindow::offMinuteSet ( Decimal input ) {
	offMinuteSet( Convert::ToInt32( input ) );
	offMinuteUpdate();
}

void ITU::MainWindow::offHourUpdate () {
	this->offTime_hour->Text = ( offHour < 10 ? "0" : "" ) + Convert::ToString( offHour );
	alignOffBoxes();
}
void ITU::MainWindow::offMinuteUpdate () {
	this->offTime_minute->Text = ( offMinute < 10 ? "0" : "" ) + Convert::ToString( offMinute );
	offHourUpdate();
}

// Ubehl cas k vypnuti ->
Void ITU::MainWindow::shutdownTimer_Tick ( Object^ sender, EventArgs^ e ) {
	// Vypnu casovac a nastavim progressbar
	this->shutdownTimer->Enabled = false;
	this->statusBar_progress->Value = 100;

	// Spustim zaverecny odpocet
	// TODO

	// Zobrazim informacni bublinu
	// TODO

	shutdown->run();	// FIXME
}

Void ITU::MainWindow::setButton_Click( Object^ sender, EventArgs^ e ) {
	// Casovac bezi
	if ( this->shutdownTimer->Enabled ) {
		if (mode == 1) {
			offMinute = offMinuteTmp;
			offHour = offHourTmp;
		}

		// Zmenim ikonu
		this->setButton->Image = ( cli::safe_cast< System::Drawing::Image^ >( images->GetObject( L"start.Image64" ) ) );
		
		// Vypnu casovac
		this->shutdownTimer->Stop();

		// Zapnu tlacitka a textove vstupy
		this->offTime_hourP    ->Enabled = true;
		this->offTime_hourM    ->Enabled = true;
		this->offTime_splitter ->Enabled = true;
		this->offTime_minuteP  ->Enabled = true;
		this->offTime_minuteM  ->Enabled = true;
		this->offTime_hour     ->Enabled = true;
		this->offTime_minute   ->Enabled = true;
		this->offTime_reset    ->Enabled = true;
		this->actionTypeButton ->Enabled = true;

		// Vynuluji progressbar
		this->statusBar_progress->Value = 0;
	}
	// Zapinam Casovac
	else {
		// Zmenim ikonu
		this->setButton->Image = ( cli::safe_cast< System::Drawing::Image^ >( images->GetObject( L"stop.Image64" ) ) );
		//this->setButton->Text = "Storno";

		// Nastavim cas vypnuti 
		if (mode == 0) {	// rezim "vypnout v"
			DateTime tmp( DateTime::Now.Year, DateTime::Now.Month, DateTime::Now.Day, offHour, offMinute, 0 );
			// K vynuti dojde az zitra -> posunu se o den
			if ( DateTime::Now > tmp ) {
				tmp = tmp.AddDays( 1 );
			}
			shutdownTime = tmp;
		}
		else {				//rezim "vypnout za"

			//ulozim stav promennych
			offHourTmp = offHour;
			offMinuteTmp = offMinute;

			//prictu k aktualnimu casu cas, ktery je nastaveny
			offMinute += DateTime::Now.Minute;
			offHour += DateTime::Now.Hour;

			//kontroly preteceni
			if (offMinute > 59) {
				offHour++;
				offMinute -= 60;
			}
			if (offHour > 23)
				offHour -= 24;
			
			
			DateTime tmp(DateTime::Now.Year, DateTime::Now.Month, DateTime::Now.Day, offHour, offMinute, 0);
			if (DateTime::Now > tmp) {
				tmp = tmp.AddDays(1);
			}
			shutdownTime = tmp;
		}
		
		// Vypnu tacitka a vstupy
		this->offTime_hourP    ->Enabled = false;
		this->offTime_hourM    ->Enabled = false;
		this->offTime_splitter ->Enabled = false;
		this->offTime_minuteP  ->Enabled = false;
		this->offTime_minuteM  ->Enabled = false;
		this->offTime_hour     ->Enabled = false;
		this->offTime_minute   ->Enabled = false;
		this->offTime_reset    ->Enabled = false;
		this->actionTypeButton ->Enabled = false;

		// Spustim casovac
		this->shutdownTimer->Interval = (int)shutdownTime->Subtract( DateTime::Now ).TotalMilliseconds;
		this->shutdownTimer->Start();
	}
}


Void ITU::MainWindow::offTime_reset_Click ( Object^ sender, EventArgs^ e ) {
	offHourSet( DateTime::Now.Hour );
	offMinuteSet( DateTime::Now.Minute );
}

Void ITU::MainWindow::actionTypeButton_Click  ( Object^ sender, EventArgs^ e ) {
	// Vytvorim nove okno
	SelectAction^ selectAction = gcnew SelectAction( this );

	// Zjistim si pozici kurzoru a podle toho umistim okno
	// TODO validace hodnot
	Point pos = Cursor->Position;	// FIXME
	pos.Offset ( - selectAction->Width / 2 , - selectAction->Height / 2  );
	
	// Zobrazim okno s vyberem akci
	selectAction->Location = pos;
	selectAction->Show();
}

Void ITU::MainWindow::MainWindow_EnabledChanged ( Object^ sender, EventArgs^ e ) {
	// Okno bylo povoleno
	if ( this->Enabled ) {
		redrawActionTypeButton();
	}
}

void ITU::MainWindow::redrawActionTypeButton () {
	// Prekreslim tlacitko
	this->actionTypeButton->Text  = shutdown->getName();
	this->actionTypeButton->Image = shutdown->getImage( images );
}


Void ITU::MainWindow::mouseTimer_Tick ( Object^ sender, EventArgs^ e ) {
	if ( mouseTimer_firstTick ) {
		mouseTimer_firstTick = false;
		this->mouseTimer->Interval = 100;
	}
	else {
		switch ( mouseTimer_action ) {
			case 1: offHourPlus();    break;
			case 2: offHourMinus();   break;
			case 3: offMinutePlus();  break;
			case 4: offMinuteMinus(); break;
		}


	}
}

/*
 # 1 = hour++
 # 2 = hour--
 # 3 = minute++
 # 4 = minute--
 */
void ITU::MainWindow::startMouseTimer ( int action ) {
	mouseTimer_action = action;
	mouseTimer_firstTick = true;
	this->mouseTimer->Interval = 500;
	this->mouseTimer->Start();
}

void ITU::MainWindow::stopMouseTimer () {
	this->mouseTimer->Stop();
}

Void ITU::MainWindow::offTime_change_Click(Object^ sender, EventArgs^ e) {

	if (mode == 0) {
		mode = 1;	// vypnout za (1)
		this->offTimeBox->Text = L"Vypnout za";
		offHour = 0;
		offHourUpdate();
		offMinute = 1;
		offMinuteUpdate();
	}
	else {
		mode = 0;	// vypnout v (0)
		this->offTimeBox->Text = L"Vypnout v";
		offHourSet(DateTime::Now.Hour);
		offMinuteSet(DateTime::Now.Minute);
	}
}
