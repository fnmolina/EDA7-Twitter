#ifndef GUI_H
#define GUI_H

#include <stdio.h>
#include <string>

#include "./lib/imgui.h"
#include "./lib/imgui_impl_allegro5.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define LONG_USUARIO 30
#define LONG_CANTTWEETS   3

class Gui {
public:
    Gui();
    int mainWindow(void);

    int showMainWindow(void);

    int configureEvents(void);
    int configureImGui(void);

    std::string cant_tweets;
    std::string usuario;
    bool cancelRequest;
private:
    int selectedDisplay;

    char buf_usuario[LONG_USUARIO + 1] = { 0 };
    char buf_canttweets[LONG_CANTTWEETS + 1] = { 0 };

    int guiWindowSizeX;

    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_EVENT ev;
    ALLEGRO_TIMER* flipTimer;

    double displaySizeX;
    double displaySizeY;

    bool runningMain;
    bool closeWindow;

    double fps;
};



#endif