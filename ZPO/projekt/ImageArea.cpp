/**
 * @Author: Vaclav Martinka
 * @Date:   02. 04. 2019 (18:01)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: ImageArea.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:38)
 * @License: CC BY-SA 3.0
 */



#include <QtWidgets>

#include "ImageArea.hpp"
#include "BasicInfo.hpp"


ImageArea::ImageArea( QSplitter *_mainWidget ) {
	mainWidget = _mainWidget;

	imageLabel = new QLabel( this );
	imageLabel->setBackgroundRole( QPalette::Base );
	imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
	imageLabel->setScaledContents( true );
	imageLabel->setMinimumWidth( 120 );

	this->setBackgroundRole( QPalette::Dark );
	this->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
	this->setWidget( imageLabel );
	this->setMinimumWidth( 300 );

	connect( &img, SIGNAL( changed() ), this, SLOT( changed() ) );
	connect( &img, SIGNAL( loaded()  ), this, SLOT( loaded()  ) );
}



void ImageArea::resizeEvent( QResizeEvent *event ) {
	if ( ! img.isNull() && fitted ) {
		fitToWindow();
	}

	QScrollArea::resizeEvent(event);
}



void ImageArea::changed() {
	imageLabel->setPixmap( QPixmap::fromImage( *img ) );
}


void ImageArea::loaded() {
	changed();

	imageLabel->adjustSize();
	fitToWindow();
}



void ImageArea::normalSize() {
	imageLabel->adjustSize();
	scaleFactor = 1.0;
	fitted = false;
}



void ImageArea::fitToWindow() {
	fitted = true;

	if ( img.landscape() ) {
		scaleFactor = .995 * width() / img.width();
	}
	else {
		scaleFactor = .995 * height() / img.height();
	}

	imageLabel->resize( scaleFactor * imageLabel->pixmap()->size() );
}



void ImageArea::scaleImage( double factor ) {
	fitted = false;

	Q_ASSERT( imageLabel->pixmap() );
	scaleFactor *= factor;

	imageLabel->resize( scaleFactor * imageLabel->pixmap()->size() );
}
