/**
 * @Author: Vaclav Martinka
 * @Date:   02. 04. 2019 (21:21)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Histograms.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:32)
 * @License: CC BY-SA 3.0
 */


#ifndef HISTOGRAMS_HPP
#define HISTOGRAMS_HPP

#include <QtWidgets>
#include "Tab.hpp"
#include "Image.hpp"


// CONFIG
const QColor histogramBackground( 204, 204, 204 );

// Počet krajních pozic
const uint   EXPOSURE_BORDER = 10;

// Množství pixelů (v procentech), které se nesmí nacházet v hraniční oblasti
const double EXPOSURE_TRESHOLD = 0.50;


// Záložka s histogramy
class Histograms : public Tab {
	Q_OBJECT

public:
	 Histograms();
	~Histograms();

	void update( Image *image );



protected:
	Image *image;

	QColor treshold;
	Image::TresholderMode mode;

	QSpacerItem *verticalSpacer;

	QLabel *infoLabel;
	QPushButton *drawExposureBtn;


	QLabel *RGBLabel;
	QLabel *RLabel;
	QLabel *GLabel;
	QLabel *BLabel;
	QLabel *GrayLabel;

	QImage *RGBHistogram;
	QImage *RHistogram;
	QImage *GHistogram;
	QImage *BHistogram;
	QImage *GrayHistogram;

	void drawHistogram( QVector< int > input, QImage *output, QColor color, bool fill = true, bool clear = true );


public slots:
	void exposureBtnClick() {
		image->tresholder( treshold.rgb(), mode, 0xffff0000, 0xffff0000 );
	}

};


#endif
