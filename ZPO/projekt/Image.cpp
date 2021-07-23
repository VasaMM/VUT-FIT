/**
 * @Author: Vaclav Martinka
 * @Date:   05. 04. 2019 (19:52)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Image.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 01. 05. 2019 (18:58)
 * @License: CC BY-SA 3.0
 */



#include <QImageReader>
#include <QDebug>

#include "Image.hpp"


bool Image::set( const QString &imageFileName, QString *error ) {
	(void)error;
	QImageReader reader( imageFileName );
	reader.setAutoTransform( true );

	QImage _image = reader.read();
	if ( _image.isNull() ) {
		QString errStr = reader.errorString();
		error = &errStr;
		return false;
	}

	_imageFileName = imageFileName;

	set( _image );

	emit loaded();

	return true;
}


void Image::set( const QImage &image ) {
	_image          = image;
	_imageOriginal  = QImage( image );

	emit loaded();
}



void Image::tresholder( QRgb treshold, TresholderMode mode, QRgb black, QRgb white ) {
	for ( int y = 0 ; y < height() ; ++y ) {
		const int Ycoord = width() * y;

		for ( int x = 0 ; x < width() ; ++x ) {
			QRgb *pixel = &static_cast< QRgb* >( static_cast< void * >( _image.bits() ) )[ ( Ycoord + x ) ];
			if (
				qRed( *pixel )   > qRed( treshold ) &&
				qGreen( *pixel ) > qGreen( treshold ) &&
				qBlue( *pixel )  > qBlue( treshold )
			) {
				if ( mode == BLACK_AND_WHITE || mode == ONLY_WHITE ) {
					*pixel = white;
				}
			}
			else {
				if ( mode == BLACK_AND_WHITE || mode == ONLY_BLACK ) {
					*pixel = black;
				}
			}
		}
	}

	emit changed();
}


bool Image::filter( int size, const double *matrix, uint coefficient ) {
	if ( size < 3 || size % 2 == 0 ) {
		return false;
	}


	const int offset = size >> 1;

	const int newWidth  = width()  - ( offset << 1 );
	const int newHeight = height() - ( offset << 1 );


	// FIXME utika pamet
	buffer = new unsigned char[ ( newWidth * newHeight ) << 2 ];

	QRgb **lines = new QRgb *[ size ];

	for ( int y = offset ; y < height() - offset ; ++y ) {
		const int Ycoord = newWidth * ( y - offset );

		for ( int l = 0 ; l < size ; ++l ) {
			lines[ l ] = static_cast< QRgb* >( static_cast< void * >( _image.scanLine( y - offset + l ) ) );
		}

		for ( int x = offset ; x < width() - offset ; ++x ) {
			double RSum = 0;
			double GSum = 0;
			double BSum = 0;

			for ( int y_matrix = 0 ; y_matrix < size ; ++y_matrix ) {
				for ( int x_matrix = 0 ; x_matrix < size ; ++x_matrix ) {
					// FIXME
					RSum += matrix[ y_matrix * size + x_matrix ] * qRed(   lines[ y_matrix ][ x - offset + x_matrix ] );
					GSum += matrix[ y_matrix * size + x_matrix ] * qGreen( lines[ y_matrix ][ x - offset + x_matrix ] );
					BSum += matrix[ y_matrix * size + x_matrix ] * qBlue(  lines[ y_matrix ][ x - offset + x_matrix ] );
				}
			}

			RSum /= coefficient;
			GSum /= coefficient;
			BSum /= coefficient;

			if ( RSum < 0 ) RSum = 0;
			if ( GSum < 0 ) GSum = 0;
			if ( BSum < 0 ) BSum = 0;

			if ( RSum > 255 ) RSum = 255;
			if ( GSum > 255 ) GSum = 255;
			if ( BSum > 255 ) BSum = 255;

			static_cast< QRgb* >( static_cast< void * >( buffer ) )[ ( Ycoord + x - offset ) ] = qRgb( static_cast<int>( RSum ), static_cast<int>( GSum ), static_cast<int>( BSum ) );
		}
	}

	_image = QImage( buffer, newWidth, newHeight, QImage::Format_RGB32 );

	delete[] lines;


	emit changed();
	return true;
}



// bool Image::median( int size ) {
// 	if ( size < 3 ) {
// 		return false;
// 	}
//
// 	int offset = size >> 1;
// 	const uint offset2D = ( size * size ) >> 1;
//
//
//
// 	const int newWidth  = width()  - ( offset << 1 );
// 	const int newHeight = height() - ( offset << 1 );
//
//
// 	// FIXME utika pamet
// 	buffer = new unsigned char[ ( newWidth * newHeight ) << 2 ];
//
// 	QRgb **lines = new QRgb *[ size ];
// 	QRgb  *mask  = new uchar[ size * size ];
//
//
//
// 	for ( int y = offset ; y < height() - offset ; ++y ) {
// 		const int Ycoord = newWidth * ( y - offset );
//
// 		for ( int l = 0 ; l < size ; ++l ) {
// 			lines[ l ] = static_cast< QRgb* >( static_cast< void * >( _image.scanLine( y - offset + l ) ) );
// 		}
//
// 		for ( int x = offset ; x < width() - offset ; ++x ) {
//
//
// 			int index = 0;
// 			// Pro kazdy pixel projdu jeho okoli a ulozim ho do pole mask
// 			for ( int j = 0; j < size ; ++j ) {
// 				for ( int i = 0; i < size ; ++i ) {
// 					mask[ index++ ] = lines[j][ x - offset + i ];
// 				}
// 			}
//
// 			// Seradim a zapisu median na vystup
// 			sort( mask, &mask[size * size] );
// 			dst.at<uchar>( y, x ) = mask[stred2D];
//
//
// 		}
// 	}
//
// 	_image = QImage( buffer, newWidth, newHeight, QImage::Format_RGB32 );
//
// 	delete[] lines;
//
//
// 	emit changed();
// 	return true;
// }




double Image::diff( Image &img, int border ) {
	int sum = 0;


	for ( int y = border ; y < height() - border ; ++y ) {
		QRgb* line_me = static_cast< QRgb* >( static_cast< void * >( _image.scanLine( y ) ) );
		QRgb* line_img = static_cast< QRgb* >( static_cast< void * >( img.image()->scanLine( y - border ) ) );

		for ( int x = border ; x < width() - border ; ++x ) {
			sum += abs( qRed(   line_me[ x ] ) - qRed(   line_img[ x - border ] ) );
			sum += abs( qGreen( line_me[ x ] ) - qGreen( line_img[ x - border ] ) );
			sum += abs( qBlue(  line_me[ x ] ) - qBlue(  line_img[ x - border ] ) );
		}
	}

	return sum / ( ( width() - ( border << 1 ) ) * ( height() - ( border << 1 ) ) * 3 );
}
