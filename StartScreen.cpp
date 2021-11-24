//
// Created by kieran on 21.11.21.
//

#include "StartScreen.h"

StartScreen::StartScreen(QWidget * parent, Qt::WindowFlags flags) : Screen(parent, flags)
{
    m_startLayout = new QHBoxLayout(this);
    m_startGameButton = new QPushButton("Start Game", this);
    m_startLayout->addWidget(m_startGameButton);
    setLayout(m_startLayout);
}

StartScreen::~StartScreen()
{
    m_startLayout->removeWidget(m_startGameButton);
    delete m_startGameButton;
    delete m_startLayout;
}

void StartScreen::enter(QWidget * parent)
{
    setLayout(m_startLayout);
    this->show();
}

void StartScreen::leave()
{
    this->hide();
}
