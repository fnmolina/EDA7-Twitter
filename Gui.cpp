#include "Gui.h"
#define DEFAULT_TWEETS  "50"

using namespace std;

Gui::Gui() {

    ev = { 0 };
    closeWindow = false;

    runningMain = true;

    display = NULL;
    queue = NULL;

    flipTimer = NULL;
    fps = 120;


    displaySizeX = 500;
    displaySizeY = 200;

    guiWindowSizeX = 500;

    selectedDisplay = 0;

    cancelRequest = false;

}

int Gui::showMainWindow()
{
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    this->display = al_create_display(displaySizeX, displaySizeY);
    if (!this->display) {

        fprintf(stderr, "Failed to create display !\n");
        return -1;
    }

    al_set_window_title(this->display, "Display Selector");

    flipTimer = al_create_timer(1 / fps);

    if (configureEvents() == -1) {
        return -1;
    }

    /*IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAllegro5_Init(this->display);
    ImGui::StyleColorsLight();*/

    if (configureImGui() == -1) {
        return -1;
    }

    al_start_timer(flipTimer);
        while (runningMain) {

            while (al_get_next_event(queue, &ev)) {

                ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese

                if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    runningMain = false;
                    return -1;
                }
                if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
                {
                    ImGui_ImplAllegro5_InvalidateDeviceObjects();
                    al_acknowledge_resize(display);
                    ImGui_ImplAllegro5_CreateDeviceObjects();
                }

                if (ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == flipTimer) {

                    ImGui_ImplAllegro5_NewFrame();
                    ImGui::NewFrame();
                    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);


                    if (mainWindow()) {
                        runningMain = false;
                    }

                    ImGui::Render();

                    al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));

                    //Todo lo que dibuje aca va a quedar por detrás de las ventanas de DearImGui

                    ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

                    //Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

                    al_flip_display();

                }
            }
        }
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();

    al_destroy_event_queue(queue);
    al_destroy_display(display);
    runningMain = true;
    return selectedDisplay;
}


