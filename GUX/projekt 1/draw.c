/**
 * @file draw.c
 * @author Vaclav Martinka
 * @date 30. 11. 2019 (15:17)
 * @brief 1. projekt do predmetu GUX
 */


// Standard XToolkit and OSF/Motif include files.
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

// Public include files for widgets used in this file.
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/MessageB.h>
#include <Xm/Protocols.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/SSpinB.h>
#include <Xm/ToggleB.h>
#include <X11/Xmu/Editres.h>


// Common C library include files 
#include <stdio.h>
#include <stdlib.h>





/***************************************************
           NEW TYPES (uint, ulong, bool)
 ***************************************************/
typedef unsigned int uint;
typedef unsigned long ulong;

typedef ulong ColorId;

typedef int bool;
#define false 0
#define true  1




/***************************************************
                      CONSTANTS
 ***************************************************/
// Size of color square
#define SQUARE_WIDTH 16

#define DRAWING_AREA_BACKGROUND 0xffffff




/***************************************************
            Enums, values and strings
 ***************************************************/
const char* toolsNames[] = {"Point", "Line", "Rectangle", "Ellipse"};
typedef enum {POINT, LINE, RECTANGLE, ELLIPSE, __TOOLS_COUNT__} ToolId;


#define __COLORS_COUNT__ sizeof(colorsValues) / sizeof(colorsValues[0])
#define WHITE 0
#define BLACK 1

const Pixel colorsValues[] = {
	0xffffff, 0x000000, 0x444444, 0x888888,
	0x000088, 0x0000ff, 0x008800, 0x008888, 0x0088ff, 0x00ff00, 0x00ff88, 0x00ffff,
	0x880000, 0x880088, 0x8800ff, 0x888800, 0x8888ff, 0x88ff00, 0x88ff88, 0x88ffff,
	0xff0000, 0xff0088, 0xff00ff, 0xff8800, 0xff8888, 0xff88ff, 0xffff00, 0xffff88};


const char* selectedColorsNames[] = {"Foreground line", "Foreground area", "Background line", "Background area" };
typedef enum {LINE_FOREGROUND, AREA_FOREGROUND, LINE_BACKGROUND, AREA_BACKGROUND, __SELECTED_COLORS_COUNT__} SelectedColorId;




/***************************************************
                  SHARED VARIABLES
 ***************************************************/
// GC used for final drawing
GC drawGC = 0;

// GC used for drawing current position
GC inputGC = 0;


// Input points
int x1, y1, x2, y2;

// Input mouse button state
int button_pressed = 0;


///////////////////////////////////////
// Selected values and defaults values
ColorId selectedColors[__SELECTED_COLORS_COUNT__] = {BLACK, BLACK, WHITE, WHITE};
ToolId  selectedTool  = LINE;
uint    selectedWidth = 1;


///////////////////////////////////////
// Widgets
Widget topLevel, exitQuestion;
Widget toolsButtons[__TOOLS_COUNT__];
Widget colorsButtons[__COLORS_COUNT__];
Widget selectedColorsButtons[__SELECTED_COLORS_COUNT__];
Widget widthInput, fillButton, dashedLineButton, dashedFillButton;

Atom wm_delete;

Pixmap selectedColorsPixmaps[4];








/***************************************************
                      CODE
 ***************************************************/
/**
 * @brief Callback pro odpoved na ukoncovaci dialog
 * 
 * @param w Nepouzito
 * @param client_data Ukoncit ano/ne
 * @param call_data Nepouzito
 */
void exitQuestionCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	if ( client_data == 0 ) {
		exit(0);
	}
}



/**
 * @brief Callback pro volani ukonceni aplikace
 * @param w Nepouzito
 * @param client_data Nepouzito
 * @param call_data Nepouzito
 */
void quitCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	XtManageChild(exitQuestion);
}



/**
 * @brief Vyplni ctvercovou pixmapu jednou barvou
 * @param pixmap Dana pixmapa
 * @param color Vysledna barva
 */
void fillSquarePixmap(const Pixmap* pixmap, const Pixel color) {
	Display* display = XtDisplay(topLevel);
	GC gc = XCreateGC(display, *pixmap, 0, NULL);
	XSetForeground(display, gc, color);
	XFillRectangle(display, *pixmap, gc, 0, 0, SQUARE_WIDTH, SQUARE_WIDTH);
}



/**
 * @brief Inicializuje ctercovou pixmapu
 * @param color Vychozi barva
 * @return Vytvorena pixmapa
 */
