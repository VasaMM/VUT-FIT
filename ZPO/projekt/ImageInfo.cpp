/**
 * @Author: Vaclav Martinka
 * @Date:   30. 03. 2019 (18:15)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: ImageInfo.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (09:04)
 * @License: CC BY-SA 3.0
 */



#include <QtWidgets>

#include "ImageInfo.hpp"
#include "ImageArea.hpp"



ImageInfo::ImageInfo():
	tabWidget( new QTabWidget ),
	mainWidget( new QSplitter )
	{

	imageArea = new ImageArea( mainWidget );


	// Init tabs
	tabWidget->addTab( basicInfo.widget(),    tr( "Basic info" ) );
	tabWidget->addTab( histograms.widget(),   tr( "Histograms" ) );
	tabWidget->addTab( colorCounter.widget(), tr( "Color counter" ) );
	tabWidget->addTab( filtering.widget(),    tr( "Filtering" ) );
	tabWidget->addTab( tresholding.widget(),  tr( "Tresholding" ) );
	tabWidget->addTab( noise.widget(),        tr( "Noise" ) );


	mainWidget->addWidget( imageArea );
	mainWidget->addWidget( tabWidget );

	mainWidget->setCollapsible( 0, false );
	mainWidget->setCollapsible( 1, false );

	// FIXME
	mainWidget->setSizes( { 600, 400 } );


	setCentralWidget( mainWidget );
	mainWidget->show();


	createActions();

	resize( QGuiApplication::primaryScreen()->availableSize() * 4 / 5 );

	// FIXME
	// mouse = new QLabel( "mouse", this );
	// this->statusBar()->addWidget(mouse);
 //    setMouseTracking(true);
 //    mainWidget->setMouseTracking(true);
}



ImageInfo::~ImageInfo() {
	delete imageArea;
}



void ImageInfo::mousePressEvent( QMouseEvent *event ) {
	(void)event;
	// QString text;
    // text = QString("%1 X %2").arg(event->pos().x()).arg(event->pos().y());
    // /** Update the info text */
    // mouse->setText(text);
}



bool ImageInfo::loadFile( const QString &fileName ) {
	QString error;
	if ( ! imageArea->setImage( fileName, &error ) ) {
		QMessageBox::information( this, QGuiApplication::applicationDisplayName(), tr( "Cannot load %1: %2" ).arg( QDir::toNativeSeparators( fileName ), error ) );
		return false;
	}
	else {
		basicInfo.update(    imageArea->image() );
		histograms.update(   imageArea->image() );
		colorCounter.update( imageArea->image() );
		filtering.update(    imageArea->image() );
		tresholding.update(  imageArea->image() );
		noise.update(        imageArea->image() );
		updateActions( static_cast< bool >( imageArea->image() ) );
		setWindowFilePath( fileName );

		const QString message = tr( "Opened \"%1\", %2x%3, Depth: %4" ).arg( QDir::toNativeSeparators( fileName ) ).arg( imageArea->image()->width() ).arg( imageArea->image()->height() ).arg( imageArea->image()->depth() );
		statusBar()->showMessage( message );

		return true;
	}
}



static void initializeImageFileDialog( QFileDialog &dialog, QFileDialog::AcceptMode acceptMode ) {
	static bool firstDialog = true;

	if ( firstDialog ) {
		firstDialog = false;
		const QStringList picturesLocations = QStandardPaths::standardLocations( QStandardPaths::PicturesLocation );
		dialog.setDirectory( picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last() );
	}

	QStringList mimeTypeFilters;
	const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
	foreach ( const QByteArray &mimeTypeName, supportedMimeTypes ) {
		mimeTypeFilters.append(mimeTypeName);
	}
	mimeTypeFilters.sort();

	dialog.setMimeTypeFilters( mimeTypeFilters );
	dialog.selectMimeTypeFilter( "image/jpeg" );
	if ( acceptMode == QFileDialog::AcceptSave ) {
		dialog.setDefaultSuffix( "jpg" );
	}
}



void ImageInfo::open() {
	QFileDialog dialog( this, tr( "Open File" ) );
	initializeImageFileDialog( dialog, QFileDialog::AcceptOpen );

	while ( dialog.exec() == QDialog::Accepted && !loadFile( dialog.selectedFiles().first() ) ) {}
}



