/**
 * @Author: Vaclav Martinka
 * @Date:   30. 03. 2019 (18:15)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: ImageInfo.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:56)
 * @License: CC BY-SA 3.0
 */



#ifndef IMAGEINFO_HPP
#define IMAGEINFO_HPP

#include <QMainWindow>
#include <QApplication>

#include "BasicInfo.hpp"
#include "ImageArea.hpp"
#include "Histograms.hpp"
#include "Tresholding.hpp"
#include "Filtering.hpp"
#include "Noise.hpp"



QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QGroupBox;
class QSplitter;
QT_END_NAMESPACE



#define ABOUT "ABOUT"


// Hlavní okno
class ImageInfo : public QMainWindow {
	Q_OBJECT

public:
	ImageInfo();
	~ImageInfo() override;
	// Načtení obrázku
	bool loadFile( const QString &fileName );


protected:
	// Změna velikosti okna
	void resizeEvent( QResizeEvent *event ) override;

// Funkce menu
private slots:
	void open();

	void paste();
	void grayscale() { imageArea->image()->switchGrayscale(); }
	void restore()   { imageArea->image()->restore(); }

	void about();

	void turnLeft()    { imageArea->turnLeft();  }
	void turnRight()   { imageArea->turnRight(); }
	void zoomIn()      { imageArea->scaleImage( 1.25 ); }
	void zoomOut()     { imageArea->scaleImage( 0.8  ); }
	void normalSize()  { imageArea->normalSize(); }
	void fitToWindow() { imageArea->fitToWindow(); }

private:
	ImageArea  *imageArea;

	QTabWidget *tabWidget;
	QSplitter  *mainWidget;

	void initTabs();
	QGroupBox *initTabBasicInfo();
	void createActions();
	void createMenus();
	void updateActions( bool enable );


	// Menu actions
	QAction *grayscaleAct;
	QAction *restoreAct;

	QAction *turnLeftAct;
	QAction *turnRightAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;
	QAction *fitToWindowAct;

	QAction *aboutAct;

	// FIXME REMOVE
	QLabel *mouse;


public:
	BasicInfo    basicInfo;
	Histograms   histograms;
	ColorCounter colorCounter;
	Tresholding  tresholding;
	Filtering    filtering;
	Noise        noise;


	void mousePressEvent( QMouseEvent *event ) override;
};


#endif
