/**
 * @Author: Vaclav Martinka
 * @Date:   26. 04. 2019 (12:48)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Noise.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 01. 05. 2019 (18:07)
 * @License: CC BY-SA 3.0
 */



// #include <QtWidgets>

#include "Noise.hpp"
#include "Image.hpp"

using namespace std;


Noise::Noise() : Tab() {
	addWidget( new QLabel( tr( "Noise" ), this ), 0, 0 );

	noiseLabel = new QLabel( tr( "Unknown" ), this );
	addWidget( noiseLabel, 0, 1 );

	workBtn = new QPushButton( tr( "Denoise" ), this );
	addWidget( workBtn, 1, 0, 1, 6 );


	verticalSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );
	layout->addItem( verticalSpacer, 2, 0, -1, -1 );

	connect( workBtn, SIGNAL( released() ), this, SLOT( work() ) );
}



Noise::~Noise() {
	delete verticalSpacer;
}

void Noise::update( Image *img ) {
	noiseLabel->setText( tr( "Unknown" ) );
	image = img;
}


void Noise::work() {
	Image tmp( *image->image() );
	image->filter( 9, GAUSSIAN_KERNEL );

	noiseLabel->setText( QString::number( tmp.diff( *image, 4 ) ) + " %" );


}
