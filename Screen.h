//
// Created by kieran on 21.11.21.
//

#ifndef STRATEKI_SCREEN_H
#define STRATEKI_SCREEN_H

#include <QWidget>
#include <QHBoxLayout>
#include <iostream>

class ScreenHandler;

class Screen : public QWidget
{
public:
    Screen(QWidget * parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags()) : QWidget(parent, flags) {};
private:
   friend class ScreenHandler;

   virtual void enter(QWidget * parent) = 0;
   virtual void update() = 0;
   virtual void leave() = 0;
};

class ScreenHandler : public QWidget
{
    Q_OBJECT
public:
    ScreenHandler() : QWidget()
    {
        m_layout = new QHBoxLayout(this);
        m_layout->setMargin(0);
        setLayout(m_layout);
    }

    void update()
    {
        if(isChanging) return;
        m_activeScreen->update();
    }

public slots:
    void changeScreen(Screen * screen)
    {
        if(isChanging) return;
        if (not screen) return;

        isChanging = true;

        if (m_activeScreen)
        {
            m_activeScreen->hide();
            m_layout->removeWidget(m_activeScreen);
            m_activeScreen->leave();
        }

        m_activeScreen = screen;
        m_activeScreen->enter(this);
        m_activeScreen->setParent(this, Qt::Widget);
        m_layout->addWidget(m_activeScreen);
        setLayout(m_layout);
        m_activeScreen->show();

        isChanging = false;
    }

private:
    Screen * m_activeScreen{nullptr};
    QHBoxLayout * m_layout;
    bool isChanging{false};
};
#endif //STRATEKI_SCREEN_H
