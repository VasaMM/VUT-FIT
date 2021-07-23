/**
 * @file resistor.c
 * @author Vaclav Martinka
 * @date 08. 12. 2019 (12:24)
 * @brief 2. projekt do predmetu GUX
 */


// Standard XToolkit and OSF/Motif include files.
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

// Public include files for widgets used in this file.
#include <Xm/ComboBox.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <X11/Xmu/Editres.h>
#include <X11/Xatom.h>


// Common C library include files 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>





/***************************************************
           NEW TYPES (uint, ulong, bool)
 ***************************************************/
typedef unsigned int uint;
typedef unsigned long ulong;


typedef int bool;
#define false 0
#define true  1

typedef enum {COLORS, RESULT} LastEdit;




/***************************************************
                      CONSTANTS
 ***************************************************/
// Minimalni rozmery okna
#define MIN_WIDTH  835 ///< Minimalni sirka
#define MIN_HEIGHT 310 ///< Minimalni vyska

// Maximalni rozmery okna
#define MAX_WIDTH  MIN_WIDTH  ///< Maximalni sirka
#define MAX_HEIGHT MIN_HEIGHT ///< Maximalni vyska

// Rozmery grafiky rezistoru
#define RESISTOR_BACKGROUND 0xc99e7b  ///< Barva pozadi rezistoru
const uint resistorWidth  = 370;  ///< Sirka rezistoru
const uint resistorHeight = 100;  ///< Vyska rezistoru
const uint arcHeight      = 50;   ///< Vyska oblouku na boku
const uint arcWidth       = 70;   ///< Sirka oblouku na boku
const uint wireThickness  = 10;   ///< Tloustka vodice
const uint wireLength     = 220;  ///< Delka vodice 
const uint stripWidth     = 23;   ///< Sirka bareneh prouzku



/***************************************************
            Enums, values and strings
 ***************************************************/
#define NUMBER_SIZE     10   ///< Pocet barev 1. pasku
#define MULTIPLIER_SIZE 10   ///< Pocet barev 4. pasku
#define TOLERANCE_SIZE  8    ///< Pocet barev 5. pasku

/// Vyznam  jednotlivych barevnych pasku
typedef enum {NUMBER_1, NUMBER_2, NUMBER_3, MULTIPLIER, TOLERANCE, __INPUTS_COUNT__} ColorType;


/// Seznam barev
typedef enum                                       {BLACK,    BROWN,    RED,      ORANGE,   YELLOW,   GREEN,    BLUE,     VIOLET,   GREY,     WHITE,    SILVER,   GOLD, __COLORS_COUNT__} ColorId;
/// RGB hodnoty barev
const int ColorValue[ __COLORS_COUNT__ ]         = {0x000000, 0xd2691e, 0xff0000, 0xffa500, 0xffff00, 0x00ff00, 0x0000ff, 0xc71585, 0x808080, 0xffffff, 0xc0c0c0, 0xffd700};
/// Je dana barva definovana na 1., 2. a 3. pasku
const bool NumberDefined[__COLORS_COUNT__]       = {true,     true,     true,     true,     true,     true,     true,     true,     true,     true,     false,    false};
/// Je dana barva definovana na 4. pasku
const bool MultiplierDefined[ __COLORS_COUNT__ ] = {true,     true,     true,     true,     true,     true,     true,     true,     false,    false,    true,     true};
/// Je dana barva definovana na 5. pasku
const bool ToleranceDefined[ __COLORS_COUNT__ ]  = {false,    true,     true,     false,    false,    true,     true,     true,     true,     false,    true,     true};


/// Seznam jednotlivych toleranci v % (t0_5 => +-0,5 %)
typedef enum                                               {  t0_05,      t0_1,       t0_25,      t0_5,      t1,        t2,      t5,       t10, __TOLERANCES_COUNT__} ToleranceListId;
/// Jednotlive tolerance jako string
char* ToleranceListName[ __TOLERANCES_COUNT__ ]          = {"+-0.05 %", "+-0.1 %",  "+-0.25 %", "+-0.5 %", "+-1 %",   "+-2 %", "+-5 %",  "+-10 %"};	// FIXME +-


/// Prevod barvy na hodnotu nasobku
//                                                BLACK  BROWN  RED  ORANGE  YELLOW  GREEN   BLUE     VIOLET    SILVER  GOLD
const float MultiplierValue[ MULTIPLIER_SIZE ] = {1,     10,    100, 1000,   10000,  100000, 1000000, 10000000, 0.01,   0.1};

