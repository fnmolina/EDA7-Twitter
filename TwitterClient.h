#ifndef __TWITTERCLIENT_H__
#define __TWITTERCLIENT_H__

#include <iostream>
#include <exception>
#include <list>
#include <curl/curl.h>
#include "json.hpp"

enum TwitterError {CONNECTION_OK, CONNECTION_ERROR, NO_USER_ERROR, NO_TWEETS_ERROR};

//Biblioteca NLOHMANN JSON 
using json = nlohmann::json;

class TwitterClient {
public:
	//Constructor 
	TwitterClient(bool* cancelRequest, int* clientError);
	~TwitterClient() {};

	//Posibilidad de cambio de usuario para acceder a los tuits.
	void setUserLoginData(std::string key, std::string SecretKey);

	//Se introduce usuario del que se recuperaran los tuits.
	void setQuery(std::string twitterUser);

	//Configuracion de cURL, autentificacion con Outh2 y obtencion del token de Twitter.
	int requestBearerToken();

	//Solicitud de tuits de un usuario de preferencia.
	json requestTweets(std::string count);

	//Concatena lo recibido en content
	static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		size_t realsize = size * nmemb;
		char* data = (char*)contents;
		//fprintf(stdout, "%s",data);
		std::string* s = (std::string*)userp;
		s->append(data, realsize);
		return realsize;						//recordar siempre devolver realsize
	}

	bool tweetsReady;

private:
	//Configuracion de las opciones de cURL
	bool* cancelRequest;		//Recibe indicacion de cancelacion de descarga
	int* clientError;			//Notifica en caso de error, sea durante la conexion, busqueda de usuario o descarga de tweets. 
	CURL* curl;					//Guarda configuraciones de una transferencia.
	CURLM* multiHandle;			//Administra easy handles.
	CURLcode output;			//Manejo de errores.
	json j;						//json con informacion recibida de Twitter.
	std::string token;			//Guarda token de acceso para recibir tweets.
	std::string query;			// Direccion de Twitter que se va a consultar. 
	std::string API_key;		//Datos de autentificacion de usuario de Twitter Developer.
	std::string API_SecretKey;
};

#endif //__CLIENT_H__
