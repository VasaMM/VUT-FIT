/**
 * @Author: Vaclav Martinka
 * @Date:   02. 04. 2019 (17:35)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: BasicInfo.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:28)
 * @License: CC BY-SA 3.0
 */



#ifndef BASICINFO_HPP
#define BASICINFO_HPP

#include <QtWidgets>
#include <cstring>

#include "Tab.hpp"
#include "ColorCounter.hpp"
#include "Image.hpp"



// Název labelů
const QVector< std::string > basicInfoNames ( {
	"Width",
	"Height",
	"Depth",
	"Resolution",
	"Unique color count",
} );


// ID labelů
enum BasicInfoEnum {
	WIDTH,
	HEIGHT,
	DEPTH,
	RESOLUTION,
	COLOR_COUNT,
	__BASIC_INFO_COUNT__
};


// Název EXIF labelů
const QVector< std::string > exifNames ( {
	"Exif width",
	"Exif height",
	"Description",
	"Model",
	"Orientation",
	"Bits per sample",
	"Software",
	"Date and time",
	"Date and time - original",
	"Date and time - digitized",
	"Date and time - subsecond",
	"Copyright",
	"ISO",
	"Exposure time",
	"F number",
	"Exposure program",
	"Shutter speed",
	"Exposure bias",
	"Subject distance",
	"Focal length",
	"Focal length in 35mm film",
	"Flash",
	"Flash returned light",
	"Flash mode",
	"Metering mode",
	"Latitude",
	"",
	"Longitude",
	"",
	"Altitude",
	"Precision",

	"Lens F stop minimal",
	"Lens F stop maximal",
	"Lens focal length minimal",
	"Lens focal length maximal",
	"Lens focal plane X resolution",
	"Lens focal plane Y resolution",
	"Lens model",
} );


// ID exif labelů
enum ExifEnum {
	EXIF_WIDTH,
	EXIF_HEIGHT,
	DESCRIPTION,
	MODEL,
	ORIENTATION,
	BITS_PER_SAMPLE,
	SOFTWARE,
	DATE_TIME,
	DATE_TIME_ORIGINAL,
	DATE_TIME_DIGITIZED,
	DATE_TIME_SUBSEC,
	COPYRIGHT,
	ISO,
	EXPOSURE_TIME,
	FNUMBER,
	EXPOSURE_PROGRAM,
	SHUTTER_SPEED,
	EXPOSURE_BIAS,
	SUBJECT_DISTANCE,
	FOCAL_LENGTH,
	FOCAL_LENGTH35,
	FLASH,
	FLAS_RETURNED_LIGHT,
	FLASH_MODE,
	METERING_MODE,
	LATITUDE,
	LATITUDE_DMS,
	LONGITUDE,
	LONGITUDE_DMS,
	ALTITUDE,
	PRECISION,
	LENS_F_STOP_MIN,
	LENS_F_STOP_MAX,
	LENS_FOCAL_MIN,
	LENS_FOCAL_MAX,
	LENS_X_RES,
	LENS_Y_RES,
	LENS_MODEL,

	__EXIF_COUNT__
};



// Záložka se základními informacemi
class BasicInfo : public Tab {
	Q_OBJECT

public:
	 BasicInfo();
	~BasicInfo();

	// Funkci update nutno volat po každé změně obrázku
	void update( Image *img );


public slots:
	void changed();



private:
	// Ukazatel na obrázek
	Image *image;

	QSpacerItem *verticalSpacer;

    // BasicInfoLabels values;
    QVector< QLabel * > basicInfoLabels;
    QVector< QLabel * > exifLabels;

	// AKtualizace informací
	void updateExif( QString imageFileName );
	void updateBasicInfo();

	// Počítadlo barev
	ColorCounterWorker *counter;
};



#endif
