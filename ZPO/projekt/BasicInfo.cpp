/**
 * @Author: Vaclav Martinka
 * @Date:   02. 04. 2019 (17:35)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: BasicInfo.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:28)
 * @License: CC BY-SA 3.0
 */



#include <fstream>
#include <QtWidgets>
#include <cstring>

#include "BasicInfo.hpp"
#include "exif.h"
#include "Fraction.hpp"
#include "ColorCounter.hpp"

using namespace std;



BasicInfo::BasicInfo() : Tab() {
	basicInfoLabels = QVector< QLabel * >( __BASIC_INFO_COUNT__ );

	QVector< QLabel ** > basicInfoLabelsPointers( __BASIC_INFO_COUNT__ );
	for ( int i = 0 ; i < basicInfoLabels.size() ; ++i ) {
		basicInfoLabelsPointers[ i ] = &basicInfoLabels[ i ];
	}

	int widgetsCount = addWidgets( basicInfoNames, basicInfoLabelsPointers, 0 );

	addWidget( new QLabel( this ), ++widgetsCount, 0 );

	QLabel *exifLabel = new QLabel( tr( "EXIF" ), this );
	exifLabel->setStyleSheet( "font-weight: bold;" );
	addWidget( exifLabel, ++widgetsCount, 0 );


	exifLabels = QVector< QLabel * >( __EXIF_COUNT__ );
	QVector< QLabel ** > exifLabelsPointers( __EXIF_COUNT__ );
	for ( int i = 0 ; i < exifLabels.size() ; ++i ) {
		exifLabelsPointers[ i ] = &exifLabels[ i ];
	}

	++widgetsCount;
	widgetsCount += addWidgets( exifNames, exifLabelsPointers, widgetsCount );

	verticalSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );
	layout->addItem( verticalSpacer, ++widgetsCount, 0 );
	layout->addItem( verticalSpacer, widgetsCount++, 1 );


	counter = new ColorCounterWorker();
	connect( counter, SIGNAL( done( QString ) ), basicInfoLabels[ COLOR_COUNT ], SLOT( setText( QString ) ) );
}



BasicInfo::~BasicInfo() {
	delete verticalSpacer;
}

void BasicInfo::update( Image *img ) {
	image = img;
	connect( image, SIGNAL( changed() ), this, SLOT( changed() ) );
	updateBasicInfo();
	if ( image->fileName() != "" ) {
		updateExif( image->fileName() );
	}
}


void BasicInfo::changed() {
	counter->update( image, ColorCounterWorker::UNIQUE_COLORS );
	basicInfoLabels[ COLOR_COUNT ]->setText( "Processing" );
	counter->start();
};



void BasicInfo::updateBasicInfo() {
	basicInfoLabels[ WIDTH  ]-> setText( QString::number( image->width()  ) + " Px" );
	basicInfoLabels[ HEIGHT ]->setText( QString::number( image->height() ) + " Px" );
	basicInfoLabels[ DEPTH  ]-> setText( QString::number( image->depth()  ) + " bits" );

	double resolution = image->width() * image->height();
	basicInfoLabels[ RESOLUTION ]->setText( QString::number( resolution > 1000000 ? resolution / 1000000 : resolution ) + " MPx" );

	changed();
}



