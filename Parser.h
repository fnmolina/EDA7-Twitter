#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <exception>
#include <list>
#include <typeinfo>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

#include "json.hpp"


using json = nlohmann::json;


class parsed_info {

public:

	void parse();

	std::list<std::string> names;	
	std::list<std::string> dates;

	json tweets;

};

#endif