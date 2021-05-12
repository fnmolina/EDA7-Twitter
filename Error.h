#pragma once
#ifndef ERROR_H
#define ERROR_H

#include <string>

enum errorClass {AL_CREATE_DISPLAY_ERROR, AL_CREATE_FONT_ERROR, AL_CREATE_BITMAP_ERROR, NOT_ERROR, CLOSE_DISPLAY, ERROR_N, ERROR_MOVEMENT};


class lcdError
{
public:
	lcdError(unsigned long code);
	lcdError() = default;

	std::string getErrorName();
	std::string getErrorDescription();
	unsigned long getErrorCode();


private:

	std::string description;
	std::string name;
	unsigned long code;


};


#endif