//
// Created by kieran on 14.11.20.
//

#include <gtest/gtest.h>
#include "../ecs/ComponentContainer.h"

TEST(ecsTest, ComponentContainer)
{
    ecs::ComponentContainer<std::string> container;

    EXPECT_DEATH({std::string str0 = container.getComponent(0);}, "Entity does not have Component" );

    container.addComponent(0, "A");
    container.addComponent(1, "B");
    container.addComponent(2, "C");
    container.addComponent(3, "D");

    container.removeComponent(1);
    std::string str0 = container.getComponent(0);
    std::string str2 = container.getComponent(2);
    std::string str3 = container.getComponent(3);

    EXPECT_EQ(str0, "A");
    EXPECT_EQ(str2, "C");
    EXPECT_EQ(str3, "D");

    container.removeComponent(3);
    EXPECT_DEATH({std::string str1 = container.getComponent(3);}, "Entity does not have Component" );

    container.addComponent(11, "eleven");
    std::string str11 = container.getComponent(11);
    EXPECT_EQ(str11, "eleven");

    EXPECT_DEATH({std::string str1 = container.getComponent(1);}, "Entity does not have Component" );
    EXPECT_DEATH({std::string str1 = container.getComponent(3);}, "Entity does not have Component" );
    EXPECT_DEATH({std::string str1 = container.getComponent(9);}, "Entity does not have Component" );
    EXPECT_DEATH({std::string str1 = container.getComponent(ecs::MAX_ENTITIES+1);}, "Entity out of range" );
}