Pixmap makeSquarePixmap(const Pixel color) {
	Pixmap pixmap = XCreatePixmap(
		XtDisplay(topLevel),
		RootWindowOfScreen(XtScreen(topLevel)),
		SQUARE_WIDTH,
		SQUARE_WIDTH,
		DefaultDepthOfScreen(XtScreen(topLevel))
	);

	fillSquarePixmap(&pixmap, color);

	return pixmap;
}



/**
 * @brief Nastavi barvu a styl pro cary
 * 
 * @param display Aktualni display
 * @param gc Graficky kontext
 */
inline void setLineColour(Display* display, GC* gc) {
	int lineStyle = XmToggleButtonGetState(dashedLineButton) ? LineDoubleDash : LineSolid;
	XSetLineAttributes(display, *gc, selectedWidth, lineStyle, CapRound, JoinMiter);
	
	XSetForeground(display, *gc, colorsValues[selectedColors[LINE_FOREGROUND]]);
	XSetBackground(display, *gc, colorsValues[selectedColors[LINE_BACKGROUND]]);
}



/**
 * @brief Nastavi barvu a styl pro plochy
 * 
 * @param display Aktualni display
 * @param gc Graficky kontext
 */
inline void setFillColour(Display* display, GC* gc) {
	// int fillStyle = XmToggleButtonGetState(dashedFillButton) ? FillOpaqueStippled : FillSolid;
	// XSetFillStyle(display, *gc, fillStyle);

	XSetForeground(display, *gc, colorsValues[selectedColors[AREA_FOREGROUND]]);
	// XSetBackground(display, *gc, colorsValues[selectedColors[AREA_BACKGROUND]]);
}



/**
 * @brief Normalizuje souradnice x a y pro vsechny 4 smery
 * 
 * @param x Vysledna souradnice x
 * @param y Vysledna souradnice y
 * @param width Vysledna sirka objektu
 * @param height Vysledna vyska objektu
 */
inline void normalizePosition(uint* x, uint* y, uint* width, uint* height) {
	if (x2 > x1) {
		*x = x1;
		*width = x2 - x1;
		
		if (y2 > y1) {
			*y = y1;
			*height = y2 - y1;
		}
		else {
			*y = y2;
			*height = y1 - y2;
		}
	}
	else {
		*x = x2;
		*width = x1 - x2;
		if (y2 > y1) {
			*y = y1;
			*height = y2 - y1;
		}
		else {
			*y = y2;
			*height = y1 - y2;
		}
	}
}



/**
 * @brief Vykresli objekt v zavislosti na aktualne zvolenem nastroji, barvach a vlastnostech
 * 
 * @param w Rodicovsky widget
 * @param final Prvek bude vykreslen do ciloveho platna
 */
void drawObject(const Widget* w, bool final) {
	Display* display = XtDisplay(topLevel);
	Window   window  = XtWindow(*w);
	GC*      gc      = final ? &drawGC : &inputGC;



	// Normalizuji hodnoty x a y + vypoctu sirku a vysku
	uint x, y, width, height;
	normalizePosition(&x, &y, &width, &height);

	// Kreslim vybrany objekt
	switch (selectedTool) {
		case POINT:
			if (selectedWidth == 0) { return; }
			else {
				setLineColour(display, gc);
				
				if (selectedWidth == 1) {
					XDrawPoint(display, window, *gc, x2, y2);
				}
				else {
					XFillArc(display, window, *gc, x2 - (selectedWidth >> 1), y2 - (selectedWidth >> 1), selectedWidth, selectedWidth, 0, 360 << 6);
				}
			}
			
			return;

		case LINE:
			if (selectedWidth != 0) {
				setLineColour(display, gc);
				XDrawLine(display, window, *gc, x1, y1, x2, y2);
			}
			return;
		
		case RECTANGLE:
			if(XmToggleButtonGetState(fillButton)) {
				setFillColour(display, gc);
				XFillRectangle(display, window, *gc, x, y, width, height);
			}
			if (selectedWidth != 0) {
				setLineColour(display, gc);
				XDrawRectangle(display, window, *gc, x, y, width, height);
			}
			return;
		
		case ELLIPSE:
			if(XmToggleButtonGetState(fillButton)) {
				setFillColour(display, gc);
				XFillArc(display, window, *gc, x1 - width, y1 - height, width << 1, height << 1, 0, 360 << 6);
			}
			if (selectedWidth != 0) {
				setLineColour(display, gc);
				XDrawArc(display, window, *gc, x1 - width, y1 - height, width << 1, height << 1, 0, 360 << 6);
			}
			return;

		default: return;
	}
}



