/**
 * @Author: Vaclav Martinka
 * @Date:   30. 03. 2019 (18:15)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: main.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:41)
 * @License: CC BY-SA 3.0
 */



// https://doc.qt.io/qt-5.9/qtwidgets-widgets-imageviewer-example.html


#include <QApplication>
#include <QCommandLineParser>

#include "ImageInfo.hpp"
#include "BasicInfo.hpp"


int main( int argc, char *argv[] ) {
	QApplication app( argc, argv );
	QGuiApplication::setApplicationDisplayName( ImageInfo::tr( "ImageInfo" ) );

	QCommandLineParser cmdParser;
	cmdParser.addHelpOption();
	cmdParser.addPositionalArgument( ImageInfo::tr("[file]"), ImageInfo::tr("Image file to open.") );
	cmdParser.process(QCoreApplication::arguments());

	ImageInfo imageInfo;

	if ( !cmdParser.positionalArguments().isEmpty() && !imageInfo.loadFile( cmdParser.positionalArguments().front() ) ) {
		return -1;
	}

	imageInfo.show();
	return app.exec();
}