/// Prevod barvy na konkretni toleranci
//                                                        BROWN  RED  GREEN  BLUE   VIOLET  GREY   SILVER  GOLD
const ToleranceListId ToleranceValue[ TOLERANCE_SIZE ] = {t1,    t2,  t0_5,  t0_25, t0_1,   t0_05, t10,    t5};

/// Stringova jmena jednotlivych barev
char* ColorName[ __COLORS_COUNT__ ] = {"Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White", "Silver", "Gold"};


/// Seznam jednotek pro vysledek
typedef enum {OHM, KILO_OHM, MEGA_OHM, __UNITS_COUNT__} UnitListId;

/// Stringove hodnoty jednotek
char* UnitListName[ __UNITS_COUNT__ ]       = { "R", "kR",   "MR"  };	// FIXME omega

/// Konkretni hodnoty jednotek
const uint UnitListValue[ __UNITS_COUNT__ ] = {  1,  1000, 1000000 };




/***************************************************
                  SHARED VARIABLES
 ***************************************************/
Widget topLevel, drawArea, calculateButton, result, unitList, toleranceList, resistorGraphicLabel;
Widget inputColor[__INPUTS_COUNT__];
GC gc = 0;
Pixmap ResistorGraphicPixmap;

int number_1, number_2, number_3;
float multiplier;
ToleranceListId tolerance;

UnitListId selectedUnit = OHM;
LastEdit lastEdit = COLORS;







/***************************************************
                      CODE
 ***************************************************/

/**
 * @brief Vypocte vyslednou hodnotu rezisoru na zaklade vstupu
 * @param unit Ukazatel na jendotku (ohm/kilo/mega), je nastaven pri vypoctu
 * @return Vysledna hodnota krat \t unit
 */
float getResult(UnitListId* unit) {
	float result;

	result = (100 * number_1 + 10 * number_2 + number_3) * multiplier;

	if (result > UnitListValue[MEGA_OHM]) {
		*unit = MEGA_OHM;
		result /= UnitListValue[MEGA_OHM];
	}
	else if (result > UnitListValue[KILO_OHM]) {
		*unit = KILO_OHM;
		result /= UnitListValue[KILO_OHM];
	}
	else {
		*unit = OHM;
	}

	return result;
}



/**
 * @brief Aktualizuje hodnotu ve vstupnim poli
 * @param value Hodnota rezistoru
 */
void updateResult(float value) {
	char str[20];
	sprintf(str, "%.4g", value);
	
	XtVaSetValues(
		result,
		XmNvalue, str,
		NULL);
}



/**
 * @brief Rozdeli vstupni hodnotu \t input na jednotliva cisla a nasobic. Tedy input = [num1][num2][num3] * multiplier
 * 
 * @param input Vstupni hodnota v ohmech
 * @param num1 Prvni cislo, odpovida prvnimu pasku
 * @param num2 Druhe cislo, odpovida druhemu pasku
 * @param num3 Treti cislo, odpovida tretimu pasku
 * @param multiplier Nasobic
 */
void divideNumber(const float input, int* num1, int* num2, int* num3, float* multiplier) {
	float value = input;
	*multiplier = 1;

	if (value >= 1) {
		while (value >= 1000) {
			value /= 10;
			*multiplier *= 10;
		}
		
		value = ceilf(value);
	}
	// FIXME Urcita nepresnost pro cisla mensi nez 1
	else {
		int intPrefix = ceilf(1000 * input);

		if (intPrefix > 999) {
			*num1 = *num2 = 0;
			*num3 = 1;
			return;
		}
		else if (intPrefix < 10) {
			*num1 = *num2 = *num3 = 0;
			return;
		}
		else if (intPrefix < 100) {
			value = intPrefix / 10;
			*multiplier = 0.01;
		}
		else {
			value = intPrefix / 100;
			*multiplier = 0.1;
		}
	}

	
	*num1 = floorf(value / 100);
	*num2 = floorf((value - 100 * (*num1)) / 10);
	*num3 = floorf(value - 100 * (*num1) - 10 * (*num2));
}



/**
 * @brief Prekresli barevne pasky na rezistoru
 * @param type ID pásku
 * @param colorId ID nové barvy
 */