/**
 * @brief Obstarava prekreslovani plochy pri zmene pozice kurzoru
 * 
 * @param w Rodicovsky widget
 * @param client_data Nepouzito
 * @param event Ukazatel a udalost (obshauje souradnice)
 * @param cont Nepouzito
 */
void ChangeMousePositionEventHandler(Widget w, XtPointer client_data, XEvent *event, Boolean *cont) {
	if (button_pressed) {
		if (!inputGC) {
			inputGC = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
			XSetFunction(XtDisplay(w), inputGC, GXequiv);
			XSetPlaneMask(XtDisplay(w), inputGC, ~0);
		}
		
		if (button_pressed > 1) {
			// Erase previous position
			drawObject(&w, false);
		}
		else {
			// Remember first MotionNotify 
			button_pressed = 2;
		}

		x2 = event->xmotion.x;
		y2 = event->xmotion.y;

		drawObject(&w, false);
	}
}



/**
 * @brief Callback pro kresleni objektu
 * 
 * @param w Prvek
 * @param client_data Nepouzito
 * @param call_data Predana data
 */
void drawObjectCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	XmDrawingAreaCallbackStruct *d = (XmDrawingAreaCallbackStruct*) call_data;

	switch (d->event->type) {
		case ButtonPress:
			if (d->event->xbutton.button == Button1) {
				button_pressed = 1;
				x1 = d->event->xbutton.x;
				y1 = d->event->xbutton.y;
			}
			break;

		case ButtonRelease:
			if (d->event->xbutton.button == Button1) {
				button_pressed = 0;

				if (!drawGC) {
					drawGC = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
				}

				x2 = d->event->xbutton.x;
				y2 = d->event->xbutton.y;
				drawObject(&w, true);
			}
			break;
	}
}



/**
 * @brief Callback pro mazaci tlacitko
 * 
 * @param w Nepouzito
 * @param client_data Adresa vykreslovaci adresy
 * @param call_data Nepouzito
 */
void clearCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	XClearWindow(XtDisplay((Widget)client_data), XtWindow((Widget)client_data));
}



/**
 * @brief Callback pro zmenu kresliciho nastroje
 * 
 * @param w Nepouzito
 * @param client_data Zvoleny nastroj
 * @param call_data Nepouzito
 */
void changeToolCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	selectedTool = (long)client_data;
}



/**
 * @brief Callback pro zmenu tloustky cary
 * 
 * @param w Nepouzito
 * @param client_data Nepouzito
 * @param call_data Predana data (vcetne nove hodnoty)
 */
void changeWidthCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	selectedWidth = atoi(XmCvtXmStringToCT(((XmSpinBoxCallbackStruct*)call_data)->value));
}



/**
 * @brief Callback pro zmenu aktualne zvoleneho typu barvy
 * 
 * @param w Nepouzito
 * @param client_data ID aktualne zvoleneho typu barvy
 * @param call_data Nepouzito
 */
void changeSelectedColorCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	for (SelectedColorId i = 0; i < __SELECTED_COLORS_COUNT__; ++i) {
		if (i != (SelectedColorId)client_data) {
			XtVaSetValues(selectedColorsButtons[i], XmNset, False, NULL);
		}
	}
}



/**
 * @brief Funkce provede zmenu barvy pro aktualne zvoleny typ barvy
 * 
 * @param w Rodicovsky prvek
 * @param pixmap Pixmapa
 * @param color Nova barva
 * @param id ID meneneho typu barvy
 */
void doChangeOfColor(ColorId color, SelectedColorId id) {
	selectedColors[id] = color;
	fillSquarePixmap(&selectedColorsPixmaps[id], colorsValues[color]);

	XtVaSetValues(selectedColorsButtons[id], XmNlabelType, XmSTRING, NULL);
	XtVaSetValues(selectedColorsButtons[id], XmNlabelType, XmPIXMAP, NULL);
}



/**
 * @brief Callback pro vyber nove barvy
 * 
 * @param w Nepouzito
 * @param client_data ID nove barvy
 * @param call_data Nepouzito
 */
void changeColorCallback(Widget w, XtPointer client_data, XtPointer call_data) {
	for (SelectedColorId i = 0; i < __SELECTED_COLORS_COUNT__; ++i)	{
		if (XmToggleButtonGetState(selectedColorsButtons[i])) {
			doChangeOfColor((ColorId)client_data, i);
		}	
	}
}