#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage() {
	if ( const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData() ) {
		if ( mimeData->hasImage() ) {
			const QImage image = qvariant_cast< QImage >( mimeData->imageData() );
			if ( !image.isNull() ) {
				return image;
			}
		}
	}
	return QImage();
}
#endif



void ImageInfo::paste() {
#ifndef QT_NO_CLIPBOARD
	const QImage newImage = clipboardImage();
	if ( newImage.isNull() ) {
		statusBar()->showMessage( tr( "No image in clipboard" ) );
	}
	else {
		imageArea->setImage( newImage );
		setWindowFilePath( QString() );
		const QString message = tr( "Obtained image from clipboard, %1x%2, Depth: %3" ).arg( newImage.width() ).arg( newImage.height() ).arg( newImage.depth() );
		statusBar()->showMessage( message );
	}
#endif
}




void ImageInfo::about() {
	QMessageBox::about(
		this,
		tr("About Image Info"),
		tr( ABOUT )
	);
}



void ImageInfo::createActions() {
	QMenu *fileMenu = menuBar()->addMenu( tr( "&File" ) );

	QAction *openAct = fileMenu->addAction( tr( "&Open..." ), this, &ImageInfo::open );
	// openAct->setShortcut( QKeySequence::Open );
	openAct->setShortcuts( { tr( "Ctrl+O" ), tr( "O" ) } );

	QAction *exitAct = fileMenu->addAction( tr( "E&xit" ), this, &QWidget::close );
	exitAct->setShortcut( tr( "Esc" ) );


	QMenu *editMenu = menuBar()->addMenu( tr( "&Edit" ) );

	QAction *pasteAct = editMenu->addAction( tr( "&Paste" ), this, &ImageInfo::paste );
	pasteAct->setShortcut( QKeySequence::Paste );

	QAction *grayscaleAct = editMenu->addAction( tr( "&Grayscale" ), this, &ImageInfo::grayscale );
	grayscaleAct->setShortcut( tr( "g" ) );

	QAction *restoreAct = editMenu->addAction( tr( "Res&tore" ), this, &ImageInfo::restore );
	restoreAct->setShortcut( tr( "t" ) );


	QMenu *viewMenu = menuBar()->addMenu( tr( "&View" ) );

	turnLeftAct = viewMenu->addAction( tr( "Turn &Left" ), this, &ImageInfo::turnLeft );
	turnLeftAct->setShortcut( tr( "l" ) );
	turnLeftAct->setEnabled( false );

	turnRightAct = viewMenu->addAction( tr( "Turn &Right" ), this, &ImageInfo::turnRight );
	turnRightAct->setShortcut( tr( "r" ) );
	turnRightAct->setEnabled( false );

	zoomInAct = viewMenu->addAction( tr( "Zoom &In (25%)" ), this, &ImageInfo::zoomIn );
	zoomInAct->setShortcut( tr( "+" ) );
	zoomInAct->setEnabled( false );

	zoomOutAct = viewMenu->addAction( tr( "Zoom &Out (25%)" ), this, &ImageInfo::zoomOut );
	zoomOutAct->setShortcut( tr( "-" ) );
	zoomOutAct->setEnabled( false );

	normalSizeAct = viewMenu->addAction( tr( "&Normal Size" ), this, &ImageInfo::normalSize );
	normalSizeAct->setShortcut( tr( "0" ) );
	normalSizeAct->setEnabled( false );

	fitToWindowAct = viewMenu->addAction( tr( "&Fit to Window" ), this, &ImageInfo::fitToWindow );
	fitToWindowAct->setShortcut( tr( "*" ) );
	fitToWindowAct->setEnabled( false );


	QMenu *helpMenu = menuBar()->addMenu( tr( "&Help" ) );

	aboutAct = helpMenu->addAction( tr( "&About" ), this, &ImageInfo::about );
	aboutAct->setShortcut( tr( "F1" ) );
	helpMenu->addAction( tr( "About &Qt" ), &QApplication::aboutQt );
}



void ImageInfo::updateActions( bool enable ) {
	   turnLeftAct->setEnabled( enable );
	  turnRightAct->setEnabled( enable );
	     zoomInAct->setEnabled( enable );
	    zoomOutAct->setEnabled( enable );
	 normalSizeAct->setEnabled( enable );
	fitToWindowAct->setEnabled( enable );
}



void ImageInfo::resizeEvent( QResizeEvent *event ) {
	//updateBasicInfo();
	QMainWindow::resizeEvent(event);
}
