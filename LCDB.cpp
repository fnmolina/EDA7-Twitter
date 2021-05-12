#include "LCDB.h"


#define LCDB_PNG		"./Images/LCDB_PNG.png"
#define PATH1           "./Fonts/Minecraft.ttf"

LCDB::LCDB()
{
	this->errorCode = NOT_ERROR;
	this->error = lcdError(errorCode);
	cp.row = 0;
	cp.column = 0;
	font = NULL;
	screen = NULL;
	memset(display, 0, sizeof(display));
	initAllegro();
}

LCDB::~LCDB()
{
	this->deinitAllegro();
}

//Devuelve en su nombre "true" si el display se inicializó correctamente (el constructor no tuvo errores) o "false" en caso contrario.
bool LCDB::lcdInitOk()
{
	if (this->error.getErrorCode() == NOT_ERROR)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Devuelve en su nombre un lcdError&
lcdError& LCDB::lcdGetError()
{
	return this->error;
}

//Borra el display y el cursor va a HOME.
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCDB::lcdClear()
{
	//Limpia el display.
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLUMNS; ++j)
		{
			display[i][j] = 0;
		}
	}
	//Resetea al cursor.
	cp.row = 0;
	cp.column = 0;

	writeLCD();
	drawCursorLCD();

	return true;
}

//Borra el display desde la posición actual del cursor hasta el final de la línea.
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCDB::lcdClearToEOL()
{
	for (int j = cp.column; j < COLUMNS; ++j)
	{
		display[cp.row][j] = 0;
	}
	writeLCD();

	return true;
}

//Devuelve en su nombre una referencia a un basicLCD que permite encascar la función.
basicLCD& LCDB::operator<<(const char c)
{
	//Verifica que sea un caracter valido.
	if (c > CHAR_LIMIT)
	{
		//Inserta caracter en la posicion del cursor.
		display[cp.row][cp.column] = c;
		//Mueve el cursor en una posicion a la derecha.
		bool movement = lcdMoveCursorRight();
	}

	//writeLCD();
	//drawCursorLCD();

	return *this;
}

//Devuelve en su nombre una referencia a un basicLCD que permite encascar la función
basicLCD& LCDB::operator<<(const char* c)
{
	//eraseCursorLCD();
	int lastChar = 0;
	int pos = 0;
	//Calcula cantidad de caracteres de la cadena.
	while (c[lastChar] != NULL)
	{
		lastChar++;
	}
	//En caso de superar el tamaño del display, escribe los ultimos 32.
	if (lastChar >= ROWS * COLUMNS)
	{
		pos = lastChar - 32;
	}
	//Va cargando los caracteres desde la posicion desde la que esta el cursor.
	do
	{
		//Verifica que sea un caracter valido.
		if (c[pos] > CHAR_LIMIT)
		{
			display[cp.row][cp.column] = c[pos];
		}
		pos++;
	} while (lcdMoveCursorRight() && pos < lastChar);
	writeLCD();
	//drawCursorLCD();

	return *this;
}

//Pasa el cursor a la primera línea del display sin alterar la columna en la que estaba.
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCDB::lcdMoveCursorUp()
{
	bool movement = false;
	//Si estaba en la segunda fila, pasa a la primera.
	if (cp.row == 1)
	{
		//eraseCursorLCD();
		--cp.row;
		movement = true;
		//drawCursorLCD();
	}
	if (!movement)
	{
		this->errorCode = ERROR_MOVEMENT;
	}
	//En cualquier otro caso, devuelve error.
	return movement;
}

//Pasa el cursor a la segunda línea del display sin alterar la columna en la que estaba.
//Devuelve en su nombre “true” si fue satisfactoria “false” en caso contrario.
bool LCDB::lcdMoveCursorDown()
{
	bool movement = false;
	//Si estaba en la primera fila, pasa a abajo.
	if (cp.row == 0)
	{
		//eraseCursorLCD();
		++cp.row;
		movement = true;
		//drawCursorLCD();
	}
	if (!movement)
	{
		this->errorCode = ERROR_MOVEMENT;
	}
	//En cualquier otro caso, devuelve error.
	return movement;
}

//Avanza el cursor una posición hacia la derecha. 
///Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCDB::lcdMoveCursorRight()
{
	bool movement = false;
	//Si hay una columna libre a la derecha, se mueve una posicion.
	if ((cp.column >= 0) && (cp.column <= COLUMNS - 1))
	{
		//eraseCursorLCD();
		//Si esta en la ultima columna de la primera fila, lo regresa al principio.
		if (cp.column == COLUMNS - 1)
		{
			if (cp.row == 0)
			{
				cp.column = 0;
				++cp.row;
				movement = true;
			}
			else if (cp.row == ROWS - 1)
			{
				movement = false;
			}
		}
		else {
			++cp.column;	//Avanza una posicion el cursor. 
			movement = true;
		}
		//drawCursorLCD();
	}
	if (!movement)
	{
		this->errorCode = ERROR_MOVEMENT;
	}
	//En cualquier otro caso, devuelve error.
	return movement;
}

