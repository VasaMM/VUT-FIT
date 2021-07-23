/**
 * @Author: Vaclav Martinka
 * @Date:   02. 04. 2019 (18:01)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: ImageArea.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (09:20)
 * @License: CC BY-SA 3.0
 */



#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include <QtWidgets>

#include "BasicInfo.hpp"
#include "Image.hpp"


// Oblast s obrázkem
class ImageArea : public QScrollArea {
	Q_OBJECT

public:
	 ImageArea( QSplitter *mainWidget );
	//~ImageArea() {}

	// Nastaví obrázek
	bool setImage( const QString &imageFileName, QString *error = nullptr ) { return img.set( imageFileName, error ); }
	void setImage( const QImage &image ) { img.set( image ); }

	void turnLeft()  { img.turnLeft();  scaleImage( 1.0 ); }
	void turnRight() { img.turnRight(); scaleImage( 1.0 ); }

	Image *image() { return &img; }

	// Změna velikostí
	void normalSize();
	void fitToWindow();
	void scaleImage( double factor );
	void setImageSize( int width, int height );



public slots:
	void changed();
	void loaded();




private:
	Image img;
	QLabel *imageLabel;

	BasicInfo *info;
	QSplitter *mainWidget;

	bool fitted = true;
	double scaleFactor = 1.0;

	int width()  { return mainWidget->sizes()[0]; }
	int height() { return mainWidget->height(); }


	void resizeEvent( QResizeEvent *event ) override;
};



#endif
