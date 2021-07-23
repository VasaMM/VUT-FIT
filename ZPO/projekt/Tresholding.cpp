/**
 * @Author: Vaclav Martinka
 * @Date:   26. 04. 2019 (12:48)
 * @Email:  xmarti76@vutbr.cz
 * @Filename: Tresholding.cpp
 * @Last modified by:   Vaclav Martinka
 * @Last modified time: 01. 05. 2019 (17:02)
 * @License: CC BY-SA 3.0
 */



// #include <QtWidgets>

#include "Tresholding.hpp"
#include "Image.hpp"

using namespace std;





Tresholding::Tresholding() : Tab() {
	treshold = QColor( 128, 128, 128 );


	selectColor = new QPushButton( tr( "Select color" ), this );
	addWidget( selectColor, 0, 0, 1, 2 );

	selectColorVisualiation = new QLabel( this );
	addWidget( selectColorVisualiation, 0, 3, 1, 2 );

	addWidget( new QLabel( tr( "Red" ),   this ), 1, 0, 1, 1, Qt::AlignRight );
	addWidget( new QLabel( tr( "Green" ), this ), 1, 2, 1, 1, Qt::AlignRight );
	addWidget( new QLabel( tr( "Blue" ),  this ), 1, 4, 1, 1, Qt::AlignRight );

	RInput = new QDoubleSpinBox( this );
	RInput->setDecimals( 0 );
	RInput->setMaximum( 256.0 );
	addWidget( RInput, 1, 1 );

	GInput = new QDoubleSpinBox( this );
	GInput->setDecimals( 0 );
	GInput->setMaximum( 256.0 );
	addWidget( GInput, 1, 3 );

	BInput = new QDoubleSpinBox( this );
	BInput->setDecimals( 0 );
	BInput->setMaximum( 256.0 );
	addWidget( BInput, 1, 5 );

	BWModeRadio = new QRadioButton( "Black and white", this );
	BModeRadio  = new QRadioButton( "Only black", this );
	WModeRadio  = new QRadioButton( "Only white", this );

	BWModeRadio->setChecked(true);

	addWidget( BWModeRadio, 2, 1 );
	addWidget( BModeRadio, 2, 3 );
	addWidget( WModeRadio, 2, 5 );


	workBtn = new QPushButton( tr( "Tresholding" ), this );
	addWidget( workBtn, 3, 0, 1, 6 );


	verticalSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding );
	layout->addItem( verticalSpacer, 5, 0, -1, -1 );

	connect( selectColor, SIGNAL( released() ), this, SLOT( updateColorPicker() ) );
	connect( RInput, SIGNAL( valueChanged( double ) ), this, SLOT( updateColorRInput( double ) ) );
	connect( GInput, SIGNAL( valueChanged( double ) ), this, SLOT( updateColorGInput( double ) ) );
	connect( BInput, SIGNAL( valueChanged( double ) ), this, SLOT( updateColorBInput( double ) ) );
	updateInputs();

	connect( workBtn, SIGNAL( released() ), this, SLOT( work() ) );
}



Tresholding::~Tresholding() {
	delete verticalSpacer;
}

void Tresholding::update( Image *img ) {
	image = img;
}




void Tresholding::updateColorPicker() {
	treshold = QColorDialog::getColor( treshold );
	updateInputs();
}


void Tresholding::updateColorRInput( double val ) {
	treshold.setRed( static_cast< int >( val ) );
	updateInputs();
}


void Tresholding::updateColorGInput( double val ) {
	treshold.setGreen( static_cast< int >( val ) );
	updateInputs();
}


void Tresholding::updateColorBInput( double val ) {
	treshold.setBlue( static_cast< int >( val ) );
	updateInputs();
}


void Tresholding::updateInputs() {
	QString style = "background-color: rgb(%1, %2, %3);";
	selectColorVisualiation->setStyleSheet( style.arg( treshold.red() ).arg( treshold.green() ).arg( treshold.blue() ) );

	RInput->setValue( treshold.red() );
	GInput->setValue( treshold.green() );
	BInput->setValue( treshold.blue() );
}


void Tresholding::work() {
	Image::TresholderMode mode = Image::BLACK_AND_WHITE;

	if ( BWModeRadio->isChecked() ) {
		mode = Image::BLACK_AND_WHITE;
	}
	else if ( BModeRadio->isChecked() ) {
		mode = Image::ONLY_BLACK;
	}
	else if ( WModeRadio->isChecked() ) {
		mode = Image::ONLY_WHITE;
	}

	image->tresholder( treshold.rgb(), mode );
}
