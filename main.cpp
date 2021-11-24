//
// Created by kieran on 18.11.20.
//

#include <QtWidgets/QApplication>
#include "Screen.h"
#include "StartScreen.h"
#include "GameScreen.h"
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication qApplication(argc, argv);

    ScreenHandler * handler = new ScreenHandler();
    handler->setWindowTitle("strateki");
    StartScreen * startScreen = new StartScreen(handler, Qt::Widget);
    GameScreen * gameScreen = new GameScreen(handler, Qt::Widget);
    //gameScreen->setAttribute(Qt::WA_DeleteOnClose);

    handler->changeScreen(startScreen);
    handler->resize(360, 180);
    handler->show();

    QObject::connect(startScreen->m_startGameButton, &QPushButton::pressed, handler, [handler, gameScreen](){
        handler->changeScreen(gameScreen);
        handler->resize(1080, 720);
    });
    QObject::connect(gameScreen, &GameScreen::endGame, handler, [handler, startScreen](){
        handler->changeScreen(startScreen);
        handler->resize(360, 180);
    });

    bool quit = false;
    while(not quit)
    {
        qApplication.processEvents();
        handler->update();
        quit = handler->isHidden();
    }
    //qApplication.exec();

    qApplication.quit();

    return 0;
}