//
// Created by kieran on 14.11.20.
//

#include <gtest/gtest.h>
#include "../ecs/ComponentManager.h"

TEST(ecsTest, ComponentManager)
{
    ecs::ComponentManager manager;

    EXPECT_DEATH({manager.addComponent<int>(0,0);},"not registered");
    EXPECT_DEATH({manager.getComponent<int>(0);},"not");
    EXPECT_DEATH({manager.removeComponent<int>(0);},"not");
    EXPECT_DEATH({manager.getComponentType<int>();},"not");

    manager.registerComponentType<int>();
    manager.registerComponentType<std::string>();

    manager.addComponent<int>(0, 0);
    manager.addComponent<int>(1, 1);
    manager.addComponent<int>(2, 2);
    manager.addComponent<int>(3, 3);

    manager.addComponent<std::string>(0, "A");
    manager.addComponent<std::string>(1, "B");
    manager.addComponent<std::string>(2, "C");
    manager.addComponent<std::string>(3, "D");

    manager.removeComponent<int>(1);
    manager.removeComponent<std::string>(1);

    EXPECT_EQ(manager.getComponent<int>(0),0);
    EXPECT_EQ(manager.getComponent<int>(2),2);
    EXPECT_EQ(manager.getComponent<int>(3),3);

    EXPECT_EQ(manager.getComponent<std::string>(0),"A");
    EXPECT_EQ(manager.getComponent<std::string>(2),"C");
    EXPECT_EQ(manager.getComponent<std::string>(3),"D");
}