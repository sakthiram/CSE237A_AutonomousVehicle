#ifndef _ASSIGNMENT_BODY_
#define _ASSIGNMENT_BODY_

#define LEFT_LED 27
#define RIGHT_LED 29
#define FRONT_LED 28

#define US_1_ECHO 7
#define US_1_TRIG 21
#define US_2_ECHO 1
#define US_2_TRIG 22
#define US_3_ECHO 0
#define US_3_TRIG 23
#define US_4_ECHO 2
#define US_4_TRIG 24
#define US_5_ECHO 3
#define US_5_TRIG 25

#define DRIVE_FORWARD 1
#define TURN_RIGHT 2
#define TURN_LEFT 3
#define OBSTACLE_DETECTED 4
#define STOP_WAIT 5
 
#define STOP 0
#define FORWARD 1
#define REVERSE 2
#define LEFT 3
#define RIGHT 4

typedef struct shared_variable {
	int bProgramExit; // Once it is set to 1, the program will be terminated.
	int right_led,left_led,front_led;
	//int us1,us2,us3,us4,us5;
	int drive_state, obstacle_detected,next_lane_obstacle_detected, current_lane;
	int init_start,manual_stop;
	int current_direction;
} SharedVariable;
static int turn;
void init_shared_variable(SharedVariable* sv);
void init_sensors(SharedVariable* sv);

void body_ultrasound(SharedVariable* sv);
void body_irled(SharedVariable* sv);
void body_linefollow(SharedVariable* sv);
void body_keypress(SharedVariable* sv);


#endif
