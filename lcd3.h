#include "basicLCD.h"
#include <iostream>
#include <stdexcept>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>      
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> 

#define SCREEN_H 250
#define SCREEN_W 650
#define CHAR_LINE_SPACING (SCREEN_H/2)
#define CHAR_SPACING 40
#define CHAR_H 45
#define TTFPATH "./Fonts/Minecraft.ttf"
#define NCOLUMNS 16
#define NROWS 2
#define UP_MARGIN 60
#define RIGHT_MARGIN 10

class LCD3 : public basicLCD {

public:

	LCD3();
	virtual ~LCD3();

	/*=====================================================
	* Name: lcdInitOk
	* Entra: -
	* Resulta: No genera ning�n cambio en el display.
	* Devuelve en su nombre �true� si el display se inicializ�
	* correctamente (el constructor no tuvo errores) o �false
	* en caso contrario.
	*=====================================================*/
	 bool lcdInitOk() ;

	/*=====================================================
	* Name: lcdGetError
	* Entra: -
	* Resulta: No genera ning�n cambio en el display.
	* Devuelve en su nombre un lcdError&
	*=====================================================*/
	 lcdError& lcdGetError() ;

	/*=====================================================
	* Name: lcdClear
	* Entra: -
	* Resulta: Borra el display y el cursor va a HOME
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	 bool lcdClear() ;

	/*=====================================================
	* Name: lcdClearToEOL
	* Entra: -
	* Resulta: Borra el display desde la posici�n actual
	* del cursor hasta el final de la l�nea.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	 bool lcdClearToEOL() ;

	/*=====================================================
	* Name: operator<<()
	* Entra: Un car�cter
	* Resulta: Pone el car�cter en la posici�n actual
	* del cursor del display y avanza el cursor a la pr�xima
	* posici�n respetando el gap (si el car�cter no es imprimible
	* lo ignora)
	*
	* Devuelve en su nombre una referencia a un basicLCD que permite
	* encascar la funci�n:
	* basicLCD lcd;
	* lcd << �a� << �b� << �c�;
	*=====================================================*/
	 basicLCD& operator<<(const char c) ;

	/*=====================================================
	* Name: operator<<()
	* Entra: Una cadena de caracteres NULL terminated
	* Resulta: imprime la cadena de caracteres en la posici�n actual
	* del cursor y avanza el cursor al final de la cadena respetando
	* el gap (si alg�n car�cter no es imprimible lo ignora). Si recibe una
	* cadena de m�s de 32 caracteres, muestra los �ltimos 32 en el display.
	*
	* Devuelve en su nombre una referencia a un basicLCD que permite
	* encascar la funci�n:
	* basicLCD lcd;
	* lcd << �Hola� << � � << �Mundo�;
	*=====================================================*/
	 basicLCD& operator<<(const char* c) ;

	/*=====================================================
	* Name: lcdMoveCursorUp
	*
	* Entra: -
	* Resulta: Pasa el cursor a la primera l�nea del display sin
	* alterar la columna en la que estaba.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	 bool lcdMoveCursorUp() ;

	/*=====================================================
	* Name: lcdMoveCursorDown
	*
	* Entra: -
	* Resulta: Pasa el cursor a la segunda l�nea del display sin
	* alterar la columna en la que estaba.
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	 bool lcdMoveCursorDown() ;

	/*=====================================================
	* Name: lcdMoveCursorRight
	*
	* Entra: -
	* Resulta: Avanza el cursor una posici�n
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	 bool lcdMoveCursorRight() ;

	/*=====================================================
	* Name: lcdMoveCursorLeft
	*
	* Entra: -
	* Resulta: Retrocede el cursor una posici�n
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	 bool lcdMoveCursorLeft() ;

	/*=====================================================
	* Name: lcdSetCursorPosition
	* Entra: Recibe una estructura tipo cursorPosition
	* Resulta: Posiciona el cursor en la posici�n dada
	* por row y column. row[0-1] col[0-19]. Ante un valor inv�lido
	* de row y/o column ignora la instrucci�n (no hace nada).
	*
	* Devuelve en su nombre �true� si fue satisfactoria �false�
	* en caso contrario.
	*=====================================================*/
	 bool lcdSetCursorPosition(const cursorPosition pos) ;

	/*=====================================================
	* Name: lcdGetCursorPosition
	* Entra: -
	* Resulta: Devuelve la posici�n actual del cursor.
	*
	*
	* Devuelve una estructura tipo cursorPosition
	*=====================================================*/
	 cursorPosition lcdGetCursorPosition() ;

private:

	//Imprime el display
	void printlcd(void);

	// Punteros de allegro
	ALLEGRO_DISPLAY* display3;
	ALLEGRO_FONT* font3;

	//Posicion del cursor
	cursorPosition pos;

	//Arreglo en el que almacenan los carcteres actuales del display
	char mat[32];

	//flag que indica si ya se termino de actualizar la matriz para poder imprimirla
	bool finished_printing;

	//Errores del lcd
	lcdError errors;

};

