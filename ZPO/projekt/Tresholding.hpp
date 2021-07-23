/**
 * @Author: Vaclav Martinka
 * @Date:   26. 04. 2019 (12:48)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Tresholding.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:42)
 * @License: CC BY-SA 3.0
 */



#ifndef TRESHOLDING_HPP
#define TRESHOLDING_HPP

#include <QtWidgets>
#include <QRgb>

#include "Tab.hpp"
#include "Image.hpp"


// Záložka pro prahování
class Tresholding : public Tab {
	Q_OBJECT

public:
	 Tresholding();
	~Tresholding();


	void update( Image *img );


private:
	Image *image;

	QColor treshold;
	void updateInputs();


	QSpacerItem *verticalSpacer;

	QPushButton *selectColor;
	QLabel      *selectColorVisualiation;

	QDoubleSpinBox *RInput;
	QDoubleSpinBox *GInput;
	QDoubleSpinBox *BInput;

	QRadioButton *BWModeRadio;
	QRadioButton *BModeRadio;
	QRadioButton *WModeRadio;


	QPushButton *workBtn;



public slots:
	void updateColorPicker();
	void updateColorRInput( double val );
	void updateColorGInput( double val );
	void updateColorBInput( double val );

	void work();

};



#endif