/**
 * @brief Naplni widget tlacitky (radioBoxy)
 * 
 * @param radioBox Rodicovsky radio box
 * @param buttons Pole, do ktereho se budou ukladat vytvorena tlacitka, musi byt jiz alokovane
 * @param names Pole se jmeny jednotlivych tlacitek
 * @param buttonsCount Pocet tlacitek
 * @param callback Spolecny callback pro sechna tlacitka, muze byt i NULL
 */
void fillRadioBox(const Widget* radioBox, Widget* buttons, const char* names[], const size_t buttonsCount, XtCallbackProc callback ) {
	// Naplnim ho daty
	for (size_t i = 0; i < buttonsCount; ++i) {
		buttons[i] = XtVaCreateManagedWidget(
			names ? names[i] : "unknown",
			xmToggleButtonWidgetClass,
			*radioBox,
			NULL);
		if (callback != NULL) {
			XtAddCallback(buttons[i], XmNvalueChangedCallback, callback, (XtPointer)i);
		}
	}
}



/**
 * @brief Vytvori ohranicujici prvek s popiskou
 * 
 * @param parent Rodicovsky prvek
 * @param name Nazev
 * @param labelContent Popisek
 * @return Vytvoreny prvek
 */
Widget makeFrame(const Widget* parent, const char* name, const char* labelContent) {
	// Ramecek
	Widget frame = XtVaCreateManagedWidget(
		name,
		xmFrameWidgetClass,
		*parent,
		NULL);

	// Jeho popisek
	XtVaCreateManagedWidget(
		labelContent,
		xmLabelWidgetClass,
		frame,
		XmNchildType, XmFRAME_TITLE_CHILD,
		NULL);

	return frame;
}



/**
 * @brief Vytvori blok se seznamem nastroju
 * @param parent Rodicovsky prvek
 */
void initToolsFrame(const Widget* parent) {
	// Obalovaci prvek s popisem
	Widget toolsFrame = makeFrame(parent, "toolsFrame", "Tools");

	Widget radioBox = XtVaCreateManagedWidget(
		"radioBox",
		xmRowColumnWidgetClass,
		toolsFrame,
		XmNradioBehavior, True,
		XmNisHomogeneous, True,
		XmNentryClass, xmToggleButtonWidgetClass,
		NULL);

	// Naplneni tlacitky
	fillRadioBox(&radioBox, toolsButtons, toolsNames, __TOOLS_COUNT__, changeToolCallback);

	// Nastavim implicitni nastroj
	XtVaSetValues(
		toolsButtons[selectedTool],
		XmNset, True,
		NULL);
}



/**
 * @brief Vytvori blok pro nastaveni sirky
 * @param parent Rodicovsky prvek
 */
void initWidthFrame(const Widget* parent) {
	// Obalovaci prvek s popisem
	Widget widthFrame = makeFrame(parent, "widthFrame", "Width");

	widthInput = XtVaCreateManagedWidget(
        "widthInput",
        xmSimpleSpinBoxWidgetClass,
        widthFrame,
		XmNshadowThickness, 0,
        XmNspinBoxChildType, XmNUMERIC,
        XmNminimumValue, 0,
        XmNmaximumValue, 20,
        XmNposition, selectedWidth,
        // XmNincrementValue, 2,
        XmNeditable, False,
        XmNcolumns, 5,
        XmNwrap, False,
        XmNcursorPositionVisible, False,
        NULL);

		XtAddCallback(widthInput, XmNvalueChangedCallback, changeWidthCallback, 0);
}



/**
 * @brief Vytvori blok pro vyber kreslicich stylu
 * @param parent Rodicovsky prvek
 */
void initStyleFrame(const Widget* parent) {
	// Obalovaci prvek s popisem
	Widget styleFrame = makeFrame(parent, "styleFrame", "Painting style");

	Widget styleFrameContainer = XtVaCreateManagedWidget(
		"styleFrameContainer",
		xmRowColumnWidgetClass,
		styleFrame,
		XmNorientation, XmVERTICAL,
		NULL);

	fillButton = XtVaCreateManagedWidget(
		"Fill object",
		xmToggleButtonWidgetClass,
		styleFrameContainer,
		XmNindicatorOn , XmINDICATOR_CHECK_BOX,
		NULL);

	dashedLineButton = XtVaCreateManagedWidget(
		"Dashed line",
		xmToggleButtonWidgetClass,
		styleFrameContainer,
		XmNindicatorOn , XmINDICATOR_CHECK_BOX,
		NULL);

	dashedFillButton = XtVaCreateManagedWidget(
		"Dashed fill",
		xmToggleButtonWidgetClass,
		styleFrameContainer,
		XmNindicatorOn , XmINDICATOR_CHECK_BOX,
		XmNsensitive, False,
		NULL);
}



