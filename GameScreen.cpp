//
// Created by kieran on 21.11.21.
//

#include "GameScreen.h"

GameScreen::GameScreen(QWidget * parent, Qt::WindowFlags flags) : Screen(parent, flags)
{
    m_gameMainWindow = new QMainWindow(this, Qt::Widget);
    m_layout = new QHBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->addWidget(m_gameMainWindow);
    setLayout(m_layout);
}

void GameScreen::enter(QWidget * parent)
{
    m_game = new Game();
    m_game->initApp();

    m_gameContainer = QWidget::createWindowContainer((QWindow*)m_game->getNativeWindow());
    m_gameMainWindow->setCentralWidget(m_gameContainer);
    m_gameMainWindow->setParent(this, Qt::Widget);

    m_gameContainer->show();
    m_gameMainWindow->show();
}

void GameScreen::update()
{
    if(m_game->getRoot()->endRenderingQueued())
    {
        emit endGame();
        return;
    }
    m_game->getRoot()->renderOneFrame();
}

void GameScreen::leave()
{
    m_gameContainer->hide();
    m_gameMainWindow->hide();

    m_game->closeApp();

    delete m_game;
}
