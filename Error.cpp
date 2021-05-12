#include "Error.h"

lcdError::lcdError(unsigned long code)
{
	this->code = code;

	switch (code)
	{
	case NOT_ERROR:
		this->name = "No Error";
		this->description = "Sin Errores";
		break;
	case AL_CREATE_FONT_ERROR:
		this->name = "Error en carga de fuentes";
		this->description = "Las fuentes no se han cargado correctamente";
		break;
	case AL_CREATE_DISPLAY_ERROR:
		this->name = "Error en la creación del display";
		this->description = "El display no fue creado correctamente";
		break;
	case AL_CREATE_BITMAP_ERROR:
		this->name = "Error en carga de bitmaps";
		this->description = "Las imágenes no fueron cargadas correctamente";
		break;

	}
}

unsigned long lcdError::getErrorCode(void)
{
	return this->code;
}

std::string lcdError::getErrorDescription(void)
{
	return this->description;
}

std::string lcdError::getErrorName(void)
{
	return this->name;
}