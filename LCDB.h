#ifndef __LCDB_H
#define __LCDB_H

#include "basicLCD.h"
#include "Error.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

using namespace std;

// --------------------------     Constantes display backend     -------------------------- //
#define ROWS 2
#define COLUMNS 16
#define CHAR_LIMIT  31

// --------------------------     display allegro      -------------------------- //
#define BUFFER_W_LCD2        640
#define BUFFER_H_LCD2        280
#define SCREEN_SCALE_LCD2    1
#define SCREEN_W_LCD2        (BUFFER_W_LCD2 * SCREEN_SCALE_LCD2)
#define SCREEN_H_LCD2        (BUFFER_H_LCD2 * SCREEN_SCALE_LCD2)
#define BORDER_X_LCD2        63
#define BORDER_Y_LCD2        90
#define CHAR_SIZE_X_LCD2     32
#define CHAR_SIZE_Y_LCD2     32
#define LINE_HEIGHT_LCD2     58
#define CURSOR_HEIGHT_LCD2   36

// --------------------------     colores      -------------------------- //
#define BLACK           al_map_rgb(0,0,0)
#define WHITE           al_map_rgb(255,255,255)
#define RED             al_map_rgb(255,0,0)
#define GREEN           al_map_rgb(0,155,0)
#define GREY            al_map_rgb(128, 128, 128)
#define BACK            al_map_rgb(150, 175, 0)


class LCDB : public basicLCD
{
public:
    //Constructor.
    LCDB();

    //Destructor.
    ~LCDB();

    //Metodos virtuales a implementar de basicLCD.
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
    void initAllegro();
    void deinitAllegro();
    void writeLCD();
    void eraseCursorLCD();
    void drawCursorLCD();

    cursorPosition cp;
    char display[ROWS][COLUMNS];

    lcdError error;
    int errorCode;

    //allegro
    ALLEGRO_FONT* font;
    ALLEGRO_DISPLAY* screen;
    ALLEGRO_BITMAP* bitmap;
};

#endif //__LCDB_H