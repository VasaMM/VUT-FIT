/**
 * @Author: Vaclav Martinka
 * @Date:   26. 04. 2019 (12:48)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Filtering.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:31)
 * @License: CC BY-SA 3.0
 */


#include "Filtering.hpp"

#include <QDebug>

using namespace std;

// Velikost zaku pro novy radek
const int EOF_SIZE = QString( "\n" ).size();


Filtering::Filtering() : Tab() {
	coreInput = new QPlainTextEdit( "-1 0 1\n-2 0 2\n-1 0 1", this );
	addWidget( coreInput, 0, 0, 1, -1 );


	addWidget( new QLabel( tr( "Divide by" ), this ), 1, 0 );
	coefficient = new QDoubleSpinBox( this );
	coefficient->setDecimals( 0 );
	coefficient->setMaximum( 10000 );
	coefficient->setValue( 1 );
	addWidget( coefficient, 1, 1 );

	workBtn = new QPushButton( tr( "Run" ), this );
	addWidget( workBtn, 2, 0, 1, -1 );



	verticalSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );
	layout->addItem( verticalSpacer, 3, 0, -1, -1 );

	connect( workBtn, SIGNAL( released() ), this, SLOT( work() ) );
}


Filtering::~Filtering() {
	delete verticalSpacer;
}

void Filtering::update( Image *img ) {
	image = img;
}


void Filtering::work() {
	QString coreStr = coreInput->toPlainText();

	if ( coreStr.right( EOF_SIZE ) != "\n" ) {
		coreStr += "\n";
	}

	int pos;
	int size = 0;

	QVector< double > core;


	// TODO kontrola na sirku
	while ( ( pos = coreStr.indexOf( "\n" ) ) != -1 ) {
		QString line = coreStr.left( pos ) + " ";

		coreStr = coreStr.remove( 0, coreStr.left(pos).size() + EOF_SIZE );

		while ( ( pos = line.indexOf( " " ) ) != -1 ) {
			QString number = line.left( pos );
			core.push_back( number.toDouble() );

			line = line.remove( 0, number.size() + 1 );
		}
		++size;
	}

	image->filter( size, core.data(), coefficient->value() );
}
