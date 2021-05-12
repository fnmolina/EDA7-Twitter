#ifndef LCDC_H
#define LCDC_H

#include "BasicLCD.h"
#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Error.h"

#define SCREEN_WIDTH 642
#define SCREEN_HEIGHT 289

struct allegroPointers
{
	ALLEGRO_FONT* font;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* lcd;
	ALLEGRO_BITMAP* buffer;
};

class LcdC : public basicLCD
{
public:
	LcdC();
	virtual ~LcdC();


	virtual bool lcdInitOk();
	virtual lcdError& lcdGetError();

	virtual bool lcdClear();
	virtual bool lcdClearToEOL();

	virtual basicLCD& operator<<(const char c);
	virtual basicLCD& operator<<(const char* c);

	virtual bool lcdMoveCursorUp();
	virtual bool lcdMoveCursorDown();
	virtual bool lcdMoveCursorRight();
	virtual bool lcdMoveCursorLeft();

	virtual bool lcdSetCursorPosition(const cursorPosition pos);
	virtual cursorPosition lcdGetCursorPosition();

private:

	cursorPosition cursor;
	allegroPointers punteros;
	lcdError error;
	std::string text[2];
	ALLEGRO_COLOR green = al_map_rgb(0, 255, 0);

	int initAllegro();
	void destroyAllegro();
	void redraw();
};

#endif