/**
 * @brief Vytvori blok s vyzobrazenim prave zvolenych barev
 * @param parent Rodicovsky prvek
 * @return Vytvoreny blok
 */
Widget makeSelectedColorsContainer(const Widget *parent) {
	for (SelectedColorId i = 0; i < __SELECTED_COLORS_COUNT__; ++i)	{
		selectedColorsPixmaps[i] = makeSquarePixmap(colorsValues[selectedColors[i]]);
	}

	Widget selectedColorsContainer = XtVaCreateManagedWidget(
		"selectedColorsContainer",
		xmRowColumnWidgetClass,
		*parent,
		XmNorientation, XmVERTICAL,
		XmNpacking, XmPACK_COLUMN,
		XmNnumColumns, 2,
		NULL);

	for (SelectedColorId i = 0; i < __SELECTED_COLORS_COUNT__; ++i) {
		XtVaCreateManagedWidget(
			selectedColorsNames[i],
			xmLabelWidgetClass,
			selectedColorsContainer,
			NULL);

		selectedColorsButtons[i] = XtVaCreateManagedWidget(
			selectedColorsNames[i], // maybe add "_btn" ?
			xmToggleButtonWidgetClass,
			selectedColorsContainer,
			XmNindicatorOn, False,
			XmNfillOnSelect, True,
			XmNshadowThickness, 2,
			XmNlabelType, XmPIXMAP,
			XmNselectPixmap, selectedColorsPixmaps[i],
			NULL);
		XtAddCallback(selectedColorsButtons[i], XmNvalueChangedCallback, changeSelectedColorCallback, (XtPointer)i);
	}

	return selectedColorsContainer;
}



/**
 * @brief Inicializuje blok pro vyber barev
 * @param parent Rodicovsky prvek
 */
void initColorsFrame(const Widget* parent) {
	// Vyrobim si obalovaci raecek s popisem
	Widget colorsFrame = makeFrame(parent, "colorsFrame", "Colors");

	// Do nej budu vkladat sloupce
	Widget horizontalContainer = XtVaCreateManagedWidget(
		"horizontalContainer",
		xmRowColumnWidgetClass,
		colorsFrame,
		XmNorientation, XmHORIZONTAL,
		NULL);

	// Prvni sloupec bude obsahovat prave zvolene barvy
	makeSelectedColorsContainer(&horizontalContainer);

	// Druhy sloupec bude obsahovat vyber barev
	Widget radioBox = XtVaCreateManagedWidget(
		"radioBox",
		xmRowColumnWidgetClass,
		horizontalContainer,
		XmNorientation, XmHORIZONTAL,
		XmNpacking, XmPACK_COLUMN,
		XmNnumColumns, 4,
		XmNradioBehavior, True,
		XmNisHomogeneous, True,
		XmNentryClass, xmToggleButtonWidgetClass,
		NULL);
	fillRadioBox(&radioBox, colorsButtons, NULL, __COLORS_COUNT__, changeColorCallback);


	for (size_t i = 0; i < __COLORS_COUNT__; ++i) {
		Pixmap tmpPixmap = makeSquarePixmap(colorsValues[i]);

		XtVaSetValues(
			colorsButtons[i],

			XmNindicatorOn, False,
			XmNlabelType, XmPIXMAP,
			XmNselectPixmap, tmpPixmap,
			NULL);
	}
}



/**
 * @brief Inicializuji toolbar
 * @param parent Rodicovsky prvek
 * @return Inicializovany toolbar
 */
Widget initToolbar(const Widget* parent) {
	Widget toolbar = XtVaCreateManagedWidget(
		"toolbar",
		xmRowColumnWidgetClass,
		*parent,
		XmNentryAlignment, XmALIGNMENT_CENTER,
		XmNorientation, XmHORIZONTAL,
		NULL);

	initToolsFrame(&toolbar);
	initWidthFrame(&toolbar);
	initStyleFrame(&toolbar);
	initColorsFrame(&toolbar);

	return toolbar;
}



/**
 * @brief Main funkce
 * @param argc Pocet argumentu
 * @param argv Ukazatel na hodnoty argumentu
 * 
 * @return Navratova hodnota
 */
