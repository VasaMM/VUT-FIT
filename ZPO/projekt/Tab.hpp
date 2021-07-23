/**
 * @Author: Vaclav Martinka
 * @Date:   03. 04. 2019 (23:12)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Tab.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:41)
 * @License: CC BY-SA 3.0
 */



#ifndef TAB_HPP
#define TAB_HPP

#include <QtWidgets>
#include <cstring>

// Obecná implementace záložky
class Tab : public QWidget {

protected:
	 Tab();

	int addWidgets( QVector< std::string > names, QVector< QLabel ** > labels, int from = 0 );
	int addWidgets( QVector< QWidget * > widgets, int from = 0, int column = 0 );
	void addWidget( QWidget *widget, int fromRow = 0, int fromColumn = 0, int rowSpan = 1, int columnSpan = 1, Qt::Alignment alignment = 0 );

	QWidget     *rootWidget;
	QVBoxLayout *rootWidgetLayout;
	QScrollArea *scrollArea;
	QWidget     *scrollContent;
	QVBoxLayout *scrollLayout;
	QGridLayout *layout;

public:
	~Tab();
	QWidget *widget() { return rootWidget; }
};


#endif