void updateResistorGraphic(const ColorType type, const ColorId colorId) {
	Display* display = XtDisplay(resistorGraphicLabel);
    uint x = wireLength + arcWidth + 5;
    uint width = stripWidth;

	switch( type ) {
		case NUMBER_1:    break;
		case NUMBER_2:    x += 1.8 * (float)stripWidth;  break;
		case NUMBER_3:    x += 3.6 * (float)stripWidth;  break;
		case MULTIPLIER:  x += 5.4 * (float)stripWidth;  break;
		case TOLERANCE:   x += 8.4 * (float)stripWidth;  width = stripWidth / 3 * 2;  break;
    	default:  return;
	}

	Pixel color = ColorValue[colorId];
	XSetForeground(display, gc, color);
	

	XFillRectangle(display, ResistorGraphicPixmap, gc, x, arcHeight / 2, width, resistorHeight);

	XtVaSetValues(resistorGraphicLabel, XmNlabelType, XmSTRING, NULL);
	XtVaSetValues(resistorGraphicLabel, XmNlabelType, XmPIXMAP, NULL);
}



/**
 * @brief Callback pro stisknuti tlacitka pro vypocet
 * @details Na zaklade naposled upravene hodnoty (nejaky barevny pasek nebo ciselna hodnota)
 *          aktualizuje bud vstupni pole nebo bareve pasky.
 * 
 * @param w Nepouzito
 * @param client_data Nepouzito
 * @param call_data Nepouzito
 */
void calculateButtonCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	if (lastEdit == COLORS) {
		UnitListId unit;
		float value = getResult(&unit);

		updateResult(value);

		XtVaSetValues(
			unitList,
			XmNselectedPosition, unit,
			NULL);

		XtVaSetValues(
			toleranceList,
			XmNselectedPosition, tolerance,
			NULL);
	}
	else if (lastEdit == RESULT) {
		float value = atof(XmTextGetString(result));
		value *= UnitListValue[selectedUnit];
		float multiplier;
		int num1, num2, num3;

		divideNumber(value, &num1, &num2, &num3, &multiplier);

		XtVaSetValues(
			inputColor[NUMBER_1],
			XmNselectedPosition, num1,
			NULL);

		XtVaSetValues(
			inputColor[NUMBER_2],
			XmNselectedPosition, num2,
			NULL);
		
		XtVaSetValues(
			inputColor[NUMBER_3],
			XmNselectedPosition, num3,
			NULL);

		for (size_t i = 0; i < MULTIPLIER_SIZE; ++i) {
			if ( MultiplierValue[i] == multiplier ) {
				XtVaSetValues(
					inputColor[MULTIPLIER],
					XmNselectedPosition, i,
					NULL);

				break;
			}
		}

		for (size_t i = 0; i < TOLERANCE_SIZE; ++i) {
			if ( ToleranceValue[i] == tolerance ) {
				XtVaSetValues(
					inputColor[TOLERANCE],
					XmNselectedPosition, i,
					NULL);

				break;
			}
		}


	}
}



/**
 * @brief Callback pri zmene barvy jednoho z pasku. Pouze si zaznamena novou hodnotu.
 * 
 * @param w Nepouzito
 * @param client_data ID pasku
 * @param call_data Struktura obsahujici pozici nove barvy
 */
void changeColorCallback(Widget w, XtPointer client_data, XtPointer call_data) {
    XmComboBoxCallbackStruct *cb = (XmComboBoxCallbackStruct *) call_data;

	ColorId colorId = 0;
	size_t counter = 0;
	switch( (ColorType)client_data ) {
		case NUMBER_1:
			number_1 = cb->item_position;
			colorId  = number_1;
			break;
		
		case NUMBER_2:
			number_2 = cb->item_position;
			colorId  = number_2;
			break;
		
		case NUMBER_3:
			number_3 = cb->item_position;
			colorId  = number_3;
			break;
		
		case MULTIPLIER:
			multiplier = MultiplierValue[cb->item_position];
			for (size_t i = 0; i < __COLORS_COUNT__; ++i) {
				if (MultiplierDefined[i]) {
					if (counter == cb->item_position) {
						colorId = i;
						break;
					}
					++counter;
				}
			}
			break;
		

		case TOLERANCE:
			tolerance = ToleranceValue [cb->item_position];
			for (size_t i = 0; i < __COLORS_COUNT__; ++i) {
				if (ToleranceDefined[i]) {
					if (counter == cb->item_position) {
						colorId = i;
						break;
					}
					++counter;
				}
			}
			break;
    	
    	default: return;
	}


    updateResistorGraphic( (ColorType)client_data, colorId );
    lastEdit = COLORS;
}