int main(int argc, char **argv)
{
	XtAppContext app_context;
	Widget mainWindow, frame, rowColumn, quitButton, clearButton, drawArea;


	// Register the default language procedure
	XtSetLanguageProc(NULL, (XtLanguageProc)NULL, NULL);


	char *fall[] = {
		"*exitQuestion.dialogTitle: Konec aplikace?",
		"*exitQuestion.messageString: Konec aplikace?",
		"*exitQuestion.okLabelString: Ano",
		"*exitQuestion.cancelLabelString: Ne",
		"*exitQuestion.messageAlignment: XmALIGNMENT_CENTER",
		NULL
	};


	topLevel = XtVaAppInitialize(
		&app_context,   /* Application context */
		"Draw",         /* Application class */
		NULL, 0,        /* command line option list */
		&argc, argv,    /* command line args */
		fall,
		XmNdeleteResponse, XmDO_NOTHING,
		NULL);          /* terminate varargs list */


	exitQuestion = XmCreateQuestionDialog(
		topLevel,
		"exitQuestion",
		NULL,
		0);

	XtUnmanageChild(XmMessageBoxGetChild(exitQuestion, XmDIALOG_HELP_BUTTON));
	XtAddCallback(exitQuestion, XmNokCallback, exitQuestionCallback, (XtPointer)0);
	XtAddCallback(exitQuestion, XmNcancelCallback, exitQuestionCallback, (XtPointer)1);

	wm_delete = XInternAtom(XtDisplay(topLevel), "WM_DELETE_WINDOW", False);
	XmAddWMProtocolCallback(topLevel, wm_delete, quitCallback, NULL);
	XmActivateWMProtocol(topLevel, wm_delete);


	XtVaSetValues(
		exitQuestion,
		XmNdialogStyle,
		XmDIALOG_FULL_APPLICATION_MODAL,
		NULL);


	mainWindow = XtVaCreateManagedWidget(
		"mainWindow",                 /* widget name */
		xmMainWindowWidgetClass,   /* widget class */
		topLevel,                     /* parent widget*/
		XmNcommandWindowLocation, XmCOMMAND_BELOW_WORKSPACE,
		NULL);                        /* terminate varargs list */

	frame = XtVaCreateManagedWidget(
		"frame",                 /* widget name */
		xmFrameWidgetClass,      /* widget class */
		mainWindow,              /* parent widget */
		NULL);                   /* terminate varargs list */

	drawArea = XtVaCreateManagedWidget(
		"drawingArea",              /* widget name */
		xmDrawingAreaWidgetClass,   /* widget class */
		frame,                      /* parent widget*/
		XmNwidth, 200,              /* set startup width */
		XmNheight, 300,             /* set startup height */
		XmNbackground, DRAWING_AREA_BACKGROUND,
		NULL);                      /* terminate varargs list */

		
	rowColumn = XtVaCreateManagedWidget(
		"rowColumn",                             /* widget name */
		xmRowColumnWidgetClass,                  /* widget class */
		mainWindow,                              /* parent widget */
		XmNentryAlignment, XmALIGNMENT_CENTER,   /* alignment */
		XmNorientation, XmHORIZONTAL,            /* orientation */
		XmNpacking, XmPACK_COLUMN,               /* packing mode */
		NULL);                                   /* terminate varargs list */
	

	clearButton = XtVaCreateManagedWidget(
		"Clear",                   /* widget name */
		xmPushButtonWidgetClass,   /* widget class */
		rowColumn,                 /* parent widget*/
		NULL);                     /* terminate varargs list */

	quitButton = XtVaCreateManagedWidget(
		"Quit",                    /* widget name */
		xmPushButtonWidgetClass,   /* widget class */
		rowColumn,                 /* parent widget*/
		NULL);                     /* terminate varargs list */





	XmMainWindowSetAreas(mainWindow, initToolbar(&mainWindow), rowColumn, NULL, NULL, frame);

	XtAddCallback(drawArea, XmNinputCallback, drawObjectCallback, drawArea);
	XtAddEventHandler(drawArea, ButtonMotionMask, False, ChangeMousePositionEventHandler, NULL);


	XtAddCallback(clearButton, XmNactivateCallback, clearCallback, drawArea);
	XtAddCallback(quitButton, XmNactivateCallback, quitCallback, 0);

	XtRealizeWidget(topLevel);

	XtAppMainLoop(app_context);

	return 0;
}

