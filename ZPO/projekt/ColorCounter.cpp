/**
 * @Author: Vaclav Martinka
 * @Date:   05. 04. 2019 (10:14)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: ColorCounter.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 01. 05. 2019 (16:48)
 * @License: CC BY-SA 3.0
 */



#include <QMap>
#include <QRgb>
#include <QImage>

#include "ColorCounter.hpp"


// QString color2string( QRgb c  ) {
// 	return "(" + QString::number(qRed(c)) + "," + QString::number(qGreen(c)) + "," + QString::number(qBlue(c)) + ")";
// }


void ColorCounterWorker::run() {
	if ( mode == UNIQUE_COLORS ) {
		counter->clear();
		for ( int y = 0 ; y < image.height() ; y++ ) {
			int Ycoord = image.width() * y;

			for ( int x = 0 ; x < image.width() ; x++ ) {
				QRgb pixel = static_cast< QRgb* >( static_cast< void * >( image.bits() ) )[ Ycoord + x ];
				counter->insert( pixel, ++(*counter)[ pixel ] );
			}
		}

		emit done( QString::number( counter->size() ) );
	}
	else if ( mode == ONE_COLOR ) {
		int counter = 0;
		int R = qRed(   color );
		int G = qGreen( color );
		int B = qBlue(  color );

		if ( buffer != nullptr ) {
			delete[] buffer;
		}
		buffer = new unsigned char[ ( image.width() * image.height() ) << 2 ];

		for ( int y = 0 ; y < image.height() ; ++y ) {
			const int Ycoord = image.width() * y;

			for ( int x = 0 ; x < image.width() ; ++x ) {
				QRgb tmpColor = image.pixel( x, y );
				if (
					abs( R - qRed(   tmpColor ) ) <= deviation &&
					abs( G - qGreen( tmpColor ) ) <= deviation &&
					abs( B - qBlue(  tmpColor ) ) <= deviation
				)
				{
					static_cast< QRgb* >( static_cast< void * >( buffer ) )[ ( Ycoord + x ) ] = 0xffffffff;
					++counter;
				}
				else {
					buffer[ ( Ycoord + x ) << 2 ] = 0;
				}
			}
		}

		if ( result != nullptr ) delete result;
		result = new QImage( buffer, image.width(), image.height(), QImage::Format_RGB32 );

		emit done( counter );
	}
}




