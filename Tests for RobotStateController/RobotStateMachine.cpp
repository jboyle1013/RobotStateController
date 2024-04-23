#include "RobotStateMachine.h"


RobotState currentState;



RobotState RobotStateMachine::getCurrentState() {
	return currentState;
}

void RobotStateMachine::setEmergencyState() {
	currentState = EMERGENCY_STOP;
}

RobotState RobotStateMachine::getPrevState() {
	return prevState;
}


// wait_for_start - because patience is a virtue, or so I'm told
// State Number 0
// Current Max Time ) seconds (Doesn't have time limit)
void RobotStateMachine::wait_for_start() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskSuspend( readDetTaskHandle );
  vTaskSuspend( processDetTaskHandle );
  Serial2.println("WAIT_FOR_START");
  Serial.println("Waiting to Start");
  currentState = GET_BIG_BOXES;
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  
}

// get_big_boxes - It's like shopping, but for robots
// State Number 1
// Current Max Time 10 seconds
void RobotStateMachine::get_big_boxes() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskResume( readDetTaskHandle );
  vTaskResume( processDetTaskHandle );
  Serial2.println("GET_BIG_BOXES");
  Serial.println("Getting Big Boxes");
  currentState = GET_SMALL_BOXES;
  xEventGroupSetBits(xDetectionsEventGroup, BIT_READ_DETECTIONS);
  vTaskDelay(5000 / portTICK_PERIOD_MS);

}


// get_small_boxes - Because size isn't everything
// State Number 2
// Current Max Time 10 seconds
void RobotStateMachine::get_small_boxes() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskResume( readDetTaskHandle );
  vTaskResume( processDetTaskHandle );
  Serial2.println("GET_SMALL_BOXES");
  Serial.println("Getting Small Boxes");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState = FOLLOW_LINE;
}


// follow_line - Staying inside the lines in sometimes necessary. 
// Don't tell my kindergarten teacher I said that.
// State Numbers 3, 8, 11, 13
// Current Max Times 7, 5, 5, and 5 seconds respectively
void RobotStateMachine::follow_line() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskSuspend( readDetTaskHandle );
  vTaskSuspend( processDetTaskHandle );
  Serial2.print("FOLLOW_LINE.");
  Serial2.println(Follow_Line_Counter);
  Serial.print("Following Line.");
  Serial.println(Follow_Line_Counter);
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  switch (Follow_Line_Counter) {
	case 0:
	  currentState = GO_TO_RED_ZONE;
	  break;
	case 1:
	  currentState = GO_TO_GREEN_ZONE;
	  break;
	case 2:
	  currentState = CROSS_GAP;
	  break;
	case 3:
	  currentState = DEPOSIT_ROCKETS;
	  break;
	default:
		break;
  }
  Follow_Line_Counter++;
}


// deposit_big_boxes - Making deposits, but sadly not in your bank account
// State Number 7
// Current Max Time 3 seconds
void RobotStateMachine::deposit_big_boxes() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskSuspend( readDetTaskHandle );
  vTaskSuspend( processDetTaskHandle );
  Serial2.println("DEPOSIT_BIG_BOXES");
  Serial.println("Depositing Big Boxes");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState = FOLLOW_LINE;
}


// deposit_small_boxes - Because every box deserves a home
// State Number 5
// Current Max Time 3 seconds
void RobotStateMachine::deposit_small_boxes() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskSuspend( readDetTaskHandle );
  vTaskSuspend( processDetTaskHandle );
  Serial2.println("DEPOSIT_SMALL_BOXES");
  Serial.println("Depositing Small Boxes");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState = GO_TO_BLUE_ZONE;
}

// go_to_red_zone - Red: The color of urgency (or tomatoes)
// State Number 4
// Current Max Time 4 seconds
void RobotStateMachine::go_to_red_zone() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskResume( readDetTaskHandle );
  vTaskResume( processDetTaskHandle );
  xEventGroupSetBits(xDetectionsEventGroup, BIT_READ_DETECTIONS);
  Serial2.println("GO_TO_RED_ZONE");
  Serial.println("Going to Red Zone");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState = DEPOSIT_SMALL_BOXES;
}


// go_to_blue_zone - Feeling blue? Head here
// State Number 6
// Current Max Time 2 seconds
void RobotStateMachine::go_to_blue_zone() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskResume( readDetTaskHandle );
  vTaskResume( processDetTaskHandle );
  Serial2.println("GO_TO_BLUE_ZONE");
  Serial.println("Going to Blue Zone");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState = DEPOSIT_BIG_BOXES;
}


