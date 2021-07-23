/**
 * @Author: Vaclav Martinka
 * @Date:   01. 05. 2019 (17:04)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Noise.hpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 02. 05. 2019 (08:41)
 * @License: CC BY-SA 3.0
 */



#ifndef NOISE_HPP
#define NOISE_HPP

#include <QtWidgets>
#include <QRgb>

#include "Tab.hpp"
#include "Image.hpp"

// Gaussův filtr pro odstranění šumu
const double GAUSSIAN_KERNEL [] = {
	0.00401,  0.005895, 0.007763, 0.009157, 0.009675, 0.009157, 0.007763, 0.005895, 0.00401,
	0.005895, 0.008667, 0.011412, 0.013461, 0.014223, 0.013461, 0.011412, 0.008667, 0.005895,
	0.007763, 0.011412, 0.015028, 0.017726, 0.018729, 0.017726, 0.015028, 0.011412, 0.007763,
	0.009157, 0.013461, 0.017726, 0.020909, 0.022092, 0.020909, 0.017726, 0.013461, 0.009157,
	0.009675, 0.014223, 0.018729, 0.022092, 0.023342, 0.022092, 0.018729, 0.014223, 0.009675,
	0.009157, 0.013461, 0.017726, 0.020909, 0.022092, 0.020909, 0.017726, 0.013461, 0.009157,
	0.007763, 0.011412, 0.015028, 0.017726, 0.018729, 0.017726, 0.015028, 0.011412, 0.007763,
	0.005895, 0.008667, 0.011412, 0.013461, 0.014223, 0.013461, 0.011412, 0.008667, 0.005895,
	0.00401,  0.005895, 0.007763, 0.009157, 0.009675, 0.009157, 0.007763, 0.005895, 0.00401
};


// Záložka pro detekci a redukci šumu
class Noise : public Tab {
	Q_OBJECT

public:
	 Noise();
	~Noise();


	void update( Image *img );


private:
	Image *image;


	QSpacerItem *verticalSpacer;

	QLabel      *noiseLabel;


	QPushButton *workBtn;



public slots:
	void work();

};



#endif
