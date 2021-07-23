/**
 * @Author: Vaclav Martinka
 * @Date:   03. 04. 2019 (23:12)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Tab.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:41)
 * @License: CC BY-SA 3.0
 */



#include <QtWidgets>
#include <cstring>

#include "Tab.hpp"



Tab::Tab() {
	rootWidget       = new QWidget    ( this );
	rootWidgetLayout = new QVBoxLayout( rootWidget );
	scrollArea       = new QScrollArea( rootWidget );
	scrollContent    = new QWidget();
	scrollLayout     = new QVBoxLayout( scrollContent );
	layout           = new QGridLayout();

	scrollLayout->addLayout( layout );
	scrollArea->setWidget( scrollContent );
	scrollArea->setWidgetResizable( true );
	rootWidgetLayout->addWidget( scrollArea );

}


Tab::~Tab() {
	// delete rootWidget;
	// delete rootWidgetLayout;
	// delete scrollArea;
	// delete scrollContent;
	// delete scrollLayout;
	// delete layout;
}



int Tab::addWidgets( QVector< std::string > names, QVector< QLabel ** > labels, int from ) {
	if ( names.size() != labels.size() ) {
		// FIXME
		return 0;
	}

	int i;
	for ( i = 0 ; i < names.size() ; ++i ) {
		layout->addWidget( new QLabel( QString::fromStdString( names[ i ] ), this ), i + from, 0 );
		*labels[ i ] = new QLabel( this );
		layout->addWidget( *labels[ i ], i + from, 1 );
	}

	return i;
}



int Tab::addWidgets( QVector< QWidget * > widgets, int from, int column ) {
	int i;
	for ( i = 0 ; i < widgets.size() ; ++i ) {
		layout->addWidget( widgets[ i ], i + from, column );
	}

	return i;
}


void Tab::addWidget( QWidget *widget, int fromRow, int fromColumn, int rowSpan, int columnSpan, Qt::Alignment alignment ) {
	layout->addWidget( widget, fromRow, fromColumn, rowSpan, columnSpan, alignment );
}
