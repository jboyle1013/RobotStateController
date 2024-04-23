// RobotStateMachine.h

#ifndef ROBOT_STATE_MACHINE_H
#define ROBOT_STATE_MACHINE_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <event_groups.h>

#define BIT_NEW_DATA_AVAILABLE (1 << 0)
#define BIT_READ_DETECTIONS    (1 << 1)
// Include other necessary headers...

// Enum for different robot states
enum RobotState {
  WAIT_FOR_START,
  GET_BIG_BOXES,
  GET_SMALL_BOXES,
  DEPOSIT_BIG_BOXES,
  DEPOSIT_SMALL_BOXES,
  FOLLOW_LINE, // Has Counter
  GO_TO_RED_ZONE,
  GO_TO_BLUE_ZONE,
  GO_TO_GREEN_ZONE,
  GET_ROCKETS,
  CROSS_GAP,
  DEPOSIT_ROCKETS,
  PUSH_BUTTON,
  DISPLAY_LOGO,
  DONE,
  EMERGENCY_STOP
};

class RobotStateMachine {
private:
    RobotState currentState;
	RobotState prevState;
	RobotState nextState;
	
	int Follow_Line_Counter = 0;


    // State handling methods
    void wait_for_start();
    void get_big_boxes();
	void get_small_boxes();
	void follow_line();
	void deposit_big_boxes();
	void deposit_small_boxes();
	void go_to_red_zone();
	void go_to_blue_zone();
	void go_to_green_zone();
	void get_rockets();
	void deposit_rockets();
	void cross_gap();
	void display_logo();
	void push_button();
	void done();
	void emergency_stop();

public:
    RobotStateMachine() : currentState(WAIT_FOR_START), prevState(WAIT_FOR_START) {}
    void update();
    void setState(RobotState newState);
	RobotState getPrevState();
	RobotState getCurrentState();
	void setEmergencyState();
	int getLineFollowCounter();
	TaskHandle_t readDetTaskHandle;
    TaskHandle_t processDetTaskHandle;
	EventGroupHandle_t xDetectionsEventGroup;
};

#endif // ROBOT_STATE_MACHINE_H