/**
 * @brief Callback pro zmenu jendotky vstupu
 * 
 * @param w Nepouzito
 * @param client_data Nepouzito
 * @param call_data Struktura obsahujici ID nove jendotky
 */
void changeUnitCallback(Widget w, XtPointer client_data, XtPointer call_data) {
    XmComboBoxCallbackStruct *cb = (XmComboBoxCallbackStruct *) call_data;

	selectedUnit = cb->item_position;
	lastEdit = RESULT;
}



/**
 * @brief Callback pro zmenu tolerance vstupu
 * 
 * @param w Nepouzito
 * @param client_data Nepouzito
 * @param call_data Struktura obsahujici ID tolerance
 */
void changeToleranceCallback(Widget w, XtPointer client_data, XtPointer call_data) {
    XmComboBoxCallbackStruct *cb = (XmComboBoxCallbackStruct *) call_data;

	tolerance = cb->item_position;
	lastEdit = RESULT;
}



/**
 * @brief Callback pro zadani noveho vznaku na vstupu. Provadi jeho validaci.
 * 
 * @param w Vstupni widget
 * @param client_data Nepouzito
 * @param call_data Struktura s novym vstupem
 */
void resultVerifyCallback(Widget w, XtPointer client_data, XtPointer call_data) {
    XmTextVerifyCallbackStruct *cb = (XmTextVerifyCallbackStruct *) call_data;
    
    char* old = XmTextGetString(w);
    char* add = cb->text->ptr;

    // Kontrola na desetinou carku
    bool decimal = false;
    for (size_t i = 0; i < strlen(old); ++i) {
    	if (old[i] == '.') {
    		decimal = true;
    		break;
    	}
    }

    for (size_t i = 0; i < cb->text->length; ++i) {
    	// Kontrola na pridani destine carky
    	if (add[i] == ',' || add[i] == '.') {
    		if (decimal) {
		    	cb->doit = false;
		    	return;    			
    		}
    		add[i] = '.';
    		decimal = true;
    		continue;
    	}
    	// Kontrola na psani pouze cisel
    	else if (add[i] < '0' || add[i] > '9' ) {
	    	cb->doit = false;
	    	return;
    	}
    }

	lastEdit = RESULT;
}



/**
 * @brief Nakresli obrys rezistoru a inicializuje potrebne widgety
 * @param parent Rodicovsky widget
 */
void initResistorGraphic(const Widget* parent) {
	Display* display = XtDisplay(*parent);
	Screen*  screen  = XtScreen( *parent);
	
	Widget resistorGraphicRow = XtVaCreateManagedWidget(
		"resistorGraphicRow",
		xmRowColumnWidgetClass,
		*parent,
		NULL);


	// Inicialozuji pixmapu
	uint pixmapWidth  = MIN_WIDTH - 20; // FIXME 20
	uint pixmapHeight = 160;

	ResistorGraphicPixmap = XCreatePixmap(
		display,
		RootWindowOfScreen(screen),
		pixmapWidth,
		pixmapHeight,
		DefaultDepthOfScreen(screen)
	);


	// Vytvorim graficky kontext pro kresleni
    gc = XCreateGC(display, ResistorGraphicPixmap, 0, 0);


	// Vykreslim pozadi
	Pixel background;
	XtVaGetValues (resistorGraphicRow, XmNbackground, &background, NULL);
	XSetForeground(display, gc, background);
    XFillRectangle(display, ResistorGraphicPixmap, gc, 0, 0, pixmapWidth, pixmapHeight);

    /////////////////
    // Postupne nakreslim telo resistoru
    uint x, y;

    // Vykreslim vodice resistoru
	XSetForeground(display, gc, 0x888888);
	XSetLineAttributes(display, gc, wireThickness, LineSolid, CapNotLast, JoinMiter);
	x = 0;
	y = (resistorHeight + arcHeight) / 2;
	XDrawLine(display, ResistorGraphicPixmap, gc, x, y, x + wireLength, y);
	x = wireLength + resistorWidth;
	XDrawLine(display, ResistorGraphicPixmap, gc, x, y, x + wireLength, y);
	x = wireLength;


	// Vykreslim telo
	XSetForeground(display, gc, RESISTOR_BACKGROUND);
	
	// Zakladni kostra
	y = arcHeight / 2;
	XFillRectangle(display, ResistorGraphicPixmap, gc, x, y, resistorWidth, resistorHeight);


	// Levy horni oblouk
	y = 0;
	XFillArc(display, ResistorGraphicPixmap, gc, x, y, arcWidth, arcHeight, 0 << 6, 180 << 6);

	// Levy dolni oblouk
	y = resistorHeight;
	XFillArc(display, ResistorGraphicPixmap, gc, x, y, arcWidth, arcHeight, 180 << 6, 180 << 6);
	
	// Pravy horni oblouk
	x += resistorWidth - arcWidth;
	y = 0;
	XFillArc(display, ResistorGraphicPixmap, gc, x, y, arcWidth, arcHeight, 0 << 6, 180 << 6);

	// Pravy dolni oblouk
	y = resistorHeight;
	XFillArc(display, ResistorGraphicPixmap, gc, x, y, arcWidth, arcHeight, 180 << 6, 180 << 6);
	

    // Vykreslim vyslednou pixmapu
    resistorGraphicLabel = XtVaCreateManagedWidget(
        "resistorGraphicLabel",
        xmLabelWidgetClass,
        resistorGraphicRow,
        XmNlabelType, XmPIXMAP,
        XmNlabelPixmap, ResistorGraphicPixmap,
        NULL);
}



