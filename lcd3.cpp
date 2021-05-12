#include "lcd3.h"

using namespace std;

LCD3::LCD3(): errors(NOT_ERROR)
{
	display3 = al_create_display(SCREEN_W, SCREEN_H);
	if (!display3) {
		errors = lcdError(AL_CREATE_DISPLAY_ERROR);
	}
	font3 = al_load_font(TTFPATH, 50, 0);
	if (!font3) {
		errors = lcdError(AL_CREATE_FONT_ERROR);
	}

	pos.column = 0;
	pos.row = 0;

	finished_printing = true;

	for (int i = 0; i <= 31; i++)
	{
		mat[i]=' ';	
	}

	printlcd();
}


LCD3::~LCD3() {
	al_destroy_font(font3);
	al_destroy_display(display3);
}

bool LCD3::lcdInitOk() {
	if (errors.getErrorCode() == 0ul)
		return true;
	else
		return false;
}

lcdError& LCD3::lcdGetError()
{
	return errors;
}

cursorPosition LCD3::lcdGetCursorPosition() {
	cursorPosition actual;
	actual.column = pos.column;
	actual.row = pos.row;
	return actual;
}

bool LCD3::lcdSetCursorPosition(const cursorPosition pos) {
	if(pos.column<NCOLUMNS && pos.row<NROWS)
		this->pos = pos;
	printlcd();
	return true;
}

bool LCD3::lcdMoveCursorLeft() {
	if (pos.column == 0 && pos.row == 1)
	{
		if (pos.row == 1)
			pos.row--;
		pos.column = NCOLUMNS-1;
	}
	else if (pos.column > 0)
		pos.column--;
	printlcd();
	return true;

}

bool LCD3::lcdMoveCursorRight() {
	
	if (pos.column == (NCOLUMNS-1) && pos.row == 0)
	{
		pos.row++;
		pos.column = 0;
	}
	else if (pos.column< (NCOLUMNS - 1))
		pos.column++;

	if (finished_printing == true)
	printlcd();
	return true;
}

bool LCD3::lcdMoveCursorDown() {
	if(pos.row==0)
		pos.row++;
		printlcd();
	return true;
}

bool LCD3::lcdMoveCursorUp() {
	if (pos.row == 1)
		pos.row--;
		printlcd();
	return true;
}

bool LCD3::lcdClear() {
	
	for (int i = 0; i <= ((NCOLUMNS*NROWS)-1); i++)
	{
		mat[i] = ' ';
	}
	pos.column = 0;
	pos.row = 0;
	printlcd();
	return true;
}

bool LCD3::lcdClearToEOL() {
	cursorPosition now = pos;
	finished_printing = false;
	for (int i = pos.column; i <NCOLUMNS ; i++)
	{
		*this << ' ';
	}
	pos = now;
	finished_printing = true;
	printlcd();
	return true;
}

basicLCD& LCD3::operator<<(const char* c) {

	int places_left = (NCOLUMNS - pos.column) + (NCOLUMNS * !pos.row);
	int size = 0;
	bool flag = false;
	int i = 0;

	while ( c[size] != '\0' )
		size++;
	
	if (size <= places_left )
	{	
		while ((flag == 0) &&(c[i] != 0))
		{
			if ((pos.column == NCOLUMNS - 1) && (pos.row == NROWS - 1))
				flag = true;
			*this << c[i];
			i++;
		}
	}
	else
	{
		for (int i = (size - places_left); size >i ; i++)
			*this << c[i];
	}

	return *this;
	
}

basicLCD& LCD3::operator<<(const char c) {
	mat[(pos.column +(NCOLUMNS* pos.row)) ] = c;
;
	lcdMoveCursorRight();
	if (finished_printing == true)
		printlcd();
	return *this;
}

void LCD3::printlcd() {

	al_clear_to_color(al_map_rgb(0,0,255));


	for (int j = 0; j<= (NROWS - 1); j++)
	{
		for (int i = 0; i<= (NCOLUMNS - 1); i++)
		{
			al_draw_textf(font3, al_map_rgb(255, 255, 255), (i*CHAR_SPACING)+RIGHT_MARGIN, (j * CHAR_LINE_SPACING)+UP_MARGIN,0,"%c", j==0?mat[i]:mat[i+ NCOLUMNS] );
		}
	}

	if (pos.row == 0)
		al_draw_line((pos.column * CHAR_SPACING) + RIGHT_MARGIN,
			CHAR_H + UP_MARGIN,
			(pos.column * CHAR_SPACING) + CHAR_SPACING  + RIGHT_MARGIN-10,
			CHAR_H + UP_MARGIN,
			al_map_rgb(255, 255, 255), 5);

	else if (pos.row == 1)
		al_draw_line((pos.column * CHAR_SPACING) + RIGHT_MARGIN,
			CHAR_LINE_SPACING+ CHAR_H + UP_MARGIN,
			(pos.column * CHAR_SPACING) + CHAR_SPACING  + RIGHT_MARGIN-10,
			CHAR_LINE_SPACING + CHAR_H + UP_MARGIN,
			al_map_rgb(255, 255, 255), 5);

	//al_flip_display();
}