//Retrocede el cursor una posición.
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCDB::lcdMoveCursorLeft()
{
	bool movement = false;
	//Si hay una columna libre a la izquierda, se mueve una posicion.
	if ((cp.column >= 0) && (cp.column < COLUMNS))
	{
		//eraseCursorLCD();
		//Si esta en la ultima fila primera columna, pasa al renglon anterior.
		if (cp.column == 0)
		{
			if (cp.row == 0)
			{
				movement = false;
			}
			else if (cp.row == ROWS - 1)
			{
				cp.column = COLUMNS - 1;
				--cp.row;
				movement = true;
			}
		}
		else
		{
			--cp.column;	//Avanza una posicion el cursor. 
			movement = true;
		}
		//drawCursorLCD();

	}
	//En cualquier otro caso, devuelve error.
	if (!movement)
	{
		this->errorCode = ERROR_MOVEMENT;
	}
	return movement;
}

//Recibe una estructura tipo cursorPosition.
//Posiciona el cursor en la posición dada por row y column. row[0-1] col[0-15]. Ante un valor inválido de row y/o column ignora la instrucción (no hace nada).
//Devuelve en su nombre "true" si fue satisfactoria "false" en caso contrario.
bool LCDB::lcdSetCursorPosition(const cursorPosition pos)
{
	bool check = false;
	//De ser valida la expresion, posiciona el cursor.
	if (pos.row >= 0 && pos.row < ROWS && pos.column >= 0 && pos.column < COLUMNS) {
		eraseCursorLCD();
		cp.row = pos.row;
		cp.column = pos.column;
		check = true;
		drawCursorLCD();
	}
	return check;
}

//Devuelve la posición actual del cursor.
//Devuelve una estructura tipo cursorPosition.
cursorPosition LCDB::lcdGetCursorPosition()
{
	//struct cursorPosition cursor = {cp.row, cp.column};
	return cp;
}


void LCDB::initAllegro()
{
	//Init display
	screen = al_create_display(SCREEN_W_LCD2, SCREEN_H_LCD2);
	if (screen == NULL) {
		fprintf(stderr, "failed to create display!\n");
		this->errorCode = AL_CREATE_DISPLAY_ERROR;
		return;
	}

	//Tipo y tamano de font
	font = al_load_ttf_font(PATH1, 40, 0);
	if (font == NULL) {
		fprintf(stderr, "failed to create font!\n");
		this->errorCode = AL_CREATE_FONT_ERROR;
		return;
	}
	bitmap = al_load_bitmap(LCDB_PNG);(PATH1, 40, 0);
	if (bitmap == NULL) {
		fprintf(stderr, "failed to create bitmap!\n");
		this->errorCode = AL_CREATE_BITMAP_ERROR;
		return;
	}
	al_draw_bitmap(bitmap, 0, 0, 0);
	al_flip_display();
}

//Destruye buffer, screen y font.
void LCDB::deinitAllegro()
{
	al_destroy_font(font);
	al_destroy_bitmap(bitmap);
	al_destroy_display(screen);
}

void LCDB::writeLCD()
{
	al_set_target_backbuffer(screen);
	al_draw_bitmap(bitmap, 0, 0, 0);
	string str = "";
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLUMNS; ++j)
		{
			if (display[i][j] != 0)
			{
				str.push_back(display[i][j]);
				al_draw_text(font, WHITE, j * CHAR_SIZE_X_LCD2 + BORDER_X_LCD2, i * LINE_HEIGHT_LCD2 + BORDER_Y_LCD2, ALLEGRO_ALIGN_LEFT, str.c_str());
				str.erase();
			}
		}
	}
	//al_draw_line(cp.column * CHAR_SIZE_X_LCD2 + BORDER_X_LCD2, CURSOR_HEIGHT_LCD2 + BORDER_Y_LCD2 + cp.row * LINE_HEIGHT_LCD2, (cp.column + 1) * CHAR_SIZE_X_LCD2 + BORDER_X_LCD2, CURSOR_HEIGHT_LCD2 + BORDER_Y_LCD2 + cp.row * LINE_HEIGHT_LCD2, WHITE, 2);
	//al_flip_display();
}

void LCDB::eraseCursorLCD()
{
	al_draw_line(cp.column * CHAR_SIZE_X_LCD2 + BORDER_X_LCD2, BORDER_Y_LCD2 + CURSOR_HEIGHT_LCD2 + cp.row * LINE_HEIGHT_LCD2, (cp.column + 1) * CHAR_SIZE_X_LCD2 + BORDER_X_LCD2, CURSOR_HEIGHT_LCD2 + BORDER_Y_LCD2 + cp.row * LINE_HEIGHT_LCD2, BACK, 2);
}

void LCDB::drawCursorLCD()
{
	//Dibuja cursor
	al_draw_line(cp.column * CHAR_SIZE_X_LCD2 + BORDER_X_LCD2, CURSOR_HEIGHT_LCD2 + BORDER_Y_LCD2 + cp.row * LINE_HEIGHT_LCD2, (cp.column + 1) * CHAR_SIZE_X_LCD2 + BORDER_X_LCD2, CURSOR_HEIGHT_LCD2 + BORDER_Y_LCD2 + cp.row * LINE_HEIGHT_LCD2, WHITE, 2);
	//al_flip_display();
}



