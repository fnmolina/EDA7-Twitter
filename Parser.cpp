#include "Parser.h"


using json = nlohmann::json;


void parsed_info::parse(){

	for (auto element : tweets)
		names.push_back(element["text"]);
	for (auto element : tweets)
		dates.push_back(element["created_at"]);
	for (std::list<std::string>::iterator it = names.begin(); it != names.end(); it++)
	{
		//Eliminamos el URL al final para mostrar
		int extended = (*it).find("https");
		*it = (*it).substr(0, extended);
		(*it).append(" -");
		(*it).insert(0,"- ");
		std::cout << "Tweet:" << *it << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
	}
	for (std::list<std::string>::iterator it = dates.begin(); it != dates.end(); it++)
	{
		struct tm tm;
		std::istringstream iss(*it);
		iss >> std::get_time(&tm, "%a %b %d %H:%M:%S +0000 %Y");
		time_t time = mktime(&tm);
		std::string stringDate = "";
		if (tm.tm_mday < 10)
		{
			stringDate += "0";
		}
		stringDate += std::to_string(tm.tm_mday) + "/";
		if (tm.tm_mon < 10)
		{
			stringDate += "0";
		}
		stringDate += std::to_string(tm.tm_mon + 1) + "/";
		if (tm.tm_year < 110 && tm.tm_year >= 100)
		{
			stringDate += "0";
		}
		stringDate += std::to_string(tm.tm_year - 100);
		stringDate += " - ";
		if (tm.tm_hour < 10)
		{
			stringDate += "0";
		}
		stringDate += std::to_string(tm.tm_hour - 3) + ":";
		if (tm.tm_min < 10)
		{
			stringDate += "0";
		}
		stringDate += std::to_string(tm.tm_min);
		*it = stringDate;
		std::cout << "Date:" << *it << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
	}
}