/**
 * @brief Inicializuje vstup pro vyber jedne barvy
 * @param parent Ukazatel na rodicovsky widget
 * @param type ID barevneho pasku
 */
void initColorList(const Widget* parent, ColorType type){
    inputColor[type] = XmCreateDropDownList(*parent, "ColorList", NULL, 0);

	for (size_t i = 0; i < __COLORS_COUNT__; ++i) {
		if (
			(type == NUMBER_1   && NumberDefined[i])     ||
			(type == NUMBER_2   && NumberDefined[i])     ||
			(type == NUMBER_3   && NumberDefined[i])     ||
			(type == MULTIPLIER && MultiplierDefined[i]) ||
			(type == TOLERANCE  && ToleranceDefined[i])
		) {
			XmComboBoxAddItem(inputColor[type], XmStringCreateSimple( ColorName[i] ), i + 1, true);
		}
	}

    XtAddCallback(inputColor[type], XmNselectionCallback, changeColorCallback, (XtPointer)type);

    int size;
    switch (type) {
    	case NUMBER_1:   size = NUMBER_SIZE;      break;
    	case NUMBER_2:   size = NUMBER_SIZE;      break;
    	case NUMBER_3:   size = NUMBER_SIZE;      break;
    	case MULTIPLIER: size = MULTIPLIER_SIZE;  break;
    	case TOLERANCE:  size = TOLERANCE_SIZE;   break;
    	default:         size = 0;
    }

	XtVaSetValues(
		inputColor[type],
		XmNvisibleItemCount, size,
		XmNselectedPosition, 0,
		NULL);

    XtManageChild(inputColor[type]);
}



/**
 * @brief Inicializuje radek s 5 listy pro vyber barvy jednoltivych pasku rezistoru.
 * @param parent Rodicovsky widget
 * @return Inicializovany widge
 */
Widget initColorListRow(const Widget* parent) {
	Widget colorListRow = XtVaCreateManagedWidget(
		"colorListRow",
		xmRowColumnWidgetClass,
		*parent,
		XmNpacking, XmPACK_COLUMN,
		XmNorientation, XmHORIZONTAL,
		NULL);

	initColorList(&colorListRow, NUMBER_1);
	initColorList(&colorListRow, NUMBER_2);
	initColorList(&colorListRow, NUMBER_3);
	initColorList(&colorListRow, MULTIPLIER);
	initColorList(&colorListRow, TOLERANCE);

	return colorListRow;
}



/**
 * @brief Inicializuje radek s vypocetnim tlacitkem
 * @param parent Rodicovsky widget
 * @return Inicializovany widget
 */
void initCalculateRow(const Widget* parent) {
	Widget calculateFrame = XtVaCreateManagedWidget(
		"calculateFrame",
		xmFrameWidgetClass,
		*parent,
		XmNorientation, XmHORIZONTAL,
		// XmNpacking, XmPACK_COLUMN,
		NULL);

	calculateButton = XtVaCreateManagedWidget(
		"Calculate",
		xmPushButtonWidgetClass,
		calculateFrame,
		// XmNalignment, XmALIGNMENT_CENTER ,
		NULL);

	XtAddCallback(calculateButton, XmNactivateCallback, calculateButtonCallback, NULL);
}



