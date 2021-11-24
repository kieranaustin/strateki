//
// Created by kieran on 21.11.21.
//

#ifndef STRATEKI_GAMESCREEN_H
#define STRATEKI_GAMESCREEN_H

#include "Game.h"
#include "Screen.h"
#include <QMainWindow>

class GameScreen : public Screen
{
    Q_OBJECT

public:
    GameScreen() : Screen() {};
    GameScreen(QWidget * parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~GameScreen() {}

    void enter(QWidget * parent) override;
    void update() override;
    void leave() override;

signals:
    void endGame();

private:
    Game * m_game;
    QMainWindow * m_gameMainWindow;
    QLayout * m_layout;
    QWidget * m_gameContainer;
};


#endif //STRATEKI_GAMESCREEN_H
