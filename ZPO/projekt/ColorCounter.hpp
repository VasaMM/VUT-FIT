/**
 * @Author: Vaclav Martinka
 * @Date:   05. 04. 2019 (10:14)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: ColorCounter.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:29)
 * @License: CC BY-SA 3.0
 */



#ifndef COLORCOUNTER_HPP
#define COLORCOUNTER_HPP

#include <QtWidgets>
#include <QThread>
#include <QRgb>
#include <QMap>

#include "Tab.hpp"
#include "Image.hpp"


// Počítadlo barev, bží ve vlastním vlákně
class ColorCounterWorker : public QThread {
	Q_OBJECT

public:
	// Dva režimy (celkový počet barev, množství jedné barvy)
	enum CounterMode { UNIQUE_COLORS, ONE_COLOR };

	ColorCounterWorker() {
		counter = new QMap< QRgb, uint >;
	}

	ColorCounterWorker( Image *img, CounterMode mode = UNIQUE_COLORS, QRgb *color = nullptr, int deviation = 0 ) : ColorCounterWorker() {
		update( img, mode, color, deviation );
	}

	~ColorCounterWorker() {
		if ( result != nullptr ) delete result;
		if ( buffer != nullptr ) delete[] buffer;
	}

	// Po změně obrázku nutno volat update
	void update( Image *img, CounterMode _mode = UNIQUE_COLORS, QRgb *_color = nullptr, int _deviation = 0 ) {
		image     = QImage( *img->image() );
		mode      = _mode;
		deviation = _deviation;

		if ( _color != nullptr ) {
			color = QRgb( *_color );
		}
		else {
			color = QRgb();
		}
	}


	QImage *result = nullptr;



private:
	QMap< QRgb, uint > *counter;
	QImage image;
	CounterMode  mode;
	QRgb         color;
	int          deviation;
	void run();

	unsigned char* buffer = nullptr;


signals:
	void done( QString );
	void done( int );
};




// Záložka s informacemi o barvách
class ColorCounter : public Tab {
	Q_OBJECT

public:
	 ColorCounter();
	~ColorCounter();

	void update( Image *image );

private:
	Image *image;
	ColorCounterWorker *counter;
	bool calculated = false;
	bool showImage  = false;
	int resolution;

	void updateInputs();


	QPushButton *selectColor;
	QLabel      *selectColorVisualiation;

	QDoubleSpinBox *RInput;
	QDoubleSpinBox *GInput;
	QDoubleSpinBox *BInput;

	QDoubleSpinBox *deviation;

	QPushButton *calcBtn;
	QPushButton *showBtn;

	QLabel *totalPixels;
	QLabel *outputPercent;
	QLabel *outputCount;

	QSpacerItem *verticalSpacer;

	QColor selectedColor;




public slots:
	void updateColorPicker();
	void updateColorRInput( double val );
	void updateColorGInput( double val );
	void updateColorBInput( double val );

	void calculate();
	void showInImage();
	void updateOutputs( int val );
};


#endif