int Gui::configureEvents(void)
{
    queue = al_create_event_queue();

    // Controla que la cola de eventos se haya generado
    if (!queue)
    {
        fprintf(stderr, "Failed to create event queue !\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    al_register_event_source(queue, al_get_timer_event_source(flipTimer));

    return 0;
}


int Gui::configureImGui(void)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAllegro5_Init(display);
    ImGui::StyleColorsLight();

    return 0;
}

int Gui::mainWindow()
{
    static bool NoTitlebar = false;
    static bool NoMenu = true;
    static bool NoCollapse = false;
    static bool NoResize = false;
    static bool NoMove = false;
    static bool NoClose = true;
    static bool NoBackground = false;
    static bool NoScrollbar = true;
    static bool no_bring_to_front = false;

    ImGuiWindowFlags window_flags = 0;
    if (NoTitlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (NoScrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!NoMenu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (NoMove)            window_flags |= ImGuiWindowFlags_NoMove;
    if (NoResize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (NoCollapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (NoBackground)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowSize(ImVec2(guiWindowSizeX, 300), ImGuiCond_Always); //Aca pongo tamaño de la pantalla
    ImGui::Begin("Configurar tweets", NULL, window_flags);

    ImGui::RadioButton("Display a", &selectedDisplay, 1); ImGui::SameLine();

    ImGui::RadioButton("Display b", &selectedDisplay, 2); ImGui::SameLine();

    ImGui::RadioButton("Display c", &selectedDisplay, 3);

    ImGui::Text("Ingrese el nombre de usuario de Twitter y la cantidad de tweets");    // Display some text (you can use a format strings too)

    ImGui::InputText("Usuario", buf_usuario, LONG_USUARIO + 1);
    ImGui::InputText("Cantidad de tweets", buf_canttweets, LONG_CANTTWEETS + 1);

    if (ImGui::Button("Show Display"))
    {
        usuario = string(buf_usuario);
        cant_tweets = buf_canttweets;
        if (cant_tweets == "")
            cant_tweets = DEFAULT_TWEETS;
        ImGui::End();
        return selectedDisplay;
    }
    
    ImGui::End();
    return 0;
}


TwitterGui::TwitterGui() {

    ev = { 0 };
    closeWindow = false;

    runningMain = true;

    display = NULL;
    queue = NULL;

    flipTimer = NULL;
    fps = 120;


    displaySizeX = 500;
    displaySizeY = 200;

    guiWindowSizeX = 500;

    selectedDisplay = 0;

    cancelRequest = false;

}

int TwitterGui::showMainWindow()
{
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    this->display = al_create_display(displaySizeX, displaySizeY);
    if (!this->display) {

        fprintf(stderr, "Failed to create display !\n");
        return -1;
    }
    buffer = al_get_backbuffer(this->display);
    al_set_window_title(this->display, "Tweet Selector");

    flipTimer = al_create_timer(1 / fps);

    if (configureEvents() == -1) {
        return -1;
    }

    /*IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAllegro5_Init(this->display);
    ImGui::StyleColorsLight();*/

    if (configureImGui() == -1) {
        return -1;
    }

    al_start_timer(flipTimer);
        //while (runningMain) {

            
        //}
    
}


int TwitterGui::configureEvents(void)
{
    queue = al_create_event_queue();

    // Controla que la cola de eventos se haya generado
    if (!queue)
    {
        fprintf(stderr, "Failed to create event queue !\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    al_register_event_source(queue, al_get_timer_event_source(flipTimer));

    return 0;
}


int TwitterGui::configureImGui(void)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAllegro5_Init(display);
    ImGui::StyleColorsLight();

    return 0;
}

int TwitterGui::mainWindow(int * indice)
{
    al_set_target_bitmap(buffer);
    static bool NoTitlebar = false;
    static bool NoMenu = true;
    static bool NoCollapse = false;
    static bool NoResize = false;
    static bool NoMove = false;
    static bool NoClose = true;
    static bool NoBackground = false;
    static bool NoScrollbar = true;
    static bool no_bring_to_front = false;

    ImGuiWindowFlags window_flags = 0;
    if (NoTitlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (NoScrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!NoMenu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (NoMove)            window_flags |= ImGuiWindowFlags_NoMove;
    if (NoResize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (NoCollapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (NoBackground)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    

    ImGui::SetNextWindowSize(ImVec2(guiWindowSizeX, 300), ImGuiCond_Always); //Aca pongo tamaño de la pantalla
    ImGui::Begin("Configurar tweets", NULL, window_flags);

    //ImGui::SliderFloat("Display a", &selectedDisplay, 1)(const char* label, float* v, float v_min, float v_max, const char* format, float power);

    //ImGui::RadioButton("Display b", &selectedDisplay, 2); ImGui::SameLine();

    //ImGui::RadioButton("Display c", &selectedDisplay, 3);

    //ImGui::Text("Ingrese el nombre de usuario de Twitter y la cantidad de tweets");    // Display some text (you can use a format strings too)

    //ImGui::InputText("Usuario", buf_usuario, LONG_USUARIO + 1);
    //ImGui::InputText("Cantidad de tweets", buf_canttweets, LONG_CANTTWEETS + 1);

    if (ImGui::Button("Next"))
    {
        (*indice)++;
    }
    if (ImGui::Button("Previous"))
    {
        (*indice)--;
    }
    if (ImGui::Button("Read again"))
    {
        (*indice) = 999;
    }
    if (ImGui::Button("Volver al menu principal"))
    {
        ImGui::End();
        return 1;
    }
    
    ImGui::End();
    return 0;
}


int TwitterGui::loop(int * indice) {
    al_set_target_bitmap(buffer);
    while (al_get_next_event(queue, &ev)) {

        ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            runningMain = false;
            close();
            return -1;
        }
        if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            ImGui_ImplAllegro5_InvalidateDeviceObjects();
            al_acknowledge_resize(display);
            ImGui_ImplAllegro5_CreateDeviceObjects();
        }

        if (ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == flipTimer) {

            ImGui_ImplAllegro5_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);


            if (mainWindow(indice)) {
                runningMain = false;
            }

            ImGui::Render();

            al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));

            //Todo lo que dibuje aca va a quedar por detrás de las ventanas de DearImGui

            ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());

            //Todo lo que dibuje aca va a quedar por encima de las ventanas de DearImGui

            al_flip_display();

        }

        if (runningMain == false)
        {
            close();
            return -1;
        }
    }
}


void TwitterGui::close() {
    ImGui_ImplAllegro5_Shutdown();
    ImGui::DestroyContext();

    al_destroy_event_queue(queue);
    al_destroy_display(display);
    runningMain = true;
}