void BasicInfo::updateExif( QString imageFileName ) {
	QFile file( imageFileName );
    if ( file.open( QIODevice::ReadOnly ) ) {
        QByteArray data = file.readAll();
        easyexif::EXIFInfo info;
		if ( int code = info.parseFrom( static_cast< unsigned char * >( static_cast< void * >( data.data() ) ), static_cast< uint >( data.size() ) ) ) {
            qDebug() << "Error parsing EXIF: code " << code;

            exifLabels[ EXIF_WIDTH ]->setText( "" );
			exifLabels[ EXIF_HEIGHT ]->setText( "" );
			exifLabels[ DESCRIPTION ]->setText( "" );
			exifLabels[ MODEL ]->setText( "" );
			exifLabels[ ORIENTATION ]->setText( "" );
			exifLabels[ BITS_PER_SAMPLE ]->setText( "" );
			exifLabels[ SOFTWARE ]->setText( "" );
			exifLabels[ DATE_TIME ]->setText( "" );
			exifLabels[ DATE_TIME_ORIGINAL ]->setText( "" );
			exifLabels[ DATE_TIME_DIGITIZED ]->setText( "" );
			exifLabels[ DATE_TIME_SUBSEC ]->setText( "" );
			exifLabels[ COPYRIGHT ]->setText( "" );
			exifLabels[ ISO ]->setText( "" );
			exifLabels[ EXPOSURE_TIME ]->setText( "" );
			exifLabels[ FNUMBER ]->setText( "" );
			exifLabels[ EXPOSURE_PROGRAM ]->setText( "" );
			exifLabels[ SHUTTER_SPEED ]->setText( "" );
			exifLabels[ EXPOSURE_BIAS ]->setText( "" );
			exifLabels[ SUBJECT_DISTANCE ]->setText( "" );
			exifLabels[ FOCAL_LENGTH ]->setText( "" );
			exifLabels[ FOCAL_LENGTH35 ]->setText( "" );
			exifLabels[ FLASH ]->setText( "" );
			exifLabels[ FLAS_RETURNED_LIGHT ]->setText( "" );
			exifLabels[ FLASH_MODE ]->setText( "" );
			exifLabels[ METERING_MODE ]->setText( "" );
			exifLabels[ LATITUDE ]->setText( "" );
			exifLabels[ LATITUDE_DMS ]->setText( "" );
			exifLabels[ LONGITUDE ]->setText( "" );
			exifLabels[ LONGITUDE_DMS ]->setText( "" );
			exifLabels[ ALTITUDE ]->setText( "" );
			exifLabels[ PRECISION ]->setText( "" );
			exifLabels[ LENS_F_STOP_MIN ]->setText( "" );
			exifLabels[ LENS_F_STOP_MAX ]->setText( "" );
			exifLabels[ LENS_FOCAL_MIN ]->setText( "" );
			exifLabels[ LENS_FOCAL_MAX ]->setText( "" );
			exifLabels[ LENS_X_RES ]->setText( "" );
			exifLabels[ LENS_Y_RES ]->setText( "" );
			exifLabels[ LENS_MODEL ]->setText( "" );

            return;
        }
        else {

			exifLabels[ EXIF_WIDTH ]->setText( QString::number( info.ImageWidth ) );
			exifLabels[ EXIF_HEIGHT ]->setText( QString::number( info.ImageHeight ) );
			exifLabels[ DESCRIPTION ]->setText( QString::fromStdString( info.ImageDescription.c_str() ) );
			exifLabels[ MODEL ]->setText( QString::fromStdString( ( info.Make + ( info.Make != "" ? " " : "" ) + info.Model ).c_str() ) );

			switch ( info.Orientation ) {
				case 0:  exifLabels[ ORIENTATION ]->setText( "Unspecified" );  break;
				case 1:  exifLabels[ ORIENTATION ]->setText( "Upper left" );   break;
				case 3:  exifLabels[ ORIENTATION ]->setText( "Lower right" );  break;
				case 6:  exifLabels[ ORIENTATION ]->setText( "Upper right" );  break;
				case 8:  exifLabels[ ORIENTATION ]->setText( "Lower left" );   break;
				case 9:  exifLabels[ ORIENTATION ]->setText( "Undefined" );    break;

				default: exifLabels[ ORIENTATION ]->setText( "Unknown value" ); break;
			}

			exifLabels[ BITS_PER_SAMPLE ]->setText( QString::number( info.BitsPerSample ) );
			exifLabels[ SOFTWARE ]->setText( QString::fromStdString( info.Software ) );
			exifLabels[ DATE_TIME ]->setText( QString::fromStdString( info.DateTime ) );
			exifLabels[ DATE_TIME_ORIGINAL ]->setText( QString::fromStdString( info.DateTimeOriginal ) );
			exifLabels[ DATE_TIME_DIGITIZED ]->setText( QString::fromStdString( info.DateTimeDigitized ) );
			exifLabels[ DATE_TIME_SUBSEC ]->setText( QString::fromStdString( info.SubSecTimeOriginal ) );
			exifLabels[ COPYRIGHT ]->setText( QString::fromStdString( info.Copyright ) );

			exifLabels[ ISO ]->setText( QString::number( info.ISOSpeedRatings ) );
			exifLabels[ EXPOSURE_TIME ]->setText( Fraction( info.ExposureTime ) );
			exifLabels[ FNUMBER ]->setText( QString::number( info.FNumber ) );

			switch ( info.ExposureProgram ) {
				case 0:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Not defined" );        break;
				case 1:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Manual" );             break;
				case 2:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Normal program" );     break;
				case 3:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Aperture priority" );  break;
				case 4:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Shutter priority" );   break;
				case 5:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Creative program" );   break;
				case 6:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Action program" );     break;
				case 7:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Portrait mode" );      break;
				case 8:  exifLabels[ EXPOSURE_PROGRAM ]->setText( "Landscape mode" );     break;

				default: exifLabels[ EXPOSURE_PROGRAM ]->setText( "Unknown value" ); break;
			}

			exifLabels[ SHUTTER_SPEED ]->setText( QString::number( info.ShutterSpeedValue ) );
			exifLabels[ EXPOSURE_BIAS ]->setText( QString::number( info.ExposureBiasValue ) );
			exifLabels[ SUBJECT_DISTANCE ]->setText( QString::number( info.SubjectDistance ) );
			exifLabels[ FOCAL_LENGTH ]->setText( QString::number( info.FocalLength ) );
			exifLabels[ FOCAL_LENGTH35 ]->setText( QString::number( info.FocalLengthIn35mm ) );

			exifLabels[ FLASH ]->setText( info.Flash ? "Used" : "No flash" );

			switch ( info.FlashReturnedLight ) {
				case 0:  exifLabels[ FLAS_RETURNED_LIGHT ]->setText( "No strobe return detection function" ); break;
				case 1:  exifLabels[ FLAS_RETURNED_LIGHT ]->setText( "Reserved" );                            break;
				case 2:  exifLabels[ FLAS_RETURNED_LIGHT ]->setText( "Strobe return light not detected" );    break;
				case 3:  exifLabels[ FLAS_RETURNED_LIGHT ]->setText( "Strobe return light detected" );        break;

				default: exifLabels[ FLAS_RETURNED_LIGHT ]->setText( "Unknown value" ); break;
			}

			switch ( info.FlashMode ) {
				case 0:  exifLabels[ FLASH_MODE ]->setText( "Unknown" );                       break;
				case 1:  exifLabels[ FLASH_MODE ]->setText( "Compulsory flash firing" );       break;
				case 2:  exifLabels[ FLASH_MODE ]->setText( "Compulsory flash suppression" );  break;
				case 3:  exifLabels[ FLASH_MODE ]->setText( "Automatic mode" );                break;

				default: exifLabels[ FLASH_MODE ]->setText( "Unknown value" ); break;
			}

			switch ( info.MeteringMode ) {
				case 1:  exifLabels[ METERING_MODE ]->setText( "Average" );                  break;
				case 2:  exifLabels[ METERING_MODE ]->setText( "Center weighted average" );  break;
				case 3:  exifLabels[ METERING_MODE ]->setText( "Spot" );                     break;
				case 4:  exifLabels[ METERING_MODE ]->setText( "Multi-spot" );               break;
				case 5:  exifLabels[ METERING_MODE ]->setText( "Multi-segment" );            break;

				default: exifLabels[ METERING_MODE ]->setText( "Unknown value" ); break;
			}

			easyexif::EXIFInfo::Geolocation_t location = info.GeoLocation;

			exifLabels[ LATITUDE ]->setText( QString::number( location.Latitude ) );
			exifLabels[ LONGITUDE ]->setText( QString::number( location.Longitude ) );

			QString LocDMS = QString::number( location.LatComponents.degrees ) + "° " +
			                 QString::number( location.LatComponents.minutes ) + "' " +
			                 QString::number( location.LatComponents.seconds ) + "''";
			exifLabels[ LATITUDE_DMS ]->setText( LocDMS );

			LocDMS = QString::number( location.LonComponents.degrees ) + "° " +
			         QString::number( location.LonComponents.minutes ) + "' " +
			         QString::number( location.LonComponents.seconds ) + "''";
			exifLabels[ LONGITUDE_DMS ]->setText( LocDMS );

			exifLabels[ ALTITUDE ]->setText( location.AltitudeRef == -1 ? "-" : "" + QString::number( location.Altitude ) );
			exifLabels[ PRECISION ]->setText( QString::number( location.DOP ) );


			easyexif::EXIFInfo::LensInfo_t lens = info.LensInfo;

			exifLabels[ LENS_F_STOP_MIN ]->setText( QString::number( lens.FStopMin ) );
			exifLabels[ LENS_F_STOP_MAX ]->setText( QString::number( lens.FStopMax ) );
			exifLabels[ LENS_FOCAL_MIN ]->setText( QString::number( lens.FocalLengthMin ) );
			exifLabels[ LENS_FOCAL_MAX ]->setText( QString::number( lens.FocalLengthMax ) );

			QString FUnit = "";
			switch ( lens.FocalPlaneResolutionUnit ) {
				case 1:  FUnit = "";  break;
				case 2:  FUnit = " ″";  break;
				case 3:  FUnit = " Cm";  break;
				case 4:  FUnit = " mm";  break;
				case 5:  FUnit = " µm";  break;
			}

			exifLabels[ LENS_X_RES ]->setText( QString::number( lens.FocalPlaneXResolution ) + FUnit );
			exifLabels[ LENS_Y_RES ]->setText( QString::number( lens.FocalPlaneYResolution ) + FUnit );

			exifLabels[ LENS_MODEL ]->setText( QString::fromStdString( ( lens.Make + ( lens.Make != "" ? " " : "" ) + lens.Model ).c_str() ) );
        }
	}
}
