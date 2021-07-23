/**
 * @Author: Vaclav Martinka <xmarti76>
 * @Date:   02. 04. 2019 (21:21)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Histograms.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 01. 05. 2019 (16:11)
 */



#include <QtWidgets>

#include "Histograms.hpp"



Histograms::Histograms() {
	RGBLabel = new QLabel( this );
	RLabel = new QLabel( this );
	GLabel = new QLabel( this );
	BLabel = new QLabel( this );
	GrayLabel = new QLabel( this );

	infoLabel = new QLabel( this );
	drawExposureBtn = new QPushButton( tr( "Show" ), this );
	drawExposureBtn->setDisabled( true );

	int widgetsCount = addWidgets( QVector< QWidget * >( {
		new QLabel( "RGB", this ),
		RGBLabel,

		new QLabel( "RED", this ),
		RLabel,

		new QLabel( "GREEN", this ),
		GLabel,

		new QLabel( "BLUE", this ),
		BLabel,

		new QLabel( "GREY", this ),
		GrayLabel,

		new QLabel( this ),
		infoLabel,
		drawExposureBtn
	} ) );


	verticalSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );
	layout->addItem( verticalSpacer, ++widgetsCount, 0 );


	RGBHistogram = new QImage( 512, 258, QImage::Format_RGB32 );
	RGBHistogram->fill( histogramBackground );

	RHistogram = new QImage( 512, 258, QImage::Format_RGB32 );
	RHistogram->fill( histogramBackground );

	GHistogram = new QImage( 512, 258, QImage::Format_RGB32 );
	GHistogram->fill( histogramBackground );

	BHistogram = new QImage( 512, 258, QImage::Format_RGB32 );
	BHistogram->fill( histogramBackground );

	GrayHistogram = new QImage( 512, 258, QImage::Format_RGB32 );
	GrayHistogram->fill( histogramBackground );


	connect( drawExposureBtn, SIGNAL( released() ), this, SLOT( exposureBtnClick() ) );
}



Histograms::~Histograms() {
	delete verticalSpacer;
	delete scrollContent;
}

void Histograms::update( Image *_image ) {
	image = _image;

	QVector< int > RVals(256,0);
	QVector< int > GVals(256,0);
	QVector< int > BVals(256,0);
	QVector< int > GrayVals(256,0);

	QImage grayscale = image->grayscale();


	for ( int y = 0 ; y < image->height() ; ++y ) {
		for ( int x = 0 ; x < image->width() ; ++x ) {
			++RVals[ qRed  ( image->pixel( x, y ) ) ];
			++GVals[ qGreen( image->pixel( x, y ) ) ];
			++BVals[ qBlue ( image->pixel( x, y ) ) ];
			++GrayVals[ qRed ( grayscale.pixel( x, y ) ) ];
		}
	}

	drawHistogram( GrayVals, RGBHistogram, QColor( 0,    0,    0    ), false, true );
	drawHistogram( RVals,    RGBHistogram, QColor( 0xff, 0,    0    ), false, false );
	drawHistogram( GVals,    RGBHistogram, QColor( 0,    0xff, 0    ), false, false );
	drawHistogram( BVals,    RGBHistogram, QColor( 0,    0,    0xff ), false, false );
	RGBLabel->setPixmap( QPixmap::fromImage( *RGBHistogram ) );

	drawHistogram( RVals, RHistogram, QColor( 0xff, 0, 0 ) );
	RLabel->setPixmap( QPixmap::fromImage( *RHistogram ) );

	drawHistogram( GVals, GHistogram, QColor( 0, 0xff, 0 ) );
	GLabel->setPixmap( QPixmap::fromImage( *GHistogram ) );

	drawHistogram( BVals, BHistogram, QColor( 0, 0, 0xff ) );
	BLabel->setPixmap( QPixmap::fromImage( *BHistogram ) );

	drawHistogram( GrayVals, GrayHistogram, QColor( 0, 0, 0 ) );
	GrayLabel->setPixmap( QPixmap::fromImage( *GrayHistogram ) );


	infoLabel->setText( "EXPOSURE IS OK" );
	drawExposureBtn->setDisabled( true );
	int sum = 0;
	for ( uint i = 0; i < EXPOSURE_BORDER ; ++i ) {
		sum += GrayVals[ i ];
	}

	if ( 1.0 * sum / image->resolution() > EXPOSURE_TRESHOLD ) {
		infoLabel->setText( "UNDER-EXPOSED: " + QString::number( 100.0 * sum / image->resolution() ) + " %" );
		treshold = QColor( EXPOSURE_BORDER, EXPOSURE_BORDER, EXPOSURE_BORDER );
		mode = Image::ONLY_BLACK;
		drawExposureBtn->setEnabled( true );
	}
	else {
		sum = 0;
		for ( uint i = 256 - EXPOSURE_BORDER; i < 256 ; ++i ) {
			sum += GrayVals[ i ];
		}

		if ( 1.0 * sum / image->resolution() > EXPOSURE_TRESHOLD ) {
			infoLabel->setText( "OVER-EXPOSED: " + QString::number( 100.0 * sum / image->resolution() ) + " %" );
			treshold = QColor( 255 - EXPOSURE_BORDER, 255 - EXPOSURE_BORDER, 255 - EXPOSURE_BORDER );
			mode = Image::ONLY_WHITE;
			drawExposureBtn->setEnabled( true );
		}
	}
}



void Histograms::drawHistogram( QVector< int > input, QImage *output, QColor color, bool fill, bool clear ) {
	int maxNumber = 0;

	for ( int i = 0; i < input.size() ; ++i ) {
		if ( input[ i ] > maxNumber ) {
			maxNumber = input[ i ];
		}
	}

	if ( clear ) {
		output->fill( histogramBackground );
	}

	// Tvorim vyplneny histogram
	if ( fill ) {
		for( int x = 0 ; x < 256; ++x ) {
			int maxY = static_cast< int >( ( 1.0 *  input[ x ] / maxNumber ) * 255 );

			for ( int y = 0 ; y < maxY ; ++y ) {
				output->setPixelColor( 2 * x    , ( 256 - y ), color );
				output->setPixelColor( 2 * x + 1, ( 256 - y ), color );
			}
		}

		for( int x = 0 ; x < 512; ++x ) {
			output->setPixelColor( x, 256, color );
		}
	}
	// Tvorim prazdny histogram
	else {
		int prevMaxY = static_cast< int >( ( 1.0 *  input[ 0 ] / maxNumber ) * 255 );

		for( int x = 0 ; x < 256; ++x ) {
			int maxY = static_cast< int >( ( 1.0 *  input[ x ] / maxNumber ) * 255 );

			// Graf roste
			for ( int y = prevMaxY ; y < maxY ; ++y ) {
				output->setPixelColor( 2 * x    , ( 256 - y ), color );
				output->setPixelColor( 2 * x + 1, ( 256 - y ), color );
			}

			// Graf klesa
			for ( int y = prevMaxY ; y > maxY ; --y ) {
				output->setPixelColor( 2 * x    , ( 256 - y ), color );
				output->setPixelColor( 2 * x - 1, ( 256 - y ), color );
			}

			output->setPixelColor( 2 * x    , ( 256 - maxY ), color );
			output->setPixelColor( 2 * x + 1, ( 256 - maxY ), color );

			prevMaxY = maxY;
		}
	}
}
