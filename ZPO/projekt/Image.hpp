/**
 * @Author: Vaclav Martinka
 * @Date:   05. 04. 2019 (19:52)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Image.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (09:14)
 * @License: CC BY-SA 3.0
 */



#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <QImage>
#include <QString>
#include <QObject>


// Rozšížený obrázek
class Image : public QObject {
	Q_OBJECT

public:
	// Prázdný konstruktor
	Image() {}

	// Konstruktor z adresy
	Image( const QString &imageFileName, QString *error = nullptr ) { set( imageFileName, error ); }

	// Konstruktor z obrázku
	Image( const QImage &image ) { set( image ); }


	~Image() {
		if ( buffer != nullptr ) delete[] buffer;
	}

	// Nastaví obrázek z adresy
	bool set( const QString &imageFileName, QString *error = nullptr );

	// Nastaví obrázek
	void set( const QImage &image );

	// Načte nová data, ! nemění originální obrázek
	void load( const QImage &image ) { _image = image; emit changed(); }

	// Vrací ukazatel na QImage
	QImage *image()     { return &_image; }

	// Vrací grayscale QImage 32 bit
	QImage  grayscale() { return QImage( ( _image.convertToFormat( QImage::Format_Grayscale8 ) ).convertToFormat( QImage::Format_RGB32 ) ); }

	// Vrací ukazatel na původní data
	QImage *original()  { return &_imageOriginal; }

	// Získá adresu obrázku
	QString fileName()  { return  _imageFileName; }

	// Převede obrázek na grayscale
	void switchGrayscale() { _image = ( _image.convertToFormat( QImage::Format_Grayscale8 ) ).convertToFormat( QImage::Format_RGB32 ); emit changed(); }

	// Obnoví obrízek na původní hodnotu
	void restore() { set( _imageOriginal ); emit changed(); }

	// Otoci obrazek
	// https://www.qtcentre.org/threads/55763-QImage-Rotation-and-Translation-amp-Invert-Specific-Colors
	void turnLeft()  { rotate( -90 ); }
	void turnRight() { rotate( 90 ); }

	void rotate( int angle ) {
		QTransform rotating;
		rotating.rotate( angle );
		_image = _image.transformed(rotating); // Works

		emit changed();
	}


	int width()  { return _image.width(); }
	int height() { return _image.height(); }
	int depth()  { return _image.depth(); }
	int resolution() { return width() * height(); }

	// Otestuje obrázek
	bool isNull() { return _image.isNull(); }

	// Je obrázek na šířku?
	bool landscape() { return _image.width() >= _image.height(); }

	// Získá jeden pixel (pomalé)
	QRgb pixel( int x, int y ) { return _image.pixel( x, y ); }

	// Nastaví jeden pixel (pomalé)
	void setPixel(      int x, int y, QRgb color,   bool noEmit = false ) { _image.setPixel(      x, y, color ); if ( !noEmit ) emit changed(); }
	void setPixelColor( int x, int y, QColor color, bool noEmit = false ) { _image.setPixelColor( x, y, color ); if ( !noEmit ) emit changed(); }

	// Vyvolá signál změny
	void touch() { emit changed(); }

	// Režimy prahování
	enum TresholderMode {
		BLACK_AND_WHITE,
		ONLY_BLACK,
		ONLY_WHITE
	};

	// Provede prahování
	void tresholder( QRgb treshold, TresholderMode mode, QRgb black = 0, QRgb white = 0xffffffff );

	// Použije filtr nad obrázkem
	bool filter( int size, const double *matrix, uint coefficient = 1 );
	// void median( int size );

	// Porovná 2 obrázky, border určuje počet okrajových pixelů, které budou ignorovány
	double diff( Image &img, int border );


	operator bool()    const { return ! _image.isNull(); }
	operator QImage*() { return image(); }
	operator QImage()  { return QImage( *image() ); }



signals:
	void changed();
	void loaded();


private:
	QImage _imageOriginal;
	QImage _image;
	QString _imageFileName;

	unsigned char* buffer = nullptr;

};


#endif