// go_to_green_zone - The eco-friendly zone
// State Number 9
// Current Max Time 3 seconds
void RobotStateMachine::go_to_green_zone() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  vTaskResume( readDetTaskHandle );
  vTaskResume( processDetTaskHandle );
  xEventGroupSetBits(xDetectionsEventGroup, BIT_READ_DETECTIONS);
  Serial2.println("GO_TO_GREEN_ZONE");
  Serial.println("Going to Green Zone");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState = GET_ROCKETS;
}

// get_rockets - It's not rocket science. Oh wait, yes it is!
// Credit for this one goes to one of my T-shirts.
// State Number 10
// Current Max Time 10 seconds
void RobotStateMachine::get_rockets() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Serial2.println("GET_ROCKETS");
  Serial.println("Getting Rockets");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState = FOLLOW_LINE;
}

// deposit_rockets - One small step for gravity. One big leap for out robot.
// State Number 14
// Current Max Time 16 seconds
void RobotStateMachine::deposit_rockets() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  xEventGroupSetBits(xDetectionsEventGroup, BIT_READ_DETECTIONS);
  Serial2.println("DEPOSIT_ROCKETS");
  Serial.println("Depositing Rockets");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState =  DISPLAY_LOGO;
}

// cross_gap - Mind the gap!
// State Number 12
// Current Max Time 16 seconds
void RobotStateMachine::cross_gap() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Serial2.println("CROSS_GAP");
  Serial.println("Crossing Gap");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState = FOLLOW_LINE;
}

// display_logo - Time for a commercial break
// State Number 5
// Current Max Time 2 seconds
void RobotStateMachine::display_logo() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Serial2.println("DISPLAY_LOGO");
  Serial.println("Displaying Logo");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState =  PUSH_BUTTON;
}

// push_button - The big red button moment we've all been waiting for. 
//No, it won't launch missiles... I think.
// State Number 16
// Current Max Time 3 seconds
void RobotStateMachine::push_button() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Serial2.println("PUSH_BUTTON");
  Serial.println("Pushing Button");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState =  DONE;
}

// done - Congratulations, you've made it to the end!
// State Number 17
// Current Max Time ) seconds (Doesn't have time limit)
void RobotStateMachine::done() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Serial2.println("DONE");
  Serial.println("Done");
  vTaskSuspend( readDetTaskHandle );
  vTaskSuspend( processDetTaskHandle );

  vTaskDelay(10000 / portTICK_PERIOD_MS);
  currentState =  WAIT_FOR_START;
  Follow_Line_Counter = 0;
  vTaskResume( readDetTaskHandle );
  vTaskResume( processDetTaskHandle );


}

// emergency_stop - In case of fire, break glass. Or just call this.
// State Number 18
// Current Max Time ) seconds (Doesn't have time limit)
void RobotStateMachine::emergency_stop() {
  vTaskDelay(100 / portTICK_PERIOD_MS);
  Serial2.println("EMERGENCY_STOP");
  Serial.println("EMERGENCY STOP");
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  currentState =  DONE;
}



void RobotStateMachine::update() {
	switch (currentState) {
	  case WAIT_FOR_START:
		// Code to handle waiting for start
		// Green Light stuff, yada yada
		wait_for_start();
		break;
	  case GET_BIG_BOXES:
		// Code for getting big blocks
		get_big_boxes();
		break;
	  case GET_SMALL_BOXES:
		// Code for getting small blocks
		get_small_boxes();
		break;
	  case DEPOSIT_BIG_BOXES:
		// Code for depositing big blocks
		deposit_big_boxes();
		break;
	  case DEPOSIT_SMALL_BOXES:
		// Code for depositing small blocks
		deposit_small_boxes();
		break;
	  case FOLLOW_LINE:
		// Code to follow the yellow line
		follow_line();
		break;
	  case GO_TO_RED_ZONE:
		// Code to go to the red zone
		go_to_red_zone();
		break;
	  case GO_TO_BLUE_ZONE:
		// Code to go to the blue zone
		go_to_blue_zone();
		break;
	  case GO_TO_GREEN_ZONE:
		// Code to go to the green zone
		go_to_green_zone();
		break;
	  case GET_ROCKETS:
		// Code for getting rockets
		get_rockets();
		break;
	  case DEPOSIT_ROCKETS:
		// Code for depositing rockets
		deposit_rockets();
		break;
	  case CROSS_GAP:
		// Code to cross the gap
		cross_gap();
		break;
	  case PUSH_BUTTON:
		// Code to push stop timer button
		push_button();
		break;
	  case DISPLAY_LOGO:
		// Code to display the logo
		display_logo();
		break;
	  case DONE:
		// Code for stopping when all tasks completed
		done();
		break;
	  case EMERGENCY_STOP:
		// Code for emergency stop
		emergency_stop();
		break;
	  default:
		break;
	}
}

void RobotStateMachine::setState(RobotState newState) {
	currentState = newState;
}

int RobotStateMachine::getLineFollowCounter(){
	return Follow_Line_Counter;
}