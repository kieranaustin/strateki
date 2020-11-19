//
// Created by kieran on 18.11.20.
//
#include "Game.h"

int main()
{
    Game app;

    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();

    return 0;
}