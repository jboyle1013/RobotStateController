//
// Created by jorda on 03/15/2024.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include "RobotStateController.h"

TEST_CASE("RobotStateController Initialization", "[init]") {
    RobotStateController controller;
    REQUIRE(controller.getCurrentRobotState() == WAIT_FOR_START);
}

TEST_CASE("RobotStateController State Transitions", "[transition]") {
    RobotStateController controller;

    SECTION("Proceed to next state") {
        controller.proceed();
        REQUIRE(controller.getCurrentRobotState() == GET_BIG_BOXES);
    }

    SECTION("Go back to previous state") {
        controller.proceed(); // Move to GET_BIG_BOXES
        controller.goBack();  // Move back to WAIT_FOR_START
        REQUIRE(controller.getCurrentRobotState() == WAIT_FOR_START);
    }
    // Add more test sections for different state transitions
}

// Add more TEST_CASE blocks for other functionalities

TEST_CASE("Loop Through All States", "[loop]") {
    RobotStateController controller;
    controller.update();
    for (int i = 0; i < 17; ++i) {
        controller.proceed();
    }
    controller.update();
    for (int j = 0; j < 17; ++j) {
        controller.goBack();
    }
}


TEST_CASE("Simulate Specific Sequence", "[sequence]") {
    RobotStateController controller;

    controller.proceed(); // Proceed to GET_BIG_BOXES
    REQUIRE(controller.getCurrentRobotState() == GET_BIG_BOXES);

    controller.proceed(); // Proceed to GET_SMALL_BOXES
    REQUIRE(controller.getCurrentRobotState() == GET_SMALL_BOXES);
    // Continue with more steps in the sequence
}

TEST_CASE("Handle Emergency State", "[emergency]") {
    RobotStateController controller;

    controller.setEmergencyState(); // Transition to EMERGENCY_STOP
    REQUIRE(controller.getCurrentRobotState() == EMERGENCY_STOP);
}
