#include "BasicLCD.h"

#include "LCDB.h"
#include "LCDC.h"
#include "lcd3.h"

#include <typeinfo>

#include "Config.h"
#include "TwitterClient.h"
#include "Gui.h"
#include "Parser.h"

/*    /$$$$$$                                                  /$$$$$$
'     /$$__  $$                                               /$$__  $$
'    | $$  \__/  /$$$$$$  /$$   /$$  /$$$$$$   /$$$$$$       |__/  \ $$
'    | $$ /$$$$ /$$__  $$| $$  | $$ /$$__  $$ /$$__  $$         /$$$$$/
'    | $$|_  $$| $$  \__/| $$  | $$| $$  \ $$| $$  \ $$        |___  $$
'    | $$  \ $$| $$      | $$  | $$| $$  | $$| $$  | $$       /$$  \ $$
'    |  $$$$$$/| $$      |  $$$$$$/| $$$$$$$/|  $$$$$$/      |  $$$$$$/
'     \______/ |__/       \______/ | $$____/  \______/        \______/
'                                  | $$
'                                  | $$
'                                  |__/
*/

#define TWEETS_FINISHED 0

int update_tweets(Gui myGui,basicLCD* lcd, parsed_info* all_info, int * indice, float velocidad);
void move_status_bar(Gui myGui,basicLCD* lcd,parsed_info* all_info);
basicLCD* MainWindowSelector(Gui& myGui, TwitterClient* client);

