/**
 * @Author: Vaclav Martinka
 * @Date:   26. 04. 2019 (12:48)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Filtering.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:30)
 * @License: CC BY-SA 3.0
 */



#ifndef FILTERING_HPP
#define FILTERING_HPP

#include <QtWidgets>
#include <QRgb>

#include "Tab.hpp"
#include "Image.hpp"


// Záložka pro spouštění filtrů
class Filtering : public Tab {
	Q_OBJECT

public:
	 Filtering();
	~Filtering();


	void update( Image *img );


private:
	Image *image;


	QSpacerItem *verticalSpacer;

	QPlainTextEdit *coreInput;
	QDoubleSpinBox *coefficient;

	QPushButton    *workBtn;


public slots:
	void work();
};



#endif
