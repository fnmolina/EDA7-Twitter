#include "Config.h"

bool initAllegro(void)
{
    if (!al_init()) {
        fprintf(stderr, " failed to initialize allegro !\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Failed to initialize image addon");
        return false;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives addon!\n");
        return -1;
    }

    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize keyboard!\n");
        return false;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "failed to initialize mouse!\n");
        return false;
    }

    if (!al_init_font_addon())
    {
        fprintf(stderr, "Failed to initialize font addon");
        return false;
    }

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Faield to initialize ttf addon");
        return false;
    }



    return true;
}