ColorCounter::ColorCounter() : Tab() {
	selectedColor = QColor( 188, 212, 238 );

	selectColor = new QPushButton(tr( "Select color" ),this);
	addWidget( selectColor, 0, 0, 1, 2 );

	selectColorVisualiation = new QLabel( this );
	addWidget( selectColorVisualiation, 0, 3, 1, 2 );



	addWidget( new QLabel( tr( "Red" ),   this ), 1, 0, 1, 1, Qt::AlignRight );
	addWidget( new QLabel( tr( "Green" ), this ), 1, 2, 1, 1, Qt::AlignRight );
	addWidget( new QLabel( tr( "Blue" ),  this ), 1, 4, 1, 1, Qt::AlignRight );

	RInput = new QDoubleSpinBox( this );
	RInput->setDecimals( 0 );
	RInput->setMaximum( 256.0 );
	addWidget( RInput, 1, 1 );

	GInput = new QDoubleSpinBox( this );
	GInput->setDecimals( 0 );
	GInput->setMaximum( 256.0 );
	addWidget( GInput, 1, 3 );

	BInput = new QDoubleSpinBox( this );
	BInput->setDecimals( 0 );
	BInput->setMaximum( 256.0 );
	addWidget( BInput, 1, 5 );

	addWidget( new QLabel( tr( "Maximal deviation" ),  this ), 2, 0, 1, 1, Qt::AlignRight );
	deviation = new QDoubleSpinBox( this );
	deviation->setDecimals( 0 );
	deviation->setMaximum( 256.0 );
	addWidget( deviation, 2, 1 );


	addWidget( new QLabel( this ), 3, 0 );


	calcBtn = new QPushButton( tr( "Calculate" ), this );
	addWidget( calcBtn, 4, 0, 1, 3 );

	showBtn = new QPushButton( tr( "Show in image" ), this );
	addWidget( showBtn, 4, 3, 1, 3 );


	addWidget( new QLabel( tr( "Total pixels" ),    this ), 5, 0 );
	addWidget( new QLabel( tr( "Selected pixels" ), this ), 6, 0 );


	totalPixels = new QLabel( "0", this );
	addWidget( totalPixels, 5, 1, 1, -1 );

	outputCount = new QLabel( "0", this );
	addWidget( outputCount, 6, 1, 1, -1 );

	outputPercent = new QLabel( "0 %", this );
	addWidget( outputPercent, 7, 1, 1, -1 );


	verticalSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );
	layout->addItem( verticalSpacer, 8, 0, -1, -1 );

	connect( selectColor, SIGNAL( released() ), this, SLOT( updateColorPicker() ) );
	connect( RInput, SIGNAL( valueChanged( double ) ), this, SLOT( updateColorRInput( double ) ) );
	connect( GInput, SIGNAL( valueChanged( double ) ), this, SLOT( updateColorGInput( double ) ) );
	connect( BInput, SIGNAL( valueChanged( double ) ), this, SLOT( updateColorBInput( double ) ) );
	updateInputs();

	connect( calcBtn, SIGNAL( released() ), this, SLOT( calculate() ) );
	connect( showBtn, SIGNAL( released() ), this, SLOT( showInImage() ) );
}


void ColorCounter::updateColorPicker() {
	selectedColor = QColorDialog::getColor( selectedColor );
	updateInputs();
}


void ColorCounter::updateColorRInput( double val ) {
	selectedColor.setRed( static_cast< int >( val ) );
	updateInputs();
}


void ColorCounter::updateColorGInput( double val ) {
	selectedColor.setGreen( static_cast< int >( val ) );
	updateInputs();
}


void ColorCounter::updateColorBInput( double val ) {
	selectedColor.setBlue( static_cast< int >( val ) );
	updateInputs();
}


void ColorCounter::calculate() {
	// FIXME
	QRgb color = selectedColor.rgb();
	counter = new ColorCounterWorker( image, ColorCounterWorker::ONE_COLOR, &color, static_cast< int >( deviation->value() ) );

	outputCount  ->setText( tr( "Processing" ) );
	outputPercent->setText( tr( "Processing" ) );
	connect( counter, SIGNAL( done( int ) ), this, SLOT( updateOutputs( int ) ) );

	calcBtn->setEnabled( false );
	showBtn->setEnabled( false );

	counter->start();
}



void ColorCounter::showInImage() {
	showImage = true;
	calculate();
}


void ColorCounter::updateInputs() {
	// FIXME predelat na prechod
	QString style = "background-color: rgb(%1, %2, %3);";
	selectColorVisualiation->setStyleSheet( style.arg( selectedColor.red() ).arg( selectedColor.green() ).arg( selectedColor.blue() ) );

	RInput->setValue( selectedColor.red() );
	GInput->setValue( selectedColor.green() );
	BInput->setValue( selectedColor.blue() );
}


void ColorCounter::updateOutputs( int val ) {
	outputCount  ->setText( QString::number( val ) );
	outputPercent->setText( QString::number( val ? 100.0 * val / resolution : 0 ) + " %" );

	calcBtn->setEnabled( true );
	showBtn->setEnabled( true );

	if ( showImage ) {
		image->load( *counter->result );
		showImage = false;
	}
}


void ColorCounter::update( Image *img ) {
	image = img;

	resolution = image->width() * image->height();
	totalPixels->setText( QString::number( resolution ) );
}


ColorCounter::~ColorCounter() {
	delete verticalSpacer;
}
