//
// Created by kieran on 21.11.21.
//

#ifndef STRATEKI_STARTSCREEN_H
#define STRATEKI_STARTSCREEN_H

#include "Screen.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QKeyEvent>

class StartScreen : public Screen
{
    Q_OBJECT

public:
    StartScreen(QWidget * parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~StartScreen();

    void enter(QWidget * parent) override;
    void update() override {};
    void leave() override;

public:
    QHBoxLayout * m_startLayout;
    QPushButton * m_startGameButton;
};


#endif //STRATEKI_STARTSCREEN_H