int main(void) {

	bool cancelRequest = false;
	bool downloading = true;
	int indice = 0;
	float velocidad = 1.5; 
	parsed_info all_info;

	bool running = true;
	bool showMainWindow = true;
	bool displaySelector[LCDN] = { true, false, false };

	if (!initAllegro())
	{
		return -1;
	}

	ALLEGRO_EVENT_QUEUE* keyQueue = al_create_event_queue();
	ALLEGRO_TIMER * timer = al_create_timer(0.1);
	al_register_event_source(keyQueue, al_get_keyboard_event_source());
	al_register_event_source(keyQueue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	//Display y muestra de tuits.
	Gui myGui;
	TwitterGui tGui;

	//Se crea una instancia de la clase TwitterClient y se pasa un puntero a cancelRequest.
	int errorCode;
	TwitterClient client(&myGui.cancelRequest, &errorCode);
	//Se intenta obtener el token.
	client.requestBearerToken();

	basicLCD* lcd;
	lcd = MainWindowSelector(myGui,&client);
	tGui.showMainWindow();

	if (lcd == NULL)
	{
		running = false;
		delete lcd;
	}

	//Main loop.
	while (running)
	{
		bool closeDisplay = false;
		while ((lcd->lcdGetError().getErrorCode()) == NOT_ERROR && !closeDisplay)
		{
			al_flip_display();

			//////////////////////////
			if (tGui.loop(&indice,&velocidad) == -1)
				closeDisplay = 1;
			/////////////////////////

			ALLEGRO_EVENT ev;

			if (al_get_next_event(keyQueue, &ev))
			{
				int prueba = ev.keyboard.keycode;
				switch (ev.type)
				{
				case ALLEGRO_EVENT_KEY_CHAR:
					
					if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
					{
						indice++;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
					{
						indice--;
					}
					else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
					{
						indice = 999;
					}
					if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					{
						closeDisplay = true;
						std::cout << "ESCAPE" << std::endl;
						downloading = true;
					}
					break;
				case ALLEGRO_EVENT_TIMER:

					if (client.isReady())
					{
						if (update_tweets(myGui, lcd, &all_info, &indice, velocidad) == TWEETS_FINISHED)
						{
							closeDisplay = true;
							tGui.close();
						}
					}
					else
					{
						//Se muestra barra de descarga
						move_status_bar(myGui, lcd, &all_info);
						//Realiza request de los tweets
						if (!client.requestTweets())
						{
							//En caso de haberse devuelto error, se carga en display
							closeDisplay = true;
							tGui.close();
						}
						if (client.isReady())
						{
							all_info.tweets = client.getTweets();
							all_info.parse(myGui.usuario);
						}
					}	
					break;
				default:
					break;
				}
			}
		}

		if (closeDisplay)
		{
			string errorMessage = client.getErrorMessage();
			if(errorMessage != "200 OK")
			{
				lcd->lcdClear();
				lcd->operator<<(errorMessage.c_str());
				al_flip_display();
				al_rest(3);
			}
			all_info.names.clear();
			all_info.dates.clear();
			delete lcd;

			basicLCD* temp = MainWindowSelector(myGui,&client);
			lcd = temp;
			tGui.showMainWindow();
			if (lcd == NULL)
			{
				return 0;
			}
			closeDisplay = false;
		}

	}


	return 0;
}


basicLCD* MainWindowSelector(Gui& myGui, TwitterClient* client)
{
	int selector = myGui.showMainWindow();

	//Una vez introducidos el usuario y la cantidad de tuits, se lo pasa al client.
	if (selector != -1)
	{
		client->setQuery(myGui.usuario);
		client->configTweetsRequest(myGui.cant_tweets);
	}
	if (selector == 1)
	{
		return new LcdC();
	}
	else if (selector == 2)
	{
		return new LCDB();
	}
	else if (selector == 3)
	{
		return new LCD3();
	}
	else
	{
		return NULL;
	}
	
}

void move_status_bar(Gui myGui, basicLCD* lcd, parsed_info* all_info) {

	static int bar_progress = 0;

	//Se setea segunda linea, en la columna correspondiente y se imprime la barra
	cursorPosition lower_line;
	lower_line.row = 1;
	lower_line.column = bar_progress;
	lcd->lcdSetCursorPosition(lower_line);

	lcd->operator<<("#");

	//Se deja el cursor en un lugar estatico
	lower_line.row = 0;
	lower_line.column = 0;
	lcd->lcdSetCursorPosition(lower_line);

	//Se muestra el nombre del usuario en la linea superior
	lcd->operator<<(myGui.usuario.c_str());

	//Se incrementa el progreso de la barra
	if (bar_progress < 15)
		bar_progress++;
	else
	{
		bar_progress = 0;
		lower_line.row = 1;
		lower_line.column = 0;
		lcd->lcdSetCursorPosition(lower_line);
		lcd->lcdClearToEOL();
	}
}

int update_tweets(Gui myGui, basicLCD* lcd, parsed_info* all_info, int* indice, float velocidad)
{
	static int index = 0;
	static float segundos = 0;
	static int pos = 0;
	segundos += 0.1;

	if (*indice == 999)
	{
		pos = 0;
		segundos = 0;
	}
	else if (index >= 1 && *indice == -1) {
		index--;
		pos = 0;
	}

	else if (index + 1 < atoi(myGui.cant_tweets.c_str()) && *indice == 1)
	{
		pos = 0;
		index++;
	}

	else if (index + 1 == atoi(myGui.cant_tweets.c_str()) && *indice == 1)
	{
		index = 0;
		segundos = 0;
		pos = 0;
		*indice = 0;
		return TWEETS_FINISHED;
	}

	*indice = 0;

	lcd->lcdClear();
	if (!(index +1 == atoi(myGui.cant_tweets.c_str())))
	{
		auto inicio1 = all_info->dates.begin();

		std::advance(inicio1, index);

		lcd->operator<< ((*inicio1).c_str());
	}
	else
	{
		lcd->operator<< ("Ultimo Tweet    ");
	}
	auto inicio2 = all_info->names.begin();


	std::advance(inicio2, index);

	std::string aux = (*inicio2).substr(pos, 16);

	lcd->operator<< (aux.c_str());
	if (segundos >= 2 && pos == 0)
	{
		pos++;
	}
	else if (segundos >= (0.19 / velocidad) && pos != 0)
	{
		segundos = 0;
		if (pos + 16 < strlen((*inicio2).c_str()))
			pos++;
		else
		{
			index++;
			pos = 0;
			if (index == atoi(myGui.cant_tweets.c_str()))
			{
				index = 0;
				segundos = 0;
				pos = 0;
				*indice = 0;
				return TWEETS_FINISHED;
			}

		}

	}

	return -1;
}