/**
 * @brief Inicializuje radek s ciselnym vstupem, vyberem jendotky a tolerance.
 *        Tyto vstupy funguji zaroven i pro vystup.
 * @param parent Rodicovsky widget
 * @return Inicializovany widget
 */
void initNumberRow(const Widget* parent) {
	Widget numberRow = XtVaCreateManagedWidget(
		"numberRow",
		xmRowColumnWidgetClass,
		*parent,
		XmNorientation, XmHORIZONTAL,
		// XmNpacking, XmPACK_COLUMN,
		NULL);


	XtVaCreateManagedWidget(
		"Value:",
		xmLabelWidgetClass,
		numberRow,
		NULL);

	result = XtVaCreateManagedWidget(
		"result",
		xmTextWidgetClass,
		numberRow,
		XmNvalue, "0",
		XmNeditMode, XmSINGLE_LINE_EDIT,
		XmNmaxLength, 20,
		NULL);

    XtAddCallback(result, XmNmodifyVerifyCallback, resultVerifyCallback, NULL);

	
	unitList = XmCreateDropDownList(numberRow, "UnitList", NULL, 0);
	toleranceList = XmCreateDropDownList(numberRow, "ToleranceList", NULL, 0);

	for (size_t i = 0; i < __UNITS_COUNT__; ++i) {
		XmComboBoxAddItem(unitList, XmStringCreateSimple( UnitListName[i] ), i + 1, true);
	}

	for (size_t i = 0; i < __TOLERANCES_COUNT__; ++i) {
		XmComboBoxAddItem(toleranceList, XmStringCreateSimple( ToleranceListName[i] ), i + 1, true);
	}

    XtAddCallback(unitList, XmNselectionCallback, changeUnitCallback, NULL);
    XtAddCallback(toleranceList, XmNselectionCallback, changeToleranceCallback, NULL);

	XtVaSetValues(
		unitList,
		XmNvisibleItemCount, __UNITS_COUNT__,
		XmNselectedPosition, 0,
		NULL);

	XtVaSetValues(
		toleranceList,
		XmNvisibleItemCount, __TOLERANCES_COUNT__,
		XmNselectedPosition, 0,
		NULL);

    XtManageChild(unitList);
    XtManageChild(toleranceList);
}





/**
 * @brief Main funkce
 * @param argc Pocet argumentu
 * @param argv Ukazatel na hodnoty argumentu
 * 
 * @return Navratova hodnota
 */
int main(int argc, char **argv) {
	XtAppContext app_context;

	// Register the default language procedure
	XtSetLanguageProc(NULL, NULL, NULL);

	topLevel = XtVaAppInitialize(
		&app_context,
		"Resistor",
		NULL, 0,
		&argc, argv,
		NULL,
		NULL);

    XtVaSetValues(topLevel, XmNheight, MIN_HEIGHT, NULL);
    XtVaSetValues(topLevel, XmNwidth,  MIN_WIDTH,  NULL);


	Widget mainWindow = XtVaCreateManagedWidget(
		"mainWindow",
		xmMainWindowWidgetClass,
		topLevel,
		NULL);

	Widget frame = XtVaCreateManagedWidget(
		"frame",
		xmFrameWidgetClass,
		mainWindow,
		NULL);

	Widget rowColumn = XtVaCreateManagedWidget(
		"rowColumn",
		xmRowColumnWidgetClass,
		frame,
		XmNorientation, XmVERTICAL,
		NULL);


	initResistorGraphic(&rowColumn);
	initColorListRow(&rowColumn);
	initCalculateRow(&rowColumn);
	initNumberRow(&rowColumn);
	

	XmMainWindowSetAreas(mainWindow, NULL, NULL, NULL, NULL, frame);

	// Spustim prvni vypocet (inicializuji pomocne promenne)
	calculateButtonCallback(NULL, NULL, NULL);


	// Vykreslim okno
	XtRealizeWidget(topLevel);


	// Nastavim rozmery okna
	XSizeHints* sh = XAllocSizeHints();
	sh->flags = PMinSize | PMaxSize;
	sh->min_width  = MIN_WIDTH;
	sh->min_height = MIN_HEIGHT;
	sh->max_width  = MAX_WIDTH;
	sh->max_height = MAX_HEIGHT;
	XSetWMSizeHints(XtDisplay(topLevel), XtWindow(topLevel), sh, XA_WM_NORMAL_HINTS);
	XFree(sh);



	XtAppMainLoop(app_context);

	return 0;
}

